#pragma once
#include <string>

enum class OperationType{
    INSERT,
    ERASE
};

struct EditOperation{
    OperationType type;
    size_t position;
    std::string text;
};