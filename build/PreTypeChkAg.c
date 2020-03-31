# define DEP(a, b) a
# define SELF yyt
# include "PreTypeChkAg.h"


static char yyb;

static void yyVisit1 ARGS((register tTree yyt));

void PreTypeChkAg
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
/* line 627 "" */
 yyt->Start.Process->Process.V1In = 0;
yyVisit1 (yyt->Start.Process);
break;

case kSpecList:
yyt->SpecList.V1Out=yyt->SpecList.V1In;
break;

case kNoSpec:
yyt->NoSpec.V1Out=yyt->NoSpec.V1In;
break;

case kASpec:
yyt->ASpec.Spec->Spec.V1In=yyt->ASpec.V1In;
yyVisit1 (yyt->ASpec.Spec);
yyt->ASpec.Next->SpecList.V1In=yyt->ASpec.Spec->Spec.V1Out;
yyVisit1 (yyt->ASpec.Next);
yyt->ASpec.V1Out=yyt->ASpec.Next->SpecList.V1Out;
break;

case kSpec:
yyt->Spec.V1Out=yyt->Spec.V1In;
break;

case kDeclType:
yyt->DeclType.Type->Type.V1In=yyt->DeclType.V1In;
yyVisit1 (yyt->DeclType.Type);
yyt->DeclType.V1Out=yyt->DeclType.Type->Type.V1Out;
break;

case kDecl:
yyVisit1 (yyt->Decl.IdList);
yyt->Decl.Type->Type.V1In=yyt->Decl.V1In;
yyVisit1 (yyt->Decl.Type);
yyt->Decl.V1Out=yyt->Decl.Type->Type.V1Out;
break;

case kIs:
yyt->Is.Type->Type.V1In=yyt->Is.V1In;
yyVisit1 (yyt->Is.Type);
yyt->Is.Elemental->Elemental.V1In=yyt->Is.Type->Type.V1Out;
/* line 682 "" */
 yyt->Is.Elemental->Elemental.FLIn = NoTree;
yyVisit1 (yyt->Is.Elemental);
/* line 683 "" */
if (! ( (SymbolType(yyt->Is.Sym)!=NoTree) )) { SymbolType(yyt->Is.Sym) = yyt->Is.Elemental->Elemental.DefType ; }
yyt->Is.V1Out=yyt->Is.Elemental->Elemental.V1Out;
break;

case kAVal:
yyt->AVal.Type->Type.V1In=yyt->AVal.V1In;
yyVisit1 (yyt->AVal.Type);
yyt->AVal.Expr->Expr.V1In=yyt->AVal.Type->Type.V1Out;
/* line 679 "" */
 yyt->AVal.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->AVal.Expr);
/* line 680 "" */
if (! ( (SymbolType(yyt->AVal.Sym)!=NoTree) )) { SymbolType(yyt->AVal.Sym) = yyt->AVal.Expr->Expr.DefType ; }
yyt->AVal.V1Out=yyt->AVal.Expr->Expr.V1Out;
break;

case kVal:
yyt->Val.Type->Type.V1In=yyt->Val.V1In;
yyVisit1 (yyt->Val.Type);
yyt->Val.Expr->Expr.V1In=yyt->Val.Type->Type.V1Out;
/* line 679 "" */
 yyt->Val.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->Val.Expr);
/* line 680 "" */
if (! ( (SymbolType(yyt->Val.Sym)!=NoTree) )) { SymbolType(yyt->Val.Sym) = yyt->Val.Expr->Expr.DefType ; }
yyt->Val.V1Out=yyt->Val.Expr->Expr.V1Out;
break;

case kValRetypes:
yyt->ValRetypes.Type->Type.V1In=yyt->ValRetypes.V1In;
yyVisit1 (yyt->ValRetypes.Type);
yyt->ValRetypes.Expr->Expr.V1In=yyt->ValRetypes.Type->Type.V1Out;
/* line 679 "" */
 yyt->ValRetypes.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->ValRetypes.Expr);
/* line 680 "" */
if (! ( (SymbolType(yyt->ValRetypes.Sym)!=NoTree) )) { SymbolType(yyt->ValRetypes.Sym) = yyt->ValRetypes.Expr->Expr.DefType ; }
yyt->ValRetypes.V1Out=yyt->ValRetypes.Expr->Expr.V1Out;
break;

case kValReshapes:
yyt->ValReshapes.Type->Type.V1In=yyt->ValReshapes.V1In;
yyVisit1 (yyt->ValReshapes.Type);
yyt->ValReshapes.Expr->Expr.V1In=yyt->ValReshapes.Type->Type.V1Out;
/* line 679 "" */
 yyt->ValReshapes.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->ValReshapes.Expr);
/* line 680 "" */
if (! ( (SymbolType(yyt->ValReshapes.Sym)!=NoTree) )) { SymbolType(yyt->ValReshapes.Sym) = yyt->ValReshapes.Expr->Expr.DefType ; }
yyt->ValReshapes.V1Out=yyt->ValReshapes.Expr->Expr.V1Out;
break;

case kRetypes:
yyt->Retypes.Type->Type.V1In=yyt->Retypes.V1In;
yyVisit1 (yyt->Retypes.Type);
yyt->Retypes.Elemental->Elemental.V1In=yyt->Retypes.Type->Type.V1Out;
/* line 684 "" */
 yyt->Retypes.Elemental->Elemental.FLIn = NoTree;
yyVisit1 (yyt->Retypes.Elemental);
yyt->Retypes.V1Out=yyt->Retypes.Elemental->Elemental.V1Out;
break;

case kReshapes:
yyt->Reshapes.Type->Type.V1In=yyt->Reshapes.V1In;
yyVisit1 (yyt->Reshapes.Type);
yyt->Reshapes.Elemental->Elemental.V1In=yyt->Reshapes.Type->Type.V1Out;
/* line 684 "" */
 yyt->Reshapes.Elemental->Elemental.FLIn = NoTree;
yyVisit1 (yyt->Reshapes.Elemental);
yyt->Reshapes.V1Out=yyt->Reshapes.Elemental->Elemental.V1Out;
break;

case kDefAPro:
yyt->DefAPro.V1Out=yyt->DefAPro.V1In;
break;

case kDefSeqPro:
yyVisit1 (yyt->DefSeqPro.SeqProList);
yyt->DefSeqPro.V1Out=yyt->DefSeqPro.V1In;
break;

case kDefTagPro:
yyVisit1 (yyt->DefTagPro.TagProList);
yyt->DefTagPro.V1Out=yyt->DefTagPro.V1In;
break;

case kDefAProc:
yyt->DefAProc.Formals->Formals.V1In=yyt->DefAProc.V1In;
yyVisit1 (yyt->DefAProc.Formals);
yyt->DefAProc.V1Out=yyt->DefAProc.Formals->Formals.V1Out;
break;

case kDefProc:
yyt->DefProc.Formals->Formals.V1In=yyt->DefProc.V1In;
yyVisit1 (yyt->DefProc.Formals);
yyt->DefProc.Process->Process.V1In=yyt->DefProc.Formals->Formals.V1Out;
yyVisit1 (yyt->DefProc.Process);
yyt->DefProc.V1Out=yyt->DefProc.Process->Process.V1Out;
break;

case kInlineDefProc:
yyt->InlineDefProc.Formals->Formals.V1In=yyt->InlineDefProc.V1In;
yyVisit1 (yyt->InlineDefProc.Formals);
yyt->InlineDefProc.Process->Process.V1In=yyt->InlineDefProc.Formals->Formals.V1Out;
yyVisit1 (yyt->InlineDefProc.Process);
yyt->InlineDefProc.V1Out=yyt->InlineDefProc.Process->Process.V1Out;
break;

case kPrototypeProc:
yyt->PrototypeProc.Formals->Formals.V1In=yyt->PrototypeProc.V1In;
yyVisit1 (yyt->PrototypeProc.Formals);
yyt->PrototypeProc.V1Out=yyt->PrototypeProc.Formals->Formals.V1Out;
break;

