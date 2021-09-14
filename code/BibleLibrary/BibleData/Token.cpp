#include <cassert>
#include <cctype>
#include "BibleData/Token.h"

namespace BIBLE_DATA
{
    /// Breaks the text of the verse into tokens.
    /// @param[in]  verse_text - The verse text to tokenize.
    /// @return The tokens of the verse.
    std::vector<Token> Token::Tokenize(const std::string_view verse_text)
    {
        // TOKENIZE THE VERSE TEXT.
        std::vector<Token> tokens;
        Token current_token;
        std::size_t current_token_start_index_in_text = 0;
        const char* verse_text_start = verse_text.data();
        const char* current_token_start = verse_text_start;
        std::size_t verse_character_count = verse_text.size();
        for (std::size_t character_index = 0; character_index < verse_character_count; ++character_index)
        {
            // CHECK IF THE CURRENT CHARACTER IS A SPACE.
            char character = verse_text[character_index];
            bool is_space = std::isspace(character);
            if (is_space)
            {
                // HANDLE THE CHARACTER APPROPRIATELY FOR THE CURRENT TYPE OF TOKEN.
                switch (current_token.Type)
                {
                    case TokenType::SPACE:
                    {
                        // ADD THE CHARACTER TO THE CURRENT SEQUENCE OF SPACES.
                        constexpr std::size_t COUNT_CURRENT_CHARACTER = 1;
                        std::size_t current_token_character_count = character_index - current_token_start_index_in_text + COUNT_CURRENT_CHARACTER;
                        current_token.Text = std::string_view(current_token_start, current_token_character_count);
                        break;
                    }
                    case TokenType::PUNCTUATION:
                    case TokenType::WORD:
                        // STORE THE CURRENT TOKEN.
                        // Now that a space has been found, the "previously" current token is now complete.
                        tokens.push_back(current_token);

                        // START A NEW SPACE TOKEN WITH THE CURRENT CHARACTER.
                        current_token = Token();
                        current_token.Type = TokenType::SPACE;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);

                        // CONTINUE ONTO THE NEXT CHARACTER.
                        continue;
                    case TokenType::INVALID:
                    default:
                        // START A NEW SPACE TOKEN WITH THE CURRENT CHARACTER.
                        current_token.Type = TokenType::SPACE;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);
                        break;
                }

                // CONTINUE ONTO THE NEXT CHARACTER.
                continue;
            }

            // CHECK IF THE CURRENT CHARACTER IS PUNCTUATION.
            bool is_punctuation = std::ispunct(character);
            if (is_punctuation)
            {
                // HANDLE THE CHARACTER APPROPRIATELY FOR THE CURRENT TYPE OF TOKEN.
                switch (current_token.Type)
                {
                    case TokenType::PUNCTUATION:
                    {
                        // ADD THE CHARACTER TO THE CURRENT SEQUENCE OF PUNCTUATION.
                        constexpr std::size_t COUNT_CURRENT_CHARACTER = 1;
                        std::size_t current_token_character_count = character_index - current_token_start_index_in_text + COUNT_CURRENT_CHARACTER;
                        current_token.Text = std::string_view(current_token_start, current_token_character_count);
                        break;
                    }
                    case TokenType::SPACE:
                        // A previous space token needs to be handled the same way as a previous word token.
                    case TokenType::WORD:
                        // STORE THE PREVIOUS TOKEN.
                        tokens.push_back(current_token);

                        // START A NEW TOKEN WITH THE CURRENT PUNCTUATION CHARACTER.
                        current_token.Type = TokenType::PUNCTUATION;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);
                        break;
                    case TokenType::INVALID:
                        // Both explicitly invalid and unknown token types
                        // are considered to be the start of a new token.
                    default:
                        // START A NEW PUNCTUATION TOKEN WITH THE CURRENT CHARACTER.
                        current_token.Type = TokenType::PUNCTUATION;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);
                        break;
                }

                // CONTINUE ONTO THE NEXT CHARACTER.
                continue;
            }

            // CHECK IF THE CURRENT CHARACTER IS PART OF A WORD.
            bool is_part_of_word = std::isalnum(character);
            if (is_part_of_word)
            {
                // HANDLE THE CHARACTER APPROPRIATELY FOR THE CURRENT TYPE OF TOKEN.
                switch (current_token.Type)
                {
                    case TokenType::WORD:
                    {
                        // ADD THE CHARACTER TO THE CURRENT WORD.
                        constexpr std::size_t COUNT_CURRENT_CHARACTER = 1;
                        std::size_t current_token_character_count = character_index - current_token_start_index_in_text + COUNT_CURRENT_CHARACTER;
                        current_token.Text = std::string_view(current_token_start, current_token_character_count);
                        break;
                    }
                    case TokenType::SPACE:
                        // A previous space token needs to be handled the same way as a previous punctuation token.
                    case TokenType::PUNCTUATION:
                        // STORE THE PREVIOUS TOKEN.
                        tokens.push_back(current_token);

                        // START A NEW TOKEN WITH THE CURRENT CHARACTER.
                        current_token.Type = TokenType::WORD;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);
                        break;
                    case TokenType::INVALID:
                        // Both explicitly invalid and unknown token types
                        // are considered to be the start of a new token.
                    default:
                        // START A NEW WORD TOKEN WITH THE CURRENT CHARACTER.
                        current_token.Type = TokenType::WORD;
                        current_token_start_index_in_text = character_index;
                        current_token_start = verse_text_start + character_index;
                        current_token.Text = std::string_view(current_token_start, 1);
                        break;
                }

                // CONTINUE ONTO THE NEXT CHARACTER.
                continue;
            }

            /// @todo   Handle contractions!

            // CHECK FOR ANY UNHANDLED CHARACTERS.
            assert(("Unhandled character: " + character, false));
        }

        // ADD THE LAST TOKEN IF ONE STILL EXISTS.
        // Tokens aren't added in the loop above unless a new character is seen
        // that is part of a different token, so there will likely be one remaining
        // token by the time the loop above exits.
        bool last_token_remains = !current_token.Text.empty();
        if (last_token_remains)
        {
            tokens.push_back(current_token);
        }

        // RETURN THE VERSE TOKENS.
        return tokens;
    }
}
