/* a function that can be used to determine if a file (the "dependent file" */
/* that is generated from a set of other files (the "parent files") should */
/* be regenerated because one of the parent files has been changed since the */
/* dependent file was last generated */

/* usage: older(n,a) where */
/*        n is the number of valid elements in the array a */
/*        a is an array of pointers to strings.  The first string */
/*        is the name of the dependent file, */
/*        the remaining n-1 are the names of the parent files */

/* If all files exist and file is newer than all of the files a[1]..a[n] */
/*  then false (0) is returned (i.e. DON'T regenerate) */

/* Otherwise true (1) is returned (i.e. DO regenerate) */

/* Note that if one of the parent files is missing, or if the dependent file */
/* does not exist, the status returned will be true.  The idea here is that */
/* if files are missing we don't know what the status of the dependent file is */
/* relative to them, so to be safe the dependent should be regenerated */

#include <sys/types.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

int
Older(char *a[])
{
  struct stat f;
  struct stat dependent;
  int i;

  /* debug stuff
  printf("older: ");
  for (i = 0; a[i] != (char *) 0; i++)
    printf("%s ",a[i]);
  */

  if (stat(a[0],&dependent) != 0)
    return TRUE;

  for (i = 1; a[i] != (char *) 0; i++)
    if (stat(a[i],&f) != 0)
      return TRUE;
    else
      if (dependent.st_mtime < f.st_mtime)
        return TRUE;

  /* debug stuff
  printf("--- FALSE\n");
  */

  return FALSE;
}

int
older(char *first, char *second)
{
  char *List[2];
  List[0] = first;
  List[1] = second;
  List[2] = (char *) 0;
  return Older(List);
}
