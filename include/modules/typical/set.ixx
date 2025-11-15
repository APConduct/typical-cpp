module;

export module typical.set;

import typical.lambda;


export namespace typical {
using EmptySet = Abs<False>;

using UniversalSet = Abs<True>;

template <typename A>
struct SingletonSet {
    using Prodicate = Abs<App<App<Abs<Abs<Var<1>>>, Var<0>>, A>>;
};

template <typename X, typename S>
struct MemberOf {
    using Result = eval_t<App<S, X>>;
};

template <typename X, typename S>
inline constexpr bool member_of_v = MemberOf<X, S>::Result::value;

template <typename X, typename S>
inline constexpr bool not_member_of_v = !member_of_v<X, S>;

// Set operations

namespace set {
using Union = Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, True>, App<App<Var<1>, Var<0>>, True>>, False>>>>;
}

template <typename S1, typename S2>
using union_set = eval_t<App<App<set::Union, S1>, S2>>;

namespace set {
using Intersertion = Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, False>, App<App<Var<1>, Var<0>>, True>>, False>>>>;
}
template <typename S1, typename S2>
using intersertion_set = eval_t<App<App<set::Intersertion, S1>, S2>>;

namespace set {
using Complement = Abs<Abs<App<App<App<Var<1>, Var<0>>, False>, True>>>;
}

template <typename S>
using complement_set = eval_t<App<set::Complement, S>>;

namespace set {
using Difference =
    Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, False>, App<App<App<Var<1>, Var<0>>, False>, True>>, False>>>>;
}

template <typename S1, typename S2>
using difference_set = eval_t<App<App<set::Difference, S1>, S2>>;

// Finite sets


} // namespace typical
