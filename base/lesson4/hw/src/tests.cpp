#include <gtest/gtest.h>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "visitor.hpp"
class ParserTest : public ::testing::Test {
protected:
    std::unique_ptr<Lexer> lexer;
    std::unique_ptr<Parser> parser;
    node_ptr ast;
    
    void parse(const std::string& input) {
        lexer = std::make_unique<Lexer>(input);
        lexer->tokenize();
        parser = std::make_unique<Parser>();
        auto begin = lexer->begin();
        auto end = lexer->end();
        ast = parser->parse(begin, end);
    }

    template<typename T, typename... Args>
    node_ptr make_node(Args&&... args) {
        T node = T(std::forward<Args>(args)...);
        return std::make_unique<Node>(std::move(node));
    }
};

TEST_F(ParserTest, SimpleEvaluation) {
    parse("x = 2 + 3;");
    EvalVisitor visitor;
    std::visit(visitor, *ast);
    EXPECT_EQ(5, visitor.get_value());
}

// TEST_F(ParserTest, CompareTrees) {
//     parse("x = (2 + 3) * 4;");

//     auto tree = make_binary(make_idnty("x"), 
//                             make_binary(make_binary(make_number(2), make_number(3), "+"), make_number(4), "*"), "=");
//     CompareVisitor compVisitor(tree.get());
//     ast->accept(&compVisitor);
//     EXPECT_TRUE(compVisitor.isEqual());
// }

TEST_F(ParserTest, CompareTrees) {
    parse("x = 2 + 3;");
    
    std::vector<node_ptr> statements;
    statements.push_back(
        make_node<BinaryNode>(
            make_node<IdntyNode>("x"),
            make_node<BinaryNode>(
                make_node<NumberNode>(2), 
                make_node<NumberNode>(3), 
                "+"
            ),
            "="
        )
    );
    auto tree = make_node<Block>(std::move(statements));
    
    CompareVisitor compVisitor(tree.get());
    std::visit(compVisitor, *ast);
    EXPECT_TRUE(compVisitor.isEqual());
}






// class LexerTest : public ::testing::Test {
// protected:
//     std::unique_ptr<Lexer> lexer;
//     std::vector<std::unique_ptr<Token>>* tokens;
//     size_t idx = 0;
    
//     void tokenize(const std::string& input) {
//         lexer = std::make_unique<Lexer>(input);
//         lexer->tokenize();
//         tokens = &lexer->getTokens();
//     }

//     void expectToken(Type expected_type, const std::string& expected_value = "") {
//         ASSERT_LT(idx, tokens->size());
//         const auto& token = (*tokens)[idx];
//         EXPECT_EQ(token->getType(), expected_type);
//         if (token->hasValue()) {
//             EXPECT_EQ(token->getValue(), expected_value);
//         }
//         ++idx;
//     }

//     void expectEnd() {
//         if (idx<tokens->size()) {
//             expectToken(Type::EOFTOKEN);
//         }
//         EXPECT_EQ(idx, tokens->size());
//     }
// };

// TEST_F(LexerTest, Assignment) {
//     tokenize("a = 5;");
//     expectToken(Type::IDENTIFIER, "a");
//     expectToken(Type::ASSIGN);
//     expectToken(Type::NUMBER, "5");
//     expectToken(Type::SEMI);
//     expectEnd();
// }

// TEST_F(LexerTest, Throw) {
//     EXPECT_THROW(tokenize("a = @"), СompilerError);
// }

// TEST_F(LexerTest, Keywords) {
//     tokenize("while in print if");
//     expectToken(Type::WHILE);
//     expectToken(Type::IN);
//     expectToken(Type::PRINT);
//     expectToken(Type::IF);
//     expectEnd();
// }

// TEST_F(LexerTest, ComparisonOperators) {
//     tokenize("< >");
    
//     expectToken(Type::LESS);
//     expectToken(Type::MORE);
//     expectEnd();
// }

// TEST_F(LexerTest, Delimiters) {
//     tokenize("( ) { } ;");
    
//     expectToken(Type::LPAREN);
//     expectToken(Type::RPAREN);
//     expectToken(Type::LBRACKET);
//     expectToken(Type::RBRACKET);
//     expectToken(Type::SEMI);
//     expectEnd();
// }

