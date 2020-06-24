/* routines for doing various useful things involving file i/o */

/* contains the following routines: */
/*	cat : file x file	*/
/*		- outputs contents of first fiel to second file */

/*	GetString : file x buffer -> int 	*/
/*		- attempts to read a newline-terminated string from file */
/*		  and places it in buffer (minus the the newline, which is */
/*		  discarded). If EOF is encountered before a newline then */
/*		  the chars read are still placed in the buffer, but the */
/*		  status returned is 1 (TRUE) (otherwise it is 0 (FALSE)) */

/*	GetStringN : file x buffer x n -> int 	*/
/*		- same as GetString except that no more than n chars */
/* 		  are inserted into the buffer. If the buffer overflows */
/* 		  then a status of 2 is returned */

#include <stdio.h>

cat(f1,f2)
FILE *f1,*f2;
/* writes contents of f1 to f2 */
{
char c;
	while ((c = getc(f1)) != EOF) {
		putc(c,f2);
	}
}

int GetString(f,buf)
FILE *f;
char buf[];
{
char c;
	while (((c = getc(f)) != EOF) && (c != '\n')) {
		*buf = c;
		buf += 1;
	}
	*buf = '\0';
	if (c == '\n') {
		return(0);
	}
	return(1);
}

int GetStringN(f,buf,n)
FILE *f;
char buf[];
int n;
{
char c;
int i;

	i = 0;
	while (((c = getc(f)) != EOF) && (c != '\n') && (i < n)) {
		*buf = c;
		buf += 1;
		i += 1;
	}
	*buf = '\0';
	if (c == '\n') {
		return(0);
	}
	if (c == EOF) {
		return(1);		
	}
	return(2);	/* if we get here then c is neither newline nor */
			/* EOF, which indicates that the buffer overflowed */
}

		
