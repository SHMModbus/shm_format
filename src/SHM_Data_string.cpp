/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#include "SHM_Data_string.hpp"

SHM_Data_string::SHM_Data_string(std::string name, const void *data, std::size_t address, std::size_t length)
    : SHM_data(std::move(name), data, address), length(length) {
}


nlohmann::json SHM_Data_string::get_data() const {
    auto json = get_json_skeleton();

    json["type"]       = "string";
    json["max_length"] = length;

    auto       data_ptr      = reinterpret_cast<const char *>(data) + address;
    const auto actual_length = strnlen(data_ptr, length);
    json["data"]             = std::string(data_ptr, actual_length);
    json["length"]           = actual_length;

    return json;
}
