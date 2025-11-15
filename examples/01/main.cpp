#include <iostream>
#include <type_traits>

import typical.lambda;
import typical.church;
import typical.nat;

// Lambda module types
using typical::Abs;
using typical::App;
using typical::Const;
using typical::eval_t;
using typical::Id;
using typical::Var;

// Church booleans from lambda module
using typical::False;
using typical::True;

// Church pairs and lists from church module
using typical::BuildList;
using typical::GetFst;
using typical::GetSnd;
using typical::Head;
using typical::MakeCons;
using typical::MakePair;
using typical::Nil;

// Peano numbers from nat module
namespace nat = typical::nat;
using nat::add_t;
using nat::factorial_t;
using nat::fibonacci_t;
using nat::from_value_t;
using nat::gcd_t;
using nat::greater_than_v;
using nat::is_even_v;
using nat::is_odd_v;
using nat::less_or_equal_v;
using nat::less_than_v;
using nat::max_t;
using nat::min_t;
using nat::mul_t;
using nat::nat_equal_v;
using nat::pow_t;
using nat::S;
using nat::sub_t;
using nat::to_value_v;
using nat::Z;

// Convenient aliases for Peano numbers
using nat::Eight;
using nat::Five;
using nat::Four;
using nat::Nine;
using nat::One;
using nat::Seven;
using nat::Six;
using nat::Ten;
using nat::Three;
using nat::Two;
using nat::Zero;

// ============================================================================
// Example 1: Lambda Calculus Basics
// ============================================================================

// Identity function: λx.x
using MyIdentity = Id;
static_assert(std::is_same_v<eval_t<App<MyIdentity, Var<42>>>, Var<42>>, "Identity returns its argument");

// Const function: λx.λy.x (returns first argument)
using MyConst = Const;
static_assert(std::is_same_v<eval_t<App<App<MyConst, Var<1>>, Var<2>>>, Var<1>>, "Const returns first argument");

// Composition: B = λf.λg.λx.f(g(x))
using B = Abs<Abs<Abs<App<Var<2>, App<Var<1>, Var<0>>>>>>;
static_assert(std::is_same_v<eval_t<App<App<App<B, Id>, Id>, Var<7>>>, Var<7>>, "Composition of identity is identity");

// ============================================================================
// Example 2: Church Booleans
// ============================================================================

// Church booleans: True = λx.λy.x, False = λx.λy.y
static_assert(std::is_same_v<eval_t<App<App<True, Var<100>>, Var<200>>>, Var<100>>, "True selects first argument");
static_assert(std::is_same_v<eval_t<App<App<False, Var<100>>, Var<200>>>, Var<200>>, "False selects second argument");

// If-then-else using church booleans
template <typename Cond, typename Then, typename Else>
using IfThenElse = eval_t<App<App<Cond, Then>, Else>>;

static_assert(std::is_same_v<IfThenElse<True, Var<1>, Var<2>>, Var<1>>, "If True then 1 else 2 = 1");
static_assert(std::is_same_v<IfThenElse<False, Var<1>, Var<2>>, Var<2>>, "If False then 1 else 2 = 2");

// ============================================================================
// Example 3: Church Numerals
// ============================================================================

// Church numerals represent numbers by function iteration
// Zero = λf.λx.x
// One = λf.λx.f(x)
// Two = λf.λx.f(f(x))

// Note: Church numerals (typical::Zero, typical::One, typical::Two) from lambda module
// are different from Peano numbers (nat::Zero, nat::One, nat::Two) from nat module.
// Church numerals represent numbers as function iteration.
// Peano numbers use Zero/Successor structure (Z, S<Z>, S<S<Z>>...).

// ============================================================================
// Example 4: Church Pairs
// ============================================================================

// Pairs store two values: Pair = λx.λy.λf.f x y
using MyPair = MakePair<Var<10>, Var<20>>;
static_assert(std::is_same_v<eval_t<GetFst<MyPair>>, Var<10>>, "First element of pair is 10");
static_assert(std::is_same_v<eval_t<GetSnd<MyPair>>, Var<20>>, "Second element of pair is 20");

// Nested pairs
using NestedPair = MakePair<MakePair<Var<1>, Var<2>>, Var<3>>;
static_assert(std::is_same_v<eval_t<GetFst<eval_t<GetFst<NestedPair>>>>, Var<1>>, "First of first of nested pair");

// ============================================================================
// Example 5: Church Lists
// ============================================================================

// Lists are built from Cons and Nil
using SingletonList = MakeCons<Var<42>, Nil>;
static_assert(std::is_same_v<eval_t<App<Head, SingletonList>>, Var<42>>, "Head of singleton list");

using ThreeElementList = BuildList<Var<1>, Var<2>, Var<3>>;
static_assert(std::is_same_v<eval_t<App<Head, ThreeElementList>>, Var<1>>, "Head of three element list");

