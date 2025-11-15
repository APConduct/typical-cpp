module;
#include <cstddef>
#include <type_traits>


export module typical.church;

import typical.lambda;

export namespace typical {

using Pair = Abs<Abs<Abs<App<App<Var<0>, Var<2>>, Var<1>>>>>;

using Fst = Abs<App<Var<0>, Abs<Abs<Var<1>>>>>;

using Snd = Abs<App<Var<0>, Abs<Abs<Var<0>>>>>;

template <typename A, typename B>
using MakePair = App<App<Pair, A>, B>;

template <typename P>
using GetFst = App<Fst, P>;

template <typename P>
using GetSnd = App<Snd, P>;

// Church lists

using Nil = Abs<Abs<Var<0>>>;

using Cons = Abs<Abs<Abs<Abs<App<App<Var<1>, Var<3>>, App<App<Var<2>, Var<1>>, Var<0>>>>>>>;

template <typename Head, typename Tail>
using MakeCons = App<App<Cons, Head>, Tail>;

using Length = Abs<App<App<Var<0>, Abs<Abs<App<Succ, Var<0>>>>>, Zero>>;

using Head = Abs<App<App<Var<0>, Abs<Abs<Var<1>>>>, Var<999>>>;

using Tail = Abs<App<
    Fst, App<App<Var<0>, Abs<Abs<MakePair<GetSnd<Var<0>>, MakeCons<Var<1>, GetSnd<Var<0>>>>>>>, MakePair<Nil, Nil>>>>;

using Map = Abs<Abs<App<App<Var<0>, Abs<Abs<MakeCons<App<Var<3>, Var<1>>, Var<0>>>>>, Nil>>>;

using Filter =
    Abs<Abs<App<App<Var<0>, Abs<Abs<App<App<App<Var<3>, Var<1>>, MakeCons<Var<1>, Var<0>>>, Var<0>>>>>, Nil>>>;

using FoldL = Abs<Abs<Abs<App<App<Var<0>, Var<2>>, Var<1>>>>>;

using FoldR = FoldL;

using Append = Abs<Abs<App<App<Var<1>, Cons>, Var<0>>>>;

using Reverse = Abs<App<App<Var<0>, Abs<Abs<App<App<Append, Var<0>>, MakeCons<Var<1>, Nil>>>>>, Nil>>;

using Sum = Abs<App<App<Var<0>, Abs<Abs<App<App<Add, Var<1>>, Var<0>>>>>, Zero>>;

using Product = Abs<App<App<Var<0>, Abs<Abs<App<App<Mul, Var<1>>, Var<0>>>>>, One>>;

// Convenience helpers

template <typename... Ts>
struct ListBuilder;

template <>
struct ListBuilder<> {
    using Result = Nil;
};

template <typename T, typename... Rest>
struct ListBuilder<T, Rest...> {
    using Result = MakeCons<T, typename ListBuilder<Rest...>::Result>;
};

template <typename... Ts>
using BuildList = typename ListBuilder<Ts...>::Result;

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
    static constexpr size_t value = extract<LengthType>();
};

template <typename List>
inline constexpr size_t list_length_v = ListLengthValue<List>::value;

// Church maybe/option

using Nothing = Abs<Abs<Var<1>>>;

using Just = Abs<Abs<Abs<App<Var<0>, Var<2>>>>>;

template <typename T>
using MakeJust = App<Just, T>;

using FromMaybe = Abs<Abs<App<App<Var<0>, Var<1>>, Id>>>;

using IsJust = Abs<App<App<Var<0>, False>, Abs<True>>>;

using IsNothing = Abs<App<App<Var<0>, True>, Abs<False>>>;

// Church Either

using Left = Abs<Abs<Abs<App<Var<1>, Var<2>>>>>;

using Right = Abs<Abs<Abs<App<Var<0>, Var<2>>>>>;

template <typename T>
using MakeLeft = App<Left, T>;

template <typename T>
using MakeRight = App<Right, T>;

using IsLeft = Abs<App<App<Var<0>, Abs<True>>, Abs<False>>>;

using IsRight = Abs<App<App<Var<0>, Abs<False>>, Abs<True>>>;

// Tests


} // namespace typical
