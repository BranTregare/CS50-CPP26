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
auto CardTypeToStringView = std::unordered_map<const CardType, const std::string_view>{
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
inline auto has_valid_checksum = [](auto CheckSum) { return CheckSum % 10 == 0 ? true : false; };
// helper functions
// validate cardnumber return credit:CardType
auto validate_card_number(auto CardNumber)
{
  // count number of digits in a long long;
  unsigned int CardNumDigits = num_digits(CardNumber);
  // create array of int of length NumberOfDigitsInCardNumber
  std::vector<std::uint8_t> CardNumberReversedArray;
  CardNumberReversedArray.reserve(CardNumDigits);
  // transpose all digits in the CardNumber into the vector
  vectorize_number(CardNumberReversedArray,
                  CardNumber); // Number vectorized. NB number is reversed. last Digit of CardNumber first.
  // copy ReversedArray and reverse. We get iriginal order. Needed for cardtype determination.
  std::vector<std::uint8_t> CardNumberArray;
  CardNumberArray.reserve(CardNumDigits);
  CardNumberArray = CardNumberReversedArray;
  std::reverse(CardNumberArray.begin(), CardNumberArray.end());
  // gather elements
  // MulSum
  auto ElementsToMulAndSum = std::views::filter(CardNumberReversedArray, credit::stride(2, false));

  // OnlySum
  auto ElementsToOnlySum = std::views::filter(CardNumberReversedArray, stride(2, true));
  // Modify using transform and MultiplyBy2;
  std::transform(ElementsToMulAndSum.begin(), ElementsToMulAndSum.end(), ElementsToMulAndSum.begin(),
                 multiply_by_2); // multiply elements by 2. NB inplace Multiply, we are modifying CardNumberReversedArray
  // Accumulate ElementsToMulAndSum by digits given 1,2,12: 1+2+1+2 = 6 and not 1+2+12 = 15
  auto MulSumValue = std::accumulate(ElementsToMulAndSum.begin(), ElementsToMulAndSum.end(), 0, sum_by_digits);
  // Accumulate ElementsToOnlySum
  auto OnlySumValue = std::accumulate(ElementsToOnlySum.begin(), ElementsToOnlySum.end(), 0
                                      );
  // Calculate CheckSum
  auto CheckSum = MulSumValue + OnlySumValue;
  if (has_valid_checksum(CheckSum)) // Now check type of card, AMEX, MASTERCARD,VISA
  {
    switch (CardNumDigits)
    {
    case 13:                       // POSSIBLE VISA CARD
      if (CardNumberArray[0] == 4) // first digit is 4 valid VISA Card;
        return credit::CardType::VISA;
      break;
    case 15:                       // POSSIBLE AMEX CARD
      if (CardNumberArray[0] == 3) // First digit
      {
        if (CardNumberArray[1] == 4 || CardNumberArray[1] == 7) // Second Digit
        {
          return credit::CardType::AMEX;
        }
      }
      break;
    case 16:                       // POSSIBLE VISA or MASTERCARD
      if (CardNumberArray[0] == 4) // VISA CARD
      {
        return credit::CardType::VISA;
      }
      else // MASTERCARD CHECK
      {
        if (CardNumberArray[0] == 5) // First Digit
        {
          switch (CardNumberArray[1])
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
  auto CardNumber = 0LL;

  // get input from user
  do {
    std::print("Number: ");
    std::string sCardNumber;
    std::getline(std::cin, sCardNumber);
    std::stringstream LongStream(sCardNumber);
    if (std::find_if_not(sCardNumber.begin(), sCardNumber.end(), [](char c) { return std::isdigit(c); }) ==
        sCardNumber.end()) // test sCardnumber: Are all digits? i.e 4003600000000014 vs 4003-6000-0000-0014
    {
      if (LongStream >> CardNumber) // this is a number break out of while loop
      {
        break;
      }
      CardNumber = -1; // not a number
    }
    CardNumber = -1; // not a number
  } while (CardNumber < 0);
  std::println("{}", credit::CardTypeToStringView.at(credit::validate_card_number(CardNumber)));
}