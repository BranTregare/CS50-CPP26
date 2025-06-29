#ifndef STOPWATCH_HXX
#define STOPWATCH_HXX
// module;
#include <chrono>
#include <print>
#include <ratio>
#include <string>

// export module StopWatch;

// export
template <typename Resolution = std::chrono::duration<double, std::nano>>
class StopWatch
{
  using Clock = std::chrono::high_resolution_clock;

private:
  std::chrono::time_point<Clock> last;
  std::string name_;

public:
  inline void reset() noexcept
  {
    last = Clock::now();
  }
  inline auto operator()() const noexcept
  {  // returns time in Resolution
    auto duration = std::chrono::duration<double, std::nano>(Clock::now() - last);
    return duration;
  }
  StopWatch(const std::string name = "unamed") noexcept : name_(name)
  {
    std::print("SW {}:\n", name_);
    reset();
  }
  ~StopWatch()
  {
    // bad encapulation, you should reconsider this design!
    // e.g. take a std::function as argument to the constructor and call it here
    auto endtime = (*this)();
    // std::println("SW ~ {0}: ns  :{1:20} ", name_, std::chrono::duration<double, std::nano>(endtime));
    std::println("SW ~ {0}: µs  :{1:10} ", name_, std::chrono::duration<double, std::micro>(endtime));
    std::println("SW ~ {0}: ms  :{1:10} ", name_, std::chrono::duration<double, std::milli>(endtime));
    std::println("SW ~ {0}: s   :{1:10} ", name_, std::chrono::duration<double>(endtime));
  }
};
#endif  // STOPWATCH_HXX