// TEST_F(LexerTest, ArithmeticOperators) {
//     tokenize("+ - * /");
    
//     expectToken(Type::PLUS);
//     expectToken(Type::MINUS);
//     expectToken(Type::MULTIPLY);
//     expectToken(Type::DIVIDE);
//     expectEnd();
// }


// TEST_F(LexerTest, DoubleOperators) {
//     tokenize("== <= >= !=");
//     expectToken(Type::EQ);
//     expectToken(Type::LESSEQ);
//     expectToken(Type::MOREEQ);
//     expectToken(Type::NEQ);
//     expectEnd();
// }

// TEST_F(LexerTest, Numbers) {
//     tokenize("123 0 999 42");
    
//     expectToken(Type::NUMBER, "123");
//     expectToken(Type::NUMBER, "0");
//     expectToken(Type::NUMBER, "999");
//     expectToken(Type::NUMBER, "42");
//     expectEnd();
// }

// TEST_F(LexerTest, LongIdentifiers) {
//     tokenize("very_long_variable_name = 5;");
    
//     expectToken(Type::IDENTIFIER, "very_long_variable_name");
//     expectToken(Type::ASSIGN);
//     expectToken(Type::NUMBER, "5");
//     expectToken(Type::SEMI);
//     expectEnd();
// }

// TEST_F(LexerTest, Whitespace) {
//     tokenize("a\n\n  =\n5  ;");
    
//     expectToken(Type::IDENTIFIER, "a");
//     expectToken(Type::ASSIGN);
//     expectToken(Type::NUMBER, "5");
//     expectToken(Type::SEMI);
//     expectEnd();
// }

// TEST_F(LexerTest, FullProgram) {
//     tokenize(R"(
//         while(a < 10) {
//             if(b <= 5) {
//                 c = 15;
//             }
//             a = a + 1;
//         }
//     )");
    
//     expectToken(Type::WHILE);
//     expectToken(Type::LPAREN);
//     expectToken(Type::IDENTIFIER, "a");
//     expectToken(Type::LESS);
//     expectToken(Type::NUMBER, "10");
//     expectToken(Type::RPAREN);
//     expectToken(Type::LBRACKET);
    
//     expectToken(Type::IF);
//     expectToken(Type::LPAREN);
//     expectToken(Type::IDENTIFIER, "b");
//     expectToken(Type::LESSEQ);
//     expectToken(Type::NUMBER, "5");
//     expectToken(Type::RPAREN);
//     expectToken(Type::LBRACKET);
    
//     expectToken(Type::IDENTIFIER, "c");
//     expectToken(Type::ASSIGN);
//     expectToken(Type::NUMBER, "15");
//     expectToken(Type::SEMI);
//     expectToken(Type::RBRACKET);
    
//     expectToken(Type::IDENTIFIER, "a");
//     expectToken(Type::ASSIGN);
//     expectToken(Type::IDENTIFIER, "a");
//     expectToken(Type::PLUS);
//     expectToken(Type::NUMBER, "1");
//     expectToken(Type::SEMI);
//     expectToken(Type::RBRACKET);
    
//     expectEnd();
// }

// TEST_F(LexerTest, EmptyInput) {
//     tokenize("");
//     expectToken(Type::EOFTOKEN);
//     expectEnd();
// }

// TEST_F(LexerTest, KeywordsAndIdentifiers) {
//     tokenize("if ifvar while whilevar");
    
//     expectToken(Type::IF);             
//     expectToken(Type::IDENTIFIER, "ifvar");  
//     expectToken(Type::WHILE);            
//     expectToken(Type::IDENTIFIER, "whilevar");  
//     expectEnd();
// }


// class ParserTest : public ::testing::Test {
// protected:
//     std::unique_ptr<Lexer> lexer;
//     std::unique_ptr<Parser> parser;
//     node_ptr ast;
    
//     void parse(const std::string& input) {
//         lexer = std::make_unique<Lexer>(input);
//         lexer->tokenize();
//         parser = std::make_unique<Parser>();
//         auto begin = lexer->begin();
//         auto end = lexer->end();
//         ast = parser->parse(begin, end);
//     }

