#include <iostream>
#include <random>
#include <stack>
#include <string>

#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"
#include "buffer/rope.hpp"

enum class RefOperationType{
    INSERT,
    ERASE
};

struct RefOperation{
    RefOperationType type;
    size_t position;
    std::string text;
};

void validate_equal(TextBuffer&buffer,const std::string&reference,const std::string&name){
    std::string actual=buffer.substr(0,buffer.size());
    if(actual!=reference){
        std::cout<<"mismatch detected in "<<name<<std::endl;
        std::cout<<"expected: "<<reference<<std::endl;
        std::cout<<"actual: "<<actual<<std::endl;
        std::exit(1);
    }
}

int main(){
    GapBuffer gap;
    PieceTable piece;
    Rope rope;
    std::string reference;
    std::stack<RefOperation>undo_stack;
    std::stack<RefOperation>redo_stack;
    std::mt19937 rng(42);
    const size_t operations=10000;
    for(size_t i=0;i<operations;i++){
        int action=rng()%100;
        if(action<50){
            size_t pos=reference.empty()?0:rng()%(reference.size()+1);
            char c='a'+(rng()%26);
            std::string text(1,c);
            gap.insert(pos,text);
            piece.insert(pos,text);
            rope.insert(pos,text);
            reference.insert(pos,text);
            undo_stack.push({RefOperationType::INSERT,pos,text});
            while(!redo_stack.empty()){
                redo_stack.pop();
            }
        }else if(action<75){
            if(reference.empty()){continue;}
            size_t pos=rng()%reference.size();
            std::string removed=reference.substr(pos,1);
            gap.erase(pos,1);
            piece.erase(pos,1);
            rope.erase(pos,1);
            reference.erase(pos,1);
            undo_stack.push({RefOperationType::ERASE,pos,removed});
            while(!redo_stack.empty()){redo_stack.pop();}
        }else if(action<90){
            if(undo_stack.empty()){continue;}
            RefOperation op=undo_stack.top();
            undo_stack.pop();
            gap.undo();
            piece.undo();
            rope.undo();
            if(op.type==RefOperationType::INSERT){
                reference.erase(op.position,op.text.size());
            }else{
                reference.insert(op.position,op.text);
            }
            redo_stack.push(op);
        }else{
            if(redo_stack.empty()){continue;}
            RefOperation op=redo_stack.top();
            redo_stack.pop();
            gap.redo();
            piece.redo();
            rope.redo();
            if(op.type==RefOperationType::INSERT){
                reference.insert(op.position,op.text);
            }else{
                reference.erase(op.position,op.text.size());
            }
            undo_stack.push(op);
        }

        validate_equal(gap,reference,"GapBuffer");
        validate_equal(piece,reference,"PieceTable");
        validate_equal(rope,reference,"Rope");
    }
    std::cout<<"undo/redo validation passed"<<std::endl;
    return 0;
}