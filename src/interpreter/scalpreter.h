#ifndef SCALPTER_H
#define SCALPTER_H

#include "scalpa/scalpa.h"
#include <unordered_map>
#include <memory>

// Interpreter class
class Scalpreter {
public:
    // Interpret the AST
    void interpret(ASTNode* node);

private:
    // Environment to store functions and variables
    std::unordered_map<std::string, std::shared_ptr<ASTNode>> environment;

    // Evaluate an expression
    void evaluate(ExprNode* expr);

    // Execute a statement
    void execute(StmtNode* stmt);
};

#endif // SCALPTER_H