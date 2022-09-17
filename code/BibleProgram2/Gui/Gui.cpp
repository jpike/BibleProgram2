#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseRange.h"
#include "Debugging/Timer.h"
#include "Gui/Gui.h"

namespace GUI
{
    /// Attempts to create the GUI to use the specified window.
    /// @param[in]  window - The window into which the GUI should be rendered.
    /// @param[in]  graphics_device - The graphics device the GUI will use for rendering.
    /// @return The GUI, if successfully created; null if not.
    std::optional<Gui> Gui::Create(const WINDOWING::SdlWindow& window, const GRAPHICS::OPEN_GL::OpenGLGraphicsDevice& graphics_device)
    {
        // INITIALIZE IM GUI.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplSDL2_InitForOpenGL(window.UnderlyingWindow, graphics_device.SdlOpenGLContext);
        const char* const GLSL_VERSION = "#version 130";
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);

        Gui gui;
        return gui;
    }

    /// Updates the GUI in response to user input and renders it for the provided window.
    /// @param[in]  window - The window being interacted with and rendered to.
    /// @param[in]  bibles - The Bibles to use for verse data in the GUI.
    /// @param[in,out]  user_settings - User settings for the GUI.
    void Gui::UpdateAndRender(
        const WINDOWING::SdlWindow& window, 
        const BIBLE_DATA::Bibles& bibles,
        UserSettings& user_settings)
    {
        // START RENDERING A NEW FRAME OF THE GUI.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.UnderlyingWindow);
        ImGui::NewFrame();

        // UPDATE AND RENDER THE MAIN MENU.
        bool translation_changed = false;
        if (ImGui::BeginMainMenuBar())
        {
            // RENDER THE DEBUG MENU.
            // No shortcut keys are displayed for menu items since ImGui doesn't currently handle them.
            constexpr char* NO_SHORTCUT_KEYS = nullptr;
            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::MenuItem("Metrics", NO_SHORTCUT_KEYS, &MetricsWindow.Open);
                ImGui::MenuItem("Style Editor", NO_SHORTCUT_KEYS, &StyleEditorWindow.Open);
                ImGui::MenuItem("Demo", NO_SHORTCUT_KEYS, &DemoWindow.Open);
                ImGui::MenuItem("About", NO_SHORTCUT_KEYS, &AboutWindow.Open);

                ImGui::EndMenu();
            }

            // RENDER A MENU FOR SELECTING THE CURRENT TRANSLATION.
            if (ImGui::BeginMenu("Translations"))
            {
                // DISPLAY OPTIONS FOR ALL TRANSLATIONS.
                for (const auto& names_and_translations : bibles.TranslationsByAbbreviatedName)
                {
                    // ALLOW THE USED TO SELECT A TRANSLATION.
                    bool originally_selected = user_settings.BibleTranslationDisplayStatusesByName[names_and_translations.first];
                    bool is_selected = originally_selected;
                    if (ImGui::MenuItem(names_and_translations.first.c_str(), NO_SHORTCUT_KEYS, &is_selected))
                    {
                        translation_changed = (originally_selected != is_selected);

                        // MAKE SURE THE TRANSLATION'S DISPLAY STATUS IS UPDATED.
                        user_settings.BibleTranslationDisplayStatusesByName[names_and_translations.first] = is_selected;
                    }
                }

                ImGui::EndMenu();
            }

            // RENDER A VERSE SEARCH BAR.
            // The longest theoretical verse range would be something like
            // 2 Thessalonians 111:111 - 2 Thessalonians 999:999
            // which is just under 50 characters, so 64 characters should be plenty of room.
            static char verse_range_text[64];
            // The width of the text box has been chosen to be long enough for all expected text.
            ImGui::SetNextItemWidth(512.0f);
            // Search box is configured to only return true when enter is pressed to make it
            // clear when the user has finished entering search text.
            bool verse_search_box_enter_pressed = ImGui::InputTextWithHint(
                "###VerseReferenceSearch", 
                "Enter verse range (ex. Genesis 1:1 - Genesis 2:3)", 
                verse_range_text,
                IM_ARRAYSIZE(verse_range_text),
                ImGuiInputTextFlags_EnterReturnsTrue);
            bool displayed_main_verses_changed = (verse_search_box_enter_pressed || translation_changed);
            if (displayed_main_verses_changed)
            {
                // PARSE THE VERSE RANGE.
                std::optional<BIBLE_DATA::BibleVerseRange> verse_range = BIBLE_DATA::BibleVerseRange::Parse(verse_range_text);
                if (verse_range)
                {
                    std::printf(
                        "Verse range: %u %u:%u - %u %u:%u\n",
                        verse_range->StartingVerse.Book,
                        verse_range->StartingVerse.ChapterNumber,
                        verse_range->StartingVerse.VerseNumber,
                        verse_range->EndingVerse.Book,
                        verse_range->EndingVerse.ChapterNumber,
                        verse_range->EndingVerse.VerseNumber);

                    MainBibleVersesWindow.Open = true;
                    MainBibleVersesWindow.VerseRange = *verse_range;

                    // POPULATE THE VERSE TEXT IN THE MAIN WINDOW.
                    MainBibleVersesWindow.VersesByTranslationName.clear();
                    for (const auto& translation_name_with_display_status : user_settings.BibleTranslationDisplayStatusesByName)
                    {
                        // SKIP OVER ANY TRANSLATION THAT ARE NOT VISIBLE.
                        bool translation_visible = translation_name_with_display_status.second;
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET ALL VERSES IN THE RANGE.
                        const std::string& translation_name = translation_name_with_display_status.first;
                        std::vector<BIBLE_DATA::BibleVerse> verses;
                        {
                            ::DEBUGGING::SystemClockTimer system_clock_timer("Bible Verse Finding System Clock Timer");
                            ::DEBUGGING::HighResolutionTimer high_resolution_timer("Bible Verse Finding High-Resolution Timer");

                            // GET ALL VERSES IN THE RANGE.
                            const auto& name_with_bible_translation = bibles.TranslationsByAbbreviatedName.find(translation_name);
                            const BIBLE_DATA::BibleTranslation& bible_translation = name_with_bible_translation->second;
                            for (const BIBLE_DATA::BibleVerseIdWithCharacterRange& verse_id_with_character_range : bible_translation.VerseCharacterRanges)
                            {
                                // CHECK IF THE CURRENT VERSE IS IN THE RANGE.
                                bool is_matching_verse = (
                                    (verse_range->StartingVerse.Book <= verse_id_with_character_range.Id.Book) && (verse_id_with_character_range.Id.Book <= verse_range->EndingVerse.Book) &&
                                    (verse_range->StartingVerse.ChapterNumber <= verse_id_with_character_range.Id.ChapterNumber) && (verse_id_with_character_range.Id.ChapterNumber <= verse_range->EndingVerse.ChapterNumber) &&
                                    (verse_range->StartingVerse.VerseNumber <= verse_id_with_character_range.Id.VerseNumber) && (verse_id_with_character_range.Id.VerseNumber <= verse_range->EndingVerse.VerseNumber));
                                if (is_matching_verse)
                                {
                                    // STORE THE CURRENT VERSE.
                                    const char* first_character_in_verse = bible_translation.Text + verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText;
                                    std::size_t verse_character_count = (
                                        verse_id_with_character_range.LastCharacterOffsetIntoFullBibleText - 
                                        verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText);
                                    std::string_view current_verse_text = std::string_view(first_character_in_verse, verse_character_count);

                                    BIBLE_DATA::BibleVerse current_verse = 
                                    {
                                        .Id = verse_id_with_character_range.Id,
                                        .Text = current_verse_text
                                    };

                                    verses.emplace_back(current_verse);

                                    // STOP CHECKING FOR MORE VERSES IF THE END OF THE RANGE HAS BEEN REACHED.
                                    bool is_final_verse = (verse_id_with_character_range.Id == verse_range->EndingVerse);
                                    if (is_final_verse)
                                    {
                                        break;
                                    }
                                }
                            }
                        }

                        MainBibleVersesWindow.VersesByTranslationName[translation_name] = verses;
                    }                   
                }
            }

            ImGui::EndMainMenuBar();
        }

        // UPDATE AND RENDER ALL WINDOWS.
        std::string currently_selected_word = BibleVersesWithWordWindow.Word;
        MainBibleVersesWindow.UpdateAndRender(CurrentlyHighlightedWord, currently_selected_word, user_settings);

        // Refreshing of the window for the current word may be needed if the word or translation changed.
        bool new_word_selected = (currently_selected_word != BibleVersesWithWordWindow.Word);
        bool displayed_verses_with_word_outdated = (new_word_selected || translation_changed);
        if (displayed_verses_with_word_outdated)
        {
            BibleVersesWithWordWindow.Open = true;
            BibleVersesWithWordWindow.Word = currently_selected_word;

            // UPDATE THE VERSES DISPLAYED IN THE WINDOW.
            BibleVersesWithWordWindow.VersesByTranslationName.clear();
            for (const auto& translation_name_with_display_status : user_settings.BibleTranslationDisplayStatusesByName)
            {
                // SKIP OVER ANY TRANSLATION THAT ARE NOT VISIBLE.
                bool translation_visible = translation_name_with_display_status.second;
                if (!translation_visible)
                {
                    continue;
                }

                // GET VERSES FOR THE CURRENT WORD IN THE CURRENT TRANSLATION.
                const std::string& translation_name = translation_name_with_display_status.first;
                const auto& name_with_bible_translation = bibles.TranslationsByAbbreviatedName.find(translation_name);
                const BIBLE_DATA::BibleTranslation& bible_translation = name_with_bible_translation->second;
                BibleVersesWithWordWindow.VersesByTranslationName[translation_name] = bible_translation.WordIndex.GetMatchingVerses(currently_selected_word);
            }
        }

        BibleVersesWithWordWindow.UpdateAndRender(CurrentlyHighlightedWord, currently_selected_word, user_settings);

        MetricsWindow.UpdateAndRender();
        StyleEditorWindow.UpdateAndRender();
        DemoWindow.UpdateAndRender();
        AboutWindow.UpdateAndRender();

        // RENDER THE GUI.
        ImGui::Render();
        ImDrawData* gui_draw_data = ImGui::GetDrawData();
        ImGui_ImplOpenGL3_RenderDrawData(gui_draw_data);
    }

    /// Shuts down the GUI.
    void Gui::Shutdown()
    {
        ImGui_ImplSDL2_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }
}