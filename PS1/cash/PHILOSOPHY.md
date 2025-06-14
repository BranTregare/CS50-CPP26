# PHILOSOPHY.md — PS1/cash

This implementation builds on the spirit of CS50’s `cash.c` problem but with deliberate generalization, modern tooling, and semantic precision. Rather than assuming only coins or a specific locale, this version treats **currency tokens** abstractly and safely.

---

## 🧠 Why Generalize?

The original problem is grounded in US coin denominations and float-based input. This version:

- Uses `std::uint32_t` for exact arithmetic in the smallest denomination (cents or eurocents)
- Allows any currency token set — including **bills**
- Adds support for **EU currency**, not just US

> **Note:**  
> Unlike the original CS50 version, this implementation **does not use floating-point types**. Currency is modeled as whole-number units (`std::uint32_t`) to ensure correctness and avoid the pitfalls of rounding, epsilon comparisons, and binary imprecision. This choice reflects professional best practice and domain fidelity.

---

## ✨ C++26 Modernization

Key C++ features used include:

- `std::array` and `std::span` for safe, static data handling
- `constexpr` for compile-time constant currency tables
- `std::print` for output clarity
- `[[nodiscard]]` to prevent logic bugs from ignored results

These choices demonstrate how **clarity and power can coexist**, even in small programs.

---

## 📊 Output Semantics

Rather than reproducing CS50’s `"4"` output verbatim, we print:

```
US tokens used: 6
EU tokens used: 4
```

This better reflects the internal semantics — `token` is now a generalized unit, not just a coin.

---

## 🧭 Teaching Notes

This problem invites students to explore:

- Greedy algorithm assumptions
- Counterexamples (e.g. `{4, 3, 1}` where greedy fails)
- Swapping `std::span` for dynamic sets
- Real-world constraints like currency rounding

See [`TEACHING-STUDENT.md`](./TEACHING-STUDENT.md) and [`TEACHING-INSTRUCTOR.md`](./TEACHING-INSTRUCTOR.md) for extension ideas.

---

