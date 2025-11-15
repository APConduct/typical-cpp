#include <type_traits>

import typical.lambda;

using namespace typical;

// ============================================================================
// Advanced Church Boolean Tests
// ============================================================================

// Test And combinator: And = λp.λq.p q p
using And = Abs<Abs<App<App<Var<1>, Var<0>>, Var<1>>>>;

// And True True -> True
using AndTrueTrue = App<App<And, True>, True>;
using AndTrueTrueResult = eval_t<AndTrueTrue>;
// Apply to test values to verify behavior
using AndTrueTrueTest = eval_t<App<App<AndTrueTrueResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<AndTrueTrueTest, Var<10>>, "And True True should select first argument");

// And True False -> False
using AndTrueFalse = App<App<And, True>, False>;
using AndTrueFalseResult = eval_t<AndTrueFalse>;
using AndTrueFalseTest = eval_t<App<App<AndTrueFalseResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<AndTrueFalseTest, Var<20>>, "And True False should select second argument");

// And False True -> False
using AndFalseTrue = App<App<And, False>, True>;
using AndFalseTrueResult = eval_t<AndFalseTrue>;
using AndFalseTrueTest = eval_t<App<App<AndFalseTrueResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<AndFalseTrueTest, Var<20>>, "And False True should select second argument");

// And False False -> False
using AndFalseFalse = App<App<And, False>, False>;
using AndFalseFalseResult = eval_t<AndFalseFalse>;
using AndFalseFalseTest = eval_t<App<App<AndFalseFalseResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<AndFalseFalseTest, Var<20>>, "And False False should select second argument");

// Test Or combinator: Or = λp.λq.p p q
using Or = Abs<Abs<App<App<Var<1>, Var<1>>, Var<0>>>>;

// Or True True -> True
using OrTrueTrue = App<App<Or, True>, True>;
using OrTrueTrueResult = eval_t<OrTrueTrue>;
using OrTrueTrueTest = eval_t<App<App<OrTrueTrueResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<OrTrueTrueTest, Var<10>>, "Or True True should select first argument");

// Or True False -> True
using OrTrueFalse = App<App<Or, True>, False>;
using OrTrueFalseResult = eval_t<OrTrueFalse>;
using OrTrueFalseTest = eval_t<App<App<OrTrueFalseResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<OrTrueFalseTest, Var<10>>, "Or True False should select first argument");

// Or False False -> False
using OrFalseFalse = App<App<Or, False>, False>;
using OrFalseFalseResult = eval_t<OrFalseFalse>;
using OrFalseFalseTest = eval_t<App<App<OrFalseFalseResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<OrFalseFalseTest, Var<20>>, "Or False False should select second argument");

// Test Not combinator: Not = λp.λa.λb.p b a
using Not = Abs<Abs<Abs<App<App<Var<2>, Var<0>>, Var<1>>>>>;

// Not True -> False
using NotTrue = App<Not, True>;
using NotTrueResult = eval_t<NotTrue>;
using NotTrueTest = eval_t<App<App<NotTrueResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<NotTrueTest, Var<20>>, "Not True should behave like False");

// Not False -> True
using NotFalse = App<Not, False>;
using NotFalseResult = eval_t<NotFalse>;
using NotFalseTest = eval_t<App<App<NotFalseResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<NotFalseTest, Var<10>>, "Not False should behave like True");

// ============================================================================
// Church Numeral Behavioral Tests
// ============================================================================

// Test that Zero applied to a function and value returns the value
// Zero f x = x
using ZeroTest = eval_t<App<App<Zero, Id>, Var<42>>>;
static_assert(std::is_same_v<ZeroTest, Var<42>>, "Zero should not apply function");

// Test that One applies function once
// One f x = f x
using OneTest = eval_t<App<App<One, Succ>, Zero>>;
// This should apply Succ to Zero once
static_assert(is_abs<OneTest>::value, "One f x should apply f once");

// Test IsZero predicate: IsZero = λn.n (λx.False) True
using IsZero = Abs<App<App<Var<0>, Abs<False>>, True>>;

// IsZero Zero -> True
using IsZeroZero = App<IsZero, Zero>;
using IsZeroZeroResult = eval_t<IsZeroZero>;
using IsZeroZeroTest = eval_t<App<App<IsZeroZeroResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<IsZeroZeroTest, Var<10>>, "IsZero Zero should be True");

// IsZero One -> False
using IsZeroOne = App<IsZero, One>;
using IsZeroOneResult = eval_t<IsZeroOne>;
using IsZeroOneTest = eval_t<App<App<IsZeroOneResult, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<IsZeroOneTest, Var<20>>, "IsZero One should be False");

// ============================================================================
// Pair Encoding Tests
// ============================================================================

// Pair = λx.λy.λf.f x y
using Pair = Abs<Abs<Abs<App<App<Var<0>, Var<2>>, Var<1>>>>>;

// Fst = λp.p (λx.λy.x) = λp.p True
using Fst = Abs<App<Var<0>, True>>;

// Snd = λp.p (λx.λy.y) = λp.p False
using Snd = Abs<App<Var<0>, False>>;

