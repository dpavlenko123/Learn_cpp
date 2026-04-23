#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <cstdint>
#include <functional>
#include "exceptions.hpp"
enum Op : uint8_t {
    PUSH = 0x01, STORE = 0x02, LOAD = 0x03, LT = 0x04, 
    GT = 0x05, LE = 0x06, GE = 0x07, EQ = 0x08, NE = 0x09,
    ADD = 0xA0, SUB = 0xA1, DIV = 0xA2, MUL = 0xA3, OUT = 0xA4,
    IN = 0xA5, JMP = 0xA6, JE = 0xA7, HALT = 0xFF
};

std::unordered_map<std::string, uint16_t> OpCode = { {"PUSH", 0x01}, {"STORE", 0x02}, {"LOAD", 0x03}, {"LT", 0x04},
{"GT", 0x05}, {"LE", 0x06}, {"GE", 0x07}, {"EQ", 0x08}, {"NE", 0x09}, {"ADD", 0xA0}, {"SUB", 0xA1}, {"DIV", 0xA2}, 
{"MUL", 0xA3}, {"OUT", 0xA4}, {"IN", 0xA5}, {"JMP", 0xA6}, {"JE", 0xA7}, {"HALT", 0xFF}};

class StackMachine {
    std::ostream& out;
    std::vector<uint8_t> code;       
    std::unordered_map<uint16_t, int16_t> data;       
    std::stack<int16_t> stack;
    uint16_t ip = 0;     
    
    uint16_t read() {
        uint16_t val = code[ip] | (code[ip + 1] << 8);
        ip += 2;
        return val;
    }

    void binaryOp(std::function<int16_t(int16_t, int16_t)> op) {
        int16_t b = stack.top(); stack.pop();
        int16_t a = stack.top(); stack.pop();
        stack.push(op(a, b));
    }             
    
    std::unordered_map<uint8_t, std::function<void()>> factory = {
        {Op::ADD, [this]() { binaryOp(std::plus<int16_t>()); }},
        {Op::SUB, [this]() { binaryOp(std::minus<int16_t>()); }},
        {Op::MUL, [this]() { binaryOp(std::multiplies<int16_t>()); }},
        {Op::DIV, [this]() { binaryOp(std::divides<int16_t>()); }},
        {Op::EQ,  [this]() { binaryOp(std::equal_to<int16_t>()); }},
        {Op::NE,  [this]() { binaryOp(std::not_equal_to<int16_t>()); }},
        {Op::LT,  [this]() { binaryOp(std::less<int16_t>()); }},
        {Op::GT,  [this]() { binaryOp(std::greater<int16_t>()); }},
        {Op::LE,  [this]() { binaryOp(std::less_equal<int16_t>()); }},
        {Op::GE,  [this]() { binaryOp(std::greater_equal<int16_t>()); }},
        {Op::PUSH, [this]() { uint16_t val = read(); 
                            stack.push(val); }},
        {Op::STORE, [this]() { int16_t addr = stack.top(); stack.pop();
                                int16_t val = stack.top(); stack.pop();
                                data[addr] = val; }},
        {Op::JMP, [this]() { ip = read(); }},
        {Op::JE, [this]() { uint16_t t = read(); uint16_t f = read();
                            ip = (stack.top() != 0) ? t : f; stack.pop();}},
        {Op::LOAD, [this]() { uint16_t addr = stack.top(); stack.pop();
                            stack.push(data.at(addr));}},
        {Op::OUT, [this]() { out << stack.top() << std::endl; stack.pop(); }}
    };
public:
    StackMachine(std::ostream& os = std::cout) : out(os) {}

    void load(const std::string& source) {
        std::stringstream ss(source);
        std::string line;
        std::vector<std::string> instructions;
        std::unordered_map<std::string, uint16_t> bbs;
        uint16_t current_ip = 0;
        
        while (std::getline(ss, line)) {
            
            line.erase(0, line.find_first_not_of(" \t\r"));
            if (line.empty()) continue;
            if (line.back() == ':') {
                bbs[line.substr(0, line.size() - 1)] = current_ip;
            } else {
                std::stringstream ls(line);
                std::string op; ls >> op;
                
                if (op == "PUSH") current_ip += 3;       
                else if (op == "JMP") current_ip += 3;   
                else if (op == "JE") current_ip += 5;    
                else current_ip += 1;                       
                instructions.push_back(line);
            }
        }
        for (const auto& instr : instructions) {
            std::stringstream ls(instr);
            std::string op;
            ls >> op;
            code.push_back(OpCode[op]);
            if (op == "PUSH") {
                int16_t val; 
                ls >> val;
                code.push_back(val & 0xFF);         
                code.push_back((val >> 8) & 0xFF);  
            } 
            else if (op == "JMP") {
                std::string bb; ls >> bb;
                uint16_t addr = bbs[bb];
                code.push_back(addr & 0xFF);
                code.push_back((addr >> 8) & 0xFF);
            }
            else if (op == "JE") {
                std::string bb1, bb2;
                ls >> bb1 >> bb2; 
                
                uint16_t addr1 = bbs[bb1];
                uint16_t addr2 = bbs[bb2];
                
                code.push_back(addr1 & 0xFF);
                code.push_back((addr1 >> 8) & 0xFF);
                
                code.push_back(addr2 & 0xFF);
                code.push_back((addr2 >> 8) & 0xFF);
            }
        }
    }
    void run() {
        while (ip < code.size()) {
            uint8_t op = code[ip++];
            if (op==Op::HALT) break;
            if (factory.find(op) != factory.end()) {
                factory[op]();
            }
            else CompilerError("Unexpected operation");
        }
    }
};