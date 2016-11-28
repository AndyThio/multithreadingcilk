#include <iostream>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
using namespace std;

#include "vectorRed.h"


void vectorRed::expand(){
    if(cap == 0){
        cap = 1;
    }
    cap *= 2;
    cilk::reducer<valueMonoid>* temp = (cilk::reducer<valueMonoid>*)malloc(cap);
    for (unsigned int i = 0; i < sz; i++)
    {
        auto temp2 = elements[i]->view_get_value();
        cilk::reducer<valueMonoid> temp1;
        temp1->add_compare(temp2);
        temp[i] = temp1;
    }
    free(elements);
    
    elements = temp;
}


vectorRed::vectorRed()
    :elements(0),sz(0),cap(0)
{}


vectorRed::vectorRed(const vectorRed &copyfrom){
    if(cap < copyfrom.cap){
        expand();
    }
    for (unsigned int i = 0; i < sz; i++)
    {
        auto temp = copyfrom.elements[i]->view_get_value();
        elements[i] = cilk::aligned_new< cilk::reducer<valueMonoid> >();
        elements[i]->add_compare(temp);
    }
}


vectorRed::~vectorRed(){
    free(elements);
}


void vectorRed::push_back(unsigned int value){
    ++sz;
    if(sz > cap){
        expand();
    }
    elements[sz-1] = cilk::aligned_new< cilk::reducer<valueMonoid> >();
    elements[sz-1]->add_compare(value);
}


void vectorRed::pop_back(){
    if(sz > 0){
        sz--;
    }
}


int vectorRed::size(){
    return sz;
}

\
cilk::reducer<valueMonoid> & vectorRed<T>::at(unsigned int index){
    if (sz > index){
        return elements[index];
    }
    else{
        cerr << "Out of Range" << endl;
        exit(1);
    }
}