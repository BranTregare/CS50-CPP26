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
constexpr auto CardType_to_index = [](CardType cardType) noexcept {
  return static_cast<std::size_t>(cardType);
};

// Convert character to uint8_t (strong typing, avoids ambiguity)
constexpr auto char_to_uint8_t = [](char character) noexcept {
  return static_cast<std::uint8_t>(character);
};

// Convert int to uint8_t explicitly
constexpr auto to_uint8_t = [](int value) noexcept {
  return static_cast<std::uint8_t>(value);
};

// Validate if character is an ASCII digit
constexpr auto is_ascii_digit = [](char character) noexcept {
  return std::isdigit(char_to_uint8_t(character)) ? true : false;
};

// Convert range distance to int (e.g., for digit count)
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
  explicit constexpr DigitSequence(std::size_t count) noexcept : count_{count}
  {
  }

  // Forward iterators
  constexpr auto cbegin() const noexcept
  {
    return digits_.cbegin();
  }
  constexpr auto cend() const noexcept
  {
    return digits_.cbegin() + count_;
  }

  // Reverse iterators
  constexpr auto crbegin() const noexcept
  {
    return std::reverse_iterator(cend());
  }
  constexpr auto crend() const noexcept
  {
    return std::reverse_iterator(cbegin());
  }

  // Prevent accidental mutation or raw access
  constexpr auto begin() const noexcept = delete;
  constexpr auto end() const noexcept = delete;
  constexpr auto rbegin() const noexcept = delete;
  constexpr auto rend() const noexcept = delete;
};

// Convert a number to reversed digit_sequence
constexpr auto vectorise_number(std::uint64_t card_number) noexcept
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
// Generate a filter predicate that returns true every Nth element (used in
// stride logic)
constexpr auto stride(std::size_t stride_size, std::size_t offset = 0) noexcept
{
  return [stride_size, offset, count = std::numeric_limits<std::size_t>::max()](auto) mutable noexcept {
    return (++count % stride_size) == offset;
  };
}

// Multiply digit by 2 (for Luhn algorithm)
constexpr auto multiply_by_2 = [](std::uint8_t d) noexcept {
  return d * 2;
};

// Sum function for Luhn doubling rule
constexpr auto luhn_sum = [](int acc, int val) noexcept{
  return acc + (val > 9 ? val - 9 : val); // Split digits if > 9
};

// Luhn: checksum must be divisible by 10
constexpr auto is_valid_checksum(int sum) noexcept
{
  return sum % 10 == 0;
};

// Luhn: Sum digits at odd positions (not doubled)
constexpr auto accumulate_odd_digits = [](const DigitSequence &digit_sequence) {
  auto luhn_identity_digits = std::ranges::subrange(digit_sequence.cbegin(), digit_sequence.cend()) //
                              | std::views::filter(stride(2));
  return std::accumulate(luhn_identity_digits.begin(), luhn_identity_digits.end(), 0);
};

// Luhn: Double every second digit (even positions), then reduce
constexpr auto accumulate_even_digits = [](const DigitSequence &digit_sequence) {
  auto luhn_doubled_digits = std::ranges::subrange(digit_sequence.cbegin(), digit_sequence.cend()) |
                             std::views::filter(stride(2, 1)) | std::views::transform(multiply_by_2);
  return std::accumulate(luhn_doubled_digits.begin(), luhn_doubled_digits.end(), 0, luhn_sum);
};

// Main card validator
constexpr auto validate_card_number(std::uint64_t card_number)
{
  if (card_number == 0)
    return CardType::INVALID;

  const auto digits = vectorise_number(card_number);
  const auto digit_count = distance_to_size_t(digits.cbegin(), digits.cend());

  if (digit_count < 13 || digit_count > 16)
    return CardType::INVALID;

  const int checksum = accumulate_odd_digits(digits) + accumulate_even_digits(digits);
  if (!is_valid_checksum(checksum))
    return CardType::INVALID;

  const auto &it = digits.crbegin();
  const auto msd = *it; // Most significant digit
  const auto second_msd = (digit_count >= 2) ? *(it + 1) : to_uint8_t(0);

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
    if (msd == 5 && (second_msd >= 1 && second_msd <= 5))
      return CardType::MASTERCARD;
    break;
  default:
    return CardType::INVALID;
  }

  return CardType::INVALID;
}
} // namespace credit

int main()
{
  std::uint64_t card_number = 0;
  constexpr std::size_t MAX_INPUT_DIGITS = static_cast<std::size_t>(credit::MAX_DIGITS);

  while (true)
  {
    std::print("Number: ");
    std::string input;
    if (!std::getline(std::cin, input))
    {
      std::println("Input error.");
      return 1;
    }

    // Check for digit-only input
    const bool all_digits = std::ranges::all_of(input, credit::is_ascii_digit);

    if (!all_digits)
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

  dconst auto type = credit::validate_card_number(card_number);
  std::println("{}", credit::CardType_to_string_view[credit::CardType_to_index(type)]);
  return 0;
}