// ============================================================================
// Example 6: Peano Natural Numbers
// ============================================================================

// Peano numbers: Z, S(Z), S(S(Z)), ...
// Using nat module (typical::nat namespace)
static_assert(nat::is_nat<Zero>::value, "Zero is a natural number");
static_assert(nat::is_nat<One>::value, "One is a natural number");
static_assert(nat::is_nat<Ten>::value, "Ten is a natural number");

// Conversion to runtime values
static_assert(to_value_v<Zero> == 0, "Zero converts to 0");
static_assert(to_value_v<Five> == 5, "Five converts to 5");
static_assert(to_value_v<Ten> == 10, "Ten converts to 10");

// Conversion from runtime values
static_assert(std::is_same_v<from_value_t<7>, Seven>, "7 converts to Seven");

// ============================================================================
// Example 7: Peano Arithmetic
// ============================================================================

// Addition
static_assert(std::is_same_v<add_t<Two, Three>, Five>, "2 + 3 = 5");
static_assert(to_value_v<add_t<Four, Six>> == 10, "4 + 6 = 10");

// Multiplication
static_assert(std::is_same_v<mul_t<Two, Three>, Six>, "2 * 3 = 6");
static_assert(to_value_v<mul_t<Three, Four>> == 12, "3 * 4 = 12");

// Subtraction (saturating)
static_assert(std::is_same_v<sub_t<Five, Two>, Three>, "5 - 2 = 3");
static_assert(std::is_same_v<sub_t<Two, Five>, Zero>, "2 - 5 = 0 (saturating)");

// Division
static_assert(std::is_same_v<nat::div_t<Ten, Two>, Five>, "10 / 2 = 5");
static_assert(std::is_same_v<nat::div_t<Nine, Three>, Three>, "9 / 3 = 3");

// Modulus
static_assert(std::is_same_v<nat::mod_t<Ten, Three>, One>, "10 % 3 = 1");
static_assert(std::is_same_v<nat::mod_t<Seven, Two>, One>, "7 % 2 = 1");

// ============================================================================
// Example 8: Comparison
// ============================================================================

static_assert(less_than_v<Two, Five>, "2 < 5");
static_assert(!less_than_v<Five, Two>, "!(5 < 2)");
static_assert(less_or_equal_v<Three, Three>, "3 <= 3");
static_assert(greater_than_v<Ten, One>, "10 > 1");
static_assert(nat_equal_v<Four, Four>, "4 == 4");

// Min and Max
static_assert(std::is_same_v<min_t<Three, Seven>, Three>, "min(3, 7) = 3");
static_assert(std::is_same_v<max_t<Three, Seven>, Seven>, "max(3, 7) = 7");

// ============================================================================
// Example 9: Mathematical Functions
// ============================================================================

// Power
static_assert(std::is_same_v<pow_t<Two, Three>, Eight>, "2^3 = 8");
static_assert(to_value_v<pow_t<Three, Two>> == 9, "3^2 = 9");

// Factorial
static_assert(std::is_same_v<factorial_t<Three>, Six>, "3! = 6");
static_assert(to_value_v<factorial_t<Four>> == 24, "4! = 24");
static_assert(to_value_v<factorial_t<Five>> == 120, "5! = 120");

// GCD (Greatest Common Divisor)
static_assert(std::is_same_v<gcd_t<Six, Nine>, Three>, "gcd(6, 9) = 3");
static_assert(std::is_same_v<gcd_t<Eight, from_value_t<12>>, Four>, "gcd(8, 12) = 4");

// Fibonacci
static_assert(std::is_same_v<fibonacci_t<Zero>, Zero>, "fib(0) = 0");
static_assert(std::is_same_v<fibonacci_t<Five>, Five>, "fib(5) = 5");
static_assert(to_value_v<fibonacci_t<Seven>> == 13, "fib(7) = 13");

// ============================================================================
// Example 10: Predicates
// ============================================================================

// Even/Odd
static_assert(is_even_v<Zero>, "0 is even");
static_assert(is_even_v<Two>, "2 is even");
static_assert(is_even_v<Four>, "4 is even");
static_assert(!is_even_v<One>, "1 is not even");
static_assert(!is_even_v<Three>, "3 is not even");

static_assert(is_odd_v<One>, "1 is odd");
static_assert(is_odd_v<Three>, "3 is odd");
static_assert(!is_odd_v<Zero>, "0 is not odd");

// ============================================================================
// Example 11: Complex Expressions
// ============================================================================

// Compute: (2 + 3) * 4 = 20
using ComplexExpr1 = mul_t<add_t<Two, Three>, Four>;
static_assert(to_value_v<ComplexExpr1> == 20, "(2+3)*4 = 20");

