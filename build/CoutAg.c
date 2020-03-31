# define DEP(a, b) a
# define SELF yyt
# include "CoutAg.h"

/* line 730 "" */

  /*{{{  void writetVUsage(tVUsage a)*/
  void writetVUsage(tVUsage a)
  {
    fprintf(yyf,"A="); 
    if (a & xxAtomic) fprintf(yyf,"y");
    else fprintf(yyf,"n");
  
    fprintf(yyf,",L=");
    if ((a & (xxLRead | xxLWrite))==0) fprintf(yyf,"None");
    if (a & xxLRead) fprintf(yyf,"Rd");
    if (a & xxLWrite) fprintf(yyf,"Wr");
  
    fprintf(yyf,",NL=");
    if ((a & (xxNLRead | xxNLWrite))==0) fprintf(yyf,"None");
    if (a & xxNLRead) fprintf(yyf,"Rd");
    if (a & xxNLWrite) fprintf(yyf,"Wr");
  }
  /*}}}*/


static char yyb;

static void yyVisit1 ARGS((register tTree yyt));

void CoutAg
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
/* line 919 "" */
 yyt->Start.Process->Process.Parent = 0;
/* line 848 "" */
 yyt->Start.Process->Process.TLA = Tree_IsType(yyt->Start.Process,kAltType);
yyVisit1 (yyt->Start.Process);
break;

case kSpecList:
/* line 761 "" */
 yyt->SpecList.DCL = 0;
break;

case kNoSpec:
/* line 765 "" */
 yyt->NoSpec.DCL = 0;
break;

case kASpec:
yyt->ASpec.Spec->Spec.AtomicSpec=yyt->ASpec.AtomicSpec;
yyt->ASpec.Spec->Spec.Parent=yyt->ASpec.Parent;
yyVisit1 (yyt->ASpec.Spec);
yyt->ASpec.Next->SpecList.AtomicSpec=yyt->ASpec.AtomicSpec;
yyt->ASpec.Next->SpecList.Parent=yyt->ASpec.Parent;
yyVisit1 (yyt->ASpec.Next);
/* line 763 "" */
 yyt->ASpec.DCL = CountSpec(yyt->ASpec.Spec) + yyt->ASpec.Next->SpecList.DCL; 
break;

case kSpec:
break;

case kDeclType:
/* line 882 "" */
 yyt->DeclType.Type->Type.D = 0;
yyVisit1 (yyt->DeclType.Type);
break;

case kDecl:
/* line 969 "" */
 yyt->Decl.IdList->IdList.AtomicSpec = yyt->Decl.AtomicSpec;
yyt->Decl.IdList->IdList.Parent=yyt->Decl.Parent;
yyVisit1 (yyt->Decl.IdList);
/* line 883 "" */
 yyt->Decl.Type->Type.D = 0;
yyVisit1 (yyt->Decl.Type);
break;

case kIs:
yyt->Is.Elemental->Elemental.Parent=yyt->Is.Parent;
yyVisit1 (yyt->Is.Elemental);
/* line 898 "" */
 yyt->Is.Type->Type.D = 0;
yyVisit1 (yyt->Is.Type);
/* line 952 "" */
if (! ( DEP(false,yyt->Is.AtomicSpec) )) { if (yyt->Is.AtomicSpec) AddVUsage(yyt->Is.Sym,xxAtomic); ; }
break;

case kAVal:
yyt->AVal.Expr->Expr.Parent=yyt->AVal.Parent;
yyVisit1 (yyt->AVal.Expr);
/* line 899 "" */
 yyt->AVal.Type->Type.D = 0;
yyVisit1 (yyt->AVal.Type);
break;

case kVal:
yyt->Val.Expr->Expr.Parent=yyt->Val.Parent;
yyVisit1 (yyt->Val.Expr);
/* line 900 "" */
 yyt->Val.Type->Type.D = 0;
yyVisit1 (yyt->Val.Type);
/* line 951 "" */
if (! ( DEP(false,yyt->Val.AtomicSpec) )) { if (yyt->Val.AtomicSpec) AddVUsage(yyt->Val.Sym,xxAtomic); ; }
break;

case kValRetypes:
yyt->ValRetypes.Expr->Expr.Parent=yyt->ValRetypes.Parent;
yyVisit1 (yyt->ValRetypes.Expr);
/* line 901 "" */
 yyt->ValRetypes.Type->Type.D = 0;
yyVisit1 (yyt->ValRetypes.Type);
/* line 953 "" */
if (! ( DEP(false,yyt->ValRetypes.AtomicSpec) )) { if (yyt->ValRetypes.AtomicSpec) AddVUsage(yyt->ValRetypes.Sym,xxAtomic); ; }
break;

case kValReshapes:
yyt->ValReshapes.Expr->Expr.Parent=yyt->ValReshapes.Parent;
yyVisit1 (yyt->ValReshapes.Expr);
/* line 901 "" */
 yyt->ValReshapes.Type->Type.D = 0;
yyVisit1 (yyt->ValReshapes.Type);
/* line 953 "" */
if (! ( DEP(false,yyt->ValReshapes.AtomicSpec) )) { if (yyt->ValReshapes.AtomicSpec) AddVUsage(yyt->ValReshapes.Sym,xxAtomic); ; }
break;

case kRetypes:
yyt->Retypes.Elemental->Elemental.Parent=yyt->Retypes.Parent;
yyVisit1 (yyt->Retypes.Elemental);
/* line 902 "" */
 yyt->Retypes.Type->Type.D = 0;
yyVisit1 (yyt->Retypes.Type);
/* line 954 "" */
if (! ( DEP(false,yyt->Retypes.AtomicSpec) )) { if (yyt->Retypes.AtomicSpec) AddVUsage(yyt->Retypes.Sym,xxAtomic); ; }
break;

case kReshapes:
yyt->Reshapes.Elemental->Elemental.Parent=yyt->Reshapes.Parent;
yyVisit1 (yyt->Reshapes.Elemental);
/* line 902 "" */
 yyt->Reshapes.Type->Type.D = 0;
yyVisit1 (yyt->Reshapes.Type);
/* line 954 "" */
if (! ( DEP(false,yyt->Reshapes.AtomicSpec) )) { if (yyt->Reshapes.AtomicSpec) AddVUsage(yyt->Reshapes.Sym,xxAtomic); ; }
break;

case kDefAPro:
break;

case kDefSeqPro:
yyVisit1 (yyt->DefSeqPro.SeqProList);
break;

case kDefTagPro:
yyVisit1 (yyt->DefTagPro.TagProList);
break;

