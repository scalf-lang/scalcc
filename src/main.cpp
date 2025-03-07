#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include "scalex.hh"
#include "scalpa.hh"
#include "scalmantic.hh"
#include "scalgen.hh"
#include "llvm/Support/raw_ostream.h"

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
        std::cerr << "Usage: scalcc <input-file.scl> [-o <output-file>]" << std::endl;
        return 1;
    }

    // Read the input SCALF file
    std::string inputFile = argv[1];
    std::string sourceCode = readFile(inputFile);

    // Initialize the lexer and parser
    Lexer lexer(sourceCode);
    Parser parser(lexer);

    // Parse the input file into an AST
    std::unique_ptr<ASTNode> ast = parser.parse();

    // Perform semantic analysis
    SemanticAnalyzer sema;
    sema.analyze(ast.get());

    // Generate LLVM IR
    CodeGenerator codegen;
    codegen.generate(ast.get());

    // Output the generated IR
    std::string outputFile = "output.ll";
    if (argc > 2 && std::string(argv[2]) == "-o") {
        outputFile = argv[3];
    }

    std::error_code EC;
    llvm::raw_fd_ostream outLLVM(outputFile, EC);
    if (EC) {
        std::cerr << "Error: Could not open output file " << outputFile << std::endl;
        return 1;
    }

    codegen.getModule().print(outLLVM, nullptr);
    std::cout << "Compilation successful. Output written to " << outputFile << std::endl;

    return 0;
}