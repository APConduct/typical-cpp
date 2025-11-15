module;
#include <cstddef>
#include <type_traits>

export module typical.nat;

import typical.eq;

export namespace typical {

struct Z {};

template <typename N>
struct S {};

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

template <typename T>
struct is_nat : std::false_type {};

template <>
struct is_nat<Z> : std::true_type {};

template <typename N>
struct is_nat<S<N>> : is_nat<N> {};

template <typename T>
concept Nat = is_nat<T>::value;

template <typename N>
struct is_zero : std::false_type {};

template <>
struct is_zero<Z> : std::true_type {};

template <typename N>
inline constexpr bool is_zero_v = is_zero<N>::value;

// Conversion
// ----------------

/// Convert Peano number = size_t
template <Nat N>
struct ToValue;

template <>
struct ToValue<Z> {
    static constexpr size_t value = 0;
};

template <Nat N>
struct ToValue<S<N>> {
    static constexpr size_t value = 1 + ToValue<N>::value;
};

template <Nat N>
inline constexpr size_t to_value_v = ToValue<N>::value;

/// Convert size_t to Peano number
template <size_t N>
struct FromValue {
    using type = S<typename FromValue<N - 1>::type>;
};

template <>
struct FromValue<0> {
    using type = Z;
};

template <size_t N>
using from_value_t = typename FromValue<N>::type;

// Addition
// ----------------

template <Nat M, Nat N>
struct Add;

template <Nat N>
struct Add<Z, N> {
    using Result = N;
};

/// Recursive case
template <Nat M, Nat N>
struct Add<S<M>, N> {
    using Result = S<typename Add<M, N>::Result>;
};

template <Nat M, Nat N>
using add_t = typename Add<M, N>::Result;

// Addition proofs

template <Nat N>
struct AddLeftIdentity {
    using Left = add_t<Z, N>;
    using Right = N;
    using Proof = typical::Eq<Left, Right>;
};

template <Nat N>
struct AddRightIdentity;

template <>
struct AddRightIdentity<Z> {
    using Left = add_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

template <Nat N>
struct AddRightIdentity<S<N>> {
    using IH = typename AddRightIdentity<N>::Proof;
    using Left = add_t<S<N>, Z>;
    using Right = S<N>;
    using Proof = typical::Eq<Left, Right>;
};

template <Nat M, Nat N>
struct AddCommutative {
    using Left = add_t<M, N>;
    using Right = add_t<N, M>;

