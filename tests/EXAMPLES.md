# Lambda Module Test Examples

This document provides practical examples of how to write tests for the lambda module, demonstrating common patterns and techniques.

## Basic Test Structure

```cpp
#include <type_traits>
import typical.lambda;

using namespace typical;

// Write your static assertions here
static_assert(/* condition */, "Descriptive message");

int main() { return 0; }
```

## Example 1: Testing a New Combinator

Let's say you want to test a new combinator `Flip = λf.λx.λy.f y x`:

```cpp
// Define the combinator
using Flip = Abs<Abs<Abs<App<App<Var<2>, Var<0>>, Var<1>>>>>;

// Test 1: Check it's an abstraction
static_assert(is_abs<Flip>::value, "Flip should be an abstraction");

// Test 2: Check it's a lambda term
static_assert(LambdaTerm<Flip>, "Flip should be a lambda term");

// Test 3: Test its behavior: Flip Const a b → Const b a → b
using FlipConst = App<App<App<Flip, Const>, Var<10>>, Var<20>>;
using FlipConstResult = eval_t<FlipConst>;
static_assert(std::is_same_v<FlipConstResult, Var<20>>, 
              "Flip Const should swap arguments");

// Test 4: Verify Flip Flip = Id (double flip is identity)
using FlipFlip = App<Flip, Flip>;
// This test would require applying to arguments to verify behavior
```

## Example 2: Testing Church Numeral Operations

```cpp
// Define Church numeral Three
using Three = Abs<Abs<App<Var<1>, App<Var<1>, App<Var<1>, Var<0>>>>>>;

// Test 1: Verify structure
static_assert(is_abs<Three>::value, "Three should be an abstraction");

// Test 2: Test Succ Three → Four
using Four = Abs<Abs<App<Var<1>, App<Var<1>, App<Var<1>, App<Var<1>, Var<0>>>>>>>;
using SuccThree = eval_t<App<Succ, Three>>;
// Note: May not be structurally identical, test behavior instead
static_assert(is_abs<SuccThree>::value, "Succ Three should be an abstraction");

// Test 3: Test behavioral equivalence by applying to test function
// Three f x should apply f three times: f (f (f x))
// We can't easily verify this at compile time without a concrete f,
// but we can verify it reduces to a value
using ThreeApplied = eval_t<App<App<Three, Succ>, Zero>>;
static_assert(is_abs<ThreeApplied>::value, 
              "Three applied to Succ and Zero should produce a numeral");
```

## Example 3: Testing Conditional Logic

```cpp
// If-Then-Else: λp.λt.λe.p t e (same structure as boolean)
using IfThenElse = Abs<Abs<Abs<App<App<Var<2>, Var<1>>, Var<0>>>>>;

// Test 1: If True Then a Else b → a
using IfTrue = eval_t<App<App<App<IfThenElse, True>, Var<100>>, Var<200>>>;
static_assert(std::is_same_v<IfTrue, Var<100>>, 
              "If True should select then branch");

// Test 2: If False Then a Else b → b
using IfFalse = eval_t<App<App<App<IfThenElse, False>, Var<100>>, Var<200>>>;
static_assert(std::is_same_v<IfFalse, Var<200>>, 
              "If False should select else branch");

// Test 3: Nested conditionals
using NestedIf = App<App<App<IfThenElse, True>, 
                          App<App<App<IfThenElse, False>, Var<1>>, Var<2>>>, 
                     Var<3>>;
using NestedIfResult = eval_t<NestedIf>;
static_assert(std::is_same_v<NestedIfResult, Var<2>>, 
              "Nested If True (If False 1 2) 3 should be 2");
```

## Example 4: Testing Variable Shifting

```cpp
// Test shifting free variables
using Term1 = Abs<App<Var<0>, Var<1>>>;  // λx. x y (y is free)

// Shift with cutoff 0 (shift all free variables)
using Shifted1 = shift_t<Term1, 1, 0>;
using Expected1 = Abs<App<Var<0>, Var<2>>>;  // λx. x y' (y shifted to y')
static_assert(std::is_same_v<Shifted1, Expected1>, 
              "Free variable should shift");

// Test shifting under multiple binders
using Term2 = Abs<Abs<Var<2>>>;  // λx.λy.z (z is free)
using Shifted2 = shift_t<Term2, 2, 0>;
using Expected2 = Abs<Abs<Var<4>>>;  // λx.λy.z'' (z shifted by 2)
static_assert(std::is_same_v<Shifted2, Expected2>, 
              "Free variable under binders should shift");
```

## Example 5: Testing Substitution

```cpp
// Test simple substitution: [x := a]x = a
using Subst1 = subst_t<Var<0>, 0, Var<5>>;
static_assert(std::is_same_v<Subst1, Var<5>>, 
              "Substituting matching variable");

// Test non-matching: [x := a]y = y
using Subst2 = subst_t<Var<1>, 0, Var<5>>;
static_assert(std::is_same_v<Subst2, Var<1>>, 
              "Non-matching variable unchanged");

// Test capture avoidance: [x := y](λy.x)
// Should become (λy'.y) not (λy.y)
using Term3 = Abs<Var<1>>;  // λy.x
using Subst3 = subst_t<Term3, 0, Var<0>>;  // [x := y]
using Expected3 = Abs<Var<1>>;  // λy'.y (shifted)
static_assert(std::is_same_v<Subst3, Expected3>, 
              "Substitution should avoid capture");
```

## Example 6: Testing Custom Data Structures

