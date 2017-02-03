#include <iostream>
#include <fstream>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer.h>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <utility>
#include <atomic>
#include <limits>
#include <sstream>

using namespace std;
using namespace std::chrono;

#include "reducerClass.cpp"

const unsigned int inf = numeric_limits<unsigned int>::max();

void update_red(unsigned int index, vector<vector<pair<unsigned int,unsigned int> > > &graph,
    vector<cilk::reducer<valueMonoid>* > &cost, unsigned int value, unsigned int prev_index){
    if(index < cost.size() && (*cost.at(index))->is_lessthan(value)){
        (*cost.at(index))->add_compare(value,prev_index);
        
        cilk_for(int i = 0; i < graph.at(index).size(); ++i){
            update_red(graph.at(index).at(i).first, graph, cost,
              graph.at(index).at(i).second+value, index);
        }
    }
 }

//param: end index to find, cost graph
vector<int> find_path_to(unsigned int index, const vector<cilk::reducer<valueMonoid>* > &cost){
    vector<int> ret;
    ret.push_back(index);
    while((*cost.at(index))->view_get_value() != 0){
        ret.push_back((*cost.at(index))->view_get_index());
        index = (*cost.at(index))->view_get_index();
    }
    return ret;
}

void print_shortest(unsigned int index, const vector<cilk::reducer<valueMonoid>* > &cost){
    vector<int> result = find_path_to(index,cost);
    cout << endl;
    for(unsigned int z = 0; z < result.size();++z){
        cout << result.at(z)<< endl;
    }
}

int main(int argc, char* argv[]){
    vector<pair<unsigned int,unsigned int> > outgoing_edges;
    vector<vector<pair<unsigned int,unsigned int> > > graph;
    vector<cilk::reducer<valueMonoid>* > cost;


    fstream fin(argv[1], fstream::in);
    string load_line;

    unsigned int prev = 0;
    unsigned int to;
    unsigned int from;
    while(fin >> to >> from) {

        if(to >= graph.size()){
            graph.resize(to+1);
        }
        graph.at(to).push_back(make_pair(from,1));
    }

    for(unsigned i = 0;  i < graph.size(); ++i){
        cost.push_back( cilk::aligned_new< cilk::reducer<valueMonoid> >());

    }

     time_point<system_clock> start, end;
     duration<double> elapsed_time;
     
     start = system_clock::now();
     update_red(0,graph,cost, 0,0);
     end = system_clock::now();
     elapsed_time = end - start;
     cout << "Time elapsed: " << elapsed_time.count() << endl;

     //large 4294967295 means infinity or otherwise unreachable
      for(unsigned i = 0; cost.size() > i ; ++i){
          cout << (*cost.at(i))->view_get_value() << endl;
      }
    return 0;
}
