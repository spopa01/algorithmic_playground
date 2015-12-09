#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <vector>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <cassert>

struct graph_t{
    enum {infinity = std::numeric_limits<uint64_t>::max()};

    //create an empty graph
    graph_t(){}

    //create a graph with sz vertices
    graph_t(uint64_t sz){resize(sz);}

    void resize(uint64_t sz){adjs.resize(sz);}

    void add_edge(uint64_t v, uint64_t w){
        assert(v != w && v < adjs.size() && w < adjs.size());

        bool inserted;
        std::tie(std::ignore, inserted) = adjs[v].insert(w);
        adjs[w].insert(v);

        if (inserted) total_num_edges++;
    }

    //number or vertices
    uint64_t vertices() const {
        assert(adjs.size());
        return adjs.size();
    }

    //number of edges/degree (per total or per vertex)
    uint64_t edges(uint64_t v = infinity) const {
        assert(adjs.size() && (v < adjs.size() || v == infinity));
        return v != infinity ? adjs[v].size() : total_num_edges;
    }

    //the maximum degree
    uint64_t max_edges() const {
        assert(adjs.size());
        uint64_t max_edges{0};
        for (auto& adj : adjs)
            max_edges = std::max<uint64_t>(max_edges, adj.size());
        return max_edges;
    }

    double average_edges() const {
        assert(adjs.size());
        return /*2. * */edges() / vertices();
    }

    //counts self-loops
    uint64_t num_of_self_loops() const {
        uint64_t self_loops{0};
        for (auto v=0; v<vertices(); ++v)
            for(auto w : adjs[v])
                if (v == w)
                    self_loops++;
        return self_loops;
    }

    //vertices adjancent to v
    std::unordered_set<uint64_t> const& adj(uint64_t v) const {
        assert(v < adjs.size());
        return adjs[v];
    }

private:
    //adjustancy lists, by using a set we disallow parallel edges.
    //(space complexity O(V+E))
    std::vector<std::unordered_set<uint64_t>> adjs;
    uint64_t total_num_edges{0};
};

//display stats
std::ostream& operator<<(std::ostream& os, graph_t const& g){
    os  << "Number of vertices: " << g.vertices() << std::endl
        << "Number of edges:    " << g.edges()    << std::endl
        << "Maximum edges:      " << g.max_edges() << std::endl
        << "Average edges:      " << g.average_edges() << std::endl
        << "Self-loops:         " << g.num_of_self_loops() << std::endl;
    for (uint64_t v=0; v < g.vertices(); ++v){
        os << v << ": ";
        for (auto w : g.adj(v))
            os << w << " ";
        os << std::endl;
    }
    return os;
}

//read the graph
std::istream& operator>>(std::istream& is, graph_t& g){
    uint64_t V{0};
    uint64_t E{0};
    std::cin >> V;
    std::cin >> E;

#ifdef LOG
    std::cout << "Number of vertices: " << V << std::endl;
    std::cout << "Number of edges: " << E << std::endl;
#endif

    g.resize(V);

    bool error {false};
    if (!std::cin.eof()) {
        while (true) {
            uint64_t v,w;
            std::cin >> v >> w;
            if (std::cin.eof())
                break;

            try {
                g.add_edge(v,w);
#ifdef LOG
                std::cout << v << " " << w << std::endl;
#endif
            } catch (...) {
                std::cerr << "error" << std::endl;
                break;
            }
        }
    }
#ifdef LOG
    std::cout<<std::endl;
#endif

    if (error || g.edges() != E)
        is.setstate(std::ios::failbit);

    return is;
}

#endif//__GRAPH_H__
