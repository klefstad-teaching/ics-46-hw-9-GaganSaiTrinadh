#include "./dijkstras.h"
#include <algorithm>
#include <queue>

struct NodeComparator {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    vector<int> distance(G.numVertices, INF);
    previous.resize(G.numVertices, -1);
    vector<bool> visited(G.numVertices, false);
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, NodeComparator> pq;
    distance[source] = 0;
    pq.push({source, 0});
    
    while (!pq.empty()) {
        int u = pq.top().first;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push({v, distance[v]});
            }
        }
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    if (distances[destination] == INF) {
        return path;  // No path exists
    }
    
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& v, int total) {
    if (v.empty()) {
        // Match exact expected output format for empty path
        cout << endl;
        cout << "Total cost is " << total << endl;
        return;
    }
    
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i < v.size() - 1) {
            cout << " ";
        }
    }
    // Add extra space at the end of the line to match expected output
    cout << " " << endl;
    cout << "Total cost is " << total << endl;
}