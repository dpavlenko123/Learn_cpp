#pragma once
#include "exceptions.hpp"
#include "ast.hpp"

class Parser {
    TokenIter current;
    TokenIter end;

    Type type() const {
        if (current == end) {
            return Type::EOFTOKEN;
        }
        return (*current)->getType();
    }

    std::string value() const {
        std::string empty = "";
        if (current == end) return empty;
        if ((*current)->hasValue()) return (*current)->getValue();
        else return empty;
    }

    std::string typeToString(Type type) const {
        switch (type) {
            case Type::WHILE: return "while";
            case Type::ASSIGN: return "=";
            case Type::DIVIDE: return "/";
            case Type::EQ: return "==";
            case Type::IF: return "IF";
            case Type::IN: return "IN";
            case Type::LBRACKET: return "{";
            case Type::LESS: return "<";
            case Type::LESSEQ: return "<=";
            case Type::LPAREN: return "(";
            case Type::MINUS: return "-";
            case Type::MORE: return ">";
            case Type::MOREEQ: return ">=";
            case Type::MULTIPLY: return "*";
            case Type::NEQ: return "!=";
            case Type::PLUS: return "+";
            case Type::PRINT: return "print";
            case Type::RBRACKET: return "}";
            case Type::RPAREN: return ")";
            case Type::SEMI: return ";";
            case Type::IDENTIFIER: return "IDENTIFIER";
            case Type::NUMBER: return "NUMBER";
            default: return "UNKNOWN";
        }
    }

    std::string typeStr() const {
        return typeToString(type());
    }

    std::string getLine() const {
        return std::to_string((*current)->getLine());
    }

    void next() {
        if (current != end) ++current;
    }

    bool match(Type t) {
        if (type()==t) {
            next();
            return true;
        }
        else return false;
    }

    template<typename T, typename... Args>
    node_ptr construct(Args&&... args) {
        return std::make_unique<Node>(std::in_place_type<T>, std::forward<Args>(args)...);
    }

    void expected(Type t) {
        if (!match(t)) throw СompilerError("Expected" + typeToString(t) + " on line " + getLine());
    }
public:
    node_ptr parse(TokenIter beg, TokenIter e) {
        current = beg;
        end = e;
        return program();
    }
private:
    node_ptr program() {
        auto list = statementList();
        return list;
    }

    node_ptr statementList() {
        std::vector<node_ptr> stmtList;

        while (type()!=Type::EOFTOKEN && type()!=Type::RBRACKET) {
            node_ptr node = statement();
            if (node) {
                stmtList.push_back(std::move(node));
            }
        }
        if (stmtList.empty()) throw СompilerError("Empty program");
        return construct<Block>(std::move(stmtList));
    }

    node_ptr block() {
        expected(Type::LBRACKET);
        auto node = statementList();
        expected(Type::RBRACKET);
        return node;
    }

    node_ptr statement() {
        auto op = type();
        if (op==Type::WHILE) {
            next();
            expected(Type::LPAREN);
            auto cond = simpleExpr();
            expected(Type::RPAREN);
            auto body = block();
            return construct<WhileNode>(std::move(cond), std::move(body));
        }
        if (op==Type::IF) {
            next();
            expected(Type::LPAREN);
            auto cond = simpleExpr();
            expected(Type::RPAREN);
            auto body = block();
            return construct<IfNode>(std::move(cond), std::move(body));
        }
        if (op==Type::PRINT) {
            next();
            expected(Type::LPAREN);
            auto expr = simpleExpr();
            expected(Type::RPAREN);
            expected(Type::SEMI);
            return construct<PrintNode>(std::move(expr));
        }
        if (op==Type::IN) {
            next();
            expected(Type::LPAREN);
            expected(Type::IDENTIFIER);
            auto expr = factor();
            expected(Type::RPAREN);
            expected(Type::SEMI);
            return construct<InNode>(std::move(expr));
        }
        return assignment();
    }

    node_ptr assignment() {
        node_ptr left = factor();
        if (type()==Type::ASSIGN) {
            if (std::holds_alternative<IdntyNode>(*left)) {
                next();
                auto right = simpleExpr();
                expected(Type::SEMI);
                return construct<BinaryNode>(std::move(left), std::move(right), "=");
            }
            else throw СompilerError("Expected IDENTIFIER before '=' on line" + getLine());
        }
        return left;
    }

    node_ptr simpleExpr() {
        node_ptr left = expression();
        auto op = typeStr();
        if (op=="<" || op==">" || op=="<=" || op==">=" || op=="==" || op=="!=") {
            next();
            auto right = expression();
            return construct<BinaryNode>(std::move(left), std::move(right), op);
        }
        return left;
    }

    node_ptr expression() {
        node_ptr left = term();
        std::string op = typeStr();
        while (type()==Type::PLUS || type()==Type::MINUS) {
            next();
            auto right = term();
            left = construct<BinaryNode>(std::move(left), std::move(right), op);
            op = typeStr();
        }
        return left;
    }

    node_ptr term() {
        node_ptr left = factor();
        auto op = typeStr();
        while (type()==Type::MULTIPLY || type()==Type::DIVIDE) {
            next();
            auto right = factor();
            left = construct<BinaryNode>(std::move(left), std::move(right), op);
            op = typeStr();
        }
        return left;
    }

    node_ptr factor() {
        if (type()==Type::IDENTIFIER) {
            node_ptr node = construct<IdntyNode>(value());
            next();
            if (type()==Type::SEMI) throw СompilerError("Unitianilized variable " + value() + " on line " + getLine());
            return node;
        }
        if (type()==Type::NUMBER) {
            node_ptr node = construct<NumberNode>(std::stod(value()));
            next();
            return node;
        }
        if (type()==Type::LPAREN) {
            next();
            node_ptr node = expression();
            if (!match(Type::RPAREN)) throw СompilerError("Expected ')' on line " + getLine());
        }
        throw СompilerError("Unexpected symbol on line" + getLine());
    }
};
