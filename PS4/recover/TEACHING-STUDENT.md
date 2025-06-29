# PS4: Recover — TEACHING-STUDENT.md

This problem gives you an opportunity to think at the byte level — reading raw binary data, recognizing structure in it, and writing results to new files. This C++26 version keeps the focus clear and uncluttered, while subtly introducing you to powerful ideas:

---

## 🔍 Things to Notice

* The main control loop models state transitions cleanly:

    * Are we scanning FAT? Are we writing a JPEG? When do those switch?
* `std::span` is used for block handling: no raw pointers, no index arithmetic
* JPEG detection logic is a named lambda —

  > Can you read it and instantly understand what makes a JPEG header?
* File reads and writes are abstracted into functions that operate on spans — not buffers
* `std::byte` is used for raw memory — not `char`, not `unsigned char` — it's explicitly untyped.

---

## 🧪 Things to Try

* Try making the block buffer non-`static`. Why was `static` used?
* Remove the ternary operator and rewrite the lambdas with an if/else — compare readability.
* What happens if `std::byte` is replaced with `char`? What changes, and what breaks?
* The write function uses `std::bit_cast` to convert `std::byte*` to `char*` — why is this preferable to a `reinterpret_cast`?
* Could this problem be solved using `std::vector` instead of `std::array`?
* Does the order of `JPEG_File.is_open()` and `JPEG_File.good()` matter in context?

---

## ✏️ Code Style to Learn From

* `constexpr` constants like block size improve clarity
* All helper lambdas are pure and local to the logic they serve
* No new types are introduced unless strictly needed
* File I/O errors are handled explicitly, with a clear error message
* Resource cleanup is redundant, but included to reinforce understanding

---

## 💡 Questions to Consider

* How does `std::bit_cast` help abstract away unsafe pointer logic?
* What does the code gain by using `std::span` over raw pointer + size?
* When would it be inappropriate to use `static` buffers?
* Why is `std::byte` preferable to `char` in this context?
* Could this logic be extended to recover other types of files? How would you generalize it?

---

Happy recovering!
