#include "../src/SHM_Data_i8.hpp"
#include "../src/endian.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>

static constexpr std::size_t DATA_SIZE  = 256;
static constexpr std::size_t ITERATIONS = 16;

int main() {
    std::array<int8_t, DATA_SIZE> data {};
    data.fill(0);

    std::random_device         rd;
    std::default_random_engine re(rd());

    std::uniform_int_distribution<int8_t> uniform_dist(0);

    // create SHM_DATA_OBJECTS
    std::vector<std::unique_ptr<SHM_data>> shm_data;
    for (std::size_t i = 0; i < DATA_SIZE; ++i) {
        std::ostringstream name;
        name << "i8_" << i;
        shm_data.emplace_back(std::make_unique<SHM_Data_i8>(name.str(), data.data(), i));
    }

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "Iteration " << i + 1 << " / " << ITERATIONS << std::endl;
        for (auto &a : data) {
            a = uniform_dist(re);
        }

        for (auto &a : shm_data) {
            const auto json_data = a->get_data();
            const auto addr      = json_data["address"].get<std::size_t>();
            const auto value     = json_data["data"].get<int>();

            assert(addr < DATA_SIZE);

            int8_t mem_value = data[addr];

            static_cast<void>(mem_value);
            assert(value == mem_value);
        }
    }
}
