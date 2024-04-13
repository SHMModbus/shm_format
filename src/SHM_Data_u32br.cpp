//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_u32br.hpp"
#include "endian.hpp"

SHM_Data_u32br::SHM_Data_u32br(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_u32br::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "uint32";
    json["endian"] = "big-swap16";

    auto data_raw = *reinterpret_cast<const uint32_t *>(reinterpret_cast<const uint8_t *>(data) + address);

    union {
        uint32_t int_data;
        uint16_t reg_data[2];
    };
    int_data = endian::big_to_host(data_raw);
    std::swap(reg_data[0], reg_data[1]);
    json["data"] = int_data;

    return json;
}
