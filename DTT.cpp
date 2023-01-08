#include <type_traits>
#include <algorithm>
#include <iostream>
#include <concepts>

#include "Check.hpp"
#include "Expr.hpp"
#include "PP.hpp"

namespace Value {
    template<> struct Postulate<"="> { using value = Pi<Type<Zero>, Pi<Var<0>, Var<1>>>; };

    template<> struct Postulate<"𝟏"> { using value = Type<Zero>; };
    template<> struct Postulate<"★"> { using value = Const<"𝟏">; };

    template<> struct Postulate<"𝟐">  { using value = Type<Zero>; };
    template<> struct Postulate<"0₂"> { using value = Const<"𝟐">; };
    template<> struct Postulate<"1₂"> { using value = Const<"𝟐">; };

    template<> struct Postulate<"¬"> { using value = Pi<Const<"𝟐">, Const<"𝟐">>; };
    template<> struct EvalM<App<Const<"¬">, Const<"0₂">>> { using value = Const<"1₂">; };
    template<> struct EvalM<App<Const<"¬">, Const<"1₂">>> { using value = Const<"0₂">; };
}

namespace Example {
    using namespace Expr;

    using id = Lam<"A", Type<Succ<Zero>>, Lam<"x", Var<"A">, Var<"x">>>;
    using idᵀ = Pi<"A", Type<Succ<Zero>>, Impl<Var<"A">, Var<"A">>>;
    static_assert(Check<id, idᵀ>);

    using test =
    Lam<"A", Type<Succ<Zero>>,
        Lam<"f", Impl<Var<"A">, Var<"A">>,
            Lam<"x", Var<"A">, App<Var<"f">, Var<"x">>>
        >
    >;
}

int main() {
    using namespace Expr;

    Value::Show<Infer<Example::test>>::show(std::cout) << std::endl;
    Value::Show<Infer<App<Var<"=">, Var<"𝟏">>>>::show(std::cout) << std::endl;
    Value::Show<Eval<App<Var<"¬">, Var<"0₂">>>>::show(std::cout) << std::endl;

    return 0;
}