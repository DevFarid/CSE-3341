#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "core.h"
#include "parser.h"
#include "printer.h"
#include "semantic.h"
#include "execute.h"
#include "tree.h"

static char* filename;

void freeEverything(struct nodeProcedure*);

int main(int argc, char *argv[]) {  
  
  // Copy the filename to a static char*, to then pass to execute for whenever IN() is called, so we can open a 2nd scanner.
  filename = strdup(argv[1]);

  scanner_open(argv[1]);

  struct nodeProcedure p;

  //printf("PARSING\n");
  parseProcedure(&p);
  
  // printf("SEMANTIC CHECKS\n"")
  // semanticProcedure(&p);


  // printf("EXECUTING\n");
  executeProcedure(&p, filename);

  // printf("PRINTING\n");
  // printProcedure(&p);


  // free(&p); // Can't really free a non-heap object. 
  // freeEverything(&p);

  // Scanning, parsing, printing is done, release memory


  scanner_close();
  return 0;
}

void freeEverything(struct nodeProcedure* p) {

}