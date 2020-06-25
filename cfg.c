/* cfg: Configure a single module.
        Written by Karl Klashinsky, Feb 6, 1986.
        Original version in shell script written by Martin Davis     */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "options.h"
#include "globals.h"

void PrintUsage ()
{
	fprintf (stderr, "usage : cfg [(one of) -g -p -O] module\n") ;
	exit (-1) ;
}

void main (argc, argv)
int argc;
char *argv[];

 {  register int i;
    char *UseList[MAXUSE] ;

    UseList[0] = ParseArgs(argc, argv) ;

    if (!(BuildModule(UseList)))
       exit(1);

    for (i=1 ; UseList[i] != (char *)0 ; i++)
       if (older(stringcat(UseList[i],".m/extern.i"),
		 stringcat(UseList[i],".m/var.i")))
          if (!(mkextern(UseList[i])))
             exit(1);

    if (!(CompileProg(UseList)))
       exit(1);
  }
