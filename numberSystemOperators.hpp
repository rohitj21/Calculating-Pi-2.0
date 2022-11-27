#include "numberSystemClassMethods.hpp"

void num::operator=(num b)
{
    for (int i = 0; i < M; i++)
    {
        digits[i] = b.digits[i];
        digits[i + M] = b.digits[i + M];
    }
}
bool num::operator==(num b)
{
    // while calculating pi we will often compare two very close number
    // so it makes more sense to start comparing from the end digits
    for (int i = M - 1; i >= 0; i--)
    {
        if (digits[i] != b.digits[i] || digits[i + M] != b.digits[i + M])
        {
            return false;
        }
    }
    return true;
}
bool num::operator!=(num b)
{
    for (int i = M - 1; i >= 0; i--)
    {
        if (digits[i] != b.digits[i] || digits[i + M] != b.digits[i + M])
        {
            return true;
        }
    }
    return false;
}
bool num::operator>(num b)
{
    // same logic follows is both of them have same sign
    if ((*this).signum() == b.signum())
    {
        for (int i = 0; i < 2 * M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] > b.digits[i];
            }
        }
        return false;
    }

    return (*this).signum();
}
bool num::operator>=(num b)
{
    // same logic follows is both of them have same sign
    if ((*this).signum() == b.signum())
    {
        for (int i = 0; i < 2 * M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] > b.digits[i];
            }
        }
        return true;
    }

    return (*this).signum();
}
bool num::operator<=(num b)
{
    // could be implemented simply like this but i guess the it will take more time for copying and accessing data
    // return !((*this) > b);

    if ((*this).signum() == b.signum())
    {
        for (int i = 0; i < 2 * M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] < b.digits[i];
            }
        }
        return true;
    }

    return b.signum();
}
bool num::operator<(num b)
{
    if ((*this).signum() == b.signum())
    {
        for (int i = 0; i < 2 * M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] < b.digits[i];
            }
        }
        return false;
    }

    return b.signum();
}

/* Now defining arithematic operators */

num num::operator-()
{
    num ans;
    int i = 2 * M - 1;
    while (!digits[i] && i >= 0)
        i--;
    ans.digits[i] = base - digits[i];
    i--;
    while (i >= 0)
    {
        ans.digits[i] = base - digits[i] - 1;
        i--;
    }
    return ans;
}

num num::operator+(num b)
{
    num ans;
    ll carry = 0;
    for (int i = 2 * M - 1; i >= 0; i--)
    {
        ans.digits[i] = (carry + b.digits[i] + digits[i]) % base;
        carry = (carry + b.digits[i] + digits[i]) >= base;
    }
    return ans;
}
num num::operator-(num b)
{
    num ans;
    ll carry = 0;
    for (int i = 2 * M - 1; i >= 0; i--)
    {
        ans.digits[i] = (digits[i] - b.digits[i] - carry + base) % base;
        carry = digits[i] < (b.digits[i] + carry);
    }
    return ans;
}

num num::operator*(num b)
{
    /* for multiplication we  multiply every pair and then group according to exponent */
    // multiplication is the most used operation so it must be as fast as possible

    //  to deal with negative numbers just convert all number to be positive
    bool sign = (*this).signum() == b.signum(); // variable to store the sign of the final ans
    num a(*this);
    if (!a.signum())
        a.negate();
    if (!b.signum())
        b.negate();

    // creating an empty array to hold the temporary result
    ll ans[4 * M];
    for (int i = 0; i < M; i++)
    {
        ans[i] = ans[i + M] = ans[i + 2 * M] = ans[i + 3 * M] = 0;
    }

    for (int i = 0; i < 2 * M; i++)
    {
        // no need to waste time multipying with zeros
        if (!b.digits[i])
            continue;

        for (int j = 0; j < 2 * M; j++)
        {
            ans[i + j + 1] += a.digits[j] * b.digits[i];
        }
    }

    // now  modify the digits of ans appropriately
    for (int i = 4 * M - 1; i > 0; i--)
    {
        ans[i - 1] += ans[i] / base;
        ans[i] = ans[i] % base;
    }
    // now set the answer variable
    num prod;
    for (int i = 0; i < 2 * M; i++)
    {
        prod.digits[i] = ans[M + i];
    }
    if (sign)
        return prod;
    prod.negate();
    return prod;
}
// for division we first need reciprocal
num num::rec()
{
    // take care of negative numbers
    num a(*this);
    if (!a.signum())
        a.negate();
    num guess, guess1;
    // now the real chalenge is to make a smart guess for the reciprocal
    // the exponent of the ans is the negative of the exponent of the given number

    int r = 0;
    while (r < 2 * M && !a.digits[r])
        r++;
    if (a.digits[r] == 0 || a.digits[0] > base/4) // handle division by zero and infinity
        return guess;                              
    guess.digits[2 * M - 1 - r] = base / a.digits[r]; // this is a decent guess { huge room for improvement }
    num two(2);                                     // use this number quite often so better to store it than to generate it everytime
    // the approximation converges quadratically so we only need to do at most 2*log2(M) iterations
    while (guess != guess1) // there is a potential chance of an infinite loop
    {
        guess1 = guess;
        guess = (two - a * guess1) * guess1;
    }
    if((*this).signum())
        return guess;
    guess.negate();
    return guess;
}

// now we can finally impliment the division
num num::operator/(num b){
    return (*this) * (b.rec());
}