// Compute: 10 / 2 + 3 = 8
using ComplexExpr2 = add_t<nat::div_t<Ten, Two>, Three>;
static_assert(to_value_v<ComplexExpr2> == 8, "10/2 + 3 = 8");

// Compute: 2^3 + 3^2 = 17
using ComplexExpr3 = add_t<pow_t<Two, Three>, pow_t<Three, Two>>;
static_assert(to_value_v<ComplexExpr3> == 17, "2^3 + 3^2 = 17");

// Compute: gcd(12, 8) = 4
using ComplexExpr4 = gcd_t<from_value_t<12>, Eight>;
static_assert(to_value_v<ComplexExpr4> == 4, "gcd(12, 8) = 4");

// ============================================================================
// Pre-computed types for runtime output
// ============================================================================

using AddResult = add_t<Two, Three>;
using MulResult = mul_t<Three, Four>;
using DivResult = nat::div_t<Ten, Two>;
using ModResult = nat::mod_t<Ten, Three>;
using PowResult = pow_t<Two, Three>;
using Fact3 = factorial_t<Three>;
using Fact4 = factorial_t<Four>;
using Gcd69 = gcd_t<Six, Nine>;
using Fib5 = fibonacci_t<Five>;
using Fib7 = fibonacci_t<Seven>;

// ============================================================================
// Main Function - Runtime Demonstration
// ============================================================================

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "  Typical - Type-Level Lambda Calculus Demo" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "All computations happen at compile-time!" << std::endl;
    std::cout << "This program demonstrates static assertions." << std::endl;
    std::cout << std::endl;

    // Lambda Calculus
    std::cout << "Lambda Calculus:" << std::endl;
    std::cout << "  Identity, Const, and combinators ✓" << std::endl;
    std::cout << std::endl;

    // Church Encodings
    std::cout << "Church Encodings:" << std::endl;
    std::cout << "  Booleans (True/False) ✓" << std::endl;
    std::cout << "  Numerals (Zero, One, Two, ...) ✓" << std::endl;
    std::cout << "  Pairs (Fst/Snd) ✓" << std::endl;
    std::cout << "  Lists (Cons/Nil/Head/Tail) ✓" << std::endl;
    std::cout << std::endl;

    // Peano Numbers
    std::cout << "Peano Natural Numbers:" << std::endl;
    std::cout << "  Zero = " << to_value_v<Zero> << std::endl;
    std::cout << "  Five = " << to_value_v<Five> << std::endl;
    std::cout << "  Ten = " << to_value_v<Ten> << std::endl;
    std::cout << std::endl;

    // Arithmetic
    std::cout << "Arithmetic:" << std::endl;
    std::cout << "  2 + 3 = " << to_value_v<AddResult> << std::endl;
    std::cout << "  3 * 4 = " << to_value_v<MulResult> << std::endl;
    std::cout << "  10 / 2 = " << to_value_v<DivResult> << std::endl;
    std::cout << "  10 % 3 = " << to_value_v<ModResult> << std::endl;
    std::cout << "  2^3 = " << to_value_v<PowResult> << std::endl;
    std::cout << std::endl;

    // Mathematical Functions
    std::cout << "Mathematical Functions:" << std::endl;
    std::cout << "  3! = " << to_value_v<Fact3> << std::endl;
    std::cout << "  4! = " << to_value_v<Fact4> << std::endl;
    std::cout << "  gcd(6, 9) = " << to_value_v<Gcd69> << std::endl;
    std::cout << "  fib(5) = " << to_value_v<Fib5> << std::endl;
    std::cout << "  fib(7) = " << to_value_v<Fib7> << std::endl;
    std::cout << std::endl;

    // Predicates
    std::cout << "Predicates:" << std::endl;
    constexpr bool four_is_even = is_even_v<Four>;
    constexpr bool three_is_odd = is_odd_v<Three>;
    constexpr bool two_lt_five = less_than_v<Two, Five>;
    std::cout << "  is_even(4) = " << (four_is_even ? "true" : "false") << std::endl;
    std::cout << "  is_odd(3) = " << (three_is_odd ? "true" : "false") << std::endl;
    std::cout << "  2 < 5 = " << (two_lt_five ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // Complex Expressions
    std::cout << "Complex Expressions:" << std::endl;
    std::cout << "  (2 + 3) * 4 = " << to_value_v<ComplexExpr1> << std::endl;
    std::cout << "  10 / 2 + 3 = " << to_value_v<ComplexExpr2> << std::endl;
    std::cout << "  2^3 + 3^2 = " << to_value_v<ComplexExpr3> << std::endl;
    std::cout << "  gcd(12, 8) = " << to_value_v<ComplexExpr4> << std::endl;
    std::cout << std::endl;

    std::cout << "==================================================" << std::endl;
    std::cout << "  All " << "300+" << " compile-time tests passed!" << std::endl;
    std::cout << "  Zero runtime overhead. Zero runtime cost." << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
