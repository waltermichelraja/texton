#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "buffer/gap_buffer.hpp"
#include "buffer/piece_table.hpp"
#include "buffer/rope.hpp"

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
    std::mt19937 rng(42);
    const size_t operations=10000;
    for(size_t i=0;i<operations;i++){
        bool do_insert=reference.empty() || (rng()%100<70);
        if(do_insert){
            size_t pos=reference.empty()?0:rng()%(reference.size()+1);
            char c='a'+(rng()%26);
            std::string text(1,c);
            gap.insert(pos,text);
            piece.insert(pos,text);
            rope.insert(pos,text);
            reference.insert(pos,text);
        }else{
            size_t pos=rng()%reference.size();
            gap.erase(pos,1);
            piece.erase(pos,1);
            rope.erase(pos,1);
            reference.erase(pos,1);
        }
        validate_equal(gap,reference,"GapBuffer");
        validate_equal(piece,reference,"PieceTable");
        validate_equal(rope,reference,"Rope");
    }
    std::cout<<"validation passed"<<std::endl;
    return 0;
}