/* this file contains some useful math functions */
#include "numberSystemOperators.hpp"

// square root function

num sqrt(num x)
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
        now how to find a good approximation to bigin with
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
        guess.digits[(i + M + 1) / 2] = sqrt(x.digits[i] * base + x.digits[i + 1]);

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

num arctan(num x)
{
    num xx = x * x;
    num ans;
    num zero;
    num term(x);
    for (int i = 1; term != zero; i += 2)
    {
        if (i & 2)
            ans = ans - (num(i).rec()) * term;
        else
            ans = ans + (num(i).rec()) * term;
        term = xx * term;
    }
    return ans;
}

