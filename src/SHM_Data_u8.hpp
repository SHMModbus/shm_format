//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#pragma once

#include "SHM_data.hpp"

class SHM_Data_u8 final : public SHM_data {
public:
    SHM_Data_u8(std::string name, const void *data, std::size_t address);

    [[nodiscard]] nlohmann::json get_data() const override;
};
