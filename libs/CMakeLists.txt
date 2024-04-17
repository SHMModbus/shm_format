#
# Copyright (C) 2020-2024 Nikolas Koesling <nikolas@koesling.info>.
# This template is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

# ---------------------------------------- packages --------------------------------------------------------------------
# ======================================================================================================================
find_package(cxxshm REQUIRED)
find_package(cxxsemaphore REQUIRED)
find_package(cxxitimer REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json REQUIRED)

# ---------------------------------------- external libs ---------------------------------------------------------------
# ======================================================================================================================


# ---------------------------------------- subdirectories --------------------------------------------------------------
# ======================================================================================================================


# ---------------------------------------- link libraries --------------------------------------------------------------
# ======================================================================================================================
target_link_libraries(${Target} PRIVATE rt)
target_link_libraries(${Target} PRIVATE INTERFACE cxxopts)
target_link_libraries(${Target} PRIVATE cxxshm)
target_link_libraries(${Target} PRIVATE cxxitimer)
target_link_libraries(${Target} PRIVATE cxxsemaphore)
target_link_libraries(${Target} PRIVATE nlohmann_json)
