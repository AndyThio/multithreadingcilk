#include <iostream>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <vector>
#include <ctime>
#include <utility>
#include <atomic>

using namespace std;
using namespace std::chrono;
struct adjListMonoid;
class adjListView;
struct adjList{
    vector<pair<int,int>> outgoing_edges;
    unsigned int weight;
};

struct adjList_red{
    vector<pair<int,int>> outgoing_edges;
    cilk::reducer<adjListMonoid> weight;
};

class adjListView{
    friend class adjListMonoid;
    unsigned int *original;
    public:
        bool is_lessthan(unsigned int updated){
            if(*original > updated){
                *original = updated;
                return true;
            }
            return false;
        }
};

struct adjListMonoid : public cilk::monoid_base<unsigned int*, adjListView> {
    static void identity(adjListView* view){
        view->original = 0;
    }
    void reduce(op_add_view* right) { this->m_value = right->m_value; }
};

void update_red(unsigned int index,  vector<adjList_red> &graph, unsigned int weight){
    if(graph.at(index).weight->is_lessthan(weight)){
        cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
            update(graph.at(index).outgoing_edges.at(i).first, graph,
                        graph.at(index).outgoing_edges.at(i).second+weight);
        }
    }
}

void update(unsigned int index,  vector<adjList> &graph, unsigned int weight){
    if(weight < graph.at(index).weight){
        graph.at(index).weight = weight;
        cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
            update(graph.at(index).outgoing_edges.at(i).first, graph,
                        graph.at(index).outgoing_edges.at(i).second+weight);
        }
    }
}

int main(){
    vector<adjList> graph;
    adjList new1;
    new1.weight = 9999999;
    new1.outgoing_edges.emplace_back(make_pair(1,4));
    new1.outgoing_edges.emplace_back(make_pair(2,2));
    graph.emplace_back(new1);
    
    new1.outgoing_edges.clear();
    new1.outgoing_edges.emplace_back(make_pair(2,5));
    new1.outgoing_edges.emplace_back(make_pair(3,10));
    graph.emplace_back(new1);
    
    new1.outgoing_edges.clear();
    new1.outgoing_edges.emplace_back(make_pair(4,3));
    graph.emplace_back(new1);
    
    new1.outgoing_edges.clear();
    new1.outgoing_edges.emplace_back(make_pair(5,11));
    graph.emplace_back(new1);
    
    new1.outgoing_edges.clear();
    new1.outgoing_edges.emplace_back(make_pair(3,4));
    graph.emplace_back(new1);
    
    new1.outgoing_edges.clear();
    graph.emplace_back(new1);
    
    vector<adjList_red> graph2;
    adjList_red new2;
    new2.weight = 9999999;
    new2.outgoing_edges.emplace_back(make_pair(1,4));
    new2.outgoing_edges.emplace_back(make_pair(2,2));
    graph2.emplace_back(new2);
    
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
    
    time_point<system_clock> start, end;
    duration<double> elapsed_time;
    
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
    for(auto &e: graph){
        cout << e.weight << endl;
    }
    cout <<"red" << endl;
    for(auto &e: graph2){
        cout << e.weight << endl;
    }
    return 0;
}