//     bool isSimpleNode(node_ptr node, Type expectedType, const std::string& expectedValue) {
//         return simple->getType() == expectedType && simple->getValue() == expectedValue;
//     }
    
//     bool isBinaryNode(ASTNode* node, Type expectedOp) {
//         auto binary = dynamic_cast<BinaryNode*>(node);
//         return binary && binary->getType() == expectedOp;
//     }
    
//     bool isInOutNode(ASTNode* node, Type expectedType) {
//         auto inout = dynamic_cast<InOutStmt*>(node);
//         return inout && inout->getType() == expectedType;
//     }
    
//     bool isConditionNode(ASTNode* node, Type expectedType) {
//         auto cond = dynamic_cast<ConditionStmt*>(node);
//         return cond && cond->getType() == expectedType;
//     }
// };



// TEST_F(ParserTest, Assignment) {
//     parse("x = 42;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
    
//     ASSERT_TRUE(isBinaryNode(assign, Type::ASSIGN));
//     EXPECT_TRUE(isSimpleNode(assign->getLeft().get(), Type::IDENTIFIER, "x"));
//     EXPECT_TRUE(isSimpleNode(assign->getRight().get(), Type::NUMBER, "42"));
// }

// TEST_F(ParserTest, Addition) {
//     parse("x = 2 + 3;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     auto add = dynamic_cast<BinaryNode*>(assign->getRight().get());
    
//     ASSERT_TRUE(isBinaryNode(add, Type::PLUS));
//     EXPECT_TRUE(isSimpleNode(add->getLeft().get(), Type::NUMBER, "2"));
//     EXPECT_TRUE(isSimpleNode(add->getRight().get(), Type::NUMBER, "3"));
// }

// TEST_F(ParserTest, MultiplicationPrecedence) {
//     parse("x = 2 + 3 * 4;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     auto add = dynamic_cast<BinaryNode*>(assign->getRight().get());
    
//     ASSERT_TRUE(isBinaryNode(add, Type::PLUS));
//     EXPECT_TRUE(isSimpleNode(add->getLeft().get(), Type::NUMBER, "2"));
    
//     auto mult = dynamic_cast<BinaryNode*>(add->getRight().get());
//     ASSERT_TRUE(isBinaryNode(mult, Type::MULTIPLY));
//     EXPECT_TRUE(isSimpleNode(mult->getLeft().get(), Type::NUMBER, "3"));
//     EXPECT_TRUE(isSimpleNode(mult->getRight().get(), Type::NUMBER, "4"));
// }

// TEST_F(ParserTest, Parentheses) {
//     parse("x = (2 + 3) * 4;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     auto mult = dynamic_cast<BinaryNode*>(assign->getRight().get());
    
//     ASSERT_TRUE(isBinaryNode(mult, Type::MULTIPLY));
    
//     auto add = dynamic_cast<BinaryNode*>(mult->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(add, Type::PLUS));
//     EXPECT_TRUE(isSimpleNode(add->getLeft().get(), Type::NUMBER, "2"));
//     EXPECT_TRUE(isSimpleNode(add->getRight().get(), Type::NUMBER, "3"));
//     EXPECT_TRUE(isSimpleNode(mult->getRight().get(), Type::NUMBER, "4"));
// }

// TEST_F(ParserTest, LeftAssociativity) {
//     parse("x = 10 - 5 - 2;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     auto sub1 = dynamic_cast<BinaryNode*>(assign->getRight().get());
    
//     ASSERT_TRUE(isBinaryNode(sub1, Type::MINUS));
    
//     auto sub2 = dynamic_cast<BinaryNode*>(sub1->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(sub2, Type::MINUS));
//     EXPECT_TRUE(isSimpleNode(sub2->getLeft().get(), Type::NUMBER, "10"));
//     EXPECT_TRUE(isSimpleNode(sub2->getRight().get(), Type::NUMBER, "5"));
//     EXPECT_TRUE(isSimpleNode(sub1->getRight().get(), Type::NUMBER, "2"));
// }

