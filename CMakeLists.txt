#
# Copyright (C) 2020-2024 Nikolas Koesling <nikolas@koesling.info>.
# This template is free software. You can redistribute it and/or modify it under the terms of the MIT License.
#

cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

# ----------------------------------------------- User settings --------------------------------------------------------
# ======================================================================================================================

# project
project(shm-format LANGUAGES CXX VERSION 2.1.0)

# settings
set(Target "shm-format")    # Executable name (without file extension!)
set(STANDARD 20)                  # C++ Standard
set(ARCHITECTURE "native")        # CPU architecture to optimize for (only relevant if OPTIMIZE_FOR_ARCHITECTURE is ON)

# options
option(BUILD_DOC "Build documentation" ON)
option(COMPILER_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_MULTITHREADING "Link the default multithreading library for the current target system" OFF)
option(MAKE_32_BIT_BINARY "Compile as 32 bit application. No effect on 32 bit Systems" OFF)
option(OPENMP "enable openmp" OFF)
option(OPTIMIZE_DEBUG "apply optimizations also in debug mode" ON)
option(CLANG_FORMAT "use clang-format" ON)
option(CLANG_TIDY "use clang-tidy" OFF)
option(CLANG_TIDY_NO_ERRORS "do not treat clang-tidy warnings as errors" ON)
option(OPTIMIZE_FOR_ARCHITECTURE "enable optimizations for specified architecture" OFF)
option(LTO_ENABLED "enable interprocedural and link time optimizations" ON)
option(COMPILER_EXTENSIONS "enable compiler specific C++ extensions" OFF)
option(ENABLE_TEST "enable test builds" ON)

# ======================================================================================================================
# ======================================================================================================================


# ----------------------------------------------- Do not change --------------------------------------------------------
# ======================================================================================================================
include(cmake_files/setup.cmake)
