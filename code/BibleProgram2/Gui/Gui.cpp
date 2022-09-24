#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <implot/implot.h>
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

        ImPlot::CreateContext();

        Gui gui;
        return gui;
    }

    /// Updates the GUI in response to user input and renders it for the provided window.
    /// @param[in]  window - The window being interacted with and rendered to.
    /// @param[in]  bibles - The Bibles to use for verse data in the GUI.
    /// @param[in,out]  user_selections - The selections the user has made in the GUI.
    /// @param[in,out]  user_settings - User settings for the GUI.
    void Gui::UpdateAndRender(
        const WINDOWING::SdlWindow& window, 
        const BIBLE_DATA::Bibles& bibles,
        UserSelections& user_selections,
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
                // CLEAR THE CURRENTLY SELECTED VERSE.
                user_selections.CurrentlySelectedBibleVerseId = {};

                // PARSE THE VERSE RANGE.
                std::optional<BIBLE_DATA::BibleVerseRange> verse_range = BIBLE_DATA::BibleVerseRange::Parse(verse_range_text);
                if (verse_range)
                {
                    // PRINT OUT THE VERSE RANGE FOR DEBUGGING.
                    std::printf(
                        "Verse range: %u %u:%u - %u %u:%u\n",
                        verse_range->StartingVerse.Book,
                        verse_range->StartingVerse.ChapterNumber,
                        verse_range->StartingVerse.VerseNumber,
                        verse_range->EndingVerse.Book,
                        verse_range->EndingVerse.ChapterNumber,
                        verse_range->EndingVerse.VerseNumber);

                    // DISPLAY THE UPDATED RANGE OF VERSES IN THE MAIN WINDOW.
                    MainBibleVersesWindow.OpenForVerses(*verse_range, bibles, user_settings);                  
                }
            }

            ImGui::EndMainMenuBar();
        }

        // UPDATE AND RENDER THE MAIN VERSES WINDOW.
        std::string initial_selected_word = user_selections.CurrentlySelectedWord;
        MainBibleVersesWindow.UpdateAndRender(user_selections, user_settings);

        // UPDATE AND RENDER THE WINDOW FOR DISPLAYING CATEGORIZED VERSES.
        // Refreshing of the window for the current word may be needed if the word or translation changed.
        bool new_word_selected = (initial_selected_word != user_selections.CurrentlySelectedWord);
        bool displayed_verses_with_word_outdated = (new_word_selected || translation_changed);
        if (displayed_verses_with_word_outdated)
        {
            CategorizedBibleVersesWithWordWindow.Open = true;
            CategorizedBibleVersesWithWordWindow.Word = user_selections.CurrentlySelectedWord;
            CategorizedBibleVersesWithWordWindow.WordStem.clear();

            BibleVerseStatisticsWindow.Open = true;
            BibleVerseStatisticsWindow.Word = user_selections.CurrentlySelectedWord;
            BibleVerseStatisticsWindow.VersesByBook.clear();

            // UPDATE THE VERSES DISPLAYED IN THE WINDOWS.
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
                BIBLE_DATA::CategorizedBibleVerseSearchResults search_results = bible_translation.WordIndex.GetMatchingVerses(
                    user_selections.CurrentlySelectedBibleVerseId,
                    user_selections.CurrentlySelectedWord);
                CategorizedBibleVersesWithWordWindow.VersesByTranslationName[translation_name] = search_results;

                /// @todo   How to handle different translations for this statistics window?
                std::vector<BIBLE_DATA::BibleVerse> verses_with_word = bible_translation.WordIndex.GetMatchingVerses(user_selections.CurrentlySelectedWord);
                for (const BIBLE_DATA::BibleVerse& verse : verses_with_word)
                {
                    BibleVerseStatisticsWindow.VersesByBook[verse.Id.Book].push_back(verse);
                }
            }
        }

        CategorizedBibleVersesWithWordWindow.UpdateAndRender(user_selections, user_settings);
        BibleVerseStatisticsWindow.UpdateAndRender();

        // UPDATE AND RENDER DEBUGGING WINDOWS.
        MetricsWindow.UpdateAndRender();
        StyleEditorWindow.UpdateAndRender();
        DemoWindow.UpdateAndRender();
        AboutWindow.UpdateAndRender();

        // FINISH RENDERING THE GUI.
        ImGui::Render();
        ImDrawData* gui_draw_data = ImGui::GetDrawData();
        ImGui_ImplOpenGL3_RenderDrawData(gui_draw_data);
    }

    /// Shuts down the GUI.
    void Gui::Shutdown()
    {
        ImPlot::DestroyContext();
        ImGui_ImplSDL2_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }
}