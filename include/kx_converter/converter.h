#pragma once

#include "kx_converter/parser.h"
#include <filesystem>
#include <string>

namespace kx_converter
{
	/**
	 * @class Converter
	 * @brief Orchestrates the entire file conversion process.
	 *
	 * This class handles file discovery, directory creation, progress reporting,
	 * and orchestrates the parsing and writing of each file.
	 */
	class Converter
	{
	public:
		/**
		 * @struct Config
		 * @brief Configuration settings for a conversion task.
		 */
		struct Config
		{
			std::filesystem::path input_dir; //!< The source directory containing .txt files.
			std::filesystem::path output_dir; //!< The destination directory for the .json files.
			std::string author; //!< The author name to embed in the output JSON.
			bool recursive{false}; //!< Whether to process subdirectories.
			bool overwrite{false}; //!< Whether to overwrite existing output files.
		};

		/**
		 * @brief Constructs a Converter with the given configuration.
		 * @param config The configuration settings for the conversion process.
		 */
		explicit Converter(Config config);

		/**
		 * @brief Executes the conversion process based on the stored configuration.
		 * @return True if the entire process completed without any file-level errors, false otherwise.
		 */
		bool run();

	private:
		/**
		 * @brief Processes a single source .txt file and writes the .json output.
		 * @param file_path The path to the source .txt file.
		 * @return The number of parsing errors encountered within that file.
		 */
		int process_file(const std::filesystem::path& file_path);

		Config m_config;
		LineParser m_parser;
		int m_processed_files{0};
		int m_total_errors{0};
	};
} // namespace kx_converter
