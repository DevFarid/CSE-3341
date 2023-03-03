#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "tree.h"
#include "printer.h"

/*
*
* Helper functions
*
*/


static void indents(int indent) {
	int i;
	for (i=0; i<indent; i++) {
		printf("  ");
	}
}

/*
*
* Print functions go here
*
*/

// void printProcedure(struct nodeProcedure* root) {
// 	if(root == NULL)
// 		fprintf(stderr, "Error: nodeProcedure was NULL");

// 	while(root != NULL) {
// 		struct nodeDeclSeq* declarations = root->declarations;
// 		struct nodeStmtSeq* statements = root->statements;

// 		while(declarations != NULL && declarations->decl != NULL) {
// 			printf("ok im in dec\n");
// 			declarations = declarations->next;
// 		}
		
// 		while(statements != NULL) {
// 			printf("ok im in stmt\n");
// 			statements = statements->next;
// 		}

// 		// At this point we can assume that, we have reached "END" token, after reading decSeq, then statements.
// 		break; 
// 	}
// }