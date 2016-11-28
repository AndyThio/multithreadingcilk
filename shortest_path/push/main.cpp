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

#include "vectorRed.h"

const unsigned int inf = numeric_limits<unsigned int>::max();

// struct valueMonoid;
// class valueView;
// struct adjList{
//     vector<pair<int,int> > outgoing_edges;
//     unsigned int value;
// };


/*
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
*/
/*
template <typename T>
struct valueMonoid : public cilk::monoid_base<, valueView> {
    static void identity(valueView* view){
        view->value = inf;
    }
    static void reduce(valueView* left, valueView* right) {
        if (left->value > right->value){
            left->value = right->value;
        }
    }
};
*/
/*

struct adjList_red{
    vector<pair<int,unsigned int> > outgoing_edges;
    adjList_red();
    adjList_red(const adjList_red& copyfrom);
    adjList_red& operator=(const adjList_red& copyfrom);
};


adjList_red::adjList_red(){
}
adjList_red::adjList_red(const adjList_red& copyfrom){
    outgoing_edges = copyfrom.outgoing_edges;
    value->add_compare(copyfrom.value->view_get_value());
}
adjList_red& adjList_red::operator=(const adjList_red& copyfrom){
    outgoing_edges = copyfrom.outgoing_edges;
    value->add_compare(copyfrom.value->view_get_value());
}
 void update_red(unsigned int index,  vectorRed<adjList_red> &graph, unsigned int value){
     if(graph.at(index).value->get_lessthan(value)){
         cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
             update_red(graph.at(index).outgoing_edges.at(i).first, graph,
                         graph.at(index).outgoing_edges.at(i).second+value);
         }
     }
 }
*/
// void update(unsigned int index,  vector<adjList> &graph, unsigned int value){
//     if(value < graph.at(index).value){
//         graph.at(index).value = value;
//         cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
//             update(graph.at(index).outgoing_edges.at(i).first, graph,
//                         graph.at(index).outgoing_edges.at(i).second+value);
//         }
//     }
// }

int main(){

//     vector<adjList> graph;
    // adjList new1;
    // new1.value = 9999999;
    // new1.outgoing_edges.emplace_back(make_pair(1,4));
    // new1.outgoing_edges.emplace_back(make_pair(2,2));
    // graph.emplace_back(new1);

    // new1.outgoing_edges.clear();
    // new1.outgoing_edges.emplace_back(make_pair(2,5));
    // new1.outgoing_edges.emplace_back(make_pair(3,10));
    // graph.emplace_back(new1);

    // new1.outgoing_edges.clear();
    // new1.outgoing_edges.emplace_back(make_pair(4,3));
    // graph.emplace_back(new1);

    // new1.outgoing_edges.clear();
    // new1.outgoing_edges.emplace_back(make_pair(5,11));
    // graph.emplace_back(new1);

    // new1.outgoing_edges.clear();
    // new1.outgoing_edges.emplace_back(make_pair(3,4));
    // graph.emplace_back(new1);

    // new1.outgoing_edges.clear();
    // graph.emplace_back(new1);
    
    // adjList_red list [24000];
    // list[0].value -> add_compare(inf);
    
    
     vectorRed graph2;
     //new2.outgoing_edges.emplace_back(make_pair(1,4));
     //new2.outgoing_edges.emplace_back(make_pair(2,2));
     //graph2.emplace_back(new2);
     graph2.push_back(5);
    return 0;
/*
     new2.outgoing_edges.clear();
     new2.outgoing_edges.emplace_back(make_pair(2,5));
     new2.outgoing_edges.emplace_back(make_pair(3,10));
     graph2.emplace_back(new2);

     new2.outgoing_edges.clear();
     new2.outgoing_edges.emplace_back(make_pair(4,3));
     graph2.emplace_back(new2);

     new2.outgoing_edges.clear();
     new2.outgoing_edges.emplace_back(make_pair(5,11));
     graph2.emplace_back(new2);

     new2.outgoing_edges.clear();
     new2.outgoing_edges.emplace_back(make_pair(3,4));
     graph2.emplace_back(new2);

     new2.outgoing_edges.clear();
     graph2.emplace_back(new2);

*/
     time_point<system_clock> start, end;
     duration<double> elapsed_time;
/*
     start = system_clock::now();
     update(0,graph, 0);
     end = system_clock::now();
     elapsed_time = end - start;
     cout << "Time elapsed: " << elapsed_time.count() << endl;
     
     start = system_clock::now();
     update_red(0,graph2, 0);
     end = system_clock::now();
     elapsed_time = end - start;
     cout << "Time elapsed: " << elapsed_time.count() << endl;
     /*
     for(auto &e: graph){
         cout << e.value << endl;
     }
     */
     cout <<"red" << endl;
     for(unsigned i = 0; graph2.size() > i ; ++i){
         cout << graph2.at(i)->view_get_value() << endl;
     }
    return 0;
}
