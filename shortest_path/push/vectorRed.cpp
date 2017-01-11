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
    cilk::reducer<valueMonoid>* temp = (cilk::reducer<valueMonoid>*)malloc(cap * sizeof(cilk::reducer<valueMonoid>));
    for (unsigned int j = 0; j < cap; j++){
        temp[j]->reset_value();
    }
    for (unsigned int i = 0; i < sz; i++)
    {
        unsigned int temp_value = elements[i]->view_get_value();
        unsigned int temp_index = elements[i]->view_get_index();
        temp[i]->add_compare(temp_value,temp_index);
    }
    free(elements);

    elements = temp;
}


vectorRed::vectorRed()
    :elements(0),sz(0),cap(0)
{}

vectorRed::vectorRed(unsigned int size){
    cap = size+1;
    elements = (cilk::reducer<valueMonoid>*)malloc(cap * sizeof(cilk::reducer<valueMonoid>));
    sz = size;
    for (unsigned int j = 0; j < sz; j++){
        elements[j]->reset_value();
    }
    for (unsigned int j = 0; j < sz; j++){
        cout << elements[j]->view_get_value() << endl;
    }
    cout << endl;
}

vectorRed::vectorRed(const vectorRed &copyfrom){
    if(cap < copyfrom.cap){
        expand();
    }
    for (unsigned int i = 0; i < sz; i++)
    {
        unsigned int temp_value = copyfrom.elements[i]->view_get_value();
        unsigned int temp_index = copyfrom.elements[i]->view_get_index();
        elements[i]->add_compare(temp_value,temp_index);
    }
}


vectorRed::~vectorRed(){
    free(elements);
}


void vectorRed::push_back(unsigned int value, unsigned int index){
    if(sz+1 > cap){
        expand();
    }
    ++sz;
    elements[sz-1]->add_compare(value, index);
}


void vectorRed::pop_back(){
    if(sz > 0){
        sz--;
        elements[sz]->reset_value();
    }
}


unsigned int vectorRed::size(){
    return sz;
}

cilk::reducer<valueMonoid> &vectorRed::at(unsigned int index){
    if (sz > index){
        return elements[index];
    }
    else{
        cerr << "Out of Range" << endl;
        exit(1);
    }
}
