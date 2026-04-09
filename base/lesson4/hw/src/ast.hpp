#pragma once
#include "types.hpp"
#include <memory>
#include <variant>
#include <vector>
#include <string>

class ASTVisitor;

class Node;

using node_ptr = std::unique_ptr<Node>;
class IdntyNode {
    std::string value;
public:
    IdntyNode(std::string val) : value(std::move(val)) {}
    const std::string& getValue() const { return value; }
    void accept(ASTVisitor* v);
};

class NumberNode {
    double value;
public:
    NumberNode(double val) : value(val) {}
    double getValue() const { return value; }
    void accept(ASTVisitor* v);
};

class BinaryNode {
    node_ptr left, right;
    std::string op;
public:
    BinaryNode(node_ptr l, node_ptr r, std::string op) 
        : left(std::move(l)), right(std::move(r)), op(std::move(op)) {}
    const node_ptr& getLeft() const { return left; }
    const node_ptr& getRight() const { return right; }
    const std::string& getOperation() const { return op; }
    void accept(ASTVisitor* v);
};

class PrintNode {
    node_ptr expr;
public:
    PrintNode(node_ptr e) : expr(std::move(e)) {}
    const node_ptr& getExpr() const { return expr; }
    void accept(ASTVisitor* v);
};

class InNode {
    node_ptr expr;
public:
    InNode(node_ptr e) : expr(std::move(e)) {}
    const node_ptr& getExpr() const { return expr; }
    void accept(ASTVisitor* v);
};

class WhileNode {
    node_ptr condition;
    node_ptr body;
public:
    WhileNode(node_ptr c, node_ptr b) 
        : condition(std::move(c)), body(std::move(b)) {}
    const node_ptr& getBody() const { return body; }
    const node_ptr& getCondition() const { return condition; }
    void accept(ASTVisitor* v);
};

class IfNode {
    node_ptr condition;
    node_ptr ifBody;
    node_ptr elseBody;
public:
    IfNode(node_ptr c, node_ptr i, node_ptr e) 
        : condition(std::move(c)), ifBody(std::move(i)), elseBody(std::move(e)) {}
    const node_ptr& getIfBody() const { return ifBody; }
    const node_ptr& getElseBody() const { return elseBody; }
    const node_ptr& getCondition() const { return condition; }
    void accept(ASTVisitor* v);
};

class Block {
    std::vector<node_ptr> body;
public:
    Block(std::vector<node_ptr> b) : body(std::move(b)) {}
    const std::vector<node_ptr>& getBody() const { return body; }
    void accept(ASTVisitor* v);
};

class Node : public std::variant<BinaryNode, NumberNode, IdntyNode, PrintNode, InNode, WhileNode, IfNode, Block> {
    
};