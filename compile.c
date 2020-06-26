#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "string.h"
#include "options.h"
#include "globals.h"
#include "older.h"

int
CompileProg(char *List[])
{
  char *CheckList[MAXUSE*2+7];
  char *progfile;
  char *ccList[5];
  int pid;
  int status;

  progfile = OptSource(stringcat(List[0],".m/prog.c"));

  CheckList[0] = stringcat(List[0],stringcat(".m/",OptObj()));
  CheckList[1] = stringcat(List[0],".m/uselist");
  CheckList[2] = stringcat(List[0],".m/define.i");
  CheckList[3] = stringcat(List[0],".m/var.i");
  CheckList[4] = stringcat(List[0],".m/proc.i");
  CheckList[5] = stringcat(List[0],".m/body.i");
  CheckList[6] = (char *) 0;

  ListCat((List+1),".m/define.i",CheckList);
  ListCat((List+1),".m/extern.i",CheckList);

  ccList[0] = "/bin/cc";
  ccList[1] = "-c";
  ccList[2] = progfile;
  ccList[3] = Option ();
  ccList[4] = (char *) 0;

  if (Older(CheckList))
  {
    printf("compiling %s.m\n",List[0]);

    switch (pid=fork())
    {
      case 0:
        fprintf(stdout,"cd %s.m \ncc -c %s %s\n",
                List[0], progfile, ccList[3]);
        chdir(stringcat(List[0], ".m"));
        execv(ccList[0], ccList);
        fprintf(stderr,"System error in execl");
        return FALSE;

      case -1:
        fprintf(stderr,"System error in fork");
        return FALSE;

      default:
        while (wait(&status) != pid);
        if (status)
          return FALSE;
        else
          return TRUE;
    }
  }
/*
  else
    fprintf(stdout,"%s.m/%s is up to date.\n",List[0],OptObj());
*/

  return TRUE;
}
