#include "parser.hpp"
#include "visitor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.spl> <output.asm>" << std::endl;
        return 1;
    }
    
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string source_code = buffer.str();
    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(source_code);
    lexer->tokenize();
    std::unique_ptr<Parser> parser = std::make_unique<Parser>();
    auto begin = lexer->begin();
    auto end = lexer->end();
    node_ptr ast = parser->parse(begin, end);
    
    StackVisitor visitor;
    std::visit(visitor, *ast);
    std::ofstream output_file(argv[2]);
    output_file << "entry:" << std::endl;
    output_file << visitor.getCode();
    output_file << "    HALT";
    return 0;
}