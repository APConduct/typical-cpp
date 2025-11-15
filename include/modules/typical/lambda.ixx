module;
#include <cstddef>
#include <type_traits>


export module typical.lambda;

export namespace typical {

/// Lambda calculus terms using de Bruijn indices
template <std::size_t Index>
struct Var {
    static constexpr size_t index = Index;
};

/// Lambda abstraction
template <typename Body>
struct Abs {
    using body = Body;
};

/// Function application
template <typename Func, typename Arg>
struct App {
    using func = Func;
    using arg = Arg;
};

/// Type traits to identify term types
template <typename T>
struct is_var : std::false_type {};

/// Specialization for Var
template <size_t Index>
struct is_var<Var<Index>> : std::true_type {};

/// Specialization for Abs
template <typename T>
struct is_abs : std::false_type {};

/// Specialization for Abs
template <typename Body>
struct is_abs<Abs<Body>> : std::true_type {};

/// Specialization for App
template <typename T>
struct is_app : std::false_type {};

/// Specialization for App
template <typename Func, typename Arg>
struct is_app<App<Func, Arg>> : std::true_type {};

/// Concept to identify lambda terms
template <typename T>
concept LambdaTerm = is_var<T>::value || is_abs<T>::value || is_app<T>::value;

/// Shift Up (for positive shifts)
template <typename Term, size_t Amount, size_t Cutoff = 0>
struct Shift;

/// Specialization for Var
template <size_t Index, size_t Amount, size_t Cutoff>
struct Shift<Var<Index>, Amount, Cutoff> {
    using Result = std::conditional_t<(Index >= Cutoff), Var<Index + Amount>, Var<Index>>;
};

/// Specialization for Abs
template <typename Body, size_t Amount, size_t Cutoff>
struct Shift<Abs<Body>, Amount, Cutoff> {
    using Result = Abs<typename Shift<Body, Amount, Cutoff + 1>::Result>;
};

/// Specialization for App
template <typename Func, typename Arg, size_t Amount, size_t Cutoff>
struct Shift<App<Func, Arg>, Amount, Cutoff> {
    using Result = App<typename Shift<Func, Amount, Cutoff>::Result, typename Shift<Arg, Amount, Cutoff>::Result>;
};

/// Alias for Shift
template <typename Term, size_t Amount, size_t Cutoff = 0>
using shift_t = Shift<Term, Amount, Cutoff>::Result;

// Shift Down (for negative shifts)
template <typename Term, size_t Amount, size_t Cutoff = 0>
struct ShiftDown;

/// Specialization for Var
template <size_t Index, size_t Amount, size_t Cutoff>
struct ShiftDown<Var<Index>, Amount, Cutoff> {
    using Result = std::conditional_t<(Index >= Cutoff), Var<Index - Amount>, Var<Index>>;
};

/// Specialization for Abs
template <typename Body, size_t Amount, size_t Cutoff>
struct ShiftDown<Abs<Body>, Amount, Cutoff> {
    using Result = Abs<typename ShiftDown<Body, Amount, Cutoff + 1>::Result>;
};

/// Specialization for App
template <typename Func, typename Arg, size_t Amount, size_t Cutoff>
struct ShiftDown<App<Func, Arg>, Amount, Cutoff> {
    using Result =
        App<typename ShiftDown<Func, Amount, Cutoff>::Result, typename ShiftDown<Arg, Amount, Cutoff>::Result>;
};

/// Alias for ShiftDown
template <typename Term, size_t Amount, size_t Cutoff = 0>
using shift_down_t = typename ShiftDown<Term, Amount, Cutoff>::Result;

/// Substitution
template <typename Term, size_t Index, typename Replacement>
struct Subst;

/// Specialization for Var
template <size_t VarIndex, size_t SubstIndex, typename Replacement>
struct Subst<Var<VarIndex>, SubstIndex, Replacement> {
    using Result = std::conditional_t<VarIndex == SubstIndex, Replacement, Var<VarIndex>>;
};

/// Specialization for Abs
template <typename Body, size_t Index, typename Replacement>
struct Subst<Abs<Body>, Index, Replacement> {
    using Result = Abs<typename Subst<Body, Index + 1, shift_t<Replacement, 1>>::Result>;
};

/// Specialization for App
template <typename Func, typename Arg, size_t Index, typename Replacement>
struct Subst<App<Func, Arg>, Index, Replacement> {
    using Result =
        App<typename Subst<Func, Index, Replacement>::Result, typename Subst<Arg, Index, Replacement>::Result>;
};

/// Alias for Subst
template <typename Term, size_t Index, typename Replacement>
using subst_t = typename Subst<Term, Index, Replacement>::Result;

// Beta Reduction

/// is_value trait
template <typename T>
struct is_value : std::false_type {};

/// Specialization for Abs
template <typename Body>
struct is_value<Abs<Body>> : std::true_type {};

/// Reduce (one step)
template <typename Term>
struct Reduce;

/// Specialization for Var
template <size_t Index>
struct Reduce<Var<Index>> {
    using Result = Var<Index>;
    static constexpr bool reduced = false;
};

/// Specialization for Abs
template <typename Body>
struct Reduce<Abs<Body>> {
    using Result = Abs<Body>;
    static constexpr bool reduced = false;
};

/// Specialization for App
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

