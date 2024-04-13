#
# Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
# This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

add_executable(test_${Target} test.cpp)
add_dependencies(test_${Target} ${Target})
add_test(test_${Target} test_${Target})

enable_warnings(test_${Target})
set_definitions(test_${Target})
set_options(test_${Target} FALSE)

if(CLANG_FORMAT)
    target_clangformat_setup(test_${Target})
endif()
