#include "BibleData/BibleBook.h"

namespace BIBLE_DATA
{
    /// Gets the book ID for the given string name or abbreviation.
    /// This method exists to centralize converting various string representations of books
    /// to a consistent standard representation for use within this program.
    /// @param[in]  book_name_or_abbrevation - The book name or abbreviation to get the ID for.
    /// @return The book ID for the provided string representation of the book; may be INVALID
    ///     if the string book representation isn't recognized.
    BibleBook::Id BibleBook::GetId(const std::string& book_name_or_abbreviation)
    {
        // This code only looks for strings that have been needed to be recognized so far.
        // It may need to be updated if new data sources are supported.
        if (
            "Gen" == book_name_or_abbreviation ||
            "Genesis" == book_name_or_abbreviation)
        {
            return BibleBook::Id::GENESIS;
        }
        else if (
            "Exo" == book_name_or_abbreviation ||
            "Exod" == book_name_or_abbreviation ||
            "Exodus" == book_name_or_abbreviation)
        {
            return BibleBook::Id::EXODUS;
        }
        else if (
            "Lev" == book_name_or_abbreviation ||
            "Leviticus" == book_name_or_abbreviation)
        {
            return BibleBook::Id::LEVITICUS;
        }
        else if (
            "Num" == book_name_or_abbreviation ||
            "Numbers" == book_name_or_abbreviation)
        {
            return BibleBook::Id::NUMBERS;
        }
        else if (
            "Deu" == book_name_or_abbreviation ||
            "Deut" == book_name_or_abbreviation ||
            "Deuteronomy" == book_name_or_abbreviation)
        {
            return BibleBook::Id::DEUTERONOMY;
        }
        else if (
            "Jos" == book_name_or_abbreviation ||
            "Josh" == book_name_or_abbreviation ||
            "Joshua" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JOSHUA;
        }
        else if (
            "Jdg" == book_name_or_abbreviation ||
            "Judg" == book_name_or_abbreviation ||
            "Judges" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JUDGES;
        }
        else if (
            "Rut" == book_name_or_abbreviation ||
            "Ruth" == book_name_or_abbreviation)
        {
            return BibleBook::Id::RUTH;
        }
        else if (
            "Sa1" == book_name_or_abbreviation ||
            "1Sam" == book_name_or_abbreviation ||
            "1 Samuel" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_SAMUEL;
        }
        else if (
            "Sa2" == book_name_or_abbreviation ||
            "2Sam" == book_name_or_abbreviation ||
            "2 Samuel" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_SAMUEL;
        }
        else if (
            "Kg1" == book_name_or_abbreviation ||
            "1Kgs" == book_name_or_abbreviation ||
            "1 Kings" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_KINGS;
        }
        else if (
            "Kg2" == book_name_or_abbreviation ||
            "2Kgs" == book_name_or_abbreviation ||
            "2 Kings" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_KINGS;
        }
        else if (
            "Ch1" == book_name_or_abbreviation ||
            "1Chr" == book_name_or_abbreviation ||
            "1 Chronicles" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_CHRONICLES;
        }
        else if (
            "Ch2" == book_name_or_abbreviation ||
            "2Chr" == book_name_or_abbreviation ||
            "2 Chronicles" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_CHRONICLES;
        }
        else if (
            "Ezr" == book_name_or_abbreviation ||
            "Ezra" == book_name_or_abbreviation)
        {
            return BibleBook::Id::EZRA;
        }
        else if (
            "Neh" == book_name_or_abbreviation ||
            "Nehemiah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::NEHEMIAH;
        }
        else if (
            "Est" == book_name_or_abbreviation ||
            "Esth" == book_name_or_abbreviation ||
            "Esther" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ESTHER;
        }
        else if ("Job" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JOB;
        }
        else if (
            "Psa" == book_name_or_abbreviation ||
            "Ps" == book_name_or_abbreviation ||
            "Psalm" == book_name_or_abbreviation ||
            "Psalms" == book_name_or_abbreviation)
        {
            return BibleBook::Id::PSALMS;
        }
        else if (
            "Pro" == book_name_or_abbreviation ||
            "Prov" == book_name_or_abbreviation ||
            "Proverbs" == book_name_or_abbreviation)
        {
            return BibleBook::Id::PROVERBS;
        }
        else if (
            "Ecc" == book_name_or_abbreviation ||
            "Eccl" == book_name_or_abbreviation ||
            "Ecclesiates" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ECCLESIASTES;
        }
        else if (
            "Sol" == book_name_or_abbreviation ||
            "Song" == book_name_or_abbreviation ||
            "Song of Songs" == book_name_or_abbreviation ||
            "Song of Solomon" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SONG_OF_SOLOMON;
        }
        else if (
            "Isa" == book_name_or_abbreviation ||
            "Isaiah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ISAIAH;
        }
        else if (
            "Jer" == book_name_or_abbreviation ||
            "Jeremiah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JEREMIAH;
        }
        else if (
            "Lam" == book_name_or_abbreviation ||
            "Lamentations" == book_name_or_abbreviation)
        {
            return BibleBook::Id::LAMENTATIONS;
        }
        else if (
            "Eze" == book_name_or_abbreviation ||
            "Ezek" == book_name_or_abbreviation ||
            "Ezekiel" == book_name_or_abbreviation)
        {
            return BibleBook::Id::EZEKIEL;
        }
        else if (
            "Dan" == book_name_or_abbreviation ||
            "Daniel" == book_name_or_abbreviation)
        {
            return BibleBook::Id::DANIEL;
        }
        else if (
            "Hos" == book_name_or_abbreviation ||
            "Hosea" == book_name_or_abbreviation)
        {
            return BibleBook::Id::HOSEA;
        }
        else if (
            "Joe" == book_name_or_abbreviation ||
            "Joel" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JOEL;
        }
        else if (
            "Amo" == book_name_or_abbreviation ||
            "Amos" == book_name_or_abbreviation)
        {
            return BibleBook::Id::AMOS;
        }
        else if (
            "Oba" == book_name_or_abbreviation ||
            "Obad" == book_name_or_abbreviation ||
            "Obadiah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::OBADIAH;
        }
        else if (
            "Jon" == book_name_or_abbreviation ||
            "Jonah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JONAH;
        }
        else if (
            "Mic" == book_name_or_abbreviation ||
            "Micah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::MICAH;
        }
        else if (
            "Nah" == book_name_or_abbreviation ||
            "Nahum" == book_name_or_abbreviation)
        {
            return BibleBook::Id::NAHUM;
        }
        else if (
            "Hab" == book_name_or_abbreviation ||
            "Habakkuk" == book_name_or_abbreviation)
        {
            return BibleBook::Id::HABAKKUK;
        }
        else if (
            "Zep" == book_name_or_abbreviation ||
            "Zeph" == book_name_or_abbreviation ||
            "Zephaniah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ZEPHANIAH;
        }
        else if (
            "Hag" == book_name_or_abbreviation ||
            "Haggai" == book_name_or_abbreviation)
        {
            return BibleBook::Id::HAGGAI;
        }
        else if (
            "Zac" == book_name_or_abbreviation ||
            "Zech" == book_name_or_abbreviation ||
            "Zechariah" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ZECHARIAH;
        }
        else if (
            "Mal" == book_name_or_abbreviation ||
            "Malachi" == book_name_or_abbreviation)
        {
            return BibleBook::Id::MALACHI;
        }
        else if (
            "Mat" == book_name_or_abbreviation ||
            "Matt" == book_name_or_abbreviation ||
            "Matthew" == book_name_or_abbreviation)
        {
            return BibleBook::Id::MATTHEW;
        }
        else if (
            "Mar" == book_name_or_abbreviation ||
            "Mark" == book_name_or_abbreviation)
        {
            return BibleBook::Id::MARK;
        }
        else if (
            "Luk" == book_name_or_abbreviation ||
            "Luke" == book_name_or_abbreviation)
        {
            return BibleBook::Id::LUKE;
        }
        else if (
            "Joh" == book_name_or_abbreviation ||
            "John" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JOHN;
        }
        else if (
            "Act" == book_name_or_abbreviation ||
            "Acts" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ACTS;
        }
        else if (
            "Rom" == book_name_or_abbreviation ||
            "Romans" == book_name_or_abbreviation)
        {
            return BibleBook::Id::ROMANS;
        }
        else if (
            "Co1" == book_name_or_abbreviation ||
            "1Cor" == book_name_or_abbreviation ||
            "1 Corinthians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_CORINTHIANS;
        }
        else if (
            "Co2" == book_name_or_abbreviation ||
            "2Cor" == book_name_or_abbreviation ||
            "2 Corinthians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_CORINTHIANS;
        }
        else if (
            "Gal" == book_name_or_abbreviation ||
            "Galatians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::GALATIANS;
        }
        else if (
            "Eph" == book_name_or_abbreviation ||
            "Ephesians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::EPHESIANS;
        }
        else if (
            "Phi" == book_name_or_abbreviation ||
            "Phil" == book_name_or_abbreviation ||
            "Philippians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::PHILIPPIANS;
        }
        else if (
            "Col" == book_name_or_abbreviation ||
            "Colossians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::COLOSSIANS;
        }
        else if (
            "Th1" == book_name_or_abbreviation ||
            "1Thess" == book_name_or_abbreviation ||
            "1 Thessalonians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_THESSALONIANS;
        }
        else if (
            "Th2" == book_name_or_abbreviation ||
            "2Thess" == book_name_or_abbreviation ||
            "2 Thessalonians" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_THESSALONIANS;
        }
        else if (
            "Ti1" == book_name_or_abbreviation ||
            "1Tim" == book_name_or_abbreviation ||
            "1 Timothy" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_TIMOTHY;
        }
        else if (
            "Ti2" == book_name_or_abbreviation ||
            "2Tim" == book_name_or_abbreviation ||
            "2 Timothy" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_TIMOTHY;
        }
        else if (
            "Tit" == book_name_or_abbreviation ||
            "Titus" == book_name_or_abbreviation)
        {
            return BibleBook::Id::TITUS;
        }
        else if (
            "Plm" == book_name_or_abbreviation ||
            "Phlm" == book_name_or_abbreviation ||
            "Philemon" == book_name_or_abbreviation)
        {
            return BibleBook::Id::PHILEMON;
        }
        else if (
            "Heb" == book_name_or_abbreviation ||
            "Hebrews" == book_name_or_abbreviation)
        {
            return BibleBook::Id::HEBREWS;
        }
        else if (
            "Jam" == book_name_or_abbreviation ||
            "Jas" == book_name_or_abbreviation ||
            "James" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JAMES;
        }
        else if (
            "Pe1" == book_name_or_abbreviation ||
            "1Pet" == book_name_or_abbreviation ||
            "1 Peter" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_PETER;
        }
        else if (
            "Pe2" == book_name_or_abbreviation ||
            "2Pet" == book_name_or_abbreviation ||
            "2 Peter" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_PETER;
        }
        else if (
            "Jo1" == book_name_or_abbreviation ||
            "1John" == book_name_or_abbreviation ||
            "1 John" == book_name_or_abbreviation)
        {
            return BibleBook::Id::FIRST_JOHN;
        }
        else if (
            "Jo2" == book_name_or_abbreviation ||
            "2John" == book_name_or_abbreviation ||
            "2 John" == book_name_or_abbreviation)
        {
            return BibleBook::Id::SECOND_JOHN;
        }
        else if (
            "Jo3" == book_name_or_abbreviation ||
            "3John" == book_name_or_abbreviation ||
            "3 John" == book_name_or_abbreviation)
        {
            return BibleBook::Id::THIRD_JOHN;
        }
        else if (
            "Jde" == book_name_or_abbreviation ||
            "Jude" == book_name_or_abbreviation)
        {
            return BibleBook::Id::JUDE;
        }
        else if (
            "Rev" == book_name_or_abbreviation ||
            "Revelation" == book_name_or_abbreviation)
        {
            return BibleBook::Id::REVELATION;
        }
        else
        {
            // Unrecognized strings can't be mapped to a known ID.
            return BibleBook::Id::INVALID;
        }
    }

