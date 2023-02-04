#include <stdio.h>

int main(int argc, char* argv[]) {

    scanner_open(argv[1]);

    struct nodeProcedure p;

    parseProcedure(&p);

    semanticProcedure(&p);

    printProcedure(&p);

    scanner_close();

    return 0;
}