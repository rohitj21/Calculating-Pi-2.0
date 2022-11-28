 #include "bigNumberSystem.hpp"
int main()
{
    clock_t tStart = clock(); /* to calculate time */

    /*
    to calculate pi we use formulas involving tan^-1 (x)
    a simple  example is :  arctan(1) = pi/4
    we calculate arctan(1) * 4 which gives pi
    
    a diffierent formula involving arctan of smaller numbers than 1 is used here for better performance 
    as the tailor series converges more quickly for smaller x.

    arctan(x) = x - x^3 /3 + x^5 /5 ....

    pi = 16*arctan(1/5) - 4*arctan(1/239)
    
    */


    num pi = arctan(int_rec(5)) * num(16) - arctan(int_rec(239)) * num(4);
    
    //pi.print();

    printf("Time taken: %.6fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;

}