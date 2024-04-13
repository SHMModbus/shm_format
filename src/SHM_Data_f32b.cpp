//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_f32b.hpp"
#include "endian.hpp"

static_assert(sizeof(float) == 4);

SHM_Data_f32b::SHM_Data_f32b(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_f32b::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "float32";
    json["endian"] = "big";

    float data_raw = *reinterpret_cast<const float *>(reinterpret_cast<const uint8_t *>(data) + address);
    json["data"]   = endian::big_to_host(data_raw);

    return json;
}
