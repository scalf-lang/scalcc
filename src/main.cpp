#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "scalex/scalex.h"
#include "scalpa/scalpa.h"
#include "interpreter/scalpreter.h"

// Function to read a file into a string
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    // Check command-line arguments
    if (argc < 2) {
        std::cerr << "Usage: scalcc <input-file.scl>" << std::endl;
        return 1;
    }

    // Read the input SCALF file
    std::string inputFile = argv[1];
    std::string sourceCode = readFile(inputFile);

    // Tokenize the input
    Scalex lexer(sourceCode);

    // Parse the tokens into an AST
    Scalpa parser(lexer);
    std::unique_ptr<ProgramNode> ast = parser.parse();

    // Interpret the AST
    Scalpreter interpreter;
    interpreter.interpret(ast.get());

    return 0;
}