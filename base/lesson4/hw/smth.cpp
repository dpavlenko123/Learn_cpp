#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <memory>
#include <cctype>
enum class Type {
        NUMBER,
        IDENTIFIER,
        OPERATOR,
        SEMICOLON,
        RPAREN,
        LPAREN,
        RBRACKET,
        LBRACKET,
        KEYWORD,
        EOFTOKEN
    };


class Token {
    Type type;
    std::string value;
    size_t line_number;
    std::string typeToString() const {
        switch(type) {
            case Type::IDENTIFIER:   return "IDENTIFIER"; break;
            case Type::NUMBER: return "NUMBER"; break;
            case Type::OPERATOR:  return "OPERATOR"; break;
            case Type::SEMICOLON: return "SEMICOLON"; break;
            case Type::RPAREN: return "RPAREN"; break;
            case Type::LPAREN: return "LPAREN"; break;
            case Type::RBRACKET: return "RBRACKET"; break;
            case Type::LBRACKET: return "LBRACKET"; break;
            case Type::KEYWORD: return "KEYWORD"; break;
            case Type::EOFTOKEN: return "EOFTOKEN"; break;
            default: return "UNKNOWN";
        }
    }
public:
    Token() = default;
    Token(Type t, size_t number, const std::string& v = "") : type(t), line_number(number), value(v) {}
    std::string getTypeStr() const {
        return typeToString();
    }
    Type getType() const {
        return type;
    }
    std::string getValue() const {
        return value;
    }
    size_t getLineNumber() const {
        return line_number;
    }
};

template <typename Iter>
class Lexer {
private:
    std::vector<Token> tokens;
    Iter current;
    Iter eof;

    bool is_idnty(char c) {
        return std::isalpha(c);
    }
    bool is_number(char c) {
        return std::isdigit(c);
    }
    bool is_operator(char c) {
        return c=='+' || c=='-' || c=='*' || c=='/' ||
            c=='=' || c=='<' || c=='>';
    }
    bool is_semicolon(char c) {
        return c==';';
    }
    bool ignore(char c) {
        return c==' ' || c=='\t' || c=='\v';
    }
    bool is_lparen(char c) {
        return c=='(';
    }
    bool is_rparen(char c) {
        return c==')';
    }
    bool is_rbracket(char c) {
        return c=='}';
    }
    bool is_lbracket(char c) {
        return c=='{';
    }
    bool is_new_line(char c) {
        return c =='\n';
    }
public:
    Lexer() = default;
    Lexer(Iter cur, Iter e) : current(cur), eof(e) {} 
public:
    void tokenize() {
        size_t line_number=0;
        while (current != eof) {
            while (ignore(*current)) {
                ++current;
            }
            
            if (current == eof) break;
            
            if (is_idnty(*current)) {
                std::string value;
                while (current != eof && is_idnty(*current)) {
                    value += *current;
                    ++current;
                }
                if (value=="while" || value=="if") tokens.emplace_back(Type::KEYWORD, line_number,value);
                else tokens.emplace_back(Type::IDENTIFIER, line_number, value);
            }
            
            else if(is_lparen(*current)) { tokens.emplace_back(Type::LPAREN, line_number); ++current; }
            
            else if(is_rparen(*current)) { tokens.emplace_back(Type::RPAREN, line_number); ++current; } 

            else if(is_lbracket(*current)) { tokens.emplace_back(Type::LBRACKET, line_number); ++current; }
            
            else if(is_rbracket(*current)) { tokens.emplace_back(Type::RBRACKET, line_number); ++current; } 
            
            else if (is_number(*current)) {
                std::string value;
                while (current != eof && is_number(*current)) {
                    value += *current;
                    ++current;
                }
                tokens.emplace_back(Type::NUMBER, line_number, value);
            }
            
            else if (is_operator(*current)) {
                std::string value;
                while (current != eof && is_operator(*current)) {
                    value += *current;
                    ++current;
                }
                tokens.emplace_back(Type::OPERATOR, line_number, value);
            }
            
            else if (is_semicolon(*current)) {
                tokens.emplace_back(Type::SEMICOLON, line_number);
                ++current;
            }

            else if (is_new_line(*current)) {++line_number; ++current; }
            
            else { ++current; }
        }
        tokens.emplace_back(Type::EOFTOKEN, line_number);
    }

