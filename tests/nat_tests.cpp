#include <type_traits>

import typical.nat;

using namespace typical::nat;

// ============================================================================
// Test Natural Number Classification
// ============================================================================

// Test is_nat
static_assert(is_nat<Z>::value, "Z should be a natural number");
static_assert(is_nat<Zero>::value, "Zero should be a natural number");
static_assert(is_nat<One>::value, "One should be a natural number");
static_assert(is_nat<Two>::value, "Two should be a natural number");
static_assert(is_nat<S<Z>>::value, "S<Z> should be a natural number");
static_assert(is_nat<S<S<Z>>>::value, "S<S<Z>> should be a natural number");
static_assert(!is_nat<int>::value, "int should not be a natural number");
static_assert(!is_nat<void>::value, "void should not be a natural number");

// Test Nat concept
static_assert(Nat<Z>, "Z should satisfy Nat concept");
static_assert(Nat<One>, "One should satisfy Nat concept");
static_assert(Nat<S<S<S<Z>>>>, "S<S<S<Z>>> should satisfy Nat concept");

// Test is_zero
static_assert(is_zero_v<Z>, "Z should be zero");
static_assert(is_zero_v<Zero>, "Zero should be zero");
static_assert(!is_zero_v<One>, "One should not be zero");
static_assert(!is_zero_v<Two>, "Two should not be zero");
static_assert(!is_zero_v<S<Z>>, "S<Z> should not be zero");

// ============================================================================
// Test Predefined Natural Numbers
// ============================================================================

static_assert(std::is_same_v<Zero, Z>, "Zero should be Z");
static_assert(std::is_same_v<One, S<Z>>, "One should be S<Z>");
static_assert(std::is_same_v<Two, S<One>>, "Two should be S<One>");
static_assert(std::is_same_v<Three, S<Two>>, "Three should be S<Two>");
static_assert(std::is_same_v<Four, S<Three>>, "Four should be S<Three>");
static_assert(std::is_same_v<Five, S<Four>>, "Five should be S<Four>");
static_assert(std::is_same_v<Six, S<Five>>, "Six should be S<Five>");
static_assert(std::is_same_v<Seven, S<Six>>, "Seven should be S<Six>");
static_assert(std::is_same_v<Eight, S<Seven>>, "Eight should be S<Seven>");
static_assert(std::is_same_v<Nine, S<Eight>>, "Nine should be S<Eight>");
static_assert(std::is_same_v<Ten, S<Nine>>, "Ten should be S<Nine>");

// ============================================================================
// Test Conversion: Nat <-> size_t
// ============================================================================

// Test ToValue
static_assert(to_value_v<Z> == 0, "Z should convert to 0");
static_assert(to_value_v<Zero> == 0, "Zero should convert to 0");
static_assert(to_value_v<One> == 1, "One should convert to 1");
static_assert(to_value_v<Two> == 2, "Two should convert to 2");
static_assert(to_value_v<Three> == 3, "Three should convert to 3");
static_assert(to_value_v<Four> == 4, "Four should convert to 4");
static_assert(to_value_v<Five> == 5, "Five should convert to 5");
static_assert(to_value_v<Six> == 6, "Six should convert to 6");
static_assert(to_value_v<Seven> == 7, "Seven should convert to 7");
static_assert(to_value_v<Eight> == 8, "Eight should convert to 8");
static_assert(to_value_v<Nine> == 9, "Nine should convert to 9");
static_assert(to_value_v<Ten> == 10, "Ten should convert to 10");

// Test FromValue
static_assert(std::is_same_v<from_value_t<0>, Z>, "0 should convert to Z");
static_assert(std::is_same_v<from_value_t<1>, S<Z>>, "1 should convert to S<Z>");
static_assert(std::is_same_v<from_value_t<2>, S<S<Z>>>, "2 should convert to S<S<Z>>");
static_assert(std::is_same_v<from_value_t<3>, S<S<S<Z>>>>, "3 should convert to S<S<S<Z>>>");

