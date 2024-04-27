/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <utility>

class SHM_data {
private:
    const std::string name;

protected:
    const void *const data;
    const std::size_t address;

    SHM_data(std::string name, const void *data, std::size_t address)
        : name(std::move(name)), data(data), address(address) {}

public:
    virtual ~SHM_data()                   = default;
    SHM_data(const SHM_data &)            = delete;
    SHM_data(SHM_data &&)                 = delete;
    SHM_data &operator=(const SHM_data &) = delete;
    SHM_data &operator=(SHM_data &&)      = delete;

    [[nodiscard]] virtual nlohmann::json get_data() const = 0;

    [[nodiscard]] inline const std::string &get_name() const { return name; }

    [[nodiscard]] inline std::size_t get_address() const { return address; }

protected:
    [[nodiscard]] nlohmann::json get_json_skeleton() const;
};
