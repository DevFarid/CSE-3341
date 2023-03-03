#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"
#include "tree.h"
#include "execute.h"
#include "memory.h"

/*
*
* Execute functions
*
*/
static char* originalFile;
static char* datafileName;
static char* dataFileExtension = ".data";

// This handles things like `Correct/1.code` and turns it into `1.data` for `IN` token.
void handleFile(char* file) {
	char* last_period = strrchr(file, '.');
	if (last_period != NULL) {
		// Get the position of the last period in the filename
		int pos = last_period - file;

		// Allocate memory for the new string
		char* new_filename = malloc(pos + 1);

		// Copy the characters from the start of the string to the last character
		// before the last period into the new string
		strncpy(new_filename, file, pos);
		new_filename[pos] = '\0';

		char* combinedFile = malloc(strlen(new_filename) + strlen(dataFileExtension) + 1);
		strcpy(combinedFile, new_filename);
		strcat(combinedFile, dataFileExtension);

		// Use the new_filename string
		datafileName = strdup(combinedFile);
		scanner_open(datafileName);

		// Free the memory allocated for new_filename when it is no longer needed
		free(new_filename);
	}
}

void executeProcedure(struct nodeProcedure* p, char* file) {

	originalFile = strdup(file);
	handleFile(file);
	// printf("Executing file: %s\nLoaded data file: %s\n", originalFile, datafileName);

	memory_init();
	executeDeclSeq(p->ds);
	executeStmtSeq(p->ss);
    // expectedToken(PROCEDURE);
    // nextToken();
    // expectedToken(ID);
    // p->name = calloc(20, sizeof(char));
    // getId(p->name);
    // nextToken();
    // expectedToken(IS);
    // nextToken();
    // p->ds = (struct nodeDeclSeq*) calloc(1, sizeof(struct nodeDeclSeq));
    // parseDeclSeq(p->ds);
    // expectedToken(BEGIN);
    // nextToken();
    // p->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
    // parseStmtSeq(p->ss);
    // expectedToken(END);
    // nextToken();
    // expectedToken(EOS);
}

void executeDeclSeq(struct nodeDeclSeq* ds) {
	// ds->type = 0;
    // ds->d = (struct nodeDecl*) calloc(1, sizeof(struct nodeDecl));
    // executeDecl(ds->d);
    // if (currentToken() != BEGIN) {
	// 	ds->type = 1;
    //     ds->ds = (struct nodeDeclSeq*) calloc(1, sizeof(struct nodeDeclSeq));
    //     parseDeclSeq(ds->ds);
    // }


	struct nodeDeclSeq* currentDeclaration = ds;
	while(currentDeclaration != NULL) {
		executeDecl(currentDeclaration->d);
		currentDeclaration = currentDeclaration->ds;
	}
}

void executeDecl(struct nodeDecl* d) {
    // if (currentToken() == INTEGER) {
	// 	d->type = 0;
    //     d->di = (struct nodeDeclInt*) calloc(1, sizeof(struct nodeDeclInt));
    //     parseDeclInt(d->di);
    // } else if (currentToken() == RECORD) {
	// 	d->type = 1;
    //     d->dr = (struct nodeDeclRec*) calloc(1, sizeof(struct nodeDeclRec));
    //     parseDeclRec(d->dr);
    // }
	if(d->type == 0) {
		executeDeclInt(d->di);
	} else if(d->type == 1) {
		executeDeclRec(d->dr);
	}
}

void executeDeclInt(struct nodeDeclInt* di) {
	declare(di->name, 0);
    // expectedToken(INTEGER);
    // nextToken();
    // di->name = calloc(20, sizeof(char));
    // getId(di->name);
    // nextToken();
    // expectedToken(SEMICOLON);
    // nextToken();
}

void executeDeclRec(struct nodeDeclRec* dr) {
	declare(dr->name, 1);
    // expectedToken(RECORD);
    // nextToken();
    // dr->name = calloc(20, sizeof(char));
    // getId(dr->name);
    // nextToken();
    // expectedToken(SEMICOLON);
    // nextToken();
}

