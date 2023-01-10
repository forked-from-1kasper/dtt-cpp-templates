#include <iostream>

#include "Check.hpp"
#include "Expr.hpp"
#include "PP.hpp"

namespace Expr {
    using U = Type<Zero>;

    using I   = Var<"I">;
    using i₀  = Var<"i₀">;
    using i₁  = Var<"i₁">;

    using coeᵀ   = Pi<"A", Impl<I, U>, Pi<"i", I, Impl<App<Var<"A">, i₀>, App<Var<"A">, Var<"i">>>>>;
    using PathPᵀ = Pi<"A", Impl<I, U>, Pi<"a", App<Var<"A">, i₀>, Pi<"b", App<Var<"A">, i₁>, U>>>;

    using coe   = Var<"coe">;
    using PathP = Var<"PathP">;

    using plamPᵀ = Pi<"A", Impl<I, U>, Pi<"φ", Pi<"i", I, App<Var<"A">, Var<"i">>>,
        App<PathP, Var<"A">, App<Var<"φ">, i₀>, App<Var<"φ">, i₁>>>>;
    using pappPᵀ = Pi<"A", Impl<I, U>, Pi<"a", App<Var<"A">, i₀>, Pi<"b", App<Var<"A">, i₁>,
        Pi<"p", App<PathP, Var<"A">, Var<"a">, Var<"b">>, Pi<"i", I, App<Var<"A">, Var<"i">>>>>>>;

    using plamP = Var<"plamP">;
    using pappP = Var<"pappP">;

    template<typename T> using refl = Lam<"_", I, T>;

    template<typename A, typename φ>
    using plam = App<plamP, refl<A>, φ>;

    template<typename A, typename a, typename b, typename p, typename i>
    using papp = App<pappP, refl<A>, a, b, p, i>;

    template<typename A, typename a, typename b>
    using Path = App<PathP, refl<A>, a, b>;

    template<typename A, typename B, typename f, typename g>
    using linv = Pi<"x", A, Path<A, App<g, App<f, Var<"x">>>, Var<"x">>>;

    template<typename A, typename B, typename f, typename g>
    using rinv = Pi<"x", B, Path<B, App<f, App<g, Var<"x">>>, Var<"x">>>;

    using isoᵀ =
    Pi<"A", U, Pi<"B", U,
        Pi<"f", Impl<Var<"A">, Var<"B">>,
            Pi<"g", Impl<Var<"B">, Var<"A">>,
                Impl<linv<Var<"A">, Var<"B">, Var<"f">, Var<"g">>,
                     rinv<Var<"A">, Var<"B">, Var<"f">, Var<"g">>,
                     I, U>>>>>;

    using iso = Var<"iso">;
}

namespace Value {
    template<> struct Postulate<"I">  { using value = Type<Zero>; };
    template<> struct Postulate<"i₀"> { using value = Const<"I">; };
    template<> struct Postulate<"i₁"> { using value = Const<"I">; };

    template<> struct Postulate<"coe"> { using value = Expr::Parse<Expr::coeᵀ>; };
    static_assert(IsType<Expr::Infer<Expr::coeᵀ>>);

    // coe(A, i₀) ~> λ a, a
    template<Val A> struct ApplyM<App<Const<"coe">, A>, Const<"i₀">>
    { using value = Lam<App<A, Const<"i₀">>, Var<0>>; };

    template<> struct Postulate<"PathP"> { using value = Expr::Parse<Expr::PathPᵀ>; };
    static_assert(IsType<Expr::Infer<Expr::PathPᵀ>>);

    template<> struct Postulate<"plamP"> { using value = Expr::Parse<Expr::plamPᵀ>; };
    static_assert(IsType<Expr::Infer<Expr::plamPᵀ>>);

    template<> struct Postulate<"pappP"> { using value = Expr::Parse<Expr::pappPᵀ>; };
    static_assert(IsType<Expr::Infer<Expr::pappPᵀ>>);

    // plamP(A, pappP(A, a, b, p)) ~> p
    template<Val A, Val a, Val b, Val p> struct ApplyM<
        App<Const<"plamP">, A>, App⁵(Const<"pappP">, A, a, b, p)
    > { using value = p; };

    // pappP(A, a, b, p, i₀) ~> a
    template<Val A, Val a, Val b, Val p> struct ApplyM<
        App⁵(Const<"pappP">, A, a, b, p), Const<"i₀">
    > { using value = a; };

    // pappP(A, a, b, p, i₁) ~> b
    template<Val A, Val a, Val b, Val p> struct ApplyM<
        App⁵(Const<"pappP">, A, a, b, p), Const<"i₁">
    > { using value = b; };

    template<> struct Postulate<"iso"> { using value = Expr::Parse<Expr::isoᵀ>; };
    static_assert(IsType<Expr::Infer<Expr::isoᵀ>>);

    // iso(A, B, f, g, p, q, i₀) ~> A
    template<Val A, Val B, Val f, Val g, Val p, Val q> struct ApplyM<
        App⁷(Const<"iso">, A, B, f, g, p, q), Const<"i₀">
    > { using value = A; };

    // iso(A, B, f, g, p, q, i₁) ~> B
    template<Val A, Val B, Val f, Val g, Val p, Val q> struct ApplyM<
        App⁷(Const<"iso">, A, B, f, g, p, q), Const<"i₁">
    > { using value = B; };

    // coe(iso(A, B, f, g, p, q), right) ~> f
    template<Val A, Val B, Val f, Val g, Val p, Val q> struct ApplyM<
        App<Const<"coe">, App⁷(Const<"iso">, A, B, f, g, p, q)>, Const<"i₁">
    > { using value = f; };
}

namespace Example {
    using namespace Expr;

    using transport =
    Lam<"A", U, Lam<"P", Impl<Var<"A">, U>,
        Lam<"a", Var<"A">, Lam<"b", Var<"A">,
            Lam<"p", Path<Var<"A">, Var<"a">, Var<"b">>,
                App<coe, Lam<"i", I, App<Var<"P">,
                    papp<Var<"A">, Var<"a">, Var<"b">, Var<"p">, Var<"i">>>>, i₁>>>>>>;

    static_assert(Value::IsType<Value::Infer<List<>, Infer<transport>>>);

    using ap =
    Lam<"A", U, Lam<"B", U, Lam<"f", Impl<Var<"A">, Var<"B">>,
        Lam<"a", Var<"A">, Lam<"b", Var<"A">,
            Lam<"p", Path<Var<"A">, Var<"a">, Var<"b">>, plam<Var<"B">,
                Lam<"i", I, App<Var<"f">, papp<Var<"A">, Var<"a">, Var<"b">, Var<"p">, Var<"i">>>>>>>>>>>;

    static_assert(Value::IsType<Value::Infer<List<>, Infer<ap>>>);
}

int main() {
    using namespace Expr;

    Value::Show<Parse<coeᵀ>>::show(std::cout << "coe : ") << std::endl;

    Value::Show<Parse<PathPᵀ>>::show(std::cout << "PathP : ") << std::endl;
    Value::Show<Parse<plamPᵀ>>::show(std::cout << "plamP : ") << std::endl;
    Value::Show<Parse<pappPᵀ>>::show(std::cout << "pappP : ") << std::endl;

    Value::Show<Parse<isoᵀ>>::show(std::cout << "iso : ") << std::endl;

    Value::Show<Infer<Example::transport>>::show(std::cout << "transport : ") << std::endl;
    Value::Show<Infer<Example::ap>>::show(std::cout << "ap : ") << std::endl;

    return 0;
}