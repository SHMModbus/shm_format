//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_u8.hpp"

SHM_Data_u8::SHM_Data_u8(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_u8::get_data() const {
    auto json = get_json_skeleton();

    json["type"] = "uint8";

    auto data_raw = *(reinterpret_cast<const uint8_t *>(data) + address);
    json["data"]  = static_cast<unsigned>(data_raw);

    return json;
}
