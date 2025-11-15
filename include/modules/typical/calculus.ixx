module;
#include <type_traits>
#include <typeinfo>


export module typical.calculus;

export namespace typical {

/// Base expression for variables in calculus
struct Var {};

template <auto C>
struct Const {
    static constexpr auto value = C;
};

// Binary operations

template <typename L, typename R>
struct Add {
    // static constexpr auto value = L::value + R::value;
};

template <typename L, typename R>
struct Sub {};

template <typename L, typename R>
struct Mul {};

template <typename L, typename R>
struct Div {};

template <typename E>
struct Neg {};

template <typename Base, auto Exponent>
struct Pow {};

template <typename E>
struct Sin {};

template <typename E>
struct Cos {};

template <typename E>
struct Tan {};

template <typename E>
struct Exp {};

template <typename E>
struct Log {};

template <typename E>
struct Sqrt {};

using X = Var;

template <auto C>
using C_ = Const<C>;

// Expression classification

template <typename T>
struct is_expr : std::false_type {};

template <>
struct is_expr<Var> : std::true_type {};

template <auto C>
struct is_expr<Const<C>> : std::true_type {};

template <typename L, typename R>
struct is_expr<Add<L, R>> : std::true_type {};

template <typename L, typename R>
struct is_expr<Sub<L, R>> : std::true_type {};

template <typename L, typename R>
struct is_expr<Mul<L, R>> : std::true_type {};

template <typename L, typename R>
struct is_expr<Div<L, R>> : std::true_type {};

template <typename E, auto N>
struct is_expr<Pow<E, N>> : std::true_type {};

template <typename E>
struct is_expr<Sin<E>> : std::true_type {};

template <typename E>
struct is_expr<Cos<E>> : std::true_type {};

template <typename E>
struct is_expr<Exp<E>> : std::true_type {};

template <typename E>
struct is_expr<Log<E>> : std::true_type {};

template <typename T>
concept Expression = is_expr<T>::value;

template <Expression E>
struct IsConstant;

template <>
struct IsConstant<Var> : std::false_type {};

template <auto C>
struct IsConstant<Const<C>> : std::true_type {};

template <typename L, typename R>
struct IsConstant<Add<L, R>> : std::bool_constant<IsConstant<L>::value && IsConstant<R>::value> {};

template <typename L, typename R>
struct IsConstant<Sub<L, R>> : std::bool_constant<IsConstant<L>::value && IsConstant<R>::value> {};

template <typename L, typename R>
struct IsConstant<Mul<L, R>> : std::bool_constant<IsConstant<L>::value && IsConstant<R>::value> {};

template <typename L, typename R>
struct IsConstant<Div<L, R>> : std::bool_constant<IsConstant<L>::value && IsConstant<R>::value> {};

template <typename E, auto N>
struct IsConstant<Pow<E, N>> : IsConstant<E> {};

template <typename E>
struct IsConstant<Sin<E>> : IsConstant<E> {};

template <typename E>
struct IsConstant<Cos<E>> : IsConstant<E> {};

template <typename E>
struct IsConstant<Exp<E>> : IsConstant<E> {};

template <typename E>
struct IsConstant<Log<E>> : IsConstant<E> {};

template <Expression E>
inline constexpr bool is_constant_v = IsConstant<E>::value;

// Differentiation
//
inline auto mul = [](auto a, auto b) { return a * b; };

// type of mul
inline auto& t_i = typeid(mul);


} // namespace typical
