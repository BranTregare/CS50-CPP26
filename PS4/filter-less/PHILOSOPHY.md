# PHILOSOPHY.md — PS4/filter-less

This implementation builds on the spirit of CS50’s `filter-less` problem but with deliberate generalization, modern tooling, and semantic precision. Rather than assuming only the simplest filters, this version treats **image filters** abstractly and safely.

---

## 🧠 Why Generalize?

The original problem is grounded in a few hardcoded filters. This version:

- Uses C++26 modern features to express filters clearly
- Treats pixel positions and image boundaries rigorously with `PixelPositionFSM`
- Allows filters like **grayscale**, **reflect**, **sepia** (a degenerate 1x1 kernel), and **blur** (a 3x3 spatial kernel)
- Avoids duplicating code by sharing helpers across filters

> **Note:**  
> This version **does not introduce edges** — that is reserved for the more advanced `filter-more` problem. The focus here is on foundational filters with increasing complexity.

---

## ✨ C++26 Modernization

Key C++ features used include:

- `std::array` and `std::mdspan` for safe, static and dynamic data handling
- `constexpr` for compile-time constant filter kernels
- Lambdas and structured bindings for clarity
- Custom strong types like `PixelPositionFSM` to manage pixel boundary cases

These choices demonstrate how **clarity and power can coexist**, even in intermediate-level problems.

---

## 📊 Output Semantics

The filters transform image pixels in-place using shared helper code, emphasizing:

- Correct boundary handling (black padding when necessary)
- Filter kernel application with floating-point arithmetic but final clamping to 8-bit values
- Reusability and modularity of the filtering logic

---

## 🧭 Teaching Notes

This problem invites students to explore:

- Kernel-based filtering concepts (sepia as a degenerate kernel, blur as spatial)
- Handling image edges safely and correctly
- Using strong typing to reduce bugs (e.g., `PixelPositionFSM`)
- Code reuse through shared helpers

See [`TEACHING-STUDENT.md`](./TEACHING-STUDENT.md) and [`TEACHING-INSTRUCTOR.md`](./TEACHING-INSTRUCTOR.md) for extension ideas.

---
