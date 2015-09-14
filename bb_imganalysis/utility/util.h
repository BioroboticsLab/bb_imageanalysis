#pragma once

#include <cstdint>
#include <vector>

namespace util {
// branchless, type-safe signum
// see: http://stackoverflow.com/a/4609795
template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T>
std::vector<T> linspace(T first, T last, size_t len) {
    std::vector<T> result(len);
    T step = (last-first) / (len - 1);
	for (size_t i=0; i<len; i++) { result[i] = first + static_cast<T>(i) * step; }
    return result;
}
}
