# define DEP(a, b) a
# define SELF yyt
# include "TypeChkAg.h"

/* line 657 "" */
 
  #define FALSE 0 
  static tTree T;
  static int ag_phase=0;


static char yyb;

static void yyVisit1 ARGS((register tTree yyt));

void TypeChkAg
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{ yyVisit1 (yyt); }

static void yyVisit1
# if defined __STDC__ | defined __cplusplus
 (register tTree yyt)
# else
 (yyt) register tTree yyt;
# endif
{

 switch (yyt->Kind) {

case kStart:
/* line 1037 "" */
 yyt->Start.Process->Process.VIn = 0;
/* line 1027 "" */
 yyt->Start.Process->Process.VS = 0;
yyVisit1 (yyt->Start.Process);
break;

case kSpecList:
yyt->SpecList.VOut=yyt->SpecList.VIn;
break;

case kNoSpec:
yyt->NoSpec.VOut=yyt->NoSpec.VIn;
break;

case kASpec:
yyt->ASpec.Spec->Spec.VS=yyt->ASpec.VS;
yyt->ASpec.Next->SpecList.VS=yyt->ASpec.VS;
yyt->ASpec.Spec->Spec.VIn=yyt->ASpec.VIn;
yyVisit1 (yyt->ASpec.Spec);
yyt->ASpec.Next->SpecList.VIn=yyt->ASpec.Spec->Spec.VOut;
yyVisit1 (yyt->ASpec.Next);
yyt->ASpec.VOut=yyt->ASpec.Next->SpecList.VOut;
break;

case kSpec:
yyt->Spec.VOut=yyt->Spec.VIn;
break;

case kDeclType:
yyt->DeclType.Type->Type.VIn=yyt->DeclType.VIn;
yyVisit1 (yyt->DeclType.Type);
yyt->DeclType.VOut=yyt->DeclType.Type->Type.VOut;
/* line 824 "" */
if (! ( CheckValidDeclType(yyt->DeclType.Type) )) { Message("Invalid type for named type",xxError,yyt->DeclType.Pos)
	       ; }
else
/* line 825 "" */
if (! ( CheckValidUserType(yyt->DeclType.Type) )) { Message("User struct has fields of inappropriate type",xxError,yyt->DeclType.Pos); }
break;

case kDecl:
yyt->Decl.Type->Type.VIn=yyt->Decl.VIn;
yyVisit1 (yyt->Decl.Type);
yyt->Decl.VOut=yyt->Decl.Type->Type.VOut;
yyVisit1 (yyt->Decl.IdList);
/* line 823 "" */
if (! ( CheckValidDeclType(yyt->Decl.Type) )) { Message("Invalid type for declaration",xxError,yyt->Decl.Pos); }
break;

case kIs:
yyt->Is.Type->Type.VIn=yyt->Is.VIn;
yyVisit1 (yyt->Is.Type);
yyt->Is.Elemental->Elemental.VIn=yyt->Is.Type->Type.VOut;
/* line 1212 "" */
yyt->Is.Elemental->Elemental.U = xxUsed;
/* line 962 "" */
 yyt->Is.Elemental->Elemental.InhType = NoTree;
yyVisit1 (yyt->Is.Elemental);
yyt->Is.VOut=yyt->Is.Elemental->Elemental.VOut;
/* line 1237 "" */
 yyt->Is.U =xxUnused;
/* line 807 "" */
if (! ( FALSE )) { SymbolType(yyt->Is.Sym)=TypeAbbrev(yyt->Is.Type,yyt->Is.Elemental->Elemental.SynType,yyt->Is.Pos);; }
/* line 808 "" */
if (! ( (yyt->Is.Elemental->Elemental.W) )) { Message("Abbreviated element is not assignable",xxError,yyt->Is.Pos); }
/* line 1196 "" */
if (! ( DEP(FALSE,yyt->Is.Elemental->Elemental.SynType) )) { yyt->Is.Elemental = ReduceElemental(yyt->Is.Elemental);; }
break;

case kAVal:
yyt->AVal.Type->Type.VIn=yyt->AVal.VIn;
yyVisit1 (yyt->AVal.Type);
yyt->AVal.Expr->Expr.VIn=yyt->AVal.Type->Type.VOut;
/* line 977 "" */
 yyt->AVal.Expr->Expr.InhType = DefaultType(yyt->AVal.Expr);
yyVisit1 (yyt->AVal.Expr);
yyt->AVal.VOut=yyt->AVal.Expr->Expr.VOut;
break;

case kVal:
yyt->Val.Type->Type.VIn=yyt->Val.VIn;
yyVisit1 (yyt->Val.Type);
yyt->Val.Expr->Expr.VIn=yyt->Val.Type->Type.VOut;
/* line 978 "" */
 yyt->Val.Expr->Expr.InhType = InheritLhsType(yyt->Val.Type,DefaultType(yyt->Val.Expr));
yyVisit1 (yyt->Val.Expr);
yyt->Val.VOut=yyt->Val.Expr->Expr.VOut;
/* line 805 "" */
if (! ( (!IsChanOrTimerType(yyt->Val.Expr->Expr.SynType)) )) { Message("Cannot VAL abbreviate a CHAN or TIMER",xxError,yyt->Val.Pos); }
/* line 806 "" */
if (! ( FALSE )) { SymbolType(yyt->Val.Sym)=TypeAbbrev(yyt->Val.Type,yyt->Val.Expr->Expr.SynType,yyt->Val.Pos);; }
break;

case kValRetypes:
yyt->ValRetypes.Type->Type.VIn=yyt->ValRetypes.VIn;
yyVisit1 (yyt->ValRetypes.Type);
yyt->ValRetypes.Expr->Expr.VIn=yyt->ValRetypes.Type->Type.VOut;
/* line 977 "" */
 yyt->ValRetypes.Expr->Expr.InhType = DefaultType(yyt->ValRetypes.Expr);
yyVisit1 (yyt->ValRetypes.Expr);
yyt->ValRetypes.VOut=yyt->ValRetypes.Expr->Expr.VOut;
/* line 813 "" */
if (! ( FALSE )) { TypeCheckRetypes(SELF,yyt->ValRetypes.Type,yyt->ValRetypes.Expr->Expr.SynType,yyt->ValRetypes.Pos); }
break;

case kValReshapes:
yyt->ValReshapes.Type->Type.VIn=yyt->ValReshapes.VIn;
yyVisit1 (yyt->ValReshapes.Type);
yyt->ValReshapes.Expr->Expr.VIn=yyt->ValReshapes.Type->Type.VOut;
/* line 977 "" */
 yyt->ValReshapes.Expr->Expr.InhType = DefaultType(yyt->ValReshapes.Expr);
yyVisit1 (yyt->ValReshapes.Expr);
yyt->ValReshapes.VOut=yyt->ValReshapes.Expr->Expr.VOut;
/* line 814 "" */
if (! ( (IsArrayType(yyt->ValReshapes.Type) && IsArrayType(yyt->ValReshapes.Expr->Expr.SynType) && SameBaseType(yyt->ValReshapes.Type,yyt->ValReshapes.Expr->Expr.SynType)) )) {
		 Message("Only allowed to reshape arrays to same base type",xxError,yyt->ValReshapes.Pos); }
/* line 813 "" */
if (! ( FALSE )) { TypeCheckRetypes(SELF,yyt->ValReshapes.Type,yyt->ValReshapes.Expr->Expr.SynType,yyt->ValReshapes.Pos); }
break;

case kRetypes:
yyt->Retypes.Type->Type.VIn=yyt->Retypes.VIn;
yyVisit1 (yyt->Retypes.Type);
yyt->Retypes.Elemental->Elemental.VIn=yyt->Retypes.Type->Type.VOut;
/* line 1213 "" */
yyt->Retypes.Elemental->Elemental.U = xxUsed;
/* line 963 "" */
 yyt->Retypes.Elemental->Elemental.InhType = NoTree;
yyVisit1 (yyt->Retypes.Elemental);
yyt->Retypes.VOut=yyt->Retypes.Elemental->Elemental.VOut;
/* line 809 "" */
if (! ( FALSE )) { TypeCheckRetypes(SELF,yyt->Retypes.Type,yyt->Retypes.Elemental->Elemental.SynType,yyt->Retypes.Pos); }
/* line 810 "" */
if (! ( (yyt->Retypes.Elemental->Elemental.W) )) { Message("Retyped element is not assignable",xxError,yyt->Retypes.Pos); }
/* line 1197 "" */
if (! ( DEP(FALSE,yyt->Retypes.Elemental->Elemental.SynType) )) { yyt->Retypes.Elemental = ReduceElemental(yyt->Retypes.Elemental);; }
break;

case kReshapes:
yyt->Reshapes.Type->Type.VIn=yyt->Reshapes.VIn;
yyVisit1 (yyt->Reshapes.Type);
yyt->Reshapes.Elemental->Elemental.VIn=yyt->Reshapes.Type->Type.VOut;
/* line 1213 "" */
yyt->Reshapes.Elemental->Elemental.U = xxUsed;
/* line 963 "" */
 yyt->Reshapes.Elemental->Elemental.InhType = NoTree;
yyVisit1 (yyt->Reshapes.Elemental);
yyt->Reshapes.VOut=yyt->Reshapes.Elemental->Elemental.VOut;
/* line 811 "" */
if (! ( (IsArrayType(yyt->Reshapes.Type) && IsArrayType(yyt->Reshapes.Elemental->Elemental.SynType) && SameBaseType(yyt->Reshapes.Type,yyt->Reshapes.Elemental->Elemental.SynType)) )) {
		   Message("Only allowed to reshape arrays to same base type",xxError,yyt->Reshapes.Pos); }
/* line 809 "" */
if (! ( FALSE )) { TypeCheckRetypes(SELF,yyt->Reshapes.Type,yyt->Reshapes.Elemental->Elemental.SynType,yyt->Reshapes.Pos); }
/* line 810 "" */
if (! ( (yyt->Reshapes.Elemental->Elemental.W) )) { Message("Retyped element is not assignable",xxError,yyt->Reshapes.Pos); }
/* line 1197 "" */
if (! ( DEP(FALSE,yyt->Reshapes.Elemental->Elemental.SynType) )) { yyt->Reshapes.Elemental = ReduceElemental(yyt->Reshapes.Elemental);; }
break;

case kDefAPro:
yyt->DefAPro.VOut=yyt->DefAPro.VIn;
break;

case kDefSeqPro:
yyt->DefSeqPro.VOut=yyt->DefSeqPro.VIn;
yyVisit1 (yyt->DefSeqPro.SeqProList);
break;

case kDefTagPro:
yyt->DefTagPro.VOut=yyt->DefTagPro.VIn;
yyVisit1 (yyt->DefTagPro.TagProList);
break;

case kDefAProc:
yyt->DefAProc.Formals->Formals.VIn=yyt->DefAProc.VIn;
/* line 1224 "" */
 yyt->DefAProc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->DefAProc.Formals);
yyt->DefAProc.VOut=yyt->DefAProc.Formals->Formals.VOut;
/* line 1224 "" */
 yyt->DefAProc.U = xxUnused;
break;

case kDefProc:
yyt->DefProc.Formals->Formals.VIn=yyt->DefProc.VIn;
/* line 1224 "" */
 yyt->DefProc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->DefProc.Formals);
yyt->DefProc.Process->Process.VIn=yyt->DefProc.Formals->Formals.VOut;
yyt->DefProc.Process->Process.VS=yyt->DefProc.VS;
yyVisit1 (yyt->DefProc.Process);
yyt->DefProc.VOut=yyt->DefProc.Process->Process.VOut;
/* line 1303 "" */
 yyt->DefProc.ATOMIC = yyt->DefProc.Process->Process.ATOMIC;
/* line 1224 "" */
 yyt->DefProc.U = xxUnused;
break;

case kInlineDefProc:
yyt->InlineDefProc.Formals->Formals.VIn=yyt->InlineDefProc.VIn;
/* line 1224 "" */
 yyt->InlineDefProc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->InlineDefProc.Formals);
yyt->InlineDefProc.Process->Process.VIn=yyt->InlineDefProc.Formals->Formals.VOut;
yyt->InlineDefProc.Process->Process.VS=yyt->InlineDefProc.VS;
yyVisit1 (yyt->InlineDefProc.Process);
yyt->InlineDefProc.VOut=yyt->InlineDefProc.Process->Process.VOut;
/* line 1303 "" */
 yyt->InlineDefProc.ATOMIC = yyt->InlineDefProc.Process->Process.ATOMIC;
/* line 1224 "" */
 yyt->InlineDefProc.U = xxUnused;
break;

case kPrototypeProc:
yyt->PrototypeProc.Formals->Formals.VIn=yyt->PrototypeProc.VIn;
/* line 1225 "" */
 yyt->PrototypeProc.Formals->Formals.InPrototype = true;
yyVisit1 (yyt->PrototypeProc.Formals);
yyt->PrototypeProc.VOut=yyt->PrototypeProc.Formals->Formals.VOut;
/* line 1224 "" */
 yyt->PrototypeProc.U = xxUnused;
break;

case kDefAFunc:
yyt->DefAFunc.TypeList->TypeList.VIn=yyt->DefAFunc.VIn;
yyVisit1 (yyt->DefAFunc.TypeList);
yyt->DefAFunc.Formals->Formals.VIn=yyt->DefAFunc.TypeList->TypeList.VOut;
/* line 1226 "" */
 yyt->DefAFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->DefAFunc.Formals);
yyt->DefAFunc.VOut=yyt->DefAFunc.Formals->Formals.VOut;
/* line 1226 "" */
 yyt->DefAFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->DefAFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->DefAFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->DefAFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->DefAFunc.Pos); }
break;

case kDefFunc:
yyt->DefFunc.TypeList->TypeList.VIn=yyt->DefFunc.VIn;
yyVisit1 (yyt->DefFunc.TypeList);
yyt->DefFunc.Formals->Formals.VIn=yyt->DefFunc.TypeList->TypeList.VOut;
/* line 1226 "" */
 yyt->DefFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->DefFunc.Formals);
