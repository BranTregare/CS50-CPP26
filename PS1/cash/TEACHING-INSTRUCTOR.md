# TEACHING-INSTRUCTOR.md — PS1/cash

> Design choices are never just technical — they reflect clarity of purpose and trust in the student.

---

This implementation reimagines CS50's `cash.c` in modern C++26, preserving the problem's spirit while emphasizing type safety, generality, and design intent. It’s deliberately simple on the surface — but pedagogically rich under the hood.

## 💰 Why Integer-Only?

The original `cash.c` uses `get_float()` and rounds to the nearest cent. But floating-point arithmetic is a dangerous abstraction for currency:

- Rounding is unpredictable.
- Float-to-double coercion can introduce hidden bugs.
- Equality comparisons require epsilon thresholds — a concept far beyond week 1.
- Currency is discrete. Float is continuous. The model is wrong.

This project uses `std::uint32_t` to model **discrete, countable currency units** (e.g., cents or eurocents). It’s cleaner, safer, and reinforces a better mental model.

## 📦 Why `std::span`?

Passing a token array by value is wasteful. Passing a pointer and size is verbose. `std::span` gives us:

- A safe, read-only view into the token array
- No copying or allocations
- Compile-time sizing when passed a `std::array`

This allows a **single** function (`calculate_tokens`) to work for both US and EU tokens — and any user-defined currency.

## 🔢 Token Count Type: `std::uint16_t`

The number of tokens returned by the greedy algorithm is tracked using `std::uint16_t`. This might seem oddly specific, but it reflects careful domain modeling:

- Token counts are never negative → use unsigned types
- `uint8_t` (max 255) is too small for pathological cases
- `uint16_t` allows up to 65,535 tokens, more than enough for real-world inputs

### 📚 Historical Context

During the Weimar Republic hyperinflation (1921–1923), prices soared to billions of marks for basic goods. In such extreme conditions, you could imagine needing **thousands of tokens** just to make change. A `std::uint8_t` would **overflow silently** in such a system. `std::uint16_t` is therefore a prudent, forward-compatible choice — and a teachable moment in **domain-aware type selection**.

## 🧪 Pathological Token Sets

Greedy algorithms do not always yield the optimal result. Two excellent examples to demonstrate this:

- `[4, 3, 1]`: Try making change for 6. Greedy gives `4 + 1 + 1` (3 tokens), but the optimal solution is `3 + 3` (2 tokens).
- `[2, 3, 4]`: Greedy always starts with `2`, meaning it will never consider `3` or `4` — and will always produce even-numbered outputs. This token set **cannot** represent all possible totals effectively.

These examples are powerful tools for teaching **algorithmic limitations** and the value of counterexamples.

Contrast with:

- `[7, 3, 1]`: A token set made of **primes**. Greedy always produces a result — and it’s always optimal for small inputs. Students may be surprised at how robust it is, despite its oddity.

These sets are also useful for discussions on **greedy optimality conditions**, **coin change problems**, and the importance of **counterexamples** in pedagogy.

## 🧵 Design Reflections

- The code generalizes the greedy algorithm to any currency.
- It uses **range-based programming idioms** (`std::array`, `std::span`) to avoid index manipulation.
- It is deliberately minimal: no dynamic memory, no unnecessary abstractions.
- The `StopWatch` utility is present but unobtrusive.

This is a good example of how C++26 can be **approachable and expressive** — without reaching for advanced metaprogramming or concepts.

## 🧭 What to Emphasize When Teaching

- Encourage reasoning about data types: Why `uint32_t` for money? Why `uint16_t` for counts?
- Point out the dangers of `float` for anything financial.
- Reinforce generalization: the algorithm isn't hardcoded to any one currency.
- Ask students to **experiment** with pathological token sets (e.g., `[4, 3, 1]`, `[2, 3, 4]`) and observe where greedy fails.
- Challenge advanced students to implement optimal algorithms (e.g., dynamic programming) — and compare results.

This project is a solid opportunity to contrast **correctness vs convenience**, and to introduce the idea that every type in C++ carries *intent* — or should.
