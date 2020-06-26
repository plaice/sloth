#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "string.h"
#include "filename.h"
#include "globals.h"

extern void PrintUsage();

int compileAll = FALSE;  /* if true, compile all files regardless */
                         /* of their last-modified date */

static char *OptionChar = " ";

char *
ParseArgs (int ac, char *av[])
{
  int i;
  char *cp;
  char *retc;

  if (ac < 2)
    PrintUsage();

  retc = (char *) 0;

  for (i=1; i<ac; i++)
    if (*(cp=av[i]) == '-')
    {
      for (cp++; *cp != '\0'; cp++)
      {
        switch (*cp)
        {
          case 'g':
          case 'p':
          case 'O':
            OptionChar[0] = *cp;
            break;
          case 'a':
            compileAll = TRUE;
            break;
          default:
            PrintUsage();
        }
      }
    }
    else if (retc != (char *) 0)
      PrintUsage();
    else
      retc = mkstring(av[i]);
    return retc;
}

char *
Option(void)
{
  if (strcmp(OptionChar," ") == 0)
    return (char *) 0;
  else
    return stringcat("-", OptionChar);
}

char *
OptObj(void)
{
  char *st;
  if (strcmp(OptionChar," ") == 0)
    return "prog.o";
  else
    return stringcat("prog", stringcat(OptionChar, ".o"));
}

char *
OptSource(char *dotcfile)
{
  char  *f;
  FILE *F;

  if (strcmp(OptionChar," ") == 0)
    return mkstring(NameFile(dotcfile));
  else
  {
    f = stringcat(stringcat(NameRoot(dotcfile),OptionChar),".c");
    if ((F=fopen(f,"r")) == (FILE *)NULL)
      (void) symlink(NameFile(dotcfile), f);
    else
      fclose(F);
    return NameFile(f);
  }
}