case kDefAFunc:
yyt->DefAFunc.TypeList->TypeList.V1In=yyt->DefAFunc.V1In;
yyVisit1 (yyt->DefAFunc.TypeList);
yyt->DefAFunc.Formals->Formals.V1In=yyt->DefAFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->DefAFunc.Formals);
yyt->DefAFunc.V1Out=yyt->DefAFunc.Formals->Formals.V1Out;
break;

case kDefFunc:
yyt->DefFunc.TypeList->TypeList.V1In=yyt->DefFunc.V1In;
yyVisit1 (yyt->DefFunc.TypeList);
yyt->DefFunc.Formals->Formals.V1In=yyt->DefFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->DefFunc.Formals);
yyt->DefFunc.Valof->Valof.V1In=yyt->DefFunc.Formals->Formals.V1Out;
/* line 685 "" */
 yyt->DefFunc.Valof->Valof.FLIn = NoTree;
yyVisit1 (yyt->DefFunc.Valof);
yyt->DefFunc.V1Out=yyt->DefFunc.Valof->Valof.V1Out;
break;

case kInlineDefFunc:
yyt->InlineDefFunc.TypeList->TypeList.V1In=yyt->InlineDefFunc.V1In;
yyVisit1 (yyt->InlineDefFunc.TypeList);
yyt->InlineDefFunc.Formals->Formals.V1In=yyt->InlineDefFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->InlineDefFunc.Formals);
yyt->InlineDefFunc.Valof->Valof.V1In=yyt->InlineDefFunc.Formals->Formals.V1Out;
/* line 685 "" */
 yyt->InlineDefFunc.Valof->Valof.FLIn = NoTree;
yyVisit1 (yyt->InlineDefFunc.Valof);
yyt->InlineDefFunc.V1Out=yyt->InlineDefFunc.Valof->Valof.V1Out;
break;

case kDefIsFunc:
yyt->DefIsFunc.TypeList->TypeList.V1In=yyt->DefIsFunc.V1In;
yyVisit1 (yyt->DefIsFunc.TypeList);
yyt->DefIsFunc.Formals->Formals.V1In=yyt->DefIsFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->DefIsFunc.Formals);
yyt->DefIsFunc.ExprList->ExprList.V1In=yyt->DefIsFunc.Formals->Formals.V1Out;
/* line 681 "" */
 yyt->DefIsFunc.ExprList->ExprList.FLIn = NoTree;
yyVisit1 (yyt->DefIsFunc.ExprList);
yyt->DefIsFunc.V1Out=yyt->DefIsFunc.ExprList->ExprList.V1Out;
break;

case kInlineDefIsFunc:
yyt->InlineDefIsFunc.TypeList->TypeList.V1In=yyt->InlineDefIsFunc.V1In;
yyVisit1 (yyt->InlineDefIsFunc.TypeList);
yyt->InlineDefIsFunc.Formals->Formals.V1In=yyt->InlineDefIsFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->InlineDefIsFunc.Formals);
yyt->InlineDefIsFunc.ExprList->ExprList.V1In=yyt->InlineDefIsFunc.Formals->Formals.V1Out;
/* line 681 "" */
 yyt->InlineDefIsFunc.ExprList->ExprList.FLIn = NoTree;
yyVisit1 (yyt->InlineDefIsFunc.ExprList);
yyt->InlineDefIsFunc.V1Out=yyt->InlineDefIsFunc.ExprList->ExprList.V1Out;
break;

case kPrototypeFunc:
yyt->PrototypeFunc.TypeList->TypeList.V1In=yyt->PrototypeFunc.V1In;
yyVisit1 (yyt->PrototypeFunc.TypeList);
yyt->PrototypeFunc.Formals->Formals.V1In=yyt->PrototypeFunc.TypeList->TypeList.V1Out;
yyVisit1 (yyt->PrototypeFunc.Formals);
yyt->PrototypeFunc.V1Out=yyt->PrototypeFunc.Formals->Formals.V1Out;
break;

case kNullSpec:
yyt->NullSpec.V1Out=yyt->NullSpec.V1In;
break;

case kHCode:
yyt->HCode.V1Out=yyt->HCode.V1In;
break;

case kCallFrame:
yyt->CallFrame.V1Out=yyt->CallFrame.V1In;
break;

case kParFrame:
yyt->ParFrame.V1Out=yyt->ParFrame.V1In;
break;

case kFormals:
yyt->Formals.V1Out=yyt->Formals.V1In;
break;

case kNoFormal:
yyt->NoFormal.V1Out=yyt->NoFormal.V1In;
break;

case kNoFormals:
yyt->NoFormals.V1Out=yyt->NoFormals.V1In;
break;

case kAFormal:
yyt->AFormal.Next->Formals.V1In=yyt->AFormal.V1In;
yyVisit1 (yyt->AFormal.Next);
yyt->AFormal.Type->Type.V1In=yyt->AFormal.Next->Formals.V1Out;
yyVisit1 (yyt->AFormal.Type);
yyt->AFormal.V1Out=yyt->AFormal.Type->Type.V1Out;
yyVisit1 (yyt->AFormal.By);
break;

case kReplicator:
yyt->Replicator.From->Expr.V1In=yyt->Replicator.V1In;
/* line 687 "" */
 yyt->Replicator.From->Expr.FLIn = NoTree;
yyVisit1 (yyt->Replicator.From);
yyt->Replicator.For->Expr.V1In=yyt->Replicator.From->Expr.V1Out;
/* line 687 "" */
 yyt->Replicator.For->Expr.FLIn = NoTree;
yyVisit1 (yyt->Replicator.For);
yyt->Replicator.V1Out=yyt->Replicator.For->Expr.V1Out;
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
break;

case kType:
yyt->Type.V1Out=yyt->Type.V1In;
break;

case kArray:
yyt->Array.Expr->Expr.V1In=yyt->Array.V1In;
/* line 688 "" */
 yyt->Array.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->Array.Expr);
yyt->Array.Type->Type.V1In=yyt->Array.Expr->Expr.V1Out;
yyVisit1 (yyt->Array.Type);
yyt->Array.V1Out=yyt->Array.Type->Type.V1Out;
break;

case kUserArray:
yyt->UserArray.Expr->Expr.V1In=yyt->UserArray.V1In;
/* line 688 "" */
 yyt->UserArray.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->UserArray.Expr);
yyt->UserArray.Type->Type.V1In=yyt->UserArray.Expr->Expr.V1Out;
yyVisit1 (yyt->UserArray.Type);
yyt->UserArray.V1Out=yyt->UserArray.Type->Type.V1Out;
break;

case kChan:
yyt->Chan.V1Out=yyt->Chan.V1In;
yyVisit1 (yyt->Chan.Protocol);
break;

case kTimer:
yyt->Timer.V1Out=yyt->Timer.V1In;
break;

case kUntyped:
yyt->Untyped.V1Out=yyt->Untyped.V1In;
break;

case kUntypedByte:
yyt->UntypedByte.V1Out=yyt->UntypedByte.V1In;
break;

case kUntypedReal:
yyt->UntypedReal.V1Out=yyt->UntypedReal.V1In;
break;

case kUntypedInteger:
yyt->UntypedInteger.V1Out=yyt->UntypedInteger.V1In;
break;

case kUntypedInt:
yyt->UntypedInt.V1Out=yyt->UntypedInt.V1In;
break;

case kUntypedHex:
yyt->UntypedHex.V1Out=yyt->UntypedHex.V1In;
break;

case kPrimitive:
yyt->Primitive.V1Out=yyt->Primitive.V1In;
break;

case kNumeric:
yyt->Numeric.V1Out=yyt->Numeric.V1In;
break;

case kInteger:
yyt->Integer.V1Out=yyt->Integer.V1In;
break;

case kInt:
yyt->Int.V1Out=yyt->Int.V1In;
break;

case kUserInt:
yyt->UserInt.V1Out=yyt->UserInt.V1In;
break;

case kInt16:
yyt->Int16.V1Out=yyt->Int16.V1In;
break;

