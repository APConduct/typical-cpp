module;
#include <cstddef>
#include <type_traits>


export module typical;

export import typical.lambda;

export namespace typical {

template <std::size_t Index>
struct Var {
    static constexpr size_t index = Index;
};

template <typename Body>
struct Abs {
    using body = Body;
};

template <typename Func, typename Arg>
struct App {
    using func = Func;
    using arg = Arg;
};

template <typename T>
struct is_var : std::false_type {};

template <size_t Index>
struct is_var<Var<Index>> : std::true_type {};

template <typename T>
struct is_abs : std::false_type {};

template <typename Body>
struct is_abs<Abs<Body>> : std::true_type {};

template <typename T>
struct is_app : std::false_type {};

template <typename Func, typename Arg>
struct is_app<App<Func, Arg>> : std::true_type {};

template <typename T>
concept LambdaTerm = is_var<T>::value || is_abs<T>::value || is_app<T>::value;


template <typename Term, size_t Amount, size_t Cutoff = 0>
struct Shift;

template <size_t Index, size_t Amount, size_t Cutoff>
struct Shift<Var<Index>, Amount, Cutoff> {
    using Result = std::conditional_t<(Index >= Cutoff), Var<Index + Amount>, Var<Index>>;
};

template <typename Body, size_t Amount, size_t Cutoff>
struct Shift<Abs<Body>, Amount, Cutoff> {
    using Result = Abs<typename Shift<Body, Amount, Cutoff + 1>::Result>;
};

template <typename Func, typename Arg, size_t Amount, size_t Cutoff>
struct Shift<App<Func, Arg>, Amount, Cutoff> {
    using Result = App<typename Shift<Func, Amount, Cutoff>::Result, typename Shift<Arg, Amount, Cutoff>::Result>;
};

template <typename Term, size_t Amount, size_t Cutoff = 0>
using shift_t = Shift<Term, Amount, Cutoff>::Result;


template <typename Term, size_t Index, typename Replacement>
struct Subst;

template <size_t VarIndex, size_t SubstIndex, typename Replacement>
struct Subst<Var<VarIndex>, SubstIndex, Replacement> {
    using Result = std::conditional_t<VarIndex == SubstIndex, Replacement, Var<VarIndex>>;
};

template <typename Body, size_t Index, typename Replacement>
struct Subst<Abs<Body>, Index, Replacement> {
    using Result = Abs<typename Subst<Body, Index + 1, shift_t<Replacement, 1>>::Result>;
};

template <typename Func, typename Arg, size_t Index, typename Replacement>
struct Subst<App<Func, Arg>, Index, Replacement> {
    using Result =
        App<typename Subst<Func, Index, Replacement>::Result, typename Subst<Arg, Index, Replacement>::Result>;
};

template <typename Term, size_t Index, typename Replacement>
using subst_t = typename Subst<Term, Index, Replacement>::Result;

// Beta Reduction

template <typename T>
struct is_value : std::false_type {};

template <typename Body>
struct is_value<Abs<Body>> : std::true_type {};

template <typename Term>
struct Reduce;

template <size_t Index>
struct Reduce<Var<Index>> {
    using Result = Var<Index>;
    static constexpr bool reduced = false;
};

template <typename Body>
struct Reduce<Abs<Body>> {
    using Result = Abs<Body>;
    static constexpr bool reduced = false;
};

template <typename Func, typename Arg>
struct Reduce<App<Func, Arg>> {
private:
    using ReducedFunc = typename Reduce<Func>::Result;
    static constexpr bool func_reduced = Reduce<Func>::reduced;

    static constexpr bool is_beta_redex = is_abs<ReducedFunc>::value;

    template <typename F, typename A>
    struct DoBetaReduction {
        using Result = App<F, A>;
        static constexpr bool reduced = false;
    };

    template <typename Body, typename A>
    struct DoBetaReduction<Abs<Body>, A> {
        using Shifted = shift_t<A, 1>;
        using Substituted = subst_t<Body, 0, Shifted>;
        using Result = shift_t<Substituted, 1, /* - */ 1>; // TODO: implement size_t wrap-around for negative shifts
        static constexpr bool reduced = true;
    };

public:
    using Result = typename DoBetaReduction<ReducedFunc, Arg>::Result;
    static constexpr bool reduced = DoBetaReduction<ReducedFunc, Arg>::reduced || func_reduced;
};

template <typename Term>
using reduce_t = typename Reduce<Term>::Result;


template <typename Term, size_t MaxSteps = 1000>
struct Eval {
protected:
    using Step = reduce_t<Term>;
    static constexpr bool made_progress = Reduce<Term>::reduced;

public:
    using Result = std::conditional_t<made_progress && (MaxSteps > 0), typename Eval<Step, MaxSteps - 1>::Result, Term>;
};

template <typename Term, size_t MaxSteps = 1000>
using eval_t = typename Eval<Term, MaxSteps>::Result;


// Church Enconcodings


using True = Abs<Abs<Var<1>>>;
using False = Abs<Abs<Var<0>>>;

} // namespace typical
