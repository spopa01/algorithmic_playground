#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <vector>
#include <set>
#include <cassert>

const uint64_t infinity = std::numeric_limits<uint64_t>::max();

//Graph represenation for static graphs...

/*

representation          space       edge between v and w?   iterate over edges incident to v?

list of edges           E                   E                           E
adjacency matrix        V^2                 1                           V
adjacency list          E + V               degree(v)                   degree(v)
adjacency set           E + V               log (degree(v))             degree(v)               <--- winner

*/

struct graph_t{
    bool is_valid() const {return valid;}

    //number or vertices
    uint64_t vertices() const {
        assert(valid);
        return adjs.size();
    }

    //vertices adjancent to v
    std::set<uint64_t> const& adj(uint64_t v) const {
        assert(valid);
        assert(v < adjs.size());
        return adjs[v];
    }

private:
    friend std::istream& operator>>(std::istream& is, graph_t& g);

    bool valid{false};

    //create a graph with n vertices
    void set_size(uint64_t n){adjs.resize(n);}

    //add an edge between v and w
    void add_edge(uint64_t v, uint64_t w){
        assert(v != w); //disallow self-loops
        assert(v < adjs.size() && w < adjs.size());

        adjs[v].insert(w);
        adjs[w].insert(v);
    }

    //adjustancy set, by using a set we disallow parallel edges.
    std::vector<std::set<uint64_t>> adjs;
};

//display the graph (not really symmetric to operator>>)
std::ostream& operator<<(std::ostream& os, graph_t const& g){
    assert(g.is_valid());

    os  << "Number of vertices: " << g.vertices() << std::endl;
    for(uint64_t v=0; v < g.vertices(); ++v){
        os << v << ": ";
        for(auto w : g.adj(v))
            os << w << " ";
        os << std::endl;
    }
    return os;
}

//read the graph from a stream (from Sedgewick's datasets)
std::istream& operator>>(std::istream& is, graph_t& g){
    assert(!g.is_valid());

    uint64_t n{0};
    uint64_t e{0};
    std::cin >> n;
    std::cin >> e;

    g.set_size(n);

    uint64_t cnt_e{0};
    if(!std::cin.eof()){
        while(true){
            uint64_t v,w;
            try{
                std::cin >> v >> w;
                if(std::cin.eof())
                    break;
                g.add_edge(v,w);
                ++cnt_e;
            }catch (...){
                break;
            }
        }
    }

    if(cnt_e != e){
        is.setstate(std::ios::failbit);
        throw std::runtime_error("Error reading the graph");
    }

    g.valid = true;
    return is;
}

#endif//__GRAPH_H__
