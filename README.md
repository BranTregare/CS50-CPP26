# Credit Card Validator – CS50 Problem Set 1 (Modern C++26 Edition)

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

## 🔍 What’s Different from CS50’s C Version?

| Feature                  | CS50 (C)                           | This Project (C++26)                     |
|--------------------------|------------------------------------|------------------------------------------|
| Input Handling           | `get_long_long()` (black box)      | `std::getline` + manual validation       |
| Luhn Implementation      | Manual loops                       | Ranges, views, transforms                |
| Digit Access             | Division-based                     | `digit_sequence` (reversed for clarity)  |
| Type Safety              | Raw `long long`, `int`             | `std::uint64_t`, `std::uint8_t`, enums   |
| Card Type Output         | `printf` string                    | `enum class` + `std::println`            |
| Static Validation        | N/A                                | `constexpr`, `static_assert`             |
| Unit Testing             | Manual                             | Compile-time & Catch2 runtime tests      |

---

## 🧪 Testing

- **Compile-time:** Includes `static_assert` tests in `credit_test.cxx` to validate logic at build time.
- **Runtime:** Uses [Catch2 v3](https://github.com/catchorg/Catch2) for unit tests in `credit_runtime_test.cxx`.

---

## 📁 Project Layout

credit/
├── CMakeLists.txt
├── src/
│ ├── credit.cxx # Main implementation
│ └── include/
│ └── stopwatch.hxx # Lightweight stopwatch (optional utility)
├── test/
│ ├── credit_test.cxx # Compile-time static_assert tests
│ ├── credit_runtime_test.cxx# Catch2 runtime unit tests
│ └── include/
│ └── test_util.hxx # Shared test helpers


---

## ⚙️ Build Instructions

Requires:

- **Clang++ 20.1.6+**
- **CMake 3.31.6+**
- **Ninja (multi-target)**

```bash
mkdir build && cd build
cmake -G Ninja ..
ninja
ctest --verbose