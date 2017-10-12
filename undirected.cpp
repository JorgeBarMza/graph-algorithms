#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <queue>
#include <chrono>
#include <limits.h>
#define INF INT_MAX

using namespace std;
using namespace std::chrono;

// the predefined property types are listed here:
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/using_adjacency_list.html#sec:adjacency-list-properties
// http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/bundles.html

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::undirectedS,
                              pair<int,int>,
                              boost::property<boost::edge_weight_t, int>
                              > undirectedG;


undirectedG g;

boost::property_map<undirectedG, boost::edge_weight_t>::type weightmap =
  get(boost::edge_weight, g);

//auto v = add_vertex(g);

void addVertex(int value){
  auto v = add_vertex(g);
  g[v] = make_pair(INF,value);
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
      cout << "vertex " << g[*iter].second << "\n";
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

void primMST(){
    priority_queue<pair<int,int>, vector<pair<int,int>> , greater<pair<int,int>> > pq; // minheap

    int src = 0; //start from node 0
    g[src].second = 0;
    int V = num_vertices(g);
    vector<int> key(V, INF);
    vector<int> parent(V, -1);
    int inMST = 0; //bitvector
    pq.push(make_pair(0, src));
    key[src] = 0;

    while (!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        inMST |= (1 << u);

        auto adjacentRange = adjacent_vertices(u, g);
        for(auto i = adjacentRange.first; i != adjacentRange.second; i++){
          int v = g[*i].second;
          int weight = weightmap[edge(u,*i,g).first];
          if (!(inMST & (1 << v)) && key[v] > weight){
              key[v] = weight;
              pq.push(make_pair(key[v], v));
              parent[v] = u;
          }
        }
    }
    for (int i = 1; i < V; ++i)
        printf("%d - %d\n", parent[i], i);
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

  cout << "Prim:\n";
  auto t1 = high_resolution_clock::now();
  primMST();
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

}
