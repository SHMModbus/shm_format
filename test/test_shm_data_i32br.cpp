#include "../src/SHM_Data_i32br.hpp"
#include "../src/endian.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

static constexpr std::size_t DATA_SIZE  = 256;
static constexpr std::size_t ITERATIONS = 32;
#define ENDIAN_A endian::host_to_big
#define ENDIAN_B endian::big_to_host
typedef int32_t        data_type;
typedef SHM_Data_i32br SHM_data_type;
static const char     *name_prefix = "i32br_";


int main() {
    std::array<data_type, DATA_SIZE> data {};
    data.fill(0);

    std::random_device         rd;
    std::default_random_engine re(rd());

    std::uniform_int_distribution<data_type> uniform_dist(0);

    // create SHM_DATA_OBJECTS
    std::vector<std::unique_ptr<SHM_data>> shm_data;
    for (std::size_t i = 0; i < DATA_SIZE; ++i) {
        std::ostringstream name;
        name << name_prefix << i;
        shm_data.emplace_back(std::make_unique<SHM_data_type>(name.str(), data.data(), i * sizeof(data_type)));
    }

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "Iteration " << i + 1 << " / " << ITERATIONS << std::endl;
        std::vector<data_type> data_tmp;
        data_tmp.reserve(DATA_SIZE);
        for (auto &a : data) {
            union {
                data_type d;
                uint16_t  reg[2];
            };
            d = ENDIAN_A(uniform_dist(re));
            data_tmp.emplace_back(d);
            std::swap(reg[0], reg[1]);
            a = d;
        }

        for (auto &a : shm_data) {
            const auto json_data = a->get_data();
            const auto addr      = json_data["address"].get<std::size_t>();
            static_assert(sizeof(long) > sizeof(data_type));
            static_assert(sizeof(long) > sizeof(data_type));
            const auto value = json_data["data"].get<long>();

            assert(addr < DATA_SIZE * sizeof(data_type));

            data_type mem_value = ENDIAN_B(data_tmp[addr / sizeof(data_type)]);

            static_cast<void>(mem_value);
            std::cerr << std::hex << "value=" << value << " mem_value=" << mem_value << " addr=" << addr << std::endl;
            assert(value == mem_value);
        }
    }
}
