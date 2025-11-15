module;
#include <cstddef>
#include <type_traits>

export module typical.nat;

import typical.eq;

export namespace typical::nat {

/// Peano number representation
struct Z {};

/// Successor
template <typename N>
struct S {};

// Convenient aliases for small natural numbers

using Zero = Z;
using One = S<Z>;
using Two = S<One>;
using Three = S<Two>;
using Four = S<Three>;
using Five = S<Four>;
using Six = S<Five>;
using Seven = S<Six>;
using Eight = S<Seven>;
using Nine = S<Eight>;
using Ten = S<Nine>;

// Natural number classification

/// Type trait to identify Peano natural numbers
template <typename T>
struct is_nat : std::false_type {};

/// Specialization for Z
template <>
struct is_nat<Z> : std::true_type {};

/// Specialization for S<N>
template <typename N>
struct is_nat<S<N>> : is_nat<N> {};

/// Concept to identify Peano natural numbers
template <typename T>
concept Nat = is_nat<T>::value;

/// is_zero trait
template <typename N>
struct is_zero : std::false_type {};

/// Specialization for Z
template <>
struct is_zero<Z> : std::true_type {};

/// Alias for is_zeros
template <typename N>
inline constexpr bool is_zero_v = is_zero<N>::value;

// Conversion
// ----------------

/// Convert Peano number = size_t
template <Nat N>
struct ToValue;

/// Specialization for Z
template <>
struct ToValue<Z> {
    static constexpr size_t value = 0;
};

/// Recursive case for S<N>
template <Nat N>
struct ToValue<S<N>> {
    static constexpr size_t value = 1 + ToValue<N>::value;
};

/// Alias for ToValue
template <Nat N>
inline constexpr size_t to_value_v = ToValue<N>::value;

/// Convert size_t to Peano number
template <size_t N>
struct FromValue {
    using type = S<typename FromValue<N - 1>::type>;
};

/// Specialization for 0
template <>
struct FromValue<0> {
    using type = Z;
};

/// Alias for FromValue
template <size_t N>
using from_value_t = typename FromValue<N>::type;

// Addition
// ----------------

/// Addition
template <Nat M, Nat N>
struct Add;

/// Base case
template <Nat N>
struct Add<Z, N> {
    using Result = N;
};

/// Recursive case
template <Nat M, Nat N>
struct Add<S<M>, N> {
    using Result = S<typename Add<M, N>::Result>;
};

/// Alias for Add
template <Nat M, Nat N>
using add_t = typename Add<M, N>::Result;

// Addition proofs

/// Left identity
template <Nat N>
struct AddLeftIdentity {
    using Left = add_t<Z, N>;
    using Right = N;
    using Proof = typical::Eq<Left, Right>;
};

/// Right identity
template <Nat N>
struct AddRightIdentity;

/// Specialization for Z
template <>
struct AddRightIdentity<Z> {
    using Left = add_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

/// Recursive case for S<N>
template <Nat N>
struct AddRightIdentity<S<N>> {
    using IH = typename AddRightIdentity<N>::Proof;
    using Left = add_t<S<N>, Z>;
    using Right = S<N>;
    using Proof = typical::Eq<Left, Right>;
};

/// Commutativity
template <Nat M, Nat N>
struct AddCommutative {
    using Left = add_t<M, N>;
    using Right = add_t<N, M>;

