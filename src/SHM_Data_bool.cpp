/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#include "SHM_Data_bool.hpp"

SHM_Data_bool::SHM_Data_bool(std::string name, const void *data, std::size_t address, std::size_t bit)
    : SHM_data(std::move(name), data, address), bit(bit) {
}


nlohmann::json SHM_Data_bool::get_data() const {
    auto json = get_json_skeleton();

    json["type"] = "bool";
    json["bit"]  = bit;

    const auto byte_value = reinterpret_cast<const uint8_t *>(data)[address];
    const bool bit_value  = (byte_value >> bit) & 0b1;

    json["data"] = bit_value;

    return json;
}
