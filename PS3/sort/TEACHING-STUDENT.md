# TEACHING-STUDENT.md — PS3/sort

> Benchmarking can teach you a lot more than just speed — it can teach you how to *see* algorithms.

This exercise isn’t about writing code. It’s about running code, observing patterns, and using your judgment to match behavior with design. You’re given three black-box sorters (`sort1`, `sort2`, `sort3`) and asked to identify which sorting algorithm each one uses based on performance.

It’s not a trick. But it *is* a test of your ability to:

- Design experiments
- Recognize algorithmic behavior
- Interpret runtime data

## 💡 How to Approach This

Use the Linux `time` command or the more precise `hyperfine` benchmarking tool (optional) to measure how fast each executable runs with different inputs:

- Random order
- Sorted order
- Reverse-sorted order

Record your results. Then analyze:

- Which algorithm is fastest on *sorted* data?
- Which algorithm is consistently fast, regardless of input?
- Which one is the slowest on reverse-sorted input?

From this, you can likely infer:

- **Bubble sort**: Very fast on sorted input (best case Ω(N)), slow otherwise.
- **Selection sort**: Consistent but slow across the board (Θ(N²)).
- **Merge sort**: Fast and stable in all cases (Θ(N log N)).

## 📌 Key Takeaways

- Sorting performance varies wildly depending on algorithm and data.
- Time complexity isn’t just a theory — you can *see* it in real runtimes.
- There is value in recognizing behavior, not just writing code.

## 🧪 Want to Go Deeper?

Try building your own implementations of bubble sort, selection sort, and merge sort in modern C++. Then benchmark them the same way. Do your results match what CS50 gave you?

This isn't required — but it's a great way to verify your understanding.

> "The goal is not just to recognize sorting — it's to recognize *thinking*."

---

This is an opportunity to get better at *reasoning*, not just programming.
