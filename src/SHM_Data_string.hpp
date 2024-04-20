/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#pragma once

#include "SHM_data.hpp"

class SHM_Data_string final : public SHM_data {
private:
    const std::size_t length;

public:
    SHM_Data_string(std::string name, const void *data, std::size_t address, std::size_t length);

    [[nodiscard]] nlohmann::json get_data() const override;
};
