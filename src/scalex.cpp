#include "scalex.hh"
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
        return Token(TokenType::TOKEN_EOF, "", line, column);
    }

    char currentChar = peek();

    // Handle identifiers and keywords
    if (isalpha(currentChar) {
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
    return Token(TokenType::TOKEN_UNKNOWN, std::string(1, currentChar), line, column);
}

// Helper: Peek at the current character
char Lexer::peek() const {
    return source[position];
}

// Helper: Advance to the next character
char Lexer::advance() {
    char currentChar = source[position++];
    if (currentChar == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return currentChar;
}

// Helper: Skip whitespace
void Lexer::skipWhitespace() {
    while (position < source.length() && isspace(peek())) {
        advance();
    }
}

// Helper: Skip comments
void Lexer::skipComment() {
    if (peek() == '?') {
        if (position + 1 < source.length() && source[position + 1] == '/') {
            // Multiline comment
            while (position < source.length() && !(peek() == '/' && source[position + 1] == '?')) {
                advance();
            }
            advance(); // Skip '/'
            advance(); // Skip '?'
        } else {
            // Single-line comment
            while (position < source.length() && peek() != '\n') {
                advance();
            }
        }
    }
}

// Helper: Read an identifier or keyword
Token Lexer::readIdentifier() {
    std::string value;
    while (position < source.length() && (isalnum(peek()) || peek() == '_')) {
        value += advance();
    }

    // Check if it's a keyword
    if (value == "func" || value == "class" || value == "struct" || value == "glob" || value == "include") {
        return Token(TokenType::TOKEN_KEYWORD, value, line, column);
    }

    return Token(TokenType::TOKEN_IDENTIFIER, value, line, column);
}

// Helper: Read a number
Token Lexer::readNumber() {
    std::string value;
    while (position < source.length() && isdigit(peek())) {
        value += advance();
    }
    return Token(TokenType::TOKEN_NUMBER, value, line, column);
}

// Helper: Read a string
Token Lexer::readString() {
    std::string value;
    advance(); // Skip opening quote
    while (position < source.length() && peek() != '"') {
        value += advance();
    }
    advance(); // Skip closing quote
    return Token(TokenType::TOKEN_STRING, value, line, column);
}

// Helper: Read an operator or symbol
Token Lexer::readOperator() {
    std::string value;
    value += advance();

    // Handle multi-character operators (e.g., >>, ::)
    if (position < source.length() && ispunct(peek())) {
        value += advance();
    }

    return Token(TokenType::TOKEN_OPERATOR, value, line, column);
}

// Get the current position in the source code
std::pair<int, int> Lexer::getPosition() const {
    return {line, column};
}