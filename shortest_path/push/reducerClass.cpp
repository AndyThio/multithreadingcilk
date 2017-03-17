#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <limits>

using namespace std;

struct valueMonoid;

class valueView{
    friend class valueMonoid;
    unsigned int value;
    unsigned int index;
    public:
        void add_compare(unsigned int updated, unsigned int new_index){
            value = updated;
            index = new_index;
        }
        void reset_value(){
            value = numeric_limits<unsigned int>::max();
            index = 0;
        }
        bool is_lessthan(unsigned int updated){
            return value > updated;
        }
        unsigned int view_get_value()const  {return value;}
        unsigned int view_get_index()const  {return index;}
};


struct valueMonoid : public cilk::monoid_base<unsigned int , valueView> {
    static void identity(valueView* view){
        view->value = numeric_limits<unsigned int>::max();
    }
    static void reduce(valueView* left, valueView* right) {
        if (left->value > right->value){
            left->value = right->value;
            left->index = right->index;
        }
    }
};