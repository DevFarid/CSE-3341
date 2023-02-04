#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "core.h"

// Do not change this main method unless you have a good reason to do so
int main(int argc, char *argv[]) {  
	// If the optional CLI argument "verbose" is given, will print out
	// string values instead of the enumeration value (for testing)
	int verbose = (argc > 2) && (strcmp(argv[1], "verbose") == 0);

	// Initialize the scanner
	scanner_open(argv[1 + verbose]);

	while (currentToken() != EOS && currentToken() != ERROR) {
	// Get the current token
	int current = currentToken();
	  
	// If verbose, convert enum value to char string 
	if (verbose == 1) {
		char str[10];
		//char* str;
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
			case LPAREN : strcpy(str, "LPAREN"); break;
			case RPAREN : strcpy(str, "RPAREN"); break;
			case CONST : strcpy(str, "CONST"); break;
			case ID : strcpy(str, "ID"); break;
			case EOS : strcpy(str, "EOS"); break;
		}
		printf("%s", str);

		// If ID/CONST, print that value
		if (currentToken() == ID) {
			char value[20];
			getId(value);
			printf("[%s]", value);
		} else if (currentToken() == CONST) {
			int value = getConst();
			printf("[%d]", value);
		}
		printf("\n");
	} else {
		printf("%d\n", current);
	}

	// Advance to the next token
	nextToken();
	}

	// Scanning is done, release memory
	scanner_close();
	return 0;
}