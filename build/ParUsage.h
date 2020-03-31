# ifndef yyParUsage
# define yyParUsage

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"

/* line 36 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

  #include "Sets.h"

/* line 204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

  struct tParUsageStruct 
  {
    tPosition Pos;
    tSet W;
    tSet R;
    tTree WAL;  /* written symbol access list */
    tTree RAL;  /* read symbol access list */
  } ;
  #define NoParUsage ((tParUsage) NULL)
  extern void AllowNoAbbrevInProc(tSymbol proc, tSymbol scope, tParUsage U, tPosition P);


extern void (* ParUsage_Exit) ();

extern tParUsage CreateUsage ARGS((tPosition yyP1));
extern bool AddWriteUsage ARGS((tParUsage yyP3, tTree yyP2));
extern bool AddReadUsage ARGS((tParUsage yyP8, tTree yyP7));
extern void AddInstanceLValues ARGS((tTree yyP13, tTree yyP12));
extern void AddLValues ARGS((tTree yyP16, bool yyP15, bool yyP14));
extern tParUsage DeleteUsage ARGS((tParUsage yyP21));
extern tParUsage CombineUsage ARGS((tParUsage yyP24, tParUsage yyP23, int yyP22));
extern tParUsage CombineFuncUsage ARGS((tParUsage yyP26, tTree yyP25));
extern tParUsage CheckUsage ARGS((tParUsage yyP30, tParUsage yyP29, tPosition yyP28, int yyP27));
extern tParUsage CheckRepUsage ARGS((tParUsage yyP35, tParUsage yyP34, tTree yyP33, tPosition yyP32, int yyP31));
extern bool CheckSimpleExpr ARGS((tTree yyP36));
extern void SimplifyLinExpr ARGS((pLinExpr yyP39, tTree yyP38, int yyP37));
extern void RangeCheckLinExpr ARGS((tTree yyP43, pLinExpr yyP42, pLinExpr yyP41, tTree yyP40));
extern void SubscriptCheckLinExpr ARGS((tTree yyP46, pLinExpr yyP45, tTree yyP44));
extern void TransferLinExpr ARGS((tTree yyP49, pLinExpr yyP48, bool yyP47));
extern bool CheckForAbbrev ARGS((tSymbol yyP109, tTree yyP108, tTree yyP107));
extern bool CheckForValAbbrev ARGS((tSymbol yyP106, tTree yyP105, tTree yyP104));
extern bool CheckForAbbrevInProc ARGS((tTree yyP113, tParUsage yyP112));
extern bool CheckForValAbbrevInProc ARGS((tTree yyP111, tParUsage yyP110));
extern tTree CreateElementList ARGS((tTree yyP56));

extern void BeginParUsage ();
extern void CloseParUsage ();

# endif
