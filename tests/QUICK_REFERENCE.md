# Lambda Module Tests - Quick Reference

## Test Overview

| Test File | Tests | Focus |
|-----------|-------|-------|
| `lambda_tests.cpp` | 70+ assertions | Core operations and basic functionality |
| `lambda_advanced_tests.cpp` | 130+ assertions | Advanced combinators and complex reductions |

## Running Tests

```bash
# Quick test (from typical directory)
bash debug.sh && cd cmake-build-debug && ctest

# Verbose output
cd cmake-build-debug && ctest --verbose

# Run individual test
./cmake-build-debug/tests/lambda_tests
./cmake-build-debug/tests/lambda_advanced_tests
```

## What's Tested

### Core Operations (lambda_tests.cpp)

✓ **Type System**
- `is_var`, `is_abs`, `is_app`, `is_value` predicates
- `LambdaTerm` concept

✓ **Variable Management**
- `shift_t<Term, Amount, Cutoff>` - Shift De Bruijn indices upward
- `shift_down_t<Term, Amount, Cutoff>` - Shift indices downward
- Proper handling of bound vs. free variables

✓ **Substitution**
- `subst_t<Term, Index, Replacement>` - Capture-avoiding substitution
- Correct shifting of replacement terms
- Preservation of binding structure

✓ **Reduction & Evaluation**
- `reduce_t<Term>` - Single beta reduction step
- `eval_t<Term, MaxSteps>` - Full normalization
- Reduction tracking with `Reduce<T>::reduced` flag

✓ **Church Encodings**
- Booleans: `True`, `False` (conditional selection)
- Numerals: `Zero`, `One`, `Two` (function iteration)
- Operations: `Succ`, `Add`, `Mul`

✓ **Basic Combinators**
- `Id` (Identity): λx.x
- `Const`: λx.λy.x
- `Y` combinator structure

### Advanced Features (lambda_advanced_tests.cpp)

✓ **Boolean Logic**
```cpp
And  = λp.λq.p q p        // Logical AND
Or   = λp.λq.p p q        // Logical OR
Not  = λp.λa.λb.p b a     // Logical NOT
```
All truth table cases validated!

✓ **Predicates**
```cpp
IsZero = λn.n (λx.False) True
```
Tests: IsZero Zero → True, IsZero One → False

✓ **Data Structures**
```cpp
Pair = λx.λy.λf.f x y     // Pair constructor
Fst  = λp.p True          // First projection
Snd  = λp.p False         // Second projection
```

✓ **Advanced Combinators**
```cpp
B = λf.λg.λx.f (g x)      // Composition
S = λf.λg.λx.f x (g x)    // S combinator
K = λx.λy.x               // K combinator (Const)
I = λx.x                  // I combinator (Id)
```
Validates: SKK = I

✓ **Complex Reductions**
- Multi-step reduction chains
- Nested applications (4+ levels deep)
- Capture-avoiding substitution under binders
- Reduction order verification

## Key Test Patterns

### Testing Type Properties
```cpp
static_assert(is_abs<MyTerm>::value, "Should be abstraction");
static_assert(LambdaTerm<MyTerm>, "Should be lambda term");
```

### Testing Reduction
```cpp
using Result = eval_t<Term>;
static_assert(std::is_same_v<Result, Expected>, "Message");
```

### Testing Behavioral Equivalence
```cpp
// For Church booleans
using TestResult = eval_t<App<App<BoolTerm, Var<10>>, Var<20>>>;
static_assert(std::is_same_v<TestResult, Var<10>>, "Should select first");
```

### Testing Structural Properties
```cpp
using Result = eval_t<ComplexTerm>;
static_assert(is_abs<Result>::value, "Should reduce to abstraction");
```

## Common Test Scenarios

1. **Identity Law**: `Id x → x`
2. **Const Law**: `Const x y → x`
3. **Boolean Selection**: `True x y → x`, `False x y → y`
4. **Church Numeral Application**: `n f x → f^n(x)`
5. **Combinator Laws**: `B f g x → f (g x)`, `SKK x → x`

## Compilation Notes

- All tests are compile-time (`static_assert`)
- Failures appear as compiler errors
- Stack warnings are normal for complex reductions
- Requires C++ modules support (Ninja generator)

## Adding New Tests

1. Choose appropriate test file (basic vs advanced)
2. Use descriptive assertion messages
3. Test both positive and negative cases
4. Document complex test scenarios
5. Verify reduction count doesn't exceed limits

## Test Execution Time

- **lambda_tests**: ~0.04 seconds
- **lambda_advanced_tests**: ~0.03 seconds
- **Total**: ~0.07 seconds

All tests are compile-time, so runtime is just process startup overhead!

## See Also

- `README.md` - Full test documentation
- `lambda.ixx` - Lambda calculus implementation
- `debug.sh` - Build script with proper compiler flags