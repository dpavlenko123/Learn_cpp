#include "types.hpp"
#include "exceptions.hpp"
class ASTNode;
using node_ptr = std::unique_ptr<ASTNode>;
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
    node_ptr left, right;
public:
    BinaryNode(Type t, node_ptr l, node_ptr r) : ASTNode(t), 
    left(std::move(l)), right(std::move(r)) {}
    const node_ptr& getLeft() const {
        return left;
    }
    const node_ptr& getRight() const {
        return right;
    }
};

class InOutStmt : public ASTNode {
    node_ptr expr;
public:
    InOutStmt(Type t, node_ptr e) : ASTNode(t), expr(std::move(e)) {}
    const node_ptr& getExpr() const {
        return expr;
    }
};

class ConditionStmt : public ASTNode {
    node_ptr condition;
    std::vector<node_ptr> body;
public:
    ConditionStmt(Type t, node_ptr c, std::vector<node_ptr> b) : ASTNode(t), 
    condition(std::move(c)), body(std::move(b)) {}
    const std::vector<node_ptr>& getBody() const {
        return body;
    }
    const node_ptr& getCondition() const {
        return condition;
    }
};

class Block : public ASTNode {
    std::vector<node_ptr> body;
public:
    Block(std::vector<node_ptr> b) : ASTNode(Type::BLOCK),  body(std::move(b)) {}
    const std::vector<node_ptr>& getBody() const {
        return body;
    }
};

class Parser {
    TokenVectorConstIterator current;
    TokenVectorConstIterator end;

    Type look() const {
    if (current == end) {
        return Type::EOFTOKEN;
    }
    return (*current)->getType();
}

    std::string lookUp() const {
        std::string empty = "";
        if (current == end) return empty;
        return (*current)->getValue();
    }

    std::string getLine() const {
        return std::to_string((*current)->getLine());
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
    node_ptr parse(TokenVectorConstIterator beg, TokenVectorConstIterator e) {
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
        if (stmtList.empty()) throw СompilerError("Empty program");
        return stmtList;
    }

    std::vector<node_ptr> block() {
        if (!match(Type::LBRACKET)) throw СompilerError("Expected '{' on line " + getLine());
        auto node = statementList();
        if (!match(Type::RBRACKET)) throw СompilerError("Expected '}' on line " + getLine());
        return node;
    }

    node_ptr statement() {
        auto op = look();
        if (op==Type::WHILE || op==Type::IF) {
            next();
            if (!match(Type::LPAREN)) throw СompilerError("Expected '(' on line " + getLine());
            auto cond = simpleExpr();
            if (!match(Type::RPAREN)) throw СompilerError("Expected ')' on line " + getLine());
            auto body = block();
            return std::make_unique<ConditionStmt>(op, std::move(cond), std::move(body));
        }
        if (op==Type::PRINT || op==Type::IN) {
            next();
            if (!match(Type::LPAREN)) throw СompilerError("Expected '(' on line " + getLine());
            auto expr = simpleExpr();
            if (!match(Type::RPAREN)) throw СompilerError("Expected ')' on line " + getLine());
            if (!match(Type::SEMI)) throw СompilerError("Expected ';' on line " + getLine());
            return std::make_unique<InOutStmt>(op, std::move(expr));
        }
        return assignment();
    }

    node_ptr assignment() {
        node_ptr left = factor();
        if (left->getType()==Type::IDENTIFIER && look()==Type::ASSIGN) {
            next();
            auto right = simpleExpr();
            if (!match(Type::SEMI)) throw СompilerError("Expected ';' on line " + getLine());
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
            if (!match(Type::RPAREN)) throw СompilerError("Expected ')' on line " + getLine());
        }
        return node;
    }

};
