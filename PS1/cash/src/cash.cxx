#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <span>
#include <string>

#include "include/stopwatch.hxx"

// Type alias for the smallest currency unit (e.g., cents or eurocents)
using CurrencyUnit = std::uint32_t;

// US currency denominations in cents, largest to smallest
constexpr auto US_Currency = std::array{
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

// EU currency denominations in eurocents, largest to smallest
constexpr auto EU_Currrency = std::array{
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

// Compute the minimum number of currency tokens required to represent a given amount
[[nodiscard]] constexpr auto calculate_tokens(
    CurrencyUnit Amount,
    std::span<const CurrencyUnit> Denominations
) -> std::uint16_t
{
    auto Tokens_Used = std::uint16_t{0};

    for (auto Token : Denominations) {
        Tokens_Used += Amount / Token;
        Amount %= Token;
    }

    return Tokens_Used;
}

auto main() -> int
{
    auto Amount_Owed = CurrencyUnit{};

    std::print("Amount owed in lowest token value: ");
    std::cin >> Amount_Owed;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1 << 10, '\n');
        std::println("Invalid input.");
        return 1;
    }

    auto t = StopWatch("CalculationTime");

    std::println("US tokens used: {}", calculate_tokens(Amount_Owed, US_Currency));
    std::println("EU tokens used: {}", calculate_tokens(Amount_Owed, EU_Currrency));

    return 0;
}
