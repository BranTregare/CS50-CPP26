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
inline auto has_valid_checksum = [](auto Check_Sum) { return Check_Sum % 10 == 0 ? true : false; };
// helper functions
// validate cardnumber return credit:CardType
auto validate_card_number(auto Card_Number)
{
  // count number of digits in a long long;
  unsigned int Card_Num_Digits = num_digits(Card_Number);
  // create array of int of length NumberOfDigitsInCardNumber
  std::vector<std::uint8_t> Card_Number_Reversed_Array;
  Card_Number_Reversed_Array.reserve(Card_Num_Digits);
  // transpose all digits in the CardNumber into the vector
  vectorize_number(Card_Number_Reversed_Array,
                  Card_Number); // Number vectorized. NB number is reversed. last Digit of CardNumber first.
  // copy ReversedArray and reverse. We get iriginal order. Needed for cardtype determination.
  std::vector<std::uint8_t> Card_Number_Array;
  Card_Number_Array.reserve(Card_Num_Digits);
  Card_Number_Array = Card_Number_Reversed_Array;
  std::reverse(Card_Number_Array.begin(), Card_Number_Array.end());
  // gather elements
  // MulSum
  auto Elements_To_Mul_And_Sum = std::views::filter(Card_Number_Reversed_Array, credit::stride(2, false));
  // OnlySum
  auto Elements_To_Only_Sum = std::views::filter(Card_Number_Reversed_Array, stride(2, true));
  // Modify using transform and MultiplyBy2;
  std::transform(Elements_To_Mul_And_Sum.begin(), Elements_To_Mul_And_Sum.end(), Elements_To_Mul_And_Sum.begin(),
                 multiply_by_2); // multiply elements by 2. NB inplace Multiply, we are modifying CardNumberReversedArray
  // Accumulate ElementsToMulAndSum by digits given 1,2,12: 1+2+1+2 = 6 and not 1+2+12 = 15
  auto Mul_Sum_Value = std::accumulate(Elements_To_Mul_And_Sum.begin(), Elements_To_Mul_And_Sum.end(), 0, sum_by_digits);
  // Accumulate ElementsToOnlySum
  auto Only_Sum_Value = std::accumulate(Elements_To_Only_Sum.begin(), Elements_To_Only_Sum.end(), 0
                                      );
  // Calculate CheckSum
  auto Check_Sum = Mul_Sum_Value + Only_Sum_Value;
  if (has_valid_checksum(Check_Sum)) // Now check type of card, AMEX, MASTERCARD,VISA
  {
    switch (Card_Num_Digits)
    {
    case 13:                       // POSSIBLE VISA CARD
      if (Card_Number_Array[0] == 4) // first digit is 4 valid VISA Card;
        return credit::CardType::VISA;
      break;
    case 15:                       // POSSIBLE AMEX CARD
      if (Card_Number_Array[0] == 3) // First digit
      {
        if (Card_Number_Array[1] == 4 || Card_Number_Array[1] == 7) // Second Digit
        {
          return credit::CardType::AMEX;
        }
      }
      break;
    case 16:                       // POSSIBLE VISA or MASTERCARD
      if (Card_Number_Array[0] == 4) // VISA CARD
      {
        return credit::CardType::VISA;
      }
      else // MASTERCARD CHECK
      {
        if (Card_Number_Array[0] == 5) // First Digit
        {
          switch (Card_Number_Array[1])
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
    std::string Card_Number_string;
    std::getline(std::cin, Card_Number_string);
    std::stringstream Long_Stream(Card_Number_string);
    if (std::find_if_not(Card_Number_string.begin(), Card_Number_string.end(), [](char c) { return std::isdigit(c); }) ==
        Card_Number_string.end()) // test sCardnumber: Are all digits? i.e 4003600000000014 vs 4003-6000-0000-0014
    {
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