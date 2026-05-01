#pragma once
#include "text_buffer.hpp"
#include <vector>

class GapBuffer:public TextBuffer{
    private:
    std::vector<char>buffer;
    size_t gap_start;
    size_t gap_end;

    void move_gap(size_t pos);
    void expand_gap(size_t min_size);

    public:
    GapBuffer(size_t initial_capacity=1024);

    void insert(size_t pos,const std::string&text)override;
    void erase(size_t pos,size_t len)override;
    std::string substr(size_t pos,size_t len)const override;
    size_t size()const override;
};