    /// Specialization for beta reduction
    template <typename Body, typename A>
    struct DoBetaReduction<Abs<Body>, A> {
        using Shifted = shift_t<A, 1>;
        using Substituted = subst_t<Body, 0, Shifted>;
        using Result = shift_down_t<Substituted, 1>;
        static constexpr bool reduced = true;
    };

public:
    /// Final result after possible beta reduction
    using Result = typename DoBetaReduction<ReducedFunc, Arg>::Result;
    /// Indicates if a reduction was performed
    static constexpr bool reduced = DoBetaReduction<ReducedFunc, Arg>::reduced || func_reduced;
};

/// Alias for Reduce
template <typename Term>
using reduce_t = typename Reduce<Term>::Result;

/// Evaluation (full reduction)
template <typename Term, size_t MaxSteps = 1000>
struct Eval {
protected:
    using Step = reduce_t<Term>;
    static constexpr bool made_progress = Reduce<Term>::reduced;

public:
    /// Result after evaluation
    using Result = std::conditional_t<made_progress && (MaxSteps > 0), typename Eval<Step, MaxSteps - 1>::Result, Term>;
};

/// Base case: prevent unsigned wraparound when MaxSteps == 0
template <typename Term>
struct Eval<Term, 0> {
    /// Result after evaluation
    using Result = Term;
};

/// Alias for Eval
template <typename Term, size_t MaxSteps = 1000>
using eval_t = typename Eval<Term, MaxSteps>::Result;


// Church Encodings


using True = Abs<Abs<Var<1>>>;
using False = Abs<Abs<Var<0>>>;

using Zero = Abs<Abs<Var<0>>>;
using One = Abs<Abs<App<Var<1>, Var<0>>>>;
using Two = Abs<Abs<App<Var<1>, App<Var<1>, Var<0>>>>>;

using Succ = Abs<Abs<Abs<App<Var<1>, App<App<Var<2>, Var<1>>, Var<0>>>>>>;

using Add = Abs<Abs<Abs<Abs<App<App<Var<3>, Var<1>>, App<App<Var<2>, Var<1>>, Var<0>>>>>>>;

using Mul = Abs<Abs<Abs<App<Var<2>, App<Var<1>, Var<0>>>>>>;


// ----------------

using Id = Abs<Var<0>>;

using Const = Abs<Abs<Var<1>>>;

using Omega = App<Abs<App<Var<0>, Var<0>>>, Abs<App<Var<0>, Var<0>>>>;

using Y = Abs<App<Abs<App<Var<1>, Abs<App<App<Var<1>, Var<1>>, Var<0>>>>>,
                  Abs<App<Var<1>, Abs<App<App<Var<1>, Var<1>>, Var<0>>>>>>>;


} // namespace typical
