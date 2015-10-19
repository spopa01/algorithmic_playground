/*
to compile (e.g.): g++ -std=c++14 percolation.cpp -O3
to run (e.g.): time ./a.out
    grid size: 100x100
    number of simulations: 10000
    percolation threshold: 0.593

    real    0m10.856s
    user    0m10.656s
    sys     0m0.196s
*/

#include <cstdlib>
#include <random>
#include <iomanip>

#include "uf_impl.h"
#include "uf_util.h"

struct percolation_simulation{
    percolation_simulation(uint64_t sz)
        : N{sz}, top{N*N}, bottom{top+1}
    {
        //add 2 virtual cells: top, bottom (read he notes)
        //and use the fastes algorithm (for large number of cells)
        uf = build_algorithm(N*N+2, "wqupc");

        cells.resize(N*N);
        std::iota(cells.begin(), cells.end(), 0);
        //open the cells in a random order
        std::random_device rd;
        std::default_random_engine gen{rd()};
        std::shuffle(cells.begin(), cells.end(), gen);
        //init all cells as closed
        state.resize(N*N, 0);
    }

    uint64_t run(){
        uint64_t pos = 0;
        for (; pos < cells.size(); ++pos) {
            //stop when the top and bottom cells are connected
            if (uf->connected(top, bottom)) break;
            //set the cell as open
            state[cells[pos]] = 1;
            //connect it to its neighbours
            connect_with_neighbors(cells[pos]);
        }
        return pos;
    }

    void connect_with_neighbors(uint64_t cell) {
        //compute 2D coordinates (l = line, c = column)
        uint64_t l{cell / N}, c{cell % N};

        //do the necessary connections
        uint64_t neighbours[] = {   (l>0    ? (l-1)*N+c : c),
                                    (l<N-1  ? (l+1)*N+c : l*N+c),
                                    (c>0    ? l*N+(c-1) : l*N+c),
                                    (c<N-1  ? l*N+(c+1) : l*N+c)  };
        for (auto neighbour : neighbours)
            if (state[neighbour])
                uf->connect(cell, neighbour);

        //connect to the virtual cells if possible
        if (l == 0) uf->connect(cell, top);
        if (l == N-1) uf->connect(cell, bottom);
    }

    uint64_t N;
    //list of closed cells
    std::vector<uint64_t> cells;
    std::vector<uint64_t> state;
    //indexes of the 2 virtual cells
    uint64_t top, bottom;
    //union-find algo
    std::unique_ptr<union_find> uf;
};

//an experiment is composed from multiple simulations
struct percolation_experiment {
    percolation_experiment(uint64_t sz, uint64_t rep)
        : N{sz}, M{rep}
    {}

    double run() {
        uint64_t result = 0;
        for (uint64_t i=0; i<M; ++i) {
            percolation_simulation ps{N};
            result += ps.run();
        }
        return (double)result / (N*N*M);
    }

    uint64_t N, M;
};

int main(int argc, char** argv){
    //the size of the grid
    uint64_t N = 100;
    std::cout << "grid size: " << N << "x" << N << std::endl;

    //the number of dimulations run as part of an experiment
    uint64_t M = 10000;
    std::cout << "number of simulations: " << M << std::endl;

    percolation_experiment pe{N, M};
    double th = pe.run();
    std::cout   << "percolation threshold: "
                << std::fixed << std::setprecision( 3 ) << th
                << std::endl;

    return EXIT_SUCCESS;
}
