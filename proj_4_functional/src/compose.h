#include <functional>
#include <iostream>

using namespace std;
 
template<typename Type, typename... Fargs>
auto compose() {
  return [](auto&& x) -> decltype(auto) { return forward<Type(Fargs...)>(x); };
}

template <typename R, typename T>
auto compose(function<R(T)> fn) {
  return [=](auto&&... x) -> decltype(auto) {
    return fn(forward<decltype(x)>(x)...);
  };
}

template <typename Fn, typename R, typename... Fargs>
auto compose(function<Fn(R)> fn, Fargs... args) {
  return [=](auto&&... x) -> decltype(auto) {
    return fn(compose(args...)(forward<decltype(x)>(x)...));
  };
}