// <stmt-seq> ::= <stmt> | <stmt><stmt-seq>
void executeStmtSeq(struct nodeStmtSeq* pss) {
	// ss->more = 0;
    // ss->s = (struct nodeStmt*) calloc(1, sizeof(struct nodeStmt));
    // executeStmt(ss->s);
    // if (currentToken() != END && currentToken() != ELSE) {
	// 	ss->more = 1;
    //     ss->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
    //     executeStmtSeq(ss->ss);
    // }
	struct nodeStmtSeq* currentStatement = pss;
	while(currentStatement != NULL) {
		executeStmt(currentStatement->s);
		currentStatement = currentStatement->ss;
	}
}

// <stmt> ::= <assign> | <if> | <loop> | <out>
void executeStmt(struct nodeStmt* s) {
    // if (currentToken() == ID) {
	// 	s->type = 0;
    //     s->assign = (struct nodeAssign*) calloc(1, sizeof(struct nodeAssign));
    //     executeAssign(s->assign);
    // } else if (currentToken() == IF) {
	// 	s->type = 1;
    //     s->ifStmt = (struct nodeIf*) calloc(1, sizeof(struct nodeIf));
    //     executeIf(s->ifStmt);
    // } else if (currentToken() == WHILE) {
	// 	s->type = 2;
    //     s->loop = (struct nodeLoop*) calloc(1, sizeof(struct nodeLoop));
    //     executeLoop(s->loop);
    // } else if (currentToken() == OUT) {
	// 	s->type = 3;
    //     s->out = (struct nodeOut*) calloc(1, sizeof(struct nodeOut));
    //     executeOut(s->out);
    // } 

	if(s->type == 0) {
		executeAssign(s->assign);
	} else if(s->type == 1) {
		executeIf(s->ifStmt);
	} else if(s->type == 2) {
		executeLoop(s->loop);
	} else if(s->type == 3) {
		executeOut(s->out);
	}
}


// <assign> ::= id <index> := <expr> ; | id := new record [ <expr> ]; | id := record id;
void executeAssign(struct nodeAssign* a) {
	if(a->type == 0) {
		// when id := <expr>
		// call memory.store();
		if(searchInteger(a->lhs) != -1) {
			store(a->lhs, executeExpr(a->expr));
		} else if(searchRecord(a->lhs) != -1) {
			storeRec(a->lhs, 0, executeExpr(a->expr));
		}
		// printf("%d\n", recall(a->lhs));
	} else if(a->type == 1) {
		// when id[<index>] := <expr>, id[], LBRACE
		int index = executeIndex(a->index);
		int expr = executeExpr(a->expr);
		// Call memory.storeRec()
		storeRec(a->lhs, index, expr);
		// printf("%s[%d] := %d\n", a->lhs, index, expr);
	} else if(a->type == 2) { // NEW RECORD[<expr>]
		int expr = executeExpr(a->expr);
		// allocateRecord from memory.
		// printf("%s := new record[%d]\n", a->lhs, expr);
		allocateRecord(a->lhs, expr);
	} else if(a->type == 3) {
		// RECORD ID assignment to another RECORD
		// printf("%s := record %s\n", a->lhs, a->rhs);
		record(a->lhs, a->rhs);
	}

	// a->type = 0;
    // expectedToken(ID);
    // a->lhs = calloc(20, sizeof(char));
    // getId(a->lhs);
    // nextToken();

    // if (currentToken() == LBRACE) {
	// 	a->type = 1;
    //     a->index = (struct nodeIndex*) calloc(1, sizeof(struct nodeIndex));
    //     executeIndex(a->index);
    // }
    // expectedToken(ASSIGN);
    // nextToken();
    // if (currentToken() == NEW) {
    //     a->type = 2;
    //     nextToken();
    //     expectedToken(RECORD);
    //     nextToken();
    //     expectedToken(LBRACE);
    //     nextToken();
    //     a->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
    //     executeExpr(a->expr);
    //     expectedToken(RBRACE);
    //     nextToken();
    // } else if (currentToken() == RECORD) {
    //     a->type = 3;
    //     nextToken();
    //     expectedToken(ID);
    //     a->rhs = calloc(20, sizeof(char));
    //     getId(a->rhs);
    //     nextToken();
    // } else  {
    //     a->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
    //     executeExpr(a->expr);
    // }
    // expectedToken(SEMICOLON);
    // nextToken();
}

