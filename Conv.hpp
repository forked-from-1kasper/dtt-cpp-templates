#pragma once

#include "Val.hpp"

namespace Value {

template<Val, Val> struct ConvM { constexpr static bool value = false; };
template<Val T, Val U> constexpr bool Conv = ConvM<T, U>::value;

template<Nat N, Nat M> struct ConvM<Type<N>, Type<M>>
{ constexpr static bool value = std::same_as<N, M>; };

template<Integer i, Integer j> struct ConvM<Var<i>, Var<j>>
{ constexpr static bool value = (i == j); };

template<Literal x₁, Literal x₂> struct ConvM<Const<x₁>, Const<x₂>>
{ constexpr static bool value = (x₁ == x₂); };

template<Val T₁, Val T₂, Val U₁, Val U₂> struct ConvM<App<T₁, U₁>, App<T₂, U₂>>
{ constexpr static bool value = Conv<T₁, T₂> && Conv<U₁, U₂>; };

template<Val T₁, Val T₂, Val U₁, Val U₂> struct ConvM<Pi<T₁, U₁>, Pi<T₂, U₂>>
{ constexpr static bool value = Conv<T₁, T₂> && Conv<U₁, U₂>; };

template<Val T₁, Val T₂, Val U₁, Val U₂> struct ConvM<Lam<T₁, U₁>, Lam<T₂, U₂>>
{ constexpr static bool value = Conv<T₁, T₂> && Conv<U₁, U₂>; };

template<Val T, Val U, Val R> struct ConvM<Lam<T, U>, R>
{ constexpr static bool value = Conv<U, Apply<R, Var<0>>>; };

template<Val T, Val U, Val R> struct ConvM<R, Lam<T, U>>
{ constexpr static bool value = Conv<Apply<R, Var<0>>, U>; };

}