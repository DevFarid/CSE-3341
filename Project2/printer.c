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

void printProcedure(struct nodeProcedure* root) {

}