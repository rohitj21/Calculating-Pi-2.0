#define N 5
#define ll long long
#define M 5000
#define baselength 10
#define mybase 10000000000
/*
    be careful , use mybase > 10e8 only if you never use multipliction/division by num object
    moreover if you use multipy/division by num then N~M for proper results
*/
#include <iostream>
#include <math.h>
#include <vector>
class num
{
    /*
    say mybase = 100000000, baselength = 8;
    m and M is the number of blocks of digits after and before the decimal point
    each block contains a number between 0 and 9999,9999 (inclusive) i.e.  8 digits
    negetive numbers are stored in 1 0000 0000 's compliment form
    for example if m=M = 2, the number 1234567890.0987654321 will be stored as
        (0000 0012) (3456 7890) (0987 6543) (2100 0000)
    and -1234567890.0987654321 will be stored as
        (9999 9987) (6543 2109) (9012 3456) (7900 0000)
    */
    
public:
    std ::vector<ll> digits;
    //  useful constructors

    num();                 // a default constrctor to instantiate numbers (to zero)
    num(const num& b);           // copy constructor
    num(const char arr[]); // first to  create numbers with a large number of digits , using  char input
    num(ll integer);       // another one to make numbes from int
    ~num();
    // print statements too!
    void print() const;

    // usual operators and methods
    bool signum() const; // returns 1 if the number is positive, 0 if neg
    num abs() const;     // returns the absolute value of the number
    void negate();       // negates the number object

    num& operator=(const num &b);

    bool operator==(const num &b) const;
    bool operator!=(const num &b) const;

    bool operator>(const num &b) const;
    bool operator>=(const num &b) const;
    bool operator<(const num &b) const;
    bool operator<=(const num &b) const;

    num operator+(const num &b) const;
    num operator-(const num &b) const;
    num operator-() const;
    num operator*(const num &b) const;
    num operator*(ll b) const;
    num rec() const;
    num operator/(const num &b) const;
    num operator/(ll b) const;
};
ll charToNum(const char str[/*length <= 8*/], int length);
ll charToNum2(const char str[/*length <= 8*/], int length);
void print_helper(ll number);
num int_rec(ll x);
num sqrt(const num &x);
num arctan(const num &x);
num arctanOfReciprocal(ll x);

/* now define class methods*/

num ::num()
{
    digits.resize(N + M, 0);
}
num::~num()
{
}
num ::num(const num& b){
    digits.resize(N + M, 0);
    for(int i =0; i<N+M; i++){
        digits[i] = b.digits[i];
    }
}
bool num::signum() const
{
    return digits[0] < mybase / 2;
}

num ::num(ll integer)
{
    digits.resize(N + M, 0);
    // if the number is negative take absolute value
    bool sign = integer < 0;
    if (sign)
        integer = -integer;

    // digits after the decimal = zero and before decimal are set one by one
    for (int i = 0; i < N; i++)
    {
        digits[N - 1 - i] = integer % mybase;
        integer /= mybase;
    }
    for (int i = N; i < N + M; i++)
    {
        digits[i] = 0;
    }

    if (sign)
        this->negate(); // if number was negative then negate it
}
ll charToNum(const char str[/*length <= baselength*/], int length)
{
    ll ans = 0;
    ll exponent = 1;
    // converts an baselength char array to a single long long integer.
    for (int i = length - 1; i >= 0; i--)
    {
        ans += (str[i] - '0') * exponent;
        exponent *= 10;
    }
    return ans;
}
ll charToNum2(const char str[/*length <= baselength*/], int length)
{
    ll ans = 0;
    ll exponent = 1;
    // converts an baselength char array to a single long long integer.
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
    return ans;
}

