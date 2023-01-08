#pragma once

#include "List.hpp"
#include "Val.hpp"

namespace Expr {

template<Nat> struct Type;
template<Literal> struct Var;
template<typename, typename> struct App;
template<Literal, typename, typename> struct Lam;
template<Literal, typename, typename> struct Pi;
template<typename, typename> struct Impl;

template<typename, typename> struct BruijnM;
template<typename Rho, typename T> using Bruijn = BruijnM<Rho, T>::value;

template<typename Rho, Nat N> struct BruijnM<Rho, Type<N>>
{ using value = Value::Type<N>; };

template<typename Rho, Literal x> struct BruijnM<Rho, Var<x>>
{ using value = Get<x, Rho, Value::Const<x>>; };

template<typename Rho, typename T, typename U> struct BruijnM<Rho, App<T, U>>
{ using value = Value::App<Bruijn<Rho, T>, Bruijn<Rho, U>>; };

template<typename Rho, Literal x, typename T, typename U> struct BruijnM<Rho, Lam<x, T, U>>
{ using value = Value::Lam<Bruijn<Rho, T>, Bruijn<Set<x, Map<Reassoc<Value::Up>::Lambda, Rho>, Value::Var<0>>, U>>; };

template<typename Rho, Literal x, typename T, typename U> struct BruijnM<Rho, Pi<x, T, U>>
{ using value = Value::Pi<Bruijn<Rho, T>, Bruijn<Set<x, Map<Reassoc<Value::Up>::Lambda, Rho>, Value::Var<0>>, U>>; };

template<typename Rho, typename T, typename U> struct BruijnM<Rho, Impl<T, U>>
{ using value = Value::Pi<Bruijn<Rho, T>, Bruijn<Map<Reassoc<Value::Up>::Lambda, Rho>, U>>; };

template<typename T> using Parse = Bruijn<List<>, T>;
template<typename T> using Eval  = Value::Eval<Parse<T>>;
template<typename T> using Infer = Value::Infer<List<>, Parse<T>>;
template<typename T, typename U> constexpr bool Check = Value::Check<Parse<T>, Parse<U>>;

}