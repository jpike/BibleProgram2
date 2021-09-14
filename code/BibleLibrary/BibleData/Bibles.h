#pragma once

#include <string>
#include <unordered_map>
#include "BibleData/BibleTranslation.h"

namespace BIBLE_DATA
{
    /// A collection of Bibles.
    /// This class provides a way to aggregate data for multiple Bible translations in a single class.
    class Bibles
    {
    public:
        /// Bible translations mapped by abbreviated name.
        std::unordered_map<std::string, BibleTranslation> TranslationsByAbbreviatedName = {};
    };
}
