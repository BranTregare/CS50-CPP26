# 🧠 Philosophy

This project reimagines Harvard's CS50 problem sets using modern C++26. It emphasizes correctness, clarity, safety, and performance — not as tradeoffs, but as complementary principles.

The goal is not to hand students pre-digested answers, but to present carefully structured solutions that expose the why as much as the how. Each problem stands on its own: documented, tested, and designed for exploration. When the original CS50 assignment succeeds (as in PS1/credit), that success is amplified; when it falls short (as in PS5/speller), the project provides a better path forward — without compromising on pedagogy.

Although written in C++26, the emphasis is not on novelty but on relevance: standard containers, value semantics, compile-time guarantees, and STL idioms are used where they clarify, not where they merely impress.

The work is informed by decades of professional experience, tempered by a desire to make that experience teachable.

---

### 💾 On Legacy Memory Management and Modern C++ Safety

Dynamic memory management has evolved significantly:

- The original C functions (`malloc`, `calloc`, `free`) were essential innovations but require manual size calculations, explicit casting, and careful pairing, making them error-prone and unsafe by today’s standards.

- The advent of C++'s `new` and `delete` operators was a major step forward, providing type safety, automatic constructor and destructor calls, and clearer semantics, reducing many common bugs in manual memory handling.

- Modern C++ builds upon these foundations with **smart pointers**, **standard containers**, and **ranges**, enabling automatic resource management (RAII), safer code, and clearer, more expressive designs.

This project embraces these modern features — not as bells and whistles, but as practical tools to write code that is correct, safe, and maintainable, even in introductory problem sets.

---

> “In 1994, we optimized because we were desperate,  
> our compilers weren’t up to it,  
> and it was the right thing to do.  
>
> Today, we should optimize because it is right.”

This project carries forward the mindset born from constrained systems, where performance wasn’t optional — it was survival. Even with today’s abundant compute, clarity, safety, and efficiency are not opposites. They are engineering virtues.

See `TEACHING-INSTRUCTOR.md` for a reflection on why these values still matter.