case kUserInt16:
yyt->UserInt16.V1Out=yyt->UserInt16.V1In;
break;

case kInt32:
yyt->Int32.V1Out=yyt->Int32.V1In;
break;

case kUserInt32:
yyt->UserInt32.V1Out=yyt->UserInt32.V1In;
break;

case kInt64:
yyt->Int64.V1Out=yyt->Int64.V1In;
break;

case kUserInt64:
yyt->UserInt64.V1Out=yyt->UserInt64.V1In;
break;

case kReal:
yyt->Real.V1Out=yyt->Real.V1In;
break;

case kReal32:
yyt->Real32.V1Out=yyt->Real32.V1In;
break;

case kUserReal32:
yyt->UserReal32.V1Out=yyt->UserReal32.V1In;
break;

case kReal64:
yyt->Real64.V1Out=yyt->Real64.V1In;
break;

case kUserReal64:
yyt->UserReal64.V1Out=yyt->UserReal64.V1In;
break;

case kByte:
yyt->Byte.V1Out=yyt->Byte.V1In;
break;

case kUserByte:
yyt->UserByte.V1Out=yyt->UserByte.V1In;
break;

case kBool:
yyt->Bool.V1Out=yyt->Bool.V1In;
break;

case kUserBool:
yyt->UserBool.V1Out=yyt->UserBool.V1In;
break;

case kUserStructTypes:
yyt->UserStructTypes.V1Out=yyt->UserStructTypes.V1In;
yyVisit1 (yyt->UserStructTypes.FieldList);
break;

case kUserDataTypes:
yyt->UserDataTypes.V1Out=yyt->UserDataTypes.V1In;
yyVisit1 (yyt->UserDataTypes.FieldList);
break;

case kUserRecord:
yyt->UserRecord.V1Out=yyt->UserRecord.V1In;
yyVisit1 (yyt->UserRecord.FieldList);
break;

case kUserPackedRecord:
yyt->UserPackedRecord.V1Out=yyt->UserPackedRecord.V1In;
yyVisit1 (yyt->UserPackedRecord.FieldList);
break;

case kUserUnion:
yyt->UserUnion.V1Out=yyt->UserUnion.V1In;
yyVisit1 (yyt->UserUnion.FieldList);
break;

case kUserChanRecord:
yyt->UserChanRecord.V1Out=yyt->UserChanRecord.V1In;
yyVisit1 (yyt->UserChanRecord.FieldList);
break;

case kTypeList:
yyt->TypeList.V1Out=yyt->TypeList.V1In;
break;

case kNoType:
yyt->NoType.V1Out=yyt->NoType.V1In;
break;

case kAType:
yyt->AType.Next->TypeList.V1In=yyt->AType.V1In;
yyVisit1 (yyt->AType.Next);
yyt->AType.Type->Type.V1In=yyt->AType.Next->TypeList.V1Out;
yyVisit1 (yyt->AType.Type);
yyt->AType.V1Out=yyt->AType.Type->Type.V1Out;
break;

case kFieldList:
break;

case kNoField:
break;

case kAField:
/* line 626 "" */
yyt->AField.Type->Type.V1In = 0;
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
/* line 628 "" */
yyt->Simple.Type->Type.V1In = 0;
yyVisit1 (yyt->Simple.Type);
break;

case kCounted:
/* line 629 "" */
yyt->Counted.Length->Type.V1In = 0;
yyVisit1 (yyt->Counted.Length);
yyt->Counted.Type->Type.V1In=yyt->Counted.Length->Type.V1Out;
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
yyt->Expr.V1Out=yyt->Expr.V1In;
yyt->Expr.FLOut=yyt->Expr.FLIn;
/* line 640 "" */
 yyt->Expr.DefType = NoTree;
break;

case kBinary:
yyVisit1 (yyt->Binary.Op);
yyt->Binary.Lop->Expr.V1In=yyt->Binary.V1In;
yyt->Binary.Lop->Expr.FLIn=yyt->Binary.FLIn;
yyVisit1 (yyt->Binary.Lop);
yyt->Binary.Rop->Expr.V1In=yyt->Binary.Lop->Expr.V1Out;
yyt->Binary.Rop->Expr.FLIn=yyt->Binary.Lop->Expr.FLOut;
yyVisit1 (yyt->Binary.Rop);
yyt->Binary.V1Out=yyt->Binary.Rop->Expr.V1Out;
yyt->Binary.FLOut=yyt->Binary.Rop->Expr.FLOut;
/* line 644 "" */
 yyt->Binary.DefType = SynthDefType(yyt->Binary.Lop->Expr.DefType,yyt->Binary.Rop->Expr.DefType,yyt->Binary.Op);
break;

case kUnary:
yyVisit1 (yyt->Unary.Op);
yyt->Unary.Expr->Expr.V1In=yyt->Unary.V1In;
yyt->Unary.Expr->Expr.FLIn=yyt->Unary.FLIn;
yyVisit1 (yyt->Unary.Expr);
yyt->Unary.V1Out=yyt->Unary.Expr->Expr.V1Out;
yyt->Unary.FLOut=yyt->Unary.Expr->Expr.FLOut;
/* line 640 "" */
 yyt->Unary.DefType = NoTree;
break;

case kConst:
yyt->Const.Type->Type.V1In=yyt->Const.V1In;
yyVisit1 (yyt->Const.Type);
yyt->Const.V1Out=yyt->Const.Type->Type.V1Out;
yyt->Const.FLOut=yyt->Const.FLIn;
/* line 645 "" */
 yyt->Const.DefType = yyt->Const.Type;
break;

case kValof:
yyt->Valof.SpecList->SpecList.V1In=yyt->Valof.V1In;
yyVisit1 (yyt->Valof.SpecList);
yyt->Valof.Process->Process.V1In=yyt->Valof.SpecList->SpecList.V1Out;
yyVisit1 (yyt->Valof.Process);
yyt->Valof.ExprList->ExprList.V1In=yyt->Valof.Process->Process.V1Out;
yyt->Valof.ExprList->ExprList.FLIn=yyt->Valof.FLIn;
yyVisit1 (yyt->Valof.ExprList);
yyt->Valof.V1Out=yyt->Valof.ExprList->ExprList.V1Out;
yyt->Valof.FLOut=yyt->Valof.ExprList->ExprList.FLOut;
/* line 652 "" */
 yyt->Valof.DefType = yyt->Valof.ExprList->ExprList.DefType;
break;

case kFuncCall:
yyt->FuncCall.ExprList->ExprList.V1In=yyt->FuncCall.V1In;
yyt->FuncCall.ExprList->ExprList.FLIn=yyt->FuncCall.FLIn;
yyVisit1 (yyt->FuncCall.ExprList);
yyt->FuncCall.V1Out=yyt->FuncCall.ExprList->ExprList.V1Out;
yyt->FuncCall.FLOut=yyt->FuncCall.ExprList->ExprList.FLOut;
/* line 646 "" */
 yyt->FuncCall.DefType = StripTypeList(SymbolType(yyt->FuncCall.Sym));
break;

case kElementalExpr:
yyt->ElementalExpr.Elemental->Elemental.V1In=yyt->ElementalExpr.V1In;
yyt->ElementalExpr.Elemental->Elemental.FLIn=yyt->ElementalExpr.FLIn;
yyVisit1 (yyt->ElementalExpr.Elemental);
yyt->ElementalExpr.V1Out=yyt->ElementalExpr.Elemental->Elemental.V1Out;
yyt->ElementalExpr.FLOut=yyt->ElementalExpr.Elemental->Elemental.FLOut;
/* line 647 "" */
 yyt->ElementalExpr.DefType = yyt->ElementalExpr.Elemental->Elemental.DefType;
break;

case kDefault:
yyt->Default.V1Out=yyt->Default.V1In;
yyt->Default.FLOut=yyt->Default.FLIn;
/* line 648 "" */
 yyt->Default.DefType = NoTree;
break;

