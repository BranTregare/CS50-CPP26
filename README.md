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

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

You may also build using `--config Debug` with multi-config generators.

> 🧱 **Note:** No IDE is required. This project is intended to build and run using just the command line. If you prefer to use an IDE, you're welcome to do so, but the code and structure assume only CMake + Ninja + Clang++ as the baseline.

## 🧪 Run Tests

```bash
./build/tests_credit
```

## 🧭 Project Philosophy

This is not a tutorial.

This is a **modern rethinking of CS50** through the lens of C++26. It emphasizes structure, type safety, and idiomatic expression — even in introductory problems — without sacrificing clarity.
