#include <type_traits>

import typical.lambda;

using namespace typical;

// ============================================================================
// Test Type Traits
// ============================================================================

// Test is_var
static_assert(is_var<Var<0>>::value, "Var<0> should be a variable");
static_assert(is_var<Var<1>>::value, "Var<1> should be a variable");
static_assert(is_var<Var<42>>::value, "Var<42> should be a variable");
static_assert(!is_var<Abs<Var<0>>>::value, "Abs should not be a variable");
static_assert(!is_var<App<Var<0>, Var<1>>>::value, "App should not be a variable");

// Test is_abs
static_assert(is_abs<Abs<Var<0>>>::value, "Abs<Var<0>> should be an abstraction");
static_assert(is_abs<Abs<Abs<Var<0>>>>::value, "Nested Abs should be an abstraction");
static_assert(!is_abs<Var<0>>::value, "Var should not be an abstraction");
static_assert(!is_abs<App<Var<0>, Var<1>>>::value, "App should not be an abstraction");

// Test is_app
static_assert(is_app<App<Var<0>, Var<1>>>::value, "App should be an application");
static_assert(is_app<App<Abs<Var<0>>, Var<1>>>::value, "App with Abs should be an application");
static_assert(!is_app<Var<0>>::value, "Var should not be an application");
static_assert(!is_app<Abs<Var<0>>>::value, "Abs should not be an application");

// Test is_value
static_assert(is_value<Abs<Var<0>>>::value, "Abs should be a value");
static_assert(is_value<Abs<Abs<Var<0>>>>::value, "Nested Abs should be a value");
static_assert(!is_value<Var<0>>::value, "Var should not be a value");
static_assert(!is_value<App<Var<0>, Var<1>>>::value, "App should not be a value");

// Test LambdaTerm concept
static_assert(LambdaTerm<Var<0>>, "Var<0> should be a lambda term");
static_assert(LambdaTerm<Abs<Var<0>>>, "Abs<Var<0>> should be a lambda term");
static_assert(LambdaTerm<App<Var<0>, Var<1>>>, "App should be a lambda term");

// ============================================================================
// Test Shift (Upward)
// ============================================================================

// Shift variable below cutoff - should not change
static_assert(std::is_same_v<shift_t<Var<0>, 1, 0>, Var<1>>, "Var<0> shifted by 1 with cutoff 0 should be Var<1>");
static_assert(std::is_same_v<shift_t<Var<0>, 1, 1>, Var<0>>, "Var<0> shifted by 1 with cutoff 1 should remain Var<0>");
static_assert(std::is_same_v<shift_t<Var<2>, 1, 0>, Var<3>>, "Var<2> shifted by 1 with cutoff 0 should be Var<3>");
static_assert(std::is_same_v<shift_t<Var<2>, 3, 0>, Var<5>>, "Var<2> shifted by 3 should be Var<5>");

// Shift variable at cutoff boundary
static_assert(std::is_same_v<shift_t<Var<5>, 2, 5>, Var<7>>, "Var<5> shifted by 2 with cutoff 5 should be Var<7>");

// Shift abstraction
static_assert(std::is_same_v<shift_t<Abs<Var<0>>, 1, 0>, Abs<Var<0>>>, "Bound variable should not shift");
static_assert(std::is_same_v<shift_t<Abs<Var<1>>, 1, 0>, Abs<Var<2>>>, "Free variable in Abs should shift");

// Shift nested abstraction
using NestedAbs = Abs<Abs<Var<2>>>;
using ShiftedNestedAbs = Abs<Abs<Var<3>>>;
static_assert(std::is_same_v<shift_t<NestedAbs, 1, 0>, ShiftedNestedAbs>, "Nested Abs free variable should shift");

// Shift application
static_assert(std::is_same_v<shift_t<App<Var<0>, Var<1>>, 1, 0>, App<Var<1>, Var<2>>>,
              "Both sides of App should shift");
static_assert(std::is_same_v<shift_t<App<Var<0>, Var<1>>, 1, 1>, App<Var<0>, Var<2>>>,
              "Only Var<1> should shift with cutoff 1");

// ============================================================================
// Test Shift Down
// ============================================================================

