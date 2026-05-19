#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "semantic.h"

#define MAX_SCOPE_DEPTH 100
#define MAX_SYMBOLS 1000

// --- Symbol Table Structure ---

typedef struct {
    char name[100];
    int scopeDepth;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int currentScopeDepth = 0;

// --- Symbol Table Management ---

void enterScope() {
    currentScopeDepth++;
}

void exitScope() {
    // Remove all symbols from the current scope
    for (int i = symbolCount - 1; i >= 0; i--) {
        if (symbolTable[i].scopeDepth == currentScopeDepth) {
            symbolCount--;
        } else {
            break;
        }
    }
    currentScopeDepth--;
}

int isDeclaredInCurrentScope(const char *name) {
    for (int i = symbolCount - 1; i >= 0; i--) {
        if (strcmp(symbolTable[i].name, name) == 0 &&
            symbolTable[i].scopeDepth == currentScopeDepth) {
            return 1;
        }
    }
    return 0;
}

int isDeclaredInAnyScope(const char *name) {
    for (int i = symbolCount - 1; i >= 0; i--) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

void declareSymbol(const char *name) {
    if (isDeclaredInCurrentScope(name)) {
        fprintf(stderr, "Semantic error: Redeclaration of variable '%s'\n", name);
        exit(1);
    }
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].scopeDepth = currentScopeDepth;
    symbolCount++;
}

void useSymbol(const char *name) {
    if (!isDeclaredInAnyScope(name)) {
        fprintf(stderr, "Semantic error: Use of undeclared variable '%s'\n", name);
        exit(1);
    }
}

// --- AST Traversal for Semantic Checks ---

void analyzeAST(ASTNode *node);

void analyzeExpression(ASTNode *node) {
    if (!node) return;

    if (strcmp(node->name, "=") == 0) {
        // LHS is variable, RHS is expression
        if (node->condition) {
            useSymbol(node->condition->name); // check variable is declared
        }
        analyzeExpression(node->body); // check RHS
    } else if (strcmp(node->name, "declare") == 0) {
        // variable declaration
        if (node->condition) {
            declareSymbol(node->condition->name);
        }
        if (node->body) {
            analyzeExpression(node->body);
        }
    } else if (strcmp(node->name, "return") == 0) {
        analyzeExpression(node->body);
    } else {
        // Binary operations or variable usage
        if (!node->condition && !node->body) {
            // variable or constant
            if (node->name[0] >= 'a' && node->name[0] <= 'z') {
                useSymbol(node->name);  // likely a variable
            }
        } else {
            analyzeExpression(node->condition);
            analyzeExpression(node->body);
        }
    }
}

void analyzeAST(ASTNode *node) {
    while (node) {
        switch (node->type) {
            case AST_FUNCTION:
                enterScope();
                analyzeAST(node->body);
                exitScope();
                break;

            case AST_BLOCK:
                enterScope();
                analyzeAST(node->body);
                exitScope();
                break;

            case AST_IF:
                analyzeExpression(node->condition);
                analyzeAST(node->body);
                if (node->elseBody) analyzeAST(node->elseBody);
                break;

            case AST_WHILE:
                analyzeExpression(node->condition);
                analyzeAST(node->body);
                break;

            case AST_EXPRESSION:
                analyzeExpression(node);
                break;
        }
        node = node->next;
    }
}
