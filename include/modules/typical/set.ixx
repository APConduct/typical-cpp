module;
#include <cstddef>
#include <type_traits>


export module typical.set;

import typical.lambda;
import typical.church;
import typical.eq;
import typical.nat;


export namespace typical {
// Basic sets

/// Empty set: λx.False
using EmptySet = Abs<False>;

/// Universal set: λx.True
using UniversalSet = Abs<True>;

/// Singleton set: λx.(x = A)
template <typename A>
struct SingletonSet {
    using Prodicate = Abs<App<App<Abs<Abs<Var<1>>>, Var<0>>, A>>;
};

/// MemberOf predicate: λx.S(x)
template <typename X, typename S>
struct MemberOf {
    using Result = eval_t<App<S, X>>;
};

/// MemberOf value
template <typename X, typename S>
inline constexpr bool member_of_v = MemberOf<X, S>::Result::value;

/// NotMemberOf value
template <typename X, typename S>
inline constexpr bool not_member_of_v = !member_of_v<X, S>;

// Set operations

namespace set {
/// Union = λs1.λs2.λx.(s1 x) ∨ (s2 x)
using Union = Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, True>, App<App<Var<1>, Var<0>>, True>>, False>>>>;
} // namespace set

/// Union of two sets
template <typename S1, typename S2>
using union_set = eval_t<App<App<set::Union, S1>, S2>>;

namespace set {
/// Intersection = λs1.λs2.λx.(s1 x) ∧ (s2 x)
using Intersertion = Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, False>, App<App<Var<1>, Var<0>>, True>>, False>>>>;
} // namespace set

/// Intersection of two sets
template <typename S1, typename S2>
using intersertion_set = eval_t<App<App<set::Intersertion, S1>, S2>>;

namespace set {
/// Complement = λs.λx.¬(s x)
using Complement = Abs<Abs<App<App<App<Var<1>, Var<0>>, False>, True>>>;
} // namespace set

/// Complement of a set
template <typename S>
using complement_set = eval_t<App<set::Complement, S>>;

namespace set {
/// Difference = λs1.λs2.λx.(s1 x) ∧ ¬(s2 x)
using Difference =
    Abs<Abs<Abs<App<App<App<App<Var<2>, Var<0>>, False>, App<App<App<Var<1>, Var<0>>, False>, True>>, False>>>>;
} // namespace set

/// Difference of two sets
template <typename S1, typename S2>
using difference_set = eval_t<App<App<set::Difference, S1>, S2>>;

// Finite sets

/// Finite set represented as a variadic template of its elements
template <typename... Elements>
struct FiniteSet {
    /// Church list of elements
    using AsList = BuildList<Elements...>;

    /// Cardinality of the set
    using Cardinality = eval_t<App<Length, AsList>>;

    /// Contains predicate
    template <typename X>
    struct Contains {
        /// Check if X is in Elements...
        static constexpr bool value = (std::is_same_v<X, Elements> || ...);
    };

    /// Contains value
    template <typename X>
    static constexpr bool contains = Contains<X>::value;
};

/// Alias for FiniteSet
template <typename... Elements>
using finite_set = FiniteSet<Elements...>;

// Subset relation

/// Subset
template <typename S1, typename S2>
struct Subset {
    /// Check helper
    template <typename FS1, typename FS2>
    struct Check : std::false_type {};

    /// Specialization for FiniteSets
    template <typename... E1, typename... E2>
    struct Check<FiniteSet<E1...>, FiniteSet<E2...>> {
        static constexpr bool value = (FiniteSet<E2...>::template contains<E1> && ...);
    };

    /// Value
    static constexpr bool value = Check<S1, S2>::value;
};

/// Subset value
template <typename S1, typename S2>
inline constexpr bool subset_v = Subset<S1, S2>::value;

/// Proper subset value
template <typename S1, typename S2>
inline constexpr bool proper_subset_v = subset_v<S1, S2> && !subset_v<S2, S1>;

/// Set equality
template <typename S1, typename S2>
struct SetEqual {
    /// Value
    static constexpr bool value = subset_v<S1, S2> && subset_v<S2, S1>;

    /// Proof
    using Proof = std::conditional_t<value, Eq<S1, S2>, void>;
};

/// Set equality value
template <typename S1, typename S2>
inline constexpr bool set_equal_v = SetEqual<S1, S2>::value;

// Cardinality

namespace set {
/// Cardinality of a set
template <typename S>
struct Cardinality;

/// Specialization for FiniteSet
template <typename... Elements>
struct Cardinality<FiniteSet<Elements...>> {
    /// Result
    using Result = typename FiniteSet<Elements...>::Cardinality;
    /// Value
    static constexpr size_t value = nat::to_value_v<Result>;
};

} // namespace set

/// Alias for Cardinality
template <typename S>
using cardinality_t = typename set::Cardinality<S>::Result;

/// Cardinality value
template <typename S>
inline constexpr size_t cardinality_v = set::Cardinality<S>::value;

/// Power set
template <typename S>
struct PowerSet;

/// Specialization for FiniteSet
template <>
struct PowerSet<FiniteSet<>> {
    using Result = FiniteSet<FiniteSet<>>;
};

/// Specialization for non-empty FiniteSet
template <typename First, typename... Rest>
struct PowerSet<FiniteSet<First, Rest...>> {
    /// Power set of the rest
    using RestPowerSet = typename PowerSet<FiniteSet<Rest...>>::Result;
    /// Result
    using Result = void; // TODO: Implement power set construction

