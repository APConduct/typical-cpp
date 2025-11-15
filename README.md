# Typical - Type-Level Lambda Calculus in C++

A compile-time lambda calculus implementation using C++ template metaprogramming and modules.

## Overview

**Typical** is a header-only C++ library that implements untyped lambda calculus at compile time. It uses template metaprogramming to perform lambda calculus reductions during compilation, enabling:

- Church encodings for booleans, numerals, pairs, lists, and more
- Beta reduction and full evaluation
- De Bruijn indices for variable representation
- Capture-avoiding substitution
- Zero runtime overhead (everything happens at compile time)

## Features

- ✅ **Pure Compile-Time**: All computation happens during compilation via `static_assert`
- ✅ **Type-Safe**: Leverages C++ type system for correctness
- ✅ **Zero Runtime Cost**: No runtime overhead whatsoever
- ✅ **Comprehensive**: Includes Church encodings, combinators, and data structures
- ✅ **Well-Tested**: 200+ compile-time assertions, 100% pass rate
- ✅ **Modern C++**: Uses C++20 modules for clean namespace management

## Modules

- **`typical.lambda`** - Core lambda calculus operations
  - Variables, abstractions, applications
  - Shifting and substitution
  - Beta reduction and evaluation
  - Basic combinators (Id, Const, Y)
  - Church booleans and numerals

- **`typical.church`** - Church encodings
  - Pairs (Fst, Snd)
  - Lists (Cons, Nil, Head, Tail, Length)
  - Maybe/Option types (Nothing, Just)
  - Either types (Left, Right)
  - List operations (Map, Filter, Fold, etc.)

- **`typical.eq`** - Equality operations
- **`typical.nat`** - Natural number operations
- **`typical.set`** - Set operations
- **`typical.calculus`** - Calculus operations

## Quick Start

### Requirements

- **CMake** 3.28 or later
- **Ninja** build system (required for C++ modules)
- **LLVM/Clang** 17.4+ (from Homebrew on macOS)
- **C++20** standard

### Installation

```bash
# Install dependencies (macOS)
brew install llvm cmake ninja

# Clone repository (if applicable)
# cd into project directory
cd typical
```

### Build and Test

```bash
# Complete build and test (recommended for first time)
./debug.sh

# Run tests only (after build)
./run_tests.sh

# Run specific tests
./run_tests.sh -f lambda

# Run with verbose output
./run_tests.sh -v

# List all tests
./run_tests.sh -l
```

## Usage Example

```cpp
#include <type_traits>
import typical.lambda;
import typical.church;

using namespace typical;

// Church booleans
using MyBool = True;
using Result = eval_t<App<App<MyBool, Var<1>>, Var<2>>>;
static_assert(std::is_same_v<Result, Var<1>>, "True selects first");

// Church numerals
using Three = App<Succ, App<Succ, App<Succ, Zero>>>;
using Five = eval_t<App<App<Add, Two>, Three>>;

// Pairs
using MyPair = MakePair<Var<100>, Var<200>>;
using First = eval_t<GetFst<MyPair>>;   // Var<100>
using Second = eval_t<GetSnd<MyPair>>;  // Var<200>

// Lists
using MyList = BuildList<Var<1>, Var<2>, Var<3>>;
using ListHead = eval_t<App<Head, MyList>>;  // Var<1>

// Custom combinators
using B = Abs<Abs<Abs<App<Var<2>, App<Var<1>, Var<0>>>>>>;  // Composition
using Composed = eval_t<App<App<App<B, Id>, Id>, Var<42>>>;  // Var<42>
```

## Project Structure

```
typical/
├── include/
│   └── modules/
│       ├── typical.ixx           # Main module
│       └── typical/
│           ├── lambda.ixx        # Core lambda calculus
│           ├── church.ixx        # Church encodings
│           ├── eq.ixx            # Equality
│           ├── nat.ixx           # Natural numbers
│           ├── set.ixx           # Sets
│           └── calculus.ixx      # Calculus
├── tests/
│   ├── lambda_tests.cpp          # Core tests (70+ assertions)
│   ├── lambda_advanced_tests.cpp # Advanced tests (130+ assertions)
│   ├── church_tests.cpp          # Church encoding tests (40+ assertions)
│   ├── README.md                 # Test documentation
│   ├── QUICK_REFERENCE.md        # Test quick reference
│   ├── EXAMPLES.md               # Test writing examples
│   └── BUG_FIXES.md              # Bug tracking
├── debug.sh                      # Build and test script
├── run_tests.sh                  # Test runner with options
├── SCRIPTS.md                    # Script documentation
└── CMakeLists.txt                # Build configuration
```

