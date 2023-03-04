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
* Print functions
*
*/

void printProcedure(struct nodeProcedure* p) {
    printf("procedure %s", p->name);

	if(p->doesHaveFunctions == 1) {
		printf(" and\n");
		printFuncSeq(p->funcs, 1);
		printf("is\n");
	} else {
		printf(" is\n");
	}

	printDeclSeq(p->ds, 1);
	printf("begin\n");
	printStmtSeq(p->ss, 1);
	printf("end\n");
	
	free(p->name);
	free(p->ds);
	free(p->ss);
}

void printFuncSeq(struct nodeFuncSeq* fs, int indent) {
	printFunc(fs->func, 1);
	if (fs->nextFunc != NULL) {
		printFuncSeq(fs->nextFunc, indent);
		
		free(fs->nextFunc);
	}
	
	free(fs->func);
}

void printFunc(struct nodeFunc* func, int indent) {
    indents(indent);
    printf("procedure %s (", func->name);

	int i;
	for(i = 0; i < func->numOfParamters; i++) {
		printf(" %s", func->parameters[i]);
		if (i != func->numOfParamters - 1) {
			printf(",");
		}
	}

	printf(" ) is\n");
	printDeclSeq(func->ds, 3);
	indents(indent);
	printf("begin\n");
	printStmtSeq(func->ss, 3);
	indents(indent);
	printf("end\n");

	free(func->name);
	free(func->ds);
	free(func->ss);
	free(func->parameters);
}

void printDeclSeq(struct nodeDeclSeq* ds, int indent) {
	printDecl(ds->d, indent);
	if (ds->type == 1) {
		printDeclSeq(ds->ds, indent);
		
		free(ds->ds);
	}
	
	free(ds->d);
}

void printDecl(struct nodeDecl* d, int indent) {
	if (d->type == 0) {
		printDeclInt(d->di, indent);
		
		free(d->di);
	} else {
		printDeclRec(d->dr, indent);
		
		free(d->dr);
	}
}

void printDeclInt(struct nodeDeclInt* di, int indent) {
    indents(indent);
	printf("integer %s;\n", di->name);
	
	free(di->name);
}

void printDeclRec(struct nodeDeclRec* dr, int indent) {
    indents(indent);
	printf("record %s;\n", dr->name);
	
	free(dr->name);
}

void printStmtSeq(struct nodeStmtSeq* ss, int indent) {
    printStmt(ss->s, indent);
	if (ss->more == 1) {
		printStmtSeq(ss->ss, indent);
		
		free(ss->ss);
	}
	
	free(ss->s);
}

void printStmt(struct nodeStmt* s, int indent) {
	if (s->type == 0) {
		printAssign(s->assign, indent);
		free(s->assign);
	} else if (s->type == 1) {
		printIf(s->ifStmt, indent);
		free(s->ifStmt);
	} else if (s->type == 2) {
		printLoop(s->loop, indent);
		free(s->loop);
	} else if (s->type == 3){
		printOut(s->out, indent);
		free(s->out);
	} else if(s->type == 4) {
		printCall(s->call, indent);
		free(s->call);
	}
}

void printCall(struct nodeCall* call, int indent) {
	indents(indent);
	printf("begin %s(", call->functionName);
	free(call->functionName);
	int i;
	for(i = 0; i < call->numOfParamters; i++) {
		printf("%s", call->parameters[i]);
		if (i != call->numOfParamters - 1) {
			printf(",");
		}
	}
	free(call->parameters);
	printf(");\n");
}

void printAssign(struct nodeAssign* a, int indent) {
	indents(indent);
	printf("%s", a->lhs);
	free(a->lhs);
	if (a->type == 1) {
		printIndex(a->index);
		free(a->index);
	}
	printf(":=");
	if (a->type <= 1) {
		printExpr(a->expr);
		free(a->expr);
	} else if (a->type == 2) {
		printf("new record [");
		printExpr(a->expr);
		free(a->expr);
		printf("]");
	} else if (a->type == 3) {
		printf("record %s", a->rhs);
		free(a->rhs);
	}
	printf(";\n");
}

void printIndex(struct nodeIndex* index) {
    printf("[");
	printExpr(index->expr);
	free(index->expr);
	printf("]");
}

void printOut(struct nodeOut* out, int indent) {
	indents(indent);
	printf("out(");
	printExpr(out->expr);
	free(out->expr);
	printf(");\n");
}

void printIf(struct nodeIf* ifStmt, int indent) {
	indents(indent);
	printf("if ");
	printCond(ifStmt->cond);
	free(ifStmt->cond);
	printf(" then\n");
	printStmtSeq(ifStmt->ss1, indent+1);
	free(ifStmt->ss1);
	if (ifStmt->type == 1) {
		indents(indent);
		printf("else\n");
		printStmtSeq(ifStmt->ss2, indent+1);
		free(ifStmt->ss2);
	}
	indents(indent);
	printf("end\n");
}

void printLoop(struct nodeLoop* loop, int indent) {
	indents(indent);
	printf("while ");
	printCond(loop->cond);
	free(loop->cond);
	printf(" do\n");
	printStmtSeq(loop->ss, indent+1);
	free(loop->ss);
	indents(indent);
	printf("end\n");
}

void printCond(struct nodeCond* cond) {
	if (cond->type == 0 || cond->type >= 2) {
		printCmpr(cond->cmpr);
		free(cond->cmpr);
	} else if (cond->type == 1) {
		printf("not ");
		printCond(cond->cond);
		free(cond->cond);
	} 
	if (cond->type == 2) {
		printf(" or ");
		printCond(cond->cond);
		free(cond->cond);
	} else if (cond->type == 3) {
		printf(" and ");
		printCond(cond->cond);
		free(cond->cond);
	}
}

void printCmpr(struct nodeCmpr* cmpr) {
	printExpr(cmpr->expr1);
	if (cmpr->type == 0) {
		printf(" = ");
	} else {
		printf(" < ");
	}
	printExpr(cmpr->expr2);
	
	free(cmpr->expr1);
	free(cmpr->expr2);
}

void printExpr(struct nodeExpr* expr) {
	printTerm(expr->term);
	if (expr->type == 1) {
		printf("+");
	} else if (expr->type == 2) {
		printf("-");
	}
	if (expr->type > 0) {
		printExpr(expr->expr);
		free(expr->expr);
	}
	free(expr->term);
}

void printTerm(struct nodeTerm* term) {
	printFactor(term->factor);
	if (term->type == 1) {
		printf("*");
	} else if (term->type == 2) {
		printf("/");
	}
	if (term->type > 0) {
		printTerm(term->term);
		free(term->term);
	}
	free(term->factor);
}

void printFactor(struct nodeFactor* factor) {
	if (factor->type == 0 || factor->type == 1) {
		printf("%s", factor->id);
		free(factor->id);
		if (factor->type == 1) {
			printf("[");
			printExpr(factor->expr);
			free(factor->expr);
			printf("]");
		}
	} else if (factor->type == 2) {
		printf("%d", factor->constant);
	} else if (factor->type == 3) {
		printf("(");
		printExpr(factor->expr);
		free(factor->expr);
		printf(")");
	} else {
		printf("in()");
	}
}