    /// Cardinality
    using CardinalityNat = nat::pow_t<nat::Two, typename FiniteSet<First, Rest...>::Cardinality>;
    /// Cardinality value
    static constexpr size_t cardinality = nat::to_value_v<CardinalityNat>;
};

/// Alias for PowerSet
template <typename S>
using power_set_t = typename PowerSet<S>::Result;

/// Catesian Product
template <typename S1, typename S2>
struct CartesianProduct;

/// Helper to make a pair type
template <typename... E1, typename... E2>
struct CartesianProduct<FiniteSet<E1...>, FiniteSet<E2...>> {
    /// Pair type constructor
    template <typename A, typename B>
    using MakePairType = MakePair<A, B>;

    /// Cardinality
    using Card1 = typename FiniteSet<E1...>::Cardinality;
    /// Cardinality
    using Card2 = typename FiniteSet<E2...>::Cardinality;
    /// Cardinality of the product
    using CardinalityNat = nat::mul_t<Card1, Card2>;
    /// Cardinality value
    static constexpr size_t cardinality = nat::to_value_v<CardinalityNat>;

    /// Result
    using Result = void; // TODO: Implement cartesian product construction
};

/// Alias for CartesianProduct
template <typename S1, typename S2>
using cartesian_product_t = CartesianProduct<S1, S2>;

// Disjoint sets

/// Are disjoint
template <typename S1, typename S2>
struct AreDisjoint;

/// Specialization for FiniteSets
template <typename... E1, typename... E2>
struct AreDisjoint<FiniteSet<E1...>, FiniteSet<E2...>> {
    static constexpr bool value = !(... || (FiniteSet<E2...>::template contains<E1>));
};

/// Are disjoint value
template <typename S1, typename S2>
inline constexpr bool are_disjoint_v = AreDisjoint<S1, S2>::value;

// Partition

/// Is partition
template <typename S, typename... Subsets>
struct IsPartition {
    static constexpr bool value = false; // TODO: Implement partition check
};

/// Is partition value
template <typename S, typename... Subsets>
inline constexpr bool is_partition_v = IsPartition<S, Subsets...>::value;

// Indexed families of sets

/// Indexed family of sets
template <typename IndexSet, template <typename> class SetFamily>
struct IndexedFamily {
    /// Indicies
    using Indicies = IndexSet;

    /// Get set by index
    template <typename I>
    using GetSet = SetFamily<I>;
};

// Cantor's Theorem

namespace cantor {
/// Cantor's Theorem: For any set S, the cardinality of S is strictly less than the cardinality of its power set
template <typename S>
struct Theorem {
    /// Cardinalities
    using Card_S = cardinality_t<S>;
    /// Cardinality of power set
    using Card_PowerS = typename PowerSet<S>::CardinalityNat;
    /// Holds
    static constexpr bool holds = nat::less_than_v<Card_S, Card_PowerS>;

    /// Proof
    using Proof = std::conditional_t<holds, void, void>; // Placeholder
};
} // namespace cantor

// Set builder notation

template <template <typename> class Predicate>
/// Set builder
struct SetBuilder {
    /// Characteristic function
    using CharacteristicFunction = void; // TODO: Implement characteristic function as lambda when encoded
};

// A binary relation

/// Binary relation between sets A and B
template <typename A, typename B, template <typename, typename> class Relation>
struct BinaryRelation {
    /// Domain
    using Domain = A;
    /// Codomain
    using Codomain = B;

    /// Check if X in A is related to Y in B
    template <typename X, typename Y>
    static constexpr bool related = Relation<X, Y>::value;
};

/// Reflexive relation
template <typename R>
struct IsReflexive {
    static constexpr bool value = false; // TODO: Implement reflexivity check
};

/// Symmetric relation
template <typename R>
struct IsSymmetric {
    static constexpr bool value = false; // TODO: Implement symmetry check
};

/// Transitive relation
template <typename R>
struct IsTransitive {
    static constexpr bool value = false; // TODO: Implement transitivity check
};

/// Equivalence relation
template <typename R>
struct IsEquivalenceRelation {
    static constexpr bool value = IsReflexive<R>::value && IsSymmetric<R>::value && IsTransitive<R>::value;
};

// Functions between sets
template <typename A, typename B, template <typename> class F>
struct Function {
    using Domain = A;
    using Codomain = B;

    template <typename X>
    using Apply = typename F<X>::type;
};

/// Injective
template <typename F>
struct IsInjective {
    static constexpr bool value = false; // TODO: Implement injectivity check
};

/// Surjective
template <typename F>
struct IsSurjective {
    static constexpr bool value = false; // TODO: Implement surjectivity check
};

/// Bijective
template <typename F>
struct IsBijective {
    static constexpr bool value = IsInjective<F>::value && IsSurjective<F>::value;
};

// Cardinal arithmetic

/// Same cardinality
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

/// Same cardinality value
template <typename S1, typename S2>
inline constexpr bool same_cardinality_v = SameCardinality<S1, S2>::value;


} // namespace typical
