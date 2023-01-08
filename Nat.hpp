#pragma once

struct Zero;
template<typename T> struct Succ { using pred = T; };

template<typename T> concept HasPred =
requires() { typename T::pred; };

template<HasPred T> using Pred = T::pred;

template<typename, typename> struct MaxM;
template<typename N, typename M> using Max = MaxM<N, M>::value;

template<> struct MaxM<Zero, Zero> { using value = Zero; };
template<typename N> struct MaxM<Zero, N> { using value = N; };
template<typename N> struct MaxM<N, Zero> { using value = N; };

template<typename N, typename M> struct MaxM<Succ<N>, Succ<M>>
{ using value = Succ<Max<N, N>>; };

template<typename> struct NatM { constexpr static bool value = false; };
template<typename T> concept Nat = NatM<T>::value;

template<> struct NatM<Zero> { constexpr static bool value = true; };
template<typename T> struct NatM<Succ<T>> { constexpr static bool value = Nat<T>; };