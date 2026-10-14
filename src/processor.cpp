#include "processor.hpp"
#include <iostream>

namespace CoreEngine {
    BufferEncrypter::Result BufferEncrypter::convert(
        const uint8_t* data,
        const std::size_t size,
        const std::uint8_t xor_key
    ) {
        std::vector<std::uint8_t> result{};
        result.resize(size);

        std::uint32_t hash{2166136261u};
        std::size_t i{0};

        static constexpr auto fnv_prime_number{16777619u};

        for (; i + 3 < size; i += 4) {
            result[i + 0] = data[i + 0] ^ xor_key;
            result[i + 1] = data[i + 1] ^ xor_key;
            result[i + 2] = data[i + 2] ^ xor_key;
            result[i + 3] = data[i + 3] ^ xor_key;

            hash = (hash ^ result[i + 0]) * fnv_prime_number;
            hash = (hash ^ result[i + 1]) * fnv_prime_number;
            hash = (hash ^ result[i + 2]) * fnv_prime_number;
            hash = (hash ^ result[i + 3]) * fnv_prime_number;
        }

        for (; i < size; ++i) {
            result[i] = data[i] ^ xor_key;
            hash = (hash ^ result[i]) * fnv_prime_number;
        }

        return Result{std::move(result),hash};
    }
}
