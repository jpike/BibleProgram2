#pragma once

#include <string>
#include <unordered_map>
#include "BibleData/Bibles.h"
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseId.h"
#include "BibleData/BibleVerseRange.h"
#include "Gui/UserSelections.h"
#include "Gui/UserSettings.h"

namespace GUI
{
    /// A window for displaying the main verse or range of verses currently being studied.
    class MainBibleVersesWindow
    {
    public:
        // PUBLIC METHODS.
        void OpenForVerses(
            const BIBLE_DATA::BibleVerseRange& verse_range, 
            const BIBLE_DATA::Bibles& bibles,
            const UserSettings& user_settings);
        void UpdateAndRender(UserSelections& user_selections, UserSettings& user_settings);

    private:
        // MEMBER VARIABLES.
        /// True if the window is open; false otherwise.
        bool Open = false;
        /// The range of verses displayed in the window.
        BIBLE_DATA::BibleVerseRange VerseRange = {};
        /// The current verses being displayed.
        std::unordered_map<std::string, std::vector<BIBLE_DATA::BibleVerse>> VersesByTranslationName = {};
    };
}
