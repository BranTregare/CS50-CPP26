# TEACHING-STUDENT.md — PS1/cash

This problem revisits CS50’s `cash.c`, but reimagined for modern C++. The goal is to explore how simple problems benefit from better structure, stronger types, and a safer approach to numbers.

Don’t worry — there are no new concepts beyond what you already know. This is just about *thinking more clearly* with the tools C++ gives you.

---

## 💰 The Problem

You’re asked to calculate the minimum number of coins (or bills) to make up a given amount. The original version only used US coins, and started with a float (like `$0.41`).

This version uses **integers only**. Why?

- Floating-point numbers are often **imprecise** for money.
- You don’t need decimal fractions to count tokens.
- This lets us focus on logic — not bugs from rounding.

---

## 🧠 Concepts in Play

- `std::array` for fixed lists (like coin denominations)
- `std::span` to pass a read-only view of that array to a function
- Greedy algorithms: take the biggest token you can, then repeat
- Strong typing with `std::uint32_t` to avoid signed/overflow issues

---

## 🧪 Explore Further

Try the following changes:

- What happens if you replace the coin denominations with `[4, 3, 1]`?
- Could you return not just the count, but *which* tokens were used?
- Could you write a version that lets the user input their own tokens?

If you find this fun, you’re already writing better code than most working engineers did 15 years ago.

---

## 🤔 Things to Think About

- Why is greedy usually right — but sometimes wrong?
- Why does `std::array` work here but `std::vector` might be overkill?
- What does `std::span` protect you from?

You don’t have to know all the answers. Just *ask the questions*. That’s what engineers do.

---

> 📎 See [`TEACHING-INSTRUCTOR.md`](./TEACHING-INSTRUCTOR.md) if you're curious what we’ve left out — and why.
