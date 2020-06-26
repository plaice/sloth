#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"
#include "fileio.h"
#include "filename.h"

/* cserror */
/* Written by Martin Davis 3/5/85 */

/* parses the output from csloth compilation commands to make it usable */
/* by emacs' "parse-errors-in-region" facility */
/* In more detail, replaces the filenames in C error messages by their */
/* absolute pathnames.  If there are "cd" commands in the output it changes */
/* the working directory accordingly, so that the pathnames are always */
/* correct */

#define BUFLEN 4000  /* max length of input line - any extra will */
                     /* be truncated */

int
IsCd(char *buf)
{
  return (strlen(buf) > 3) &&
         buf[0] == 'c' &&
         buf[1] == 'd' &&
         buf[2] == ' ';
}

void
ChDir(char *dir)
{
  if (chdir(dir) != 0)
  {
    fprintf(stderr,"cserror: %s not a directory\n",dir);
    exit(1);
  }
}

int
IsError(char *buf)
{
  return buf[0] == '"';
}

/* prints error message but with absolute filename */
void
PrintNewError(char buf[])
{
  char name[1000];
  int i;
  int restOfMsgPtr;

  i = 0;
  while (buf[i+1] != '"')
  {
    name[i] = buf[i+1];
    i += 1;
  }
  name[i] = '\0';
  restOfMsgPtr = i + 1;
  printf("\"%s%s\n", AbsPath(name), &buf[restOfMsgPtr]);
}

int
main(int argc, char *argv[])
{
  char buf[BUFLEN];
  char str[1000];
  int status;

  status = 0;
  while (status != 1)
  {
    status = GetStringN(stdin,buf,BUFLEN);
    if (! (status == 1 && strlen(buf) == 0))
    {
      if (IsCd(buf))
      {
        ChDir(&buf[3]);
        printf("%s\n",buf);
      }
      else if (IsError(buf))
      {
        PrintNewError(buf);
      }
      else
      {
        printf("%s\n",buf);
      }
    }
  }
}
