#pragma once
#include "text_buffer.hpp"
#include "core/history_manager.hpp"
#include <vector>
#include <string>

enum class BufferType{
    ORIGINAL,
    ADD
};

struct Piece{
    BufferType buffer;
    size_t start;
    size_t length;
};

class PieceTable:public TextBuffer{
private:
    std::string original_buffer;
    std::string add_buffer;
    std::vector<Piece>pieces;
    HistoryManager history;
    bool suppress_history=false;
    size_t find_piece(size_t pos,size_t&offset)const;

public:
    PieceTable(const std::string&initial="");
    void insert(size_t pos,const std::string&text)override;
    void erase(size_t pos,size_t len)override;
    void undo()override;
    void redo()override;
    std::string substr(size_t pos,size_t len)const override;
    size_t size()const override;
    void merge_adjacent();
    size_t memory_usage()const override;
    size_t fragment_count()const override;
};