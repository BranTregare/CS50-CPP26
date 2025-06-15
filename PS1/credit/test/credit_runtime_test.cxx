#include <catch2/catch_test_macros.hpp>

#include "test_util.hxx"  // Your helper macros or test types

using credit::CardType;
using credit::test::test_validate;

TEST_CASE("Valid card numbers pass Luhn and match card type", "[valid]")
{
  SECTION("VISA 16-digit")
  {
    REQUIRE(test_validate(4111111111111111) == CardType::VISA);
  }

  SECTION("AMEX")
  {
    REQUIRE(test_validate(378282246310005) == CardType::AMEX);
  }

  SECTION("MASTERCARD")
  {
    REQUIRE(test_validate(5105105105105100) == CardType::MASTERCARD);
  }

  SECTION("VISA 13-digit")
  {
    REQUIRE(test_validate(4222222222222) == CardType::VISA);
  }
}

TEST_CASE("Invalid cards fail due to checksum", "[invalid][luhn]")
{
  REQUIRE(test_validate(4111111111111112) == CardType::INVALID);
  REQUIRE(test_validate(378282246310004) == CardType::INVALID);
}

TEST_CASE("Invalid cards fail due to length or unsupported prefix", "[invalid][prefix]")
{
  REQUIRE(test_validate(6011111111111117) == CardType::INVALID);  // Discover not supported
  REQUIRE(test_validate(123) == CardType::INVALID);
  REQUIRE(test_validate(0) == CardType::INVALID);
  //  NOTE: This test is commented out because the input exceeds the internal digit buffer size.
  //  With -Werror and constexpr validation, this is caught at compile time.
  //  Uncommenting this test may cause compilation to fail.
  //  NOTE: This test is commented out because the input exceeds the internal digit buffer size.
  //  With -Werror and constexpr validation, this is caught at compile time.
  //  Uncommenting this test may cause compilation to fail.
  //  REQUIRE(test_validate(41111111111111112222) == CardType::INVALID); // Too long
}