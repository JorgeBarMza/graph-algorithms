#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

// the predefined property types are listed here:
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/using_adjacency_list.html#sec:adjacency-list-properties
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/bundles.html

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::directedS,
                              int,
                              boost::property<boost::edge_weight_t, int>
                              > directedG;


directedG g;

boost::property_map<directedG, boost::edge_weight_t>::type weightmap =
  get(boost::edge_weight, g);

auto v = add_vertex(g);

void addVertex(int value){
  auto v = add_vertex(g);
  g[v] = value;
}

void removeVertex(int value){
  remove_vertex(value,g);
}

void addEdge(int v1, int v2, int w){
  auto e = add_edge(v1,v2,g).first;
  weightmap[e] = w;
}

void printVertices(){
  auto vpair = vertices(g);
  for(auto iter=vpair.first; iter!=vpair.second; iter++) {
      cout << "vertex " << g[*iter] << "\n";
  }
}

void printEdges(){
  auto epair = edges(g);
  for(auto iter=epair.first; iter!=epair.second; iter++) {
    std::cout << "edge " << source(*iter, g) << " -> " << target(*iter, g) << " weight: " << weightmap[*iter] << std::endl;
  }
}

void removeEdge(int v1, int v2){
  remove_edge(v1, v2, g);
}



void DFSUtil(int v, int& visited){
    visited |= (1 << v);
    cout << v << " ";
    auto adjacentRange = adjacent_vertices(v, g);
    for(auto i = adjacentRange.first; i != adjacentRange.second; i++)
        if (!(visited & (1 << *i)))
            DFSUtil(*i, visited);
}

void DFS(int v){
    //int verticesNum = num_vertices(g)-1; // g[0] is ignored
    int visited = 0;
    DFSUtil(v, visited);
}

void BFS(int v){
  int verticesNum = num_vertices(g)-1; // g[0] is ignored
  int visited = 0;
  queue<int> q;

    visited |= (1 << v);
    q.push(v);

    while(!q.empty()){
        v = q.front();
        cout << v << " ";
        q.pop();

        auto adjacentRange = adjacent_vertices(v, g);
        for(auto i = adjacentRange.first; i != adjacentRange.second; i++)
            if (!(visited & (1 << *i))){
                q.push(*i);
                visited |= (1 << *i);
            }
    }
}

void modelDirectedGraph(){
  for(int i = 1; i<=14; i++){
    addVertex(i);
  }

  addEdge(1,4,8);
  addEdge(4,8,2);
  addEdge(8,9,3);
  addEdge(9,12,4);
  addEdge(12,14,9);
  addEdge(14,13,2);
  addEdge(13,14,6);
  addEdge(11,12,6);
  addEdge(12,11,8);
  addEdge(12,9,2);
  addEdge(9,10,2);
  addEdge(10,6,6);
  addEdge(6,13,4);
  addEdge(8,7,3);
  addEdge(7,4,6);
  addEdge(4,7,3);
  addEdge(4,5,1);
  addEdge(5,6,9);
  addEdge(1,3,8);
  addEdge(3,2,7);
  addEdge(2,5,7);
  addEdge(3,5,8);
  addEdge(3,10,4);
  addEdge(10,3,10);
  addEdge(5,6,9);
}

int main(){

  cout << "Vertex Insertion ";
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  addVertex(1);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

  cout << "Vertex Deletion ";
  t1 = high_resolution_clock::now();
  removeVertex(1);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

  addVertex(1);
  addVertex(2);

  cout << "Edge Insertion ";
  t1 = high_resolution_clock::now();
  addEdge(1,2,10);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

  cout << "Edge Deletion ";
  t1 = high_resolution_clock::now();
  removeEdge(1,2);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

  removeVertex(2);
  removeVertex(1);

  modelDirectedGraph();

  cout << "DFS: ";
  t1 = high_resolution_clock::now();
  DFS(1);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

  cout << "BFS: ";
  t1 = high_resolution_clock::now();
  BFS(1);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";
}
