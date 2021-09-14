#pragma once

#include <string>
#include <vector>
#include "BibleData/BibleVerse.h"
#include "Gui/UserSettings.h"

namespace GUI
{
    /// A basic panel that can display text for multiple Bible verses.
    /// This class encapsulates all of the special rendering for highlighting, colorizing,
    /// wrapping, selecting, etc. for individual words in a verse (something ImGui does not
    /// do by default).
    class BibleVersesTextPanel
    {
    public:
        static void UpdateAndRender(
            const std::vector<BIBLE_DATA::BibleVerse>& verses, 
            std::string& currently_highlighted_word,
            std::string& currently_selected_word,
            UserSettings& user_settings);
    };
}
