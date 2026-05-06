#include <iostream>
#include <memory>
#include "benchmark/runner.hpp"
#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"

int main(){
    size_t ops=100000;
    size_t ops_random=20000;

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
    return 0;
}