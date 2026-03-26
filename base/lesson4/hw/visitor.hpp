#pragma once
#include "ast.hpp"
#include <stack>
#include <unordered_map>
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void operator()(const NumberNode& node) = 0;
    virtual void operator()(const IdntyNode& node) = 0;
    virtual void operator()(const BinaryNode& node) = 0;
    virtual void operator()(const InNode& node) = 0;
    virtual void operator()(const PrintNode& node) = 0;
    virtual void operator()(const WhileNode& node) = 0;
    virtual void operator()(const IfNode& node) = 0;
    virtual void operator()(const Block& node) = 0;
};

class EvalVisitor : public ASTVisitor {
    std::unordered_map<std::string, double> identifiers;
    std::stack<double> evalStack;
public:
    void operator()(const NumberNode& node) override {
        evalStack.push(node.getValue());
    }
    void operator()(const IdntyNode& node) override {
        auto val = identifiers.at(node.getValue());
        evalStack.push(val);
    }
    void operator()(const BinaryNode& node) override {
        auto op = node.getOperation();
        if (op=="=") {
            node.getRight()->accept(this);
            auto val = evalStack.top(); evalStack.pop();
            const auto* id = std::get_if<IdntyNode>(node.getLeft().get());
            identifiers[id->getValue()] = val;
            return;
        }
        node.getLeft()->accept(this);
        node.getRight()->accept(this);

        double right = evalStack.top(); evalStack.pop();
        double left = evalStack.top(); evalStack.pop();

        if (op == "+") evalStack.push(left + right);
        else if (op == "-") evalStack.push(left - right);
        else if (op == "*") evalStack.push(left * right);
        else if (op == "/") evalStack.push(left / right);
        else if (op == "<") evalStack.push(left < right ? 1.0 : 0.0);
        else if (op == ">") evalStack.push(left > right ? 1.0 : 0.0);
        else if (op == "<=") evalStack.push(left <= right ? 1.0 : 0.0);
        else if (op == ">=") evalStack.push(left >= right ? 1.0 : 0.0);
        else if (op == "==") evalStack.push(left == right ? 1.0 : 0.0);
        else if (op == "!=") evalStack.push(left != right ? 1.0 : 0.0);
    }
    void operator()(const InNode& node) override {
        const auto* id = std::get_if<IdntyNode>(node.getExpr().get());

        double val;
        std::cin >> val;

        identifiers[id->getValue()] = val;
    }
    void operator()(const PrintNode& node) override {
        node.getExpr()->accept(this);
        double val = evalStack.top(); evalStack.pop();
        std::cout << val <<std::endl;
    }
    void operator()(const WhileNode& node) override {
        node.getCondition()->accept(this);
        bool condition = evalStack.top()!=0.0; evalStack.pop();
        while (condition) {
            node.getBody()->accept(this);
            node.getCondition()->accept(this);
            condition = evalStack.top()!=0.0; evalStack.pop();
        }
    }
    void operator()(const IfNode& node) override{
        node.getCondition()->accept(this);
        bool condition = evalStack.top()!=0.0; evalStack.pop();
        if (condition) {
            node.getBody()->accept(this);
        }
    }
    void operator()(const Block& node) override {
        for (const auto& stmt : node.getBody()) {
            stmt->accept(this);
        }
    }
};

class CompareVisitor : public ASTVisitor {
    bool equal = true;
    const Node* current;
public:
    CompareVisitor(const Node* curr) : current(curr) {}
    bool isEqual() const {
        return equal;
    }
    void operator()(const NumberNode& node) override {
        if (!equal) return;

        if (const auto* other = std::get_if<NumberNode>(current)) {
            equal = other->getValue()==node.getValue();
        }
        else equal = false;
    }
    void operator()(const IdntyNode& node) override {
        if (!equal) return;

        if (const auto* other = std::get_if<IdntyNode>(current)) {
            equal = other->getValue()==node.getValue();
        }
        else equal = false;
    }
    void operator()(const BinaryNode& node) override {
        if (!equal) return;

        if (const auto* other = std::get_if<BinaryNode>(current)) {
            if (other->getOperation()!=node.getOperation()) {
                equal = false;
                return;
            }
            CompareVisitor left(other->getLeft().get());
            node.getLeft()->accept(&left);
            if (!left.isEqual()) { equal = false; return; }
            
            CompareVisitor right(other->getRight().get());
            node.getRight()->accept(&right);
            equal = right.isEqual();
        } 
        else equal = false;
        
    }
    void operator()(const InNode& node) override {
        if (!equal) return;

        if (const auto* other = std::get_if<InNode>(current)) {
            CompareVisitor expr(other->getExpr().get());
            node.getExpr()->accept(&expr);
            equal = expr.isEqual();
        } 
        else equal = false;
    }
    void operator()(const PrintNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<PrintNode>(current)) {
            CompareVisitor expr(other->getExpr().get());
            node.getExpr()->accept(&expr);
            equal = expr.isEqual();
        } 
        else equal = false;
    }
    void operator()(const WhileNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<WhileNode>(current)) {
            CompareVisitor condition(other->getCondition().get());
            node.getCondition()->accept(&condition);
            if (!condition.isEqual()) { equal = false; return; }

            CompareVisitor body(other->getBody().get());
            node.getBody()->accept(&body);
            equal = body.isEqual();
        }
        else equal = false;
        
    }
    void operator()(const IfNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<IfNode>(current)) {
            CompareVisitor condition(other->getCondition().get());
            node.getCondition()->accept(&condition);
            if (!condition.isEqual()) { equal = false; return; }

            CompareVisitor body(other->getBody().get());
            node.getBody()->accept(&body);
            equal = body.isEqual();
        }
        else equal = false;

    }
    void operator()(const Block& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<Block>(current)) {
            const auto& this_body = node.getBody();
            const auto& other_body = other->getBody();
            
            if (this_body.size() != other_body.size()) {
                equal = false;
                return;
            }
            
            for (size_t i = 0; i < this_body.size(); ++i) {
                CompareVisitor stmt(other_body[i].get()); 
                this_body[i]->accept(&stmt);
                if (!stmt.isEqual()) { equal = false; return; }
            }
        } 
        else equal = false;
    }
};