    std::pair<std::string, std::string> operator[](size_t idx) const {
        return std::make_pair(tokens[idx].getTypeStr(), tokens[idx].getValue());
    }

    std::vector<Token>::iterator begin() {
        return tokens.begin();
    }

    std::vector<Token>::iterator end() {
        return tokens.end();
    }

    std::vector<Token>& getTokens() { return tokens; }
    const std::vector<Token>& getTokens() const { return tokens; }
}; 

enum class NodeT {EXPR, STMT};

enum class ExprT {OP_EXPR, CONST_EXPR, ID_EXPR};

enum class StmtT {PRINT_STMT, IN_STMT, WHILE_STMT, IF_STMT, ASSIGN_STMT};

struct ASTNode;

using node_ptr = std::unique_ptr<ASTNode>;

struct ASTNode {
    NodeT kind;
    union {ExprT expr; StmtT stmt;} type;
    struct {Type token_type; std::string value;} data;
    node_ptr next;
    node_ptr left;
    node_ptr right;
    ASTNode(Type token, const std::string& val) {
        data.token_type = token;
        data.value = val;
        next = nullptr;
        left = nullptr;
        right = nullptr;
    }
    void print(int depth = 0) {
        for (int i = 0; i < depth; i++) std::cout << "  ";
        
        if (kind == NodeT::EXPR) {
            switch (type.expr) {
                case ExprT::ID_EXPR:
                    std::cout << "ID: " << data.value << std::endl;
                    break;
                case ExprT::CONST_EXPR:
                    std::cout << "CONST: " << data.value << std::endl;
                    break;
                case ExprT::OP_EXPR:
                    std::cout << "OP: " << data.value << std::endl;
                    break;
                default:
                    std::cout << "EXPR" << std::endl;
            }
        } else { 
            switch (type.stmt) {
                case StmtT::ASSIGN_STMT:
                    std::cout << "ASSIGN" << std::endl;
                    break;
                case StmtT::WHILE_STMT:
                    std::cout << "WHILE" << std::endl;
                    break;
                case StmtT::IF_STMT:
                    std::cout << "IF" << std::endl;
                    break;
                case StmtT::PRINT_STMT:
                    std::cout << "PRINT" << std::endl;
                    break;
                case StmtT::IN_STMT:
                    std::cout << "IN" << std::endl;
                    break;
            }
        }
        
        if (left) {
            for (int i = 0; i < depth; i++) std::cout << "  ";
            std::cout << "LEFT:" << std::endl;
            left->print(depth + 1);
        }
        
        if (right) {
            for (int i = 0; i < depth; i++) std::cout << "  ";
            std::cout << "RIGHT:" << std::endl;
            right->print(depth + 1);
        }
        
        if (next) {
            for (int i = 0; i < depth; i++) std::cout << "  ";
            std::cout << "NEXT:" << std::endl;
            next->print(depth);
        }
    }
};

node_ptr makeExprNode(ExprT type, Type token, const std::string& val) {
    auto node = std::make_unique<ASTNode>(token, val);
    node->kind = NodeT::EXPR;
    node->type.expr = type;
    return node;
}

node_ptr makeStmtNode(StmtT type, Type token, const std::string& val) {
    auto node = std::make_unique<ASTNode>(token, val);
    node->kind = NodeT::STMT;
    node->type.stmt = type;
    return node;
}

class Parser {
    std::vector<Token>::iterator current;
    std::vector<Token>::iterator end;

    Type look() const {
    if (current == end) {
        return Type::EOFTOKEN;
    }
    return current->getType();
}