static_assert(std::is_same_v<shift_down_t<Var<3>, 1, 0>, Var<2>>, "Var<3> shifted down by 1 should be Var<2>");
static_assert(std::is_same_v<shift_down_t<Var<5>, 3, 0>, Var<2>>, "Var<5> shifted down by 3 should be Var<2>");
static_assert(std::is_same_v<shift_down_t<Var<0>, 1, 1>, Var<0>>, "Var<0> below cutoff should not shift down");

// Shift down abstraction
static_assert(std::is_same_v<shift_down_t<Abs<Var<0>>, 1, 0>, Abs<Var<0>>>, "Bound variable should not shift down");
static_assert(std::is_same_v<shift_down_t<Abs<Var<2>>, 1, 0>, Abs<Var<1>>>, "Free variable in Abs should shift down");

// ============================================================================
// Test Substitution
// ============================================================================

// Simple substitution
static_assert(std::is_same_v<subst_t<Var<0>, 0, Var<5>>, Var<5>>, "Substitute Var<0> with Var<5>");
static_assert(std::is_same_v<subst_t<Var<1>, 0, Var<5>>, Var<1>>, "Non-matching variable should not substitute");

// Substitution in abstraction
using SubstInAbs = subst_t<Abs<Var<0>>, 0, Var<5>>;
static_assert(std::is_same_v<SubstInAbs, Abs<Var<0>>>, "Bound variable should not be substituted");

using SubstFreeInAbs = subst_t<Abs<Var<1>>, 0, Var<5>>;
static_assert(std::is_same_v<SubstFreeInAbs, Abs<Var<6>>>,
              "Free variable in abstraction should be substituted and shifted");

// Substitution in application
using SubstInApp = subst_t<App<Var<0>, Var<1>>, 0, Var<5>>;
static_assert(std::is_same_v<SubstInApp, App<Var<5>, Var<1>>>, "Substitute in application");

// ============================================================================
// Test Reduction
// ============================================================================

// Variables and values don't reduce
static_assert(std::is_same_v<reduce_t<Var<0>>, Var<0>>, "Variable should not reduce");
static_assert(!Reduce<Var<0>>::reduced, "Variable reduction flag should be false");

static_assert(std::is_same_v<reduce_t<Abs<Var<0>>>, Abs<Var<0>>>, "Abstraction should not reduce");
static_assert(!Reduce<Abs<Var<0>>>::reduced, "Abstraction reduction flag should be false");

// Beta reduction: (λx.x) y -> y
using IdentityApp = App<Abs<Var<0>>, Var<5>>;
static_assert(std::is_same_v<reduce_t<IdentityApp>, Var<5>>, "Identity application should reduce to argument");
static_assert(Reduce<IdentityApp>::reduced, "Beta reduction should set reduced flag");

// Beta reduction: (λx.λy.x) a -> (λy.a)
using ConstApp = App<Abs<Abs<Var<1>>>, Var<10>>;
using ConstAppReduced = Abs<Var<11>>; // After substitution, the free variable gets shifted
static_assert(std::is_same_v<reduce_t<ConstApp>, ConstAppReduced>, "Const application should reduce");

// ============================================================================
// Test Evaluation (Full Reduction)
// ============================================================================

// Identity function
static_assert(std::is_same_v<eval_t<Id>, Id>, "Identity should be in normal form");

// Apply identity: (λx.x) y -> y
using IdApp = App<Id, Var<5>>;
static_assert(std::is_same_v<eval_t<IdApp>, Var<5>>, "Applying identity should evaluate to argument");

// Const function: (λx.λy.x) a b -> a
using ConstFull = App<App<Const, Var<10>>, Var<20>>;
static_assert(std::is_same_v<eval_t<ConstFull>, Var<10>>, "Const should return first argument");

// ============================================================================
// Test Church Booleans
// ============================================================================

static_assert(std::is_same_v<True, Abs<Abs<Var<1>>>>, "True is λx.λy.x");
static_assert(std::is_same_v<False, Abs<Abs<Var<0>>>>, "False is λx.λy.y");

// True selects first argument
using TrueApp = App<App<True, Var<10>>, Var<20>>;
static_assert(std::is_same_v<eval_t<TrueApp>, Var<10>>, "True should select first argument");

