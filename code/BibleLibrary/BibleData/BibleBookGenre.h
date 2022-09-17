#pragma once

#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// A genre ofa book of the Bible.
    /// Essentially an enum but with methods.
    /// Since regular scoped enum classes cannot have methods,
    /// this wrapper class is used.
    class BibleBookGenre
    {
    public:
        // ENUM VALUES.
        /// The underlying ID of the genre.
        enum Id
        {
            INVALID = 0,
            TORAH_PENTATEUCH,
            HISTORY,
            WISDOM,
            MOSES,
            PROPHECY,
            GOSPEL,
            EPISTLE,
        };

        // METHODS.
        static BibleBookGenre::Id Get(const BibleVerseId& verse_id);
    };
}
