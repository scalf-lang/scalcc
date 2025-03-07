#include "scalpa.h"
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
            program->addStatement(parseFunctionDefinition());
        } else {
            throw std::runtime_error("Unexpected token: " + currentToken.value);
        }
    }

    return program;
}

// Helper: Consume a token of the expected type
void Parser::eat(TokenType type) {
    if (currentToken.type == type) {
        currentToken = lexer.getNextToken();
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value);
    }
}

// Helper: Peek at the next token
Token Parser::peekNextToken() {
    Lexer tempLexer = lexer;
    return tempLexer.getNextToken();
}

// Parse a function definition
std::unique_ptr<StmtNode> Parser::parseFunctionDefinition() {
    eat(TokenType::TOKEN_KEYWORD); // func

    std::string funcName = currentToken.value;
    eat(TokenType::TOKEN_IDENTIFIER);

    eat(TokenType::TOKEN_SYMBOL); // (
    // Parse parameters (if any)
    eat(TokenType::TOKEN_SYMBOL); // )

    eat(TokenType::TOKEN_OPERATOR); // ->
    std::string returnType = currentToken.value;
    eat(TokenType::TOKEN_IDENTIFIER);

    eat(TokenType::TOKEN_SYMBOL); // {

    // Parse function body
    while (currentToken.type != TokenType::TOKEN_SYMBOL || currentToken.value != "}") {
        // Parse statements
    }

    eat(TokenType::TOKEN_SYMBOL); // }

    return std::make_unique<FunctionDefinitionNode>(funcName, returnType);
}