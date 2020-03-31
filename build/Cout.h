# ifndef yyCout
# define yyCout

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"

/* line 41 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */

  typedef enum { xxTerminateOnError, xxContinueOnError, xxStoppOnError } eErrorModes;
  extern eErrorModes ErrorMode;
  extern void OutputC(char *,tTree); 
  extern void MarkCCodeUsage(tStringRef, tPosition, int);


extern void (* Cout_Exit) ();

extern void CodeSymRef ARGS((tTree yyP329, tSymbol yyP328, bool yyP327));
extern void CShowSym ARGS((tTree yyP383, tSymbol yyP382));
extern int CountSpec ARGS((tTree yyP41));
extern int CountRepDecls ARGS((tTree yyP36));
extern int CountAltRepDecls ARGS((tTree yyP37));
extern void CodeMainEntryParameters ARGS((tTree yyP358));
extern bool IsASEntry ARGS((tTree yyP357));
extern void GenStatics ARGS((tTree yyP361));
extern void GenShowValue ARGS((tTree yyP371, tTree yyP370, tSymbol yyP369));
extern void OccCodeType ARGS((tTree yyP339));
extern tTree GenTypeDef ARGS((tTree T));
extern tTree GenDeclStruct ARGS((tTree T));
extern tTree GenCode ARGS((tTree T));
extern tTree GenOccPrototypes ARGS((tTree yyP353));
extern tTree GenCPrototype ARGS((tTree yyP355));
extern int MapKinds ARGS((tTree yyP386, tTree yyP385, tSymbol yyP384));
extern tSymbol CalcParent ARGS((tTree T));
extern tIdent CalcModule ARGS((tTree yyP387));
extern tStringRef CalcHeader ARGS((tTree yyP388));
extern tUsage CalcUsage ARGS((tTree yyP389));
extern bool IsAtomicProc ARGS((tTree yyP347));
extern bool IsParProc ARGS((tTree yyP348));
extern bool IsSTVFunction ARGS((tTree yyP398));
extern bool IsAtomic ARGS((tTree yyP396));
extern bool IsLocal ARGS((tSymbol yyP394));
extern bool IsNonLocal ARGS((tSymbol yyP395));
extern bool IsPrototype ARGS((tTree yyP349));
extern void MarkRefParent ARGS((tSymbol yyP408, tTree yyP407, tVUsage yyP406));
extern tTree GenCGDebug ARGS((tTree yyP356));

extern void BeginCout ();
extern void CloseCout ();

# endif
