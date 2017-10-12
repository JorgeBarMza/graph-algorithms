#include "stdafx.h"
#include "disjointset.h"

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
    typedef TPt<TNEANet> PNGraph;
    typedef pair<int, int> customPair;
public:
    Graph(int n) {
        visited.assign(n, false);
        g = TNEANet::New();
        max_size = n;
    }
    void newVertex(int value) {
        g->AddNode(value);
    }
    void newEdge(int a, int b, int weight = 1) {
        g->AddEdge(a,b);
        TNEANet::TEdgeI EI = g->GetEI(a,b);
        g->AddIntAttrDatE(EI, weight, "weight");
    }
    void deleteNode(int value) {
        g->DelNode(value);
    }
    void deleteEdge(int a, int b) {
        g->DelEdge(a,b);
    }
    void DFS(int root) {
        visited.assign(max_size, false);
        DFSHelper(root);
    }
    void DFSHelper (int root) {
        cout << root << " ";
        visited[root] = true;
        TNEANet::TNodeI NI = g->GetNI(root);
        for (int i = 0; i < NI.GetOutDeg(); i++) {
            int curr = NI.GetOutNId(i);
            if(!visited[curr]) DFSHelper(curr);
        }
    }
    void BFS (int root) {
        visited.assign(max_size, false);
        queue<int> q;
        q.push(root);
        visited[root] = true;
        while (q.size() > 0) {
            int tmp = q.front();
            cout << tmp << " ";
            q.pop();
            TNEANet::TNodeI NI = g->GetNI(tmp);
            for (int i = 0; i < NI.GetOutDeg(); i++) {
                int curr = NI.GetOutNId(i);
                if(!visited[curr]) {
                    visited[curr] = true;
                    q.push(curr);
                }
            }
        }
    }
    void Prim () {
        vector<int> llave(g->GetNodes(), INT_MAX);
        vector<int> padre(g->GetNodes(), -1);
        priority_queue<customPair, vector <customPair> , greater<customPair> > q;
        visited.assign(max_size, false);
        llave[0] = 0;
        q.push(make_pair(0, 0));
        while(!q.empty()) {
            int current = q.top().second;
            q.pop();
            visited[current] = true;
            TNEANet::TNodeI NI = g->GetNI(current);
            for (int i = 0; i < NI.GetOutDeg(); i++) {
                int vertex = NI.GetOutNId(i);
                TNEANet::TEdgeI EI = g->GetEI(current,vertex);
                int weight = g->GetIntAttrDatE(EI, "weight");
                if(!visited[vertex] && llave[vertex] > weight) {
                    llave[vertex] = weight;
                    q.push(make_pair(weight, vertex));
                    padre[vertex] = current;
                }
            }

        }
        for(int i = 1; i < g->GetNodes(); i++) {
            cout << padre[i] << " - " << i << "\n";
        }
    }
    void Dijkstra () {
        vector<int> dist(g->GetNodes(), INT_MAX);
        priority_queue<customPair, vector <customPair> , greater<customPair> > q;
        visited.assign(max_size, false);
        dist[0] = 0;
        q.push(make_pair(0, 0));
        while(!q.empty()) {
            int current = q.top().second;
            q.pop();
            TNEANet::TNodeI NI = g->GetNI(current);
            for (int i = 0; i < NI.GetOutDeg(); i++) {
                int vertex = NI.GetOutNId(i);
                TNEANet::TEdgeI EI = g->GetEI(current,vertex);
                int weight = g->GetIntAttrDatE(EI, "weight");
                if(dist[vertex] > weight + dist[current]) {
                    dist[vertex] = dist[current] + weight;
                    q.push(make_pair(dist[vertex], vertex));
                }
            }
            
        }
        for(int i = 1; i < g->GetNodes(); i++) {
            cout << i << " - " << dist[i] << "\n";
        }
    }
    void FloydWarshall () {
        int graph_size = g->GetNodes();
        vector<int> row(graph_size, INT_MAX/2);
        vector<vector <int> > dist(graph_size, row);
        for (int i = 0; i < graph_size; i++) {
            dist[i][i] = 0;
        }
        for (TNEANet::TEdgeI EI = g->BegEI(); EI < g->EndEI(); EI++) {
            dist[EI.GetSrcNId()][EI.GetDstNId()] = g->GetIntAttrDatE(EI, "weight");
        }
        for (int i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                for (int k = 0; k < graph_size; k++) {
                    if(dist[j][k] > dist[j][i] + dist[i][k]) {
                        dist[j][k] = dist[j][i] + dist[i][k];
                    }
                }
            }
        }
        for (int i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                if(dist[i][j] == INT_MAX/2) {
                    cout << "-  ";
                } else {
                    cout << dist[i][j] << " ";
                    if(dist[i][j] / 10 < 1) {
                        cout << " ";
                    }
                }
            }
            cout << "\n";
        }
    }
    void Kruskal () {
        int V = g->GetNodes();
        DisjointSets ds(V);
        for (TNEANet::TEdgeI EI = g->BegEI(); EI < g->EndEI(); EI++) {
            int u = EI.GetSrcNId();
            int v = EI.GetDstNId();
            int set_u = ds.find(u);
            int set_v = ds.find(v);
            if (set_u != set_v){
                cout << u << " - " << v << endl;
                ds.merge(set_u, set_v);
            }
        }
    }
    void PrintGraph() {
        g->Dump();
    }
private:
    PNGraph g;
    vector<bool> visited;
    int max_size;
};


