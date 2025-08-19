#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "token.h"
#include "parser.h"

#define BUFFER_SIZE 8
void usage(char *exec) {
    fprintf(stderr, 

        ". . . . . . . . . . . . . . . . . \n"
        ". The Jam Programming Language  .\n"
        ". . . . . . . . . . . . . . . . .\n"
        ".          How to Use           .\n"
        ".                               .\n"
        ". ./jam <source file path.jelly .\n"
        ".                               .\n"
        ". . . . . . . . . . . . . . . . .\n"


        ,
        exec
    );
}

int main (int argc, char **argv) {

    // Checking for proper arguments
    if (argc != 2) {
        fprintf(stderr, "\n! Incorrect usage !\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Organizing user input
    char* sourceFilename = argv[1];
    FILE* sourceFile = fopen(sourceFilename, "r");

    // Checking if source file was successfully opened
    if (!sourceFile) {
        fprintf(stderr, "\n! Unable to open source file: %s!\n", sourceFilename);
        return EXIT_FAILURE;
    }

    // Getting filesize
    fseek(sourceFile, 0, SEEK_END);
    long int fileSize = ftell(sourceFile);
    fseek(sourceFile, 0, SEEK_SET);

    // Allocate Buffer
    char* buffer = (char*)malloc(fileSize + 1); // +1 for '\0'
    if (!buffer) {
        fclose(sourceFile);
        fprintf(stderr, "\n! Unable to allocate buffer memory !\n");
        return EXIT_FAILURE;
    }
    
    // Read file into buffer
    size_t bytesRead = fread(buffer, 1, fileSize, sourceFile);
    fclose(sourceFile);
    if (bytesRead != fileSize) {
        free(buffer);
        fprintf(stderr, "\n! Unable to read file into buffer ! \n");
        return EXIT_FAILURE;
    }
    
    // Adding null-terminate
    buffer[fileSize] = '\0';

    Tokenizer* tokenizer = createTokenizer(buffer);
    TokenArray* tokens = tokenize(tokenizer);

    
    for (int i = 0; i < tokens->count; ++i) {
        printf("Token %d: type=%d, lexeme='%s', line=%d\n",
               i,
               tokens->tokData[i].type,
               tokens->tokData[i].lexeme,
               tokens->tokData[i].line);
    }

    Parser* parser = createParser(tokens);
    NodeExit node = parse(parser);

    if (node.expr != NULL) {
        printf("Parsed return value: %s\n", node.expr->int_lit.lexeme);
    }
    else {
        printf("Failed to parse return expression.\n");
    }

    freeTokenArray(tokens);
    free(tokens);
    freeTokenizer(tokenizer);
    freeParser(parser);

    return 0;
}
