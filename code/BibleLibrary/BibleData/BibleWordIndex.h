#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "BibleData/BibleVerse.h"
#include "BibleData/BibleVerseIdWithCharacterRange.h"

namespace BIBLE_DATA
{
    /// @todo   Get rid of forward declaration?
    class BibleTranslation;

    /// An index of words in a Bible translation for easier looking up of verses by word.
    /// Indexing is done based on lowercasing each word.
    class BibleWordIndex
    {
    public:
        static BibleWordIndex Build(const BibleTranslation* bible_translation);
        std::vector<BibleVerse> GetMatchingVerses(const std::string_view word) const;

        /// The Bible translation this index corresponds to.
        const BibleTranslation* Bible = nullptr;
        /// A mapping of Bible verses by lowercase word.
        std::unordered_map<std::string, std::vector<BibleVerseIdWithCharacterRange>> BibleVersesByWord = {};
    };
}
