#ifndef TREE_H
#define TREE_H

/*
*
* Define structs to serve as your tree nodes
*
*/


/*
    LANGUAGE GRAMMAR

    <procedure> ::= procedure ID is <decl-seq> begin <stmt-seq> end
    <decl-seq> ::= <decl > | <decl><decl-seq>
    <stmt-seq> ::= <stmt> | <stmt><stmt-seq>
    <decl> ::= <decl-integer> | <decl-record>
    <decl-integer> ::= integer ID ;
    <decl-record> ::= record ID ;
    <stmt> ::= <assign> | <if> | <loop> | <out>
    <assign> ::= id <index> := <expr> ; | id := new record [ <expr> ]; | id := record id;
    <index> ::= [ <expr> ] | epsilon
    <out> ::= out ( <expr> ) ;
    <if> ::= if <cond> then <stmt-seq> end | if <cond> then <stmt-seq> else <stmt-seq> end
    <loop> ::= while <cond> do <stmt-seq> end
    <cond> ::= <cmpr> | not <cond> | <cmpr> or <cond> | <cmpr> and <cond>
    <cmpr> ::= <expr> = <expr> | <expr> < <expr>
    <expr> ::= <term> | <term> + <expr> | <term> – <expr>
    <term> ::= <factor> | <factor> * <term> | <factor> / <term>
    <factor> ::= id | id [ <expr> ] | const | ( <expr> ) | in ( )
*/

struct nodeExpr {
    int type; // 0 = term, 1 = plus, 2 = minus
    struct nodeTerm* t;
    struct nodeExpr* l;
    struct nodeExpr* r;
};

struct nodeTerm {
    int type; // 0 = factor, 1 = mult, 2 = div
    struct nodeFactor* f;
    struct nodeTerm* l;
    struct nodeTerm* r;
};

struct nodeFactor {
    int type; // 0 = ID, 1 = const, 2 = in, 3 = expr
    char* id;
    int val;
    struct nodeExpr* expr;
};

struct nodeCmpr {
    int type; // 0 = eq, 1 = lt, 2 = not, 3 = or, 4 = and
    struct nodeExpr* l;
    struct nodeExpr* r;
    struct nodeCmpr* c;
};

struct nodeCond {
    struct nodeCmpr* c;
};

struct nodeLoop {
    struct nodeCond* cond;
    struct nodeStmtSeq* ss;
};

struct nodeIf {
    struct nodeCond* cond;
    struct nodeStmtSeq* ss1;
    struct nodeStmtSeq* ss2;
};

struct nodeAssign {
    char* id;
    struct nodeExpr* expr;
    int isIndex;
    struct nodeExpr* index;
    int isNewRecord;
    int isRecordAssign;
    char* recordId;
};

struct nodeOut {
    struct nodeExpr* expr;
};

struct nodeStmt {
    int type; // 0 = assign, 1 = if, 2 = loop, 3 = out
    struct nodeAssign* a;
    struct nodeIf* i;
    struct nodeLoop* l;
    struct nodeOut* o;
};

struct nodeStmtSeq {
    struct nodeStmt* stmt;
    struct nodeStmtSeq* next;
};

struct nodeDecl {
    int type; // 0 = integer, 1 = record
    char* id;
};

struct nodeDeclSeq {
    struct nodeDecl* decl;
    struct nodeDeclSeq* next;
};

struct nodeProcedure {
    struct nodeDeclSeq* declarations;
    struct nodeStmtSeq* statements;
    char* name;
};

#endif