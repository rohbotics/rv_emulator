#pragma once

#include <type_traits>

// Adapted from http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
template <typename T, unsigned B>
constexpr inline static T signextend(const T x) {
    static_assert(sizeof(T) * 8 > B, "size of type must be larger than sign extended value");
    static_assert(std::is_signed<T>::value, "return must be of signed type or else this function invokes undefined "
                                            "behavior");
    struct {
        T x : B;
    } s = { 0 };
    return s.x = x;
}

