#include <algorithm>
#include <cctype>
#include "BibleData/BibleTranslation.h"
#include "BibleData/BibleWordIndex.h"

namespace BIBLE_DATA
{
    /// Builds a word index for the given Bible translation.
    /// @param[in]  bible_translation - The translation to index.
    /// @return The word index for the corresponding Bible translation.
    BibleWordIndex BibleWordIndex::Build(const BibleTranslation* bible_translation)
    {
        BibleWordIndex word_index;
        word_index.Bible = bible_translation;

        // INDEX EACH VERSE IN THE BIBLE TRANSLATION.
        for (const BibleVerseIdWithCharacterRange& verse_id_with_character_range : bible_translation->VerseCharacterRanges)
        {
            // GET THE CURRENT VERSE TEXT.
            const char* first_character_in_verse = bible_translation->Text + verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText;
            std::size_t verse_character_count = (
                verse_id_with_character_range.LastCharacterOffsetIntoFullBibleText - 
                verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText);
            std::string_view current_verse_text(first_character_in_verse, verse_character_count);

            // INDEX EACH WORD IN THE VERSE.
            std::size_t current_word_start_character_index = 0;
            std::size_t current_verse_character_count = current_verse_text.length();
            for (std::size_t character_index = 0; character_index < current_verse_character_count; ++character_index)
            {
                char current_character = current_verse_text[character_index];

                // CHECK IF THE CURRENT CHARACTER IS PART OF A WORD.
                /// @todo   Handle contractions or other more complex scenarios!
                bool is_part_of_word = std::isalnum(current_character);
                if (!is_part_of_word)
                {
                    // GET THE LOWERCASE VERSION OF THE CURRENT WORD FOR INDEXING.
                    const char* first_character_in_word = first_character_in_verse + current_word_start_character_index;
                    std::size_t current_word_character_count = (character_index - current_word_start_character_index);
                    std::string current_word(first_character_in_word, current_word_character_count);
                    std::transform(
                        current_word.begin(), 
                        current_word.end(), 
                        current_word.begin(), 
                        [](const char character)
                        { 
                            return static_cast<char>(std::tolower(static_cast<int>(character)));
                        });

                    // INDEX THE VERSES FOR THE WORD AS LONG AS THE WORD EXISTS.
                    if (!current_word.empty())
                    {
                        word_index.BibleVersesByWord[current_word].emplace_back(verse_id_with_character_range);
                    }

                    // START MOVING TO THE NEXT WORD.
                    current_word_start_character_index = character_index + 1;
                }
            }
        }

        // RETURN THE POPULATED WORD INDEX.
        return word_index;
    }

    /// Gets matching verses for the corresponding word.
    /// @param[in]  word - The word for which to get matching verses.
    /// @return The verses matching the word.
    std::vector<BibleVerse> BibleWordIndex::GetMatchingVerses(const std::string_view word) const
    {
        std::printf("Verse index word count: %zu\n", BibleVersesByWord.size());

        // MAKE SURE THE WORD IS LOWERCASE.
        std::string lowercase_word(word);
        std::transform(
            lowercase_word.begin(), 
            lowercase_word.end(), 
            lowercase_word.begin(), 
            [](const char character)
            { 
                return static_cast<char>(std::tolower(static_cast<int>(character)));
            });
        auto word_with_verse_ids_with_character_ranges = BibleVersesByWord.find(lowercase_word);
        bool word_found = (BibleVersesByWord.cend() != word_with_verse_ids_with_character_ranges);
        if (!word_found)
        {
            // INDICATE THAT NO MATCHING VERSES COULD BE FOUND.
            return {};
        }

        // GET THE FULL VERSES FOR THE MATCHING VERSES IDENTIFIED ABOVE.
        const std::vector<BibleVerseIdWithCharacterRange>& verse_ids_with_character_ranges = word_with_verse_ids_with_character_ranges->second;

        std::printf("Matching verse count: %zu\n", verse_ids_with_character_ranges.size());

        std::vector<BibleVerse> matching_verses;
        for (const BibleVerseIdWithCharacterRange& verse_id_with_character_range : verse_ids_with_character_ranges)
        {
            // GET THE CURRENT VERSE TEXT.
            const char* first_character_in_verse = Bible->Text + verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText;
            std::size_t verse_character_count = (
                verse_id_with_character_range.LastCharacterOffsetIntoFullBibleText - 
                verse_id_with_character_range.FirstCharacterOffsetIntoFullBibleText);
            std::string_view current_verse_text(first_character_in_verse, verse_character_count);

            // TRACK THE CURRENT MATCHING VERSE.
            BibleVerse current_verse = 
            {
                .Id = BibleVerseId
                {
                    .Book = verse_id_with_character_range.Id.Book,
                    .ChapterNumber = verse_id_with_character_range.Id.ChapterNumber,
                    .VerseNumber = verse_id_with_character_range.Id.VerseNumber,
                },
                .Text = current_verse_text
            };

            matching_verses.emplace_back(current_verse);
        }

        return matching_verses;
    }
}
