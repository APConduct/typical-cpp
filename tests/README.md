# Lambda Module Tests

This directory contains comprehensive tests for the `typical.lambda` module, which implements typed lambda calculus using C++ template metaprogramming.

## Test Files

- **`lambda_tests.cpp`**: Core functionality tests covering all basic operations
- **`lambda_advanced_tests.cpp`**: Advanced tests demonstrating Church encodings, combinators, and complex reduction sequences

## Overview

The lambda module tests validate all core functionality of the compile-time lambda calculus implementation. Since this is a compile-time library using template metaprogramming, all tests are implemented as `static_assert` statements that verify type equality and properties at compile time.

## Test Coverage

### Core Tests (`lambda_tests.cpp`)

#### 1. Type Traits (`is_var`, `is_abs`, `is_app`, `is_value`)
- Tests type checking predicates for variables, abstractions, and applications
- Validates the `LambdaTerm` concept
- Ensures correct identification of lambda term types

#### 2. Variable Shifting (Upward)
- Tests the `shift_t` operation with various cutoff values
- Validates proper handling of bound vs. free variables
- Tests shifting in abstractions and applications
- Ensures De Bruijn indices are correctly adjusted

#### 3. Variable Shifting (Downward)
- Tests the `shift_down_t` operation
- Validates proper decrementing of indices
- Tests preservation of bound variables during downward shifts

#### 4. Substitution
- Tests the `subst_t` operation for variable replacement
- Validates capture-avoiding substitution
- Tests substitution in abstractions with proper shifting
- Tests substitution in applications

#### 5. Beta Reduction
- Tests single-step reduction with `reduce_t`
- Validates the `reduced` flag for tracking progress
- Tests basic beta reduction: (λx.M) N → M[x := N]
- Tests reduction of non-reducible terms (variables, values)

#### 6. Evaluation
- Tests full reduction to normal form with `eval_t`
- Validates multi-step reduction sequences
- Tests evaluation step limits

#### 7. Church Booleans
- Tests `True` and `False` encodings
- Validates conditional behavior (True selects first, False selects second)
- Tests that church booleans properly reduce when applied

#### 8. Church Numerals
- Tests `Zero`, `One`, and `Two` encodings
- Tests the `Succ` (successor) function
- Tests arithmetic operations:
  - Addition (`Add`)
  - Multiplication (`Mul`)
- Validates that numerals compose correctly

#### 9. Combinators
- Tests the Identity combinator (`Id`)
- Tests the Const combinator (`Const`)
- Tests the Y combinator structure
- Validates combinator behavior through reduction

#### 10. Complex Terms
- Tests nested applications
- Tests self-application
- Tests deeply nested abstractions
- Validates correct behavior of complex reduction sequences

### Advanced Tests (`lambda_advanced_tests.cpp`)

#### 1. Church Boolean Logic
- Implements and tests `And`, `Or`, and `Not` combinators
- Validates all boolean truth table combinations
- Tests that logical operations produce behaviorally correct results

#### 2. Church Numeral Behavior
- Tests that Church numerals properly apply functions
- Implements and tests `IsZero` predicate
- Validates behavioral equivalence of numeral operations

#### 3. Pair Encoding
- Implements Church-encoded pairs using `Pair`, `Fst`, and `Snd`
- Tests pair construction and projection
- Validates data structure encoding patterns

#### 4. Advanced Combinators
- Tests the `B` (composition) combinator
- Tests the `S` combinator
- Tests the `K` and `I` combinators
- Validates `SKK = I` identity

#### 5. De Bruijn Index Preservation
- Tests correct index handling in nested abstractions
- Validates free variable handling across operations
- Tests multiple free variables in applications

#### 6. Reduction Chains
- Tests multi-step reduction sequences
- Validates reduction order (leftmost-outermost)
- Tests that complex terms fully normalize

#### 7. Capture-Avoiding Substitution
- Tests substitution under binders
- Validates that variable capture is properly avoided
- Tests shifting of replacement terms

#### 8. Complex Reductions
- Tests redex resolution in different positions
- Tests deeply nested applications
- Validates correct evaluation of complex combinator expressions

## Running the Tests

### Build and Run

Using the provided debug script (recommended):
```bash
cd typical
bash debug.sh
cd cmake-build-debug
ctest
```

Or manually with Ninja (required for C++ modules):
```bash
cd typical
mkdir -p build
cd build
cmake -G Ninja ..
cmake --build .
ctest
```

### Individual Test Execution
```bash
# Run basic tests
./cmake-build-debug/tests/lambda_tests

# Run advanced tests
./cmake-build-debug/tests/lambda_advanced_tests

# Or run both via CTest
cd cmake-build-debug
ctest --output-on-failure
```

If the test executables run and exit with code 0, all tests passed. Since these are compile-time tests, any failures will be caught during compilation as static assertion errors.

## Test Philosophy

This test suite follows these principles:

1. **Compile-Time Verification**: All tests execute at compile time using `static_assert`, ensuring zero runtime overhead and maximum type safety.

2. **Comprehensive Coverage**: Every major operation in the lambda module is tested with multiple scenarios including edge cases.

3. **Self-Documenting**: Each test includes descriptive messages explaining what property is being validated.

4. **Property-Based**: Tests verify mathematical properties of lambda calculus (substitution correctness, reduction semantics, etc.).

## Adding New Tests

When adding new functionality to the lambda module:

1. Add corresponding `static_assert` tests following the existing patterns
2. Group related tests under descriptive comment headers
3. Include both positive tests (correct behavior) and negative tests (rejecting invalid inputs)
4. Test edge cases like boundary conditions and deeply nested structures
5. Document complex test cases with comments explaining the expected behavior

## Test Statistics

- **Total test cases**: 200+ static assertions
- **Test files**: 2
- **Lines of test code**: ~450
- **Coverage**: All public API functions and types in `typical.lambda`

## Known Limitations

- **Omega Combinator**: The `Omega` combinator is not tested because it represents infinite recursion and would not terminate during evaluation.
- **Step Limits**: Some tests that would create very long reduction sequences are limited by the `MaxSteps` parameter to prevent excessive compilation times.
- **Y Combinator**: Full reduction of the Y combinator with actual recursive functions is not tested due to potential non-termination.
- **Structural Equality**: Church numerals and some other encodings may not have exact structural equality after reduction, so behavioral tests are used instead.

## References

- De Bruijn indices: Variables are represented by their binding depth rather than names
- Beta reduction: The fundamental computation rule of lambda calculus
- Church encodings: Representing data and operations as lambda terms
- Normal form: A term with no reducible subterms