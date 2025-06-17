# Teaching Notes – Student Edition

This project is a modern C++26 reimagining of the classic CS50 "Credit" problem.

You already know the original version — it asks you to validate a credit card number using the Luhn algorithm and identify its type (AMEX, MASTERCARD, or VISA).

Now you're doing the same thing using **modern C++ features** like `constexpr`, ranges, and strong typing — not to overcomplicate the problem, but to **explore good design in a familiar space**.

---

## 🧠 Goals

* Show that **even small programs deserve good engineering**
* Introduce students to **safe, modular, and modern C++**
* Encourage curiosity and experimentation with modern tools

---

## ✅ What This Project Demonstrates

* That C++ can be **expressive and safe**, even in small beginner-friendly domains
* That **compile-time programming** can enforce correctness and constraints
* That testing, modularity, and **type safety** scale even to small exercises
* That writing clear, modern C++ code doesn’t require a large or complex problem
* That range-based loops like `for (auto i : std::views::iota(0, N))` express intent clearly — even if you're more familiar with `for (int i = 0; i < N; ++i)`

---

## 🛑 What This Project Avoids

* No CI, no build complexity — keep it approachable
* No C++ modules (not yet fully supported in compilers)
* No real-world card validation (this is **didactic**, not secure)

---

## 🔍 CS50 C Version vs. This Modern C++26 Version

| Feature             | CS50 (C Version)        | This Project (Modern C++26)           |
| ------------------- | ----------------------- | ------------------------------------- |
| Input Handling      | `get_long_long()`       | `std::getline` + manual validation    |
| Luhn Implementation | Manual loops            | Ranges, views, transforms             |
| Digit Access        | Division-based indexing | `DigitSequence` (reversed, type-safe) |
| Type Safety         | `long long`, `int`      | `uint64_t`, `uint8_t`, `enum class`   |
| Card Type Output    | `printf` string literal | `enum class` + `std::println`         |
| Static Validation   | None                    | `constexpr`, `static_assert`          |
| Unit Testing        | Manual or none          | Compile-time + Catch2 runtime tests   |

---

## ✨ Tips for Exploring

* Try changing the validation logic — what fails?
* Add support for Discover or other card types
* Uncomment static tests in `credit_test.cxx` and watch what the compiler does
* Experiment with the `stopwatch.hxx` utility — it’s safe to ignore or explore!
* Spot the modern loop in `vectorise_number()` — it uses a range view instead of a classic `for (int i = …)` syntax. Look up `std::views::iota` if you're curious!

You're not just solving a problem — you're learning to design, validate, and express intent clearly. That’s the heart of modern C++.

Enjoy the journey!
