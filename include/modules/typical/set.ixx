module;
#include <cstddef>
#include <type_traits>


export module typical.set;

import typical.lambda;
import typical.church;
import typical.eq;
import typical.nat;


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

template <typename... Elements>
struct FiniteSet {
    using AsList = BuildList<Elements...>;

    using Cardinality = eval_t<App<Length, AsList>>;

    template <typename X>
    struct Contains {
        static constexpr bool value = (std::is_same_v<X, Elements> || ...);
    };

    template <typename X>
    static constexpr bool contains = Contains<X>::value;
};

template <typename... Elements>
using finite_set = FiniteSet<Elements...>;

// Subset relation

template <typename S1, typename S2>
struct Subset {
    template <typename FS1, typename FS2>
    struct Check : std::false_type {};

    template <typename... E1, typename... E2>
    struct Check<FiniteSet<E1...>, FiniteSet<E2...>> {
        static constexpr bool value = (FiniteSet<E2...>::template contains<E1> && ...);
    };

    static constexpr bool value = Check<S1, S2>::value;
};

template <typename S1, typename S2>
inline constexpr bool subset_v = Subset<S1, S2>::value;

template <typename S1, typename S2>
inline constexpr bool proper_subset_v = subset_v<S1, S2> && !subset_v<S2, S1>;

// Set equality

template <typename S1, typename S2>
struct SetEqual {
    static constexpr bool value = subset_v<S1, S2> && subset_v<S2, S1>;

    using Proof = std::conditional_t<value, Eq<S1, S2>, void>;
};

template <typename S1, typename S2>
inline constexpr bool set_equal_v = SetEqual<S1, S2>::value;

// Cardinality

namespace set {
template <typename S>
struct Cardinality;

template <typename... Elements>
struct Cardinality<FiniteSet<Elements...>> {
    using Result = typename FiniteSet<Elements...>::Cardinality;
    static constexpr size_t value = nat::to_value_v<Result>;
};

} // namespace set

template <typename S>
using cardinality_t = typename set::Cardinality<S>::Result;

template <typename S>
inline constexpr size_t cardinality_v = set::Cardinality<S>::value;

// Power set

template <typename S>
struct PowerSet;

template <>
struct PowerSet<FiniteSet<>> {
    using Result = FiniteSet<FiniteSet<>>;
};

template <typename First, typename... Rest>
struct PowerSet<FiniteSet<First, Rest...>> {
    using RestPowerSet = typename PowerSet<FiniteSet<Rest...>>::Result;
    using Result = void; // TODO: Implement power set construction

    using CardinalityNat = nat::pow_t<nat::Two, typename FiniteSet<First, Rest...>::Cardinality>;
    static constexpr size_t cardinality = nat::to_value_v<CardinalityNat>;
};

template <typename S>
using power_set_t = typename PowerSet<S>::Result;

// Catesian Product

template <typename S1, typename S2>
struct CartesianProduct;

template <typename... E1, typename... E2>
struct CartesianProduct<FiniteSet<E1...>, FiniteSet<E2...>> {
    template <typename A, typename B>
    using MakePairType = MakePair<A, B>;

    using Card1 = typename FiniteSet<E1...>::Cardinality;
    using Card2 = typename FiniteSet<E2...>::Cardinality;
    using CardinalityNat = nat::mul_t<Card1, Card2>;
    static constexpr size_t cardinality = nat::to_value_v<CardinalityNat>;

    using Result = void; // TODO: Implement cartesian product construction
};

template <typename S1, typename S2>
using cartesian_product_t = CartesianProduct<S1, S2>;

// Disjoint sets

template <typename S1, typename S2>
struct AreDisjoint;

template <typename... E1, typename... E2>
struct AreDisjoint<FiniteSet<E1...>, FiniteSet<E2...>> {
    static constexpr bool value = !(... || (FiniteSet<E2...>::template contains<E1>));
};

template <typename S1, typename S2>
inline constexpr bool are_disjoint_v = AreDisjoint<S1, S2>::value;

// Partition

template <typename S, typename... Subsets>
struct IsPartition {
    static constexpr bool value = false; // TODO: Implement partition check
};

template <typename S, typename... Subsets>
inline constexpr bool is_partition_v = IsPartition<S, Subsets...>::value;

// Indexed families of sets

template <typename IndexSet, template <typename> class SetFamily>
struct IndexedFamily {
    using Indicies = IndexSet;

    template <typename I>
    using GetSet = SetFamily<I>;
};

// Cantor's Theorem

namespace cantor {
template <typename S>
struct Theorem {
    using Card_S = cardinality_t<S>;
    using Card_PowerS = typename PowerSet<S>::CardinalityNat;
    static constexpr bool holds = nat::less_than_v<Card_S, Card_PowerS>;

    using Proof = std::conditional_t<holds, void, void>; // Placeholder
};
} // namespace cantor

// Set builder notation

template <template <typename> class Predicate>
struct SetBuilder {
    using CharacteristicFunction = void; // TODO: Implement characteristic function as lambda when encoded
};

// A binary relation

template <typename A, typename B, template <typename, typename> class Relation>
struct BinaryRelation {
    using Domain = A;
    using Codomain = B;

    template <typename X, typename Y>
    static constexpr bool related = Relation<X, Y>::value;
};

// Reflexive relation

template <typename R>
struct IsReflexive {
    static constexpr bool value = false; // TODO: Implement reflexivity check
};

template <typename R>
struct IsSymmetric {
    static constexpr bool value = false; // TODO: Implement symmetry check
};

template <typename R>
struct IsTransitive {
    static constexpr bool value = false; // TODO: Implement transitivity check
};

template <typename R>
struct IsEquivalenceRelation {
    static constexpr bool value = IsReflexive<R>::value && IsSymmetric<R>::value && IsTransitive<R>::value;
};

template <typename A, typename B, template <typename> class F>
struct Function {
    using Domain = A;
    using Codomain = B;

    template <typename X>
    using Apply = typename F<X>::type;
};

// Injective

template <typename F>
struct IsInjective {
    static constexpr bool value = false; // TODO: Implement injectivity check
};

// Surjective

template <typename F>
struct IsSurjective {
    static constexpr bool value = false; // TODO: Implement surjectivity check
};

// Bijective

template <typename F>
struct IsBijective {
    static constexpr bool value = IsInjective<F>::value && IsSurjective<F>::value;
};

// Cardinal arithmetic

template <typename S1, typename S2>
struct SameCardinality {
    template <typename T1, typename T2>
    struct Check : std::false_type {};

    template <typename... E1, typename... E2>
    struct Check<FiniteSet<E1...>, FiniteSet<E2...>> {
        static constexpr bool value = (cardinality_v<FiniteSet<E1...>> == cardinality_v<FiniteSet<E2...>>);
    };

    static constexpr bool value = Check<S1, S2>::value;
};

template <typename S1, typename S2>
inline constexpr bool same_cardinality_v = SameCardinality<S1, S2>::value;


} // namespace typical
