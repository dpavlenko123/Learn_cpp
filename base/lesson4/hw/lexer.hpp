#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <vector>
#include <cctype>
#include <fstream>
#include "types.hpp"

using FactoryMethod = std::function<std::unique_ptr<Token>()>;

class Lexer {
private: 
    std::vector<std::unique_ptr<Token>> tokens;
    std::string::const_iterator current;
    std::string::const_iterator eof;

    std::unordered_map<std::string, FactoryMethod> factory = {
        {"while", []() { return std::make_unique<TokenNoVal>(Type::WHILE); } },
        {"if", []() { return std::make_unique<TokenNoVal>(Type::IF); } },
        {"in", []() { return std::make_unique<TokenNoVal>(Type::IN); } },
        {"print", []() { return std::make_unique<TokenNoVal>(Type::PRINT); } },
        {"=", []() { return std::make_unique<TokenNoVal>(Type::ASSIGN); } },
        {"+", []() { return std::make_unique<TokenNoVal>(Type::PLUS); } },
        {"-", []() { return std::make_unique<TokenNoVal>(Type::MINUS); } },
        {"/", []() { return std::make_unique<TokenNoVal>(Type::DIVIDE); } },
        {"*", []() { return std::make_unique<TokenNoVal>(Type::MULTIPLY); } },
        {"<", []() { return std::make_unique<TokenNoVal>(Type::LESS); } },
        {">", []() { return std::make_unique<TokenNoVal>(Type::MORE); } },
        {"<=", []() { return std::make_unique<TokenNoVal>(Type::LESSEQ); } },
        {">=", []() { return std::make_unique<TokenNoVal>(Type::MOREEQ); } },
        {"==", []() { return std::make_unique<TokenNoVal>(Type::EQ); } },
        {"!=", []() { return std::make_unique<TokenNoVal>(Type::NEQ); } },
        {"(", []() { return std::make_unique<TokenNoVal>(Type::LPAREN); } },
        {")", []() { return std::make_unique<TokenNoVal>(Type::RPAREN); } },
        {"{", []() { return std::make_unique<TokenNoVal>(Type::LBRACKET); } },
        {"}", []() { return std::make_unique<TokenNoVal>(Type::RBRACKET); } },
        {";", []() { return std::make_unique<TokenNoVal>(Type::SEMI); } } 
    };

public:
    Lexer(std::string::const_iterator cur, std::string::const_iterator e) : current(cur), eof(e) {} 

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
            else ++current;
        }
        tokens.push_back(std::make_unique<TokenNoVal>(Type::EOFTOKEN));
    }


    std::vector<std::unique_ptr<Token>>::const_iterator begin() const {
        return tokens.cbegin();
    }

    std::vector<std::unique_ptr<Token>>::const_iterator end() const {
        return tokens.cend();
    }

    std::vector<std::unique_ptr<Token>>& getTokens() { return tokens; }
    const std::vector<std::unique_ptr<Token>>& getTokens() const { return tokens; }

private:
    void handleStr() {
        std::string res;
        while ((isalpha(*current) || *current=='_')&& current!=eof) {
            res += *current;
            ++current;
        }
        auto it = factory.find(res);
        if (it != factory.end()) tokens.push_back(it->second()); 
        else tokens.push_back(std::make_unique<TokenVal>(Type::IDENTIFIER, res));
    }

    void handleNum() {
        std::string res;
        while (isdigit(*current) && current!=eof) {
            res += *current;
            ++current;
        }
        tokens.push_back(std::make_unique<TokenVal>(Type::NUMBER, res));
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
    }

    const std::unordered_set<char> symbols = {'+', '-', '*', '/', '>', '<', 
        '=', '<', '>', '!', '(', ')', '{','}', ';' };
};

