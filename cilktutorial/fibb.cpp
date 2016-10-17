#include <iostream>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

//Serial Implmentation
unsigned fib_serial(int n){
    if(n < 2){
        return n;
    }

    int a = fib_serial(n-1);
    int b = fib_serial(n-2);

    return a+b;
}

//Single Spawn Implementation
unsigned fib_1spawn(int n){
    if(n < 2){
        return n;
    }

    int a = cilk_spawn fib_1spawn(n-1);
    int b = fib_1spawn(n-2);
    cilk_sync;

    return a+b;
}

//Double Spawn Implementation
unsigned fib_2spawn(int n){
    if(n < 2){
        return n;
    }

    int a = cilk_spawn fib_2spawn(n-1);
    int b = cilk_spawn fib_2spawn(n-2);
    cilk_sync;

    return a+b;
}

int main(int argc, char *argv[]){
    //nth fib number
    int n = 42;

    time_point<system_clock> start, end;
    duration<double> elapsed_time;

    __cilkrts_set_param("nworkers", argv[1]);
    int workers = __cilkrts_get_nworkers();

    if(workers == 1){
        //Serial
        cout << "Serial Implementation" <<endl;
        start= system_clock::now();
        cout << fib_serial(n) << endl;
        end = system_clock::now();
        elapsed_time = end-start;
        cout << "Time elapsed: " << elapsed_time.count() << endl << endl;
    }

    //Single Spawn
    cout << "Single Spawn Implementation" <<endl;
    cout << "Using " << __cilkrts_get_nworkers() << " workers" << endl;
    start= system_clock::now();
    cout << fib_1spawn(n) << endl;
    end = system_clock::now();
    elapsed_time = end-start;
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;

    //Double Spawn
    cout << "Double Spawn Implementation" <<endl;
    cout << "Using " << __cilkrts_get_nworkers() << " workers" << endl;
    start= system_clock::now();
    cout << fib_2spawn(n) << endl;
    end = system_clock::now();
    elapsed_time = end-start;
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;

    return 0;
}
