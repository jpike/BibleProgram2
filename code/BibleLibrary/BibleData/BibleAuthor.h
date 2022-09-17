#pragma once

#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// A human author of part of the Bible.
    /// Essentially an enum but with methods.
    /// Since regular scoped enum classes cannot have methods,
    /// this wrapper class is used.
    class BibleAuthor
    {
    public:
        // ENUM VALUES.
        /// The underlying ID of the Biblical author.
        /// Note that some of these IDs are just "best guesses".
        /// @todo   Further verify some authorship as best as possible.
        enum Id
        {
            INVALID = 0,
            MOSES,
            JOSHUA_AUTHOR,
            JUDGES_AUTHOR,
            RUTH_AUTHOR,
            SAMUEL_AUTHOR,
            KINGS_AUTHOR,
            CHRONICLES_AUTHOR,
            EZRA,
            NEHEMIAH,
            ESTHER_AUTHOR,
            JOB,
            DAVID, /// @todo more of Psalms!
            SOLOMON, /// @todo more of Proverbs!
            ISAIAH,
            JEREMIAH,
            EZEKIEL,
            DANIEL,
            HOSEA,
            JOEL,
            AMOS,
            OBADIAH,
            JONAH,
            MICAH,
            NAHUM,
            HABAKKUK,
            ZEPHANIAH,
            HAGGAI,
            ZECHARIAH,
            MALACHI,
            MATTHEW,
            MARK,
            LUKE,
            JOHN,
            PAUL,
            HEBREWS_AUTHOR,
            JAMES,
            PETER,
            JUDE
        };

        // METHODS.
        static BibleAuthor::Id Get(const BibleVerseId& verse_id);
    };
}
