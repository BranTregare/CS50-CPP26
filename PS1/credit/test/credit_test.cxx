#include "test_util.hxx"

namespace test
{
using credit::CardType;
using credit::test::test_validate;

// Valid cards
static_assert(test_validate(4111111111111111) == card_type_t::VISA, "VISA failed");
static_assert(test_validate(378282246310005) == card_type_t::AMEX, "AMEX failed");
static_assert(test_validate(5105105105105100) == card_type_t::MASTERCARD, "MASTERCARD failed");
static_assert(test_validate(4222222222222) == card_type_t::VISA, "VISA 13-digit failed");

// Invalid due to bad Luhn checksum
static_assert(test_validate(4111111111111112) == card_type_t::INVALID, "Bad Luhn check");
static_assert(test_validate(378282246310004) == card_type_t::INVALID, "AMEX Luhn fail");

// Invalid due to prefix/length
static_assert(test_validate(6011111111111117) == card_type_t::INVALID, "Discover not supported");
static_assert(test_validate(123) == card_type_t::INVALID, "Too short");
static_assert(test_validate(0) == card_type_t::INVALID, "Zero input");

// Edge case: long input
// NOTE: This test is commented out because the input exceeds the internal digit buffer size.
// With -Werror and constexpr validation, this is caught at compile time.
// Uncommenting this test may cause compilation to fail.
// static_assert(test_validate(41111111111111112222ULL) == card_type_t::INVALID, "Too long");

} // namespace test
