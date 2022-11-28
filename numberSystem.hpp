#define ll long long int
#define base 100000000
#define M 300
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
    ll digits[2*M];

    // these are the useful constructors 

    num();              // a default constrctor to instantiate numbers (to zero)
    num(const char arr[]);    // first to  create numbers with a large number of digits , using  char input
    num(ll integer);   // another one to make numbes from int

    
    // we need print statements too!
    void print();

    // usual operators and methods
    bool signum(); // returns 1 if the number is positive, -1 if neg, 0 is zero
    num abs(); // returns the absolute value of the number
    void negate(); // negates the number object

    void operator=(num b);

    bool operator==(num b);  
    bool operator!=(num b);

    bool operator>(num b);
    bool operator>=(num b);
    bool operator<(num b);
    bool operator<=(num b);

    num operator+(num b);
    num operator-(num b);
    num operator-();
    num operator*(num b);
    num rec();
    num operator/(num b);

};
