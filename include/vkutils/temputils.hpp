#pragma once
// template utility functions
#include <external.hpp>
namespace vtuto {

template <std::size_t Min, std::size_t Max>
struct static_loop {
  template <typename LoopResult, typename... InnerFuncArgs>
  void operator()(
      const std::function<void(std::size_t, LoopResult &,
                               InnerFuncArgs...)> &fn,
      LoopResult &result, InnerFuncArgs... args) const {
    if
      constexpr(Min < Max) {
        fn(Min, result, args...);
        static_loop<Min + 1, Max>()(fn, result, args...);
      }
  }
};
} // namespace vtuto
