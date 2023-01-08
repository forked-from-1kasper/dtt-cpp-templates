#pragma once

#include <algorithm>
#include <cstring>

template<size_t N>
struct Literal {
    char unquote[N];
    constexpr Literal(const char (&decl)[N])
    { std::copy_n(decl, N, unquote); }
};

template<size_t N, size_t M>
constexpr bool operator==(const Literal<N> & s1, const Literal<M> & s2)
{ return !strcmp(s1.unquote, s2.unquote); }

template<bool B, typename T, typename U>
using If = std::conditional<B, T, U>::type;