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

 struct adjList{
     atomic<unsigned int> value;
     vector<pair<int,int> > incoming_edges;
     
     adjList(unsigned int a);
     adjList(const adjList& copyfrom);
     adjList& operator=(const adjList& copyfrom);
 };

adjList::adjList(unsigned int a){
    value.exchange(a);
}

adjList::adjList(const adjList& copyfrom){
    value.exchange(copyfrom.value);
    for(auto &e: copyfrom.incoming_edges){
        incoming_edges.emplace_back(e);
    }
}
adjList& adjList::operator=(const adjList& copyfrom){
    value.exchange(copyfrom.value);
    for(auto &e: copyfrom.incoming_edges){
        incoming_edges.emplace_back(e);
    }
}
bool non_inf(vector<adjList> &v){
    for(auto &e: v){
        if(e.value == inf){
            return false;
        }
    }
    return true;
}

void edgeCentric(pair<int,int> edge,int index , vector<adjList> &v){
    unsigned int temp;
    unsigned int old;
    do{
        temp = v.at(index).value;
        old = temp;
        unsigned int p = inf;
        if(v.at(edge.first).value != inf){
            p = v.at(edge.first).value + edge.second;
        }
        if(temp > p){
            temp = p;
        }
    // cout << "Old: " << old << endl << "temp: " << temp << endl << endl;
    }while(!v.at(index).value.compare_exchange_strong(old, temp));
}

int main(){
    vector<adjList> graph;
    adjList new1(inf);
    new1.value.exchange(0);
    graph.emplace_back(new1);
    
    new1.value.exchange(inf);
    new1.incoming_edges.clear();
    new1.incoming_edges.emplace_back(make_pair(0,4));
    graph.emplace_back(new1);
    
    new1.incoming_edges.clear();
    new1.incoming_edges.emplace_back(make_pair(0,2));
    new1.incoming_edges.emplace_back(make_pair(1,5));
    graph.emplace_back(new1);
    
    new1.incoming_edges.clear();
    new1.incoming_edges.emplace_back(make_pair(1,10));
    new1.incoming_edges.emplace_back(make_pair(4,4));
    graph.emplace_back(new1);
    
    new1.incoming_edges.clear();
    new1.incoming_edges.emplace_back(make_pair(2,3));
    graph.emplace_back(new1);
    
    new1.incoming_edges.clear();
    new1.incoming_edges.emplace_back(make_pair(3,11));
    graph.emplace_back(new1);
    
    // for(auto &e: graph){
    //     cout << e.value << endl;
    //     for(auto &j : e.incoming_edges){
    //         cout << "incomgin edge: " << j.first << " ::: " << j.second << endl;
    //     }
    // }
    
    // vector<pair<cilk::reducer<valueMonoid> value,vector<pair<int,int> > incoming_edges> graph2;
    // adjList_red new2;
    // new2.value = 9999999;
    // new2.incoming_edges.emplace_back(make_pair(1,4));
    // new2.incoming_edges.emplace_back(make_pair(2,2));
    // graph2.emplace_back(new2);
    
    // new2.incoming_edges.clear();
    // new2.incoming_edges.emplace_back(make_pair(2,5));
    // new2.incoming_edges.emplace_back(make_pair(3,10));
    // graph2.emplace_back(new2);
    
    // new2.incoming_edges.clear();
    // new2.incoming_edges.emplace_back(make_pair(4,3));
    // graph2.emplace_back(new2);
    
    // new2.incoming_edges.clear();
    // new2.incoming_edges.emplace_back(make_pair(5,11));
    // graph2.emplace_back(new2);
    
    // new2.incoming_edges.clear();
    // new2.incoming_edges.emplace_back(make_pair(3,4));
    // graph2.emplace_back(new2);
    
    // new2.incoming_edges.clear();
    // graph2.emplace_back(new2);
    
    time_point<system_clock> start, end;
    duration<double> elapsed_time;
    
    start = system_clock::now();
    while(!non_inf(graph)){
        cilk_for(unsigned int i = 0; i < graph.size(); ++i){
            cilk_for(unsigned int j = 0; j < graph.at(i).incoming_edges.size(); ++j){
                edgeCentric(graph.at(i).incoming_edges.at(j), i, graph);
            }
        }
    }
    cilk_for(unsigned int i = 0; i < graph.size(); ++i){
        cilk_for(unsigned int j = 0; j < graph.at(i).incoming_edges.size(); ++j){
            edgeCentric(graph.at(i).incoming_edges.at(j), i, graph);
        }
    }
    
    end = system_clock::now();
    elapsed_time = end - start;
    cout << "Time elapsed: " << elapsed_time.count() << endl;
    
    // start = system_clock::now();
    // update_red(0,graph2, 0);
    // end = system_clock::now();
    // elapsed_time = end - start;
    cout << "Time elapsed: " << elapsed_time.count() << endl;
    for(auto &e: graph){
        cout << e.value << endl;
    }
    // cout <<"red" << endl;
    // for(auto &e: graph2){
    //     cout << e.value << endl;
    // }
    return 0;
}