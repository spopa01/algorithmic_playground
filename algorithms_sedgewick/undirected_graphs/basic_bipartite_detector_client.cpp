// to compile (e.g.): g++ -std=c++14 basic_bipartite_detector_client.cpp -O3
// to run (e.g.): ./a.out < datasets/bipartiteG.txt

#include "graph.h"
#include "bipartite_detector.h"

#include <cstdlib>

int main(int argc, char** argv){
    if(argc != 1){
        std::cerr << "Invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    graph_t graph;
    std::cin >> graph;

    bipartite_detector_t detector{graph};
    if(detector.positive()){
        auto& labels = detector.get_labels();

        std::cout << "Partition A: ";
        for(auto v=0; v<graph.vertices(); ++v)
            if(labels[v] == label_t::neg)
                std::cout << v << " ";
        std::cout << std::endl;

        std::cout << "Partition B: ";
        for(auto v=0; v<graph.vertices(); ++v)
            if(labels[v] == label_t::pos)
                std::cout << v << " ";
        std::cout << std::endl;
    }else{
        std::cout << "Not bipartite" << std::endl;
    }

    return EXIT_SUCCESS;
}
