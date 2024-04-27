//
// Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
//

#include <iostream>

#include "parse_config_file.hpp"

#include "SHM_Data_bool.hpp"
#include "SHM_Data_bytearray.hpp"
#include "SHM_Data_f32b.hpp"
#include "SHM_Data_f32br.hpp"
#include "SHM_Data_f32l.hpp"
#include "SHM_Data_f32lr.hpp"
#include "SHM_Data_f64b.hpp"
#include "SHM_Data_f64br.hpp"
#include "SHM_Data_f64l.hpp"
#include "SHM_Data_f64lr.hpp"
#include "SHM_Data_i16b.hpp"
#include "SHM_Data_i16l.hpp"
#include "SHM_Data_i32b.hpp"
#include "SHM_Data_i32br.hpp"
#include "SHM_Data_i32l.hpp"
#include "SHM_Data_i32lr.hpp"
#include "SHM_Data_i64b.hpp"
#include "SHM_Data_i64br.hpp"
#include "SHM_Data_i64l.hpp"
#include "SHM_Data_i64lr.hpp"
#include "SHM_Data_i8.hpp"
#include "SHM_Data_string.hpp"
#include "SHM_Data_u16b.hpp"
#include "SHM_Data_u16l.hpp"
#include "SHM_Data_u32b.hpp"
#include "SHM_Data_u32br.hpp"
#include "SHM_Data_u32l.hpp"
#include "SHM_Data_u32lr.hpp"
#include "SHM_Data_u64b.hpp"
#include "SHM_Data_u64br.hpp"
#include "SHM_Data_u64l.hpp"
#include "SHM_Data_u64lr.hpp"
#include "SHM_Data_u8.hpp"

#include "split_string.hpp"

#include <cerrno>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

enum class types : uint8_t {
    UNDEFINED,
    BOOL,
    U8,
    I8,
    U16B,
    U16L,
    I16B,
    I16L,
    U32B,
    U32L,
    I32B,
    I32L,
    U32BR,
    U32LR,
    I32BR,
    I32LR,
    U64B,
    U64L,
    I64B,
    I64L,
    U64BR,
    U64LR,
    I64BR,
    I64LR,
    F32L,
    F32LR,
    F32B,
    F32BR,
    F64L,
    F64LR,
    F64B,
    F64BR,
    ARRAY,
    STRING
};

static const std::unordered_map<std::string, types> TYPE_MAP {
        // bool
        {"b", types::BOOL},
        {"bit", types::BOOL},
        {"bool", types::BOOL},

        // float
        {"f32_abcd", types::F32B},
        {"f32_cdab", types::F32BR},
        {"f32_badc", types::F32LR},
        {"f32_dcba", types::F32L},
        {"f32_little", types::F32L},
        {"f32l", types::F32L},
        {"f32_big", types::F32B},
        {"f32b", types::F32B},
        {"f32_little_rev", types::F32LR},
        {"f32lr", types::F32LR},
        {"f32_big_rev", types::F32BR},
        {"f32br", types::F32BR},

        // double
        {"f64_abcdefgh", types::F64B},
        {"f64_ghefcdab", types::F64BR},
        {"f64_badcfehg", types::F64LR},
        {"f64_hgfedcba", types::F64L},
        {"f64_little", types::F64L},
        {"f64l", types::F64L},
        {"f64_big", types::F64B},
        {"f64b", types::F64B},
        {"f64_little_rev", types::F64LR},
        {"f64lr", types::F64LR},
        {"f64_big_rev", types::F64BR},
        {"f64br", types::F64BR},

        // 8 bit integer
        {"u8", types::U8},
        {"i8", types::I8},

        // 16 bit integer
        {"u16_ab", types::I16B},
        {"u16_big", types::I16B},
        {"u16b", types::I16B},
        {"u16_ba", types::U16L},
        {"u16_little", types::U16L},
        {"u16l", types::U16L},
        {"i16_ab", types::U16B},
        {"i16_big", types::U16B},
        {"i16b", types::U16B},
        {"i16_ba", types::I16L},
        {"i16_little", types::I16L},
        {"i16l", types::I16L},

        // 32 bit integer
        {"u32_abcd", types::U32B},
        {"u32_cdab", types::U32BR},
        {"u32_badc", types::U32LR},
        {"u32_dcba", types::U32L},
        {"u32_little", types::U32L},
        {"u32l", types::U32L},
        {"u32_big", types::U32B},
        {"u32b", types::U32B},
        {"u32_little_rev", types::U32LR},
        {"u32lr", types::U32LR},
        {"u32_big_rev", types::U32BR},
        {"u32br", types::U32BR},
        {"i32_abcd", types::I32B},
        {"i32_cdab", types::I32BR},
        {"i32_badc", types::I32LR},
        {"i32_dcba", types::I32L},
        {"i32_little", types::I32L},
        {"i32l", types::I32L},
        {"i32_big", types::I32B},
        {"i32b", types::I32B},
        {"i32_little_rev", types::I32LR},
        {"i32lr", types::I32LR},
        {"i32_big_rev", types::I32BR},
        {"i32br", types::I32BR},

        // 64 bit integer
        {"u64_abcdefgh", types::U64B},
        {"u64_ghefcdab", types::U64BR},
        {"u64_badcfehg", types::U64LR},
        {"u64_hgfedcba", types::U64L},
        {"u64_little", types::U64L},
        {"u64l", types::U64L},
        {"u64_big", types::U64B},
        {"u64b", types::U64B},
        {"u64_little_rev", types::U64LR},
        {"u64lr", types::U64LR},
        {"u64_big_rev", types::U64BR},
        {"u64br", types::U64BR},
        {"i64_abcdefgh", types::I64B},
        {"i64_ghefcdab", types::I64BR},
        {"i64_badcfehg", types::I64LR},
        {"i64_hgfedcba", types::I64L},
        {"i64_little", types::I64L},
        {"i64l", types::I64L},
        {"i64_big", types::I64B},
        {"i64b", types::I64B},
        {"i64_little_rev", types::I64LR},
        {"i64lr", types::I64LR},
        {"i64_big_rev", types::I64BR},
        {"i64br", types::I64BR},
};


