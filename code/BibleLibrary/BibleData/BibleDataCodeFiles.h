#pragma once

#include <filesystem>
#include <string>

namespace BIBLE_DATA
{
    /// C++ code files containing Bible data as needed/used for this library.
    /// This class supports generating such C++ files such that the data can
    /// be compiled into the library for more efficient access.
    class BibleDataCodeFiles
    {
    public:
        static void Generate(
            const std::string& translation_name,
            const std::filesystem::path input_osis_xml_filepath,
            const std::filesystem::path output_folder_path);
    };
}
