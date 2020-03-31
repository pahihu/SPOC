/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Omega test interface for occam 2 c (from 1.12)
||@(#)   System : occam 2 c
||@(#) Filename : Omega.c
||@(#)  Version : 1.4
||@(#)     Date : 10/28/93
\*@(#)====================================================*/
/*}}}*/

#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"
#include "ParUsage.h"
#include "ip.h"

int debugLevel=0;
int omegaPrintResult=0;

int RepCount;
int *MapReps;
#define EQ(P) ((P)->_EQs[(P)->_numEQs-1].coef)
#define GEQ(P) ((P)->_GEQs[(P)->_numGEQs-1].coef)

/*{{{  static void Fail(int line,char *s)*/
static void Fail(int line,char *s)
{
  fprintf(stderr,"Fatal-%s(%d)- Omega test failed due to %s\n",__FILE__,line,s);
  exit(-1);
}
/*}}}*/

/*{{{  void InitOmega()*/
void InitOmega()
{
  MapReps = (int*)malloc(sizeof(int)*(SymSize+1));
  {
    int i;
    RepCount=0;
    for (i=1;i<=SymSize;i++)
      if (Tree_IsType(SymbolKind(i),kReplicator))
        MapReps[i]= RepCount++;
      else
        MapReps[i]= -1;
  }
  setOutputFile(stdout);
  initializeOmega();
}  
/*}}}*/
/*{{{  pLinExpr CreateLinExpr(tTree E)*/
pLinExpr CreateLinExpr(tTree E)
{
  tLinExpr *LE = (tLinExpr*)malloc(sizeof(tLinExpr));
  if (IsConstInt(E))
  {
    LE->Coefs = NULL;
    LE->Const = true;
    LE->Value = ValConstInt(E);
  }
  else
  {
    MakeSet(&LE->Used,RepCount);
    LE->Coefs = (int*)malloc(sizeof(int)*RepCount);
    LE->Const = false;
    {
      int i;
      for (i=0;i<RepCount;i++)
        LE->Coefs[i] = 0;
    }
    LE->Value = 0;
    SimplifyLinExpr(LE,E,+1);
  }
  return(LE);
}
/*}}}*/
/*{{{  void AddLinExprConst(pLinExpr LE, int V)*/
void AddLinExprConst(pLinExpr LE, int V)
{
  LE->Value += V;
}
/*}}}*/
/*{{{  void AddLinExprCoef(pLinExpr LE, tSymbol S, int V)*/
void AddLinExprCoef(pLinExpr LE, tSymbol S, int V)
{
  LE->Coefs[MapReps[S]] += V;
  Include(&LE->Used,MapReps[S]);
  if (!SymbolKind(S)->Replicator.FromLE->Const)
    Union(&LE->Used,&SymbolKind(S)->Replicator.FromLE->Used);
  if (!SymbolKind(S)->Replicator.ForLE->Const)
    Union(&LE->Used,&SymbolKind(S)->Replicator.ForLE->Used);
}
/*}}}*/
/*{{{  void WriteLinExpr(FILE *F, pLinExpr LE)*/
void WriteLinExpr(FILE *F, pLinExpr LE)
{
  if (LE==NULL)
    fprintf(F,"non-linear");
  else
  {
    fprintf(F,"%d",LE->Value);
    if (!LE->Const)
    {
      int i;
      for (i=1;i<SymSize;i++)
        if (MapReps[i]>=0)
          if (IsElement(MapReps[i],&LE->Used))
          {
            fprintf(F,"+%d.",LE->Coefs[MapReps[i]]);
            WriteIdent(F,Symbol(i).Id);      
          }
    }
  }
}
/*}}}*/
/*{{{  static void Prob_NewInequality(Problem *P)*/
static void Prob_NewInequality(Problem *P)
{
  int i;
  if (P->_numGEQs>=maxGEQs) Fail(__LINE__,"Inquality exhaustion");
  P->_GEQs[P->_numGEQs].touched = true;
  P->_GEQs[P->_numGEQs].key = 0;
  P->_GEQs[P->_numGEQs].color = 0;
  for (i=0;i<=P->_nVars;i++)
    P->_GEQs[P->_numGEQs].coef[i] = 0;
  P->_numGEQs++;
}
/*}}}*/
/*{{{  static void Prob_NewEquality(Problem *P)*/
static void Prob_NewEquality(Problem *P)
{
  int i;
  if (P->_numEQs>=maxEQs) Fail(__LINE__,"Equality exhaustion");
  P->_EQs[P->_numEQs].touched = true;
  P->_EQs[P->_numEQs].key = 0;
  P->_EQs[P->_numEQs].color = 0;
  for (i=0;i<=P->_nVars;i++)
    P->_EQs[P->_numEQs].coef[i] = 0;
  P->_numEQs++;
}
/*}}}*/
/*{{{  static int GetSymbolIndex(tSet *U,tSymbol S)*/
static int GetSymbolIndex(tSet *U,tSymbol S)
{
  int j=1;
  int i;

  for (i=Minimum(U);i<MapReps[S];i++)
    if (IsElement(i,U))
      j++;
  return j;
} 
/*}}}*/
/*{{{  static void Prob_AddLinExpr(int *C,tSet *U, pLinExpr LE)*/
static void Prob_AddLinExpr(int *C,tSet *U, pLinExpr LE)
{
  int i;
  int j=1;

  C[0] += LE->Value;
  if (!LE->Const)
    for (i=Minimum(U);i<=Maximum(U);i++)
      if (IsElement(i,U))
        C[j++] += LE->Coefs[i];
}
/*}}}*/
/*{{{  static void Prob_SubLinExpr(int *C,tSet *U, pLinExpr LE)*/
static void Prob_SubLinExpr(int *C,tSet *U, pLinExpr LE)
{
  int i;
  int j=1;
  C[0] -= LE->Value;
  if (!LE->Const)
    for (i=Minimum(U);i<=Maximum(U);i++)
      if (IsElement(i,U))
        C[j++] -= LE->Coefs[i];
}
/*}}}*/
/*{{{  static void Prob_CompLinExpr_Const(Problem *P,tSet *U, pLinExpr LE, Tree_tKind K, int C)*/
static void Prob_CompLinExpr_Const(Problem *P,tSet *U, pLinExpr LE, Tree_tKind K, int C)
{
  int *coef = GEQ(P);
  switch(K)
  {
      case kLess : coef[0] += C-1; break;
      case kLessEqual : coef[0] += C; break;
      case kGreater : coef[0] += -(C+1); break;
      case kGreaterEqual : coef[0] += -C; break;
      default : fprintf(stderr,"Fatal- Invalid comparison operator\n"); exit(-1);
  }
  if (K==kLess || K==kLessEqual)
    Prob_SubLinExpr(coef,U,LE);
  else
    Prob_AddLinExpr(coef,U,LE);
}
/*}}}*/
/*{{{  static void Prob_AddReplicatorBound(tSymbol S, int R, tSet *U, Problem *P)*/
static void Prob_AddReplicatorBound(tSymbol S, int R, tSet *U, Problem *P)
{
  pLinExpr FromLE = SymbolKind(S)->Replicator.FromLE;
  pLinExpr ForLE = SymbolKind(S)->Replicator.ForLE;

  Prob_NewInequality(P);
  Prob_CompLinExpr_Const(P,U,FromLE,kLessEqual,0);
  GEQ(P)[R] = 1;
  Prob_NewInequality(P);
  Prob_CompLinExpr_Const(P,U,FromLE,kGreater,0);
  Prob_AddLinExpr(GEQ(P),U,ForLE);
  GEQ(P)[R] = -1;
}
/*}}}*/
/*{{{  static void Prob_AddReplicatorBounds(tSet *U,Problem *P)*/
static void Prob_AddReplicatorBounds(tSet *U,Problem *P)
{
  int i,s;
  i=1;
  for (s=1;s<=SymSize;s++)
    if ((MapReps[s]>=0) && IsElement(MapReps[s],U))
      Prob_AddReplicatorBound(s,GetSymbolIndex(U,s),U,P);
}
/*}}}*/
/*{{{  static void Prob_AddPrimedReplicatorBounds(tSymbol rep, int index, tSet *U, Problem *P)*/
static void Prob_AddPrimedReplicatorBounds(tSymbol rep, int index, tSet *U, Problem *P)
{
  tSymbol s;
  for (s=rep;s<=SymSize;s++)
    if ((MapReps[s]>=0)&& IsElement(MapReps[s],U))
      Prob_AddReplicatorBound(s,index++,U,P);
}
/*}}}*/
/*{{{  static int GetNewSymbolIndex(Problem *P)*/
static int GetNewSymbolIndex(Problem *P)
{
  P->_safeVars++;
  if (P->_nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");
  return ++(P->_nVars);
}
/*}}}*/
/*{{{  static int GetNewPrimedRepsBelowRep(int Rep, tSet *U, Problem *p)*/
static int GetNewPrimedRepsBelowRep(int Rep, tSet *U, Problem *p)
{
  int i;
  int Rsub = Rep;
  for (i=Rep;i<=Card(U);i++)
    if (i==Rep)
      Rsub=GetNewSymbolIndex(p);
    else
      GetNewSymbolIndex(p);
  return(Rsub);
}
/*}}}*/
/*{{{  static void PrimeAllRepsBelowRepInLinExpr(int from,int to, int * C, tSet *U)*/
static void PrimeAllRepsBelowRepInLinExpr(int from,int to, int * C, tSet *U)
{
  for (;from<=Card(U);from++)
  {
    C[to++] = C[from];
    C[from] = 0;
  }
}
/*}}}*/
/*{{{  bool TestLinExpr_Const(pLinExpr LE, Tree_tKind OP, int C)*/
bool TestLinExpr_Const(pLinExpr LE, Tree_tKind OP, int C)
{
  if (LE->Const)
  {
    switch(OP)
    {
      case kLess : return (LE->Const<C);;
      case kGreater : return (LE->Const>C);
      case kGreaterEqual : return (LE->Const>=C);
      case kLessEqual : return (LE->Const<=C);
      case kEqual : return (LE->Const==C);
    }
  }
  else
  {
    Problem P;
 
    initializeProblem(&P);
    P._nVars = Card(&LE->Used);
    P._safeVars = P._nVars;
    if (P._nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");
    if (OP==kEqual)
    {
      Prob_NewEquality(&P);
      {
        int *coef = EQ(&P);
        Prob_AddLinExpr(coef,&LE->Used,LE);
        coef[0] -= C;
      }
    }
    else
    {
      Prob_NewInequality(&P);
      Prob_CompLinExpr_Const(&P,&LE->Used,LE,OP,C);
    }  
    Prob_AddReplicatorBounds(&LE->Used,&P);
    return(simplifyProblem(&P));
  }
}
/*}}}*/
/*{{{  bool TestLinExpr_LinExpr(pLinExpr LE, Tree_tKind OP, pLinExpr RE)*/
bool TestLinExpr_LinExpr(pLinExpr LE, Tree_tKind OP, pLinExpr RE)
{
  Problem P;
  tSet U;
 
  MakeSet(&U,RepCount);
  if (!LE->Const) Union(&U,&LE->Used);
  if (!RE->Const) Union(&U,&RE->Used);
  
  initializeProblem(&P);
  P._nVars = Card(&U);
  P._safeVars = P._nVars;
  if (P._nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");

  if (OP==kEqual)
  {
    Prob_NewEquality(&P);
    {
      int *coef = EQ(&P);
      Prob_AddLinExpr(coef,&U,LE);
      Prob_SubLinExpr(coef,&U,RE);
    }
  }
  else
  {
    Prob_NewInequality(&P);
    if (kLess==OP || kLessEqual==OP)
      Prob_AddLinExpr(GEQ(&P),&U,RE);
    else
      Prob_SubLinExpr(GEQ(&P),&U,RE);
    Prob_CompLinExpr_Const(&P,&U,LE,OP,0);
  }
 
  Prob_AddReplicatorBounds(&U,&P);
  return(simplifyProblem(&P));
}
/*}}}*/
/*{{{  bool TestLinExprPlusLinExpr_LinExpr(pLinExpr LE1, pLinExpr LE2, Tree_tKind OP, pLinExpr RE)*/
bool TestLinExprPlusLinExpr_LinExpr(pLinExpr LE1, pLinExpr LE2, Tree_tKind OP, pLinExpr RE)
{
  Problem P;
  tSet U;
 
  MakeSet(&U,RepCount);
  if (!LE1->Const) Union(&U,&LE1->Used);
  if (!LE2->Const) Union(&U,&LE2->Used);
  if (!RE->Const) Union(&U,&RE->Used);
  
  initializeProblem(&P);
  P._nVars = Card(&U);
  P._safeVars = P._nVars;
  if (P._nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");

  Prob_NewInequality(&P);
  Prob_CompLinExpr_Const(&P,&U,LE1,OP,0);
  if (kLess==OP || kLessEqual==OP)
  {
    Prob_SubLinExpr(GEQ(&P),&U,LE2);
    Prob_AddLinExpr(GEQ(&P),&U,RE);
  }
  else
  {
    Prob_AddLinExpr(GEQ(&P),&U,LE2);
    Prob_SubLinExpr(GEQ(&P),&U,RE);
  }
  Prob_AddReplicatorBounds(&U,&P);
  return(simplifyProblem(&P));
}
/*}}}*/
/*{{{  bool SameLinExpr(pLinExpr LE1, pLinExpr LE2)*/
bool SameLinExpr(pLinExpr LE1, pLinExpr LE2)
{
  int i;
  if (LE1->Value!=LE2->Value) return(false);
  if (LE1->Const && LE2->Const) return(true);
  if (LE1->Const != LE2->Const) return(false);
  if (!IsEqual(&LE1->Used,&LE2->Used)) return(false);
  for (i=0;i<RepCount;i++)
    if (LE1->Coefs[i]!=LE2->Coefs[i]) return(false);
  return(true);
} 
/*}}}*/
/*{{{  bool TestLinExprInRange(pLinExpr E, pLinExpr FROME, pLinExpr FORE)*/
bool TestLinExprInRange(pLinExpr E, pLinExpr FROME, pLinExpr FORE)
{
  Problem P;
  tSet U;

  MakeSet(&U,RepCount);
  if (!E->Const) Union(&U,&E->Used);
  if (!FROME->Const) Union(&U,&FROME->Used);
  if (!FORE->Const) Union(&U,&FORE->Used);

  initializeProblem(&P);
  P._nVars = Card(&U);
  P._safeVars = P._nVars;
  if (P._nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");

  Prob_NewInequality(&P);
  Prob_AddLinExpr(GEQ(&P),&U,E);
  Prob_SubLinExpr(GEQ(&P),&U,FROME);      

  Prob_NewInequality(&P);
  Prob_CompLinExpr_Const(&P,&U,FROME,kGreater,0);
  Prob_AddLinExpr(GEQ(&P),&U,FORE);
  Prob_SubLinExpr(GEQ(&P),&U,E);

  Prob_AddReplicatorBounds(&U,&P);
  return(simplifyProblem(&P));
}
/*}}}*/
/*{{{  bool TestLinExprUsesSym(pLinExpr LE, tSymbol R)*/
bool TestLinExprUsesSym(pLinExpr LE, tSymbol R)
{
  return ((!LE->Const) && LE->Coefs[MapReps[R]]!=0);
}
/*}}}*/

/*{{{  bool TestLinExprsDistinctWrtRepOverRange(pLinExpr LE1, pLinExpr LE2, tSymbol R, pLinExpr FOR1, pLinExpr FOR2)*/
bool TestLinExprsDistinctWrtRepOverRange(pLinExpr LE1, pLinExpr LE2, tSymbol R, pLinExpr FOR1, pLinExpr FOR2)
{
  Problem P;
  tSet U;
  int iR, iF1, iF2, iRsub;
  int i;
  bool BothUseR = (TestLinExprUsesSym(LE1,R) && TestLinExprUsesSym(LE2,R));
  
  MakeSet(&U,RepCount);
  if (!LE1->Const) Union(&U,&LE1->Used);
  if (!LE2->Const) Union(&U,&LE2->Used);
  if (FOR1!=NULL && !FOR1->Const) Union(&U,&FOR1->Used);
  if (FOR1!=NULL && !FOR2->Const) Union(&U,&FOR2->Used);

  for (i=0;i<2;i++)
  {
    /* only need second test if BothUseR */
    if (i==1 && (!BothUseR)) continue;

    initializeProblem(&P);
    P._nVars = Card(&U);
    P._safeVars =  P._nVars;
    if (P._nVars>=maxVars) Fail(__LINE__,"Variable exhaustion");
    
    iR = GetSymbolIndex(&U,R);
    iRsub = GetNewPrimedRepsBelowRep(iR,&U,&P);
    if (FOR1!=NULL) iF1 = GetNewSymbolIndex(&P);
    if (FOR2!=NULL) iF2 = GetNewSymbolIndex(&P);
    
    Prob_NewEquality(&P);
    /*{{{  LE1(R->Rsub) + F1 = LE2 + F2*/
    {
      int *C = EQ(&P);
      Prob_AddLinExpr(C,&U,LE1);
      PrimeAllRepsBelowRepInLinExpr(iR,iRsub,C,&U); /* i -> i' forall replicators below R */
      if (FOR1!=NULL) C[iF1] = 1;
      Prob_SubLinExpr(C,&U,LE2);
      if (FOR2!=NULL) C[iF2] = -1;
    }    
    /*}}}*/
    
    if (BothUseR)
      /*{{{  then test for R'<R and R>R'*/
      {
        Prob_NewInequality(&P);
        if (i==0)
          /*{{{  R>Rsub*/
          {
            int *C = GEQ(&P);
            C[0]=-1;
            C[iR]=1;
            C[iRsub]=-1;
          }
          /*}}}*/
        else
          /*{{{  R<Rsub */
          {
            int *C = GEQ(&P);
            C[0]=-1;
            C[iR]=-1;
            C[iRsub]=1;
          }
          /*}}}*/
      }
      /*}}}*/
    
    if (FOR1!=NULL)
      /*{{{  0<=F1<=FOR1*/
      {
        Prob_NewInequality(&P);
        GEQ(&P)[iF1] = 1;
      
        Prob_NewInequality(&P);
        /*{{{  FOR1-F1>=1*/
        {
          int *C = GEQ(&P);
          Prob_AddLinExpr(C,&U,FOR1);
          C[iF1]=-1;
          C[0]--;
        }  
        /*}}}*/
      }
      /*}}}*/
    if (FOR2!=NULL)
      /*{{{  0<=F2<=FOR2*/
      {
        Prob_NewInequality(&P);
        GEQ(&P)[iF2] = 1;
      
        Prob_NewInequality(&P);
        /*{{{  FOR2-F2>=1*/
        {
          int *C = GEQ(&P);
          Prob_AddLinExpr(C,&U,FOR2);
          C[iF2]=-1;
          C[0]--;
        }  
        /*}}}*/
      }
      /*}}}*/
    
    Prob_AddReplicatorBounds(&U,&P);
    Prob_AddPrimedReplicatorBounds(R,iRsub,&U,&P);
      
    if (simplifyProblem(&P)) break;
  }
  ReleaseSet(&U);
  return (i==2);
}
/*}}}*/
/*{{{  bool TestLinExprsDistinctWrtRep(pLinExpr LE1, pLinExpr LE2, tSymbol R)*/
bool TestLinExprsDistinctWrtRep(pLinExpr LE1, pLinExpr LE2, tSymbol R)
{
  return (TestLinExprsDistinctWrtRepOverRange(LE1, LE2, R, NULL,NULL));
}
/*}}}*/
