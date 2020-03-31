# include "Transform.h"
# include "yyTransform.w"
# include "System.h"
# include <stdio.h>
# include <string.h>
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

/* line 26 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */

  #include <stdlib.h>
  #include "TypeChk.h"

  #define writetIdent(a)  { if (a != NoIdent && a != 0) WriteIdent(yyf,a); else fputs("NoIdent",yyf); }
  #define writetSymbol(a) { if (Symbol(a).CId != NoIdent && Symbol(a).CId != 0) WriteIdent(yyf,Symbol(a).CId); else fputs("NoSymbol",yyf); }
  #define NewLine()       { fputc('\n',yyf); }

  /*{{{  static tIdent EnsureUniqueId(tSymbol,char *prefix)*/
  static tIdent EnsureUniqueId(tSymbol S,char *prefix,bool check)
  {
    int i;
    tIdent PId = PrefixId(MakeIdent(prefix,strlen(prefix)),Symbol(S).CId,'_');
    
    if (check)
    {
      for(i = Symbol(S).Scope; i > 0; i = Symbol(i).Scope)
      {
        if (Symbol(i).CId == PId)
        {
          int len = LengthSt(GetStringRef(PId));
          char *str;
        
          if (!(str = (char *)malloc(len + 12))) Message("Out of Heap",xxFatal,NoPosition);
          GetString(PId,str);
          sprintf(&str[len],"_%d",i);
          /*
            printf("Name Clash on symbol %d - ",S);
            printf("renaming to %s\n",str);
          */
          Symbol(i).CId = MakeIdent(str,strlen(str));
          free(str);
          break;
        }
      }
    }
  
    return PId;
  }
  /*}}}*/



static void yyExit () { Exit (1); }

void (* Transform_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module Transform, routine %s failed\n", yyFunction);
 Transform_Exit ();
}

void Transform ARGS((tTree yyP1));
static void TransOuterSpecList ARGS((tTree yyP2));
static void TransOuterSpec ARGS((tTree yyP3));
static void TransProc ARGS((tTree yyP4));
static void TransProcList ARGS((tTree yyP5));
static void TransParProcList ARGS((tTree yyP7, tPosition yyP6));
static tTree TransParProc ARGS((tTree yyP11, tPosition yyP10, tTree yyP9, tSymbol yyP8));
static void TransPlacedList ARGS((tTree yyP13, tPosition yyP12));
static void TransPlaced ARGS((tTree yyP17, tPosition yyP16, tTree yyP15, tSymbol yyP14));
static tTree TransCall ARGS((tSymbol yyP19, tPosition yyP18));
static tTree TransExprList ARGS((tTree yyP21, bool yyP20));
static tTree TransParamList ARGS((tTree yyP23, tTree yyP22));
static tTree TransParamExpr ARGS((tTree * E, bool yyP24));
static tTree TransExpr ARGS((tTree * E, bool yyP26, bool yyP25));
static tTree TransCaseList ARGS((tTree yyP27));
static tTree TransElemental ARGS((tTree yyP28));
static tTree TransElement ARGS((tTree yyP29));
static tTree TransTableElement ARGS((tTree yyP31, bool yyP30, tTree * yyP32));
static tTree TransStringElement ARGS((tTree yyP33, tTree * yyP34));
static tTree RecurseTableElement ARGS((tTree yyP35));
static tTree RecurseStringElement ARGS((tTree yyP36));
static tTree TransRepl ARGS((tTree yyP37));
static void TransAltList ARGS((tTree yyP38));
static tTree TransGuard ARGS((tTree yyP39));
static void TransChoiceList ARGS((tTree yyP40));
static tTree TransSpecList ARGS((tTree yyP41));
static tTree TransSpec ARGS((tTree yyP42));
static tTree TransDefIs2Func ARGS((tTree yyP43));
static tTree Par2Instance ARGS((tTree yyP47, tPosition yyP46, tTree yyP45, tSymbol yyP44));
static tTree CatSpecList ARGS((tTree yyP49, tTree yyP48));
static tTree AppendSpec ARGS((tTree yyP51, tTree yyP50));
static tTree ForceTypeList ARGS((tTree yyP52));
static int CountTypeList ARGS((tTree yyP53));
static tTree SimplifyTableElement ARGS((tTree E));
static tTree SimplifyStringElement ARGS((tTree E));
static tTree EvalExprListSubscript ARGS((tTree yyP55, int yyP54));
static tTree EvalExprListConstantSlice ARGS((tTree yyP58, int yyP57, int yyP56));
static tTree EvalExprListVariableSlice ARGS((tTree yyP60, int yyP59));
static tTree EvalStringSubscript ARGS((tTree yyP62, int yyP61));
static tTree EvalStringConstantSlice ARGS((tTree yyP65, int yyP64, int yyP63));
static tTree EvalStringVariableSlice ARGS((tTree yyP67, int yyP66));
static tTree ReduceOneDimArraySize ARGS((tTree yyP69, int yyP68));
static tTree EnsureTableElement ARGS((tTree yyP70));
static tTree EnsureStringElement ARGS((tTree yyP71));
static void AddCGNext ARGS((tTree T));
static void SetCGNext ARGS((tTree T, tTree N));

void Transform
# if defined __STDC__ | defined __cplusplus
(register tTree yyP1)
# else
(yyP1)
 register tTree yyP1;
