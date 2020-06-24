/* mkextern */
/* Written by Martin Davis	3/5/85 */
/* Completely hacked by Karl Klashinsky, 17/2/86, in order to make it */
/* a routine, as opposed to a main program.                           */

/* make a "extern.i" file from the "var.i" in the module argv */

/* the module name must be the absolute filename */
/* of the module, minus the ".m" suffix */

#include <stdio.h>
#include "globals.h"
#include "macros.h"

#define READ 4		/* codes for specifying access mode to "access" */
#define WRITE 2

char *stringcat();
int GetString();

mkextern(argv)
char *argv;
{
char *mod,*extName,*varName,buf[1025];
FILE *var,*ext;

        fprintf(stdout,"mkextern %s\n",argv);

	/* check that top-level module exists */

	mod = stringcat(argv,".m");
	if (access(mod,READ) != 0) {
		fprintf(stderr,"mkextern: can't find module %s\n",mod);
		return(FALSE);
	}

	/* open "var.i" file */

	varName = stringcat(mod,"/var.i");
	if ((var = fopen(varName,"r")) == NULL) {
		fprintf(stderr,"mkextern: can't read file %s\n",varName);
		return(FALSE);
	}


	extName = stringcat(mod,"/extern.i");
	if ((ext = fopen(extName,"w")) == NULL) {
		fprintf(stderr,"mkextern: can't open file %s\n",extName);
		return(FALSE);
	}
	
	/* construct "extern.c" file */

        while (!(GetString(var,buf)))
          if (isalpha(buf[0]))         /* if line starts with an alpha char,  */
             fprintf(ext,"extern %s\n",buf);/* its probably a var declaration */
          else 
             fprintf(ext,"%s\n",buf);

	/*** wrap-up ****/

	fclose(var);
	fclose(ext);

	return(TRUE);
}

