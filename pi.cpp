#include "mathfunctions.hpp"
int main()
{
    clock_t tStart = clock();
    num pi = (arctan(num("0.2")) * num(16)) - (arctan(num(239).rec()) * num(4));
    pi.print();
    std::cout << std::endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;

}