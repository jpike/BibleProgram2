#include "BibleData/BibleBookGenre.h"

namespace BIBLE_DATA
{
    /// Gets the genre for the specific verse.
    /// @param[in]  verse_id - The ID for which to get the genre.
    /// @return The genre for the specific verse.
    BibleBookGenre::Id BibleBookGenre::Get(const BibleVerseId& verse_id)
    {
        // GET THE GENRE BASED ON THE BOOK.
        switch (verse_id.Book)
        {
            case BibleBook::GENESIS:
            case BibleBook::EXODUS:
            case BibleBook::LEVITICUS:
            case BibleBook::NUMBERS:
            case BibleBook::DEUTERONOMY:
                return BibleBookGenre::TORAH_PENTATEUCH;
            case BibleBook::JOSHUA:
            case BibleBook::JUDGES:
            case BibleBook::RUTH:
            case BibleBook::FIRST_SAMUEL:
            case BibleBook::SECOND_SAMUEL:
            case BibleBook::FIRST_KINGS:
            case BibleBook::SECOND_KINGS:
            case BibleBook::FIRST_CHRONICLES:
            case BibleBook::SECOND_CHRONICLES:
            case BibleBook::EZRA:
            case BibleBook::NEHEMIAH:
            case BibleBook::ESTHER:
                return BibleBookGenre::HISTORY;
            case BibleBook::JOB:
            case BibleBook::PSALMS:
            case BibleBook::PROVERBS:
            case BibleBook::ECCLESIASTES:
            case BibleBook::SONG_OF_SOLOMON:
                return BibleBookGenre::WISDOM;
            case BibleBook::ISAIAH:
            case BibleBook::JEREMIAH:
            case BibleBook::LAMENTATIONS:
            case BibleBook::EZEKIEL:
            case BibleBook::DANIEL:
            case BibleBook::HOSEA:
            case BibleBook::JOEL:
            case BibleBook::AMOS:
            case BibleBook::OBADIAH:
            case BibleBook::JONAH:
            case BibleBook::MICAH:
            case BibleBook::NAHUM:
            case BibleBook::HABAKKUK:
            case BibleBook::ZEPHANIAH:
            case BibleBook::HAGGAI:
            case BibleBook::ZECHARIAH:
            case BibleBook::MALACHI:
                return BibleBookGenre::PROPHECY;
            case BibleBook::MATTHEW:
            case BibleBook::MARK:
            case BibleBook::LUKE:
            case BibleBook::JOHN:
                return BibleBookGenre::GOSPEL;
            case BibleBook::ACTS:
                return BibleBookGenre::HISTORY;
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
            case BibleBook::HEBREWS:
            case BibleBook::JAMES:
            case BibleBook::FIRST_PETER:
            case BibleBook::SECOND_PETER:
            case BibleBook::FIRST_JOHN:
            case BibleBook::SECOND_JOHN:
            case BibleBook::THIRD_JOHN:
            case BibleBook::JUDE:
                return BibleBookGenre::EPISTLE;
            case BibleBook::REVELATION:
                return BibleBookGenre::PROPHECY;
            default:
                return BibleBookGenre::INVALID;
        }
    }
}
