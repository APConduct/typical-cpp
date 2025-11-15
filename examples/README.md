# Examples

This directory contains example programs demonstrating the `typical` library's capabilities.

## Example 01: Comprehensive Demonstration

**Location**: `01/main.cpp`

A comprehensive demonstration showcasing all major features of the typical library including:

### Lambda Calculus
- Identity function (`Id`)
- Const combinator
- Function composition (`B` combinator)
- Beta reduction and evaluation

### Church Encodings
- **Booleans**: `True` and `False` with conditional behavior
- **Numerals**: Church numerals representing numbers via function iteration
- **Pairs**: Church-encoded product types with `Fst` and `Snd` projections
- **Lists**: Church-encoded lists with `Cons`, `Nil`, `Head`, and `Tail`

### Peano Natural Numbers (typical::nat)
- Natural number type system using Zero and Successor
- Conversion between Peano numbers and runtime values
- **Arithmetic**: Addition, multiplication, subtraction (saturating), division, modulus
- **Comparison**: `<`, `<=`, `>`, `==`
- **Min/Max**: Finding minimum and maximum values
- **Mathematical functions**: 
  - Power (exponentiation)
  - Factorial
  - GCD (Greatest Common Divisor)
  - Fibonacci sequence
- **Predicates**: Even/odd testing
- **Complex expressions**: Combining multiple operations

## Building and Running

### Build the Example

```bash
cd typical
./debug.sh
```

Or manually:

```bash
cd typical
mkdir -p build && cd build
cmake -G Ninja ..
ninja example_01
```

### Run the Example

```bash
./cmake-build-debug/examples/01/example_01
```

### Expected Output

```
==================================================
  Typical - Type-Level Lambda Calculus Demo
==================================================

All computations happen at compile-time!
This program demonstrates static assertions.

Lambda Calculus:
  Identity, Const, and combinators ✓

Church Encodings:
  Booleans (True/False) ✓
  Numerals (Zero, One, Two, ...) ✓
  Pairs (Fst/Snd) ✓
  Lists (Cons/Nil/Head/Tail) ✓

Peano Natural Numbers:
  Zero = 0
  Five = 5
  Ten = 10

Arithmetic:
  2 + 3 = 5
  3 * 4 = 12
  10 / 2 = 5
  10 % 3 = 1
  2^3 = 8

Mathematical Functions:
  3! = 6
  4! = 24
  gcd(6, 9) = 3
  fib(5) = 5
  fib(7) = 13

Predicates:
  is_even(4) = true
  is_odd(3) = true
  2 < 5 = true

Complex Expressions:
  (2 + 3) * 4 = 20
  10 / 2 + 3 = 8
  2^3 + 3^2 = 17
  gcd(12, 8) = 4

==================================================
  All 300+ compile-time tests passed!
  Zero runtime overhead. Zero runtime cost.
==================================================
```

## Key Concepts Demonstrated

### 1. Compile-Time Computation

All computations happen at compile-time using template metaprogramming:

```cpp
// This computation happens during compilation
static_assert(to_value_v<add_t<Two, Three>> == 5, "2 + 3 = 5");
```

### 2. Zero Runtime Cost

Since everything is computed at compile-time, there is literally zero runtime overhead. The `to_value_v` calls in the example simply return compile-time constants.

### 3. Type-Level Programming

Numbers, booleans, and data structures are all represented as C++ types:

```cpp
using Five = S<S<S<S<S<Z>>>>>;  // Peano: 5 = Successor^5(Zero)
using Result = add_t<Two, Three>;  // Result is the type representing 5
constexpr size_t value = to_value_v<Result>;  // Convert to runtime value: 5
```

### 4. Church vs Peano Encodings

The library provides two different encoding schemes:

**Church Encodings** (from `typical::lambda`):
- Represent numbers by how many times they apply a function
- `Zero = λf.λx.x` (apply f zero times)
- `One = λf.λx.f(x)` (apply f once)

**Peano Numbers** (from `typical::nat`):
- Represent numbers using Zero and Successor
- `Zero = Z`
- `One = S<Z>` (successor of zero)
- `Two = S<S<Z>>` (successor of successor of zero)

### 5. Proper Namespacing

To avoid conflicts, use proper namespacing:

```cpp
namespace nat = typical::nat;  // Peano numbers
using nat::Zero;
using nat::One;
// ... use Peano numbers
```

## Notes

- All assertions in the example are `static_assert`, meaning they're checked at compile-time
- If any assertion fails, the code won't compile
- The `main()` function only prints pre-computed results
- Compilation warnings about stack exhaustion are normal for complex lambda calculus reductions

## Creating Your Own Examples

To create a new example:

1. Create a new directory: `examples/02/`
2. Add a `main.cpp` file
3. Create `examples/02/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.28)

add_executable(example_02 main.cpp)
target_link_libraries(example_02 PRIVATE typical)
set_target_properties(example_02 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
```

4. Add to `examples/CMakeLists.txt`:

```cmake
add_subdirectory(02)
```

5. Build and run:

```bash
./debug.sh
./cmake-build-debug/examples/02/example_02
```

## See Also

- `../README.md` - Main project documentation
- `../tests/README.md` - Test suite documentation
- `../tests/EXAMPLES.md` - How to write tests
- `../SCRIPTS.md` - Build script documentation