#pragma once

#include <string>
#include <vector>
#include "BibleData/BibleVerseIdWithCharacterRange.h"
#include "BibleData/BibleWordIndex.h"

namespace BIBLE_DATA
{
    /// A single translation of the Bible.
    class BibleTranslation
    {
    public:
        // STATIC CONSTANTS.
        /// The abbreviated name for the KJV (King James Version) translation.
        inline static const std::string KJV_ABBREVIATED_NAME = "KJV";
        /// The abbreviated name for the WEB (World English Bible) translation.
        inline static const std::string WEB_ABBREVIATED_NAME = "WEB";

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The raw, full text of the Bible translation (in order).
        const char* Text = nullptr;
        /// Offsets identifying the ranges of characters in the full text for individual verses.
        std::vector<BibleVerseIdWithCharacterRange> VerseCharacterRanges = {};
        /// An index of words in the Bible translation.
        BibleWordIndex WordIndex = {};
    };
}
