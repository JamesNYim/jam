// Header file for the Token Class
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    _return,
    int_lit,
    semi
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
    int line;
} Token;

typedef struct {
    Token* tokData;
    int count;
    int capacity;
} TokenArray;

typedef struct {
    char* src;
    int index;
} Tokenizer;

// Initalizer for a tokenizer
Tokenizer* createTokenizer(const char* src);

// Destructor for a tokenizer
// Notes: Need to free tokenizer pointer itself
void freeTokenizer(Tokenizer* tokenizer);

// Initalizer for Token Array
void initTokenArray(TokenArray* tkArray);

// Destructor for Token Array
// Notes: Need to free Token Array pointer itself
void freeTokenArray(TokenArray* tkArray);

// Adds a token to the given Token Array
void addToken(TokenArray* tkArray, TokenType type, const char* lexeme, int line);

// Tokenizes a given source string and places the tokens in a token array and returns the token array.
TokenArray* tokenize(Tokenizer* tokenizer);

#endif
