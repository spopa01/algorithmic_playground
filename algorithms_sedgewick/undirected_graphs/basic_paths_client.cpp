// to compile (e.g.): g++ -std=c++14 basic_paths_client.cpp -O3
// to run (e.g.): ./a.out algo < datasets/tinyG.txt
//      where algo: dfs_rec | dfs_eq_rec | dfs | bfs

// DFSRec vs DFSEqRec
// run: ./a.out dfs_rec < datasets/tinyG.txt > dfs_rec.txt
// run: ./a.out dfs_eq_rec < datasets/tinyG.txt > dfs_eq_rec.txt
// the following command should return 0 : sdiff -s dfs_rec.txt dfs_eq_rec.txt | wc -l

#include "graph.h"
#include "paths.h"

#include <map>
#include <string>

#include <cstdlib>

std::string default_algo = "dfs_rec";

//not the perfect factory, but we are more concerned about the algorithms here :)
std::unique_ptr<paths_t> build_algorithm(graph_t const& graph, uint64_t source, std::string const& algo){
    static std::map<std::string, std::function<std::unique_ptr<paths_t>(graph_t const& graph, uint64_t source)>> str_to_algo = {
            std::make_pair(std::string("dfs_rec"), [](graph_t const& graph, uint64_t source){return std::make_unique<dfs_rec_paths_t>(graph,source);})
        ,   std::make_pair(std::string("dfs_eq_rec"), [](graph_t const& graph, uint64_t source){return std::make_unique<dfs_eq_rec_paths_t>(graph,source);})
        ,   std::make_pair(std::string("dfs"), [](graph_t const& graph, uint64_t source){return std::make_unique<dfs_paths_t>(graph,source);})
        ,   std::make_pair(std::string("bfs"), [](graph_t const& graph, uint64_t source){return std::make_unique<bfs_paths_t>(graph,source);})
    };

    auto algo_it = str_to_algo.find(algo);
    if (algo_it != str_to_algo.end())
        return (algo_it->second)(graph,source);

    std::cerr << "Invalid algo, use default algo" << std::endl;
    return (str_to_algo[default_algo])(graph,source);
}

int main(int argc, char** argv){
    if (argc != 1 && argc != 2){
        std::cerr << "Invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    auto algo = default_algo;
    if (argc == 2) algo = argv[1];

    graph_t graph;
    std::cin >> graph;

    for(uint64_t v = 0; v < graph.vertices(); ++v){
        auto paths = build_algorithm(graph, v, algo);

        std::cout << "From " << v << " to" <<std::endl;
        for(uint64_t w = 0; w < graph.vertices(); ++w){
            if (v == w) continue;

            std::cout << "\t" << w;
            if(paths->connected_to(w)){
                std::cout << " (dist: " << paths->distance_to(w) << ") -> ";
                auto path = paths->path_to(w);
                for (auto vw : path)
                    std::cout << vw << " ";
            } else {
                std::cout << " -> no path";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
