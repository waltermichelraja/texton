#include "buffer/rope.hpp"
#include <stdexcept>

RopeNode::RopeNode(const std::string&text){
    data=text;
    weight=text.size();
}

Rope::Rope(){
    root=nullptr;
}

size_t Rope::node_size(const std::shared_ptr<RopeNode>&node)const{
    if(!node){return 0;}
    return node_size(node->left)+node->data.size()+node_size(node->right);
}

size_t Rope::size()const{
    return node_size(root);
}

std::string Rope::build_string(const std::shared_ptr<RopeNode>&node)const{
    if(!node){return "";}
    return build_string(node->left)+node->data+build_string(node->right);
}

void Rope::insert(size_t pos,const std::string&text){
    std::string current=build_string(root);
    if(pos>current.size()){
        throw std::out_of_range("insert out of range");
    }
    current.insert(pos,text);
    root=std::make_shared<RopeNode>(current);
}

void Rope::erase(size_t pos,size_t len){
    std::string current=build_string(root);
    if(pos+len>current.size()){
        throw std::out_of_range("erase out of range");
    }
    current.erase(pos,len);
    root=std::make_shared<RopeNode>(current);
}

std::string Rope::substr(size_t pos,size_t len)const{
    std::string current=build_string(root);
    if(pos+len>current.size()){
        throw std::out_of_range("substr out of range");
    }
    return current.substr(pos,len);
}