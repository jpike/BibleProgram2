#include <cstddef>
#include <iostream>
#include "BibleData/BibleTranslation.h"
#include "BibleData/VerseCharacterRanges_KJV.h"
#include "BibleData/VerseCharacterRanges_WEB.h"
#include "BibleData/VerseText_KJV.h"
#include "BibleData/VerseText_WEB.h"
#include "Debugging/Timer.h"

int main()
{
    BIBLE_DATA::BibleTranslation kjv_translation;
    {
        DEBUGGING::SystemClockTimer system_clock_timer("KJV System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("KJV High-Resolution Timer");
        kjv_translation.Text = BIBLE_DATA::KJV_VERSE_TEXT;
        kjv_translation.VerseCharacterRanges = BIBLE_DATA::KJV_VERSE_CHARACTER_RANGES;
        kjv_translation.WordIndex = BIBLE_DATA::BibleWordIndex::Build(&kjv_translation);
    }

    {
        DEBUGGING::SystemClockTimer system_clock_timer("KJV Print System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("KJV Print High-Resolution Timer");
        
        std::cout << "KJV Word Index:" << std::endl;
        for (const auto&[word, verses] : kjv_translation.WordIndex.BibleVersesByWord)
        {
            std::size_t verse_count = verses.size();
            std::cout << word << " = " << verse_count << std::endl;
        }
    }

    std::cout << "---------------------------------------------------" << std::endl;

    BIBLE_DATA::BibleTranslation web_translation;
    {
        DEBUGGING::SystemClockTimer system_clock_timer("WEB System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("WEB High-Resolution Timer");
        web_translation.Text = BIBLE_DATA::WEB_VERSE_TEXT;
        web_translation.VerseCharacterRanges = BIBLE_DATA::WEB_VERSE_CHARACTER_RANGES;
        web_translation.WordIndex = BIBLE_DATA::BibleWordIndex::Build(&web_translation);
    }

    {
        DEBUGGING::SystemClockTimer system_clock_timer("WEB Print System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("WEB Print High-Resolution Timer");
        
        std::cout << "WEB Word Index:" << std::endl;
        for (const auto&[word, verses] : web_translation.WordIndex.BibleVersesByWord)
        {
            std::size_t verse_count = verses.size();
            std::cout << word << " = " << verse_count << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
