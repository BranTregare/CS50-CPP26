
# PS4: Filter-Less — Image Filters in Modern C++26

This project reimagines the CS50 `filter-less` problem using modern **C++26**, compiled with **Clang++ 20.1.6** and **libc++**. It implements a set of image filters — **grey**, **reflect**, **sepia**, and **blur** — applied to BMP images using clean, reusable abstractions.

---

## 💡 Problem Statement

Given an input BMP image, apply one or more image filters to transform the image’s colors or layout.

Filters include:

- **Grey**: Convert the image to grayscale  
- **Reflect**: Flip the image horizontally  
- **Sepia**: Apply a sepia tone effect (implemented as a degenerate 1x1 kernel)  
- **Blur**: Apply a simple box blur using a 3x3 kernel  

The user supplies an image, and the program outputs a filtered version.

---

## ✨ Key Features

- Implements **grey**, **reflect**, **sepia**, and **blur** filters  
- Uses **`std::mdspan`** to handle 2D image data cleanly and safely  
- Modern **C++26** features including `constexpr`, lambdas, and structured bindings  
- Shared helper utilities in `helpers.hxx` for common image operations  
- Clear separation of filters for learners at different comfort levels  

---

## 🔍 What’s Different?

| Aspect                 | Traditional CS50 `filter-less` | This C++26 Version                  |
|------------------------|---------------------------------|------------------------------------|
| Image data access      | Raw pointer arithmetic           | `std::mdspan` for 2D indexing      |
| Filter implementations | C-style loops and manual logic   | Modern lambdas and abstractions    |
| Sepia filter           | Hardcoded formula                | Implemented as degenerate 1x1 kernel |
| Shared helpers         | Separate for each filter         | Single `helpers.hxx` shared by filters |
| Language features      | C                              | C++26 with modern standard library |

---

## 📁 Layout

```
PS4/filter-less/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── PHILOSOPHY.md               # Design goals and rationale
├── TEACHING-INSTRUCTOR.md      # Instructor guidance
├── TEACHING-STUDENT.md         # Exploration hints for students
├── src/
│   └── filter_less.cxx         # Main filter logic and driver
└── src/include/
    └── helpers.hxx             # Shared helper functions and utilities
```

---

## ⚙️ Build Instructions

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/filter-less
```

---

## ⚠️ Requirements

> - **Clang++ 20.1.6**  
> - **libc++ 20.1.6**  
> - **CMake 3.31.6**  
> - Proper `-stdlib=libc++` configuration  
> - BMP images as input/output format (using Microsoft `RGBTRIPLE` format)  

---

## 🔍 Things to Explore

- How does `std::mdspan` simplify 2D image access?  
- Why is sepia a degenerate kernel compared to blur?  
- How do lambdas improve clarity in filter implementations?  
- How could you add new filters using the same helper abstractions?  

---

## 📌 Project Philosophy

This implementation is designed to mirror CS50’s `filter-less` functionality exactly, but using modern **C++26** idioms to promote clarity, correctness, and reusability. It is not a simplified or reduced version, but a faithful reimplementation that exposes learners to the future direction of C++ programming.

Filters like **edges** are deliberately excluded here, reserved for a “more comfortable” project (`filter-more`), but all shared helpers are present to encourage exploration and understanding of common utilities.

See [`PHILOSOPHY.md`](./PHILOSOPHY.md) for a deeper dive into the design goals and values driving this project.

---

> **Disclaimer**  
> This reimplementation is not affiliated with Harvard or CS50.  
> It is offered for learning and exploration, and should not be considered official course material.
