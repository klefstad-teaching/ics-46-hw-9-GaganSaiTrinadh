#include "./dijkstras.h"
#include <algorithm>
#include <queue>

struct NodeComparator {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    previous.assign(n, -1);  // Use assign instead of resize to ensure all values are -1
    vector<bool> visited(n, false);
    
    // Initialize priority queue
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
            
            // Check if we found a shorter path
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
    // Check if destination is valid and reachable
    if (destination < 0 || destination >= (int)distances.size() || distances[destination] == INF) {
        return path; // No path exists or invalid destination
    }
    
    // Reconstruct path
    for (int v = destination; v != -1; v = previous[v]) {
        // Safety check to avoid infinite loops or invalid indices
        if (v < 0 || v >= (int)previous.size()) {
            break;
        }
        path.push_back(v);
    }
    
    // Check if we actually reached the source (previous[source] should be -1)
    if (path.empty() || previous[path.back()] != -1) {
        return {}; // Incomplete path
    }
    
    reverse(path.begin(), path.end());
    return path;
}


void print_path(const vector<int>& v, int total) {
    if (v.empty()) {
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
    cout << " " << endl;
    cout << "Total cost is " << total << endl;
}