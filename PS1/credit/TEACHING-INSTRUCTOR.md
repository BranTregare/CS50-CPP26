# TEACHING-INSTRUCTOR.md — PS1/credit

This implementation exists to model how modern C++26 can be applied with clarity and restraint — not just novelty. Students are encouraged to explore both the structure and the constraints of this example.

## Design Goals

* **Clarity**: The `DigitSequence` abstraction provides a safe, type-oriented approach to working with fixed-size card numbers.
* **Correctness**: Card validation logic is verified at compile time with `constexpr` and `static_assert`.
* **Modularity**: `enum class CardType`, clean separation of parsing, and validation steps.
* **Safety**: No dynamic allocation, no raw pointers, and constant-size stack-only data structures.
* **Testing**: Built-in test coverage via static assertions and runtime tests.

## Suggested Teaching Directions

* Ask students to re-implement `luhn_valid()` using classic loops, then compare to the provided version using ranges.
* Encourage breaking the logic: Can they make it accept an invalid card?
* Can they modify the digit parser to use dynamic input? What tradeoffs arise?
* Discuss how `std::string_view` and `std::array` enable correctness by construction.
* Show how `StopWatch` models local benchmarking — but also how micro-benchmarks can mislead.
* **Explore modern iteration**:
  In `vectorise_number()`, `std::views::iota` replaces the classic loop counter. This expresses the same idea — “from 0 to N” — but with fewer moving parts. There’s no separate initializer, boundary check, or increment; instead, the iteration range is declared explicitly.

  Students familiar with:

  ```cpp
  for (int i = 0; i < 10; ++i) { ... }
  ```

  can now see a modern alternative:

  ```cpp
  for (int i : std::views::iota(0, 10)) { ... }
  ```

  While it’s not shorter in text, it is *tighter in meaning*. It can help avoid subtle bugs like off-by-one errors, and it becomes especially powerful when composed with other transformations like `.filter()` or `.transform()`.

  This isn’t required knowledge — but it gives curious students a safe, modern stepping stone into the world of ranges.

## Discussion Prompt

Why do we reverse the digit sequence? Where should parsing responsibility live?

What assumptions about input structure are safe to encode at compile time?

---

> 📎 See the top-level [`PHILOSOPHY.md`](../../../PHILOSOPHY.md) for project-wide design values and the origin of this project’s engineering mindset. It includes a reflection on optimization, performance, and clarity — especially:
>
> “In 1994, we optimized because we were desperate,
> our compilers weren’t up to it,
> and it was the right thing to do.
>
> Today, we should optimize because it is right.”
