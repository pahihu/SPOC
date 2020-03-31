/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Opening a file on environment variable search path
||@(#)   System : OCC2C
||@(#) Filename : Fopenenv.h
||@(#)  Version : 1.50
||@(#)     Date : 2/18/97
\*@(#)====================================================*/
/*}}}*/

extern void *malloc_check(size_t size);

extern FILE *fopenenv(char *file, char *opts, char *env, char *def, char **name);

extern int   ftestenv(char *file, char *opts, char *env, char *def, char **name);

#define EXIT_ERR (-1)
