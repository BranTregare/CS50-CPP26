#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <span>
#include <string>

#include "include/stopwatch.hxx"

// Define a type alias for currency units (in cents or eurocents)
using CurrencyUnit = std::uint32_t;

// Define US currency denominations in cents (sorted from largest to smallest)
constexpr auto Default_US_Currency = std::array{
    CurrencyUnit{10000},  // $100
    CurrencyUnit{5000},   // $50
    CurrencyUnit{2000},   // $20
    CurrencyUnit{1000},   // $10
    CurrencyUnit{500},    // $5
    CurrencyUnit{100},    // $1
    CurrencyUnit{25},     // quarter
    CurrencyUnit{10},     // dime
    CurrencyUnit{5},      // nickel
    CurrencyUnit{1}       // penny
};

// Define EU currency denominations in eurocents (sorted from largest to smallest)
constexpr auto Default_EU_Currency = std::array{
    CurrencyUnit{50000},  // €500
    CurrencyUnit{20000},  // €200
    CurrencyUnit{10000},  // €100
    CurrencyUnit{5000},   // €50
    CurrencyUnit{2000},   // €20
    CurrencyUnit{1000},   // €10
    CurrencyUnit{500},    // €5
    CurrencyUnit{200},    // €2 coin
    CurrencyUnit{100},    // €1 coin
    CurrencyUnit{50},     // 50¢
    CurrencyUnit{20},     // 20¢
    CurrencyUnit{10},     // 10¢
    CurrencyUnit{5},      // 5¢
    CurrencyUnit{2},      // 2¢
    CurrencyUnit{1}       // 1¢
};

// Calculates the minimum number of tokens required to represent the given amount
[[nodiscard]] constexpr auto calculate_tokens(CurrencyUnit amount, std::span<const CurrencyUnit> denominations)
{
  // use std::uint16_t for token count: safe upper bound, unsigned, strongly typed.
  auto tokens_used = std::uint16_t{0};  // Accumulator for token count

  // Iterate over each denomination greedily using the largest possible token
  for (auto token : denominations) {
    tokens_used += amount / token;  // Add the number of tokens used for this denomination
    amount %= token;                // Update amount to remainder
  }

  return tokens_used;
}

int main()
{
  auto amount_owed = CurrencyUnit{};  // Amount entered by the user (in the smallest currency unit)

  // Prompt the user for input
  std::print("Amount owed in lowest token value: ");
  std::cin >> amount_owed;

  // Validate input
  if (std::cin.fail()) {
    std::cin.clear();                // Clear error flag
    std::cin.ignore(1 << 10, '\n');  // Discard invalid input
    std::println("Invalid input.");
    return 1;
  }

  // Start timing the calculation
  auto t = StopWatch("CalculationTime");

  // Output the token counts for both currency systems
  std::println("US tokens used: {}", calculate_tokens(amount_owed, Default_US_Currency));
  std::println("EU tokens used: {}", calculate_tokens(amount_owed, Default_EU_Currency));

  return 0;
}
