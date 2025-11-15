module;
#include <type_traits>


export module typical.eq;

import typical.lambda;
import typical.church;


export namespace typical {

template <typename A, typename B>
struct Eq;

template <typename A>
struct Eq<A, A> {
    using Left = A;
    using Right = A;

    struct Refl {
        static constexpr bool is_refl = true;
    };

    using type = Refl;
};

template <typename A, typename B>
concept ProvablyEqual = requires { typename Eq<A, B>::type; };

template <typename A, typename B>
    requires ProvablyEqual<A, B>
using eq_proof = typename Eq<A, B>::type;

// Symmetry

template <typename A, typename B, typename Proof>
struct Sym;

template <typename A, typename B>
    requires ProvablyEqual<A, B>
struct Sym<A, B, eq_proof<A, B>> {
    using Result = Eq<B, A>;
    using type = typename Result::type;
};

template <typename A, typename B, typename Proof>
using sym = typename Sym<A, B, Proof>::Result;

// Transitivity

template <typename A, typename B, typename C, typename ProofAB, typename ProofBC>
struct Trans;

template <typename A, typename B, typename C>
    requires ProvablyEqual<A, B> && ProvablyEqual<B, C>
struct Trans<A, B, C, eq_proof<A, B>, eq_proof<B, C>> {
    using Result = Eq<A, C>;
    using type = typename Result::type;
};

template <typename A, typename B, typename C, typename ProofAB, typename ProofBC>
using trans = typename Trans<A, B, C, ProofAB, ProofBC>::Result;

// Congruence

template <template <typename> class F, typename A, typename B, typename Proof>
struct Cong;

template <template <typename> class F, typename A, typename B>
    requires ProvablyEqual<A, B>
struct Cong<F, A, B, eq_proof<A, B>> {
    using Result = Eq<F<A>, F<B>>;
    using type = typename Result::type;
};

template <template <typename> class F, typename A, typename B, typename Proof>
using cong = typename Cong<F, A, B, Proof>::Result;

// Transport (substitution)

template <typename A, typename B, template <typename> class P, typename Proof>
struct Transport;

template <typename A, typename B, template <typename> class P>
    requires ProvablyEqual<A, B>
struct Transport<A, B, P, eq_proof<A, B>> {

    static auto apply(P<A> value) -> P<B> { return static_cast<P<B>>(value); }

    using From = P<A>;
    using To = P<B>;
};

template <typename A, typename B, template <typename> class P, typename Proof>
using transport = Transport<A, B, P, Proof>;

// Leibniz equality

template <typename A, typename B>
struct Leibinz {
    template <template <typename> class P>
    using Subst = transport<A, B, P, eq_proof<A, B>>;

    static constexpr bool equal = ProvablyEqual<A, B>;
};

// Inequality (negation)

template <typename A, typename B>
struct NotEq {
    static constexpr bool value = !ProvablyEqual<A, B>;
};

template <typename A, typename B>
inline constexpr bool not_eq_v = NotEq<A, B>::value;

// Decidable equality

template <typename A, typename B>
struct DecidableEq {
    static constexpr bool decidable = std::is_same_v<A, B>;

    using Result = std::conditional_t<decidable, Eq<A, A>, NotEq<A, B>>;
};

template <typename A, typename B>
using decide_eq = typename DecidableEq<A, B>::Result;

// Chain of equalities

template <typename... Types>
struct EqChain;

template <typename A>
struct EqChain<A> {
    using Result = Eq<A, A>;
};

template <typename A, typename B>
struct EqChain<A, B> {
    static_assert(ProvablyEqual<A, B>, "Types must be provably equal to form an equality chain.");
    using Result = Eq<A, B>;
};

template <typename A, typename B, typename C, typename... Rest>
struct EqChain<A, B, C, Rest...> {
    static_assert(ProvablyEqual<A, B>, "Adjacent types must be provably equal to form an equality chain.");
    using RestProof = EqChain<B, C, Rest...>::Result;
    using Result = trans<A, B, typename RestProof::Right, eq_proof<A, B>, typename RestProof::type>;
};

template <typename... Types>
using eq_chain = typename EqChain<Types...>::Result;

// Proof combinators

template <typename ProofAB, typename ProofBC>
struct ComposeEq;

template <typename A, typename B, typename C>
    requires ProvablyEqual<A, B> && ProvablyEqual<B, C>
struct ComposeEq<Eq<A, B>, Eq<B, C>> {
    using Result = trans<A, B, C, eq_proof<A, B>, eq_proof<B, C>>;
};

// Type-level computation proofs


// Arithmetic proofs


// List proofs


// Function extensionality


// Proof tactics

namespace proof_tactics {

template <typename A>
using refl = Eq<A, A>;

template <typename Proof>
using symetry = Sym<typename Proof::Left, typename Proof::Right, Proof>;

template <template <typename> class F, typename Proof>
using apply_both_sides = cong<F, typename Proof::Left, typename Proof::Right, Proof>;

} // namespace proof_tactics

// Tests


//

} // namespace typical
