/* mkmain */
/* Written by Martin Davis 3/5/85 */
/* csloth version */

/* takes the modules named as arguments and assembles a C main program */
/* containing calls to the Init routines for the modules */
/* This program is called "main.c" and is placed in the directory of the */
/* top-level module */

/* The first module named is assumed to be the top-level module */
/* Its initialization code must be executed last */

/* args are:  module1 module2 module3 ... */

/* the imported module names must be the absolute filenames */
/* of the modules, minus the ".m" suffix */

#include <stdio.h>
#include "globals.h"
#include "macros.h"

char *stringcat();
char *NameFile();

int
mkmain(char *List[])
{
  char *progName;
  int i;
  FILE *progFile;

  progName = stringcat(List[0],".m/main.c");
  if ((progFile = fopen(progName,"w")) == NULL)
  {
    fprintf(stderr,"mkmain: can't open file %s\n",progName);
    return FALSE;
  }

  /* construct contents of "main.c" file */
  fprintf(progFile, "main(argc,argv)\nint argc;\nchar *argv[];\n{\n");
  for (i = 1; List[i] != (char *) 0; i++)
    fprintf(progFile, "\tInit%s(argc,argv);\n", NameFile(List[i]));
  fprintf(progFile, "\tInit%s(argc,argv);\n}\n", NameFile(List[0]));
  fclose(progFile);
  return TRUE;
}
