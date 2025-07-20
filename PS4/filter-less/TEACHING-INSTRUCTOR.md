
# TEACHING-INSTRUCTOR.md — PS4/filter-less

This document offers guidance and insights for instructors facilitating the **filter-less** project, part of PS4 in the CS50-CPP26 repository. It complements the **student exploration guide** and supports clear, effective teaching.

---

## 🎯 Learning Goals

- Understand basic image processing filters: **grayscale**, **reflect**, **sepia**, and **blur**
- Learn the practical use of `std::mdspan` for 2D image data access and manipulation
- Explore kernel-based operations and pixel neighborhood processing
- Practice careful boundary handling in image filters
- Appreciate the benefit of code reuse and abstraction (e.g., `PixelPositionFSM`)
- Gain confidence in modern C++26 idioms and safety features

---

## 🧩 Key Concepts to Emphasize

- **Kernel Convolution:** Even simple filters like blur depend on iterating pixel neighborhoods using spatial kernels (here 3x3). Explain how kernels apply weights to pixel channels.
- **Degenerate Kernels:** Discuss why sepia is effectively a 1x1 kernel (no spatial neighborhood) and contrast that with blur’s 3x3.
- **Pixel Position State Machine:** Demonstrate how `PixelPositionFSM` helps handle edges and corners cleanly without cluttering the filter code with bounds checks.
- **Data Copy and Aliasing:** Explain the use of a copy buffer (`Image_Ref`) to avoid aliasing issues during in-place filtering.
- **Modular Design:** Highlight how shared helpers (e.g., in `helpers.hxx`) allow code reuse between `filter-less` and more advanced projects.

---

## 📝 Teaching Tips

- Encourage students to step through the filters with small images and debug prints to visualize kernel application.
- Highlight modern C++ features and how they improve safety and clarity compared to C-style arrays.
- Discuss performance implications of different copy methods (e.g., `std::copy` vs manual loops).
- Stress the importance of separating concerns: image boundary logic vs pixel kernel computation.
- For more advanced students, suggest extending filters or experimenting with custom kernels.

---

## 📚 Suggested Exercises

- Modify the blur kernel size or weights and observe effects.
- Implement an additional simple filter (e.g., invert colors) using the same framework.
- Replace the `PixelPositionFSM` with explicit boundary checks — compare complexity.
- Measure performance with and without the reference image copy.

---

## ⚠️ Common Pitfalls

- Forgetting to handle image boundaries correctly, causing out-of-bounds access.
- Confusing channel ordering in `RGBTRIPLE` (remember: Blue, Green, Red).
- Modifying the source image directly during kernel application (leading to incorrect results).

---

## 🔗 Additional Resources

- Review [`helpers.hxx`](/path/to/helpers.hxx) for shared utilities and abstractions.
- See [`TEACHING-STUDENT.md`](/path/to/TEACHING-STUDENT.md) for student exploration guidance.
- Consult [C++26 mdspan proposals](https://en.cppreference.com/w/cpp/experimental/mdspan) for deeper insight into multidimensional spans.

---