// Test roundtrip conversion
static_assert(to_value_v<from_value_t<0>> == 0, "Roundtrip 0");
static_assert(to_value_v<from_value_t<1>> == 1, "Roundtrip 1");
static_assert(to_value_v<from_value_t<5>> == 5, "Roundtrip 5");
static_assert(to_value_v<from_value_t<10>> == 10, "Roundtrip 10");
static_assert(to_value_v<from_value_t<15>> == 15, "Roundtrip 15");

static_assert(std::is_same_v<from_value_t<to_value_v<Zero>>, Zero>, "Roundtrip Zero");
static_assert(std::is_same_v<from_value_t<to_value_v<Five>>, Five>, "Roundtrip Five");
static_assert(std::is_same_v<from_value_t<to_value_v<Ten>>, Ten>, "Roundtrip Ten");

// ============================================================================
// Test Addition
// ============================================================================

// Addition with zero
static_assert(std::is_same_v<add_t<Z, Z>, Z>, "0 + 0 = 0");
static_assert(std::is_same_v<add_t<Z, One>, One>, "0 + 1 = 1");
static_assert(std::is_same_v<add_t<One, Z>, One>, "1 + 0 = 1");
static_assert(std::is_same_v<add_t<Z, Five>, Five>, "0 + 5 = 5");

// Simple addition
static_assert(std::is_same_v<add_t<One, One>, Two>, "1 + 1 = 2");
static_assert(std::is_same_v<add_t<One, Two>, Three>, "1 + 2 = 3");
static_assert(std::is_same_v<add_t<Two, Two>, Four>, "2 + 2 = 4");
static_assert(std::is_same_v<add_t<Two, Three>, Five>, "2 + 3 = 5");
static_assert(std::is_same_v<add_t<Three, Three>, Six>, "3 + 3 = 6");
static_assert(std::is_same_v<add_t<Three, Four>, Seven>, "3 + 4 = 7");
static_assert(std::is_same_v<add_t<Four, Four>, Eight>, "4 + 4 = 8");
static_assert(std::is_same_v<add_t<Five, Five>, Ten>, "5 + 5 = 10");

// Verify with to_value_v
static_assert(to_value_v<add_t<Three, Two>> == 5, "3 + 2 = 5");
static_assert(to_value_v<add_t<Four, Six>> == 10, "4 + 6 = 10");
static_assert(to_value_v<add_t<Seven, Three>> == 10, "7 + 3 = 10");

// Addition commutativity (spot check)
static_assert(std::is_same_v<add_t<Two, Three>, add_t<Three, Two>>, "2 + 3 = 3 + 2");
static_assert(std::is_same_v<add_t<One, Four>, add_t<Four, One>>, "1 + 4 = 4 + 1");
static_assert(std::is_same_v<add_t<Five, Two>, add_t<Two, Five>>, "5 + 2 = 2 + 5");

// Addition associativity (spot check)
static_assert(std::is_same_v<add_t<add_t<One, Two>, Three>, add_t<One, add_t<Two, Three>>>,
              "(1 + 2) + 3 = 1 + (2 + 3)");
static_assert(std::is_same_v<add_t<add_t<Two, Two>, Two>, add_t<Two, add_t<Two, Two>>>, "(2 + 2) + 2 = 2 + (2 + 2)");

// ============================================================================
// Test Multiplication
// ============================================================================

// Multiplication by zero
static_assert(std::is_same_v<mul_t<Z, Z>, Z>, "0 * 0 = 0");
static_assert(std::is_same_v<mul_t<Z, One>, Z>, "0 * 1 = 0");
static_assert(std::is_same_v<mul_t<Z, Five>, Z>, "0 * 5 = 0");
static_assert(std::is_same_v<mul_t<One, Z>, Z>, "1 * 0 = 0");
static_assert(std::is_same_v<mul_t<Five, Z>, Z>, "5 * 0 = 0");

