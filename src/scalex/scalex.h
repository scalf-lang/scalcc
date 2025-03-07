#ifndef SCALEX_H
#define SCALEX_H

#include <string>
#include <vector>

// Token types for SCALF
enum class TokenType {
    TOKEN_EOF,          // End of file
    TOKEN_IDENTIFIER,   // Variable or function name
    TOKEN_NUMBER,       // Numeric literal
    TOKEN_STRING,       // String literal
    TOKEN_OPERATOR,     // Operators like >>, ::, etc.
    TOKEN_KEYWORD,      // Keywords like func, class, etc.
    TOKEN_SYMBOL,       // Symbols like {, }, (, ), etc.
    TOKEN_COMMENT,      // Comments (single-line or multi-line)
    TOKEN_UNKNOWN       // Unknown token
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType type, std::string value, int line, int column)
        : type(type), value(std::move(value)), line(line), column(column) {}
};

// Lexer class
class Scalex {
public:
    explicit Scalex(const std::string& source);

    // Get the next token
    Token getNextToken();

    // Get the current position in the source code
    std::pair<int, int> getPosition() const;

private:
    std::string source;  // Input source code
    size_t position;     // Current position in the source
    int line;            // Current line number
    int column;          // Current column number

    // Helper functions
    char peek() const;
    char advance();
    void skipWhitespace();
    void skipComment();
    Token readIdentifier();
    Token readNumber();
    Token readString();
    Token readOperator();
};

#endif // SCALEX_H