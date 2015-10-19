# Algorithms, Part 1

## Week 0: Welcome to Algorithms

Algorithms and Data Structures are sooo cool...

## Week 1: Union-Find

### Dynamic Connectivity

Given a set of `N` objects (representated as numbers between `0...N-1`), the objective is to efficiently support the following two operations:
* __union/connect command__: connect two objects
* __find/connected query__: is there a path connecting the two objects?

Observation: here we are not interested in returning a path (or shortest path) between two objects,
but only to answer the question _is there a path connecting the two objects?_.

When modeling the connection between two objects we assume that __is connected to__ is an equivalence relation:
* reflexive: `p` is connected to `p`
* symmetric: if `p` is connected to `q`, then `q` is connected to `p`
* transitive: if `p` is connected to `q` and `q` is connected to `r`, then `p` is connected to `r`

Further, when we have a group of connected objects, we group them into connected components.
A connected component is defined as the maximal set of objects that are mutually connected.

The algorithms defined in the next sections will gain efficiency by maintaining these connected components.

To reflect this model, we implement the two required operations as:
* __find/connected query__: check if two objects are in the same component
* __union/connect command__: replace components containing two objects with their union

The goal is to design an efficient data structure for union-find which supports:
* a huge number of objects `N`
* a huge number of operations `M` (commands/queries)
* intermixed find queries and union commands

Union-find data type specification:

```C++
struct UF {
    //initialize union-find data structure with N objects (0 to N-1)
    UF(unsigned int N);

    //add connection between p and q (union/connect command)
    void connect(unsigned int p, unsigned int q);
    //are p and q connected - in the same component (find/connected query)
    bool connected(unsigned int p, unsigned int q);

    //component identifier for p (0 to N-1)
    unsigned int find(unsigned int p);
    //number of components
    unsigned int count();
};
```

Dynamic connectivity client design (an implementation can be found in union_find/uf_client.cpp):
* Read in number of objects `N` from standard input
* Repeat:
    * read in pair of integers from input
    * if they are not yet connected, connect them and print out pair

### Quick Find (eager approach)

Data structure:
* integer array `id[]` of size `N`
* interpretation: `p` and `q` are connected iff they have the same `id`

Operations implementation:
* find/connected query: check if `p` and `q` have the same `id`
* union/connect command: to merge components containing `p` and `q`, change all entries whose `id` equals `id[p]` to `id[q]`

Cost mode: number of array accesses (for read and write)
Defect: Union too expensive (`N` array accesses per command), so it can take `O(N^2)` array accesses to process sequence of `N` union commands on `N` objects.

### Quick Union (lazy approach)

Data structure:
* integer array `id[]` of size `N`
* interpretation: `id[i]` is parent of `i` and root of `i` is `id[id[...id[i]...]]` (keep going until it doesn't change -> no cycles)

Operations implementation:
* find/connected query: check if `p` and `q` have the same root
* union/connect command: to merge components containing `p` and `q`, set the `id` of the `p`'s root to the `id` of `q`'s root

Defects:
* trees can get tall
* find too expensive (`N` array accesses per query)

### Quick Union Improvements

#### Improvement 1: Weighted Quick-Union

Weighted quick-union
* modify quick-union to avoid tall trees
* keep track of size of each tree (number of objects or height)
* balance by linking root of smaller tree to root of larger tree

Running time:
* find/connected query: takes time proportional to the depth of `p` and `q`
* union/connect command: takes constant time (given roots)

Proposition: 
* depth of any node `x` si at most `lg(N) [lg = base-2 logarithm]`

Proof: 
* depth of `x` increases by `1` when the tree containing `x` (`T1`) is merged into another (bigger) tree (`T2`)
* which means that the size of the tree containing `x` at least doubles (`|T2| >= |T1|`)
* which further means that the size of tree containing `x` can only double at most `lg(N)` times => `O(lg(N))` depth

#### Improvement 2: Quick-Union with Path Compression

Quick-Union with Path Compression:
* just after computing the root of `p`, set the `id` of each examinated node to point to the root (this requires two-passes)
* in practice it is enough to make every other node in the path no point to its grandparent, thereby halving path length (which can be done in only one-pass)

#### Improvement 3: Weighted Quick-Union with Path Compression

Proposition:
* starting from an emptydata structure, any sequence of `M` union-find operations on `N` objects is upper bounded by `c(N + M lg* (N))` array accesses (`lg*` - iterative log function, `lg*(2^65536) = 5`)
* so, in practice WQUPC is _almost_ liniar.

#### Summary

algorithm | worst-case time
|:----------:|:-----------:|
| quick-find | `M N` |
| quick-union | `M N` |
| weighted QU | `N + M lg(N)` |
| quick-union + path compression | `N + M lg(N)` |
| weighted quick-union + path compression | `N + M lg*(N)` |
Where `M` represents the number of union-find operations on a set of `N` objects.

The implementation of these `5` algorithms (QF, QU, QUPC, WQU, WQUPC) can be found in union_find/uf_impl.h

### Union Find Applications

#### Percolation

Percolation is a model for many physical systems:
* `N-by-N` grid of sites
* each site is open with probability `p` (or blocket with probability `1-p`)
* system percolates iff top and bottom are connected by sites

Likelihood of percolation (Percolation phase transition threshol)

When `N` is large, theory guarantees a sharp threshold `p*`
* `p > P*`: almost certainly percolatrs
* `p < P*`: almost certainly does not percolate

Question: what is the value of `p*` ? (`p ~ 0.593` for `N = 100`)
Answer: it can only be computed numerically using monte-carlo simulations.

Monte-Carlo simulation methodology:
* initialize `N-by-N` whole grid to be blocked
* declare random sites open until top connected to bottom
* compute the percentage of open sites at the time of percolation and use it to estimate `p*` (by running he test many-many times)

Question: How to check whether an `N-by-N` system percolates?
* create an object for each site and name them `0...N^2-1`
* sites are in same component if connected by open sites
* the systempercolates iff any site on bottom row is connected yo site on top row (a brute-force algorithm would require `O(N^2)`)
* in order to avoid `N^2`, we introduce `2` virtual sites (and connect one to top and one to bottom)
* the system percolates iff virtual top site is connected to virtual bottom site
* the opening of a new site is modeled by connecting the newly opened site to all of its (opened) adjacent sites (up to `4` calls to union)

## Week 1: Analysis of Algorithms

I like math on paper...

## Week 2: Stacks and Queues

### Stacks

### Resizing Arrays

### Queues

### Stack and Queue Applications

....
