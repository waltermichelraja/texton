#include "buffer/gap_buffer.hpp"
#include <stdexcept>
#include <algorithm>

GapBuffer::GapBuffer(size_t initial_capacity){
    buffer.resize(initial_capacity,'\0');
    gap_start=0;
    gap_end=initial_capacity;
}

size_t GapBuffer::size()const{
    return buffer.size()-(gap_end-gap_start);
}

void GapBuffer::move_gap(size_t pos){
    if(pos>size()){throw std::out_of_range("position out of range");}
    if(pos<gap_start){
        size_t shift=gap_start-pos;
        for(size_t i=0;i<shift;i++){
        buffer[gap_end-1-i]=buffer[gap_start-1-i];
    }
    gap_start-=shift;
    gap_end-=shift;
    }else if(pos>gap_start){
        size_t shift=pos-gap_start;
        for(size_t i=0;i<shift;i++){
        buffer[gap_start+i]=buffer[gap_end+i];
    }
    gap_start+=shift;
    gap_end+=shift;
    }
}

void GapBuffer::expand_gap(size_t min_size){
    size_t current_gap=gap_end-gap_start;
    if(current_gap>=min_size){return;}
    size_t new_capacity=buffer.size()+std::max(min_size,current_gap)+1024;
    std::vector<char>new_buffer(new_capacity);
    size_t new_gap_end=new_capacity-(buffer.size()-gap_end);

    std::copy(buffer.begin(),buffer.begin()+gap_start,new_buffer.begin());

    size_t right_size=buffer.size()-gap_end;
    std::copy(buffer.begin()+gap_end,buffer.end(),new_buffer.begin()+new_gap_end);

    gap_end=new_gap_end;
    buffer=std::move(new_buffer);
}

void GapBuffer::insert(size_t pos,const std::string&text){
    move_gap(pos);
    expand_gap(text.size());
    for(char c:text){
        buffer[gap_start++]=c;
    }
}

void GapBuffer::erase(size_t pos,size_t len){
    if(pos+len>size()){throw std::out_of_range("erase out of range");}
    move_gap(pos);
    gap_end+=len;
}

std::string GapBuffer::substr(size_t pos,size_t len)const{
    if(pos+len>size()){throw std::out_of_range("substr out of range");}
    std::string result;
    result.reserve(len);
    for(size_t i=0;i<len;i++){
        size_t idx=pos+i;
        if(idx<gap_start)result.push_back(buffer[idx]);
        else result.push_back(buffer[idx+(gap_end-gap_start)]);
    }
    return result;
}