void num ::negate()
{
    //  convert the input to  the mybase compliment of itself

    int i = N + M - 1;
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
    digits.resize(N + M, 0);
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
    int i = N - 1, j = digitsBeforeDecimal;
    while (i >= 0 && j > baselength) // break string into parts of length 8 and convert one by one
    {
        digits[i] = charToNum(number + j - baselength, baselength);
        j -= baselength;
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

    i = N;
    j = digitsBeforeDecimal;
    while (i < N + M && digitsAfterDecimal > baselength)
    {
        digits[i] = charToNum(number + j + 1, baselength);
        i++;
        digitsAfterDecimal -= baselength;
        j += baselength;
    }
    digits[i] = charToNum2(number + j + 1, digitsAfterDecimal);
    i++;
    // set the remaining digits to 0
    while (i < N + M)
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
    int arr[baselength];
    for (int i = baselength - 1; i >= 0; i--)
    {
        arr[i] = number % 10;
        number /= 10;
    }
    for (int i = 0; i < baselength; i++)
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
    while (!toPrint.digits[i] && i < N - 1)
        i++;
    // print first non zero digits
    std ::cout << toPrint.digits[i];
    i++;
    // print rest of the digits before decimal
    while (i < N)
    {
        std::cout << " ";
        print_helper(toPrint.digits[i]);
        i++;
    }
    // avoid trailing zeroes
    int t = N + M;
    while (!toPrint.digits[t - 1] && t >= N)
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

num& num::operator=(const num &b)
{
    for (int i = 0; i < M + N; i++)
    {
        digits[i] = b.digits[i];
    }
    return *this;
}
bool num::operator==(const num &b) const
{
    // while calculating pi we will often compare two very close number
    // so it makes more sense to start comparing from the end digits
    for (int i = N + M - 1; i >= 0; i--)
    {
        if (digits[i] != b.digits[i])
        {
            return false;
        }
    }
    return true;
}
bool num::operator!=(const num &b) const
{
    for (int i = N + M - 1; i >= 0; i--)
    {
        if (digits[i] != b.digits[i])
        {
            return true;
        }
    }
    return false;
}
bool num::operator>(const num &b) const
{
    // same logic follows is both of them have same sign
    if (this->signum() == b.signum())
    {
        for (int i = 0; i < N + M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] > b.digits[i];
            }
        }
        return false;
    }

    return this->signum();
}
bool num::operator>=(const num &b) const
{
    // same logic follows is both of them have same sign
    if (this->signum() == b.signum())
    {
        for (int i = 0; i < N + M; i++)
        {
            if (digits[i] != b.digits[i])
            {
                return digits[i] > b.digits[i];
            }
        }
        return true;
    }

    return this->signum();
}
bool num::operator<=(const num &b) const
{
    // could be implemented simply like this but i guess the it will take more time for copying and accessing data
    // return !((*this) > b);

    if (this->signum() == b.signum())
    {
        for (int i = 0; i < N + M; i++)
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
    if (this->signum() == b.signum())
    {
        for (int i = 0; i < N + M; i++)
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
    int i = N + M - 1;
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
    for (int i = N + M - 1; i >= 0; i--)
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
    for (int i = N + M - 1; i >= 0; i--)
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
    ll *ans = new ll[2 * (N + M)];
    for (int i = 0; i < 2 * (N + M); i++)
    {
        ans[i] = 0;
    }

    for (int i = 0; i < N + M; i++)
    {
        // no need to waste time multipying with zeros
        if (!b.digits[i])
            continue;

        for (int j = 0; j < N + M; j++)
        {
            ans[i + j + 1] += a.digits[j] * b.digits[i];
        }
    }

    // now  modify the digits of ans appropriately
    for (int i = 2 * (N + M) - 1; i > 0; i--)
    {
        ans[i - 1] += ans[i] / mybase;
        ans[i] = ans[i] % mybase;
    }
    // now set the answer variable
    num prod;
    for (int i = 0; i < N + M; i++)
    {
        prod.digits[i] = ans[N + i];
    }
    delete[] ans;
    if (sign)
        return prod;
    prod.negate();
    return prod;
}
// multiplication by an integer should ideally take linear time
num num::operator*(ll x) const
{
    num ans;
    if (this->signum())
    {
        if (x > 0)
        {
            ll carry = 0;
            for (int i = N + M - 1; i >= 0; i--)
            {
                ans.digits[i] = (digits[i] * x + carry) % mybase;
                carry = (digits[i] * x + carry) / mybase;
            }
            return ans;
        }
        else if (x < 0)
        {
            return *this * (-x);
        }
    }
    else
    {
        // if number is negative

        /* we have to handle an edge case
        say M = 2
        then the number -inf = 50000000 00000000 is produces a problem
        i am calling it -inf because it is the smallest number in our representation
        the problem is that it is its own negative so if we just write

            return -((-(*this))*(x));
            it will go into an infinite recursion

        */
        num infinity;
        infinity.digits[0] = mybase / 2;

        if (*this != infinity)
        {
            num newNumber(*this);
            newNumber.negate();
            return -(newNumber * x);
        }
    }
    /*return num(0) when x == 0 or num == infinity*/
    return num();
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
    while (r < N + M && !a.digits[r])
        r++;
    if (a.digits[r] == 0 || a.digits[0] > mybase / 4) // handle division by zero and infinity
        return guess;
    guess.digits[N + M - 1 - r] = mybase / a.digits[r];
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
    int logM8 = log2(baselength * (M + N)) + 2;
    for (int k = 0; k < logM8; k++)
    {
        guess = guess * (two - a * guess);
    }
    if (this->signum())
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
num int_rec(ll x)
{
    if (x < 0)
    {
        return -int_rec(-x);
    }
    else if (x == 1)
    {
        return num(1);
    }
    else if (x > 0)
    {
        num ans;
        ll remainder = 1;
        for (int i = N; i < N + M; i++)
        {
            ans.digits[i] = (remainder * mybase) / x;
            remainder = (remainder * mybase) % x;
        }
        return ans;
    }
    return num();
}

// moreover a function which divides a given number by an integer in linear time would make the calculations even faster
num num::operator/(ll x) const
{
    /*if the number is positive */
    if (this->signum())
    {
        /* x is negative*/
        if (x < 0)
        {
            num negnumber = -(*this);
            return -(negnumber / (-x));
        }
        else if (x == 1)
        {
            return *this;
        }
        else if (x > 1)
        {
            num ans;
            ll remainder = 0;
            for (int i = 0; i < N + M; i++)
            {
                ans.digits[i] = (remainder * mybase + digits[i]) / x;
                remainder = (remainder * mybase + digits[i]) % x;
            }
            return ans;
        }
    }
    else
    {
        /* we have to handle an edge case
        say M = 2
        then the number -inf = 50000000 00000000 is produces a problem
        i am calling it -inf because it is the smallest number in our representation
        the problem is that it is its own negative so if we just write

            return -((-(*this)).int_div(x));
            it will go into an infinite recursion

        */
        num infinity;
        infinity.digits[0] = mybase / 2;

        if (*this != infinity)
        {
            num newNumber(*this);
            newNumber.negate();
            return -(newNumber / x);
        }
    }
    /*return 0 if x is zero or the given num is infinity*/
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
    num guess;
    if (x <= num()) /*edge case*/
        return num();

    /*
        now how to find a good approximation to begin with?
        finding the exponent and halfing it looks like a good idea
        in our array indexing  the index (i) of the first non zero digit
        is related to the exponent (ex) as
            i = ex + M-1
    */
    int i = 0;
    /* find the first non zero digit's index i*/
    while (i < N + M && !x.digits[i])
    {
        i++;
    }
    if ((i - N + 1) % 2 == 0)
        /* case when the exponent is even */
        guess.digits[(i + N - 1) / 2] = sqrt(x.digits[i]);
    else
        /* if the exponent is odd */
        guess.digits[(i + N + 1) / 2] = sqrt(x.digits[i] * mybase + x.digits[i + 1]);

    /*
        since we are starting with a very good approximation and the seq of guesses converges
        quadratically we only ever need to do log2(8M) iterations
    */
    int logM8 = log2(baselength * (N + M)) + 2;
    for (i = 0; i < logM8; i++)
    {
        guess = (guess + (guess.rec()) * x) / 2;
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
            ans = ans - term / i;
        else
            ans = ans + term / i;
        term = xx * term; /* this multiplication is O(n^2) */
    }
    return ans;
}

/* exploit the fact that we only need the arctan of the reciprocals of integers*/
num arctanOfReciprocal(ll x)
{

    ll xx = x * x;
    num ans, zero;
    num term(int_rec(x));

    for (int i = 1; term != zero; i += 2)
    {
        if (i & 2)
            ans = ans - term / i;
        else
            ans = ans + term / i;
        term = term / xx; /* this  is O(n) */
    }
    return ans;
}
