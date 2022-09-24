#include <imgui/imgui.h>
#include <implot/implot.h>
#include "Gui/BibleVerseStatisticsWindow.h"

namespace GUI
{
    /// Updates and renders the window, if it is visible.
    void BibleVerseStatisticsWindow::UpdateAndRender()
    {
        // DON'T RENDER ANYTHING IF THE WINDOW ISN'T OPEN.
        if (!Open)
        {
            return;
        }

        // INCLUDE THE WORD IN THE WINDOW TITLE FOR EASY VISIBILITY.
        std::string window_title_and_id = Word + "###Statistics";

        // POSITION THE WINDOW NEAR THE RIGHT-QUARTER OF THE SCREEN.
        // Window positioning/sizing is only done upon the first use to allow preserving a user's manual changes.
        ImVec2 current_drawing_cursor_position = ImGui::GetCursorPos();
        ImGuiIO& io = ImGui::GetIO();
        float verses_window_default_x_position = current_drawing_cursor_position.x + (io.DisplaySize.x * 3.0f / 4.0f) + ImGui::GetFrameHeightWithSpacing();
        ImVec2 verses_window_default_position(verses_window_default_x_position, current_drawing_cursor_position.y);
        ImGui::SetNextWindowPos(verses_window_default_position, ImGuiCond_FirstUseEver);

        // SIZE THE WINDOW TO TAKE UP A SMALLER PORTION OF THE RIGHT SIDE OF THE WINDOW.
        ImVec2 available_screen_space_in_pixels = (io.DisplaySize / 2.0f);
        // The window is designed to take up the left quarter of the screen.
        available_screen_space_in_pixels.x = available_screen_space_in_pixels.x / 2.0f - 2.0f * ImGui::GetFrameHeightWithSpacing();
        available_screen_space_in_pixels.y = io.DisplaySize.y - 2.0f * ImGui::GetFrameHeightWithSpacing();
        ImGui::SetNextWindowSize(available_screen_space_in_pixels, ImGuiCond_FirstUseEver);

        // RENDER THE STATISTICS IN THE WINDOW.
        if (ImGui::Begin(window_title_and_id.c_str(), &Open))
        {
            std::vector<std::size_t> word_counts_for_each_book;
            std::vector<std::string> book_names;
            book_names.reserve(VersesByBook.size());
            std::vector<const char*> book_labels;
            std::vector<std::size_t> book_ids;
            for (const auto& [book_id, verses_in_book] : VersesByBook)
            {
                word_counts_for_each_book.push_back(verses_in_book.size());

                book_ids.push_back(static_cast<std::size_t>(book_id));
                book_names.push_back(BIBLE_DATA::BibleBook::FullName(book_id));
                book_labels.push_back(book_names.back().c_str());
            }

            std::vector<double> book_id_ticks;
            for (const std::size_t book_id : book_ids)
            {
                book_id_ticks.push_back(static_cast<double>(book_id));
            }

            std::vector<std::size_t> word_count_matrix;
            for (std::size_t row_index = 0; row_index < book_labels.size(); ++row_index)
            {
                for (std::size_t column_index = 0; column_index < book_labels.size(); ++column_index)
                {
                    bool same_row_column = (row_index == column_index);
                    if (same_row_column)
                    {
                        word_count_matrix.push_back(word_counts_for_each_book[row_index]);
                    }
                    else
                    {
                        word_count_matrix.push_back(0);
                    }
                }
            }

            if (ImPlot::BeginPlot("Statistics"))
            {
#if 0
                ImPlot::SetupAxisTicks(
                    ImAxis_Y1,
                    book_id_ticks.data(),
                    book_id_ticks.size(),
                    book_labels.data());
                ImPlot::SetupFinish();
#endif

                ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);

                ImPlot::PlotBars(
                    "Word Counts By Book",
                    word_counts_for_each_book.data(), 
                    book_ids.data(),
                    book_ids.size(),
                    8.0, 
                    ImPlotBarsFlags_Horizontal);

                ImPlot::EndPlot();
            }

#if 0
            if (ImPlot::BeginPlot("Plot Bars 1"))
            {
                constexpr std::size_t VALUE_COUNT = 10;
                const std::size_t values[VALUE_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                ImPlot::PlotBars(
                    "Plot Bars 1",
                    values,
                    VALUE_COUNT);

                ImPlot::EndPlot();
            }

            if (ImPlot::BeginPlot("Plot Bars 2 Vertical"))
            {
                constexpr std::size_t VALUE_COUNT = 10;
                const std::size_t x_values[VALUE_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                const std::size_t y_values[VALUE_COUNT] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
                constexpr double BAR_SIZE = 1.0f;
                ImPlot::PlotBars(
                    "Plot Bars 2",
                    x_values,
                    y_values,
                    VALUE_COUNT,
                    BAR_SIZE);

                ImPlot::EndPlot();
            }

            if (ImPlot::BeginPlot("Plot Bars 2 Horizontal"))
            {
                constexpr std::size_t VALUE_COUNT = 10;
                const std::size_t x_values[VALUE_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                constexpr double BAR_SIZE = 1.0f;
                const std::size_t y_values[VALUE_COUNT] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
                ImPlot::PlotBars(
                    "Plot Bars 2 Horizontal",
                    x_values,
                    y_values,
                    VALUE_COUNT,
                    BAR_SIZE,
                    ImPlotBarsFlags_Horizontal);

                ImPlot::EndPlot();
            }

            if (ImPlot::BeginPlot("Plot Bars 2 Offset"))
            {
                constexpr std::size_t VALUE_COUNT = 10;
                const std::size_t x_values[VALUE_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                const std::size_t y_values[VALUE_COUNT] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
                constexpr double BAR_SIZE = 1.0f;
                constexpr int OFFSET = 1;
                ImPlot::PlotBars(
                    "Plot Bars 2 Offset",
                    x_values,
                    y_values,
                    VALUE_COUNT,
                    BAR_SIZE,
                    ImPlotBarsFlags_Horizontal,
                    OFFSET);

                ImPlot::EndPlot();
            }

            if (ImPlot::BeginPlot("Plot Bars 2 Stride"))
            {
                constexpr std::size_t VALUE_COUNT = 10;
                const std::size_t x_values[VALUE_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                const std::size_t y_values[VALUE_COUNT] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
                constexpr double BAR_SIZE = 1.0f;
                constexpr int OFFSET = 1;
                constexpr int STRIDE = 1;
                ImPlot::PlotBars(
                    "Plot Bars 2 Stride",
                    x_values,
                    y_values,
                    VALUE_COUNT,
                    BAR_SIZE,
                    ImPlotBarsFlags_Horizontal,
                    OFFSET,
                    STRIDE);

                ImPlot::EndPlot();
            }
#endif

            if (ImPlot::BeginPlot("Plot Bar Groups"))
            {
                ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);

                constexpr double GROUP_SIZE = 0.67;
                constexpr double SHIFT = 0.0;
                ImPlot::PlotBarGroups(
                    book_labels.data(),
                    word_count_matrix.data(),
                    word_counts_for_each_book.size(),
                    word_counts_for_each_book.size(),
                    GROUP_SIZE,
                    SHIFT,
                    ImPlotBarGroupsFlags_Horizontal);

                ImPlot::EndPlot();
            }

#if 1
            if (ImPlot::BeginPlot("Pie Chart"))
            {
                ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);

                ImPlot::PlotPieChart(
                    book_labels.data(),
                    word_counts_for_each_book.data(),
                    word_counts_for_each_book.size(),
                    0.5,
                    0.5,
                    0.4);

                ImPlot::EndPlot();
            }
#endif

#if 0
            if (ImPlot::BeginPlot("Heatmap"))
            {
                ImPlot::PlotHeatmap(
                    "Heatmap",
                    word_count_matrix.data(),
                    word_counts_for_each_book.size(),
                    word_counts_for_each_book.size());

                ImPlot::EndPlot();
            }
#endif
        }

        // END RENDERING THE WINDOW.
        ImGui::End();
    }
}