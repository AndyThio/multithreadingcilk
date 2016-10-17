#include <iostream>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <vector>
#include <ctime>

using namespace std;
using namespace std::chrono;

//checks if v is in order
template<typename T>
void checkv(const vector<T> &v){
    for (int i = 0; i < v.size()-1; ++i){
        if (v.at(i) > v.at(i+1)){
            cout << "error, not in order" << endl;
            for(auto &e: v){
                cout << e << ", ";
            }
            cout << endl;
            exit(1);
        }
    }
}

//merging
template<typename T>
void merge(vector<T> &v, int begin, int mid, int end){
    vector<T> vl;
    for(int s1 = 0; s1 < (mid-begin+1); ++s1){
        vl.emplace_back( v.at(s1+begin));
    }

    vector<T> vr;
    for(int s2 = 0; s2 < (end-mid); ++s2){
        vr.emplace_back( v.at(s2+mid+1));
    }

    int i = 0;
    int j = 0;
    int k = begin;
    while (i < vl.size() && j < vr.size()){
        if(vl.at(i) <= vr.at(j)){
            v.at(k) = vl.at(i);
            ++i;
        }
        else{
            v.at(k) = vr.at(j);
            ++j;
        }
        ++k;
    }

    while (i < vl.size()){
        v.at(k) = vl.at(i);
        ++i;
        ++k;
    }
    while (j < vr.size()){
        v.at(k) = vr.at(j);
        ++j;
        ++k;
    }
}

//Serial Implementation
template<typename T>
void mergesort_serial(vector<T> &v, int begin, int end){
    if( begin < end){
        int mid = begin+(end-begin)/2;
        mergesort_serial(v, begin, mid);
        mergesort_serial(v,mid+1,end);
        merge(v, begin, mid, end);
    }
}

//Double Spawn Implementation
template<typename T>
void mergesort_single(vector<T> &v, int begin, int end){
    if( begin < end){
        int mid = begin+(end-begin)/2;
        cilk_spawn mergesort_single(v, begin, mid);
        mergesort_single(v,mid+1,end);
        cilk_sync;
        merge(v, begin, mid, end);
    }
}

//Double Spawn Implementation
template<typename T>
void mergesort_double(vector<T> &v, int begin, int end){
    if( begin < end){
        int mid = begin+(end-begin)/2;
        cilk_spawn mergesort_double(v, begin, mid);
        cilk_spawn mergesort_double(v,mid+1,end);
        cilk_sync;
        merge(v, begin, mid, end);
    }
}
int main(int agvc, char* argv[]){
    vector<int> test;
    int test_size = 1000000;
    int largest_number = 20000;

    srand(time(0));

    for(int i = 0; i < test_size; i++){
        test.emplace_back(rand()%largest_number+1);
    }

    cout << "Vector Size: " << test.size() << endl;
    //cout << "Vector Content: ";
    //for(auto& e: test){
    //    cout << e << ", ";
    //}
    __cilkrts_set_param("nworkers",argv[1]);
    cout << endl << endl;

    vector<int> test2(test.size());
    copy(test.begin(),test.end(), test2.begin());

    vector<int> test3(test.size());
    copy(test.begin(),test.end(), test3.begin());

    time_point<system_clock> start, end;
    duration<double> elapsed_time;

    if(__cilkrts_get_nworkers() == 1){
        //Serial Implmentation
        cout << "Serial Implmentation" << endl;
        start = system_clock::now();
        mergesort_serial(test,0,test.size()-1);
        end = system_clock::now();
        elapsed_time = end-start;
        checkv(test);
        cout << "Time elapsed: " << elapsed_time.count() << endl << endl;
    }

    //Single Spawn
    cout << "Single Spawn" << endl;
    cout << "Using " << __cilkrts_get_nworkers() << " workers" << endl;
    start = system_clock::now();
    mergesort_single(test2,0,test2.size()-1);
    end = system_clock::now();
    elapsed_time =end - start;
    checkv(test2);
    cout << "Time elapsed: " << elapsed_time.count() << endl << endl;

    //Double Spawn
    cout << "Double Spawn" << endl;
    cout << "Using " << __cilkrts_get_nworkers() << " workers" << endl;
    start = system_clock::now();
    mergesort_double(test3,0,test3.size()-1);
    end = system_clock::now();
    elapsed_time = end - start;
    checkv(test3);
    cout << "Time elapsed: " << elapsed_time.count() << endl;
    return 0;
}
