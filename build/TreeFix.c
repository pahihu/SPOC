# include "TreeFix.h"
# include "yyTreeFix.w"
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


static void yyExit () { Exit (1); }

void (* TreeFix_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module TreeFix, routine %s failed\n", yyFunction);
 TreeFix_Exit ();
}

tTree mArrayFromExprList ARGS((tTree yyP2, tTree yyP1));
tTree mTableElementFromExprList ARGS((tTree yyP5, tPosition yyP4, int yyP3));
tTree CatFieldList ARGS((tTree yyP7, tTree yyP6));

tTree mArrayFromExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP2, register tTree yyP1)
# else
(yyP2, yyP1)
 register tTree yyP2;
 register tTree yyP1;
# endif
{
  if (yyP2->Kind == kNoExpr) {
/* line 20 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   return yyP1;

  }
  if (yyP2->Kind == kAnExpr) {
/* line 21 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
    yyV1->Array.Expr = yyP2->AnExpr.Expr;
    yyV1->Array.Type = mArrayFromExprList (yyP2->AnExpr.Next, yyP1);
   return yyV1;
  }
 }

  }
 yyAbort ("mArrayFromExprList");
}

tTree mTableElementFromExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP5, register tPosition yyP4, register int yyP3)
# else
(yyP5, yyP4, yyP3)
 register tTree yyP5;
 register tPosition yyP4;
 register int yyP3;
# endif
{
  if (yyP5->Kind == kNoExpr) {
/* line 24 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
 {
  {
/* line 24 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   Message ("Badly formed table", xxError, yyP4);
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoExpr)
   return mTable (yyP4, yyV1, yyP3, 0);
  }
 }

  }
  if (yyP5->Kind == kAnExpr) {
  if (yyP5->AnExpr.Expr->Kind == kUnBound) {
/* line 25 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
 {
  {
/* line 25 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   Message ("Empty braces in table literal", xxError, yyP4);
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoExpr)
   return mTable (yyP4, yyV1, yyP3, 0);
  }
 }

  }
  if (yyP5->AnExpr.Next->Kind == kNoExpr) {
/* line 26 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   return mTable (yyP4, yyP5, yyP3, AddSymbol (NoIdent));

  }
/* line 27 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kTableSubscript)
    yyV1->TableSubscript.Pos = yyP4;
    yyV1->TableSubscript.Next = mTableElementFromExprList (yyP5->AnExpr.Next, yyP4, yyP3);
    yyV1->TableSubscript.Expr = yyP5->AnExpr.Expr;
   return yyV1;
  }
 }

  }
 yyAbort ("mTableElementFromExprList");
}

tTree CatFieldList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP7, register tTree yyP6)
# else
(yyP7, yyP6)
 register tTree yyP7;
 register tTree yyP6;
# endif
{
  if (yyP7->Kind == kNoField) {
/* line 30 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   return yyP6;

  }
  if (yyP7->Kind == kAField) {
/* line 31 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
  {
/* line 31 "/home/hpcc/SPOC/spoc.1.3/source/gmd/treefix.trafo" */
   yyP7->AField.Next = CatFieldList (yyP7->AField.Next, yyP6);
  }
   return yyP7;

  }
 yyAbort ("CatFieldList");
}

void BeginTreeFix ()
{
}

void CloseTreeFix ()
{
}
