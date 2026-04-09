#pragma once
#include <stdexcept>
#include <iostream>
#include <string>
class CompilerError : public std::runtime_error {  
public:
    CompilerError(const std::string& msg): std::runtime_error(msg) {}
};

