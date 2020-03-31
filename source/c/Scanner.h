/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Header file for lexical analyser
||@(#)   System : occam 2 c
||@(#) Filename : Scanner.h
||@(#)  Version : 1.8
||@(#)     Date : 12/10/93
\*@(#)====================================================*/
/*}}}*/

# ifndef yyScanner
# define yyScanner

/* $Id: Scanner.h,v 1.2 1995/03/21 10:20:44 mbh Exp $ */

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# include "StringMem.h"
# include "Positions.h"
# include "Idents.h"

/*# include "MySet.h"*/
typedef int tSymbol;

#define NoString 0

extern int ldebug;
extern bool colon_ended;
extern int tab_size;
extern bool no_tabs;
extern char* fgets_padding_tabs(char *s, int n, FILE *stream,bool *full);

extern char * OSEARCH;
extern char * ODEFAULT_LIB;
extern char * OSPOC;

#ifdef SUPPORT_INT64
  typedef long long tInt;
  typedef unsigned long long tUInt;
#else
  typedef long tInt;
  typedef unsigned long tUInt;
#endif

typedef long int  INT32;
typedef short int INT16;
typedef unsigned char BYTE;

typedef union
{
  tSymbol    Symbol;
  tIdent     Ident;
  tInt       Number;
  struct { double n; int p;} RNumber;
  char       Char;
  tStringRef String;
} tValue;

typedef struct { tPosition Position; tValue Value; } tScanAttribute;

extern	tScanAttribute	Attribute;

extern	int	GetToken	();
extern  int     LUIncludeLevel  ARGS((tStringRef f));
extern	void	ErrorAttribute	ARGS((short yyToken, tScanAttribute * yyRepairAttribute));
extern  void    InitScanner ARGS((char *filename, FILE* fileptr));
extern  FILE   *OpenInclude ARGS((char *inc));
extern  void    CloseInclude ARGS((FILE *fptr));
extern  bool    EnterInclude ARGS((tStringRef f, int NewMode));
extern  int yydebug;
extern  void PushTokenColumn ARGS((void));
extern void PushColumn ARGS((int l));
extern void PopColumn ARGS((void));

# endif
