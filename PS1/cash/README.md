# PS1: Cash — Greedy Token Counter

This project reimagines the CS50 `cash.c` problem using modern **C++26**, compiled with **Clang++ 20.1.6** and **libc++**. It calculates the minimum number of discrete currency tokens needed to make change for a given amount — generalized for both **US** and **EU** currency systems.

---

## 💡 Problem Statement

Given an amount of change (in cents or eurocents), determine the fewest number of **tokens** (coins and bills) needed to represent that amount using a greedy algorithm.

The user is prompted to enter the amount owed, expressed in the lowest currency unit (e.g., cents). The program outputs the number of tokens required under two currency systems.

---

## ✨ Key Features

- Fully integer-based (`std::uint32_t`); no floating-point arithmetic  
- Uses `std::array`, `std::span`, and `constexpr` for clarity and safety  
- Supports both **US** and **EU** currency sets  
- Generalized greedy algorithm using reusable logic  
- Built-in timing via a `StopWatch` utility  

**Sample Output:**
```
Amount owed in lowest token value: 117  
US tokens used: 6  
EU tokens used: 4
```

---

## 🔍 What’s Different?

| Aspect          | Traditional CS50 `cash.c`       | This C++26 Version                        |
|----------------|----------------------------------|-------------------------------------------|
| Input handling | `get_float()` + rounding         | `std::cin` with `std::uint32_t`           |
| Currency set   | US coins only                    | US and EU coins and bills                 |
| Algorithm      | Hardcoded greedy logic           | Generalized using `std::span` and `array` |
| Data structures| Primitives                       | `std::array`, `std::span`, `constexpr`    |
| Validation     | Float-based, potential rounding  | Fully integer-safe, no rounding required  |

---

## 📁 Layout

```
PS1/cash/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── PHILOSOPHY.md               # Design goals and rationale
├── TEACHING-INSTRUCTOR.md      # Instructor guidance
├── TEACHING-STUDENT.md         # Exploration hints for students
├── src/
│   └── cash.cxx                # Main logic: token calculation
└── src/include/
    └── stopwatch.hxx           # Stopwatch utility (header-only)
```

---

## ⚙️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/cash
```

---

## ⚠️ Requirements

> - **Clang++ 20.1.6**  
> - **libc++ 20.1.6**  
> - **CMake 3.31.6**  
> - Proper `-stdlib=libc++` configuration

---

## 🔍 Things to Explore

- Why avoid floating-point math for currency?  
- What are the advantages of `std::span` over raw pointers?  
- Could this support other currencies by passing different denominations?  
- How would adding runtime configuration affect the implementation?

---

## 📌 Project Philosophy

This implementation is not limited to coins, nor is it hardcoded to a single currency. It aims to demonstrate how clarity, correctness, and extensibility can coexist — even in an introductory-level problem.

See [`PHILOSOPHY.md`](./PHILOSOPHY.md) for a broader look at the design values behind this project.

---

> **Disclaimer**  
> This reimplementation is not affiliated with Harvard or CS50.  
> It is offered for learning and exploration, and should not be considered official course material.