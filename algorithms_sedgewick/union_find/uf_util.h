#pragma once 

#include <iostream>
#include <map>
#include <memory>
#include <functional>

#include "uf_impl.h"

std::string default_algo = "wqupc";

//not the perfect factory, but we are more concerned about the algorithms here :)
std::unique_ptr<union_find> build_algorithm(uint64_t N, std::string const& algo_name){
    static std::map<std::string, std::function<std::unique_ptr<union_find>(uint64_t)>> str_to_algo = {
            std::make_pair(std::string("qf"), [](uint64_t N){return std::make_unique<union_find_quick_find>(N);})
        ,   std::make_pair(std::string("qu"), [](uint64_t N){return std::make_unique<union_find_quick_union>(N);})
        ,   std::make_pair(std::string("wqu"), [](uint64_t N){return std::make_unique<union_find_weighted_quick_union>(N);})
        ,   std::make_pair(std::string("qupc"), [](uint64_t N){return std::make_unique<union_find_quick_union_path_compression>(N);})
        ,   std::make_pair(std::string("wqupc"), [](uint64_t N){return std::make_unique<union_find_weighted_quick_union_path_compression>(N);})
    };

    auto algo_it = str_to_algo.find(algo_name);
    if (algo_it != str_to_algo.end())
        return (algo_it->second)(N);

    std::cerr << "invalid version, use default algo" << std::endl;
    return (str_to_algo[default_algo])(N);
}

//display stats
std::ostream& operator<<(std::ostream& os, union_find const& uf){
    os  << "connected components: " << uf.count() << std::endl
        << "algo: " << uf.name() << std::endl;
    return os;
}
