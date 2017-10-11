#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>

using namespace std;

// the predefined property types are listed here:
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/using_adjacency_list.html#sec:adjacency-list-properties
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/bundles.html

// struct VertexData
// {
//   int val;
// };

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::directedS,
                              int,
                              boost::property<boost::edge_weight_t, int>
                              > MyGraphType;
MyGraphType g;

boost::property_map<MyGraphType, boost::edge_weight_t>::type weightmap =
  get(boost::edge_weight, g);


auto v = add_vertex(g);

void addVertex(int value){
  auto v = add_vertex(g);
  //g[v].val = value;
  g[v] = value;
}

void removeVertex(int value){
  // auto vpair = vertices(g);
  // cout << boost::num_vertices(g) << "\n";
  // for(auto iter=vpair.first; iter!=vpair.second; iter++) {
  //   cout << g[*iter].val << "\n";
  //     if(g[*iter].val == value) {
  //       cout << "found\n";
  //       boost::remove_vertex(*iter, g);
  //       break;
  //     }
  // }
  // cout << boost::num_vertices(g) << "\n";
  remove_vertex(value, g);
}

void addEdge(int v1, int v2, int w){
  auto e = add_edge(v1,v2,g).first;
  weightmap[e] = w;
}

void printVertices(){
  auto vpair = vertices(g);
  for(auto iter=vpair.first; iter!=vpair.second; iter++) {
      // std::cout << "vertex " << g[*iter].val << std::endl;
      cout << "vertex " << g[*iter] << "\n";
  }
}

void printEdges(){
  auto epair = edges(g);
  for(auto iter=epair.first; iter!=epair.second; iter++) {
    std::cout << "edge " << source(*iter, g) << " - " << target(*iter, g) << "weight: " << weightmap[*iter] << std::endl;
  }
}

void removeEdge(int v1, int v2){
  remove_edge(v1, v2, g);
}

void modelSampleGraph(){
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

  modelSampleGraph();


}
