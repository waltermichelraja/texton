#pragma once
#include <string>

class TextBuffer{
    public:
    virtual void insert(size_t pos,const std::string&text)=0;
    virtual void erase(size_t pos,size_t len)=0;
    virtual std::string substr(size_t pos,size_t len)const=0;
    virtual size_t size()const=0;
    virtual ~TextBuffer()=default;
};