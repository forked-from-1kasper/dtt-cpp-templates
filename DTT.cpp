#include <iostream>

#include "Check.hpp"
#include "Expr.hpp"
#include "PP.hpp"

namespace Example {
    using namespace Expr;
    using Idᵀ = Parse<Pi<"A", Type<Zero>, Impl<Var<"A">, Impl<Var<"A">, Type<Zero>>>>>;

    template<typename T, typename t₁, typename t₂>
    using Id = App<App<App<Var<"=">, T>, t₁>, t₂>;

    using Reflᵀ = Parse<Pi<"A", Type<Zero>, Pi<"a", Var<"A">, Id<Var<"A">, Var<"a">, Var<"a">>>>>;

    template<typename T, typename t>
    using Refl = App<App<Var<"refl">, T>, t>;

    using Bᵀ = Pi<"a", Var<"A">, Pi<"b", Var<"A">, Impl<Id<Var<"A">, Var<"a">, Var<"b">>, Type<Zero>>>>;
    using BRefl = Pi<"x", Var<"A">, App<App<App<Var<"B">, Var<"x">>, Var<"x">>, Refl<Var<"A">, Var<"x">>>>;

    using BQuantified =
    Pi<"a", Var<"A">, Pi<"b", Var<"A">, Pi<"p", Id<Var<"A">, Var<"a">, Var<"b">>,
        App<App<App<Var<"B">, Var<"a">>, Var<"b">>, Var<"p">>>>>;

    using Jᵀ = Parse<Pi<"A", Type<Zero>, Pi<"B", Bᵀ, Impl<BRefl, BQuantified>>>>;
}

namespace Value {
    template<> struct Postulate<"=">    { using value = Example::Idᵀ; };
    template<> struct Postulate<"refl"> { using value = Example::Reflᵀ; };
    template<> struct Postulate<"J">    { using value = Example::Jᵀ; };

    static_assert(IsType<Infer<List<>, Example::Idᵀ>>);
    static_assert(IsType<Infer<List<>, Example::Reflᵀ>>);
    static_assert(IsType<Infer<List<>, Example::Jᵀ>>);

    // J(A, B, BRefl, a, a, refl(a)) ~> BRefl(a)
    template<Val A, Val B, Val BRefl, Val a> struct ApplyM<
        App<App<App<App<App<Const<"J">, A>, B>, BRefl>, a>, a>,
        App<App<Const<"refl">, A>, a>
    > { using value = Value::Apply<BRefl, a>; };

    template<> struct Postulate<"𝟏"> { using value = Type<Zero>; };
    template<> struct Postulate<"★"> { using value = Const<"𝟏">; };

    template<> struct Postulate<"𝟐">  { using value = Type<Zero>; };
    template<> struct Postulate<"0₂"> { using value = Const<"𝟐">; };
    template<> struct Postulate<"1₂"> { using value = Const<"𝟐">; };

    template<> struct Postulate<"¬"> { using value = Pi<Const<"𝟐">, Const<"𝟐">>; };
    template<> struct ApplyM<Const<"¬">, Const<"0₂">> { using value = Const<"1₂">; };
    template<> struct ApplyM<Const<"¬">, Const<"1₂">> { using value = Const<"0₂">; };
}

namespace Example {
    using namespace Expr;

    using idfunᵀ = Pi<"A", Type<Succ<Zero>>, Impl<Var<"A">, Var<"A">>>;
    using idfun  = Lam<"A", Type<Succ<Zero>>, Lam<"x", Var<"A">, Var<"x">>>;
    static_assert(Check<idfun, idfunᵀ>);

    using test =
    Lam<"A", Type<Succ<Zero>>,
        Lam<"f", Impl<Var<"A">, Var<"A">>,
            Lam<"x", Var<"A">, App<Var<"f">, Var<"x">>>>>;

    using symmᵀ =
    Pi<"A", Type<Zero>,
        Pi<"a", Var<"A">, Pi<"b", Var<"A">,
            Impl<Id<Var<"A">, Var<"a">, Var<"b">>,
                 Id<Var<"A">, Var<"b">, Var<"a">>>>>>;

    using φ =
    Lam<"a", Var<"A">, Lam<"b", Var<"A">,
        Lam<"p", Id<Var<"A">, Var<"a">, Var<"b">>,
            Id<Var<"A">, Var<"b">, Var<"a">>>>>;

    using symm =
    Lam<"A", Type<Zero>, Lam<"x", Var<"A">, Lam<"y", Var<"A">,
        Lam<"p", Id<Var<"A">, Var<"x">, Var<"y">>,
            App<App<App<App<App<App<Var<"J">, Var<"A">>, φ>,
                App<Var<"refl">, Var<"A">>>,
                    Var<"x">>, Var<"y">>, Var<"p">>>>>>;

    static_assert(Check<symm, symmᵀ>);

    using symmTest =
    Lam<"A", Type<Zero>,
        Lam<"a", Var<"A">,
            App<App<App<App<symm, Var<"A">>, Var<"a">>, Var<"a">>,
                App<App<Var<"refl">, Var<"A">>, Var<"a">>
            >
        >
    >;
}

int main() {
    using namespace Expr;

    Value::Show<Infer<Example::test>>::show(std::cout << "test : ") << std::endl;

    Value::Show<Infer<Example::symm>>::show(std::cout << "symm : ") << std::endl;
    Value::Show<Eval<Example::symm>>::show(std::cout << "symm ≡ ") << std::endl;

    Value::Show<Infer<Example::symmTest>>::show(std::cout << "symmTest : ") << std::endl;
    Value::Show<Eval<Example::symmTest>>::show(std::cout << "symmTest ≡ ") << std::endl;

    Value::Show<Infer<App<Var<"=">, Var<"𝟏">>>>::show(std::cout) << std::endl;
    Value::Show<Eval<App<Var<"¬">, Var<"0₂">>>>::show(std::cout) << std::endl;

    return 0;
}