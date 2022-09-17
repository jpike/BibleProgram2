#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "BibleData/BibleVerseId.h"
#include "BibleData/CategorizedBibleVerseSearchResults.h"
#include "Gui/UserSettings.h"

namespace GUI
{
    /// A window that displays verses that contain a given word
    /// in categorized form (based on an original word).
    class CategorizedBibleVersesWithWordWindow
    {
    public:
        void UpdateAndRender(
            std::string& currently_highlighted_word,
            std::string& currently_selected_word,
            BIBLE_DATA::BibleVerseId& currently_selected_verse_id,
            UserSettings& user_settings);

        /// True if the window is open; false otherwise.
        bool Open = false;
        /// The word whose verses displayed to display in the window.
        std::string Word = "";
        /// The current verses being displayed.
        /// @todo   std::vector<BIBLE_DATA::BibleVerse> Verses = {};
        std::unordered_map<std::string, BIBLE_DATA::CategorizedBibleVerseSearchResults> VersesByTranslationName = {};
    };
}
