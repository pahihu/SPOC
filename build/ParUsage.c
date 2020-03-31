# include "ParUsage.h"
# include "yyParUsage.w"
# include "System.h"
# include <stdio.h>
# include "Tree.h"

# define yyInline
# ifndef NULL
# define NULL 0L
# endif
# ifndef false
# define false 0
# endif
# ifndef true
# define true 1
# endif

# ifdef yyInline
# define yyALLOC(tree, free, max, alloc, nodesize, make, ptr, kind) \
  if ((ptr = (tree) free) >= (tree) max) ptr = alloc (); \
  free += nodesize [kind]; \
  ptr->yyHead.yyMark = 0; \
  ptr->Kind = kind;
# else
# define yyALLOC(tree, free, max, alloc, nodesize, make, ptr, kind) ptr = make (kind);
# endif

# define yyWrite(s) (void) fputs (s, yyf)
# define yyWriteNl (void) fputc ('\n', yyf)

/* line 43 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

  typedef tSet *pSet;

  static UEMode = xxError; 
  #define IsLVExprList(EL) EL->ExprList.LV
  #define IsSimpEl(S) S->Element.SE
  #define IsParAssigned(E) E->ElementalExpr.PAR

  /*#define EXCLUDE_REPORTED_CONFLICTS*/
  /*{{{  static bool IsDisjoint (tSet *Set1,tSet *Set2)*/
  static bool IsDisjoint (tSet *Set1,tSet *Set2)
  {
     register tSet *   rSet1   = Set1;
     register cardinal i       = rSet1->LastBitset + 1;
     register BITSET * s1      = rSet1->BitsetPtr;
     register BITSET * s2      = Set2->BitsetPtr;
     do {
	if (* s1 ++ & * s2 ++) return false;
     } while (-- i);
     return true;
  }
  /*}}}*/

  /*{{{  static void AllowNoValAbbrev(tSymbol this, tSymbol scope, tPosition P, tTree E)*/
  static void AllowNoValAbbrev(tSymbol this, tSymbol scope, tPosition P, tTree E)
  {
    tTree EL = CreateElementList(E);
   
    while (scope!=this)
      if (CheckForValAbbrev(this,SymbolKind(scope),EL))
      {
	Message("Variable has been VAL abbreviated",UEMode,P);
	return;
      }
      else if (CheckForAbbrev(this,SymbolKind(scope),EL))
      {
	Message("Variable has been abbreviated",UEMode,P);
	return;
      }
      else
	scope = Symbol(scope).Scope;
  }
  /*}}}*/
  /*{{{  static void AllowNoAbbrev(tSymbol this, tSymbol scope, tPosition P, tTree E)*/
  static void AllowNoAbbrev(tSymbol this, tSymbol scope, tPosition P, tTree E)
  {
    tTree EL = CreateElementList(E);
  
    while (scope!=this)
      if (CheckForAbbrev(this,SymbolKind(scope),EL))
      {
	Message("Variable has been abbreviated",UEMode,P);
	return;
      }
      else
	scope = Symbol(scope).Scope;
  }
  /*}}}*/
  /*{{{  void AllowNoAbbrevInProc(tSymbol proc,tSymbol scope, tParUsage U, tPosition P)*/
  void AllowNoAbbrevInProc(tSymbol proc,tSymbol scope, tParUsage U, tPosition P)
  {
    while (scope!=proc)
    {
      if (CheckForValAbbrevInProc(SymbolKind(scope),U))
      {
	 Message("Procedure assigns to variables which have since been VAL abbreviated",UEMode,P);
	 return;
      } else if (CheckForAbbrevInProc(SymbolKind(scope),U))
      {
	 Message("Procedure uses variables which have since been abbreviated",UEMode,P);
	 return;
      }
      else
	scope = Symbol(scope).Scope;
    }
  }
  /*}}}*/

  /*{{{  static void RemoveLocalSymbols(pSet S, int Depth)*/
  static void RemoveLocalSymbols(pSet S, int Depth)
  {
    int i;
    for (i=Minimum(S);i<=Maximum(S);i++)
      if (IsElement(i,S))
	if (Symbol(i).Depth>Depth)
	   Exclude(S,i); 
  }    
  /*}}}*/

  /*{{{  static void ReportUsages(tParUsage ALL, tParUsage U, tPosition P)*/
  static void ReportUsages(tParUsage ALL, tParUsage U, tPosition P)
  {
    int i;
    tSet D;
    MakeSet(&D,SymSize+1);
    Assign(&D,&ALL->W);
    Intersection(&D,&U->W);
    for (i=Minimum(&D);i<=Maximum(&D);i++)
      if (IsElement(i,&D)) 
      {
	MessageI("\tWrite/Write Conflict on ",xxInformation,P,xxIdent,(char*)&Symbol(i).Id);
	#ifdef EXCLUDE_REPORTED_CONFLICTS 
	Exclude(&ALL->W,i);  Exclude(&U->W,i);
	#else
	Exclude(&ALL->R,i);  Exclude(&U->R,i);
	#endif
      }
    Assign(&D,&ALL->R);
    Intersection(&D,&U->W);
    for (i=Minimum(&D);i<=Maximum(&D);i++)
      if (IsElement(i,&D))
      {
	 MessageI("\tWrite/Read Conflict on ",xxInformation,P,xxIdent,(char*)&Symbol(i).Id);
	 #ifdef EXCLUDE_REPORTED_CONFLICTS 
	 Exclude(&ALL->R,i); Exclude(&U->W,i); 
	 #endif
      }
    Assign(&D,&ALL->W);
    Intersection(&D,&U->R);
    for (i=Minimum(&D);i<=Maximum(&D);i++)
      if (IsElement(i,&D)) 
      {
	MessageI("\tRead/Write Conflict on ",xxInformation,P,xxIdent,(char*)&Symbol(i).Id);
	#ifdef EXCLUDE_REPORTED_CONFLICTS 
	Exclude(&ALL->W,i);  Exclude(&U->R,i);
	#endif
      }
    ReleaseSet(&D);
  }
  /*}}}*/
  /*{{{  static void ReportRepUsages(tParUsage U, tPosition P)*/
  static void ReportRepUsages(tParUsage U, tPosition P)
  {
    int i;
    tSet D;
    MakeSet(&D,SymSize+1);
    Assign(&D,&U->R);
    Intersection(&D,&U->W);
    for (i=Minimum(&D);i<=Maximum(&D);i++)
      if (IsElement(i,&D))
      {
	MessageI("\tWrite/Read Conflict on ",xxInformation,P,xxIdent,(char*)&Symbol(i).Id);
	#ifdef EXCLUDE_REPORTED_CONFLICTS 
	Exclude(&U->R,i); Exclude(&U->W,i); 
	#endif
      }
    ReleaseSet(&D);
    for (i=Minimum(&U->W);i<=Maximum(&U->W);i++)
      if (IsElement(i,&U->W)) 
      {
	MessageI("\tWrite/Write Conflict on ",xxInformation,P,xxIdent,(char*)&Symbol(i).Id);
	#ifdef EXCLUDE_REPORTED_CONFLICTS 
	Exclude(&U->W,i); 
	#endif
      }
  }
  /*}}}*/


static void yyExit () { Exit (1); }

void (* ParUsage_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module ParUsage, routine %s failed\n", yyFunction);
 ParUsage_Exit ();
}

