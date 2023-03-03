#include <stdio.h>
#include "tree.h"
#include "semantic.h"
#include "parser.h"
#include "scanner.h"

int main(int argc, char* argv[]) {
    // Open the scanner.
    scanner_open(argv[1]);

    // 
    struct nodeProcedure p;

    // Generate parse tree.
    parseProcedure(&p);

    /*
        Verifying that every ID being used has been declared.
        Verifying that IDs were declared with the appropriate type (integer or record) for how they are being used.
        Checking for "doubly-declared" variable names.
        Checking that IDs declared as record are used appropriately in assignments.
    */
    semanticProcedure(&p);

    printProcedure(&p);

    scanner_close();

    return 0;
}