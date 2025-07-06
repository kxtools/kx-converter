#include "kx_converter/models.h"

namespace kx_converter::Models {

    void to_json(nlohmann::json& j, const Coordinates& c) {
        j = nlohmann::json{
            {"Name", c.name},
            {"X", c.x},
            {"Y", c.y},
            {"Z", c.z}
        };
    }

    void to_json(nlohmann::json& j, const Map& m) {
        j = nlohmann::json{
            {"Name", m.name},
            {"Author", m.author},
            {"Coordinates", m.coordinates}
        };
    }


} // namespace kx_converter::Models