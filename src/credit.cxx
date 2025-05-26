#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace credit
{
// type definitions
enum class CardType
{
  AMEX,
  MASTERCARD,
  VISA,
  INVALID
};
// Map cardtype to std::string_view for printing
auto Card_Type_To_String_View = std::unordered_map<const CardType, const std::string_view>{
    {CardType::AMEX, "AMEX"},
    {CardType::MASTERCARD, "MASTERCARD"},
    {CardType::VISA, "VISA"},
    {CardType::INVALID, "INVALID"},
};
// helper named lambdas
// number of digits in a credit card number
auto num_digits = [](auto Number) {
  int Digits = 0;
  while (Number)
  {
    Number /= 10;
    Digits++;
  }
  return Digits;
};
// multiply a value by 2. used with std::transform
auto multiply_by_2 = [](auto Value) { return Value * 2; };
// sum by individual digits
auto sum_by_digits = [](auto Sum, auto Val) { return Val > 9 ? Sum + (Val - 9) : Sum + Val; };
// convert number to and array of std::uint8_t
inline auto vectorize_number = [](auto& Vector, auto Number) {
  while (Number != 0)
  {
    Vector.emplace_back(Number % 10);
    Number /= 10;
  }
};
//
inline auto stride = [](std::size_t StrideSize, auto ReturnFirst) {
  auto temp = -1;
  if (!ReturnFirst) temp = 0;
  return [s = temp, StrideSize](auto const&) mutable {
    s = (s + 1) % StrideSize;
    return s == 0 ? true : false;
  };
};
// validate creditcard luhn checksum
inline auto is_valid_checksum = [](auto Check_Sum) { return Check_Sum % 10 == 0 ? true : false; };
// helper functions
// validate cardnumber return credit:CardType
auto validate_card_number(auto Card_Number)
{
  // count number of digits in a long long;
  auto Card_Num_Digits = num_digits(Card_Number);
  // create vector of std::uint8_t of length Card_Num_Digits and reserve space
  // cannot use std::array as length is not known at compile time => std::vector
  std::vector<std::uint8_t> Card_Num_Reversed;
  Card_Num_Reversed.reserve(Card_Num_Digits);
  // transpose all digits in the Card_Number into Card_Num_Reversed
  vectorize_number(Card_Num_Reversed,
                   Card_Number); // Number vectorized. NB number is reversed. last Digit of CardNumber first.
  // create reversed view of Card_Number_Reversed. i.e. natural order
  auto Card_Num = Card_Num_Reversed | std::views::reverse;
  // gather elements using views
  auto Mul_Sum = Card_Num_Reversed                              //
                 | std::views::filter(credit::stride(2, false)) //
                 | std::views::transform(multiply_by_2);
  auto Only_Sum = Card_Num_Reversed //
                  | std::views::filter(credit::stride(2, true));
  auto Check_Sum =
      // Accumulate Mul_Sum by digits given 1,2,12: 1+2+1+2 = 6 and not 1+2+12 = 15
      std::accumulate(Mul_Sum.begin(), Mul_Sum.end(), 0, sum_by_digits) +
      // Accumulate Only_Sum
      std::accumulate(Only_Sum.begin(), Only_Sum.end(), 0);
  // Calculate CheckSum
  if (is_valid_checksum(Check_Sum)) // Now check type of card, AMEX, MASTERCARD,VISA
  {
    switch (Card_Num_Digits)
    {
    case 13:                // POSSIBLE VISA CARD
      if (Card_Num[0] == 4) // first digit is 4 valid VISA Card;
        return credit::CardType::VISA;
      break;
    case 15:                // POSSIBLE AMEX CARD
      if (Card_Num[0] == 3) // First digit
      {
        if (Card_Num[1] == 4 || Card_Num[1] == 7) // Second Digit
        {
          return credit::CardType::AMEX;
        }
      }
      break;
    case 16:                // POSSIBLE VISA or MASTERCARD
      if (Card_Num[0] == 4) // VISA CARD
      {
        return credit::CardType::VISA;
      }
      else // MASTERCARD CHECK
      {
        if (Card_Num[0] == 5) // First Digit
        {
          switch (Card_Num[1])
          {
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
            return credit::CardType::MASTERCARD;
          default:
            break;
          }
        }
      }
      break;
    default:
      return credit::CardType::INVALID;
    }
  }
  return credit::CardType::INVALID;
}
} // namespace credit

int main(void)
{
  auto Card_Number = 0LL;

  // get input from user
  do {
    std::print("Number: ");
    std::string User_Input_String;
    std::getline(std::cin, User_Input_String);
    if (std::find_if_not(User_Input_String.begin(),              //
                         User_Input_String.end(),                //
                         [](char c) { return std::isdigit(c); }) //
        == User_Input_String.end()) // User_Input_String: Are all digits? i.e 4003600000000014 vs 4003-6000-0000-0014
    {
      std::stringstream Long_Stream(User_Input_String);
      if (Long_Stream >> Card_Number) // this is a number break out of while loop
      {
        break;
      }
      Card_Number = -1; // not a number
    }
    Card_Number = -1; // not a number
  } while (Card_Number < 0);
  std::println("{}", credit::Card_Type_To_String_View.at(credit::validate_card_number(Card_Number)));
}