//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_i16b.hpp"
#include "endian.hpp"

SHM_Data_i16b::SHM_Data_i16b(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_i16b::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "int16";
    json["endian"] = "big";

    auto data_raw = *reinterpret_cast<const int16_t *>(reinterpret_cast<const uint8_t *>(data) + address);
    json["data"]  = endian::big_to_host(data_raw);

    return json;
}