    /// Gets the full name of the book given its ID.
    /// @param[in]  book_id - The ID of the book whose name to get.
    /// @return The full name of the book with the given ID.
    std::string BibleBook::FullName(const BibleBook::Id book_id)
    {
        // GET THE FULL NAME CORRESPONDING TO THE BOOK ID.
        switch (book_id)
        {
            case BibleBook::Id::GENESIS:
                return "Genesis";
            case BibleBook::Id::EXODUS:
                return "Exodus";
            case BibleBook::Id::LEVITICUS:
                return "Leviticus";
            case BibleBook::Id::NUMBERS:
                return "Numbers";
            case BibleBook::Id::DEUTERONOMY:
                return "Deuteronomy";
            case BibleBook::Id::JOSHUA:
                return "Joshua";
            case BibleBook::Id::JUDGES:
                return "Judges";
            case BibleBook::Id::RUTH:
                return "Ruth";
            case BibleBook::Id::FIRST_SAMUEL:
                return "1 Samuel";
            case BibleBook::Id::SECOND_SAMUEL:
                return "2 Samuel";
            case BibleBook::Id::FIRST_KINGS:
                return "1 Kings";
            case BibleBook::Id::SECOND_KINGS:
                return "2 Kings";
            case BibleBook::Id::FIRST_CHRONICLES:
                return "1 Chronicles";
            case BibleBook::Id::SECOND_CHRONICLES:
                return "2 Chronicles";
            case BibleBook::Id::EZRA:
                return "Ezra";
            case BibleBook::Id::NEHEMIAH:
                return "Nehemiah";
            case BibleBook::Id::ESTHER:
                return "Esther";
            case BibleBook::Id::JOB:
                return "Job";
            case BibleBook::Id::PSALMS:
                return "Psalms";
            case BibleBook::Id::PROVERBS:
                return "Proverbs";
            case BibleBook::Id::ECCLESIASTES:
                return "Ecclesiastes";
            case BibleBook::Id::SONG_OF_SOLOMON:
                return "Song of Solomon";
            case BibleBook::Id::ISAIAH:
                return "Isaiah";
            case BibleBook::Id::JEREMIAH:
                return "Jeremiah";
            case BibleBook::Id::LAMENTATIONS:
                return "Lamentations";
            case BibleBook::Id::EZEKIEL:
                return "Ezekiel";
            case BibleBook::Id::DANIEL:
                return "Daniel";
            case BibleBook::Id::HOSEA:
                return "Hosea";
            case BibleBook::Id::JOEL:
                return "Joel";
            case BibleBook::Id::AMOS:
                return "Amos";
            case BibleBook::Id::OBADIAH:
                return "Obadiah";
            case BibleBook::Id::JONAH:
                return "Jonah";
            case BibleBook::Id::MICAH:
                return "Micah";
            case BibleBook::Id::NAHUM:
                return "Nahum";
            case BibleBook::Id::HABAKKUK:
                return "Habakkuk";
            case BibleBook::Id::ZEPHANIAH:
                return "Zephaniah";
            case BibleBook::Id::HAGGAI:
                return "Haggai";
            case BibleBook::Id::ZECHARIAH:
                return "Zechariah";
            case BibleBook::Id::MALACHI:
                return "Malachi";
            case BibleBook::Id::MATTHEW:
                return "Matthew";
            case BibleBook::Id::MARK:
                return "Mark";
            case BibleBook::Id::LUKE:
                return "Luke";
            case BibleBook::Id::JOHN:
                return "John";
            case BibleBook::Id::ACTS:
                return "Acts";
            case BibleBook::Id::ROMANS:
                return "Romans";
            case BibleBook::Id::FIRST_CORINTHIANS:
                return "1 Corinthians";
            case BibleBook::Id::SECOND_CORINTHIANS:
                return "2 Corinthians";
            case BibleBook::Id::GALATIANS:
                return "Galatians";
            case BibleBook::Id::EPHESIANS:
                return "Ephesians";
            case BibleBook::Id::PHILIPPIANS:
                return "Philippians";
            case BibleBook::Id::COLOSSIANS:
                return "Colossians";
            case BibleBook::Id::FIRST_THESSALONIANS:
                return "1 Thessalonians";
            case BibleBook::Id::SECOND_THESSALONIANS:
                return "2 Thessalonians";
            case BibleBook::Id::FIRST_TIMOTHY:
                return "1 Timothy";
            case BibleBook::Id::SECOND_TIMOTHY:
                return "2 Timothy";
            case BibleBook::Id::TITUS:
                return "Titus";
            case BibleBook::Id::PHILEMON:
                return "Philemon";
            case BibleBook::Id::HEBREWS:
                return "Hebrews";
            case BibleBook::Id::JAMES:
                return "James";
            case BibleBook::Id::FIRST_PETER:
                return "1 Peter";
            case BibleBook::Id::SECOND_PETER:
                return "2 Peter";
            case BibleBook::Id::FIRST_JOHN:
                return "1 John";
            case BibleBook::Id::SECOND_JOHN:
                return "2 John";
            case BibleBook::Id::THIRD_JOHN:
                return "3 John";
            case BibleBook::Id::JUDE:
                return "Jude";
            case BibleBook::Id::REVELATION:
                return "Revelation";
            case BibleBook::Id::INVALID:
                // The invalid and default cases should be the same.
                [[fallthrough]];
            default:
                return "INVALID";
        }
    }

