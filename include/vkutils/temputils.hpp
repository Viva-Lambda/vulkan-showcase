#pragma once
// template utility functions
#include <external.hpp>
namespace vtuto {

template <std::size_t Min, std::size_t Max> struct foldl {
  template <typename LoopResult, typename... InnerFuncArgs>
  void operator()(const std::function<void(
                      LoopResult &, InnerFuncArgs...)> &fn,
                  LoopResult &result,
                  InnerFuncArgs... args) const {
    if
      constexpr(Min < Max) {
        fn(result, args...);
        static_loop<Min + 1, Max>()(fn, result, args...);
      }
  }
};

template <typename... Ts> struct param_counter {
  static std::size_t N = sizeof...(Ts);
};

template <typename Return, typename... Args> struct op {
  Return operator(const std::function<Return(Args...)> &f,
                  Args... args) {
    return f(args...);
  }
};

template <typename... Args> struct tupler {
  static std::tuple<Args...> tpl;
};

template <std::size_t Min, std::size_t Max, typename Acc,
          typename El, tupler<typename... Els> tpl>
struct foldl {
  void operator(const std::function<Acc(Acc, El)> &f,
                Acc &a) {
    if
      constexpr(Min < Max) {
        a = f(a, std::get<Min>(tpl::tpl));
        foldl<Min + 1, Max, Acc, El, tpl>(f, a);
      }
  }
};
template <typename Acc, tupler<typename... Els> tpl>
struct foldl {
  void operator(const std::function<Acc(Acc, El)> &f,
                Acc &a) {
    auto el = decltype(std::get<0>(tpl::tpl));
    foldl<0, sizeof...(Els), Acc, el, tpl>(f, a);
  }
}
};

template <std::size_t... TupleIndices, typename Return,
          typename OverloadedFn, typename Tuple1,
          typename Tuple2>
constexpr std::array<Return, sizeof...(TupleIndices)>
mergeHelper(std::index_sequence<TupleIndices...>, Tuple1 t1,
            Tuple2 t2, const OverloadedFn &f) {
  return std::array<Return, sizeof...(TupleIndices)>{
      f(std::get<TupleIndices>(t1),
        std::get<TupleIndices>(t2))...};
}

template <tupler<typename... Args1> t1,
          tupler<typename... Args2> t2>
struct merger {

  template <typename Return, typename A1, typename A2>
  std::array<Return,
             std::min<std::size_t>(sizeof...(Args1),
                                   sizeof...(Args2))>
  operator(const std::function<Return(A1, A2)> &f) {
    constexpr std::size_t Min = std::min<std::size_t>(
        sizeof...(Args1), sizeof...(Args2));

    constexpr auto Indices =
        std::make_index_sequence<Min>{};
    return mergeHelper(Indices, t1::tpl, t2::tpl, f)
  }
};

template <std::size_t... TupleIndices,
          typename RetTuple typename OverloadedFn,
          typename Tuple1>
constexpr RetTuple
mapHelper(std::index_sequence<TupleIndices...>, Tuple1 t1,
          const OverloadedFn &f) {
  return std::make_tuple(f(std::get<TupleIndices>(t1))...);
}

template <tupler<typename... Args> t> struct mapper {
  template <typename OverloadedFn, typename... Return>
  std::tuple<Return...> operator(const OverloadedFn &f) {
    constexpr auto Indices =
        std::make_index_sequence<sizeof...(Args)>{};
    return mapHelper(Indices, t::tpl, f);
  }
};

} // namespace vtuto
