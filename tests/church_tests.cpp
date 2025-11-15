#include <type_traits>

import typical.lambda;
import typical.church;

using namespace typical;

// ============================================================================
// Test Pair Encoding
// ============================================================================

// Test Pair structure
static_assert(is_abs<Pair>::value, "Pair should be an abstraction");

// Test Fst and Snd structure
static_assert(is_abs<Fst>::value, "Fst should be an abstraction");
static_assert(is_abs<Snd>::value, "Snd should be an abstraction");

// BUG TEST: Fst and Snd should NOT be the same!
static_assert(!std::is_same_v<Fst, Snd>, "Fst and Snd should be different");

// Test MakePair
using TestPair = MakePair<Var<100>, Var<200>>;
static_assert(is_app<TestPair>::value, "MakePair should create an application");

// Test GetFst: Fst (Pair a b) -> a
using TestFst = eval_t<GetFst<TestPair>>;
static_assert(std::is_same_v<TestFst, Var<100>>, "Fst should extract first element");

// Test GetSnd: Snd (Pair a b) -> b
using TestSnd = eval_t<GetSnd<TestPair>>;
static_assert(std::is_same_v<TestSnd, Var<200>>, "Snd should extract second element");

// Test with different values
using Pair2 = MakePair<True, False>;
using Fst2 = eval_t<GetFst<Pair2>>;
using Snd2 = eval_t<GetSnd<Pair2>>;
// These should behave as True and False respectively
using FstTest = eval_t<App<App<Fst2, Var<1>>, Var<2>>>;
using SndTest = eval_t<App<App<Snd2, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<FstTest, Var<1>>, "First element should be True");
static_assert(std::is_same_v<SndTest, Var<2>>, "Second element should be False");

// ============================================================================
// Test Church Lists
// ============================================================================

// Test Nil
static_assert(is_abs<Nil>::value, "Nil should be an abstraction");

// Test Cons
static_assert(is_abs<Cons>::value, "Cons should be an abstraction");

// Test MakeCons
using SingletonList = MakeCons<Var<42>, Nil>;
static_assert(is_app<SingletonList>::value, "MakeCons should create an application");

// Test Head on singleton
using HeadResult = eval_t<App<Head, SingletonList>>;
static_assert(std::is_same_v<HeadResult, Var<42>>, "Head of singleton should be element");

// Test Tail on singleton
using TailResult = eval_t<App<Tail, SingletonList>>;
// Tail of singleton should be Nil (after evaluation)
static_assert(is_abs<TailResult>::value, "Tail should reduce to an abstraction");

// Test BuildList
using EmptyList = BuildList<>;
static_assert(std::is_same_v<EmptyList, Nil>, "Empty BuildList should be Nil");

using List1 = BuildList<Var<1>>;
static_assert(is_app<List1>::value, "BuildList with one element should be application");

using List2 = BuildList<Var<1>, Var<2>>;
static_assert(is_app<List2>::value, "BuildList with two elements should be application");

// Test Length on empty list
using LengthNil = eval_t<App<Length, Nil>>;
static_assert(std::is_same_v<LengthNil, Zero>, "Length of Nil should be Zero");

// Test Length on singleton - result should be an abstraction (behaviorally equivalent to One)
using LengthOne = eval_t<App<Length, SingletonList>>;
static_assert(is_abs<LengthOne>::value, "Length of singleton should be an abstraction");

// ============================================================================
// Test Maybe/Option
// ============================================================================

// Test Nothing and Just structures
static_assert(is_abs<Nothing>::value, "Nothing should be an abstraction");
static_assert(is_abs<Just>::value, "Just should be an abstraction");

// Test MakeJust
using JustValue = MakeJust<Var<42>>;
static_assert(is_app<JustValue>::value, "MakeJust should create application");

// Test IsNothing
using IsNothingTrue = eval_t<App<IsNothing, Nothing>>;
using IsNothingTrueTest = eval_t<App<App<IsNothingTrue, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsNothingTrueTest, Var<1>>, "IsNothing Nothing should be True");

using IsNothingFalse = eval_t<App<IsNothing, JustValue>>;
using IsNothingFalseTest = eval_t<App<App<IsNothingFalse, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsNothingFalseTest, Var<2>>, "IsNothing Just should be False");

// Test IsJust
using IsJustFalse = eval_t<App<IsJust, Nothing>>;
using IsJustFalseTest = eval_t<App<App<IsJustFalse, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsJustFalseTest, Var<2>>, "IsJust Nothing should be False");

using IsJustTrue = eval_t<App<IsJust, JustValue>>;
using IsJustTrueTest = eval_t<App<App<IsJustTrue, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsJustTrueTest, Var<1>>, "IsJust Just should be True");

// ============================================================================
// Test Either
// ============================================================================

// Test Left and Right structures
static_assert(is_abs<Left>::value, "Left should be an abstraction");
static_assert(is_abs<Right>::value, "Right should be an abstraction");

// Test MakeLeft and MakeRight
using LeftValue = MakeLeft<Var<10>>;
using RightValue = MakeRight<Var<20>>;
static_assert(is_app<LeftValue>::value, "MakeLeft should create application");
static_assert(is_app<RightValue>::value, "MakeRight should create application");

// Test IsLeft
using IsLeftTrue = eval_t<App<IsLeft, LeftValue>>;
using IsLeftTrueTest = eval_t<App<App<IsLeftTrue, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsLeftTrueTest, Var<1>>, "IsLeft Left should be True");

using IsLeftFalse = eval_t<App<IsLeft, RightValue>>;
using IsLeftFalseTest = eval_t<App<App<IsLeftFalse, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsLeftFalseTest, Var<2>>, "IsLeft Right should be False");

// Test IsRight
using IsRightFalse = eval_t<App<IsRight, LeftValue>>;
using IsRightFalseTest = eval_t<App<App<IsRightFalse, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsRightFalseTest, Var<2>>, "IsRight Left should be False");

using IsRightTrue = eval_t<App<IsRight, RightValue>>;
using IsRightTrueTest = eval_t<App<App<IsRightTrue, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<IsRightTrueTest, Var<1>>, "IsRight Right should be True");

// ============================================================================
// Test List Operations
// ============================================================================

// Test Map structure
static_assert(is_abs<Map>::value, "Map should be an abstraction");

// Test Filter structure
static_assert(is_abs<Filter>::value, "Filter should be an abstraction");

// Test FoldL and FoldR
static_assert(is_abs<FoldL>::value, "FoldL should be an abstraction");
static_assert(is_abs<FoldR>::value, "FoldR should be an abstraction");

// Test Append structure
static_assert(is_abs<Append>::value, "Append should be an abstraction");

// Test Reverse structure
static_assert(is_abs<Reverse>::value, "Reverse should be an abstraction");

// Test Sum structure
static_assert(is_abs<Sum>::value, "Sum should be an abstraction");

// Test Product structure
static_assert(is_abs<Product>::value, "Product should be an abstraction");

// Test Sum on empty list
using SumNil = eval_t<App<Sum, Nil>>;
static_assert(std::is_same_v<SumNil, Zero>, "Sum of empty list should be Zero");

// Test Product on empty list
using ProductNil = eval_t<App<Product, Nil>>;
static_assert(std::is_same_v<ProductNil, One>, "Product of empty list should be One");

int main() { return 0; }
