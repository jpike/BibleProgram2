#pragma once

#include <string>
#include <map>
#include <vector>

namespace BIBLE_DATA
{
    /// A single book in the Bible.
    class BibleBook
    {
    public:
        // ENUMERATIONS.
        /// Unique IDs for identifying each book of the Bible in this program.
        /// Having this enum helps separate identification of books in this program
        /// from various string representations (full names, abbreviations, etc.).
        enum Id
        {
            INVALID = 0,
            GENESIS,
            EXODUS,
            LEVITICUS,
            NUMBERS,
            DEUTERONOMY,
            JOSHUA,
            JUDGES,
            RUTH,
            FIRST_SAMUEL,
            SECOND_SAMUEL,
            FIRST_KINGS,
            SECOND_KINGS,
            FIRST_CHRONICLES,
            SECOND_CHRONICLES,
            EZRA,
            NEHEMIAH,
            ESTHER,
            JOB,
            PSALMS,
            PROVERBS,
            ECCLESIASTES,
            SONG_OF_SOLOMON,
            ISAIAH,
            JEREMIAH,
            LAMENTATIONS,
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
            ACTS,
            ROMANS,
            FIRST_CORINTHIANS,
            SECOND_CORINTHIANS,
            GALATIANS,
            EPHESIANS,
            PHILIPPIANS,
            COLOSSIANS,
            FIRST_THESSALONIANS,
            SECOND_THESSALONIANS,
            FIRST_TIMOTHY,
            SECOND_TIMOTHY,
            TITUS,
            PHILEMON,
            HEBREWS,
            JAMES,
            FIRST_PETER,
            SECOND_PETER,
            FIRST_JOHN,
            SECOND_JOHN,
            THIRD_JOHN,
            JUDE,
            REVELATION
        };

        // STATIC METHODS.
        static BibleBook::Id GetId(const std::string& book_name_or_abbreviation);
        static std::string FullName(const BibleBook::Id book_id);

        // CONSTRUCTION.
        explicit BibleBook(const std::vector<unsigned int>& verse_counts_by_chapter);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The number of verses per chapter.  Index 0 corresponds to chapter 1, etc.
        std::vector<unsigned int> VerseCountsByChapter = {};
    };

    /// The books in the Bible, mapped by their IDs.
    /// This only holds the high-level organizational/hierarchical metadata,
    /// rather than the contents of each book.
    /// This defines the overall organizational structure, rather than content, of the Bible.
    /// An ordered map is used for automatic sorting.
    extern const std::map<BibleBook::Id, BibleBook> BOOKS_BY_ID;
}
