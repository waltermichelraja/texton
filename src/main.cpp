#include <memory>
#include <cstdio>
#include <iostream>
#include "benchmark/runner.hpp"
#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"
#include "buffer/rope.hpp"

void test_rope(){
    // Rope rope;
    // rope.insert(0,"Hello");
    // rope.insert(5," World");

    // std::cout<<rope.substr(0,rope.size())<<std::endl;
    // rope.erase(5,1);
    // std::cout<<rope.substr(0,rope.size())<<std::endl;
    Rope rope;

    rope.insert(0,"Hello");
    rope.insert(5," World");

    std::cout<<rope.substr(0,11)<<std::endl;
    std::cout<<rope.substr(3,5)<<std::endl;
}


int main(){
    std::remove("benchmark_results.csv");
    size_t ops=10000;
    size_t ops_random=5000;

    // test_rope();

    BenchmarkRunner::run_insert_test(
        "GapBuffer",
        [](){return std::make_unique<GapBuffer>();},
        ops
    );
    BenchmarkRunner::run_insert_test(
        "PieceTable",
        [](){return std::make_unique<PieceTable>("");},
        ops
    );
    BenchmarkRunner::run_insert_test(
        "Rope",
        [](){return std::make_unique<Rope>();},
        ops
    );

    BenchmarkRunner::run_random_insert_test(
        "GapBuffer",
        [](){return std::make_unique<GapBuffer>();},
        ops_random
    );
    BenchmarkRunner::run_random_insert_test(
        "PieceTable",
        [](){return std::make_unique<PieceTable>("");},
        ops_random
    );
    BenchmarkRunner::run_random_insert_test(
        "Rope",
        [](){return std::make_unique<Rope>();},
        ops_random
    );

    BenchmarkRunner::run_mixed_workload(
        "GapBuffer",
        [](){return std::make_unique<GapBuffer>();},
        50000
    );
    BenchmarkRunner::run_mixed_workload(
        "PieceTable",
        [](){return std::make_unique<PieceTable>("");},
        50000
    );
    BenchmarkRunner::run_mixed_workload(
        "Rope",
        [](){return std::make_unique<Rope>();},
        30000
    );
    return 0;
}