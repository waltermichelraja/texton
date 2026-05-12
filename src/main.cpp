#include <memory>
#include <cstdio>
#include <vector>
#include <iostream>
#include "benchmark/runner.hpp"
#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"
#include "buffer/rope.hpp"

int main(){
    Rope rope;
    rope.insert(0,"abc");
    rope.insert(3,"def");
    rope.insert(6,"ghi");
    std::cout<<rope.substr(0,rope.size())<<std::endl;
    rope.undo();
    std::cout<<rope.substr(0,rope.size())<<std::endl;
    rope.undo();
    std::cout<<rope.substr(0,rope.size())<<std::endl;
    rope.redo();
    std::cout<<rope.substr(0,rope.size())<<std::endl;
    rope.erase(1,1);
    std::cout<<rope.substr(0,rope.size())<<std::endl;
    rope.undo();
    std::cout<<rope.substr(0,rope.size())<<std::endl;

    return 0;
    
    std::remove("benchmark_results.csv");

    std::vector<size_t>sequential_scales={1000,5000,10000,20000};
    for(size_t ops:sequential_scales){
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
    }

    std::vector<size_t>random_scales={1000,3000,5000};
    for(size_t ops:random_scales){
        BenchmarkRunner::run_random_insert_test(
            "GapBuffer",
            [](){return std::make_unique<GapBuffer>();},
            ops
        );
        BenchmarkRunner::run_random_insert_test(
            "PieceTable",
            [](){return std::make_unique<PieceTable>("");},
            ops
        );
        BenchmarkRunner::run_random_insert_test(
            "Rope",
            [](){return std::make_unique<Rope>();},
            ops
        );
    }

    std::vector<size_t>mixed_scales={5000,10000,20000};
    for(size_t ops:mixed_scales){
        BenchmarkRunner::run_mixed_workload(
            "GapBuffer",
            [](){return std::make_unique<GapBuffer>();},
            ops
        );
        BenchmarkRunner::run_mixed_workload(
            "PieceTable",
            [](){return std::make_unique<PieceTable>("");},
            ops
        );
        BenchmarkRunner::run_mixed_workload(
            "Rope",
            [](){return std::make_unique<Rope>();},
            ops
        );
    }
    return 0;
}