yyt->DefFunc.Valof->Valof.VIn=yyt->DefFunc.Formals->Formals.VOut;
/* line 974 "" */
 yyt->DefFunc.Valof->Valof.InhType = yyt->DefFunc.TypeList;
yyVisit1 (yyt->DefFunc.Valof);
yyt->DefFunc.VOut=yyt->DefFunc.Valof->Valof.VOut;
/* line 1304 "" */
 yyt->DefFunc.ATOMIC = true;
/* line 822 "" */
if (! ( FALSE )) { TypeCheckResults(yyt->DefFunc.TypeList,yyt->DefFunc.Valof->Valof.ExprList,yyt->DefFunc.Pos); }
/* line 1226 "" */
 yyt->DefFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->DefFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->DefFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->DefFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->DefFunc.Pos); }
break;

case kInlineDefFunc:
yyt->InlineDefFunc.TypeList->TypeList.VIn=yyt->InlineDefFunc.VIn;
yyVisit1 (yyt->InlineDefFunc.TypeList);
yyt->InlineDefFunc.Formals->Formals.VIn=yyt->InlineDefFunc.TypeList->TypeList.VOut;
/* line 1226 "" */
 yyt->InlineDefFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->InlineDefFunc.Formals);
yyt->InlineDefFunc.Valof->Valof.VIn=yyt->InlineDefFunc.Formals->Formals.VOut;
/* line 974 "" */
 yyt->InlineDefFunc.Valof->Valof.InhType = yyt->InlineDefFunc.TypeList;
yyVisit1 (yyt->InlineDefFunc.Valof);
yyt->InlineDefFunc.VOut=yyt->InlineDefFunc.Valof->Valof.VOut;
/* line 1304 "" */
 yyt->InlineDefFunc.ATOMIC = true;
/* line 822 "" */
if (! ( FALSE )) { TypeCheckResults(yyt->InlineDefFunc.TypeList,yyt->InlineDefFunc.Valof->Valof.ExprList,yyt->InlineDefFunc.Pos); }
/* line 1226 "" */
 yyt->InlineDefFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->InlineDefFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->InlineDefFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->InlineDefFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->InlineDefFunc.Pos); }
break;

case kDefIsFunc:
yyt->DefIsFunc.TypeList->TypeList.VIn=yyt->DefIsFunc.VIn;
yyVisit1 (yyt->DefIsFunc.TypeList);
yyt->DefIsFunc.Formals->Formals.VIn=yyt->DefIsFunc.TypeList->TypeList.VOut;
/* line 944 "" */
InheritFromType(yyt->DefIsFunc.TypeList,yyt->DefIsFunc.ExprList);
/* line 1226 "" */
 yyt->DefIsFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->DefIsFunc.Formals);
yyt->DefIsFunc.ExprList->ExprList.VIn=yyt->DefIsFunc.Formals->Formals.VOut;
yyVisit1 (yyt->DefIsFunc.ExprList);
yyt->DefIsFunc.VOut=yyt->DefIsFunc.ExprList->ExprList.VOut;
/* line 1305 "" */
 yyt->DefIsFunc.ATOMIC = true;
/* line 821 "" */
if (! ( FALSE )) { TypeCheckResults(yyt->DefIsFunc.TypeList,DEP(yyt->DefIsFunc.ExprList,yyt->DefIsFunc.ExprList->ExprList.SynType),yyt->DefIsFunc.Pos); }
/* line 1226 "" */
 yyt->DefIsFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->DefIsFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->DefIsFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->DefIsFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->DefIsFunc.Pos); }
break;

case kInlineDefIsFunc:
yyt->InlineDefIsFunc.TypeList->TypeList.VIn=yyt->InlineDefIsFunc.VIn;
yyVisit1 (yyt->InlineDefIsFunc.TypeList);
yyt->InlineDefIsFunc.Formals->Formals.VIn=yyt->InlineDefIsFunc.TypeList->TypeList.VOut;
/* line 944 "" */
InheritFromType(yyt->InlineDefIsFunc.TypeList,yyt->InlineDefIsFunc.ExprList);
/* line 1226 "" */
 yyt->InlineDefIsFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->InlineDefIsFunc.Formals);
yyt->InlineDefIsFunc.ExprList->ExprList.VIn=yyt->InlineDefIsFunc.Formals->Formals.VOut;
yyVisit1 (yyt->InlineDefIsFunc.ExprList);
yyt->InlineDefIsFunc.VOut=yyt->InlineDefIsFunc.ExprList->ExprList.VOut;
/* line 1305 "" */
 yyt->InlineDefIsFunc.ATOMIC = true;
/* line 821 "" */
if (! ( FALSE )) { TypeCheckResults(yyt->InlineDefIsFunc.TypeList,DEP(yyt->InlineDefIsFunc.ExprList,yyt->InlineDefIsFunc.ExprList->ExprList.SynType),yyt->InlineDefIsFunc.Pos); }
/* line 1226 "" */
 yyt->InlineDefIsFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->InlineDefIsFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->InlineDefIsFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->InlineDefIsFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->InlineDefIsFunc.Pos); }
break;

case kPrototypeFunc:
yyt->PrototypeFunc.TypeList->TypeList.VIn=yyt->PrototypeFunc.VIn;
yyVisit1 (yyt->PrototypeFunc.TypeList);
yyt->PrototypeFunc.Formals->Formals.VIn=yyt->PrototypeFunc.TypeList->TypeList.VOut;
/* line 1226 "" */
 yyt->PrototypeFunc.Formals->Formals.InPrototype = false;
yyVisit1 (yyt->PrototypeFunc.Formals);
yyt->PrototypeFunc.VOut=yyt->PrototypeFunc.Formals->Formals.VOut;
/* line 1226 "" */
 yyt->PrototypeFunc.U = xxUnused;
/* line 818 "" */
if (! ( CheckForVals(yyt->PrototypeFunc.Formals) )) { Message("All formal parameters of functions must be passed by value",xxError,yyt->PrototypeFunc.Pos); }
/* line 819 "" */
if (! ( CheckFuncType(yyt->PrototypeFunc.TypeList) )) { Message("Function returns invalid type",xxError,yyt->PrototypeFunc.Pos); }
break;

case kNullSpec:
yyt->NullSpec.VOut=yyt->NullSpec.VIn;
break;

case kHCode:
yyt->HCode.VOut=yyt->HCode.VIn;
break;

case kCallFrame:
yyt->CallFrame.VOut=yyt->CallFrame.VIn;
break;

case kParFrame:
yyt->ParFrame.VOut=yyt->ParFrame.VIn;
break;

case kFormals:
yyt->Formals.VOut=yyt->Formals.VIn;
break;

case kNoFormal:
yyt->NoFormal.VOut=yyt->NoFormal.VIn;
break;

case kNoFormals:
yyt->NoFormals.VOut=yyt->NoFormals.VIn;
break;

case kAFormal:
yyt->AFormal.Next->Formals.VIn=yyt->AFormal.VIn;
yyt->AFormal.Next->Formals.InPrototype=yyt->AFormal.InPrototype;
yyVisit1 (yyt->AFormal.Next);
yyt->AFormal.Type->Type.VIn=yyt->AFormal.Next->Formals.VOut;
yyVisit1 (yyt->AFormal.Type);
yyt->AFormal.VOut=yyt->AFormal.Type->Type.VOut;
yyVisit1 (yyt->AFormal.By);
/* line 1227 "" */

		     if (yyt->AFormal.InPrototype)
		     {
		       if (MakeIdent("out",3)==Symbol(yyt->AFormal.Sym).Id) yyt->AFormal.U=xxOutput;
		       if (MakeIdent("in",2)==Symbol(yyt->AFormal.Sym).Id) yyt->AFormal.U=xxInput;
		     }
		     else 
		       yyt->AFormal.U=xxUnused;
		   
break;

case kReplicator:
yyt->Replicator.From->Expr.VIn=yyt->Replicator.VIn;
/* line 979 "" */
 yyt->Replicator.From->Expr.InhType = mInt();
yyVisit1 (yyt->Replicator.From);
yyt->Replicator.For->Expr.VIn=yyt->Replicator.From->Expr.VOut;
/* line 979 "" */
 yyt->Replicator.For->Expr.InhType = mInt();
yyVisit1 (yyt->Replicator.For);
/* line 931 "" */
if (! ( Tree_IsType(yyt->Replicator.For->Expr.SynType,kInt) )) { Message("Replicator FOR expression not of type INT",xxError,yyt->Replicator.For->Expr.Pos); }
yyt->Replicator.VOut=yyt->Replicator.For->Expr.VOut;
/* line 930 "" */
if (! ( Tree_IsType(yyt->Replicator.From->Expr.SynType,kInt) )) { Message("Replicator FROM expression not of type INT",xxError,yyt->Replicator.From->Expr.Pos); }
break;

case kTagProList:
break;

case kNoTagPro:
break;

case kATagPro:
yyVisit1 (yyt->ATagPro.SeqProList);
yyVisit1 (yyt->ATagPro.Next);
break;

case kBy:
break;

case kByVal:
break;

case kByRef:
break;

case kByUndef:
break;

case kIdList:
break;

case kNoId:
break;

case kAnId:
yyVisit1 (yyt->AnId.Next);
/* line 1236 "" */
 yyt->AnId.U = xxUnused;
break;

case kType:
yyt->Type.VOut=yyt->Type.VIn;
break;

case kArray:
yyt->Array.Expr->Expr.VIn=yyt->Array.VIn;
/* line 980 "" */
 yyt->Array.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->Array.Expr);
yyt->Array.Type->Type.VIn=yyt->Array.Expr->Expr.VOut;
yyVisit1 (yyt->Array.Type);
yyt->Array.VOut=yyt->Array.Type->Type.VOut;
break;

case kUserArray:
yyt->UserArray.Expr->Expr.VIn=yyt->UserArray.VIn;
/* line 980 "" */
 yyt->UserArray.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->UserArray.Expr);
yyt->UserArray.Type->Type.VIn=yyt->UserArray.Expr->Expr.VOut;
yyVisit1 (yyt->UserArray.Type);
yyt->UserArray.VOut=yyt->UserArray.Type->Type.VOut;
break;

case kChan:
yyt->Chan.VOut=yyt->Chan.VIn;
yyVisit1 (yyt->Chan.Protocol);
break;

case kTimer:
yyt->Timer.VOut=yyt->Timer.VIn;
break;

case kUntyped:
yyt->Untyped.VOut=yyt->Untyped.VIn;
break;

case kUntypedByte:
yyt->UntypedByte.VOut=yyt->UntypedByte.VIn;
break;

case kUntypedReal:
yyt->UntypedReal.VOut=yyt->UntypedReal.VIn;
break;

case kUntypedInteger:
yyt->UntypedInteger.VOut=yyt->UntypedInteger.VIn;
break;

case kUntypedInt:
yyt->UntypedInt.VOut=yyt->UntypedInt.VIn;
break;

case kUntypedHex:
yyt->UntypedHex.VOut=yyt->UntypedHex.VIn;
break;

case kPrimitive:
yyt->Primitive.VOut=yyt->Primitive.VIn;
break;

case kNumeric:
yyt->Numeric.VOut=yyt->Numeric.VIn;
break;

case kInteger:
yyt->Integer.VOut=yyt->Integer.VIn;
break;

case kInt:
yyt->Int.VOut=yyt->Int.VIn;
break;

case kUserInt:
yyt->UserInt.VOut=yyt->UserInt.VIn;
break;

case kInt16:
yyt->Int16.VOut=yyt->Int16.VIn;
break;

case kUserInt16:
yyt->UserInt16.VOut=yyt->UserInt16.VIn;
break;

case kInt32:
yyt->Int32.VOut=yyt->Int32.VIn;
break;

case kUserInt32:
yyt->UserInt32.VOut=yyt->UserInt32.VIn;
break;

case kInt64:
yyt->Int64.VOut=yyt->Int64.VIn;
break;

case kUserInt64:
yyt->UserInt64.VOut=yyt->UserInt64.VIn;
break;

case kReal:
yyt->Real.VOut=yyt->Real.VIn;
break;

case kReal32:
yyt->Real32.VOut=yyt->Real32.VIn;
break;

case kUserReal32:
yyt->UserReal32.VOut=yyt->UserReal32.VIn;
break;

case kReal64:
yyt->Real64.VOut=yyt->Real64.VIn;
break;

case kUserReal64:
yyt->UserReal64.VOut=yyt->UserReal64.VIn;
break;

case kByte:
yyt->Byte.VOut=yyt->Byte.VIn;
break;

case kUserByte:
yyt->UserByte.VOut=yyt->UserByte.VIn;
break;

case kBool:
yyt->Bool.VOut=yyt->Bool.VIn;
break;

case kUserBool:
yyt->UserBool.VOut=yyt->UserBool.VIn;
break;

case kUserStructTypes:
yyt->UserStructTypes.VOut=yyt->UserStructTypes.VIn;
yyVisit1 (yyt->UserStructTypes.FieldList);
break;

case kUserDataTypes:
yyt->UserDataTypes.VOut=yyt->UserDataTypes.VIn;
yyVisit1 (yyt->UserDataTypes.FieldList);
break;

case kUserRecord:
yyt->UserRecord.VOut=yyt->UserRecord.VIn;
yyVisit1 (yyt->UserRecord.FieldList);
break;

case kUserPackedRecord:
yyt->UserPackedRecord.VOut=yyt->UserPackedRecord.VIn;
yyVisit1 (yyt->UserPackedRecord.FieldList);
break;

case kUserUnion:
yyt->UserUnion.VOut=yyt->UserUnion.VIn;
yyVisit1 (yyt->UserUnion.FieldList);
break;

case kUserChanRecord:
yyt->UserChanRecord.VOut=yyt->UserChanRecord.VIn;
yyVisit1 (yyt->UserChanRecord.FieldList);
break;

case kTypeList:
yyt->TypeList.VOut=yyt->TypeList.VIn;
break;

case kNoType:
yyt->NoType.VOut=yyt->NoType.VIn;
break;

case kAType:
yyt->AType.Next->TypeList.VIn=yyt->AType.VIn;
yyVisit1 (yyt->AType.Next);
yyt->AType.Type->Type.VIn=yyt->AType.Next->TypeList.VOut;
yyVisit1 (yyt->AType.Type);
yyt->AType.VOut=yyt->AType.Type->Type.VOut;
break;

