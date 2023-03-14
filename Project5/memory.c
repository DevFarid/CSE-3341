#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "memory.h"
#include "execute.h"

/*
*
* Data Structs
*
*/

static struct nodeFunc** funcLookup;	// function lookup
static int fLen;		// array length for function lookup.

struct nodeFrame {
	char** iLookup;
	int* iValues;
	int iLen;

	char** rLookup;
	int** rValues;
	int* individualRValuesSize;
	int rLen;
};

static struct nodeFrame* frameStack[MAX_STACK];
int currentSp = 0; // points to the current index in the stack that's the active SP.
static struct nodeFrame* currentFrame;



/*
*
* Helper functions
*
*/

// If iden is an integer, return the index. Otherwise, return -1
int searchInteger(char* iden) {
	int location = -1, i;
	for (i=0; i<currentFrame->iLen; i++) {
		if (strcmp(currentFrame->iLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a record, return the index. Otherwise, return -1
int searchRecord(char* iden) {
	int location = -1, i;
	for (i=0; i<currentFrame->rLen; i++) {
		if (strcmp(currentFrame->rLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

int searchFunction(char *iden) {
	int location = -1, i;
	for (i=0; i<fLen; i++) {
		if (strcmp(funcLookup[i]->name, iden)==0) {
			location = i;
		}
	}
	return location;
}

static int* retrieveValueRec(char* identifier) {
	int identifierExists = searchRecord(identifier);

	if(identifierExists != -1) {
		return currentFrame->rValues[identifierExists];
	}

	return NULL;
}

/*
*
* Memory functions
*
*/

// Initialize data structures
// Don't worry about freeing memory, will do that in project 5
void memory_init() {
	frameStack[currentSp] = calloc(1, sizeof(struct nodeFrame));
	currentFrame = frameStack[currentSp];
	currentSp++;

	currentFrame->iLen = 0;
	currentFrame->rLen = 0;

	currentFrame->iLookup = (char**) calloc(1, sizeof(char*));
	currentFrame->iValues = (int*) calloc(1, sizeof(int));

	currentFrame->rLookup = (char**) calloc(1, sizeof(char*));
	currentFrame->rValues = (int**) calloc(1, sizeof(int*));
}

void initFuncLookup() {
	fLen = 0;
	funcLookup = (struct nodeFunc**) calloc(1, sizeof(struct nodeFunc*));
}

void memory_free() {

}

void declareFunc(struct nodeFunc* func) {

	if(searchFunction(func->name) == -1) {
		fLen++;
		
		struct nodeFunc** newBuffer = realloc(funcLookup, (fLen * sizeof(struct nodeFunc*)) );

		if(newBuffer == NULL) {
			printf("ERROR: REALLOCATION ERROR INSIDE declareFunc().\n");
			exit(1);
		}

		funcLookup = newBuffer;
		funcLookup[fLen - 1] = func;
	} else {
		printf("ERROR: Procedure %s already declared.\n", func->name);
		exit(1);
	}
}

// Handle an integer or record declaration
void declare(char* iden, int type) {
	if(iden == NULL) return; // We don't need NULL strings. Waste of memory if some part of the pogram is not implemented properly. 

	char** newBuffer;
	// printf("Declaring %s as %s\n", iden, (type == 0 ? "int" : "record"));

	if(type == 0) {
		int* newValBuffer;

		currentFrame->iLen++;
		newBuffer = realloc(currentFrame->iLookup, (currentFrame->iLen * sizeof(char*)) );
		newValBuffer = realloc(currentFrame->iValues, (currentFrame->iLen * sizeof(int)) );

		if(newBuffer == NULL || newValBuffer == NULL) {
			printf("ERROR: REALLOCATION ERROR INSIDE declare().\n");
			exit(1);
		}
		currentFrame->iLookup = newBuffer;
		currentFrame->iValues = newValBuffer;

		currentFrame->iLookup[currentFrame->iLen - 1] = strdup(iden);

	} else if(type == 1) {
		int** newRValBuffer, *indvidualSizeRebuffer;

		currentFrame->rLen++;
		newBuffer = realloc(currentFrame->rLookup, (currentFrame->rLen * sizeof(char*)) );
		newRValBuffer = realloc(currentFrame->rValues, (currentFrame->rLen * sizeof(int*)) );
		indvidualSizeRebuffer = realloc(currentFrame->individualRValuesSize, (currentFrame->rLen * sizeof(int)));

		if(newBuffer == NULL || newRValBuffer == NULL || indvidualSizeRebuffer == NULL) {
			printf("ERROR: REALLOCATION ERROR INSIDE declare().\n");
			exit(1);
		}

		currentFrame->rLookup = newBuffer;
		currentFrame->rValues = newRValBuffer;
		currentFrame->individualRValuesSize = indvidualSizeRebuffer;
		currentFrame->rLookup[currentFrame->rLen - 1] = strdup(iden);
	}
}

// Store a value to a variable. Remember, unindexed stores to a record go to index 0
void store(char* iden, int value) {
	// printf("memory.c#store(): was called iValues[%d] = %d\n", iLen-1, value);
	int identifierIndexExists = searchInteger(iden);

	if(identifierIndexExists != -1) {
		currentFrame->iValues[identifierIndexExists] = value;
	}
}

// Read a value from a variable. Remember, unindexed reads from a record read index 0
int recall(char* iden) {
	int identifierExists = searchInteger(iden);

	if(identifierExists != -1) {
		return currentFrame->iValues[identifierExists];
	}

	return -1;
}

// Store a value to a record variable, at the given index
void storeRec(char* iden, int index, int value) {
	int recordExists = searchRecord(iden);

	if(recordExists != -1) {
		if(currentFrame->rValues[recordExists] == NULL) {
			printf("Inner memory for record was not configured properly.\n");
			exit(1);
		}
		if(index > currentFrame->individualRValuesSize[recordExists]) {
			printf("Inner memory was configured, however index was out of bounds.\n");
			exit(1);
		}
		currentFrame->rValues[recordExists][index] = value;
	}
}

// Read a value from a record variable, from the given index
int recallRec(char* iden, int index) {
	int recordExists = searchRecord(iden);

	if(recordExists != -1) {
		return currentFrame->rValues[recordExists][index];
	}

	return -1;
}

// Handle "id := record id" type assignment
void record(char* lhs, char* rhs) {

	if(strcmp(lhs, rhs) == 0) return;

	int x_lhs = searchRecord(lhs);
	int y_rhs = searchRecord(rhs);

	if(x_lhs != -1 && y_rhs != -1) {
		currentFrame->rValues[x_lhs] = currentFrame->rValues[y_rhs];
	}

}

// Handle "id := new record[<expr>]" type assignment
void allocateRecord(char* iden, int size) {
	// Where we want to allocate rValues[rLen - 1] = bytes * size for a specific record id segment space.
	
	int recordExists = searchRecord(iden);
	if(recordExists != -1) {
		currentFrame->rValues[recordExists] = calloc(size, sizeof(int));
		currentFrame->individualRValuesSize[recordExists] = size - 1;
	}
}

void callFunc(char* iden, int numberOfParameters, char** parameters) {
	
	int funcExists = searchFunction(iden);
	if(funcExists == -1) {
		printf("ERROR: procedure %s does not exist.\n", iden);
		exit(1);
	}

	if(numberOfParameters != funcLookup[funcExists]->numOfParamters) {
		printf("ERROR: procedure %s was called with %d parameters, but it was declared with %d parameters.\n", iden, numberOfParameters, funcLookup[funcExists]->numOfParamters);
		exit(1);
	}

	int i, j;
	for (i = 0; i < numberOfParameters; i++) {
		if(searchRecord(parameters[i]) == -1) {
			printf("ERROR: The record %s that was passed to the procedure %s not exist.\n", parameters[i], iden);
			exit(1);
		}
	}
	
	// Make new frame
	struct nodeFrame* newFrame = calloc(1, sizeof(struct nodeFrame));
	struct nodeFrame* oldFrame = frameStack[currentSp - 1];


	
	newFrame->rLookup = calloc(funcLookup[funcExists]->numOfParamters, sizeof(char*));
	newFrame->rValues = calloc(funcLookup[funcExists]->numOfParamters, sizeof(int*));
	newFrame->individualRValuesSize = calloc(funcLookup[funcExists]->numOfParamters, sizeof(int));
	newFrame->rLen = numberOfParameters;

	for(j = 0; j < funcLookup[funcExists]->numOfParamters; j++) {
		newFrame->rLookup[j] = funcLookup[funcExists]->parameters[j]; // Grab the formal parameter name.
		newFrame->rValues[j] = oldFrame->rValues[searchRecord(parameters[j])]; // Grab the share by call value.
		newFrame->individualRValuesSize[j] = oldFrame->individualRValuesSize[j];
		// printf("oldFrame->rLook[%d][0] = %d\n", searchRecord(parameters[j]), oldFrame->rValues[searchRecord(parameters[j])][0]);
		// printf("searchRecord(parameters[j]) = %d\n", searchRecord(parameters[j]));
		// printf("newFrame->rLookup[%d] = %s\nnewFrame->rValues[%d] = %d\n", j, funcLookup[funcExists]->parameters[j], searchRecord(parameters[j]), oldFrame->rValues[searchRecord(parameters[j])]);
	}
	
	// Get old values into the function param
	// Execute the new function

	currentFrame = newFrame;
	currentSp++;

	frameStack[currentSp - 1] = currentFrame;

	executeDeclSeq(funcLookup[funcExists]->ds);
	executeStmtSeq(funcLookup[funcExists]->ss);

	frameStack[currentSp - 1] = NULL;
	currentSp--;

	currentFrame = oldFrame;
}