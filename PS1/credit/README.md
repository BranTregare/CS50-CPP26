# Credit Card Validator – CS50 Problem Set 1 (Modern C++26 Edition)

> Part of the [`CS50-CPP26`](https://github.com/BranTregare/CS50-CPP26) repository.

This is a modern C++26 implementation of the **CS50 Problem Set 1: Credit** problem. It validates credit card numbers using the Luhn algorithm and identifies card types (AMEX, MASTERCARD, VISA).

---

## ✨ Purpose

This project is a **didactic and demonstrator implementation**, intended to:

- Showcase how to implement classic CS50 problems in **modern idiomatic C++ (C++26)**.
- Teach **safe input handling**, **compile-time validation**, and **clean modular design**.
- Replace opaque library functions like `get_long_long()` with **transparent, real-world alternatives**.
- Demonstrate **`constexpr`-based logic**, **ranges/views**, and **strong typing**.
- Encourage understanding of the **Luhn algorithm** by reversing the digit sequence for clarity.

---

## 📁 Project Layout

```
credit/
├── CMakeLists.txt
├── src/
│   ├── credit.cxx              # Main implementation
│   └── include/
│       └── stopwatch.hxx       # Lightweight stopwatch (not used — explore it!)
├── test/
│   ├── credit_test.cxx         # Compile-time static_assert tests (no Catch2)
│   ├── credit_runtime_test.cxx # Catch2 runtime unit tests
│   └── include/
│       └── test_util.hxx       # Shared test helpers
```

> 📌 The included `stopwatch.hxx` is a minimal high-resolution timer utility.  
> It is **not used** in the code, by design — curious students are encouraged to explore and integrate it.

---

## 🔍 What’s Different from CS50’s C Version?

| Feature                  | CS50 (C)                           | This Project (C++26)                     |
|--------------------------|------------------------------------|------------------------------------------|
| Input Handling           | `get_long_long()` (black box)      | `std::getline` + manual validation       |
| Luhn Implementation      | Manual loops                       | Ranges, views, transforms                |
| Digit Access             | Division-based                     | `DigitSequence` (reversed for clarity)   |
| Type Safety              | Raw `long long`, `int`             | `std::uint64_t`, `std::uint8_t`, enums   |
| Card Type Output         | `printf` string                    | `enum class` + `std::println`            |
| Static Validation        | N/A                                | `constexpr`, `static_assert`             |
| Unit Testing             | Manual                             | Compile-time & Catch2 runtime tests      |

---

## ⚠️ Disclaimer

This code is provided for **educational and demonstrative purposes only**.  
It is **not production-ready** and is shared *as-is*, **without warranty or guarantee**.

---

## 🧪 Testing

- **Compile-time:** Includes `static_assert` tests in `credit_test.cxx` to validate logic at build time.
- **Runtime:** Uses [Catch2 v3](https://github.com/catchorg/Catch2) for unit tests in `credit_runtime_test.cxx`.

### Compile-Time Failure as a Feature

In this project, some invalid credit card inputs — such as numbers with more than 16 digits — are caught **at compile time**, not at runtime.

Example: Overlong Input (Compile-Time Error)

This test is intentionally commented out in `credit_test.cxx`:

```cpp
// static_assert(test_validate(41111111111111112222ULL) == CardType::INVALID, "Too long");
```

Why Does This Fail?

Internally, credit card digits are stored in a fixed-size buffer:

```cpp
std::array<std::uint8_t, 16> digits_;
```

When `vectorise_number()` is called on a number with more than 16 digits, it attempts to write past the end of this buffer. Because the logic is marked `constexpr`, this triggers a compile-time failure.

What’s the Benefit?

- No runtime length checks are needed inside the Luhn logic.
- The compiler enforces structural constraints before the program is built.
- It demonstrates a fail-fast and safe-by-construction design.

---

## ⚙️ Build Instructions

**Requirements:**

- `clang++` 20.1.6+
- `libc++` 20.1.6+
- `cmake` 3.31.6+
- `ninja` (multi-config capable)

### Build and Run (Debug by Default)

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build --build-config Debug --verbose
```

> 🐛 `Debug` is the default build type, with full symbols and zero optimization.
>
> 🚀 A `Release` build is available and enabled:
>
> ```bash
> cmake --build build --config Release
> ctest --test-dir build --build-config Release --verbose
> ```

---

## 🧭 Things to Explore

If you're new to modern C++, this project contains ideas and tools worth exploring further:

- `std::string_view` — for safe, efficient string slicing
- `std::ranges::views` — declarative data transformations
- `constexpr` logic and `static_assert` — compile-time correctness
- Minimal `enum class`-based API design
- CMake multi-config with Ninja (Debug vs Release)
- Adding `stopwatch.hxx` to measure runtime performance

---
