#include <functional>
#include <iostream>

#include "compose.h"

using namespace std;

int main() {
  const std::function<bool(int)> a = [](int x) -> bool {
    std::cout << "a" << std::endl;
    return x > 10;
  };

  const std::function<int(float)> b = [](float y) -> int {
    std::cout << "b" << std::endl;
    return int(y * y);
  };

  const std::function<float(bool)> c = [](bool z) -> float {
    std::cout << "c" << std::endl;
    return z ? 3.1f : 3.34f;
  };

  {
    auto d = compose(a, b, c, a, b, c);
    std::cout << d(true) << std::endl;
  }

  {
    auto res = compose(a);
    auto true_res = a(11);
    if (res(11) != true_res) {
      cout << "Test 1 Failed" << endl;
    }
  }

  {
    auto res = compose(a, b);
    auto true_res = a(b(11));
    if (res(11) != true_res) {
      cout << "Test 2 Failed" << endl;
    }
  }

  {
    auto res = compose(a, b, c, a, b, c);
    auto true_res = a(b(c(a(b(c(true))))));
    if (res(true) != true_res) {
      cout << "Test 3 Failed" << endl;
    }
  }

  return 0;
}