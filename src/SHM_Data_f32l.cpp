//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_f32l.hpp"
#include "endian.hpp"

static_assert(sizeof(float) == 4);

SHM_Data_f32l::SHM_Data_f32l(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_f32l::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "float32";
    json["endian"] = "little";

    float data_raw = *reinterpret_cast<const float *>(reinterpret_cast<const uint8_t *>(data) + address);
    json["data"]   = endian::little_to_host(data_raw);

    return json;
}
