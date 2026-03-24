class ASTNode {
public:
    virtual int traverse() const = 0;
};


class BinaryNode : public ASTNode {
    node_ptr left, right;
public:
    BinaryNode(Type t, node_ptr l, node_ptr r)
        : ASTNode(t), left(std::move(l)), right(std::move(r)) {}

    ASTNode* getLeft() const { return left.get(); }
    ASTNode* getRight() const { return right.get(); }

    int traverse() const override {
        int left = left->traverse();
        int right = right->traverse();
        return left + right;
    }
};




class ASTNode; 

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(class NumberNode* node) = 0;
    virtual void visit(class IdentNode* node) = 0;
    virtual void visit(class BinaryNode* node) = 0;
};

class PrintVisitor: ASTVisitor [
    void visit(class NumberNode* node);
]


class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;

    void accept() {
        *this->accept();
    }
};


class SimpleNode : public ASTNode {
    std::string value;
public:
    SimpleNode(Type t, std::string val) : ASTNode(t), value(val) {}
    const std::string& getValue() const { return value; }

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class BinaryNode : public ASTNode {
    node_ptr left, right;
public:
    BinaryNode(Type t, node_ptr l, node_ptr r)
        : ASTNode(t), left(std::move(l)), right(std::move(r)) {}

    ASTNode* getLeft() const { return left.get(); }
    ASTNode* getRight() const { return right.get(); }

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

int EvalVisitor::visit(const BinaryNode* node) const
{
    int left_value = node->left_->accept(this);
    int right_value = node->right_->accept(this);

    return node.operations_.at(node.op_)(left_value, right_value);
}

int EvalVisitor::visit(const NumberNode& node) const
{
    return node.value_;
}

int EvalVisitor::visit(const Block& node) const
{
    for(auto node : node.vector) {
        node->accept();
    }
}

using VariantNode = std::variant<BinaryNode, NumberNode>;

struct Node: public VariantNode {};

using node_ptr = std::unique_ptr<Node>;

struct Vistor {
    void operator()(const BinaryNode* node) {
        Visitor visitor;
        int ret = std::visit(visitor, node->left);
        int ret = std::visit(visitor, node->right);
    }
    void operator()(const NumberNode* node) {
        return node.value();
    }
}

