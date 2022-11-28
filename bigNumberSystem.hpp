#define ll long long int
#define mybase 100000000
#define M 125
#include <iostream>
#include <math.h>
class num
{
    /*
    M is the number of blocks of digits after and before the decimal point
    each block contains a number between 0 and 9999,9999 (inclusive) i.e.  8 digits
    negetive numbers are stored in 1 0000 0000 's compliment form
    for example if M = 2, the number 1234567890.0987654321 will be stored as
        (0000 0012) (3456 7890) (0987 6543) (2100 0000)
    and -1234567890.0987654321 will be stored as
        (9999 9987) (6543 2109) (9012 3456) (7900 0000)
    */
public:
    ll digits[2 * M];

    //  useful constructors

    num();                 // a default constrctor to instantiate numbers (to zero)
    num(const char arr[]); // first to  create numbers with a large number of digits , using  char input
    num(ll integer);       // another one to make numbes from int

    // print statements too!
    void print()const;

    // usual operators and methods
    bool signum() const; // returns 1 if the number is positive, 0 if neg
    num abs()const;     // returns the absolute value of the number
    void negate(); // negates the number object

    void operator=(const num &b);

    bool operator==(const num &b)const;
    bool operator!=(const num &b)const;

    bool operator>(const num &b)const;
    bool operator>=(const num &b)const;
    bool operator<(const num &b)const;
    bool operator<=(const num &b)const;

    num operator+(const num &b)const;
    num operator-(const num &b)const;
    num operator-()const;
    num operator*(const num &b)const;
    num rec() const;
    num operator/(const num &b) const;
};
num rec(int i);
bool signum(const num &b) ;
num rec(num x) ;

/* now we define class methods*/

num ::num()
{
    for (int i = 0; i < 2 * M; i++)
    {
        digits[i] = 0; // set all values to zero;
    }
}
bool num::signum() const
{
    return digits[0] < mybase / 2;
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
        digits[M - 1 - i] = integer % mybase;
        integer /= mybase;
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
    while (exponent != mybase)
    {
        ans *= 10;
        exponent *= 10;
    }
    return (ll)ans;
}

void num ::negate()
{
    //  convert the input to  the mybase compliment of itself

    int i = 2 * M - 1;
    while (!digits[i] && i >= 0) // skip last zeroes
        i--;
    digits[i] = mybase - digits[i];
    i--;
    while (i >= 0)
    {
        digits[i] = mybase - digits[i] - 1;
        i--;
    }
}
num num::abs() const
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
void num ::print() const
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

/* now we define operators */

void num::operator=(const num &b)
{
    for (int i = 0; i < M; i++)
    {
        digits[i] = b.digits[i];
        digits[i + M] = b.digits[i + M];
    }
}
bool num::operator==(const num &b)const
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
bool num::operator!=(const num &b)const
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
bool num::operator>(const num &b)const
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
bool num::operator>=(const num &b) const
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
bool num::operator<=(const num &b)const
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
bool num::operator<(const num &b) const
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

num num::operator-() const
{
    num ans;
    int i = 2 * M - 1;
    while (!digits[i] && i >= 0)
        i--;
    ans.digits[i] = mybase - digits[i];
    i--;
    while (i >= 0)
    {
        ans.digits[i] = mybase - digits[i] - 1;
        i--;
    }
    return ans;
}

num num::operator+(const num &b) const
{
    num ans;
    ll carry = 0;
    for (int i = 2 * M - 1; i >= 0; i--)
    {
        ans.digits[i] = (carry + b.digits[i] + digits[i]) % mybase;
        carry = (carry + b.digits[i] + digits[i]) >= mybase;
    }
    return ans;
}
num num::operator-(const num &b) const
{
    num ans;
    ll carry = 0;
    for (int i = 2 * M - 1; i >= 0; i--)
    {
        ans.digits[i] = (digits[i] - b.digits[i] - carry + mybase) % mybase;
        carry = digits[i] < (b.digits[i] + carry);
    }
    return ans;
}

