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

struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            //every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

 /* Functions returns weight of the MST*/

int kruskalMST(vector<pair<int,pair<int,int>>>& edges){
    int mst_wt = 0; // Initialize result
    int V = num_vertices(g);
    DisjointSets ds(V);

    vector< pair<int, pair<int,int>> >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++){
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        if (set_u != set_v){
            cout << u << " - " << v << endl;
            mst_wt += it->first;
            ds.merge(set_u, set_v);
        }
    }
    return mst_wt;
}

int main(){

  modelDirectedGraph();

  auto epair = edges(g);
  vector<pair<int,pair<int,int>>> edges;
  for(auto iter=epair.first; iter!=epair.second; iter++) {
     edges.push_back(make_pair(weightmap[*iter], make_pair(source(*iter, g),target(*iter, g))));
  }
  sort(edges.begin(), edges.end());


  cout << "Kruskal:\n";
  auto t1 = high_resolution_clock::now();
  cout << kruskalMST(edges) << "\n";
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  cout << "duration: " << duration << " μs\n";

}
