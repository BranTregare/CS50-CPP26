# PS4: Recover — JPEG Recovery from a FAT Image

This project reimagines the CS50 `recover.c` problem using modern **C++26**, compiled with **Clang++ 20.1.6** and **libc++**.
It scans a raw **FAT-formatted disk image** for embedded JPEG headers and extracts each image to a separate file.

---

## 💡 Problem Statement

Given a raw memory card image (e.g., from a digital camera), **recover all JPEG files** embedded in it.
JPEGs begin with specific header bytes, and each file consists of a sequence of 512-byte blocks.

---

## ✨ Key Features

* Uses `std::byte`, `std::span`, and `std::bit_cast` to express byte-level logic safely and clearly
* Avoids raw pointer arithmetic; all memory access is safe, typed, and bounds-checked
* Precise state-driven recovery model with minimal branching and clean transitions
* `constexpr` header check logic using bitmasking for JPEG signature detection
* RAII ensures clean resource handling (manual file closes are **retained for educational clarity**)
* `StopWatch` utility tracks execution time for **illustrative performance profiling**

**Sample Output:**

```text
usage: recover <input_filename>
File card.raw could not be opened: File does not exist, or you do not have permission to read.
```

or:

```text
Found JPEGs: 50
Time JPG recovery: 0.014s
```

---

## 🔍 What’s Different?

| Aspect            | Traditional CS50 `recover.c`     | This C++26 Version                              |
| ----------------- | -------------------------------- | ----------------------------------------------- |
| Byte handling     | Raw pointers and `uint8_t`       | `std::byte`, `std::span`, `std::bit_cast`       |
| JPEG detection    | Magic numbers in `if` statements | `constexpr` predicate with bitmasking           |
| I/O               | Manual, untyped buffer reads     | Safe `std::ifstream` with typed buffers         |
| Output filenames  | Formatted with `sprintf`         | `std::format("{:0>3}.jpg", n)`                  |
| Error handling    | Basic `if`/`else`                | Uses `[[likely]]`, `[[unlikely]]` for clarity   |
| Memory management | Manual buffer on stack           | Static allocation with type-safe access         |
| Style & clarity   | Minimal and imperative           | Strong typing, modern idioms, and explicit flow |

---

## 📁 Layout

```
PS4/recover/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── PHILOSOPHY.md               # Design goals and rationale
├── TEACHING-INSTRUCTOR.md      # Instructor guidance
├── TEACHING-STUDENT.md         # Exploration hints for students
├── recover.cxx                 # Main implementation
└── include/
    └── stopwatch.hxx           # Stopwatch utility for timing
```

---

## ⚙️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## ⚠️ Requirements

> * **Clang++ 20.1.6**
> * **libc++ 20.1.6**
> * **CMake 3.31.6**
> * Ensure `-stdlib=libc++` is correctly configured

---

## 🥪 Run the Program

```bash
./build/recover card.raw
```

Each discovered JPEG will be written as `000.jpg`, `001.jpg`, etc.
A `StopWatch` timer reports **approximate** recovery time for the operation.

---

## 🔍 Things to Explore

* How does using `std::byte` prevent accidental reinterpretation of memory?
* What does `std::bit_cast` offer over manual casting?
* Why is `std::span` safer than passing raw pointers and lengths?
* Why is the fourth JPEG header byte masked with `0xf0`?
* What do `[[likely]]` and `[[unlikely]]` convey—to both the compiler *and* to human readers?
* Could this recovery process be parallelized? What would the tradeoffs be?

---

## 📌 Project Philosophy

This implementation emphasizes **clarity, correctness, and safety** in low-level programming.
It reworks a classic file recovery challenge to demonstrate that even byte-wise logic can benefit from modern **C++ abstractions**.

See [`PHILOSOPHY.md`](./PHILOSOPHY.md) for deeper insights into the design and reasoning behind this approach.

---

> **Disclaimer**
> This reimplementation is not affiliated with Harvard or CS50.
> It is offered for learning and exploration and should not be considered official course material.

## 📜 License

This project is licensed under the [MIT License](./LICENSE).