// Multiplication by one
static_assert(std::is_same_v<mul_t<One, One>, One>, "1 * 1 = 1");
static_assert(std::is_same_v<mul_t<One, Two>, Two>, "1 * 2 = 2");
static_assert(std::is_same_v<mul_t<One, Five>, Five>, "1 * 5 = 5");
static_assert(std::is_same_v<mul_t<Two, One>, Two>, "2 * 1 = 2");
static_assert(std::is_same_v<mul_t<Five, One>, Five>, "5 * 1 = 5");

// Simple multiplication
static_assert(std::is_same_v<mul_t<Two, Two>, Four>, "2 * 2 = 4");
static_assert(std::is_same_v<mul_t<Two, Three>, Six>, "2 * 3 = 6");
static_assert(std::is_same_v<mul_t<Two, Four>, Eight>, "2 * 4 = 8");
static_assert(std::is_same_v<mul_t<Three, Two>, Six>, "3 * 2 = 6");
static_assert(std::is_same_v<mul_t<Three, Three>, Nine>, "3 * 3 = 9");
static_assert(std::is_same_v<mul_t<Two, Five>, Ten>, "2 * 5 = 10");
static_assert(std::is_same_v<mul_t<Five, Two>, Ten>, "5 * 2 = 10");

// Verify with to_value_v
static_assert(to_value_v<mul_t<Three, Four>> == 12, "3 * 4 = 12");
static_assert(to_value_v<mul_t<Four, Three>> == 12, "4 * 3 = 12");
static_assert(to_value_v<mul_t<Four, Four>> == 16, "4 * 4 = 16");

// Multiplication commutativity (spot check)
static_assert(std::is_same_v<mul_t<Two, Three>, mul_t<Three, Two>>, "2 * 3 = 3 * 2");
static_assert(std::is_same_v<mul_t<Two, Four>, mul_t<Four, Two>>, "2 * 4 = 4 * 2");

// ============================================================================
// Test Subtraction
// ============================================================================

// Subtraction with zero
static_assert(std::is_same_v<sub_t<Z, Z>, Z>, "0 - 0 = 0");
static_assert(std::is_same_v<sub_t<One, Z>, One>, "1 - 0 = 1");
static_assert(std::is_same_v<sub_t<Five, Z>, Five>, "5 - 0 = 5");
static_assert(std::is_same_v<sub_t<Z, One>, Z>, "0 - 1 = 0 (saturating)");
static_assert(std::is_same_v<sub_t<Z, Five>, Z>, "0 - 5 = 0 (saturating)");

// Simple subtraction
static_assert(std::is_same_v<sub_t<One, One>, Z>, "1 - 1 = 0");
static_assert(std::is_same_v<sub_t<Two, One>, One>, "2 - 1 = 1");
static_assert(std::is_same_v<sub_t<Three, One>, Two>, "3 - 1 = 2");
static_assert(std::is_same_v<sub_t<Five, Two>, Three>, "5 - 2 = 3");
static_assert(std::is_same_v<sub_t<Ten, Five>, Five>, "10 - 5 = 5");
static_assert(std::is_same_v<sub_t<Ten, Three>, Seven>, "10 - 3 = 7");

// Saturating subtraction (result would be negative)
static_assert(std::is_same_v<sub_t<One, Two>, Z>, "1 - 2 = 0 (saturating)");
static_assert(std::is_same_v<sub_t<Two, Five>, Z>, "2 - 5 = 0 (saturating)");
static_assert(std::is_same_v<sub_t<Three, Ten>, Z>, "3 - 10 = 0 (saturating)");

// Verify with to_value_v
static_assert(to_value_v<sub_t<Eight, Three>> == 5, "8 - 3 = 5");
static_assert(to_value_v<sub_t<Nine, Four>> == 5, "9 - 4 = 5");
static_assert(to_value_v<sub_t<Ten, Ten>> == 0, "10 - 10 = 0");