case kDefAProc:
yyt->DefAProc.Formals->Formals.AtomicSpec=yyt->DefAProc.AtomicSpec;
yyt->DefAProc.Formals->Formals.Parent=yyt->DefAProc.Parent;
yyVisit1 (yyt->DefAProc.Formals);
/* line 957 "" */
if (! ( false )) { if (IsAtomicProc(SELF)) AddVUsage(yyt->DefAProc.Sym,xxAtomic); ; }
break;

case kDefProc:
/* line 923 "" */
 yyt->DefProc.Process->Process.Parent = yyt->DefProc.Sym;
/* line 849 "" */
 yyt->DefProc.Process->Process.TLA = Tree_IsType(yyt->DefProc.Process,kAltType);
yyVisit1 (yyt->DefProc.Process);
/* line 982 "" */
 yyt->DefProc.Formals->Formals.AtomicSpec = IsAtomicProc(SELF) && !IsParProc(SELF);
/* line 923 "" */
 yyt->DefProc.Formals->Formals.Parent = yyt->DefProc.Sym;
yyVisit1 (yyt->DefProc.Formals);
/* line 957 "" */
if (! ( false )) { if (IsAtomicProc(SELF)) AddVUsage(yyt->DefProc.Sym,xxAtomic); ; }
break;

case kInlineDefProc:
/* line 923 "" */
 yyt->InlineDefProc.Process->Process.Parent = yyt->InlineDefProc.Sym;
/* line 849 "" */
 yyt->InlineDefProc.Process->Process.TLA = Tree_IsType(yyt->InlineDefProc.Process,kAltType);
yyVisit1 (yyt->InlineDefProc.Process);
/* line 982 "" */
 yyt->InlineDefProc.Formals->Formals.AtomicSpec = IsAtomicProc(SELF) && !IsParProc(SELF);
/* line 923 "" */
 yyt->InlineDefProc.Formals->Formals.Parent = yyt->InlineDefProc.Sym;
yyVisit1 (yyt->InlineDefProc.Formals);
/* line 957 "" */
if (! ( false )) { if (IsAtomicProc(SELF)) AddVUsage(yyt->InlineDefProc.Sym,xxAtomic); ; }
break;

case kPrototypeProc:
yyt->PrototypeProc.Formals->Formals.AtomicSpec=yyt->PrototypeProc.AtomicSpec;
/* line 924 "" */
 yyt->PrototypeProc.Formals->Formals.Parent = yyt->PrototypeProc.Sym;
yyVisit1 (yyt->PrototypeProc.Formals);
/* line 957 "" */
if (! ( false )) { if (IsAtomicProc(SELF)) AddVUsage(yyt->PrototypeProc.Sym,xxAtomic); ; }
break;

case kDefAFunc:
yyt->DefAFunc.Formals->Formals.AtomicSpec=yyt->DefAFunc.AtomicSpec;
yyt->DefAFunc.Formals->Formals.Parent=yyt->DefAFunc.Parent;
yyVisit1 (yyt->DefAFunc.Formals);
/* line 903 "" */
 yyt->DefAFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->DefAFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->DefAFunc.Sym,xxAtomic); ; }
break;

case kDefFunc:
/* line 921 "" */
 yyt->DefFunc.Valof->Valof.Parent = yyt->DefFunc.Sym;
yyVisit1 (yyt->DefFunc.Valof);
/* line 981 "" */
 yyt->DefFunc.Formals->Formals.AtomicSpec = true;
/* line 921 "" */
 yyt->DefFunc.Formals->Formals.Parent = yyt->DefFunc.Sym;
yyVisit1 (yyt->DefFunc.Formals);
/* line 904 "" */
 yyt->DefFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->DefFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->DefFunc.Sym,xxAtomic); ; }
break;

case kInlineDefFunc:
/* line 921 "" */
 yyt->InlineDefFunc.Valof->Valof.Parent = yyt->InlineDefFunc.Sym;
yyVisit1 (yyt->InlineDefFunc.Valof);
/* line 981 "" */
 yyt->InlineDefFunc.Formals->Formals.AtomicSpec = true;
/* line 921 "" */
 yyt->InlineDefFunc.Formals->Formals.Parent = yyt->InlineDefFunc.Sym;
yyVisit1 (yyt->InlineDefFunc.Formals);
/* line 904 "" */
 yyt->InlineDefFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->InlineDefFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->InlineDefFunc.Sym,xxAtomic); ; }
break;

case kDefIsFunc:
/* line 922 "" */
 yyt->DefIsFunc.ExprList->ExprList.Parent = yyt->DefIsFunc.Sym;
yyVisit1 (yyt->DefIsFunc.ExprList);
yyt->DefIsFunc.Formals->Formals.AtomicSpec=yyt->DefIsFunc.AtomicSpec;
/* line 922 "" */
 yyt->DefIsFunc.Formals->Formals.Parent = yyt->DefIsFunc.Sym;
yyVisit1 (yyt->DefIsFunc.Formals);
/* line 905 "" */
 yyt->DefIsFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->DefIsFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->DefIsFunc.Sym,xxAtomic); ; }
break;

case kInlineDefIsFunc:
/* line 922 "" */
 yyt->InlineDefIsFunc.ExprList->ExprList.Parent = yyt->InlineDefIsFunc.Sym;
yyVisit1 (yyt->InlineDefIsFunc.ExprList);
yyt->InlineDefIsFunc.Formals->Formals.AtomicSpec=yyt->InlineDefIsFunc.AtomicSpec;
/* line 922 "" */
 yyt->InlineDefIsFunc.Formals->Formals.Parent = yyt->InlineDefIsFunc.Sym;
yyVisit1 (yyt->InlineDefIsFunc.Formals);
/* line 905 "" */
 yyt->InlineDefIsFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->InlineDefIsFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->InlineDefIsFunc.Sym,xxAtomic); ; }
break;

case kPrototypeFunc:
yyt->PrototypeFunc.Formals->Formals.AtomicSpec=yyt->PrototypeFunc.AtomicSpec;
/* line 925 "" */
 yyt->PrototypeFunc.Formals->Formals.Parent = yyt->PrototypeFunc.Sym;
yyVisit1 (yyt->PrototypeFunc.Formals);
/* line 907 "" */
 yyt->PrototypeFunc.TypeList->TypeList.D = 0;
yyVisit1 (yyt->PrototypeFunc.TypeList);
/* line 956 "" */
if (! ( false )) { AddVUsage(yyt->PrototypeFunc.Sym,xxAtomic); ; }
break;

case kNullSpec:
break;

case kHCode:
break;

case kCallFrame:
break;

case kParFrame:
break;

case kFormals:
break;

case kNoFormal:
break;

