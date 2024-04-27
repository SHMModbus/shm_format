#include "../src/generated/version_info.hpp"
#include "../src/parse_config_file.hpp"

#include "../src/SHM_Data_bool.hpp"
#include "../src/SHM_Data_bytearray.hpp"
#include "../src/SHM_Data_f32b.hpp"
#include "../src/SHM_Data_f32br.hpp"
#include "../src/SHM_Data_f32l.hpp"
#include "../src/SHM_Data_f32lr.hpp"
#include "../src/SHM_Data_f64b.hpp"
#include "../src/SHM_Data_f64br.hpp"
#include "../src/SHM_Data_f64l.hpp"
#include "../src/SHM_Data_f64lr.hpp"
#include "../src/SHM_Data_i16b.hpp"
#include "../src/SHM_Data_i16l.hpp"
#include "../src/SHM_Data_i32b.hpp"
#include "../src/SHM_Data_i32br.hpp"
#include "../src/SHM_Data_i32l.hpp"
#include "../src/SHM_Data_i32lr.hpp"
#include "../src/SHM_Data_i64b.hpp"
#include "../src/SHM_Data_i64br.hpp"
#include "../src/SHM_Data_i64l.hpp"
#include "../src/SHM_Data_i64lr.hpp"
#include "../src/SHM_Data_i8.hpp"
#include "../src/SHM_Data_string.hpp"
#include "../src/SHM_Data_u16b.hpp"
#include "../src/SHM_Data_u16l.hpp"
#include "../src/SHM_Data_u32b.hpp"
#include "../src/SHM_Data_u32br.hpp"
#include "../src/SHM_Data_u32l.hpp"
#include "../src/SHM_Data_u32lr.hpp"
#include "../src/SHM_Data_u64b.hpp"
#include "../src/SHM_Data_u64br.hpp"
#include "../src/SHM_Data_u64l.hpp"
#include "../src/SHM_Data_u64lr.hpp"
#include "../src/SHM_Data_u8.hpp"

#include <cassert>
#include <cxxshm.hpp>
#include <sstream>
#include <string>

int main() {
    cxxshm::SharedMemory shm("shm_format_test_config_parser" + shm_format_version_info::GIT_HASH, 256, false, false);

    const std::string test_data = "0:0,bool,Bool\n"

                                  "1,i8\n"
                                  "2,u8\n"

                                  "03,i16b\n"
                                  "4,i16l\n"
                                  "5,u16b\n"
                                  "0x6,u16l\n"

                                  "07,i32b\n"
                                  "8,i32br\n"
                                  "9,i32l\n"
                                  "0xA,i32lr\n"
                                  "11,u32b\n"
                                  "12,u32br\n"
                                  "13,u32l\n"
                                  "14,u32lr\n"

                                  "15,i64b\n"
                                  "16,i64br\n"
                                  "17,i64l\n"
                                  "18,i64lr\n"
                                  "19,u64b\n"
                                  "20,u64br\n"
                                  "21,u64l\n"
                                  "22,u64lr\n"

                                  "23,f32b\n"
                                  "24,f32br\n"
                                  "25,f32l\n"
                                  "26,f32lr\n"

                                  "27,f64b,\n"
                                  "28,f64br,\n"
                                  "29,f64l,\n"
                                  "30,f64lr,\n"

                                  "31:1,bool,Bool1\n"
                                  "32:2,bool,Bool2\n"
                                  "33:3,bool,Bool3\n"
                                  "34:4,bool,Bool4\n"
                                  "35:5,bool,Bool5\n"
                                  "36:6,bool,Bool6\n"
                                  "37:7,bool,Bool7\n"

                                  "38,i16_big\n"
                                  "39,i16_little\n"
                                  "40,u16_big\n"
                                  "41,u16_little\n"

                                  "42,i16_ab\n"
                                  "43,i16_ba\n"
                                  "44,u16_ab\n"
                                  "45,u16_ba\n"

                                  "46,i32_big\n"
                                  "47,i32_big_rev\n"
                                  "48,i32_little\n"
                                  "49,i32_little_rev\n"
                                  "50,u32_big\n"
                                  "51,u32_big_rev\n"
                                  "52,u32_little\n"
                                  "53,u32_little_rev\n"

                                  "54,i32_abcd\n"
                                  "55,i32_cdab\n"
                                  "56,i32_dcba\n"
                                  "57,i32_badc\n"
                                  "58,u32_abcd\n"
                                  "59,u32_cdab\n"
                                  "60,u32_dcba\n"
                                  "61,u32_badc\n"

                                  "62,i64_big\n"
                                  "63,i64_big_rev\n"
                                  "64,i64_little\n"
                                  "65,i64_little_rev\n"
                                  "66,u64_big\n"
                                  "67,u64_big_rev\n"
                                  "68,u64_little\n"
                                  "69,u64_little_rev\n"

                                  "70,i64_abcdefgh\n"
                                  "71,i64_ghefcdab\n"
                                  "72,i64_hgfedcba\n"
                                  "73,i64_badcfehg\n"
                                  "74,u64_abcdefgh\n"
                                  "75,u64_ghefcdab\n"
                                  "76,u64_hgfedcba\n"
                                  "77,u64_badcfehg\n"

                                  "78,a16\n"
                                  "79,arr32\n"
                                  "80,array64\n"

                                  "81,s16\n"
                                  "82,str32\n"
                                  "83,string64\n"

                                  "";


    // TODO: bytearray
    // TODO: string

    std::istringstream stream(test_data);

    auto parse_result = parse_config_file(stream, shm);

    assert(parse_result.size() == 84);

    std::size_t i = 0;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 0);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_name() == "Bool");
    ++i;

    assert(dynamic_cast<SHM_Data_i8 &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u8 &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_i16b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i16l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u16b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u16l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_i32b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i32br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i32l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i32lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u32b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u32br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u32l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u32lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_i64b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i64br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i64l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_i64lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u64b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u64br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u64l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_u64lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_f32b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f32br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f32l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f32lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_f64b &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f64br &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f64l &>(*parse_result.at(i).get()).get_address() == i);
    ++i;
    assert(dynamic_cast<SHM_Data_f64lr &>(*parse_result.at(i).get()).get_address() == i);
    ++i;

    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 1);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 2);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 3);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 4);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 5);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 6);
    ++i;
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bool &>(*parse_result.at(i).get()).get_bit() == 7);
    ++i;

    for (std::size_t j = 0; j < 2; ++j) {
        assert(dynamic_cast<SHM_Data_i16b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i16l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u16b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u16l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
    }

    for (std::size_t j = 0; j < 2; ++j) {
        assert(dynamic_cast<SHM_Data_i32b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i32br &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i32l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i32lr &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u32b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u32br &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u32l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u32lr &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
    }

    for (std::size_t j = 0; j < 2; ++j) {
        assert(dynamic_cast<SHM_Data_i64b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i64br &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i64l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_i64lr &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u64b &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u64br &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u64l &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
        assert(dynamic_cast<SHM_Data_u64lr &>(*parse_result.at(i).get()).get_address() == i);
        ++i;
    }

    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_length() == 16);
    ++i;
    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_length() == 32);
    ++i;
    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_bytearray &>(*parse_result.at(i).get()).get_length() == 64);
    ++i;

    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_length() == 16);
    ++i;
    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_length() == 32);
    ++i;
    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_address() == i);
    assert(dynamic_cast<SHM_Data_string &>(*parse_result.at(i).get()).get_length() == 64);
    ++i;
}
