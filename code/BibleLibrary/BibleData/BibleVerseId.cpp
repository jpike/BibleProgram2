#include "BibleData/BibleVerseId.h"

namespace BIBLE_DATA
{
    /// Equality operator.
    /// @param[in]  rhs - The verse ID on the right side to compare against.
    /// @return True if the verse IDs are equal; false if not.
    bool BibleVerseId::operator==(const BibleVerseId& rhs) const
    {
        bool equal = (
            Book == rhs.Book &&
            ChapterNumber == rhs.ChapterNumber &&
            VerseNumber == rhs.VerseNumber);
        return equal;
    }
    
    /// Less-than operator.
    /// @param[in]  rhs - The verse ID on the right side to compare against.
    /// @return True if this verse ID comes before the right side one; false otherwise.
    bool BibleVerseId::operator<(const BibleVerseId& rhs) const
    {
        if (Book < rhs.Book) return true;
        if (Book > rhs.Book) return false;

        // Books are equal...
        if (ChapterNumber < rhs.ChapterNumber) return true;
        if (ChapterNumber > rhs.ChapterNumber) return false;

        // Books and chapters are equal...
        bool this_verse_before_rhs_verse = (VerseNumber < rhs.VerseNumber);
        return this_verse_before_rhs_verse;
    }
}
