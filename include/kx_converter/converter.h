#pragma once

#include "kx_converter/parser.h"
#include <filesystem>
#include <string>

namespace kx_converter {

    class Converter {
    public:
        struct Config {
            std::filesystem::path input_dir;
            std::filesystem::path output_dir;
            std::string author;
            bool recursive{ false };
            bool overwrite{ false };
        };

        explicit Converter(Config config);

        // Executes the conversion process.
        // Returns true on success, false if any errors occurred.
        bool run();

    private:
        // Processes a single .txt file.
        // Returns the number of parsing errors encountered in that file.
        int process_file(const std::filesystem::path& file_path);

        Config m_config;
        LineParser m_parser;
        int m_processed_files{ 0 };
        int m_total_errors{ 0 };
    };

} // namespace kx_converter