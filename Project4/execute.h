void executeProcedure(struct nodeProcedure*, char*);

void executeFuncDecSeq(struct nodeFuncSeq*);

void executeFuncDec(struct nodeFunc*);

void executeDeclSeq(struct nodeDeclSeq*);

void executeDecl(struct nodeDecl*);

void executeDeclInt(struct nodeDeclInt*) ;

void executeDeclRec(struct nodeDeclRec*);

void executeStmtSeq(struct nodeStmtSeq*);

void executeStmt(struct nodeStmt*);

void executeCall(struct nodeCall*);

void executeAssign(struct nodeAssign*);

int executeIndex(struct nodeIndex*);

void executeOut(struct nodeOut*);

void executeIf(struct nodeIf*);

void executeLoop(struct nodeLoop*);

int executeCond(struct nodeCond*);

int executeCmpr(struct nodeCmpr*);

int executeExpr(struct nodeExpr*);

int executeTerm(struct nodeTerm*);

int executeFactor(struct nodeFactor*);