#include "kx_converter/parser.h"
#include <vector>
#include <string>
#include <algorithm> // Needed for std::replace

namespace kx_converter {

    // Helper function to check if a string can be parsed as a double.
    static bool is_numeric(const std::string& s) {
        if (s.empty()) {
            return false;
        }
        char* end = nullptr;
        strtod(s.c_str(), &end);
        return end != s.c_str() && *end == '\0';
    }

    std::optional<Models::Coordinates> LineParser::parse(const std::string& line) const {
        std::string processed_line = line;

        // --- FINAL ENHANCEMENT: Handle comma-separated coordinates ---
        std::replace(processed_line.begin(), processed_line.end(), ',', ' ');
        // --- END OF ENHANCEMENT ---

        // 1. Split the string by whitespace into tokens.
        std::vector<std::string> parts;
        std::string current_part;
        std::istringstream stream(processed_line);
        while (stream >> current_part) {
            parts.push_back(current_part);
        }

        // A valid line must have at least 4 parts (num, name, x, y, z).
        if (parts.size() < 4) {
            return std::nullopt;
        }

        // 2. Iterate BACKWARDS to find the right-most block of three numeric tokens.
        for (int i = parts.size() - 3; i >= 1; --i) {
            const std::string& x_str = parts[i];
            const std::string& y_str = parts[i + 1];
            const std::string& z_str = parts[i + 2];

            if (is_numeric(x_str) && is_numeric(y_str) && is_numeric(z_str)) {
                // SUCCESS: We found the coordinate block.

                std::string name;
                for (size_t j = 1; j < i; ++j) {
                    name += parts[j] + " ";
                }
                if (!name.empty()) {
                    name.pop_back();
                }

                if (name.length() >= 2 && name.front() == '\'' && name.back() == '\'') {
                    name = name.substr(1, name.length() - 2);
                }

                double x = std::stod(x_str);
                double y = std::stod(y_str);
                double z = std::stod(z_str);

                return Models::Coordinates{ name, x, z, y }; // Y and Z are swapped
            }
        }

        return std::nullopt;
    }

} // namespace kx_converter