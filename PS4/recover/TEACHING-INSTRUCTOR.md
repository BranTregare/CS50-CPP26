# PS4: Recover — Instructor Guidance

This document offers instructor insights for teaching the CS50 Recover problem reimplemented in modern **C++26**.

---

## 🧩 Core Learning Objectives

- Understanding **raw binary file I/O** with `std::ifstream` and `std::ofstream`.
- Recognizing **FAT block structure** and JPEG magic headers.
- Using **`std::byte`** and **`std::span`** for safer byte-level data manipulation.
- Abstracting low-level operations using **lambdas** and `std::bit_cast` to minimize unsafe casts.
- Applying **stateful control flow** to manage file extraction logic.
- Appreciating clear and maintainable code through **idiomatic C++26 features**.

---

## 🛠️ Teaching Tips

- Emphasize why `std::byte` is preferable to `unsigned char` or raw pointers for raw binary data.
- Discuss the motivation for using **`std::span`** to represent contiguous memory without copying.
- Explore the use of **`std::bit_cast`** as a safe alternative to `reinterpret_cast` for type punning.
- Highlight how the lambdas encapsulate *small*, *pure* operations and improve modularity.
- Use the program’s explicit state transitions as a teaching example of readable control flow.
- Point out the reason for static allocation of the buffer — avoiding dynamic memory for simplicity.

---

## ⚠️ Common Pitfalls

- Students may misunderstand how to detect JPEG headers correctly—stress the bitmasking logic on the fourth byte.
- File stream state checking (`good()`, `is_open()`) may be overlooked or misused.
- Forgetting to close open files before opening a new one can cause resource leaks.
- Not considering that the last JPEG might not be followed by another header—closing files properly is important.

---

## 🧪 Assessment Suggestions

- Ask students to implement variants that log diagnostic info (e.g., print each block’s first bytes).
- Encourage students to modify buffer size and observe effects.
- Challenge students to replace lambdas with functions or classes and discuss tradeoffs.
- Have students write tests to verify JPEG header detection logic.
- Suggest adding error recovery when file operations fail.

---

## 🧭 Further Exploration

- Compare this C++26 solution with the original CS50 C implementation.
- Discuss how more advanced C++ features (e.g., classes, RAII wrappers) might improve or complicate the design.
- Introduce memory mapping as an alternative for large disk images.
- Explore performance implications of buffer size or different I/O strategies.

---

> **Note:** This reimplementation balances clarity and educational value over complexity or performance optimizations. It aims to help students internalize key concepts safely and idiomatically.

---

> **Disclaimer:**  
> This is an independent educational resource, not official CS50 material.