## Scripts

### `debug.sh`
Complete build and test workflow. Use this for:
- Initial setup
- After pulling changes
- Full rebuild + test cycle

```bash
./debug.sh
```

### `run_tests.sh`
Flexible test runner. Use this for:
- Quick test runs
- Filtering specific tests
- Verbose output

```bash
# Run all tests
./run_tests.sh

# Filter tests
./run_tests.sh -f lambda

# Verbose output
./run_tests.sh -v

# Help
./run_tests.sh -h
```

See `SCRIPTS.md` for detailed documentation.

## Testing

The project includes comprehensive compile-time tests:

- **200+ static assertions** covering all functionality
- **3 test suites**: lambda, lambda_advanced, church
- **100% pass rate** with zero runtime overhead
- **Full documentation** with examples

### Run Tests

```bash
# All tests
./run_tests.sh

# Lambda tests only
./run_tests.sh -f lambda

# Church tests only
./run_tests.sh -f church

# With verbose output
./run_tests.sh -v
```

### Test Documentation

- `tests/README.md` - Comprehensive test documentation
- `tests/QUICK_REFERENCE.md` - Quick lookup guide
- `tests/EXAMPLES.md` - 10 detailed test examples
- `tests/BUG_FIXES.md` - Bug tracking and fixes

## Lambda Calculus Primer

### Terms

- **Variable**: `Var<0>`, `Var<1>`, etc. (De Bruijn indices)
- **Abstraction**: `Abs<Body>` (lambda function)
- **Application**: `App<Func, Arg>` (function call)

### Operations

- **Shift**: `shift_t<Term, Amount, Cutoff>` - Adjust variable indices
- **Substitute**: `subst_t<Term, Index, Replacement>` - Replace variables
- **Reduce**: `reduce_t<Term>` - Single beta reduction step
- **Evaluate**: `eval_t<Term, MaxSteps>` - Reduce to normal form

### Church Encodings

```cpp
// Booleans
True  = λx.λy.x    (Abs<Abs<Var<1>>>)
False = λx.λy.y    (Abs<Abs<Var<0>>>)

// Numerals
Zero = λf.λx.x         (Abs<Abs<Var<0>>>)
One  = λf.λx.f x       (Abs<Abs<App<Var<1>, Var<0>>>>)
Two  = λf.λx.f (f x)   (Abs<Abs<App<Var<1>, App<Var<1>, Var<0>>>>>)

// Pairs
Pair = λx.λy.λf.f x y
Fst  = λp.p True
Snd  = λp.p False

// Combinators
I = λx.x              (Identity)
K = λx.λy.x           (Const)
S = λf.λg.λx.f x (g x)
B = λf.λg.λx.f (g x)  (Composition)
```

## Performance

- **Compilation time**: ~2-3 seconds for full build
- **Test execution**: <0.1 seconds (all tests are compile-time)
- **Runtime overhead**: Zero (everything is compile-time)
- **Binary size**: No impact (template metaprogramming)

## Known Limitations

- **Non-terminating terms**: Cannot evaluate (e.g., Omega combinator)
- **Step limits**: Complex reductions limited by `MaxSteps` parameter
- **Structural equality**: Church numerals may not have exact structural equality after reduction
- **Compilation resources**: Complex terms can use significant stack during compilation

## Contributing

When contributing:

1. Write tests first (see `tests/EXAMPLES.md`)
2. Ensure all tests pass (`./debug.sh`)
3. Add documentation for new features
4. Follow existing code style
5. Update CHANGELOG (if applicable)

## License

[Specify your license here]

## References

- Church, A. (1932). "A set of postulates for the foundation of logic"
- de Bruijn, N. G. (1972). "Lambda calculus notation with nameless dummies"
- Church, A. (1936). "An unsolvable problem of elementary number theory"

## See Also

- `SCRIPTS.md` - Build and test script documentation
- `tests/README.md` - Comprehensive test documentation
- `tests/QUICK_REFERENCE.md` - Quick test reference
- `tests/EXAMPLES.md` - Test writing guide