case kFieldList:
break;

case kNoField:
break;

case kAField:
/* line 1036 "" */
yyt->AField.Type->Type.VIn = 0;
yyVisit1 (yyt->AField.Type);
yyVisit1 (yyt->AField.Next);
break;

case kProtocol:
break;

case kAny:
break;

case kNamed:
/* line 816 "" */
if (! ( Tree_IsType(SymbolKind(yyt->Named.Sym),kDefAPro) )) { Message("Channel protocol not declared as a protocol",xxError,yyt->Named.Pos); }
break;

case kSimple:
/* line 926 "" */
if (! ( (!IsChanOrTimerType(yyt->Simple.Type)) )) { Message("CHAN and TIMER cannot be part of PROTOCOL",xxError,yyt->Simple.Pos); }
/* line 1038 "" */
yyt->Simple.Type->Type.VIn = 0;
yyVisit1 (yyt->Simple.Type);
break;

case kCounted:
/* line 1039 "" */
yyt->Counted.Length->Type.VIn = 0;
yyVisit1 (yyt->Counted.Length);
yyt->Counted.Type->Type.VIn=yyt->Counted.Length->Type.VOut;
yyVisit1 (yyt->Counted.Type);
break;

case kSeqProList:
break;

case kNoSeqPro:
break;

case kASeqPro:
yyVisit1 (yyt->ASeqPro.Next);
yyVisit1 (yyt->ASeqPro.Protocol);
break;

case kExpr:
yyt->Expr.VOut=yyt->Expr.VIn;
/* line 795 "" */
 yyt->Expr.SynType = NoTree;
/* line 1049 "" */
 yyt->Expr.C = false;
/* line 1049 "" */
 yyt->Expr.CV.Number = 0; 
break;

case kBinary:
yyt->Binary.Lop->Expr.VIn=yyt->Binary.VIn;
/* line 985 "" */
 
    yyt->Binary.Lop->Expr.InhType = ((Tree_IsType(yyt->Binary.Op,kRelOp)) ? DefaultType(SELF) :yyt->Binary.InhType);
yyVisit1 (yyt->Binary.Lop);
yyt->Binary.Rop->Expr.VIn=yyt->Binary.Lop->Expr.VOut;
/* line 986 "" */

    yyt->Binary.Rop->Expr.InhType = ((Tree_IsType(yyt->Binary.Op,kShiftOp)) ? mInt() : ((Tree_IsType(yyt->Binary.Op,kRelOp)) ? DefaultType(SELF) :yyt->Binary.InhType));
yyVisit1 (yyt->Binary.Rop);
yyt->Binary.VOut=yyt->Binary.Rop->Expr.VOut;
yyVisit1 (yyt->Binary.Op);
/* line 855 "" */
 yyt->Binary.SynType = TypeBinary(yyt->Binary.Lop->Expr.SynType,yyt->Binary.Op,yyt->Binary.Rop->Expr.SynType,yyt->Binary.Pos);
/* line 1076 "" */
 yyt->Binary.C =  yyt->Binary.Lop->Expr.C && yyt->Binary.Rop->Expr.C && CheckEvalBiOp(yyt->Binary.Op,yyt->Binary.SynType) ;
/* line 1077 "" */
 if (yyt->Binary.C) yyt->Binary.CV=EvalBiExpr(yyt->Binary.Pos,yyt->Binary.Lop->Expr.SynType,yyt->Binary.Lop->Expr.CV,yyt->Binary.Op,yyt->Binary.Rop->Expr.CV); else yyt->Binary.CV.Number=0; 
break;

case kUnary:
yyt->Unary.Expr->Expr.VIn=yyt->Unary.VIn;
yyt->Unary.Expr->Expr.InhType=yyt->Unary.InhType;
yyVisit1 (yyt->Unary.Expr);
yyt->Unary.VOut=yyt->Unary.Expr->Expr.VOut;
yyVisit1 (yyt->Unary.Op);
/* line 856 "" */
 yyt->Unary.SynType = TypeUnary(yyt->Unary.Op,yyt->Unary.Expr->Expr.SynType,yyt->Unary.Pos);
/* line 1079 "" */
 yyt->Unary.C = CheckEvalUniOp(ExprType(yyt->Unary.Expr),yyt->Unary.Op,yyt->Unary.Expr);
/* line 1080 "" */
 if (yyt->Unary.C) yyt->Unary.CV=EvalUniExpr(ExprType(yyt->Unary.Expr),yyt->Unary.Op,yyt->Unary.Expr); else yyt->Unary.CV.Number=0;
break;

case kConst:
yyt->Const.Type->Type.VIn=yyt->Const.VIn;
yyVisit1 (yyt->Const.Type);
yyt->Const.VOut=yyt->Const.Type->Type.VOut;
/* line 858 "" */
 yyt->Const.SynType = InheritLhsType(yyt->Const.Type,yyt->Const.InhType);
/* line 859 "" */
if (! ( (!Tree_IsType(yyt->Const.Type,kUntypedReal) || Tree_IsType(yyt->Const.SynType,kReal)) )) { Message("Implied type of real constant not a real data type",xxError,yyt->Const.Pos); }
/* line 1051 "" */
 yyt->Const.C = true;
/* line 1051 "" */
 
     if (Tree_IsType(yyt->Const.Type,kUntypedInteger)) {
       yyt->Const.CV = CheckConst(yyt->Const.Pos,yyt->Const.SynType,yyt->Const.Value.Number,Tree_IsType(yyt->Const.Type,kUntypedHex));
     } else {
       yyt->Const.CV = yyt->Const.Value;
     } 
  
break;

case kValof:
yyt->Valof.SpecList->SpecList.VS=yyt->Valof.VS;
yyt->Valof.SpecList->SpecList.VIn=yyt->Valof.VIn;
yyVisit1 (yyt->Valof.SpecList);
yyt->Valof.Process->Process.VIn=yyt->Valof.SpecList->SpecList.VOut;
/* line 947 "" */
InheritFromType(yyt->Valof.InhType,yyt->Valof.ExprList);
/* line 1026 "" */
 yyt->Valof.Process->Process.VS = yyt->Valof.VS;
yyVisit1 (yyt->Valof.Process);
yyt->Valof.ExprList->ExprList.VIn=yyt->Valof.Process->Process.VOut;
yyVisit1 (yyt->Valof.ExprList);
yyt->Valof.VOut=yyt->Valof.ExprList->ExprList.VOut;
/* line 1307 "" */
if (! ( (yyt->Valof.Process->Process.ATOMIC) )) { Message("Function process must be atomic",xxError,yyt->Valof.Pos); }
/* line 862 "" */
 yyt->Valof.SynType = DEP(TypeExprList(yyt->Valof.ExprList),yyt->Valof.ExprList->ExprList.SynType);
/* line 1059 "" */
 yyt->Valof.C = yyt->Valof.ExprList->ExprList.C && (CountExprs(yyt->Valof.ExprList)==1);
/* line 1060 "" */
 if (yyt->Valof.C) yyt->Valof.CV=yyt->Valof.ExprList->AnExpr.Expr->Expr.CV; 
break;

case kFuncCall:
/* line 946 "" */
InheritFromFormals(SymbolKind(yyt->FuncCall.Sym),yyt->FuncCall.ExprList);
yyt->FuncCall.ExprList->ExprList.VIn=yyt->FuncCall.VIn;
yyVisit1 (yyt->FuncCall.ExprList);
yyt->FuncCall.VOut=yyt->FuncCall.ExprList->ExprList.VOut;
/* line 863 "" */
if (! ( Tree_IsType(SymbolKind(yyt->FuncCall.Sym),kDefAFunc) )) { Message("Not declared as a Function",xxError,yyt->FuncCall.Pos) ; }
/* line 864 "" */
if (! ( DEP(FALSE,yyt->FuncCall.ExprList->ExprList.SynType) )) { TypeCheckParameters(SymbolKind(yyt->FuncCall.Sym)->DefAFunc.Formals,yyt->FuncCall.ExprList,yyt->FuncCall.Pos,0); }
/* line 865 "" */
if (! ( (yyt->FuncCall.Depth>0) )) { Message("Only inline functions can be called at outermost scope",xxError,yyt->FuncCall.Pos) ; }
/* line 1239 "" */
if (! ( DEP(FALSE,SpecList) )) { SymbolKind(yyt->FuncCall.Sym)->DefAFunc.U=xxUsed; ; }
/* line 866 "" */

	       yyt->FuncCall.SynType = StripTypeList(SymbolType(yyt->FuncCall.Sym));
/* line 1049 "" */
 yyt->FuncCall.C = false;
/* line 1049 "" */
 yyt->FuncCall.CV.Number = 0; 
break;

case kElementalExpr:
yyt->ElementalExpr.Elemental->Elemental.VIn=yyt->ElementalExpr.VIn;
/* line 1214 "" */
yyt->ElementalExpr.Elemental->Elemental.U = xxUsed;
/* line 970 "" */
 yyt->ElementalExpr.Elemental=TransformUserLiterals(yyt->ElementalExpr.InhType,yyt->ElementalExpr.Elemental); yyt->ElementalExpr.Elemental->Elemental.InhType = yyt->ElementalExpr.InhType;
yyVisit1 (yyt->ElementalExpr.Elemental);
yyt->ElementalExpr.VOut=yyt->ElementalExpr.Elemental->Elemental.VOut;
/* line 1198 "" */
if (! ( DEP(FALSE,yyt->ElementalExpr.Elemental->Elemental.SynType) )) { yyt->ElementalExpr.Elemental = ReduceElemental(yyt->ElementalExpr.Elemental);; }
/* line 868 "" */
 yyt->ElementalExpr.SynType = yyt->ElementalExpr.Elemental->Elemental.SynType;
/* line 1082 "" */
 yyt->ElementalExpr.C = yyt->ElementalExpr.Elemental->Elemental.C;
/* line 1082 "" */
 yyt->ElementalExpr.CV = yyt->ElementalExpr.Elemental->Elemental.CV;
break;

case kDefault:
yyt->Default.VOut=yyt->Default.VIn;
/* line 795 "" */
 yyt->Default.SynType = NoTree;
/* line 1049 "" */
 yyt->Default.C = false;
/* line 1049 "" */
 yyt->Default.CV.Number = 0; 
break;

case kConvert:
yyt->Convert.Type->Type.VIn=yyt->Convert.VIn;
yyVisit1 (yyt->Convert.Type);
yyt->Convert.Expr->Expr.VIn=yyt->Convert.Type->Type.VOut;
/* line 981 "" */
 yyt->Convert.Expr->Expr.InhType = DefaultType(yyt->Convert.Expr);
yyVisit1 (yyt->Convert.Expr);
yyt->Convert.VOut=yyt->Convert.Expr->Expr.VOut;
/* line 879 "" */
if (! ( !(Tree_IsType(yyt->Convert.Type,kReal64)&&Tree_IsType(yyt->Convert.Expr->Expr.SynType,kReal)) )) {
	      ; }
else
/* line 880 "" */
if (! ( !(Tree_IsType(yyt->Convert.Type,kReal32)&&Tree_IsType(yyt->Convert.Expr->Expr.SynType,kReal32)) )) {
	      ; }
else
/* line 881 "" */
if (! ( (Tree_IsType(yyt->Convert.Type,kPrimitive)&&(!Tree_IsType(yyt->Convert.Type,kReal))) )) { Message("Attempted conversion to type other than integer, boolean or byte",xxError,yyt->Convert.Pos)
	      ; }
else
/* line 882 "" */
if (! ( (Tree_IsType(yyt->Convert.Expr->Expr.SynType,kPrimitive)&&(!Tree_IsType(yyt->Convert.Expr->Expr.SynType,kReal))) )) { Message("Attempted conversion from type other than Integer, BOOL or BYTE",xxError,yyt->Convert.Expr->Expr.Pos); }
/* line 878 "" */
 yyt->Convert.SynType = yyt->Convert.Type;
/* line 1067 "" */
 yyt->Convert.C = yyt->Convert.Expr->Expr.C && CheckConstConvert(yyt->Convert.Type);
/* line 1068 "" */
 if (yyt->Convert.C) yyt->Convert.CV=EvalConstConvert(yyt->Convert.Pos,yyt->Convert.Type,yyt->Convert.Expr); else yyt->Convert.CV.Number=0;
break;

case kRound:
yyt->Round.Type->Type.VIn=yyt->Round.VIn;
yyVisit1 (yyt->Round.Type);
yyt->Round.Expr->Expr.VIn=yyt->Round.Type->Type.VOut;
/* line 982 "" */
 yyt->Round.Expr->Expr.InhType = DefaultType(yyt->Round.Expr);
yyVisit1 (yyt->Round.Expr);
yyt->Round.VOut=yyt->Round.Expr->Expr.VOut;
/* line 871 "" */
if (! ( IsComputableType(yyt->Round.Type)||Tree_IsType(yyt->Round.Type,kReal) )) { Message("Attempted ROUND to non-numeric type",xxError,yyt->Round.Pos) 
	    ; }
else
/* line 872 "" */
if (! ( IsComputableType(yyt->Round.Expr->Expr.SynType)||Tree_IsType(yyt->Round.Expr->Expr.SynType,kReal) )) { Message("Attempted ROUND of non-numeric type",xxError,yyt->Round.Expr->Expr.Pos)
	    ; }
else
/* line 873 "" */
if (! ( Tree_IsType(yyt->Round.Type,kReal)||Tree_IsType(yyt->Round.Expr->Expr.SynType,kReal) )) { Message("Attempted ROUND without involving real type",xxError,yyt->Round.Pos); }
/* line 870 "" */
 yyt->Round.SynType = yyt->Round.Type;
/* line 1070 "" */
 yyt->Round.C = yyt->Round.Expr->Expr.C && CheckConstRoundTrunc(yyt->Round.Type,ExprType(yyt->Round.Expr));
/* line 1071 "" */
 if (yyt->Round.C) yyt->Round.CV=EvalConstRound(yyt->Round.Pos,yyt->Round.Type,ExprType(yyt->Round.Expr),yyt->Round.Expr->Expr.CV); else yyt->Round.CV.Number=0;
