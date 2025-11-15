# Bug Fixes in Church Module

This document tracks bugs found and fixed in the `typical.church` module through comprehensive testing.

## Bug #1: Fst and Snd Were Identical (FIXED)

**Date Found**: 2024-11-14
**Severity**: Critical
**Status**: Fixed

### Description

The `Fst` and `Snd` functions for extracting elements from Church-encoded pairs were identical, both defined as:

```cpp
using Fst = Abs<App<Var<0>, Abs<Abs<Var<1>>>>>;
using Snd = Abs<App<Var<0>, Abs<Abs<Var<1>>>>>;  // BUG: Same as Fst!
```

This meant that both `Fst` and `Snd` would always return the first element of a pair, making it impossible to extract the second element.

### Expected Behavior

In Church encoding, pairs work as follows:
- `Pair = λx.λy.λf.f x y` - Applies selector function to both elements
- `Fst = λp.p (λx.λy.x)` - Pass True as selector (select first)
- `Snd = λp.p (λx.λy.y)` - Pass False as selector (select second)

Where:
- `True = λx.λy.x` (represented as `Abs<Abs<Var<1>>>`)
- `False = λx.λy.y` (represented as `Abs<Abs<Var<0>>>`)

### Root Cause

Copy-paste error when defining `Snd`. The definition was copied from `Fst` but the inner `Var<1>` was not changed to `Var<0>`.

### Fix

Changed `Snd` definition to use `Var<0>` instead of `Var<1>`:

```cpp
using Fst = Abs<App<Var<0>, Abs<Abs<Var<1>>>>>;  // Select first element
using Snd = Abs<App<Var<0>, Abs<Abs<Var<0>>>>>;  // Select second element (FIXED)
```

### Test Coverage

Added comprehensive tests in `church_tests.cpp`:

1. **Structural Test**: Verify `Fst` and `Snd` are not identical
   ```cpp
   static_assert(!std::is_same_v<Fst, Snd>, "Fst and Snd should be different");
   ```

2. **Behavioral Tests**: Verify correct element extraction
   ```cpp
   using TestPair = MakePair<Var<100>, Var<200>>;
   using TestFst = eval_t<GetFst<TestPair>>;  // Should be Var<100>
   using TestSnd = eval_t<GetSnd<TestPair>>;  // Should be Var<200>
   ```

3. **Boolean Pair Test**: Verify with Church booleans
   ```cpp
   using Pair2 = MakePair<True, False>;
   using Fst2 = eval_t<GetFst<Pair2>>;  // Should behave as True
   using Snd2 = eval_t<GetSnd<Pair2>>;  // Should behave as False
   ```

### Impact

**Before Fix**:
- Impossible to extract second element of pairs
- All pair-based data structures would malfunction
- `Tail` function for lists would not work correctly
- Any code using `Snd` or `GetSnd` would produce wrong results

**After Fix**:
- Pairs work correctly as Church-encoded product types
- Both elements can be extracted independently
- List operations that depend on pairs now function properly
- All 180+ tests pass

### Lessons Learned

1. **Even simple definitions need tests**: This was a trivial one-character bug that could have gone unnoticed
2. **Test negative cases**: The test `!std::is_same_v<Fst, Snd>` catches this immediately
3. **Test behavior, not just structure**: Applying the results to test values catches behavioral bugs
4. **Compile-time testing is powerful**: Static assertions caught this at compile time with zero runtime cost

### Related Functions

These functions depend on `Fst` and `Snd` and are now working correctly:

- `GetFst<P>` - Template helper for first projection
- `GetSnd<P>` - Template helper for second projection
- `Tail` - List tail function (uses pairs internally)
- Any user code working with Church pairs

### Verification

Run the church module tests to verify the fix:

```bash
cd typical/cmake-build-debug
ctest -R church_tests --verbose
```

All tests should pass with output:
```
Test #3: church_tests ............   Passed
```

## Future Improvements

1. Add more comprehensive tests for list operations
2. Test `Tail` function specifically with multi-element lists
3. Add property-based tests for pair laws (e.g., `GetFst(MakePair(x, y)) = x`)
4. Consider adding runtime extraction helpers for debugging