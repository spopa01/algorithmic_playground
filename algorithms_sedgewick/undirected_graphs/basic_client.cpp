// to compile (e.g.): g++ -std=c++14 basic_client.cpp -O3
// to run (e.g.): ./a.out < datasets/tinyG.txt

#include "graph.h"

#include <cstdlib>

int main(int argc, char** argv){
    if(argc != 1){
        std::cerr << "invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    graph_t graph;
    std::cin >> graph;
    std::cout << graph;

    return EXIT_SUCCESS;
}
