//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#pragma once

#include "SHM_data.hpp"

class SHM_Data_i64b final : public SHM_data {
public:
    SHM_Data_i64b(std::string name, const void *data, std::size_t address);

    [[nodiscard]] nlohmann::json get_data() const override;
};
