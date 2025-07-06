#include "kx_converter/converter.h"
#include "kx_converter/models.h"
#include "indicators/progress_bar.hpp"
#include "spdlog/spdlog.h"
#include <fstream>
#include <vector>

namespace kx_converter {

    Converter::Converter(Config config) : m_config(std::move(config)) {}

    bool Converter::run() {
        spdlog::info("Starting conversion...");
        spdlog::debug("Input directory: {}", m_config.input_dir.string());
        spdlog::debug("Output directory: {}", m_config.output_dir.string());
        spdlog::debug("Recursive: {}", m_config.recursive);
        spdlog::debug("Overwrite: {}", m_config.overwrite);

        if (!std::filesystem::exists(m_config.output_dir)) {
            spdlog::info("Output directory '{}' does not exist. Creating it.", m_config.output_dir.string());
            std::filesystem::create_directories(m_config.output_dir);
        }

        std::vector<std::filesystem::path> files_to_process;
        if (m_config.recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(m_config.input_dir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    files_to_process.push_back(entry.path());
                }
            }
        }
        else {
            for (const auto& entry : std::filesystem::directory_iterator(m_config.input_dir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    files_to_process.push_back(entry.path());
                }
            }
        }

        if (files_to_process.empty()) {
            spdlog::warn("No .txt files found in the input directory.");
            return true;
        }

        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"="},
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{"]"},
            indicators::option::PostfixText{"Processing files"},
            indicators::option::MaxProgress{files_to_process.size()}
        };

        for (const auto& file_path : files_to_process) {
            // --- START OF THE FINAL FIX ---
            std::string safe_filename = file_path.filename().string();
            // Sanitize the filename for the progress bar, removing non-printable characters.
            safe_filename.erase(std::remove_if(safe_filename.begin(), safe_filename.end(), [](unsigned char c) { return !isprint(c); }), safe_filename.end());

            // Use the sanitized filename for the progress bar text.
            bar.set_option(indicators::option::PostfixText{ safe_filename });
            // --- END OF THE FINAL FIX ---

            // Do the actual work.
            m_total_errors += process_file(file_path);
            m_processed_files++;

            // Tick the bar to show the work for this file is complete.
            bar.tick();
        }

        // Set the final completion message.
        bar.set_option(indicators::option::PostfixText{ "Conversion Complete" });
        bar.mark_as_completed();

        spdlog::info("----------------------------------------");
        spdlog::info("Conversion complete.");
        spdlog::info("Processed {} files with {} errors.", m_processed_files, m_total_errors);

        if (m_total_errors > 0) {
            spdlog::error("Check log for details on parsing errors.");
            return false;
        }

        return true;
    }

    int Converter::process_file(const std::filesystem::path& file_path) {
        spdlog::debug("Processing file: {}", file_path.string());
        std::filesystem::path relative_path = std::filesystem::relative(file_path, m_config.input_dir);
        std::filesystem::path output_path = m_config.output_dir / relative_path;
        output_path.replace_extension(".json");

        if (std::filesystem::exists(output_path) && !m_config.overwrite) {
            spdlog::error("Output file '{}' already exists. Use --overwrite to replace.", output_path.string());
            return 1;
        }

        if (const auto parent_path = output_path.parent_path(); !std::filesystem::exists(parent_path)) {
            spdlog::debug("Creating intermediate directory: {}", parent_path.string());
            std::filesystem::create_directories(parent_path);
        }

        // --- START OF CHANGE ---
        // Open the file in binary mode to read raw bytes.
        std::ifstream input_file(file_path, std::ios::binary);
        if (!input_file.is_open()) {
            spdlog::error("Failed to open input file: {}", file_path.string());
            return 1;
        }

        Models::Map map;
        map.name = file_path.stem().string();
        map.name.erase(std::remove_if(map.name.begin(), map.name.end(), [](unsigned char c) { return !isprint(c); }), map.name.end());

        map.author = m_config.author;

        std::string line;
        int line_num = 0;
        int file_errors = 0;

        while (std::getline(input_file, line)) {
            line_num++;

            // Sanitize the line by removing non-printable ASCII characters.
            // This will remove the bad bytes like 0x94 that cause JSON errors.
            line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) { return !isprint(c) && c != '\t'; }), line.end());

            // The carriage return fix is still needed.
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) continue;

            auto parsed_coords = m_parser.parse(line);
            if (parsed_coords) {
                map.coordinates.push_back(*parsed_coords);
            }
            else {
                spdlog::error("Failed to parse line in {}:{}. Content: \"{}\"", file_path.string(), line_num, line);
                file_errors++;
            }
        }
        // --- END OF CHANGE ---

        if (file_errors > 0) {
            spdlog::warn("Skipping JSON output for '{}' due to {} parsing errors.", file_path.string(), file_errors);
            return file_errors;
        }

        nlohmann::json final_json = map;

        std::ofstream output_file(output_path);
        if (!output_file.is_open()) {
            spdlog::error("Failed to create or open output file: {}", output_path.string());
            return file_errors + 1;
        }

        output_file << final_json.dump(2);
        spdlog::debug("Successfully wrote {}", output_path.string());

        return file_errors;
    }

} // namespace kx_converter