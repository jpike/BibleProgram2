#pragma once

#include <string>
#include <vector>
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// Bible verse search results categorized based on proximity to some original verse.
    struct CategorizedBibleVerseSearchResults
    {
        /// The word searched for to produce the results.
        std::string SearchedWord = "";
        /// The ID of the original Bible verse from which results were searched for.
        BibleVerseId OriginalVerseId = {};
        /// Verses in the same chapter as the original verse that contain the searched word.
        std::vector<BibleVerse> VersesInSameChapter = {};
        /// Verses in the same book (but not same chapter) as the original verse that contain the searched word.
        std::vector<BibleVerse> VersesInSameBook = {};
        /// Verses in the other books by the same author as the original verse that contain the searched word.
        std::vector<BibleVerse> VersesInOtherBooksByAuthor = {};
        /// Verses other than the original that contain the searched word that meet all of the following criteria:
        /// - In other books
        /// - In books not by the same author
        /// - In books of the same genre (letter, etc.)
        /// - In books of the same testament
        std::vector<BibleVerse> VersesInBooksOfSameGenreByOtherAuthorsInSameTestament = {};
        /// Verses other than the original that contain the searched word from the same testament
        /// that do not fit into one of the more specific categories above.
        std::vector<BibleVerse> VersesElsewhereInSameTestament = {};
        /// Verses other than the original that contain the searched word from the other testament.
        std::vector<BibleVerse> VersesFromOtherTestament = {};
    };
}