case kConvert:
yyt->Convert.Type->Type.V1In=yyt->Convert.V1In;
yyVisit1 (yyt->Convert.Type);
yyt->Convert.Expr->Expr.V1In=yyt->Convert.Type->Type.V1Out;
/* line 678 "" */
 yyt->Convert.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->Convert.Expr);
yyt->Convert.V1Out=yyt->Convert.Expr->Expr.V1Out;
yyt->Convert.FLOut=yyt->Convert.Expr->Expr.FLOut;
/* line 649 "" */
 yyt->Convert.DefType = yyt->Convert.Type;
break;

case kRound:
yyt->Round.Type->Type.V1In=yyt->Round.V1In;
yyVisit1 (yyt->Round.Type);
yyt->Round.Expr->Expr.V1In=yyt->Round.Type->Type.V1Out;
yyt->Round.Expr->Expr.FLIn=yyt->Round.FLIn;
yyVisit1 (yyt->Round.Expr);
yyt->Round.V1Out=yyt->Round.Expr->Expr.V1Out;
yyt->Round.FLOut=yyt->Round.Expr->Expr.FLOut;
/* line 650 "" */
 yyt->Round.DefType = yyt->Round.Type;
break;

case kTrunc:
yyt->Trunc.Type->Type.V1In=yyt->Trunc.V1In;
yyVisit1 (yyt->Trunc.Type);
yyt->Trunc.Expr->Expr.V1In=yyt->Trunc.Type->Type.V1Out;
yyt->Trunc.Expr->Expr.FLIn=yyt->Trunc.FLIn;
yyVisit1 (yyt->Trunc.Expr);
yyt->Trunc.V1Out=yyt->Trunc.Expr->Expr.V1Out;
yyt->Trunc.FLOut=yyt->Trunc.Expr->Expr.FLOut;
/* line 651 "" */
 yyt->Trunc.DefType = yyt->Trunc.Type;
break;

case kMostPos:
yyt->MostPos.Type->Type.V1In=yyt->MostPos.V1In;
yyVisit1 (yyt->MostPos.Type);
yyt->MostPos.V1Out=yyt->MostPos.Type->Type.V1Out;
yyt->MostPos.FLOut=yyt->MostPos.FLIn;
/* line 653 "" */
 yyt->MostPos.DefType = yyt->MostPos.Type;
break;

case kMostNeg:
yyt->MostNeg.Type->Type.V1In=yyt->MostNeg.V1In;
yyVisit1 (yyt->MostNeg.Type);
yyt->MostNeg.V1Out=yyt->MostNeg.Type->Type.V1Out;
yyt->MostNeg.FLOut=yyt->MostNeg.FLIn;
/* line 654 "" */
 yyt->MostNeg.DefType = yyt->MostNeg.Type;
break;

case kCountArray:
yyt->CountArray.Length->Expr.V1In=yyt->CountArray.V1In;
yyt->CountArray.Length->Expr.FLIn=yyt->CountArray.FLIn;
yyVisit1 (yyt->CountArray.Length);
yyt->CountArray.Expr->Expr.V1In=yyt->CountArray.Length->Expr.V1Out;
yyt->CountArray.Expr->Expr.FLIn=yyt->CountArray.Length->Expr.FLOut;
yyVisit1 (yyt->CountArray.Expr);
yyt->CountArray.V1Out=yyt->CountArray.Expr->Expr.V1Out;
yyt->CountArray.FLOut=yyt->CountArray.Expr->Expr.FLOut;
/* line 640 "" */
 yyt->CountArray.DefType = NoTree;
break;

case kUnBound:
yyt->UnBound.V1Out=yyt->UnBound.V1In;
yyt->UnBound.FLOut=yyt->UnBound.FLIn;
/* line 655 "" */
 yyt->UnBound.DefType = NoTree;
break;

case kBytesIn:
yyt->BytesIn.V1Out=yyt->BytesIn.V1In;
yyt->BytesIn.FLOut=yyt->BytesIn.FLIn;
/* line 656 "" */
 yyt->BytesIn.DefType = mInt();
break;

case kBytesInType:
yyt->BytesInType.Type->Type.V1In=yyt->BytesInType.V1In;
yyVisit1 (yyt->BytesInType.Type);
yyt->BytesInType.V1Out=yyt->BytesInType.Type->Type.V1Out;
yyt->BytesInType.FLOut=yyt->BytesInType.FLIn;
/* line 656 "" */
 yyt->BytesInType.DefType = mInt();
break;

case kBytesInExpr:
yyt->BytesInExpr.Expr->Expr.V1In=yyt->BytesInExpr.V1In;
yyt->BytesInExpr.Expr->Expr.FLIn=yyt->BytesInExpr.FLIn;
yyVisit1 (yyt->BytesInExpr.Expr);
yyt->BytesInExpr.V1Out=yyt->BytesInExpr.Expr->Expr.V1Out;
yyt->BytesInExpr.FLOut=yyt->BytesInExpr.Expr->Expr.FLOut;
/* line 656 "" */
 yyt->BytesInExpr.DefType = mInt();
break;

case kSizeOf:
yyt->SizeOf.Type->Type.V1In=yyt->SizeOf.V1In;
yyVisit1 (yyt->SizeOf.Type);
yyt->SizeOf.V1Out=yyt->SizeOf.Type->Type.V1Out;
yyt->SizeOf.FLOut=yyt->SizeOf.FLIn;
/* line 640 "" */
 yyt->SizeOf.DefType = NoTree;
break;

case kOffsetOf:
yyt->OffsetOf.Type->Type.V1In=yyt->OffsetOf.V1In;
yyVisit1 (yyt->OffsetOf.Type);
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.V1In=yyt->OffsetOf.Type->Type.V1Out;
/* line 703 "" */
 yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.FLIn = GetFieldList(yyt->OffsetOf.Type);
yyVisit1 (yyt->OffsetOf.UnmappedIdentifier);
yyt->OffsetOf.V1Out=yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.V1Out;
/* line 703 "" */
 yyt->OffsetOf.FLOut = NoTree;
/* line 640 "" */
 yyt->OffsetOf.DefType = NoTree;
break;

case kStringElement:
yyt->StringElement.V1Out=yyt->StringElement.V1In;
yyt->StringElement.FLOut=yyt->StringElement.FLIn;
/* line 657 "" */
 yyt->StringElement.DefType = NoTree;
break;

case kString:
yyt->String.Type->Type.V1In=yyt->String.V1In;
yyVisit1 (yyt->String.Type);
yyt->String.V1Out=yyt->String.Type->Type.V1Out;
yyt->String.FLOut=yyt->String.FLIn;
/* line 658 "" */
 yyt->String.DefType = NoTree;
break;

case kStringSubscript:
yyt->StringSubscript.Next->StringElement.V1In=yyt->StringSubscript.V1In;
yyt->StringSubscript.Next->StringElement.FLIn=yyt->StringSubscript.FLIn;
yyVisit1 (yyt->StringSubscript.Next);
yyt->StringSubscript.Expr->Expr.V1In=yyt->StringSubscript.Next->StringElement.V1Out;
yyt->StringSubscript.Expr->Expr.FLIn=yyt->StringSubscript.Next->StringElement.FLOut;
yyVisit1 (yyt->StringSubscript.Expr);
yyt->StringSubscript.V1Out=yyt->StringSubscript.Expr->Expr.V1Out;
yyt->StringSubscript.FLOut=yyt->StringSubscript.Expr->Expr.FLOut;
/* line 657 "" */
 yyt->StringSubscript.DefType = NoTree;
break;

case kStringRange:
yyt->StringRange.Next->StringElement.V1In=yyt->StringRange.V1In;
yyt->StringRange.Next->StringElement.FLIn=yyt->StringRange.FLIn;
yyVisit1 (yyt->StringRange.Next);
yyt->StringRange.From->Expr.V1In=yyt->StringRange.Next->StringElement.V1Out;
yyt->StringRange.From->Expr.FLIn=yyt->StringRange.Next->StringElement.FLOut;
yyVisit1 (yyt->StringRange.From);
yyt->StringRange.For->Expr.V1In=yyt->StringRange.From->Expr.V1Out;
yyt->StringRange.For->Expr.FLIn=yyt->StringRange.From->Expr.FLOut;
yyVisit1 (yyt->StringRange.For);
yyt->StringRange.V1Out=yyt->StringRange.For->Expr.V1Out;
yyt->StringRange.FLOut=yyt->StringRange.For->Expr.FLOut;
/* line 657 "" */
 yyt->StringRange.DefType = NoTree;
