/* mm (modify module) and vm (view module) command */
/* Wriiten by Martin Davis        8/11/84  */

/* mm takes pairs of block code lists and module names and  */
/* invokes editor on the appropriate files */

/* vm takes pairs of block code lists and module names and invokes display  */
/* command on the appropriate files */

/* the block codes are */
/*     v - var.i       */
/*     d - define.i    */
/*     p - proc.i      */
/*     b - body.i      */
/*     c - prog.c      */
/*     i - import      */
/*     u - uselist     */
/*     w - words       */
/*     g - docs        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "globals.h"
#include "filename.h"

extern char **environ;

int argPos;
char *commandName;
char *command;

void
DoHelp(void)
{
#if VM
  fprintf(stderr,"vm: view module.  Syntax is:\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"         vm code-list name {code-list name}\n");
#else
  fprintf(stderr,"mm: modify module.  Syntax is:\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"         mm code-list name {code-list name}\n");
#endif

  fprintf(stderr,"\n");
  fprintf(stderr,"Codes are:\n");
  fprintf(stderr,"    v - var.i\n");
  fprintf(stderr,"    d - define.i\n");
  fprintf(stderr,"    p - proc.i\n");
  fprintf(stderr,"    b - body.i\n");
  fprintf(stderr,"    c - prog.c\n");
  fprintf(stderr,"    i - import\n");
  fprintf(stderr,"    u - uselist\n");
  fprintf(stderr,"    w - words\n");
  fprintf(stderr,"    g - docs\n");
}

void
DoIt(char code, char *name, int label)
{
  char *file;
  char buf[2000];
  int pid;
  int status;
  switch (code)
  {
    case 'v':
      file = "var.i";
      break;
    case 'd':
      file = "define.i";
      break;
    case 'p':
      file = "proc.i";
      break;
    case 'b':
      file = "body.i";
      break;
    case 'c':
      file = "prog.c";
      break;
    case 'i':
      file = "import";
      break;
    case 'u':
      file = "uselist";
      break;
    case 'w':
      file = "words";
      break;
    case 'g':
      file = "docs";
      break;
    default:
      fprintf(stderr,"%s: unknown code '%c'\n",commandName,code);
      exit(1);
  }
  sprintf(buf,"%s.m/%s",name,file);

#if VM
  if (label)
  {
    printf("%s:\n",buf);
  }
#endif

  /* do fancy system stuff to invoke editor */
  switch (pid = fork())
  {
    case 0:
      execl(command, NameFile(command), buf, NULL);
      /* LATER: take command from shell variable */
      fprintf(stderr,"whoops! system error in execl\n");
      exit(1);
    case -1:
      fprintf(stderr,"whoops! system error in fork\n");
      exit(1);
    default:
      while (wait(&status) != pid);
  }
}

struct sigvec {
  void (*sv_handler)(int); /* Signal disposition */
  int    sv_mask;          /* Signals to be blocked in handler */
  int    sv_flags;         /* Flags */
};


int
main(int argc, char *argv[])
{
  char *name;
  char code;
  int i;
  int label;
  struct sigvec NewValue;  /* Kbk:Jan 25,86 To pass to sigvec routine     */

#if VM
  commandName = "vm";
  command = getenv("VIEWER");
  if (command == 0)
  {
    command = "/bin/cat";
  }
#else
  commandName = "mm";
  command = getenv("EDITOR");
  if (command == 0)
  {
    command = "/usr/ucb/vi";
  }
#endif

  if (strcmp(argv[1],"-h") == 0)
  {
    DoHelp();
    exit(1);
  }

  if (argc < 3 || (argc - 1) % 2 != 0)
  {
    fprintf(stderr,"usage: %s vdpbciuwg module_name ...\n",commandName);
    exit(1);
  }

  /* enable labelling if more than one file is being dealt with */
  if ((argc > 3) || (strlen(argv[1]) > 1))
  {
    label = TRUE;
  }
  else
  {
    label = FALSE;
  }

  /* KbK: Jan 24,86  next 3 lines */
  NewValue.sv_handler = SIG_IGN;
  NewValue.sv_mask = NewValue.sv_flags = 0;
  sigvec(SIGINT, &NewValue, (struct sigvec *) NULL);

  argPos = 1; /* points to first word of current code/name pair */
  while (argPos < argc)
  {
    for (i = 0; i < strlen(argv[argPos]); i++)
    {
      code = argv[argPos][i];
      name = argv[argPos + 1];
      DoIt(code,name,label);
    }
    argPos += 2;
  }

  /* KbK: Jan 24,86 to restore default interrupt handler */
  NewValue.sv_handler = SIG_DFL;
  NewValue.sv_mask = NewValue.sv_flags = 0;
  sigvec(SIGINT, &NewValue, (struct sigvec *) 0);
}
