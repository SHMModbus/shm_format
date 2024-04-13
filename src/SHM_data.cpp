/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#include "SHM_data.hpp"

nlohmann::json SHM_data::get_json_skeleton() const {
    nlohmann::json json;
    json["name"]    = name;
    json["address"] = address;
    return json;
}
