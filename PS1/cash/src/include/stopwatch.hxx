#ifndef STOPWATCH_HXX
#define STOPWATCH_HXX

#include <chrono>
#include <print>
#include <ratio>
#include <string>

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
  {
    auto Duration = std::chrono::duration<double, std::nano>(Clock::now() - Last_);
    return Duration;
  }

  StopWatch(const std::string Name = "anonymous timer") noexcept : Name_(Name)
  {
    std::println("SW  {}:\n>", Name_);
    reset();
  }

  ~StopWatch()
  {
    // Bad encapsulation: you should reconsider this design!
    // e.g., take a std::function as argument to the constructor and call it here
    auto End_Time = (*this)();
    std::print("<\n~SW");
    std::println(" {0}: ns: {1:10}", Name_, End_Time);
    std::println("    {0}: µs: {1:10}", Name_, std::chrono::duration<double, std::micro>(End_Time));
    std::println("    {0}: ms: {1:10}", Name_, std::chrono::duration<double, std::milli>(End_Time));
    std::println("    {0}: s : {1:10}\n", Name_, std::chrono::duration<double>(End_Time));
  }
};

#endif  // STOPWATCH_HXX