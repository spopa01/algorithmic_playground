#ifndef __PATHS_H__
#define __PATHS_H__

#include "graph.h"

#include <stack>
#include <queue>
#include <deque>

struct paths_t{
    //finds paths in g from v to all connected vertices at a maximum distance d
    paths_t(graph_t const& g, uint64_t v) : graph{g}, source{v}
    {
        assert(source < graph.vertices());
        marked.resize(graph.vertices(), false);
        edge_to.resize(graph.vertices(), graph_t::infinity);
        dist_to.resize(graph.vertices(), graph_t::infinity);
        dist_to[source] = 0;
    }

    //just to force this type to be only base class
    virtual ~paths_t() = 0;

    //is there a path from v to w?
    bool connected_to(uint64_t w){
        assert(w < graph.vertices());
        return marked[w];
    };

    //returns the distance from vertex v a vertex w
    uint64_t distance_to(uint64_t w){
        assert(connected_to(w));
        return dist_to[w];
    }

    //returns the path from vertex v to vertex w
    std::deque<uint64_t> const path_to(uint64_t w){
        assert(connected_to(w));

        std::deque<uint64_t> path;
        do{
            path.push_front(w);
            w = edge_to[w];
        }while(w != graph_t::infinity);

        return path;
    }

protected:
    graph_t const& graph;
    uint64_t const source;

    std::vector<bool> marked;
    std::vector<uint64_t> edge_to;
    std::vector<uint64_t> dist_to;
};

paths_t::~paths_t(){};

//DFSRec
struct dfs_rec_paths_t : public paths_t{
    dfs_rec_paths_t(graph_t const& g, uint64_t v) : paths_t(g,v) {algo(source);}

private:
    void algo(uint64_t v){
        marked[v] = true;
        for(auto w : graph.adj(v)){
            if(!marked[w]){
                edge_to[w] = v;
                dist_to[w] = dist_to[v]+1;
                algo(w);
            }
        }
    }
};

//DFSEqRec - this dfs non-recursive/iterative implementation computes the same paths as DFSRec
struct dfs_eq_rec_paths_t : public paths_t{
    dfs_eq_rec_paths_t(graph_t const& g, uint64_t v) : paths_t(g,v)
    {
        for(uint64_t v = 0; v<graph.vertices(); ++v){
            auto& adj = graph.adj(v);
            its.push_back(adj.begin());
            its_end.push_back(adj.end());
        }

        algo(source);
    }

private:
    void algo(uint64_t v){
        mark_and_push(graph_t::infinity, v);
        while(not_empty()){
            v = peek();
            if(has_next(v)){
                auto w = next(v);
                if(!marked[w])
                    mark_and_push(v, w);
            } else {
                pop();
            }
        }
    }

    void mark_and_push(uint64_t v, uint64_t w){
        marked[w] = true;
        edge_to[w] = v;
        dist_to[w] = v != graph_t::infinity ? dist_to[v]+1 : 0;
        stack.push(w);
    }
    bool not_empty(){return stack.size() > 0;}
    uint64_t peek(){return stack.top();}
    void pop(){stack.pop();}

    bool has_next(uint64_t v){return its[v] != its_end[v];}
    uint64_t next(uint64_t v){auto w = *its[v]; its[v]++; return w;}

    //the stack is replacing the recursive calls
    std::stack<uint64_t> stack;

    //this is necessary in order to make DFS to behave exactly like the DFSRec
    std::vector<std::unordered_set<uint64_t>::iterator> its;
    std::vector<std::unordered_set<uint64_t>::iterator> its_end;
};

namespace util{
    uint64_t peek(std::stack<uint64_t>& adt){return adt.top();}
    uint64_t peek(std::queue<uint64_t>& adt){return adt.front();}
}

//generic paths finder (DFS/BFS)
//the only difference is the type of the underlying ADT used by the algo
template<typename ADT>
struct generic_paths_t : public paths_t{
    generic_paths_t(graph_t const& g, uint64_t v) : paths_t(g,v) {algo(source);}

private:
    void algo(uint64_t v){
        mark_and_push(graph_t::infinity, v);
        while(not_empty()){
            v = peek(); pop();
            for(auto w : graph.adj(v)){
                if(!marked[w])
                    mark_and_push(v, w);
            }
        }
    }

    void mark_and_push(uint64_t v, uint64_t w){
        marked[w] = true;
        edge_to[w] = v;
        dist_to[w] = v != graph_t::infinity ? dist_to[v]+1 : 0;
        adt.push(w);
    }
    bool not_empty(){return adt.size() > 0;}
    uint64_t peek(){return util::peek(adt);}
    void pop(){adt.pop();}

    //implementation specific:
    //stack for dfs and queue for bfs
    ADT adt;
};

using dfs_paths_t = generic_paths_t<std::stack<uint64_t>>;
using bfs_paths_t = generic_paths_t<std::queue<uint64_t>>;

#endif//__PATHS_H__
