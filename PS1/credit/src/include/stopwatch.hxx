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
  std::chrono::time_point<Clock> Last_;
  std::string Name_;

public:
  inline void reset() noexcept
  {
    Last_ = Clock::now();
  }
  inline auto operator()() const noexcept
  {  // returns time in Resolution
    auto Duration = std::chrono::duration<double, std::nano>(Clock::now() - Last_);
    return Duration;
  }
  StopWatch(const std::string Name = "unamed") noexcept : Name_(Name)
  {
    std::print("SW {}:\n", Name_);
    reset();
  }
  ~StopWatch()
  {
    // bad encapulation, you should reconsider this design!
    // e.g. take a std::function as argument to the constructor and call it here
    auto End_Time = (*this)();
    // std::println("SW ~ {0}: ns  :{1:20} ", name_, std::chrono::duration<double, std::nano>(endtime));
    std::println("SW ~ {0}: µs  :{1:10} ", Name_, std::chrono::duration<double, std::micro>(End_Time));
    std::println("SW ~ {0}: ms  :{1:10} ", Name_, std::chrono::duration<double, std::milli>(End_Time));
    std::println("SW ~ {0}: s   :{1:10} ", Name_, std::chrono::duration<double>(End_Time));
  }
};
#endif  // STOPWATCH_HXX