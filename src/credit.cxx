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
constexpr std::array<std::string_view, 4> CardType_to_string_view{"AMEX", "MASTERCARD", "VISA", "INVALID"};

// Convert card_type_t to std::size_t index into map array
constexpr auto CardType_to_index = [](CardType card_type) noexcept {
  return static_cast<std::size_t>(card_type);
};

// Convert character to uint8_t (strong typing, avoids ambiguity)
constexpr auto char_to_uint8_t = [](const char character) noexcept {
  return static_cast<std::uint8_t>(character);
};

// Convert int to uint8_t explicitly
constexpr auto to_uint8_t = [](const int value) noexcept {
  return static_cast<std::uint8_t>(value);
};

// Explicitly convert char to uint8_t before calling std::isdigit
// to avoid implicit conversions and highlight strong typing.
constexpr auto is_ascii_digit = [](const char character) noexcept {
  return std::isdigit(char_to_uint8_t(character)) ? true : false;
};

// Convert range distance to std::size_t (e.g., for digit count)
constexpr auto distance_to_size_t = [](auto first, auto last) noexcept {
  return static_cast<std::size_t>(std::ranges::distance(first, last));
};

// Credit card constraints
constexpr auto MAX_DIGITS = std::size_t{16};
constexpr auto BASE = std::uint8_t{10};

// Compute number of decimal digits in an integer
constexpr auto num_digits(std::uint64_t number) noexcept
{
  std::size_t digits = 0;
  while (number != 0)
  {
    number /= BASE;
    ++digits;
  }
  return digits;
}

// Compact struct to represent a credit card number as a reversed digit array
struct DigitSequence final
{
private:
  std::array<std::uint8_t, MAX_DIGITS> digits_{};
  const std::size_t count_;

  friend constexpr auto vectorise_number(std::uint64_t) noexcept;

public:
  explicit constexpr DigitSequence(const std::size_t count) noexcept : count_{count}
  {
  }

