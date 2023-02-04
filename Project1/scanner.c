#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"

// Below are the values we want to persist as the scanner runs

// This maintains a pointer to the file
static FILE* fp;

// This is the string making up the current token
static char* tokenString;

// This is the enum value for the current token
static int token;



// Below are the functions that make up the scanner

// Opens the file, find the first token
int scanner_open(char* filename) {
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error: File %s couldn't be opened!\n", filename);
    }
    tokenString = NULL;
    return nextToken();
}

// Frees memory allocated for the scanner
int scanner_close() {
    fclose(fp);
    free(tokenString);
}

// Returns the current token (enum value)
int currentToken() {
	// You will need to fill this in with appropriate things
}

// Finds the next token (or the first token when called by scanner_open)
int nextToken() {
    // You will need to fill this in with appropriate things
}

// Passes back the string value, so the parse tree (project 2) will be able to store identifiers
void getId(char* identifier) {
    strcpy(identifier, tokenString);
}

// Passes back the constant value, so the parse tree (project 2) will be able to store const values
int getConst() {
    return atoi(tokenString);
}