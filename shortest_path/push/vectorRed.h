#ifndef vectorRed_h
#define vectorRed_h

#include <iostream>
using namespace std;


#include "reducerClass.cpp"

class vectorRed{
    private:
        cilk::reducer<valueMonoid> *elements;
        unsigned int sz;
        unsigned int cap;
    public:
        vectorRed();
        vectorRed(unsigned int size);
        vectorRed(const vectorRed &copyfrom);
        ~vectorRed();
        void push_back(unsigned int value, unsigned int index);
        void pop_back();
        unsigned int size();
        cilk::reducer<valueMonoid> & at(unsigned int index);
    private:
        void expand();
        
};

#endif