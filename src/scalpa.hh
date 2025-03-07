#ifndef SCALCC_PARSER_H
#define SCALCC_PARSER_H

#include "scalex.hh"
#include <memory>
#include <vector>

// Forward declaration of AST nodes
class ASTNode;
class ExprNode;
class StmtNode;
class ProgramNode;

// Parser class
class Parser {
public:
    explicit Parser(Lexer& lexer);

    // Parse the entire program and return the root AST node
    std::unique_ptr<ProgramNode> parse();

private:
    Lexer& lexer;               // Reference to the lexer
    Token currentToken;         // Current token being processed

    // Helper functions
    void eat(TokenType type);   // Consume a token of the expected type
    Token peekNextToken();      // Peek at the next token without consuming it

    // Parsing functions for different constructs
    std::unique_ptr<ExprNode> parseExpression();
    std::unique_ptr<StmtNode> parseStatement();
    std::unique_ptr<StmtNode> parseFunctionDefinition();
    std::unique_ptr<StmtNode> parseVariableDeclaration();
    std::unique_ptr<StmtNode> parseControlStructure();
};

#endif // SCALCC_PARSER_H