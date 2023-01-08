#include <iostream>

#include "Check.hpp"
#include "Expr.hpp"
#include "PP.hpp"

using namespace Expr;

using N = Pi<"A", Type<Zero>, Impl<Impl<Var<"A">, Var<"A">>, Impl<Var<"A">, Var<"A">>>>;
using N0 = Lam<"A", Type<Zero>, Lam<"S", Impl<Var<"A">, Var<"A">>, Lam<"Z", Var<"A">, Var<"Z">>>>;

using NSucc =
Lam<"n", N, Lam<"A", Type<Zero>, Lam<"S", Impl<Var<"A">, Var<"A">>, Lam<"Z", Var<"A">,
    App<Var<"S">, App<App<App<Var<"n">, Var<"A">>, Var<"S">>, Var<"Z">>>>>>>;

#include "N3000.hpp"

static_assert(Check<N3000, N>);

int main() { return 0; }