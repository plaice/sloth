#include <stdio.h>
#include <sys/wait.h>
#include "globals.h"

char *stringcat();

main (argc,argv)
int argc ;
char *argv[];

  {char *ImpList[MAXUSE], *ccList[MAXUSE+5];
   int  ImpCount ;

   if (argc != 2)
     {fprintf(stderr,"usage : qdlkm modname\n");
      exit(1);
     }

   ImpList[0] = argv[1] ;

   if (!(BuildModule(&ImpCount,ImpList)))
      exit(1) ;

   ccList[0] = "cc" ;
   ccList[1] = "-o" ;
   ccList[2] = ImpList[0] ;
   ListCat(ImpList,".m/prog.o",ImpCount,(ccList+3));
   ccList[ImpCount+3] = stringcat(ImpList[0],".m/main.c");
   ccList[ImpCount+4] = "-lm" ;
   ccList[ImpCount+5] = (char *)0;

   switch (fork())
     { case 0 : for (i = 0 ; i < ImpCount+5 ; i++)
                   fprintf(stdout,"%s ",ccList[i]);
                fprintf(stdout,"\n");
                execv("/bin/cc",ccList) ;
                fprintf(stderr,"System error in execv\n");
                exit(1);
       case -1: fprintf(stderr,"System error in fork\n");
                exit(1);
       default: wait(0) ;
     }
 }