num num::operator*(const num &c) const
{
    /* for multiplication we  multiply every pair and then group according to exponent */
    // multiplication is the most used operation so it must be as fast as possible

    //  to deal with negative numbers just convert all number to be positive
    num b(c);
    num a(*this);
    bool sign = a.signum() == b.signum(); // variable to store the sign of the final ans
    
    
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
        ans[i - 1] += ans[i] / mybase;
        ans[i] = ans[i] % mybase;
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
num num::rec() const
{
    // take care of negative numbers
    num a(*this);
    if (!a.signum())
        a.negate();
    num guess;
    // now the real chalenge is to make a smart guess for the reciprocal
    // the exponent of the ans is the negative of the exponent of the given number

    int r = 0;
    while (r < 2 * M && !a.digits[r])
        r++;
    if (a.digits[r] == 0 || a.digits[0] > mybase / 4) // handle division by zero and infinity
        return guess;
    guess.digits[2 * M - 1 - r] = mybase / a.digits[r];
    num two(2); // use this number quite often so better to store it than to generate it everytime
    // the approximation converges quadratically so we only need to do at most log2(8M) iterations

    /*
    the logic is, say we know an approximation of 1/x call it y, then we can find a better
    approximation y2 = y+d ( d is the small error ).
    we have 1/(y+d) = x
    -> 1/(1+(d/y)) = xy
    -> 1 - d/y = xy                         using the tailor exp of (1+d/y)^-1 and d/y <<< 1
    -> d = y - y*yx
    now y2 = y + d = 2y - y*xy
    */
    int logM8 = log2(8 * M) + 2;
    for (int k = 0; k < logM8; k++)
    {
        guess = guess * (two - a * guess);
    }
    if ((*this).signum())
        return guess;
    guess.negate();
    return guess;
}

// now we can finally impliment the division
num num::operator/(const num &b) const
{
    return (*this) * (b.rec());
}

// it would be useful if we could implement the reciprocal in a better way
// for calculating pi we only need the reciprocals of the integer
num rec(int x)
{
    if (x < 0)
    {
        return -rec(-x);
    }
    else if (x == 1)
    {
        return num(1);
    }
    else if (x > 0)
    {
        num ans;
        ll remainder = 1;
        for (int i = M; i < 2 * M; i++)
        {
            ans.digits[i] = (remainder * mybase) / x;
            remainder = (remainder * mybase) % x;
        }
        return ans;
    }
    return num();
}

/* now some useful math functions */

// square root function

num sqrt(const num &x)
{

    /*
    say we have an approximation for sqrt(x) call it y. let y2 = y + d be a better approximation
    where d is the small error
    then,
            ( y + d )^2 = x
        ->  yy + 2dy + dd = x
        assuming d to be small dd can be ignored
        ->  yy + 2dy = x
        ->  d = (x - yy)/2y
        so y2 = y + (x-yy)/2y
        the above form has lots of multiplications so we modify it.
        -> y2 = 0.5 (y + x/y)

        */
    num guess, half = num("0.5");
    int k = 0;
    if (x <= num("0")) /*edge case*/
        return num("0");

    /*
        now how to find a good approximation to begin with?
        finding the exponent and halfing it looks like a good idea
        in our array indexing  the index (i) of the first non zero digit
        is related to the exponent (ex) as
            i = ex + M-1
    */
    int i = 0;
    /* find the first non zero digit's index i*/
    while (i < 2 * M && !x.digits[i])
    {
        i++;
    }
    if ((i - M + 1) % 2 == 0)
        /* case when the exponent is even */
        guess.digits[(i + M - 1) / 2] = sqrt(x.digits[i]);
    else
        /* if the exponent is odd */
        guess.digits[(i + M + 1) / 2] = sqrt(x.digits[i] * mybase + x.digits[i + 1]);

    /*
        since we are starting with a very good approximation and the seq of guesses converges
        quadratically we only ever need to do log2(8M) iterations
    */
    int logM8 = log2(8 * M) + 2;
    for (i = 0; i < logM8; i++)
    {
        guess = (guess + x * (guess.rec())) * half;
    }

    return guess;
}

num arctan(const num &x)
{
    num xx = x * x;
    num ans;
    num zero;
    num term(x);
    for (int i = 1; term != zero; i += 2)
    {
        if (i & 2)
            ans = ans - (rec(i)) * term;
        else
            ans = ans + (rec(i)) * term;
        term = xx * term;
    }
    return ans;
}