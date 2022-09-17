#include "BibleData/BibleAuthor.h"

namespace BIBLE_DATA
{
    /// Gets the author for the specific verse.
    /// @param[in]  verse_id - The ID for which to get the author.
    /// @return The author for the specific verse.
    BibleAuthor::Id BibleAuthor::Get(const BibleVerseId& verse_id)
    {
        // GET THE AUTHOR BASED ON THE BOOK.
        switch (verse_id.Book)
        {
            case BibleBook::GENESIS:
            case BibleBook::EXODUS:
            case BibleBook::LEVITICUS:
            case BibleBook::NUMBERS:
            case BibleBook::DEUTERONOMY:
                return BibleAuthor::MOSES;
            case BibleBook::JOSHUA:
                return BibleAuthor::JOSHUA_AUTHOR;
            case BibleBook::JUDGES:
                return BibleAuthor::JUDGES_AUTHOR;
            case BibleBook::RUTH:
                return BibleAuthor::RUTH_AUTHOR;
            case BibleBook::FIRST_SAMUEL:
            case BibleBook::SECOND_SAMUEL:
                return BibleAuthor::SAMUEL_AUTHOR;
            case BibleBook::FIRST_KINGS:
            case BibleBook::SECOND_KINGS:
                return BibleAuthor::KINGS_AUTHOR;
            case BibleBook::FIRST_CHRONICLES:
            case BibleBook::SECOND_CHRONICLES:
                return BibleAuthor::CHRONICLES_AUTHOR;
            case BibleBook::EZRA:
                return BibleAuthor::EZRA;
            case BibleBook::NEHEMIAH:
                return BibleAuthor::NEHEMIAH;
            case BibleBook::ESTHER:
                return BibleAuthor::ESTHER_AUTHOR;
            case BibleBook::JOB:
                return BibleAuthor::JOB;
            case BibleBook::PSALMS:
                /// @todo   Need to examine chapters!
                return BibleAuthor::DAVID;
            case BibleBook::PROVERBS:
                /// @todo   Need to examine verse ID more closely!
                return BibleAuthor::SOLOMON;
            case BibleBook::ECCLESIASTES:
            case BibleBook::SONG_OF_SOLOMON:
                return BibleAuthor::SOLOMON;
            case BibleBook::ISAIAH:
                return BibleAuthor::ISAIAH;
            case BibleBook::JEREMIAH:
            case BibleBook::LAMENTATIONS:
                return BibleAuthor::JEREMIAH;
            case BibleBook::EZEKIEL:
                return BibleAuthor::EZEKIEL;
            case BibleBook::DANIEL:
                return BibleAuthor::DANIEL;
            case BibleBook::HOSEA:
                return BibleAuthor::HOSEA;
            case BibleBook::JOEL:
                return BibleAuthor::JOEL;
            case BibleBook::AMOS:
                return BibleAuthor::AMOS;
            case BibleBook::OBADIAH:
                return BibleAuthor::OBADIAH;
            case BibleBook::JONAH:
                return BibleAuthor::JONAH;
            case BibleBook::MICAH:
                return BibleAuthor::MICAH;
            case BibleBook::NAHUM:
                return BibleAuthor::NAHUM;
            case BibleBook::HABAKKUK:
                return BibleAuthor::HABAKKUK;
            case BibleBook::ZEPHANIAH:
                return BibleAuthor::ZEPHANIAH;
            case BibleBook::HAGGAI:
                return BibleAuthor::HAGGAI;
            case BibleBook::ZECHARIAH:
                return BibleAuthor::ZECHARIAH;
            case BibleBook::MALACHI:
                return BibleAuthor::MALACHI;
            case BibleBook::MATTHEW:
                return BibleAuthor::MATTHEW;
            case BibleBook::MARK:
                return BibleAuthor::MARK;
            case BibleBook::LUKE:
            case BibleBook::ACTS:
                return BibleAuthor::LUKE;
            case BibleBook::JOHN:
            case BibleBook::FIRST_JOHN:
            case BibleBook::SECOND_JOHN:
            case BibleBook::THIRD_JOHN:
            case BibleBook::REVELATION:
                return BibleAuthor::JOHN;
            case BibleBook::ROMANS:
            case BibleBook::FIRST_CORINTHIANS:
            case BibleBook::SECOND_CORINTHIANS:
            case BibleBook::GALATIANS:
            case BibleBook::EPHESIANS:
            case BibleBook::PHILIPPIANS:
            case BibleBook::COLOSSIANS:
            case BibleBook::FIRST_THESSALONIANS:
            case BibleBook::SECOND_THESSALONIANS:
            case BibleBook::FIRST_TIMOTHY:
            case BibleBook::SECOND_TIMOTHY:
            case BibleBook::TITUS:
            case BibleBook::PHILEMON:
                return BibleAuthor::PAUL;
            case BibleBook::HEBREWS:
                return BibleAuthor::HEBREWS_AUTHOR;
            case BibleBook::JAMES:
                return BibleAuthor::JAMES;
            case BibleBook::FIRST_PETER:
            case BibleBook::SECOND_PETER:
                return BibleAuthor::PETER;
            case BibleBook::JUDE:
                return BibleAuthor::JUDE;
            default:
                return BibleAuthor::INVALID;
        }
    }
}
