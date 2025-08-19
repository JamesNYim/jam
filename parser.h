// Header file for Parser

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "token.h"

// -= Nodes =- //
typedef struct {
    Token int_lit;
} NodeExpression;

typedef struct {
    NodeExpression* expr;
} NodeExit;

typedef struct {
    const TokenArray* tokens;
    size_t index;
} Parser;

Parser* createParser(const TokenArray* tokens);

void freeParser(Parser* parser);

NodeExit parse(Parser* parser);
