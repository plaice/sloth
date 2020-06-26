/* June 23, 1983 added stringcat */
/* This is a static type for string space */
#include "globals.h"
#define BUFSIZE 5012

#include <string.h>
#include <stdlib.h>

static int remaining = -1; /* no space to begin */
static char *freeString;

static void
getmore(void)
{
  freeString = (char *) malloc(BUFSIZE);
  remaining = BUFSIZE;
}

char *
mkstring(char *s)
{
  int len;
  char *t;
  len = strlen(s);
  if (len >= remaining)
    getmore();
  t = freeString;
  (void)strcpy(freeString,s);
  freeString += len+1;
  remaining -= len+1;
  return t;
}

int
stringeq(char *s1, char *s2)
{
   if(strcmp(s1,s2) == 0)
     return TRUE;
   return FALSE;
}

char *
stringcat(char *s1, char *s2)
{
  int len;
  char *t;
  len = strlen(s1) + strlen(s2);
  if(len >= remaining) getmore();
  t = freeString;
  (void)strcpy(freeString,s1);
  (void)strcat(freeString,s2);
  freeString += len+1;
  remaining -= len+1;
  return t;
}

void
ListCat(char *First[], char *tail, char *Second[])
{
  while (*Second != (char *) 0) Second++;
  while (*First != (char *) 0)
    *(Second++) = stringcat(*(First++), tail);
  *Second = (char *) 0;
}
