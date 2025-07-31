// File for some helper functions

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addChar(char **s, char c) {
    if (*s == NULL) {
        // If string is NULL, allocate initial space for 2 characters
        *s = malloc(2);
        if (*s == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        (*s)[0] = c;
        (*s)[1] = '\0';
        return;
    }

    int len = strlen(*s);
    char *newStr = realloc(*s, len + 2); // +1 for new char, +1 for '\0'
    if (!newStr) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }

    newStr[len] = c;
    newStr[len + 1] = '\0';
    *s = newStr;
}

