// to compile (e.g.): g++ -std=c++14 uf_client.cpp -O3
// to run (e.g.): ./a.out wqupc < datasets/tinyUF.txt

#include "uf_impl.h"
#include "uf_util.h"
#include <cstdlib>

int main(int argc, char** argv){
    auto algo_name = default_algo;
    if (argc == 2)
        algo_name = argv[1];

    uint64_t N = 0;
    std::cin >> N;
#ifdef LOG
    std::cout << "number of objects: " << N << std::endl;
#endif

    auto uf = build_algorithm(N, algo_name);

    if (!std::cin.eof()) {
        while (true) {
            uint64_t p,q;
            std::cin >> p >> q;
            if (std::cin.eof())
                break;

            try {
                auto connected = uf->connected(p, q);
                if (!connected) {
                    uf->connect(p, q);
                }
#ifdef LOG
                std::cout << p << " " << q << " -> " << (connected ? "connected" : "not connected") << std::endl;
#endif
            } catch (...) {
                std::cerr << p << " " << q << " -> " << "invalid input" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }

    std::cout << *uf;
    return EXIT_SUCCESS;
}

/*
results:
tinyUF      ->  2 connected components
mediumUF    ->  3 connected components
largeUF     ->  6 connected components
*/
