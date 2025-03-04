#include "parser.h"
#include <stdexcept>

// Constructor
Parser::Parser(Lexer& lexer)
    : lexer(lexer) {
    currentToken = lexer.getNextToken();
}

// Parse the entire program
std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (currentToken.type != TokenType::TOKEN_EOF) {
        if (currentToken.type == TokenType::TOKEN_KEYWORD && currentToken.value == "func") {
            program->statements.push_back(parseFunctionDefinition());
        } else {
            throw std::runtime_error("Unexpected token: " + currentToken.value);
        }
    }

    return program;
}

// Consume a token of the expected type
void Parser::eat(TokenType type) {
    if (currentToken.type == type) {
        currentToken = lexer.getNextToken();
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value);
    }
}

// Peek at the next token without consuming it
Token Parser::peekNextToken() {
    Lexer tempLexer = lexer; // Copy the lexer to avoid modifying the original
    return tempLexer.getNextToken();
}

// Parse a function definition
std::unique_ptr<StmtNode> Parser::parseFunctionDefinition() {
    eat(TokenType::TOKEN_KEYWORD); // func
    eat(TokenType::TOKEN_IDENTIFIER); // Function name
    eat(TokenType::TOKEN_SYMBOL); // (
    // Parse parameters
    eat(TokenType::TOKEN_SYMBOL); // )
    eat(TokenType::TOKEN_OPERATOR); // ->
    eat(TokenType::TOKEN_IDENTIFIER); // Return type
    eat(TokenType::TOKEN_SYMBOL); // {
    // Parse function body
    eat(TokenType::TOKEN_SYMBOL); // }

    return std::make_unique<StmtNode>(); // Placeholder
}