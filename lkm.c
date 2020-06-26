#include <stdio.h>
#include <sys/wait.h>
#include "string.h"
#include "options.h"
#include "globals.h"
#include "build.h"
#include "compile.h"
#include "mkextern.h"
#include "older.h"

void
PrintUsage()
{
  fprintf(stderr,"usage : lkm [-pOga] module\n");
  exit(1);
}

int
main (int argc, char *argv[])
{
  char *ImpList[MAXUSE];
  char *UseeImpList[MAXUSE];
  char *ccList[MAXUSE+5];
  int i;
  int ImpCount;
  int UseeImpCount;

  ImpList[0] = ParseArgs(argc,argv);

  if (!(BuildModule(ImpList)))
    exit(1);

  for (i = 0; ImpList[i] != (char *) 0; i++)
    if (older(stringcat(ImpList[i],".m/extern.i"),
              stringcat(ImpList[i],".m/var.i")))
      if (!(mkextern(ImpList[i])))
        exit(1);

  if (!(CompileProg(ImpList)))
    exit(1);

  for (i = 1; ImpList[i] != (char *) 0; i++ )
  {
    UseeImpList[0] = ImpList[i];
    if (!(BuildModule(UseeImpList)))
      exit(1);
    if (!(CompileProg(UseeImpList)))
      exit(1);
  }

  if (older(stringcat(ImpList[0],".m/main.c"),
            stringcat(ImpList[0],".m/uselist")))
  {
    if (!(mkmain(ImpList)))
      exit(1);
  }

  ccList[0] = "cc";
  ccList[1] = "-o";
  ccList[2] = ImpList[0];
  ccList[3] = stringcat(
                ImpList[0],
                stringcat(".m/", OptSource(
                                   stringcat(ImpList[0], ".m/main.c"))));
  ccList[4] = (char *) 0;
  ListCat(ImpList, stringcat(".m/", OptObj()), ccList);

  for (i=0; ccList[i] != (char *) 0; i++);

  ccList[i++] = "-lm";
  ccList[i++] = Option();
  ccList[i] = (char *) 0;

  switch (fork())
  {
    case 0:
      for (i = 0; ccList[i] != (char *) 0; i++)
        fprintf(stdout,"%s ",ccList[i]);
      fprintf(stdout,"\n");
      execv("/bin/cc",ccList);
      fprintf(stderr,"System error in execv\n");
      exit(1);
    case -1:
      fprintf(stderr,"System error in fork\n");
      exit(1);
    default: wait(0);
  }
}
