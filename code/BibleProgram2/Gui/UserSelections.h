#pragma once

#include <string>
#include "BibleData/BibleVerseId.h"

namespace GUI
{
    /// Information on the user's selections in the GUI.
    /// This information encapsulates the different pieces of data that a user can
    /// select that have significant alterations to what is displayed on the GUI.
    class UserSelections
    {
    public:
        /// The word currently highlighted in the GUI by hovering.
        std::string CurrentlyHighlightedWord = "";
        /// The word currently selected (by clicking) in the GUI.
        std::string CurrentlySelectedWord = "";
        /// The currently selected (by clicking) Bible verse ID.
        BIBLE_DATA::BibleVerseId CurrentlySelectedBibleVerseId = {};
    };
}
