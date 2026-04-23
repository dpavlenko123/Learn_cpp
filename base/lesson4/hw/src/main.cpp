#include "parser.hpp"
#include "visitor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "lexer.hpp"
#include "stack_machine.cpp"

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
    Lexer lexer = Lexer(source_code);
    lexer.tokenize();
    Parser parser = Parser();
    auto begin = lexer.begin();
    auto end = lexer.end();
    node_ptr ast = parser.parse(begin, end);
    
    StackVisitor visitor;
    std::visit(visitor, *ast);
    std::stringstream operations;
    operations << "entry:" << std::endl;
    operations << visitor.getCode();
    operations << "\tHALT";
    std::ofstream output_file(argv[2]);
    StackMachine sm(output_file);
    sm.load(operations.str());
    sm.run();
    return 0;
}