tParUsage CreateUsage ARGS((tPosition yyP1));
bool AddWriteUsage ARGS((tParUsage yyP3, tTree yyP2));
static bool SetWriteUsage ARGS((tParUsage yyP6, tSymbol yyP5, tTree yyP4));
bool AddReadUsage ARGS((tParUsage yyP8, tTree yyP7));
static bool SetReadUsage ARGS((tParUsage yyP11, tSymbol yyP10, tTree yyP9));
void AddInstanceLValues ARGS((tTree yyP13, tTree yyP12));
void AddLValues ARGS((tTree yyP16, bool yyP15, bool yyP14));
static tTree GetElement ARGS((tTree yyP17));
static tSymbol GetElementSymbol ARGS((tTree yyP18));
static tPosition GetElementPosition ARGS((tTree yyP19));
static int GetElementScope ARGS((tTree yyP20));
tParUsage DeleteUsage ARGS((tParUsage yyP21));
tParUsage CombineUsage ARGS((tParUsage yyP24, tParUsage yyP23, int yyP22));
tParUsage CombineFuncUsage ARGS((tParUsage yyP26, tTree yyP25));
tParUsage CheckUsage ARGS((tParUsage yyP30, tParUsage yyP29, tPosition yyP28, int yyP27));
tParUsage CheckRepUsage ARGS((tParUsage yyP35, tParUsage yyP34, tTree yyP33, tPosition yyP32, int yyP31));
bool CheckSimpleExpr ARGS((tTree yyP36));
void SimplifyLinExpr ARGS((pLinExpr yyP39, tTree yyP38, int yyP37));
void RangeCheckLinExpr ARGS((tTree yyP43, pLinExpr yyP42, pLinExpr yyP41, tTree yyP40));
void SubscriptCheckLinExpr ARGS((tTree yyP46, pLinExpr yyP45, tTree yyP44));
void TransferLinExpr ARGS((tTree yyP49, pLinExpr yyP48, bool yyP47));
static bool CheckUnconstrained ARGS((tTree yyP50));
static tTree InsertAccess ARGS((tTree yyP54, tSymbol yyP53, tTree yyP52, pSet yyP51, bool * yyP55));
tTree CreateElementList ARGS((tTree yyP56));
static bool IsElementSubset ARGS((tTree yyP58, tTree yyP57));
static tTree InsertElement ARGS((tTree yyP60, tTree yyP59, bool * yyP61));
static tTree MergeAccess ARGS((tTree yyP65, tTree yyP64, pSet yyP63, int yyP62));
static tTree MergeAccess2 ARGS((tTree yyP69, tTree yyP68, pSet yyP67, int yyP66));
static tTree CopyElementList ARGS((tTree yyP70));
static tTree MergeElementList ARGS((tTree yyP72, tTree yyP71));
static void UsageCheckAccess ARGS((tTree yyP76, pSet yyP75, tTree yyP74, pSet yyP73));
static bool CheckElement ARGS((tTree yyP78, tTree yyP77));
static bool CheckEachElement ARGS((tTree yyP80, tTree yyP79));
static bool CheckElementVsElement ARGS((tTree yyP82, tTree yyP81));
static void UsageCheckRepWRAccess ARGS((tTree yyP88, pSet yyP87, tTree yyP86, pSet yyP85, int yyP84, tSymbol yyP83));
static bool CheckRepWRElementList ARGS((tTree yyP91, tTree yyP90, tSymbol yyP89));
static void UsageCheckRepAccess ARGS((tTree yyP95, pSet yyP94, int yyP93, tSymbol yyP92));
static bool CheckRepElementList ARGS((tTree yyP97, tSymbol yyP96));
static bool CheckRepEachElement ARGS((tTree yyP100, tTree yyP99, tSymbol yyP98));
static bool CheckRepElementVsElement ARGS((tTree yyP103, tTree yyP102, tSymbol yyP101));
bool CheckForValAbbrev ARGS((tSymbol yyP106, tTree yyP105, tTree yyP104));
bool CheckForAbbrev ARGS((tSymbol yyP109, tTree yyP108, tTree yyP107));
bool CheckForValAbbrevInProc ARGS((tTree yyP111, tParUsage yyP110));
bool CheckForAbbrevInProc ARGS((tTree yyP113, tParUsage yyP112));
static bool CheckAccessForSymbol ARGS((tTree yyP115, tSymbol yyP114));
static bool CheckAccessForAlias ARGS((tTree yyP119, pSet yyP118, tTree yyP117, pSet yyP116));
static bool CheckAccessVsElementList ARGS((tTree yyP122, tSymbol yyP121, tTree yyP120));
static bool IsChanType ARGS((tTree yyP123));
static tTree ElementRevLink ARGS((tTree yyP124));

tParUsage CreateUsage
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP1)
# else
(yyP1)
 register tPosition yyP1;
