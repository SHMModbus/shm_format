//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_u16l.hpp"
#include "endian.hpp"

static_assert(sizeof(float) == 4);

SHM_Data_u16l::SHM_Data_u16l(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_u16l::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "uint16";
    json["endian"] = "little";

    auto data_raw = *reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(data) + address);
    json["data"]  = endian::little_to_host(data_raw);

    return json;
}
