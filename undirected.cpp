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
                              boost::undirectedS,
                              int,
                              boost::property<boost::edge_weight_t, int>
                              > undirectedG;


undirectedG g;

boost::property_map<undirectedG, boost::edge_weight_t>::type weightmap =
  get(boost::edge_weight, g);

//auto v = add_vertex(g);

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
  // e = add_edge(v2,v1,g).first;
  // weightmap[e] = w;
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
    std::cout << "edge " << source(*iter, g) << " - " << target(*iter, g) << " weight: " << weightmap[*iter] << std::endl;
  }
}

void removeEdge(int v1, int v2){
  remove_edge(v1, v2, g);
}


void modelDirectedGraph(){
  for(int i = 0; i<=8; i++){
    addVertex(i);
  }
  addEdge(0,1,4);
  addEdge(0,7,8);
  addEdge(1,7,11);
  addEdge(1,2,8);
  addEdge(7,6,1);
  addEdge(7,8,7);
  addEdge(2,8,2);
  addEdge(2,5,4);
  addEdge(2,3,7);
  addEdge(6,5,2);
  addEdge(3,5,14);
  addEdge(3,4,9);
  addEdge(5,4,10);
  addEdge(8,6,6);
}

int main(){

  modelDirectedGraph();

}
