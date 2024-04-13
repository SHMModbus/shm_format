/*
 * MIT License
 *
 * Copyright (c) 2023 Nikolas Koesling <nikolas@koesling.info>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>
#include <vector>

/*! \brief Split a string into a vector of strings
 *
 * @param string the string that shall be split
 * @param delimiter Specifies the separator to use when splitting the string (string of any length)
 * @param max_split optional: Specifies how many splits to do. (default: infinite from the point of view of possible
 * system resources)
 * @return split string as vector of strings
 */
[[nodiscard]] static inline std::vector<std::string> split_string(
        std::string string, const std::string &delimiter, std::size_t max_split = ~static_cast<std::size_t>(0)) {
    std::vector<std::string> split_string;  // result vector

    std::size_t pos = 0;
    while (max_split && ((pos = string.find(delimiter)) != std::string::npos)) {
        auto tmp_str = string.substr(0, pos);
        if (!tmp_str.empty()) split_string.emplace_back(tmp_str);
        string.erase(0, pos + delimiter.length());
        max_split--;
    }

    if (!string.empty()) split_string.emplace_back(string);

    return split_string;
}

/*! \brief Split a string into a vector of strings
 *
 * @param string the string that shall be split
 * @param delimiter Specifies the separator to use when splitting the string (single character)
 * @param max_split optional: Specifies how many splits to do. (default: infinite from the point of view of possible
 * system resources)
 * @return split string as vector of strings
 */
[[nodiscard]] static inline std::vector<std::string>
        split_string(const std::string &string, char delimiter, std::size_t max_split = ~static_cast<std::size_t>(0)) {
    return split_string(string, std::string(1, delimiter), max_split);
}