# endif
{
  if (yyP1->Kind == kStart) {
  if (yyP1->Start.Process->Kind == kNullProcess) {
/* line 87 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 87 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP1);
/* line 87 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransOuterSpecList (yyP1->Start.Process->NullProcess.SpecList);
  }
   return;

  }
  }
;
}

static void TransOuterSpecList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP2)
# else
(yyP2)
 register tTree yyP2;
# endif
{
  if (yyP2->Kind == kASpec) {
/* line 93 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 94 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP2->ASpec.Spec = TransDefIs2Func (yyP2->ASpec.Spec);
/* line 95 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransOuterSpec (yyP2->ASpec.Spec);
/* line 96 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransOuterSpecList (yyP2->ASpec.Next);
  }
   return;

  }
  if (yyP2->Kind == kNoSpec) {
/* line 99 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }
;
}

static void TransOuterSpec
# if defined __STDC__ | defined __cplusplus
(register tTree yyP3)
# else
(yyP3)
 register tTree yyP3;
# endif
{

  switch (yyP3->Kind) {
  case kDefProc:
  case kInlineDefProc:
/* line 113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 115 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ResetUniqueIds ();
/* line 116 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP3 -> DefProc . isPar = false;
/* line 117 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TreeRoot -> Start . OuterProc = yyP3->DefProc.Sym;
/* line 118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP3->DefProc.Process);
/* line 119 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->DefProc.Sym) . CId = EnsureUniqueId (yyP3->DefProc.Sym, "P", true);
  }
   return;

  case kPrototypeProc:
/* line 124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP3->PrototypeProc.Lang == xxOccam || yyP3->PrototypeProc.Lang == xxAtomicOccam))) goto yyL2;
  {
/* line 125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->PrototypeProc.Sym) . CId = EnsureUniqueId (yyP3->PrototypeProc.Sym, "P", false);
  }
  }
   return;
yyL2:;

/* line 128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 129 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->PrototypeProc.Sym) . CId = Symbol (yyP3->PrototypeProc.Sym) . Id;
  }
   return;

  case kDefFunc:
  case kInlineDefFunc:
/* line 133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ResetUniqueIds ();
/* line 136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TreeRoot -> Start . OuterProc = yyP3->DefFunc.Sym;
/* line 137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP3->DefFunc.Valof->Valof.SpecList = TransSpecList (yyP3->DefFunc.Valof->Valof.SpecList);
/* line 138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP3->DefFunc.Valof->Valof.Process);
/* line 139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP3->DefFunc.Valof->Valof.SpecList = CatSpecList (yyP3->DefFunc.Valof->Valof.SpecList, TransExprList (yyP3->DefFunc.Valof->Valof.ExprList, false));
/* line 140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 141 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->DefFunc.Sym) . CId = EnsureUniqueId (yyP3->DefFunc.Sym, "F", true);
  }
   return;

  case kPrototypeFunc:
/* line 145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP3->PrototypeFunc.Lang == xxOccam || yyP3->PrototypeFunc.Lang == xxAtomicOccam))) goto yyL5;
  {
/* line 146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->PrototypeFunc.Sym) . CId = EnsureUniqueId (yyP3->PrototypeFunc.Sym, "F", false);
  }
  }
   return;
yyL5:;

/* line 150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
/* line 152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->PrototypeFunc.Sym) . CId = Symbol (yyP3->PrototypeFunc.Sym) . Id;
  }
   return;

  case kHCode:
/* line 155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP3);
  }
   return;

  case kNullSpec:
/* line 159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  case kAVal:
  case kVal:
  case kValRetypes:
  case kValReshapes:
/* line 160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP3->AVal.Sym) . CId = EnsureUniqueId (yyP3->AVal.Sym, "S", true);
  }
   return;

  case kDefSeqPro:
/* line 161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  case kDefTagPro:
/* line 162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  case kDeclType:
/* line 163 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }

/* line 165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyWrite ("Fallen through TransOuterSpec with kind ");
/* line 165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   printf ("%d", yyP3 -> Kind);
/* line 165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyWrite ("\n");
  }
   return;

;
}

static void TransProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP4)
# else
(yyP4)
 register tTree yyP4;
# endif
{

  switch (yyP4->Kind) {
  case kAssign:
/* line 171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Assign.SpecList = TransSpecList (yyP4->Assign.SpecList);
/* line 171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Assign.SpecList = CatSpecList (yyP4->Assign.SpecList, TransExprList (yyP4->Assign.Lval, false));
/* line 171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Assign.SpecList = CatSpecList (yyP4->Assign.SpecList, TransExprList (yyP4->Assign.Rval, false));
  }
   return;

  case kAlt:
/* line 173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Alt.SpecList = TransSpecList (yyP4->Alt.SpecList);
/* line 173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransAltList (yyP4->Alt.AltList);
  }
   return;

  case kAltRep:
/* line 174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->AltRep.SpecList = TransSpecList (yyP4->AltRep.SpecList);
/* line 174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->AltRep.SpecList = CatSpecList (yyP4->AltRep.SpecList, TransRepl (yyP4->AltRep.Replicator));
/* line 174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransAltList (yyP4->AltRep.AltList);
  }
   return;

  case kInput:
/* line 176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Input.SpecList = TransSpecList (yyP4->Input.SpecList);
/* line 176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Input.SpecList = CatSpecList (yyP4->Input.SpecList, TransElement (yyP4->Input.Element));
/* line 176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Input.SpecList = CatSpecList (yyP4->Input.SpecList, TransExprList (yyP4->Input.ExprList, false));
  }
   return;

  case kOutput:
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Output.SpecList = TransSpecList (yyP4->Output.SpecList);
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Output.SpecList = CatSpecList (yyP4->Output.SpecList, TransElement (yyP4->Output.Element));
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Output.SpecList = CatSpecList (yyP4->Output.SpecList, TransExprList (yyP4->Output.ExprList, false));
  }
   return;

  case kCaseIn:
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->CaseIn.SpecList = TransSpecList (yyP4->CaseIn.SpecList);
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->CaseIn.SpecList = CatSpecList (yyP4->CaseIn.SpecList, TransElement (yyP4->CaseIn.Element));
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->CaseIn.SpecList = CatSpecList (yyP4->CaseIn.SpecList, TransCaseList (yyP4->CaseIn.CaseList));
  }
   return;

  case kAfter:
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->After.SpecList = TransSpecList (yyP4->After.SpecList);
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->After.SpecList = CatSpecList (yyP4->After.SpecList, TransElement (yyP4->After.Element));
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->After.SpecList = CatSpecList (yyP4->After.SpecList, TransExpr (& yyP4->After.Expr, false, false));
  }
   return;

  case kSeq:
/* line 181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Seq.SpecList = TransSpecList (yyP4->Seq.SpecList);
/* line 181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProcList (yyP4->Seq.ProcList);
  }
   return;

  case kSeqRep:
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->SeqRep.SpecList = TransSpecList (yyP4->SeqRep.SpecList);
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->SeqRep.SpecList = CatSpecList (yyP4->SeqRep.SpecList, TransRepl (yyP4->SeqRep.Replicator));
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP4->SeqRep.Process);
  }
   return;

  case kPar:
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP4->Par.PType == xxPlacedPar))) goto yyL10;
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Par.SpecList = TransSpecList (yyP4->Par.SpecList);
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlacedList (yyP4->Par.ProcList, yyP4->Par.Pos);
  }
  }
   return;
yyL10:;

/* line 187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Par.SpecList = TransSpecList (yyP4->Par.SpecList);
/* line 187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransParProcList (yyP4->Par.ProcList, yyP4->Par.Pos);
  }
   return;

  case kParRep:
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP4->ParRep.PType == xxPlacedPar))) goto yyL11;
  {
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->ParRep.SpecList = TransSpecList (yyP4->ParRep.SpecList);
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlaced (yyP4->ParRep.Process, yyP4->ParRep.Pos, yyP4->ParRep.Replicator, yyP4 -> ParRep . Sym);
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->ParRep.SpecList = CatSpecList (yyP4->ParRep.SpecList, TransRepl (yyP4->ParRep.Replicator));
  }
  }
   return;
yyL11:;

/* line 188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->ParRep.SpecList = TransSpecList (yyP4->ParRep.SpecList);
/* line 188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->ParRep.Process = TransParProc (yyP4->ParRep.Process, yyP4->ParRep.Pos, yyP4->ParRep.Replicator, yyP4 -> ParRep . Sym);
/* line 188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->ParRep.SpecList = CatSpecList (yyP4->ParRep.SpecList, TransRepl (yyP4->ParRep.Replicator));
  }
   return;

  case kIf:
/* line 190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->If.SpecList = TransSpecList (yyP4->If.SpecList);
/* line 190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransChoiceList (yyP4->If.ChoiceList);
  }
   return;

  case kIfRep:
/* line 191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->IfRep.SpecList = TransSpecList (yyP4->IfRep.SpecList);
/* line 191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->IfRep.SpecList = CatSpecList (yyP4->IfRep.SpecList, TransRepl (yyP4->IfRep.Replicator));
/* line 191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransChoiceList (yyP4->IfRep.ChoiceList);
  }
   return;

  case kCase:
/* line 193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Case.SpecList = TransSpecList (yyP4->Case.SpecList);
/* line 193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Case.SpecList = CatSpecList (yyP4->Case.SpecList, TransExpr (& yyP4->Case.Selector, false, false));
/* line 193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransChoiceList (yyP4->Case.ChoiceList);
  }
   return;

  case kWhile:
/* line 194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->While.SpecList = TransSpecList (yyP4->While.SpecList);
/* line 194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->While.SpecList = CatSpecList (yyP4->While.SpecList, TransExpr (& yyP4->While.Expr, false, false));
/* line 194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP4->While.Process);
  }
   return;

  case kSkip:
/* line 196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Skip.SpecList = TransSpecList (yyP4->Skip.SpecList);
  }
   return;

  case kStop:
/* line 197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Stop.SpecList = TransSpecList (yyP4->Stop.SpecList);
  }
   return;

  case kCCode:
/* line 199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->CCode.SpecList = TransSpecList (yyP4->CCode.SpecList);
  }
   return;

  case kInstance:
/* line 201 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 202 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Instance.SpecList = TransSpecList (yyP4->Instance.SpecList);
/* line 203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Instance.SpecList = CatSpecList (yyP4->Instance.SpecList, TransParamList (yyP4->Instance.ExprList, SymbolKind (yyP4->Instance.Sym) -> DefAProc . Formals));
/* line 204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4 -> Instance . Frame = TransCall (yyP4->Instance.Sym, yyP4->Instance.Pos);
/* line 205 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->Instance.SpecList = AppendSpec (yyP4->Instance.SpecList, yyP4 -> Instance . Frame);
  }
   return;

  case kNullProcess:
/* line 207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP4->NullProcess.SpecList = TransSpecList (yyP4->NullProcess.SpecList);
  }
   return;

  }

;
}

static void TransProcList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP5)
# else
(yyP5)
 register tTree yyP5;
# endif
{
  if (yyP5->Kind == kAProcess) {
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP5->AProcess.Process);
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProcList (yyP5->AProcess.Next);
  }
   return;

  }
  if (yyP5->Kind == kNoProcess) {
/* line 213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }
;
}

static void TransParProcList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP7, register tPosition yyP6)
# else
(yyP7, yyP6)
 register tTree yyP7;
 register tPosition yyP6;
# endif
{
  if (yyP7->Kind == kAProcess) {
/* line 218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP7->AProcess.Process = TransParProc (yyP7->AProcess.Process, yyP6, NoTree, yyP7 -> AProcess . Sym);
/* line 218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransParProcList (yyP7->AProcess.Next, yyP6);
  }
   return;

  }
  if (yyP7->Kind == kNoProcess) {
/* line 219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }
;
}

static tTree TransParProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP11, register tPosition yyP10, register tTree yyP9, tSymbol yyP8)
# else
(yyP11, yyP10, yyP9, yyP8)
 register tTree yyP11;
 register tPosition yyP10;
 register tTree yyP9;
 tSymbol yyP8;
# endif
{
/* line 225 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree NewP,SL; tFrame Frame;
  if (yyP11->Kind == kProcessor) {
/* line 227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP = TransParProc (yyP11->Processor.Process, yyP10, yyP9, yyP8);
  }
   return NewP;

  }
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP9 == NoTree))) goto yyL2;
  {
/* line 232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP = Par2Instance (yyP11, yyP10, yyP9, yyP8);
/* line 233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . Scope = yyP8;
/* line 234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = NewP -> Instance . SpecList;
/* line 235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExprList (NewP -> Instance . ExprList, false));
/* line 236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Frame = mParFrame (yyP10, UniqueId ('T'), UniqueId ('C'), yyP8, - 1);
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = AppendSpec (SL, Frame);
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . SpecList = SL;
/* line 239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . Frame = Frame;
  }
  }
   return NewP;
yyL2:;

/* line 241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((ValConstInt (yyP9 -> Replicator . For) <= 0))) goto yyL3;
  {
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP = mSkip (yyP10, mNoSpec ());
  }
  }
   return NewP;
yyL3:;

/* line 244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP = Par2Instance (yyP11, yyP10, yyP9, yyP8);
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . Scope = yyP8;
/* line 247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = NewP -> Instance . SpecList;
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExprList (NewP -> Instance . ExprList, false));
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Frame = mParFrame (yyP10, UniqueId ('T'), UniqueId ('C'), yyP8, ValConstInt (yyP9 -> Replicator . For));
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = AppendSpec (SL, Frame);
/* line 251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . SpecList = SL;
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   NewP -> Instance . Frame = Frame;
  }
   return NewP;

}

static void TransPlacedList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP13, register tPosition yyP12)
# else
(yyP13, yyP12)
 register tTree yyP13;
 register tPosition yyP12;
# endif
{
  if (yyP13->Kind == kAProcess) {
/* line 259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlaced (yyP13->AProcess.Process, yyP12, NoTree, yyP13 -> AProcess . Sym);
/* line 259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlacedList (yyP13->AProcess.Next, yyP12);
  }
   return;

  }
  if (yyP13->Kind == kNoProcess) {
/* line 260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }
;
}

static void TransPlaced
# if defined __STDC__ | defined __cplusplus
(register tTree yyP17, register tPosition yyP16, register tTree yyP15, tSymbol yyP14)
# else
(yyP17, yyP16, yyP15, yyP14)
 register tTree yyP17;
 register tPosition yyP16;
 register tTree yyP15;
 tSymbol yyP14;
# endif
{
  if (yyP17->Kind == kProcessor) {
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP17->Processor.Process = TransParProc (yyP17->Processor.Process, yyP16, yyP15, yyP14);
  }
   return;

  }
  if (yyP17->Kind == kPar) {
/* line 270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlacedList (yyP17->Par.ProcList, yyP16);
  }
   return;

  }
  if (yyP17->Kind == kParRep) {
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransPlaced (yyP17->ParRep.Process, yyP16, yyP17->ParRep.Replicator, yyP17 -> ParRep . Sym);
  }
   return;

  }
;
}

static tTree TransCall
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP19, register tPosition yyP18)
# else
(yyP19, yyP18)
 tSymbol yyP19;
 register tPosition yyP18;
# endif
{
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kCallFrame)
    yyV1->CallFrame.Pos = yyP18;
    yyV1->CallFrame.Id = UniqueId ('C');
    yyV1->CallFrame.Sym = yyP19;
   return yyV1;
  }
 }

}

static tTree TransExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP21, register bool yyP20)
# else
(yyP21, yyP20)
 register tTree yyP21;
 register bool yyP20;
# endif
{
  if (yyP21->Kind == kAnExpr) {
/* line 288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransExpr (& yyP21->AnExpr.Expr, false, yyP20), TransExprList (yyP21->AnExpr.Next, yyP20));

  }
  if (yyP21->Kind == kNoExpr) {
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }
 yyAbort ("TransExprList");
}

static tTree TransParamList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP23, register tTree yyP22)
# else
(yyP23, yyP22)
 register tTree yyP23;
 register tTree yyP22;
# endif
{
  if (yyP23->Kind == kAnExpr) {
  if (yyP22->Kind == kAFormal) {
  if (yyP22->AFormal.By->Kind == kByRef) {
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransParamExpr (& yyP23->AnExpr.Expr, false), TransParamList (yyP23->AnExpr.Next, yyP22->AFormal.Next));

  }
  if (yyP22->AFormal.By->Kind == kByVal) {
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransParamExpr (& yyP23->AnExpr.Expr, true), TransParamList (yyP23->AnExpr.Next, yyP22->AFormal.Next));

  }
  }
  }
  if (yyP23->Kind == kNoExpr) {
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }
 yyAbort ("TransParamList");
}

static tTree TransParamExpr
# if defined __STDC__ | defined __cplusplus
(register tTree * E, register bool yyP24)
# else
(E, yyP24)
 register tTree * E;
 register bool yyP24;
# endif
{
/* line 300 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL; 
  if ((* E)->Kind == kElementalExpr) {
  if (Tree_IsType ((* E)->ElementalExpr.Elemental, kTableElement)) {
 {
  tTree yyV1;
  if (equalbool (yyP24, false)) {
/* line 302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement ((* E)->ElementalExpr.Elemental, true, & yyV1);
/* line 304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) -> ElementalExpr . Elemental = yyV1;
  }
  {
   return SL;
  }

  }
 }
  }
  }
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& (* E), false, false);

}

static tTree TransExpr
# if defined __STDC__ | defined __cplusplus
(register tTree * E, register bool yyP26, register bool yyP25)
# else
(E, yyP26, yyP25)
 register tTree * E;
 register bool yyP26;
 register bool yyP25;
# endif
{
/* line 322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL; tTree TMP1; 
  if ((* E)->Kind == kElementalExpr) {
  if (Tree_IsType ((* E)->ElementalExpr.Elemental, kTableElement)) {
/* line 324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP1 = SimplifyTableElement ((* E)->ElementalExpr.Elemental);
/* line 327 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! (((* E)->ElementalExpr.Elemental != TMP1))) goto yyL1;
  {
/* line 328 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */

      if (Tree_IsType(TMP1,kTableElement))
        (* E)->ElementalExpr.Elemental = TMP1;
      else
        (* E) = TMP1;
    
/* line 334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& (* E), yyP26, yyP25);
  }
  }
   return SL;
yyL1:;

  }
  }
  if (Tree_IsType ((* E), kStringElement)) {
/* line 337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP1 = SimplifyStringElement ((* E));
/* line 340 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! (((* E) != TMP1))) goto yyL2;
  {
/* line 341 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) = TMP1;
/* line 342 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& (* E), yyP26, yyP25);
/* line 343 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((false))) goto yyL2;
  }
  }
   return SL;
yyL2:;

  }
  if ((* E)->Kind == kBinary) {
/* line 347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (TransExpr (& (* E)->Binary.Lop, false, false), TransExpr (& (* E)->Binary.Rop, false, false));
  }
   return SL;

  }
  if ((* E)->Kind == kUnary) {
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& (* E)->Unary.Expr, false, false);
  }
   return SL;

  }
  if ((* E)->Kind == kValof) {
/* line 355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((! (* E) -> Valof . C))) goto yyL5;
  {
/* line 355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! (((* E) -> Valof . FuncCall != NoTree))) goto yyL5;
  {
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) = (* E) -> Valof . FuncCall;
/* line 358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& (* E), yyP26, yyP25);
  }
  }
  }
   return SL;
yyL5:;

  }

  switch ((* E)->Kind) {
  case kValof:
/* line 361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  register tTree yyV2;
  {
/* line 361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((! (* E) -> Valof . C || ! IsScalarType ((* E) -> Valof . SynType)))) goto yyL6;
  {
/* line 363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoFormal)
   TMP1 = mInlineDefFunc ((* E)->Valof.Pos, (* E) -> Valof . Sym, ForceTypeList ((* E) -> Valof . SynType), yyV1, (* E));
/* line 364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (TMP1 -> DefFunc . Sym) = TMP1;
/* line 365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolType (TMP1 -> DefFunc . Sym) = (* E) -> Valof . SynType;
/* line 366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kASpec)
    yyV2->ASpec.Spec = TMP1;
    yyV2->ASpec.Next = TransSpec (TMP1);
   SL = yyV2;
/* line 367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP1 -> DefFunc . PUOut = (* E) -> Valof . PUOut;
/* line 369 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) = mFuncCall ((* E)->Valof.Pos, (* E) -> Valof . Sym, mNoExpr (), (* E) -> Valof . VS);
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) -> FuncCall . SynType = TMP1 -> DefFunc . TypeList;
/* line 371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExpr (& (* E), yyP26, yyP25));
  }
  }
  {
   return SL;
  }
 }
yyL6:;

  break;
  case kFuncCall:
/* line 374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransParamList ((* E)->FuncCall.ExprList, SymbolKind ((* E)->FuncCall.Sym) -> DefAFunc . Formals);

  case kConvert:
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& (* E)->Convert.Expr, false, false);

  case kRound:
/* line 378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& (* E)->Round.Expr, false, false);

  case kTrunc:
/* line 380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& (* E)->Trunc.Expr, false, false);

  case kCountArray:
/* line 382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransExpr (& (* E)->CountArray.Length, false, false), TransExpr (& (* E)->CountArray.Expr, false, false));

  case kElementalExpr:
  if (Tree_IsType ((* E)->ElementalExpr.Elemental, kElement)) {
/* line 384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransElement ((* E)->ElementalExpr.Elemental);

  }
  if (Tree_IsType ((* E)->ElementalExpr.Elemental, kTable)) {
 {
  tTree yyV1;
  if (equalbool (yyP26, false)) {
  if (equalbool (yyP25, false)) {
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement ((* E)->ElementalExpr.Elemental, false, & yyV1);
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) -> ElementalExpr . Elemental = yyV1;
  }
  {
   return SL;
  }

  }
  }
 }
/* line 390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 391 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseTableElement ((* E)->ElementalExpr.Elemental);
  }
   return SL;

  }
  if (Tree_IsType ((* E)->ElementalExpr.Elemental, kTableElement)) {
/* line 393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  {
/* line 394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement ((* E)->ElementalExpr.Elemental, false, & yyV1);
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   (* E) -> ElementalExpr . Elemental = yyV1;
  }
  {
   return SL;
  }
 }

  }
  break;
  case kStringElement:
  case kString:
  case kStringSubscript:
  case kStringRange:
 {
  tTree yyV1;
  register tTree yyV2;
  if (equalbool (yyP26, false)) {
/* line 397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 398 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransStringElement ((* E), & yyV1);
/* line 399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kElementalExpr)
    yyV2->ElementalExpr.Pos = (* E)->StringElement.Pos;
    yyV2->ElementalExpr.Elemental = yyV1;
   (* E) = yyV2;
/* line 400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag ((* E) -> Expr) = false;
/* line 401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType ((* E)) = ElementType (yyV1);
  }
  {
   return SL;
  }

  }
 }
/* line 403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseStringElement ((* E));
  }
   return SL;

  }

/* line 406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

}

static tTree TransCaseList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP27)
# else
(yyP27)
 register tTree yyP27;
# endif
{
  if (yyP27->Kind == kACase) {
/* line 411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP27->ACase.SpecList = TransSpecList (yyP27->ACase.SpecList);
/* line 413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP27->ACase.SpecList = CatSpecList (yyP27->ACase.SpecList, TransExprList (yyP27->ACase.ExprList, false));
/* line 414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP27->ACase.Process);
  }
   return TransCaseList (yyP27->ACase.Next);

  }
  if (yyP27->Kind == kNoCase) {
/* line 416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }
 yyAbort ("TransCaseList");
}

static tTree TransElemental
# if defined __STDC__ | defined __cplusplus
(register tTree yyP28)
# else
(yyP28)
 register tTree yyP28;
# endif
{
  if (Tree_IsType (yyP28, kElement)) {
/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransElement (yyP28);

  }
  if (Tree_IsType (yyP28, kTableElement)) {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return RecurseTableElement (yyP28);

  }
 yyAbort ("TransElemental");
}

static tTree TransElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP29)
# else
(yyP29)
 register tTree yyP29;
# endif
{
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL; 
  if (Tree_IsType (yyP29, kIdentifier)) {
/* line 433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }
  if (yyP29->Kind == kSubscript) {
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransElement (yyP29->Subscript.Next), TransExpr (& yyP29->Subscript.Expr, false, false));

  }
  if (yyP29->Kind == kRange) {
  if (yyP29->Range.For->Kind == kFuncCall) {
/* line 437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (CatSpecList (TransElement (yyP29->Range.Next), TransExpr (& yyP29->Range.From, false, false)), TransExpr (& yyP29->Range.For, false, false));
/* line 439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP29 -> Range . SynType -> Array . Expr = yyP29->Range.For;
  }
   return SL;

  }
/* line 442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (CatSpecList (TransElement (yyP29->Range.Next), TransExpr (& yyP29->Range.From, false, false)), TransExpr (& yyP29->Range.For, false, false));

  }
 yyAbort ("TransElement");
}

static tTree TransTableElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP31, register bool yyP30, register tTree * yyP32)
# else
(yyP31, yyP30, yyP32)
 register tTree yyP31;
 register bool yyP30;
 register tTree * yyP32;
# endif
{
/* line 455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree TMP1, TMP2, SL;
  if (yyP31->Kind == kTableExpr) {
/* line 457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP31->TableExpr.Expr, false, false);
/* line 459 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP1 = mVal (yyP31->TableExpr.Pos, ElementType (yyP31), yyP31->TableExpr.Sym, yyP31->TableExpr.Expr);
/* line 460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP31->TableExpr.Sym) . CId = UniqueId ('V');
/* line 461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP31->TableExpr.Sym) . Kind = TMP1;
/* line 462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP31->TableExpr.Sym) . Type = ElementType (yyP31);
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = AppendSpec (SL, TMP1);
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP2 = mIdentifier (yyP31->TableExpr.Pos, yyP31->TableExpr.Sym, yyP31->TableExpr.Sym);
/* line 465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = ElementType (yyP31);
/* line 466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = ConstFlag (yyP31 -> TableElement);
/* line 467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementalConst (TMP2) = ElementalConst (yyP31);
  }
   * yyP32 = TMP2;
   return SL;

  }
  if (Tree_IsType (yyP31, kTable)) {
/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  {
/* line 471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree T; tSymbol Sym = yyP31->Table.Sym; 
/* line 472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExprList (yyP31->Table.ExprList, ConstFlag (yyP31 -> TableElement));
/* line 475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kElementalExpr)
    yyV1->ElementalExpr.Pos = yyP31->Table.Pos;
    yyV1->ElementalExpr.Elemental = yyP31;
   TMP1 = yyV1;
/* line 476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (TMP1) = ElementType (yyP31);
/* line 477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP1 -> Expr) = ConstFlag (yyP31 -> TableElement);
/* line 478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementalConst (TMP1) = ElementalConst (yyP31);
/* line 480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 if (yyP30) T = mIs(yyP31->Table.Pos, ElementType(yyP31), Sym, yyP31);
      else T = mVal(yyP31->Table.Pos, ElementType(yyP31), Sym, TMP1); 
/* line 483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (Sym) . CId = UniqueId ('V');
/* line 484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (Sym) . Kind = T;
/* line 485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (Sym) . Type = T -> Val . Type;
/* line 486 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = AppendSpec (SL, T);
/* line 488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP2 = mIdentifier (yyP31->Table.Pos, Sym, Sym);
/* line 489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = ElementType (yyP31);
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = ConstFlag (yyP31 -> TableElement);
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementalConst (TMP2) = ElementalConst (yyP31);
  }
   * yyP32 = TMP2;
  {
   return SL;
  }
 }

  }
  if (yyP31->Kind == kTableSubscript) {
/* line 494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement (yyP31->TableSubscript.Next, yyP30, & yyV1);
/* line 496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kSubscript)
    yyV2->Subscript.Pos = yyP31->TableSubscript.Pos;
    yyV2->Subscript.Next = yyV1;
    yyV2->Subscript.Expr = yyP31->TableSubscript.Expr;
   TMP2 = yyV2;
/* line 497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeSubscript (yyP31->TableSubscript.Expr, ElementType (yyV1), ExprFieldList (yyP31->TableSubscript.Expr), yyP31->TableSubscript.Pos);
/* line 498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
/* line 499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExpr (& yyP31->TableSubscript.Expr, false, false));
  }
   * yyP32 = TMP2;
  {
   return SL;
  }
 }

  }
  if (yyP31->Kind == kTableRange) {
  if (yyP31->TableRange.For->Kind == kFuncCall) {
/* line 502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement (yyP31->TableRange.Next, yyP30, & yyV1);
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kRange)
    yyV2->Range.Pos = yyP31->TableRange.Pos;
    yyV2->Range.Next = yyV1;
    yyV2->Range.From = yyP31->TableRange.From;
    yyV2->Range.For = yyP31->TableRange.For;
   TMP2 = yyV2;
/* line 505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeRange (yyP31->TableRange.From, yyP31->TableRange.For, ElementType (yyV1), yyP31->TableRange.Pos);
/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP31 -> TableRange . SynType -> Array . Expr = yyP31->TableRange.For;
/* line 507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP31->TableRange.From, false, false), TransExpr (& yyP31->TableRange.For, false, false)));
  }
   * yyP32 = TMP2;
  {
   return SL;
  }
 }

  }
/* line 511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement (yyP31->TableRange.Next, yyP30, & yyV1);
/* line 513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kRange)
    yyV2->Range.Pos = yyP31->TableRange.Pos;
    yyV2->Range.Next = yyV1;
    yyV2->Range.From = yyP31->TableRange.From;
    yyV2->Range.For = yyP31->TableRange.For;
   TMP2 = yyV2;
/* line 514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeRange (yyP31->TableRange.From, yyP31->TableRange.For, ElementType (yyV1), yyP31->TableRange.Pos);
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP31->TableRange.From, false, false), TransExpr (& yyP31->TableRange.For, false, false)));
  }
   * yyP32 = TMP2;
  {
   return SL;
  }
 }

  }
 yyAbort ("TransTableElement");
}

static tTree TransStringElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP33, register tTree * yyP34)
# else
(yyP33, yyP34)
 register tTree yyP33;
 register tTree * yyP34;
# endif
{
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree TMP1; tTree TMP2; tTree SL; 
  if (yyP33->Kind == kString) {
/* line 529 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  register tTree yyV2;
  {
/* line 530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree T; tSymbol Sym = yyP33->String.Sym; 
/* line 531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kVal)
    yyV1->Val.Pos = yyP33->String.Pos;
    yyV1->Val.Type = ExprType (yyP33);
    yyV1->Val.Sym = Sym;
    yyV1->Val.Expr = yyP33;
   T = yyV1;
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (Sym) . CId = UniqueId ('V');
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (Sym) = T;
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolType (Sym) = T -> Val . Type;
/* line 535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kASpec)
    yyV2->ASpec.Spec = T;
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kNoSpec)
    yyV2->ASpec.Next = yyW1;
   }
   SL = yyV2;
/* line 536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP1 = mIdentifier (yyP33->String.Pos, Sym, Sym);
/* line 537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP1) = ExprType (yyP33);
/* line 538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP1 -> Element) = IsConstExpr (yyP33);
/* line 539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementalConst (TMP1) = StringConst (yyP33);
  }
   * yyP34 = TMP1;
  {
   return SL;
  }
 }

  }
  if (yyP33->Kind == kStringSubscript) {
/* line 542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransStringElement (yyP33->StringSubscript.Next, & yyV1);
/* line 545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kSubscript)
    yyV2->Subscript.Pos = yyP33->StringSubscript.Pos;
    yyV2->Subscript.Next = yyV1;
    yyV2->Subscript.Expr = yyP33->StringSubscript.Expr;
   TMP2 = yyV2;
/* line 546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeSubscript (yyP33->StringSubscript.Expr, ElementType (yyV1), NoTree, yyP33->StringSubscript.Pos);
/* line 547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExpr (& yyP33->StringSubscript.Expr, false, false));
/* line 548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
  }
   * yyP34 = TMP2;
  {
   return SL;
  }
 }

  }
  if (yyP33->Kind == kStringRange) {
  if (yyP33->StringRange.For->Kind == kFuncCall) {
/* line 551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransStringElement (yyP33->StringRange.Next, & yyV1);
/* line 554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kRange)
    yyV2->Range.Pos = yyP33->StringRange.Pos;
    yyV2->Range.Next = yyV1;
    yyV2->Range.From = yyP33->StringRange.From;
    yyV2->Range.For = yyP33->StringRange.For;
   TMP2 = yyV2;
/* line 555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeRange (yyP33->StringRange.From, yyP33->StringRange.For, ElementType (yyV1), yyP33->StringRange.Pos);
/* line 556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP33->StringRange.From, false, false), TransExpr (& yyP33->StringRange.For, false, false)));
/* line 557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP33 -> StringRange . SynType -> Array . Expr = yyP33->StringRange.For;
/* line 558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
  }
   * yyP34 = TMP2;
  {
   return SL;
  }
 }

  }
/* line 561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  register tTree yyV2;
  {
/* line 563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransStringElement (yyP33->StringRange.Next, & yyV1);
/* line 564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kRange)
    yyV2->Range.Pos = yyP33->StringRange.Pos;
    yyV2->Range.Next = yyV1;
    yyV2->Range.From = yyP33->StringRange.From;
    yyV2->Range.For = yyP33->StringRange.For;
   TMP2 = yyV2;
/* line 565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = TypeRange (yyP33->StringRange.From, yyP33->StringRange.For, ElementType (yyV1), yyP33->StringRange.Pos);
/* line 566 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP33->StringRange.From, false, false), TransExpr (& yyP33->StringRange.For, false, false)));
/* line 567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
  }
   * yyP34 = TMP2;
  {
   return SL;
  }
 }

  }
 yyAbort ("TransStringElement");
}

static tTree RecurseTableElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP35)
# else
(yyP35)
 register tTree yyP35;
# endif
{
/* line 579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL;
  if (Tree_IsType (yyP35, kTable)) {
/* line 581 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExprList (yyP35->Table.ExprList, IsConstExprList (yyP35->Table.ExprList));
  }
   return SL;

  }
  if (yyP35->Kind == kTableSubscript) {
/* line 585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseTableElement (yyP35->TableSubscript.Next);
/* line 587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExpr (& yyP35->TableSubscript.Expr, false, false));
  }
   return SL;

  }
  if (yyP35->Kind == kTableRange) {
  if (yyP35->TableRange.For->Kind == kFuncCall) {
/* line 590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseTableElement (yyP35->TableRange.Next);
/* line 592 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP35->TableRange.From, false, false), TransExpr (& yyP35->TableRange.For, false, false)));
  }
   return SL;

  }
/* line 595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseTableElement (yyP35->TableRange.Next);
/* line 597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP35->TableRange.From, false, false), TransExpr (& yyP35->TableRange.For, false, false)));
  }
   return SL;

  }
 yyAbort ("RecurseTableElement");
}

static tTree RecurseStringElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP36)
# else
(yyP36)
 register tTree yyP36;
# endif
{
/* line 608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL;
  if (yyP36->Kind == kString) {
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }
  if (yyP36->Kind == kStringSubscript) {
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseStringElement (yyP36->StringSubscript.Next);
/* line 614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, TransExpr (& yyP36->StringSubscript.Expr, false, false));
  }
   return SL;

  }
  if (yyP36->Kind == kStringRange) {
  if (yyP36->StringRange.For->Kind == kFuncCall) {
/* line 617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseStringElement (yyP36->StringRange.Next);
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP36->StringRange.From, false, false), TransExpr (& yyP36->StringRange.For, false, false)));
  }
   return SL;

  }
/* line 622 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = RecurseStringElement (yyP36->StringRange.Next);
/* line 624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = CatSpecList (SL, CatSpecList (TransExpr (& yyP36->StringRange.From, false, false), TransExpr (& yyP36->StringRange.For, false, false)));
  }
   return SL;

  }
 yyAbort ("RecurseStringElement");
}

static tTree TransRepl
# if defined __STDC__ | defined __cplusplus
(register tTree yyP37)
# else
(yyP37)
 register tTree yyP37;
# endif
{
/* line 630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CatSpecList (TransExpr (& yyP37->Replicator.From, false, false), TransExpr (& yyP37->Replicator.For, false, false));

}

static void TransAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP38)
# else
(yyP38)
 register tTree yyP38;
# endif
{
  if (yyP38->Kind == kAnAlt) {
/* line 636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP38->AnAlt.SpecList = TransSpecList (yyP38->AnAlt.SpecList);
/* line 638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP38->AnAlt.SpecList = CatSpecList (yyP38->AnAlt.SpecList, TransGuard (yyP38->AnAlt.Guard));
/* line 639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransAltList (yyP38->AnAlt.Next);
  }
   return;

  }
  if (yyP38->Kind == kANewAlt) {
/* line 641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 642 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP38->ANewAlt.SpecList = TransSpecList (yyP38->ANewAlt.SpecList);
/* line 643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransAltList (yyP38->ANewAlt.Next);
/* line 644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP38->ANewAlt.AltType);
  }
   return;

  }
;
}

static tTree TransGuard
# if defined __STDC__ | defined __cplusplus
(register tTree yyP39)
# else
(yyP39)
 register tTree yyP39;
# endif
{
/* line 649 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL; 
  if (yyP39->Kind == kAltInput) {
/* line 651 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP39->AltInput.Expr, false, false);
/* line 653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltInput.Input->Input.SpecList = TransSpecList (yyP39->AltInput.Input->Input.SpecList);
/* line 654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltInput.Input->Input.SpecList = CatSpecList (yyP39->AltInput.Input->Input.SpecList, TransElement (yyP39->AltInput.Input->Input.Element));
/* line 655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltInput.Input->Input.SpecList = CatSpecList (yyP39->AltInput.Input->Input.SpecList, TransExprList (yyP39->AltInput.Input->Input.ExprList, false));
/* line 656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP39->AltInput.Process);
  }
   return SL;

  }
  if (yyP39->Kind == kAltTimer) {
/* line 658 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP39->AltTimer.Expr, false, false);
/* line 660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltTimer.After->After.SpecList = TransSpecList (yyP39->AltTimer.After->After.SpecList);
/* line 661 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltTimer.After->After.SpecList = CatSpecList (yyP39->AltTimer.After->After.SpecList, TransElement (yyP39->AltTimer.After->After.Element));
/* line 662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltTimer.After->After.SpecList = CatSpecList (yyP39->AltTimer.After->After.SpecList, TransExpr (& yyP39->AltTimer.After->After.Expr, false, false));
/* line 663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP39->AltTimer.Process);
  }
   return SL;

  }
  if (yyP39->Kind == kAltCaseIn) {
/* line 665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP39->AltCaseIn.Expr, false, false);
/* line 667 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltCaseIn.CaseIn->CaseIn.SpecList = TransSpecList (yyP39->AltCaseIn.CaseIn->CaseIn.SpecList);
/* line 668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltCaseIn.CaseIn->CaseIn.SpecList = CatSpecList (yyP39->AltCaseIn.CaseIn->CaseIn.SpecList, TransElement (yyP39->AltCaseIn.CaseIn->CaseIn.Element));
/* line 669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP39->AltCaseIn.CaseIn->CaseIn.SpecList = CatSpecList (yyP39->AltCaseIn.CaseIn->CaseIn.SpecList, TransCaseList (yyP39->AltCaseIn.CaseIn->CaseIn.CaseList));
  }
   return SL;

  }
  if (yyP39->Kind == kNoInput) {
/* line 671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP39->NoInput.Expr, false, false);
/* line 673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP39->NoInput.Process);
  }
   return SL;

  }
 yyAbort ("TransGuard");
}

static void TransChoiceList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP40)
# else
(yyP40)
 register tTree yyP40;
# endif
{
  if (yyP40->Kind == kAChoice) {
/* line 679 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP40->AChoice.SpecList = TransSpecList (yyP40->AChoice.SpecList);
/* line 681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransChoiceList (yyP40->AChoice.Next);
/* line 682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP40->AChoice.SpecList = CatSpecList (yyP40->AChoice.SpecList, TransExprList (yyP40->AChoice.ExprList, false));
/* line 683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP40->AChoice.Process);
  }
   return;

  }
  if (yyP40->Kind == kANewIf) {
/* line 685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 686 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP40->ANewIf.SpecList = TransSpecList (yyP40->ANewIf.SpecList);
/* line 687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransChoiceList (yyP40->ANewIf.Next);
/* line 688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP40->ANewIf.IfType);
  }
   return;

  }
  if (yyP40->Kind == kNoChoice) {
/* line 690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return;

  }
;
}

static tTree TransSpecList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP41)
# else
(yyP41)
 register tTree yyP41;
# endif
{
/* line 710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL; 
  if (yyP41->Kind == kASpec) {
/* line 712 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP41->ASpec.Spec = TransDefIs2Func (yyP41->ASpec.Spec);
/* line 714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransSpec (yyP41->ASpec.Spec);
  }
   return CatSpecList (AppendSpec (SL, yyP41->ASpec.Spec), TransSpecList (yyP41->ASpec.Next));

  }
  if (yyP41->Kind == kNoSpec) {
/* line 717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP41;

  }
 yyAbort ("TransSpecList");
}

static tTree TransSpec
# if defined __STDC__ | defined __cplusplus
(register tTree yyP42)
# else
(yyP42)
 register tTree yyP42;
# endif
{
/* line 730 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree SL ; 

  switch (yyP42->Kind) {
  case kIs:
/* line 732 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransElemental (yyP42->Is.Elemental);

  case kRetypes:
  case kReshapes:
  if (Tree_IsType (yyP42->Retypes.Elemental, kElement)) {
/* line 733 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransElement (yyP42->Retypes.Elemental);

  }
  if (Tree_IsType (yyP42->Retypes.Elemental, kTableElement)) {
/* line 734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  tTree yyV1;
  {
/* line 736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransTableElement (yyP42->Retypes.Elemental, true, & yyV1);
/* line 737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP42 -> Retypes . Elemental = yyV1;
  }
  {
   return SL;
  }
 }

  }
  break;
  case kVal:
/* line 741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& yyP42->Val.Expr, true, false);

  case kValRetypes:
  case kValReshapes:
  if (yyP42->ValRetypes.Expr->Kind == kElementalExpr) {
/* line 743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return TransExpr (& yyP42->ValRetypes.Expr, false, false);

  }
/* line 744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsScalarType (yyP42->ValRetypes.Type)))) goto yyL6;
  {
/* line 744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((! IS_Mode (xxEmulate)))) goto yyL6;
  }
  }
   return TransExpr (& yyP42->ValRetypes.Expr, false, false);
yyL6:;

/* line 745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  register tTree yyV2;
  {
/* line 747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree TMP1,TMP2,TMP3; tSymbol Sym = TempSymbol(SymbolDepth(yyP42->ValRetypes.Sym)); 
/* line 749 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TMP2 = mIdentifier (yyP42->ValRetypes.Pos, Sym, Sym);
/* line 750 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ElementType (TMP2) = ExprType (yyP42->ValRetypes.Expr);
/* line 751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP2 -> Element) = false;
/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kElementalExpr)
    yyV1->ElementalExpr.Pos = yyP42->ValRetypes.Pos;
    yyV1->ElementalExpr.Elemental = TMP2;
   TMP1 = yyV1;
/* line 754 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (TMP1) = ExprType (yyP42->ValRetypes.Expr);
/* line 755 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ConstFlag (TMP1 -> Expr) = false;
/* line 757 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SL = TransExpr (& yyP42->ValRetypes.Expr, false, false);
/* line 758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kVal)
    yyV2->Val.Pos = yyP42->ValRetypes.Pos;
    yyV2->Val.Type = ExprType (yyP42->ValRetypes.Expr);
    yyV2->Val.Sym = Sym;
    yyV2->Val.Expr = yyP42->ValRetypes.Expr;
   SL = AppendSpec (SL, TMP3 = yyV2);
/* line 760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (Sym) . CId = UniqueId ('V');
/* line 761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (Sym) = TMP3;
/* line 762 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolScope (Sym) = SymbolScope (yyP42->ValRetypes.Sym);
/* line 763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolType (Sym) = ExprType (yyP42->ValRetypes.Expr);
/* line 764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP42->ValRetypes.Expr = TMP1;
  }
  {
   return SL;
  }
 }

  case kDefProc:
  case kInlineDefProc:
/* line 769 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 771 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP42 -> DefProc . isPar = false;
/* line 772 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP42->DefProc.Process);
/* line 773 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP42);
/* line 774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP42->DefProc.Sym) . CId = PrefixId (MakeIdent ("P", 1), Symbol (yyP42->DefProc.Sym) . CId, '_');
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kDefFunc:
  case kInlineDefFunc:
/* line 778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP42->DefFunc.Valof->Valof.SpecList = TransSpecList (yyP42->DefFunc.Valof->Valof.SpecList);
/* line 781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TransProc (yyP42->DefFunc.Valof->Valof.Process);
/* line 782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP42->DefFunc.Valof->Valof.SpecList = CatSpecList (yyP42->DefFunc.Valof->Valof.SpecList, TransExprList (yyP42->DefFunc.Valof->Valof.ExprList, false));
/* line 783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP42);
/* line 784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP42->DefFunc.Sym) . CId = PrefixId (MakeIdent ("F", 1), Symbol (yyP42->DefFunc.Sym) . CId, '_');
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kPrototypeProc:
/* line 788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP42);
/* line 790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP42->PrototypeProc.Sym) . CId = PrefixId (MakeIdent ("P", 1), Symbol (yyP42->PrototypeProc.Sym) . CId, '_');
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kPrototypeFunc:
/* line 793 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 794 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   AddCGNext (yyP42);
/* line 795 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Symbol (yyP42->PrototypeFunc.Sym) . CId = PrefixId (MakeIdent ("F", 1), Symbol (yyP42->PrototypeFunc.Sym) . CId, '_');
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kDeclType:
/* line 798 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kDecl:
/* line 799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kNullSpec:
/* line 800 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kDefSeqPro:
/* line 801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kDefTagPro:
/* line 802 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  case kHCode:
/* line 804 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Message ("#H directive ignored : only valid at outermost scope", xxWarning, yyP42->HCode.Pos);
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

  }

/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyWrite ("Fallen through TransSpec with kind ");
/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   printf ("%d", yyP42 -> Kind);
/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyWrite ("\n");
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   return yyV1;
  }
 }

}

static tTree TransDefIs2Func
# if defined __STDC__ | defined __cplusplus
(register tTree yyP43)
# else
(yyP43)
 register tTree yyP43;
# endif
{
/* line 812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 tTree Func; 
  if (yyP43->Kind == kInlineDefIsFunc) {
/* line 814 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Func = mInlineDefFunc (yyP43->InlineDefIsFunc.Pos, yyP43->InlineDefIsFunc.Sym, yyP43->InlineDefIsFunc.TypeList, yyP43->InlineDefIsFunc.Formals, mValof (yyP43->InlineDefIsFunc.Pos, mNoSpec (), mSkip (yyP43->InlineDefIsFunc.Pos, mNoSpec ()), yyP43->InlineDefIsFunc.ExprList, yyP43->InlineDefIsFunc.Sym, Symbol (yyP43->InlineDefIsFunc.Sym) . Depth));
/* line 816 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (yyP43->InlineDefIsFunc.Sym) = Func;
  }
   return Func;

  }
  if (Tree_IsType (yyP43, kDefIsFunc)) {
/* line 818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Func = mDefFunc (yyP43->DefIsFunc.Pos, yyP43->DefIsFunc.Sym, yyP43->DefIsFunc.TypeList, yyP43->DefIsFunc.Formals, mValof (yyP43->DefIsFunc.Pos, mNoSpec (), mSkip (yyP43->DefIsFunc.Pos, mNoSpec ()), yyP43->DefIsFunc.ExprList, yyP43->DefIsFunc.Sym, Symbol (yyP43->DefIsFunc.Sym) . Depth));
/* line 820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (yyP43->DefIsFunc.Sym) = Func;
  }
   return Func;

  }
/* line 823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP43;

}

static tTree Par2Instance
# if defined __STDC__ | defined __cplusplus
(register tTree yyP47, register tPosition yyP46, register tTree yyP45, tSymbol yyP44)
# else
(yyP47, yyP46, yyP45, yyP44)
 register tTree yyP47;
 register tPosition yyP46;
 register tTree yyP45;
 tSymbol yyP44;
# endif
{
/* line 828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
tTree SL,DP,EX,FL; 
/* line 830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  register tTree yyV2;
  {
/* line 831 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 if (yyP45 != NoTree)
      {
        tSymbol RS,FS;
        tTree TMP1,TMP2;
        int TMP3;

        RS = yyP45->Replicator.Sym; FS=RS+1;

        
        SymbolKind(FS)=SymbolKind(RS);
        SymbolType(FS)=SymbolType(RS);
        Symbol(FS).CId = Symbol(RS).CId;
        Symbol(FS).Id = Symbol(RS).Id;

        
        TMP3 = Symbol(FS).Depth;
        Symbol(FS).Depth = Symbol(RS).Depth;
        Symbol(RS).Depth = TMP3;

        
        
        FL = mAFormal(mNoFormal(),yyP46,mByVal(),SymbolType(RS),RS);
        SymbolKind(RS)=FL;
        yyP45->Replicator.Sym = FS;

        TMP1 = mIdentifier(yyP46,FS,FS);
        ElementType(TMP1) = SymbolType(FS);
        ConstFlag(TMP1->Element) = false;

        TMP2 = mElementalExpr(yyP46,TMP1);
        ExprType(TMP2) = ElementType(TMP1);
        ConstFlag(TMP2->Expr) = false;

        EX = mAnExpr(mNoExpr(),TMP2);
        ExprType(EX) = ElementType(TMP2);
        ConstFlag(EX->AnExpr) = false;

      }
      else
      {
        EX = mNoExpr();
        FL = mNoFormal();
      }
    
/* line 875 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kDefProc)
    yyV1->DefProc.Pos = yyP46;
    yyV1->DefProc.Sym = yyP44;
    yyV1->DefProc.Formals = FL;
    yyV1->DefProc.Process = yyP47;
   DP = yyV1;
/* line 876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV2,kASpec)
    yyV2->ASpec.Spec = DP;
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kNoSpec)
    yyV2->ASpec.Next = yyW1;
   }
   SL = TransSpecList (yyV2);
/* line 877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   DP -> DefProc . isPar = true;
/* line 878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   DP -> DefProc . ATOMIC = yyP47 -> Process . ATOMIC;
/* line 879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SymbolKind (DP -> DefProc . Sym) = DP;
/* line 880 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   DP -> DefProc . PUOut = CreateUsage (yyP46);
/* line 880 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   Include (& DP -> DefProc . PUOut -> R, yyP44);
  }
  {
  register tTree yyV3;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV3,kInstance)
    yyV3->Instance.Pos = yyP46;
    yyV3->Instance.SpecList = SL;
    yyV3->Instance.Sym = yyP44;
    yyV3->Instance.ExprList = EX;
   return yyV3;
  }
 }

}

static tTree CatSpecList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP49, register tTree yyP48)
# else
(yyP49, yyP48)
 register tTree yyP49;
 register tTree yyP48;
# endif
{
  if (yyP49->Kind == kNoSpec) {
/* line 896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP48;

  }
  if (yyP49->Kind == kASpec) {
/* line 898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP49->ASpec.Next = CatSpecList (yyP49->ASpec.Next, yyP48);
  }
   return yyP49;

  }
 yyAbort ("CatSpecList");
}

static tTree AppendSpec
# if defined __STDC__ | defined __cplusplus
(register tTree yyP51, register tTree yyP50)
# else
(yyP51, yyP50)
 register tTree yyP51;
 register tTree yyP50;
# endif
{
  if (yyP51->Kind == kNoSpec) {
/* line 908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kASpec)
    yyV1->ASpec.Spec = yyP50;
    yyV1->ASpec.Next = yyP51;
   return yyV1;
  }
 }

  }
  if (yyP51->Kind == kASpec) {
/* line 909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP51->ASpec.Next = AppendSpec (yyP51->ASpec.Next, yyP50);
  }
   return yyP51;

  }
 yyAbort ("AppendSpec");
}

static tTree ForceTypeList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP52)
# else
(yyP52)
 register tTree yyP52;
# endif
{
  if (yyP52->Kind == kAType) {
/* line 921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP52;

  }
/* line 922 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAType)
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kNoType)
    yyV1->AType.Next = yyW1;
   }
    yyV1->AType.Type = yyP52;
   return yyV1;
  }
 }

}

static int CountTypeList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP53)
# else
(yyP53)
 register tTree yyP53;
# endif
{
  if (yyP53->Kind == kNoType) {
/* line 927 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return 0;

  }
  if (yyP53->Kind == kAType) {
/* line 928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return CountTypeList (yyP53->AType.Next) + 1;

  }
 yyAbort ("CountTypeList");
}

static tTree SimplifyTableElement
# if defined __STDC__ | defined __cplusplus
(register tTree E)
# else
(E)
 register tTree E;
# endif
{
/* line 944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((ConstFlag (E -> TableElement)))) goto yyL1;
  }
   return E;
yyL1:;

  if (Tree_IsType (E, kTable)) {
/* line 946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
  if (E->Kind == kTableExpr) {
/* line 948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
  if (E->Kind == kTableSubscript) {
/* line 950 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->TableSubscript.Next = EnsureTableElement (SimplifyTableElement (E->TableSubscript.Next));
/* line 953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((false))) goto yyL4;
  }
   return E;
yyL4:;

  if (Tree_IsType (E->TableSubscript.Next, kTable)) {
/* line 955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 957 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->TableSubscript.Expr)))) goto yyL5;
  }
   return EvalExprListSubscript (E->TableSubscript.Next->Table.ExprList, ValConstInt (E->TableSubscript.Expr));
yyL5:;

  }
/* line 959 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
  if (E->Kind == kTableRange) {
/* line 961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 963 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->TableRange.Next = EnsureTableElement (SimplifyTableElement (E->TableRange.Next));
/* line 964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((false))) goto yyL7;
  }
   return E;
yyL7:;

  if (Tree_IsType (E->TableRange.Next, kTable)) {
/* line 966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->TableRange.From)))) goto yyL8;
  {
/* line 969 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->TableRange.For)))) goto yyL8;
  {
/* line 970 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->TableRange.Next->Table.ExprList = EvalExprListConstantSlice (E->TableRange.Next->Table.ExprList, ValConstInt (E->TableRange.From), ValConstInt (E->TableRange.For));
/* line 971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (E->TableRange.Next) = ExprType (E);
  }
  }
  }
   return E->TableRange.Next;
yyL8:;

/* line 973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->TableRange.From)))) goto yyL9;
  {
/* line 976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->TableRange.Next->Table.ExprList = EvalExprListVariableSlice (E->TableRange.Next->Table.ExprList, ValConstInt (E->TableRange.From));
/* line 977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (E->TableRange.Next) = ReduceOneDimArraySize (ExprType (E->TableRange.Next), ValConstInt (E->TableRange.From));
/* line 978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->TableRange.From -> Const . Value . Number = 0;
/* line 979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ValConstInt (E->TableRange.From) = 0;
  }
  }
   return E;
yyL9:;

  }
/* line 981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
 yyAbort ("SimplifyTableElement");
}

static tTree SimplifyStringElement
# if defined __STDC__ | defined __cplusplus
(register tTree E)
# else
(E)
 register tTree E;
# endif
{
/* line 994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstExpr (E)))) goto yyL1;
  }
   return E;
yyL1:;

  if (E->Kind == kString) {
/* line 996 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
  if (E->Kind == kStringSubscript) {
/* line 998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->StringSubscript.Next = EnsureStringElement (SimplifyStringElement (E->StringSubscript.Next));
/* line 1001 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((false))) goto yyL3;
  }
   return E;
yyL3:;

  if (E->StringSubscript.Next->Kind == kString) {
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->StringSubscript.Expr)))) goto yyL4;
  }
   return EvalStringSubscript (E->StringSubscript.Next, ValConstInt (E->StringSubscript.Expr));
yyL4:;

  }
/* line 1007 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
  if (E->Kind == kStringRange) {
/* line 1009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->StringRange.Next = EnsureStringElement (SimplifyStringElement (E->StringRange.Next));
/* line 1012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((false))) goto yyL6;
  }
   return E;
yyL6:;

  if (E->StringRange.Next->Kind == kString) {
/* line 1014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1016 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->StringRange.From)))) goto yyL7;
  {
/* line 1017 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->StringRange.For)))) goto yyL7;
  {
/* line 1018 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->StringRange.Next = EvalStringConstantSlice (E->StringRange.Next, ValConstInt (E->StringRange.From), ValConstInt (E->StringRange.For));
/* line 1019 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (E->StringRange.Next) = ExprType (E);
  }
  }
  }
   return E->StringRange.Next;
yyL7:;

  }
  if (E->StringRange.Next->Kind == kString) {
/* line 1021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1023 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((IsConstInt (E->StringRange.From)))) goto yyL8;
  {
/* line 1024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->StringRange.Next = EvalStringVariableSlice (E->StringRange.Next, ValConstInt (E->StringRange.From));
/* line 1025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ExprType (E->StringRange.Next) = ReduceOneDimArraySize (ExprType (E->StringRange.Next), ValConstInt (E->StringRange.From));
/* line 1026 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   E->StringRange.From -> Const . Value . Number = 0;
/* line 1027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   ValConstInt (E->StringRange.From) = 0;
  }
  }
   return E;
yyL8:;

  }
/* line 1029 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return E;

  }
 yyAbort ("SimplifyStringElement");
}

static tTree EvalExprListSubscript
# if defined __STDC__ | defined __cplusplus
(register tTree yyP55, register int yyP54)
# else
(yyP55, yyP54)
 register tTree yyP55;
 register int yyP54;
# endif
{
  if (yyP55->Kind == kAnExpr) {
/* line 1045 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1045 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP54 == 0))) goto yyL1;
  }
   return yyP55->AnExpr.Expr;
yyL1:;

/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP54 > 0))) goto yyL2;
  }
   return EvalExprListSubscript (yyP55->AnExpr.Next, yyP54 - 1);
yyL2:;

  }
 yyAbort ("EvalExprListSubscript");
}

static tTree EvalExprListConstantSlice
# if defined __STDC__ | defined __cplusplus
(register tTree yyP58, register int yyP57, register int yyP56)
# else
(yyP58, yyP57, yyP56)
 register tTree yyP58;
 register int yyP57;
 register int yyP56;
# endif
{
  if (yyP58->Kind == kAnExpr) {
/* line 1062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  {
/* line 1062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP56 == 0))) goto yyL1;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoExpr)
   return yyV1;
  }
 }
yyL1:;

/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 {
  register tTree yyV1;
  {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP57 == 0))) goto yyL2;
  {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP56 == 1))) goto yyL2;
  {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoExpr)
   yyP58->AnExpr.Next = yyV1;
  }
  }
  }
  {
   return yyP58;
  }
 }
yyL2:;

/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP57 == 0))) goto yyL3;
  {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP56 > 1))) goto yyL3;
  {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   yyP58->AnExpr.Next = EvalExprListConstantSlice (yyP58->AnExpr.Next, yyP57, yyP56 - 1);
  }
  }
  }
   return yyP58;
yyL3:;

/* line 1065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP57 > 0))) goto yyL4;
  }
   return EvalExprListConstantSlice (yyP58->AnExpr.Next, yyP57 - 1, yyP56);
yyL4:;

  }
 yyAbort ("EvalExprListConstantSlice");
}

static tTree EvalExprListVariableSlice
# if defined __STDC__ | defined __cplusplus
(register tTree yyP60, register int yyP59)
# else
(yyP60, yyP59)
 register tTree yyP60;
 register int yyP59;
# endif
{
/* line 1080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP59 == 0))) goto yyL1;
  }
   return yyP60;
yyL1:;

  if (yyP60->Kind == kAnExpr) {
/* line 1081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((yyP59 > 0))) goto yyL2;
  }
   return EvalExprListVariableSlice (yyP60->AnExpr.Next, yyP59 - 1);
yyL2:;

  }
 yyAbort ("EvalExprListVariableSlice");
}

static tTree EvalStringSubscript
# if defined __STDC__ | defined __cplusplus
(register tTree yyP62, register int yyP61)
# else
(yyP62, yyP61)
 register tTree yyP62;
 register int yyP61;
# endif
{
/* line 1097 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
char c; char *tmp; tTree TMP; tValue v;
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 int len=LengthSt(yyP62->String.Value);
      if ((tmp = malloc((len==0)?1:len+1)) == NULL)
        Message("Out of heap allocating string",xxError,yyP62->String.Pos);
      StGetString(yyP62->String.Value,tmp) ;
      c = tmp[yyP61] ;
      free(tmp) ; 

      v.Number = c;
      TMP = mConst(yyP62->String.Pos,v,mByte()) ;
      ConstFlag(TMP->Expr) = true;
      ValConstInt(TMP) = c;
      ExprType(TMP) = mByte();
    
  }
   return TMP;

}

static tTree EvalStringConstantSlice
# if defined __STDC__ | defined __cplusplus
(register tTree yyP65, register int yyP64, register int yyP63)
# else
(yyP65, yyP64, yyP63)
 register tTree yyP65;
 register int yyP64;
 register int yyP63;
# endif
{
/* line 1128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 char *tmp; 
/* line 1130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 int len=LengthSt(yyP65->String.Value);
      if ((tmp = malloc((len==0)?1:len+1)) == NULL)
        Message("Out of heap allocating string",xxError,yyP65->String.Pos);
      StGetString(yyP65->String.Value,tmp) ;
      yyP65->String.Value = PutString (&tmp[yyP64],yyP63) ;
      free(tmp) ;
    
  }
   return yyP65;

}

static tTree EvalStringVariableSlice
# if defined __STDC__ | defined __cplusplus
(register tTree yyP67, register int yyP66)
# else
(yyP67, yyP66)
 register tTree yyP67;
 register int yyP66;
# endif
{
/* line 1152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 char *tmp;
/* line 1154 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 int len=LengthSt(yyP67->String.Value);
      if ((tmp = malloc((len==0)?1:len+1)) == NULL)
        Message("Out of heap allocating string",xxError,yyP67->String.Pos);
      StGetString(yyP67->String.Value,tmp) ;
      yyP67->String.Value = PutString (&tmp[yyP66],LengthSt(yyP67->String.Value)-yyP66) ;
      free(tmp) ; 
  }
   return yyP67;

}

static tTree ReduceOneDimArraySize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP69, register int yyP68)
# else
(yyP69, yyP68)
 register tTree yyP69;
 register int yyP68;
# endif
{
  if (Tree_IsType (yyP69, kArray)) {
  if (yyP69->Array.Expr->Kind == kConst) {
/* line 1176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 yyP69->Array.Expr->Const.Value.Number = yyP69->Array.Expr->Const.Value.Number - yyP68;
  }
   return yyP69;

  }
  }
 yyAbort ("ReduceOneDimArraySize");
}

static tTree EnsureTableElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP70)
# else
(yyP70)
 register tTree yyP70;
# endif
{
  if (Tree_IsType (yyP70, kTableElement)) {
/* line 1186 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP70;

  }
  if (yyP70->Kind == kElementalExpr) {
  if (Tree_IsType (yyP70->ElementalExpr.Elemental, kTableElement)) {
/* line 1187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP70->ElementalExpr.Elemental;

  }
  }
 yyAbort ("EnsureTableElement");
}

static tTree EnsureStringElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP71)
# else
(yyP71)
 register tTree yyP71;
# endif
{
  if (Tree_IsType (yyP71, kStringElement)) {
/* line 1197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   return yyP71;

  }
 yyAbort ("EnsureStringElement");
}

static void AddCGNext
# if defined __STDC__ | defined __cplusplus
(register tTree T)
# else
(T)
 register tTree T;
# endif
{
/* line 1204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
 static tTree CGLast; 
  if (T->Kind == kStart) {
/* line 1206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   CGLast = NoTree;
  }
   return;

  }
/* line 1208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   if (! ((CGLast == NoTree))) goto yyL2;
  {
/* line 1208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   TreeRoot -> Start . CGNext = T;
/* line 1208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   CGLast = T;
/* line 1208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SetCGNext (T, TreeRoot);
  }
  }
   return;
yyL2:;

/* line 1210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SetCGNext (CGLast, T);
/* line 1210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   CGLast = T;
/* line 1210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   SetCGNext (T, TreeRoot);
  }
   return;

;
}

static void SetCGNext
# if defined __STDC__ | defined __cplusplus
(register tTree T, register tTree N)
# else
(T, N)
 register tTree T;
 register tTree N;
# endif
{
  if (Tree_IsType (T, kDefAProc)) {
/* line 1215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   T -> DefAProc . CGNext = N;
  }
   return;

  }
  if (Tree_IsType (T, kDefAFunc)) {
/* line 1216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   T -> DefAFunc . CGNext = N;
  }
   return;

  }
  if (T->Kind == kHCode) {
/* line 1217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
  {
/* line 1217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/trans.trafo" */
   T -> HCode . CGNext = N;
  }
   return;

  }
;
}

void BeginTransform ()
{
}

void CloseTransform ()
{
}