// TEST_F(ParserTest, ComplicatedStmt) {
//     parse("x = 10 * (5 - a) + 3 * (2 - 1 * 8);");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     auto add = dynamic_cast<BinaryNode*>(assign->getRight().get());

//     ASSERT_TRUE(isBinaryNode(add, Type::PLUS));

//     auto sub1 = dynamic_cast<BinaryNode*>(add->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(sub1, Type::MULTIPLY));
//     EXPECT_TRUE(isSimpleNode(sub1->getLeft().get(), Type::NUMBER, "10"));
//     auto sub11 = dynamic_cast<BinaryNode*>(sub1->getRight().get());

//     ASSERT_TRUE(isBinaryNode(sub11, Type::MINUS));
//     EXPECT_TRUE(isSimpleNode(sub11->getLeft().get(), Type::NUMBER, "5"));
//     EXPECT_TRUE(isSimpleNode(sub11->getRight().get(), Type::IDENTIFIER, "a"));

//     auto sub2 = dynamic_cast<BinaryNode*>(add->getRight().get());
//     ASSERT_TRUE(isBinaryNode(sub2, Type::MULTIPLY));
//     EXPECT_TRUE(isSimpleNode(sub2->getLeft().get(), Type::NUMBER, "3"));
//     auto sub21 = dynamic_cast<BinaryNode*>(sub2->getRight().get());
    
//     ASSERT_TRUE(isBinaryNode(sub21, Type::MINUS));
//     EXPECT_TRUE(isSimpleNode(sub21->getLeft().get(), Type::NUMBER, "2"));

//     auto sub211 = dynamic_cast<BinaryNode*>(sub21->getRight().get());
//     ASSERT_TRUE(isBinaryNode(sub211, Type::MULTIPLY));
//     EXPECT_TRUE(isSimpleNode(sub211->getLeft().get(), Type::NUMBER, "1"));
//     EXPECT_TRUE(isSimpleNode(sub211->getRight().get(), Type::NUMBER, "8"));
// }


// TEST_F(ParserTest, IfStatement) {
//     parse("if (x < 10) { y = 5; }");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto ifStmt = dynamic_cast<ConditionStmt*>(block->getBody()[0].get());
    
//     ASSERT_TRUE(isConditionNode(ifStmt, Type::IF));
//     ASSERT_EQ(ifStmt->getBody().size(), 1);
    
//     auto assign = dynamic_cast<BinaryNode*>(ifStmt->getBody()[0].get());
//     EXPECT_TRUE(isBinaryNode(assign, Type::ASSIGN));
// }

// TEST_F(ParserTest, WhileLoop) {
//     parse("while (i < 10) { i = i + 1; }");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto whileStmt = dynamic_cast<ConditionStmt*>(block->getBody()[0].get());
    
//     ASSERT_TRUE(isConditionNode(whileStmt, Type::WHILE));
//     ASSERT_EQ(whileStmt->getBody().size(), 1);
    
//     auto assign = dynamic_cast<BinaryNode*>(whileStmt->getBody()[0].get());
//     EXPECT_TRUE(isBinaryNode(assign, Type::ASSIGN));
// }

// TEST_F(ParserTest, NestedIf) {
//     parse("if (a < 5) { if (b < 3) { c = 10; } }");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto outerIf = dynamic_cast<ConditionStmt*>(block->getBody()[0].get());
    
//     ASSERT_TRUE(isConditionNode(outerIf, Type::IF));
//     ASSERT_EQ(outerIf->getBody().size(), 1);
    
//     auto innerIf = dynamic_cast<ConditionStmt*>(outerIf->getBody()[0].get());
//     EXPECT_TRUE(isConditionNode(innerIf, Type::IF));
// }

// TEST_F(ParserTest, MultipleStatements) {
//     parse("a = 5; b = 10; c = a + b;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     ASSERT_EQ(block->getBody().size(), 3);
// }

// TEST_F(ParserTest, FullProgram) {
//     parse(R"(
//         i = 0;
//         while (i < 10) {
//             print(i);
//             i = i + 1;
//         }
//         print(i);
//     )");
    
//     auto block = dynamic_cast<Block*>(ast.get());
//     ASSERT_EQ(block->getBody().size(), 3);
    
