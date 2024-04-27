//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#pragma once

#include "SHM_data.hpp"
#include <cxxshm.hpp>

std::vector<std::unique_ptr<SHM_data>> parse_config_file(std::istream &input_stream, const cxxshm::SharedMemory &shm);
