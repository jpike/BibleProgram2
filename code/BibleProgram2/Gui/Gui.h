#pragma once

#include <optional>
#include "BibleData/Bibles.h"
#include "Graphics/OpenGL/OpenGLGraphicsDevice.h"
#include "Gui/BibleVersesWithWordWindow.h"
#include "Gui/Debugging/AboutWindow.h"
#include "Gui/Debugging/DemoWindow.h"
#include "Gui/Debugging/GuiMetricsWindow.h"
#include "Gui/Debugging/StyleEditorWindow.h"
#include "Gui/MainBibleVersesWindow.h"
#include "Gui/UserSettings.h"
#include "Windowing/SdlWindow.h"

/// Holds GUI code for the program that uses a WIMP paradigm (https://en.wikipedia.org/wiki/WIMP_(computing)).
namespace GUI
{
    /// The entire WIMP-based GUI for the program.
    class Gui
    {
    public:
        static std::optional<Gui> Create(const WINDOWING::SdlWindow& window, const GRAPHICS::OPEN_GL::OpenGLGraphicsDevice& graphics_device);

        void UpdateAndRender(
            const WINDOWING::SdlWindow& window, 
            const BIBLE_DATA::Bibles& bibles,
            UserSettings& user_settings);

        void Shutdown();

        /// The word currently highlighted in the GUI.
        std::string CurrentlyHighlightedWord = "";
        /// The window for displaying the main verse(s) being studied.
        MainBibleVersesWindow MainBibleVersesWindow = {};
        /// The window for other verses with the currently clicked-on word.
        BibleVersesWithWordWindow BibleVersesWithWordWindow = {};
        /// A window displaying basic GUI metrics.
        DEBUGGING::GuiMetricsWindow MetricsWindow = {};
        /// A window allowing editing of GUI styles.
        DEBUGGING::StyleEditorWindow StyleEditorWindow = {};
        /// A demo window for ImGui features.
        DEBUGGING::DemoWindow DemoWindow = {};
        /// A basic "about" window for the GUI.
        DEBUGGING::AboutWindow AboutWindow = {};
    };
}