# endif
{
/* line 226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 tParUsage U=(tParUsage)malloc(sizeof(tParUsageStruct)); 
/* line 227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
if (U==NULL) Message("Malloc failure during usage checking",xxFatal,yyP1); 
/* line 229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   MakeSet (& U -> W, SymSize + 1);
/* line 230 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   MakeSet (& U -> R, SymSize + 1);
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   U -> WAL = mNoAccess ();
/* line 232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   U -> RAL = mNoAccess ();
/* line 233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   U -> Pos = yyP1;
  }
   return U;

}

bool AddWriteUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP3, register tTree yyP2)
# else
(yyP3, yyP2)
 tParUsage yyP3;
 register tTree yyP2;
# endif
{
  if (yyP3 == NULL) {
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (Tree_IsType (yyP2, kElement)) {
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return SetWriteUsage (yyP3, GetElementSymbol (yyP2), yyP2);

  }
/* line 239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

}

static bool SetWriteUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP6, tSymbol yyP5, register tTree yyP4)
# else
(yyP6, yyP5, yyP4)
 tParUsage yyP6;
 tSymbol yyP5;
 register tTree yyP4;
# endif
{
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP4 -> Element . U == xxInput))) goto yyL1;
  {
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP5, & yyP6 -> W)))) goto yyL1;
  }
  }
   return false;
yyL1:;

/* line 243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP4 -> Element . U == xxOutput))) goto yyL2;
  {
/* line 243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (Symbol (yyP5) . Out, & yyP6 -> W)))) goto yyL2;
  }
  }
   return false;
yyL2:;

/* line 244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP4 -> Element . U == xxInput))) goto yyL3;
  {
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP6 -> WAL = InsertAccess (yyP6 -> WAL, yyP5, yyP4, & yyP6 -> W, & yyV1);
  }
  }
  {
   return yyV1;
  }
 }
yyL3:;

/* line 247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP4 -> Element . U == xxOutput))) goto yyL4;
  {
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP6 -> WAL = InsertAccess (yyP6 -> WAL, Symbol (yyP5) . Out, yyP4, & yyP6 -> W, & yyV1);
  }
  }
  {
   return yyV1;
  }
 }
yyL4:;

/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsChanType (SymbolType (yyP5))))) goto yyL5;
  {
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP5, & yyP6 -> W)))) goto yyL5;
  {
/* line 251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AllowNoValAbbrev (yyP5, GetElementScope (yyP4), GetElementPosition (yyP4), yyP4);
  }
  }
  }
   return false;
yyL5:;

/* line 253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsChanType (SymbolType (yyP5))))) goto yyL6;
  {
/* line 254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP6 -> WAL = InsertAccess (yyP6 -> WAL, yyP5, yyP4, & yyP6 -> W, & yyV1);
/* line 255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AllowNoValAbbrev (yyP5, GetElementScope (yyP4), GetElementPosition (yyP4), yyP4);
  }
  }
  {
   return yyV1;
  }
 }
yyL6:;

/* line 257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

}

bool AddReadUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP8, register tTree yyP7)
# else
(yyP8, yyP7)
 tParUsage yyP8;
 register tTree yyP7;
# endif
{
  if (yyP8 == NULL) {
/* line 261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
/* line 262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP7 -> Elemental . FLOut != NoTree))) goto yyL2;
  }
   return true;
yyL2:;

  if (Tree_IsType (yyP7, kElement)) {
/* line 263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return SetReadUsage (yyP8, GetElementSymbol (yyP7), yyP7);

  }
/* line 264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

}

static bool SetReadUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP11, tSymbol yyP10, register tTree yyP9)
# else
(yyP11, yyP10, yyP9)
 tParUsage yyP11;
 tSymbol yyP10;
 register tTree yyP9;
# endif
{
/* line 267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP9 -> Element . U == xxInput))) goto yyL1;
  {
/* line 267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP10, & yyP11 -> R)))) goto yyL1;
  }
  }
   return false;
yyL1:;

/* line 268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP9 -> Element . U == xxOutput))) goto yyL2;
  {
/* line 268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (Symbol (yyP10) . Out, & yyP11 -> R)))) goto yyL2;
  }
  }
   return false;
yyL2:;

/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP9 -> Element . U == xxInput))) goto yyL3;
  {
/* line 270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP11 -> RAL = InsertAccess (yyP11 -> RAL, yyP10, yyP9, & yyP11 -> R, & yyV1);
  }
  }
  {
   return yyV1;
  }
 }
yyL3:;

/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP9 -> Element . U == xxOutput))) goto yyL4;
  {
/* line 273 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP11 -> RAL = InsertAccess (yyP11 -> RAL, Symbol (yyP10) . Out, yyP9, & yyP11 -> R, & yyV1);
  }
  }
  {
   return yyV1;
  }
 }
yyL4:;

/* line 275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsChanType (SymbolType (yyP10))))) goto yyL5;
  {
/* line 275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP10, & yyP11 -> R)))) goto yyL5;
  {
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AllowNoAbbrev (yyP10, GetElementScope (yyP9), GetElementPosition (yyP9), yyP9);
  }
  }
  }
   return false;
yyL5:;

/* line 278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsChanType (SymbolType (yyP10))))) goto yyL6;
  {
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP11 -> RAL = InsertAccess (yyP11 -> RAL, yyP10, yyP9, & yyP11 -> R, & yyV1);
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AllowNoAbbrev (yyP10, GetElementScope (yyP9), GetElementPosition (yyP9), yyP9);
  }
  }
  {
   return yyV1;
  }
 }
yyL6:;

/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

}

void AddInstanceLValues
# if defined __STDC__ | defined __cplusplus
(register tTree yyP13, register tTree yyP12)
# else
(yyP13, yyP12)
 register tTree yyP13;
 register tTree yyP12;
# endif
{
  if (Tree_IsType (yyP13, kNoFormal)) {
  if (yyP12->Kind == kNoExpr) {
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP12) = false;
  }
   return;

  }
  }
  if (yyP13->Kind == kAFormal) {
  if (yyP13->AFormal.By->Kind == kByRef) {
  if (yyP12->Kind == kAnExpr) {
  if (yyP12->AnExpr.Expr->Kind == kElementalExpr) {
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP12) = true;
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsParAssigned (yyP12->AnExpr.Expr) = true;
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddInstanceLValues (yyP13->AFormal.Next, yyP12->AnExpr.Next);
  }
   return;

  }
  }
  }
  if (yyP13->AFormal.By->Kind == kByVal) {
  if (yyP12->Kind == kAnExpr) {
/* line 288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP12) = false;
/* line 288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddInstanceLValues (yyP13->AFormal.Next, yyP12->AnExpr.Next);
  }
   return;

  }
  }
  }
;
}

void AddLValues
# if defined __STDC__ | defined __cplusplus
(register tTree yyP16, register bool yyP15, register bool yyP14)
# else
(yyP16, yyP15, yyP14)
 register tTree yyP16;
 register bool yyP15;
 register bool yyP14;
# endif
{
  if (yyP16->Kind == kNoExpr) {
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP16) = yyP15;
  }
   return;

  }
  if (yyP16->Kind == kAnExpr) {
  if (yyP16->AnExpr.Expr->Kind == kElementalExpr) {
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP16) = yyP15;
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsParAssigned (yyP16->AnExpr.Expr) = yyP14;
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddLValues (yyP16->AnExpr.Next, yyP15, yyP14);
  }
   return;

  }
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   IsLVExprList (yyP16) = yyP15;
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddLValues (yyP16->AnExpr.Next, yyP15, yyP14);
  }
   return;

  }
;
}

static tTree GetElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP17)
# else
(yyP17)
 register tTree yyP17;
# endif
{
  if (Tree_IsType (yyP17, kIdentifier)) {
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP17;

  }
  if (yyP17->Kind == kSubscript) {
/* line 299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElement (yyP17->Subscript.Next);

  }
  if (yyP17->Kind == kRange) {
/* line 300 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElement (yyP17->Range.Next);

  }
 yyAbort ("GetElement");
}

static tSymbol GetElementSymbol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP18)
# else
(yyP18)
 register tTree yyP18;
# endif
{
  if (Tree_IsType (yyP18, kIdentifier)) {
/* line 304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP18->Identifier.Sym;

  }
  if (yyP18->Kind == kSubscript) {
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementSymbol (yyP18->Subscript.Next);

  }
  if (yyP18->Kind == kRange) {
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementSymbol (yyP18->Range.Next);

  }
 yyAbort ("GetElementSymbol");
}

static tPosition GetElementPosition
# if defined __STDC__ | defined __cplusplus
(register tTree yyP19)
# else
(yyP19)
 register tTree yyP19;
# endif
{
  if (Tree_IsType (yyP19, kIdentifier)) {
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP19->Identifier.Pos;

  }
  if (yyP19->Kind == kSubscript) {
/* line 312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementPosition (yyP19->Subscript.Next);

  }
  if (yyP19->Kind == kRange) {
/* line 313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementPosition (yyP19->Range.Next);

  }
 yyAbort ("GetElementPosition");
}

static int GetElementScope
# if defined __STDC__ | defined __cplusplus
(register tTree yyP20)
# else
(yyP20)
 register tTree yyP20;
# endif
{
  if (Tree_IsType (yyP20, kIdentifier)) {
/* line 318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP20 -> Identifier . Scope;

  }
  if (yyP20->Kind == kSubscript) {
/* line 319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementScope (yyP20->Subscript.Next);

  }
  if (yyP20->Kind == kRange) {
/* line 320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return GetElementScope (yyP20->Range.Next);

  }
 yyAbort ("GetElementScope");
}

tParUsage DeleteUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP21)
# else
(yyP21)
 tParUsage yyP21;
# endif
{
/* line 325 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return NoParUsage;

}

tParUsage CombineUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP24, tParUsage yyP23, register int yyP22)
# else
(yyP24, yyP23, yyP22)
 tParUsage yyP24;
 tParUsage yyP23;
 register int yyP22;
# endif
{
/* line 329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   RemoveLocalSymbols (& yyP23 -> W, yyP22);
/* line 329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   RemoveLocalSymbols (& yyP23 -> R, yyP22);
/* line 330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Union (& yyP24 -> W, & yyP23 -> W);
/* line 330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Union (& yyP24 -> R, & yyP23 -> R);
/* line 331 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP24 -> WAL = MergeAccess (yyP24 -> WAL, yyP23 -> WAL, & yyP24 -> W, yyP22);
/* line 332 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP24 -> RAL = MergeAccess (yyP24 -> RAL, yyP23 -> RAL, & yyP24 -> R, yyP22);
  }
   return yyP24;

}

tParUsage CombineFuncUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP26, register tTree yyP25)
# else
(yyP26, yyP25)
 tParUsage yyP26;
 register tTree yyP25;
# endif
{
  if (Tree_IsType (yyP25, kDefFunc)) {
/* line 336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CombineUsage (yyP26, yyP25->DefFunc.Valof->Valof.ExprList -> ExprList . PUOut, yyP25->DefFunc.Valof -> Valof . VS);

  }
  if (Tree_IsType (yyP25, kDefIsFunc)) {
/* line 337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CombineUsage (yyP26, yyP25->DefIsFunc.ExprList -> ExprList . PUOut, Symbol (yyP25->DefIsFunc.Sym) . Depth);

  }
/* line 338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP26;

}

tParUsage CheckUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP30, tParUsage yyP29, register tPosition yyP28, register int yyP27)
# else
(yyP30, yyP29, yyP28, yyP27)
 tParUsage yyP30;
 tParUsage yyP29;
 register tPosition yyP28;
 register int yyP27;
# endif
{
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP30 -> RAL, & yyP30 -> R, yyP29 -> WAL, & yyP29 -> W);
/* line 346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP30 -> WAL, & yyP30 -> W, yyP29 -> WAL, & yyP29 -> W);
/* line 347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP30 -> WAL, & yyP30 -> W, yyP29 -> RAL, & yyP29 -> R);
/* line 348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsDisjoint (& yyP30 -> R, & yyP29 -> W)))) goto yyL1;
  {
/* line 349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsDisjoint (& yyP30 -> W, & yyP29 -> W)))) goto yyL1;
  {
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsDisjoint (& yyP30 -> W, & yyP29 -> R)))) goto yyL1;
  }
  }
  }
   return CombineUsage (yyP30, yyP29, yyP27);
yyL1:;

/* line 366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Usage violation within this construct", UEMode, yyP28);
/* line 368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   ReportUsages (yyP30, yyP29, yyP28);
  }
   return CombineUsage (yyP30, yyP29, yyP27);

}

tParUsage CheckRepUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP35, tParUsage yyP34, register tTree yyP33, register tPosition yyP32, register int yyP31)
# else
(yyP35, yyP34, yyP33, yyP32, yyP31)
 tParUsage yyP35;
 tParUsage yyP34;
 register tTree yyP33;
 register tPosition yyP32;
 register int yyP31;
# endif
{
/* line 372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP33->Replicator.For) && ValConstInt (yyP33->Replicator.For) == 1))) goto yyL1;
  }
   return CombineUsage (yyP35, yyP34, yyP31);
yyL1:;

/* line 373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepAccess (yyP34 -> WAL, & yyP34 -> W, yyP31, yyP33->Replicator.Sym);
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP34 -> WAL, & yyP34 -> W, yyP34 -> RAL, & yyP34 -> R, yyP31, yyP33->Replicator.Sym);
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   RemoveLocalSymbols (& yyP34 -> W, yyP31);
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsEmpty (& yyP34 -> W)))) goto yyL2;
  }
   return CombineUsage (yyP35, yyP34, yyP31);
yyL2:;

/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Parallel use violation within replicated PAR", UEMode, yyP32);
/* line 390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   ReportRepUsages (yyP34, yyP32);
  }
   return CombineUsage (yyP35, yyP34, yyP31);

}

bool CheckSimpleExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP36)
# else
(yyP36)
 register tTree yyP36;
# endif
{
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP36)))) goto yyL1;
  }
   return true;
yyL1:;

  if (yyP36->Kind == kElementalExpr) {
  if (Tree_IsType (yyP36->ElementalExpr.Elemental, kIdentifier)) {
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP36->ElementalExpr.Elemental->Identifier.Sym), kReplicator)))) goto yyL2;
  {
/* line 398 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (SymbolKind (yyP36->ElementalExpr.Elemental->Identifier.Sym) -> Replicator . From))) goto yyL2;
  {
/* line 399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (SymbolKind (yyP36->ElementalExpr.Elemental->Identifier.Sym) -> Replicator . For))) goto yyL2;
  }
  }
  }
   return true;
yyL2:;

/* line 400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP36->ElementalExpr.Elemental->Identifier.Sym), kVal)))) goto yyL3;
  {
/* line 402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (SymbolKind (yyP36->ElementalExpr.Elemental->Identifier.Sym) -> Val . Expr))) goto yyL3;
  }
  }
   return true;
yyL3:;

  }
  }
  if (yyP36->Kind == kBinary) {
  if (yyP36->Binary.Op->Kind == kMultiply) {
/* line 403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP36->Binary.Lop)))) goto yyL4;
  {
/* line 403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Rop))) goto yyL4;
  }
  }
   return true;
yyL4:;

/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP36->Binary.Rop)))) goto yyL5;
  {
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Lop))) goto yyL5;
  }
  }
   return true;
yyL5:;

  }
  if (yyP36->Binary.Op->Kind == kAdd) {
/* line 405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Lop))) goto yyL6;
  {
/* line 405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Rop))) goto yyL6;
  }
  }
   return true;
yyL6:;

  }
  if (yyP36->Binary.Op->Kind == kSubtract) {
/* line 406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Lop))) goto yyL7;
  {
/* line 406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Binary.Rop))) goto yyL7;
  }
  }
   return true;
yyL7:;

  }
  }
  if (yyP36->Kind == kUnary) {
  if (yyP36->Unary.Op->Kind == kAdd) {
/* line 407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Unary.Expr))) goto yyL8;
  }
   return true;
yyL8:;

  }
  if (yyP36->Unary.Op->Kind == kSubtract) {
/* line 408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckSimpleExpr (yyP36->Unary.Expr))) goto yyL9;
  }
   return true;
yyL9:;

  }
  }
  return false;
}

void SimplifyLinExpr
# if defined __STDC__ | defined __cplusplus
(pLinExpr yyP39, register tTree yyP38, register int yyP37)
# else
(yyP39, yyP38, yyP37)
 pLinExpr yyP39;
 register tTree yyP38;
 register int yyP37;
# endif
{
/* line 413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP38)))) goto yyL1;
  {
/* line 414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddLinExprConst (yyP39, yyP37 * ValConstInt (yyP38));
  }
  }
   return;
yyL1:;

  if (yyP38->Kind == kElementalExpr) {
  if (Tree_IsType (yyP38->ElementalExpr.Elemental, kIdentifier)) {
/* line 416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP38->ElementalExpr.Elemental->Identifier.Sym), kReplicator)))) goto yyL2;
  {
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   AddLinExprCoef (yyP39, yyP38->ElementalExpr.Elemental->Identifier.Sym, yyP37);
  }
  }
   return;
yyL2:;

/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP38->ElementalExpr.Elemental->Identifier.Sym), kVal)))) goto yyL3;
  {
/* line 420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, SymbolKind (yyP38->ElementalExpr.Elemental->Identifier.Sym) -> Val . Expr, yyP37);
  }
  }
   return;
yyL3:;

  }
  }
  if (yyP38->Kind == kBinary) {
  if (yyP38->Binary.Op->Kind == kMultiply) {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP38->Binary.Lop)))) goto yyL4;
  {
/* line 423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Rop, yyP37 * ValConstInt (yyP38->Binary.Lop));
  }
  }
   return;
yyL4:;

/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsConstInt (yyP38->Binary.Rop)))) goto yyL5;
  {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Lop, yyP37 * ValConstInt (yyP38->Binary.Rop));
  }
  }
   return;
yyL5:;

  }
  if (yyP38->Binary.Op->Kind == kAdd) {
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Lop, yyP37);
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Rop, yyP37);
  }
   return;

  }
  if (yyP38->Binary.Op->Kind == kSubtract) {
/* line 432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Lop, yyP37);
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Binary.Rop, - yyP37);
  }
   return;

  }
  }
  if (yyP38->Kind == kUnary) {
  if (yyP38->Unary.Op->Kind == kAdd) {
/* line 436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Unary.Expr, yyP37);
  }
   return;

  }
  if (yyP38->Unary.Op->Kind == kSubtract) {
/* line 439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   SimplifyLinExpr (yyP39, yyP38->Unary.Expr, - yyP37);
  }
   return;

  }
  }
;
}

void RangeCheckLinExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP43, pLinExpr yyP42, pLinExpr yyP41, register tTree yyP40)
# else
(yyP43, yyP42, yyP41, yyP40)
 register tTree yyP43;
 pLinExpr yyP42;
 pLinExpr yyP41;
 register tTree yyP40;
# endif
{
/* line 446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

      if (!yyP42->Const)
	{
	  if (TestLinExpr_Const(yyP42,kLess,0))
	     Message("Array slice start potentially out of range",xxWarning,yyP43->Elemental.Pos);
	   else if (yyP40->Array.SE && TestLinExpr_LinExpr(yyP42,kGreaterEqual,yyP40->Array.LE))
	     Message("Array slice start potentially out of range",xxWarning,yyP43->Elemental.Pos);
	   else
	     DoLwrBndChk(yyP43)=false;
	}
	if (!yyP42->Const || !yyP41->Const)
	{
	  if (yyP40->Array.SE && TestLinExprPlusLinExpr_LinExpr(yyP42,yyP41,kGreater,yyP40->Array.LE))
	    Message("Array slice end potentially out of range",xxWarning,yyP43->Elemental.Pos);
	  else
	    DoUprBndChk(yyP43)=false;
	}
    
  }
   return;

;
}

void SubscriptCheckLinExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP46, pLinExpr yyP45, register tTree yyP44)
# else
(yyP46, yyP45, yyP44)
 register tTree yyP46;
 pLinExpr yyP45;
 register tTree yyP44;
# endif
{
  if (Tree_IsType (yyP44, kArray)) {
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

    if (TestLinExpr_Const(yyP45,kLess,0))
	Message("Array subscript potentially out of range",xxWarning,yyP46->Elemental.Pos);
      else if (yyP44->Array.SE && TestLinExpr_LinExpr(yyP45,kGreaterEqual,yyP44->Array.LE))
	Message("Array subscript potentially out of range",xxWarning,yyP46->Elemental.Pos);
      else
	DoBndChk(yyP46)=false;
  
  }
   return;

  }
/* line 477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 DoBndChk(yyP46)=false; 
  }
   return;

;
}

void TransferLinExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP49, pLinExpr yyP48, register bool yyP47)
# else
(yyP49, yyP48, yyP47)
 register tTree yyP49;
 pLinExpr yyP48;
 register bool yyP47;
# endif
{
  if (Tree_IsType (yyP49, kArray)) {
/* line 483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 yyP49->Array.LE = yyP48; yyP49->Array.SE = yyP47; 
  }
   return;

  }
/* line 484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return;

;
}

static bool CheckUnconstrained
# if defined __STDC__ | defined __cplusplus
(register tTree yyP50)
# else
(yyP50)
 register tTree yyP50;
# endif
{
  if (Tree_IsType (yyP50, kIdentifier)) {
/* line 489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (yyP50->Kind == kRange) {
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP50)))) goto yyL2;
  {
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckUnconstrained (yyP50->Range.Next))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  if (yyP50->Kind == kSubscript) {
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP50)))) goto yyL3;
  {
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckUnconstrained (yyP50->Subscript.Next))) goto yyL3;
  }
  }
   return true;
yyL3:;

  }
  return false;
}

static tTree InsertAccess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP54, tSymbol yyP53, register tTree yyP52, pSet yyP51, register bool * yyP55)
# else
(yyP54, yyP53, yyP52, yyP51, yyP55)
 register tTree yyP54;
 tSymbol yyP53;
 register tTree yyP52;
 pSet yyP51;
 register bool * yyP55;
# endif
{
  if (yyP54->Kind == kNoAccess) {
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckUnconstrained (yyP52)))) goto yyL1;
  {
/* line 496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP51, yyP53);
  }
  }
   * yyP55 = true;
   return yyP54;
yyL1:;

/* line 498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
   * yyP55 = true;
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnAccess)
    yyV1->AnAccess.Next = yyP54;
    yyV1->AnAccess.Sym = yyP53;
    yyV1->AnAccess.ElementList = CreateElementList (yyP52);
   return yyV1;
  }
 }

  }
  if (yyP54->Kind == kAnAccess) {
/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP53 > yyP54->AnAccess.Sym))) goto yyL3;
  {
/* line 501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP54->AnAccess.Next = InsertAccess (yyP54->AnAccess.Next, yyP53, yyP52, yyP51, & yyV1);
  }
  }
   * yyP55 = yyV1;
  {
   return yyP54;
  }
 }
yyL3:;

/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP53 < yyP54->AnAccess.Sym))) goto yyL4;
  {
/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckUnconstrained (yyP52))) goto yyL4;
  {
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP51, yyP53);
  }
  }
  }
   * yyP55 = true;
   return yyP54->AnAccess.Next;
yyL4:;

/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP53 < yyP54->AnAccess.Sym))) goto yyL5;
  }
   * yyP55 = true;
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnAccess)
    yyV1->AnAccess.Next = yyP54;
    yyV1->AnAccess.Sym = yyP53;
    yyV1->AnAccess.ElementList = CreateElementList (yyP52);
   return yyV1;
  }
 }
yyL5:;

/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP53 == yyP54->AnAccess.Sym))) goto yyL6;
  {
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckUnconstrained (yyP52))) goto yyL6;
  {
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP51, yyP53);
  }
  }
  }
   * yyP55 = false;
   return yyP54->AnAccess.Next;
yyL6:;

/* line 511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP53 == yyP54->AnAccess.Sym))) goto yyL7;
  {
/* line 512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP54->AnAccess.ElementList = InsertElement (yyP54->AnAccess.ElementList, GetElement (yyP52), & yyV1);
  }
  }
   * yyP55 = yyV1;
  {
   return yyP54;
  }
 }
yyL7:;

  }
 yyAbort ("InsertAccess");
}

tTree CreateElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP56)
# else
(yyP56)
 register tTree yyP56;
# endif
{
  if (Tree_IsType (yyP56, kIdentifier)) {
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoElement)
   return yyV1;
  }
 }

  }
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnElement)
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kNoElement)
    yyV1->AnElement.Next = yyW1;
   }
    yyV1->AnElement.Element = GetElement (yyP56);
   return yyV1;
  }
 }

}

static bool IsElementSubset
# if defined __STDC__ | defined __cplusplus
(register tTree yyP58, register tTree yyP57)
# else
(yyP58, yyP57)
 register tTree yyP58;
 register tTree yyP57;
# endif
{
  if (Tree_IsType (yyP58, kIdentifier)) {
  if (Tree_IsType (yyP57, kIdentifier)) {
/* line 523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP58->Identifier.Pos);
  }
   return true;

  }
  if (Tree_IsType (yyP57, kIdentifier)) {
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP57->Identifier.Pos);
  }
   return true;

  }
  if (Tree_IsType (yyP58, kElementAccess)) {
  if (Tree_IsType (yyP57, kElementAccess)) {
/* line 529 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 529 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP57)))) goto yyL4;
  }
   return IsElementSubset (yyP58->ElementAccess.Next, yyP57->ElementAccess.Next);
yyL4:;

  }
  }
/* line 531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP58)))) goto yyL5;
  }
   return false;
yyL5:;

  if (yyP58->Kind == kSubscript) {
  if (yyP57->Kind == kSubscript) {
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((SameLinExpr (yyP58 -> Subscript . LE, yyP57 -> Subscript . LE)))) goto yyL6;
  }
   return IsElementSubset (yyP58->Subscript.Next, yyP57->Subscript.Next);
yyL6:;

  }
  }
  if (yyP58->Kind == kRange) {
  if (yyP57->Kind == kRange) {
/* line 536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((SameLinExpr (yyP58 -> Range . FromLE, yyP57 -> Range . FromLE)))) goto yyL7;
  {
/* line 538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((SameLinExpr (yyP58 -> Range . ForLE, yyP57 -> Range . ForLE)))) goto yyL7;
  }
  }
   return IsElementSubset (yyP58->Range.Next, yyP57->Range.Next);
yyL7:;

  }
  }
/* line 540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

}

static tTree InsertElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP60, register tTree yyP59, register bool * yyP61)
# else
(yyP60, yyP59, yyP61)
 register tTree yyP60;
 register tTree yyP59;
 register bool * yyP61;
# endif
{
  if (yyP60->Kind == kNoElement) {
/* line 545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
   * yyP61 = true;
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnElement)
    yyV1->AnElement.Next = yyP60;
    yyV1->AnElement.Element = yyP59;
   return yyV1;
  }
 }

  }
  if (yyP60->Kind == kAnElement) {
/* line 547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElementSubset (yyP59, yyP60->AnElement.Element)))) goto yyL2;
  }
   * yyP61 = false;
   return yyP60;
yyL2:;

/* line 549 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  bool yyV1;
  {
/* line 550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP60->AnElement.Next = InsertElement (yyP60->AnElement.Next, yyP59, & yyV1);
  }
   * yyP61 = yyV1;
  {
   return yyP60;
  }
 }

  }
 yyAbort ("InsertElement");
}

static tTree MergeAccess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP65, register tTree yyP64, pSet yyP63, register int yyP62)
# else
(yyP65, yyP64, yyP63, yyP62)
 register tTree yyP65;
 register tTree yyP64;
 pSet yyP63;
 register int yyP62;
# endif
{
  if (yyP65->Kind == kNoAccess) {
  if (yyP64->Kind == kNoAccess) {
/* line 556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP65;

  }
  if (yyP64->Kind == kAnAccess) {
/* line 563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
/* line 563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Symbol (yyP64->AnAccess.Sym) . Depth <= yyP62))) goto yyL4;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnAccess)
    yyV1->AnAccess.Next = MergeAccess (yyP65, yyP64->AnAccess.Next, yyP63, yyP62);
    yyV1->AnAccess.Sym = yyP64->AnAccess.Sym;
    yyV1->AnAccess.ElementList = CopyElementList (yyP64->AnAccess.ElementList);
   return yyV1;
  }
 }
yyL4:;

  }
  }
  if (yyP65->Kind == kAnAccess) {
  if (yyP64->Kind == kNoAccess) {
/* line 558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP65->AnAccess.Sym, yyP63)))) goto yyL2;
  }
   return MergeAccess (yyP65->AnAccess.Next, yyP64, yyP63, yyP62);
yyL2:;

/* line 560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP65->AnAccess.Next = MergeAccess (yyP65->AnAccess.Next, yyP64, yyP63, yyP62);
  }
   return yyP65;

  }
  }
  if (yyP64->Kind == kAnAccess) {
/* line 565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Symbol (yyP64->AnAccess.Sym) . Depth > yyP62))) goto yyL5;
  }
   return yyP65;
yyL5:;

  }
/* line 567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return MergeAccess2 (yyP65, yyP64, yyP63, yyP62);

}

static tTree MergeAccess2
# if defined __STDC__ | defined __cplusplus
(register tTree yyP69, register tTree yyP68, pSet yyP67, register int yyP66)
# else
(yyP69, yyP68, yyP67, yyP66)
 register tTree yyP69;
 register tTree yyP68;
 pSet yyP67;
 register int yyP66;
# endif
{
  if (yyP69->Kind == kAnAccess) {
/* line 571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP69->AnAccess.Sym, yyP67)))) goto yyL1;
  }
   return MergeAccess (yyP69->AnAccess.Next, yyP68, yyP67, yyP66);
yyL1:;

  if (yyP68->Kind == kAnAccess) {
/* line 575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP69->AnAccess.Sym < yyP68->AnAccess.Sym))) goto yyL3;
  {
/* line 576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP69->AnAccess.Next = MergeAccess (yyP69->AnAccess.Next, yyP68, yyP67, yyP66);
  }
  }
   return yyP69;
yyL3:;

/* line 578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
/* line 578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP68->AnAccess.Sym < yyP69->AnAccess.Sym))) goto yyL4;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnAccess)
    yyV1->AnAccess.Next = MergeAccess (yyP69, yyP68->AnAccess.Next, yyP67, yyP66);
    yyV1->AnAccess.Sym = yyP68->AnAccess.Sym;
    yyV1->AnAccess.ElementList = yyP68->AnAccess.ElementList;
   return yyV1;
  }
 }
yyL4:;

/* line 580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP68->AnAccess.Sym == yyP69->AnAccess.Sym))) goto yyL5;
  {
/* line 580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   yyP69->AnAccess.ElementList = MergeElementList (yyP69->AnAccess.ElementList, yyP68->AnAccess.ElementList);
  }
  }
   return yyP69;
yyL5:;

  }
  }
  if (yyP68->Kind == kAnAccess) {
/* line 573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP68->AnAccess.Sym, yyP67)))) goto yyL2;
  }
   return MergeAccess (yyP69, yyP68->AnAccess.Next, yyP67, yyP66);
yyL2:;

  }
 yyAbort ("MergeAccess2");
}

static tTree CopyElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP70)
# else
(yyP70)
 register tTree yyP70;
# endif
{
  if (yyP70->Kind == kNoElement) {
/* line 585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoElement)
   return yyV1;
  }
 }

  }
  if (yyP70->Kind == kAnElement) {
/* line 586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAnElement)
    yyV1->AnElement.Next = CopyElementList (yyP70->AnElement.Next);
    yyV1->AnElement.Element = yyP70->AnElement.Element;
   return yyV1;
  }
 }

  }
 yyAbort ("CopyElementList");
}

static tTree MergeElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP72, register tTree yyP71)
# else
(yyP72, yyP71)
 register tTree yyP72;
 register tTree yyP71;
# endif
{
  if (yyP71->Kind == kNoElement) {
/* line 590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP72;

  }
  if (yyP71->Kind == kAnElement) {
/* line 591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
 {
  {
  bool yyV1;
   return MergeElementList (InsertElement (yyP72, yyP71->AnElement.Element, & yyV1), yyP71->AnElement.Next);
  }
 }

  }
 yyAbort ("MergeElementList");
}

static void UsageCheckAccess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP76, pSet yyP75, register tTree yyP74, pSet yyP73)
# else
(yyP76, yyP75, yyP74, yyP73)
 register tTree yyP76;
 pSet yyP75;
 register tTree yyP74;
 pSet yyP73;
# endif
{
  if (yyP76->Kind == kNoAccess) {
  if (yyP74->Kind == kNoAccess) {
/* line 596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return;

  }
  if (yyP74->Kind == kAnAccess) {
/* line 605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP74->AnAccess.Sym, yyP75)))) goto yyL4;
  {
/* line 606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
   return;
yyL4:;

/* line 608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP73, yyP74->AnAccess.Sym);
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76, yyP75, yyP74->AnAccess.Next, yyP73);
  }
   return;

  }
  }
  if (yyP76->Kind == kAnAccess) {
  if (yyP74->Kind == kNoAccess) {
/* line 598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP76->AnAccess.Sym, yyP73)))) goto yyL2;
  {
/* line 599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74, yyP73);
  }
  }
   return;
yyL2:;

/* line 601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 602 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP75, yyP76->AnAccess.Sym);
/* line 603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74, yyP73);
  }
   return;

  }
  if (yyP74->Kind == kAnAccess) {
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym < yyP74->AnAccess.Sym))) goto yyL6;
  {
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP76->AnAccess.Sym, yyP73)))) goto yyL6;
  {
/* line 613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74, yyP73);
  }
  }
  }
   return;
yyL6:;

/* line 615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym < yyP74->AnAccess.Sym))) goto yyL7;
  {
/* line 616 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP75, yyP76->AnAccess.Sym);
/* line 617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74, yyP73);
  }
  }
   return;
yyL7:;

/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym > yyP74->AnAccess.Sym))) goto yyL8;
  {
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP74->AnAccess.Sym, yyP75)))) goto yyL8;
  {
/* line 620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
  }
   return;
yyL8:;

/* line 622 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 622 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym > yyP74->AnAccess.Sym))) goto yyL9;
  {
/* line 623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP73, yyP74->AnAccess.Sym);
/* line 624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
   return;
yyL9:;

/* line 626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym == yyP74->AnAccess.Sym))) goto yyL10;
  {
/* line 627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP76->AnAccess.Sym, yyP73) || IsElement (yyP74->AnAccess.Sym, yyP75)))) goto yyL10;
  {
/* line 628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP75, yyP76->AnAccess.Sym);
/* line 628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP73, yyP74->AnAccess.Sym);
/* line 629 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
  }
   return;
yyL10:;

/* line 631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym == yyP74->AnAccess.Sym))) goto yyL11;
  {
/* line 632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckElement (yyP76->AnAccess.ElementList, yyP74->AnAccess.ElementList))) goto yyL11;
  {
/* line 633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
  }
   return;
yyL11:;

/* line 635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP76->AnAccess.Sym == yyP74->AnAccess.Sym))) goto yyL12;
  {
/* line 636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP75, yyP76->AnAccess.Sym);
/* line 637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP73, yyP74->AnAccess.Sym);
/* line 638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckAccess (yyP76->AnAccess.Next, yyP75, yyP74->AnAccess.Next, yyP73);
  }
  }
   return;
yyL12:;

  }
  }
;
}

static bool CheckElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP78, register tTree yyP77)
# else
(yyP78, yyP77)
 register tTree yyP78;
 register tTree yyP77;
# endif
{
  if (yyP78->Kind == kNoElement) {
/* line 648 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (yyP78->Kind == kAnElement) {
/* line 650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckEachElement (yyP78->AnElement.Element, yyP77))) goto yyL2;
  {
/* line 650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckElement (yyP78->AnElement.Next, yyP77))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckEachElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP80, register tTree yyP79)
# else
(yyP80, yyP79)
 register tTree yyP80;
 register tTree yyP79;
# endif
{
  if (yyP79->Kind == kNoElement) {
/* line 658 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (yyP79->Kind == kAnElement) {
/* line 660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 661 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckElementVsElement (yyP80, yyP79->AnElement.Element))) goto yyL2;
  {
/* line 662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckEachElement (yyP80, yyP79->AnElement.Next))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckElementVsElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP82, register tTree yyP81)
# else
(yyP82, yyP81)
 register tTree yyP82;
 register tTree yyP81;
# endif
{
  if (yyP82 == NULL) {
/* line 674 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (yyP81 == NULL) {
/* line 676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (Tree_IsType (yyP82, kIdentifier)) {
  if (Tree_IsType (yyP81, kIdentifier)) {
/* line 678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckElementVsElement (ElementRevLink (yyP82), ElementRevLink (yyP81));

  }
/* line 680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP82->Identifier.Pos);
  }
   return false;

  }
  if (Tree_IsType (yyP81, kIdentifier)) {
/* line 680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP81->Identifier.Pos);
  }
   return false;

  }
  if (Tree_IsType (yyP82, kElementAccess)) {
  if (Tree_IsType (yyP81, kElementAccess)) {
/* line 683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP82)))) goto yyL6;
  }
   return CheckElementVsElement (ElementRevLink (yyP82), ElementRevLink (yyP81));
yyL6:;

/* line 685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP81)))) goto yyL7;
  }
   return CheckElementVsElement (ElementRevLink (yyP82), ElementRevLink (yyP81));
yyL7:;

  }
  }
  if (yyP82->Kind == kSubscript) {
  if (yyP81->Kind == kSubscript) {
/* line 687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! TestLinExpr_LinExpr (yyP82 -> Subscript . LE, kEqual, yyP81 -> Subscript . LE)))) goto yyL8;
  }
   return true;
yyL8:;

  }
  if (yyP81->Kind == kRange) {
/* line 691 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 691 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! TestLinExprInRange (yyP82 -> Subscript . LE, yyP81 -> Range . FromLE, yyP81 -> Range . ForLE)))) goto yyL10;
  }
   return true;
yyL10:;

  }
  }
  if (yyP82->Kind == kRange) {
  if (yyP81->Kind == kSubscript) {
/* line 689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! TestLinExprInRange (yyP81 -> Subscript . LE, yyP82 -> Range . FromLE, yyP82 -> Range . ForLE)))) goto yyL9;
  }
   return true;
yyL9:;

  }
  if (yyP81->Kind == kRange) {
/* line 693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprPlusLinExpr_LinExpr (yyP82 -> Range . FromLE, yyP82 -> Range . ForLE, kLessEqual, yyP81 -> Range . FromLE)))) goto yyL11;
  }
   return true;
yyL11:;

/* line 695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprPlusLinExpr_LinExpr (yyP81 -> Range . FromLE, yyP81 -> Range . ForLE, kLessEqual, yyP82 -> Range . FromLE)))) goto yyL12;
  }
   return true;
yyL12:;

  }
  }
  if (Tree_IsType (yyP82, kElementAccess)) {
  if (Tree_IsType (yyP81, kElementAccess)) {
/* line 697 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckElementVsElement (ElementRevLink (yyP82), ElementRevLink (yyP81));

  }
  }
 yyAbort ("CheckElementVsElement");
}

static void UsageCheckRepWRAccess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP88, pSet yyP87, register tTree yyP86, pSet yyP85, register int yyP84, tSymbol yyP83)
# else
(yyP88, yyP87, yyP86, yyP85, yyP84, yyP83)
 register tTree yyP88;
 pSet yyP87;
 register tTree yyP86;
 pSet yyP85;
 register int yyP84;
 tSymbol yyP83;
# endif
{
  if (yyP88->Kind == kNoAccess) {
/* line 703 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return;

  }
  if (yyP88->Kind == kAnAccess) {
/* line 705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Symbol (yyP88->AnAccess.Sym) . Depth > yyP84))) goto yyL2;
  }
   return;
yyL2:;

  if (yyP86->Kind == kNoAccess) {
/* line 707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP88->AnAccess.Sym, yyP85)))) goto yyL3;
  {
/* line 708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86, yyP85, yyP84, yyP83);
  }
  }
   return;
yyL3:;

/* line 710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP87, yyP88->AnAccess.Sym);
/* line 711 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86, yyP85, yyP84, yyP83);
  }
   return;

  }
  if (yyP86->Kind == kAnAccess) {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym < yyP86->AnAccess.Sym))) goto yyL5;
  {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP88->AnAccess.Sym, yyP85)))) goto yyL5;
  {
/* line 714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86, yyP85, yyP84, yyP83);
  }
  }
  }
   return;
yyL5:;

/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym < yyP86->AnAccess.Sym))) goto yyL6;
  {
/* line 717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP87, yyP88->AnAccess.Sym);
/* line 718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86, yyP85, yyP84, yyP83);
  }
  }
   return;
yyL6:;

/* line 720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym > yyP86->AnAccess.Sym))) goto yyL7;
  {
/* line 720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsElement (yyP88->AnAccess.Sym, yyP85)))) goto yyL7;
  {
/* line 721 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88, yyP87, yyP86->AnAccess.Next, yyP85, yyP84, yyP83);
  }
  }
  }
   return;
yyL7:;

/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym > yyP86->AnAccess.Sym))) goto yyL8;
  {
/* line 724 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP87, yyP88->AnAccess.Sym);
/* line 725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88, yyP87, yyP86->AnAccess.Next, yyP85, yyP84, yyP83);
  }
  }
   return;
yyL8:;

/* line 727 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 727 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym == yyP86->AnAccess.Sym))) goto yyL9;
  {
/* line 728 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepWRElementList (yyP88->AnAccess.ElementList, yyP86->AnAccess.ElementList, yyP83))) goto yyL9;
  {
/* line 729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86->AnAccess.Next, yyP85, yyP84, yyP83);
  }
  }
  }
   return;
yyL9:;

/* line 731 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 731 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP88->AnAccess.Sym == yyP86->AnAccess.Sym))) goto yyL10;
  {
/* line 732 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP87, yyP88->AnAccess.Sym);
/* line 733 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP85, yyP86->AnAccess.Sym);
/* line 734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepWRAccess (yyP88->AnAccess.Next, yyP87, yyP86->AnAccess.Next, yyP85, yyP84, yyP83);
  }
  }
   return;
yyL10:;

  }
  }
;
}

static bool CheckRepWRElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP91, register tTree yyP90, tSymbol yyP89)
# else
(yyP91, yyP90, yyP89)
 register tTree yyP91;
 register tTree yyP90;
 tSymbol yyP89;
# endif
{
  if (yyP91->Kind == kAnElement) {
/* line 742 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepWRElementList (yyP91->AnElement.Next, yyP90, yyP89))) goto yyL1;
  {
/* line 744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepEachElement (yyP91->AnElement.Element, yyP90, yyP89))) goto yyL1;
  }
  }
   return true;
yyL1:;

  }
  if (yyP91->Kind == kNoElement) {
/* line 746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  return false;
}

static void UsageCheckRepAccess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP95, pSet yyP94, register int yyP93, tSymbol yyP92)
# else
(yyP95, yyP94, yyP93, yyP92)
 register tTree yyP95;
 pSet yyP94;
 register int yyP93;
 tSymbol yyP92;
# endif
{
  if (yyP95->Kind == kNoAccess) {
/* line 751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return;

  }
  if (yyP95->Kind == kAnAccess) {
/* line 752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Symbol (yyP95->AnAccess.Sym) . Depth <= yyP93))) goto yyL2;
  {
/* line 752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepElementList (yyP95->AnAccess.ElementList, yyP92))) goto yyL2;
  {
/* line 752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepAccess (yyP95->AnAccess.Next, yyP94, yyP93, yyP92);
  }
  }
  }
   return;
yyL2:;

/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((Symbol (yyP95->AnAccess.Sym) . Depth <= yyP93))) goto yyL3;
  {
/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Include (yyP94, yyP95->AnAccess.Sym);
/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   UsageCheckRepAccess (yyP95->AnAccess.Next, yyP94, yyP93, yyP92);
  }
  }
   return;
yyL3:;

/* line 754 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return;

  }
;
}

static bool CheckRepElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP97, tSymbol yyP96)
# else
(yyP97, yyP96)
 register tTree yyP97;
 tSymbol yyP96;
# endif
{
  if (yyP97->Kind == kAnElement) {
/* line 759 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepElementList (yyP97->AnElement.Next, yyP96))) goto yyL1;
  {
/* line 761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepEachElement (yyP97->AnElement.Element, yyP97, yyP96))) goto yyL1;
  }
  }
   return true;
yyL1:;

  }
  if (yyP97->Kind == kNoElement) {
/* line 763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  return false;
}

static bool CheckRepEachElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP100, register tTree yyP99, tSymbol yyP98)
# else
(yyP100, yyP99, yyP98)
 register tTree yyP100;
 register tTree yyP99;
 tSymbol yyP98;
# endif
{
  if (yyP99->Kind == kNoElement) {
/* line 767 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (yyP99->Kind == kAnElement) {
/* line 768 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 769 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepElementVsElement (yyP100, yyP99->AnElement.Element, yyP98))) goto yyL2;
  {
/* line 770 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckRepEachElement (yyP100, yyP99->AnElement.Next, yyP98))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckRepElementVsElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP103, register tTree yyP102, tSymbol yyP101)
# else
(yyP103, yyP102, yyP101)
 register tTree yyP103;
 register tTree yyP102;
 tSymbol yyP101;
# endif
{
  if (yyP103 == NULL) {
/* line 782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (yyP102 == NULL) {
/* line 783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (Tree_IsType (yyP103, kIdentifier)) {
  if (Tree_IsType (yyP102, kIdentifier)) {
/* line 784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckRepElementVsElement (ElementRevLink (yyP103), ElementRevLink (yyP102), yyP101);

  }
/* line 786 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP103->Identifier.Pos);
  }
   return false;

  }
  if (Tree_IsType (yyP102, kIdentifier)) {
/* line 786 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   Message ("Internal parallel usage error", xxError, yyP102->Identifier.Pos);
  }
   return false;

  }
  if (Tree_IsType (yyP103, kElementAccess)) {
  if (Tree_IsType (yyP102, kElementAccess)) {
/* line 789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP103)))) goto yyL6;
  }
   return CheckRepElementVsElement (ElementRevLink (yyP103), ElementRevLink (yyP102), yyP101);
yyL6:;

/* line 792 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 793 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsSimpEl (yyP102)))) goto yyL7;
  }
   return CheckRepElementVsElement (ElementRevLink (yyP103), ElementRevLink (yyP102), yyP101);
yyL7:;

  }
  }
  if (yyP103->Kind == kSubscript) {
  if (yyP102->Kind == kSubscript) {
/* line 795 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprsDistinctWrtRep (yyP103 -> Subscript . LE, yyP102 -> Subscript . LE, yyP101)))) goto yyL8;
  }
   return true;
yyL8:;

  }
  if (yyP102->Kind == kRange) {
/* line 801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 802 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprsDistinctWrtRepOverRange (yyP103 -> Subscript . LE, yyP102 -> Range . FromLE, yyP101, NULL, yyP102 -> Range . ForLE)))) goto yyL10;
  }
   return true;
yyL10:;

  }
  }
  if (yyP103->Kind == kRange) {
  if (yyP102->Kind == kSubscript) {
/* line 798 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprsDistinctWrtRepOverRange (yyP102 -> Subscript . LE, yyP103 -> Range . FromLE, yyP101, NULL, yyP103 -> Range . ForLE)))) goto yyL9;
  }
   return true;
yyL9:;

  }
  if (yyP102->Kind == kRange) {
/* line 804 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((TestLinExprsDistinctWrtRepOverRange (yyP103 -> Range . FromLE, yyP102 -> Range . FromLE, yyP101, yyP103 -> Range . ForLE, yyP102 -> Range . ForLE)))) goto yyL11;
  }
   return true;
yyL11:;

  }
  }
  if (Tree_IsType (yyP103, kElementAccess)) {
  if (Tree_IsType (yyP102, kElementAccess)) {
/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckRepElementVsElement (ElementRevLink (yyP103), ElementRevLink (yyP102), yyP101);

  }
  }
 yyAbort ("CheckRepElementVsElement");
}

bool CheckForValAbbrev
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP106, register tTree yyP105, register tTree yyP104)
# else
(yyP106, yyP105, yyP104)
 tSymbol yyP106;
 register tTree yyP105;
 register tTree yyP104;
# endif
{
  if (yyP105 == NULL) {
/* line 813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (Tree_IsType (yyP105, kAVal)) {
/* line 815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 816 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP106, & yyP105->AVal.Expr -> Expr . PUOut -> R)))) goto yyL2;
  }
   return true;
yyL2:;

  if (yyP104->Kind == kNoElement) {
/* line 818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckAccessForSymbol (yyP105->AVal.Expr -> Expr . PUOut -> RAL, yyP106);

  }
/* line 820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! CheckAccessVsElementList (yyP105->AVal.Expr -> Expr . PUOut -> RAL, yyP106, yyP104)))) goto yyL4;
  }
   return true;
yyL4:;

  }
/* line 823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

}

bool CheckForAbbrev
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP109, register tTree yyP108, register tTree yyP107)
# else
(yyP109, yyP108, yyP107)
 tSymbol yyP109;
 register tTree yyP108;
 register tTree yyP107;
# endif
{
  if (yyP108 == NULL) {
/* line 828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

  }
  if (yyP108->Kind == kIs) {
/* line 830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 831 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP109, & yyP108->Is.Elemental -> Element . PUOut -> W)))) goto yyL2;
  }
   return true;
yyL2:;

  if (yyP107->Kind == kNoElement) {
/* line 833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckAccessForSymbol (yyP108->Is.Elemental -> Element . PUOut -> WAL, yyP109);

  }
/* line 835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! CheckAccessVsElementList (yyP108->Is.Elemental -> Element . PUOut -> WAL, yyP109, yyP107)))) goto yyL6;
  }
   return true;
yyL6:;

  }
  if (Tree_IsType (yyP108, kRetypes)) {
/* line 830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 831 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP109, & yyP108->Retypes.Elemental -> Element . PUOut -> W)))) goto yyL3;
  }
   return true;
yyL3:;

  if (yyP107->Kind == kNoElement) {
/* line 833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return CheckAccessForSymbol (yyP108->Retypes.Elemental -> Element . PUOut -> WAL, yyP109);

  }
/* line 835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! CheckAccessVsElementList (yyP108->Retypes.Elemental -> Element . PUOut -> WAL, yyP109, yyP107)))) goto yyL7;
  }
   return true;
yyL7:;

  }
/* line 838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;

}

bool CheckForValAbbrevInProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP111, tParUsage yyP110)
# else
(yyP111, yyP110)
 register tTree yyP111;
 tParUsage yyP110;
# endif
{
  if (yyP111 == NULL) {
/* line 843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;
  }

  }
  if (yyP110 == NULL) {
/* line 844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;
  }

  }
  if (Tree_IsType (yyP111, kAVal)) {
/* line 845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsDisjoint (& yyP111->AVal.Expr -> Expr . PUOut -> R, & yyP110 -> W)))) goto yyL3;
  }
   return true;
yyL3:;

/* line 848 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckAccessForAlias (yyP111->AVal.Expr -> Expr . PUOut -> RAL, & yyP111->AVal.Expr -> Expr . PUOut -> R, yyP110 -> WAL, & yyP110 -> W)))) goto yyL4;
  }
   return true;
yyL4:;

  }
  return false;
}

bool CheckForAbbrevInProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP113, tParUsage yyP112)
# else
(yyP113, yyP112)
 register tTree yyP113;
 tParUsage yyP112;
# endif
{
  if (yyP112 == NULL) {
/* line 854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;
  }

  }
  if (yyP113 == NULL) {
/* line 854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return false;
  }

  }
  if (yyP113->Kind == kIs) {
/* line 857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 858 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsDisjoint (& yyP113->Is.Elemental -> Element . PUOut -> W, & yyP112 -> W)))) goto yyL3;
  }
   return true;
yyL3:;

/* line 860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsDisjoint (& yyP113->Is.Elemental -> Element . PUOut -> W, & yyP112 -> R)))) goto yyL5;
  }
   return true;
yyL5:;

/* line 863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckAccessForAlias (yyP113->Is.Elemental -> Element . PUOut -> WAL, & yyP113->Is.Elemental -> Element . PUOut -> W, yyP112 -> WAL, & yyP112 -> W)))) goto yyL7;
  }
   return true;
yyL7:;

/* line 866 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 867 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckAccessForAlias (yyP113->Is.Elemental -> Element . PUOut -> WAL, & yyP113->Is.Elemental -> Element . PUOut -> W, yyP112 -> RAL, & yyP112 -> R)))) goto yyL9;
  }
   return true;
yyL9:;

  }
  if (Tree_IsType (yyP113, kRetypes)) {
/* line 857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 858 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsDisjoint (& yyP113->Retypes.Elemental -> Element . PUOut -> W, & yyP112 -> W)))) goto yyL4;
  }
   return true;
yyL4:;

/* line 860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! IsDisjoint (& yyP113->Retypes.Elemental -> Element . PUOut -> W, & yyP112 -> R)))) goto yyL6;
  }
   return true;
yyL6:;

/* line 863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckAccessForAlias (yyP113->Retypes.Elemental -> Element . PUOut -> WAL, & yyP113->Retypes.Elemental -> Element . PUOut -> W, yyP112 -> WAL, & yyP112 -> W)))) goto yyL8;
  }
   return true;
yyL8:;

/* line 866 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 867 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((CheckAccessForAlias (yyP113->Retypes.Elemental -> Element . PUOut -> WAL, & yyP113->Retypes.Elemental -> Element . PUOut -> W, yyP112 -> RAL, & yyP112 -> R)))) goto yyL10;
  }
   return true;
yyL10:;

  }
  return false;
}

static bool CheckAccessForSymbol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP115, tSymbol yyP114)
# else
(yyP115, yyP114)
 register tTree yyP115;
 tSymbol yyP114;
# endif
{
  if (yyP115->Kind == kAnAccess) {
/* line 872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP115->AnAccess.Sym < yyP114))) goto yyL1;
  {
/* line 874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForSymbol (yyP115->AnAccess.Next, yyP114))) goto yyL1;
  }
  }
   return true;
yyL1:;

/* line 876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP115->AnAccess.Sym == yyP114))) goto yyL2;
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckAccessForAlias
# if defined __STDC__ | defined __cplusplus
(register tTree yyP119, pSet yyP118, register tTree yyP117, pSet yyP116)
# else
(yyP119, yyP118, yyP117, yyP116)
 register tTree yyP119;
 pSet yyP118;
 register tTree yyP117;
 pSet yyP116;
# endif
{
  if (yyP119->Kind == kAnAccess) {
  if (yyP117->Kind == kNoAccess) {
/* line 882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 883 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP119->AnAccess.Sym, yyP116)))) goto yyL1;
  }
   return true;
yyL1:;

/* line 885 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119->AnAccess.Next, yyP118, yyP117, yyP116))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (yyP117->Kind == kAnAccess) {
/* line 894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym < yyP117->AnAccess.Sym))) goto yyL5;
  {
/* line 896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP119->AnAccess.Sym, yyP116)))) goto yyL5;
  }
  }
   return true;
yyL5:;

/* line 898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym < yyP117->AnAccess.Sym))) goto yyL6;
  {
/* line 900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119->AnAccess.Next, yyP118, yyP117, yyP116))) goto yyL6;
  }
  }
   return true;
yyL6:;

/* line 902 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym > yyP117->AnAccess.Sym))) goto yyL7;
  {
/* line 904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP117->AnAccess.Sym, yyP118)))) goto yyL7;
  }
  }
   return true;
yyL7:;

/* line 906 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 907 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym > yyP117->AnAccess.Sym))) goto yyL8;
  {
/* line 908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119, yyP118, yyP117->AnAccess.Next, yyP116))) goto yyL8;
  }
  }
   return true;
yyL8:;

/* line 910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 911 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym == yyP117->AnAccess.Sym))) goto yyL9;
  {
/* line 912 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((! CheckElement (yyP119->AnAccess.ElementList, yyP117->AnAccess.ElementList)))) goto yyL9;
  {
/* line 913 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119->AnAccess.Next, yyP118, yyP117->AnAccess.Next, yyP116))) goto yyL9;
  }
  }
  }
   return true;
yyL9:;

/* line 915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP119->AnAccess.Sym == yyP117->AnAccess.Sym))) goto yyL10;
  {
/* line 917 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119->AnAccess.Next, yyP118, yyP117->AnAccess.Next, yyP116))) goto yyL10;
  }
  }
   return true;
yyL10:;

  }
  }
  if (yyP119->Kind == kNoAccess) {
  if (yyP117->Kind == kAnAccess) {
/* line 888 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((IsElement (yyP117->AnAccess.Sym, yyP118)))) goto yyL3;
  }
   return true;
yyL3:;

/* line 891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessForAlias (yyP119, yyP118, yyP117->AnAccess.Next, yyP116))) goto yyL4;
  }
   return true;
yyL4:;

  }
  }
  return false;
}

static bool CheckAccessVsElementList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP122, tSymbol yyP121, register tTree yyP120)
# else
(yyP122, yyP121, yyP120)
 register tTree yyP122;
 tSymbol yyP121;
 register tTree yyP120;
# endif
{
  if (yyP122->Kind == kNoAccess) {
/* line 926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  if (yyP122->Kind == kAnAccess) {
/* line 928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 929 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP122->AnAccess.Sym > yyP121))) goto yyL2;
  }
   return true;
yyL2:;

/* line 931 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 932 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP122->AnAccess.Sym < yyP121))) goto yyL3;
  {
/* line 933 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckAccessVsElementList (yyP122->AnAccess.Next, yyP121, yyP120))) goto yyL3;
  }
  }
   return true;
yyL3:;

/* line 935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! ((yyP122->AnAccess.Sym == yyP121))) goto yyL4;
  {
/* line 937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (CheckElement (yyP120, yyP122->AnAccess.ElementList))) goto yyL4;
  }
  }
   return true;
yyL4:;

  }
  return false;
}

static bool IsChanType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP123)
# else
(yyP123)
 register tTree yyP123;
# endif
{
  if (Tree_IsType (yyP123, kArray)) {
/* line 943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
  {
/* line 943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   if (! (IsChanType (yyP123->Array.Type))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP123->Kind == kChan) {
/* line 944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return true;

  }
  return false;
}

static tTree ElementRevLink
# if defined __STDC__ | defined __cplusplus
(register tTree yyP124)
# else
(yyP124)
 register tTree yyP124;
# endif
{
/* line 948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */
   return yyP124 -> Element . RVL;

}

void BeginParUsage ()
{
/* line 219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/parusage.trafo" */

  if (IS_Mode(xxUsageWarnings)) UEMode=xxWarning;

}

void CloseParUsage ()
{
}
