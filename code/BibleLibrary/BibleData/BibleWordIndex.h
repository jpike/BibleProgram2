#pragma once

#include <map>
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
        /// Root words mapped by original word.
        /// Used for more useful lookups of related verses for similar words.
        /// The data here has been hardcoded based on observation.
        static std::unordered_map<std::string, std::string> RootWordsByOriginalWord;

        static BibleWordIndex Build(const BibleTranslation* bible_translation);
        std::vector<BibleVerse> GetMatchingVerses(const std::string_view word) const;

        /// The Bible translation this index corresponds to.
        const BibleTranslation* Bible = nullptr;
        /// A mapping of Bible verses by lowercase word.
        /// @todo std::unordered_map<std::string, std::vector<BibleVerseIdWithCharacterRange>> BibleVersesByWord = {};
        std::map<std::string, std::vector<BibleVerseIdWithCharacterRange>> BibleVersesByWord = {};
    };
}
