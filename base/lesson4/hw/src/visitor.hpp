#pragma once
#include "ast.hpp"
#include <stack>
#include <unordered_map>
#include <sstream>
#include "exceptions.hpp"
#include <cmath>
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
    double get_value() const {
        if (evalStack.empty()) {
            throw std::runtime_error("No value on stack");
        }
        return evalStack.top();
    }
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
            std::visit(*this, *node.getRight());
            auto val = evalStack.top(); evalStack.pop();
            const auto* id = std::get_if<IdntyNode>(node.getLeft().get());
            if (!id) {
                throw CompilerError("Left side of assignment is not an identifier");
            }
            identifiers[id->getValue()] = val;
            evalStack.push(val);
            return;
        }
        std::visit(*this, *node.getLeft());
        std::visit(*this, *node.getRight());

        if (evalStack.size() < 2) {
            throw CompilerError("Not enough values on stack");
        }

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
        double val;
        std::cin >> val;

        const auto* id = std::get_if<IdntyNode>(node.getExpr().get());
        identifiers[id->getValue()] = val;
    }
    void operator()(const PrintNode& node) override {
        std::visit(*this, *node.getExpr());
        double val = evalStack.top(); evalStack.pop();
        std::cout << val <<std::endl;
    }
    void operator()(const WhileNode& node) override {
        std::visit(*this, *node.getCondition());
        bool condition = evalStack.top()!=0.0; evalStack.pop();
        while (condition) {
            std::visit(*this, *node.getBody());
            std::visit(*this, *node.getCondition());
            condition = evalStack.top()!=0.0; evalStack.pop();
        }
    }
    void operator()(const IfNode& node) override {
        std::visit(*this, *node.getCondition());
        bool condition = evalStack.top()!=0.0; evalStack.pop();
        if (condition) {
            std::visit(*this, *node.getIfBody());
        }
        else if (node.getElseBody()) {
            std::visit(*this, *node.getElseBody());
        }
    }
    void operator()(const Block& node) override {
        for (const auto& stmt : node.getBody()) {
            std::visit(*this, *stmt);
        }
    }
};

class CompareVisitor : public EvalVisitor {
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
            std::visit(left, *node.getLeft());
            if (!left.isEqual()) { equal = false; return; }
            
            CompareVisitor right(other->getRight().get());
            std::visit(right, *node.getRight());
            equal = right.isEqual();
        } 
        else equal = false;
        
    }
    void operator()(const InNode& node) override {
        if (!equal) return;

        if (const auto* other = std::get_if<InNode>(current)) {
            CompareVisitor expr(other->getExpr().get());
            std::visit(expr, *node.getExpr());
            equal = expr.isEqual();
        } 
        else equal = false;
    }
    void operator()(const PrintNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<PrintNode>(current)) {
            CompareVisitor expr(other->getExpr().get());
            std::visit(expr, *node.getExpr());
            equal = expr.isEqual();
        } 
        else equal = false;
    }
    void operator()(const WhileNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<WhileNode>(current)) {
            CompareVisitor condition(other->getCondition().get());
            std::visit(condition, *node.getCondition());
            if (!condition.isEqual()) { equal = false; return; }

            CompareVisitor body(other->getBody().get());
            std::visit(body, *node.getBody());
            equal = body.isEqual();
        }
        else equal = false;
    }
    void operator()(const IfNode& node) override {
        if (!equal) return;
        
        if (const auto* other = std::get_if<IfNode>(current)) {
            CompareVisitor condition(other->getCondition().get());
            std::visit(condition, *node.getCondition());
            if (!condition.isEqual()) { equal = false; return; }

            CompareVisitor ifBody(other->getIfBody().get());
            std::visit(ifBody, *node.getIfBody());
            if (!ifBody.isEqual()) { equal=false; return; } 
            CompareVisitor elseBody(other->getElseBody().get());
            std::visit(elseBody, *node.getElseBody());
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
                std::visit(stmt, *this_body[i]);
                if (!stmt.isEqual()) { equal = false; return; }
            }
        } 
        else equal = false;
    }
};



class StackVisitor : public ASTVisitor {
    std::stringstream text;
    std::unordered_map<std::string, size_t> vars;
    size_t addr = 0x100;
    size_t bbCounter = 0;

