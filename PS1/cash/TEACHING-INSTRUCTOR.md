# TEACHING-INSTRUCTOR.md — PS1/cash

> This is one of those deceptively simple problems that invites rich insight. There's elegance in the exercise — and just enough depth to challenge without overwhelming.

This project builds on CS50’s `cash.c`, which introduces a greedy algorithm for computing the minimum number of coins needed for change. This C++ version modernizes that logic by using containers like `std::array`, `std::span`, and `constexpr` constructs to enforce clarity, safety, and generality.

At the PS1 level, students are only just learning to reason algorithmically. But they’ve seen loops, conditions, basic types — and they’ve already been introduced to memory errors in C. This version gives them a taste of correctness without losing the simplicity of the original.

---

## Teaching Emphases

### ✅ Simplicity and Generalization

* The logic in `calculate_tokens` is not hard — it’s a single loop with a division and a modulus.
* But it’s generalized: not tied to one currency, not tied to coins, and not hardcoded.
* `std::span` keeps the logic generic and flexible, showing a design that scales without extra complexity.

Encourage students to notice that the exact same code works for US, EU, or arbitrary currencies. That’s the core insight here: parameterization leads to reuse.

### 💡 Greedy Algorithm Pitfalls

This is the moment to introduce failure modes of greedy algorithms.

**\[4, 3, 1]** is the classic example:

* Try making change for `6`.
* Greedy chooses `4 + 1 + 1` (3 tokens).
* Optimal is `3 + 3` (2 tokens).

This is where students can see the difference between locally optimal and globally optimal strategies.

**\[2, 3, 4]** is a red herring — but useful:

* Greedy *always* fails here.
* Why? Because it chooses `2` for everything, even when larger tokens are better.
* Example: for `6`, greedy uses `2 + 2 + 2`, while optimal is `3 + 3` or `4 + 2`.

The trick: `2` is the smallest even token and divides everything. Greedy never even *looks* at the others.

This highlights a subtle teaching point: **order matters**, and **greedy assumes earlier is better**.

Letting students trip on this early gives you an opportunity to talk about algorithmic assumptions without going over their heads. It doesn’t need theory yet — just experimentation.

**\[7, 3, 1]** is a great teaching contrast:

* Greedy always works here.
* Why? Because the denominations are **prime**.
* No smaller token can be combined to make a larger one.

This makes it a useful way to explore what makes some greedy algorithms robust. It's also good practice in pattern recognition: students can test cases and start asking *why* it never fails.

---

### 🔍 Note on Currency and Floating-Point

The original CS50 `cash.c` uses `get_float` and then rounds to an integer number of cents. This modern C++ version bypasses that entirely — and deliberately so.

Currency should almost never be modeled using floating-point types. The reasons are technical and pedagogical:

* Decimal rounding is inherently imprecise in binary floating-point
* Comparisons require epsilon thresholds, which students aren't ready to reason about
* Mixing `float` and `double` silently introduces new errors
* Most importantly: **money is discrete**, and floating-point suggests continuity

By modeling amounts in the smallest unit (e.g. cents or eurocents) as `std::uint32_t`, we make correctness the default. There's no rounding bug to debug. No epsilon to fudge. Just a clear, deterministic representation.

Let students take this for granted at first. But it helps to understand *why* this matters — and to surface that understanding when the opportunity arises. This isn’t just a coding trick; it’s an important modeling choice that avoids a class of subtle and pervasive bugs.

---

## Discussion Starters

* Why is this integer-only? What problems do floats introduce?
* Why are `std::array` and `std::span` better than raw C arrays or `std::vector`?
* What would it take to report *which* tokens were used?
* Could you implement the same logic in pure C?
* What if the currency data came from the user at runtime?

---

## Teaching Strategy

Let students experiment with alternative currencies. Let them discover the failure modes. Don’t spoon-feed the theory — let the code surface the questions:

* “Why didn’t it choose `3 + 3`?”
* “Why does greedy fail here?”
* “How would I fix that?”

Let those questions guide the learning.

The goal here isn’t just to write a token counter. It’s to build an intuition for:

* When algorithms work
* When they don’t
* And what design tradeoffs are worth making

In short: teach the *thinking*, not just the *code*.
