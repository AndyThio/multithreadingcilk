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
*/
void update_red(unsigned int index, vector<vector<pair<int,unsigned int> > > &graph,
    vector<cilk::reducer<valueMonoid>* > &cost, unsigned int value, unsigned int prev_index){

    if((*cost.at(index))->is_lessthan(value)){
        (*cost.at(index))->add_compare(value,prev_index);

        cilk_for(int i = 0; i < graph.at(index).size(); ++i){
        //cilk_for(int i = 0; i < 1; ++i){
            update_red(graph.at(index).at(i).first, graph, cost,
              graph.at(index).at(i).second+value, index);
        }
    }
 }

// void update(unsigned int index,  vector<adjList> &graph, unsigned int value){
//     if(value < graph.at(index).value){
//         graph.at(index).value = value;
//         cilk_for(int i = 0; i < graph.at(index).outgoing_edges.size(); ++i){
//             update(graph.at(index).outgoing_edges.at(i).first, graph,
//                         graph.at(index).outgoing_edges.at(i).second+value);
//         }
//     }
// }

int main(int argc, char* argv[]){
    vector<pair<int,unsigned int> > outgoing_edges;
    vector<vector<pair<int,unsigned int> > > graph;
    vector<cilk::reducer<valueMonoid>* > cost;
/*
    fstream fin(argv[1], fstream::in);
    string load_line;

    unsigned int prev = 0;
    while(getline(fin, load_line)){
        cout << load_line << endl;
        unsigned int to;
        unsigned int from;

        istringstream load(load_line);
        load >> to >> from;

        if(prev != to){
            graph.emplace_back(outgoing_edges);
            outgoing_edges.clear();
            for(int i = 0; i < to-(prev+1);++i){
                graph.emplace_back(outgoing_edges);
            }
            if(prev > to){
                cerr << "nodes out of order" << endl;
                return 1;
            }
        }



        if (prev < 5){
            cout << to << "::" << from << endl;
        }
        outgoing_edges.emplace_back(make_pair(from,1));
        prev = to;
    }
            graph.emplace_back(outgoing_edges);
*/
    for(unsigned i = 0;  i < 6; ++i){
        cost.emplace_back();
        (cost.at(i)) = cilk::aligned_new< cilk::reducer<valueMonoid> >();

    }


    outgoing_edges.emplace_back(make_pair(1,4));
    outgoing_edges.emplace_back(make_pair(2,2));
    graph.emplace_back(outgoing_edges);

    outgoing_edges.clear();
    outgoing_edges.emplace_back(make_pair(2,5));
    outgoing_edges.emplace_back(make_pair(3,10));
    graph.emplace_back(outgoing_edges);

    outgoing_edges.clear();
    outgoing_edges.emplace_back(make_pair(4,3));
    graph.emplace_back(outgoing_edges);

    outgoing_edges.clear();
    outgoing_edges.emplace_back(make_pair(5,11));
    graph.emplace_back(outgoing_edges);

    outgoing_edges.clear();
    outgoing_edges.emplace_back(make_pair(3,4));
    graph.emplace_back(outgoing_edges);

    outgoing_edges.clear();
    graph.emplace_back(outgoing_edges);



/*

    adjList_red list [24000];
    list[0].value -> add_compare(inf);
    cout << numeric_limits<unsigned int>::max()<< endl;

     vectorRed graph2;
     //new2.outgoing_edges.emplace_back(make_pair(1,4));
     //new2.outgoing_edges.emplace_back(make_pair(2,2));
     //graph2.emplace_back(new2);
     graph2.push_back(inf,23);
     graph2.push_back(15,23);

     graph2.at(1)->add_compare(4,2);
     graph2.at(1)->add_compare(2,12);
     graph2.at(1)->add_compare(5,41);


    return 0;
        */
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
*/
     start = system_clock::now();
     update_red(0,graph,cost, 0,0);
     //assert(false);
     end = system_clock::now();
     elapsed_time = end - start;
     cout << "Time elapsed: " << elapsed_time.count() << endl;
     /*
     for(auto &e: graph){
         cout << e.value << endl;
     }
     */
      cout <<"red" << endl;
      for(unsigned i = 0; cost.size() > i ; ++i){
          cout << (*cost.at(i))->view_get_value() << endl;
      }
    return 0;
}
