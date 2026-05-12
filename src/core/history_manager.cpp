#include "core/history_manager.hpp"
#include <stdexcept>

void HistoryManager::record(const EditOperation&op){
    undo_stack.push(op);
    while(!redo_stack.empty()){
        redo_stack.pop();
    }
}

bool HistoryManager::can_undo()const{
    return !undo_stack.empty();
}

bool HistoryManager::can_redo()const{
    return !redo_stack.empty();
}

EditOperation HistoryManager::pop_undo(){
    if(undo_stack.empty()){
        throw std::runtime_error("nothing to undo");
    }
    EditOperation op=undo_stack.top();
    undo_stack.pop();
    return op;
}

EditOperation HistoryManager::pop_redo(){
    if(redo_stack.empty()){
        throw std::runtime_error("nothing to redo");
    }
    EditOperation op=redo_stack.top();
    redo_stack.pop();
    return op;
}

void HistoryManager::push_redo(const EditOperation&op){
    redo_stack.push(op);
}

void HistoryManager::clear_redo(){
    while(!redo_stack.empty()){
        redo_stack.pop();
    }
}