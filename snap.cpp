#include "stdafx.h"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

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
                    cout << "checking " << dist[j][k] << endl;
                    if(dist[j][k] > dist[j][i] + dist[i][k]) {
                        dist[j][k] = dist[j][i] + dist[i][k];
                    }
                }
            }
        }
        for (int i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                cout << dist[i][j] << " ";
            }
            cout << "\n";
        }
    }
private:
    PNGraph g;
    vector<bool> visited;
    int max_size;
};


int main() {
    Graph test(100);
//    test.newVertex(0);
//    test.newVertex(1);
//    test.newVertex(2);
//    test.newVertex(3);
//    test.newVertex(4);
//    test.newVertex(5);
//    test.newVertex(6);
//    test.newVertex(7);
//    test.newVertex(8);
//    test.newEdge(0, 1, 4);
//    test.newEdge(0, 7, 8);
//    test.newEdge(1, 2, 8);
//    test.newEdge(1, 7, 11);
//    test.newEdge(2, 3, 7);
//    test.newEdge(2, 8, 2);
//    test.newEdge(2, 5, 4);
//    test.newEdge(3, 4, 9);
//    test.newEdge(3, 5, 14);
//    test.newEdge(4, 5, 10);
//    test.newEdge(5, 6, 2);
//    test.newEdge(6, 7, 1);
//    test.newEdge(6, 8, 6);
//    test.newEdge(7, 8, 7);
    test.newVertex(0);
    test.newVertex(1);
    test.newVertex(2);
    test.newVertex(3);
    test.newEdge(0, 2, -2);
    test.newEdge(2, 3, 2);
    test.newEdge(3, 1, -1);
    test.newEdge(1, 0, 4);
    test.newEdge(1, 2, 3);
    //test.DFS(0);
    //cout << endl;
    //test.BFS(0);
    //cout << endl;
    //test.Prim();
    test.FloydWarshall();
    return 1;
}
