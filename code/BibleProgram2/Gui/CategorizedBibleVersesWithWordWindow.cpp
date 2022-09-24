#include <string_view>
#include <imgui/imgui.h>
#include <stmr/stmr.h>
#include "Gui/BibleVersesTextPanel.h"
#include "Gui/CategorizedBibleVersesWithWordWindow.h"

namespace GUI
{
    /// Updates and renders a single frame of the window, if it's open.
    /// @param[in,out]  user_selection - User selections that migth need updating.
    /// @param[in,out]  user_settings - User settings.
    void CategorizedBibleVersesWithWordWindow::UpdateAndRender(UserSelections& user_selections, UserSettings& user_settings)
    {
        // DON'T RENDER ANYTHING IF THE WINDOW ISN'T OPEN.
        if (!Open)
        {
            return;
        }

        // UPDATE THE WORD STEM IF IT IS OUT-OF-DATE.
        bool word_stem_populated = !WordStem.empty();
        if (!word_stem_populated)
        {
            constexpr int WORD_START_INDEX = 0;
            int word_character_count = static_cast<int>(Word.length());
            int word_stem_end_index = stem(const_cast<char*>(Word.c_str()), WORD_START_INDEX, word_character_count);
            WordStem = Word.substr(WORD_START_INDEX, word_stem_end_index);
        }

        // RENDER EACH VERSE IN THE WINDOW.
        // Window positioning/sizing is only done upon the first use to allow preserving a user's manual changes.
        ImVec2 current_drawing_cursor_position = ImGui::GetCursorPos();
        ImGuiIO& io = ImGui::GetIO();
        float verses_window_default_x_position = (io.DisplaySize.x / 2.0f);
        ImVec2 verses_window_default_position(verses_window_default_x_position, current_drawing_cursor_position.y);
        ImGui::SetNextWindowPos(verses_window_default_position, ImGuiCond_FirstUseEver);
        ImVec2 available_screen_space_in_pixels = (io.DisplaySize / 2.0f);
        available_screen_space_in_pixels.x -= 2.0f * ImGui::GetFrameHeightWithSpacing();
        available_screen_space_in_pixels.y = io.DisplaySize.y - 2.0f * ImGui::GetFrameHeightWithSpacing();
        ImGui::SetNextWindowSize(available_screen_space_in_pixels, ImGuiCond_FirstUseEver);

        std::string window_title_and_id = Word + "###Word";

        if (ImGui::Begin(window_title_and_id.c_str(), &Open))
        {
            // ALLOW A USER TO EDIT THE COLOR FOR THE CURRENT WORD.
            ImVec4 word_color = { 1.0f, 1.0f, 1.0f, 1.0f };
            bool word_already_has_color = user_settings.ColorsByWord.contains(Word);
            if (word_already_has_color)
            {
                word_color = user_settings.ColorsByWord[Word];
            }
            float color_components[3] =
            {
                word_color.x,
                word_color.y,
                word_color.z,
            };

            // No label is displayed because it results in too much space to the right of the colored square.
            if (ImGui::ColorEdit3(
                Word.c_str(),
                color_components,
                ImGuiColorEditFlags_NoInputs))
            {
                word_color.x = color_components[0];
                word_color.y = color_components[1];
                word_color.z = color_components[2];
                user_settings.ColorsByWord[Word] = word_color;
            }

            // DISPLAY THE STEM FOR THE CURRENT WORD.
            ImGui::SameLine();
            ImGui::Text("(stem: %s)", WordStem.c_str());

            // RENDER THE VERSES FOR EACH CATEGORY.
            constexpr int VERSE_CATEGORY_COUNT = 6;
            if (ImGui::BeginTable("RelatedVersesByCategoryTable", VERSE_CATEGORY_COUNT, ImGuiTableFlags_ScrollY))
            {
                // DEFINE COLUMN HEADERS FOR EACH CATEGORY.
                ImGui::TableSetupColumn("Same Chapter");
                ImGui::TableSetupColumn("Same Book");
                ImGui::TableSetupColumn("Same Author");
                ImGui::TableSetupColumn("Same Genre in Testament");
                ImGui::TableSetupColumn("Same Testament");
                ImGui::TableSetupColumn("Other Testament");
                ImGui::TableHeadersRow();

                // DETERMINE THE COLUMNS FOR DIFFERENT TRANSLATIONS.
                // One column is needed per displayed translation.
                unsigned int translation_column_count = 0;
                std::vector<std::string> visible_translation_names;
                for (const auto& translation_name_and_display_status : user_settings.BibleTranslationDisplayStatusesByName)
                {
                    // TRACK THE VISIBLE TRANSLATION.
                    bool translation_displayed = translation_name_and_display_status.second;
                    if (translation_displayed)
                    {
                        ++translation_column_count;
                        visible_translation_names.push_back(translation_name_and_display_status.first);
                    }
                }

                // RENDER VERSES FOR EACH CATEGORY IN SEPARATE COLUMNS.
                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###SameChapterVersesByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + " (" + std::to_string(current_search_results.VersesInSameChapter.size()) +")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesInSameChapter, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###SameBookVersesByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + "(" + std::to_string(current_search_results.VersesInSameBook.size()) + ")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesInSameBook, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###SameAuthorByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + "(" + std::to_string(current_search_results.VersesInOtherBooksByAuthor.size()) + ")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesInOtherBooksByAuthor, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###SameGenreByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + "(" + std::to_string(current_search_results.VersesInBooksOfSameGenreByOtherAuthorsInSameTestament.size()) + ")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesInBooksOfSameGenreByOtherAuthorsInSameTestament, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###SameTestamentByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + "(" + std::to_string(current_search_results.VersesElsewhereInSameTestament.size()) + ")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesElsewhereInSameTestament, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();
                if (ImGui::BeginTable("###OtherTestamentByTranslationTable", translation_column_count, ImGuiTableFlags_ScrollY))
                {
                    // DEFINE COLUMN HEADERS FOR EACH TRANSLATION.
                    for (const std::string& translation_name : visible_translation_names)
                    {
                        // The number of matching verses is included in the title for easy statistical information.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];
                        std::string column_title = translation_name + "(" + std::to_string(current_search_results.VersesFromOtherTestament.size()) + ")";
                        ImGui::TableSetupColumn(column_title.c_str());
                    }
                    ImGui::TableHeadersRow();

                    // DISPLAY VERSES FOR EACH TRANSLATION.
                    for (const auto& translation_name_with_verses : VersesByTranslationName)
                    {
                        // SKIP OVER INVISIBLE TRANSLATIONS.
                        const std::string& translation_name = translation_name_with_verses.first;
                        bool translation_visible = user_settings.BibleTranslationDisplayStatusesByName[translation_name];
                        if (!translation_visible)
                        {
                            continue;
                        }

                        // GET THE VERSES FOR THE TRANSLATION.
                        const BIBLE_DATA::CategorizedBibleVerseSearchResults& current_search_results = VersesByTranslationName[translation_name];

                        // DISPLAY THE VERSES IN THE CATEGORY.
                        ImGui::TableNextColumn();
                        BibleVersesTextPanel::UpdateAndRender(current_search_results.VersesFromOtherTestament, user_selections, user_settings);
                    }

                    // FINISH RENDERING THE TABLE.
                    ImGui::EndTable();
                }

                // FINISH RENDERING THE TABLE DISPLAYING VERSES ACROSS ALL CATEGORIES.
                ImGui::EndTable();
            }
        }
        ImGui::End();
    }
}
