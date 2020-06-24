/* routines to do useful things with filenames */

/* the routines a written so that they may be nested to any level during */
/* a call, BUT the result of calling a routine is not guaranteed after */
/* any further, non-nested calls to the routines */

/* the routines included are: */

/*	NameRoot : string -> string */
/*		- removes any trailing ".xxx" suffix from 	*/
/*			filename "/a/b/f.xxx", where f has at least */
/*			one character	*/
/* 			e.g. when passed "a/b/c.d" returns "a/b/c" */
/*			     when passed "a/.b"	returns "a/.b" */

/*	NameExt : string -> string */
/*		- returns the extension (if any) of a filename */
/*			(returns "" if no extension 		*/
/*			e.g. when passed "a/b.c" returns "c" 	*/

/*	NameFile : string -> string */
/* 		- removes all leading pathname components from a filename s */
/* 			e.g. when passed "a/b/c.d" returns "c.d" */

/* 	NameDir : string -> string */
/*		- returns name of directory containing file s */
/*			e.g. when passed "a/b/c.d" returns "a/b" */
/*				when passed "/s" returns "" */

/* 	NamePath : string -> string */
/*		- returns path of directory containing file s */
/*			e.g. when passed "a/b/c.d" returns "a/b/" */
/*				when passed "/s" returns "/" */

/* 	NameHead : string -> string */
/*		- returns head directory of pathname */
/*			e.g. when passed "a/b/c.d" returns "a" */
/*				when passed "/s" returns "" */

/* 	NameTail : string -> string */
/*		- returns tail of pathname */
/*			e.g. when passed "a/b/c.d" returns "b/c.d" */
/*				when passed "/s" returns "s" */

/*	AbsPath : string -> string */
/*		- returns the absolute path (i.e. one starting at / ) */
/*			of given file.  (actually, returns canonical path, */
/* 			i.e. one starting at root and containing no */
/* 			'..'s or '.'s).  Uses system routine "getwd" and */
/* 			will bomb completely if this fails */


char *NameHead(),*NameTail(), *NameRoot() , *NameExt(), *NameFile(), *NameDir(),
	*NamePath(), *AbsPath(); 
