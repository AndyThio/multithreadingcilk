/*
 * fib.cpp
 *
 * Time how long it takes to calculate a Fibonacci number. See
 * http://en.wikipedia.org/wiki/Fibonacci_number for information about the
 * Fibonacci sequence. This application demonstrates the use of the cilk_spawn
 * and cilk_sync keywords.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

int fib(int n)
{
    if (n < 2)
        return n;
    int x = cilk_spawn fib(n-1);
    int y = fib(n-2);
    cilk_sync;
    return x + y;
}

int fib_s(int n)
{
    if (n < 2)
        return n;
    int x = fib_s(n-1);
    int y = fib_s(n-2);

    return x + y;
}

int main(int argc, char *argv[])
{
    // Fibonacci number to be calculated.  39 is big enough to take a
    // reasonable amount of time
    int n = 42;

    // Time how long it takes to calculate the nth Fibonacci number
    clock_t start = clock();
    int result = fib(n);
    clock_t end = clock();

    // Display our results
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("PARALLEL:  Fibonacci number #%d is %d.\n", n, result);
    printf("Calculated in %.3f seconds using %d workers.\n",
           duration, __cilkrts_get_nworkers());

        // Time how long it takes to calculate the nth Fibonacci number
    start = clock();
    result = fib_s(n);
    end = clock();

    // Display our results
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("SERIAL:  Fibonacci number #%d is %d.\n", n, result);
    printf("Calculated in %.3f seconds using %d workers.\n",
           duration, __cilkrts_get_nworkers());

    return 0;
}
