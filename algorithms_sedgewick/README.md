## Lecture 1: Union-Find

### Dynamic Connectivity

Given a set of `N` objects (represented as numbers between `0...N-1`), the objective is to efficiently support the following two operations:

* _union/connect command_: connect two objects
* _find/connected query_: is there a path connecting the two objects?

__Note:__ Here we are not interested in returning a path (or shortest path) between two objects,
but only to answer the question _is there a path connecting the two objects?_.

When modelling the connection between two objects we assume that _is connected to_ is an equivalence relation:

* _reflexive:_ `p` is connected to `p`
* _symmetric:_ if `p` is connected to `q`, then `q` is connected to `p`
* _transitive:_ if `p` is connected to `q` and `q` is connected to `r`, then `p` is connected to `r`

Further, we model groups of connected objects as _connected components_.
A connected component is defined as the maximal set of objects that are mutually connected.

The algorithms defined in the next sections will gain efficiency by maintaining these connected components.

To reflect this model, we implement the two required operations as:

* _find/connected query_: check if two objects are in the same component
* _union/connect command_: replace components containing two objects with their union

The goal is to design an efficient data structure for union-find which supports:

* a huge number of objects `N`
* a huge number of operations `M` of intermixed commands and queries.

Union-find data type specification:

```C++
struct UF {
    //initialise union-find data structure with N objects [0 … N-1]
    UF(unsigned int N);

    //add connection between p and q (union/connect command)
    void connect(unsigned int p, unsigned int q);
    //are p and q connected - in the same component (find/connected query)
    bool connected(unsigned int p, unsigned int q);

    //component identifier for p in [0 … N-1]
    unsigned int find(unsigned int p);

    //number of components
    unsigned int count();
};
```

Dynamic connectivity client design (an implementation can be found in `union_find/uf_client.cpp`):

