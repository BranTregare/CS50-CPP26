# PS4: Recover — JPEG File Extractor

This problem reimagines the CS50 `recover.c` assignment using modern **C++26** with a clear, idiomatic, and type-safe design. It demonstrates how even byte-level manipulations and binary file I/O can be cleanly expressed with modern abstractions.

---

## 🎯 Design Intent

The CS50 Recover problem is deceptively low-level: it asks students to walk a FAT-formatted disk image and extract JPEG files based on their magic headers.

This solution preserves the simplicity of the problem while applying modern tools that make reasoning about safety and correctness easier:

* **Strong typing:** Uses `std::byte` and `std::span` instead of raw pointers.
* **Abstraction of unsafe logic:** All `reinterpret_cast` behavior is localized in named lambdas that use `std::bit_cast`, clearly communicating intent and removing ambiguity.
* **Functionally pure logic:** JPEG detection and read/write operations are written as stateless lambdas.
* **Readability-first:** The main control loop models the state transitions clearly without clever tricks.

---

## 🧱 Simplicity by Convention

* **Block size is `constexpr`** and all read buffers are static: no heap, no dynamic growth.
* **No STL containers beyond `std::array`** are needed; this reinforces the minimalism of the original problem.
* **No classes or structs** are introduced — the entire design is flat, but maintainable.

We deliberately avoid modern C++ features that would obscure the core learning goals of:

* understanding file streams,
* performing byte-level operations safely,
* and recognizing structured binary data (JPEG headers).

---

## ⚖️ Educational Tradeoffs

While this version is modern, it does **not over-abstract**:

* There is **no overengineering**: no `JpegFile` class, no virtual functions, no interface layering.
* Type safety and clarity are achieved with lambdas, `std::bit_cast`, and `std::span`.
* Performance is unaffected, but correctness is easier to audit.

The goal is not to "modernize for the sake of modernity" — but to explore how clean and readable such code can become when rethought with C++26 idioms.

---

## 🧭 Why It Matters

CS50’s original implementation teaches raw file I/O and magic number detection — valuable skills. This C++26 version teaches the same, but adds:

* Expressiveness without verbosity
* Explicitness without excess boilerplate
* Safety without runtime overhead

It shows students and instructors alike how idiomatic C++ can enhance readability, safety, and maintainability — even for the most fundamental I/O problems.