// ============================================================================
// Test Comparison Operators
// ============================================================================

// LessThan
static_assert(less_than_v<Z, One>, "0 < 1");
static_assert(less_than_v<Z, Five>, "0 < 5");
static_assert(less_than_v<One, Two>, "1 < 2");
static_assert(less_than_v<Two, Three>, "2 < 3");
static_assert(less_than_v<Five, Ten>, "5 < 10");
static_assert(!less_than_v<Z, Z>, "!(0 < 0)");
static_assert(!less_than_v<One, One>, "!(1 < 1)");
static_assert(!less_than_v<One, Z>, "!(1 < 0)");
static_assert(!less_than_v<Five, Two>, "!(5 < 2)");
static_assert(!less_than_v<Ten, Five>, "!(10 < 5)");

// LessOrEqual
static_assert(less_or_equal_v<Z, Z>, "0 <= 0");
static_assert(less_or_equal_v<One, One>, "1 <= 1");
static_assert(less_or_equal_v<Five, Five>, "5 <= 5");
static_assert(less_or_equal_v<Z, One>, "0 <= 1");
static_assert(less_or_equal_v<One, Two>, "1 <= 2");
static_assert(less_or_equal_v<Five, Ten>, "5 <= 10");
static_assert(!less_or_equal_v<One, Z>, "!(1 <= 0)");
static_assert(!less_or_equal_v<Five, Two>, "!(5 <= 2)");
static_assert(!less_or_equal_v<Ten, Five>, "!(10 <= 5)");

// GreaterThan
static_assert(greater_than_v<One, Z>, "1 > 0");
static_assert(greater_than_v<Two, One>, "2 > 1");
static_assert(greater_than_v<Ten, Five>, "10 > 5");
static_assert(!greater_than_v<Z, Z>, "!(0 > 0)");
static_assert(!greater_than_v<One, One>, "!(1 > 1)");
static_assert(!greater_than_v<Z, One>, "!(0 > 1)");
static_assert(!greater_than_v<Two, Five>, "!(2 > 5)");

// Equality
static_assert(nat_equal_v<Z, Z>, "0 == 0");
static_assert(nat_equal_v<One, One>, "1 == 1");
static_assert(nat_equal_v<Five, Five>, "5 == 5");
static_assert(!nat_equal_v<Z, One>, "!(0 == 1)");
static_assert(!nat_equal_v<One, Two>, "!(1 == 2)");
static_assert(!nat_equal_v<Five, Three>, "!(5 == 3)");

// ============================================================================
// Test Min and Max
// ============================================================================

// Min
static_assert(std::is_same_v<min_t<Z, Z>, Z>, "min(0, 0) = 0");
static_assert(std::is_same_v<min_t<Z, One>, Z>, "min(0, 1) = 0");
static_assert(std::is_same_v<min_t<One, Z>, Z>, "min(1, 0) = 0");
static_assert(std::is_same_v<min_t<One, One>, One>, "min(1, 1) = 1");
static_assert(std::is_same_v<min_t<Two, Five>, Two>, "min(2, 5) = 2");
static_assert(std::is_same_v<min_t<Five, Two>, Two>, "min(5, 2) = 2");
static_assert(std::is_same_v<min_t<Three, Seven>, Three>, "min(3, 7) = 3");
static_assert(std::is_same_v<min_t<Ten, Five>, Five>, "min(10, 5) = 5");

