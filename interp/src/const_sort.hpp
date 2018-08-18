#pragma once

#include <array>
#include <utility>

namespace det {
    template<typename T>
    constexpr void const_swap(T &l, T &r) {
        T tmp = std::move(l);
        l = std::move(r);
        r = std::move(tmp);
    }

    template<typename T, std::size_t N>
    constexpr void const_sort(std::array<T, N> &arr, std::size_t l, std::size_t r) {
        if (l < r) {
            std::size_t m = l;
            for (std::size_t i = l + 1; i < r; ++i) {
                if (arr[i] > arr[l]) {
                    const_swap(arr[++m], arr[i]);
                }
            }
            const_swap(arr[l], arr[m]);
            const_sort(arr, l, m);
            const_sort(arr, m + 1, r);
        }
    }
}

template<typename T, std::size_t N>
constexpr std::array<T, N> const_sort(std::array<T, N> arr) {
    auto sorted = move(arr);
    det::const_sort(sorted, 0, N);
    return sorted;
}
