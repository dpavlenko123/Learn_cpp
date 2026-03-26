#include "ast.hpp"
#include "visitor.hpp"

void IdntyNode::accept(ASTVisitor* v) { (*v)(*this); }
void NumberNode::accept(ASTVisitor* v) { (*v)(*this); }
void BinaryNode::accept(ASTVisitor* v) { (*v)(*this); }
void PrintNode::accept(ASTVisitor* v) { (*v)(*this); }
void InNode::accept(ASTVisitor* v) { (*v)(*this); }
void WhileNode::accept(ASTVisitor* v) { (*v)(*this); }
void IfNode::accept(ASTVisitor* v) { (*v)(*this); }
void Block::accept(ASTVisitor* v) { (*v)(*this); }

void Node::accept(ASTVisitor* v) {
    std::visit([v](auto& node) {
        node.accept(v);
    }, *this);
}