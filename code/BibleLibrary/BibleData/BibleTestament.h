#pragma once

#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// A testament of part of the Bible.
    /// Essentially an enum but with methods.
    /// Since regular scoped enum classes cannot have methods,
    /// this wrapper class is used.
    class BibleTestament
    {
    public:
        // ENUM VALUES.
        enum Id
        {
            INVALID = 0,
            OLD,
            NEW
        };

        // METHODS.
        static BibleTestament::Id Get(const BibleVerseId& verse_id);
    };
}
