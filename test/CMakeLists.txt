#
# Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
# This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

find_package(nlohmann_json REQUIRED)

function(add_format_test_target target)
    add_executable(test_${target} test_${target}.cpp)
    add_test(test_${target} test_${target})
    enable_warnings(test_${target})
    set_definitions(test_${target})
    set_options(test_${target} FALSE)
    if (CLANG_FORMAT)
        target_clangformat_setup(test_${target})
    endif ()
    target_link_libraries(test_${target} PRIVATE nlohmann_json test_formatter)
endfunction()

file(GLOB shm_data_src
            "../src/SHM_*.hpp"
            "../src/SHM_*.cpp"
    )
add_library(test_formatter ${shm_data_src})

#add_test_target(${Target} "")
#add_dependencies(test_${Target} ${Target})

add_format_test_target(shm_data_bool)
add_format_test_target(shm_data_u8)
add_format_test_target(shm_data_i8)
add_format_test_target(shm_data_u16l)
add_format_test_target(shm_data_u16b)
add_format_test_target(shm_data_i16l)
add_format_test_target(shm_data_i16b)
add_format_test_target(shm_data_u32l)
add_format_test_target(shm_data_u32lr)
add_format_test_target(shm_data_u32b)
add_format_test_target(shm_data_u32br)
add_format_test_target(shm_data_i32l)
add_format_test_target(shm_data_i32lr)
add_format_test_target(shm_data_i32b)
add_format_test_target(shm_data_i32br)
add_format_test_target(shm_data_u64l)
add_format_test_target(shm_data_u64b)
add_format_test_target(shm_data_i64l)
add_format_test_target(shm_data_i64b)
add_format_test_target(shm_data_u64lr)
add_format_test_target(shm_data_u64br)
add_format_test_target(shm_data_i64lr)
add_format_test_target(shm_data_i64br)
add_format_test_target(shm_data_array)
add_format_test_target(shm_data_string)

# TODO test config parser
