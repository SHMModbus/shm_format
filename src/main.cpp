/*
 * Copyright (C) 2020-2022 Nikolas Koesling <nikolas@koesling.info>.
 * This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#include "generated/version_info.hpp"

#include <iostream>

#ifdef _OPENMP
#    include <omp.h>
#endif

int main() {
    // TODO implement
    std::cout << "PROJECT_VERSION: " << PROJECT_VERSION << '\n';
    std::cout << "PROJECT_VERSION_MAJOR: " << PROJECT_VERSION_MAJOR << '\n';
    std::cout << "PROJECT_VERSION_MINOR: " << PROJECT_VERSION_MINOR << '\n';
    std::cout << "PROJECT_VERSION_PATCH: " << PROJECT_VERSION_PATCH << '\n';
    std::cout << "PROJECT_NAME: " << PROJECT_NAME << '\n';
    std::cout << "COMPILER_INFO: " << COMPILER_INFO << '\n';
    std::cout << "SYSTEM_INFO: " << SYSTEM_INFO << '\n';
    std::cout << "CPU_WORD_BYTES: " << CPU_WORD_BYTES << '\n';
    std::cout << "GIT_HASH: " << RCS_HASH << '\n';
    std::cout << shm_format_version_info::NAME << ' ' << shm_format_version_info::VERSION_STR
              << ' ' << "Hello CMake!!!\n";
    std::cout << "git commit: " << shm_format_version_info::GIT_HASH << '\n';

#ifdef _OPENMP
#    pragma omp parallel default(none)
    { printf("Hello from thread: %d\n", omp_get_thread_num()); }
#endif
}
