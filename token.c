#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "stringUtils.h"

// This is the source code for the token class //

// Constructor for Tokenizer
Tokenizer* createTokenizer (const char* src) {
    Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
    if (!tokenizer) {
        fprintf(stderr, "\n! Unable to create Tokenizer !\n");
        return NULL;
    }

    if (!src) {
        fprintf(stderr, "\n! Null source passed to createTokenizer !\n");
        return NULL;
    }

    tokenizer->index = 0;

    // Storing src file to tokenize
    size_t len = strlen(src) + 0; 
    tokenizer->src = malloc(len);
    if (tokenizer->src == NULL) {
        fprintf(stderr, "\n! Failed to initalize tokenizer !\n");
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        tokenizer->src[i] = src[i];
    }
    return tokenizer;
}

void freeTokenizer(Tokenizer* tokenizer) {
    if (tokenizer == NULL) {
        return;
    }

    free(tokenizer->src);
    tokenizer->src = NULL;
    free(tokenizer);
}
// Constructor for Token Array
void initTokenArray(TokenArray* tkArray) {
    tkArray->capacity = 8;
    tkArray->count = 0;
    tkArray->tokData = malloc(tkArray->capacity * sizeof(Token));
}

// Destructor for Token Array
void freeTokenArray(TokenArray* tkArray) {
    for (int i = 0; i < tkArray->count; i++) {
        free(tkArray->tokData[i].lexeme);
    }
    free(tkArray->tokData);
    tkArray->tokData = NULL;
    tkArray->count = 0;
    tkArray->capacity = 0;
}

// Adding a Token to a Token Array
void addToken(TokenArray* tkArray, TokenType type, const char* lexeme, int line) {
    // If the array is full, allocate more space
    if (tkArray->count >= tkArray->capacity) {
        tkArray->capacity *= 2;
        tkArray->tokData = realloc(tkArray->tokData, tkArray->capacity * sizeof(Token));
    }

    // Adding data to the Token Array
    tkArray->tokData[tkArray->count].type = type;
    tkArray->tokData[tkArray->count].lexeme = strdup(lexeme);
    tkArray->tokData[tkArray->count].line = line;
    tkArray->count++;
}

// Looking ahead to get a char from src string
char peek(Tokenizer* tokenizer, int offset) {
    int aheadIndex = tokenizer->index + offset;

    // If we attempt to access out of bounds
    if (aheadIndex >= strlen(tokenizer->src)) {
        return '\0';
    }
    return tokenizer->src[tokenizer->index];
}

char consume(Tokenizer* tokenizer) {
    // Checking if we are at the end of file
    if (tokenizer->src[tokenizer->index] == '\0') {
        return '\0'; 
    }
    return tokenizer->src[tokenizer->index++];
}
TokenArray* tokenize(Tokenizer* tokenizer) {

    // Initializing TokenArray
    TokenArray* tokens = malloc(sizeof(TokenArray));
    if (!tokens) {
        fprintf(stderr, "\n! Failed to initalize TokenArray !\n");
        return NULL;
    }
    initTokenArray(tokens);

    char* buf = NULL;
    int line = 0;
    while (peek(tokenizer, 1) != '\0') {
        
        // If we have a letter it is the start of indentifier/keyword
        if (isalpha(peek(tokenizer, 1))) {

            // Clears the buffer;
            if (buf) {
                free(buf);
            }
            buf = NULL;

            addChar(&buf, consume(tokenizer));

            // Check the next characters for rest of the word
            while (peek(tokenizer, 1) != '\0' && isalnum(peek(tokenizer, 1))) {
                addChar(&buf, consume(tokenizer));
            }

            // Add finished token
            if (strcmp(buf, "finish") == 0) {
                addToken(tokens, _return, buf, line);

            }
        }
        
        // If we have a number
        else if (isdigit(peek(tokenizer, 1))) {

            // Clears the buffer;
            if (buf) {
                free(buf);
            }
            buf = NULL;

            // Check the next characters for the rest of number
            while (peek(tokenizer, 1) != '\0' && isdigit(peek(tokenizer, 1))) {
                addChar(&buf, consume(tokenizer));

            }
            addToken(tokens, int_lit, buf, line);
        }

        // If we have a semicolon
        else if (peek(tokenizer, 1) == ';') {

            // Clears the buffer;
            if (buf) {
                free(buf);
            }
            buf = NULL;

            addChar(&buf, consume(tokenizer));
            addToken(tokens, semi, buf, line);
        }

        // If we have a new line
        else if (peek(tokenizer, 0) == '\n') {
            consume(tokenizer);
            line++;
        }

        // If we have a character that is unknown
        else {
            consume(tokenizer);
        }
    }
    return tokens;
}
