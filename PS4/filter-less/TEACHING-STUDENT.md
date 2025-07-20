# TEACHING-STUDENT.md — PS4/filter-less

This document provides hints, questions, and exploration ideas for students working through the PS4/filter-less project.

---

## 🔍 Core Concepts to Explore

- **Image Processing Basics:**  
  Understand how images are represented as grids of pixels, each containing RGB components.

- **Kernel Operations:**  
  Explore what convolution kernels (like the blur kernel) do and how they affect pixel values.

- **Pixel Position Handling:**  
  Learn why edge and corner pixels require special handling to avoid accessing out-of-bounds data.

- **Using Modern C++ Features:**  
  Notice the use of `std::mdspan` to view and manipulate image data safely and efficiently.

- **Avoiding Code Duplication:**  
  Recognize how helpers.hxx centralizes logic for common tasks like pixel position handling.

---

## 💡 Questions to Consider

- What is the purpose of applying a kernel in image processing?  
- How does the blur kernel combine neighboring pixels to soften an image?  
- Why is it necessary to treat border pixels differently?  
- How does the `PixelPositionFSM` class help manage pixel position state?  
- What advantages does using `std::mdspan` provide over raw pointers or arrays?

---

## 🛠 Suggested Experiments

- Modify the blur kernel values to create different blur intensities.  
- Try adding a simple color filter (like increasing the red channel) using the existing helpers.  
- Implement a grayscale conversion and compare its visual effect.  
- Examine the effects of changing pixel handling logic on edges and corners.

---

## 📚 Additional Resources

- Review CS50’s official lecture notes on image filters for background context.  
- Explore C++20/26 proposals for `std::mdspan` to understand its design and usage.  
- Study convolution in image processing to deepen understanding of kernels.

---

## 🚀 Encouragement

This project demonstrates how even complex image manipulations can be achieved with modern, clean C++ code. Keep experimenting, and try to understand not just *how* the code works, but *why* these techniques are used.

---

> **Note:**  
> The filter-less implementation intentionally excludes the more complex "edges" filter to focus on foundational image operations.
