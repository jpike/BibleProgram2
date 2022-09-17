#pragma once

#include <string>
#include <unordered_map>
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseId.h"
#include "BibleData/BibleVerseRange.h"
#include "Gui/UserSettings.h"

namespace GUI
{
    /// A window for displaying the main verse or range of verses currently being studied.
    class MainBibleVersesWindow
    {
    public:
        void UpdateAndRender(
            std::string& currently_highlighted_word, 
            std::string& currently_selected_word,
            BIBLE_DATA::BibleVerseId& currently_selected_verse_id,
            UserSettings& user_settings);

        /// True if the window is open; false otherwise.
        bool Open = false;
        /// The range of verses displayed in the window.
        BIBLE_DATA::BibleVerseRange VerseRange = {};
        /// The current verses being displayed.
        std::unordered_map<std::string, std::vector<BIBLE_DATA::BibleVerse>> VersesByTranslationName = {};
    };
}
