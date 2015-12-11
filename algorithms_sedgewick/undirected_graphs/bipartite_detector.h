#ifndef __BIPARTITE_DETECTOR_H__
#define __BIPARTITE_DETECTOR_H__

#include "graph.h"

//Bipartite is a graph whose vertices can be divided
//into two disjunct sets (POS and NEG) such that every
//edge connects a vertex in P to one in N.

struct bipartite_detector_t{
    //finds if an undirected graph is bipartite or not
    bipartite_detector_t(graph_t const& g) : g{g}{
        assert(g.is_valid());
        neg_or_pos.resize(g.vertices(), label_t::unknown);
        for(uint64_t v=0; v<g.vertices(); ++v){
            if(neg_or_pos[v] == label_t::unknown){
                neg_or_pos[v] = label_t::neg;
                dfs(v, v);
            }
        }
        if(bipartite == label_t::unknown)
            bipartite = label_t::pos;
    }

    bool positive()const{return bipartite == label_t::pos;}
    std::vector<label_t> const& get_labels()const{
        assert(bipartite == label_t::pos);
        return neg_or_pos;
    }
private:
    void dfs(uint64_t u, uint64_t v){
        neg_or_pos[v] = inv(neg_or_pos[u]);
        for(auto w : g.adj(v)){
            if(bipartite == label_t::neg)
                return;
            if(neg_or_pos[w]==label_t::unknown){
                dfs(v, w);
            }else if(neg_or_pos[w] == neg_or_pos[v]){
                bipartite = label_t::neg;
            }
        }
    }

    graph_t const& g;

    std::vector<label_t> neg_or_pos;
    label_t bipartite{label_t::unknown};
};

#endif//__BIPARTITE_DETECTOR_H__
