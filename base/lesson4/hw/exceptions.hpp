#pragma once
#include <stdexcept>
#include <iostream>
#include <string>
class СompilerError : public std::runtime_error {  
public:
    СompilerError(const std::string& msg): std::runtime_error(msg) {}
};

