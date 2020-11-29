#include <iostream>

using namespace std;

template<typename F>
class Functor {
 public:
  F f;
  Functor(F f) : f(f) {}
  template<typename IN>
  decltype(auto) operator()(IN i) {
    return f(i);
  }
};

template <typename F1, typename F2>
class Composite {
 public:
  F1 f1;
  F2 f2;

  Composite(F1 f1, F2 f2) : f1(f1), f2(f2) {}

  template <typename IN>
  decltype(auto) operator()(IN i) {
    return f2(f1(i));
  }
};

template<typename F>
decltype(auto) compose(F f) {
  return Functor<F>(f);
}

template <typename F1, typename F2>
decltype(auto) compose(F1 f, F2 g) {
  return Composite<F1, F2>{f, g};
}

template <typename F1, typename... Fs>
decltype(auto) compose(F1 f, Fs... args) {
  return compose(f, compose(args...));
}
