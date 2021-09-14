#include <cctype>
#include <exception>
#include <iostream>
#include <limits>
#include <regex>
#include "BibleData/BibleBook.h"
#include "BibleData/BibleVerseRange.h"

namespace BIBLE_DATA
{
    /// Parses a verse range from the text.
    /// @param[in]  verse_range_text - The text from which to parse a verse range.
    /// @return The range, if successfully parsed; null otherwise.
    std::optional<BibleVerseRange> BibleVerseRange::Parse(const std::string& verse_range_text)
    {
        // PROVIDE BASIC DEBUG VISIBILITY.
        std::cout << "----------------------" << std::endl;
        std::cout << "Parsing: " << verse_range_text << std::endl;

        // DEFINE REGEXES FOR PARSING OUT VERSE RANGES.
        const std::string BOOK_NAME_REGEX("([A-za-z0-9 ]+)");
        const std::string CHAPTER_NUMBER_REGEX("(\\d+)");
        const std::string NUMERIC_RANGE_REGEX("((\\d+)-(\\d+))");
        const std::string CHAPTER_AND_VERSE_NUMBER_REGEX("((\\d+):(\\d+))");
        const std::string VERSE_NUMBER_REGEX("(\\d+)");

        const std::string OPTIONAL_WHITESPACE_REGEX = "\\s*";
        const std::string REQUIRED_WHITESPACE_REGEX = "\\s+";
        const std::string SINGLE_REFERENCE_REGEX = (
            OPTIONAL_WHITESPACE_REGEX +
            BOOK_NAME_REGEX + "?" +
            "(" +
            CHAPTER_NUMBER_REGEX + "|" + NUMERIC_RANGE_REGEX + "|" + CHAPTER_AND_VERSE_NUMBER_REGEX +
            ")"
            );
        const std::string FULL_REFERENCE_RANGE_REGEX = (
            SINGLE_REFERENCE_REGEX + "?" + 
            OPTIONAL_WHITESPACE_REGEX + "-" + OPTIONAL_WHITESPACE_REGEX +
            SINGLE_REFERENCE_REGEX
            );
        std::regex verse_range_regex(FULL_REFERENCE_RANGE_REGEX);

        const std::string single_book_reference_regex =
            "(" +
            // First verse reference.
            BOOK_NAME_REGEX + "?" +
            REQUIRED_WHITESPACE_REGEX +
            CHAPTER_NUMBER_REGEX +
            // Optional stuff for first reference.
            "(" +
            // Optional dash followed by another chapter number.
            "-" + CHAPTER_NUMBER_REGEX +
            "|" +
            // Optional colon followed by verse number.
            ":" + VERSE_NUMBER_REGEX +
            ")?" +
            ")";

        verse_range_regex = std::regex(
            single_book_reference_regex +
            // Optional 2nd verse reference after a dash.
            "(" + "\\s*-\\s*" +
            single_book_reference_regex +
            ")?"
        );

        // TRY MATCHING VERSES FROM THE TEXT.
        std::smatch verse_regex_matches;
        bool match_found = std::regex_match(verse_range_text, verse_regex_matches, verse_range_regex);
        if (match_found)
        {
            try
            {
                BibleVerseRange verse_range;

                // TRY PARSING OUT THE FIRST VERSE OF THE RANGE.
                // The ending verse of the range should be the same as the starting verse until a different verse is found for the end.
                constexpr std::size_t FIRST_BOOK_NAME_MATCH_INDEX = 2;
                std::string first_book_name = verse_regex_matches[FIRST_BOOK_NAME_MATCH_INDEX];
                BibleBook::Id first_book_id = BibleBook::GetId(first_book_name);
                verse_range.StartingVerse.Book = verse_range.EndingVerse.Book = first_book_id;

                constexpr std::size_t FIRST_CHAPTER_NUMBER_MATCH_INDEX = 3;
                std::string first_chapter = verse_regex_matches[FIRST_CHAPTER_NUMBER_MATCH_INDEX];
                unsigned int first_chapter_number = static_cast<unsigned int>(std::stoul(first_chapter));
                verse_range.StartingVerse.ChapterNumber = verse_range.EndingVerse.ChapterNumber = first_chapter_number;

                verse_range.StartingVerse.VerseNumber = 1;
                unsigned int chapter_index = first_chapter_number - 1;
                verse_range.EndingVerse.VerseNumber = BOOKS_BY_ID.at(first_book_id).VerseCountsByChapter.at(chapter_index);

                // CHECK FOR A DELIMITER INDICATING ANOTHER VERSE.
                constexpr std::size_t FIRST_DELIMITER_NUMBER_MATCH_INDEX = 4;
                std::string first_delimiter_and_number = verse_regex_matches[FIRST_DELIMITER_NUMBER_MATCH_INDEX];
                if (!first_delimiter_and_number.empty())
                {
                    // LOOK FOR A DELIMITER INDICATING A RANGE OF VERSES.
                    if (first_delimiter_and_number.starts_with('-'))
                    {
                        // PARSE THE ENDING VERSE OF THE RANGE.
                        constexpr std::size_t SECOND_CHAPTER_NUMBER_MATCH_INDEX = 5;
                        std::string second_chapter = verse_regex_matches[SECOND_CHAPTER_NUMBER_MATCH_INDEX];
                        unsigned int second_chapter_number = static_cast<unsigned int>(std::stoul(second_chapter));
                        verse_range.EndingVerse.ChapterNumber = second_chapter_number;
                        unsigned int second_chapter_index = second_chapter_number - 1;
                        verse_range.EndingVerse.VerseNumber = BOOKS_BY_ID.at(first_book_id).VerseCountsByChapter.at(second_chapter_index);
                    }
                    else if (first_delimiter_and_number.starts_with(':'))
                    {
                        constexpr std::size_t FIRST_VERSE_NUMBER_MATCH_INDEX = 6;
                        std::string first_verse = verse_regex_matches[FIRST_VERSE_NUMBER_MATCH_INDEX];
                        unsigned int first_verse_number = static_cast<unsigned int>(std::stoul(first_verse));
                        verse_range.StartingVerse.VerseNumber = verse_range.EndingVerse.VerseNumber = first_verse_number;
                    }
                }

                // PARSE ANY BOOK NAME FOR THE ENDING VERSE.
                constexpr std::size_t SECOND_BOOK_NAME_MATCH_INDEX = 9;
                std::string second_book_name = verse_regex_matches[SECOND_BOOK_NAME_MATCH_INDEX];
                if (!second_book_name.empty())
                {
                    BibleBook::Id second_book_id = BibleBook::GetId(second_book_name);
                    verse_range.EndingVerse.Book = second_book_id;
                    verse_range.EndingVerse.ChapterNumber = 1;
                    verse_range.EndingVerse.VerseNumber = 1;
                }

                // PARSE ANY CHAPTER NUMBER FOR THE ENDING VERSE.
                constexpr std::size_t SECOND_CHAPTER_NUMBER_MATCH_INDEX = 10;
                std::string second_chapter = verse_regex_matches[SECOND_CHAPTER_NUMBER_MATCH_INDEX];
                if (!second_chapter.empty())
                {
                    unsigned int second_chapter_number = static_cast<unsigned int>(std::stoul(second_chapter));
                    verse_range.EndingVerse.ChapterNumber = second_chapter_number;
                    unsigned int second_chapter_index = second_chapter_number - 1;
                    verse_range.EndingVerse.VerseNumber = BOOKS_BY_ID
                        .at(verse_range.EndingVerse.Book).VerseCountsByChapter.at(second_chapter_index);
                }

                // PARSE ANY VERSE NUMBER FOR THE ENDING VERSE.
                constexpr std::size_t SECOND_VERSE_NUMBER_MATCH_INDEX = 13;
                std::string second_verse = verse_regex_matches[SECOND_VERSE_NUMBER_MATCH_INDEX];
                if (!second_verse.empty())
                {
                    unsigned int second_verse_number = static_cast<unsigned int>(std::stoul(second_verse));
                    verse_range.EndingVerse.VerseNumber = second_verse_number;
                }

                // RETURN THE PARSED VERSE RANGE.
                return verse_range;
            }
            catch (const std::exception& exception)
            {
                std::cerr << "Standard exception parsing " << verse_range_text << ": " << exception.what() << std::endl;
                return std::nullopt;
            }
            catch (...)
            {
                std::cerr << "Unknown exception parsing " << verse_range_text << std::endl;
                return std::nullopt;
            }
        }
        else
        {
            // INDICATE THAT NO VERSE RANGE COULD BE PARSED.
            return std::nullopt;
        }
    }

    /// Equality operator.
    /// @param[in]  rhs - The verse range on the right-hand-side to compare against.
    /// @return True if the verse ranges are equal; false if not.
    bool BibleVerseRange::operator==(const BibleVerseRange& rhs) const
    {
        bool equal = (
            StartingVerse == rhs.StartingVerse &&
            EndingVerse == rhs.EndingVerse);
        return equal;
    }

    /// Inequality operator.
    /// @param[in]  rhs - The verse range on the right-hand-side to compare against.
    /// @return True if the verse ranges aren't equal; false if they are equal.
    bool BibleVerseRange::operator!=(const BibleVerseRange& rhs) const
    {
        bool equal = (*this == rhs);
        return !equal;
    }
}
