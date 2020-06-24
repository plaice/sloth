/* June 23, 1983 added stringcat */
/* This is a static type for string space */
#include "globals.h"
#define BUFSIZE 5012

extern char *malloc(), *strcpy(), *strcat() ;

static int remaining = -1; /* no space to begin */
static char *free;
typedef char * string;

static void getmore()
{
    free = (char *) malloc(BUFSIZE);
    remaining = BUFSIZE;
 }

string mkstring(s)
char *s;
{
    int len; char *t;
    len = strlen(s);
    if(len >= remaining)getmore();
    t = free;
    (void)strcpy(free,s);
    free += len+1;
    remaining -= len+1;
    return(t);
 }

int stringeq(s1,s2)
string s1,s2;
{
   if(strcmp(s1,s2) == 0)return(TRUE);
   return(FALSE);
}

string stringcat(s1,s2)
string s1,s2;
{
    int len; string t;
    len = strlen(s1) + strlen(s2);
    if(len >= remaining) getmore();
    t = free;
    (void)strcpy(free,s1);
    (void)strcat(free,s2);
    free += len+1;
    remaining -= len+1;
    return(t);
}

void ListCat(First,tail,Second)
register char *First[],*Second[],*tail;

 { while (*Second != (char *)0 ) Second++ ;

   while ( *First != (char *)0 )
      *(Second++) = stringcat(*(First++),tail);

   *Second = (char *)0 ;
 }