```cpp
// List encoding: Cons = λh.λt.λc.λn.c h (t c n)
using Cons = Abs<Abs<Abs<Abs<
    App<App<Var<1>, Var<3>>, 
        App<App<Var<2>, Var<1>>, Var<0>>>
>>>>;

// Nil = λc.λn.n
using Nil = Abs<Abs<Var<0>>>;

// Head = λl.l (λh.λt.h) error
using Head = Abs<App<App<Var<0>, Abs<Abs<Var<1>>>>, Id>>;

// Test: Head (Cons a Nil) → a
using SingletonList = App<App<Cons, Var<42>>, Nil>;
using HeadResult = eval_t<App<Head, SingletonList>>;
static_assert(std::is_same_v<HeadResult, Var<42>>, 
              "Head of singleton list should be the element");
```

## Example 7: Testing Reduction Properties

```cpp
// Test idempotence: eval (eval t) = eval t for normal forms
using NormalForm = Abs<Var<0>>;
using Eval1 = eval_t<NormalForm>;
using Eval2 = eval_t<Eval1>;
static_assert(std::is_same_v<Eval1, Eval2>, 
              "Evaluating normal form should be idempotent");

// Test reduction flag
static_assert(!Reduce<Abs<Var<0>>>::reduced, 
              "Abstraction should not reduce");
static_assert(Reduce<App<Abs<Var<0>>, Var<1>>>::reduced, 
              "Beta redex should reduce");

// Test step limit
using DeepTerm = App<Id, App<Id, App<Id, Var<0>>>>;
using Limited = eval_t<DeepTerm, 1>;  // Only one step
static_assert(!std::is_same_v<Limited, Var<0>>, 
              "Limited steps should not fully reduce");
using FullEval = eval_t<DeepTerm, 10>;  // Enough steps
static_assert(std::is_same_v<FullEval, Var<0>>, 
              "Full evaluation should reduce completely");
```

## Example 8: Testing Error Cases (Negative Tests)

```cpp
// These tests verify that certain types are NOT equal

// Verify True ≠ False
static_assert(!std::is_same_v<True, False>, 
              "True and False should be different");

// Verify different numerals are different
static_assert(!std::is_same_v<Zero, One>, 
              "Zero and One should be different");
static_assert(!std::is_same_v<One, Two>, 
              "One and Two should be different");

// Verify variables are distinguished by index
static_assert(!std::is_same_v<Var<0>, Var<1>>, 
              "Different variables should be different");

// Verify reduction doesn't happen for non-redexes
static_assert(std::is_same_v<Var<0>, reduce_t<Var<0>>>, 
              "Variable should not reduce");
static_assert(std::is_same_v<Abs<Var<0>>, reduce_t<Abs<Var<0>>>>, 
              "Value should not reduce");
```

## Example 9: Testing Combinator Laws

```cpp
// Test composition law: B f g x = f (g x)
using CompLaw = eval_t<App<App<App<B, Id>, Id>, Var<7>>>;
static_assert(std::is_same_v<CompLaw, Var<7>>, 
              "B Id Id x should be x");

// Test S combinator: S K K x = I x = x
using SKKx = eval_t<App<App<App<S, K>, K>, Var<8>>>;
static_assert(std::is_same_v<SKKx, Var<8>>, 
              "SKK should behave as identity");

// Test K combinator: K x y = x
using Kxy = eval_t<App<App<K, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<Kxy, Var<1>>, 
              "K should return first argument");
```

## Example 10: Performance Testing

```cpp
// Test that complex terms don't exceed step limits
using Complex = App<App<Add, Two>, Two>;
// This should complete within default step limit
using ComplexResult = eval_t<Complex>;
static_assert(is_abs<ComplexResult>::value, 
              "Complex term should reduce to value");

// If a term needs more steps, increase the limit
using VeryComplex = App<App<Mul, Three>, Three>;
using VeryComplexResult = eval_t<VeryComplex, 5000>;  // Higher limit
static_assert(is_abs<VeryComplexResult>::value, 
              "Very complex term should reduce with higher limit");
```

## Best Practices

1. **Use Descriptive Names**: `FlipConst` is better than `Test1`
2. **Add Clear Messages**: Every assertion should explain what's being tested
3. **Test Boundaries**: Test with Var<0>, Var<1>, and higher indices
4. **Test Both Paths**: Test true and false cases for conditionals
5. **Document Complex Cases**: Add comments explaining non-obvious tests
6. **Group Related Tests**: Use comment headers to organize tests
7. **Test Structural and Behavioral**: Some terms need both types of tests
8. **Consider Step Limits**: Increase MaxSteps for complex reductions

## Common Pitfalls

❌ **Don't assume structural equality for complex reductions**
```cpp
// This might fail even if behaviorally equivalent:
static_assert(std::is_same_v<eval_t<App<Succ, Zero>>, One>, "...");
```

✓ **Instead, test behavior or structure separately**
```cpp
// Test that result is an abstraction:
static_assert(is_abs<eval_t<App<Succ, Zero>>>::value, "...");
// Or test behavior by applying it:
using Applied = eval_t<App<App<Result, SomeFunc>, SomeArg>>;
```

❌ **Don't test non-terminating terms**
```cpp
// This will cause compilation issues:
using OmegaResult = eval_t<Omega>;  // Never terminates!
```

✓ **Only test that structure is correct**
```cpp
static_assert(is_app<Omega>::value, "Omega should be application");
```

## Running Your Tests

1. Add your test file to `tests/CMakeLists.txt`
2. Run `bash debug.sh` from the `typical` directory
3. Run `ctest` from `cmake-build-debug`
4. Check compilation output for static assertion failures

## Further Reading

- See `lambda_tests.cpp` for basic examples
- See `lambda_advanced_tests.cpp` for advanced patterns
- See `README.md` for full test documentation
- See `lambda.ixx` for implementation details