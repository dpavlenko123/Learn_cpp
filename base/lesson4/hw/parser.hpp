#include "types.hpp"

class ASTNode {
    Type type;
public:
    ASTNode(Type t) : type(t) {}
    virtual ~ASTNode() = default;
    Type getType() {
        return type;
    }
    virtual const std::string& getValue() const {
        throw std::runtime_error("Token has no value");
    }
    virtual bool hasValue() const {
        return false;
    }
};

class SimpleNode : public ASTNode {
    std::string value;
public:
    SimpleNode(Type t, std::string val) : ASTNode(t), value(val) {}
    const std::string& getValue() const override {
        return value;
    }
    bool hasValue() const override {
        return true;
    }
};

class BinaryNode : public ASTNode {
    std::unique_ptr<ASTNode> left, right;
public:
    BinaryNode(Type t, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r) : ASTNode(t), 
    left(std::move(l)), right(std::move(r)) {}
    const std::unique_ptr<ASTNode>& getLeft() const {
        return left;
    }
    const std::unique_ptr<ASTNode>& getRight() const {
        return right;
    }
};

class InOutStmt : public ASTNode {
    std::unique_ptr<ASTNode> expr;
public:
    InOutStmt(Type t, std::unique_ptr<ASTNode> e) : ASTNode(t), expr(std::move(e)) {}
    const std::unique_ptr<ASTNode>& getExpr() const {
        return expr;
    }
};

class ConditionStmt : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;
public:
    ConditionStmt(Type t, std::unique_ptr<ASTNode> c, std::vector<std::unique_ptr<ASTNode>> b) : ASTNode(t), 
    condition(std::move(c)), body(std::move(b)) {}
    const std::vector<std::unique_ptr<ASTNode>>& getBody() const {
        return body;
    }
    const std::unique_ptr<ASTNode>& getCondition() const {
        return condition;
    }
};

class Block : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> body;
public:
    Block(std::vector<std::unique_ptr<ASTNode>> b) : ASTNode(Type::BLOCK),  body(std::move(b)) {}
    const std::vector<std::unique_ptr<ASTNode>>& getBody() const {
        return body;
    }
};

using node_ptr = std::unique_ptr<ASTNode>;

class Parser {
    std::vector<std::unique_ptr<Token>>::const_iterator current;
    std::vector<std::unique_ptr<Token>>::const_iterator end;

    Type look() const {
    if (current == end) {
        return Type::EOFTOKEN;
    }
    return (*current)->getType();
}

    std::string lookUp() const {
        static std::string empty = "";
        if (current == end) return empty;
        return (*current)->getValue();
    }

    void next() {
        if (current != end) ++current;
    }

    bool match(Type type) {
        if (look()==type) {
            next();
            return true;
        }
        else return false;
    }
public:
    node_ptr parse(typename std::vector<std::unique_ptr<Token>>::const_iterator beg, typename std::vector<std::unique_ptr<Token>>::const_iterator e) {
        current = beg;
        end = e;
        return program();
    }
private:
    node_ptr program() {
        auto list = statementList();
        return std::make_unique<Block>(std::move(list));
    }

    std::vector<node_ptr> statementList() {
        std::vector<node_ptr> stmtList;

        while (look()!=Type::EOFTOKEN && look()!=Type::RBRACKET) {
            node_ptr node = statement();
            if (node) {
                stmtList.push_back(std::move(node));
            }
        }
        return stmtList;
    }

    std::vector<node_ptr> block() {
        match(Type::LBRACKET);
        auto node = statementList();
        match(Type::RBRACKET);
        return node;
    }

    node_ptr statement() {
        auto op = look();
        if (op==Type::WHILE || op==Type::IF) {
            next();
            match(Type::LPAREN);
            auto cond = simpleExpr();
            match(Type::RPAREN);
            auto body = block();
            return std::make_unique<ConditionStmt>(op, std::move(cond), std::move(body));
        }
        if (op==Type::PRINT || op==Type::IN) {
            next();
            match(Type::LPAREN);
            auto expr = simpleExpr();
            match(Type::RPAREN);
            match(Type::SEMI);
            return std::make_unique<InOutStmt>(op, std::move(expr));
        }
        return assignment();
    }

    node_ptr assignment() {
        node_ptr left = factor();
        if (left->getType()==Type::IDENTIFIER && look()==Type::ASSIGN) {
            next();
            auto right = simpleExpr();
            match(Type::SEMI);
            return std::make_unique<BinaryNode>(Type::ASSIGN, std::move(left), std::move(right));
        }
        return left;
    }

    node_ptr simpleExpr() {
        node_ptr left = expression();
        auto op = look();
        if (op==Type::EQ || op==Type::NEQ || op==Type::LESS || op==Type::MORE || op==Type::LESSEQ || op==Type::MOREEQ) {
            next();
            auto right = expression();
            return std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
        }
        return left;
    }

    node_ptr expression() {
        node_ptr left = term();
        auto op = look();
        while (op==Type::PLUS || op==Type::MINUS) {
            next();
            auto right = term();
            left = std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
            op = look();
        }
        return left;
    }

    node_ptr term() {
        node_ptr left = factor();
        auto op = look();
        while (op==Type::MULTIPLY || op==Type::DIVIDE) {
            next();
            auto right = factor();
            left = std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
            op = look();
        }
        return left;
    }

    node_ptr factor() {
        node_ptr node;
        if (look()==Type::IDENTIFIER) {
            node = std::make_unique<SimpleNode>(Type::IDENTIFIER, lookUp());
            next();
            return node;
        }
        if (look()==Type::NUMBER) {
            node = std::make_unique<SimpleNode>(Type::NUMBER, lookUp());
            next();
            return node;
        }
        if (look()==Type::LPAREN) {
            next();
            node = expression();
            match(Type::RPAREN);
        }
        return node;
    }

};
