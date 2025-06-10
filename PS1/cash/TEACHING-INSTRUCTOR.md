# TEACHING-INSTRUCTOR.md — PS1/cash

This exercise reimagines CS50’s `cash.c` for a modern C++26 context. While the original version uses floating point input and fixed US coins, this version generalizes the problem into **integer-based greedy change-making**, applicable to any currency system.

This instructor guide outlines the design decisions, teaching opportunities, and areas of exploration for more advanced learners.

---

## 🎯 Learning Objectives

- Introduce `std::array`, `std::span`, and `constexpr` through a practical, intuitive domain (money).
- Model safe and idiomatic integer arithmetic using `std::uint32_t` instead of float.
- Demonstrate greedy algorithms in the real world — and their limits.
- Encourage curiosity with built-in EU currency support and token generalization.

---

## 🧱 Design Notes

- `Default_US_Currency` and `Default_EU_Currency` are `constexpr std::array<CurrencyUnit>`, where `CurrencyUnit` is a typedef to `std::uint32_t`.
- Input is done in the smallest unit (cents/eurocents) to avoid rounding.
- `calculate_tokens()` takes a `std::span<const CurrencyUnit>`, illustrating safe range-based design.
- `StopWatch` is used to model simple benchmarking.
- No dynamic memory, no exceptions, no complex branching.

---

## 📎 Teaching Prompts

- What is the minimum type that ensures safe calculation of coin change?
- Why was `std::span` chosen over raw arrays or `std::vector`?
- Is greedy always optimal? Try `[4, 3, 1]` as a custom currency.
- What are the tradeoffs in adding bills vs. keeping coins only?
- Could we adapt the function for floating-point input? Should we?

---

## 💡 Extensions for Advanced Students

- Replace `std::array` with runtime-defined `std::vector` currency input.
- Allow the user to specify denominations interactively.
- Track not just the count, but *which* tokens are used.
- Benchmark different token orders or compare greedy to dynamic programming.

---

## 🧭 Scope Boundaries

Avoid overloading `cash` with concepts not yet introduced at this stage of CS50:

- No templates, metaprogramming, or polymorphism  
- No formatted parsing or exception handling  
- No currency formatting or localization  

Instead, focus on:
- `std::span`, `array`, `print`, and simple benchmarking  
- Clear, type-safe greedy logic  
- Subtle correctness (integer vs float)

---

## 🔍 Note on Currency and Floating-Point

CS50’s original `cash.c` uses `get_float` for input and rounding of float values. This project deliberately avoids that approach. Currency should **never** be modeled with float or double due to:

- Inherent rounding imprecision  
- Unsafe comparison semantics (e.g., using epsilon thresholds)  
- Dangerous coercions between `float` and `double` that may amplify errors  
- Domain mismatches: money is discrete, not continuous

This version models amounts in the smallest unit (cents/eurocents), using `std::uint32_t`. This is clearer, safer, and aligned with real-world engineering.

---

> 📎 See [`PHILOSOPHY.md`](./PHILOSOPHY.md) for the broader values and justification behind the design of this project.
