/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#include "SHM_Format.hpp"
#include "parse_config_file.hpp"

#include <fstream>

SHM_Format::SHM_Format(const std::string &shm_name, const std::string &config_file_path) {
    // open shared memory
    shared_memory = std::make_unique<cxxshm::SharedMemory>(shm_name, true);

    // shm data from config file
    std::ifstream cfg_file(config_file_path);
    if (!cfg_file.is_open())
        throw std::runtime_error(std::string("failed to open config file " + config_file_path + ": ") +
                                 strerror(errno));

    shm_data = parse_config_file(cfg_file, *shared_memory);
}

nlohmann::json SHM_Format::execute() const {
    nlohmann::json result;

    nlohmann::json data_list = nlohmann::json::array();
    for (auto &a : shm_data) {
        data_list += a->get_data();
    }

    result["data"] = data_list;

    nlohmann::json shm_json;
    shm_json["name"] = shared_memory->get_name();
    shm_json["size"] = shared_memory->get_size();
    result["shm"]    = shm_json;

    return result;
}