break;

case kElemental:
yyt->Elemental.V1Out=yyt->Elemental.V1In;
yyt->Elemental.FLOut=yyt->Elemental.FLIn;
/* line 659 "" */
 yyt->Elemental.DefType = NoTree;
break;

case kTableElement:
yyt->TableElement.V1Out=yyt->TableElement.V1In;
yyt->TableElement.FLOut=yyt->TableElement.FLIn;
/* line 660 "" */
 yyt->TableElement.DefType = NoTree;
break;

case kTable:
yyt->Table.ExprList->ExprList.V1In=yyt->Table.V1In;
yyt->Table.ExprList->ExprList.FLIn=yyt->Table.FLIn;
yyVisit1 (yyt->Table.ExprList);
yyt->Table.V1Out=yyt->Table.ExprList->ExprList.V1Out;
yyt->Table.FLOut=yyt->Table.ExprList->ExprList.FLOut;
/* line 665 "" */
 yyt->Table.DefType = SynthDefTableType(yyt->Table.ExprList->ExprList.DefType);
break;

case kUserLiteral:
yyt->UserLiteral.ExprList->ExprList.V1In=yyt->UserLiteral.V1In;
yyt->UserLiteral.ExprList->ExprList.FLIn=yyt->UserLiteral.FLIn;
yyVisit1 (yyt->UserLiteral.ExprList);
yyt->UserLiteral.Type->Type.V1In=yyt->UserLiteral.ExprList->ExprList.V1Out;
yyVisit1 (yyt->UserLiteral.Type);
yyt->UserLiteral.V1Out=yyt->UserLiteral.Type->Type.V1Out;
yyt->UserLiteral.FLOut=yyt->UserLiteral.ExprList->ExprList.FLOut;
/* line 668 "" */
 yyt->UserLiteral.DefType = yyt->UserLiteral.Type;
break;

case kTableExpr:
yyt->TableExpr.Expr->Expr.V1In=yyt->TableExpr.V1In;
yyt->TableExpr.Expr->Expr.FLIn=yyt->TableExpr.FLIn;
yyVisit1 (yyt->TableExpr.Expr);
yyt->TableExpr.V1Out=yyt->TableExpr.Expr->Expr.V1Out;
yyt->TableExpr.FLOut=yyt->TableExpr.Expr->Expr.FLOut;
/* line 666 "" */
 yyt->TableExpr.DefType = yyt->TableExpr.Expr->Expr.DefType;
break;

case kTableSubscript:
yyt->TableSubscript.Next->TableElement.V1In=yyt->TableSubscript.V1In;
yyt->TableSubscript.Next->TableElement.FLIn=yyt->TableSubscript.FLIn;
yyVisit1 (yyt->TableSubscript.Next);
yyt->TableSubscript.Expr->Expr.V1In=yyt->TableSubscript.Next->TableElement.V1Out;
/* line 701 "" */
 yyt->TableSubscript.Expr->Expr.FLIn = GetFieldList(yyt->TableSubscript.Next->TableElement.DefType);
yyVisit1 (yyt->TableSubscript.Expr);
yyt->TableSubscript.V1Out=yyt->TableSubscript.Expr->Expr.V1Out;
/* line 701 "" */
 yyt->TableSubscript.FLOut = NoTree;
/* line 667 "" */
 yyt->TableSubscript.DefType = SynthDefSubscriptType(yyt->TableSubscript.Next->TableElement.DefType,yyt->TableSubscript.Expr->Expr.FLOut);
break;

case kTableRange:
yyt->TableRange.Next->TableElement.V1In=yyt->TableRange.V1In;
yyt->TableRange.Next->TableElement.FLIn=yyt->TableRange.FLIn;
yyVisit1 (yyt->TableRange.Next);
yyt->TableRange.From->Expr.V1In=yyt->TableRange.Next->TableElement.V1Out;
yyt->TableRange.From->Expr.FLIn=yyt->TableRange.Next->TableElement.FLOut;
yyVisit1 (yyt->TableRange.From);
yyt->TableRange.For->Expr.V1In=yyt->TableRange.From->Expr.V1Out;
yyt->TableRange.For->Expr.FLIn=yyt->TableRange.From->Expr.FLOut;
yyVisit1 (yyt->TableRange.For);
yyt->TableRange.V1Out=yyt->TableRange.For->Expr.V1Out;
yyt->TableRange.FLOut=yyt->TableRange.For->Expr.FLOut;
/* line 660 "" */
 yyt->TableRange.DefType = NoTree;
break;

case kElement:
yyt->Element.V1Out=yyt->Element.V1In;
yyt->Element.FLOut=yyt->Element.FLIn;
/* line 661 "" */
 yyt->Element.DefType = NoTree;
break;

case kIdentifier:
yyt->Identifier.V1Out=yyt->Identifier.V1In;
yyt->Identifier.FLOut=yyt->Identifier.FLIn;
/* line 662 "" */
 yyt->Identifier.DefType = SymbolType(yyt->Identifier.Sym);
break;

case kUnmappedIdentifier:
yyt->UnmappedIdentifier.V1Out=yyt->UnmappedIdentifier.V1In;
/* line 706 "" */
 
      if (Tree_IsType(yyt->UnmappedIdentifier.FLIn,kFieldList))
	yyt->UnmappedIdentifier.FLOut = LookupField(yyt->UnmappedIdentifier.Ident,yyt->UnmappedIdentifier.Pos,yyt->UnmappedIdentifier.FLIn,&yyt->UnmappedIdentifier.Sym);
      else if (Tree_IsType(yyt->UnmappedIdentifier.FLIn,kTagProList))
      {
	LookupProtocolTag(yyt->UnmappedIdentifier.Ident,yyt->UnmappedIdentifier.Pos,yyt->UnmappedIdentifier.FLIn,&yyt->UnmappedIdentifier.Sym);
	yyt->UnmappedIdentifier.FLOut = NoTree;
      }
      else  
      { 
	yyt->UnmappedIdentifier.FLOut = NoTree; 
	yyt->UnmappedIdentifier.Sym = LookupSymbolInScope(yyt->UnmappedIdentifier.Ident,yyt->UnmappedIdentifier.Pos,yyt->UnmappedIdentifier.Scope);
      }
    
/* line 662 "" */
 yyt->UnmappedIdentifier.DefType = SymbolType(yyt->UnmappedIdentifier.Sym);
break;

case kElementAccess:
yyt->ElementAccess.Next->Element.V1In=yyt->ElementAccess.V1In;
yyt->ElementAccess.Next->Element.FLIn=yyt->ElementAccess.FLIn;
yyVisit1 (yyt->ElementAccess.Next);
yyt->ElementAccess.V1Out=yyt->ElementAccess.Next->Element.V1Out;
yyt->ElementAccess.FLOut=yyt->ElementAccess.Next->Element.FLOut;
/* line 661 "" */
 yyt->ElementAccess.DefType = NoTree;
break;

case kSubscript:
yyt->Subscript.Next->Element.V1In=yyt->Subscript.V1In;
yyt->Subscript.Next->Element.FLIn=yyt->Subscript.FLIn;
yyVisit1 (yyt->Subscript.Next);
yyt->Subscript.Expr->Expr.V1In=yyt->Subscript.Next->Element.V1Out;
/* line 700 "" */
 yyt->Subscript.Expr->Expr.FLIn = GetFieldList(yyt->Subscript.Next->Element.DefType);
yyVisit1 (yyt->Subscript.Expr);
yyt->Subscript.V1Out=yyt->Subscript.Expr->Expr.V1Out;
/* line 700 "" */
 yyt->Subscript.FLOut = NoTree;