// Max
static_assert(std::is_same_v<max_t<Z, Z>, Z>, "max(0, 0) = 0");
static_assert(std::is_same_v<max_t<Z, One>, One>, "max(0, 1) = 1");
static_assert(std::is_same_v<max_t<One, Z>, One>, "max(1, 0) = 1");
static_assert(std::is_same_v<max_t<One, One>, One>, "max(1, 1) = 1");
static_assert(std::is_same_v<max_t<Two, Five>, Five>, "max(2, 5) = 5");
static_assert(std::is_same_v<max_t<Five, Two>, Five>, "max(5, 2) = 5");
static_assert(std::is_same_v<max_t<Three, Seven>, Seven>, "max(3, 7) = 7");
static_assert(std::is_same_v<max_t<Ten, Five>, Ten>, "max(10, 5) = 10");

// Min/Max symmetry
static_assert(std::is_same_v<min_t<Two, Five>, min_t<Five, Two>>, "min is symmetric");
static_assert(std::is_same_v<max_t<Two, Five>, max_t<Five, Two>>, "max is symmetric");

// ============================================================================
// Test Division and Modulus
// ============================================================================

// Division by zero (returns 0 by definition)
static_assert(std::is_same_v<div_t<Z, Z>, Z>, "0 / 0 = 0 (by definition)");
static_assert(std::is_same_v<div_t<Five, Z>, Z>, "5 / 0 = 0 (by definition)");

// Division with zero dividend
static_assert(std::is_same_v<div_t<Z, One>, Z>, "0 / 1 = 0");
static_assert(std::is_same_v<div_t<Z, Five>, Z>, "0 / 5 = 0");

// Simple division
static_assert(std::is_same_v<div_t<One, One>, One>, "1 / 1 = 1");
static_assert(std::is_same_v<div_t<Two, One>, Two>, "2 / 1 = 2");
static_assert(std::is_same_v<div_t<Five, One>, Five>, "5 / 1 = 5");
static_assert(std::is_same_v<div_t<Two, Two>, One>, "2 / 2 = 1");
static_assert(std::is_same_v<div_t<Four, Two>, Two>, "4 / 2 = 2");
static_assert(std::is_same_v<div_t<Six, Two>, Three>, "6 / 2 = 3");
static_assert(std::is_same_v<div_t<Six, Three>, Two>, "6 / 3 = 2");
static_assert(std::is_same_v<div_t<Eight, Two>, Four>, "8 / 2 = 4");
static_assert(std::is_same_v<div_t<Nine, Three>, Three>, "9 / 3 = 3");
static_assert(std::is_same_v<div_t<Ten, Two>, Five>, "10 / 2 = 5");
static_assert(std::is_same_v<div_t<Ten, Five>, Two>, "10 / 5 = 2");

// Division with remainder (truncates)
static_assert(std::is_same_v<div_t<Three, Two>, One>, "3 / 2 = 1");
static_assert(std::is_same_v<div_t<Five, Two>, Two>, "5 / 2 = 2");
static_assert(std::is_same_v<div_t<Seven, Three>, Two>, "7 / 3 = 2");
static_assert(std::is_same_v<div_t<Ten, Three>, Three>, "10 / 3 = 3");

// Division where dividend < divisor
static_assert(std::is_same_v<div_t<One, Two>, Z>, "1 / 2 = 0");
static_assert(std::is_same_v<div_t<Two, Three>, Z>, "2 / 3 = 0");
static_assert(std::is_same_v<div_t<Five, Ten>, Z>, "5 / 10 = 0");

// Modulus by zero (returns 0 by definition)
static_assert(std::is_same_v<mod_t<Z, Z>, Z>, "0 % 0 = 0 (by definition)");
static_assert(std::is_same_v<mod_t<Five, Z>, Z>, "5 % 0 = 0 (by definition)");

// Modulus with zero dividend
static_assert(std::is_same_v<mod_t<Z, One>, Z>, "0 % 1 = 0");
static_assert(std::is_same_v<mod_t<Z, Five>, Z>, "0 % 5 = 0");

