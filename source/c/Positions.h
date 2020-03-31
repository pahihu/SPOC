/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : GMD Based file Position module
||@(#)   System : Occam 2 C
||@(#) Filename : Positions.h
||@(#)  Version : 1.1
||@(#)     Date : 9/23/93
\*@(#)====================================================*/
/*}}}*/

# ifndef yyPositions
# define yyPositions

/* $Id: Positions.h,v 1.0 1992/08/07 14:31:43 grosch rel $ */

/* $Log: Positions.h,v $
 * Revision 1.0  1992/08/07  14:31:43  grosch
 * Initial revision
 *
 */

/* Ich, Doktor Josef Grosch, Informatiker, Juli 1992 */

# include <stdio.h>

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

#include "StringMem.h"

typedef struct { unsigned short Line, Column; tStringRef File;} tPosition;

extern tPosition NoPosition;
			/* A default position (0, 0).			*/

extern int  Compare	  ARGS((tPosition Position1, tPosition Position2));
			/* Returns -1 if Position1 < Position2.		*/
			/* Returns  0 if Position1 = Position2.		*/
			/* Returns  1 if Position1 > Position2.		*/

extern void WritePosition ARGS((FILE * File, tPosition Position));
			/* The 'Position' is printed on the 'File'.	*/

# endif
