/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : GMD Based Error handling unit include
||@(#)   System : Occam 2 C
||@(#) Filename : Errors.h
||@(#)  Version : 1.2
||@(#)     Date : 10/8/93
\*@(#)====================================================*/
/*}}}*/

# ifndef yyErrors
# define yyErrors

/* $Id: Errors.h,v 2.6 1992/08/07 15:28:42 grosch rel $ */

# include "Positions.h"

# define xxNoText		0
# define xxSyntaxError		1	/* error codes		*/
# define xxExpectedTokens	2
# define xxRestartPoint		3
# define xxTokenInserted	4
# define xxParserError          5
# define xxOpError              6
# define xxTypeError            7
# define xxUndeclared           8
# define xxUsage                9
# define xxParallelUsage        10
# define xxNotImplemented       11
# define xxNonConstant          12
# define xxTooManyErrors        13

# define xxFatal		1	/* error classes	*/
# define xxRestriction		2
# define xxError		3
# define xxWarning		4
# define xxRepair		5
# define xxNote			6
# define xxInformation		7

# define xxNone			0
# define xxInteger		1	/* info classes		*/
# define xxShort		2
# define xxLong			3
# define xxReal			4
# define xxBoolean		5
# define xxCharacter		6
# define xxString		7
# define xxSet			8
# define xxIdent		9

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif


extern int ErrorCount;
extern int nowarnings;

extern void (* Errors_Exit) ();
			/* Refers to a procedure that specifies		*/
			/* what to do if 'ErrorClass' = Fatal.		*/
			/* Default: terminate program execution.	*/

extern void StoreMessages ARGS((bool Store));
			/* Messages are stored if 'Store' = TRUE	*/
			/* for printing with the routine 'WriteMessages'*/
			/* otherwise they are printed immediately.	*/
			/* If 'Store'=TRUE the message store is cleared.*/

extern void ErrorMessage  ARGS((int ErrorCode, int ErrorClass, tPosition Position));
			/* Report a message represented by an integer	*/
			/* 'ErrorCode' and classified by 'ErrorClass'.	*/

extern void ErrorMessageI ARGS((int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info));
			/* Like the previous routine with additional	*/
			/* information of type 'InfoClass' at the	*/
			/* address 'Info'.				*/

extern void Message	  ARGS((char * ErrorText, int ErrorClass, tPosition Position));
			/* Report a message represented by a string	*/
			/* 'ErrorText' and classified by 'ErrorClass'.	*/

extern void MessageI	  ARGS((char * ErrorText, int ErrorClass, tPosition Position, int InfoClass, char * Info));
			/* Like the previous routine with additional	*/
			/* information of type 'InfoClass' at the	*/
			/* address 'Info'.				*/

extern void WriteMessages ARGS((FILE * File));
			/* The stored messages are sorted by their	*/
			/* source position and printed on 'File'.	*/

# endif
