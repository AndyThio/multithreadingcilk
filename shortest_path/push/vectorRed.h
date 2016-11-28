#ifndef vectorRed_h
#define vectorRed_h

#include <iostream>
using namespace std;


#include "reducerClass.cpp"

class vectorRed{
    private:
        cilk::reducer<valueMonoid> *elements;
        int sz;
        int cap;
    public:
        vectorRed();
        vectorRed(const vectorRed &copyfrom);
        ~vectorRed();
        void push_back(unsigned int value);
        void pop_back();
        int size();
        cilk::reducer<valueMonoid> & at(unsigned index);
    private:
        void expand();
        
};

#endif