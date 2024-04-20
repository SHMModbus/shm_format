/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#include "SHM_Data_bytearray.hpp"

SHM_Data_bytearray::SHM_Data_bytearray(std::string name, const void *data, std::size_t address, std::size_t length)
    : SHM_data(std::move(name), data, address), length(length) {
}


nlohmann::json SHM_Data_bytearray::get_data() const {
    auto json = get_json_skeleton();

    json["type"] = "bytearray";
    json["size"] = length;

    auto data_ptr = reinterpret_cast<const uint8_t *>(data) + address;

    auto data_array = nlohmann::json::array();
    for (std::size_t i = 0; i < length; ++i)
        data_array += data_ptr[i];
    json["data"] = data_array;

    return json;
}
