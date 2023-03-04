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

int main(int argc, char *argv[]) {  
  
  // Copy the filename to a static char*, to then pass to execute for whenever IN() is called, so we can open a 2nd scanner.
  filename = strdup(argv[1]);

  scanner_open(argv[1]);

  struct nodeProcedure p;

  //printf("PARSING\n");
  
  parseProcedure(&p);
  // printf("PRINTING\n");
  
  // semanticProcedure(&p);

  printProcedure(&p);

  // printf("EXECUTING\n");

  // executeProcedure(&p, filename);

  // free(&p); // Can't really free a non-heap object. 
  
  // Scanning, parsing, printing is done, release memory
  scanner_close();
  return 0;
}