#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

/*
* * Put headers for parse functions here
*
*/
void parseProcedure(struct nodeProcedure*);
void parseStmtSeq(struct nodeStmtSeq*);
void parseDeclSeq(struct nodeDeclSeq*);

#endif