/* line 663 "" */
 yyt->Subscript.DefType = SynthDefSubscriptType(yyt->Subscript.Next->Element.DefType,yyt->Subscript.Expr->Expr.FLOut);
break;

case kRange:
yyt->Range.Next->Element.V1In=yyt->Range.V1In;
yyt->Range.Next->Element.FLIn=yyt->Range.FLIn;
yyVisit1 (yyt->Range.Next);
yyt->Range.From->Expr.V1In=yyt->Range.Next->Element.V1Out;
yyt->Range.From->Expr.FLIn=yyt->Range.Next->Element.FLOut;
yyVisit1 (yyt->Range.From);
yyt->Range.For->Expr.V1In=yyt->Range.From->Expr.V1Out;
yyt->Range.For->Expr.FLIn=yyt->Range.From->Expr.FLOut;
yyVisit1 (yyt->Range.For);
yyt->Range.V1Out=yyt->Range.For->Expr.V1Out;
yyt->Range.FLOut=yyt->Range.For->Expr.FLOut;
/* line 664 "" */
 yyt->Range.DefType = yyt->Range.Next->Element.DefType;
break;

case kExprList:
yyt->ExprList.FLOut=yyt->ExprList.FLIn;
yyt->ExprList.V1Out=yyt->ExprList.V1In;
/* line 641 "" */
 yyt->ExprList.DefType = NoTree;
break;

case kNoExpr:
yyt->NoExpr.FLOut=yyt->NoExpr.FLIn;
yyt->NoExpr.V1Out=yyt->NoExpr.V1In;
/* line 641 "" */
 yyt->NoExpr.DefType = NoTree;
break;

case kAnExpr:
yyt->AnExpr.Next->ExprList.V1In=yyt->AnExpr.V1In;
/* line 702 "" */
 yyt->AnExpr.Next->ExprList.FLIn = NoTree;
yyVisit1 (yyt->AnExpr.Next);
yyt->AnExpr.Expr->Expr.V1In=yyt->AnExpr.Next->ExprList.V1Out;
/* line 702 "" */
 yyt->AnExpr.Expr->Expr.FLIn= yyt->AnExpr.FLIn;
yyVisit1 (yyt->AnExpr.Expr);
yyt->AnExpr.FLOut=yyt->AnExpr.Expr->Expr.FLOut;
yyt->AnExpr.V1Out=yyt->AnExpr.Expr->Expr.V1Out;
/* line 643 "" */
 yyt->AnExpr.DefType = SynthDefType(yyt->AnExpr.Next->ExprList.DefType,yyt->AnExpr.Expr->Expr.DefType,NoTree);
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
yyt->Process.SpecList->SpecList.V1In=yyt->Process.V1In;
yyVisit1 (yyt->Process.SpecList);
yyt->Process.V1Out=yyt->Process.SpecList->SpecList.V1Out;
break;

case kAssign:
yyt->Assign.SpecList->SpecList.V1In=yyt->Assign.V1In;
yyVisit1 (yyt->Assign.SpecList);
yyt->Assign.Lval->ExprList.V1In=yyt->Assign.SpecList->SpecList.V1Out;
/* line 689 "" */
 yyt->Assign.Lval->ExprList.FLIn = NoTree;
yyVisit1 (yyt->Assign.Lval);
yyt->Assign.Rval->ExprList.V1In=yyt->Assign.Lval->ExprList.V1Out;
/* line 689 "" */
 yyt->Assign.Rval->ExprList.FLIn = NoTree;
yyVisit1 (yyt->Assign.Rval);
yyt->Assign.V1Out=yyt->Assign.Rval->ExprList.V1Out;
break;

case kAltType:
yyt->AltType.SpecList->SpecList.V1In=yyt->AltType.V1In;
yyVisit1 (yyt->AltType.SpecList);
yyt->AltType.V1Out=yyt->AltType.SpecList->SpecList.V1Out;
break;

case kAlt:
yyt->Alt.SpecList->SpecList.V1In=yyt->Alt.V1In;
yyVisit1 (yyt->Alt.SpecList);
yyt->Alt.AltList->AltList.V1In=yyt->Alt.SpecList->SpecList.V1Out;
yyVisit1 (yyt->Alt.AltList);
yyt->Alt.V1Out=yyt->Alt.AltList->AltList.V1Out;
break;

case kAltRep:
yyt->AltRep.SpecList->SpecList.V1In=yyt->AltRep.V1In;
yyVisit1 (yyt->AltRep.SpecList);
yyt->AltRep.Replicator->Replicator.V1In=yyt->AltRep.SpecList->SpecList.V1Out;
yyVisit1 (yyt->AltRep.Replicator);
yyt->AltRep.AltList->AltList.V1In=yyt->AltRep.Replicator->Replicator.V1Out;
yyVisit1 (yyt->AltRep.AltList);
yyt->AltRep.V1Out=yyt->AltRep.AltList->AltList.V1Out;
break;

case kParType:
yyt->ParType.SpecList->SpecList.V1In=yyt->ParType.V1In;
yyVisit1 (yyt->ParType.SpecList);
yyt->ParType.V1Out=yyt->ParType.SpecList->SpecList.V1Out;
break;

case kPar:
yyt->Par.SpecList->SpecList.V1In=yyt->Par.V1In;
yyVisit1 (yyt->Par.SpecList);
yyt->Par.ProcList->ProcList.V1In=yyt->Par.SpecList->SpecList.V1Out;
yyVisit1 (yyt->Par.ProcList);
yyt->Par.V1Out=yyt->Par.ProcList->ProcList.V1Out;
break;

case kParRep:
yyt->ParRep.SpecList->SpecList.V1In=yyt->ParRep.V1In;
yyVisit1 (yyt->ParRep.SpecList);
yyt->ParRep.Replicator->Replicator.V1In=yyt->ParRep.SpecList->SpecList.V1Out;
yyVisit1 (yyt->ParRep.Replicator);
yyt->ParRep.Process->Process.V1In=yyt->ParRep.Replicator->Replicator.V1Out;
yyVisit1 (yyt->ParRep.Process);
yyt->ParRep.V1Out=yyt->ParRep.Process->Process.V1Out;
break;

case kProcessor:
yyt->Processor.SpecList->SpecList.V1In=yyt->Processor.V1In;
yyVisit1 (yyt->Processor.SpecList);
yyt->Processor.Expr->Expr.V1In=yyt->Processor.SpecList->SpecList.V1Out;
/* line 690 "" */
 yyt->Processor.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->Processor.Expr);
yyt->Processor.Process->Process.V1In=yyt->Processor.Expr->Expr.V1Out;
yyVisit1 (yyt->Processor.Process);
yyt->Processor.V1Out=yyt->Processor.Process->Process.V1Out;
break;

case kIfType:
yyt->IfType.SpecList->SpecList.V1In=yyt->IfType.V1In;
yyVisit1 (yyt->IfType.SpecList);
yyt->IfType.V1Out=yyt->IfType.SpecList->SpecList.V1Out;
break;

case kIf:
yyt->If.SpecList->SpecList.V1In=yyt->If.V1In;
yyVisit1 (yyt->If.SpecList);
yyt->If.ChoiceList->ChoiceList.V1In=yyt->If.SpecList->SpecList.V1Out;
yyVisit1 (yyt->If.ChoiceList);
yyt->If.V1Out=yyt->If.ChoiceList->ChoiceList.V1Out;
break;

case kIfRep:
yyt->IfRep.SpecList->SpecList.V1In=yyt->IfRep.V1In;
yyVisit1 (yyt->IfRep.SpecList);
yyt->IfRep.Replicator->Replicator.V1In=yyt->IfRep.SpecList->SpecList.V1Out;
yyVisit1 (yyt->IfRep.Replicator);
yyt->IfRep.ChoiceList->ChoiceList.V1In=yyt->IfRep.Replicator->Replicator.V1Out;
yyVisit1 (yyt->IfRep.ChoiceList);
yyt->IfRep.V1Out=yyt->IfRep.ChoiceList->ChoiceList.V1Out;
break;

