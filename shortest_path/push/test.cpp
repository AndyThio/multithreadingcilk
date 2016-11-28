#include <iostream>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <vector>
#include <ctime>
#include <utility>
#include <atomic>
#include <limits>

using namespace std;
using namespace std::chrono;


const unsigned int inf = numeric_limits<unsigned int>::max();

struct valueMonoid;
class valueView;
// struct adjList{
//     vector<pair<int,int> > outgoing_edges;
//     unsigned int value;
// };



class valueView{
    friend class valueMonoid;
    unsigned int value;
    public:
        void add_compare(unsigned int updated){
            if(value > updated){
                value = updated;
            }
        }
        bool get_lessthan(unsigned int updated){
            return value < updated;
        }
        unsigned int view_get_value()const  {return value;}
};

struct valueMonoid : public cilk::monoid_base<unsigned int , valueView> {
    static void identity(valueView* view){
        view->value = inf;
    }
    static void reduce(valueView* left, valueView* right) {
        if (left->value > right->value){
            left->value = right->value;
        }
    }
};


int main(){
    vector < cilk::reducer<valueMonoid>* > test1;
    unsigned int size = 15;
    cout << "entering forloop1" << endl;
    for (unsigned int i = 0; i < size; i++){
        test1.emplace_back();
        (test1[i]) = cilk::aligned_new< cilk::reducer<valueMonoid> >();
        (*test1[i])->add_compare(i);
    }
    for (unsigned int i = 0; i < size; i++){
        (*test1.at(i))->add_compare(65);
    }
    for (unsigned int i = 0; i < size; i++){
        (*test1.at(i))->add_compare(3);
    }
    cout << "entering print loop" << endl;
    for (unsigned int i = 0; i < size; i++){
        cout << (*test1.at(i))->view_get_value() << endl;
    }
}