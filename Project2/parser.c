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
static int expectedToken(int expected) {
	int actual = currentToken();
	
    if (actual != expected) {
		char actualStr[20];
		char expectedStr[20];
		
		tokenString(actualStr, actual);
		tokenString(expectedStr, expected);
		
        printf("Error: expected %s but recieved %s", expectedStr, actualStr);
        exit(0);
    } else {
		return 1;
	}
}


/*
*	Traveses the scanner entirely via a while loop.
*/
void traverseEntirely() {
	/* STRING representation of a token. */
	char str[10];

	/* When the token is an ID, CONST, or possbile an ERROR token. */
	char value[20]; 

	int current;
	while(currentToken() != EOS && currentToken() != ERROR) {
		current = currentToken();
		tokenString(str, current);
		printf("%s\n", str);

		if(current == ID) {
			getId(value);
			printf("[%s]", value);
		} else if(current == CONST) {
			int value = getConst();
			printf("[%d]", value);
		} else if(current == ERROR) {
			printf("We received an ERROR token, but we do not know the exactly details yet.\n");
		}
		printf("\n");
    	nextToken();
	}
}

void parseProcedure(struct nodeProcedure* root) {
	char value[20]; 

	if(expectedToken(PROCEDURE)) {
		nextToken();
		if(expectedToken(ID)) {
			getId(root->name);
			if(root->name != NULL) {
				nextToken();
				if(expectedToken(IS)) {
					printf("PROCEDURE %s IS\n", root->name);
					
					root->declarations = calloc(1, sizeof(struct nodeDeclSeq));
					root->statements = calloc(1, sizeof(struct nodeStmtSeq));
					/* 
						At this point we have verified that the tokens ares `PROCEDURE name IS` 
						We can assume that now the declaration starts.
					*/
					if(root->declarations == NULL)
						printf("Error: Allocation failure!\n");
				
					parseDeclSeq(root->declarations);
					printf("%d\n",root->declarations->decl->type);
					
					/* 
						If the next token is `BEGIN` declaration area has ended.
					*/
					if(expectedToken(BEGIN)) {
						// parseStmtSeq(root->statements); 
					}

				}
			} else {
				fprintf(stderr, "Expected a program name, but was NULL\n");
			}
		}
	}

}

void parseDeclSeq(struct nodeDeclSeq* declarationBlock) {
	nextToken();
	char value[20]; 
	
	declarationBlock->decl = calloc(1, sizeof(struct nodeDecl));
	if(currentToken() == INTEGER) {
		nextToken();
		if(currentToken() == ID) {
			getId(value);
			declarationBlock->decl->type = 0; 
			declarationBlock->decl->id = value; 
		} else {
			printf("Expected ID after INTEGER declaration, but received none.");
		}
	} else if(currentToken() == RECORD) {
		nextToken();
		if(currentToken() == ID) {
			getId(value);
			declarationBlock->decl->type = 0; 
			declarationBlock->decl->id = value; 
		} else {
			printf("Expected ID after RECORD declaration, but received none.");
		}
	}
}

void parseStmtSeq(struct nodeStmtSeq* stmts) {
	printf("it worked");
}