#pragma once

#include <string_view>
#include <vector>
#include "BibleData/TokenType.h"

namespace BIBLE_DATA
{
    /// A single token that may be part of a Bible verse (https://en.wikipedia.org/wiki/Lexical_analysis#Tokenization).
    /// Verses are tokenized into words and punctuation in order to make it easier to manipulate text on an
    /// individual word basis.
    class Token
    {
    public:
        static std::vector<Token> Tokenize(const std::string_view verse_text);

        /// The type of token.
        TokenType Type = TokenType::INVALID;
        /// The text of the token.
        std::string_view Text = "";
    };
}