case kInput:
yyt->Input.SpecList->SpecList.V1In=yyt->Input.V1In;
yyVisit1 (yyt->Input.SpecList);
yyt->Input.Element->Element.V1In=yyt->Input.SpecList->SpecList.V1Out;
/* line 691 "" */
 yyt->Input.Element->Element.FLIn = NoTree;
yyVisit1 (yyt->Input.Element);
yyt->Input.ExprList->ExprList.V1In=yyt->Input.Element->Element.V1Out;
/* line 691 "" */
 yyt->Input.ExprList->ExprList.FLIn = GetTagProtocol(yyt->Input.Element->Element.DefType);
yyVisit1 (yyt->Input.ExprList);
yyt->Input.V1Out=yyt->Input.ExprList->ExprList.V1Out;
break;

case kOutput:
yyt->Output.SpecList->SpecList.V1In=yyt->Output.V1In;
yyVisit1 (yyt->Output.SpecList);
yyt->Output.Element->Element.V1In=yyt->Output.SpecList->SpecList.V1Out;
/* line 692 "" */
 yyt->Output.Element->Element.FLIn = NoTree;
yyVisit1 (yyt->Output.Element);
yyt->Output.ExprList->ExprList.V1In=yyt->Output.Element->Element.V1Out;
/* line 692 "" */
 yyt->Output.ExprList->ExprList.FLIn = GetTagProtocol(yyt->Output.Element->Element.DefType);
yyVisit1 (yyt->Output.ExprList);
yyt->Output.V1Out=yyt->Output.ExprList->ExprList.V1Out;
break;

case kCaseIn:
yyt->CaseIn.SpecList->SpecList.V1In=yyt->CaseIn.V1In;
yyVisit1 (yyt->CaseIn.SpecList);
yyt->CaseIn.Element->Element.V1In=yyt->CaseIn.SpecList->SpecList.V1Out;
/* line 686 "" */
 yyt->CaseIn.Element->Element.FLIn = NoTree;
yyVisit1 (yyt->CaseIn.Element);
yyt->CaseIn.CaseList->CaseList.V1In=yyt->CaseIn.Element->Element.V1Out;
/* line 686 "" */
 yyt->CaseIn.CaseList->CaseList.FLIn = GetTagProtocol(yyt->CaseIn.Element->Element.DefType);
yyVisit1 (yyt->CaseIn.CaseList);
yyt->CaseIn.V1Out=yyt->CaseIn.CaseList->CaseList.V1Out;
break;

case kAfter:
yyt->After.SpecList->SpecList.V1In=yyt->After.V1In;
yyVisit1 (yyt->After.SpecList);
yyt->After.Element->Element.V1In=yyt->After.SpecList->SpecList.V1Out;
/* line 693 "" */
 yyt->After.Element->Element.FLIn = NoTree ;
yyVisit1 (yyt->After.Element);
yyt->After.Expr->Expr.V1In=yyt->After.Element->Element.V1Out;
/* line 693 "" */
 yyt->After.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->After.Expr);
yyt->After.V1Out=yyt->After.Expr->Expr.V1Out;
break;

case kSeqRep:
yyt->SeqRep.SpecList->SpecList.V1In=yyt->SeqRep.V1In;
yyVisit1 (yyt->SeqRep.SpecList);
yyt->SeqRep.Replicator->Replicator.V1In=yyt->SeqRep.SpecList->SpecList.V1Out;
yyVisit1 (yyt->SeqRep.Replicator);
yyt->SeqRep.Process->Process.V1In=yyt->SeqRep.Replicator->Replicator.V1Out;
yyVisit1 (yyt->SeqRep.Process);
yyt->SeqRep.V1Out=yyt->SeqRep.Process->Process.V1Out;
break;

case kSeq:
yyt->Seq.SpecList->SpecList.V1In=yyt->Seq.V1In;
yyVisit1 (yyt->Seq.SpecList);
yyt->Seq.ProcList->ProcList.V1In=yyt->Seq.SpecList->SpecList.V1Out;
yyVisit1 (yyt->Seq.ProcList);
yyt->Seq.V1Out=yyt->Seq.ProcList->ProcList.V1Out;
break;

case kCase:
yyt->Case.SpecList->SpecList.V1In=yyt->Case.V1In;
yyVisit1 (yyt->Case.SpecList);
yyt->Case.Selector->Expr.V1In=yyt->Case.SpecList->SpecList.V1Out;
/* line 694 "" */
 yyt->Case.Selector->Expr.FLIn = NoTree;
yyVisit1 (yyt->Case.Selector);
yyt->Case.ChoiceList->ChoiceList.V1In=yyt->Case.Selector->Expr.V1Out;
yyVisit1 (yyt->Case.ChoiceList);
yyt->Case.V1Out=yyt->Case.ChoiceList->ChoiceList.V1Out;
break;

case kWhile:
yyt->While.SpecList->SpecList.V1In=yyt->While.V1In;
yyVisit1 (yyt->While.SpecList);
yyt->While.Expr->Expr.V1In=yyt->While.SpecList->SpecList.V1Out;
/* line 695 "" */
 yyt->While.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->While.Expr);
yyt->While.Process->Process.V1In=yyt->While.Expr->Expr.V1Out;
yyVisit1 (yyt->While.Process);
yyt->While.V1Out=yyt->While.Process->Process.V1Out;
break;

case kSkip:
yyt->Skip.SpecList->SpecList.V1In=yyt->Skip.V1In;
yyVisit1 (yyt->Skip.SpecList);
yyt->Skip.V1Out=yyt->Skip.SpecList->SpecList.V1Out;
break;

case kStop:
yyt->Stop.SpecList->SpecList.V1In=yyt->Stop.V1In;
yyVisit1 (yyt->Stop.SpecList);
yyt->Stop.V1Out=yyt->Stop.SpecList->SpecList.V1Out;
break;

case kInstance:
yyt->Instance.SpecList->SpecList.V1In=yyt->Instance.V1In;
yyVisit1 (yyt->Instance.SpecList);
yyt->Instance.ExprList->ExprList.V1In=yyt->Instance.SpecList->SpecList.V1Out;
/* line 696 "" */
 yyt->Instance.ExprList->ExprList.FLIn = NoTree;
yyVisit1 (yyt->Instance.ExprList);
yyt->Instance.V1Out=yyt->Instance.ExprList->ExprList.V1Out;
break;

case kCCode:
yyt->CCode.SpecList->SpecList.V1In=yyt->CCode.V1In;
yyVisit1 (yyt->CCode.SpecList);
yyt->CCode.V1Out=yyt->CCode.SpecList->SpecList.V1Out;
break;

case kNullProcess:
yyt->NullProcess.SpecList->SpecList.V1In=yyt->NullProcess.V1In;
yyVisit1 (yyt->NullProcess.SpecList);
yyt->NullProcess.V1Out=yyt->NullProcess.SpecList->SpecList.V1Out;
break;

case kAltList:
yyt->AltList.V1Out=yyt->AltList.V1In;
break;

case kNoAlt:
yyt->NoAlt.V1Out=yyt->NoAlt.V1In;
break;

case kAnAlts:
yyt->AnAlts.SpecList->SpecList.V1In=yyt->AnAlts.V1In;
yyVisit1 (yyt->AnAlts.SpecList);
yyt->AnAlts.Next->AltList.V1In=yyt->AnAlts.SpecList->SpecList.V1Out;
yyVisit1 (yyt->AnAlts.Next);
yyt->AnAlts.V1Out=yyt->AnAlts.Next->AltList.V1Out;
break;

case kAnAlt:
yyt->AnAlt.SpecList->SpecList.V1In=yyt->AnAlt.V1In;
yyVisit1 (yyt->AnAlt.SpecList);
yyt->AnAlt.Next->AltList.V1In=yyt->AnAlt.SpecList->SpecList.V1Out;
yyVisit1 (yyt->AnAlt.Next);
yyt->AnAlt.Guard->Guard.V1In=yyt->AnAlt.Next->AltList.V1Out;
yyVisit1 (yyt->AnAlt.Guard);
yyt->AnAlt.V1Out=yyt->AnAlt.Guard->Guard.V1Out;
break;

