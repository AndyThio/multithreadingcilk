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

unsigned int inf = numeric_limits<unsigned int>::infinity();

struct valueMonoid;
class valueView;
// struct adjList{
//     vector<pair<int,int> > outgoing_edges;
//     unsigned int value;
// };

// struct adjList_red{
//     vector<pair<int,int> > outgoing_edges;
// };

class valueView{
    friend class valueMonoid;
    unsigned int* value;
    public:
        void is_lessthan(unsigned int updated){
            value* = updated;
        }
        bool get_lessthan(unsigned int updated){
            return value* < updated;
        }
        unsigned int view_get_value() {return value;}
};

struct valueMonoid : public cilk::monoid_base<unsigned int , valueView> {
    static void identity(valueView* view){
        view->value = inf;
    }
    static void reduce(valueView* left, valueView* right) {
        if (left->value* > right->value*){
            left->value* = right->value*;
        }
    }
};

// void update_red(unsigned int index,  vector<adjList_red> &graph, unsigned int value){
//     if(graph.at(index).value->get_lessthan(value)){
//         cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
//             update_red(graph.at(index).outgoing_edges.at(i).first, graph,
//                         graph.at(index).outgoing_edges.at(i).second+value);
//         }
//     }
// }

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
    
    cilk::reducer<valueMonoid> value;
    
    value->is_lessthan(1);
    value->is_lessthan(6);
    value->is_lessthan(2);
    
    cout << value->view_get_value() << endl;
    
    // vector<adjList> graph;
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
    
    // vector<pair<cilk::reducer<valueMonoid> value,vector<pair<int,int> > outgoing_edges> graph2;
    // adjList_red new2;
    // new2.value = 9999999;
    // new2.outgoing_edges.emplace_back(make_pair(1,4));
    // new2.outgoing_edges.emplace_back(make_pair(2,2));
    // graph2.emplace_back(new2);
    
    // new2.outgoing_edges.clear();
    // new2.outgoing_edges.emplace_back(make_pair(2,5));
    // new2.outgoing_edges.emplace_back(make_pair(3,10));
    // graph2.emplace_back(new2);
    
    // new2.outgoing_edges.clear();
    // new2.outgoing_edges.emplace_back(make_pair(4,3));
    // graph2.emplace_back(new2);
    
    // new2.outgoing_edges.clear();
    // new2.outgoing_edges.emplace_back(make_pair(5,11));
    // graph2.emplace_back(new2);
    
    // new2.outgoing_edges.clear();
    // new2.outgoing_edges.emplace_back(make_pair(3,4));
    // graph2.emplace_back(new2);
    
    // new2.outgoing_edges.clear();
    // graph2.emplace_back(new2);
    
    // time_point<system_clock> start, end;
    // duration<double> elapsed_time;
    
    // start = system_clock::now();
    // update(0,graph, 0);
    // end = system_clock::now();
    // elapsed_time = end - start;
    // cout << "Time elapsed: " << elapsed_time.count() << endl;
    
    // start = system_clock::now();
    // update_red(0,graph2, 0);
    // end = system_clock::now();
    // elapsed_time = end - start;
    // cout << "Time elapsed: " << elapsed_time.count() << endl;
    // for(auto &e: graph){
    //     cout << e.value << endl;
    // }
    // cout <<"red" << endl;
    // for(auto &e: graph2){
    //     cout << e.value << endl;
    // }
    return 0;
}