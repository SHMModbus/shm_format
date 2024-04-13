//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include "SHM_Data_f64lr.hpp"
#include "endian.hpp"

static_assert(sizeof(double) == 8);

SHM_Data_f64lr::SHM_Data_f64lr(std::string name, const void *data, std::size_t address)
    : SHM_data(std::move(name), data, address) {
}

nlohmann::json SHM_Data_f64lr::get_data() const {
    auto json = get_json_skeleton();

    json["type"]   = "double64";
    json["endian"] = "little-swap16";

    double data_raw = *reinterpret_cast<const double *>(reinterpret_cast<const uint8_t *>(data) + address);
    union {
        double   double_data;
        uint16_t reg_data[4];
    };
    double_data = endian::little_to_host(data_raw);
    std::swap(reg_data[0], reg_data[3]);
    std::swap(reg_data[1], reg_data[2]);
    json["data"] = double_data;

    return json;
}
