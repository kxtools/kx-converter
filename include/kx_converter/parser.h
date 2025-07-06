#pragma once

#include "kx_converter/models.h"
#include <optional>
#include <string>
#include <sstream>

namespace kx_converter {

    class LineParser {
    public:
        // Parses a single line from a kx-map file.
        // The key feature is swapping the Y and Z coordinates upon successful parsing.
        // Returns an optional Coordinates struct. If parsing fails, returns std::nullopt.
        std::optional<Models::Coordinates> parse(const std::string& line) const;
    };

} // namespace kx_converter