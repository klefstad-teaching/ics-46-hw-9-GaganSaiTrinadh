#include <iostream>
#include <vector>
#include "dijkstras.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }

    Graph G;
    file_to_graph(argv[1], G);

    std::vector<int> previous;
    std::vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    for (int v = 0; v < G.numVertices; ++v)
    {
        std::vector<int> path = extract_shortest_path(distances, previous, v);
        print_path(path, distances[v]);
    }

    return 0;
}
