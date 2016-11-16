#include <iostream>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
using namespace std;

#include "vectorRed.h"

template <typename T>
void vectorRed<T>::expand(){
    if(cap == 0){
        cap = 1;
        elements = cilk::aligned_new(T[1]);
    }
    cap *= 2;
    T* temp = cilk::aligned_new(T[cap]);
    for (unsigned i = 0; i < sz; i++)
    {
        temp[i] = elements[i];
    }
    cilk::aligned_delete(elements);
    
    elements = temp;
}

template <typename T>
vectorRed<T>::vectorRed()
    :elements(0),sz(0),cap(0)
{}

template <typename T>
vectorRed<T>::vectorRed(const vectorRed<T> &copyfrom){
    for (unsigned i = 0; i < sz; i++)
    {
        elements[i] = copyfrom.elements[i];
    }
}

template <typename T>
vectorRed<T>::~vectorRed(){
    cilk::aligned_delete(elements);
}

template <typename T>
void vectorRed<T>::push_back(T value){
    ++sz;
    if(sz > cap){
        expand();
    }
    elements[sz-1] = value;
}

template <typename T>
void vectorRed<T>::pop_back(){
    if(sz > 0){
        sz--;
    }
}

template <typename T>
int vectorRed<T>::size(){
    return sz;
}

template <typename T>
T & vectorRed<T>::at(unsigned index){
    if (sz > index){
        return elements[index];
    }
    else{
        cerr << "Out of Range" << endl;
        exit(1);
    }
}