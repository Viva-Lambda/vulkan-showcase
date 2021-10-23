#pragma once
// template utility functions
#include <external.hpp>
namespace vtuto {

/* template utility functions from
Wei, L. (2021) C++ template metaprogramming in practice: a
deep learning framework. First edition. Boca Raton, FL: CRC
Press.
*/
/** input - output mapping description
  Metafunction template basically takes FnOut<int>::type
 */
template <typename T> struct FnOut { using type = T; };
template <> struct FnOut<char> { using type = int; };
// we use it like FnOut<char>::type h = 3; meaning the
// function that takes input uint outputs int which is the
// type declaration
// of the h

// higher order functions
// F(T, v) = T(v)
// we assume here is that T is something like FnOut above
template <template <typename> class T, typename v> struct F {
  using type = typename T<v>::type;
};

template <unsigned int... Vals> struct UintContainer;
template <int... Vals> struct IntContainer;
template <std::size_t... Vals> struct SizeContainer;
template <char... Vals> struct CharContainer;
template <bool... Vals> struct BoolContainer;

// get for compile time

// compile time search operation on non type templates

// base case: uint
template <unsigned int... Nums>
constexpr std::pair<bool, unsigned int> find_nontype(const unsigned int &&e) {
  //
  return std::make_pair(false, e);
}
// recursive case
template <unsigned int El, unsigned int... Nums>
constexpr std::pair<bool, unsigned int> find_nontype(const unsigned int &&e) {
  if constexpr (El == e) {
    return std::make_pair(true, El);
  }
  return find_nontype<Nums...>(e);
}
// base case: int
template <int... Nums>
constexpr std::pair<bool, int> find_nontype(const int &&e) {
  //
  return std::make_pair(false, e);
}
// recursive case
template <int El, int... Nums>
constexpr std::pair<bool, int> find_nontype(const int &&e) {
  if constexpr (El == e) {
    return std::make_pair(true, El);
  }
  return find_nontype<Nums...>(e);
}

// base case: char
template <char... Nums>
constexpr std::pair<bool, char> find_nontype(const char &&e) {
  //
  return std::make_pair(false, e);
}
// recursive case
template <char El, char... Nums>
constexpr std::pair<bool, char> find_nontype(const char &&e) {
  if constexpr (El == e) {
    return std::make_pair(true, El);
  }
  return find_nontype<Nums...>(e);
}
// base case: bool
template <bool... Nums>
constexpr std::pair<bool, bool> find_nontype(const bool &&e) {
  //
  return std::make_pair(false, e);
}
// recursive case
template <bool El, bool... Nums>
constexpr std::pair<bool, bool> find_nontype(const bool &&e) {
  if constexpr (e == El) {
    return std::make_pair(true, El);
  }
  return find_nontype<Nums...>(e);
}
// base case: size_t
template <std::size_t... Nums>
constexpr std::pair<bool, std::size_t> find_nontype(const std::size_t &&e) {
  //
  return std::make_pair(false, e);
}
// recursive case
template <std::size_t El, std::size_t... Nums>
constexpr std::pair<bool, std::size_t> find_nontype(const std::size_t &&e) {
  if constexpr (e == El) {
    return std::make_pair(true, El);
  }
  return find_nontype<Nums...>(e);
}

// compile time arithmetic on numeric templates
enum class ArtOp : std::uint8_t { PLUS = 1, MINUS = 2, MULTIPLY = 3 };

template <typename T, ArtOp op>
constexpr T binop = [](T Acc, T arg) -> T {
  if constexpr (op == ArtOp::PLUS) {
    return Acc + arg;
  }
  if constexpr (op == ArtOp::MINUS) {
    return Acc - arg;
  }
  if constexpr (op == ArtOp::MULTIPLY) {
    return Acc * arg;
  }
};
// base case: uint
template <ArtOp op, unsigned int... Nums> constexpr unsigned int uint_foldl() {
  if constexpr (op == ArtOp::PLUS)
    return 0;
  if constexpr (op == ArtOp::MINUS)
    return 0;
  if constexpr (op == ArtOp::MULTIPLY)
    return 1;
  return 0;
}
// recursive case
template <ArtOp op, unsigned int Acc, unsigned int... Nums>
constexpr unsigned int uint_foldl() {
  return binop<unsigned int, op>(Acc, uint_foldl<op, Nums...>);
}
// base case: int
template <ArtOp op, int... Nums> constexpr int int_foldl() {
  if constexpr (op == ArtOp::PLUS)
    return 0;
  if constexpr (op == ArtOp::MINUS)
    return 0;
  if constexpr (op == ArtOp::MULTIPLY)
    return 1;
  return 0;
}
// recursive case
template <ArtOp op, int Acc, int... Nums> constexpr int int_foldl() {
  return binop<int, op>(Acc, int_foldl<op, Nums...>);
}
// base case: std::size_t
template <ArtOp op, std::size_t... Nums> constexpr std::size_t size_foldl() {
  if constexpr (op == ArtOp::PLUS)
    return static_cast<std::size_t>(0);
  if constexpr (op == ArtOp::MINUS)
    return static_cast<std::size_t>(0);
  if constexpr (op == ArtOp::MULTIPLY)
    return static_cast<std::size_t>(1);
  return static_cast<std::size_t>(0);
}
// recursive case
template <ArtOp op, std::size_t Acc, std::size_t... Nums>
constexpr std::size_t size_foldl() {
  return binop<std::size_t, op>(Acc, size_foldl<op, Nums...>);
}

template <typename... Vals> struct TypeContainer;

template <template <typename> class... T> struct SingleTemplateCont;
template <template <typename...> class... T> struct MultiTemplateCont;

template <typename Return, typename OverloadFn, typename... Args> struct op {
  Return operator()(const OverloadFn &f, Args... args) { return f(args...); }
};

template <class...> struct TypeList {};

// 1 - N
template <typename OverloadFn, typename InArg, typename... OutArgs>
struct vec_fn {
  constexpr std::tuple<OutArgs...> operator()(const OverloadFn &f,
                                              const InArg &inArg) {
    return f(inArg);
  }
};

// N - 1

template <typename Fn, typename OutArg, typename... InArgs> struct reducer {
  constexpr std::tuple<OutArg> operator()(const Fn &f) {
    constexpr auto ins = tupler<InArgs...>();
    return f(ins);
  }
};

// N - N
template <std::size_t... TupleIndices, typename RetTuple, typename OverloadedFn,
          typename Tuple1>
constexpr RetTuple mapperHelper(std::index_sequence<TupleIndices...>, Tuple1 t1,
                                const OverloadedFn &f) {
  return std::make_tuple(f(std::get<TupleIndices>(t1))...);
}

template <typename OverloadedFn, typename OutArgsList, typename InArgsList>
struct mapper {};

template <typename OverloadedFn, typename... OutArgs, typename... InArgs>
struct mapper<OverloadedFn, TypeList<OutArgs...>, TypeList<InArgs...>> {
  std::tuple<OutArgs...> operator()(const OverloadedFn &f, InArgs... inArgs) {
    constexpr auto Indices = std::make_index_sequence<sizeof...(InArgs)>{};
    constexpr auto intpl = std::make_tuple(inArgs...);
    return mapperHelper(Indices, intpl, f);
  }
};

// N - M

template <typename OverloadedFn, typename TypeLOut, typename TypeLIn>
struct TupleCall {};

template <typename OverloadedFn, typename... OutArgs, typename... InArgs>
struct TupleCall<OverloadedFn, TypeList<OutArgs...>, TypeList<InArgs...>> {
  constexpr std::tuple<OutArgs...> operator()(const OverloadedFn &f,
                                              InArgs... inArgs) {
    constexpr auto intpl = std::make_tuple(inArgs...);
    return f(intpl);
  }
};

template <std::size_t Min, std::size_t Max, typename Acc, typename El,
          typename... Els>
struct foldl {
  constexpr void operator()(const std::function<Acc(Acc, El)> &f, Acc &a,
                            Els... els) {
    if constexpr (Min < Max) {
      constexpr auto tpl = std::make_tuple(els...);
      a = f(a, std::get<Min>(tpl));
      foldl<Min + 1, Max, Acc, El, Els...>(f, a, els...);
    }
  }
};
template <typename Acc, typename Fn, typename... Els>
constexpr void foldfn(const Fn &f, Acc &a, Els... els) {
  constexpr auto es = std::make_tuple(els...);
  foldl<0, sizeof...(Els), Acc, decltype(std::get<0>(es)), Els...>(f, a,
                                                                   els...);
}

template <std::size_t... TupleIndices, typename Return, typename OverloadedFn,
          typename Tuple1, typename Tuple2>
constexpr std::array<Return, sizeof...(TupleIndices)>
mergeHelper(std::index_sequence<TupleIndices...>, Tuple1 t1, Tuple2 t2,
            const OverloadedFn &f) {
  return std::array<Return, sizeof...(TupleIndices)>{
      f(std::get<TupleIndices>(t1), std::get<TupleIndices>(t2))...};
}

template <typename OverloadedFn, typename RetList, typename ArgList1,
          typename ArgList2>
struct merger {};

template <typename OverloadedFn, typename... RetVals, typename... Args1,
          typename... Args2>
struct merger<OverloadedFn, TypeList<RetVals...>, TypeList<Args1...>,
              TypeList<Args2...>> {

  std::tuple<RetVals...> operator()(const OverloadedFn &fn, Args1... args1,
                                    Args2... args2) {
    //
    return std::make_tuple(fn((args1), (args2))...);
  }
};

} // namespace vtuto
