#include "benchmark/runner.hpp"
#include <chrono>
#include <random>
#include <fstream>
#include <iostream>

void BenchmarkRunner::write_csv(const std::string&structure,const std::string&workload,size_t operations,long long time){
    static bool header_written=false;
    std::ofstream file;
    if(!header_written){
        file.open("benchmark_results.csv");
        file<<"structure,workload,operations,time_microseconds\n";
        header_written=true;
    }else{
        file.open("benchmark_results.csv",std::ios::app);
    }
    file<<structure<<","<<workload<<","<<operations<<","<<time<<"\n";
    file.close();
}

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
    write_csv(name,"sequential_insert",operations,total_time/runs);
}

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
    write_csv(name,"random_insert",operations,total_time/runs);
}

void BenchmarkRunner::run_mixed_workload(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations){
    using namespace std::chrono;
    const int runs=5;
    long long total_time=0;

    std::mt19937 rng(42);
    for(int r=0;r<runs;r++){
        auto buffer=factory();
        auto start=high_resolution_clock::now();
        for(size_t i=0;i<operations;i++){
            bool do_insert=(buffer->size()==0)||(rng()%100<70);
            if(do_insert){
                size_t pos=(buffer->size()==0) ? 0 : rng()%(buffer->size()+1);
                buffer->insert(pos,"a");
            }else{
                size_t pos=rng()%buffer->size();
                buffer->erase(pos,1);
            }
        }
        auto end=high_resolution_clock::now();
        total_time+=duration_cast<microseconds>(end-start).count();
    }
    std::cout<<"--- "<<name<<" [mixed workload] ---"<<std::endl;
    std::cout<<"operations: "<<operations<<std::endl;
    std::cout<<"average time [microseconds]: "<<(total_time/runs)<<std::endl;
    write_csv(name,"mixed_workload",operations,total_time/runs);
}
