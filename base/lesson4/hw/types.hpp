#pragma once
#include <iostream>
#include <string>
enum class Type {
    NUMBER, IDENTIFIER,
    WHILE, IF, IN, PRINT, ASSIGN,
    LPAREN, RPAREN, LBRACKET, RBRACKET, SEMI,
    PLUS, MINUS, DIVIDE, MULTIPLY,
    MORE, LESS, MOREEQ, LESSEQ, EQ, NEQ,
    BLOCK,
    EOFTOKEN
};

class Token {
protected:
    Type type;
public:
    Token(Type t) : type(t) {}
    virtual std::string typeToString() const = 0;
    virtual bool hasValue() const { return false; }
    Type getType() const { return type; }
    virtual const std::string& getValue() const {
        throw std::runtime_error("Token has no value");
    }
    virtual ~Token() = default;
};

class TokenVal : public Token {
    std::string value;
public:
    TokenVal(Type t, const std::string& val)
        : Token(t), value(val) {}
    bool hasValue() const override { return true; }
    std::string typeToString() const override { 
        switch(type) {
            case Type::IDENTIFIER: return "IDENTIFIER";
            case Type::NUMBER: return "NUMBER";
            default: return "UNKNOWN";
        }
    }
    const std::string& getValue() const override {
        return value;
    }
};

class TokenNoVal : public Token {
public:
    TokenNoVal(Type type)
        : Token(type) {}

    std::string typeToString() const override {
        switch (type) {
            case Type::WHILE: return "WHILE";
            case Type::ASSIGN: return "ASSIGN";
            case Type::DIVIDE: return "DIVIDE";
            case Type::EQ: return "EQ";
            case Type::IF: return "IF";
            case Type::IN: return "IN";
            case Type::LBRACKET: return "LBRACKET";
            case Type::LESS: return "LESS";
            case Type::LESSEQ: return "LESSEQ";
            case Type::LPAREN: return "LPAREN";
            case Type::MINUS: return "MINUS";
            case Type::MORE: return "MORE";
            case Type::MOREEQ: return "MOREEQ";
            case Type::MULTIPLY: return "MULTIPLY";
            case Type::NEQ: return "NEQ";
            case Type::PLUS: return "PLUS";
            case Type::PRINT: return "PRINT";
            case Type::RBRACKET: return "RBRACKET";
            case Type::RPAREN: return "RPAREN";
            case Type::SEMI: return "SEMI";
            default: return "UNKNOWN";
        }
    }
};