int main() {
    Graph undirected_graph(9);
    Graph directed_graph(15);
    
    for(int i = 0; i<=8; i++) {
        undirected_graph.newVertex(i);
    }
    undirected_graph.newEdge(0,1,4);
    undirected_graph.newEdge(0,7,8);
    undirected_graph.newEdge(1,7,11);
    undirected_graph.newEdge(1,2,8);
    undirected_graph.newEdge(7,6,1);
    undirected_graph.newEdge(7,8,7);
    undirected_graph.newEdge(2,8,2);
    undirected_graph.newEdge(2,5,4);
    undirected_graph.newEdge(2,3,7);
    undirected_graph.newEdge(6,5,2);
    undirected_graph.newEdge(3,5,14);
    undirected_graph.newEdge(3,4,9);
    undirected_graph.newEdge(5,4,10);
    undirected_graph.newEdge(8,6,6);
    
    for(int i = 1; i<=14; i++) {
        directed_graph.newVertex(i);
    }
    
    directed_graph.newEdge(1,4,8);
    directed_graph.newEdge(4,8,2);
    directed_graph.newEdge(8,9,3);
    directed_graph.newEdge(9,12,4);
    directed_graph.newEdge(12,14,9);
    directed_graph.newEdge(14,13,2);
    directed_graph.newEdge(13,14,6);
    directed_graph.newEdge(11,12,6);
    directed_graph.newEdge(12,11,8);
    directed_graph.newEdge(12,9,2);
    directed_graph.newEdge(9,10,2);
    directed_graph.newEdge(10,6,6);
    directed_graph.newEdge(6,13,4);
    directed_graph.newEdge(8,7,3);
    directed_graph.newEdge(7,4,6);
    directed_graph.newEdge(4,7,3);
    directed_graph.newEdge(4,5,1);
    directed_graph.newEdge(5,6,9);
    directed_graph.newEdge(1,3,8);
    directed_graph.newEdge(3,2,7);
    directed_graph.newEdge(2,5,7);
    directed_graph.newEdge(3,5,8);
    directed_graph.newEdge(3,10,4);
    directed_graph.newEdge(10,3,10);
    directed_graph.newEdge(5,6,9);
    
    undirected_graph.PrintGraph();
    directed_graph.PrintGraph();

    cout << "Insert 1 vertex:\n";
    auto t1_a = high_resolution_clock::now();
    undirected_graph.newVertex(10);
    auto t2_a = high_resolution_clock::now();
    auto duration_a = duration_cast<microseconds>( t2_a - t1_a ).count();
    cout << "duration: " << duration_a << " μs\n";
    
    cout << "Insert 1 edge:\n";
    auto t1_b = high_resolution_clock::now();
    undirected_graph.newEdge(1,10,5);
    auto t2_b = high_resolution_clock::now();
    auto duration_b = duration_cast<microseconds>( t2_b - t1_b ).count();
    cout << "duration: " << duration_b << " μs\n";
    
    cout << "Delete 1 edge:\n";
    auto t1_d = high_resolution_clock::now();
    undirected_graph.deleteEdge(1,10);
    auto t2_d = high_resolution_clock::now();
    auto duration_d = duration_cast<microseconds>( t2_d - t1_d ).count();
    cout << "duration: " << duration_d << " μs\n";
    
    cout << "Delete 1 vertex:\n";
    auto t1_c = high_resolution_clock::now();
    undirected_graph.deleteNode(10);
    auto t2_c = high_resolution_clock::now();
    auto duration_c = duration_cast<microseconds>( t2_c - t1_c ).count();
    cout << "duration: " << duration_c << " μs\n";
    
    cout << "DFS:\n";
    auto t1_e = high_resolution_clock::now();
    directed_graph.DFS(0);
    auto t2_e = high_resolution_clock::now();
    auto duration_e = duration_cast<microseconds>( t2_e - t1_e ).count();
    cout << "duration: " << duration_e << " μs\n";
    
    cout << "BFS:\n";
    auto t1_f = high_resolution_clock::now();
    directed_graph.BFS(0);
    auto t2_f = high_resolution_clock::now();
    auto duration_f = duration_cast<microseconds>( t2_f - t1_f ).count();
    cout << "duration: " << duration_f << " μs\n";
    
    cout << "Prim:\n";
    auto t1_g = high_resolution_clock::now();
    undirected_graph.Prim();
    auto t2_g = high_resolution_clock::now();
    auto duration_g = duration_cast<microseconds>( t2_g - t1_g ).count();
    cout << "duration: " << duration_g << " μs\n";
    
    cout << "Kruskal:\n";
    auto t1_h = high_resolution_clock::now();
    undirected_graph.Kruskal();
    auto t2_h = high_resolution_clock::now();
    auto duration_h = duration_cast<microseconds>( t2_h - t1_h ).count();
    cout << "duration: " << duration_h << " μs\n";
    
    cout << "Dijkstra:\n";
    auto t1_i = high_resolution_clock::now();
    directed_graph.Dijkstra();
    auto t2_i = high_resolution_clock::now();
    auto duration_i = duration_cast<microseconds>( t2_i - t1_i ).count();
    cout << "duration: " << duration_i << " μs\n";
    
    cout << "Floyd-Warshall:\n";
    auto t1_j = high_resolution_clock::now();
    directed_graph.FloydWarshall();
    auto t2_j = high_resolution_clock::now();
    auto duration_j = duration_cast<microseconds>( t2_j - t1_j ).count();
    cout << "duration: " << duration_j << " μs\n";
    
    return 1;
}
