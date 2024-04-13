/*
 * Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <cstddef>
#include <cstdint>

static_assert(sizeof(uint8_t) == 1);

namespace endian {

/**
 * @brief endianness detection class
 */
constexpr static struct {
private:
    const union EndiannessDetector {
        int  i;
        char c[sizeof(int)];

        constexpr EndiannessDetector() : i(1) {}
    } e;

public:
    [[maybe_unused, nodiscard]] constexpr bool isBig() const { return e.c[0] == 0; }
    [[maybe_unused, nodiscard]] constexpr bool isLittle() const { return e.c[0] != 0; }
} HostEndianness;

/**
 * @brief swap endianness
 * @tparam T data type
 * @param i input
 * @return swapped endianness
 */
template <typename T>
[[maybe_unused]] static T swap(const T &i) {
    T ret;

    auto *dst = reinterpret_cast<uint8_t *>(&ret);
    auto *src = reinterpret_cast<const uint8_t *>(&i + 1);

    for (std::size_t j = 0; j < sizeof(T); j++)
        *dst++ = *--src;

    return ret;
}

/**
 * @brief convert big endian to little endian
 * @details alias for swap
 * @tparam T data type
 * @param b big endian
 * @return little endian
 */
template <typename T>
[[maybe_unused]] static T big_to_little(const T &b) {
    return swap(b);
}

/**
 * @brief convert little endian to big endian
 * @details alias for swap
 * @tparam T data type
 * @param l little endian
 * @return big endian
 */
template <typename T>
[[maybe_unused]] static T little_to_big(const T &l) {
    return swap(l);
}

/**
 * @brief convert host endian to big endian
 * @tparam T data type
 * @param b host endian
 * @return big endian
 */
template <typename T>
[[maybe_unused]] static T host_to_big(const T &b) {
    return HostEndianness.isLittle() ? swap(b) : b;
}

/**
 * @brief convert host endian to little endian
 * @tparam T data type
 * @param h host endian
 * @return little endian
 */
template <typename T>
[[maybe_unused]] static T host_to_little(const T &h) {
    return HostEndianness.isBig() ? swap(h) : h;
}

/**
 * @brief convert big endian to host endian
 * @tparam T data type
 * @param b big endian
 * @return host endian
 */
template <typename T>
[[maybe_unused]] static T big_to_host(const T &b) {
    return HostEndianness.isLittle() ? swap(b) : b;
}

/**
 * @brief convert little endian to host endian
 * @tparam T data type
 * @param l little endian
 * @return host endian
 */
template <typename T>
[[maybe_unused]] static T little_to_host(const T &l) {
    return HostEndianness.isBig() ? swap(l) : l;
}

/**
 * @brief convert big endian to little endian
 * @details alias for big_to_little
 * @tparam T data type
 * @param b big endian
 * @return little endian
 */
template <typename T>
[[maybe_unused]] static T bl(const T &b) {
    return big_to_little(b);
}

/**
 * @brief convert little endian to big endian
 * @details alias for little_to_big
 * @tparam T data type
 * @param l little endian
 * @return big endian
 */
template <typename T>
[[maybe_unused]] static T lb(const T &l) {
    return little_to_big(l);
}

/**
 * @brief convert host endian to big endian
 * @details alias for host_to_big
 * @tparam T data type
 * @param b host endian
 * @return big endian
 */
template <typename T>
[[maybe_unused]] static T hb(const T &b) {
    return host_to_big(b);
}

/**
 * @brief convert host endian to little endian
 * @details alias for host_to_little
 * @tparam T data type
 * @param h host endian
 * @return little endian
 */
template <typename T>
[[maybe_unused]] static T hl(const T &h) {
    return host_to_little(h);
}

/**
 * @brief convert big endian to host endian
 * @details alias for big_to_host
 * @tparam T data type
 * @param b big endian
 * @return host endian
 */
template <typename T>
[[maybe_unused]] static T bh(const T &b) {
    return big_to_host(b);
}

/**
 * @brief convert little endian to host endian
 * @details alias for little_to_host
 * @tparam T data type
 * @param l little endian
 * @return host endian
 */
template <typename T>
[[maybe_unused]] static T lh(const T &l) {
    return little_to_host(l);
}

}  // namespace endian
