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
                              boost::directedS,
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
  //g[e] = make_pair(w, make_pair(v1,v2));
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

void modelDirectedGraph(){
  for(int i = 1; i<=14; i++){
    addVertex(i);
  }
  addEdge(3,7,2);
  addEdge(7,14,2);
  addEdge(3,2,1);
  addEdge(2,8,8);
  addEdge(8,10,6);
  addEdge(10,12,7);
  addEdge(7,6,4);
  addEdge(7,8,3);
  addEdge(14,10,4);
  addEdge(8,6,2);
  addEdge(6,9,9);
  addEdge(9,8,9);
  addEdge(2,1,6);
  addEdge(1,4,7);
  addEdge(1,5,9);
  addEdge(5,11,9);
  addEdge(10,11,8);
  addEdge(11,13,2);
}

void shortestPath(int src){
    priority_queue<pair<int,int>, vector<pair<int,int>> , greater<pair<int,int>> > pq; // minheap
    int V = num_vertices(g);
    vector<int> parent(V, -1);
    vector<int> dist(V, INF);
    pq.push(make_pair(0, src));
    dist[src] = 0;
    int countI = 1;
    cout << "iter "<< countI++;
    printf(" Vertex   Distance and Parent\n");
    for (int i = 1; i < V; ++i)
        printf("%d \t\t %d \t\t %d \n", i, dist[i], parent[i]);
        
    while (!pq.empty()){

        int u = pq.top().second;
        pq.pop();
        auto adjacentRange = adjacent_vertices(u, g);
        for(auto i = adjacentRange.first; i != adjacentRange.second; i++){
          int v = g[*i];
          int weight = weightmap[edge(u,*i,g).first];
            if (dist[v] > dist[u] + weight){
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
                cout << "iter "<< countI++;
                printf(" Vertex   Distance and Parent\n");
                for (int i = 1; i < V; ++i)
                    printf("%d \t\t %d \t\t %d \n", i, dist[i], parent[i]);
            }
        }
    }

    // printf("Vertex   Distance from Source\n");
    // for (int i = 0; i < V; ++i)
    //     printf("%d \t\t %d\n", i, dist[i]);
}


int main(){

  auto v = add_vertex(g);

  modelDirectedGraph();

  cout << "Dijkstra:\n";
  auto t1 = high_resolution_clock::now();
  shortestPath(3);
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

}
