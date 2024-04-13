/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */

#pragma once

#include <iostream>

static void print_data_type_info() {
    std::cout << "Supported data types:" << '\n';
    std::cout << "  - b, bit, bool ................................. 1-Bit bool" << '\n';
    std::cout << "  - Float:" << '\n';
    std::cout << "      - 32 Bit:" << '\n';
    std::cout << "          - f32_abcd, f32_big, f32b .............. 32-Bit floating point   in big endian" << '\n';
    std::cout << "          - f32_dcba, f32_little, f32l ........... 32-Bit floating point   in little endian" << '\n';
    std::cout << "          - f32_cdab, f32_big_rev, f32br ......... 32-Bit floating point   in big endian,     - "
                 "reversed register order"
              << '\n';
    std::cout << "          - f32_badc, f32_little_rev, f32lr ...... 32-Bit floating point   in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << "      - 64 Bit:" << '\n';
    std::cout << "          - f64_abcdefgh, f64_big, f64b .......... 64-Bit floating point   in big endian" << '\n';
    std::cout << "          - f64_ghefcdab, f64_little, f64l ....... 64-Bit floating point   in little endian" << '\n';
    std::cout << "          - f64_badcfehg, f64_big_rev, f64br ..... 64-Bit floating point   in big endian,     - "
                 "reversed register order"
              << '\n';
    std::cout << "          - f64_hgfedcba, f64_little_rev, f64lr .. 64-Bit floating point   in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << "  - Int:" << '\n';
    std::cout << "      - 8 Bit:" << '\n';
    std::cout << "          - u8 ................................... 8-Bit unsigned integer" << '\n';
    std::cout << "          - i8 ................................... 8-Bit   signed integer" << '\n';
    std::cout << "      - 16 Bit" << '\n';
    std::cout << "          - u16_ab, u16_big, u16b ................ 16-Bit unsigned integer in big endian" << '\n';
    std::cout << "          - i16_ab, i16_big, i16b ................ 16-Bit signed integer   in big endian" << '\n';
    std::cout << "          - u16_ba, u16_little, u16l ............. 16-Bit unsigned integer in little endian" << '\n';
    std::cout << "          - i16_ba, i16_little, i16l ............. 16-Bit signed integer   in little endian" << '\n';
    std::cout << "      - 32 Bit:" << '\n';
    std::cout << "          - u32_abcd, u32_big, u32b .............. 32-Bit unsigned integer in big endian" << '\n';
    std::cout << "          - i32_abcd, i32_big, i32b .............. 32-Bit   signed integer in big endian" << '\n';
    std::cout << "          - u32_dcba, u32_little, u32l ........... 32-Bit unsigned integer in little endian" << '\n';
    std::cout << "          - i32_dcba, i32_little, i32l ........... 32-Bit   signed integer in little endian" << '\n';
    std::cout << "          - u32_cdab, u32_big_rev, u32br ......... 32-Bit unsigned integer in big endian,     - "
                 "reversed register order"
              << '\n';
    std::cout << "          - i32_cdab, i32_big_rev, i32br ......... 32-Bit   signed integer in big endian,     - "
                 "reversed register order"
              << '\n';
    std::cout << "          - u32_badc, u32_little_rev, u32lr ...... 32-Bit unsigned integer in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << "          - i32_badc, i32_little_rev, i32lr ...... 32-Bit   signed integer in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << "      - 64 Bit:" << '\n';
    std::cout << "          - u64_abcdefgh, u64_big, u64b .......... 64-Bit unsigned integer in big endian" << '\n';
    std::cout << "          - i64_abcdefgh, i64_big, i64b .......... 64-Bit   signed integer in big endian" << '\n';
    std::cout << "          - u64_hgfedcba, u64_little, u64l ....... 64-Bit unsigned integer in little endian" << '\n';
    std::cout << "          - i64_hgfedcba, i64_little, i64l ....... 64-Bit   signed integer in little endian" << '\n';
    std::cout << "          - u64_ghefcdab, u64_big_rev, u64br ..... 64-Bit unsigned integer in big endian      - "
                 "reversed register order"
              << '\n';
    std::cout << "          - i64_ghefcdab, i64_big_rev, i64br ..... 64-Bit   signed integer in big endian      - "
                 "reversed register order"
              << '\n';
    std::cout << "          - u64_badcfehg, u64_little_rev, u64lr .. 64-Bit unsigned integer in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << "          - i64_badcfehg, i64_little_rev, i64lr .. 64-Bit   signed integer in little endian,  - "
                 "reversed register order"
              << '\n';
    std::cout << std::flush;
}