* Read in number of objects `N` from standard input
* Repeat:
    * Read in pair of integers from input
    * If they are not yet connected, connect them and print out pair

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
* interpretation: `id[i]` is parent of `i` and root of `i` is `id[id[...id[i]...]]` (keep going until it doesn't change, no cycles)

Operations implementation:

* find/connected query: check if `p` and `q` have the same root
* union/connect command: to merge components containing `p` and `q`, set the `id` of the `p`'s root to the `id` of `q`'s root

Defects: trees can get tall which makes find too expensive (`N` array accesses per query).

### Quick Union Improvements

#### Improvement 1: Weighted Quick-Union

* modify quick-union to avoid tall trees
* keep track of size of each tree (number of objects or height)
* balance by linking root of smaller tree to root of larger tree

Running time:

* find/connected query: takes time proportional to the depth of `p` and `q`
* union/connect command: takes constant time (given roots)

Proposition: depth of any node `x` si at most `lg(N) [lg = base-2 logarithm]`

Proof: 

* depth of `x` increases by `1` when the tree containing `x` (`T1`) is merged into another (bigger) tree (`T2`)
* which means that the size of the tree containing `x` at least doubles (`|T2| >= |T1|`)
* which further means that the size of tree containing `x` can only double at most `lg(N)` times => `O(lg(N))` depth

#### Improvement 2: Quick-Union with Path Compression

* just after computing the root of `p`, set the `id` of each examined node to point to the root (this requires two-passes)
* in practice it is enough to make every other node in the path no point to its grandparent, thereby halving path length (which can be done in only one-pass)

#### Improvement 3: Weighted Quick-Union with Path Compression

Proposition:

* starting from an empty data structure, any sequence of `M` union-find operations on `N` objects is upper bounded by `c(N + M lg* (N))` array accesses (`lg*` - iterative log function, `lg*(2^65536) = 5`)
* so, in practice `WQUPC` is _almost_ linear.

#### Summary

Algorithm | Worst-case time
|:----------:|:-----------:|
| quick-find | `M N` |
| quick-union | `M N` |
| weighted QU | `N + M lg(N)` |
| quick-union + path compression | `N + M lg(N)` |
| weighted quick-union + path compression | `N + M lg*(N)` |
Where `M` represents the number of union-find operations on a set of `N` objects.

The implementation of these `5` algorithms (`QF`, `QU`, `QUPC`, `WQU`, `WQUPC`) can be found in `union_find/uf_impl.h`

### Union-Find Applications

#### Percolation

Percolation is a model for many physical systems:

* `N-by-N` grid of sites
* each site is open with probability `p` (or blocked with probability `1-p`)
* system percolates iff top and bottom are connected by sites

Likelihood of percolation (Percolation phase transition threshold)

When `N` is large, theory guarantees a sharp threshold `p*`

* `p > P*`: almost certainly percolates
* `p < P*`: almost certainly does not percolate

Question: what is the value of `p*` ? (`p ~ 0.593` for `N = 100`)

Answer: it can only be computed numerically using Monte-Carlo simulations.

Monte-Carlo simulation methodology:

* initialise `N-by-N` whole grid to be blocked
* declare random sites open until top connected to bottom
* compute the percentage of open sites at the time of percolation and use it to estimate `p*` (by running he test many-many times)

Question: How to check whether an `N-by-N` system percolates?

* create an object for each site and name them `0...N^2-1`
* sites are in same component if connected by open sites
* the system percolates iff any site on bottom row is connected yo site on top row (a brute-force algorithm would require `O(N^2)`)
* in order to avoid `N^2`, we introduce `2` virtual sites (and connect one to top and one to bottom)
* the system percolates iff virtual top site is connected to virtual bottom site
* the opening of a new site is modelled by connecting the newly opened site to all of its (opened) adjacent sites (up to `4` calls to union)

## Lecture 2: Analysis of Algorithms

Algorithms are sooo cool but I like math on paper...

## Lecture 3: Stacks and Queues

### Stacks

### Resizing Arrays

### Queues

### Stack and Queue Applications

....

## Lecture 12: Undirected Graphs

### Introduction to graphs

Graph: Set of _vertices_ connected pairwise by _edges_.

| graph | vertex | edge |
|:----------:|:-----------:|:----------:|
| communication | telephone | fiber optic cable |
| circuit |gate, resistor, processor | wire |
| mechanical | joint | rod, beam, spring |
| financial | stock, currency | transactions |
| transportation | street intersection, airport | highway, airway |
| internet | class C network | connection |
| game | board position | legal move |
| social relationship | person, actor | friendship, movie cast |
| neural network | neuron | synapse |
| protein network | protein | protein-protein interaction |
| molecule | atom | bond |

Graph terminology:

* path = sequence of vertices connected by edges
* cycle = path whose first and last vertices are the same
* two vertices are connected if there is a path between them

Graph-processing problems:

* Path : is there a path between s and t?
* Shortest path: what is the shortest path between s and t?
* Cycle: is there a cycle in the graph?
* Euler tour: is there a cycle that uses each edge exactly once?
* Hamilton tour: is there a cycle that uses each vertex exactly once?
* Connectivity: is there a way to connect all the vertices?
* MST (minimum spanning tree): what is the best way to connect all the vertices?
* Bi-Connectivity: is there a vertex whose removal disconnects the graph?
* Planarity: Can we draw the graph in the plane with no crossings edges?
* Graph isomorphism: Do two adjacency lists represent the same graph?

### Graph API

Vertex representation: use integers between 0 and V-1 and convert between integers and names with symbol table.

Type of anomalies: self-loop and parallel edges.

Graph data type specification:

```C++
struct graph{
	//create an empty graph with V vertices
	graph(unsigned int v);

	//add an edge v-w
	void add_edge(unsigned int v, unsigned int w);

	//number of vertices
	unsigned int v();

	//number of edges
	unsigned int e();

	//vertices adjacent to v
	std::vector<unsigned int>& adj(unsigned int v);
};

//compute the degree of v (the number of edges associated with this vertex)
unsigned int degree(graph const& g, unsigned int v);
	
//compute maximum degree
unsigned int max_degree(graph const& g);

//average degree
double average_degree(graph const& g){ return 2.0 * g.e()/g.v(); }

//number of self loops
unsigned int num_of_self_loops(graph const& g);
```

Basic graph client design (an implementation can be found in `graph/basic_graph_client.cpp`):

- Read in the number of vertices V from input
- Read in the number of edges E from input
- Repeat:
	- Read in pairs of integers
- Repeat:
	- For each vertices print the adjacent list

Graph representations:

- list of edges: maintain a list of the edges (linked list or vector).
- adjacency matrix: maintain a 2D `VxV` boolean array; for each edge v-w in graph: `adj[v][w] = adj[v][w] = true` (two entries for each edge), better for dense graphs.
- adjacency list: maintain vertex-indexed array of lists(bags), better for sparse graphs.

Note: real world graphs tend to be sparse.

| [Representation] | [Space] | [Add edge] | [Is v connected to w?] | [Iterate over vertices adjacent to v] |
|:----------:|:-----------:|:-----------:|:-----------:|:-----------:|
| list of edges | `E` | `1` | `E` | `E` |
| adjacency matrix | `V^2` | `1` | `1` | `V` |
| adjacency list | `V+E` | `1` | `degree(V)` | `degree(V)` |

Note: adjacency matrix disallows parallel edges.

### Depth-First Search

Goal: systematically search through a graph (mimic maze exploration).

DFS(to visit a vertex `v`):

- Mark `v` as visited.
- Recursively visit all unmarked vertices `w` adjacent to `v`.

Typical applications:

- find all vertices connected to a given source vertex.
- find a path between two vertices.

Design pattern: decouple graph data type from graph processing.

- create a graph object
- pass the graph to a graph-processing routine
- query that graph-processing routine for information

```C++
struct paths{
	//finds paths in g from source s
	paths(graph const& g, unsigned int s);

	//is there a path from s to v?
	bool has_path_to(unsigned int v);

	//path from s to v, empty string if no such path
	std::vector<unsigned int>& path_to(unsigned int v);
};
```

A possible basic client which prints all vertices connected to a given vertex s (`graph/basic_graph_paths_client.cpp`)

```C++
paths ps{g, s};
for(unsigned int v = 0; v < g.v(); ++v)
	if(ps.has_path_to(v))
		std::cout << v << std::cout;
```

The algorithm is using 2 arrays:

- a boolean array `marked` used to mark the visited vertices.
- an unsigned int array `edge_to` used to keep track of paths; `edge_to[w] = v` means that edge `v-w` taken to visit `w` for first time.

An implementation can be found in `graph/paths_dfs.h`.

Proposition: DFS marks all vertices connected to s in time proportional to the sum of their degrees.

Pf: Each vertex connected to s is visited once.

Other application examples: flood fill

Assumptions: a picture has millions to billions of pixels.

Solution: build a grid graph, where

- vertex: pixels
- edge: only between adjacent pixels with similar colour
- blob: all pixels connected to given pixel.


### Breadth-First Search

### Connected Components

### Graph Challenges