case kNoFormals:
break;

case kAFormal:
/* line 950 "" */
if (! ( DEP(false,yyt->AFormal.AtomicSpec) )) { if (yyt->AFormal.AtomicSpec) AddVUsage(yyt->AFormal.Sym,xxAtomic); ; }
/* line 906 "" */
 yyt->AFormal.Type->Type.D = 0;
yyVisit1 (yyt->AFormal.Type);
yyVisit1 (yyt->AFormal.By);
yyt->AFormal.Next->Formals.AtomicSpec=yyt->AFormal.AtomicSpec;
yyt->AFormal.Next->Formals.Parent=yyt->AFormal.Parent;
yyVisit1 (yyt->AFormal.Next);
break;

case kReplicator:
/* line 946 "" */
if (! ( DEP(false,yyt->Replicator.AtomicSpec) )) { AddVUsage(yyt->Replicator.Sym,xxLWrite); if (yyt->Replicator.AtomicSpec) AddVUsage(yyt->Replicator.Sym,xxAtomic); ; }
yyt->Replicator.For->Expr.Parent=yyt->Replicator.Parent;
yyVisit1 (yyt->Replicator.For);
yyt->Replicator.From->Expr.Parent=yyt->Replicator.Parent;
yyVisit1 (yyt->Replicator.From);
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
/* line 949 "" */
if (! ( DEP(false,yyt->AnId.AtomicSpec) )) { if (yyt->AnId.AtomicSpec) AddVUsage(yyt->AnId.Sym,xxAtomic); ; }
yyt->AnId.Next->IdList.AtomicSpec=yyt->AnId.AtomicSpec;
yyt->AnId.Next->IdList.Parent=yyt->AnId.Parent;
yyVisit1 (yyt->AnId.Next);
break;

case kType:
break;

case kArray:
/* line 930 "" */
 yyt->Array.Expr->Expr.Parent = 0;
yyVisit1 (yyt->Array.Expr);
/* line 880 "" */
 yyt->Array.Type->Type.D = yyt->Array.D+1;
yyVisit1 (yyt->Array.Type);
break;

case kUserArray:
/* line 880 "" */
 yyt->UserArray.Type->Type.D = yyt->UserArray.D+1;
yyVisit1 (yyt->UserArray.Type);
/* line 930 "" */
 yyt->UserArray.Expr->Expr.Parent = 0;
yyVisit1 (yyt->UserArray.Expr);
break;

case kChan:
yyVisit1 (yyt->Chan.Protocol);
break;

case kTimer:
break;

case kUntyped:
break;

case kUntypedByte:
break;

case kUntypedReal:
break;

case kUntypedInteger:
break;

case kUntypedInt:
break;

case kUntypedHex:
break;

case kPrimitive:
break;

case kNumeric:
break;

case kInteger:
break;

case kInt:
break;

case kUserInt:
break;

case kInt16:
break;

case kUserInt16:
break;

case kInt32:
break;

case kUserInt32:
break;

case kInt64:
break;

case kUserInt64:
break;

case kReal:
break;

case kReal32:
break;

case kUserReal32:
break;

case kReal64:
break;

case kUserReal64:
break;

case kByte:
break;

case kUserByte:
break;

case kBool:
break;

case kUserBool:
break;

case kUserStructTypes:
yyVisit1 (yyt->UserStructTypes.FieldList);
break;

case kUserDataTypes:
yyVisit1 (yyt->UserDataTypes.FieldList);
break;

case kUserRecord:
yyVisit1 (yyt->UserRecord.FieldList);
break;

case kUserPackedRecord:
yyVisit1 (yyt->UserPackedRecord.FieldList);
break;

case kUserUnion:
yyVisit1 (yyt->UserUnion.FieldList);
break;

case kUserChanRecord:
yyVisit1 (yyt->UserChanRecord.FieldList);
break;

case kTypeList:
break;

case kNoType:
break;

case kAType:
yyt->AType.Next->TypeList.D=yyt->AType.D;
yyVisit1 (yyt->AType.Next);
/* line 884 "" */
 yyt->AType.Type->Type.D = 0;
yyVisit1 (yyt->AType.Type);
break;

case kFieldList:
break;

case kNoField:
break;

case kAField:
/* line 885 "" */
 yyt->AField.Type->Type.D = 0;
yyVisit1 (yyt->AField.Type);
yyVisit1 (yyt->AField.Next);
break;

case kProtocol:
break;

case kAny:
break;

case kNamed:
break;

case kSimple:
/* line 886 "" */
 yyt->Simple.Type->Type.D = 0;
yyVisit1 (yyt->Simple.Type);
break;

case kCounted:
/* line 887 "" */
 yyt->Counted.Type->Type.D = 0;
yyVisit1 (yyt->Counted.Type);
/* line 887 "" */
 yyt->Counted.Length->Type.D = 0;
yyVisit1 (yyt->Counted.Length);
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
break;

case kBinary:
yyVisit1 (yyt->Binary.Op);
yyt->Binary.Rop->Expr.Parent=yyt->Binary.Parent;
yyVisit1 (yyt->Binary.Rop);
yyt->Binary.Lop->Expr.Parent=yyt->Binary.Parent;
yyVisit1 (yyt->Binary.Lop);
break;

case kUnary:
yyVisit1 (yyt->Unary.Op);
yyt->Unary.Expr->Expr.Parent=yyt->Unary.Parent;
yyVisit1 (yyt->Unary.Expr);
break;

case kConst:
/* line 888 "" */
 yyt->Const.Type->Type.D = 0;
yyVisit1 (yyt->Const.Type);
break;

case kValof:
yyt->Valof.ExprList->ExprList.Parent=yyt->Valof.Parent;
yyVisit1 (yyt->Valof.ExprList);
/* line 928 "" */
 yyt->Valof.Process->Process.Parent = yyt->Valof.Parent;
/* line 850 "" */
 yyt->Valof.Process->Process.TLA = Tree_IsType(yyt->Valof.Process,kAltType);
yyVisit1 (yyt->Valof.Process);
/* line 971 "" */
 yyt->Valof.SpecList->SpecList.AtomicSpec = IsAtomic(yyt->Valof.Process);
yyt->Valof.SpecList->SpecList.Parent=yyt->Valof.Parent;
yyVisit1 (yyt->Valof.SpecList);
break;

case kFuncCall:
yyt->FuncCall.ExprList->ExprList.Parent=yyt->FuncCall.Parent;
yyVisit1 (yyt->FuncCall.ExprList);
/* line 943 "" */
if (! ( false )) { AddVUsage(yyt->FuncCall.Sym,xxAccess); ; }
break;

