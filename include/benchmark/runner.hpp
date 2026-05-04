#pragma once
#include <memory>
#include <string>
#include <functional>
#include "buffer/text_buffer.hpp"

class BenchmarkRunner{
public:
    static void run_insert_test(const std::string&name,std::function<std::unique_ptr<TextBuffer>()>factory,size_t operations);
};