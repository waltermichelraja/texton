#include "buffer/rope.hpp"
#include <algorithm>
#include <stdexcept>

RopeNode::RopeNode(const std::string&text){
    data=text;
    weight=text.size();
}

Rope::Rope(){
    root=nullptr;
    total_size=0;
}

size_t Rope::node_size(const std::shared_ptr<RopeNode>&node)const{
    if(!node){return 0;}
    return node_size(node->left)+node->data.size()+node_size(node->right);
}

bool Rope::is_leaf(const std::shared_ptr<RopeNode>&node)const{
    return node && !node->left && !node->right;
}

size_t Rope::size()const{
    return total_size;
}

std::string Rope::build_string(const std::shared_ptr<RopeNode>&node)const{
    if(!node){return "";}
    return build_string(node->left)+node->data+build_string(node->right);
}

void Rope::insert(size_t pos,const std::string&text){
    if(pos>size()){
        throw std::out_of_range("insert out of range");
    }
    if(!suppress_history){
        history.record({OperationType::INSERT,pos,text});
    }
    auto new_node=std::make_shared<RopeNode>(text);
    std::shared_ptr<RopeNode>left;
    std::shared_ptr<RopeNode>right;
    split(root,pos,left,right);
    root=concatenate(concatenate(left,new_node),right);
    total_size+=text.size();
    if(total_size%1000==0){rebalance();}
}

void Rope::erase(size_t pos,size_t len){
    if(pos+len>size()){
        throw std::out_of_range("erase out of range");
    }
    std::string removed=substr(pos,len);
    if(!suppress_history){
        history.record({OperationType::ERASE,pos,removed});
    }
    std::shared_ptr<RopeNode>left;
    std::shared_ptr<RopeNode>temp;
    std::shared_ptr<RopeNode>discard;
    std::shared_ptr<RopeNode>right;
    split(root,pos,left,temp);
    split(temp,len,discard,right);
    root=concatenate(left,right);
    total_size-=len;
    if(total_size>0 && total_size%1000==0){
        rebalance();
    }
}

std::string Rope::substr(size_t pos,size_t len)const{
    if(pos+len>size()){
        throw std::out_of_range("substr out of range");
    }
    std::string result;
    result.reserve(len);
    collect_substr(root,pos,len,result);
    return result;
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
    if(is_leaf(node)){
        size_t split_pos=std::min(pos,node->data.size());
        std::string left_text=node->data.substr(0,split_pos);
        std::string right_text=node->data.substr(split_pos);
        left=left_text.empty()?nullptr:std::make_shared<RopeNode>(left_text);
        right=right_text.empty()?nullptr:std::make_shared<RopeNode>(right_text);
        return;
    }
    if(pos<node->weight){
        std::shared_ptr<RopeNode>l1;
        std::shared_ptr<RopeNode>l2;
        split(node->left,pos,l1,l2);
        left=l1;
        right=concatenate(l2,node->right);
    }else{
        std::shared_ptr<RopeNode>r1;
        std::shared_ptr<RopeNode>r2;
        split(node->right,pos-node->weight,r1,r2);
        left=concatenate(node->left,r1);
        right=r2;
    }
}

void Rope::collect_substr(const std::shared_ptr<RopeNode>&node,size_t pos,size_t len,std::string&result)const{
    if(!node || len==0){return;}
    if(!node->left && !node->right){
        if(pos>=node->data.size()){return;}
        size_t take=std::min(len,node->data.size()-pos);
        result.append(node->data.substr(pos,take));
        return;
    }
    if(pos<node->weight){
        size_t left_take=std::min(len,node->weight-pos);
        collect_substr(node->left,pos,left_take,result);
        collect_substr(node->right,0,len-left_take,result);

    }else{
        collect_substr(node->right,pos-node->weight,len,result);
    }
}

void Rope::collect_leaves(const std::shared_ptr<RopeNode>&node,std::vector<std::string>&leaves)const{
    if(!node){return;}
    if(!node->left && !node->right){
        if(!node->data.empty()){
            leaves.push_back(node->data);
        }
        return;
    }
    collect_leaves(node->left,leaves);
    collect_leaves(node->right,leaves);
}

std::shared_ptr<RopeNode>Rope::build_balanced(const std::vector<std::string>&leaves,size_t left,size_t right){
    if(left>=right){return nullptr;}
    if(right-left==1){
        return std::make_shared<RopeNode>(leaves[left]);
    }
    size_t mid=(left+right)/2;
    auto left_node=build_balanced(leaves,left,mid);
    auto right_node=build_balanced(leaves,mid,right);
    return concatenate(left_node,right_node);
}

void Rope::rebalance(){
    std::vector<std::string>leaves;
    collect_leaves(root,leaves);
    root=build_balanced(leaves,0,leaves.size());
}

void Rope::undo(){
    if(!history.can_undo()){return;}
    EditOperation op=history.pop_undo();
    suppress_history=true;
    if(op.type==OperationType::INSERT){
        erase(op.position,op.text.size());
    }else{
        insert(op.position,op.text);
    }
    suppress_history=false;
    history.push_redo(op);
}

void Rope::redo(){
    if(!history.can_redo()){return;}
    EditOperation op=history.pop_redo();
    suppress_history=true;
    if(op.type==OperationType::INSERT){
        insert(op.position,op.text);
    }else{
        erase(op.position,op.text.size());
    }
    suppress_history=false;
    history.push_undo(op);
}

size_t count_nodes(const std::shared_ptr<RopeNode>&node){
    if(!node){return 0;}
    return 1+count_nodes(node->left)+count_nodes(node->right);
}

size_t Rope::memory_usage()const{
    return count_nodes(root)*sizeof(RopeNode);
}

size_t Rope::fragment_count()const{
    return count_nodes(root);
}