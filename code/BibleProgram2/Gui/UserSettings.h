#pragma once

#include <map>
#include <string>
#include <imgui/imgui.h>

namespace GUI
{
    /// Various user-specific settings for use across the GUI.
    class UserSettings
    {
    public:
        /// Bible translation display statuses (true = displayed; false = not displayed) in the window.
        std::map<std::string, bool> BibleTranslationDisplayStatusesByName = {};
        /// The colors associated with each word in the Bible.  Populated on-demand as needed.
        std::map<std::string, ImVec4> ColorsByWord = {};
    };
}
