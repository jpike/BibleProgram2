#pragma once

#include <string>
#include <vector>
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseId.h"
#include "Gui/UserSelections.h"
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
            UserSelections& user_selections,
            UserSettings& user_settings);
    };
}
