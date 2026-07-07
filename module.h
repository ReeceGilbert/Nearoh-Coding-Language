#ifndef MODULE_H
#define MODULE_H

#include "ast.h"
#include "lexer.h"

typedef struct ImportedModule ImportedModule;

struct ImportedModule {
    char* path;
    int loaded;
    int loading;

    char* source;
    TokenArray* tokens;
    Diagnostics* diagnostics;
    AstNode* ast;
};

#endif