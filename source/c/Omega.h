/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Omega Test interface header file
||@(#)   System : occam 2 c  (from 1.6)
||@(#) Filename : Omega.h
||@(#)  Version : 1.2
||@(#)     Date : 11/5/93
\*@(#)====================================================*/
/*}}}*/


#ifndef Included_Omega
#define Included_Omega

#include "Sets.h"

struct tLinExpr
{
  bool Const;
  int Value;
  tSet Used;
  int *Coefs;
} ;

extern void InitOmega();
extern pLinExpr CreateLinExpr(tTree E);
extern void AddLinExprConst(pLinExpr LE, int V);
extern void AddLinExprCoef(pLinExpr LE, tSymbol S, int V);

extern bool TestLinExpr_Const(pLinExpr LE, Tree_tKind OP, int C);
extern bool TestLinExpr_LinExpr(pLinExpr LE, Tree_tKind OP, pLinExpr RE);
extern bool TestLinExprPlusLinExpr_LinExpr(pLinExpr LE1, pLinExpr LE2, Tree_tKind OP, pLinExpr RE);
extern bool SameLinExpr(pLinExpr LE1, pLinExpr LE2);
extern bool TestLinExprInRange(pLinExpr E, pLinExpr FROME, pLinExpr FORE);

extern bool TestLinExprsDistinctWrtRep(pLinExpr LE1, pLinExpr LE2, tSymbol R);
extern bool TestLinExprsDistinctWrtRepOverRange(pLinExpr LE1, pLinExpr LE2, tSymbol R, pLinExpr J, pLinExpr K);
extern bool TestLinExprUsesSym(pLinExpr LE, tSymbol R);
#endif
