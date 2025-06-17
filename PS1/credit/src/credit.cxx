#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <string_view>

namespace credit
{
// Strongly typed enumeration for card types
enum class CardType : std::uint8_t
{
  AMEX,
  MASTERCARD,
  VISA,
  INVALID
};

// Map card type enum to string_view (for display purposes)
constexpr std::array<std::string_view, 4> CardType_to_string_view_Map{"AMEX", "MASTERCARD", "VISA", "INVALID"};

// Convert card_type_t to std::size_t index into map array
constexpr auto CardType_to_index = [](CardType Card_Type) noexcept {
  return static_cast<std::size_t>(Card_Type);
};

// Convert character to uint8_t (strong typing, avoids ambiguity)
constexpr auto const_char_to_uint8_t = [](const char Character) noexcept {
  return static_cast<std::uint8_t>(Character);
};

// Convert int to uint8_t explicitly
constexpr auto const_int_to_uint8_t = [](const int Value) noexcept {
  return static_cast<std::uint8_t>(Value);
};

// Explicitly convert char to uint8_t before calling std::isdigit
// to avoid implicit conversions and highlight strong typing.
constexpr auto is_ascii_digit = [](const char Character) noexcept {
  return std::isdigit(const_char_to_uint8_t(Character)) ? true : false;
};

// Convert range distance to std::size_t (e.g., for digit count)
constexpr auto distance_to_size_t = [](auto First, auto Last) noexcept {
  return static_cast<std::size_t>(std::ranges::distance(First, Last));
};

// Credit card constraints
constexpr auto MAX_DIGITS = std::size_t{16};
constexpr auto BASE = std::uint8_t{10};

// Compute number of decimal digits in an integer
constexpr auto num_digits(std::uint64_t Number) noexcept
{
  std::size_t Digits = 0;
  while (Number != 0) {
    Number /= BASE;
    ++Digits;
  }
  return Digits;
}

// Compact struct to represent a credit card number as a reversed digit array
struct DigitSequence final
{
private:
  std::array<std::uint8_t, MAX_DIGITS> Digits_{};
  const std::size_t Count_;

  friend constexpr auto vectorise_number(std::uint64_t) noexcept;

public:
  explicit constexpr DigitSequence(const std::size_t count) noexcept : Count_{count}
  {
  }

  // Forward iterators
  [[nodiscard]] constexpr auto cbegin() const noexcept
  {
    return Digits_.cbegin();
  }
  [[nodiscard]] constexpr auto cend() const noexcept
  {
    return Digits_.cbegin() + Count_;
  }

  // Reverse iterators
  [[nodiscard]] constexpr auto crbegin() const noexcept
  {
    return std::reverse_iterator(cend());
  }
  [[nodiscard]] constexpr auto crend() const noexcept
  {
    return std::reverse_iterator(cbegin());
  }

