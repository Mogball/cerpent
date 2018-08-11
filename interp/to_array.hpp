#pragma once
#include <array>
#include <utility>

namespace det {
    template<typename T, std::size_t N, std::size_t... I>
    constexpr std::array<std::remove_cv_t<T>, N>
    to_array(T (&a)[N], std::index_sequence<I...>) {
        return { {a[I]...} };
    }
}

template<typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
    return det::to_array(a, std::make_index_sequence<N>{});
}
