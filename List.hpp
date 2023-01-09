#pragma once

#include <type_traits>
#include <concepts>

#include "Literal.hpp"

using Integer = int64_t;

template<typename...> struct List;

template<typename T, typename... Ts> struct List<T, Ts...>
{ using head = T; using tail = List<Ts...>; };

template<typename U> concept Empty = std::same_as<U, List<>>;

template<typename U> concept Inhabited = requires()
{ typename U::head; typename U::tail; };

template<Inhabited U> using Head = U::head;
template<Inhabited U> using Tail = U::tail;

template<typename, typename> struct ConsM;
template<typename T, typename Ts>
using Cons = ConsM<T, Ts>::value;

template<typename T, typename... Ts>
struct ConsM<T, List<Ts...>> { using value = List<T, Ts...>; };

template<template<typename> typename, typename> struct MapM;
template<template<typename> typename φ, typename Ts>
using Map = MapM<φ, Ts>::value;

template<template<typename> typename φ>
struct MapM<φ, List<>> { using value = List<>; };

template<template<typename> typename φ, typename T, typename... Ts>
struct MapM<φ, List<T, Ts...>> { using value = Cons<φ<T>, Map<φ, List<Ts...>>>; };

template<template<typename, typename> typename, typename> struct FoldrM;

template<template<typename, typename> typename φ, Inhabited Ts>
using Foldr = FoldrM<φ, Ts>::value;

template<template<typename, typename> typename φ, typename T>
struct FoldrM<φ, List<T>> { using value = T; };

template<template<typename, typename> typename φ, typename T, typename... Ts>
struct FoldrM<φ, List<T, Ts...>> { using value = φ<T, Foldr<φ, List<Ts...>>>; };

template<template<typename, typename> typename, typename, typename> struct FoldlM;

template<template<typename, typename> typename φ, typename Acc>
struct FoldlM<φ, Acc, List<>> { using value = Acc; };

template<template<typename, typename> typename φ, typename Acc, typename T, typename... Ts>
struct FoldlM<φ, Acc, List<T, Ts...>> { using value = typename FoldlM<φ, φ<Acc, T>, List<Ts...>>::value; };

template<template<typename, typename> typename φ, Inhabited Ts>
using Foldl = FoldlM<φ, Head<Ts>, Tail<Ts>>::value;

template<Integer, typename> struct NthM;
template<Integer n, typename T> using Nth = NthM<n, T>::value;

template<typename T, typename... Ts>
struct NthM<0, List<T, Ts...>> { using value = T; };

template<Integer n, typename T, typename... Ts>
struct NthM<n, List<T, Ts...>> { using value = Nth<n - 1, List<Ts...>>; };

template<Literal k, typename T> struct Assoc
{ static constexpr auto key = k; using value = T; };

template<Literal, typename, typename> struct GetM;
template<Literal k, typename L, typename U> using Get = GetM<k, L, U>::value;

template<Literal k, typename T> struct GetM<k, List<>, T> { using value = T; };

template<Literal k₁, typename U, Literal k₂, typename T, typename... Ts> struct GetM<k₁, List<Assoc<k₂, T>, Ts...>, U>
{ using value = If<k₁ == k₂, T, Get<k₁, List<Ts...>, U>>; };

template<Literal, typename, typename> struct SetM;
template<Literal k, typename L, typename U> using Set = SetM<k, L, U>::value;

template<Literal k, typename T> struct SetM<k, List<>, T> { using value = List<Assoc<k, T>>; };

template<Literal k₁, typename U, Literal k₂, typename T, typename... Ts> struct SetM<k₁, List<Assoc<k₂, T>, Ts...>, U>
{ using value = If<k₁ == k₂, List<Assoc<k₁, U>, Ts...>, Cons<Assoc<k₂, T>, Set<k₁, List<Ts...>, U>>>; };

template<template<typename> typename Φ> struct AssocMap {
    template<typename> struct AppM;

    template<Literal k, typename T> struct AppM<Assoc<k, T>>
    { using value = Assoc<k, Φ<T>>; };

    template<typename T> using App = AppM<T>::value;
};