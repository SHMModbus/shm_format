//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_i8.hpp"

SHM_Data_i8::SHM_Data_i8(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_i8::get_data() const {
    auto json = get_json_skeleton();

    json["type"] = "uint8";

    auto data_raw = *(reinterpret_cast<const int8_t *>(data) + address);
    json["data"]  = static_cast<int>(data_raw);

    return json;
}
