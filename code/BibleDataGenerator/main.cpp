#include <cstdlib>
#include <filesystem>
#include "BibleData/BibleDataCodeFiles.h"
#include "Debugging/Timer.h"

int main()
{
    // GENERATE CODE FILES FOR KJV DATA.
    std::filesystem::path output_directory_path = "../../code/BibleLibrary/BibleData/";
    {
        DEBUGGING::SystemClockTimer system_clock_timer("KJV System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("KJV High-Resolution Timer");

        BIBLE_DATA::BibleDataCodeFiles::Generate(
            "KJV",
            "../../ThirdParty/GratisBible/kjv.xml",
            output_directory_path);
    }

    // GENERATE CODE FILES FOR WEB DATA.
    {
        DEBUGGING::SystemClockTimer system_clock_timer("WEB System Clock Timer");
        DEBUGGING::HighResolutionTimer high_resolution_timer("WEB High-Resolution Timer");

        BIBLE_DATA::BibleDataCodeFiles::Generate(
            "WEB",
            "../../ThirdParty/GratisBible/web.xml",
            output_directory_path);
    }

    return EXIT_SUCCESS;
}
