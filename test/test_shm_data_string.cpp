#include "../src/SHM_Data_string.hpp"
#include "../src/endian.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>

static constexpr std::size_t DATA_SIZE  = 4096;
static constexpr std::size_t ITERATIONS = 4096;

int main() {
    std::array<uint8_t, DATA_SIZE> data {};
    data.fill(0);

    std::random_device         rd;
    std::default_random_engine re(rd());

    std::uniform_int_distribution<uint8_t> uniform_dist(' ', '~');

    SHM_Data_string shm_data("array", data.data(), 0, DATA_SIZE);

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "Iteration " << i + 1 << " / " << ITERATIONS << std::endl;
        for (auto &a : data) {
            a = uniform_dist(re);
        }

        std::string str(reinterpret_cast<const char *>(data.data()), DATA_SIZE);

        const auto json_data = shm_data.get_data();
        assert(json_data["address"].get<std::size_t>() == 0);
        auto value = json_data["data"].get<std::string>();

        assert(value == str);
    }
}
