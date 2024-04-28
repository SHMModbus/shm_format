/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#pragma once

#include "SHM_data.hpp"

#include <cxxshm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

class SHM_Format final {
private:
    std::unique_ptr<cxxshm::SharedMemory>  shared_memory;
    std::vector<std::unique_ptr<SHM_data>> shm_data;

public:
    SHM_Format(const std::string &shm_name, const std::string &config_file_path);
    ~SHM_Format() = default;

    SHM_Format(const SHM_Format &other) = delete;
    SHM_Format(SHM_Format &&other)      = delete;

    SHM_Format &operator=(const SHM_Format &other) = delete;
    SHM_Format &operator=(SHM_Format &&other)      = delete;

    [[nodiscard]] nlohmann::json execute() const;

    [[nodiscard]] inline const std::string &get_shm_name() const { return shared_memory->get_name(); }
};
