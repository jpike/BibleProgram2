#include <algorithm>
#include <fstream>
#include <pugixml/pugixml.hpp>
#include "BibleData/BibleBook.h"
#include "BibleData/BibleDataCodeFiles.h"

namespace BIBLE_DATA
{
    /// Generates appropriate Bible data code files based on the specified data.
    /// @param[in]  translation_name - The name of the Bible translation for which files are being generated.
    ///     This helps form a suffix for the filenames (before the file extension) along with constant names
    ///     in the generated code files.
    /// @param[in]  input_osis_xml_filepath - The path to the OSIS XML file for the Bible data to parse as
    ///     input to use for generating the output code files.
    /// @param[in]  output_folder_path - The path for the output folder in which to write the generated code files.
    void BibleDataCodeFiles::Generate(
        const std::string& translation_name,
        const std::filesystem::path input_osis_xml_filepath,
        const std::filesystem::path output_folder_path)
    {
        // PARSE THE XML FILE.
        pugi::xml_document osis_xml_document;
        pugi::xml_parse_result parse_result = osis_xml_document.load_file(input_osis_xml_filepath.c_str());
        if (!parse_result)
        {
            return;
        }

        // WRITE THE INITIAL OUTPUT HEADER FILES.
        std::string verse_text_code_file_base_name = "VerseText_" + translation_name;
        std::string verse_text_header_filename = verse_text_code_file_base_name + ".h";
        // Absolute paths are used for easier debugging and better guaranteeing the files can be opened.
        std::filesystem::path verse_text_header_filepath = std::filesystem::absolute(output_folder_path / verse_text_header_filename);

        // An array is written in the same namespace as the library for mroe consistent use.
        std::string verse_text_array_name = translation_name + "_VERSE_TEXT";
        std::ofstream output_verse_text_header_file(verse_text_header_filepath);
        output_verse_text_header_file
            << "#pragma once\n"
            << "namespace BIBLE_DATA\n"
            << "{\n"
            << "extern const char " << verse_text_array_name << "[];\n"
            << "}\n";

        // Another file is written for verse character ranges for easy lookup of verses within the entire Biblical text
        // without requiring extra string allocation.
        std::string verse_ranges_code_file_base_name = "VerseCharacterRanges_" + translation_name;
        std::string verse_ranges_header_filename = verse_ranges_code_file_base_name + ".h";
        std::filesystem::path verse_ranges_header_filepath = std::filesystem::absolute(output_folder_path / verse_ranges_header_filename);

        // Thus far, a std::vector seems fast enough without requiring pre-computation of the number of verses for an array.
        std::string verse_ranges_vector_name = translation_name + "_VERSE_CHARACTER_RANGES";
        std::ofstream output_verse_ranges_header_file(verse_ranges_header_filepath);
        output_verse_ranges_header_file
            << "#pragma once\n"
            << "#include <vector>\n"
            << "#include \"BibleData/BibleVerseIdWithCharacterRange.h\"\n"
            << "namespace BIBLE_DATA\n"
            << "{\n"
            << "extern const std::vector<BIBLE_DATA::BibleVerseIdWithCharacterRange> " << verse_ranges_vector_name << ";\n"
            << "}\n";
        
        // WRITE OUT THE INITIAL OUTPUT CPP FILES.
        std::string verse_text_cpp_filename = verse_text_code_file_base_name + ".cpp";
        std::filesystem::path verse_text_cpp_filepath = std::filesystem::absolute(output_folder_path / verse_text_cpp_filename);
        std::ofstream output_verse_text_cpp_file(verse_text_cpp_filepath);
        output_verse_text_cpp_file
            << "#include \"BibleData/" << verse_text_header_filename << "\"\n"
            << "namespace BIBLE_DATA\n"
            << "{\n"
            << "const char " << verse_text_array_name << "[] =\n"
            << "{\n";

        std::string verse_ranges_cpp_filename = verse_ranges_code_file_base_name + ".cpp";
        std::filesystem::path verse_ranges_cpp_filepath = std::filesystem::absolute(output_folder_path / verse_ranges_cpp_filename);
        std::ofstream output_verse_ranges_cpp_file(verse_ranges_cpp_filepath);
        output_verse_ranges_cpp_file
            << "#include \"BibleData/" << verse_ranges_header_filename << "\"\n"
            << "namespace BIBLE_DATA\n"
            << "{\n"
            << "const std::vector<BIBLE_DATA::BibleVerseIdWithCharacterRange> " << verse_ranges_vector_name << " =\n"
            << "{\n";

        // PARSE VERSES FROM EACH BOOK IN THE INPUT FILE.
        std::size_t current_verse_start_offset = 0;
        pugi::xml_node osis_text_xml_node = osis_xml_document.select_node("/osis/osisText").node();
        // An assumption is being made that all direct child divs are for books, which is consistent with all files thus far.
        auto book_xml_nodes = osis_text_xml_node.children("div");
        for (const pugi::xml_node& book_xml_node : book_xml_nodes)
        {
            // PARSE ALL CHAPTERS FROM THE CURRENT BOOK.
            auto chapter_xml_nodes = book_xml_node.children("chapter");
            for (const pugi::xml_node& chapter_xml_node : chapter_xml_nodes)
            {
                // PARSE ALL VERSES FROM THE CURRENT CHAPTER.
                auto verse_xml_nodes = chapter_xml_node.children("verse");
                for (const pugi::xml_node& verse_xml_node : verse_xml_nodes)
                {
                    // PARSE IDENTIFYING INFORMATION ABOUT THE VERSE.
                    pugi::xml_attribute verse_id_attribute = verse_xml_node.attribute("osisID");
                    std::string verse_id = verse_id_attribute.as_string();

                    /// @todo   Factor this out into helper function!
                    std::vector<std::string> verse_id_components;
                    const char VERSE_ID_COMPONENT_DELIMITER = '.';
                    std::size_t string_search_start_index = 0;
                    std::size_t next_verse_id_component_delimiter_index = verse_id.find(VERSE_ID_COMPONENT_DELIMITER, string_search_start_index);
                    while (std::string::npos != next_verse_id_component_delimiter_index)
                    {
                        // STORE THE CURRENT COMPONENT.
                        std::size_t current_component_length_in_characters = next_verse_id_component_delimiter_index - string_search_start_index;
                        std::string verse_id_component = verse_id.substr(string_search_start_index, current_component_length_in_characters);
                        verse_id_components.push_back(verse_id_component);

                        // CONTINUE SEARCHING FOR MORE COMPONENTS.
                        string_search_start_index = next_verse_id_component_delimiter_index + 1;
                        next_verse_id_component_delimiter_index = verse_id.find(VERSE_ID_COMPONENT_DELIMITER, string_search_start_index);
                    }

                    // The final component won't be parsed in the last iteration of the loop.
                    std::size_t verse_number_length_in_characters = next_verse_id_component_delimiter_index - string_search_start_index;
                    std::string verse_number_component = verse_id.substr(string_search_start_index, verse_number_length_in_characters);
                    verse_id_components.push_back(verse_number_component);

                    const std::size_t BOOK_COMPONENT_INDEX = 0;
                    std::string book_name = verse_id_components.at(BOOK_COMPONENT_INDEX);
                    BibleBook::Id book_id = BibleBook::GetId(book_name);

                    const std::size_t CHAPTER_COMPONENT_INDEX = 1;
                    std::string chapter_number_string = verse_id_components.at(CHAPTER_COMPONENT_INDEX);
                    unsigned int chapter_number = std::stoi(chapter_number_string);

                    const std::size_t VERSE_NUMBER_COMPONENT_INDEX = 2;
                    std::string verse_number_string = verse_id_components.at(VERSE_NUMBER_COMPONENT_INDEX);
                    unsigned int verse_number = std::stoi(verse_number_string);

                    // GET THE VERSE TEXT.
                    std::string verse_text = verse_xml_node.text().as_string();

                    // DETERMINE THE ENDING OFFSET FOR THE VERSE WITHIN THE ENTIRE VERSE TEXT.
                    std::size_t verse_text_character_count = verse_text.size();
                    std::size_t verse_end_offset = current_verse_start_offset + verse_text_character_count;

                    // WRITE OUT EACH CHARACTER IN THE VERSE TEXT.
                    // Characters must be written out to the array individually as characters require special escaping/quoting
                    for (const char character : verse_text)
                    {
                        // Single quotes must be escaped since they are used to surround characters.
                        if (character == '\'')
                        {
                            output_verse_text_cpp_file << "'\\" << character << "',";
                        }
                        else 
                        {
                            output_verse_text_cpp_file << "'" << character << "',";
                        }
                    }
                    // Writing out a new line makes it easier to see when verses start/end.
                    output_verse_text_cpp_file << "\n";

                    // The character ranges within the text also need to be written out.
                    output_verse_ranges_cpp_file << "BIBLE_DATA::BibleVerseIdWithCharacterRange { BIBLE_DATA::BibleVerseId { static_cast<BIBLE_DATA::BibleBook::Id>(" << book_id << "), " << chapter_number << ", " << verse_number << " }, " << current_verse_start_offset << ", " << verse_end_offset << "}," << "\n";

                    // The starting offset for each verse needs to be tracked.
                    current_verse_start_offset += verse_text_character_count;
                }
            }
        }

        // Double curly braces to end both the arrays and the namespaces.
        output_verse_text_cpp_file << "};\n}" << std::endl;
        output_verse_ranges_cpp_file << "};\n}" << std::endl;
    }
}
