//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_u64br.hpp"
#include "endian.hpp"

SHM_Data_u64br::SHM_Data_u64br(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_u64br::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "uint64";
    json["endian"] = "big-swap16";

    auto data_raw = *reinterpret_cast<const uint64_t *>(reinterpret_cast<const uint8_t *>(data) + address);

    union {
        uint64_t int_data;
        uint16_t reg_data[4];
    };
    int_data = endian::big_to_host(data_raw);
    std::swap(reg_data[0], reg_data[3]);
    std::swap(reg_data[1], reg_data[2]);
    json["data"] = int_data;

    return json;
}
