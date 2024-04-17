#include "../src/SHM_Data_bool.hpp"
#include "../src/endian.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>

static constexpr std::size_t DATA_SIZE  = 16;
static constexpr std::size_t ITERATIONS = 16;

int main() {
    std::array<uint8_t, DATA_SIZE> data {};
    data.fill(0);

    std::random_device         rd;
    std::default_random_engine re(rd());

    std::uniform_int_distribution<uint8_t> uniform_dist(0);

    // create SHM_DATA_OBJECTS
    std::vector<std::unique_ptr<SHM_data>> shm_data;
    for (std::size_t i = 0; i < DATA_SIZE; ++i) {
        for (std::size_t bit = 0; bit < 8; ++bit) {
            std::ostringstream name;
            name << "bool_" << i << "_" << bit;
            shm_data.emplace_back(std::make_unique<SHM_Data_bool>(name.str(), data.data(), i, bit));
        }
    }

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "Iteration " << i + 1 << " / " << ITERATIONS << std::endl;
        for (auto &a : data) {
            a = uniform_dist(re);
        }

        for (auto &a : shm_data) {
            const auto json_data = a->get_data();
            const auto addr      = json_data["address"].get<std::size_t>();
            const auto bit       = json_data["bit"].get<std::size_t>();
            const auto value     = json_data["data"].get<bool>();

            assert(addr < DATA_SIZE);
            assert(bit < 8);

            uint8_t mem_byte  = data[addr];
            bool    mem_value = (mem_byte >> bit) & 0b1;

            static_cast<void>(mem_value);
            assert(value == mem_value);
        }
    }
}
