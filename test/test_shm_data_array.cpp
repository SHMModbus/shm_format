#include "../src/SHM_Data_bytearray.hpp"
#include "../src/endian.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>

static constexpr std::size_t DATA_SIZE  = 1024;
static constexpr std::size_t ITERATIONS = 16;

int main() {
    std::array<uint8_t, DATA_SIZE> data {};
    data.fill(0);

    std::random_device         rd;
    std::default_random_engine re(rd());

    std::uniform_int_distribution<uint8_t> uniform_dist(0);

    SHM_Data_bytearray shm_data("array", data.data(), 0, DATA_SIZE);

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "Iteration " << i + 1 << " / " << ITERATIONS << std::endl;
        for (auto &a : data) {
            a = uniform_dist(re);
        }

        const auto json_data = shm_data.get_data();
        assert(json_data["address"].get<std::size_t>() == 0);
        auto values = json_data["data"];

        for (std::size_t j = 0; j < DATA_SIZE; ++j) {
            const auto value = values[j].get<uint8_t>();

            uint8_t mem_value = data[j];

            static_cast<void>(mem_value);
            assert(value == mem_value);
        }
    }
}