    std::string lookUp() const {
        static std::string empty = "";
        if (current == end) return empty;
        return current->getValue();
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
    node_ptr parse(typename std::vector<Token>::iterator beg, typename std::vector<Token>::iterator e) {
        current = beg;
        end = e;
        return program();
    }
private:
    node_ptr program() {
        return statementList();
    }

    node_ptr statementList() {
        node_ptr head = statement();
        auto tail = head.get();

        while (look()!=Type::EOFTOKEN && look()!=Type::RBRACKET) {
            node_ptr node = statement();
            if (head && node) {
                tail->next = std::move(node);
                tail = tail->next.get();
            }
        }
        return head;
    }

    node_ptr block() {
        match(Type::LBRACKET);
        node_ptr node = statementList();
        match(Type::RBRACKET);
        return node;
    }

    node_ptr statement() {
        node_ptr node;
        if (look()==Type::KEYWORD && (lookUp()=="while" || lookUp()=="if")) {
            if (lookUp()=="while") node = makeStmtNode(StmtT::WHILE_STMT, look(), lookUp());
            else node = makeStmtNode(StmtT::IF_STMT, look(), lookUp());
            next();
            match(Type::LPAREN);
            node->left = simpleExpr();
            match(Type::RPAREN);
            node->right = block();
            return node;
        }
        if (look()==Type::KEYWORD && (lookUp()=="print" || lookUp()=="in")) {
            if (lookUp()=="print") node = makeStmtNode(StmtT::PRINT_STMT, look(), lookUp());
            else node = makeStmtNode(StmtT::IN_STMT, look(), lookUp());
            next();
            match(Type::LPAREN);
            node->next = simpleExpr();
            match(Type::RPAREN);
            return node;
        }
        node = assignment();
        return node;
    }

    node_ptr assignment() {
        node_ptr node = factor();
        if (node->data.token_type==Type::IDENTIFIER) {
            if (look()==Type::OPERATOR) {
                if (lookUp()=="=") {
                    node_ptr node2 = makeStmtNode(StmtT::ASSIGN_STMT, look(), lookUp());
                    next();
                    node2->left = std::move(node);
                    node = std::move(node2);
                    node->right = simpleExpr();
                    match(Type::SEMICOLON);
                }
            }
        }
        return node;
    }

    node_ptr simpleExpr() {
        node_ptr node = expression();
        if (look()==Type::OPERATOR && std::regex_match(lookUp(), std::regex("[<>]|==|<=|>=|!="))) {
            node_ptr node2 = makeExprNode(ExprT::OP_EXPR, look(), lookUp());
            next();
            node2->left = std::move(node);
            node = std::move(node2);
            node->right = expression();
        }
        return node;
    }

    node_ptr expression() {
        node_ptr node = term();
        if (look()==Type::OPERATOR && (lookUp()=="+" || lookUp()=="-")) {
            node_ptr node2 = makeExprNode(ExprT::OP_EXPR, look(), lookUp());
            next();
            node2->left = std::move(node);
            node = std::move(node2);
            node->right = term();
        }
        return node;
    }

    node_ptr term() {
        node_ptr node = factor();
        if (look()==Type::OPERATOR && (lookUp()=="*" || lookUp()=="/")) {
            node_ptr node2 = makeExprNode(ExprT::OP_EXPR, look(), lookUp());
            next();
            node2->left = std::move(node);
            node = std::move(node2);
            node->right = term();
        }
        return node;
    }

    node_ptr factor() {
        node_ptr node;
        if (look()==Type::IDENTIFIER) {
            node = makeExprNode(ExprT::ID_EXPR, look(), lookUp()); 
            next();
            return node;
        }
        if (look()==Type::NUMBER) {
            node = makeExprNode(ExprT::CONST_EXPR, look(), lookUp()); 
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


int main(int argc, char* argv[]) {
    if (argc!=2) return 1;

    std::ifstream file(argv[1]);

    if (!file) return 1;

    std::string code((std::istreambuf_iterator<char>(file)), 
                  std::istreambuf_iterator<char>());
    
    Lexer<std::string::iterator> lexer(code.begin(), code.end());
    lexer.tokenize();
    
    for (const auto& token: lexer) {
        std::cout << token.getTypeStr() << ": " << token.getValue() << std::endl;
    }
    
    Parser parser;
    auto ast = parser.parse(lexer.begin(), lexer.end());
    std::cout << "\n=== AST ===\n";
    ast->print();
    
    return 0;
}