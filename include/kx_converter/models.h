#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

namespace kx_converter::Models {

    struct Coordinates {
        std::string name;
        double x{ 0.0 };
        double y{ 0.0 };
        double z{ 0.0 };
    };

    struct Map {
        std::string name;
        std::string author;
        std::vector<Coordinates> coordinates;
    };

    // JSON serialization declarations
    void to_json(nlohmann::json& j, const Coordinates& c);
    void to_json(nlohmann::json& j, const Map& m);

} // namespace kx_converter::Models