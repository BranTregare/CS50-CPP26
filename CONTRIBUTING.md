# CONTRIBUTING.md — CS50-CPP26

> Thank you for your interest in this project.

This repository reflects my own exploration and reimagining of Harvard CS50 problem sets in **modern C++26**, using a clean and consistent coding style.

While I do **not actively invite contributions**, they are welcome. If you choose to fork, open issues, or submit pull requests, please be aware of the following:

---

## ✍️ Code Formatting

This project does **not include or enforce** a `.clang-format` file.

You are free to:
- Use your own formatting style or tools
- Apply your own `.clang-format` configuration

**However**, all source files in this repo follow my personal formatting style, which is based on Google’s with some customizations.

> **I maintain a global `.clang-format`** that formats all my C++ code.
> This is not part of the repo — and contributors are not expected to match it automatically.

---

## 📐 Style Guide (Conventions Used)

| Element              | Style            | Example                                     |
|----------------------|------------------|---------------------------------------------|
| Types (class/struct) | PascalCase       | `Person`, `StopWatch`                       |
| Public members       | Camel_Case       | `Parents`, `Alleles`                        |
| Private members      | Camel_Case_      | `Parents_`, `Alleles_`                      |
| Constants            | ALL_CAPS         | `GENERATIONS`, `INDENT_LENGTH`              |
| Functions            | snake_case       | `create_family()`, `print_family()`         |
| Named lambdas        | snake_case       | `multiply_by_2`, `accumulate_even_digits()` |
| Type-to-type lambdas | TypeA_to_TypeB   | `CardType_to_size_t`, `Digit_to_uint8_t`    |

### Notes:
- Structs and data-only types use `struct` (with public access by default).
- Trailing underscores mark **private data members** clearly — not Hungarian notation.
- Identifiers (including parameters and public members) use `Camel_Case`.
- Constants are written in ALL_CAPS only when they are primitive literals (like integers, chars, or booleans) and are usable in constant expressions.
- Compound constant objects — including `std::array`, `std::string_view`, and `struct` instances — use `Camel_Case`, even if they are `constexpr`.
- Prefer `std::array`, `std::unique_ptr`, and ranges-based logic where appropriate.
- Named lambdas that are used as ordinary functions follow `snake_case`.
- Conversion lambdas from one type to another follow `TypeA_to_TypeB` convention, reflecting precise semantic intent.

---

## 🧭 Contribution Etiquette

- PRs are welcome but may be reworked or rejected if they do not fit the goals of this project.
- This project is educational in nature. Clear structure and correctness matter more than performance.
- Feel free to use the code as a reference, fork, or remix — credit is appreciated, but not required.

---

## 🤝 License

This repository is published under the [MIT License](LICENSE). Contributions are welcome under the same terms.
