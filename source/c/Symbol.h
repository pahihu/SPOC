/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Symbol Table Header
||@(#)   System : occam 2 c (based on 1.4)
||@(#) Filename : Symbol.h
||@(#)  Version : 1.5
||@(#)     Date : 11/9/93
\*@(#)====================================================*/
/*}}}*/
# ifndef yySymbolModule
# define yySymbolModule

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)       parameters
# else
# define ARGS(parameters)       ()
# endif

#define NULLSYMBOL { 0, 0, 0, NoTree , NoTree,  0 }

#define NoSymbol ((tSymbol) 0)

#define Symbol(x) SymTable[x]
#define SymbolKind(x) SymTable[x].Kind
#define SymbolType(x) SymTable[x].Type
#define SymbolDepth(x) SymTable[x].Depth
#define SymbolScope(x) SymTable[x].Scope
#define SymbolUsage(x) SymTable[x].Usage
#define ReplaceSymId(s,i) SymTable[s].CId = i;

typedef unsigned int tVUsage;

typedef struct tSymEntry tSymEntry;
struct tSymEntry
{
  tIdent Id;
  tIdent CId;
  tSymbol Scope;
  tTree  Type;
  tTree  Kind; 
  int    Depth;
  tSymbol  Out;
  tVUsage Usage;
} ;

extern int Depth;
extern int SymSize;
extern int OccSymSize;
extern tSymEntry* SymTable;
extern void  InitSymbols  ARGS((void));
extern void  EndSymbols  ARGS((void));
extern tSymbol  AddSymbol   ARGS((tIdent id));
extern tSymbol  AddChanSymbol   ARGS((tIdent id));
extern tSymbol  TempSymbol   ARGS((int Depth));
extern tIdent  UniqueId   ARGS((char type));
extern tIdent PrefixId  ARGS((tIdent pfid,tIdent id,char sep));
extern void ResetUniqueIds ARGS((void));
extern tSymbol  LookupSymbolWithNoFail   ARGS((tIdent Id));
extern tSymbol  LookupSymbol   ARGS((tScanAttribute scan));
extern tSymbol  LookupSymbolInScope   ARGS((tIdent Id, tPosition Pos, tSymbol scope));
extern tSymbol  CurrentScope  ARGS((void));
extern int CountSameNames ARGS((tSymbol s));
extern void  OpenAtScope   ARGS((tSymbol s));
extern void  OpenScope   ARGS((void));
extern void  CloseScope  ARGS((void));
extern void  WriteSymTable ARGS((void));

#endif

