#include <algorithm>
#include <array>
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
constexpr std::array<std::string_view, 4> map_card_type_as_string_view{"AMEX", "MASTERCARD", "VISA", "INVALID"};

constexpr std::array<std::pair<std::uint8_t, CardType>, 4> CardPrefixes = {{
    {34, CardType::AMEX},
    {37, CardType::AMEX},
    {4, CardType::VISA},
    {5, CardType::MASTERCARD}
}};

// Convert card_type_t to std::size_t index into map array
constexpr auto card_type_to_index = [](CardType ct) noexcept -> std::size_t {
  return static_cast<std::size_t>(ct);
};

// Convert character to uint8_t (strong typing, avoids ambiguity)
constexpr auto char_to_uint8_t = [](char c) noexcept -> std::uint8_t {
  return static_cast<std::uint8_t>(c);
};

// Convert int to uint8_t explicitly
constexpr auto to_uint8_t = [](int val) noexcept -> std::uint8_t {
  return static_cast<std::uint8_t>(val);
};

// Validate if character is an ASCII digit
constexpr auto is_ascii_digit = [](char c) noexcept -> bool {
  return std::isdigit(char_to_uint8_t(c));
};

// Convert range distance to int (e.g., for digit count)
constexpr auto distance_to_size_t = [](auto first, auto last) noexcept {
  return static_cast<std::size_t>(std::ranges::distance(first, last));
};

// Credit card constraints
constexpr std::size_t MAX_DIGITS = 16;
constexpr auto BASE = 10;

// Compute number of decimal digits in an integer
constexpr auto num_digits(std::uint64_t number) noexcept
{
  std::size_t digits = 0U;
  while (number != 0U)
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
  std::array<std::uint8_t, MAX_DIGITS> digits{};
  const std::size_t count;

  friend constexpr DigitSequence vectorise_number(std::uint64_t) noexcept;

public:
  explicit constexpr DigitSequence(std::size_t c) noexcept : count{c}
  {
  }

  // Forward iterators
  constexpr auto cbegin() const noexcept
  {
    return digits.cbegin();
  }
  constexpr auto cend() const noexcept
  {
    return digits.cbegin() + count;
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

constexpr CardType infer_card_type(const DigitSequence& ds) noexcept
{
    const auto prefix = *ds.cbegin()*10 + (*(ds.cbegin()+1)); // First two digits
    for (const auto& [prefix_str , type] : CardPrefixes) {
        if (prefix == prefix_str) {
            return type;
        }
    }
    return CardType::INVALID;
}

// Convert a number to reversed digit_sequence
constexpr auto vectorise_number(std::uint64_t number) noexcept -> DigitSequence
{
  const auto count = num_digits(number);
  DigitSequence ds(count);
  for (std::size_t i = 0; i < count; ++i)
  {
    ds.digits[i] = to_uint8_t(number % BASE);
    number /= BASE;
  }
  return ds;
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
constexpr auto multiply_by_2 = [](std::uint8_t d) noexcept -> int {
  return d * 2;
};

// Sum function for Luhn doubling rule
constexpr auto luhn_sum = [](int acc, int val) noexcept -> int {
  return acc + (val > 9 ? val - 9 : val); // Split digits if > 9
};

// Luhn: checksum must be divisible by 10
constexpr auto is_valid_checksum(int sum) noexcept -> bool
{
  return sum % 10 == 0;
};

// Luhn: Sum digits at odd positions (not doubled)
constexpr auto accumulate_odd_digits = [](const DigitSequence &ds) {
  auto luhn_identity_digits = std::ranges::subrange(ds.cbegin(), ds.cend()) //
                              | std::views::filter(stride(2, 0));
  return std::accumulate(luhn_identity_digits.begin(), luhn_identity_digits.end(), 0);
};

// Luhn: Double every second digit (even positions), then reduce
constexpr auto accumulate_even_digits = [](const DigitSequence &ds) {
  auto luhn_doubled_digits = std::ranges::subrange(ds.cbegin(), ds.cend()) | std::views::filter(stride(2, 1)) |
                             std::views::transform(multiply_by_2);
  return std::accumulate(luhn_doubled_digits.begin(), luhn_doubled_digits.end(), 0, luhn_sum);
};

// Main card validator
constexpr CardType validate_card_number(std::uint64_t card_number)
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

  [[maybe_unused]]auto test = infer_card_type(digits);

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
  constexpr std::size_t max_input_digits = static_cast<std::size_t>(credit::MAX_DIGITS);

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

    if (input.size() > max_input_digits)
    {
      std::println("Invalid input. Maximum supported length is {} digits.", max_input_digits);
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
  std::println("{}", credit::map_card_type_as_string_view[credit::card_type_to_index(type)]);
  return 0;
}