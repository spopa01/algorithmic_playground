// to compile (e.g.): g++ -std=c++14 basic_connected_comps_client.cpp -O3 , add -DLOG for logging
// to run (e.g.): ./a.out < datasets/tinyG.txt

#include "graph.h"
#include "connected_comps.h"

#include <cstdlib>

int main(int argc, char** argv){
    if(argc != 1){
        std::cerr << "invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    graph_t graph;
    std::cin >> graph;

    connected_comps_t cc{graph};

    std::cout << "Number of connected components: " << cc.count() << std::endl;

    std::vector<std::vector<uint64_t>> components{cc.count()};
    for(uint64_t v = 0; v<graph.vertices(); ++v){
        components[cc.id(v)].push_back(v);
    }

    for(uint64_t c = 0; c<components.size(); ++c){
        std::cout << "Component " << c << ": ";
        for(uint64_t v = 0; v<components[c].size(); ++v)
            std::cout << components[c][v] << " ";
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
