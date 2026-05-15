#pragma once
#include <memory>
#include <string>
#include <functional>
#include "buffer/text_buffer.hpp"

class BenchmarkRunner{
public:
    static void run_insert_test(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations);
    static void run_random_insert_test(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations);
    static void run_mixed_workload(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations);
    static void write_csv(const std::string&structure,const std::string&workload,size_t operations,long long time,size_t memory,size_t fragments);
};