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
    size_t line;
public:
    Token(Type t, size_t l) : type(t), line(l) {}
    virtual bool hasValue() const { return false; }
    Type getType() const { return type; }
    size_t getLine() const { return line; }
    virtual const std::string& getValue() const {
        throw std::runtime_error("Token has no value");
    }
    virtual ~Token() = default;
};

class TokenVal : public Token {
    std::string value;
public:
    TokenVal(Type t, size_t l, const std::string& val)
        : Token(t, l), value(val) {}
    bool hasValue() const override { return true; }
    const std::string& getValue() const override {
        return value;
    }
};

class TokenNoVal : public Token {
public:
    TokenNoVal(Type type, size_t l)
        : Token(type, l) {}
};

using TokenIter = std::vector<std::unique_ptr<Token>>::const_iterator;
using TokenVector = std::vector<std::unique_ptr<Token>>;
using FactoryMethod = std::function<std::unique_ptr<Token>()>;