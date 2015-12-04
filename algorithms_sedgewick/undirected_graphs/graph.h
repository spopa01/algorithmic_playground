#include <iostream>

struct graph{
    //create an empty graph with nv vertices
    graph(unsigned int nv){ adjs.resize(nv); }

    void add_graph(unsigned int v, unsigned int w){
        adjs[v].insert(w);
        adjs[w].insert(v);
    }

    //number or vertices
    unsigned int vertices(){ return adjs.size(); }

    //number of edges (per total or per vertex)
    unsigned int edges(unsigned int v = std::numeric_limits<unsigned int>::max()){
        unsigned int no_edges{0};
        if (v == std::numeric_limits<unsigned int>::max()){
            //this can be reduced to O(1) by keeping a counter 
            //for edges the total number of edges
            for (auto adj : adjs)
                no_edges += adj.size();
        } else
            no_edges = adjs[v].size();

        return no_edges;
    }

    //the maximum degree
    unsigned int max_edges(){
        unsigned int max_edges{0};
        for (auto adj : adjs)
            std::max(max_edges, adj.size());
        return max_edges;
    }

    //counts self-loops
    unsigned int num_of_self_loops(){
        unsigned int self_loops{0};
        for (auto v=0; v<vertices(); ++v){
            for(auto w : adjs[v])
                if (v == w) self_loops++;
        }
    }

    //vertices adjanced to v
    std::unordered_set<unsigned int> const& adj(unsigned int v){ return adjs[v]; }

    //adjustancy lists, by using a set we disallow parallel edges.
    //(space complexity O(V+E))
    std::vector<std::unordered_set<unsigned int>> adjs;
};
