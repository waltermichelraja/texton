#pragma once
#include <stack>
#include "core/edit_operation.hpp"

class HistoryManager{
private:
    std::stack<EditOperation>undo_stack;
    std::stack<EditOperation>redo_stack;

public:
    void push_undo(const EditOperation&op);
    void record(const EditOperation&op);
    bool can_undo()const;
    bool can_redo()const;
    EditOperation pop_undo();
    EditOperation pop_redo();
    void push_redo(const EditOperation&op);
    void clear_redo();
};