#include "buffer/piece_table.hpp"
#include <stdexcept>

PieceTable::PieceTable(const std::string&initial){
    original_buffer=initial;
    if(!initial.empty()){
        pieces.push_back({BufferType::ORIGINAL,0,initial.size()});
    }
}

size_t PieceTable::size()const{
    size_t total=0;
    for(const auto&p:pieces){
        total+=p.length;
    }
    return total;
}

size_t PieceTable::find_piece(size_t pos,size_t&offset)const{
    size_t current=0;
    for(size_t i=0;i<pieces.size();i++){
        if(pos<current+pieces[i].length){
            offset=pos-current;
            return i;
        }
        current+=pieces[i].length;
    }
    if(pos==current && !pieces.empty()){
        offset=pieces.back().length;
        return pieces.size()-1;
    }
    throw std::out_of_range("position out of range");
}

void PieceTable::insert(size_t pos,const std::string&text){
    if(pos>size()){throw std::out_of_range("insert out of range");}
    size_t add_start=add_buffer.size();
    add_buffer+=text;
    if(pieces.empty()){
        pieces.push_back({BufferType::ADD,add_start,text.size()});
        return;
    }
    size_t offset=0;
    size_t index=find_piece(pos,offset);
    Piece current=pieces[index];
    std::vector<Piece>new_pieces;
    if(offset>0){
        new_pieces.push_back({current.buffer,current.start,offset});
    }
    new_pieces.push_back({BufferType::ADD,add_start,text.size()});
    if(offset<current.length){
        new_pieces.push_back({
            current.buffer,
            current.start+offset,
            current.length-offset
        });
    }
    pieces.erase(pieces.begin()+index);
    pieces.insert(pieces.begin()+index,new_pieces.begin(),new_pieces.end());
}

std::string PieceTable::substr(size_t pos,size_t len)const{
    if(pos+len>size()){throw std::out_of_range("substr out of range");}
    std::string result;
    result.reserve(len);
    size_t remaining=len;
    size_t current_pos=pos;
    while(remaining>0){
        size_t offset=0;
        size_t index=find_piece(current_pos,offset);
        const Piece&p=pieces[index];
        size_t take=std::min(remaining,p.length-offset);
        const std::string&buffer=(p.buffer==BufferType::ORIGINAL)?original_buffer:add_buffer;
        result.append(buffer.substr(p.start+offset,take));
        current_pos+=take;
        remaining-=take;
    }

    return result;
}

void PieceTable::erase(size_t pos,size_t len){
    if(pos+len>size()){throw std::out_of_range("erase out of range");}
    size_t remaining=len;
    while(remaining>0){
        size_t offset=0;
        size_t index=find_piece(pos,offset);
        Piece current=pieces[index];
        size_t erase_len=std::min(remaining,current.length-offset);
        std::vector<Piece>new_pieces;
        if(offset>0){
            new_pieces.push_back({
                current.buffer,
                current.start,
                offset
            });
        }
        if(offset+erase_len<current.length){
            new_pieces.push_back({
                current.buffer,
                current.start+offset+erase_len,
                current.length-(offset+erase_len)
            });
        }
        pieces.erase(pieces.begin()+index);
        pieces.insert(pieces.begin()+index,new_pieces.begin(),new_pieces.end());
        remaining-=erase_len;
    }
}