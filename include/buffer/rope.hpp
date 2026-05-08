#pragma once
#include "text_buffer.hpp"
#include <memory>
#include <string>

struct RopeNode{
    std::string data;
    std::shared_ptr<RopeNode>left;
    std::shared_ptr<RopeNode>right;
    size_t weight;

    RopeNode(const std::string&text);
};

class Rope:public TextBuffer{
private:
    std::shared_ptr<RopeNode>root;
    size_t node_size(const std::shared_ptr<RopeNode>&node)const;
    std::string build_string(const std::shared_ptr<RopeNode>&node)const;
    std::shared_ptr<RopeNode>concatenate(const std::shared_ptr<RopeNode>&left,const std::shared_ptr<RopeNode>&right);
    void split(const std::shared_ptr<RopeNode>&node,size_t pos,std::shared_ptr<RopeNode>&left,std::shared_ptr<RopeNode>&right);
    void collect_substr(const std::shared_ptr<RopeNode>&node,size_t pos,size_t len,std::string&result)const;

public:
    Rope();
    void insert(size_t pos,const std::string&text)override;
    void erase(size_t pos,size_t len)override;
    std::string substr(size_t pos,size_t len)const override;
    size_t size()const override;
};