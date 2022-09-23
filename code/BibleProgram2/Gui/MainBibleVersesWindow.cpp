#include <imgui/imgui.h>
#include "BibleData/BibleBook.h"
#include "Debugging/Timer.h"
#include "Gui/BibleVersesTextPanel.h"
#include "Gui/MainBibleVersesWindow.h"

namespace GUI
{
    /// Opens the window for displaying the specified range of verses according to the user settings.
    /// @param[in]  verse_range - The range of verses to display.
    /// @param[in]  bibles - The Bibles for which to display the verses.
    /// @param[in]  user_settings - User settings controlling display of verses.
    void MainBibleVersesWindow::OpenForVerses(
        const BIBLE_DATA::BibleVerseRange& verse_range, 
        const BIBLE_DATA::Bibles& bibles,
        const UserSettings& user_settings)
    {
        // INITIALIZE THE WINDOW FOR DISPLAYING THE NEW RANGE.
        Open = true;
        VerseRange = verse_range;

        // POPULATE THE VERSE TEXT IN THE WINDOW.
        VersesByTranslationName.clear();
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
                // TIME VERSE RETRIEVAL FOR ASSESSING PERFORMANCE.
                ::DEBUGGING::SystemClockTimer system_clock_timer("Bible Verse Finding System Clock Timer");
                ::DEBUGGING::HighResolutionTimer high_resolution_timer("Bible Verse Finding High-Resolution Timer");

                // GET ALL VERSES IN THE RANGE.
                const auto& name_with_bible_translation = bibles.TranslationsByAbbreviatedName.find(translation_name);
                const BIBLE_DATA::BibleTranslation& bible_translation = name_with_bible_translation->second;
                for (const BIBLE_DATA::BibleVerseIdWithCharacterRange& verse_id_with_character_range : bible_translation.VerseCharacterRanges)
                {
                    // CHECK IF THE CURRENT VERSE IS IN THE RANGE.
                    bool is_matching_verse = (
                        (verse_range.StartingVerse.Book <= verse_id_with_character_range.Id.Book) && (verse_id_with_character_range.Id.Book <= verse_range.EndingVerse.Book) &&
                        (verse_range.StartingVerse.ChapterNumber <= verse_id_with_character_range.Id.ChapterNumber) && (verse_id_with_character_range.Id.ChapterNumber <= verse_range.EndingVerse.ChapterNumber) &&
                        (verse_range.StartingVerse.VerseNumber <= verse_id_with_character_range.Id.VerseNumber) && (verse_id_with_character_range.Id.VerseNumber <= verse_range.EndingVerse.VerseNumber));
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
                        bool is_final_verse = (verse_id_with_character_range.Id == verse_range.EndingVerse);
                        if (is_final_verse)
                        {
                            break;
                        }
                    }
                }
            }

            // STORE THE VERSES FOR THE TRANSLATION.
            VersesByTranslationName[translation_name] = verses;
        }
    }

    /// Updates and renders a single frame of the window, if it's open.
    /// @param[in,out]  user_selections - Selections the user has made that might need updating.
    /// @param[in,out]  user_settings - User settings.
    void MainBibleVersesWindow::UpdateAndRender(UserSelections& user_selections, UserSettings& user_settings)
    {
        // DON'T RENDER ANYTHING IF THE WINDOW ISN'T OPEN.
        if (!Open)
        {
            return;
        }

        // INCLUDE THE VERSE RANGE IN THE WINDOW TITLE FOR EASY VISIBILITY.
        std::string window_title = (
            BIBLE_DATA::BibleBook::FullName(VerseRange.StartingVerse.Book) + " " +
            std::to_string(VerseRange.StartingVerse.ChapterNumber) + ":" +
            std::to_string(VerseRange.StartingVerse.VerseNumber));
        bool multiple_verses = (VerseRange.StartingVerse != VerseRange.EndingVerse);
        if (multiple_verses)
        {
            window_title += " - " +
                BIBLE_DATA::BibleBook::FullName(VerseRange.EndingVerse.Book) + " " +
                std::to_string(VerseRange.EndingVerse.ChapterNumber) + ":" +
                std::to_string(VerseRange.EndingVerse.VerseNumber);
        }
        std::string window_title_and_id = window_title + "###Verses";

        // POSITION THE WINDOW NEAR THE LEFT OF THE SCREEN.
        // Window positioning/sizing is only done upon the first use to allow preserving a user's manual changes.
        ImVec2 current_drawing_cursor_position = ImGui::GetCursorPos();
        float verses_window_default_x_position = current_drawing_cursor_position.x;
        ImVec2 verses_window_default_position(verses_window_default_x_position, current_drawing_cursor_position.y);
        ImGui::SetNextWindowPos(verses_window_default_position, ImGuiCond_FirstUseEver);

        // SIZE THE WINDOW TO TAKE UP A SMALLER PORTION OF THE LEFT SIDE OF THE WINDOW.
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 available_screen_space_in_pixels = (io.DisplaySize / 2.0f);
        available_screen_space_in_pixels.x -= 2.0f * ImGui::GetFrameHeightWithSpacing();
        available_screen_space_in_pixels.y = io.DisplaySize.y - 2.0f * ImGui::GetFrameHeightWithSpacing();
        ImGui::SetNextWindowSize(available_screen_space_in_pixels, ImGuiCond_FirstUseEver);

        // RENDER THE VERSES IN THE WINDOW.
        if (ImGui::Begin(window_title_and_id.c_str(), &Open))
        {
            // DETERMINE THE NUMBER OF COLUMNS TO DISPLAY IN THE WINDOW.
            // One column is needed per displayed translation.
            unsigned int column_count = 0;
            for (const auto& translation_name_and_display_status : user_settings.BibleTranslationDisplayStatusesByName)
            {
                bool translation_displayed = translation_name_and_display_status.second;
                if (translation_displayed)
                {
                    ++column_count;
                }
            }

            // CHECK IF ANY COLUMNS EXIST.
            // If data is still being loaded, then no columns will exist.
            // Providing 0 columns to ImGui will cause a crash.
            bool columns_exist = (column_count > 0);
            if (columns_exist)
            {
                // SET THE COLUMNS FOR THIS WINDOW.
                ImGui::Columns(column_count);

                // DISPLAY VERSES EACH TRANSLATION IN SEPARATE COLUMNS.
                for (const auto& translation_name_with_verses : VersesByTranslationName)
                {
                    // SKIP OVER VERSES FOR INVISIBLE TRNASLATIONS.
                    const std::string& translation_name = translation_name_with_verses.first;
                    bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                    if (!translation_visible)
                    {
                        continue;
                    }

                    // RENDER A CHILD WINDOW FOR THE TRANSLATION.
                    // This allows for independent scrolling of each column.
                    if (ImGui::BeginChild(translation_name.c_str()))
                    {
                        // RENDER THE TRANSLATION NAME.
                        ImGui::Text(translation_name.c_str());

                        // REDNER THE VERSES FOR THE TRANSLATION.
                        const std::vector<BIBLE_DATA::BibleVerse>& verses_for_translation = VersesByTranslationName[translation_name];
                        BibleVersesTextPanel::UpdateAndRender(verses_for_translation, user_selections, user_settings);
                    }
                    ImGui::EndChild();

                    // MOVE TO THE NEXT COLUMN.
                    ImGui::NextColumn();
                }
            }
        }

        // END RENDERING THE WINDOW.
        ImGui::End();
    }
}
