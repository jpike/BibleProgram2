#pragma once

#include <string_view>
#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// A single Bible verse.
    class BibleVerse
    {
    public:
        /// The ID of the verse.
        BibleVerseId Id = {};
        /// The text of the verse.
        std::string_view Text = "";
    };
}
