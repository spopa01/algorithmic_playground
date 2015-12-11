// to compile (e.g.): g++ -std=c++14 basic_cycle_client.cpp -O3
// to run (e.g.): ./a.out < datasets/tinyG.txt

#include "graph.h"
#include "cycle_detector.h"

#include <cstdlib>

int main(int argc, char** argv){
    if(argc != 1){
        std::cerr << "invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    graph_t graph;
    std::cin >> graph;

    cycle_detector_t detector{graph};
    if(detector.positive()){
        std::cout << "First detected cycle: ";
        for(auto e : detector.get_cycle())
            std::cout << e << " ";
        std::cout << std::endl;
    }else{
        std::cout << "No cycle detected" << std::endl;
    }

    return EXIT_SUCCESS;
}
