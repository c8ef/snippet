#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

template <typename Callable, typename... Args>
decltype(auto) timed_invoke(Callable&& callable, Args&&... args) {
  struct timer {
    timer() { t0 = high_resolution_clock::now(); }
    ~timer() {
      auto t1 = high_resolution_clock::now();
      cout << (t1 - t0).count() << " nanoseconds." << endl;
    }
    time_point<high_resolution_clock> t0;
  } t;
  return std::invoke(callable, std::forward<Args>(args)...);
}

void test() {
  for (int i = 0; i < 200000000; ++i) hash<int>{}(i);
}

int main() { timed_invoke(test); }