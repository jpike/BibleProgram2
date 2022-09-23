#include <algorithm>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "BibleData/BibleWordIndex.h"
#include "BibleData/Token.h"
#include "Gui/BibleVersesTextPanel.h"
#include "Gui/TextRenderCommand.h"

namespace GUI
{
    /// Updates and renders the specified verse content in the window.
    /// Some of the implementation here is based on ImGui's multi-line text input
    /// but modified/simplified for our purposes here.
    /// @param[in]  verses - The verse content to render.
    /// @param[in,out]  user_selections - User selections.
    /// @param[in,out]  user_settings - User settings.
    void BibleVersesTextPanel::UpdateAndRender(
        const std::vector<BIBLE_DATA::BibleVerse>& verses, 
        UserSelections& user_selections,
        UserSettings& user_settings)
    {
        // COMPUTE THE BOUNDING BOX FOR THE VERSE TEXT.
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 window_content_region_size = ImGui::GetWindowContentRegionMax();
        ImVec2 entire_text_bounding_box_max_coordinates = window->DC.CursorPos + window_content_region_size;
        // Each window has some amount of padding (indentation) around its border for readability
        // that needs to be accounted for in order to keep the right side of the next from
        // bumping up against the left of the window.
        entire_text_bounding_box_max_coordinates.x -= window->DC.Indent.x;
        ImRect entire_text_bounding_box(window->DC.CursorPos, entire_text_bounding_box_max_coordinates);

        // FORM THE FINAL TEXT FOR THE VERSES.
        std::vector<TextRenderCommand> text_render_commands;

        BIBLE_DATA::BibleBook::Id previous_book = BIBLE_DATA::BibleBook::Id::INVALID;
        unsigned int previous_chapter_number = 0;
        ImVec2 current_draw_position = window->DC.CursorPos;
        bool first_verse = true;
        bool first_chapter = true;
        bool first_book = true;
        for (const BIBLE_DATA::BibleVerse& verse : verses)
        {
            // PRINT SOME TEXT TO INDICATE IF A NEW BOOK IS BEING STARTED.
            bool new_book_being_started = (previous_book != verse.Id.Book);
            if (new_book_being_started)
            {
                if (!first_book)
                {
                    current_draw_position.y += ImGui::GetTextLineHeight();
                    text_render_commands.back().Text += '\n';
                }
                current_draw_position.x = entire_text_bounding_box.GetBL().x;

                std::string book_name = BIBLE_DATA::BibleBook::FullName(verse.Id.Book);
                std::transform(
                    book_name.begin(),
                    book_name.end(),
                    book_name.begin(),
                    [](const char character) { return static_cast<char>(std::toupper(character)); });

                ImVec2 text_size = ImGui::CalcTextSize(book_name.c_str());

                TextRenderCommand book_title_render_command =
                {
                    .BibleVerseId = verse.Id,
                    .Text = book_name + '\n',
                    .Color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                    .DrawPosition = current_draw_position,
                };
                book_title_render_command.TextSize = ImGui::CalcTextSize(book_title_render_command.Text.c_str());
                book_title_render_command.TextBoundingBox = ImRect(
                    book_title_render_command.DrawPosition, 
                    book_title_render_command.DrawPosition + text_size);
                text_render_commands.push_back(book_title_render_command);

                current_draw_position.y += ImGui::GetTextLineHeight();

                previous_book = verse.Id.Book;
                first_book = false;
                first_chapter = true;
            }
            // PRINT SOME TEXT TO INDICATE IF A NEW CHAPTER IS BEING STARTED.
            bool new_chapter_being_started = (previous_chapter_number != verse.Id.ChapterNumber) || (new_book_being_started);
            if (new_chapter_being_started)
            {
                if (!first_chapter)
                {
                    current_draw_position.y += ImGui::GetTextLineHeight();
                    text_render_commands.back().Text += '\n';
                }
                current_draw_position.x = entire_text_bounding_box.GetBL().x;

                std::string chapter_text = "CHAPTER " + std::to_string(verse.Id.ChapterNumber);

                ImVec2 text_size = ImGui::CalcTextSize(chapter_text.c_str());

                TextRenderCommand chapter_title_render_command =
                {
                    .BibleVerseId = verse.Id,
                    .Text = chapter_text + '\n',
                    .Color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                    .DrawPosition = current_draw_position,
                };
                chapter_title_render_command.TextSize = ImGui::CalcTextSize(chapter_title_render_command.Text.c_str());
                chapter_title_render_command.TextBoundingBox = ImRect(
                    chapter_title_render_command.DrawPosition,
                    chapter_title_render_command.DrawPosition + text_size);
                text_render_commands.push_back(chapter_title_render_command);

                current_draw_position.y += ImGui::GetTextLineHeight();

                previous_chapter_number = verse.Id.ChapterNumber;
                first_chapter = false;
                first_verse = true;
            }

            // PRINT THE VERSE NUMBER.
            {
                std::string verse_number = std::to_string(verse.Id.VerseNumber) + ' ';
                if (first_verse)
                {
                    first_verse = false;
                }
                else
                {
                    verse_number = ' ' + verse_number;
                }

                ImVec2 text_size = ImGui::CalcTextSize(verse_number.c_str());
                ImVec2 text_end_position = current_draw_position + text_size;
                bool text_fits_on_current_line = (text_end_position.x <= entire_text_bounding_box_max_coordinates.x);
                if (!text_fits_on_current_line)
                {
                    current_draw_position.x = entire_text_bounding_box.GetBL().x;
                    current_draw_position.y += ImGui::GetTextLineHeight();

                    text_render_commands.back().Text += '\n';

                    // Remove any extra spaces from the beginning of the line.
                    verse_number = std::to_string(verse.Id.VerseNumber) + ' ';
                }

                TextRenderCommand verse_number_render_command =
                {
                    .BibleVerseId = verse.Id,
                    .Text = verse_number,
                    .Color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                    .DrawPosition = current_draw_position,
                };
                verse_number_render_command.TextSize = ImGui::CalcTextSize(verse_number_render_command.Text.c_str());
                verse_number_render_command.TextBoundingBox = ImRect(
                    verse_number_render_command.DrawPosition,
                    verse_number_render_command.DrawPosition + text_size);
                text_render_commands.push_back(verse_number_render_command);

                current_draw_position.x += text_size.x;
            }

            std::vector<BIBLE_DATA::Token> tokens = BIBLE_DATA::Token::Tokenize(verse.Text);
            for (const BIBLE_DATA::Token& token : tokens)
            {
                std::string token_text = std::string(token.Text);
                ImVec2 text_size = ImGui::CalcTextSize(token_text.c_str());
                ImVec2 text_end_position = current_draw_position + text_size;
                bool text_fits_on_current_line = 
                    (text_end_position.x <= entire_text_bounding_box_max_coordinates.x) ||
                    // Spaces don't need to start a new line.
                    (BIBLE_DATA::TokenType::SPACE == token.Type);
                if (!text_fits_on_current_line)
                {
                    current_draw_position.x = entire_text_bounding_box.GetBL().x;
                    current_draw_position.y += ImGui::GetTextLineHeight();

                    //token_text = '\n' + token_text;
                    text_render_commands.back().Text += '\n';
                }

                TextRenderCommand token_render_command =
                {
                    .BibleVerseId = verse.Id,
                    .Text = token_text,
                    .Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                    .DrawPosition = current_draw_position,
                };
                token_render_command.TextSize = ImGui::CalcTextSize(token_render_command.Text.c_str());
                token_render_command.TextBoundingBox = ImRect(
                    token_render_command.DrawPosition,
                    token_render_command.DrawPosition + text_size);
                text_render_commands.push_back(token_render_command);

                current_draw_position.x += text_size.x;
            }
        }
        std::string final_rendered_verse_text;
        for (const TextRenderCommand& text_render_command : text_render_commands)
        {
            final_rendered_verse_text += text_render_command.Text;
        }

        // ALLOCATE AN ID FOR REFERENCING THE TEXT.
        ImGuiID text_id = window->GetID("###VerseText");

        // CHANGE THE CURSOR TO INDICATE SELECTABLE TEXT IF THE ITEM IS HOVERABLE.
        ImGuiContext& gui_context = *GImGui;
        bool hovered = ImGui::ItemHoverable(entire_text_bounding_box, text_id);
        if (hovered)
        {
            gui_context.MouseCursor = ImGuiMouseCursor_TextInput;
        }

        // GET THE INPUT TEXT STATE FOR THIS VERSE TEXT BOX.
        ImGuiInputTextState* input_text_state = nullptr;
        bool current_verse_text_being_tracked_for_input = (gui_context.InputTextState.ID == text_id);
        if (current_verse_text_being_tracked_for_input)
        {
            input_text_state = &gui_context.InputTextState;
        }

        // ALLOW THIS TEXT BOX TO RECEIVE FOCUS.
        /// @todo bool focus_requested = ImGui::FocusableItemRegister(window, text_id);

        // CHECK IF THE USER CLICKED INTO THIS TEXT AREA.
        ImGuiIO& user_input = gui_context.IO;
        constexpr int LEFT_MOUSE_BUTTON = 0;
        bool user_clicked_on_text_area = hovered && user_input.MouseClicked[LEFT_MOUSE_BUTTON];

        // CHECK THE STATE OF SCROLLING IN THE WINDOW.
        bool no_current_active_id = (gui_context.ActiveId == 0);
        ImGuiID vertical_scrollbar_id = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
        bool vertical_scrollbar_active_previously = (gui_context.ActiveIdPreviousFrame == vertical_scrollbar_id);
        bool user_scroll_finish = no_current_active_id && vertical_scrollbar_active_previously;
        bool user_scroll_active = (gui_context.ActiveId == vertical_scrollbar_id);

        // INITIALIZE INFORMATION FOR TRACKING INPUT ON THE TEXT.
        bool clear_active_id = false;
        /// @todo bool should_activate = (focus_requested || user_clicked_on_text_area || user_scroll_finish);
        bool should_activate = (user_clicked_on_text_area || user_scroll_finish);
        bool different_item_previously_active = (gui_context.ActiveId != text_id);
        bool newly_initializing_input_text = (should_activate || user_scroll_active) && different_item_previously_active;
        if (newly_initializing_input_text)
        {
            // INITIALIZE THE INPUT TEXT STATE.
            input_text_state = &gui_context.InputTextState;
            // +1 is needed to be able to select last character.
            int entire_text_length = static_cast<int>(final_rendered_verse_text.length() + 1);

            // TextW is used for copying, so it needs to be populated.
            constexpr int ALLOW_AT_LEAST_1_CHARACTER_FOR_EMPTY_STRING = 1;
            input_text_state->TextW.resize(entire_text_length + ALLOW_AT_LEAST_1_CHARACTER_FOR_EMPTY_STRING);
            constexpr char* USE_IMPLICIT_NULL_TERMINATOR = nullptr;
            input_text_state->CurLenW = ImTextStrFromUtf8(
                input_text_state->TextW.Data, 
                entire_text_length, 
                final_rendered_verse_text.c_str(), 
                USE_IMPLICIT_NULL_TERMINATOR);

            input_text_state->ID = text_id;
            input_text_state->ScrollX = 0.0f;

            constexpr bool IS_MULTI_LINE = false;
            stb_textedit_initialize_state(&input_text_state->Stb, IS_MULTI_LINE);

            // ACTIVATE THE WINDOW
            ImGui::SetActiveID(text_id, window);
            ImGui::SetFocusID(text_id, window);
            ImGui::FocusWindow(window);
        }
        else if (user_input.MouseClicked[LEFT_MOUSE_BUTTON])
        {
            // RELEASE FOCUS WHEN CLICKED OUTSIDE THE WINDOW.
            clear_active_id = true;
        }

        // PROCESS MOUSE INPUT IF THE TEXT BOX IS ACTIVE.
        bool current_text_box_active = (gui_context.ActiveId == text_id);
        if (current_text_box_active)
        {
            // CALCULATE THE MOUSE POSITION WITHIN THE WINDOW.
            float mouse_x = (user_input.MousePos.x - entire_text_bounding_box.Min.x - gui_context.Style.FramePadding.x + input_text_state->ScrollX);
            float mouse_y = (user_input.MousePos.y - window->DC.CursorPos.y - gui_context.Style.FramePadding.y);

            // DETERMINE THE KIND OF MOUSE INTERACTION THAT'S OCCURRED.
            bool user_double_clicked_on_text_area = hovered && user_input.MouseDoubleClicked[LEFT_MOUSE_BUTTON];
            bool user_clicked_when_not_already_selecting_all = user_input.MouseClicked[LEFT_MOUSE_BUTTON] && !input_text_state->SelectedAllMouseLock;
            bool user_dragged_over_text = (
                user_input.MouseDown[LEFT_MOUSE_BUTTON] && 
                !input_text_state->SelectedAllMouseLock && 
                (user_input.MouseDelta.x != 0.0f || user_input.MouseDelta.y != 0.0f));
            if (user_double_clicked_on_text_area)
            {
                // SELECT ALL TEXT.
                input_text_state->SelectAll();
                input_text_state->SelectedAllMouseLock = true;
            }
            else if (user_clicked_when_not_already_selecting_all)
            {
                if (hovered)
                {
                    stb_textedit_click(input_text_state, &input_text_state->Stb, mouse_x, mouse_y);
                    input_text_state->CursorAnimReset();
                }
            }
            else if (user_dragged_over_text)
            {
                stb_textedit_drag(input_text_state, &input_text_state->Stb, mouse_x, mouse_y);
                input_text_state->CursorAnimReset();
                input_text_state->CursorFollow = true;
            }

            // RELEASE LOCKING OF SELECT ALL IF THE USER RELEASED THE MOUSE AFTERWARDS.
            bool select_all_mouse_button_released = input_text_state->SelectedAllMouseLock && !user_input.MouseDown[LEFT_MOUSE_BUTTON];
            if (select_all_mouse_button_released)
            {
                input_text_state->SelectedAllMouseLock = false;
            }
        }

        // COMPUTE WHAT MAY NEED TO BE RENDERED.
        bool render_cursor = current_text_box_active || (input_text_state && user_scroll_active);
        bool render_selection = render_cursor && input_text_state && input_text_state->HasSelection();

        // HANDLE KEYBOARD INPUT.
        bool text_box_will_remain_active_and_did_not_just_activate = current_text_box_active && !gui_context.ActiveIdIsJustActivated && !clear_active_id;
        if (text_box_will_remain_active_and_did_not_just_activate)
        {
            // DETERMINE IF SPECIAL KEYS WERE PRESSED.
            bool is_shortcut_key = user_input.KeyCtrl;
            bool is_copy = is_shortcut_key && ImGui::IsKeyPressedMap(ImGuiKey_C);
            bool is_select_all = is_shortcut_key && ImGui::IsKeyPressedMap(ImGuiKey_A);

            // PROCESS DIFFERENT KEY PRESSES.
            if (ImGui::IsKeyPressedMap(ImGuiKey_LeftArrow))
            {
                input_text_state->OnKeyPressed(STB_TEXTEDIT_K_LEFT);
            }
            else if (ImGui::IsKeyPressedMap(ImGuiKey_RightArrow))
            {
                input_text_state->OnKeyPressed(STB_TEXTEDIT_K_RIGHT);
            }
            else if (ImGui::IsKeyPressedMap(ImGuiKey_UpArrow))
            {
                input_text_state->OnKeyPressed(STB_TEXTEDIT_K_UP);
            }
            else if (ImGui::IsKeyPressedMap(ImGuiKey_DownArrow))
            {
                input_text_state->OnKeyPressed(STB_TEXTEDIT_K_DOWN);
            }
            else if (is_select_all)
            {
                input_text_state->SelectAll();
                input_text_state->CursorFollow = true;
            }
            else if (is_copy)
            {
                // MAKE SURE A CLIPBOARD IS SUPPORT.
                if (user_input.SetClipboardTextFn)
                {
                    // COMPUTE THE RANGE OF SELECTED TEXT.
                    int selected_text_begin = 0;
                    int selected_text_end = input_text_state->CurLenW;
                    if (input_text_state->HasSelection())
                    {
                        selected_text_begin = ImMin(input_text_state->Stb.select_start, input_text_state->Stb.select_end);
                        selected_text_end = ImMax(input_text_state->Stb.select_start, input_text_state->Stb.select_end);
                    }
                    
                    const ImWchar* text_beginning = input_text_state->TextW.Data + selected_text_begin;
                    const ImWchar* text_ending = input_text_state->TextW.Data + selected_text_end;
                    int clipboard_data_length = ImTextCountUtf8BytesFromStr(text_beginning, text_ending);
                    constexpr int ROOM_FOR_NULL_TERMINATOR = 1;
                    clipboard_data_length += ROOM_FOR_NULL_TERMINATOR;

                    // COPY DATA TO THE CLIPBOARD.
                    char* clipboard_data = (char*)IM_ALLOC(clipboard_data_length * sizeof(char));
                    ImTextStrToUtf8(clipboard_data, clipboard_data_length, text_beginning, text_ending);
                    ImGui::SetClipboardText(clipboard_data);
                    ImGui::MemFree(clipboard_data);
                }
            }

            // ALLOW UPDATED INPUT TO AFFECT RENDERING OF A SELECTION.
            render_selection |= input_text_state->HasSelection() && render_cursor;
        }

        // RELEASE THE ACTIVE ID ON THIS TEXT BOX IF APPROPRIATE.
        bool current_text_box_active_but_should_be_released = clear_active_id && current_text_box_active;
        if (current_text_box_active_but_should_be_released)
        {
            ImGui::ClearActiveID();
        }

        // DETERMINE IF SPECIAL TEXT INPUT RENDERING SHOULD BE APPLIED.
        bool render_text_input_features = render_cursor || render_selection;
        if (render_text_input_features)
        {
            // COMPUTE THE CLIPPING RECTANGLE.
            const ImVec4 clip_rectangle(
                entire_text_bounding_box.Min.x,
                entire_text_bounding_box.Min.y,
                entire_text_bounding_box.Min.x + window_content_region_size.x,
                entire_text_bounding_box.Min.y + window_content_region_size.y);

            // INTIALIZE VARIABLES RELATED TO DRAWING THE SELECTION AND CURSOR.
            ImVec2 draw_position = window->DC.CursorPos;
            const ImVec2 draw_scroll = ImVec2(input_text_state->ScrollX, 0.0f);
            ImVec2 cursor_offset;

            // RENDER THE SELECTION IF APPLICABLE.
            if (render_selection)
            {
                // COMPUTE THE OFFSET FOR THE SELECTION.
                ImVec2 select_start_offset;
                const ImWchar* text_begin = input_text_state->TextW.Data;

                constexpr int LINE_NUMBER_NOT_FOUND = -1;
                int searches_remaining = 0;

                const ImWchar* character_straddling_cursor = nullptr;
                int line_number_straddling_cursor = -1000;
                if (render_cursor)
                {
                    character_straddling_cursor = text_begin + input_text_state->Stb.cursor;
                    line_number_straddling_cursor = LINE_NUMBER_NOT_FOUND;
                    ++searches_remaining;
                }

                const ImWchar* character_straddling_selection_start = nullptr;
                int line_number_straddling_selection_start = -1000;
                if (render_selection)
                {
                    character_straddling_selection_start = text_begin + ImMin(input_text_state->Stb.select_start, input_text_state->Stb.select_end);
                    line_number_straddling_selection_start = LINE_NUMBER_NOT_FOUND;
                    ++searches_remaining;
                }

                ++searches_remaining;
                int line_count = 0;
                constexpr char NULL_TERMINATOR = 0;
                for (const ImWchar* character = text_begin; *character != NULL_TERMINATOR; ++character)
                {
                    // COUNT ANY NEWLINES.
                    bool is_newline = *character == '\n';
                    if (is_newline)
                    {
                        ++line_count;

                        // UPDATE THE LINE STRADDLING THE CURSOR.
                        bool line_number_straddling_cursor_not_found = line_number_straddling_cursor == LINE_NUMBER_NOT_FOUND && character >= character_straddling_cursor;
                        if (line_number_straddling_cursor_not_found)
                        { 
                            line_number_straddling_cursor = line_count;

                            // STOP IF WE SHOULDN'T SEARCH ANYMORE.
                            if (--searches_remaining <= 0)
                            {
                                break;
                            }
                        }

                        // UPDATE THE LINE STRADDLING THE SELECTION.
                        bool line_number_straddling_selection_not_found = line_number_straddling_selection_start == LINE_NUMBER_NOT_FOUND && character >= character_straddling_selection_start;
                        if (line_number_straddling_selection_not_found)
                        { 
                            line_number_straddling_selection_start = line_count; 

                            // STOP IF WE SHOULDN'T SEARCH ANYMORE.
                            if (--searches_remaining <= 0)
                            {
                                break;
                            }
                        }
                    }
                }
                // MAKE SURE SOME LINE NUMBERS WERE FOUND.
                line_count++;
                if (line_number_straddling_cursor == LINE_NUMBER_NOT_FOUND)
                {
                    line_number_straddling_cursor = line_count;
                }
                if (line_number_straddling_selection_start == LINE_NUMBER_NOT_FOUND)
                {
                    line_number_straddling_selection_start = line_count;
                }

                // CALCULATE OFFSETS TO THE CURSOR AND SELECTION.
                cursor_offset.x = InputTextCalcTextSizeW(
                    ImStrbolW(character_straddling_cursor, text_begin), 
                    character_straddling_cursor).x;
                cursor_offset.y = line_number_straddling_cursor * gui_context.FontSize;
                if (line_number_straddling_selection_start >= 0)
                {
                    select_start_offset.x = InputTextCalcTextSizeW(
                        ImStrbolW(character_straddling_selection_start, text_begin), 
                        character_straddling_selection_start).x;
                    select_start_offset.y = line_number_straddling_selection_start * gui_context.FontSize;
                }

                // HANDLE SCROLLING FOR THE CURSOR.
                bool following_and_rendering_cursor = render_cursor && input_text_state->CursorFollow;
                if (following_and_rendering_cursor)
                {
                    // RESET PARTS OF THE INPUT TEXT STATE..
                    input_text_state->ScrollX = 0.0f;
                    input_text_state->CursorFollow = false;

                    // CALCULATE THE SIZE OF THE CURRENT TEXT ITEM.
                    // This is needed for proper, stable scrolling.
                    ImVec2 text_item_size = ImGui::CalcItemSize(
                        ImVec2(),
                        ImGui::CalcItemWidth(),
                        (ImGui::GetTextLineHeight() * 8.0f) + gui_context.Style.FramePadding.y * 2.0f);

                    // UPDATE VERTICAL SCROLLING.
                    float scroll_y = window->Scroll.y;
                    bool cursor_before_window_scroll_position = cursor_offset.y - gui_context.FontSize < scroll_y;
                    bool cursor_at_or_after_window_scroll_position = cursor_offset.y - text_item_size.y >= scroll_y;
                    if (cursor_before_window_scroll_position)
                    {
                        // KEEP THE CURSOR IN VIEW OF THE WINDOW'S SCROLLING.
                        scroll_y = ImMax(0.0f, cursor_offset.y - gui_context.FontSize);
                    }
                    else if (cursor_at_or_after_window_scroll_position)
                    {
                        scroll_y = cursor_offset.y - text_item_size.y;
                    }
                    // UPDATE THE DRAW POSITION FOR THE CURSOR.
                    draw_position.y += (window->Scroll.y - scroll_y);

                    // UPDATE THE WINDOW SCROLL AMOUNT.
                    window->Scroll.y = scroll_y;
                }

                // RENDER SHADED RECTANGLES BEHIND THE SELECTED TEXT.
                const ImWchar* text_selected_begin = text_begin + ImMin(input_text_state->Stb.select_start, input_text_state->Stb.select_end);
                const ImWchar* text_selected_end = text_begin + ImMax(input_text_state->Stb.select_start, input_text_state->Stb.select_end);
                ImU32 selected_text_background_color = ImGui::GetColorU32(ImGuiCol_TextSelectedBg);
                float background_top_offset = 0.0f;
                float background_bottom_offset = 0.0f;
                ImVec2 rectangle_position = draw_position + select_start_offset - draw_scroll;
                for (const ImWchar* character = text_selected_begin; character < text_selected_end;)
                {
                    // CHECK IF THERE'S MORE ROOM BEFORE THE TOP OF THE CLIP RECTANGLE.
                    bool room_before_top_of_clip_rectangle = rectangle_position.y < clip_rectangle.y;
                    if (room_before_top_of_clip_rectangle)
                    {
                        // MOVE TO THE END OF THE TEXT OR THE NEXT NEWLINE.
                        while (character < text_selected_end)
                        {
                            // BREAK OUT OF THE LOOP IF WE SHOULD MOVE TO A NEW LINE.
                            if (*character++ == '\n')
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        // RENDER A SHADED RECTANGLE FOR THE BACKGROUND.
                        ImVec2* no_offset_output_needed = nullptr;
                        constexpr bool STOP_ON_NEW_LINES = true;
                        ImVec2 rectangle_size = InputTextCalcTextSizeW(character, text_selected_end, &character, no_offset_output_needed, STOP_ON_NEW_LINES);
                        // MAKE SURE SELECTED EMPTY LINES CAN BE SEEN.
                        bool line_is_empty = rectangle_size.x <= 0.0f;
                        if (line_is_empty)
                        {
                            // Use half the width of a space for empty lines.
                            float space_character_advance = gui_context.Font->GetCharAdvance((ImWchar)' ');
                            rectangle_size.x = space_character_advance * 0.5f;
                        }

                        ImRect background_rectangle(
                            rectangle_position + ImVec2(0.0f, background_top_offset - gui_context.FontSize), 
                            rectangle_position + ImVec2(rectangle_size.x, background_bottom_offset));
                        window->DrawList->AddRectFilled(background_rectangle.Min, background_rectangle.Max, selected_text_background_color);
                    }

                    // MOVE THE RECTANGLE TO THE NEXT POSITION.
                    rectangle_position.x = draw_position.x - draw_scroll.x;
                    rectangle_position.y += gui_context.FontSize;
                }
            }

            // DRAW A BLINKING CURSOR IF APPLICABLE.
            if (render_cursor)
            {
                // UPDATE THE CURSOR BLINKING.
                input_text_state->CursorAnim += user_input.DeltaTime;

                // DETERMINE IF THE CURSOR SHOULD BE VISIBLE.
                // The numbers used here reflect what the actual ImGui code does.
                bool cursor_is_visible = (input_text_state->CursorAnim <= 0.0f) || ImFmod(input_text_state->CursorAnim, 1.20f) <= 0.80f;
                if (cursor_is_visible)
                {
                    // DRAW THE CURSOR.
                    ImVec2 cursor_screen_position = draw_position + cursor_offset - draw_scroll;
                    ImRect cursor_screen_rectangle(
                        cursor_screen_position.x,
                        /// @todo   Why does the actual ImGui code use this formula?
                        cursor_screen_position.y - gui_context.FontSize + 0.5f, cursor_screen_position.x + 1.0f, cursor_screen_position.y - 1.5f);
                    window->DrawList->AddLine(cursor_screen_rectangle.Min, cursor_screen_rectangle.GetBL(), ImGui::GetColorU32(ImGuiCol_Text));
                }
            }
        }

        // RENDER ALL OF THE TEXT.
        for (const TextRenderCommand& text_render_command : text_render_commands)
        {
            // CALCULATE THE BOUNDING BOX FOR THE TEXT.
            /// @todo Adding items here messed a bit with scrolling.
            // ItemSize adds scrolling
#if 1
            ImGui::ItemSize(text_render_command.TextSize);
#endif

            std::string lowercase_word = text_render_command.Text;
            std::transform(
                lowercase_word.begin(),
                lowercase_word.end(),
                lowercase_word.begin(),
                [](const char character) { return static_cast<char>(std::tolower(character)); });
            std::string normalized_word = lowercase_word;
            auto current_word_and_root_word = BIBLE_DATA::BibleWordIndex::RootWordsByOriginalWord.find(lowercase_word);
            bool root_word_found = (BIBLE_DATA::BibleWordIndex::RootWordsByOriginalWord.cend() != current_word_and_root_word);
            if (root_word_found)
            {
                normalized_word = current_word_and_root_word->second;
            }

            // GET THE COLOR.
            ImVec4 color = text_render_command.Color;
            auto current_word_color = user_settings.ColorsByWord.find(normalized_word);
            bool current_word_color_exists = (user_settings.ColorsByWord.cend() != current_word_color);
            if (current_word_color_exists)
            {
                color = current_word_color->second;
            }

            // SET THE TEXT COLOR.
            // A temporarily different (usually) highlight color is used when hovering over.
            bool mouse_over_text_bounding_box = text_render_command.TextBoundingBox.Contains(gui_context.IO.MousePos);
            bool is_currently_highlighted_word = (normalized_word == user_selections.CurrentlyHighlightedWord) || mouse_over_text_bounding_box;
            if (is_currently_highlighted_word)
            {
                /// @todo   Something other than yellow for highlights?
                color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

                user_selections.CurrentlyHighlightedWord = normalized_word;
            }

            if (mouse_over_text_bounding_box)
            {
                if (user_input.MouseClicked[LEFT_MOUSE_BUTTON])
                {
                    std::printf("Clicked on %s\n", user_selections.CurrentlyHighlightedWord.c_str());

                    user_selections.CurrentlySelectedWord = user_selections.CurrentlyHighlightedWord;

                    user_selections.CurrentlySelectedBibleVerseId = text_render_command.BibleVerseId;
                }
            }
                   
            ImU32 packed_color = ImGui::ColorConvertFloat4ToU32(color);

            // RENDER THE TEXT.
            constexpr char* USE_IMPLICIT_NULL_TERMINATOR = nullptr;
            window->DrawList->AddText(
                gui_context.Font,
                gui_context.FontSize,
                text_render_command.DrawPosition,
                packed_color,
                text_render_command.Text.c_str(),
                USE_IMPLICIT_NULL_TERMINATOR);
        }
    }
}
