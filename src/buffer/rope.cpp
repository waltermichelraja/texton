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
    if(pos>size()){
        throw std::out_of_range("insert out of range");
    }
    auto new_node=std::make_shared<RopeNode>(text);
    std::shared_ptr<RopeNode>left;
    std::shared_ptr<RopeNode>right;
    split(root,pos,left,right);
    root=concatenate(concatenate(left,new_node),right);
}

void Rope::erase(size_t pos,size_t len){
    if(pos+len>size()){
        throw std::out_of_range("erase out of range");
    }
    std::shared_ptr<RopeNode>left;
    std::shared_ptr<RopeNode>middle;
    std::shared_ptr<RopeNode>right;
    split(root,pos,left,middle);
    split(middle,len,middle,right);
    root=concatenate(left,right);
}

std::string Rope::substr(size_t pos,size_t len)const{
    std::string current=build_string(root);
    if(pos+len>current.size()){
        throw std::out_of_range("substr out of range");
    }
    return current.substr(pos,len);
}

std::shared_ptr<RopeNode>Rope::concatenate(const std::shared_ptr<RopeNode>&left,const std::shared_ptr<RopeNode>&right){
    if(!left){return right;}
    if(!right){return left;}
    auto parent=std::make_shared<RopeNode>("");
    parent->left=left;
    parent->right=right;
    parent->weight=node_size(left);
    return parent;
}

void Rope::split(const std::shared_ptr<RopeNode>&node,size_t pos,std::shared_ptr<RopeNode>&left,std::shared_ptr<RopeNode>&right){
    if(!node){
        left=nullptr;
        right=nullptr;
        return;
    }
    if(!node->left && !node->right){
        std::string left_text=node->data.substr(0,pos);
        std::string right_text=node->data.substr(pos);
        left=left_text.empty()?nullptr:std::make_shared<RopeNode>(left_text);
        right=right_text.empty()?nullptr:std::make_shared<RopeNode>(right_text);
        return;
    }
    if(pos<node->weight){
        std::shared_ptr<RopeNode>split_left;
        std::shared_ptr<RopeNode>split_right;
        split(node->left,pos,split_left,split_right);
        left=split_left;
        right=concatenate(split_right,node->right);
    }else{
        std::shared_ptr<RopeNode>split_left;
        std::shared_ptr<RopeNode>split_right;
        split(node->right,pos-node->weight,split_left,split_right);
        left=concatenate(node->left,split_left);
        right=split_right;
    }
}