case kANewAlt:
yyt->ANewAlt.SpecList->SpecList.V1In=yyt->ANewAlt.V1In;
yyVisit1 (yyt->ANewAlt.SpecList);
yyt->ANewAlt.Next->AltList.V1In=yyt->ANewAlt.SpecList->SpecList.V1Out;
yyVisit1 (yyt->ANewAlt.Next);
yyt->ANewAlt.AltType->AltType.V1In=yyt->ANewAlt.Next->AltList.V1Out;
yyVisit1 (yyt->ANewAlt.AltType);
yyt->ANewAlt.V1Out=yyt->ANewAlt.AltType->AltType.V1Out;
break;

case kGuard:
yyt->Guard.Expr->Expr.V1In=yyt->Guard.V1In;
/* line 697 "" */
 yyt->Guard.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->Guard.Expr);
yyt->Guard.V1Out=yyt->Guard.Expr->Expr.V1Out;
break;

case kAltInput:
yyt->AltInput.Expr->Expr.V1In=yyt->AltInput.V1In;
/* line 697 "" */
 yyt->AltInput.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->AltInput.Expr);
yyt->AltInput.Input->Input.V1In=yyt->AltInput.Expr->Expr.V1Out;
yyVisit1 (yyt->AltInput.Input);
yyt->AltInput.Process->Process.V1In=yyt->AltInput.Input->Input.V1Out;
yyVisit1 (yyt->AltInput.Process);
yyt->AltInput.V1Out=yyt->AltInput.Process->Process.V1Out;
break;

case kAltTimer:
yyt->AltTimer.Expr->Expr.V1In=yyt->AltTimer.V1In;
/* line 697 "" */
 yyt->AltTimer.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->AltTimer.Expr);
yyt->AltTimer.After->After.V1In=yyt->AltTimer.Expr->Expr.V1Out;
yyVisit1 (yyt->AltTimer.After);
yyt->AltTimer.Process->Process.V1In=yyt->AltTimer.After->After.V1Out;
yyVisit1 (yyt->AltTimer.Process);
yyt->AltTimer.V1Out=yyt->AltTimer.Process->Process.V1Out;
break;

case kAltCaseIn:
yyt->AltCaseIn.Expr->Expr.V1In=yyt->AltCaseIn.V1In;
/* line 697 "" */
 yyt->AltCaseIn.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->AltCaseIn.Expr);
yyt->AltCaseIn.CaseIn->CaseIn.V1In=yyt->AltCaseIn.Expr->Expr.V1Out;
yyVisit1 (yyt->AltCaseIn.CaseIn);
yyt->AltCaseIn.V1Out=yyt->AltCaseIn.CaseIn->CaseIn.V1Out;
break;

case kNoInput:
yyt->NoInput.Expr->Expr.V1In=yyt->NoInput.V1In;
/* line 697 "" */
 yyt->NoInput.Expr->Expr.FLIn = NoTree;
yyVisit1 (yyt->NoInput.Expr);
yyt->NoInput.Process->Process.V1In=yyt->NoInput.Expr->Expr.V1Out;
yyVisit1 (yyt->NoInput.Process);
yyt->NoInput.V1Out=yyt->NoInput.Process->Process.V1Out;
break;

case kChoiceList:
/* line 642 "" */
 yyt->ChoiceList.DefType = NoTree;
yyt->ChoiceList.V1Out=yyt->ChoiceList.V1In;
break;

case kNoChoice:
/* line 642 "" */
 yyt->NoChoice.DefType = NoTree;
yyt->NoChoice.V1Out=yyt->NoChoice.V1In;
break;

case kAChoices:
/* line 642 "" */
 yyt->AChoices.DefType = NoTree;
yyt->AChoices.SpecList->SpecList.V1In=yyt->AChoices.V1In;
yyVisit1 (yyt->AChoices.SpecList);
yyt->AChoices.Next->ChoiceList.V1In=yyt->AChoices.SpecList->SpecList.V1Out;
yyVisit1 (yyt->AChoices.Next);
yyt->AChoices.V1Out=yyt->AChoices.Next->ChoiceList.V1Out;
break;

case kAChoice:
/* line 642 "" */
 yyt->AChoice.DefType = NoTree;
yyt->AChoice.SpecList->SpecList.V1In=yyt->AChoice.V1In;
yyVisit1 (yyt->AChoice.SpecList);
yyt->AChoice.Next->ChoiceList.V1In=yyt->AChoice.SpecList->SpecList.V1Out;
yyVisit1 (yyt->AChoice.Next);
yyt->AChoice.ExprList->ExprList.V1In=yyt->AChoice.Next->ChoiceList.V1Out;
/* line 698 "" */
 yyt->AChoice.ExprList->ExprList.FLIn = NoTree;
yyVisit1 (yyt->AChoice.ExprList);
yyt->AChoice.Process->Process.V1In=yyt->AChoice.ExprList->ExprList.V1Out;
yyVisit1 (yyt->AChoice.Process);
yyt->AChoice.V1Out=yyt->AChoice.Process->Process.V1Out;
break;

case kANewIf:
/* line 642 "" */
 yyt->ANewIf.DefType = NoTree;
yyt->ANewIf.SpecList->SpecList.V1In=yyt->ANewIf.V1In;
yyVisit1 (yyt->ANewIf.SpecList);
yyt->ANewIf.Next->ChoiceList.V1In=yyt->ANewIf.SpecList->SpecList.V1Out;
yyVisit1 (yyt->ANewIf.Next);
yyt->ANewIf.IfType->IfType.V1In=yyt->ANewIf.Next->ChoiceList.V1Out;
yyVisit1 (yyt->ANewIf.IfType);
yyt->ANewIf.V1Out=yyt->ANewIf.IfType->IfType.V1Out;
break;

case kProcList:
yyt->ProcList.V1Out=yyt->ProcList.V1In;
break;

case kNoProcess:
yyt->NoProcess.V1Out=yyt->NoProcess.V1In;
break;

case kAProcess:
yyt->AProcess.Next->ProcList.V1In=yyt->AProcess.V1In;
yyVisit1 (yyt->AProcess.Next);
yyt->AProcess.Process->Process.V1In=yyt->AProcess.Next->ProcList.V1Out;
yyVisit1 (yyt->AProcess.Process);
yyt->AProcess.V1Out=yyt->AProcess.Process->Process.V1Out;
break;

case kCaseList:
yyt->CaseList.FLOut=yyt->CaseList.FLIn;
yyt->CaseList.V1Out=yyt->CaseList.V1In;
break;

case kNoCase:
yyt->NoCase.FLOut=yyt->NoCase.FLIn;
yyt->NoCase.V1Out=yyt->NoCase.V1In;
break;

case kACase:
yyt->ACase.Next->CaseList.V1In=yyt->ACase.V1In;
/* line 699 "" */
 yyt->ACase.Next->CaseList.FLIn = yyt->ACase.FLIn;
yyVisit1 (yyt->ACase.Next);
yyt->ACase.SpecList->SpecList.V1In=yyt->ACase.Next->CaseList.V1Out;
yyVisit1 (yyt->ACase.SpecList);
yyt->ACase.ExprList->ExprList.V1In=yyt->ACase.SpecList->SpecList.V1Out;
/* line 699 "" */
 yyt->ACase.ExprList->ExprList.FLIn = yyt->ACase.FLIn;
yyVisit1 (yyt->ACase.ExprList);
yyt->ACase.FLOut=yyt->ACase.ExprList->ExprList.FLOut;
yyt->ACase.Process->Process.V1In=yyt->ACase.ExprList->ExprList.V1Out;
yyVisit1 (yyt->ACase.Process);
yyt->ACase.V1Out=yyt->ACase.Process->Process.V1Out;
break;
 default: ;
 }
}

void BeginPreTypeChkAg ()
{
}

void ClosePreTypeChkAg ()
{
}
