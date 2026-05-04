#include <iostream>
#include <memory>
#include "benchmark/runner.hpp"
#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"

int main(){
    size_t ops=100000;

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
    return 0;
}