break;

case kTrunc:
yyt->Trunc.Type->Type.VIn=yyt->Trunc.VIn;
yyVisit1 (yyt->Trunc.Type);
yyt->Trunc.Expr->Expr.VIn=yyt->Trunc.Type->Type.VOut;
/* line 983 "" */
 yyt->Trunc.Expr->Expr.InhType = DefaultType(yyt->Trunc.Expr);
yyVisit1 (yyt->Trunc.Expr);
yyt->Trunc.VOut=yyt->Trunc.Expr->Expr.VOut;
/* line 875 "" */
if (! ( IsComputableType(yyt->Trunc.Type)||Tree_IsType(yyt->Trunc.Type,kReal) )) { Message("Attempted TRUNC to non-numeric type",xxError,yyt->Trunc.Pos) 
	    ; }
else
/* line 876 "" */
if (! ( IsComputableType(yyt->Trunc.Expr->Expr.SynType)||Tree_IsType(yyt->Trunc.Expr->Expr.SynType,kReal) )) { Message("Attempted TRUNC of non-numeric type",xxError,yyt->Trunc.Expr->Expr.Pos)
	    ; }
else
/* line 877 "" */
if (! ( Tree_IsType(yyt->Trunc.Type,kReal)||Tree_IsType(yyt->Trunc.Expr->Expr.SynType,kReal) )) { Message("Attempted TRUNC without involving real type",xxError,yyt->Trunc.Pos); }
/* line 874 "" */
 yyt->Trunc.SynType = yyt->Trunc.Type;
/* line 1073 "" */
 yyt->Trunc.C = yyt->Trunc.Expr->Expr.C && CheckConstRoundTrunc(yyt->Trunc.Type,ExprType(yyt->Trunc.Expr));
/* line 1074 "" */
 if (yyt->Trunc.C) yyt->Trunc.CV=EvalConstTrunc(yyt->Trunc.Pos,yyt->Trunc.Type,ExprType(yyt->Trunc.Expr),yyt->Trunc.Expr->Expr.CV); else yyt->Trunc.CV.Number=0;
break;

case kMostPos:
yyt->MostPos.Type->Type.VIn=yyt->MostPos.VIn;
yyVisit1 (yyt->MostPos.Type);
yyt->MostPos.VOut=yyt->MostPos.Type->Type.VOut;
/* line 884 "" */
if (! ( IsComputableType(yyt->MostPos.Type) )) { Message("MOSTPOS can only be applied to integer types",xxError,yyt->MostPos.Pos); }
/* line 884 "" */
 yyt->MostPos.SynType = yyt->MostPos.Type;
/* line 1061 "" */
 yyt->MostPos.C = IsComputableType(yyt->MostPos.Type);
/* line 1062 "" */
 if (yyt->MostPos.C) yyt->MostPos.CV.Number = EvalMostPos(yyt->MostPos.Type); 
break;

case kMostNeg:
yyt->MostNeg.Type->Type.VIn=yyt->MostNeg.VIn;
yyVisit1 (yyt->MostNeg.Type);
yyt->MostNeg.VOut=yyt->MostNeg.Type->Type.VOut;
/* line 885 "" */
if (! ( IsComputableType(yyt->MostNeg.Type) )) { Message("MOSTNEG can only be applied to integer types",xxError,yyt->MostNeg.Pos); }
/* line 885 "" */
 yyt->MostNeg.SynType = yyt->MostNeg.Type;
/* line 1064 "" */
 yyt->MostNeg.C = IsComputableType(yyt->MostNeg.Type);
/* line 1065 "" */
 if (yyt->MostNeg.C) yyt->MostNeg.CV.Number = EvalMostNeg(yyt->MostNeg.Type); 
break;

case kCountArray:
yyt->CountArray.Length->Expr.VIn=yyt->CountArray.VIn;
/* line 988 "" */

yyVisit1 (yyt->CountArray.Length);
yyt->CountArray.Expr->Expr.VIn=yyt->CountArray.Length->Expr.VOut;
/* line 988 "" */
 yyt->CountArray.Expr->Expr.InhType = yyt->CountArray.InhType;
yyVisit1 (yyt->CountArray.Expr);
yyt->CountArray.VOut=yyt->CountArray.Expr->Expr.VOut;
/* line 795 "" */
 yyt->CountArray.SynType = NoTree;
/* line 1049 "" */
 yyt->CountArray.C = false;
/* line 1049 "" */
 yyt->CountArray.CV.Number = 0; 
break;

case kUnBound:
yyt->UnBound.VOut=yyt->UnBound.VIn;
/* line 795 "" */
 yyt->UnBound.SynType = NoTree;
/* line 1049 "" */
 yyt->UnBound.C = false;
/* line 1049 "" */
 yyt->UnBound.CV.Number = 0; 
break;

case kBytesIn:
yyt->BytesIn.VOut=yyt->BytesIn.VIn;
/* line 886 "" */
 yyt->BytesIn.SynType = mInt();
/* line 1050 "" */
 yyt->BytesIn.C= false;
/* line 1050 "" */
 yyt->BytesIn.CV.Number = 0; 
break;

case kBytesInType:
yyt->BytesInType.Type->Type.VIn=yyt->BytesInType.VIn;
yyVisit1 (yyt->BytesInType.Type);
yyt->BytesInType.VOut=yyt->BytesInType.Type->Type.VOut;
/* line 886 "" */
 yyt->BytesInType.SynType = mInt();
/* line 1084 "" */
 yyt->BytesInType.C = CheckValidDeclType(yyt->BytesInType.Type);
/* line 1085 "" */
 if (yyt->BytesInType.C) yyt->BytesInType.CV.Number = CalcTypeSize(yyt->BytesInType.Type);
break;

case kBytesInExpr:
yyt->BytesInExpr.Expr->Expr.VIn=yyt->BytesInExpr.VIn;
yyt->BytesInExpr.Expr->Expr.InhType=yyt->BytesInExpr.InhType;
yyVisit1 (yyt->BytesInExpr.Expr);
yyt->BytesInExpr.VOut=yyt->BytesInExpr.Expr->Expr.VOut;
/* line 886 "" */
 yyt->BytesInExpr.SynType = mInt();
/* line 1087 "" */
 yyt->BytesInExpr.C = CheckValidDeclType(yyt->BytesInExpr.Expr->Expr.SynType);
/* line 1088 "" */
 if (yyt->BytesInExpr.C) yyt->BytesInExpr.CV.Number = CalcTypeSize(yyt->BytesInExpr.Expr->Expr.SynType);
break;

case kSizeOf:
yyt->SizeOf.Type->Type.VIn=yyt->SizeOf.VIn;
yyVisit1 (yyt->SizeOf.Type);
yyt->SizeOf.VOut=yyt->SizeOf.Type->Type.VOut;
/* line 889 "" */
if (! ( IsArrayType(yyt->SizeOf.Type) )) { Message("Can only apply SIZE operator to array types",xxError,yyt->SizeOf.Pos); }
/* line 889 "" */
 yyt->SizeOf.SynType = mInt();
/* line 1090 "" */
 yyt->SizeOf.C = (IsArrayType(yyt->SizeOf.Type) && IsConstExpr(yyt->SizeOf.Type->Array.Expr));
/* line 1091 "" */
 if (yyt->SizeOf.C) yyt->SizeOf.CV.Number = ValConstInt(yyt->SizeOf.Type->Array.Expr); else yyt->SizeOf.CV.Number=0;
break;

case kOffsetOf:
yyt->OffsetOf.Type->Type.VIn=yyt->OffsetOf.VIn;
yyVisit1 (yyt->OffsetOf.Type);
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.VIn=yyt->OffsetOf.Type->Type.VOut;
/* line 1211 "" */
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.U = xxUsed;
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.InhType=yyt->OffsetOf.InhType;
yyVisit1 (yyt->OffsetOf.UnmappedIdentifier);
yyt->OffsetOf.VOut=yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.VOut;
/* line 888 "" */
if (! ( (yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.Sym!=NoSymbol) )) { Message("Invalid parameters to OFFSETOF operator",xxError,yyt->OffsetOf.Pos); }
/* line 887 "" */
 yyt->OffsetOf.SynType = mInt();
/* line 1093 "" */
 yyt->OffsetOf.C = (yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.Sym!=NoSymbol);
/* line 1094 "" */
 if (yyt->OffsetOf.C) yyt->OffsetOf.CV.Number = CalcOffset(yyt->OffsetOf.Type,SymbolKind(yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.Sym)); else yyt->OffsetOf.CV.Number=0;
break;

case kStringElement:
yyt->StringElement.VOut=yyt->StringElement.VIn;
/* line 802 "" */
 yyt->StringElement.SynType = NoTree;
/* line 1128 "" */
 yyt->StringElement.C = false;
/* line 1178 "" */
 yyt->StringElement.CA = NoConstArray;
/* line 1049 "" */
 yyt->StringElement.CV.Number = 0; 
break;

case kString:
yyt->String.Type->Type.VIn=yyt->String.VIn;
yyVisit1 (yyt->String.Type);
yyt->String.VOut=yyt->String.Type->Type.VOut;
/* line 846 "" */
 yyt->String.SynType = TypeString(LengthSt(yyt->String.Value),InheritLhsType(yyt->String.Type,yyt->String.InhType),yyt->String.Pos);
/* line 1130 "" */
 yyt->String.C = true;
/* line 1180 "" */
 if (yyt->String.C) yyt->String.CA = ConstArrayFromString(yyt->String.SynType,yyt->String.Value); else yyt->String.CA = NoConstArray; 
/* line 1131 "" */
 yyt->String.CV.Number = 0; 
break;

case kStringSubscript:
yyt->StringSubscript.Next->StringElement.VIn=yyt->StringSubscript.VIn;
yyt->StringSubscript.Next->StringElement.InhType=yyt->StringSubscript.InhType;
yyVisit1 (yyt->StringSubscript.Next);
yyt->StringSubscript.Expr->Expr.VIn=yyt->StringSubscript.Next->StringElement.VOut;
/* line 989 "" */
 yyt->StringSubscript.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->StringSubscript.Expr);
yyt->StringSubscript.VOut=yyt->StringSubscript.Expr->Expr.VOut;
/* line 848 "" */
 yyt->StringSubscript.SynType = TypeSubscript(yyt->StringSubscript.Expr,yyt->StringSubscript.Next->StringElement.SynType,NoTree,yyt->StringSubscript.Pos);
/* line 1133 "" */
 yyt->StringSubscript.C = yyt->StringSubscript.Next->StringElement.C && yyt->StringSubscript.Expr->Expr.C;
/* line 1182 "" */
 yyt->StringSubscript.CA = NoConstArray;
/* line 1134 "" */
 if (yyt->StringSubscript.C && !IsArrayType(yyt->StringSubscript.SynType)) yyt->StringSubscript.CV = EvalConstSubscript(yyt->StringSubscript.SynType,ValConstInt(yyt->StringSubscript.Expr),yyt->StringSubscript.Next->StringElement.CA); 
break;

case kStringRange:
yyt->StringRange.Next->StringElement.VIn=yyt->StringRange.VIn;
yyt->StringRange.Next->StringElement.InhType=yyt->StringRange.InhType;
yyVisit1 (yyt->StringRange.Next);
yyt->StringRange.From->Expr.VIn=yyt->StringRange.Next->StringElement.VOut;
/* line 990 "" */
 yyt->StringRange.From->Expr.InhType = mInt();
yyVisit1 (yyt->StringRange.From);
yyt->StringRange.For->Expr.VIn=yyt->StringRange.From->Expr.VOut;
/* line 990 "" */
 yyt->StringRange.For->Expr.InhType = mInt();
yyVisit1 (yyt->StringRange.For);
yyt->StringRange.VOut=yyt->StringRange.For->Expr.VOut;
/* line 849 "" */
if (! ( Tree_IsType(yyt->StringRange.From->Expr.SynType,kInt) )) { Message("String range FROM expression not of type INT",xxError,yyt->StringRange.Pos); }
/* line 850 "" */
if (! ( Tree_IsType(yyt->StringRange.For->Expr.SynType,kInt) )) { Message("String range FOR expression not of type INT",xxError,yyt->StringRange.Pos); }
/* line 851 "" */

		  yyt->StringRange.SynType = TypeRange(yyt->StringRange.From,yyt->StringRange.For,yyt->StringRange.Next->StringElement.SynType,yyt->StringRange.Pos);
/* line 1136 "" */
 yyt->StringRange.C = yyt->StringRange.Next->StringElement.C && yyt->StringRange.From->Expr.C && yyt->StringRange.For->Expr.C;
/* line 1184 "" */
 if (yyt->StringRange.C) yyt->StringRange.CA = RangeConstArray(yyt->StringRange.SynType,yyt->StringRange.Next->StringElement.CA,ValConstInt(yyt->StringRange.From)); else yyt->StringRange.CA = NoConstArray; 
/* line 1049 "" */
 yyt->StringRange.CV.Number = 0; 
break;

case kElemental:
yyt->Elemental.VOut=yyt->Elemental.VIn;
/* line 1097 "" */
 yyt->Elemental.C = false;
/* line 799 "" */
 yyt->Elemental.SynType = NoTree;
/* line 1151 "" */
 yyt->Elemental.CA = NoConstArray;
/* line 1097 "" */
 yyt->Elemental.CV.Number = 0; 
/* line 1005 "" */
 yyt->Elemental.W = true;
break;

case kTableElement:
yyt->TableElement.VOut=yyt->TableElement.VIn;
/* line 1113 "" */
 yyt->TableElement.C = false;
/* line 801 "" */
 yyt->TableElement.SynType = NoTree;
/* line 1165 "" */
 yyt->TableElement.CA = NoConstArray;
/* line 1097 "" */
 yyt->TableElement.CV.Number = 0; 
/* line 1012 "" */
 yyt->TableElement.W = true;
break;

case kTable:
/* line 952 "" */
InheritFromTable(yyt->Table.InhType,yyt->Table.ExprList);
yyt->Table.ExprList->ExprList.VIn=yyt->Table.VIn;
yyVisit1 (yyt->Table.ExprList);
yyt->Table.VOut=yyt->Table.ExprList->ExprList.VOut;
/* line 1115 "" */
 yyt->Table.C = yyt->Table.ExprList->ExprList.C;
