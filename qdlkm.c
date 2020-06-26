#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "globals.h"
#include "build.h"
#include "string.h"

char *stringcat();

int
main (int argc, char *argv[])
{
  char *ImpList[MAXUSE];
  char *ccList[MAXUSE+5];

  if (argc != 2)
  {
    fprintf(stderr,"usage : qdlkm modname\n");
    exit(1);
  }

  ImpList[0] = argv[1];

  if (!(BuildModule(ImpList)))
    exit(1);

  ccList[0] = "cc";
  ccList[1] = "-o";
  ccList[2] = ImpList[0];
  ListCat(ImpList, ".m/prog.o", (ccList+3));
  ccList[ImpCount+3] = stringcat(ImpList[0], ".m/main.c");
  ccList[ImpCount+4] = "-lm";
  ccList[ImpCount+5] = (char *) 0;

  switch (fork())
  {
    case 0:
      for (int i = 0; i < ImpCount+5; i++)
        fprintf(stdout, "%s ", ccList[i]);
      fprintf(stdout, "\n");
      execv("/bin/cc", ccList);
      fprintf(stderr, "System error in execv\n");
      exit(1);
    case -1:
      fprintf(stderr, "System error in fork\n");
      exit(1);
    default:
      wait(0);
  }
}
