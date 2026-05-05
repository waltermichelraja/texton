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

#include <random>

void BenchmarkRunner::run_random_insert_test(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations){
    using namespace std::chrono;
    const int runs=5;
    long long total_time=0;

    std::mt19937 rng(42);
    for(int r=0;r<runs;r++){
        auto buffer=factory();
        auto start=high_resolution_clock::now();
        for(size_t i=0;i<operations;i++){
            size_t pos = buffer->size()==0 ? 0 : rng()%(buffer->size()+1);
            buffer->insert(pos,"a");
        }
        auto end=high_resolution_clock::now();
        total_time+=duration_cast<microseconds>(end-start).count();
    }

    std::cout<<"--- "<<name<<" [random insert] ---"<<std::endl;
    std::cout<<"operations: "<<operations<<std::endl;
    std::cout<<"average time [microseconds]: "<<(total_time/runs)<<std::endl;
}