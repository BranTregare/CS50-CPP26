# PS3: Sort — Algorithm Recognition via Benchmarking

This project contains **no code**, by design.

The task is to benchmark and analyze the behavior of three black-box sorting executables:
- `sort1`
- `sort2`
- `sort3`

Each uses a different algorithm. The goal is to determine **which** based solely on performance patterns across various inputs.

---

## 🔍 Why No C++?

This is not a coding exercise. It’s a recognition problem.

Implementing versions of bubble sort, selection sort, or merge sort would miss the point. The challenge is to:
- Observe empirical behavior
- Relate it to theoretical expectations
- Formulate a reasoned conclusion

---

## 📊 Benchmark Results

I used `hyperfine` to measure timing across three input types (`random`, `sorted`, and `reversed`):

- `sort1` is **fastest on sorted data**, and slowest on random — indicating an optimized **bubble sort**.
- `sort2` is **consistently fast** — unaffected by input order — likely a **merge sort**.
- `sort3` is **consistently slow**, regardless of input — consistent with **selection sort**.

---

## 🧭 Takeaway

There is no modern C++ here — deliberately so.

This is a problem in **algorithmic fingerprinting**, not implementation. The lesson lies in how different algorithms respond to different data — and in using **data to reason about design**.
