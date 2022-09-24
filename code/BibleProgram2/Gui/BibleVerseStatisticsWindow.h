#pragma once

#include <string>
#include <map>
#include <vector>
#include "BibleData/BibleBook.h"
#include "BibleData/BibleVerse.h"

namespace GUI
{
    /// A window for displaying statistics about Bible verse.
    /// The exact statistics are still to-be-determined,
    /// so this window could heavily change.
    class BibleVerseStatisticsWindow
    {
    public:
        void UpdateAndRender();

        /// True if the window is open; false if not.
        bool Open = false;
        /// The word for which verse statistics are being displayed.
        std::string Word = "";
        /// Bible verses by book.
        std::map<BIBLE_DATA::BibleBook::Id, std::vector<BIBLE_DATA::BibleVerse>> VersesByBook = {};
    };
}
