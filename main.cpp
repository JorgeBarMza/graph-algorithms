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

int main(){

  // int vertexNum = 14;
  // MyGraphType g(vertexNum);
  //
  // for(int i = 0; i<=vertexNum; i++){
  //   g[i].val = i+1;
  // }
  for(int i = 1; i<=14; i++){
    addVertex(i);
  }
  addEdge(1,3,5);
  printEdges();

}
