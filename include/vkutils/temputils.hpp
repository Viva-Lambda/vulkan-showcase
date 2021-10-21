#pragma once
// template utility functions
#include <external.hpp>
namespace vtuto {

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
struct merger<OverloadedFn, TypeList<RetVals...>, TypeList<Args1...>, TypeList<Args2...>> {

  std::tuple<RetVals...> operator()(const OverloadedFn &fn, Args1... args1,
                                    Args2... args2) {
    //
    return std::make_tuple(fn((args1), (args2))...);
  }
};

} // namespace vtuto