  // Forward iterators
  [[nodiscard]] constexpr auto cbegin() const noexcept
  {
    return digits_.cbegin();
  }
  [[nodiscard]] constexpr auto cend() const noexcept
  {
    return digits_.cbegin() + count_;
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
[[nodiscard]] constexpr auto vectorise_number(std::uint64_t card_number) noexcept
{
  const auto count = num_digits(card_number);
  DigitSequence digit_sequence(count);
  for (std::size_t i = 0; i < count; ++i)
  {
    digit_sequence.digits_[i] = to_uint8_t(card_number % BASE);
    card_number /= BASE;
  }
  return digit_sequence;
}

// Returns a stateful predicate that selects every Nth element starting at a given offset.
// It tests the current count before incrementing, so counting starts at zero.
// Using pre-increment (++count) after the test for efficiency and correctness.
constexpr auto stride(std::size_t stride_size, std::size_t offset = 0) noexcept {
  return [stride_size, offset, count = std::size_t{0}](auto) mutable noexcept {
    bool result = (count % stride_size) == offset;  // Check if current element matches stride position
    ++count;  // Increment count after the check (pre-increment style)
    return result;
  };
}

// Multiply digit by 2 (for Luhn algorithm)
// Strongly typed doubling: preserves uint8_t type for clarity and safety
// Avoids implicit promotion to int; keeps result in uint8_t domain
constexpr auto multiply_by_2 = [](const std::uint8_t digit) noexcept{
  const std::uint8_t result = digit * 2;
  return result;
};

// Sum function for Luhn doubling rule
// Strongly typed sum: ensures result remains within uint16_t domain
// Avoids implicit promotion to int; keeps result in uint16_t domain
constexpr auto luhn_sum = [](std::uint16_t acc, std::uint8_t val) noexcept {
  const std::uint16_t result = acc + (val > 9 ? val - 9 : val);
  return result;
};

// Luhn: checksum must be divisible by 10
[[nodiscard]] constexpr auto is_valid_checksum(const uint16_t checksum) noexcept
{
  return checksum % 10 == 0;
}

// Luhn: Sum digits at odd positions (not doubled)
constexpr auto accumulate_odd_digits = [](const DigitSequence &digit_sequence) {
  auto luhn_identity_digits = std::ranges::subrange(digit_sequence.cbegin(), digit_sequence.cend()) //
                              | std::views::filter(stride(2));
  const std::uint16_t result = std::accumulate(luhn_identity_digits.begin(), luhn_identity_digits.end(),std::uint16_t{0});
  return result;
};

// Luhn: Double every second digit (even positions), then reduce
constexpr auto accumulate_even_digits = [](const DigitSequence &digit_sequence) {
  auto luhn_doubled_digits = std::ranges::subrange(digit_sequence.cbegin(), digit_sequence.cend()) |
                             std::views::filter(stride(2, 1)) | std::views::transform(multiply_by_2);
  const std::uint16_t result = std::accumulate(luhn_doubled_digits.begin(), luhn_doubled_digits.end(), std::uint16_t{0}, luhn_sum);
  return result;
};

// Main card validator
[[nodiscard]] constexpr auto validate_card_number(const std::uint64_t card_number)
{
  if (card_number == 0)
    return CardType::INVALID;

  const auto digits = vectorise_number(card_number);
  const auto digit_count = distance_to_size_t(digits.cbegin(), digits.cend());

  if (digit_count < 13 || digit_count > 16)
    return CardType::INVALID;

  // Explicitly using uint16_t to prevent implicit promotion to int (C++ usual arithmetic conversions)
  if (const std::uint16_t checksum = accumulate_odd_digits(digits) + accumulate_even_digits(digits); !is_valid_checksum(checksum))
    return CardType::INVALID;

  const auto &it = digits.crbegin();
  const auto msd = *it;              // Most significant digit
  const auto second_msd = *(it + 1); // Second most significant digit

  // Card type inference based on prefix and length
  switch (digit_count)
  {
  case 13:
    if (msd == 4)
      return CardType::VISA;
    break;
  case 15:
    if (msd == 3 && (second_msd == 4 || second_msd == 7))
      return CardType::AMEX;
    break;
  case 16:
    if (msd == 4)
      return CardType::VISA;
    if (msd == 5 && second_msd >= 1 && second_msd <= 5)
      return CardType::MASTERCARD;
    break;
  default:
    return CardType::INVALID;
  }

  return CardType::INVALID;
}
} // namespace credit

#if defined(CREDIT_NO_MAIN)

int main()
{
  return 0;
}

#else

int main()
{
  std::uint64_t card_number = 0;
  constexpr auto MAX_INPUT_DIGITS = static_cast<std::size_t>(credit::MAX_DIGITS);

  while (true)
  {
    std::print("Number: ");
    std::string input;

    if (!std::getline(std::cin, input))
    {
      if (std::cin.eof())
      {
        std::println("EOF detected aborting");
        return 1;
      }
      else
      {
        std::println("Input error.");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
    }

    // Check for digit-only input

    if (const bool all_digits = std::ranges::all_of(input, credit::is_ascii_digit); !all_digits)
    {
      std::println("Invalid input. Please enter digits only (no letters or symbols).");
      continue;
    }

    if (input.size() > MAX_INPUT_DIGITS)
    {
      std::println("Invalid input. Maximum supported length is {} digits.", MAX_INPUT_DIGITS);
      continue;
    }

    // Convert string to number
    std::istringstream iss(input);
    iss >> card_number;

    if (card_number == 0)
    {
      std::println("Invalid input. Must be a non-zero number.");
      continue;
    }

    break;
  }

  const auto type = credit::validate_card_number(card_number);
  std::println("{}", credit::CardType_to_string_view[credit::CardType_to_index(type)]);
  return 0;
}
#endif // CREDIT_NO_MAIN