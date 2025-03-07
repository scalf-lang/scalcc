#include "scalpreter.h"
#include "scalpa/scalpa.h"
#include <iostream>

// Interpret the AST
void Scalpreter::interpret(ASTNode* node) {
    if (auto program = dynamic_cast<ProgramNode*>(node)) {
        for (const auto& stmt : program->getStatements()) {
            interpret(stmt.get());
        }
    } else if (auto funcDef = dynamic_cast<FunctionDefinitionNode*>(node)) {
        // Register the function in the environment
        environment[funcDef->getName()] = funcDef;
    } else if (auto expr = dynamic_cast<ExprNode*>(node)) {
        // Evaluate expressions
        evaluate(expr);
    } else if (auto stmt = dynamic_cast<StmtNode*>(node)) {
        // Execute statements
        execute(stmt);
    } else {
        throw std::runtime_error("Unknown AST node type");
    }
}

// Evaluate an expression
void Scalpreter::evaluate(ExprNode* expr) {
    if (auto intLit = dynamic_cast<IntLiteralNode*>(expr)) {
        // Handle integer literals
        std::cout << "Evaluated integer: " << intLit->getValue() << std::endl;
    } else if (auto strLit = dynamic_cast<StringLiteralNode*>(expr)) {
        // Handle string literals
        std::cout << "Evaluated string: " << strLit->getValue() << std::endl;
    } else {
        throw std::runtime_error("Unknown expression type");
    }
}

// Execute a statement
void Scalpreter::execute(StmtNode* stmt) {
    if (auto funcCall = dynamic_cast<FunctionCallNode*>(stmt)) {
        // Handle function calls
        std::cout << "Calling function: " << funcCall->getName() << std::endl;
        if (environment.find(funcCall->getName()) != environment.end()) {
            interpret(environment[funcCall->getName()]);
        } else {
            throw std::runtime_error("Function not found: " + funcCall->getName());
        }
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}