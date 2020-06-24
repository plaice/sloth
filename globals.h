#define NULL 0
#define TRUE 1
#define FALSE 0
#define MAXINT 0x7FFFFFFF
#define MININT 0x80000000
#define MAXUSE 100          /* maximum number of modules that a modulecan use */
#define MAXLINE 1025        /* maximum length of a line from a file           */
#define REP(t) t
#define TYPE(t) typedef struct REP(t)rep * t
/** example of use:  **/
/** TYPE(ftree);     **/
/**   ftree t;       **/           /** Feb 3, 1982 **/
