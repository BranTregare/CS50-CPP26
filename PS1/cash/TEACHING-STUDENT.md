# TEACHING-STUDENT.md — PS1/cash

> You already have all the tools. This is just about *seeing clearly* what they can do.

---

## Problem

This project is a modern C++ take on the CS50 problem `cash.c`. In that version, you wrote a small greedy algorithm to calculate how many coins were needed to make change for a given amount.

Here, the idea is the same. But:

- You’re using **integers only**.
- You’re using **modern containers** like `std::array`.
- You’re using **views** on those containers (`std::span`).
- And you’re solving the problem for **multiple currencies** with the same logic.

That’s not just an update — it’s a simplification.

If you find this fun, you’re already writing better code than most working engineers did 15 years ago.

---

## Concepts in Play

- Greedy algorithms (and when they *don’t* work)
- Exact integer arithmetic — no floating-point headaches
- `std::array` for safe, fixed-size tokens
- `std::span` for passing token sets without copying
- `constexpr` for compile-time guarantees and clarity

---

## What You Can Try

This version isn’t locked to US coins. You can easily swap in another currency.

Try changing the line:
```cpp
calculate_tokens(amount, Default_US_Currency)
```
To use your own:
```cpp
constexpr std::array my_coins = { 7, 3, 1 };
```
Then try inputting various amounts.

- Why does this work for all inputs?
- What happens for `10`?
- What makes this particular set so effective?

Now try a token set where greedy *fails*:
```cpp
constexpr std::array tricky_coins = { 4, 3, 1 };
```
- Why does greedy fail here?
- What would an optimal algorithm do differently?

> Greedy algorithms don’t always give optimal answers. Can you figure out why?

---

## Why Integers?

The original problem uses `get_float()`, rounds the result, and works in cents.

This version skips all that:

- You type the value **already in cents** (e.g., `117` for \$1.17)
- There’s **no rounding**, so no chance of floating-point errors
- `std::uint32_t` makes the range and intent **explicit**

No bugs. No surprises. Just numbers.

---

## Why `std::span`?

You could just pass `std::array`. But `std::span` is more flexible:

- You can write `calculate_tokens` once
- You can pass **any range** of tokens
- It doesn't copy anything

This is a tiny project, but it shows the power of designing your functions to work with views.

---

## Going Further

- Add **command-line arguments** to let the user choose which currency to use
- Add **support for additional currencies** (e.g., GBP, INR)
- Show **which tokens were used** (not just how many)
- Replace the token array at runtime with **user input**

None of these require new concepts. Just keep asking:

> What can I do with what I already know?

That’s the mindset of a working engineer.
