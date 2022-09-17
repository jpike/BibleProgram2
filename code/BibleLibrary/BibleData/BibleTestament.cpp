#include "BibleData/BibleTestament.h"

namespace BIBLE_DATA
{
    /// Gets the testament for the specific verse.
    /// @param[in]  verse_id - The ID for which to get the testament.
    /// @return The testament for the specific verse.
    BibleTestament::Id BibleTestament::Get(const BibleVerseId& verse_id)
    {
        bool is_old = ((BibleBook::GENESIS <= verse_id.Book) && (verse_id.Book <= BibleBook::MALACHI));
        bool is_new = ((BibleBook::MATTHEW <= verse_id.Book) && (verse_id.Book <= BibleBook::REVELATION));
        if (is_old)
        {
            return BibleTestament::OLD;
        }
        else if (is_new)
        {
            return BibleTestament::NEW;
        }
        else
        {
            return BibleTestament::INVALID;
        }
    }
}
