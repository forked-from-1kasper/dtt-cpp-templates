#pragma once

#include "Eval.hpp"
#include "Conv.hpp"

namespace Value {

template<typename, Val> struct InferM;
template<typename Ctx, Val T> using Infer = InferM<Ctx, T>::value;

template<typename Ctx, Nat N> struct InferM<Ctx, Type<N>>
{ using value = Type<Succ<N>>; };

template<typename Ctx, Integer n> struct InferM<Ctx, Var<n>>
{ using value = Nth<n, Ctx>; };

template<typename Ctx, Literal x> struct InferM<Ctx, Const<x>>
{ using value = Lookup<x>; };

template<typename Ctx, Val T, Val U>
struct InferM<Ctx, Lam<T, U>> {
    static_assert(IsType<Infer<Ctx, T>>); using τ = Eval<T>;
    using value = Pi<τ, Infer<Map<Up, Cons<τ, Ctx>>, U>>;
};

template<typename Ctx, Val T, Val U>
struct InferM<Ctx, Pi<T, U>> {
    using τ₁ = Infer<Ctx, T>; static_assert(IsType<τ₁>);
    using τ₂ = Infer<Map<Up, Cons<Eval<T>, Ctx>>, U>; static_assert(IsType<τ₂>);
    using value = Type<Max<typename τ₁::level, typename τ₂::level>>;
};

template<typename Ctx, Val T, Val U>
struct InferM<Ctx, App<T, U>> {
    using τ = Infer<Ctx, T>; static_assert(IsPiType<τ>);
    static_assert(Conv<typename τ::dom, Infer<Ctx, U>>);
    using value = Eval<App<typename τ::lambda, U>>;
};

template<typename T, typename U> struct CheckM {
    static_assert(IsType<Infer<List<>, U>>);
    constexpr static bool value = Conv<Infer<List<>, T>, Eval<U>>;
};

template<typename T, typename U> constexpr bool Check = CheckM<T, U>::value;

}