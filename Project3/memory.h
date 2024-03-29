#ifndef MEMORY_H
#define MEMORY_H

int searchInteger(char* iden);
int searchRecord(char* iden);

void memory_init();

void memory_free();

void declare(char* iden, int type);

void store(char* iden, int value);

int recall(char* iden);

void storeRec(char* iden, int index, int value);

int recallRec(char* iden, int index);

void record(char* lhs, char* rhs);

void allocateRecord(char* iden, int size);

#endif