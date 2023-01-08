#pragma once

#include "Val.hpp"

namespace Value {

template<Integer, Integer, Val> struct ShiftM;
template<Integer i, Integer c, Val T> using Shift = ShiftM<i, c, T>::value;

template<Integer i, Integer c, Nat N> struct ShiftM<i, c, Type<N>>
{ using value = Type<N>; };

template<Integer i, Integer c, Literal x> struct ShiftM<i, c, Const<x>>
{ using value = Const<x>; };

template<Integer i, Integer c, Integer n> struct ShiftM<i, c, Var<n>>
{ using value = If<n < c, Var<n>, Var<n + i>>; };

template<Integer i, Integer c, Val T₁, Val T₂> struct ShiftM<i, c, App<T₁, T₂>>
{ using value = App<Shift<i, c, T₁>, Shift<i, c, T₂>>; };

template<Integer i, Integer c, Val T, Val U> struct ShiftM<i, c, Lam<T, U>>
{ using value = Lam<Shift<i, c, T>, Shift<i, c + 1, U>>; };

template<Integer i, Integer c, Val T, Val U> struct ShiftM<i, c, Pi<T, U>>
{ using value = Pi<Shift<i, c, T>, Shift<i, c + 1, U>>; };

template<Val T> using Up   = Shift<1, 0, T>;
template<Val T> using Down = Shift<-1, 0, T>;

template<Val, Integer, Val> struct SubstM;
template<Val T, Integer i, Val U> using Subst = SubstM<T, i, U>::value;

template<Val T, Val U> struct ApplyM { using value = App<T, U>; };
template<Val T, Val U> using Apply = typename ApplyM<T, U>::value;

template<Nat N, Integer i, Val U> struct SubstM<Type<N>, i, U>
{ using value = Type<N>; };

template<Literal x, Integer i, Val U> struct SubstM<Const<x>, i, U>
{ using value = Const<x>; };

template<Integer j, Integer i, Val U> struct SubstM<Var<j>, i, U>
{ using value = If<i == j, U, Var<j>>; };

template<Val T₁, Val T₂, Integer i, Val U> struct SubstM<App<T₁, T₂>, i, U>
{ using value = Apply<Subst<T₁, i, U>, Subst<T₂, i, U>>; };

template<Val τ, Val φ, Integer i, Val U> struct SubstM<Lam<τ, φ>, i, U>
{ using value = Lam<Subst<τ, i, U>, Subst<φ, i + 1, Up<U>>>; };

template<Val τ, Val φ, Integer i, Val U> struct SubstM<Pi<τ, φ>, i, U>
{ using value = Pi<Subst<τ, i, U>, Subst<φ, i + 1, Up<U>>>; };

template<Val τ, Val φ, Val T> struct ApplyM<Lam<τ, φ>, T>
{ using value = Down<Subst<φ, 0, Up<T>>>; };

template<Val T> struct EvalM { using value = T; };
template<Val T> using Eval = typename EvalM<T>::value;

template<Val T, Val U> struct EvalM<App<T, U>> { using value = Apply<Eval<T>, Eval<U>>; };
template<Val T, Val U> struct EvalM<Lam<T, U>> { using value = Lam<Eval<T>, Eval<U>>; };
template<Val T, Val U> struct EvalM<Pi<T, U>>  { using value = Pi<Eval<T>, Eval<U>>; };

}