// index> ::= [ <expr> ] | epsilon
int executeIndex(struct nodeIndex* index) {
    // expectedToken(LBRACE);
    // nextToken();
    // index->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
    return executeExpr(index->expr);
    // expectedToken(RBRACE);
    // nextToken();
}

// <out> ::= out ( <expr> ) ;
void executeOut(struct nodeOut* out) {
	// expectedToken(OUT);
	// nextToken();
	// expectedToken(LPAREN);
	// nextToken();
	// out->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	// printf("out(%d)\n", executeExpr(out->expr));
    printf("%d\n", executeExpr(out->expr));
    // expectedToken(RPAREN);
    // nextToken();
	// expectedToken(SEMICOLON);
    // nextToken();
}

// <if> ::= if <cond> then <stmt-seq> end | if <cond> then <stmt-seq> else <stmt-seq> end
void executeIf(struct nodeIf* ifStmt) {
	// ifStmt->type = 0;
	// expectedToken(IF);
	// nextToken();
	// ifStmt->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
    // executeCond(ifStmt->cond);
	// expectedToken(THEN);
	// nextToken();
	// ifStmt->ss1 = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
    // executeStmtSeq(ifStmt->ss1);
	// if (currentToken() == ELSE) {
	// 	ifStmt->type = 1;
	// 	nextToken();
	// 	ifStmt->ss2 = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
	// 	executeStmtSeq(ifStmt->ss2);
	// }
	// expectedToken(END);
	// nextToken();
	
	
	if(executeCond(ifStmt->cond) == 1) {
		executeStmtSeq(ifStmt->ss1);
	} else {
		if(ifStmt->type == 1) {
			executeStmtSeq(ifStmt->ss2);
		}
	}
}


// <loop> ::= while <cond> do <stmt-seq> end
void executeLoop(struct nodeLoop* loop) {
	// expectedToken(WHILE);
	// nextToken();
	// loop->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
    // executeCond(loop->cond);
	// expectedToken(DO);
	// nextToken();
	// loop->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
	// executeStmtSeq(loop->ss);
	// expectedToken(END);
	// nextToken();
	while(executeCond(loop->cond) == 1) {
		executeStmtSeq(loop->ss);
	}
}

// <cond> ::= <cmpr> | not <cond> | <cmpr> or <cond> | <cmpr> and <cond>
// DONE, returns a 1/0 (T/F) to see if the condition was met.
int executeCond(struct nodeCond* cond) {
	int boolCmprVal;
	if(cond->type == 0) {
		boolCmprVal = executeCmpr(cond->cmpr);
	} else if(cond->type == 1) {
		return !executeCond(cond->cond);
	} else if(cond->type == 2 || cond->type == 3) {
		int cmprVal = executeCmpr(cond->cmpr);
		int condVal = executeCond(cond->cond);
		return (cond->type == 2) ? cmprVal || condVal : cmprVal && condVal;
	}

	// cond->type = 0;
	// if (currentToken() == NOT) {
	// 	nextToken();
	// 	cond->type = 1;
	// 	cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
	// 	executeCond(cond->cond);
	// } else {
	// 	cond->cmpr = (struct nodeCmpr*) calloc(1, sizeof(struct nodeCmpr));
	// 	executeCmpr(cond->cmpr);
	// 	if (currentToken() == OR) {
	// 		nextToken();
	// 		cond->type = 2;
	// 		cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
	// 		executeCond(cond->cond);
	// 	} else if (currentToken() == AND) {
	// 		nextToken();
	// 		cond->type = 3;
	// 		cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
	// 		executeCond(cond->cond);
	// 	}
	// }
	
}

// <cmpr> ::= <expr> = <expr> | <expr> < <expr>
// DONE
int executeCmpr(struct nodeCmpr* cmpr) {
	int exprValLHS = executeExpr(cmpr->expr1);
	int exprValRHS = executeExpr(cmpr->expr2);


	if(cmpr->type == 0) {
		return (exprValLHS == exprValRHS);
	} else if(cmpr->type == 1) {
		return (exprValLHS < exprValRHS);
	}
	// cmpr->expr1 = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	// executeExpr(cmpr->expr1);
	// if (currentToken() == EQUAL) {
	// 	cmpr->type = 0;
	// } else if (currentToken() == LESS) {
	// 	cmpr->type = 1;
	// }
	// nextToken();
	// cmpr->expr2 = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	// executeExpr(cmpr->expr2);
}

