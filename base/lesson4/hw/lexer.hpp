#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <vector>
#include <cctype>
#include <fstream>
#include "types.hpp"
#include "exceptions.hpp"

class Lexer {
private: 
    TokenVector tokens;
    std::string::const_iterator current;
    std::string::const_iterator eof;
    size_t line = 1;

    std::unordered_map<std::string, FactoryMethod> factory = {
        {"while", [this]() { return std::make_unique<TokenNoVal>(Type::WHILE, line); } },
        {"if", [this]() { return std::make_unique<TokenNoVal>(Type::IF, line); } },
        {"in", [this]() { return std::make_unique<TokenNoVal>(Type::IN, line); } },
        {"print", [this]() { return std::make_unique<TokenNoVal>(Type::PRINT, line); } },
        {"=", [this]() { return std::make_unique<TokenNoVal>(Type::ASSIGN, line); } },
        {"+", [this]() { return std::make_unique<TokenNoVal>(Type::PLUS, line); } },
        {"-", [this]() { return std::make_unique<TokenNoVal>(Type::MINUS, line); } },
        {"/", [this]() { return std::make_unique<TokenNoVal>(Type::DIVIDE, line); } },
        {"*", [this]() { return std::make_unique<TokenNoVal>(Type::MULTIPLY, line); } },
        {"<", [this]() { return std::make_unique<TokenNoVal>(Type::LESS, line); } },
        {">", [this]() { return std::make_unique<TokenNoVal>(Type::MORE, line); } },
        {"<=", [this]() { return std::make_unique<TokenNoVal>(Type::LESSEQ, line); } },
        {">=", [this]() { return std::make_unique<TokenNoVal>(Type::MOREEQ, line); } },
        {"==", [this]() { return std::make_unique<TokenNoVal>(Type::EQ, line); } },
        {"!=", [this]() { return std::make_unique<TokenNoVal>(Type::NEQ, line); } },
        {"(", [this]() { return std::make_unique<TokenNoVal>(Type::LPAREN, line); } },
        {")", [this]() { return std::make_unique<TokenNoVal>(Type::RPAREN, line); } },
        {"{", [this]() { return std::make_unique<TokenNoVal>(Type::LBRACKET, line); } },
        {"}", [this]() { return std::make_unique<TokenNoVal>(Type::RBRACKET, line); } },
        {";", [this]() { return std::make_unique<TokenNoVal>(Type::SEMI, line); } } 
    };

public:
    Lexer(const std::string& str) : current(str.cbegin()), eof(str.cend()) {} 

    void tokenize() {
        while (current!=eof) {
            if (isalpha(*current) || *current=='_') {
                handleStr();
            }
            else if (isdigit(*current)) {
                handleNum();
            }
            else if (symbols.contains(*current)) {
                handleSymbol();
            }
            else if (*current=='\n') { 
                ++line;
                ++current;
            }
            else if (*current==' ' || *current == '\t') ++current;
            else throw СompilerError("Unexpected character: '" + std::string(1, *current) +"' on line " + std::to_string(line));
        }
        tokens.push_back(std::make_unique<TokenNoVal>(Type::EOFTOKEN, line));
    }


    TokenVectorConstIterator begin() const {
        return tokens.cbegin();
    }

    TokenVectorConstIterator end() const {
        return tokens.cend();
    }

    TokenVector& getTokens() { return tokens; }
    const TokenVector& getTokens() const { return tokens; }

private:
    void handleStr() {
        std::string res;
        while ((isalpha(*current) || *current=='_')&& current!=eof) {
            res += *current;
            ++current;
        }
        auto it = factory.find(res);
        if (it != factory.end()) tokens.push_back(it->second()); 
        else tokens.push_back(std::make_unique<TokenVal>(Type::IDENTIFIER, line, res));
    }

    void handleNum() {
        std::string res;
        while (isdigit(*current) && current!=eof) {
            res += *current;
            ++current;
        }
        tokens.push_back(std::make_unique<TokenVal>(Type::NUMBER, line, res));
    }

    void handleSymbol() {
        std::string res(1, *current);
        ++current;
        if (current != eof && (
            (res == "=" && *current == '=') ||
            (res == "!" && *current == '=') ||
            (res == "<" && *current == '=') ||
            (res == ">" && *current == '=')
        )) {
            res += *current;
            ++current;
        }
        auto it = factory.find(res);
        if (it != factory.end()) tokens.push_back(it->second()); 
        else throw СompilerError("Unexpected operator: '" + res + "' on line " + std::to_string(line));
    }

    const std::unordered_set<char> symbols = {'+', '-', '*', '/', '>', '<', 
        '=', '<', '>', '!', '(', ')', '{','}', ';' };
};