case kElementalExpr:
yyt->ElementalExpr.Elemental->Elemental.Parent=yyt->ElementalExpr.Parent;
yyVisit1 (yyt->ElementalExpr.Elemental);
break;

case kDefault:
break;

case kConvert:
yyt->Convert.Expr->Expr.Parent=yyt->Convert.Parent;
yyVisit1 (yyt->Convert.Expr);
/* line 889 "" */
 yyt->Convert.Type->Type.D = 0;
yyVisit1 (yyt->Convert.Type);
break;

case kRound:
yyt->Round.Expr->Expr.Parent=yyt->Round.Parent;
yyVisit1 (yyt->Round.Expr);
/* line 891 "" */
 yyt->Round.Type->Type.D = 0;
yyVisit1 (yyt->Round.Type);
break;

case kTrunc:
yyt->Trunc.Expr->Expr.Parent=yyt->Trunc.Parent;
yyVisit1 (yyt->Trunc.Expr);
/* line 892 "" */
 yyt->Trunc.Type->Type.D = 0;
yyVisit1 (yyt->Trunc.Type);
break;

case kMostPos:
/* line 893 "" */
 yyt->MostPos.Type->Type.D = 0;
yyVisit1 (yyt->MostPos.Type);
break;

case kMostNeg:
/* line 894 "" */
 yyt->MostNeg.Type->Type.D = 0;
yyVisit1 (yyt->MostNeg.Type);
break;

case kCountArray:
yyt->CountArray.Expr->Expr.Parent=yyt->CountArray.Parent;
yyVisit1 (yyt->CountArray.Expr);
yyt->CountArray.Length->Expr.Parent=yyt->CountArray.Parent;
yyVisit1 (yyt->CountArray.Length);
break;

case kUnBound:
break;

case kBytesIn:
break;

case kBytesInType:
/* line 895 "" */
 yyt->BytesInType.Type->Type.D = 0;
yyVisit1 (yyt->BytesInType.Type);
break;

case kBytesInExpr:
yyt->BytesInExpr.Expr->Expr.Parent=yyt->BytesInExpr.Parent;
yyVisit1 (yyt->BytesInExpr.Expr);
break;

case kSizeOf:
/* line 897 "" */
 yyt->SizeOf.Type->Type.D = 0;
yyVisit1 (yyt->SizeOf.Type);
break;

case kOffsetOf:
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.Parent=yyt->OffsetOf.Parent;
yyVisit1 (yyt->OffsetOf.UnmappedIdentifier);
/* line 896 "" */
 yyt->OffsetOf.Type->Type.D = 0;
yyVisit1 (yyt->OffsetOf.Type);
break;

case kStringElement:
break;

case kString:
/* line 881 "" */
 yyt->String.Type->Type.D = 0;
yyVisit1 (yyt->String.Type);
break;

case kStringSubscript:
yyt->StringSubscript.Expr->Expr.Parent=yyt->StringSubscript.Parent;
yyVisit1 (yyt->StringSubscript.Expr);
yyt->StringSubscript.Next->StringElement.Parent=yyt->StringSubscript.Parent;
yyVisit1 (yyt->StringSubscript.Next);
break;

case kStringRange:
yyt->StringRange.For->Expr.Parent=yyt->StringRange.Parent;
yyVisit1 (yyt->StringRange.For);
yyt->StringRange.From->Expr.Parent=yyt->StringRange.Parent;
yyVisit1 (yyt->StringRange.From);
yyt->StringRange.Next->StringElement.Parent=yyt->StringRange.Parent;
yyVisit1 (yyt->StringRange.Next);
break;

case kElemental:
break;

case kTableElement:
break;

case kTable:
yyt->Table.ExprList->ExprList.Parent=yyt->Table.Parent;
yyVisit1 (yyt->Table.ExprList);
break;

case kUserLiteral:
/* line 890 "" */
 yyt->UserLiteral.Type->Type.D = 0;
yyVisit1 (yyt->UserLiteral.Type);
yyt->UserLiteral.ExprList->ExprList.Parent=yyt->UserLiteral.Parent;
yyVisit1 (yyt->UserLiteral.ExprList);
break;

case kTableExpr:
yyt->TableExpr.Expr->Expr.Parent=yyt->TableExpr.Parent;
yyVisit1 (yyt->TableExpr.Expr);
break;

case kTableSubscript:
yyt->TableSubscript.Expr->Expr.Parent=yyt->TableSubscript.Parent;
yyVisit1 (yyt->TableSubscript.Expr);
yyt->TableSubscript.Next->TableElement.Parent=yyt->TableSubscript.Parent;
yyVisit1 (yyt->TableSubscript.Next);
break;

case kTableRange:
yyt->TableRange.For->Expr.Parent=yyt->TableRange.Parent;
yyVisit1 (yyt->TableRange.For);
yyt->TableRange.From->Expr.Parent=yyt->TableRange.Parent;
yyVisit1 (yyt->TableRange.From);
yyt->TableRange.Next->TableElement.Parent=yyt->TableRange.Parent;
yyVisit1 (yyt->TableRange.Next);
break;

case kElement:
break;

case kIdentifier:
/* line 944 "" */
if (! ( false )) { if (IsLVIdent(SELF)) { AddWVUsage(yyt->Identifier.Sym,yyt->Identifier.Parent);} else {AddRVUsage(yyt->Identifier.Sym,yyt->Identifier.Parent);} ; }
break;

case kUnmappedIdentifier:
/* line 944 "" */
if (! ( false )) { if (IsLVIdent(SELF)) { AddWVUsage(yyt->UnmappedIdentifier.Sym,yyt->UnmappedIdentifier.Parent);} else {AddRVUsage(yyt->UnmappedIdentifier.Sym,yyt->UnmappedIdentifier.Parent);} ; }
break;

case kElementAccess:
yyt->ElementAccess.Next->Element.Parent=yyt->ElementAccess.Parent;
yyVisit1 (yyt->ElementAccess.Next);
break;

case kSubscript:
yyt->Subscript.Expr->Expr.Parent=yyt->Subscript.Parent;
yyVisit1 (yyt->Subscript.Expr);
yyt->Subscript.Next->Element.Parent=yyt->Subscript.Parent;
yyVisit1 (yyt->Subscript.Next);
break;

case kRange:
yyt->Range.For->Expr.Parent=yyt->Range.Parent;
yyVisit1 (yyt->Range.For);
yyt->Range.From->Expr.Parent=yyt->Range.Parent;
yyVisit1 (yyt->Range.From);
yyt->Range.Next->Element.Parent=yyt->Range.Parent;
yyVisit1 (yyt->Range.Next);
break;

case kExprList:
break;

