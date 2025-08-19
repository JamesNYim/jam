#include "parser.h"

Parser* createParser (const TokenArray* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    if (!parser) {
        fprintf(stderr, "\n! Unable to create Parser !\n");
        return NULL;
    }

    if (!tokens) {
        fprintf(stderr, "\n! Null token array passed to createParser !\n");
        // Free parser
        return NULL;
    }

    parser->index = 0;
    parser->tokens = tokens;

    return parser;
}

void freeParser(Parser* parser) {
    if (!parser) {
        return;
    }
    free(parser);
}

// Looking ahead to get a token 
// private
static Token* peek(Parser* parser, int offset) {
    
    int aheadIndex = parser->index + offset;
    // If we attempt to access out of bounds
    if (aheadIndex >= parser->tokens->count) {
        return NULL;
    }
    
    return &parser->tokens->tokData[aheadIndex];
}

// Consuming a token
// private
static Token* consume(Parser* parser) {
    // If we attempt to access out of bounds

    if (parser->index >= parser->tokens->count) {
        return NULL;
    }
    
    return &parser->tokens->tokData[parser->index++];
}

// Parsing an expression
// private
NodeExpression* parseExpression(Parser* parser) {
    Token* token = peek(parser, 0);
    if (token != NULL && token->type == int_lit) {
        NodeExpression* expr = malloc(sizeof(NodeExpression));
        if (!expr) {
            fprintf(stderr, "\n! Failed to allocate NodeExpression !\n");
            return NULL;
        }
        expr->int_lit = *consume(parser);
        return expr;
    }
    else {
        fprintf(stderr, "\n! (parseExpr) Invalid Expression !\n");

        printf("At token: %u\n", peek(parser, 0)->type);
        consume(parser);
        return NULL;
    }
}

NodeExit parse(Parser* parser) {
    NodeExit exitNode;
    while (peek(parser, 0) != NULL) {

        Token* token = peek(parser, 0);
        // If we have a finish token
        if (token->type == _return) {
            consume(parser);
            NodeExpression* expression = parseExpression(parser);
            if (expression != NULL) {
                exitNode.expr = expression;
            }
            else {
                fprintf(stderr, "\n! (parse) Invalid Expression !\n");
                consume(parser);
                // what to return here?
            }
        }
        // Checking for a semi
        if (token->type == semi) {
            consume(parser);
        }
    }
    return exitNode;
}
