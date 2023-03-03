#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "memory.h"

/*
*
* Data Structs
*
*/

static char** iLookup; 	// 	identifier lookup
static int* iValues; 	// 	identifier values
static int iLen;		//  array length for integer lookup and value.

static char** rLookup;	// identifier record lookup
static int** rValues;	// 2D array where a record identifier may store its value for all indices.
static int rLen;		// array length for record lookup and value.
static int* individualRValuesSize;


/*
*
* Helper functions
*
*/

// If iden is an integer, return the index. Otherwise, return -1
int searchInteger(char* iden) {
	int location = -1, i;
	for (i=0; i<iLen; i++) {
		if (strcmp(iLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a record, return the index. Otherwise, return -1
int searchRecord(char* iden) {
	int location = -1, i;
	for (i=0; i<rLen; i++) {
		if (strcmp(rLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

static int* retrieveValueRec(char* identifier) {
	int identifierExists = searchRecord(identifier);

	if(identifierExists != -1) {
		return rValues[identifierExists];
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
	iLen = 0; 
	rLen = 0;

	iLookup = (char**) calloc(1, sizeof(char*));
	iValues = (int*) calloc(1, sizeof(int));

	rLookup = (char**) calloc(1, sizeof(char*));
	rValues = (int**) calloc(1, sizeof(int*));
}

// Handle an integer or record declaration
void declare(char* iden, int type) {
	if(iden == NULL) return; // We don't need NULL strings. Waste of memory if some part of the pogram is not implemented properly. 

	char** newBuffer;
	// printf("Declaring %s as %s\n", iden, (type == 0 ? "int" : "record"));

	if(type == 0) {
		int* newValBuffer;

		iLen++;
		newBuffer = realloc(iLookup, (iLen * sizeof(char*)) );
		newValBuffer = realloc(iValues, (iLen * sizeof(int)) );

		if(newBuffer == NULL || newValBuffer == NULL) {
			printf("ERROR: REALLOCATION ERROR INSIDE declare().\n");
			exit(1);
		}
		iLookup = newBuffer;
		iValues = newValBuffer;

		iLookup[iLen - 1] = strdup(iden);

	} else if(type == 1) {
		int** newRValBuffer, *indvidualSizeRebuffer;

		rLen++;
		newBuffer = realloc(rLookup, (rLen * sizeof(char*)) );
		newRValBuffer = realloc(rValues, (rLen * sizeof(int*)) );
		indvidualSizeRebuffer = realloc(individualRValuesSize, (rLen * sizeof(int)));

		if(newBuffer == NULL || newRValBuffer == NULL || indvidualSizeRebuffer == NULL) {
			printf("ERROR: REALLOCATION ERROR INSIDE declare().\n");
			exit(1);
		}

		rLookup = newBuffer;
		rValues = newRValBuffer;
		individualRValuesSize = indvidualSizeRebuffer;
		rLookup[rLen - 1] = strdup(iden);
	}
}

// Store a value to a variable. Remember, unindexed stores to a record go to index 0
void store(char* iden, int value) {
	// printf("memory.c#store(): was called iValues[%d] = %d\n", iLen-1, value);
	int identifierIndexExists = searchInteger(iden);

	if(identifierIndexExists != -1) {
		iValues[identifierIndexExists] = value;
	}
}

// Read a value from a variable. Remember, unindexed reads from a record read index 0
int recall(char* iden) {
	int identifierExists = searchInteger(iden);

	if(identifierExists != -1) {
		return iValues[identifierExists];
	}

	return -1;
}

// Store a value to a record variable, at the given index
void storeRec(char* iden, int index, int value) {
	int recordExists = searchRecord(iden);

	if(recordExists != -1) {
		if(rValues[recordExists] == NULL) {
			printf("Inner memory for record was not configured properly.\n");
			exit(1);
		}
		if(index > individualRValuesSize[recordExists]) {
			printf("Inner memory was configured, however index was out of bounds.\n");
			exit(1);
		}
		rValues[recordExists][index] = value;
	}
}

// Read a value from a record variable, from the given index
int recallRec(char* iden, int index) {
	int recordExists = searchRecord(iden);

	if(recordExists != -1) {
		return rValues[recordExists][index];
	}

	return -1;
}

// Handle "id := record id" type assignment
void record(char* lhs, char* rhs) {

	if(strcmp(lhs, rhs) == 0) return;

	int x_lhs = searchRecord(lhs);
	int y_rhs = searchRecord(rhs);

	if(x_lhs != -1 && y_rhs != -1) {
		rValues[x_lhs] = rValues[y_rhs];
	}

}

// Handle "id := new record[<expr>]" type assignment
void allocateRecord(char* iden, int size) {
	// Where we want to allocate rValues[rLen - 1] = bytes * size for a specific record id segment space.
	
	int recordExists = searchRecord(iden);
	if(recordExists != -1) {
		rValues[recordExists] = calloc(size, sizeof(int));
		individualRValuesSize[recordExists] = size - 1;
	}

}