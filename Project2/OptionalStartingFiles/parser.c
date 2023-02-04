#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"
#include "tree.h"
#include "parser.h"

/*
*
* Helper functions
*
*/

// Converts token value to a string
static void tokenString(char* str, int current) {
	switch (current) {
	  case AND : strcpy(str, "AND"); break;
	  case BEGIN : strcpy(str, "BEGIN"); break;
	  case DO : strcpy(str, "DO"); break;
	  case ELSE : strcpy(str, "ELSE"); break;
	  case END : strcpy(str, "END"); break;
	  case IF : strcpy(str, "IF"); break;
	  case IN : strcpy(str, "IN"); break;
	  case INTEGER : strcpy(str, "INTEGER"); break;
	  case IS : strcpy(str, "IS"); break;
	  case NEW : strcpy(str, "NEW"); break;
	  case NOT : strcpy(str, "NOT"); break;
	  case OR : strcpy(str, "OR"); break;
	  case OUT : strcpy(str, "OUT"); break;
	  case PROCEDURE : strcpy(str, "PROCEDURE"); break;
	  case RECORD : strcpy(str, "RECORD"); break;
	  case THEN : strcpy(str, "THEN"); break;
	  case WHILE : strcpy(str, "WHILE"); break;
	  case ADD : strcpy(str, "ADD"); break;
	  case SUBTRACT : strcpy(str, "SUBTRACT"); break;
	  case MULTIPLY : strcpy(str, "MULTIPLY"); break;
	  case DIVIDE : strcpy(str, "DIVIDE"); break;
	  case ASSIGN : strcpy(str, "ASSIGN"); break;
	  case EQUAL : strcpy(str, "EQUAL"); break;
	  case LESS : strcpy(str, "LESS"); break;
	  case COLON : strcpy(str, "COLON"); break;
	  case SEMICOLON : strcpy(str, "SEMICOLON"); break;
	  case PERIOD : strcpy(str, "PERIOD"); break;
	  case COMMA : strcpy(str, "COMMA"); break;
	  case LPAREN : strcpy(str, "LPAREN"); break;
	  case RPAREN : strcpy(str, "RPAREN"); break;
	  case LBRACE : strcpy(str, "LBRACE"); break;
	  case RBRACE : strcpy(str, "RBRACE"); break;
	  case CONST : strcpy(str, "CONST"); break;
	  case ID : strcpy(str, "ID"); break;
	  case EOS : strcpy(str, "EOS"); break;
	}
}

// If the current token is something other than expected,
// prints a meaningful error message and halts the program
static void expectedToken(int expected) {
	int actual = currentToken();
	
    if (actual != expected) {
		char actualStr[20];
		char expectedStr[20];
		
		tokenString(actualStr, actual);
		tokenString(expectedStr, expected);
		
        printf("Error: expected %s but recieved %s", expectedStr, actualStr);
        exit(0);
    }
}


/*
*
* Parse functions go here
*
*/


