#include "lexer.h"
#include <cctype>
#include <utility>

// Constructor
Lexer::Lexer(const std::string& source)
    : source(source), position(0), line(1), column(1) {}

// Get the next token
Token Lexer::getNextToken() {
    skipWhitespace();
    skipComment();

    if (position >= source.length()) {
        return {TokenType::TOKEN_EOF, "", line, column};
    }

    char currentChar = peek();

    // Handle identifiers and keywords
    if (isalpha(currentChar)) {
        return readIdentifier();
    }

    // Handle numbers
    if (isdigit(currentChar)) {
        return readNumber();
    }

    // Handle strings
    if (currentChar == '"') {
        return readString();
    }

    // Handle operators and symbols
    if (ispunct(currentChar)) {
        return readOperator();
    }

    // Unknown token
    advance();
    return {TokenType::TOKEN_UNKNOWN, std::string(1, currentChar), line, column};
}

// Get the current position in the source code
std::pair<int, int> Lexer::getPosition() const {
    return {line, column};
}

// Peek at the current character without advancing
char Lexer::peek() const {
    return position < source.length() ? source[position] : '\0';
}

// Advance to the next character
char Lexer::advance() {
    char currentChar = peek();
    if (currentChar == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
    return currentChar;
}

// Skip whitespace characters
void Lexer::skipWhitespace() {
    while (isspace(peek())) {
        advance();
    }
}

// Skip comments
void Lexer::skipComment() {
    if (peek() == '/' && position + 1 < source.length() && source[position + 1] == '/') {
        // Skip single-line comment
        while (peek() != '\n' && peek() != '\0') {
            advance();
        }
    }
}

// Read an identifier or keyword
Token Lexer::readIdentifier() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    while (isalnum(peek()) || peek() == '_') {
        value += advance();
    }

    // Check if the identifier is a keyword
    if (value == "func" || value == "class" || value == "glob" || value == "include") {
        return {TokenType::TOKEN_KEYWORD, value, startLine, startColumn};
    }

    return {TokenType::TOKEN_IDENTIFIER, value, startLine, startColumn};
}

// Read a number
Token Lexer::readNumber() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    while (isdigit(peek())) {
        value += advance();
    }

    return {TokenType::TOKEN_NUMBER, value, startLine, startColumn};
}

// Read a string
Token Lexer::readString() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    advance(); // Skip the opening quote
    while (peek() != '"' && peek() != '\0') {
        value += advance();
    }
    advance(); // Skip the closing quote

    return {TokenType::TOKEN_STRING, value, startLine, startColumn};
}

// Read an operator or symbol
Token Lexer::readOperator() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    // Handle multi-character operators (e.g., >>, ::)
    value += advance();
    if (peek() == '>' || peek() == ':') {
        value += advance();
    }

    return {TokenType::TOKEN_OPERATOR, value, startLine, startColumn};
}