case kNoExpr:
break;

case kAnExpr:
yyt->AnExpr.Next->ExprList.Parent=yyt->AnExpr.Parent;
yyVisit1 (yyt->AnExpr.Next);
yyt->AnExpr.Expr->Expr.Parent=yyt->AnExpr.Parent;
yyVisit1 (yyt->AnExpr.Expr);
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
/* line 927 "" */
 yyt->Process.SpecList->SpecList.Parent = yyt->Process.Parent;
/* line 970 "" */
 yyt->Process.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Process.SpecList);
/* line 788 "" */
 yyt->Process.SLC = (yyt->Process.SpecList->SpecList.DCL > 0);
break;

case kAssign:
/* line 927 "" */
 yyt->Assign.SpecList->SpecList.Parent = yyt->Assign.Parent;
yyt->Assign.Rval->ExprList.Parent=yyt->Assign.Parent;
yyVisit1 (yyt->Assign.Rval);
yyt->Assign.Lval->ExprList.Parent=yyt->Assign.Parent;
yyVisit1 (yyt->Assign.Lval);
/* line 970 "" */
 yyt->Assign.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Assign.SpecList);
/* line 788 "" */
 yyt->Assign.SLC = (yyt->Assign.SpecList->SpecList.DCL > 0);
break;

case kAltType:
/* line 970 "" */
 yyt->AltType.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->AltType.SpecList->SpecList.Parent = yyt->AltType.Parent;
yyVisit1 (yyt->AltType.SpecList);
/* line 837 "" */
 yyt->AltType.ARC = 0;
/* line 788 "" */
 yyt->AltType.SLC = (yyt->AltType.SpecList->SpecList.DCL > 0);
break;

case kAlt:
yyt->Alt.AltList->AltList.Parent=yyt->Alt.Parent;
yyVisit1 (yyt->Alt.AltList);
/* line 970 "" */
 yyt->Alt.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->Alt.SpecList->SpecList.Parent = yyt->Alt.Parent;
yyVisit1 (yyt->Alt.SpecList);
/* line 839 "" */
 yyt->Alt.ARC = yyt->Alt.AltList->AltList.ARC;
/* line 790 "" */
 yyt->Alt.SpecList->SpecList.DCL+=(yyt->Alt.TLA ? yyt->Alt.ARC:0); yyt->Alt.SLC = (yyt->Alt.SpecList->SpecList.DCL > 0) + yyt->Alt.AltList->AltList.SLC; 
break;

case kAltRep:
yyt->AltRep.AltList->AltList.Parent=yyt->AltRep.Parent;
yyVisit1 (yyt->AltRep.AltList);
/* line 978 "" */
 yyt->AltRep.Replicator->Replicator.AtomicSpec = false;
yyt->AltRep.Replicator->Replicator.Parent=yyt->AltRep.Parent;
yyVisit1 (yyt->AltRep.Replicator);
/* line 970 "" */
 yyt->AltRep.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->AltRep.SpecList->SpecList.Parent = yyt->AltRep.Parent;
yyVisit1 (yyt->AltRep.SpecList);
/* line 841 "" */
 yyt->AltRep.ARC = CountAltRepDecls(yyt->AltRep.Replicator) + yyt->AltRep.AltList->AltList.ARC;
/* line 791 "" */
 yyt->AltRep.SpecList->SpecList.DCL+=(yyt->AltRep.TLA ? yyt->AltRep.ARC:0); yyt->AltRep.SLC = (yyt->AltRep.SpecList->SpecList.DCL > 0) + yyt->AltRep.AltList->AltList.SLC; 
break;

case kParType:
/* line 970 "" */
 yyt->ParType.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->ParType.SpecList->SpecList.Parent = yyt->ParType.Parent;
yyVisit1 (yyt->ParType.SpecList);
/* line 788 "" */
 yyt->ParType.SLC = (yyt->ParType.SpecList->SpecList.DCL > 0);
break;

case kPar:
yyt->Par.ProcList->ProcList.Parent=yyt->Par.Parent;
yyVisit1 (yyt->Par.ProcList);
/* line 970 "" */
 yyt->Par.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->Par.SpecList->SpecList.Parent = yyt->Par.Parent;
yyVisit1 (yyt->Par.SpecList);
/* line 795 "" */
 yyt->Par.SLC = (yyt->Par.SpecList->SpecList.DCL > 0) + yyt->Par.ProcList->ProcList.SLC;
break;

case kParRep:
yyt->ParRep.Process->Process.Parent=yyt->ParRep.Parent;
/* line 852 "" */
 yyt->ParRep.Process->Process.TLA = Tree_IsType(yyt->ParRep.Process,kAltType);
yyVisit1 (yyt->ParRep.Process);
/* line 979 "" */
 yyt->ParRep.Replicator->Replicator.AtomicSpec = true;
yyt->ParRep.Replicator->Replicator.Parent=yyt->ParRep.Parent;
yyVisit1 (yyt->ParRep.Replicator);
/* line 970 "" */
 yyt->ParRep.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->ParRep.SpecList->SpecList.Parent = yyt->ParRep.Parent;
yyVisit1 (yyt->ParRep.SpecList);
/* line 804 "" */
 yyt->ParRep.SpecList->SpecList.DCL+=CountRepDecls(yyt->ParRep.Replicator);    yyt->ParRep.SLC = (yyt->ParRep.SpecList->SpecList.DCL > 0) + yyt->ParRep.Process->Process.SLC; 
break;

case kProcessor:
yyt->Processor.Process->Process.Parent=yyt->Processor.Parent;
yyt->Processor.Process->Process.TLA=yyt->Processor.TLA;
yyVisit1 (yyt->Processor.Process);
yyt->Processor.Expr->Expr.Parent=yyt->Processor.Parent;
yyVisit1 (yyt->Processor.Expr);
/* line 970 "" */
 yyt->Processor.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->Processor.SpecList->SpecList.Parent = yyt->Processor.Parent;
yyVisit1 (yyt->Processor.SpecList);
/* line 788 "" */
 yyt->Processor.SLC = (yyt->Processor.SpecList->SpecList.DCL > 0);
break;

case kIfType:
/* line 970 "" */
 yyt->IfType.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->IfType.SpecList->SpecList.Parent = yyt->IfType.Parent;
yyVisit1 (yyt->IfType.SpecList);
/* line 788 "" */
 yyt->IfType.SLC = (yyt->IfType.SpecList->SpecList.DCL > 0);
break;

case kIf:
yyt->If.ChoiceList->ChoiceList.Parent=yyt->If.Parent;
yyVisit1 (yyt->If.ChoiceList);
/* line 970 "" */
 yyt->If.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->If.SpecList->SpecList.Parent = yyt->If.Parent;