  // Prevent accidental mutation or raw access
  [[nodiscard]] constexpr auto begin() const noexcept = delete;
  [[nodiscard]] constexpr auto end() const noexcept = delete;
  [[nodiscard]] constexpr auto rbegin() const noexcept = delete;
  [[nodiscard]] constexpr auto rend() const noexcept = delete;
};

// Convert a number to reversed digit_sequence
[[nodiscard]] constexpr auto vectorise_number(std::uint64_t Card_Number) noexcept
{
  const auto Count = num_digits(Card_Number);
  DigitSequence Digit_Sequence(Count);
  for (std::size_t i = 0; i < Count; ++i) {
    Digit_Sequence.Digits_[i] = const_int_to_uint8_t(Card_Number % BASE);
    Card_Number /= BASE;
  }
  return Digit_Sequence;
}

// Returns a stateful predicate that selects every Nth element starting at a given offset.
// It tests the current count before incrementing, so counting starts at zero.
// Using pre-increment (++count) after the test for efficiency and correctness.
constexpr auto stride(std::size_t Stride_Size, std::size_t Offset = 0) noexcept
{
  return [Stride_Size, Offset, Count = std::size_t{0}](auto) mutable noexcept {
    bool result = (Count % Stride_Size) == Offset;  // Check if current element matches stride position
    ++Count;                                        // Increment count after the check (pre-increment style)
    return result;
  };
}

// Multiply digit by 2 (for Luhn algorithm)
// Strongly typed doubling: preserves uint8_t type for clarity and safety
// Avoids implicit promotion to int; keeps result in uint8_t domain
constexpr auto multiply_by_2 = [](const std::uint8_t Digit) noexcept {
  const std::uint8_t Result = Digit * 2;
  return Result;
};

// Sum function for Luhn doubling rule
// Strongly typed sum: ensures result remains within uint16_t domain
// Avoids implicit promotion to int; keeps result in uint16_t domain
constexpr auto luhn_sum = [](std::uint16_t Accumulation, std::uint8_t Value) noexcept {
  const std::uint16_t result = Accumulation + (Value > 9 ? Value - 9 : Value);
  return result;
};

// Luhn: checksum must be divisible by 10
[[nodiscard]] constexpr auto is_valid_checksum(const uint16_t Checksum) noexcept
{
  return Checksum % 10 == 0;
}

// Luhn: Sum digits at odd positions (not doubled)
constexpr auto accumulate_odd_digits = [](const DigitSequence &Digit_Sequence) {
  auto Luhn_Identity_Digits = std::ranges::subrange(Digit_Sequence.cbegin(), Digit_Sequence.cend())  //
                              | std::views::filter(stride(2));
  const std::uint16_t result =
      std::accumulate(Luhn_Identity_Digits.begin(), Luhn_Identity_Digits.end(), std::uint16_t{0});
  return result;
};

// Luhn: Double every second digit (even positions), then reduce
constexpr auto accumulate_even_digits = [](const DigitSequence &digit_sequence) {
  auto Luhn_Doubled_Digits = std::ranges::subrange(digit_sequence.cbegin(), digit_sequence.cend()) |
                             std::views::filter(stride(2, 1)) | std::views::transform(multiply_by_2);
  const std::uint16_t result =
      std::accumulate(Luhn_Doubled_Digits.begin(), Luhn_Doubled_Digits.end(), std::uint16_t{0}, luhn_sum);
  return result;
};

// Main card validator
[[nodiscard]] constexpr auto validate_card_number(const std::uint64_t Card_Number)
{
  if (Card_Number == 0) return CardType::INVALID;

  const auto Digits = vectorise_number(Card_Number);
  const auto Digit_Count = distance_to_size_t(Digits.cbegin(), Digits.cend());

  if (Digit_Count < 13 || Digit_Count > 16) return CardType::INVALID;

  // Explicitly using uint16_t to prevent implicit promotion to int (C++ usual arithmetic conversions)
  if (const std::uint16_t Checksum = accumulate_odd_digits(Digits) + accumulate_even_digits(Digits);
      !is_valid_checksum(Checksum))
    return CardType::INVALID;

  const auto &it = Digits.crbegin();
  const auto Msd = *it;               // Most significant digit
  const auto Second_Msd = *(it + 1);  // Second most significant digit

  // Card type inference based on prefix and length
  switch (Digit_Count) {
    case 13:
      if (Msd == 4) return CardType::VISA;
      break;
    case 15:
      if (Msd == 3 && (Second_Msd == 4 || Second_Msd == 7)) return CardType::AMEX;
      break;
    case 16:
      if (Msd == 4) return CardType::VISA;
      if (Msd == 5 && Second_Msd >= 1 && Second_Msd <= 5) return CardType::MASTERCARD;
      break;
    default:
      return CardType::INVALID;
  }

  return CardType::INVALID;
}
}  // namespace credit

#if defined(CREDIT_NO_MAIN)

int main()
{
  return 0;
}

#else

int main()
{
  std::uint64_t Card_Number = 0;
  constexpr auto MAX_INPUT_DIGITS = static_cast<std::size_t>(credit::MAX_DIGITS);

  while (true) {
    std::print("Number: ");
    std::string Input;

    if (!std::getline(std::cin, Input)) {
      if (std::cin.eof()) {
        std::println("EOF detected aborting");
        return 1;
      }
      else {
        std::println("Input error.");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
    }

    // Check for digit-only input

    if (const bool all_digits = std::ranges::all_of(Input, credit::is_ascii_digit); !all_digits) {
      std::println("Invalid input. Please enter digits only (no letters or symbols).");
      continue;
    }

    if (Input.size() > MAX_INPUT_DIGITS) {
      std::println("Invalid input. Maximum supported length is {} digits.", MAX_INPUT_DIGITS);
      continue;
    }

    // Convert string to number
    std::istringstream iss(Input);
    iss >> Card_Number;

    if (Card_Number == 0) {
      std::println("Invalid input. Must be a non-zero number.");
      continue;
    }

    break;
  }

  const auto type = credit::validate_card_number(Card_Number);
  std::println("{}", credit::CardType_to_string_view_Map[credit::CardType_to_index(type)]);
  return 0;
}
#endif  // CREDIT_NO_MAIN