/* line 838 "" */
 yyt->Table.SynType = TypeTable(yyt->Table.ExprList,yyt->Table.ExprList->ExprList.SynType,yyt->Table.InhType);
/* line 1167 "" */
 if (yyt->Table.C) yyt->Table.CA = ConstArrayFromExprList(yyt->Table.SynType,yyt->Table.ExprList); else yyt->Table.CA = NoConstArray; 
/* line 1097 "" */
 yyt->Table.CV.Number = 0; 
/* line 1013 "" */
 yyt->Table.W = CheckWritableExprList(yyt->Table.ExprList);
break;

case kUserLiteral:
/* line 952 "" */
InheritFromTable(yyt->UserLiteral.InhType,yyt->UserLiteral.ExprList);
yyt->UserLiteral.ExprList->ExprList.VIn=yyt->UserLiteral.VIn;
yyVisit1 (yyt->UserLiteral.ExprList);
yyt->UserLiteral.Type->Type.VIn=yyt->UserLiteral.ExprList->ExprList.VOut;
yyVisit1 (yyt->UserLiteral.Type);
yyt->UserLiteral.VOut=yyt->UserLiteral.Type->Type.VOut;
/* line 1115 "" */
 yyt->UserLiteral.C = yyt->UserLiteral.ExprList->ExprList.C;
/* line 860 "" */
 yyt->UserLiteral.SynType = yyt->UserLiteral.Type;
/* line 1167 "" */
 if (yyt->UserLiteral.C) yyt->UserLiteral.CA = ConstArrayFromExprList(yyt->UserLiteral.SynType,yyt->UserLiteral.ExprList); else yyt->UserLiteral.CA = NoConstArray; 
/* line 1097 "" */
 yyt->UserLiteral.CV.Number = 0; 
/* line 1013 "" */
 yyt->UserLiteral.W = CheckWritableExprList(yyt->UserLiteral.ExprList);
break;

case kTableExpr:
yyt->TableExpr.Expr->Expr.VIn=yyt->TableExpr.VIn;
/* line 951 "" */
 yyt->TableExpr.Expr->Expr.InhType = yyt->TableExpr.InhType;
yyVisit1 (yyt->TableExpr.Expr);
yyt->TableExpr.VOut=yyt->TableExpr.Expr->Expr.VOut;
/* line 1116 "" */
 yyt->TableExpr.C = yyt->TableExpr.Expr->Expr.C;
/* line 839 "" */
 yyt->TableExpr.SynType = yyt->TableExpr.Expr->Expr.SynType;
/* line 1168 "" */
 if (yyt->TableExpr.C) Message("Constant TableExpr not implemented",xxError, yyt->TableExpr.Pos); 
/* line 1097 "" */
 yyt->TableExpr.CV.Number = 0; 
/* line 1014 "" */
 yyt->TableExpr.W = false;
break;

case kTableSubscript:
yyt->TableSubscript.Next->TableElement.VIn=yyt->TableSubscript.VIn;
yyt->TableSubscript.Next->TableElement.U=yyt->TableSubscript.U;
yyt->TableSubscript.Next->TableElement.InhType=yyt->TableSubscript.InhType;
yyVisit1 (yyt->TableSubscript.Next);
yyt->TableSubscript.Expr->Expr.VIn=yyt->TableSubscript.Next->TableElement.VOut;
/* line 991 "" */
 yyt->TableSubscript.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->TableSubscript.Expr);
yyt->TableSubscript.VOut=yyt->TableSubscript.Expr->Expr.VOut;
/* line 1118 "" */
 yyt->TableSubscript.C = yyt->TableSubscript.Next->TableElement.C && yyt->TableSubscript.Expr->Expr.C;
/* line 840 "" */
 yyt->TableSubscript.SynType = TypeSubscript(yyt->TableSubscript.Expr,yyt->TableSubscript.Next->TableElement.SynType,ExprFieldList(yyt->TableSubscript.Expr),yyt->TableSubscript.Expr->Expr.Pos);
/* line 1169 "" */
 if (yyt->TableSubscript.C) yyt->TableSubscript.CA = SubscriptConstArray(yyt->TableSubscript.SynType,yyt->TableSubscript.Next->TableElement.CA,yyt->TableSubscript.Expr,yyt->TableSubscript.Next->TableElement.SynType); else yyt->TableSubscript.CA = NoConstArray; 
/* line 1119 "" */
 if (yyt->TableSubscript.C && !IsArrayType(yyt->TableSubscript.SynType)) yyt->TableSubscript.CV = EvalConstSubscript(yyt->TableSubscript.SynType,ValConstInt(yyt->TableSubscript.Expr),yyt->TableSubscript.Next->TableElement.CA); 
/* line 1015 "" */
 yyt->TableSubscript.W = yyt->TableSubscript.Next->TableElement.W;
break;

case kTableRange:
yyt->TableRange.Next->TableElement.VIn=yyt->TableRange.VIn;
yyt->TableRange.Next->TableElement.U=yyt->TableRange.U;
yyt->TableRange.Next->TableElement.InhType=yyt->TableRange.InhType;
yyVisit1 (yyt->TableRange.Next);
yyt->TableRange.From->Expr.VIn=yyt->TableRange.Next->TableElement.VOut;
/* line 992 "" */
 yyt->TableRange.From->Expr.InhType = mInt();
yyVisit1 (yyt->TableRange.From);
yyt->TableRange.For->Expr.VIn=yyt->TableRange.From->Expr.VOut;
/* line 992 "" */
 yyt->TableRange.For->Expr.InhType = mInt();
yyVisit1 (yyt->TableRange.For);
yyt->TableRange.VOut=yyt->TableRange.For->Expr.VOut;
/* line 842 "" */
if (! ( Tree_IsType(yyt->TableRange.From->Expr.SynType,kInt) )) { Message("Table range FROM expression not of type INT",xxError,yyt->TableRange.Pos); }
/* line 843 "" */
if (! ( Tree_IsType(yyt->TableRange.For->Expr.SynType,kInt) )) { Message("Table range FOR expression not of type INT",xxError,yyt->TableRange.Pos); }
/* line 1121 "" */
 yyt->TableRange.C = yyt->TableRange.Next->TableElement.C && yyt->TableRange.From->Expr.C && yyt->TableRange.For->Expr.C;
/* line 844 "" */

		 yyt->TableRange.SynType = TypeRange(yyt->TableRange.From,yyt->TableRange.For,yyt->TableRange.Next->TableElement.SynType,yyt->TableRange.Pos);
/* line 1171 "" */
 if (yyt->TableRange.C) yyt->TableRange.CA = RangeConstArray(yyt->TableRange.SynType,yyt->TableRange.Next->TableElement.CA,ValConstInt(yyt->TableRange.From)); else yyt->TableRange.CA = NoConstArray; 
/* line 1097 "" */
 yyt->TableRange.CV.Number = 0; 
/* line 1016 "" */
 yyt->TableRange.W = yyt->TableRange.Next->TableElement.W;
break;

case kElement:
yyt->Element.VOut=yyt->Element.VIn;
/* line 1100 "" */
 yyt->Element.C = false;
/* line 800 "" */
 yyt->Element.SynType = NoTree;
/* line 1154 "" */
 yyt->Element.CA = NoConstArray;
/* line 1100 "" */
 yyt->Element.CV.Number = 0; 
/* line 1007 "" */
 yyt->Element.W = true;
break;

case kIdentifier:
yyt->Identifier.VOut=yyt->Identifier.VIn;
/* line 1222 "" */
if (! ( DEP(FALSE,FLOut) )) { UpdateElementalUsage(SELF,yyt->Identifier.U); }
/* line 1102 "" */
 yyt->Identifier.C = CheckConstId(SymbolKind(yyt->Identifier.Sym));
/* line 828 "" */
 yyt->Identifier.SynType = ((Tree_IsType(SymbolKind(yyt->Identifier.Sym),kAField))?mInt():SymbolType(yyt->Identifier.Sym));
/* line 1156 "" */
 if (yyt->Identifier.C && !IsScalarType(yyt->Identifier.SynType)) yyt->Identifier.CA = ConstArrayFromDecl(yyt->Identifier.SynType,SymbolKind(yyt->Identifier.Sym)); else yyt->Identifier.CA = NoConstArray; 
/* line 1103 "" */
if (yyt->Identifier.C) yyt->Identifier.CV=EvalConstId(SymbolKind(yyt->Identifier.Sym)); else yyt->Identifier.CV.Number=0;
/* line 1008 "" */
 yyt->Identifier.W = CheckWritable(SymbolKind(yyt->Identifier.Sym));
break;

case kUnmappedIdentifier:
yyt->UnmappedIdentifier.VOut=yyt->UnmappedIdentifier.VIn;
/* line 1222 "" */
if (! ( DEP(FALSE,FLOut) )) { UpdateElementalUsage(SELF,yyt->UnmappedIdentifier.U); }
/* line 1102 "" */
 yyt->UnmappedIdentifier.C = CheckConstId(SymbolKind(yyt->UnmappedIdentifier.Sym));
/* line 828 "" */
 yyt->UnmappedIdentifier.SynType = ((Tree_IsType(SymbolKind(yyt->UnmappedIdentifier.Sym),kAField))?mInt():SymbolType(yyt->UnmappedIdentifier.Sym));
/* line 1156 "" */
 if (yyt->UnmappedIdentifier.C && !IsScalarType(yyt->UnmappedIdentifier.SynType)) yyt->UnmappedIdentifier.CA = ConstArrayFromDecl(yyt->UnmappedIdentifier.SynType,SymbolKind(yyt->UnmappedIdentifier.Sym)); else yyt->UnmappedIdentifier.CA = NoConstArray; 
/* line 1103 "" */
if (yyt->UnmappedIdentifier.C) yyt->UnmappedIdentifier.CV=EvalConstId(SymbolKind(yyt->UnmappedIdentifier.Sym)); else yyt->UnmappedIdentifier.CV.Number=0;
/* line 1008 "" */
 yyt->UnmappedIdentifier.W = CheckWritable(SymbolKind(yyt->UnmappedIdentifier.Sym));
break;

case kElementAccess:
yyt->ElementAccess.Next->Element.VIn=yyt->ElementAccess.VIn;
yyt->ElementAccess.Next->Element.U=yyt->ElementAccess.U;
yyt->ElementAccess.Next->Element.InhType=yyt->ElementAccess.InhType;
yyVisit1 (yyt->ElementAccess.Next);
yyt->ElementAccess.VOut=yyt->ElementAccess.Next->Element.VOut;
/* line 1100 "" */
 yyt->ElementAccess.C = false;
/* line 800 "" */
 yyt->ElementAccess.SynType = NoTree;
/* line 1154 "" */
 yyt->ElementAccess.CA = NoConstArray;
/* line 1100 "" */
 yyt->ElementAccess.CV.Number = 0; 
/* line 1007 "" */
 yyt->ElementAccess.W = true;
break;

case kSubscript:
yyt->Subscript.Next->Element.VIn=yyt->Subscript.VIn;
/* line 1220 "" */
 yyt->Subscript.Next->Element.U = yyt->Subscript.U;
yyt->Subscript.Next->Element.InhType=yyt->Subscript.InhType;
yyVisit1 (yyt->Subscript.Next);
yyt->Subscript.Expr->Expr.VIn=yyt->Subscript.Next->Element.VOut;
/* line 993 "" */
 yyt->Subscript.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->Subscript.Expr);
yyt->Subscript.VOut=yyt->Subscript.Expr->Expr.VOut;
/* line 830 "" */
 yyt->Subscript.CB = (ag_phase==0) ? (!yyt->Subscript.Expr->Expr.C) : yyt->Subscript.CB; 
/* line 829 "" */
if (! ( (ExprFieldList(yyt->Subscript.Expr)!=NoTree || Tree_IsType(yyt->Subscript.Expr->Expr.SynType,kInt)) )) { Message("Array subscript not of type INT",xxError,yyt->Subscript.Pos); }
/* line 1106 "" */
 yyt->Subscript.C = yyt->Subscript.Next->Element.C && yyt->Subscript.Expr->Expr.C;
/* line 831 "" */
  
		yyt->Subscript.SynType = TypeSubscript(yyt->Subscript.Expr,yyt->Subscript.Next->Element.SynType,ExprFieldList(yyt->Subscript.Expr),yyt->Subscript.Pos);
/* line 1158 "" */
 if (yyt->Subscript.C && !IsScalarType(yyt->Subscript.SynType)) yyt->Subscript.CA = SubscriptConstArray(yyt->Subscript.SynType,yyt->Subscript.Next->Element.CA,yyt->Subscript.Expr,yyt->Subscript.Next->Element.SynType); else yyt->Subscript.CA = NoConstArray; 
/* line 1107 "" */
 if (yyt->Subscript.C && !IsArrayType(yyt->Subscript.SynType)) yyt->Subscript.CV = EvalConstSubscript(yyt->Subscript.SynType,ValConstInt(yyt->Subscript.Expr),yyt->Subscript.Next->Element.CA); 
/* line 1009 "" */
 yyt->Subscript.W = yyt->Subscript.Next->Element.W;
break;

case kRange:
yyt->Range.Next->Element.VIn=yyt->Range.VIn;
/* line 1221 "" */
 yyt->Range.Next->Element.U = yyt->Range.U;
yyt->Range.Next->Element.InhType=yyt->Range.InhType;
yyVisit1 (yyt->Range.Next);
yyt->Range.From->Expr.VIn=yyt->Range.Next->Element.VOut;
/* line 994 "" */
 yyt->Range.From->Expr.InhType = mInt();
yyVisit1 (yyt->Range.From);
yyt->Range.For->Expr.VIn=yyt->Range.From->Expr.VOut;
/* line 994 "" */
 yyt->Range.For->Expr.InhType = mInt();
yyVisit1 (yyt->Range.For);
yyt->Range.VOut=yyt->Range.For->Expr.VOut;
/* line 835 "" */
 yyt->Range.CUB = (ag_phase==0) ? (!(yyt->Range.From->Expr.C&&yyt->Range.For->Expr.C)) : yyt->Range.CUB; 
