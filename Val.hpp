#pragma once

#include "Literal.hpp"
#include "List.hpp"
#include "Nat.hpp"

namespace Value {

template<Nat N> struct Type { using level = N; };
template<Integer> struct Var;
template<Literal> struct Const;
template<typename, typename> struct App;
template<typename, typename> struct Lam;
template<typename T, typename U> struct Pi
{ using dom = T; using cod = U; using lambda = Lam<T, U>; };

template<typename> struct IsTypeM
{ constexpr static bool value = false; };

template<Nat N> struct IsTypeM<Type<N>>
{ constexpr static bool value = true; };

template<typename T> concept IsType = IsTypeM<T>::value;

template<typename> struct IsPiTypeM
{ constexpr static bool value = false; };

template<typename T, typename U> struct IsPiTypeM<Pi<T, U>>
{ constexpr static bool value = true; };

template<typename T> concept IsPiType = IsPiTypeM<T>::value;

template<typename> struct ValM
{ constexpr static bool value = false; };

template<Nat N> struct ValM<Type<N>>
{ constexpr static bool value = true; };

template<Literal x> struct ValM<Const<x>>
{ constexpr static bool value = true; };

template<Integer n> struct ValM<Var<n>>
{ constexpr static bool value = true; };

template<typename T, typename U> struct ValM<App<T, U>>
{ constexpr static bool value = ValM<T>::value && ValM<U>::value; };

template<typename T, typename U> struct ValM<Lam<T, U>>
{ constexpr static bool value = ValM<T>::value && ValM<U>::value; };

template<typename T, typename U> struct ValM<Pi<T, U>>
{ constexpr static bool value = ValM<T>::value && ValM<U>::value; };

template<typename T> concept Val = ValM<T>::value;

template<Literal> struct Postulate;
template<Literal x> using Lookup = typename Postulate<x>::value;

}

#define App³(T₁, T₂, T₃)                         App<App<T₁, T₂>, T₃>
#define App⁴(T₁, T₂, T₃, T₄)                     App<App<App<T₁, T₂>, T₃>, T₄>
#define App⁵(T₁, T₂, T₃, T₄, T₅)                 App<App<App<App<T₁, T₂>, T₃>, T₄>, T₅>
#define App⁶(T₁, T₂, T₃, T₄, T₅, T₆)             App<App<App<App<App<T₁, T₂>, T₃>, T₄>, T₅>, T₆>
#define App⁷(T₁, T₂, T₃, T₄, T₅, T₆, T₇)         App<App<App<App<App<App<T₁, T₂>, T₃>, T₄>, T₅>, T₆>, T₇>
#define App⁸(T₁, T₂, T₃, T₄, T₅, T₆, T₇, T₈)     App<App<App<App<App<App<App<T₁, T₂>, T₃>, T₄>, T₅>, T₆>, T₇>, T₈>
#define App⁹(T₁, T₂, T₃, T₄, T₅, T₆, T₇, T₈, T₉) App<App<App<App<App<App<App<App<T₁, T₂>, T₃>, T₄>, T₅>, T₆>, T₇>, T₈>, T₉>