yyVisit1 (yyt->If.SpecList);
/* line 796 "" */
 yyt->If.SLC = (yyt->If.SpecList->SpecList.DCL > 0) + yyt->If.ChoiceList->ChoiceList.SLC;
break;

case kIfRep:
yyt->IfRep.ChoiceList->ChoiceList.Parent=yyt->IfRep.Parent;
yyVisit1 (yyt->IfRep.ChoiceList);
/* line 977 "" */
 yyt->IfRep.Replicator->Replicator.AtomicSpec = IsAtomic(SELF);
yyt->IfRep.Replicator->Replicator.Parent=yyt->IfRep.Parent;
yyVisit1 (yyt->IfRep.Replicator);
/* line 970 "" */
 yyt->IfRep.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->IfRep.SpecList->SpecList.Parent = yyt->IfRep.Parent;
yyVisit1 (yyt->IfRep.SpecList);
/* line 805 "" */
 yyt->IfRep.SpecList->SpecList.DCL+=CountRepDecls(yyt->IfRep.Replicator);    yyt->IfRep.SLC = (yyt->IfRep.SpecList->SpecList.DCL > 0) + yyt->IfRep.ChoiceList->ChoiceList.SLC; 
break;

case kInput:
/* line 927 "" */
 yyt->Input.SpecList->SpecList.Parent = yyt->Input.Parent;
yyt->Input.ExprList->ExprList.Parent=yyt->Input.Parent;
yyVisit1 (yyt->Input.ExprList);
yyt->Input.Element->Element.Parent=yyt->Input.Parent;
yyVisit1 (yyt->Input.Element);
/* line 970 "" */
 yyt->Input.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Input.SpecList);
/* line 788 "" */
 yyt->Input.SLC = (yyt->Input.SpecList->SpecList.DCL > 0);
break;

case kOutput:
/* line 927 "" */
 yyt->Output.SpecList->SpecList.Parent = yyt->Output.Parent;
yyt->Output.ExprList->ExprList.Parent=yyt->Output.Parent;
yyVisit1 (yyt->Output.ExprList);
yyt->Output.Element->Element.Parent=yyt->Output.Parent;
yyVisit1 (yyt->Output.Element);
/* line 970 "" */
 yyt->Output.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Output.SpecList);
/* line 788 "" */
 yyt->Output.SLC = (yyt->Output.SpecList->SpecList.DCL > 0);
break;

case kCaseIn:
/* line 927 "" */
 yyt->CaseIn.SpecList->SpecList.Parent = yyt->CaseIn.Parent;
yyt->CaseIn.CaseList->CaseList.Parent=yyt->CaseIn.Parent;
yyVisit1 (yyt->CaseIn.CaseList);
yyt->CaseIn.Element->Element.Parent=yyt->CaseIn.Parent;
yyVisit1 (yyt->CaseIn.Element);
/* line 970 "" */
 yyt->CaseIn.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->CaseIn.SpecList);
/* line 793 "" */
 yyt->CaseIn.SLC = (yyt->CaseIn.SpecList->SpecList.DCL > 0) + yyt->CaseIn.CaseList->CaseList.SLC;
break;

case kAfter:
/* line 927 "" */
 yyt->After.SpecList->SpecList.Parent = yyt->After.Parent;
yyt->After.Expr->Expr.Parent=yyt->After.Parent;
yyVisit1 (yyt->After.Expr);
yyt->After.Element->Element.Parent=yyt->After.Parent;
yyVisit1 (yyt->After.Element);
/* line 970 "" */
 yyt->After.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->After.SpecList);
/* line 788 "" */
 yyt->After.SLC = (yyt->After.SpecList->SpecList.DCL > 0);
break;

case kSeqRep:
/* line 927 "" */
 yyt->SeqRep.SpecList->SpecList.Parent = yyt->SeqRep.Parent;
yyt->SeqRep.Process->Process.Parent=yyt->SeqRep.Parent;
/* line 851 "" */
 yyt->SeqRep.Process->Process.TLA = Tree_IsType(yyt->SeqRep.Process,kAltType);
yyVisit1 (yyt->SeqRep.Process);
/* line 976 "" */
 yyt->SeqRep.Replicator->Replicator.AtomicSpec = IsAtomic(yyt->SeqRep.Process);
yyt->SeqRep.Replicator->Replicator.Parent=yyt->SeqRep.Parent;
yyVisit1 (yyt->SeqRep.Replicator);
/* line 970 "" */
 yyt->SeqRep.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->SeqRep.SpecList);
/* line 803 "" */
 yyt->SeqRep.SpecList->SpecList.DCL+=CountRepDecls(yyt->SeqRep.Replicator);    yyt->SeqRep.SLC = (yyt->SeqRep.SpecList->SpecList.DCL > 0) + yyt->SeqRep.Process->Process.SLC; 
break;

case kSeq:
/* line 927 "" */
 yyt->Seq.SpecList->SpecList.Parent = yyt->Seq.Parent;
yyt->Seq.ProcList->ProcList.Parent=yyt->Seq.Parent;
yyVisit1 (yyt->Seq.ProcList);
/* line 970 "" */
 yyt->Seq.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Seq.SpecList);
/* line 794 "" */
 yyt->Seq.SLC = (yyt->Seq.SpecList->SpecList.DCL > 0) + yyt->Seq.ProcList->ProcList.SLC;
break;

case kCase:
/* line 927 "" */
 yyt->Case.SpecList->SpecList.Parent = yyt->Case.Parent;
yyt->Case.ChoiceList->ChoiceList.Parent=yyt->Case.Parent;
yyVisit1 (yyt->Case.ChoiceList);
yyt->Case.Selector->Expr.Parent=yyt->Case.Parent;
yyVisit1 (yyt->Case.Selector);
/* line 970 "" */
 yyt->Case.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Case.SpecList);
/* line 797 "" */
 yyt->Case.SLC = (yyt->Case.SpecList->SpecList.DCL > 0) + yyt->Case.ChoiceList->ChoiceList.SLC;
break;

case kWhile:
/* line 927 "" */
 yyt->While.SpecList->SpecList.Parent = yyt->While.Parent;
yyt->While.Process->Process.Parent=yyt->While.Parent;
/* line 853 "" */
 yyt->While.Process->Process.TLA = Tree_IsType(yyt->While.Process,kAltType);
yyVisit1 (yyt->While.Process);
yyt->While.Expr->Expr.Parent=yyt->While.Parent;
yyVisit1 (yyt->While.Expr);
/* line 970 "" */
 yyt->While.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->While.SpecList);
