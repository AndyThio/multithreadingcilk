#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <chrono>
#include <vector>
#include <ctime>

using namespace std;
using namespace std::chrono;

//Serial Implementation
int product( const vector<int> &nums){
    int p =1;
    for(auto &e: nums){
        p *= e;
    }
    return p;
}

//Spawn Every Element
int product_perelem(const vector<int> &nums,vector<int>::iterator curr){
    if (curr==nums.end()){
        return 1;
    }
    else{
        int sum1 = cilk_spawn product_perelem(nums, curr+1);
        int sum2 = *curr;
        return sum1*sum2;
    }
}

//Cilk_for
int product_for(const vector<int> &nums){
    int p = 1;
    cilk_for (auto i= nums.begin();  i != nums.end(); ++i){
        p *= *i;
    }
    return p;
}

//Single Spawn
//
int product_1spawn(const vector<int> &nums,vector<int>::iterator begin, vector<int>::iterator end){
    if(distance(begin,end) <= 1){
        return *begin;
    }
    else{
        int sum1 = cilk_spawn product_1spawn(nums, begin, begin+ distance(begin,end)/2);
        int sum2 = product_1spawn(nums, begin+distance(begin,end)/2, end);
        cilk_sync;
        return sum1*sum2;
    }
}
/*
int product_1spawn( const vector<int> &nums){
    int n = nums.size();
    if (n==1){
        return nums.front();
    }
    else{

        int sum1 = cilk_spawn product_1spawn(v);
        int sum2 = product_1spawn(s);
        cilk_sync;
        return sum1*sum2;
    }
}
*/

//Double Spawn
int product_2spawn(const vector<int> &nums,vector<int>::iterator begin, vector<int>::iterator end){
    if(distance(begin,end) <= 1){
        return *begin;
    }
    else{
        int sum1 = cilk_spawn product_2spawn(nums, begin, begin+ distance(begin,end)/2);
        int sum2 = cilk_spawn product_2spawn(nums, begin+distance(begin,end)/2, end);
        cilk_sync;
        return sum1*sum2;
    }
}

int main(int argc, char* argv[]){
    vector<int> test;
    int test_size = 100000;
    int largest_number = 200;


    srand(time(0));

    for(int i = 0; i < test_size; i++){
        test.emplace_back(rand()%largest_number+1);
    }

    cout << "Vector Size: " << test.size() << endl;
    cout << endl << endl;

    /*
    for(auto &e: test){
        cout << e << ", ";
    }
    cout << endl;
    */

    time_point<system_clock> start, end;

    __cilkrts_set_param("nworkers", argv[1]);
    int workers = __cilkrts_get_nworkers();
    duration<double> elapsed_time;

    if (workers == 1){
        //Serial Implmentation
        cout << "Serial Implmentation" << endl;
        start = system_clock::now();
        cout << product(test) << endl;
        end = system_clock::now();
        elapsed_time = end-start;
        cout << "Time elapsed: " << elapsed_time.count() << endl << endl;
    }

/*
    //Spawn every element
    cout << "Spawn every element" << endl;
    start = system_clock::now();
    cout << product_perelem(test, test.begin()) << endl;
    end = system_clock::now();
    elapsed_time =end - start;
    cout << "Using " << workers << " workers" << endl;
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;

    //Cilk For
    cout << "Cilk For" << endl;
    start = system_clock::now();
    cout << product_for(test) << endl;
    end = system_clock::now();
    elapsed_time =end - start;
    cout << "Using " << workers << " workers" << endl;
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;
    */

    //Single Spawn
    cout << "Single Spawn" << endl;
    start = system_clock::now();
    cout << product_1spawn(test,test.begin(),test.end()) << endl;
    end = system_clock::now();
    elapsed_time =end - start;
    cout << "Using " << workers << " workers" << endl;
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;

    //Double Spawn
    cout << "Double Spawn" << endl;
    start = system_clock::now();
    cout << product_2spawn(test, test.begin(),test.end()) << endl;
    end = system_clock::now();
    elapsed_time = end - start;
    cout << "Using " << workers << " workers" << endl;
    cout << "Time elapsed: " << elapsed_time.count() << endl;
    return 0;
}