//     auto assign1 = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     EXPECT_TRUE(isBinaryNode(assign1, Type::ASSIGN));
    
//     auto whileStmt = dynamic_cast<ConditionStmt*>(block->getBody()[1].get());
//     EXPECT_TRUE(isConditionNode(whileStmt, Type::WHILE));
    
//     auto print2 = dynamic_cast<InOutStmt*>(block->getBody()[2].get());
//     EXPECT_TRUE(isInOutNode(print2, Type::PRINT));
// }

// TEST_F(ParserTest, ThrowSemi) {
//     try {
//         parse("a = 4");
//         FAIL() << "Expected ParseError";
//     }
//     catch(const СompilerError& e) {
//         EXPECT_STREQ(e.what(), "Expected ';' on line 1");
//     }
// }

// TEST_F(ParserTest, UnclosedParen) {
//     try {
//          parse(R"(
//             i = 0;
//             while (i < 10 {
//                 print(i);
//                 i = i + 1;
//             }
//             print(i);
//         )");
//         FAIL() << "Expected ParseError";
//     }
//     catch(const СompilerError& e) {
//         EXPECT_STREQ(e.what(), "Expected ')' on line 3");
//     }
// }

// TEST_F(ParserTest, EmptyProgram) {
//     try {
//         parse("     ");
//         FAIL() << "Expected ParseError";
//     }
//     catch(const СompilerError& e) {
//         EXPECT_STREQ(e.what(), "Empty program");
//     }
// }

// TEST_F(ParserTest, MoreParentheses) {
//     parse("x = (1 + 2) * (3 - 4) / (5 + 6);");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     ASSERT_TRUE(isBinaryNode(assign, Type::ASSIGN));
    
//     auto div = dynamic_cast<BinaryNode*>(assign->getRight().get());
//     ASSERT_TRUE(isBinaryNode(div, Type::DIVIDE));
    
//     auto mult = dynamic_cast<BinaryNode*>(div->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(mult, Type::MULTIPLY));
    
//     auto add1 = dynamic_cast<BinaryNode*>(mult->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(add1, Type::PLUS));
//     auto sub = dynamic_cast<BinaryNode*>(mult->getRight().get());
//     ASSERT_TRUE(isBinaryNode(sub, Type::MINUS));
//     auto add2 = dynamic_cast<BinaryNode*>(div->getRight().get());
//     ASSERT_TRUE(isBinaryNode(add2, Type::PLUS));
// }


// TEST_F(ParserTest, AllOperators) {
//     parse("x = 1 + 2 - 3 * 4 / 5;");
//     auto block = dynamic_cast<Block*>(ast.get());
//     auto assign = dynamic_cast<BinaryNode*>(block->getBody()[0].get());
//     ASSERT_TRUE(isBinaryNode(assign, Type::ASSIGN));

//     EXPECT_TRUE(isSimpleNode(assign->getLeft().get(), Type::IDENTIFIER, "x"));
    
//     auto firstMinus = dynamic_cast<BinaryNode*>(assign->getRight().get());
//     ASSERT_TRUE(isBinaryNode(firstMinus, Type::MINUS));
    
//     auto plus = dynamic_cast<BinaryNode*>(firstMinus->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(plus, Type::PLUS));
//     EXPECT_TRUE(isSimpleNode(plus->getLeft().get(), Type::NUMBER, "1"));
//     EXPECT_TRUE(isSimpleNode(plus->getRight().get(), Type::NUMBER, "2"));
    
//     auto division = dynamic_cast<BinaryNode*>(firstMinus->getRight().get());
//     ASSERT_TRUE(isBinaryNode(division, Type::DIVIDE));

//     auto multiply = dynamic_cast<BinaryNode*>(division->getLeft().get());
//     ASSERT_TRUE(isBinaryNode(multiply, Type::MULTIPLY));
//     EXPECT_TRUE(isSimpleNode(multiply->getLeft().get(), Type::NUMBER, "3"));
//     EXPECT_TRUE(isSimpleNode(multiply->getRight().get(), Type::NUMBER, "4"));

//     EXPECT_TRUE(isSimpleNode(division->getRight().get(), Type::NUMBER, "5"));
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }