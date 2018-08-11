#pragma once

#include <string>
#include <cstdint>

#define const_hash(s) det::fnv1a_32(s, sizeof(s) - 1)

namespace det {
    constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
}

constexpr std::uint32_t operator"" _hash(const char *s, std::size_t count) {
    return det::fnv1a_32(s, count);
}