/* line 798 "" */
 yyt->While.SLC = (yyt->While.SpecList->SpecList.DCL > 0) + yyt->While.Process->Process.SLC;
break;

case kSkip:
/* line 927 "" */
 yyt->Skip.SpecList->SpecList.Parent = yyt->Skip.Parent;
/* line 970 "" */
 yyt->Skip.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Skip.SpecList);
/* line 799 "" */
 yyt->Skip.SLC = (yyt->Skip.SpecList->SpecList.DCL > 0);
break;

case kStop:
/* line 927 "" */
 yyt->Stop.SpecList->SpecList.Parent = yyt->Stop.Parent;
/* line 970 "" */
 yyt->Stop.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->Stop.SpecList);
/* line 800 "" */
 yyt->Stop.SLC = (yyt->Stop.SpecList->SpecList.DCL > 0);
break;

case kInstance:
/* line 945 "" */
if (! ( false )) { AddVUsage(yyt->Instance.Sym,xxAccess); ; }
yyt->Instance.ExprList->ExprList.Parent=yyt->Instance.Parent;
yyVisit1 (yyt->Instance.ExprList);
/* line 970 "" */
 yyt->Instance.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->Instance.SpecList->SpecList.Parent = yyt->Instance.Parent;
yyVisit1 (yyt->Instance.SpecList);
/* line 801 "" */
 yyt->Instance.SLC = (yyt->Instance.SpecList->SpecList.DCL > 0);
break;

case kCCode:
/* line 947 "" */
if (! ( false )) { MarkCCodeUsage(yyt->CCode.Code,yyt->CCode.Pos,yyt->CCode.Scope); ; }
/* line 970 "" */
 yyt->CCode.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
/* line 927 "" */
 yyt->CCode.SpecList->SpecList.Parent = yyt->CCode.Parent;
yyVisit1 (yyt->CCode.SpecList);
/* line 788 "" */
 yyt->CCode.SLC = (yyt->CCode.SpecList->SpecList.DCL > 0);
break;

case kNullProcess:
/* line 927 "" */
 yyt->NullProcess.SpecList->SpecList.Parent = yyt->NullProcess.Parent;
/* line 970 "" */
 yyt->NullProcess.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->NullProcess.SpecList);
/* line 788 "" */
 yyt->NullProcess.SLC = (yyt->NullProcess.SpecList->SpecList.DCL > 0);
break;

case kAltList:
/* line 836 "" */
 yyt->AltList.ARC = 0;
/* line 782 "" */
 yyt->AltList.SLC = 0;
break;

case kNoAlt:
/* line 843 "" */
 yyt->NoAlt.ARC = 0;
/* line 812 "" */
 yyt->NoAlt.SLC = 0;
break;

case kAnAlts:
yyt->AnAlts.SpecList->SpecList.Parent=yyt->AnAlts.Parent;
yyt->AnAlts.Next->AltList.Parent=yyt->AnAlts.Parent;
yyVisit1 (yyt->AnAlts.Next);
/* line 972 "" */
 yyt->AnAlts.SpecList->SpecList.AtomicSpec = false;
yyVisit1 (yyt->AnAlts.SpecList);
/* line 836 "" */
 yyt->AnAlts.ARC = 0;
/* line 782 "" */
 yyt->AnAlts.SLC = 0;
break;

case kAnAlt:
yyt->AnAlt.Guard->Guard.Parent=yyt->AnAlt.Parent;
yyVisit1 (yyt->AnAlt.Guard);
yyt->AnAlt.Next->AltList.Parent=yyt->AnAlt.Parent;
yyVisit1 (yyt->AnAlt.Next);
/* line 972 "" */
 yyt->AnAlt.SpecList->SpecList.AtomicSpec = false;
yyt->AnAlt.SpecList->SpecList.Parent=yyt->AnAlt.Parent;
yyVisit1 (yyt->AnAlt.SpecList);
/* line 844 "" */
 yyt->AnAlt.ARC = yyt->AnAlt.Next->AltList.ARC;
/* line 813 "" */
 yyt->AnAlt.SLC = yyt->AnAlt.Next->AltList.SLC + (yyt->AnAlt.SpecList->SpecList.DCL > 0 || yyt->AnAlt.Guard->Guard.SLC   > 0) ;
break;

case kANewAlt:
yyt->ANewAlt.SpecList->SpecList.Parent=yyt->ANewAlt.Parent;
yyt->ANewAlt.AltType->AltType.Parent=yyt->ANewAlt.Parent;
/* line 868 "" */
 yyt->ANewAlt.AltType->AltType.TLA = false;
yyVisit1 (yyt->ANewAlt.AltType);
yyt->ANewAlt.Next->AltList.Parent=yyt->ANewAlt.Parent;
yyVisit1 (yyt->ANewAlt.Next);
/* line 972 "" */
 yyt->ANewAlt.SpecList->SpecList.AtomicSpec = false;
yyVisit1 (yyt->ANewAlt.SpecList);
/* line 845 "" */
 yyt->ANewAlt.ARC = yyt->ANewAlt.Next->AltList.ARC + yyt->ANewAlt.AltType->AltType.ARC;
/* line 814 "" */
 yyt->ANewAlt.SLC = yyt->ANewAlt.Next->AltList.SLC + (yyt->ANewAlt.SpecList->SpecList.DCL > 0 || yyt->ANewAlt.AltType->AltType.SLC > 0) ;
break;

case kGuard:
yyt->Guard.Expr->Expr.Parent=yyt->Guard.Parent;
yyVisit1 (yyt->Guard.Expr);
/* line 783 "" */
 yyt->Guard.SLC = 0;
break;

case kAltInput:
yyt->AltInput.Process->Process.Parent=yyt->AltInput.Parent;
/* line 855 "" */
 yyt->AltInput.Process->Process.TLA = Tree_IsType(yyt->AltInput.Process,kAltType);
yyVisit1 (yyt->AltInput.Process);
yyt->AltInput.Input->Input.Parent=yyt->AltInput.Parent;
/* line 854 "" */
 yyt->AltInput.Input->Input.TLA   = false;
yyVisit1 (yyt->AltInput.Input);
yyt->AltInput.Expr->Expr.Parent=yyt->AltInput.Parent;
yyVisit1 (yyt->AltInput.Expr);
/* line 807 "" */
 yyt->AltInput.SLC = yyt->AltInput.Input->Input.SLC + yyt->AltInput.Process->Process.SLC;
break;

case kAltTimer:
yyt->AltTimer.Process->Process.Parent=yyt->AltTimer.Parent;
/* line 857 "" */
 yyt->AltTimer.Process->Process.TLA = Tree_IsType(yyt->AltTimer.Process,kAltType);
