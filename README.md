# CS50 in C++26 — A Modern Approach

This repository reimplements selected CS50 problem sets using **C++26**, compiled with **Clang 20.1.6** and **libc++ 20.1.6**.

## ⚠️ Requirements

A **bleeding-edge toolchain** is required:

* **CMake 3.31.6**
* **Clang++ 20.1.6**
* **libc++ 20.1.6**
* Support for most **C++26** language features

> ⚠️ *Clang++ 20.1.6 is not fully standard-compliant with C++26 — no compiler is at this time. However, it is currently the most portable and practical choice.
> Older or partially-supported environments will not build this code correctly.*

> ⚠️ **Note on portability**
> This code is written and tested using Clang++ 20.1.6 and libc++ 20.1.6.
> While you are free to use a different toolchain, no guarantees are made about compatibility or correctness outside this configuration.
> Some features (e.g., `cartesian_product`) are implemented manually due to lack of standard library support.

## 🎯 Goals

* Showcase **idiomatic, modern C++** in the spirit of CS50
* Emphasize **clarity, correctness, and safety**, using:

  * `std::string_view`, `std::ranges`, `constexpr`, and more
* Avoid spoon-feeding — encourage **exploration and curiosity**

## ✅ Completed Problem Sets

* **PS1/cash** — Complete and public
  See [`PS1/cash/README.md`](./PS1/cash/README.md) for full details.

* **PS1/credit** — Complete and public
  See [`PS1/credit/README.md`](./PS1/credit/README.md) for full details.

* **PS4/recover** — Complete and public
  See [`PS4/recover/README.md`](./PS4/recover/README.md) for full details.

> Other problem sets will be published when they are ready.

## 🛠️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

You may also build using `--config Debug` with multi-config generators.

> 🧱 **Note:** No IDE is required. This project is intended to build and run using just the command line. If you prefer to use an IDE, you're welcome to do so, but the code and structure assume only CMake + Ninja + Clang++ as the baseline.

## 🥪 Run Tests

```bash
./build/tests_credit
```

## 🛍️ Code Style and Formatting

All code in this repository is formatted using the author's **global `.clang-format`** settings. No local `.clang-format` file is included.

> Contributors are welcome to use their own formatting preferences. No particular formatting style is enforced.

## 🛍️ Project Philosophy

This is not a tutorial.

This is a **modern rethinking of CS50** through the lens of C++26. It emphasizes structure, type safety, and idiomatic expression — even in introductory problems — without sacrificing clarity.