    /// Constructor.
    /// @param[in]  verse_counts_by_chapter - \ref VerseCountsByChapter.
    BibleBook::BibleBook(const std::vector<unsigned int>& verse_counts_by_chapter) :
        VerseCountsByChapter(verse_counts_by_chapter)
    {}

    const std::map<BibleBook::Id, BibleBook> BOOKS_BY_ID
    {
        // Verse counts organized into rows of 10 (10 chapters per row).
        { 
            BibleBook::Id::GENESIS, 
            BibleBook({
                31, 25, 24, 26, 32, 22, 24, 22, 29, 32, 
                32, 20, 18, 24, 21, 16, 27, 33, 38, 18, 
                34, 24, 20, 67, 34, 35, 46, 22, 35, 43, 
                55, 32, 20, 31, 29, 43, 36, 30, 23, 23, 
                57, 38, 34, 34, 28, 34, 31, 22, 33, 26
                })
        },
        {
            BibleBook::Id::EXODUS,
            BibleBook({
                22, 25, 22, 31, 23, 30, 25, 32, 35, 29,
                10, 51, 22, 31, 27, 36, 16, 27, 25, 26,
                36, 31, 33, 18, 40, 37, 21, 43, 46, 38,
                18, 35, 23, 35, 35, 38, 29, 31, 43, 38
                })
        },
        {
            BibleBook::Id::LEVITICUS,
            BibleBook({
                17, 16, 17, 35, 19, 30, 38, 36, 24, 20,
                47,  8, 59, 57, 33, 34, 16, 30, 37, 27,
                24, 33, 44, 23, 55, 46, 34
                })
        },
        {
            BibleBook::Id::NUMBERS,
            BibleBook({
                54, 34, 51, 49, 31, 27, 89, 26, 23, 36,
                35, 16, 33, 45, 41, 50, 13, 32, 22, 29,
                35, 41, 30, 25, 18, 65, 23, 31, 40, 16, 
                54, 42, 56, 29, 34, 13
                })
        },
        {
            BibleBook::Id::DEUTERONOMY,
            BibleBook({
                46, 37, 29, 49, 33, 25, 26, 20, 29, 22, 
                32, 32, 18, 29, 23, 22, 20, 22, 21, 20, 
                23, 30, 25, 22, 19, 19, 26, 68, 29, 20, 
                30, 52, 29, 12
                })
        },
        {
            BibleBook::Id::JOSHUA,
            BibleBook({
                18, 24, 17, 24, 15, 27, 26, 35, 27, 43, 
                23, 24, 33, 15, 63, 10, 18, 28, 51,  9, 
                45, 34, 16, 33
                })
        },
        {
            BibleBook::Id::JUDGES,
            BibleBook({
                36, 23, 31, 24, 31, 40, 25, 35, 57, 18, 
                40, 15, 25, 20, 20, 31, 13, 31, 30, 48, 
                25
                })
        },
        {
            BibleBook::Id::RUTH,
            BibleBook({
                22, 23, 18, 22
                })
        },
        {
            BibleBook::Id::FIRST_SAMUEL,
            BibleBook({
                28, 36, 21, 22, 12, 21, 17, 22, 27, 27, 
                15, 25, 23, 52, 35, 23, 58, 30, 24, 42, 
                15, 23, 29, 22, 44, 25, 12, 25, 11, 31, 
                13
                })
        },
        {
            BibleBook::Id::SECOND_SAMUEL,
            BibleBook({
                27, 32, 39, 12, 25, 23, 29, 18, 13, 19, 
                27, 31, 39, 33, 37, 23, 29, 33, 43, 26, 
                22, 51, 39, 25
                })
        },
        {
            BibleBook::Id::FIRST_KINGS,
            BibleBook({
                53, 46, 28, 34, 18, 38, 51, 66, 28, 29, 
                43, 33, 34, 31, 34, 34, 24, 46, 21, 43, 
                29, 53
                })
        },
        {
            BibleBook::Id::SECOND_KINGS,
            BibleBook({
                18, 25, 27, 44, 27, 33, 20, 29, 37, 36, 
                21, 21, 25, 29, 38, 20, 41, 37, 37, 21, 
                26, 20, 37, 20, 30
                })
        },
        {
            BibleBook::Id::FIRST_CHRONICLES,
            BibleBook({
                54, 55, 24, 43, 26, 81, 40, 40, 44, 14, 
                47, 40, 14, 17, 29, 43, 27, 17, 19,  8, 
                30, 19, 32, 31, 31, 32, 34, 21, 30
                })
        },
        {
            BibleBook::Id::SECOND_CHRONICLES,
            BibleBook({
                17, 18, 17, 22, 14, 42, 22, 18, 31, 19, 
                23, 16, 22, 15, 19, 14, 19, 34, 11, 37, 
                20, 12, 21, 27, 28, 23,  9, 27, 36, 27, 
                21, 33, 25, 33, 27, 23
                })
        },
        {
            BibleBook::Id::EZRA,
            BibleBook({
                11, 70, 13, 24, 17, 22, 28, 36, 15, 44
                })
        },
        {
            BibleBook::Id::NEHEMIAH,
            BibleBook({
                11, 20, 32, 23, 19, 19, 73, 18, 38, 39, 
                36, 47, 31
                })
        },
        {
            BibleBook::Id::ESTHER,
            BibleBook({
                22, 23, 15, 17, 14, 14, 10, 17, 32, 3
                })
        },
        {
            BibleBook::Id::JOB,
            BibleBook({
                22, 13, 26, 21, 27, 30, 21, 22, 35, 22, 
                20, 25, 28, 22, 35, 22, 16, 21, 29, 29, 
                34, 30, 17, 25,  6, 14, 23, 28, 25, 31, 
                40, 22, 33, 37, 16, 33, 24, 41, 30, 24, 
                34, 17
                })
        },
        {
            BibleBook::Id::PSALMS,
            BibleBook({
                 6, 12,  8,  8, 12, 10, 17,  9, 20, 18, 
                 7,  8,  6,  7,  5, 11, 15, 50, 14,  9, 
                13, 31,  6, 10, 22, 12, 14,  9, 11, 12, 
                24, 11, 22, 22, 28, 12, 40, 22, 13, 17, 
                13, 11,  5, 26, 17, 11,  9, 14, 20, 23, 
                19,  9,  6,  7, 23, 13, 11, 11, 17, 12, 
                 8, 12, 11, 10, 13, 20,  7, 35, 36,  5, 
                24, 20, 28, 23, 10, 12, 20, 72, 13, 19, 
                16,  8, 18, 12, 13, 17,  7, 18, 52, 17, 
                16, 15,  5, 23, 11, 13, 12,  9,  9,  5, 
                 8, 28, 22, 35, 45, 48, 43, 13, 31,  7, 
                10, 10,  9,  8, 18, 19,  2, 29, 176, 7, 
                 8,  9,  4,  8,  5,  6,  5,  6,  8,  8, 
                 3, 18,  3,  3, 21, 26,  9,  8, 24, 13, 
                10,  7, 12, 15, 21, 10, 20, 14,  9,  6
                })
        },
        {
            BibleBook::Id::PROVERBS,
            BibleBook({
                33, 22, 35, 27, 23, 35, 27, 36, 18, 32, 
                31, 28, 25, 35, 33, 33, 28, 24, 29, 30, 
                31, 29, 35, 34, 28, 28, 27, 28, 27, 33, 
                31
                })
        },
        {
            BibleBook::Id::ECCLESIASTES,
            BibleBook({
                18, 26, 22, 16, 20, 12, 29, 17, 18, 20, 
                10, 14
                })
        },
        {
            BibleBook::Id::SONG_OF_SOLOMON,
            BibleBook({
                17, 17, 11, 16, 16, 13, 13, 14
                })
        },
        {
            BibleBook::Id::ISAIAH,
            BibleBook({
                31, 22, 26,  6, 30, 13, 25, 22, 21, 34, 
                16,  6, 22, 32,  9, 14, 14,  7, 25,  6, 
                17, 25, 18, 23, 12, 21, 13, 29, 24, 33, 
                 9, 20, 24, 17, 10, 22, 38, 22,  8, 31, 
                29, 25, 28, 28, 25, 13, 15, 22, 26, 11, 
                23, 15, 12, 17, 13, 12, 21, 14, 21, 22, 
                11, 12, 19, 12, 25, 24
                })
        },
        {
            BibleBook::Id::JEREMIAH,
            BibleBook({
                37, 25, 31, 31, 30, 34, 22, 26, 25, 23, 
                17, 27, 22, 21, 21, 27, 23, 15, 18, 14, 
                30, 40, 10, 38, 24, 22, 17, 32, 24, 40, 
                44, 26, 22, 19, 32, 21, 28, 18, 16, 18, 
                22, 13, 30,  5, 28,  7, 47, 39, 46, 64, 
                34
                })
        },
        {
            BibleBook::Id::LAMENTATIONS,
            BibleBook({
                22, 22, 66, 22, 22
                })
        },
        {
            BibleBook::Id::EZEKIEL,
            BibleBook({
                28, 10, 27, 17, 17, 14, 27, 18, 11, 22, 
                25, 28, 23, 23,  8, 63, 24, 32, 14, 49, 
                32, 31, 49, 27, 17, 21, 36, 26, 21, 26, 
                18, 32, 33, 31, 15, 38, 28, 23, 29, 49, 
                26, 20, 27, 31, 25, 24, 23, 35
                })
        },
        {
            BibleBook::Id::DANIEL,
            BibleBook({
                21, 49, 30, 37, 31, 28, 28, 27, 27, 21, 
                45, 13
                })
        },
        {
            BibleBook::Id::HOSEA,
            BibleBook({
                11, 23,  5, 19, 15, 11, 16, 14, 17, 15, 
                12, 14, 16,  9
                })
        },
        {
            BibleBook::Id::JOEL,
            BibleBook({
                20, 32, 21
                })
        },
        {
            BibleBook::Id::AMOS,
            BibleBook({
                15, 16, 15, 13, 27, 14, 17, 14, 15
                })
        },
        {
            BibleBook::Id::OBADIAH,
            BibleBook({
                21
                })
        },
        {
            BibleBook::Id::JONAH,
            BibleBook({
                17, 10, 10, 11
                })
        },
        {
            BibleBook::Id::MICAH,
            BibleBook({
                16, 13, 12, 13, 15, 16, 20
                })
        },
        {
            BibleBook::Id::NAHUM,
            BibleBook({
                15, 13, 19
                })
        },
        {
            BibleBook::Id::HABAKKUK,
            BibleBook({
                17, 20, 19
                })
        },
        {
            BibleBook::Id::ZEPHANIAH,
            BibleBook({
                18, 15, 20
                })
        },
        {
            BibleBook::Id::HAGGAI,
            BibleBook({
                15, 23
                })
        },
        {
            BibleBook::Id::ZECHARIAH,
            BibleBook({
                21, 13, 10, 14, 11, 15, 14, 23, 17, 12, 
                17, 14,  9, 21
                })
        },
        {
            BibleBook::Id::MALACHI,
            BibleBook({
                14, 17, 18, 6
                })
        },
        {
            BibleBook::Id::MATTHEW,
            BibleBook({
                25, 23, 17, 25, 48, 34, 29, 34, 38, 42, 
                30, 50, 58, 36, 39, 28, 27, 35, 30, 34, 
                46, 46, 39, 51, 46, 75, 66, 20
                })
        },
        {
            BibleBook::Id::MARK,
            BibleBook({
                45, 28, 35, 41, 43, 56, 37, 38, 50, 52, 
                33, 44, 37, 72, 47, 20
                })
        },
        {
            BibleBook::Id::LUKE,
            BibleBook({
                80, 52, 38, 44, 39, 49, 50, 56, 62, 42, 
                54, 59, 35, 35, 32, 31, 37, 43, 48, 47, 
                38, 71, 56, 53
                })
        },
        {
            BibleBook::Id::JOHN,
            BibleBook({
                51, 25, 36, 54, 47, 71, 53, 59, 41, 42, 
                57, 50, 38, 31, 27, 33, 26, 40, 42, 31, 
                25
                })
        },
        {
            BibleBook::Id::ACTS,
            BibleBook({
                47, 26, 37, 42, 15, 60, 40, 43, 48, 30, 
                25, 52, 28, 41, 40, 34, 28, 41, 38, 40, 
                30, 35, 27, 27, 32, 44, 31
                })
        },
        {
            BibleBook::Id::ROMANS,
            BibleBook({
                32, 29, 31, 25, 21, 23, 25, 39, 33, 21, 
                36, 21, 14, 26, 33, 25
                })
        },
        {
            BibleBook::Id::FIRST_CORINTHIANS,
            BibleBook({
                31, 16, 23, 21, 13, 20, 40, 13, 27, 33, 
                34, 31, 13, 40, 58, 24
                })
        },
        {
            BibleBook::Id::SECOND_CORINTHIANS,
            BibleBook({
                24, 17, 18, 18, 21, 18, 16, 24, 15, 18, 
                33, 21, 14
                })
        },
        {
            BibleBook::Id::GALATIANS,
            BibleBook({
                24, 21, 29, 31, 26, 18
                })
        },
        {
            BibleBook::Id::EPHESIANS,
            BibleBook({
                23, 22, 21, 32, 33, 24
                })
        },
        {
            BibleBook::Id::PHILIPPIANS,
            BibleBook({
                30, 30, 21, 23
                })
        },
        {
            BibleBook::Id::COLOSSIANS,
            BibleBook({
                29, 23, 25, 18
                })
        },
        {
            BibleBook::Id::FIRST_THESSALONIANS,
            BibleBook({
                10, 20, 13, 18, 28
                })
        },
        {
            BibleBook::Id::SECOND_THESSALONIANS,
            BibleBook({
                12, 17, 18
                })
        },
        {
            BibleBook::Id::FIRST_TIMOTHY,
            BibleBook({
                20, 15, 16, 16, 25, 21
                })
        },
        {
            BibleBook::Id::SECOND_TIMOTHY,
            BibleBook({
                18, 26, 17, 22
                })
        },
        {
            BibleBook::Id::TITUS,
            BibleBook({
                16, 15, 15
                })
        },
        {
            BibleBook::Id::PHILEMON,
            BibleBook({
                25
                })
        },
        {
            BibleBook::Id::HEBREWS,
            BibleBook({
                14, 18, 19, 16, 14, 20, 28, 13, 28, 39, 
                40, 29, 25
                })
        },
        {
            BibleBook::Id::JAMES,
            BibleBook({
                27, 26, 18, 17, 20
                })
        },
        {
            BibleBook::Id::FIRST_PETER,
            BibleBook({
                25, 25, 22, 19, 14
                })
        },
        {
            BibleBook::Id::SECOND_PETER,
            BibleBook({
                21, 22, 18
                })
        },
        {
            BibleBook::Id::FIRST_JOHN,
            BibleBook({
                10, 29, 24, 21, 21
                })
        },
        {
            BibleBook::Id::SECOND_JOHN,
            BibleBook({
                13
                })
        },
        {
            BibleBook::Id::THIRD_JOHN,
            BibleBook({
                14
                })
        },
        {
            BibleBook::Id::JUDE,
            BibleBook({
                25
                })
        },
        {
            BibleBook::Id::REVELATION,
            BibleBook({
                20, 29, 22, 11, 14, 17, 17, 13, 21, 11, 
                19, 17, 18, 20,  8, 21, 18, 24, 21, 15, 
                27, 21
                })
        },
    };
}