yyVisit1 (yyt->AltTimer.Process);
yyt->AltTimer.After->After.Parent=yyt->AltTimer.Parent;
/* line 856 "" */
 yyt->AltTimer.After->After.TLA   = false;
yyVisit1 (yyt->AltTimer.After);
yyt->AltTimer.Expr->Expr.Parent=yyt->AltTimer.Parent;
yyVisit1 (yyt->AltTimer.Expr);
/* line 808 "" */
 yyt->AltTimer.SLC = yyt->AltTimer.After->After.SLC + yyt->AltTimer.Process->Process.SLC;
break;

case kAltCaseIn:
yyt->AltCaseIn.CaseIn->CaseIn.Parent=yyt->AltCaseIn.Parent;
/* line 858 "" */
 yyt->AltCaseIn.CaseIn->CaseIn.TLA  = false;
yyVisit1 (yyt->AltCaseIn.CaseIn);
yyt->AltCaseIn.Expr->Expr.Parent=yyt->AltCaseIn.Parent;
yyVisit1 (yyt->AltCaseIn.Expr);
/* line 809 "" */
 yyt->AltCaseIn.SLC = yyt->AltCaseIn.CaseIn->CaseIn.SLC;
break;

case kNoInput:
yyt->NoInput.Process->Process.Parent=yyt->NoInput.Parent;
/* line 859 "" */
 yyt->NoInput.Process->Process.TLA = Tree_IsType(yyt->NoInput.Process,kAltType);
yyVisit1 (yyt->NoInput.Process);
yyt->NoInput.Expr->Expr.Parent=yyt->NoInput.Parent;
yyVisit1 (yyt->NoInput.Expr);
/* line 810 "" */
 yyt->NoInput.SLC = yyt->NoInput.Process->Process.SLC;
break;

case kChoiceList:
/* line 784 "" */
 yyt->ChoiceList.SLC = 0;
break;

case kNoChoice:
/* line 816 "" */
 yyt->NoChoice.SLC = 0;
break;

case kAChoices:
yyt->AChoices.SpecList->SpecList.Parent=yyt->AChoices.Parent;
yyt->AChoices.Next->ChoiceList.Parent=yyt->AChoices.Parent;
yyVisit1 (yyt->AChoices.Next);
/* line 973 "" */
 yyt->AChoices.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->AChoices.SpecList);
/* line 784 "" */
 yyt->AChoices.SLC = 0;
break;

case kAChoice:
yyt->AChoice.Process->Process.Parent=yyt->AChoice.Parent;
/* line 860 "" */
 yyt->AChoice.Process->Process.TLA = Tree_IsType(yyt->AChoice.Process,kAltType);
yyVisit1 (yyt->AChoice.Process);
yyt->AChoice.ExprList->ExprList.Parent=yyt->AChoice.Parent;
yyVisit1 (yyt->AChoice.ExprList);
yyt->AChoice.Next->ChoiceList.Parent=yyt->AChoice.Parent;
yyVisit1 (yyt->AChoice.Next);
/* line 973 "" */
 yyt->AChoice.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyt->AChoice.SpecList->SpecList.Parent=yyt->AChoice.Parent;
yyVisit1 (yyt->AChoice.SpecList);
/* line 817 "" */
 yyt->AChoice.SLC = yyt->AChoice.Next->ChoiceList.SLC + (yyt->AChoice.SpecList->SpecList.DCL > 0 || yyt->AChoice.Process->Process.SLC > 0) ;
break;

case kANewIf:
yyt->ANewIf.SpecList->SpecList.Parent=yyt->ANewIf.Parent;
yyt->ANewIf.IfType->IfType.Parent=yyt->ANewIf.Parent;
/* line 861 "" */
 yyt->ANewIf.IfType->IfType.TLA  = false;
yyVisit1 (yyt->ANewIf.IfType);
yyt->ANewIf.Next->ChoiceList.Parent=yyt->ANewIf.Parent;
yyVisit1 (yyt->ANewIf.Next);
/* line 973 "" */
 yyt->ANewIf.SpecList->SpecList.AtomicSpec = IsAtomic(SELF);
yyVisit1 (yyt->ANewIf.SpecList);
/* line 818 "" */
 yyt->ANewIf.SLC = yyt->ANewIf.Next->ChoiceList.SLC + (yyt->ANewIf.SpecList->SpecList.DCL > 0 || yyt->ANewIf.IfType->IfType.SLC  > 0) ;
break;

case kProcList:
/* line 785 "" */
 yyt->ProcList.SLC = 0;
break;

case kNoProcess:
/* line 820 "" */
 yyt->NoProcess.SLC = 0;
break;

case kAProcess:
yyt->AProcess.Process->Process.Parent=yyt->AProcess.Parent;
/* line 862 "" */
 yyt->AProcess.Process->Process.TLA = Tree_IsType(yyt->AProcess.Process,kAltType);
yyVisit1 (yyt->AProcess.Process);
yyt->AProcess.Next->ProcList.Parent=yyt->AProcess.Parent;
yyVisit1 (yyt->AProcess.Next);
/* line 821 "" */
 yyt->AProcess.SLC = yyt->AProcess.Next->ProcList.SLC + (yyt->AProcess.Process->Process.SLC > 0);
break;

case kCaseList:
/* line 786 "" */
 yyt->CaseList.SLC = 0;
break;

case kNoCase:
/* line 823 "" */
 yyt->NoCase.SLC = 0;
break;

case kACase:
yyt->ACase.Process->Process.Parent=yyt->ACase.Parent;
/* line 863 "" */
 yyt->ACase.Process->Process.TLA = Tree_IsType(yyt->ACase.Process,kAltType);
yyVisit1 (yyt->ACase.Process);
yyt->ACase.ExprList->ExprList.Parent=yyt->ACase.Parent;
yyVisit1 (yyt->ACase.ExprList);
/* line 974 "" */
 yyt->ACase.SpecList->SpecList.AtomicSpec = false;
yyt->ACase.SpecList->SpecList.Parent=yyt->ACase.Parent;
yyVisit1 (yyt->ACase.SpecList);
yyt->ACase.Next->CaseList.Parent=yyt->ACase.Parent;
yyVisit1 (yyt->ACase.Next);
/* line 824 "" */
 yyt->ACase.SLC = yyt->ACase.Next->CaseList.SLC + (yyt->ACase.SpecList->SpecList.DCL > 0 || yyt->ACase.Process->Process.SLC > 0);
break;
 default: ;
 }
}

void BeginCoutAg ()
{
}

void CloseCoutAg ()
{
}
