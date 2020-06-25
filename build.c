/* build.c */

/* History: */
/* 	26/9/86  modified by Martin Davis to fix bug ("prog.c" file */
/*		not created in certain situations) */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>                                                                                                       
#include <sys/stat.h>
#include "string.h"
#include "filename.h"
#include "globals.h"

extern int compileAll;

static void FinishProg() ;
static char *modList[MAXUSE];/* data structures to store list of used modules */
static int nMods ;

static char *markedList[MAXUSE] ;
static int nMarked ;

static int Marked(mod)
char *mod;
{
 int i;
   for (i = 0; i < nMarked ; i++) 
      if (stringeq(markedList[i],mod)) 
         return(TRUE);
	
   return(FALSE);
}

#define NonNull(s) (*s != '\0')

static int MakeUseList(mod) /* generates list of all modules used by mod */
char *mod;
 { char *imports[MAXUSE],buffer[MAXLINE],*modPath,*importfile,*dir;
   FILE *f;
   register int i,importPtr,status;

   modPath = mkstring(AbsPath(mod));
   dir = stringcat(modPath,".m/..");	/* want to change to dir */
					/* ABOVE module dir */
	
   if (! Marked(modPath))
     {
      markedList[nMarked++] = modPath;
	
      /* read import file and store imports */
	
      importfile = stringcat(modPath,".m/import");
      importPtr = 0;
      if ((f = fopen(importfile,"r")) != NULL) 
        {
            do {   
		status = GetString(f,buffer);
		if (NonNull(buffer))
			imports[importPtr++] = mkstring(buffer);
            } while (! status);

         fclose(f);
        }
      else
        {
         fprintf(stderr,"Build Module : can't read %s\n",importfile);
         return(FALSE);
        }

      /* recursively generate imports for import modules */

      for (i = 0; i < importPtr; i++) 
         if (chdir(dir) != 0) 
           {
            fprintf(stderr,"Build Module : can't find module %s.m\n",modPath);
            return(FALSE);
           }
         else 
            if (!(MakeUseList(imports[i]))) 
               return(FALSE);

      modList[nMods++] = modPath;

     }
  return(TRUE);
}

static void FinishProg(prog,arg)
char *arg;
FILE *prog;
 {
  /* local declarations and routines */

  fprintf(prog,"\n#include \"define.i\"\n#include \"var.i\"\n#include \"proc.i\"\n\n");

  /* init routine */

  fprintf(prog,"Init%s(argc,argv)\nint argc;\nchar *argv[];\n{\n#include \"body.i\"\n}\n",arg);

 }

int BuildModule(Imports)
char *Imports[];
 {register int i;
  FILE *useFile,*progFile;
  char *useName,*progName,path[MAXLINE],buffer[MAXLINE],*oldArgs[MAXUSE+2];
	
  oldArgs[0] = stringcat(Imports[0],".m/uselist");
  oldArgs[1] = stringcat(Imports[0],".m/import");
  i = 1;
  if ((useFile = fopen(oldArgs[0],"r")) != NULL) 
    { while (!(GetString(useFile,buffer)))    
        { Imports[i++] = mkstring(buffer);
          oldArgs[i] = stringcat(buffer,".m/import");
        }
   
      Imports[i] = oldArgs[i + 1] = (char *)0 ;

      fclose(useFile);
    }

    /* only recompute uselist if: */
    /*	- it is out of date */
    /* 	- the compileAll flag is set */
    /*	- the "prog.c" is missing (this is a hack - it */
    /*	  is not necessary to recompute the uselist, but it */
    /* 	  is an easy way of forcing the "prog.c" file to be */
    /*	  rewritten) */

    progName = stringcat(Imports[0],".m/prog.c");
if (! Older(oldArgs) && ! compileAll && fexists(progName)) {   
        return(TRUE);
}

  /* save working directory path */

  if (getwd(path) == 0) 
    { fprintf(stderr,"%s\n",path); 	/* path contains error msg */
      return(FALSE);
    }

  nMods = nMarked = 0;

  if (!(MakeUseList(Imports[0])))  /* if failed, there was a serious    */
     return(FALSE);                /* error                             */

  chdir(path);                /* change back to saved directory    */

  /* prepare to write out list to "uselist" file */

  useName = stringcat(Imports[0],".m/uselist");
  if ((useFile = fopen(useName,"w")) == NULL) 
    { fprintf(stderr,"Build Module : can't write %s\n",useName);
      return(FALSE);
    }

  /* prepare to write to "prog.c" file */
     
  if ((progFile = fopen(progName,"w")) == NULL)
    { fprintf(stderr,"Build Module : can't write %s\n",progName);
      return(FALSE);
    }
	
  for (i = 0; i < nMods-1 ; i++) 
    { fprintf(useFile,"%s\n",modList[i]);
      fprintf(progFile,"#include \"%s.m/define.i\"\n",modList[i]);
      fprintf(progFile,"#include \"%s.m/extern.i\"\n",modList[i]);
      Imports[i+1] = modList[i] ;
    }

  Imports[i+1] = (char *)0 ;

  FinishProg(progFile,NameFile(Imports[0]));

  fclose(useFile);
  fclose(progFile);

  return(TRUE);
}

/* fexists(fname) - given a filename, returns a boolean indicating */
/* whether the file exists */

int fexists(fname)
char *fname;
{
struct stat temp;

	return(stat(fname,&temp) == 0);
}