    std::string newBb() {
        return "bb" + std::to_string(bbCounter++);
    }
    
    void generate(std::string&& str) {
        text << std::move(str) << std::endl;
    }

    void allocateVar(const std::string& name) {
        if (!vars.contains(name)) {
            vars[name] = addr;
            addr += 4;
        }
    }

    size_t getVarAddress(const std::string& name) const {
        auto it = vars.find(name);
        if (it == vars.end()) {
            throw CompilerError("Variable '" + name + "' used before initialization");
        }
        return it->second;
    }
    std::string toHex(size_t value) {
        return std::to_string(value); 
    }
public:
    std::string getCode() const { return text.str(); }

    void operator()(const NumberNode& node) override {
        int val = node.getValue();
        generate("\tPUSH " + std::to_string(val));
    }
    void operator()(const IdntyNode& node) override {
        size_t address = getVarAddress(node.getValue());
        generate("\tPUSH " + toHex((address)));
    }
    void operator()(const BinaryNode& node) override {
        const std::string& op = node.getOperation();
    
        if (op == "=") {
            auto* leftId = std::get_if<IdntyNode>(node.getLeft().get());
            if (!leftId) {
                throw CompilerError("Left side of assignment must be variable");
            }
            
            allocateVar(leftId->getValue());
            std::visit(*this, *node.getRight());
            std::visit(*this, *node.getLeft());
            
            generate("\tSTORE");
        }
        else {
            std::visit(*this, *node.getLeft());
            if (std::get_if<IdntyNode>(node.getLeft().get())) {
                generate("\tLOAD");
            }

            std::visit(*this, *node.getRight());
            if (std::get_if<IdntyNode>(node.getRight().get())) {
                generate("\tLOAD");
            }
            
            if (op == "<") generate("\tLT");
            else if (op == ">") generate("\tGT");
            else if (op == ">=") generate("\tGE");
            else if (op == "<=") generate("\tLE");
            else if (op == "==") generate("\tEQ");
            else if (op == "!=") generate("\tNE");
            else if (op == "+") generate("\tADD");
            else if (op == "-") generate("\tSUB");
            else if (op == "/") generate("\tDIV");
            else if (op == "*") generate("\tMUL");
            else {
                throw std::runtime_error("Unknown operator: " + op);
            }
        }
    }
    void operator()(const InNode& node) override {
        generate("\tIN");
        auto* expr = std::get_if<IdntyNode>(node.getExpr().get());
        if (!expr) throw CompilerError("Expression of IN must be variable");
        allocateVar(expr->getValue());
        std::visit(*this, *node.getExpr());
        generate("\tSTORE");
    }
    void operator()(const PrintNode& node) override {
        std::visit(*this, *node.getExpr());
        if (std::get_if<IdntyNode>(node.getExpr().get())) {
                generate("\tLOAD");
            }
        generate("\tOUT");
    }
    void operator()(const WhileNode& node) override {
        std::string condition = newBb();
        std::string body = newBb();
        std::string end = newBb();
        generate("\tJMP " + condition);
        generate(condition + ":");
        std::visit(*this, *node.getCondition());
        generate("\tJE " + body + " " + end);
        generate(body + ":");
        std::visit(*this, *node.getBody());
        
        generate("\tJMP " + condition);
        generate(end + ":");
    }
    void operator()(const IfNode& node) override {
        std::string ifBody = newBb();
        std::string end;
        std::visit(*this, *node.getCondition());
        if (node.getElseBody()) {
            std::string elseBody = newBb();
            end = newBb();
            generate("\tJE " + ifBody + " " + elseBody);
            generate(ifBody + ":");
            std::visit(*this, *node.getIfBody());
            generate("\tJMP " + end);
            generate(elseBody + ":");
            std::visit(*this, *node.getElseBody());
            generate("\tJMP " + end);
        }
        else {
            end = newBb();
            generate("\tJE " + ifBody + " " + end);
            generate(ifBody + ":");
            std::visit(*this, *node.getIfBody());
            generate("\tJMP " + end);
        }
        generate(end + ":");
    }
    void operator()(const Block& node) override {
        for (const auto& stmt : node.getBody()) {
            std::visit(*this, *stmt);
        }
    }
};