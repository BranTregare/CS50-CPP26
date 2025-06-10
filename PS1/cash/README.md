# PS1: Cash — Greedy Token Counter

This project reimagines the CS50 `cash.c` problem using modern **C++26** with **Clang++ 20.1.6** and **libc++**.  
It calculates the minimum number of discrete currency tokens needed to make change for a given amount — generalized for both **US** and **EU** currency systems.

---

## 🧠 Problem Summary

Given an amount of change (in cents or eurocents), determine the fewest number of **tokens** (coins and bills) needed to represent that amount using a greedy algorithm.

---

## ✨ Key Features

- Uses `std::array`, `std::span`, and `constexpr` for clarity and safety.
- Fully integer-based (`std::uint32_t`), no floating-point arithmetic.
- Includes both **US** and **EU** currency definitions.
- Tracks performance with a `StopWatch` utility.
- Output is semantic:
  ```
  Amount owed in lowest token value: 117
  US tokens used: 6
  EU tokens used: 4
  ```

---

## ⚙️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/cash
```

> ⚠️ Requires:
> - **Clang++ 20.1.6**
> - **libc++ 20.1.6**
> - **CMake 3.31.6**
> - `-stdlib=libc++` correctly configured

---

## 📎 Project Philosophy

Unlike the original CS50 `cash.c`, this implementation is not limited to coins, nor is it hardcoded to a single currency. It emphasizes correctness, clarity, and extensibility — without sacrificing simplicity.

See [`PHILOSOPHY.md`](../../../PHILOSOPHY.md) for more on the values behind this project.
