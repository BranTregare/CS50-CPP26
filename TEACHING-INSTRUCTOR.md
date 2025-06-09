# TEACHING-INSTRUCTOR.md — PS1/credit

This implementation exists to model how modern C++26 can be applied with clarity and restraint — not just novelty. Students are encouraged to explore both the structure and the constraints of this example.

## Design Goals

- **Clarity**: The `DigitSequence` abstraction provides a safe, type-oriented approach to working with fixed-size card numbers.
- **Correctness**: Card validation logic is verified at compile time with `constexpr` and `static_assert`.
- **Modularity**: `enum class CardType`, clean separation of parsing, and validation steps.
- **Safety**: No dynamic allocation, no raw pointers, and constant-size stack-only data structures.
- **Testing**: Built-in test coverage via static assertions and runtime tests.

## Suggested Teaching Directions

- Ask students to re-implement `luhn_valid()` using loops, then ranges.
- Encourage breaking the logic: Can they make it accept an invalid card?
- Can they modify the digit parser to use dynamic input? What tradeoffs arise?
- Discuss how `std::string_view` and `std::array` enable correctness by construction.
- Show how `StopWatch` models local benchmarking — but also how micro-benchmarks can mislead.

## Discussion Prompt

Why do we reverse the digit sequence? Where should parsing responsibility live?

What assumptions about input structure are safe to encode at compile time?

---

> 📎 See the top-level [`PHILOSOPHY.md`](../../../PHILOSOPHY.md) for project-wide design values and the origin of this project’s engineering mindset. It includes a reflection on optimization, performance, and clarity — especially:
>
> “In 1994, we optimized because we were desperate.  
> Today, we should optimize because it’s right.”