// Simple modulus
static_assert(std::is_same_v<mod_t<One, One>, Z>, "1 % 1 = 0");
static_assert(std::is_same_v<mod_t<Two, Two>, Z>, "2 % 2 = 0");
static_assert(std::is_same_v<mod_t<Five, Five>, Z>, "5 % 5 = 0");
static_assert(std::is_same_v<mod_t<Three, Two>, One>, "3 % 2 = 1");
static_assert(std::is_same_v<mod_t<Five, Two>, One>, "5 % 2 = 1");
static_assert(std::is_same_v<mod_t<Seven, Two>, One>, "7 % 2 = 1");
static_assert(std::is_same_v<mod_t<Four, Three>, One>, "4 % 3 = 1");
static_assert(std::is_same_v<mod_t<Five, Three>, Two>, "5 % 3 = 2");
static_assert(std::is_same_v<mod_t<Seven, Three>, One>, "7 % 3 = 1");
static_assert(std::is_same_v<mod_t<Eight, Three>, Two>, "8 % 3 = 2");
static_assert(std::is_same_v<mod_t<Ten, Three>, One>, "10 % 3 = 1");
static_assert(std::is_same_v<mod_t<Ten, Four>, Two>, "10 % 4 = 2");

// Modulus where dividend < divisor (returns dividend)
static_assert(std::is_same_v<mod_t<One, Two>, One>, "1 % 2 = 1");
static_assert(std::is_same_v<mod_t<Two, Three>, Two>, "2 % 3 = 2");
static_assert(std::is_same_v<mod_t<Five, Ten>, Five>, "5 % 10 = 5");

// Verify division and modulus relationship: a = (a/b)*b + (a%b)
static_assert(std::is_same_v<add_t<mul_t<div_t<Seven, Three>, Three>, mod_t<Seven, Three>>, Seven>,
              "7 = (7/3)*3 + (7%3)");
static_assert(std::is_same_v<add_t<mul_t<div_t<Ten, Three>, Three>, mod_t<Ten, Three>>, Ten>, "10 = (10/3)*3 + (10%3)");

// ============================================================================
// Test Power
// ============================================================================

// Power with zero exponent
static_assert(std::is_same_v<pow_t<Z, Z>, One>, "0^0 = 1 (by definition)");
static_assert(std::is_same_v<pow_t<One, Z>, One>, "1^0 = 1");
static_assert(std::is_same_v<pow_t<Two, Z>, One>, "2^0 = 1");
static_assert(std::is_same_v<pow_t<Five, Z>, One>, "5^0 = 1");

// Power with one
static_assert(std::is_same_v<pow_t<Z, One>, Z>, "0^1 = 0");
static_assert(std::is_same_v<pow_t<One, One>, One>, "1^1 = 1");
static_assert(std::is_same_v<pow_t<Two, One>, Two>, "2^1 = 2");
static_assert(std::is_same_v<pow_t<Five, One>, Five>, "5^1 = 5");

// Powers of two
static_assert(std::is_same_v<pow_t<Two, Two>, Four>, "2^2 = 4");
static_assert(std::is_same_v<pow_t<Two, Three>, Eight>, "2^3 = 8");
static_assert(to_value_v<pow_t<Two, Four>> == 16, "2^4 = 16");

// Other powers
static_assert(std::is_same_v<pow_t<Three, Two>, Nine>, "3^2 = 9");
static_assert(to_value_v<pow_t<Three, Three>> == 27, "3^3 = 27");
static_assert(std::is_same_v<pow_t<Four, Two>, from_value_t<16>>, "4^2 = 16");
static_assert(std::is_same_v<pow_t<Five, Two>, from_value_t<25>>, "5^2 = 25");

// ============================================================================
// Test Factorial
// ============================================================================

static_assert(std::is_same_v<factorial_t<Z>, One>, "0! = 1");
static_assert(std::is_same_v<factorial_t<One>, One>, "1! = 1");
static_assert(std::is_same_v<factorial_t<Two>, Two>, "2! = 2");
static_assert(std::is_same_v<factorial_t<Three>, Six>, "3! = 6");
static_assert(to_value_v<factorial_t<Four>> == 24, "4! = 24");
static_assert(to_value_v<factorial_t<Five>> == 120, "5! = 120");
static_assert(to_value_v<factorial_t<Six>> == 720, "6! = 720");