std::vector<std::unique_ptr<SHM_data>> parse_config_file(const std::string          &file_name,
                                                         const cxxshm::SharedMemory &shm) {
    std::ifstream cfg_file(file_name);
    if (!cfg_file.is_open()) throw std::runtime_error(std::string("failed to open config file: ") + strerror(errno));

    std::vector<std::unique_ptr<SHM_data>> result;
    std::unordered_set<std::string>        names;

    std::string line;
    std::size_t line_nr = 0;
    while (std::getline(cfg_file, line)) {
        ++line_nr;
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        auto split_line = split_string(line, ',', 3);

        if (split_line.size() < 2)
            throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                     ": to few delimiters");

        // parse address
        const auto  split_addr = split_string(split_line.at(0), ':', 1);
        std::size_t addr       = 0;
        const auto &addr_str   = split_addr.at(0);
        bool        fail       = false;
        std::size_t idx        = 0;
        try {
            addr = std::stoull(addr_str, &idx, 0);
        } catch (const std::exception &) { fail = true; }
        fail = fail || idx != addr_str.size();

        if (fail)
            throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                     ": failed to parse address: invalid address (" + addr_str + ')');

        // parse bit
        std::size_t bit = 0;
        if (split_addr.size() > 1) {
            const auto &bit_str = split_addr.at(1);
            fail                = false;
            idx                 = 0;
            try {
                bit = std::stoull(bit_str, &idx, 0);
            } catch (const std::exception &) { fail = true; }
            fail = fail || idx != bit_str.size();

            if (fail)
                throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                         ": failed to parse address: invalid bit (" + bit_str + ')');

            if (bit >= 8)
                throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                         ": failed to parse address: bit value > 7");
        }

        types       data_type     = types::UNDEFINED;
        std::size_t length        = 0;
        const auto &data_type_str = split_line.at(1);
        if (!TYPE_MAP.contains(data_type_str)) {
            static const std::vector<std::pair<std::string, types>> type_prefix_map = {
                    {"array", types::ARRAY},
                    {"string", types::STRING},
                    {"arr", types::ARRAY},
                    {"str", types::STRING},
                    {"a", types::ARRAY},
                    {"s", types::STRING},
            };

            std::size_t prefix_length = 0;
            for (const auto &a : type_prefix_map) {
                if (data_type_str.starts_with(a.first)) {
                    prefix_length = a.first.length();
                    data_type     = a.second;
                    break;
                }
            }

            if (data_type == types::UNDEFINED)
                throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                         ": unknown data type");

            // get size
            const auto length_str = data_type_str.substr(prefix_length);
            fail                  = false;
            idx                   = 0;
            try {
                length = std::stoull(length_str, &idx, 0);
            } catch (const std::exception &) { fail = true; }
            fail = fail || idx != length_str.size();

            if (fail)
                throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                         ": failed to parse data type: invalid length (" + length_str + ')');


        } else {
            data_type = TYPE_MAP.at(data_type_str);
        }

        // name: create if not specified
        std::string name;
        if (split_line.size() > 2) {
            name = split_line.at(2);
        } else {
            std::ostringstream name_str;
            name_str << "unnamed_" << line_nr << "_0x" << std::hex << addr;
            name = name_str.str();
        }

        // check name
        if (names.contains(name))
            throw std::runtime_error(std::string("invalid config file: line ") + std::to_string(line_nr) +
                                     ": duplicate name");

        const auto shm_size = shm.get_size();

        auto check_shm_size = [&](std::size_t size) {
            std::cerr << "check_size: size=" << size << " addr=" << addr << " shm_size=" << shm_size << std::endl;
            if (addr + size >= shm_size) {
                std::ostringstream err_msg;
                err_msg << "invalid config file: line " << line_nr << ": shared memory " << shm.get_name()
                        << " is to small to read " << size << " Byte at address 0x" << std::hex << addr;
                throw std::runtime_error(err_msg.str());
            }
        };

        switch (data_type) {
            case types::BOOL:
                check_shm_size(sizeof(uint8_t));
                result.emplace_back(std::make_unique<SHM_Data_bool>(name, shm.get_addr(), addr, bit));
                break;
            case types::U8:
                check_shm_size(sizeof(uint8_t));
                result.emplace_back(std::make_unique<SHM_Data_u8>(name, shm.get_addr(), addr));
                break;
            case types::I8:
                check_shm_size(sizeof(uint8_t));
                result.emplace_back(std::make_unique<SHM_Data_i8>(name, shm.get_addr(), addr));
                break;
            case types::U16B:
                check_shm_size(sizeof(uint16_t));
                result.emplace_back(std::make_unique<SHM_Data_u16b>(name, shm.get_addr(), addr));
                break;
            case types::U16L:
                check_shm_size(sizeof(uint16_t));
                result.emplace_back(std::make_unique<SHM_Data_u16l>(name, shm.get_addr(), addr));
                break;
            case types::I16B:
                check_shm_size(sizeof(uint16_t));
                result.emplace_back(std::make_unique<SHM_Data_i16b>(name, shm.get_addr(), addr));
                break;
            case types::I16L:
                check_shm_size(sizeof(uint16_t));
                result.emplace_back(std::make_unique<SHM_Data_i16l>(name, shm.get_addr(), addr));
                break;
            case types::U32B:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_u32b>(name, shm.get_addr(), addr));
                break;
            case types::U32L:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_u32l>(name, shm.get_addr(), addr));
                break;
            case types::I32B:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_i32b>(name, shm.get_addr(), addr));
                break;
            case types::I32L:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_i32l>(name, shm.get_addr(), addr));
                break;
            case types::U32BR:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_u32br>(name, shm.get_addr(), addr));
                break;
            case types::U32LR:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_u32lr>(name, shm.get_addr(), addr));
                break;
            case types::I32BR:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_i32br>(name, shm.get_addr(), addr));
                break;
            case types::I32LR:
                check_shm_size(sizeof(uint32_t));
                result.emplace_back(std::make_unique<SHM_Data_i32lr>(name, shm.get_addr(), addr));
                break;
            case types::U64B:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_u64b>(name, shm.get_addr(), addr));
                break;
            case types::U64L:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_u64l>(name, shm.get_addr(), addr));
                break;
            case types::I64B:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_i64b>(name, shm.get_addr(), addr));
                break;
            case types::I64L:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_i64l>(name, shm.get_addr(), addr));
                break;
            case types::U64BR:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_u64br>(name, shm.get_addr(), addr));
                break;
            case types::U64LR:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_u64lr>(name, shm.get_addr(), addr));
                break;
            case types::I64BR:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_i64br>(name, shm.get_addr(), addr));
                break;
            case types::I64LR:
                check_shm_size(sizeof(uint64_t));
                result.emplace_back(std::make_unique<SHM_Data_i64lr>(name, shm.get_addr(), addr));
                break;
            case types::F32L:
                check_shm_size(sizeof(float));
                result.emplace_back(std::make_unique<SHM_Data_f32l>(name, shm.get_addr(), addr));
                break;
            case types::F32LR:
                check_shm_size(sizeof(float));
                result.emplace_back(std::make_unique<SHM_Data_f32lr>(name, shm.get_addr(), addr));
                break;
            case types::F32B:
                check_shm_size(sizeof(float));
                result.emplace_back(std::make_unique<SHM_Data_f32b>(name, shm.get_addr(), addr));
                break;
            case types::F32BR:
                check_shm_size(sizeof(float));
                result.emplace_back(std::make_unique<SHM_Data_f32br>(name, shm.get_addr(), addr));
                break;
            case types::F64L:
                check_shm_size(sizeof(double));
                result.emplace_back(std::make_unique<SHM_Data_f64l>(name, shm.get_addr(), addr));
                break;
            case types::F64LR:
                check_shm_size(sizeof(double));
                result.emplace_back(std::make_unique<SHM_Data_f64lr>(name, shm.get_addr(), addr));
                break;
            case types::F64B:
                check_shm_size(sizeof(double));
                result.emplace_back(std::make_unique<SHM_Data_f64b>(name, shm.get_addr(), addr));
                break;
            case types::F64BR:
                check_shm_size(sizeof(double));
                result.emplace_back(std::make_unique<SHM_Data_f64br>(name, shm.get_addr(), addr));
                break;
            case types::ARRAY:
                check_shm_size(length);
                result.emplace_back(std::make_unique<SHM_Data_bytearray>(name, shm.get_addr(), addr, length));
                break;
            case types::STRING:
                result.emplace_back(std::make_unique<SHM_Data_string>(name, shm.get_addr(), addr, length));
                break;
            case types::UNDEFINED: throw std::runtime_error("internal error: unexpected data type: UNKNOWN");
            default: throw std::runtime_error("internal error: unexpected data type");  // NOLINT
        }
    }

    return result;
}
