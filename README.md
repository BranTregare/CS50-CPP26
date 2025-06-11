# PS1: Credit — Card Validator

This project reimagines the CS50 `credit.c` problem using modern **C++26**, compiled with **Clang++ 20.1.6** and **libc++**.  
It validates credit card numbers using the **Luhn algorithm** and determines the card type (AMEX, MASTERCARD, VISA).

---

## 💡 Problem Statement

Given a credit card number, determine its validity and identify its issuer using a checksum algorithm (Luhn).  
If the number is invalid, output `INVALID`. Otherwise, return the card type.

---

## ✨ Key Features

- Uses `std::array`, `std::ranges`, and `constexpr` for type safety and clarity
- Validates input structure and performs checksum at compile time where possible
- Separates parsing, validation, and reporting logic into clear abstractions
- Includes static and runtime tests (via `static_assert` and Catch2)
- Clean, modular structure and header-only utilities

**Sample Output:**
```text
Number: 378282246310005
AMEX
```

---

## 🔍 What’s Different?

| Aspect              | Traditional CS50 `credit.c`         | This C++26 Version                          |
|---------------------|-------------------------------------|---------------------------------------------|
| Input handling      | `get_long()`                        | `std::string_view` + `constexpr` parsing    |
| Validation          | Loops and branching                 | `std::ranges`, `constexpr`, `enum class`    |
| Card type checking  | Manual branching                    | Pattern-matched via `constexpr` logic       |
| Memory usage        | Implicit                            | Stack-only, no dynamic allocation           |
| Error checking      | Implicit, runtime only              | Compile-time where possible (`static_assert`) |

---

## 📁 Layout

```
PS1/credit/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── PHILOSOPHY.md               # Design goals and rationale
├── TEACHING-INSTRUCTOR.md      # Instructor guidance
├── TEACHING-STUDENT.md         # Exploration hints for students
├── src/
│   ├── credit.cxx              # Main implementation
│   └── include/
│       └── stopwatch.hxx       # Stopwatch utility (optional)
└── test/
    ├── credit_test.cxx         # Compile-time tests (static_assert)
    ├── credit_runtime_test.cxx # Catch2 runtime unit tests
    └── include/
        └── test_util.hxx       # Shared test helpers
```

---

## ⚙️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## ⚠️ Requirements

> - **Clang++ 20.1.6**
> - **libc++ 20.1.6**
> - **CMake 3.31.6**
> - `-stdlib=libc++` must be correctly configured

---

## 🧪 Run Tests

```bash
./build/credit_test          # Static assertions (compile-time logic)
./build/credit_runtime_test  # Catch2 unit tests (runtime behavior)
```

Tests are split into:
- `credit_test`: Verifies compile-time logic (e.g., `constexpr` correctness)
- `credit_runtime_test`: Uses **Catch2** to validate behavior with runtime input

> Both are automatically built and require no extra configuration.

---

## 🔍 Things to Explore

- Why use `std::string_view` instead of `std::string`?
- What are the advantages of compile-time validation (`constexpr`, `static_assert`)?
- How does using `enum class` help clarify logic and avoid bugs?
- Could this be adapted to use a different algorithm (e.g., Verhoeff)?
- Why is `std::array` used instead of `std::vector`?

---

## 📎 Project Philosophy

This implementation demonstrates how clarity, correctness, and structure can be achieved even in early programming problems.  
It avoids legacy functions like `get_long()` and encourages strong types, modern abstractions, and compile-time safety.

See [`PHILOSOPHY.md`](./PHILOSOPHY.md) for broader design goals and the origin of this project’s approach.

---

> **Disclaimer**  
> This reimplementation is not affiliated with Harvard or CS50.  
> It is offered for learning and exploration, and should not be considered official course material.
