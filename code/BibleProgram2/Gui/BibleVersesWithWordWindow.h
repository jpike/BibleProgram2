#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "BibleData/BibleVerse.h"
#include "Gui/UserSettings.h"

namespace GUI
{
    /// A window that displays verses that contain a given word.
    class BibleVersesWithWordWindow
    {
    public:
        void UpdateAndRender(
            std::string& currently_highlighted_word, 
            std::string& currently_selected_word,
            UserSettings& user_settings);

        /// True if the window is open; false otherwise.
        bool Open = false;
        /// The word whose verses displayed to display in the window.
        std::string Word = "";
        /// The current verses being displayed.
        /// @todo   std::vector<BIBLE_DATA::BibleVerse> Verses = {};
        std::unordered_map<std::string, std::vector<BIBLE_DATA::BibleVerse>> VersesByTranslationName = {};
    };
}
