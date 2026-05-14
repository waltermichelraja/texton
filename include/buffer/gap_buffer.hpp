#pragma once
#include "text_buffer.hpp"
#include "core/history_manager.hpp"
#include <vector>

class GapBuffer:public TextBuffer{
    private:
    std::vector<char>buffer;
    HistoryManager history;
    bool suppress_history=false;
    size_t gap_start;
    size_t gap_end;

    void move_gap(size_t pos);
    void expand_gap(size_t min_size);

    public:
    GapBuffer(size_t initial_capacity=1024);
    void insert(size_t pos,const std::string&text)override;
    void erase(size_t pos,size_t len)override;
    void undo()override;
    void redo()override;
    std::string substr(size_t pos,size_t len)const override;
    size_t size()const override;
    size_t memory_usage()const override;
    size_t fragment_count()const override;
};