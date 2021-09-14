#pragma once

#include <cstddef>
#include "BibleData/BibleBook.h"
#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// A Bible verse ID with an associated range of character offsets into the raw Bible text.
    struct BibleVerseIdWithCharacterRange
    {
        /// The ID of the verse.
        BibleVerseId Id = {};
        /// The offset into the full Bible text of the first character of this verse.
        std::size_t FirstCharacterOffsetIntoFullBibleText = 0;
        /// The offset into the full Bible text of the last character of this verse.
        std::size_t LastCharacterOffsetIntoFullBibleText = 0;
    };
}