/* line 834 "" */
 yyt->Range.CLB = (ag_phase==0) ? (!yyt->Range.From->Expr.C) : yyt->Range.CLB; 
/* line 832 "" */
if (! ( Tree_IsType(yyt->Range.From->Expr.SynType,kInt) )) { Message("Range FROM expression not of type INT",xxError,yyt->Range.Pos); }
/* line 833 "" */
if (! ( Tree_IsType(yyt->Range.For->Expr.SynType,kInt) )) { Message("Range FOR expression not of type INT",xxError,yyt->Range.Pos); }
/* line 1109 "" */
 yyt->Range.C = yyt->Range.Next->Element.C && yyt->Range.From->Expr.C && yyt->Range.For->Expr.C;
/* line 836 "" */
  
	    yyt->Range.SynType = DEP(TypeRange(yyt->Range.From,yyt->Range.For,yyt->Range.Next->Element.SynType,yyt->Range.Pos),yyt->Range.For->Expr.C);
/* line 1160 "" */
 if (yyt->Range.C) yyt->Range.CA = RangeConstArray(yyt->Range.SynType,yyt->Range.Next->Element.CA,ValConstInt(yyt->Range.From)); else yyt->Range.CA = NoConstArray; 
/* line 1100 "" */
 yyt->Range.CV.Number = 0; 
/* line 1010 "" */
 yyt->Range.W = yyt->Range.Next->Element.W;
break;

case kExprList:
yyt->ExprList.VOut=yyt->ExprList.VIn;
/* line 1249 "" */
 yyt->ExprList.TR = false;
/* line 1139 "" */
 yyt->ExprList.C = true;
/* line 796 "" */
 yyt->ExprList.SynType = NoTree;
break;

case kNoExpr:
yyt->NoExpr.VOut=yyt->NoExpr.VIn;
/* line 1250 "" */
 yyt->NoExpr.TR = false;
/* line 1141 "" */
 yyt->NoExpr.C = true;
/* line 796 "" */
 yyt->NoExpr.SynType = NoTree;
break;

case kAnExpr:
yyt->AnExpr.Next->ExprList.InhType=yyt->AnExpr.InhType;
yyt->AnExpr.Next->ExprList.VIn=yyt->AnExpr.VIn;
yyVisit1 (yyt->AnExpr.Next);
yyt->AnExpr.Expr->Expr.VIn=yyt->AnExpr.Next->ExprList.VOut;
/* line 943 "" */

yyVisit1 (yyt->AnExpr.Expr);
yyt->AnExpr.VOut=yyt->AnExpr.Expr->Expr.VOut;
/* line 1251 "" */
 yyt->AnExpr.TR = (ag_phase==0)? !(Tree_IsType(yyt->AnExpr.Next,kNoExpr)||yyt->AnExpr.Expr->Expr.C) : yyt->AnExpr.TR;
/* line 1140 "" */
 yyt->AnExpr.C = yyt->AnExpr.Next->ExprList.C && yyt->AnExpr.Expr->Expr.C;
/* line 798 "" */
 yyt->AnExpr.SynType = yyt->AnExpr.Expr->Expr.SynType;
break;

case kOp:
break;

case kRelOp:
break;

case kLess:
break;

case kGreater:
break;

case kLessEqual:
break;

case kGreaterEqual:
break;

case kEqual:
break;

case kNotEqual:
break;

case kMathsOp:
break;

case kAdd:
break;

case kSubtract:
break;

case kMultiply:
break;

case kDivide:
break;

case kRem:
break;

case kBitOp:
break;

case kBitAnd:
break;

case kBitOr:
break;

case kBitEor:
break;

case kShiftOp:
break;

case kLeftShift:
break;

case kRightShift:
break;

case kUniOp:
break;

case kSize:
break;

case kBitNot:
break;

case kNot:
break;

case kClockOp:
break;

case kPlus:
break;

case kMinus:
break;

case kTimes:
break;

case kLogOp:
break;

case kAnd:
break;

case kOr:
break;

case kAfterOp:
break;

case kProcess:
yyt->Process.SpecList->SpecList.VS=yyt->Process.VS;
yyt->Process.SpecList->SpecList.VIn=yyt->Process.VIn;
yyVisit1 (yyt->Process.SpecList);
yyt->Process.VOut=yyt->Process.SpecList->SpecList.VOut;
/* line 1263 "" */
 yyt->Process.ATOMIC = false;
break;

case kAssign:
yyt->Assign.SpecList->SpecList.VS=yyt->Assign.VS;
/* line 958 "" */
InheritFromType(NoTree,yyt->Assign.Lval);
yyt->Assign.SpecList->SpecList.VIn=yyt->Assign.VIn;
yyVisit1 (yyt->Assign.SpecList);
yyt->Assign.Lval->ExprList.VIn=yyt->Assign.SpecList->SpecList.VOut;
yyVisit1 (yyt->Assign.Lval);
/* line 959 "" */
InheritFromType(TypeExprList(DEP(yyt->Assign.Lval,yyt->Assign.Lval->ExprList.SynType)),yyt->Assign.Rval);
yyt->Assign.Rval->ExprList.VIn=yyt->Assign.Lval->ExprList.VOut;
yyVisit1 (yyt->Assign.Rval);
yyt->Assign.VOut=yyt->Assign.Rval->ExprList.VOut;
/* line 893 "" */
if (! ( CheckValofScope(yyt->Assign.Lval,yyt->Assign.VS) )) { Message("Free variable modified within VALOF",xxError,yyt->Assign.Pos); }
/* line 894 "" */
if (! ( FALSE )) { DEP(DEP(TypeCheckAssign(yyt->Assign.Lval,yyt->Assign.Rval,yyt->Assign.Pos),yyt->Assign.Lval->ExprList.SynType),yyt->Assign.Rval->ExprList.SynType); }
/* line 1268 "" */
 yyt->Assign.ATOMIC = true;
break;

case kAltType:
yyt->AltType.SpecList->SpecList.VS=yyt->AltType.VS;
yyt->AltType.SpecList->SpecList.VIn=yyt->AltType.VIn;
yyVisit1 (yyt->AltType.SpecList);
yyt->AltType.VOut=yyt->AltType.SpecList->SpecList.VOut;
/* line 1266 "" */
 yyt->AltType.ATOMIC = false;
break;

case kAlt:
yyt->Alt.SpecList->SpecList.VS=yyt->Alt.VS;
yyt->Alt.AltList->AltList.VS=yyt->Alt.VS;
yyt->Alt.SpecList->SpecList.VIn=yyt->Alt.VIn;
yyVisit1 (yyt->Alt.SpecList);
yyt->Alt.AltList->AltList.VIn=yyt->Alt.SpecList->SpecList.VOut;
yyVisit1 (yyt->Alt.AltList);
yyt->Alt.VOut=yyt->Alt.AltList->AltList.VOut;
/* line 1270 "" */
 yyt->Alt.ATOMIC = false;
break;

case kAltRep:
yyt->AltRep.SpecList->SpecList.VS=yyt->AltRep.VS;
yyt->AltRep.AltList->AltList.VS=yyt->AltRep.VS;
yyt->AltRep.SpecList->SpecList.VIn=yyt->AltRep.VIn;
yyVisit1 (yyt->AltRep.SpecList);
yyt->AltRep.Replicator->Replicator.VIn=yyt->AltRep.SpecList->SpecList.VOut;
yyVisit1 (yyt->AltRep.Replicator);
yyt->AltRep.AltList->AltList.VIn=yyt->AltRep.Replicator->Replicator.VOut;
yyVisit1 (yyt->AltRep.AltList);
yyt->AltRep.VOut=yyt->AltRep.AltList->AltList.VOut;
/* line 1271 "" */
 yyt->AltRep.ATOMIC = false;
break;

case kParType:
yyt->ParType.SpecList->SpecList.VS=yyt->ParType.VS;
yyt->ParType.SpecList->SpecList.VIn=yyt->ParType.VIn;
yyVisit1 (yyt->ParType.SpecList);
yyt->ParType.VOut=yyt->ParType.SpecList->SpecList.VOut;
/* line 924 "" */
if (! (((yyt->ParType.PType!=xxPlacedPar)||IS_Mode(xxSupportPlaced)) )) { Message("PLACED PAR not allowed",xxError,yyt->ParType.Pos); }
/* line 1263 "" */
 yyt->ParType.ATOMIC = false;
break;

case kPar:
yyt->Par.SpecList->SpecList.VS=yyt->Par.VS;
yyt->Par.ProcList->ProcList.VS=yyt->Par.VS;
yyt->Par.SpecList->SpecList.VIn=yyt->Par.VIn;
yyVisit1 (yyt->Par.SpecList);
yyt->Par.ProcList->ProcList.VIn=yyt->Par.SpecList->SpecList.VOut;
yyVisit1 (yyt->Par.ProcList);
yyt->Par.VOut=yyt->Par.ProcList->ProcList.VOut;
/* line 924 "" */
if (! (((yyt->Par.PType!=xxPlacedPar)||IS_Mode(xxSupportPlaced)) )) { Message("PLACED PAR not allowed",xxError,yyt->Par.Pos); }
/* line 1276 "" */
 yyt->Par.ATOMIC = false;
break;

case kParRep:
yyt->ParRep.SpecList->SpecList.VS=yyt->ParRep.VS;
yyt->ParRep.SpecList->SpecList.VIn=yyt->ParRep.VIn;
yyVisit1 (yyt->ParRep.SpecList);
yyt->ParRep.Replicator->Replicator.VIn=yyt->ParRep.SpecList->SpecList.VOut;
yyVisit1 (yyt->ParRep.Replicator);
yyt->ParRep.Process->Process.VIn=yyt->ParRep.Replicator->Replicator.VOut;
yyt->ParRep.Process->Process.VS=yyt->ParRep.VS;
yyVisit1 (yyt->ParRep.Process);
yyt->ParRep.VOut=yyt->ParRep.Process->Process.VOut;
/* line 924 "" */
if (! (((yyt->ParRep.PType!=xxPlacedPar)||IS_Mode(xxSupportPlaced)) )) { Message("PLACED PAR not allowed",xxError,yyt->ParRep.Pos); }
/* line 1277 "" */
 yyt->ParRep.ATOMIC = false;
break;

case kProcessor:
yyt->Processor.SpecList->SpecList.VS=yyt->Processor.VS;
yyt->Processor.SpecList->SpecList.VIn=yyt->Processor.VIn;
yyVisit1 (yyt->Processor.SpecList);
yyt->Processor.Expr->Expr.VIn=yyt->Processor.SpecList->SpecList.VOut;
/* line 975 "" */
 yyt->Processor.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->Processor.Expr);
yyt->Processor.Process->Process.VIn=yyt->Processor.Expr->Expr.VOut;
yyt->Processor.Process->Process.VS=yyt->Processor.VS;
yyVisit1 (yyt->Processor.Process);
yyt->Processor.VOut=yyt->Processor.Process->Process.VOut;
/* line 1263 "" */
 yyt->Processor.ATOMIC = false;
break;

case kIfType:
yyt->IfType.SpecList->SpecList.VS=yyt->IfType.VS;
yyt->IfType.SpecList->SpecList.VIn=yyt->IfType.VIn;
yyVisit1 (yyt->IfType.SpecList);
yyt->IfType.VOut=yyt->IfType.SpecList->SpecList.VOut;
/* line 1263 "" */
 yyt->IfType.ATOMIC = false;
break;

case kIf:
yyt->If.SpecList->SpecList.VS=yyt->If.VS;
yyt->If.ChoiceList->ChoiceList.VS=yyt->If.VS;
yyt->If.SpecList->SpecList.VIn=yyt->If.VIn;
yyVisit1 (yyt->If.SpecList);
yyt->If.ChoiceList->ChoiceList.VIn=yyt->If.SpecList->SpecList.VOut;
/* line 953 "" */
 yyt->If.ChoiceList->ChoiceList.InhType = NoTree;
yyVisit1 (yyt->If.ChoiceList);
yyt->If.VOut=yyt->If.ChoiceList->ChoiceList.VOut;
/* line 919 "" */
if (! ( DEP(FALSE,yyt->If.ChoiceList->ChoiceList.SynType) )) { TypeCheckIfList(yyt->If.ChoiceList); }
/* line 1281 "" */
 yyt->If.ATOMIC = yyt->If.ChoiceList->ChoiceList.ATOMIC;
/* line 1282 "" */
if (! ( (yyt->If.ATOMIC) )) { MarkNonAtomicIf(SELF); }
break;

case kIfRep:
yyt->IfRep.SpecList->SpecList.VS=yyt->IfRep.VS;
yyt->IfRep.ChoiceList->ChoiceList.VS=yyt->IfRep.VS;
yyt->IfRep.SpecList->SpecList.VIn=yyt->IfRep.VIn;
yyVisit1 (yyt->IfRep.SpecList);
yyt->IfRep.Replicator->Replicator.VIn=yyt->IfRep.SpecList->SpecList.VOut;
yyVisit1 (yyt->IfRep.Replicator);
yyt->IfRep.ChoiceList->ChoiceList.VIn=yyt->IfRep.Replicator->Replicator.VOut;
/* line 954 "" */
 yyt->IfRep.ChoiceList->ChoiceList.InhType = NoTree;
yyVisit1 (yyt->IfRep.ChoiceList);
yyt->IfRep.VOut=yyt->IfRep.ChoiceList->ChoiceList.VOut;
/* line 917 "" */
if (! ( DEP(FALSE,yyt->IfRep.ChoiceList->ChoiceList.SynType) )) { TypeCheckIfList(yyt->IfRep.ChoiceList); }
/* line 1284 "" */
 yyt->IfRep.ATOMIC = yyt->IfRep.ChoiceList->ChoiceList.ATOMIC;
/* line 1285 "" */
if (! ( (yyt->IfRep.ATOMIC) )) { MarkNonAtomicIf(SELF); }
break;

