/* routines to do useful things with filenames */

/* the routines a written so that they may be nested to any level during */
/* a call, BUT the result of calling a routine is not guaranteed after */
/* any further, non-nested calls to the routines */

/* the routines included are: */

/* NameRoot : string -> string */
/*          - removes any trailing ".xxx" suffix from */
/*            filename "/a/b/f.xxx", where f has at least */
/*            one character */
/*            e.g. when passed "a/b/c.d" returns "a/b/c" */
/*            when passed "a/.b" returns "a/.b" */

/* NameExt : string -> string */
/*           - returns the extension (if any) of a filename */
/*             (returns "" if no extension */
/*             e.g. when passed "a/b.c" returns "c" */

/* NameFile : string -> string */
/*          - removes all leading pathname components from a filename s */
/*            e.g. when passed "a/b/c.d" returns "c.d" */

/* NameDir : string -> string */
/*         - returns name of directory containing file s */
/*           e.g. when passed "a/b/c.d" returns "a/b" */
/*           when passed "/s" returns "" */

/* NamePath : string -> string */
/*          - returns path of directory containing file s */
/*            e.g. when passed "a/b/c.d" returns "a/b/" */
/*            when passed "/s" returns "/" */

/* NameHead : string -> string */
/*          - returns head directory of pathname */
/*            e.g. when passed "a/b/c.d" returns "a" */
/*            when passed "/s" returns "" */

/* NameTail : string -> string */
/*          - returns tail of pathname */
/*            e.g. when passed "a/b/c.d" returns "b/c.d" */
/*            when passed "/s" returns "s" */

/* AbsPath : string -> string */
/*         - returns the absolute path (i.e. one starting at / ) */
/*           of given file.  (actually, returns canonical path, */
/*           i.e. one starting at root and containing no */
/*           '..'s or '.'s).  Uses system routine "getwd" and */
/*           will bomb completely if this fails */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "filename.h"
#include "globals.h"

char buffer[MAXLINE]; /* big enough for any valid filename */

/* removes any trailing ".xxx" suffix from s */
/* e.g. when passed "a/b/c.d" returns "a/b/c" */
char *
NameRoot(char *s)
{
  int i;
  (void)strcpy(buffer,s);
  i = strlen(buffer) - 1;
  while (i >= 0 && buffer[i] != '/' && buffer[i] != '.')
  {
    i -= 1;
  }
  if (i > 0 && buffer[i] == '.' && buffer[i-1] != '/')
  {
    buffer[i] = '\0';
  }
  return buffer;
}

/* returns extension of filename */
char *
NameExt(char *s)
{
  int i;
  (void)strcpy(buffer,s);
  i = strlen(buffer) - 1;

  /* search for last dot or slash in name */
  while (i >= 0 && buffer[i] != '/' && buffer[i] != '.')
  {
    i -= 1;
  }
  if (i > 0 &&
      buffer[i] == '.' &&  /* do if we are at a dot, not a / */
      buffer[i-1] != '/')  /* check for invisible (dot) */
                                /* filenames */
  {
    return &buffer[i + 1];
  }
  return "";
}

/* removes all leading pathname components from a filename s */
/* e.g. when passed "a/b/c.d" returns "c.d" */
char *
NameFile(char *s)
{
  static char buffer[MAXLINE];
  int i;
  int j;
  int k;
  int len;

  len = strlen(s);
  i = len - 1;
  while (i >= 0 && s[i] != '/')
  {
    i -= 1;
  }
  i += 1;
  j = 0;
  for (k = i; k <= len; k++)
  {
    buffer[j] = s[k];
    j += 1;
  }
/*printf("NameFile: string is \"%s\", buffer is \"%s\"\n",s,buffer);*/
  return buffer;
}

/* returns path of directory containing file s */
/* e.g. when passed "a/b/c.d" returns "a/b" */
char *
NameDir(char *s)
{
  static char buffer[MAXLINE];
  int i;
  int j;

  i = strlen(s) - 1;
  while (i >= 0 && s[i] != '/')
  {
    i -= 1;
  }
  for (j = 0; j < i; j++)
  {
    buffer[j] = s[j];
  }
  buffer[i] = '\0';
  return buffer;
}

/* returns path of files in directory containing file s */
/* e.g. when passed "a/b/c.d" returns "a/b/" */
char *
NamePath(char *s)
{
  static char buffer[MAXLINE];
  int i;
  int j;

  i = strlen(s) - 1;
  while (i >= 0 && s[i] != '/')
  {
    i -= 1;
  }
  for (j = 0; j <= i; j++)
  {
    buffer[j] = s[j];
  }
  buffer[i + 1] = '\0';
  return buffer;
}

/* adds "name" to the path name in "buffer" */
static int
AddLevel(char *buffer, char *name)
{
  (void)strcat(buffer,"/");
  (void)strcat(buffer,name);
}

/* subtracts a directory level from the pathname in buffer */
static int
SubtractLevel(char *buffer)
{
  int i;
  i = strlen(buffer) - 1;
  while (i != 0 && buffer[i] != '/')
  {
    i -= 1;
  }
  buffer[i] = '\0';
}

int
IsNullName(char *name)
{
  return strlen(name) == 0;
}

char *
AbsPath(char *file)
{
  static char buffer[MAXLINE];
  char hd[MAXLINE];

  if (file[0] != '/')
  {
    if (getwd(buffer) == 0)
    {
      fprintf(stderr,"%s\n",buffer);
      exit(1);
    }
  }
  else
  {
    buffer[0] = '\0';
    file = NameTail(file);
  }
  while (! IsNullName(file))
  {
    (void)strcpy(hd,NameHead(file));
    file = NameTail(file);
    if (strcmp(hd,"..") == 0)
    {
      SubtractLevel(buffer);
    }
    else if (strcmp(hd,".") == 0)
    {
      /* do nothing */
    }
    else
    {
      AddLevel(buffer,hd);
    }
  }
  return buffer;
}

char *
NameTail(char *name)
{
  int i;
  int len;
  if (name[0] == '/')
  {
    return &name[1];
  }
  len = strlen(name);
  i = 0;
  while (i < len && name[i] != '/')
  {
    i += 1;
  }
  if (name[i] == '/')
  {
    i += 1;
  }
  return &name[i];
}


char *
NameHead(char *name)
{
  int i;
  int len;
  static char buffer[MAXLINE];
  len = strlen(name);
  i = 0;
  while (i < len && name[i] != '/')
  {
    buffer[i] = name[i];
    i += 1;
  }
  buffer[i] = '\0';
  return buffer;
}
