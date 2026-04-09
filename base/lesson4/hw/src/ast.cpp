#include "ast.hpp"
#include "visitor.hpp"

void IdntyNode::accept(ASTVisitor* v) { v->operator()(*this); }
void NumberNode::accept(ASTVisitor* v) { v->operator()(*this); }
void BinaryNode::accept(ASTVisitor* v) { v->operator()(*this); }
void PrintNode::accept(ASTVisitor* v) { v->operator()(*this); }
void InNode::accept(ASTVisitor* v) { v->operator()(*this); }
void WhileNode::accept(ASTVisitor* v) { v->operator()(*this); }
void IfNode::accept(ASTVisitor* v) { v->operator()(*this); }
void Block::accept(ASTVisitor* v) { v->operator()(*this); }