case kInput:
yyt->Input.SpecList->SpecList.VS=yyt->Input.VS;
yyt->Input.SpecList->SpecList.VIn=yyt->Input.VIn;
yyVisit1 (yyt->Input.SpecList);
yyt->Input.Element->Element.VIn=yyt->Input.SpecList->SpecList.VOut;
/* line 1215 "" */
 yyt->Input.Element->Element.U = xxInput;
/* line 964 "" */
 yyt->Input.Element->Element.InhType = NoTree;
yyVisit1 (yyt->Input.Element);
/* line 949 "" */
InheritFromProtocol(yyt->Input.Element->Element.SynType,yyt->Input.ExprList);
yyt->Input.ExprList->ExprList.VIn=yyt->Input.Element->Element.VOut;
yyVisit1 (yyt->Input.ExprList);
yyt->Input.VOut=yyt->Input.ExprList->ExprList.VOut;
/* line 899 "" */
if (! ( CheckValofScope(yyt->Input.ExprList,yyt->Input.VS) )) { Message("Free variable modified within VALOF",xxError,yyt->Input.Pos); }
/* line 900 "" */
if (! ( (!Tree_IsType(yyt->Input.Element->Element.SynType,kTimer)) )) { TypeCheckTimerInput(yyt->Input.ExprList,yyt->Input.Pos)
	    ; }
else
/* line 901 "" */
if (! ( Tree_IsType(yyt->Input.Element->Element.SynType,kChan) )) { Message("Attempted input on non-channel",xxError,yyt->Input.Pos)
	    ; }
else
/* line 902 "" */
if (! ( DEP(FALSE,yyt->Input.ExprList->ExprList.SynType) )) { TypeCheckInputProtocol(yyt->Input.Element->Element.SynType->Chan.Protocol,yyt->Input.ExprList,yyt->Input.Pos); }
/* line 1199 "" */
if (! ( DEP(FALSE,yyt->Input.Element->Element.SynType) )) { yyt->Input.Element = ReduceElement(yyt->Input.Element,NoTree);; }
/* line 1272 "" */
 yyt->Input.ATOMIC = !IsChanType(yyt->Input.Element->Element.SynType);
break;

case kOutput:
yyt->Output.SpecList->SpecList.VS=yyt->Output.VS;
yyt->Output.SpecList->SpecList.VIn=yyt->Output.VIn;
yyVisit1 (yyt->Output.SpecList);
yyt->Output.Element->Element.VIn=yyt->Output.SpecList->SpecList.VOut;
/* line 1218 "" */
 yyt->Output.Element->Element.U = xxOutput;
/* line 965 "" */
 yyt->Output.Element->Element.InhType = NoTree;
yyVisit1 (yyt->Output.Element);
/* line 948 "" */
InheritFromProtocol(yyt->Output.Element->Element.SynType,yyt->Output.ExprList);
yyt->Output.ExprList->ExprList.VIn=yyt->Output.Element->Element.VOut;
yyVisit1 (yyt->Output.ExprList);
yyt->Output.VOut=yyt->Output.ExprList->ExprList.VOut;
/* line 904 "" */
if (! ( Tree_IsType(yyt->Output.Element->Element.SynType,kChan) )) { Message("Attempted output on non-channel",xxError,yyt->Output.Pos)
	     ; }
else
/* line 905 "" */
if (! ( DEP(FALSE,yyt->Output.ExprList->ExprList.SynType) )) { TypeCheckOutputProtocol(yyt->Output.Element->Element.SynType->Chan.Protocol,yyt->Output.ExprList,yyt->Output.Pos); }
/* line 1200 "" */
if (! ( DEP(FALSE,yyt->Output.Element->Element.SynType) )) { yyt->Output.Element = ReduceElement(yyt->Output.Element,NoTree);; }
/* line 1273 "" */
 yyt->Output.ATOMIC = false;
break;

case kCaseIn:
yyt->CaseIn.SpecList->SpecList.VS=yyt->CaseIn.VS;
yyt->CaseIn.SpecList->SpecList.VIn=yyt->CaseIn.VIn;
yyVisit1 (yyt->CaseIn.SpecList);
yyt->CaseIn.Element->Element.VIn=yyt->CaseIn.SpecList->SpecList.VOut;
yyt->CaseIn.CaseList->CaseList.VS=yyt->CaseIn.VS;
/* line 1216 "" */
 yyt->CaseIn.Element->Element.U = xxInput;
/* line 966 "" */
 yyt->CaseIn.Element->Element.InhType = NoTree;
yyVisit1 (yyt->CaseIn.Element);
yyt->CaseIn.CaseList->CaseList.VIn=yyt->CaseIn.Element->Element.VOut;
/* line 950 "" */
 yyt->CaseIn.CaseList->CaseList.InhType = yyt->CaseIn.Element->Element.SynType;
yyVisit1 (yyt->CaseIn.CaseList);
yyt->CaseIn.VOut=yyt->CaseIn.CaseList->CaseList.VOut;
/* line 907 "" */
if (! ( Tree_IsType(T=yyt->CaseIn.Element->Element.SynType,kChan) )) { Message("Attempted input on non-channel",xxError,yyt->CaseIn.Pos)
	     ; }
else
/* line 908 "" */
if (! ( Tree_IsType(T=T->Chan.Protocol,kNamed) )) { Message("Attempted CASE input on non-named protocol channel",xxError,yyt->CaseIn.Pos)
	     ; }
else
/* line 909 "" */
if (! ( Tree_IsType(T=SymbolKind(T->Named.Sym),kDefTagPro) )) { Message("Attempted CASE input on a channel which does not support a tagged protocol",xxError,yyt->CaseIn.Pos)
	     ; }
else
/* line 910 "" */
if (! ( FALSE )) { TypeCheckCaseInputs(T->DefTagPro.TagProList, yyt->CaseIn.CaseList, yyt->CaseIn.Pos); }
/* line 1201 "" */
if (! ( DEP(FALSE,yyt->CaseIn.Element->Element.SynType) )) { yyt->CaseIn.Element = ReduceElement(yyt->CaseIn.Element,NoTree);; }
/* line 1274 "" */
 yyt->CaseIn.ATOMIC = false;
break;

case kAfter:
yyt->After.SpecList->SpecList.VS=yyt->After.VS;
yyt->After.SpecList->SpecList.VIn=yyt->After.VIn;
yyVisit1 (yyt->After.SpecList);
yyt->After.Element->Element.VIn=yyt->After.SpecList->SpecList.VOut;
/* line 1217 "" */
 yyt->After.Element->Element.U = xxInput;
/* line 967 "" */
 yyt->After.Element->Element.InhType = NoTree;
yyVisit1 (yyt->After.Element);
yyt->After.Expr->Expr.VIn=yyt->After.Element->Element.VOut;
/* line 976 "" */
 yyt->After.Expr->Expr.InhType = mInt();
yyVisit1 (yyt->After.Expr);
yyt->After.VOut=yyt->After.Expr->Expr.VOut;
/* line 912 "" */
if (! ( Tree_IsType(yyt->After.Element->Element.SynType,kTimer) )) { Message("Attempted delayed input on an identifier which is not a timer",xxError,yyt->After.Pos)
	    ; }
else
/* line 913 "" */
if (! ( Tree_IsType(yyt->After.Expr->Expr.SynType,kInt) )) { Message("Delayed input expression is not of type INT",xxError,yyt->After.Pos); }
/* line 1202 "" */
if (! ( DEP(FALSE,yyt->After.Element->Element.SynType) )) { yyt->After.Element = ReduceElement(yyt->After.Element,NoTree);; }
/* line 1275 "" */
 yyt->After.ATOMIC = false;
break;

case kSeqRep:
yyt->SeqRep.SpecList->SpecList.VS=yyt->SeqRep.VS;
yyt->SeqRep.SpecList->SpecList.VIn=yyt->SeqRep.VIn;
yyVisit1 (yyt->SeqRep.SpecList);
yyt->SeqRep.Replicator->Replicator.VIn=yyt->SeqRep.SpecList->SpecList.VOut;
yyVisit1 (yyt->SeqRep.Replicator);
yyt->SeqRep.Process->Process.VIn=yyt->SeqRep.Replicator->Replicator.VOut;
yyt->SeqRep.Process->Process.VS=yyt->SeqRep.VS;
yyVisit1 (yyt->SeqRep.Process);
yyt->SeqRep.VOut=yyt->SeqRep.Process->Process.VOut;
/* line 1279 "" */
 yyt->SeqRep.ATOMIC = yyt->SeqRep.Process->Process.ATOMIC;
break;

case kSeq:
yyt->Seq.SpecList->SpecList.VS=yyt->Seq.VS;
yyt->Seq.ProcList->ProcList.VS=yyt->Seq.VS;
yyt->Seq.SpecList->SpecList.VIn=yyt->Seq.VIn;
yyVisit1 (yyt->Seq.SpecList);
yyt->Seq.ProcList->ProcList.VIn=yyt->Seq.SpecList->SpecList.VOut;
yyVisit1 (yyt->Seq.ProcList);
yyt->Seq.VOut=yyt->Seq.ProcList->ProcList.VOut;
/* line 1278 "" */
 yyt->Seq.ATOMIC = yyt->Seq.ProcList->ProcList.ATOMIC;
break;

case kCase:
yyt->Case.SpecList->SpecList.VS=yyt->Case.VS;
yyt->Case.SpecList->SpecList.VIn=yyt->Case.VIn;
yyVisit1 (yyt->Case.SpecList);
yyt->Case.Selector->Expr.VIn=yyt->Case.SpecList->SpecList.VOut;
yyt->Case.ChoiceList->ChoiceList.VS=yyt->Case.VS;
/* line 971 "" */
 yyt->Case.Selector->Expr.InhType = DefaultType(yyt->Case.Selector);
yyVisit1 (yyt->Case.Selector);
yyt->Case.ChoiceList->ChoiceList.VIn=yyt->Case.Selector->Expr.VOut;
/* line 955 "" */
 yyt->Case.ChoiceList->ChoiceList.InhType = yyt->Case.Selector->Expr.SynType;
yyVisit1 (yyt->Case.ChoiceList);
yyt->Case.VOut=yyt->Case.ChoiceList->ChoiceList.VOut;
/* line 921 "" */
if (! ( (Tree_IsType(yyt->Case.Selector->Expr.SynType,kInteger)||Tree_IsType(yyt->Case.Selector->Expr.SynType,kByte)||Tree_IsType(yyt->Case.Selector->Expr.SynType,kBool)) )) { Message ("CASE selector must be of integer or BYTE (or BOOL) type",xxError,yyt->Case.Pos)
	 ; }
else
/* line 922 "" */
if (! ( DEP(FALSE,yyt->Case.ChoiceList->ChoiceList.SynType) )) { TypeCheckCaseList(yyt->Case.Selector->Expr.SynType,yyt->Case.ChoiceList,false); }
/* line 1288 "" */
 yyt->Case.ATOMIC = yyt->Case.ChoiceList->ChoiceList.ATOMIC;
break;

case kWhile:
yyt->While.SpecList->SpecList.VS=yyt->While.VS;
yyt->While.SpecList->SpecList.VIn=yyt->While.VIn;
yyVisit1 (yyt->While.SpecList);
yyt->While.Expr->Expr.VIn=yyt->While.SpecList->SpecList.VOut;
/* line 972 "" */
 yyt->While.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->While.Expr);
yyt->While.Process->Process.VIn=yyt->While.Expr->Expr.VOut;
yyt->While.Process->Process.VS=yyt->While.VS;
yyVisit1 (yyt->While.Process);
yyt->While.VOut=yyt->While.Process->Process.VOut;
/* line 915 "" */
if (! ( Tree_IsType(yyt->While.Expr->Expr.SynType,kBool) )) { Message("Conditional not of type BOOL",xxError,yyt->While.Pos); }
/* line 1289 "" */
 yyt->While.ATOMIC = yyt->While.Process->Process.ATOMIC;
break;

case kSkip:
yyt->Skip.SpecList->SpecList.VS=yyt->Skip.VS;
yyt->Skip.SpecList->SpecList.VIn=yyt->Skip.VIn;
yyVisit1 (yyt->Skip.SpecList);
yyt->Skip.VOut=yyt->Skip.SpecList->SpecList.VOut;
/* line 1291 "" */
 yyt->Skip.ATOMIC = true;
break;

case kStop:
yyt->Stop.SpecList->SpecList.VS=yyt->Stop.VS;
yyt->Stop.SpecList->SpecList.VIn=yyt->Stop.VIn;
yyVisit1 (yyt->Stop.SpecList);
yyt->Stop.VOut=yyt->Stop.SpecList->SpecList.VOut;
/* line 1292 "" */
 yyt->Stop.ATOMIC = true;
break;

case kInstance:
yyt->Instance.SpecList->SpecList.VS=yyt->Instance.VS;
/* line 945 "" */
InheritFromFormals(SymbolKind(yyt->Instance.Sym),yyt->Instance.ExprList);
yyt->Instance.SpecList->SpecList.VIn=yyt->Instance.VIn;
yyVisit1 (yyt->Instance.SpecList);
yyt->Instance.ExprList->ExprList.VIn=yyt->Instance.SpecList->SpecList.VOut;
yyVisit1 (yyt->Instance.ExprList);
yyt->Instance.VOut=yyt->Instance.ExprList->ExprList.VOut;
/* line 896 "" */
if (! ( Tree_IsType(SymbolKind(yyt->Instance.Sym),kDefAProc) )) { Message("Identifier not declared as a procedure",xxError,yyt->Instance.Pos)
	       ; }
else
/* line 897 "" */
if (! ( DEP(FALSE,yyt->Instance.ExprList->ExprList.SynType) )) { TypeCheckParameters(SymbolKind(yyt->Instance.Sym)->DefAProc.Formals,yyt->Instance.ExprList,yyt->Instance.Pos,yyt->Instance.VS); }
/* line 1240 "" */
if (! ( DEP(FALSE,yyt->Instance.SpecList) )) { SymbolKind(yyt->Instance.Sym)->DefAProc.U=xxUsed; ; }
/* line 1294 "" */
 yyt->Instance.ATOMIC = IsAtomicProc(Symbol(yyt->Instance.Sym).Kind);
break;

