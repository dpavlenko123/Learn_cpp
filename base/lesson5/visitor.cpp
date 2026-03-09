#include <memory>
#include <variant>

class NumberNode;
class AddNode;
class MulNode;

class Visitor {
public:
    virtual void visit(const NumberNode&) = 0;
    virtual void visit(const AddNode&) = 0;
    virtual void visit(const MulNode&) = 0;

    virtual ~Visitor() = default;
};

class Node {
public:
    virtual void accept(Visitor& v) const = 0;
    virtual ~Node() = default;
};

using NodePtr = std::unique_ptr<Node>;

class NumberNode : public Node {
public:
    explicit NumberNode(int v) : value(v) {}

    void accept(Visitor& v) const override {
        v.visit(*this);
    }

    int value;
};

class AddNode : public Node {
public:
    AddNode(NodePtr l, NodePtr r)
        : left(std::move(l)), right(std::move(r)) {}

    void accept(Visitor& v) const override {
        v.visit(*this);
    }

    NodePtr left;
    NodePtr right;
};

class MulNode : public Node {
public:
    MulNode(NodePtr l, NodePtr r)
        : left(std::move(l)), right(std::move(r)) {}

    void accept(Visitor& v) const override {
        v.visit(*this);
    }

    NodePtr left;
    NodePtr right;
};
///////////////////////////////




struct DumpVisitor;
struct EvalVisitor;

struct Node;

using AstNodePtr = std::unique_ptr<Node>;

class BinNode {
    friend DumpVisitor;
    friend EvalVisitor;
public:
    BinNode(AstNodePtr left, AstNodePtr right):
        left_(std::move(left)), right_(std::move(right)) {}
protected:
    AstNodePtr left_;
    AstNodePtr right_;
};

class AssignNode: public BinNode {
    friend DumpVisitor;
    friend EvalVisitor;
public:
    AssignNode(AstNodePtr left, AstNodePtr right): 
        BinNode(std::move(left), std::move(right)) {}
};

class IfNode: public BinNode {
    friend DumpVisitor;
    friend EvalVisitor;
public:
    IfNode(AstNodePtr left, AstNodePtr right): 
        BinNode(std::move(left), std::move(right)) {}
};

using VariantNode = std::variant<AssignNode,IfNode>;

struct Node: public VariantNode {};

struct EvalVisitor {
    int operator()(const AssignNode& node) {
        EvalVisitor visitor;
        int value = std::visit(visitor, *node.right_);

        auto ident_node = std::get<IdentNode>(*node.left_);
        ident_node.setValue(value);

        return 0;
    }

    int operator()(const IfNode& node) {
        EvalVisitor visitor;
        if (std::visit(visitor, *node.left_)) {
            return std::visit(visitor, *node.right_);
        }
        return 0;
    }
    int operator()(const NumberNode& node) {
        return node.value;
    }
}

template<typename Derived>
class Base {
    void print() {
        asDerived().output();
    };

    const Derived& asDerived() {
        return static_cast<const Derived&>(*this);
    }
};


class Derived: public Base<Derived> {
    void output() {
        std::cout << "Derived" << std::endl;
    }
};

class Derived1: public Base<Derived1> {
    void output() {
        std::cout << "Derived1" << std::endl;
    }
};