// <expr> ::= <term> | <term> + <expr> | <term> – <expr>
// DONE
int executeExpr(struct nodeExpr* expr) {
	int value = executeTerm(expr->term);
	if(expr->type == 1) {
		value += executeExpr(expr->expr);
	} else if(expr->type == 2) {
		value -= executeExpr(expr->expr);
	} 
	return value;
}

// <term> ::= <factor> | <factor> * <term> | <factor> / <term> 
// DONE
int executeTerm(struct nodeTerm* term) {
	int factor = executeFactor(term->factor);
	int termVal = 0;

	if(term->type > 0) {
		termVal = executeTerm(term->term);

		if(term->type == 1) {
			factor *= termVal;
		} else if(term->type == 2) {
			if(termVal != 0) {
				factor /= termVal;
			} else {
				printf("ERROR: Tried to divide by 0\n");
				exit(1);
			}
		} 
		
	}

	return factor;
	// term->type = 0;
	// term->factor = (struct nodeFactor*) calloc(1, sizeof(struct nodeFactor));
	// executeFactor(term->factor);
	// if (currentToken() == MULTIPLY) {
	// 	term->type = 1;
	// } else if (currentToken() == DIVIDE) {
	// 	term->type = 2;
	// }
	// if (term->type > 0) {
	// 	nextToken();
	// 	term->term = (struct nodeTerm*) calloc(1, sizeof(struct nodeTerm));
	// 	executeTerm((*term).term);
	// }
}

// <factor> ::= id | id [ <expr> ] | const | ( <expr> ) | in ( )
int executeFactor(struct nodeFactor* factor) {
	
	// ID 
	if(factor->type == 0) {
		// The `ID` should be declared in `iLookup` during declaration execution
		// Here tho, we should grab the value of `ID` and put it in `iValue`.
		// ID := ID
		// printf("expr(identifier) => %d\n", recall(factor->id));
		if(searchInteger(factor->id) != -1) {
			return recall(factor->id);
		} else if(searchRecord(factor->id) != -1) {
			return recallRec(factor->id, 0);
		}
	} else if(factor->type == 1) { // when token is `[`
		int index = executeExpr(factor->expr);
		// Want to make sure to access a record id array at specific index based on the [<expr>]
		return recallRec(factor->id, index);
	} else if(factor->type == 2) { // Constant
		// printf("in constant: %d\n", factor->constant);
		return factor->constant;
	} else if(factor->type == 3) { // when token is `(`
		return executeExpr(factor->expr);
	} else if(factor->type == 4) {// when using `IN`.
		// Using scanner, we need to read CONST tokens inside files i think?

		// printf("From IN: %d\n", currentToken());
		if(currentToken() != EOS) {
			int constant = getConst();
			nextToken(); 
			return constant;
		} else {
			printf("ERROR: Reached END-OF-FILE for %s\n", datafileName);
			exit(1);
		}

		// Close scanner when done
	}

	// if (currentToken() == ID) {
	// 	factor->type = 0;
	// 	factor->id = calloc(20, sizeof(char));
    //     getId(factor->id);
    //     nextToken();
	// 	if (currentToken() == LBRACE) {
	// 		factor->type = 1;
	// 		nextToken();
	// 		factor->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	// 		executeExpr((*factor).expr);
	// 		expectedToken(RBRACE);
	// 		nextToken();
	// 	}
	// } else if (currentToken() == CONST) {
	// 	factor->type = 2;
	// 	factor->constant = getConst();
	// 	nextToken();
	// } else if (currentToken() == LPAREN) {
	// 	factor->type = 3;
	// 	nextToken();
	// 	factor->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	// 	executeExpr(factor->expr);
	// 	expectedToken(RPAREN);
	// 	nextToken();
	// } else {
	// 	factor->type = 4;
	// 	expectedToken(IN);
	// 	nextToken();
	// 	expectedToken(LPAREN);
	// 	nextToken();
	// 	expectedToken(RPAREN);
	// 	nextToken();
	// }
}