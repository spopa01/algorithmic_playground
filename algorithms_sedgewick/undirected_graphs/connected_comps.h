#ifndef __CONNECTED_COMPS_H__
#define __CONNECTED_COMPS_H__

#include "graph.h"

struct connected_comps_t{
    //finds the connected components
    connected_comps_t(graph_t const& g) : graph{g}
    {
        assert(g.is_valid());

        marked.resize(graph.vertices(), false);
        cc.resize(graph.vertices(), infinity);

        for(uint64_t v=0; v<graph.vertices(); ++v){
            if(!marked[v]){ dfs(v); ncc++; }
        }
    }

    //is v connected to w?
    bool connected(uint64_t v, uint64_t w){
        assert(v != w); 
        assert(v < graph.vertices());
        assert(w < graph.vertices());
        return cc[v] == cc[w];
    };

    //returns the connected component id associated to v
    uint64_t id(uint64_t v){
        assert(v < graph.vertices());
        return cc[v];
    }

    //returns the total number of connected components
    uint64_t count(){return ncc;}

private:
    void dfs(uint64_t v){
        marked[v] = true; cc[v] = ncc;
        for(auto w : graph.adj(v)){
            if(!marked[w]) dfs(w);
        }
    }

    graph_t const& graph;

    std::vector<bool> marked;
    std::vector<uint64_t> cc;
    uint64_t ncc{0};
};

#endif//__CONNECTED_COMPS_H__
