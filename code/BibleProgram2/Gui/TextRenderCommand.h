#pragma once

#include <string>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "BibleData/BibleVerseId.h"

namespace GUI
{
    /// A simple command for rendering some text.
    struct TextRenderCommand
    {
        /// The ID of the Bible verse associated with the command.
        /// @todo   Figure out a better place to put this?
        BIBLE_DATA::BibleVerseId BibleVerseId = {};
        /// The text to render.
        std::string Text = "";
        /// The color of the text.
        ImVec4 Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        /// The position for drawing the text.
        ImVec2 DrawPosition = ImVec2(0.0f, 0.0f);
        /// The dimensions of the text to draw.
        ImVec2 TextSize = ImVec2(0.0f, 0.0f);
        /// The bounding box of the text.
        ImRect TextBoundingBox = ImRect(0.0f, 0.0f, 0.0f, 0.0f);
    };
}
