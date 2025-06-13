<<<<<<< HEAD
# CS50 in C++26 — A Modern Approach

This repository reimplements selected CS50 problem sets using **C++26**, compiled with **Clang 20.1.6** and **libc++ 20.1.6**.

## ⚠️ Requirements

A **bleeding-edge toolchain** is required:

- **CMake 3.31.6**
- **Clang++ 20.1.6**
- **libc++ 20.1.6**
- Support for most **C++26** language features

> ⚠️ _Clang++ 20.1.6 is not fully standard-compliant with C++26 — no compiler is at this time. However, it is currently the most portable and practical choice.  
> Older or partially-supported environments will not build this code correctly._

## 🎯 Goals

- Showcase **idiomatic, modern C++** in the spirit of CS50
- Emphasize **clarity, correctness, and safety**, using:
  - `std::string_view`, `std::ranges`, `constexpr`, and more
- Avoid spoon-feeding — encourage **exploration and curiosity**

## ✅ Completed Problem Sets

- **PS1/cash** — Complete and public  
  See [`PS1/cash/README.md`](./PS1/cash/README.md) for full details.

- **PS1/credit** — Complete and public  
  See [`PS1/credit/README.md`](./PS1/credit/README.md) for full details.

> Other problem sets will be published when they are ready.

## 🛠️ Build Instructions
=======
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
>>>>>>> private-repo/main

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

<<<<<<< HEAD
You may also build using `--config Debug` with multi-config generators.

> 🧱 **Note:** No IDE is required. This project is intended to build and run using just the command line. If you prefer to use an IDE, you're welcome to do so, but the code and structure assume only CMake + Ninja + Clang++ as the baseline.
=======
---

## ⚠️ Requirements

> - **Clang++ 20.1.6**
> - **libc++ 20.1.6**
> - **CMake 3.31.6**
> - `-stdlib=libc++` must be correctly configured

---
>>>>>>> private-repo/main

## 🧪 Run Tests

```bash
<<<<<<< HEAD
./build/tests_credit
```

## 🧭 Project Philosophy

This is not a tutorial.

This is a **modern rethinking of CS50** through the lens of C++26. It emphasizes structure, type safety, and idiomatic expression — even in introductory problems — without sacrificing clarity.
=======
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
>>>>>>> private-repo/main
