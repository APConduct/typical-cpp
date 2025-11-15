module;
#include <cstddef>
#include <type_traits>


export module typical.church;

import typical.lambda;

export namespace typical {

// Church pairs

/// Pair = λx.λy.λf.f x y
using Pair = Abs<Abs<Abs<App<App<Var<0>, Var<2>>, Var<1>>>>>;

/// Fst = λp.p (λx.λy.x)
using Fst = Abs<App<Var<0>, Abs<Abs<Var<1>>>>>;

/// Snd = λp.p (λx.λy.y)
using Snd = Abs<App<Var<0>, Abs<Abs<Var<0>>>>>;

/// MakePair constructs a Church pair from two elements A and B
template <typename A, typename B>
using MakePair = App<App<Pair, A>, B>;

/// GetFst extracts the first element from a Church pair P
template <typename P>
using GetFst = App<Fst, P>;

/// GetSnd extracts the second element from a Church pair P
template <typename P>
using GetSnd = App<Snd, P>;

// Church lists

/// Nil = λc.λn.n
using Nil = Abs<Abs<Var<0>>>;

/// Cons = λh.λt.λc.λn.c h (t c n)
using Cons = Abs<Abs<Abs<Abs<App<App<Var<1>, Var<3>>, App<App<Var<2>, Var<1>>, Var<0>>>>>>>;

/// MakeCons constructs a Church list from head and tail
template <typename Head, typename Tail>
using MakeCons = App<App<Cons, Head>, Tail>;

/// Length = λl.l (λh.λt.S t) 0
using Length = Abs<App<App<Var<0>, Abs<Abs<App<Succ, Var<0>>>>>, Zero>>;

/// Head = λl.l (λh.λt.h) (λx.λy.y)  -- returns a dummy value if list is empty
using Head = Abs<App<App<Var<0>, Abs<Abs<Var<1>>>>, Var<999>>>;

/// Tail = λl.l (λh.λt.(MakePair (Snd l) (Cons h (Snd l)))) (MakePair Nil Nil)) Fst
using Tail = Abs<App<
    Fst, App<App<Var<0>, Abs<Abs<MakePair<GetSnd<Var<0>>, MakeCons<Var<1>, GetSnd<Var<0>>>>>>>, MakePair<Nil, Nil>>>>;

// Higher-order list functions

/// Map = λl.λf.l (λh.λt.(Cons (f h) t)) Nil
using Map = Abs<Abs<App<App<Var<0>, Abs<Abs<MakeCons<App<Var<3>, Var<1>>, Var<0>>>>>, Nil>>>;

/// Filter = λl.λp.l (λh.λt.(If (p h) (Cons h t) t)) Nil
using Filter =
    Abs<Abs<App<App<Var<0>, Abs<Abs<App<App<App<Var<3>, Var<1>>, MakeCons<Var<1>, Var<0>>>, Var<0>>>>>, Nil>>>;

/// FoldL = λl.λz.λf.l (λh.λt.f t h) z
using FoldL = Abs<Abs<Abs<App<App<Var<0>, Var<2>>, Var<1>>>>>;

/// FoldR = λl.λz.λf.l (λh.λt.f h t) z
using FoldR = FoldL;

// Additional list operations

/// Append = λl1.λl2.l1 (λh.λt.(Cons h t)) l2
using Append = Abs<Abs<App<App<Var<1>, Cons>, Var<0>>>>;

/// Reverse = λl.FoldL l (Nil) (λacc.λx.Append acc (MakeCons x Nil))
using Reverse = Abs<App<App<Var<0>, Abs<Abs<App<App<Append, Var<0>>, MakeCons<Var<1>, Nil>>>>>, Nil>>;

// Church arithmetic operations

/// Sum = λm.λn.m (λx.λy.S y) n
using Sum = Abs<App<App<Var<0>, Abs<Abs<App<App<Add, Var<1>>, Var<0>>>>>, Zero>>;

/// Product = λm.λn.m (λx.λy.Add n y) 0
using Product = Abs<App<App<Var<0>, Abs<Abs<App<App<Mul, Var<1>>, Var<0>>>>>, One>>;

// Convenience helpers

/// BuildList constructs a Church list from a variadic pack of types
template <typename... Ts>
struct ListBuilder;

/// Base case: empty list
template <>
struct ListBuilder<> {
    using Result = Nil;
};

/// Recursive case: construct Cons from head and recursively built tail
template <typename T, typename... Rest>
struct ListBuilder<T, Rest...> {
    using Result = MakeCons<T, typename ListBuilder<Rest...>::Result>;
};

/// Alias for BuildList
template <typename... Ts>
using BuildList = typename ListBuilder<Ts...>::Result;

/// ListLengthValue extracts the length of a Church list as a size_t value
template <typename List>
struct ListLengthValue {
protected:
    using LengthType = eval_t<App<Length, List>>;

    template <typename ChurchNum>
    struct ToSizeT;

    template <typename T>
    static constexpr size_t extract() {
        if constexpr (std::is_same_v<T, Zero>) {
            return 0;
        }
        else if constexpr (std::is_same_v<T, One>) {
            return 1;
        }
        else if constexpr (std::is_same_v<T, Two>) {
            return 2;
        }
        else {
            // TODO: For arbitrary Church numerals, evaluate them
            // with a proper extractor function
            return 999;
        }
    }

public:
    /// Extracted length value
    static constexpr size_t value = extract<LengthType>();
};

/// Alias for ListLengthValue
template <typename List>
inline constexpr size_t list_length_v = ListLengthValue<List>::value;

// Church maybe/option

/// Nothing = λn.λj.j
using Nothing = Abs<Abs<Var<1>>>;

/// Just = λv.λn.λj.n v
using Just = Abs<Abs<Abs<App<Var<0>, Var<2>>>>>;

/// MakeJust constructs a Just value from T
template <typename T>
using MakeJust = App<Just, T>;

/// FromMaybe = λm.m Id (λx.x)
using FromMaybe = Abs<Abs<App<App<Var<0>, Var<1>>, Id>>>;

/// IsJust = λm.m False (λx.True)
using IsJust = Abs<App<App<Var<0>, False>, Abs<True>>>;

/// IsNothing = λm.m True (λx.False)
using IsNothing = Abs<App<App<Var<0>, True>, Abs<False>>>;

// Church Either

/// Left = λa.λb.λf.f a
using Left = Abs<Abs<Abs<App<Var<1>, Var<2>>>>>;

/// Right = λa.λb.λf.f b
using Right = Abs<Abs<Abs<App<Var<0>, Var<2>>>>>;

/// MakeLeft constructs a Left value from T
template <typename T>
using MakeLeft = App<Left, T>;

/// MakeRight constructs a Right value from T
template <typename T>
using MakeRight = App<Right, T>;

/// IsLeft = λe.e (λx.True) (λy.False)
using IsLeft = Abs<App<App<Var<0>, Abs<True>>, Abs<False>>>;

/// IsRight = λe.e (λx.False) (λy.True)
using IsRight = Abs<App<App<Var<0>, Abs<False>>, Abs<True>>>;
} // namespace typical
