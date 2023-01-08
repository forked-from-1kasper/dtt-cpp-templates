#pragma once

#include "Val.hpp"

namespace Value {

template<typename> struct Show;

template<> struct Show<Zero> {
    static std::ostream & show(std::ostream & os) {
        return os << "Zero";
    }
};

template<Nat T> struct Show<Succ<T>> {
    static std::ostream & show(std::ostream & os) {
        return Show<T>::show(os << "Succ(") << ")";
    }
};

template<Nat N> struct Show<Type<N>> {
    static std::ostream & show(std::ostream & os) {
        return Show<N>::show(os << "(Type ") << ")";
    }
};

template<Integer n> struct Show<Var<n>> {
    static std::ostream & show(std::ostream & os) {
        return os << n;
    }
};

template<Literal x> struct Show<Const<x>> {
    static std::ostream & show(std::ostream & os) {
        return os << x.unquote;
    }
};

template<Val T, Val U> struct Show<App<T, U>> {
    static std::ostream & show(std::ostream & os) {
        return Show<U>::show(Show<T>::show(os << "(") << " ") << ")";
    }
};

template<Val T, Val U> struct Show<Lam<T, U>> {
    static std::ostream & show(std::ostream & os) {
        return Show<U>::show(Show<T>::show(os << "(λ ") << " ") << ")";
    }
};

template<Val T, Val U> struct Show<Pi<T, U>> {
    static std::ostream & show(std::ostream & os) {
        return Show<U>::show(Show<T>::show(os << "(Π ") << " ") << ")";
    }
};

}