// ============================================================================
// Test GCD (Greatest Common Divisor)
// ============================================================================

// GCD with zero
static_assert(std::is_same_v<gcd_t<Z, Z>, Z>, "gcd(0, 0) = 0");
static_assert(std::is_same_v<gcd_t<Five, Z>, Five>, "gcd(5, 0) = 5");
static_assert(std::is_same_v<gcd_t<Z, Five>, Five>, "gcd(0, 5) = 5");

// GCD with one
static_assert(std::is_same_v<gcd_t<One, One>, One>, "gcd(1, 1) = 1");
static_assert(std::is_same_v<gcd_t<Five, One>, One>, "gcd(5, 1) = 1");
static_assert(std::is_same_v<gcd_t<One, Five>, One>, "gcd(1, 5) = 1");

// GCD with same numbers
static_assert(std::is_same_v<gcd_t<Two, Two>, Two>, "gcd(2, 2) = 2");
static_assert(std::is_same_v<gcd_t<Five, Five>, Five>, "gcd(5, 5) = 5");
static_assert(std::is_same_v<gcd_t<Ten, Ten>, Ten>, "gcd(10, 10) = 10");

// GCD of coprime numbers
static_assert(std::is_same_v<gcd_t<Three, Five>, One>, "gcd(3, 5) = 1");
static_assert(std::is_same_v<gcd_t<Five, Three>, One>, "gcd(5, 3) = 1");
static_assert(std::is_same_v<gcd_t<Seven, Nine>, One>, "gcd(7, 9) = 1");

// GCD of non-coprime numbers
static_assert(std::is_same_v<gcd_t<Two, Four>, Two>, "gcd(2, 4) = 2");
static_assert(std::is_same_v<gcd_t<Four, Two>, Two>, "gcd(4, 2) = 2");
static_assert(std::is_same_v<gcd_t<Four, Six>, Two>, "gcd(4, 6) = 2");
static_assert(std::is_same_v<gcd_t<Six, Nine>, Three>, "gcd(6, 9) = 3");
static_assert(std::is_same_v<gcd_t<Eight, from_value_t<12>>, Four>, "gcd(8, 12) = 4");

// GCD is commutative
static_assert(std::is_same_v<gcd_t<Six, Nine>, gcd_t<Nine, Six>>, "gcd is commutative");
static_assert(std::is_same_v<gcd_t<Four, Ten>, gcd_t<Ten, Four>>, "gcd is commutative");

// ============================================================================
// Test Fibonacci
// ============================================================================

static_assert(std::is_same_v<fibonacci_t<Z>, Z>, "fib(0) = 0");
static_assert(std::is_same_v<fibonacci_t<One>, One>, "fib(1) = 1");
static_assert(std::is_same_v<fibonacci_t<Two>, One>, "fib(2) = 1");
static_assert(std::is_same_v<fibonacci_t<Three>, Two>, "fib(3) = 2");
static_assert(std::is_same_v<fibonacci_t<Four>, Three>, "fib(4) = 3");
static_assert(std::is_same_v<fibonacci_t<Five>, Five>, "fib(5) = 5");
static_assert(std::is_same_v<fibonacci_t<Six>, Eight>, "fib(6) = 8");
static_assert(to_value_v<fibonacci_t<Seven>> == 13, "fib(7) = 13");
static_assert(to_value_v<fibonacci_t<Eight>> == 21, "fib(8) = 21");

// ============================================================================
// Test Even/Odd Predicates
// ============================================================================

