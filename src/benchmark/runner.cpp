#include "benchmark/runner.hpp"
#include <chrono>
#include <iostream>

void BenchmarkRunner::run_insert_test(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations){
    using namespace std::chrono;
    const int runs=5;
    long long total_time=0;

    for(int r=0;r<runs;r++){
        auto buffer=factory();
        auto start=high_resolution_clock::now();
        for(size_t i=0;i<operations;i++){
            buffer->insert(buffer->size(),"a");
        }
        auto end=high_resolution_clock::now();
        total_time+=duration_cast<microseconds>(end-start).count();
    }
    std::cout<<"--- "<<name<<" ---"<<std::endl;
    std::cout<<"operations: "<<operations<<std::endl;
    std::cout<<"average time [microseconds]: "<<(total_time/runs)<<std::endl;
}