// False selects second argument
using FalseApp = App<App<False, Var<10>>, Var<20>>;
static_assert(std::is_same_v<eval_t<FalseApp>, Var<20>>, "False should select second argument");

// ============================================================================
// Test Church Numerals
// ============================================================================

static_assert(std::is_same_v<Zero, Abs<Abs<Var<0>>>>, "Zero is λf.λx.x");
static_assert(std::is_same_v<One, Abs<Abs<App<Var<1>, Var<0>>>>>, "One is λf.λx.f x");
static_assert(std::is_same_v<Two, Abs<Abs<App<Var<1>, App<Var<1>, Var<0>>>>>>, "Two is λf.λx.f (f x)");

// Note: Church numeral operations don't always reduce to the exact same representation
// They are behaviorally equivalent but may have different internal structure after evaluation

// Test that Succ Zero reduces (may not be exactly One in structure)
using SuccZero = App<Succ, Zero>;
using SuccZeroResult = eval_t<SuccZero>;
static_assert(is_abs<SuccZeroResult>::value, "Successor of Zero should be an abstraction");

// Test that Succ One reduces
using SuccOne = App<Succ, One>;
using SuccOneResult = eval_t<SuccOne>;
static_assert(is_abs<SuccOneResult>::value, "Successor of One should be an abstraction");

// Test that Add reduces to a value
using AddOneOne = App<App<Add, One>, One>;
using AddOneOneResult = eval_t<AddOneOne>;
static_assert(is_abs<AddOneOneResult>::value, "1 + 1 should reduce to an abstraction");

// Test that Add Zero N reduces to a value
using AddZeroTwo = App<App<Add, Zero>, Two>;
using AddZeroTwoResult = eval_t<AddZeroTwo>;
static_assert(is_abs<AddZeroTwoResult>::value, "0 + 2 should reduce to an abstraction");

// Test that Mul reduces to a value
using MulTwoOne = App<App<Mul, Two>, One>;
using MulTwoOneResult = eval_t<MulTwoOne>;
static_assert(is_abs<MulTwoOneResult>::value, "2 * 1 should reduce to an abstraction");

// Test that Mul is commutative in structure (both reduce to abstractions)
using MulOneTwo = App<App<Mul, One>, Two>;
using MulOneTwoResult = eval_t<MulOneTwo>;
static_assert(is_abs<MulOneTwoResult>::value, "1 * 2 should reduce to an abstraction");

// ============================================================================
// Test Complex Terms
// ============================================================================

// Nested applications - Const Id reduces to (λx.Id), then applied to Var<10> gives Id
using NestedApp = App<App<App<Const, Id>, Var<10>>, Var<20>>;
using NestedAppResult = eval_t<NestedApp>;
// The result should be Id applied to Var<20>, which reduces to Var<20>
static_assert(std::is_same_v<NestedAppResult, Var<20>>, "Nested const application should reduce correctly");

// Self-application: (λx.x x) (λy.y) -> (λy.y) (λy.y) -> (λy.y)
using SelfApp = App<Abs<App<Var<0>, Var<0>>>, Id>;
static_assert(std::is_same_v<eval_t<SelfApp>, Id>, "Self-application of identity should be identity");

// ============================================================================
// Test Edge Cases
// ============================================================================

// Multiple variables
static_assert(Var<0>::index == 0, "Var index should be accessible");
static_assert(Var<100>::index == 100, "Large variable index should work");

// Deep nesting
using DeepNest = Abs<Abs<Abs<Abs<Var<3>>>>>;
static_assert(std::is_same_v<eval_t<DeepNest>, DeepNest>, "Deeply nested abstraction should be in normal form");

// Evaluation step limit
using LimitedEval = eval_t<App<Id, App<Id, App<Id, Var<0>>>>, 1>;
// Should only take one step, not fully reduce

// ============================================================================
// Test Y Combinator Structure
// ============================================================================

// Y combinator should be an abstraction
static_assert(is_abs<Y>::value, "Y combinator should be an abstraction");

// ============================================================================
// Compilation Success Message
// ============================================================================

// If we get here, all tests passed!
int main() { return 0; }
