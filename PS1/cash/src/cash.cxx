#include "include/stopwatch.hxx"
#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <span>
#include <string>

using CurrencyUnit = std::uint32_t;

// US currency in cents
constexpr auto Default_US_Currency = std::array{
    CurrencyUnit{10000}, // $100
    CurrencyUnit{5000},  // $50
    CurrencyUnit{2000},  // $20
    CurrencyUnit{1000},  // $10
    CurrencyUnit{500},   // $5
    CurrencyUnit{100},   // $1
    CurrencyUnit{25},    // quarter
    CurrencyUnit{10},    // dime
    CurrencyUnit{5},     // nickel
    CurrencyUnit{1}      // penny
};

// EU currency in eurocents
constexpr auto Default_EU_Currency = std::array{
    CurrencyUnit{50000}, // €500
    CurrencyUnit{20000}, // €200
    CurrencyUnit{10000}, // €100
    CurrencyUnit{5000},  // €50
    CurrencyUnit{2000},  // €20
    CurrencyUnit{1000},  // €10
    CurrencyUnit{500},   // €5
    CurrencyUnit{200},   // €2 coin
    CurrencyUnit{100},   // €1 coin
    CurrencyUnit{50},    // 50¢
    CurrencyUnit{20},    // 20¢
    CurrencyUnit{10},    // 10¢
    CurrencyUnit{5},     // 5¢
    CurrencyUnit{2},     // 2¢
    CurrencyUnit{1}      // 1¢
};

[[nodiscard ]]auto calculate_tokens(CurrencyUnit amount, std::span<const CurrencyUnit> denominations) -> int
{
  auto tokens_used = 0;

  for (auto token : denominations)
  {
    tokens_used += amount / token;
    amount %= token;
  }

  return tokens_used;
}

int main()
{
  auto amount_owed = CurrencyUnit{};

  std::print("Amount owed in lowest token value: ");
  std::cin >> amount_owed;

  if (std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(1 << 10, '\n');
    std::println("Invalid input.");
    return 1;
  }

  auto t = StopWatch("CalculationTime");

  std::println("US tokens used: {}", calculate_tokens(amount_owed, Default_US_Currency));
  std::println("EU tokens used: {}", calculate_tokens(amount_owed, Default_EU_Currency));

  return 0;
}