    // TODO: enhance complexity of proof
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

// Multiplication

/// Multiplication
template <Nat M, Nat N>
struct Mul;

/// Base case
template <Nat N>
struct Mul<Z, N> {
    using Result = Z;
};

/// Recursive case
template <Nat M, Nat N>
struct Mul<S<M>, N> {
    using Result = add_t<N, typename Mul<M, N>::Result>;
};

/// Alias for Mul
template <Nat M, Nat N>
using mul_t = typename Mul<M, N>::Result;

// Multiplication proofs

/// Left zero
template <Nat N>
struct MulLeftZero {
    using Left = mul_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

/// Right zero
template <Nat N>
struct MulRightZero;

/// Specialization for Z
template <>
struct MulRightZero<Z> {
    using Left = mul_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

/// Recursive case for S<N>
template <Nat N>
struct MulRightZero<S<N>> {
    using Left = mul_t<S<N>, Z>;
    using Right = Z;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

/// Left identity
template <Nat N>
struct MulLeftIdentity {
    using Left = mul_t<One, N>;
    using Right = N;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

/// Right identity
template <Nat M, Nat N>
struct MulCommutative {
    using Lfet = mul_t<M, N>;
    using Right = mul_t<N, M>;
    using Proof = std::conditional_t<std::is_same_v<Lfet, Right>, Eq<Lfet, Right>, void>;
};

/// Distributive property (left)
template <Nat M, Nat N, Nat P>
struct MulDistributiveLeft {
    using Left = mul_t<M, add_t<N, P>>;
    using Right = add_t<mul_t<M, N>, mul_t<M, P>>;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

// Subtraction (partial - results Z if result would be negative)
// ----------------

/// Subtraction
template <Nat M, Nat N>
struct Sub;

/// Base case
template <Nat M>
struct Sub<M, Z> {
    using Result = M;
};

/// Recursive case
template <Nat N>
struct Sub<Z, S<N>> {
    using Result = Z;
};

/// Recursive case
template <Nat M, Nat N>
struct Sub<S<M>, S<N>> {
    using Result = typename Sub<M, N>::Result;
};

/// Alias for Sub
template <Nat M, Nat N>
using sub_t = typename Sub<M, N>::Result;

// Comparison
// ----------------

/// Less than
template <Nat M, Nat N>
struct LessThan;

/// Base case
template <Nat N>
struct LessThan<Z, S<N>> {
    static constexpr bool value = true;
    using Proof = void;
};

/// Base case
template <Nat M>
struct LessThan<S<M>, Z> {
    static constexpr bool value = false;
    using Proof = void;
};

/// Base case
template <>
struct LessThan<Z, Z> {
    static constexpr bool value = false;
    using Proof = void;
};

/// Recursive case
template <Nat M, Nat N>
struct LessThan<S<M>, S<N>> {
    static constexpr bool value = LessThan<M, N>::value;
    using Proof = typename LessThan<M, N>::Proof;
};

/// Alias for LessThan
template <Nat M, Nat N>
inline constexpr bool less_than_v = LessThan<M, N>::value;

/// Less or equal
template <Nat M, Nat N>
struct LessOrEqual {
    static constexpr bool value = std::is_same_v<M, N> || less_than_v<M, N>;
};

/// Alias for LessOrEqual
template <Nat M, Nat N>
inline constexpr bool less_or_equal_v = LessOrEqual<M, N>::value;

/// Greater than
template <Nat M, Nat N>
inline constexpr bool greater_than_v = less_than_v<N, M>;

/// Equality
template <Nat M, Nat N>
inline constexpr bool nat_equal_v = std::is_same_v<M, N>;

// Min and Max
// ----------------

/// Minimum
template <Nat M, Nat N>
struct Min {
    /// Result
    using Result = std::conditional_t<less_or_equal_v<M, N>, M, N>;
};

/// Alias for Min
template <Nat M, Nat N>
using min_t = typename Min<M, N>::Result;

/// Maximum
template <Nat M, Nat N>
struct Max {
    using Result = std::conditional_t<greater_than_v<M, N>, M, N>;
};

/// Alias for Max
template <Nat M, Nat N>
using max_t = typename Max<M, N>::Result;

// Division and Modulus
// ----------------

/// Division
template <Nat M, Nat N>
struct Div;

/// Division by zero yields zero
template <Nat M>
struct Div<M, Z> {
    using Result = Z;
};

/// Zero divided by any non-zero number is zero
template <Nat N>
struct Div<Z, S<N>> {
    using Result = Z;
};

/// Recursive case
template <Nat M, Nat N>
    requires(!is_zero_v<N> && !is_zero_v<M>)
struct Div<M, N> {
protected:
    using Difference = sub_t<M, N>;
    static constexpr bool m_ge_n = less_or_equal_v<N, M>;

public:
    /// Result
    using Result = std::conditional_t<m_ge_n, S<typename Div<Difference, N>::Result>, Z>;
};

/// Alias for Div
template <Nat M, Nat N>
using div_t = typename Div<M, N>::Result;

/// Modulus
template <Nat M, Nat N>
struct Mod;

/// Modulus by zero yields zero
template <Nat M>
struct Mod<M, Z> {
    using Result = Z;
};

/// Zero modulus any non-zero number is zero
template <Nat N>
struct Mod<Z, S<N>> {
    /// Result
    using Result = Z;
};

/// Recursive case
template <Nat M, Nat N>
    requires(!is_zero_v<N> && !is_zero_v<M>)
struct Mod<M, N> {
protected:
    static constexpr bool m_lt_n = less_than_v<M, N>;
    using Difference = sub_t<M, N>;

public:
    /// Result
    using Result = std::conditional_t<m_lt_n, M, typename Mod<Difference, N>::Result>;
};

/// Alias for Mod
template <Nat M, Nat N>
using mod_t = typename Mod<M, N>::Result;

// Exponentiation
// ----------------

/// Exponentiation
template <Nat Base, Nat Exp>
struct Pow;

/// Base case
template <Nat Base>
struct Pow<Base, Z> {
    using Result = One;
};

/// Recursive case
template <Nat Base, Nat Exp>
struct Pow<Base, S<Exp>> {
    using Result = mul_t<Base, typename Pow<Base, Exp>::Result>;
};

/// Alias for Pow
template <Nat Base, Nat Exp>
using pow_t = typename Pow<Base, Exp>::Result;

/// Factorial
template <Nat N>
struct Factorial;

/// Base case
template <>
struct Factorial<Z> {
    using Result = One;
};

/// Recursive case
template <Nat N>
struct Factorial<S<N>> {
    using Result = mul_t<S<N>, typename Factorial<N>::Result>;
};

/// Alias for Factorial
template <Nat N>
using factorial_t = typename Factorial<N>::Result;

/// GCD (Greatest Common Divisor)
template <Nat M, Nat N>
struct GCD;

/// Base case
template <Nat M>
struct GCD<M, Z> {
    using Result = M;
};

/// Recursive case
template <Nat M, Nat N>
    requires(!is_zero_v<N>)
struct GCD<M, N> {
    /// Result
    using Result = typename GCD<N, mod_t<M, N>>::Result;
};

/// Alias for GCD
template <Nat M, Nat N>
using gcd_t = typename GCD<M, N>::Result;

/// Fibonacci
template <Nat N>
struct Fibonacci;

/// Base case
template <>
struct Fibonacci<Z> {
    using Result = Z;
};

/// Recursive case
template <>
struct Fibonacci<One> {
    using Result = One;
};

/// Recursive case
template <Nat N>
struct Fibonacci<S<S<N>>> {
    using Result = add_t<typename Fibonacci<S<N>>::Result, typename Fibonacci<N>::Result>;
};

/// Alias for Fibonacci
template <Nat N>
using fibonacci_t = typename Fibonacci<N>::Result;

// Properties and predicates
// ----------------

/// Is even
template <Nat N>
struct IsEven;

/// Specialization for Z
template <>
struct IsEven<Z> {
    static constexpr bool value = true;
};

/// Specialization for One
template <>
struct IsEven<One> {
    static constexpr bool value = false;
};

/// Recursive case
template <Nat N>
struct IsEven<S<S<N>>> {
    static constexpr bool value = IsEven<N>::value;
};

/// Alias for IsEven
template <Nat N>
inline constexpr bool is_even_v = IsEven<N>::value;

/// Is odd
template <Nat N>
inline constexpr bool is_odd_v = !is_even_v<N>;


}; // namespace typical::nat