    // TODO: enhance complexity of proof
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

// Multiplication

template <Nat M, Nat N>
struct Mul;

template <Nat N>
struct Mul<Z, N> {
    using Result = Z;
};

template <Nat M, Nat N>
struct Mul<S<M>, N> {
    using Result = add_t<N, typename Mul<M, N>::Result>;
};

template <Nat M, Nat N>
using mul_t = typename Mul<M, N>::Result;

// Multiplication proofs

template <Nat N>
struct MulLeftZero {
    using Left = mul_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

template <Nat N>
struct MulRightZero;

template <>
struct MulRightZero<Z> {
    using Left = mul_t<Z, Z>;
    using Right = Z;
    using Proof = typical::Eq<Left, Right>;
};

template <Nat N>
struct MulRightZero<S<N>> {
    using Left = mul_t<S<N>, Z>;
    using Right = Z;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

template <Nat N>
struct MulLeftIdentity {
    using Left = mul_t<One, N>;
    using Right = N;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

template <Nat M, Nat N>
struct MulCommutative {
    using Lfet = mul_t<M, N>;
    using Right = mul_t<N, M>;
    using Proof = std::conditional_t<std::is_same_v<Lfet, Right>, Eq<Lfet, Right>, void>;
};

template <Nat M, Nat N, Nat P>
struct MulDistributiveLeft {
    using Left = mul_t<M, add_t<N, P>>;
    using Right = add_t<mul_t<M, N>, mul_t<M, P>>;
    using Proof = std::conditional_t<std::is_same_v<Left, Right>, Eq<Left, Right>, void>;
};

// Subtraction (partial - results Z if result would be negative)
// ----------------

template <Nat M, Nat N>
struct Sub;

template <Nat M>
struct Sub<M, Z> {
    using Result = M;
};

template <Nat N>
struct Sub<Z, S<N>> {
    using Result = Z;
};

template <Nat M, Nat N>
struct Sub<S<M>, S<N>> {
    using Result = typename Sub<M, N>::Result;
};

template <Nat M, Nat N>
using sub_t = typename Sub<M, N>::Result;

// Comparison
// ----------------

template <Nat M, Nat N>
struct LessThan;

template <Nat N>
struct LessThan<Z, S<N>> {
    static constexpr bool value = true;
    using Proof = void;
};

template <Nat M>
struct LessThan<S<M>, Z> {
    static constexpr bool value = false;
};


template <>
struct LessThan<Z, Z> {
    static constexpr bool value = false;
};

template <Nat M, Nat N>
struct LessThan<S<M>, S<N>> {
    static constexpr bool value = LessThan<M, N>::value;
    using Proof = typename LessThan<M, N>::Proof;
};

template <Nat M, Nat N>
inline constexpr bool less_than_v = LessThan<M, N>::value;

template <Nat M, Nat N>
struct LessOrEqual {
    static constexpr bool value = std::is_same_v<M, N> || less_than_v<M, N>;
};

template <Nat M, Nat N>
inline constexpr bool less_or_equal_v = LessOrEqual<M, N>::value;

template <Nat M, Nat N>
inline constexpr bool greater_than_v = less_than_v<N, M>;

template <Nat M, Nat N>
inline constexpr bool nat_equal_v = std::is_same_v<M, N>;

// Min and Max
// ----------------


template <Nat M, Nat N>
struct Min {
    using Result = std::conditional_t<less_or_equal_v<M, N>, M, N>;
};

template <Nat M, Nat N>
using min_t = typename Min<M, N>::Result;

template <Nat M, Nat N>
struct Max {
    using Result = std::conditional_t<greater_than_v<M, N>, M, N>;
};

template <Nat M, Nat N>
using max_t = typename Max<M, N>::Result;

// Division and Modulus
// ----------------

template <Nat M, Nat N>
struct Div;

template <Nat M>
struct Div<M, Z> {
    using Result = Z;
};

template <Nat N>
struct Div<Z, S<N>> {
    using Result = Z;
};

template <Nat M, Nat N>
    requires(!is_zero_v<N> && !is_zero_v<M>)
struct Div<M, N> {
protected:
    using Difference = sub_t<M, N>;
    static constexpr bool m_ge_n = less_or_equal_v<N, M>;

public:
    using Result = std::conditional_t<m_ge_n, S<typename Div<Difference, N>::Result>, Z>;
};

template <Nat M, Nat N>
using div_t = typename Div<M, N>::Result;

template <Nat M, Nat N>
struct Mod;

template <Nat M>
struct Mod<M, Z> {
    using Result = Z;
};

template <Nat N>
struct Mod<Z, S<N>> {
    using Result = Z;
};

template <Nat M, Nat N>
    requires(!is_zero_v<N> && !is_zero_v<M>)
struct Mod<M, N> {
protected:
    static constexpr bool m_lt_n = less_than_v<M, N>;
    using Difference = sub_t<M, N>;

public:
    using Result = std::conditional_t<m_lt_n, M, typename Mod<Difference, N>::Result>;
};

template <Nat M, Nat N>
using mod_t = typename Mod<M, N>::Result;

template <Nat Base, Nat Exp>
struct Pow;

template <Nat Base>
struct Pow<Base, Z> {
    using Result = One;
};

template <Nat Base, Nat Exp>
struct Pow<Base, S<Exp>> {
    using Result = mul_t<Base, typename Pow<Base, Exp>::Result>;
};

template <Nat Base, Nat Exp>
using pow_t = typename Pow<Base, Exp>::Result;

// Factorial

template <Nat N>
struct Factorial;

template <>
struct Factorial<Z> {
    using Result = One;
};

template <Nat N>
struct Factorial<S<N>> {
    using Result = mul_t<S<N>, typename Factorial<N>::Result>;
};

template <Nat N>
using factorial_t = typename Factorial<N>::Result;

// GCD (Greatest Common Divisor)

template <Nat M, Nat N>
struct GCD;

template <Nat M>
struct GCD<M, Z> {
    using Result = M;
};

template <Nat M, Nat N>
    requires(!is_zero_v<N>)
struct GCD<M, N> {
    using Result = typename GCD<N, mod_t<M, N>>::Result;
};

template <Nat M, Nat N>
using gcd_t = typename GCD<M, N>::Result;

// Fibonacci

template <Nat N>
struct Fibonacci;

template <>
struct Fibonacci<Z> {
    using Result = Z;
};

template <>
struct Fibonacci<One> {
    using Result = One;
};

template <Nat N>
    requires(!is_zero_v<N>)
struct Fibonacci<S<S<N>>> {
    using Result = add_t<Fibonacci<typename S<N>::Result>, typename Fibonacci<N>::Result>;
};

template <Nat N>
using fibonacci_t = typename Fibonacci<N>::Result;

// Properties and predicates
// ----------------

template <Nat N>
struct IsEven;

template <>
struct IsEven<Z> {
    static constexpr bool value = true;
};

template <>
struct IsEven<One> {
    static constexpr bool value = false;
};

template <Nat N>
struct IsEven<S<S<N>>> {
    static constexpr bool value = IsEven<N>::value;
};

template <Nat N>
inline constexpr bool is_even_v = IsEven<N>::value;

template <Nat N>
inline constexpr bool is_odd_v = !is_even_v<N>;


}; // namespace typical
