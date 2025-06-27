# TEACHING-INSTRUCTOR.md — PS3/sort

> Not every exercise is about implementation. Sometimes, the insight is the solution.

---

This problem set from CS50 challenges students to **analyze performance behavior** by timing three unknown sorting programs (`sort1`, `sort2`, and `sort3`) under different conditions:

- Random data
- Sorted data
- Reverse-sorted data

The student is then asked to deduce which algorithm each executable implements.

This is not a coding task, but a **benchmarking and algorithmic reasoning** task. That distinction is critical.

## 🔬 What This Exercise Teaches

- How to benchmark in a principled, repeatable way (e.g. using `time` or `hyperfine`)
- How performance characteristics vary between inputs
- How to connect performance patterns to algorithmic complexity
- How to reason from output to implementation

## 🔍 Identifying the Algorithms

Students are expected to:

- Observe fast execution on already-sorted data
- Notice quadratic growth on reverse or random inputs
- Compare consistent vs input-sensitive behavior

These patterns hint at:

- **Bubble sort with early-exit**: fast on sorted input
- **Selection sort**: slow, consistent regardless of input
- **Merge sort**: fast and consistent, optimized for large input sizes

## 🧰 Teaching Emphases

- This is a rare opportunity to teach **Big-O and Ω(…)** not just as formulas, but as **empirical tools**
- Encourage students to **hypothesize and test**
- Guide them toward the realization that algorithmic complexity is observable
- Emphasize the role of **input shape** in performance

## 🔍 No C++ Here — On Purpose

In the C++26 project, we deliberately avoid reimplementing `sort1.cpp`, `sort2.cpp`, or `sort3.cpp`. Doing so would:

- Distract from the didactic intent
- Risk turning the exercise into a performance contest
- Undermine the message that **understanding behavior is itself a valid goal**

That said, advanced students may optionally be invited to implement the various algorithms and verify predictions. But that belongs elsewhere.

## 🤔 Discussion Starters

- Why is `sort1` so much faster on sorted input?
- What does it tell you when performance is input-invariant?
- Can you explain the data in terms of best-case, worst-case, average-case?
- Which algorithm do you think is used where — and why?
- What are the limits of benchmarking? (e.g., OS noise, caching, etc.)

## 💭 Final Thought

This exercise teaches the art of reading behavior and inferring structure. That, more than code, is what makes a good engineer.

Encourage curiosity. Celebrate observation. Reward insight.
