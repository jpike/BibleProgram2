#include <imgui/imgui.h>
#include "BibleData/BibleBook.h"
#include "Gui/BibleVersesTextPanel.h"
#include "Gui/MainBibleVersesWindow.h"

namespace GUI
{
    /// Updates and renders a single frame of the window, if it's open.
    /// @param[in,out]  currently_highlighted_word - The currently highlighted word.
    /// @param[in,out]  currently_selected_word - The currently selected word.
    /// @param[in,out]  user_settings - User settings.
    void MainBibleVersesWindow::UpdateAndRender(
        std::string& currently_highlighted_word, 
        std::string& currently_selected_word,
        UserSettings& user_settings)
    {
        // DON'T RENDER ANYTHING IF THE WINDOW ISN'T OPEN.
        if (!Open)
        {
            return;
        }

        // RENDER EACH VERSE IN THE WINDOW.
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

        // Window positioning/sizing is only done upon the first use to allow preserving a user's manual changes.
        ImVec2 current_drawing_cursor_position = ImGui::GetCursorPos();
        //constexpr float ADDITIONAL_PADDING_IN_PIXELS = 4.0f;
        constexpr float ADDITIONAL_PADDING_IN_PIXELS = 0.0f;
        float verses_window_default_x_position = current_drawing_cursor_position.x + ADDITIONAL_PADDING_IN_PIXELS;
        ImVec2 verses_window_default_position(verses_window_default_x_position, current_drawing_cursor_position.y);
        ImGui::SetNextWindowPos(verses_window_default_position, ImGuiCond_FirstUseEver);
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 available_screen_space_in_pixels = (io.DisplaySize / 2.0f);
        available_screen_space_in_pixels.x -= 2.0f * ImGui::GetFrameHeightWithSpacing();
        available_screen_space_in_pixels.y = io.DisplaySize.y - 2.0f * ImGui::GetFrameHeightWithSpacing();
        ImGui::SetNextWindowSize(available_screen_space_in_pixels, ImGuiCond_FirstUseEver);

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

                for (const auto& translation_name_with_verses : VersesByTranslationName)
                {
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
                        BibleVersesTextPanel::UpdateAndRender(verses_for_translation, currently_highlighted_word, currently_selected_word, user_settings);
                    }
                    ImGui::EndChild();

                    // MOVE TO THE NEXT COLUMN.
                    ImGui::NextColumn();
                }
            }
        }
        ImGui::End();
    }
}
