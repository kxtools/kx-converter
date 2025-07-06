#include <iostream>
#include "CLI/CLI.hpp"
#include "kx_converter/converter.h"
#include "spdlog/spdlog.h"

int main(int argc, char** argv) {
    CLI::App app{ "A utility to convert KX-Maps .txt to .json" };

    std::string input_path;
    app.add_option("-i,--input", input_path, "Path to the input directory containing .txt files")->required()->check(CLI::ExistingDirectory);

    std::string output_path;
    app.add_option("-o,--output", output_path, "Path to the output directory for converted .json files")->required();

    std::string author = "Unknown";
    app.add_option("--author", author, "Name of the author to add to the JSON file");

    bool recursive = false;
    app.add_flag("--recursive", recursive, "Process the input directory recursively");

    bool overwrite = false;
    app.add_flag("--overwrite", overwrite, "Allow overwriting existing files in the output directory");

    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Enable detailed logging output");

    CLI11_PARSE(app, argc, argv);

    try {
        if (verbose) {
            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("Verbose logging enabled.");
        }
        else {
            spdlog::set_level(spdlog::level::info);
        }

        kx_converter::Converter::Config config{
        	input_path,
        	output_path,
            author,
            recursive,
            overwrite
        };

        kx_converter::Converter converter(config);
        bool success = converter.run();

        return success ? 0 : 1;

    }
    catch (const std::exception& e) {
        spdlog::critical("An unhandled error occurred: {}", e.what());
        return 1;
    }
}