#include <iostream>
#include "numberSystem.hpp"

num ::num()
{
    for (int i = 0; i < 2 * M; i++)
    {
        digits[i] = 0; // set all values to zero;
    }
}
bool num::signum()
{
    return digits[0] < base / 2;
}

num ::num(ll integer)
{
    // if the number is negative take absolute value
    bool sign = integer < 0;
    if (sign)
        integer = -integer;

    // digits after the decimal = zero and before decimal are set one by one
    for (int i = 0; i < M; i++)
    {
        digits[i + M] = 0;
        digits[M - 1 - i] = integer % base;
        integer /= base;
    }

    if (sign)
        (*this).negate(); // if number was negative then negate it
}
ll charToNum(const char str[/*length <= 8*/], int length)
{
    int ans = 0;
    int exponent = 1;
    // converts an 8 char array to a single long long integer.
    for (int i = length - 1; i >= 0; i--)
    {
        ans += (str[i] - '0') * exponent;
        exponent *= 10;
    }
    return (ll)ans;
}
ll charToNum2(const char str[/*length <= 8*/], int length)
{
    int ans = 0;
    int exponent = 1;
    // converts an 8 char array to a single long long integer.
    int i = length - 1;
    for (; i >= 0; i--)
    {
        ans += (str[i] - '0') * exponent;
        exponent *= 10;
    }
    while (exponent != base)
    {
        ans *= 10;
        exponent *= 10;
    }
    return (ll)ans;
}

void num ::negate()
{
    //  convert the input to  the base compliment of itself

    int i = 2 * M - 1;
    while (!digits[i] && i >= 0) // skip last zeroes
        i--;
    digits[i] = base - digits[i];
    i--;
    while (i >= 0)
    {
        digits[i] = base - digits[i] - 1;
        i--;
    }
}
num num::abs()
{
    num ans(*this);
    if (ans.signum())
        ans.negate();
    return ans;
}
num ::num(const char number[])
{
    int totalLength = 0, digitsBeforeDecimal = 0, positive = number[0] != '-';
    if (!positive)
        number = number + 1; // to skip the '-' symbol

    while (number[totalLength] != '\0')
    {
        totalLength++;
    }

    while (number[digitsBeforeDecimal] != '\0' && number[digitsBeforeDecimal] != '.')
        digitsBeforeDecimal++;

    // first deal with the digits before decimal
    int i = M - 1, j = digitsBeforeDecimal;
    while (i >= 0 && j > 8) // break string into parts of length 8 and convert one by one
    {
        digits[i] = charToNum(number + j - 8, 8);
        j -= 8;
        i--;
    }
    digits[i] = charToNum(number, j);
    // set rest of the digits before decimal to 0
    i--;
    while (i >= 0)
    {
        digits[i] = 0;
        i--;
    }

    // now the digits after the decimal
    int digitsAfterDecimal = totalLength - digitsBeforeDecimal;

    if (totalLength != digitsBeforeDecimal)
        digitsAfterDecimal--; // for "1.2" totalLength = 3, beforeDecimal = 1

    i = M;
    j = digitsBeforeDecimal;
    while (i < 2 * M && digitsAfterDecimal > 8)
    {
        digits[i] = charToNum(number + j + 1, 8);
        i++;
        digitsAfterDecimal -= 8;
        j += 8;
    }
    digits[i] = charToNum2(number + j + 1, digitsAfterDecimal);
    i++;
    // set the remaining digits to 0
    while (i < 2 * M)
    {
        digits[i] = 0;
        i++;
    }

    if (!positive)
        (*this).negate();
}
void print_helper(ll number)
{
    // this function prints the digits properly . say the digit is 123 then we print 00000123
    int arr[8];
    for (int i = 7; i >= 0; i--)
    {
        arr[i] = number % 10;
        number /= 10;
    }
    for (int i = 0; i < 8; i++)
        std::cout << arr[i];
}
void num ::print()
{
    num toPrint(*this); // using the default copy constructor
    // deal with the negative case
    if (!toPrint.signum())
    {
        std::cout << "-";
        toPrint.negate(); // nagates the value inside the variable
    }

    // skip leading zeros
    int i = 0;
    while (!toPrint.digits[i] && i < M - 1)
        i++;
    // print first non zero digits
    std ::cout << toPrint.digits[i];
    i++;
    // print rest of the digits before decimal
    while (i < M)
    {
        std::cout << " ";
        print_helper(toPrint.digits[i]);
        i++;
    }
    // avoid trailing zeroes
    int t = 2 * M;
    while (!toPrint.digits[t - 1] && t >= M)
        t--;

    // if t == M there is no need to print the decimal point
    if (t != M)
    {
        std::cout << ".";
        while (i < t)
        {
            print_helper(toPrint.digits[i]);
            std::cout << ' ';
            i++;
        }
    }
    std::cout << std::endl;
}
