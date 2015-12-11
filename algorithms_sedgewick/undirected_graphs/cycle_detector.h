#ifndef __CYCLE_DETECTOR_H__
#define __CYCLE_DETECTOR_H__

#include "graph.h"

#include <deque>

struct cycle_detector_t{
    //finds cycles in an undirected graph
    cycle_detector_t(graph_t const& g) : g{g}{
        assert(g.is_valid());
        marked.resize(g.vertices(), false);
        edge_to.resize(g.vertices(), infinity);
        for(uint64_t v=0; v<g.vertices(); ++v)
            if(!marked[v]) dfs(v, v);
    }

    bool positive()const{return cycle.size();}
    std::deque<uint64_t> const& get_cycle()const{
        assert(cycle.size());
        return cycle;
    }
private:
    void dfs(uint64_t u, uint64_t v){
        marked[v] = true;
        for(auto w : g.adj(v)){
            if(cycle.size())
                return;

            if(!marked[w]){
                edge_to[w] = v;
                dfs(v, w);
            }else if(w!=u){
                build_path(v,w);
            }
        }
    }

    ///w--->u--->v
    ///\_________|
    ///w - u - v - w
    void build_path(uint64_t v, uint64_t w){
        for(auto x=v; x!=w; x=edge_to[x])
            cycle.push_front(x);
        cycle.push_front(w);
        cycle.push_front(v);
    }

    graph_t const& g;

    std::vector<bool> marked;
    std::vector<uint64_t> edge_to;
    std::deque<uint64_t> cycle;
};

#endif//__CYCLE_DETECTOR_H__