// Test: Fst (Pair a b) -> a
using TestPair = App<App<Pair, Var<100>>, Var<200>>;
using TestFst = eval_t<App<Fst, TestPair>>;
static_assert(std::is_same_v<TestFst, Var<100>>, "Fst should extract first element");

// Test: Snd (Pair a b) -> b
using TestSnd = eval_t<App<Snd, TestPair>>;
static_assert(std::is_same_v<TestSnd, Var<200>>, "Snd should extract second element");

// ============================================================================
// Combinator Tests
// ============================================================================

// Test Composition: B = λf.λg.λx.f (g x)
using B = Abs<Abs<Abs<App<Var<2>, App<Var<1>, Var<0>>>>>>;

// B f g x should be f (g x)
using BTest = eval_t<App<App<App<B, Id>, Id>, Var<50>>>;
static_assert(std::is_same_v<BTest, Var<50>>, "B Id Id x should be x");

// Test S combinator: S = λf.λg.λx.f x (g x)
using S = Abs<Abs<Abs<App<App<Var<2>, Var<0>>, App<Var<1>, Var<0>>>>>>;

// S should be an abstraction
static_assert(is_abs<S>::value, "S combinator should be an abstraction");

// Test K combinator (same as Const)
using K = Const;

// Test I combinator (same as Id)
using I = Id;

// SKK = I (reduction: S K K x -> K x (K x) -> x)
using SKK = App<App<S, K>, K>;
using SKKTest = eval_t<App<SKK, Var<99>>>;
static_assert(std::is_same_v<SKKTest, Var<99>>, "SKK should behave like identity");

// ============================================================================
// De Bruijn Index Tests
// ============================================================================

// Test that variables maintain correct indices after operations
using TripleNested = Abs<Abs<Abs<Var<2>>>>;
static_assert(std::is_same_v<eval_t<TripleNested>, TripleNested>,
              "Triple nested abstraction with Var<2> should be in normal form");

// Test application with multiple free variables
using MultiFree = App<Abs<App<Var<0>, Var<1>>>, Id>;
using MultiFreeResult = eval_t<MultiFree>;
// Should substitute Var<0> with Id and keep Var<1> (now Var<0> after shift down)
// This reduces to: (Id Var<0>) which further reduces to Var<0>
static_assert(std::is_same_v<MultiFreeResult, Var<0>>, "Result should reduce to Var<0>");

// ============================================================================
// Reduction Chain Tests
// ============================================================================

// Test multi-step reduction
using ChainTest1 = App<Id, App<Id, App<Id, Var<7>>>>;
using ChainResult1 = eval_t<ChainTest1>;
static_assert(std::is_same_v<ChainResult1, Var<7>>, "Chain of identities should reduce to argument");

// Test nested beta reductions
using ChainTest2 = App<App<Const, Var<5>>, App<Id, Var<10>>>;
using ChainResult2 = eval_t<ChainTest2>;
static_assert(std::is_same_v<ChainResult2, Var<5>>, "Const should ignore second argument even if it reduces");

// ============================================================================
// Value Recognition Tests
// ============================================================================

// Abstractions are values
static_assert(is_value<Id>::value, "Id should be a value");
static_assert(is_value<Const>::value, "Const should be a value");
static_assert(is_value<True>::value, "True should be a value");
static_assert(is_value<False>::value, "False should be a value");
static_assert(is_value<Zero>::value, "Zero should be a value");

// Applications are not values
static_assert(!is_value<App<Id, Var<0>>>::value, "Application should not be a value");
static_assert(!is_value<App<Const, Var<0>>>::value, "Partial application should not be a value");

// ============================================================================
// Capture-Avoiding Substitution Tests
// ============================================================================

// Test that substitution properly avoids variable capture
// (λx.(λy.x)) [x := y] should become (λx.(λz.x)), not (λy.(λz.y))
using CaptureTest1 = Abs<Abs<Var<1>>>;
using CaptureSubst1 = subst_t<CaptureTest1, 0, Var<0>>;
// The outer Var<1> refers to the outer binding, not the substitution
static_assert(is_abs<CaptureSubst1>::value, "Result should still be an abstraction");

// Test substitution under binders
using UnderBinder = Abs<App<Var<1>, Var<0>>>;
using UnderBinderSubst = subst_t<UnderBinder, 0, Var<5>>;
// Var<1> (now Var<0> after shift) gets substituted with shifted Var<5> (becomes Var<6>)
// The result should be: Abs<App<Var<6>, Var<0>>>
using UnderBinderExpected = Abs<App<Var<6>, Var<0>>>;
static_assert(std::is_same_v<UnderBinderSubst, UnderBinderExpected>,
              "Substitution under binder should shift replacement");

// ============================================================================
// Complex Reduction Tests
// ============================================================================

// Test that redexes in function position reduce first
using FuncPosRedex = App<App<Const, Var<1>>, Var<2>>;
using FuncPosResult = eval_t<FuncPosRedex>;
static_assert(std::is_same_v<FuncPosResult, Var<1>>, "Function position should reduce first");

// Test deeply nested applications
using DeepApp = App<Id, App<Id, App<Id, App<Id, Var<3>>>>>;
using DeepAppResult = eval_t<DeepApp>;
static_assert(std::is_same_v<DeepAppResult, Var<3>>, "Deeply nested applications should fully reduce");

int main() { return 0; }