// Even numbers
static_assert(is_even_v<Z>, "0 is even");
static_assert(is_even_v<Two>, "2 is even");
static_assert(is_even_v<Four>, "4 is even");
static_assert(is_even_v<Six>, "6 is even");
static_assert(is_even_v<Eight>, "8 is even");
static_assert(is_even_v<Ten>, "10 is even");

// Odd numbers
static_assert(!is_even_v<One>, "1 is not even");
static_assert(!is_even_v<Three>, "3 is not even");
static_assert(!is_even_v<Five>, "5 is not even");
static_assert(!is_even_v<Seven>, "7 is not even");
static_assert(!is_even_v<Nine>, "9 is not even");

// Odd via is_odd_v
static_assert(!is_odd_v<Z>, "0 is not odd");
static_assert(!is_odd_v<Two>, "2 is not odd");
static_assert(!is_odd_v<Four>, "4 is not odd");
static_assert(is_odd_v<One>, "1 is odd");
static_assert(is_odd_v<Three>, "3 is odd");
static_assert(is_odd_v<Five>, "5 is odd");
static_assert(is_odd_v<Seven>, "7 is odd");

// Even/Odd are complementary
static_assert(is_even_v<Two> != is_odd_v<Two>, "even and odd are complementary");
static_assert(is_even_v<Three> != is_odd_v<Three>, "even and odd are complementary");

// ============================================================================
// Test Mathematical Properties and Laws
// ============================================================================

// Additive identity
static_assert(std::is_same_v<add_t<Z, Three>, Three>, "Additive identity (left)");
static_assert(std::is_same_v<add_t<Five, Z>, Five>, "Additive identity (right)");

// Multiplicative identity
static_assert(std::is_same_v<mul_t<One, Four>, Four>, "Multiplicative identity (left)");
static_assert(std::is_same_v<mul_t<Six, One>, Six>, "Multiplicative identity (right)");

// Multiplicative zero
static_assert(std::is_same_v<mul_t<Z, Seven>, Z>, "Multiplicative zero (left)");
static_assert(std::is_same_v<mul_t<Eight, Z>, Z>, "Multiplicative zero (right)");

// Distributive law: a * (b + c) = a * b + a * c
static_assert(std::is_same_v<mul_t<Two, add_t<Three, Four>>, add_t<mul_t<Two, Three>, mul_t<Two, Four>>>,
              "Distributive law");
static_assert(std::is_same_v<mul_t<Three, add_t<Two, One>>, add_t<mul_t<Three, Two>, mul_t<Three, One>>>,
              "Distributive law");

// Power laws
static_assert(std::is_same_v<pow_t<Two, add_t<One, One>>, mul_t<pow_t<Two, One>, pow_t<Two, One>>>,
              "a^(b+c) = a^b * a^c (for b=c=1)");

// Subtraction and addition inverse (when result is non-negative)
static_assert(std::is_same_v<sub_t<add_t<Three, Two>, Two>, Three>, "add/sub inverse");
static_assert(std::is_same_v<add_t<sub_t<Five, Two>, Two>, Five>, "sub/add inverse");

// ============================================================================
// Test Complex Expressions
// ============================================================================

// (2 + 3) * 4 = 20
static_assert(to_value_v<mul_t<add_t<Two, Three>, Four>> == 20, "Complex expression 1");

// 10 / 2 + 3 = 8
static_assert(to_value_v<add_t<div_t<Ten, Two>, Three>> == 8, "Complex expression 2");

// gcd(12, 8) = 4
static_assert(to_value_v<gcd_t<from_value_t<12>, Eight>> == 4, "Complex expression 3");

// factorial(3) + factorial(2) = 8
static_assert(to_value_v<add_t<factorial_t<Three>, factorial_t<Two>>> == 8, "Complex expression 4");

// 2^3 + 3^2 = 17
static_assert(to_value_v<add_t<pow_t<Two, Three>, pow_t<Three, Two>>> == 17, "Complex expression 5");

int main() { return 0; }
