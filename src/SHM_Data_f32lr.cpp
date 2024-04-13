//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_f32lr.hpp"
#include "endian.hpp"

static_assert(sizeof(float) == 4);

SHM_Data_f32lr::SHM_Data_f32lr(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_f32lr::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "float32";
    json["endian"] = "little-swap16";

    float data_raw = *reinterpret_cast<const float *>(reinterpret_cast<const uint8_t *>(data) + address);
    union {
        float    float_data;
        uint16_t reg_data[2];
    };
    float_data = endian::little_to_host(data_raw);
    std::swap(reg_data[0], reg_data[1]);
    json["data"] = float_data;

    return json;
}
