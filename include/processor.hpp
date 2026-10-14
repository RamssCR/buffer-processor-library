#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace CoreEngine {
    class BufferEncrypter {
    public:
        struct Result {
            std::vector<std::uint8_t> data{};
            std::uint32_t checksum{};
        };

        static Result convert(const uint8_t* data, std::size_t size, std::uint8_t xor_key);
    };
}