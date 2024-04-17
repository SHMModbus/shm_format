#
# Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
# This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

find_package(nlohmann_json REQUIRED)

function(add_format_test_target target)
    file(GLOB shm_data_src
            "../src/SHM_*.hpp"
            "../src/SHM_*.cpp"
    )
    add_executable(test_${target} test_${target}.cpp ${shm_data_src})
    add_test(test_${target} test_${target})
    enable_warnings(test_${target})
    set_definitions(test_${target})
    set_options(test_${target} FALSE)
    if(CLANG_FORMAT)
        target_clangformat_setup(test_${target})
    endif()
    target_link_libraries(test_${target} PRIVATE nlohmann_json)
endfunction()

#add_test_target(${Target} "")
#add_dependencies(test_${Target} ${Target})

add_format_test_target("shm_data_bool")