case kCCode:
yyt->CCode.SpecList->SpecList.VS=yyt->CCode.VS;
yyt->CCode.SpecList->SpecList.VIn=yyt->CCode.VIn;
yyVisit1 (yyt->CCode.SpecList);
yyt->CCode.VOut=yyt->CCode.SpecList->SpecList.VOut;
/* line 1269 "" */
 yyt->CCode.ATOMIC = true;
break;

case kNullProcess:
yyt->NullProcess.SpecList->SpecList.VS=yyt->NullProcess.VS;
yyt->NullProcess.SpecList->SpecList.VIn=yyt->NullProcess.VIn;
yyVisit1 (yyt->NullProcess.SpecList);
yyt->NullProcess.VOut=yyt->NullProcess.SpecList->SpecList.VOut;
/* line 1263 "" */
 yyt->NullProcess.ATOMIC = false;
break;

case kAltList:
yyt->AltList.VOut=yyt->AltList.VIn;
break;

case kNoAlt:
yyt->NoAlt.VOut=yyt->NoAlt.VIn;
break;

case kAnAlts:
yyt->AnAlts.SpecList->SpecList.VS=yyt->AnAlts.VS;
yyt->AnAlts.Next->AltList.VS=yyt->AnAlts.VS;
yyt->AnAlts.SpecList->SpecList.VIn=yyt->AnAlts.VIn;
yyVisit1 (yyt->AnAlts.SpecList);
yyt->AnAlts.Next->AltList.VIn=yyt->AnAlts.SpecList->SpecList.VOut;
yyVisit1 (yyt->AnAlts.Next);
yyt->AnAlts.VOut=yyt->AnAlts.Next->AltList.VOut;
break;

case kAnAlt:
yyt->AnAlt.SpecList->SpecList.VS=yyt->AnAlt.VS;
yyt->AnAlt.Guard->Guard.VS=yyt->AnAlt.VS;
yyt->AnAlt.Next->AltList.VS=yyt->AnAlt.VS;
yyt->AnAlt.SpecList->SpecList.VIn=yyt->AnAlt.VIn;
yyVisit1 (yyt->AnAlt.SpecList);
yyt->AnAlt.Next->AltList.VIn=yyt->AnAlt.SpecList->SpecList.VOut;
yyVisit1 (yyt->AnAlt.Next);
yyt->AnAlt.Guard->Guard.VIn=yyt->AnAlt.Next->AltList.VOut;
yyVisit1 (yyt->AnAlt.Guard);
yyt->AnAlt.VOut=yyt->AnAlt.Guard->Guard.VOut;
break;

case kANewAlt:
yyt->ANewAlt.SpecList->SpecList.VS=yyt->ANewAlt.VS;
yyt->ANewAlt.Next->AltList.VS=yyt->ANewAlt.VS;
yyt->ANewAlt.SpecList->SpecList.VIn=yyt->ANewAlt.VIn;
yyVisit1 (yyt->ANewAlt.SpecList);
yyt->ANewAlt.Next->AltList.VIn=yyt->ANewAlt.SpecList->SpecList.VOut;
yyVisit1 (yyt->ANewAlt.Next);
yyt->ANewAlt.AltType->AltType.VIn=yyt->ANewAlt.Next->AltList.VOut;
yyt->ANewAlt.AltType->AltType.VS=yyt->ANewAlt.VS;
yyVisit1 (yyt->ANewAlt.AltType);
yyt->ANewAlt.VOut=yyt->ANewAlt.AltType->AltType.VOut;
break;

case kGuard:
yyt->Guard.Expr->Expr.VIn=yyt->Guard.VIn;
/* line 973 "" */
 yyt->Guard.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->Guard.Expr);
/* line 928 "" */
if (! ( Tree_IsType(yyt->Guard.Expr->Expr.SynType,kBool) )) { Message("Alternation guard is not of type boolean",xxError,yyt->Guard.Pos); }
yyt->Guard.VOut=yyt->Guard.Expr->Expr.VOut;
break;

case kAltInput:
yyt->AltInput.Expr->Expr.VIn=yyt->AltInput.VIn;
/* line 973 "" */
 yyt->AltInput.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->AltInput.Expr);
/* line 928 "" */
if (! ( Tree_IsType(yyt->AltInput.Expr->Expr.SynType,kBool) )) { Message("Alternation guard is not of type boolean",xxError,yyt->AltInput.Pos); }
yyt->AltInput.Input->Input.VIn=yyt->AltInput.Expr->Expr.VOut;
yyt->AltInput.Input->Input.VS=yyt->AltInput.VS;
yyVisit1 (yyt->AltInput.Input);
yyt->AltInput.Process->Process.VIn=yyt->AltInput.Input->Input.VOut;
yyt->AltInput.Process->Process.VS=yyt->AltInput.VS;
yyVisit1 (yyt->AltInput.Process);
yyt->AltInput.VOut=yyt->AltInput.Process->Process.VOut;
break;

case kAltTimer:
yyt->AltTimer.Expr->Expr.VIn=yyt->AltTimer.VIn;
/* line 973 "" */
 yyt->AltTimer.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->AltTimer.Expr);
/* line 928 "" */
if (! ( Tree_IsType(yyt->AltTimer.Expr->Expr.SynType,kBool) )) { Message("Alternation guard is not of type boolean",xxError,yyt->AltTimer.Pos); }
yyt->AltTimer.After->After.VIn=yyt->AltTimer.Expr->Expr.VOut;
yyt->AltTimer.After->After.VS=yyt->AltTimer.VS;
yyVisit1 (yyt->AltTimer.After);
yyt->AltTimer.Process->Process.VIn=yyt->AltTimer.After->After.VOut;
yyt->AltTimer.Process->Process.VS=yyt->AltTimer.VS;
yyVisit1 (yyt->AltTimer.Process);
yyt->AltTimer.VOut=yyt->AltTimer.Process->Process.VOut;
break;

case kAltCaseIn:
yyt->AltCaseIn.Expr->Expr.VIn=yyt->AltCaseIn.VIn;
/* line 973 "" */
 yyt->AltCaseIn.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->AltCaseIn.Expr);
/* line 928 "" */
if (! ( Tree_IsType(yyt->AltCaseIn.Expr->Expr.SynType,kBool) )) { Message("Alternation guard is not of type boolean",xxError,yyt->AltCaseIn.Pos); }
yyt->AltCaseIn.CaseIn->CaseIn.VIn=yyt->AltCaseIn.Expr->Expr.VOut;
yyt->AltCaseIn.CaseIn->CaseIn.VS=yyt->AltCaseIn.VS;
yyVisit1 (yyt->AltCaseIn.CaseIn);
yyt->AltCaseIn.VOut=yyt->AltCaseIn.CaseIn->CaseIn.VOut;
break;

case kNoInput:
yyt->NoInput.Expr->Expr.VIn=yyt->NoInput.VIn;
/* line 973 "" */
 yyt->NoInput.Expr->Expr.InhType = mBool();
yyVisit1 (yyt->NoInput.Expr);
/* line 928 "" */
if (! ( Tree_IsType(yyt->NoInput.Expr->Expr.SynType,kBool) )) { Message("Alternation guard is not of type boolean",xxError,yyt->NoInput.Pos); }
yyt->NoInput.Process->Process.VIn=yyt->NoInput.Expr->Expr.VOut;
yyt->NoInput.Process->Process.VS=yyt->NoInput.VS;
yyVisit1 (yyt->NoInput.Process);
yyt->NoInput.VOut=yyt->NoInput.Process->Process.VOut;
break;

case kChoiceList:
yyt->ChoiceList.VOut=yyt->ChoiceList.VIn;
/* line 1264 "" */
 yyt->ChoiceList.ATOMIC = false;
/* line 797 "" */
 yyt->ChoiceList.SynType = NoTree;
break;

case kNoChoice:
yyt->NoChoice.VOut=yyt->NoChoice.VIn;
/* line 1299 "" */
 yyt->NoChoice.ATOMIC = true;
/* line 797 "" */
 yyt->NoChoice.SynType = NoTree;
break;

case kAChoices:
yyt->AChoices.SpecList->SpecList.VS=yyt->AChoices.VS;
yyt->AChoices.Next->ChoiceList.VS=yyt->AChoices.VS;
yyt->AChoices.SpecList->SpecList.VIn=yyt->AChoices.VIn;
yyVisit1 (yyt->AChoices.SpecList);
yyt->AChoices.Next->ChoiceList.VIn=yyt->AChoices.SpecList->SpecList.VOut;
yyt->AChoices.Next->ChoiceList.InhType=yyt->AChoices.InhType;
yyVisit1 (yyt->AChoices.Next);
yyt->AChoices.VOut=yyt->AChoices.Next->ChoiceList.VOut;
/* line 1264 "" */
 yyt->AChoices.ATOMIC = false;
/* line 797 "" */
 yyt->AChoices.SynType = NoTree;
break;

case kAChoice:
yyt->AChoice.SpecList->SpecList.VS=yyt->AChoice.VS;
/* line 956 "" */
InheritFromType(InheritLhsType(yyt->AChoice.InhType,yyt->AChoice.ExprList->ExprList.DefType),yyt->AChoice.ExprList);
yyt->AChoice.Next->ChoiceList.VS=yyt->AChoice.VS;
yyt->AChoice.SpecList->SpecList.VIn=yyt->AChoice.VIn;
yyVisit1 (yyt->AChoice.SpecList);
yyt->AChoice.Next->ChoiceList.VIn=yyt->AChoice.SpecList->SpecList.VOut;
yyt->AChoice.Next->ChoiceList.InhType=yyt->AChoice.InhType;
yyVisit1 (yyt->AChoice.Next);
yyt->AChoice.ExprList->ExprList.VIn=yyt->AChoice.Next->ChoiceList.VOut;
yyVisit1 (yyt->AChoice.ExprList);
yyt->AChoice.Process->Process.VIn=yyt->AChoice.ExprList->ExprList.VOut;
yyt->AChoice.Process->Process.VS=yyt->AChoice.VS;
yyVisit1 (yyt->AChoice.Process);
yyt->AChoice.VOut=yyt->AChoice.Process->Process.VOut;
/* line 1300 "" */
 yyt->AChoice.ATOMIC = (yyt->AChoice.Next->ChoiceList.ATOMIC && yyt->AChoice.Process->Process.ATOMIC);
/* line 797 "" */
 yyt->AChoice.SynType = NoTree;
break;

case kANewIf:
yyt->ANewIf.SpecList->SpecList.VS=yyt->ANewIf.VS;
yyt->ANewIf.Next->ChoiceList.VS=yyt->ANewIf.VS;
yyt->ANewIf.SpecList->SpecList.VIn=yyt->ANewIf.VIn;
yyVisit1 (yyt->ANewIf.SpecList);
yyt->ANewIf.Next->ChoiceList.VIn=yyt->ANewIf.SpecList->SpecList.VOut;
yyt->ANewIf.Next->ChoiceList.InhType=yyt->ANewIf.InhType;
yyVisit1 (yyt->ANewIf.Next);
yyt->ANewIf.IfType->IfType.VIn=yyt->ANewIf.Next->ChoiceList.VOut;
yyt->ANewIf.IfType->IfType.VS=yyt->ANewIf.VS;
yyVisit1 (yyt->ANewIf.IfType);
yyt->ANewIf.VOut=yyt->ANewIf.IfType->IfType.VOut;
/* line 1301 "" */
 yyt->ANewIf.ATOMIC = (yyt->ANewIf.Next->ChoiceList.ATOMIC && yyt->ANewIf.IfType->IfType.ATOMIC);
/* line 797 "" */
 yyt->ANewIf.SynType = NoTree;
break;

case kProcList:
yyt->ProcList.VOut=yyt->ProcList.VIn;
/* line 1265 "" */
 yyt->ProcList.ATOMIC = false;
break;

case kNoProcess:
yyt->NoProcess.VOut=yyt->NoProcess.VIn;
/* line 1296 "" */
 yyt->NoProcess.ATOMIC = true;
break;

case kAProcess:
yyt->AProcess.Next->ProcList.VS=yyt->AProcess.VS;
yyt->AProcess.Next->ProcList.VIn=yyt->AProcess.VIn;
yyVisit1 (yyt->AProcess.Next);
yyt->AProcess.Process->Process.VIn=yyt->AProcess.Next->ProcList.VOut;
yyt->AProcess.Process->Process.VS=yyt->AProcess.VS;
yyVisit1 (yyt->AProcess.Process);
yyt->AProcess.VOut=yyt->AProcess.Process->Process.VOut;
/* line 1297 "" */
 yyt->AProcess.ATOMIC = (yyt->AProcess.Next->ProcList.ATOMIC && yyt->AProcess.Process->Process.ATOMIC);
break;

case kCaseList:
yyt->CaseList.VOut=yyt->CaseList.VIn;
break;

case kNoCase:
yyt->NoCase.VOut=yyt->NoCase.VIn;
break;

case kACase:
yyt->ACase.Next->CaseList.VS=yyt->ACase.VS;
/* line 957 "" */
InheritFromProtocol(yyt->ACase.InhType,yyt->ACase.ExprList);
yyt->ACase.SpecList->SpecList.VS=yyt->ACase.VS;
yyt->ACase.Next->CaseList.VIn=yyt->ACase.VIn;
yyt->ACase.Next->CaseList.InhType=yyt->ACase.InhType;
yyVisit1 (yyt->ACase.Next);
yyt->ACase.SpecList->SpecList.VIn=yyt->ACase.Next->CaseList.VOut;
yyVisit1 (yyt->ACase.SpecList);
yyt->ACase.ExprList->ExprList.VIn=yyt->ACase.SpecList->SpecList.VOut;
yyVisit1 (yyt->ACase.ExprList);
yyt->ACase.Process->Process.VIn=yyt->ACase.ExprList->ExprList.VOut;
yyt->ACase.Process->Process.VS=yyt->ACase.VS;
yyVisit1 (yyt->ACase.Process);
yyt->ACase.VOut=yyt->ACase.Process->Process.VOut;
break;
 default: ;
 }
}

void BeginTypeChkAg ()
{
/* line 665 "" */

  BeginTypeChk();

}

void CloseTypeChkAg ()
{
/* line 671 "" */

  CloseTypeChk();
  ag_phase++;

}
