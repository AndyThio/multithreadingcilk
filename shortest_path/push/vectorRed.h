#ifndef vectorRed_h
#define vectorRed_h

#include <iostream>
using namespace std;

template <typename T>
class vectorRed{
    private:
        T *elements;
        int sz;
        int cap;
    public:
        vectorRed();
        vectorRed(const vectorRed<T> &copyfrom);
        ~vectorRed();
        void push_back(T element);
        void pop_back();
        int size();
        T & at(unsigned index);
    private:
        void expand();
        
};

#endif