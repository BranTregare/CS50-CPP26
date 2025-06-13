#ifndef CREDIT_TEST_UTIL_HXX
#define CREDIT_TEST_UTIL_HXX

#define USE_SAFE_VALIDATE

#include <cstdint>
#include "../../src/credit.cxx" // Single translation unit use

namespace credit::test {

// Compile-time check for valid input size
constexpr bool is_valid_test_input(std::uint64_t number) noexcept {
  return num_digits(number) <= MAX_DIGITS;
}

// consteval-safe wrappeUSE_SAFE_VALIDATEr for test validation
constexpr CardType safe_validate(std::uint64_t number) {
  if (!is_valid_test_input(number)) {
    return CardType::INVALID;
  }
  return validate_card_number(number);
}

// Macro selector
#ifdef USE_SAFE_VALIDATE
inline constexpr auto test_validate = safe_validate;
#else
inline constexpr auto test_validate = validate_card_number;
#endif

} // namespace credit::test

#endif // CREDIT_TEST_UTIL_HXX