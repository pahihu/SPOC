# define DEP(a, b) a
# define SELF yyt
# include "ParUsageAg.h"

/* line 687 "" */

  #define IsMVAssign(EL) (Tree_IsType(EL->AnExpr.Next,kAnExpr))
  #define BIGDEPTH 10000


static char yyb;

static void yyVisit1 ARGS((register tTree yyt));

void ParUsageAg
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
/* line 709 "" */
 
    yyt->Start.Process->Process.PUIn = CreateUsage(yyt->Start.Process->Process.Pos);
yyVisit1 (yyt->Start.Process);
break;

case kSpecList:
yyt->SpecList.PUOut=yyt->SpecList.PUIn;
break;

case kNoSpec:
yyt->NoSpec.PUOut=yyt->NoSpec.PUIn;
break;

case kASpec:
yyt->ASpec.Spec->Spec.PUIn=yyt->ASpec.PUIn;
yyVisit1 (yyt->ASpec.Spec);
yyt->ASpec.Next->SpecList.PUIn=yyt->ASpec.Spec->Spec.PUOut;
yyVisit1 (yyt->ASpec.Next);
yyt->ASpec.PUOut=yyt->ASpec.Next->SpecList.PUOut;
break;

case kSpec:
yyt->Spec.PUOut=yyt->Spec.PUIn;
break;

case kDeclType:
yyt->DeclType.Type->Type.PUIn=yyt->DeclType.PUIn;
yyVisit1 (yyt->DeclType.Type);
yyt->DeclType.PUOut=yyt->DeclType.Type->Type.PUOut;
break;

case kDecl:
yyt->Decl.Type->Type.PUIn=yyt->Decl.PUIn;
yyVisit1 (yyt->Decl.Type);
yyt->Decl.PUOut=yyt->Decl.Type->Type.PUOut;
yyVisit1 (yyt->Decl.IdList);
break;

case kIs:
/* line 1082 "" */
 yyt->Is.Elemental->Elemental.RVL = NoTree;
/* line 913 "" */
 yyt->Is.Elemental->Elemental.LV = true;
/* line 783 "" */
 
    yyt->Is.Elemental->Elemental.PUIn = DEP(CreateUsage(yyt->Is.Pos),yyt->Is.PUIn);
yyVisit1 (yyt->Is.Elemental);
/* line 784 "" */

    yyt->Is.PUOut = (AddWriteUsage(yyt->Is.Elemental->Elemental.PUOut,yyt->Is.Elemental),CombineUsage(yyt->Is.PUIn,yyt->Is.Elemental->Elemental.PUOut,BIGDEPTH));
yyt->Is.Type->Type.PUIn=yyt->Is.PUIn;
yyVisit1 (yyt->Is.Type);
break;

case kAVal:
yyt->AVal.Type->Type.PUIn=yyt->AVal.PUIn;
/* line 912 "" */
 yyt->AVal.Expr->Expr.LV = false;
yyVisit1 (yyt->AVal.Type);
yyt->AVal.Expr->Expr.PUIn=yyt->AVal.Type->Type.PUOut;
yyVisit1 (yyt->AVal.Expr);
yyt->AVal.PUOut=yyt->AVal.Expr->Expr.PUOut;
break;

case kVal:
/* line 912 "" */
 yyt->Val.Expr->Expr.LV = false;
/* line 797 "" */
 
    yyt->Val.Expr->Expr.PUIn = DEP(CreateUsage(yyt->Val.Pos),yyt->Val.PUIn);
yyVisit1 (yyt->Val.Expr);
/* line 798 "" */
 
    yyt->Val.PUOut = CombineUsage(yyt->Val.PUIn,yyt->Val.Expr->Expr.PUOut,BIGDEPTH);
yyt->Val.Type->Type.PUIn=yyt->Val.PUIn;
yyVisit1 (yyt->Val.Type);
break;

case kValRetypes:
/* line 912 "" */
 yyt->ValRetypes.Expr->Expr.LV = false;
/* line 804 "" */
 
    yyt->ValRetypes.Expr->Expr.PUIn = DEP(CreateUsage(yyt->ValRetypes.Pos),yyt->ValRetypes.PUIn);
yyVisit1 (yyt->ValRetypes.Expr);
/* line 805 "" */
 
    yyt->ValRetypes.PUOut = CombineUsage(yyt->ValRetypes.PUIn,yyt->ValRetypes.Expr->Expr.PUOut,BIGDEPTH);
yyt->ValRetypes.Type->Type.PUIn=yyt->ValRetypes.PUIn;
yyVisit1 (yyt->ValRetypes.Type);
break;

case kValReshapes:
/* line 912 "" */
 yyt->ValReshapes.Expr->Expr.LV = false;
/* line 804 "" */
 
    yyt->ValReshapes.Expr->Expr.PUIn = DEP(CreateUsage(yyt->ValReshapes.Pos),yyt->ValReshapes.PUIn);
yyVisit1 (yyt->ValReshapes.Expr);
/* line 805 "" */
 
    yyt->ValReshapes.PUOut = CombineUsage(yyt->ValReshapes.PUIn,yyt->ValReshapes.Expr->Expr.PUOut,BIGDEPTH);
yyt->ValReshapes.Type->Type.PUIn=yyt->ValReshapes.PUIn;
yyVisit1 (yyt->ValReshapes.Type);
break;

case kRetypes:
/* line 1083 "" */
 yyt->Retypes.Elemental->Elemental.RVL = NoTree;
/* line 914 "" */
 yyt->Retypes.Elemental->Elemental.LV = true;
/* line 790 "" */
 
    yyt->Retypes.Elemental->Elemental.PUIn = DEP(CreateUsage(yyt->Retypes.Pos),yyt->Retypes.PUIn);
yyVisit1 (yyt->Retypes.Elemental);
/* line 791 "" */
 
    yyt->Retypes.PUOut = (AddWriteUsage(yyt->Retypes.Elemental->Elemental.PUOut,yyt->Retypes.Elemental),CombineUsage(yyt->Retypes.PUIn,yyt->Retypes.Elemental->Elemental.PUOut,BIGDEPTH));
yyt->Retypes.Type->Type.PUIn=yyt->Retypes.PUIn;
yyVisit1 (yyt->Retypes.Type);
break;

case kReshapes:
/* line 1083 "" */
 yyt->Reshapes.Elemental->Elemental.RVL = NoTree;
/* line 914 "" */
 yyt->Reshapes.Elemental->Elemental.LV = true;
/* line 790 "" */
 
    yyt->Reshapes.Elemental->Elemental.PUIn = DEP(CreateUsage(yyt->Reshapes.Pos),yyt->Reshapes.PUIn);
yyVisit1 (yyt->Reshapes.Elemental);
/* line 791 "" */
 
    yyt->Reshapes.PUOut = (AddWriteUsage(yyt->Reshapes.Elemental->Elemental.PUOut,yyt->Reshapes.Elemental),CombineUsage(yyt->Reshapes.PUIn,yyt->Reshapes.Elemental->Elemental.PUOut,BIGDEPTH));
yyt->Reshapes.Type->Type.PUIn=yyt->Reshapes.PUIn;
yyVisit1 (yyt->Reshapes.Type);
break;

case kDefAPro:
yyt->DefAPro.PUOut=yyt->DefAPro.PUIn;
break;

case kDefSeqPro:
yyt->DefSeqPro.SeqProList->SeqProList.PUIn=yyt->DefSeqPro.PUIn;
yyVisit1 (yyt->DefSeqPro.SeqProList);
yyt->DefSeqPro.PUOut=yyt->DefSeqPro.SeqProList->SeqProList.PUOut;
break;

case kDefTagPro:
yyt->DefTagPro.TagProList->TagProList.PUIn=yyt->DefTagPro.PUIn;
yyVisit1 (yyt->DefTagPro.TagProList);
yyt->DefTagPro.PUOut=yyt->DefTagPro.TagProList->TagProList.PUOut;
break;

case kDefAProc:
yyt->DefAProc.Formals->Formals.PUIn=yyt->DefAProc.PUIn;
yyVisit1 (yyt->DefAProc.Formals);
yyt->DefAProc.PUOut=yyt->DefAProc.Formals->Formals.PUOut;
break;

case kDefProc:
yyt->DefProc.Formals->Formals.PUIn=yyt->DefProc.PUIn;
yyVisit1 (yyt->DefProc.Formals);
/* line 754 "" */
 
    yyt->DefProc.Process->Process.PUIn = DEP(CreateUsage(yyt->DefProc.Pos),yyt->DefProc.Formals->Formals.PUOut);
yyVisit1 (yyt->DefProc.Process);
/* line 755 "" */

    yyt->DefProc.PUOut = CombineUsage(yyt->DefProc.PUIn,yyt->DefProc.Process->Process.PUOut,Symbol(yyt->DefProc.Sym).Depth);
break;

case kInlineDefProc:
yyt->InlineDefProc.Formals->Formals.PUIn=yyt->InlineDefProc.PUIn;
yyVisit1 (yyt->InlineDefProc.Formals);
/* line 754 "" */
 
    yyt->InlineDefProc.Process->Process.PUIn = DEP(CreateUsage(yyt->InlineDefProc.Pos),yyt->InlineDefProc.Formals->Formals.PUOut);
yyVisit1 (yyt->InlineDefProc.Process);
/* line 755 "" */

    yyt->InlineDefProc.PUOut = CombineUsage(yyt->InlineDefProc.PUIn,yyt->InlineDefProc.Process->Process.PUOut,Symbol(yyt->InlineDefProc.Sym).Depth);
break;

case kPrototypeProc:
yyt->PrototypeProc.Formals->Formals.PUIn=yyt->PrototypeProc.PUIn;
yyVisit1 (yyt->PrototypeProc.Formals);
yyt->PrototypeProc.PUOut=yyt->PrototypeProc.Formals->Formals.PUOut;
break;

case kDefAFunc:
yyt->DefAFunc.TypeList->TypeList.PUIn=yyt->DefAFunc.PUIn;
yyVisit1 (yyt->DefAFunc.TypeList);
yyt->DefAFunc.Formals->Formals.PUIn=yyt->DefAFunc.TypeList->TypeList.PUOut;
yyVisit1 (yyt->DefAFunc.Formals);
yyt->DefAFunc.PUOut=yyt->DefAFunc.Formals->Formals.PUOut;
break;

case kDefFunc:
yyt->DefFunc.TypeList->TypeList.PUIn=yyt->DefFunc.PUIn;
yyVisit1 (yyt->DefFunc.TypeList);
yyt->DefFunc.Formals->Formals.PUIn=yyt->DefFunc.TypeList->TypeList.PUOut;
/* line 939 "" */
 yyt->DefFunc.Valof->Valof.LV = false;
yyVisit1 (yyt->DefFunc.Formals);
/* line 761 "" */

    yyt->DefFunc.Valof->Valof.PUIn = DEP(CreateUsage(yyt->DefFunc.Pos),yyt->DefFunc.Formals->Formals.PUOut);
yyVisit1 (yyt->DefFunc.Valof);
/* line 762 "" */

    yyt->DefFunc.PUOut = CombineUsage(yyt->DefFunc.PUIn,yyt->DefFunc.Valof->Valof.PUOut,Symbol(yyt->DefFunc.Sym).Depth);
break;

case kInlineDefFunc:
yyt->InlineDefFunc.TypeList->TypeList.PUIn=yyt->InlineDefFunc.PUIn;
yyVisit1 (yyt->InlineDefFunc.TypeList);
yyt->InlineDefFunc.Formals->Formals.PUIn=yyt->InlineDefFunc.TypeList->TypeList.PUOut;
/* line 939 "" */
 yyt->InlineDefFunc.Valof->Valof.LV = false;
yyVisit1 (yyt->InlineDefFunc.Formals);
/* line 761 "" */

    yyt->InlineDefFunc.Valof->Valof.PUIn = DEP(CreateUsage(yyt->InlineDefFunc.Pos),yyt->InlineDefFunc.Formals->Formals.PUOut);
yyVisit1 (yyt->InlineDefFunc.Valof);
/* line 762 "" */

    yyt->InlineDefFunc.PUOut = CombineUsage(yyt->InlineDefFunc.PUIn,yyt->InlineDefFunc.Valof->Valof.PUOut,Symbol(yyt->InlineDefFunc.Sym).Depth);
break;

case kDefIsFunc:
yyt->DefIsFunc.TypeList->TypeList.PUIn=yyt->DefIsFunc.PUIn;
yyVisit1 (yyt->DefIsFunc.TypeList);
yyt->DefIsFunc.Formals->Formals.PUIn=yyt->DefIsFunc.TypeList->TypeList.PUOut;
yyVisit1 (yyt->DefIsFunc.Formals);
/* line 768 "" */

    yyt->DefIsFunc.ExprList->ExprList.PUIn = DEP(CreateUsage(yyt->DefIsFunc.Pos),yyt->DefIsFunc.Formals->Formals.PUOut);
/* line 917 "" */
AddLValues(yyt->DefIsFunc.ExprList,false,false);
/* line 882 "" */
 yyt->DefIsFunc.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->DefIsFunc.ExprList);
/* line 769 "" */

    yyt->DefIsFunc.PUOut = CombineUsage(yyt->DefIsFunc.PUIn,yyt->DefIsFunc.ExprList->ExprList.PUOut,Symbol(yyt->DefIsFunc.Sym).Depth);
break;

case kInlineDefIsFunc:
yyt->InlineDefIsFunc.TypeList->TypeList.PUIn=yyt->InlineDefIsFunc.PUIn;
yyVisit1 (yyt->InlineDefIsFunc.TypeList);
yyt->InlineDefIsFunc.Formals->Formals.PUIn=yyt->InlineDefIsFunc.TypeList->TypeList.PUOut;
yyVisit1 (yyt->InlineDefIsFunc.Formals);
/* line 768 "" */

    yyt->InlineDefIsFunc.ExprList->ExprList.PUIn = DEP(CreateUsage(yyt->InlineDefIsFunc.Pos),yyt->InlineDefIsFunc.Formals->Formals.PUOut);
/* line 917 "" */
AddLValues(yyt->InlineDefIsFunc.ExprList,false,false);
/* line 882 "" */
 yyt->InlineDefIsFunc.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->InlineDefIsFunc.ExprList);
/* line 769 "" */

    yyt->InlineDefIsFunc.PUOut = CombineUsage(yyt->InlineDefIsFunc.PUIn,yyt->InlineDefIsFunc.ExprList->ExprList.PUOut,Symbol(yyt->InlineDefIsFunc.Sym).Depth);
break;

case kPrototypeFunc:
yyt->PrototypeFunc.TypeList->TypeList.PUIn=yyt->PrototypeFunc.PUIn;
yyVisit1 (yyt->PrototypeFunc.TypeList);
yyt->PrototypeFunc.Formals->Formals.PUIn=yyt->PrototypeFunc.TypeList->TypeList.PUOut;
yyVisit1 (yyt->PrototypeFunc.Formals);
yyt->PrototypeFunc.PUOut=yyt->PrototypeFunc.Formals->Formals.PUOut;
break;

case kNullSpec:
yyt->NullSpec.PUOut=yyt->NullSpec.PUIn;
break;

case kHCode:
yyt->HCode.PUOut=yyt->HCode.PUIn;
break;

case kCallFrame:
yyt->CallFrame.PUOut=yyt->CallFrame.PUIn;
break;

case kParFrame:
yyt->ParFrame.PUOut=yyt->ParFrame.PUIn;
break;

case kFormals:
yyt->Formals.PUOut=yyt->Formals.PUIn;
break;

case kNoFormal:
yyt->NoFormal.PUOut=yyt->NoFormal.PUIn;
break;

case kNoFormals:
yyt->NoFormals.PUOut=yyt->NoFormals.PUIn;
break;

case kAFormal:
yyt->AFormal.Next->Formals.PUIn=yyt->AFormal.PUIn;
yyVisit1 (yyt->AFormal.Next);
yyt->AFormal.Type->Type.PUIn=yyt->AFormal.Next->Formals.PUOut;
yyVisit1 (yyt->AFormal.Type);
yyVisit1 (yyt->AFormal.By);
yyt->AFormal.PUOut=yyt->AFormal.Type->Type.PUOut;
break;

case kReplicator:
/* line 918 "" */
 yyt->Replicator.For->Expr.LV = false;
/* line 918 "" */
 yyt->Replicator.From->Expr.LV = false;
yyt->Replicator.From->Expr.PUIn=yyt->Replicator.PUIn;
yyVisit1 (yyt->Replicator.From);
yyt->Replicator.For->Expr.PUIn=yyt->Replicator.From->Expr.PUOut;
yyVisit1 (yyt->Replicator.For);
/* line 986 "" */
 yyt->Replicator.SE = CheckSimpleExpr(yyt->Replicator.From) && CheckSimpleExpr(yyt->Replicator.For);
/* line 1014 "" */
 
    yyt->Replicator.ForLE = (yyt->Replicator.SE)?CreateLinExpr(yyt->Replicator.For):NULL;
/* line 1013 "" */
 
    yyt->Replicator.FromLE = (yyt->Replicator.SE)?CreateLinExpr(yyt->Replicator.From):NULL;
yyt->Replicator.PUOut=yyt->Replicator.For->Expr.PUOut;
break;

case kTagProList:
yyt->TagProList.PUOut=yyt->TagProList.PUIn;
break;

case kNoTagPro:
yyt->NoTagPro.PUOut=yyt->NoTagPro.PUIn;
break;

case kATagPro:
yyt->ATagPro.Next->TagProList.PUIn=yyt->ATagPro.PUIn;
yyVisit1 (yyt->ATagPro.Next);
yyt->ATagPro.SeqProList->SeqProList.PUIn=yyt->ATagPro.Next->TagProList.PUOut;
yyVisit1 (yyt->ATagPro.SeqProList);
yyt->ATagPro.PUOut=yyt->ATagPro.SeqProList->SeqProList.PUOut;
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
yyt->Type.PUOut=yyt->Type.PUIn;
break;

case kArray:
/* line 919 "" */
 yyt->Array.Expr->Expr.LV = false;
yyt->Array.Expr->Expr.PUIn=yyt->Array.PUIn;
yyVisit1 (yyt->Array.Expr);
yyt->Array.Type->Type.PUIn=yyt->Array.Expr->Expr.PUOut;
yyVisit1 (yyt->Array.Type);
yyt->Array.PUOut=yyt->Array.Type->Type.PUOut;
/* line 966 "" */
 yyt->Array.SE = CheckSimpleExpr(yyt->Array.Expr);
/* line 1009 "" */
 yyt->Array.LE = (yyt->Array.SE) ? CreateLinExpr(yyt->Array.Expr):NULL;
break;

case kUserArray:
/* line 919 "" */
 yyt->UserArray.Expr->Expr.LV = false;
yyt->UserArray.Expr->Expr.PUIn=yyt->UserArray.PUIn;
yyVisit1 (yyt->UserArray.Expr);
yyt->UserArray.Type->Type.PUIn=yyt->UserArray.Expr->Expr.PUOut;
yyVisit1 (yyt->UserArray.Type);
yyt->UserArray.PUOut=yyt->UserArray.Type->Type.PUOut;
/* line 966 "" */
 yyt->UserArray.SE = CheckSimpleExpr(yyt->UserArray.Expr);
/* line 1009 "" */
 yyt->UserArray.LE = (yyt->UserArray.SE) ? CreateLinExpr(yyt->UserArray.Expr):NULL;
break;

case kChan:
yyt->Chan.Protocol->Protocol.PUIn=yyt->Chan.PUIn;
yyVisit1 (yyt->Chan.Protocol);
yyt->Chan.PUOut=yyt->Chan.Protocol->Protocol.PUOut;
break;

case kTimer:
yyt->Timer.PUOut=yyt->Timer.PUIn;
break;

case kUntyped:
yyt->Untyped.PUOut=yyt->Untyped.PUIn;
break;

case kUntypedByte:
yyt->UntypedByte.PUOut=yyt->UntypedByte.PUIn;
break;

case kUntypedReal:
yyt->UntypedReal.PUOut=yyt->UntypedReal.PUIn;
break;

case kUntypedInteger:
yyt->UntypedInteger.PUOut=yyt->UntypedInteger.PUIn;
break;

case kUntypedInt:
yyt->UntypedInt.PUOut=yyt->UntypedInt.PUIn;
break;

case kUntypedHex:
yyt->UntypedHex.PUOut=yyt->UntypedHex.PUIn;
break;

case kPrimitive:
yyt->Primitive.PUOut=yyt->Primitive.PUIn;
break;

case kNumeric:
yyt->Numeric.PUOut=yyt->Numeric.PUIn;
break;

case kInteger:
yyt->Integer.PUOut=yyt->Integer.PUIn;
break;

case kInt:
yyt->Int.PUOut=yyt->Int.PUIn;
break;

case kUserInt:
yyt->UserInt.PUOut=yyt->UserInt.PUIn;
break;

case kInt16:
yyt->Int16.PUOut=yyt->Int16.PUIn;
break;

case kUserInt16:
yyt->UserInt16.PUOut=yyt->UserInt16.PUIn;
break;

case kInt32:
yyt->Int32.PUOut=yyt->Int32.PUIn;
break;

case kUserInt32:
yyt->UserInt32.PUOut=yyt->UserInt32.PUIn;
break;

case kInt64:
yyt->Int64.PUOut=yyt->Int64.PUIn;
break;

case kUserInt64:
yyt->UserInt64.PUOut=yyt->UserInt64.PUIn;
break;

case kReal:
yyt->Real.PUOut=yyt->Real.PUIn;
break;

case kReal32:
yyt->Real32.PUOut=yyt->Real32.PUIn;
break;

case kUserReal32:
yyt->UserReal32.PUOut=yyt->UserReal32.PUIn;
break;

case kReal64:
yyt->Real64.PUOut=yyt->Real64.PUIn;
break;

case kUserReal64:
yyt->UserReal64.PUOut=yyt->UserReal64.PUIn;
break;

case kByte:
yyt->Byte.PUOut=yyt->Byte.PUIn;
break;

case kUserByte:
yyt->UserByte.PUOut=yyt->UserByte.PUIn;
break;

case kBool:
yyt->Bool.PUOut=yyt->Bool.PUIn;
break;

case kUserBool:
yyt->UserBool.PUOut=yyt->UserBool.PUIn;
break;

case kUserStructTypes:
yyt->UserStructTypes.FieldList->FieldList.PUIn=yyt->UserStructTypes.PUIn;
yyVisit1 (yyt->UserStructTypes.FieldList);
yyt->UserStructTypes.PUOut=yyt->UserStructTypes.FieldList->FieldList.PUOut;
break;

case kUserDataTypes:
yyt->UserDataTypes.FieldList->FieldList.PUIn=yyt->UserDataTypes.PUIn;
yyVisit1 (yyt->UserDataTypes.FieldList);
yyt->UserDataTypes.PUOut=yyt->UserDataTypes.FieldList->FieldList.PUOut;
break;

case kUserRecord:
yyt->UserRecord.FieldList->FieldList.PUIn=yyt->UserRecord.PUIn;
yyVisit1 (yyt->UserRecord.FieldList);
yyt->UserRecord.PUOut=yyt->UserRecord.FieldList->FieldList.PUOut;
break;

case kUserPackedRecord:
yyt->UserPackedRecord.FieldList->FieldList.PUIn=yyt->UserPackedRecord.PUIn;
yyVisit1 (yyt->UserPackedRecord.FieldList);
yyt->UserPackedRecord.PUOut=yyt->UserPackedRecord.FieldList->FieldList.PUOut;
break;

case kUserUnion:
yyt->UserUnion.FieldList->FieldList.PUIn=yyt->UserUnion.PUIn;
yyVisit1 (yyt->UserUnion.FieldList);
yyt->UserUnion.PUOut=yyt->UserUnion.FieldList->FieldList.PUOut;
break;

case kUserChanRecord:
yyt->UserChanRecord.FieldList->FieldList.PUIn=yyt->UserChanRecord.PUIn;
yyVisit1 (yyt->UserChanRecord.FieldList);
yyt->UserChanRecord.PUOut=yyt->UserChanRecord.FieldList->FieldList.PUOut;
break;

case kTypeList:
yyt->TypeList.PUOut=yyt->TypeList.PUIn;
break;

case kNoType:
yyt->NoType.PUOut=yyt->NoType.PUIn;
break;

case kAType:
yyt->AType.Next->TypeList.PUIn=yyt->AType.PUIn;
yyVisit1 (yyt->AType.Next);
yyt->AType.Type->Type.PUIn=yyt->AType.Next->TypeList.PUOut;
yyVisit1 (yyt->AType.Type);
yyt->AType.PUOut=yyt->AType.Type->Type.PUOut;
break;

case kFieldList:
yyt->FieldList.PUOut=yyt->FieldList.PUIn;
break;

case kNoField:
yyt->NoField.PUOut=yyt->NoField.PUIn;
break;

case kAField:
yyt->AField.Next->FieldList.PUIn=yyt->AField.PUIn;
yyVisit1 (yyt->AField.Next);
yyt->AField.Type->Type.PUIn=yyt->AField.Next->FieldList.PUOut;
yyVisit1 (yyt->AField.Type);
yyt->AField.PUOut=yyt->AField.Type->Type.PUOut;
break;

case kProtocol:
yyt->Protocol.PUOut=yyt->Protocol.PUIn;
break;

case kAny:
yyt->Any.PUOut=yyt->Any.PUIn;
break;

case kNamed:
yyt->Named.PUOut=yyt->Named.PUIn;
break;

case kSimple:
yyt->Simple.Type->Type.PUIn=yyt->Simple.PUIn;
yyVisit1 (yyt->Simple.Type);
yyt->Simple.PUOut=yyt->Simple.Type->Type.PUOut;
break;

case kCounted:
yyt->Counted.Length->Type.PUIn=yyt->Counted.PUIn;
yyVisit1 (yyt->Counted.Length);
yyt->Counted.Type->Type.PUIn=yyt->Counted.Length->Type.PUOut;
yyVisit1 (yyt->Counted.Type);
yyt->Counted.PUOut=yyt->Counted.Type->Type.PUOut;
break;

case kSeqProList:
yyt->SeqProList.PUOut=yyt->SeqProList.PUIn;
break;

case kNoSeqPro:
yyt->NoSeqPro.PUOut=yyt->NoSeqPro.PUIn;
break;

case kASeqPro:
yyt->ASeqPro.Next->SeqProList.PUIn=yyt->ASeqPro.PUIn;
yyVisit1 (yyt->ASeqPro.Next);
yyt->ASeqPro.Protocol->Protocol.PUIn=yyt->ASeqPro.Next->SeqProList.PUOut;
yyVisit1 (yyt->ASeqPro.Protocol);
yyt->ASeqPro.PUOut=yyt->ASeqPro.Protocol->Protocol.PUOut;
break;

case kExpr:
yyt->Expr.PUOut=yyt->Expr.PUIn;
break;

case kBinary:
yyVisit1 (yyt->Binary.Op);
yyt->Binary.Rop->Expr.LV=yyt->Binary.LV;
yyt->Binary.Lop->Expr.LV=yyt->Binary.LV;
yyt->Binary.Lop->Expr.PUIn=yyt->Binary.PUIn;
yyVisit1 (yyt->Binary.Lop);
yyt->Binary.Rop->Expr.PUIn=yyt->Binary.Lop->Expr.PUOut;
yyVisit1 (yyt->Binary.Rop);
yyt->Binary.PUOut=yyt->Binary.Rop->Expr.PUOut;
break;

case kUnary:
yyVisit1 (yyt->Unary.Op);
yyt->Unary.Expr->Expr.LV=yyt->Unary.LV;
/* line 858 "" */
 
      if (Tree_IsType(yyt->Unary.Op,kSize)) 
	yyt->Unary.Expr->Expr.PUIn=NoParUsage;
      else
	yyt->Unary.Expr->Expr.PUIn=yyt->Unary.PUIn; 
    
yyVisit1 (yyt->Unary.Expr);
/* line 865 "" */
 
      if (Tree_IsType(yyt->Unary.Op,kSize))
	yyt->Unary.PUOut=yyt->Unary.PUIn;
      else
	yyt->Unary.PUOut=yyt->Unary.Expr->Expr.PUIn;
    
break;

case kConst:
yyt->Const.Type->Type.PUIn=yyt->Const.PUIn;
yyVisit1 (yyt->Const.Type);
yyt->Const.PUOut=yyt->Const.Type->Type.PUOut;
break;

case kValof:
yyt->Valof.ExprList->ExprList.LV=yyt->Valof.LV;
/* line 883 "" */
 yyt->Valof.ExprList->ExprList.PAR = false;
/* line 776 "" */
 
    yyt->Valof.SpecList->SpecList.PUIn = DEP(CreateUsage(yyt->Valof.Pos),yyt->Valof.PUIn);
yyVisit1 (yyt->Valof.SpecList);
yyt->Valof.Process->Process.PUIn=yyt->Valof.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Valof.Process);
yyt->Valof.ExprList->ExprList.PUIn=yyt->Valof.Process->Process.PUOut;
yyVisit1 (yyt->Valof.ExprList);
/* line 777 "" */
 
    yyt->Valof.PUOut = CombineUsage(yyt->Valof.PUIn,yyt->Valof.ExprList->ExprList.PUOut,SELF->Valof.VS);
break;

case kFuncCall:
/* line 937 "" */
AddLValues(yyt->FuncCall.ExprList,false,false);
/* line 885 "" */
 yyt->FuncCall.ExprList->ExprList.PAR = false;
yyt->FuncCall.ExprList->ExprList.PUIn=yyt->FuncCall.PUIn;
yyVisit1 (yyt->FuncCall.ExprList);
/* line 876 "" */
 yyt->FuncCall.PUOut = CombineFuncUsage(yyt->FuncCall.PUIn,SymbolKind(yyt->FuncCall.Sym));
break;

case kElementalExpr:
/* line 1084 "" */
 yyt->ElementalExpr.Elemental->Elemental.RVL = NoTree;
yyt->ElementalExpr.Elemental->Elemental.LV=yyt->ElementalExpr.LV;
yyt->ElementalExpr.Elemental->Elemental.PUIn=yyt->ElementalExpr.PUIn;
yyVisit1 (yyt->ElementalExpr.Elemental);
/* line 844 "" */

yyt->ElementalExpr.PUOut=yyt->ElementalExpr.Elemental->Elemental.PUOut;
/* line 845 "" */
if (! ( (yyt->ElementalExpr.LV) )) { AddReadUsage(yyt->ElementalExpr.PUOut, yyt->ElementalExpr.Elemental)
    ; }
else
/* line 846 "" */
if (! ( (AddWriteUsage(yyt->ElementalExpr.PUIn,yyt->ElementalExpr.Elemental)||(!yyt->ElementalExpr.PAR)) )) { Message("Violation of occam usage rules",(IS_Mode(xxUsageWarnings))?xxWarning:xxError,yyt->ElementalExpr.Elemental->Elemental.Pos); }
break;

case kDefault:
yyt->Default.PUOut=yyt->Default.PUIn;
break;

case kConvert:
yyt->Convert.Type->Type.PUIn=yyt->Convert.PUIn;
yyt->Convert.Expr->Expr.LV=yyt->Convert.LV;
yyVisit1 (yyt->Convert.Type);
yyt->Convert.Expr->Expr.PUIn=yyt->Convert.Type->Type.PUOut;
yyVisit1 (yyt->Convert.Expr);
yyt->Convert.PUOut=yyt->Convert.Expr->Expr.PUOut;
break;

case kRound:
yyt->Round.Type->Type.PUIn=yyt->Round.PUIn;
yyt->Round.Expr->Expr.LV=yyt->Round.LV;
yyVisit1 (yyt->Round.Type);
yyt->Round.Expr->Expr.PUIn=yyt->Round.Type->Type.PUOut;
yyVisit1 (yyt->Round.Expr);
yyt->Round.PUOut=yyt->Round.Expr->Expr.PUOut;
break;

case kTrunc:
yyt->Trunc.Type->Type.PUIn=yyt->Trunc.PUIn;
yyt->Trunc.Expr->Expr.LV=yyt->Trunc.LV;
yyVisit1 (yyt->Trunc.Type);
yyt->Trunc.Expr->Expr.PUIn=yyt->Trunc.Type->Type.PUOut;
yyVisit1 (yyt->Trunc.Expr);
yyt->Trunc.PUOut=yyt->Trunc.Expr->Expr.PUOut;
break;

case kMostPos:
yyt->MostPos.Type->Type.PUIn=yyt->MostPos.PUIn;
yyVisit1 (yyt->MostPos.Type);
yyt->MostPos.PUOut=yyt->MostPos.Type->Type.PUOut;
break;

case kMostNeg:
yyt->MostNeg.Type->Type.PUIn=yyt->MostNeg.PUIn;
yyVisit1 (yyt->MostNeg.Type);
yyt->MostNeg.PUOut=yyt->MostNeg.Type->Type.PUOut;
break;

case kCountArray:
yyt->CountArray.Expr->Expr.LV=yyt->CountArray.LV;
yyt->CountArray.Length->Expr.LV=yyt->CountArray.LV;
yyt->CountArray.Length->Expr.PUIn=yyt->CountArray.PUIn;
yyVisit1 (yyt->CountArray.Length);
yyt->CountArray.Expr->Expr.PUIn=yyt->CountArray.Length->Expr.PUOut;
yyVisit1 (yyt->CountArray.Expr);
yyt->CountArray.PUOut=yyt->CountArray.Expr->Expr.PUOut;
break;

case kUnBound:
yyt->UnBound.PUOut=yyt->UnBound.PUIn;
break;

case kBytesIn:
yyt->BytesIn.PUOut=yyt->BytesIn.PUIn;
break;

case kBytesInType:
yyt->BytesInType.Type->Type.PUIn=yyt->BytesInType.PUIn;
yyVisit1 (yyt->BytesInType.Type);
yyt->BytesInType.PUOut=yyt->BytesInType.Type->Type.PUOut;
break;

case kBytesInExpr:
yyt->BytesInExpr.Expr->Expr.LV=yyt->BytesInExpr.LV;
yyt->BytesInExpr.Expr->Expr.PUIn=yyt->BytesInExpr.PUIn;
yyVisit1 (yyt->BytesInExpr.Expr);
yyt->BytesInExpr.PUOut=yyt->BytesInExpr.Expr->Expr.PUOut;
break;

case kSizeOf:
yyt->SizeOf.Type->Type.PUIn=yyt->SizeOf.PUIn;
yyVisit1 (yyt->SizeOf.Type);
yyt->SizeOf.PUOut=yyt->SizeOf.Type->Type.PUOut;
break;

case kOffsetOf:
yyt->OffsetOf.Type->Type.PUIn=yyt->OffsetOf.PUIn;
/* line 1092 "" */
 yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.RVL = NoTree;
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.LV=yyt->OffsetOf.LV;
yyVisit1 (yyt->OffsetOf.Type);
yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.PUIn=yyt->OffsetOf.Type->Type.PUOut;
yyVisit1 (yyt->OffsetOf.UnmappedIdentifier);
yyt->OffsetOf.PUOut=yyt->OffsetOf.UnmappedIdentifier->UnmappedIdentifier.PUOut;
break;

case kStringElement:
/* line 975 "" */
 yyt->StringElement.SE = false;
yyt->StringElement.PUOut=yyt->StringElement.PUIn;
break;

case kString:
/* line 976 "" */
 yyt->String.SE = CheckSimpleExpr(ExprType(SELF)->Array.Expr);
/* line 1034 "" */

    yyt->String.LE = (yyt->String.SE) ? CreateLinExpr(ExprType(SELF)->Array.Expr):NULL;
/* line 1035 "" */
if (! ( false )) { TransferLinExpr(ExprType(SELF),yyt->String.LE,yyt->String.SE); }
yyt->String.Type->Type.PUIn=yyt->String.PUIn;
yyVisit1 (yyt->String.Type);
yyt->String.PUOut=yyt->String.Type->Type.PUOut;
break;

case kStringSubscript:
yyt->StringSubscript.Next->StringElement.LV=yyt->StringSubscript.LV;
yyt->StringSubscript.Next->StringElement.PUIn=yyt->StringSubscript.PUIn;
yyVisit1 (yyt->StringSubscript.Next);
/* line 978 "" */
  yyt->StringSubscript.SE = CheckSimpleExpr(yyt->StringSubscript.Expr);
/* line 1048 "" */
 
    yyt->StringSubscript.LE = (yyt->StringSubscript.SE) ? CreateLinExpr(yyt->StringSubscript.Expr):NULL;
/* line 1049 "" */
if (! ( DEP(!(yyt->StringSubscript.SE && !IsConstInt(yyt->StringSubscript.Expr)),yyt->StringSubscript.Next->StringElement.SE) )) { SubscriptCheckLinExpr(SELF,yyt->StringSubscript.LE,ExprType(yyt->StringSubscript.Next)); }
/* line 927 "" */
 yyt->StringSubscript.Expr->Expr.LV = false;
yyt->StringSubscript.Expr->Expr.PUIn=yyt->StringSubscript.Next->StringElement.PUOut;
yyVisit1 (yyt->StringSubscript.Expr);
yyt->StringSubscript.PUOut=yyt->StringSubscript.Expr->Expr.PUOut;
break;

case kStringRange:
/* line 977 "" */
 yyt->StringRange.SE = CheckSimpleExpr(ExprType(SELF)->Array.Expr);
/* line 1040 "" */
 yyt->StringRange.ForLE = (yyt->StringRange.SE)?CreateLinExpr(yyt->StringRange.For):NULL;
/* line 1040 "" */
 yyt->StringRange.FromLE = (yyt->StringRange.SE)?CreateLinExpr(yyt->StringRange.From):NULL;
/* line 1042 "" */
if (! ( (!yyt->StringRange.SE) )) { RangeCheckLinExpr(SELF,yyt->StringRange.FromLE,yyt->StringRange.ForLE,ExprType(yyt->StringRange.Next)); }
/* line 928 "" */
 yyt->StringRange.For->Expr.LV = false;
/* line 928 "" */
 yyt->StringRange.From->Expr.LV = false;
yyt->StringRange.Next->StringElement.LV=yyt->StringRange.LV;
yyt->StringRange.Next->StringElement.PUIn=yyt->StringRange.PUIn;
yyVisit1 (yyt->StringRange.Next);
yyt->StringRange.From->Expr.PUIn=yyt->StringRange.Next->StringElement.PUOut;
yyVisit1 (yyt->StringRange.From);
yyt->StringRange.For->Expr.PUIn=yyt->StringRange.From->Expr.PUOut;
yyVisit1 (yyt->StringRange.For);
/* line 1041 "" */
if (! ( DEP(false,yyt->StringRange.Next->StringElement.SE) )) { TransferLinExpr(ExprType(SELF),yyt->StringRange.ForLE,yyt->StringRange.SE); }
yyt->StringRange.PUOut=yyt->StringRange.For->Expr.PUOut;
break;

case kElemental:
/* line 968 "" */
 yyt->Elemental.SE = false ;
yyt->Elemental.PUOut=yyt->Elemental.PUIn;
break;

case kTableElement:
/* line 980 "" */
 yyt->TableElement.SE = false;
yyt->TableElement.PUOut=yyt->TableElement.PUIn;
break;

case kTable:
/* line 923 "" */
AddLValues(yyt->Table.ExprList,yyt->Table.LV,yyt->Table.LV);
/* line 982 "" */
 yyt->Table.SE = (Tree_IsType(ElementalType(SELF),kArray)) ? CheckSimpleExpr(ElementalType(SELF)->Array.Expr) : false;
/* line 1055 "" */
 
    yyt->Table.LE = (yyt->Table.SE) ? CreateLinExpr(ElementalType(SELF)->Array.Expr):NULL;
/* line 1056 "" */
if (! ( DEP(false,yyt->Table.ExprList->ExprList.LV) )) { TransferLinExpr(ElementalType(SELF),yyt->Table.LE,yyt->Table.SE); }
/* line 884 "" */
 yyt->Table.ExprList->ExprList.PAR = false;
/* line 811 "" */
 
    yyt->Table.ExprList->ExprList.PUIn=CreateUsage(yyt->Table.Pos);
yyVisit1 (yyt->Table.ExprList);
/* line 812 "" */
 yyt->Table.PUOut = (yyt->Table.PUIn==NoParUsage)?NoParUsage : CombineUsage(yyt->Table.PUIn,yyt->Table.ExprList->ExprList.PUOut,BIGDEPTH); 
break;

case kUserLiteral:
/* line 923 "" */
AddLValues(yyt->UserLiteral.ExprList,yyt->UserLiteral.LV,yyt->UserLiteral.LV);
/* line 981 "" */
 yyt->UserLiteral.SE = false;
/* line 1055 "" */
 
    yyt->UserLiteral.LE = (yyt->UserLiteral.SE) ? CreateLinExpr(ElementalType(SELF)->Array.Expr):NULL;
/* line 1056 "" */
if (! ( DEP(false,yyt->UserLiteral.ExprList->ExprList.LV) )) { TransferLinExpr(ElementalType(SELF),yyt->UserLiteral.LE,yyt->UserLiteral.SE); }
/* line 735 "" */

    yyt->UserLiteral.ExprList->ExprList.PAR = false;
/* line 811 "" */
 
    yyt->UserLiteral.ExprList->ExprList.PUIn=CreateUsage(yyt->UserLiteral.Pos);
yyVisit1 (yyt->UserLiteral.ExprList);
yyt->UserLiteral.Type->Type.PUIn=yyt->UserLiteral.ExprList->ExprList.PUOut;
yyVisit1 (yyt->UserLiteral.Type);
/* line 812 "" */
 yyt->UserLiteral.PUOut = (yyt->UserLiteral.PUIn==NoParUsage)?NoParUsage : CombineUsage(yyt->UserLiteral.PUIn,yyt->UserLiteral.ExprList->ExprList.PUOut,BIGDEPTH); 
break;

case kTableExpr:
yyt->TableExpr.Expr->Expr.LV=yyt->TableExpr.LV;
yyt->TableExpr.Expr->Expr.PUIn=yyt->TableExpr.PUIn;
yyVisit1 (yyt->TableExpr.Expr);
/* line 980 "" */
 yyt->TableExpr.SE = false;
yyt->TableExpr.PUOut=yyt->TableExpr.Expr->Expr.PUOut;
break;

case kTableSubscript:
/* line 1085 "" */
 yyt->TableSubscript.Next->TableElement.RVL = SELF;
yyt->TableSubscript.Next->TableElement.LV=yyt->TableSubscript.LV;
yyt->TableSubscript.Next->TableElement.PUIn=yyt->TableSubscript.PUIn;
yyVisit1 (yyt->TableSubscript.Next);
/* line 984 "" */
  yyt->TableSubscript.SE = CheckSimpleExpr(yyt->TableSubscript.Expr);
/* line 1069 "" */
 
    yyt->TableSubscript.LE = (yyt->TableSubscript.SE) ? CreateLinExpr(yyt->TableSubscript.Expr):NULL;
/* line 1070 "" */
if (! ( DEP(!(yyt->TableSubscript.SE && !IsConstInt(yyt->TableSubscript.Expr)),yyt->TableSubscript.Next->TableElement.SE) )) { SubscriptCheckLinExpr(SELF,yyt->TableSubscript.LE,ElementType(yyt->TableSubscript.Next)); }
/* line 924 "" */
 yyt->TableSubscript.Expr->Expr.LV = false;
yyt->TableSubscript.Expr->Expr.PUIn=yyt->TableSubscript.Next->TableElement.PUOut;
yyVisit1 (yyt->TableSubscript.Expr);
yyt->TableSubscript.PUOut=yyt->TableSubscript.Expr->Expr.PUOut;
break;

case kTableRange:
/* line 983 "" */
 yyt->TableRange.SE = CheckSimpleExpr(yyt->TableRange.From) && CheckSimpleExpr(yyt->TableRange.For);
/* line 1061 "" */
 yyt->TableRange.ForLE = (yyt->TableRange.SE)?CreateLinExpr(yyt->TableRange.For):NULL;
/* line 1061 "" */
 yyt->TableRange.FromLE = (yyt->TableRange.SE)?CreateLinExpr(yyt->TableRange.From):NULL;
/* line 1063 "" */
if (! ( (!yyt->TableRange.SE) )) { RangeCheckLinExpr(SELF,yyt->TableRange.FromLE,yyt->TableRange.ForLE,ElementType(yyt->TableRange.Next)); }
/* line 925 "" */
 yyt->TableRange.For->Expr.LV = false;
/* line 925 "" */
 yyt->TableRange.From->Expr.LV = false;
/* line 1086 "" */
 yyt->TableRange.Next->TableElement.RVL = SELF;
yyt->TableRange.Next->TableElement.LV=yyt->TableRange.LV;
yyt->TableRange.Next->TableElement.PUIn=yyt->TableRange.PUIn;
yyVisit1 (yyt->TableRange.Next);
yyt->TableRange.From->Expr.PUIn=yyt->TableRange.Next->TableElement.PUOut;
yyVisit1 (yyt->TableRange.From);
yyt->TableRange.For->Expr.PUIn=yyt->TableRange.From->Expr.PUOut;
yyVisit1 (yyt->TableRange.For);
/* line 1062 "" */
if (! ( DEP(false,yyt->TableRange.Next->TableElement.SE) )) { TransferLinExpr(ElementalType(SELF),yyt->TableRange.ForLE,yyt->TableRange.SE); }
yyt->TableRange.PUOut=yyt->TableRange.For->Expr.PUOut;
break;

case kElement:
/* line 970 "" */
 yyt->Element.SE = false ;
yyt->Element.PUOut=yyt->Element.PUIn;
break;

case kIdentifier:
/* line 971 "" */
 yyt->Identifier.SE = false;
yyt->Identifier.PUOut=yyt->Identifier.PUIn;
break;

case kUnmappedIdentifier:
/* line 971 "" */
 yyt->UnmappedIdentifier.SE = false;
yyt->UnmappedIdentifier.PUOut=yyt->UnmappedIdentifier.PUIn;
break;

case kElementAccess:
/* line 1087 "" */
 yyt->ElementAccess.Next->Element.RVL = SELF;
yyt->ElementAccess.Next->Element.LV=yyt->ElementAccess.LV;
yyt->ElementAccess.Next->Element.PUIn=yyt->ElementAccess.PUIn;
yyVisit1 (yyt->ElementAccess.Next);
/* line 970 "" */
 yyt->ElementAccess.SE = false ;
yyt->ElementAccess.PUOut=yyt->ElementAccess.Next->Element.PUOut;
break;

case kSubscript:
/* line 1087 "" */
 yyt->Subscript.Next->Element.RVL = SELF;
yyt->Subscript.Next->Element.LV=yyt->Subscript.LV;
yyt->Subscript.Next->Element.PUIn=yyt->Subscript.PUIn;
yyVisit1 (yyt->Subscript.Next);
/* line 973 "" */
  yyt->Subscript.SE = CheckSimpleExpr(yyt->Subscript.Expr);
/* line 1027 "" */
 
    yyt->Subscript.LE = (yyt->Subscript.SE) ? CreateLinExpr(yyt->Subscript.Expr):NULL;
/* line 1028 "" */
if (! ( DEP(!(yyt->Subscript.SE && !IsConstInt(yyt->Subscript.Expr)),yyt->Subscript.Next->Element.SE) )) { SubscriptCheckLinExpr(SELF,yyt->Subscript.LE,ElementType(yyt->Subscript.Next)); }
/* line 920 "" */
 yyt->Subscript.Expr->Expr.LV = false;
yyt->Subscript.Expr->Expr.PUIn=yyt->Subscript.Next->Element.PUOut;
yyVisit1 (yyt->Subscript.Expr);
yyt->Subscript.PUOut=yyt->Subscript.Expr->Expr.PUOut;
break;

case kRange:
/* line 972 "" */
 yyt->Range.SE = CheckSimpleExpr(yyt->Range.From) && CheckSimpleExpr(yyt->Range.For);
/* line 1019 "" */
 yyt->Range.ForLE = (yyt->Range.SE)?CreateLinExpr(yyt->Range.For):NULL;
/* line 1019 "" */
 yyt->Range.FromLE = (yyt->Range.SE)?CreateLinExpr(yyt->Range.From):NULL;
/* line 1021 "" */
if (! ( (!yyt->Range.SE) )) { RangeCheckLinExpr(SELF,yyt->Range.FromLE,yyt->Range.ForLE,ElementType(yyt->Range.Next)); }
/* line 921 "" */
 yyt->Range.For->Expr.LV = false;
/* line 921 "" */
 yyt->Range.From->Expr.LV = false;
/* line 1087 "" */
 yyt->Range.Next->Element.RVL = SELF;
yyt->Range.Next->Element.LV=yyt->Range.LV;
yyt->Range.Next->Element.PUIn=yyt->Range.PUIn;
yyVisit1 (yyt->Range.Next);
yyt->Range.From->Expr.PUIn=yyt->Range.Next->Element.PUOut;
yyVisit1 (yyt->Range.From);
yyt->Range.For->Expr.PUIn=yyt->Range.From->Expr.PUOut;
yyVisit1 (yyt->Range.For);
/* line 1020 "" */
if (! ( DEP(false,yyt->Range.Next->Element.SE) )) { TransferLinExpr(ElementType(SELF),yyt->Range.ForLE,yyt->Range.SE) ; }
yyt->Range.PUOut=yyt->Range.For->Expr.PUOut;
break;

case kExprList:
yyt->ExprList.PUOut=yyt->ExprList.PUIn;
break;

case kNoExpr:
yyt->NoExpr.PUOut=yyt->NoExpr.PUIn;
break;

case kAnExpr:
yyt->AnExpr.Next->ExprList.PUIn=yyt->AnExpr.PUIn;
yyt->AnExpr.Expr->Expr.LV=yyt->AnExpr.LV;
/* line 944 "" */

yyt->AnExpr.Next->ExprList.PAR=yyt->AnExpr.PAR;
yyVisit1 (yyt->AnExpr.Next);
yyt->AnExpr.Expr->Expr.PUIn=yyt->AnExpr.Next->ExprList.PUOut;
yyVisit1 (yyt->AnExpr.Expr);
yyt->AnExpr.PUOut=yyt->AnExpr.Expr->Expr.PUOut;
/* line 852 "" */
if (! ( !(yyt->AnExpr.LV && AddReadUsage(yyt->AnExpr.PUOut,yyt->AnExpr.Expr->ElementalExpr.Elemental)) )) { TempRequired(SELF)=false;; }
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
yyt->Process.SpecList->SpecList.PUIn=yyt->Process.PUIn;
yyVisit1 (yyt->Process.SpecList);
yyt->Process.PUOut=yyt->Process.SpecList->SpecList.PUOut;
break;

case kAssign:
yyt->Assign.SpecList->SpecList.PUIn=yyt->Assign.PUIn;
yyVisit1 (yyt->Assign.SpecList);
/* line 820 "" */
 
    yyt->Assign.Rval->ExprList.PUIn = DEP(CreateUsage(yyt->Assign.Pos),yyt->Assign.SpecList->SpecList.PUOut);
/* line 910 "" */
AddLValues(yyt->Assign.Rval,false,false);
/* line 819 "" */
 
    yyt->Assign.Rval->ExprList.PAR=false;
yyVisit1 (yyt->Assign.Rval);
/* line 821 "" */
 
    yyt->Assign.Lval->ExprList.PUIn = yyt->Assign.Rval->ExprList.PUOut;
/* line 910 "" */
AddLValues(yyt->Assign.Lval,true,true);
/* line 818 "" */
 
    yyt->Assign.Lval->ExprList.PAR = true;
yyVisit1 (yyt->Assign.Lval);
/* line 822 "" */

    yyt->Assign.PUOut = CombineUsage(yyt->Assign.SpecList->SpecList.PUOut,yyt->Assign.Lval->ExprList.PUOut,BIGDEPTH);
break;

case kAltType:
yyt->AltType.SpecList->SpecList.PUIn=yyt->AltType.PUIn;
yyVisit1 (yyt->AltType.SpecList);
yyt->AltType.PUOut=yyt->AltType.SpecList->SpecList.PUOut;
break;

case kAlt:
yyt->Alt.SpecList->SpecList.PUIn=yyt->Alt.PUIn;
yyVisit1 (yyt->Alt.SpecList);
yyt->Alt.AltList->AltList.PUIn=yyt->Alt.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Alt.AltList);
yyt->Alt.PUOut=yyt->Alt.AltList->AltList.PUOut;
break;

case kAltRep:
yyt->AltRep.SpecList->SpecList.PUIn=yyt->AltRep.PUIn;
yyVisit1 (yyt->AltRep.SpecList);
yyt->AltRep.Replicator->Replicator.PUIn=yyt->AltRep.SpecList->SpecList.PUOut;
yyVisit1 (yyt->AltRep.Replicator);
yyt->AltRep.AltList->AltList.PUIn=yyt->AltRep.Replicator->Replicator.PUOut;
yyVisit1 (yyt->AltRep.AltList);
yyt->AltRep.PUOut=yyt->AltRep.AltList->AltList.PUOut;
break;

case kParType:
yyt->ParType.SpecList->SpecList.PUIn=yyt->ParType.PUIn;
yyVisit1 (yyt->ParType.SpecList);
yyt->ParType.PUOut=yyt->ParType.SpecList->SpecList.PUOut;
break;

case kPar:
yyt->Par.ProcList->ProcList.Pos=yyt->Par.Pos;
/* line 721 "" */
 
    yyt->Par.ProcList->ProcList.PAR = true;
yyt->Par.SpecList->SpecList.PUIn=yyt->Par.PUIn;
yyVisit1 (yyt->Par.SpecList);
yyt->Par.ProcList->ProcList.PUIn=yyt->Par.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Par.ProcList);
/* line 722 "" */

    yyt->Par.PUOut = CombineUsage(yyt->Par.PUIn,yyt->Par.ProcList->ProcList.PUOut,BIGDEPTH);
break;

case kParRep:
yyt->ParRep.SpecList->SpecList.PUIn=yyt->ParRep.PUIn;
yyVisit1 (yyt->ParRep.SpecList);
yyt->ParRep.Replicator->Replicator.PUIn=yyt->ParRep.SpecList->SpecList.PUOut;
yyVisit1 (yyt->ParRep.Replicator);
/* line 728 "" */
 
    yyt->ParRep.Process->Process.PUIn = DEP(CreateUsage(yyt->ParRep.Pos),yyt->ParRep.Replicator->Replicator.PUOut);
yyVisit1 (yyt->ParRep.Process);
/* line 729 "" */
 
    yyt->ParRep.PUOut = CheckRepUsage(yyt->ParRep.PUIn, yyt->ParRep.Process->Process.PUOut, yyt->ParRep.Replicator,yyt->ParRep.Pos, Symbol(yyt->ParRep.Sym).Depth);
break;

case kProcessor:
/* line 915 "" */
 yyt->Processor.Expr->Expr.LV = false;
yyt->Processor.SpecList->SpecList.PUIn=yyt->Processor.PUIn;
yyVisit1 (yyt->Processor.SpecList);
yyt->Processor.Expr->Expr.PUIn=yyt->Processor.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Processor.Expr);
yyt->Processor.Process->Process.PUIn=yyt->Processor.Expr->Expr.PUOut;
yyVisit1 (yyt->Processor.Process);
yyt->Processor.PUOut=yyt->Processor.Process->Process.PUOut;
break;

case kIfType:
yyt->IfType.SpecList->SpecList.PUIn=yyt->IfType.PUIn;
yyVisit1 (yyt->IfType.SpecList);
yyt->IfType.PUOut=yyt->IfType.SpecList->SpecList.PUOut;
break;

case kIf:
yyt->If.SpecList->SpecList.PUIn=yyt->If.PUIn;
yyVisit1 (yyt->If.SpecList);
yyt->If.ChoiceList->ChoiceList.PUIn=yyt->If.SpecList->SpecList.PUOut;
yyVisit1 (yyt->If.ChoiceList);
yyt->If.PUOut=yyt->If.ChoiceList->ChoiceList.PUOut;
break;

case kIfRep:
yyt->IfRep.SpecList->SpecList.PUIn=yyt->IfRep.PUIn;
yyVisit1 (yyt->IfRep.SpecList);
yyt->IfRep.Replicator->Replicator.PUIn=yyt->IfRep.SpecList->SpecList.PUOut;
yyVisit1 (yyt->IfRep.Replicator);
yyt->IfRep.ChoiceList->ChoiceList.PUIn=yyt->IfRep.Replicator->Replicator.PUOut;
yyVisit1 (yyt->IfRep.ChoiceList);
yyt->IfRep.PUOut=yyt->IfRep.ChoiceList->ChoiceList.PUOut;
break;

case kInput:
yyt->Input.SpecList->SpecList.PUIn=yyt->Input.PUIn;
/* line 1088 "" */
 yyt->Input.Element->Element.RVL = NoTree;
/* line 930 "" */
 yyt->Input.Element->Element.LV = true;
yyVisit1 (yyt->Input.SpecList);
yyt->Input.Element->Element.PUIn=yyt->Input.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Input.Element);
yyt->Input.ExprList->ExprList.PUIn=yyt->Input.Element->Element.PUOut;
/* line 930 "" */
AddLValues(yyt->Input.ExprList,true,false);
/* line 886 "" */
 yyt->Input.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->Input.ExprList);
/* line 877 "" */
if (! ( AddWriteUsage(yyt->Input.ExprList->ExprList.PUOut,yyt->Input.Element))) {; }
yyt->Input.PUOut=yyt->Input.ExprList->ExprList.PUOut;
break;

case kOutput:
yyt->Output.SpecList->SpecList.PUIn=yyt->Output.PUIn;
/* line 1089 "" */
 yyt->Output.Element->Element.RVL = NoTree;
/* line 931 "" */
 yyt->Output.Element->Element.LV = true;
yyVisit1 (yyt->Output.SpecList);
yyt->Output.Element->Element.PUIn=yyt->Output.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Output.Element);
yyt->Output.ExprList->ExprList.PUIn=yyt->Output.Element->Element.PUOut;
/* line 931 "" */
AddLValues(yyt->Output.ExprList,false,false);
/* line 887 "" */
 yyt->Output.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->Output.ExprList);
/* line 878 "" */
if (! ( AddWriteUsage(yyt->Output.ExprList->ExprList.PUOut,yyt->Output.Element))) {; }
yyt->Output.PUOut=yyt->Output.ExprList->ExprList.PUOut;
break;

case kCaseIn:
yyt->CaseIn.SpecList->SpecList.PUIn=yyt->CaseIn.PUIn;
/* line 1090 "" */
 yyt->CaseIn.Element->Element.RVL = NoTree;
/* line 933 "" */
 yyt->CaseIn.Element->Element.LV = true;
yyVisit1 (yyt->CaseIn.SpecList);
yyt->CaseIn.Element->Element.PUIn=yyt->CaseIn.SpecList->SpecList.PUOut;
yyVisit1 (yyt->CaseIn.Element);
yyt->CaseIn.CaseList->CaseList.PUIn=yyt->CaseIn.Element->Element.PUOut;
yyVisit1 (yyt->CaseIn.CaseList);
/* line 879 "" */
if (! ( AddWriteUsage(yyt->CaseIn.CaseList->CaseList.PUOut,yyt->CaseIn.Element))) {; }
yyt->CaseIn.PUOut=yyt->CaseIn.CaseList->CaseList.PUOut;
break;

case kAfter:
yyt->After.SpecList->SpecList.PUIn=yyt->After.PUIn;
/* line 932 "" */
 yyt->After.Expr->Expr.LV = false;
/* line 1091 "" */
 yyt->After.Element->Element.RVL = NoTree;
/* line 932 "" */
 yyt->After.Element->Element.LV = true;
yyVisit1 (yyt->After.SpecList);
yyt->After.Element->Element.PUIn=yyt->After.SpecList->SpecList.PUOut;
yyVisit1 (yyt->After.Element);
yyt->After.Expr->Expr.PUIn=yyt->After.Element->Element.PUOut;
yyVisit1 (yyt->After.Expr);
/* line 880 "" */
if (! ( AddWriteUsage(yyt->After.Expr->Expr.PUOut,yyt->After.Element))) {; }
yyt->After.PUOut=yyt->After.Expr->Expr.PUOut;
break;

case kSeqRep:
yyt->SeqRep.SpecList->SpecList.PUIn=yyt->SeqRep.PUIn;
yyVisit1 (yyt->SeqRep.SpecList);
yyt->SeqRep.Replicator->Replicator.PUIn=yyt->SeqRep.SpecList->SpecList.PUOut;
yyVisit1 (yyt->SeqRep.Replicator);
yyt->SeqRep.Process->Process.PUIn=yyt->SeqRep.Replicator->Replicator.PUOut;
yyVisit1 (yyt->SeqRep.Process);
yyt->SeqRep.PUOut=yyt->SeqRep.Process->Process.PUOut;
break;

case kSeq:
yyt->Seq.SpecList->SpecList.PUIn=yyt->Seq.PUIn;
yyt->Seq.ProcList->ProcList.Pos=yyt->Seq.Pos;
/* line 715 "" */
 
    yyt->Seq.ProcList->ProcList.PAR = false;
yyVisit1 (yyt->Seq.SpecList);
yyt->Seq.ProcList->ProcList.PUIn=yyt->Seq.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Seq.ProcList);
yyt->Seq.PUOut=yyt->Seq.ProcList->ProcList.PUOut;
break;

case kCase:
yyt->Case.SpecList->SpecList.PUIn=yyt->Case.PUIn;
/* line 934 "" */
 yyt->Case.Selector->Expr.LV = false;
yyVisit1 (yyt->Case.SpecList);
yyt->Case.Selector->Expr.PUIn=yyt->Case.SpecList->SpecList.PUOut;
yyVisit1 (yyt->Case.Selector);
yyt->Case.ChoiceList->ChoiceList.PUIn=yyt->Case.Selector->Expr.PUOut;
yyVisit1 (yyt->Case.ChoiceList);
yyt->Case.PUOut=yyt->Case.ChoiceList->ChoiceList.PUOut;
break;

case kWhile:
yyt->While.SpecList->SpecList.PUIn=yyt->While.PUIn;
/* line 935 "" */
 yyt->While.Expr->Expr.LV = false;
yyVisit1 (yyt->While.SpecList);
yyt->While.Expr->Expr.PUIn=yyt->While.SpecList->SpecList.PUOut;
yyVisit1 (yyt->While.Expr);
yyt->While.Process->Process.PUIn=yyt->While.Expr->Expr.PUOut;
yyVisit1 (yyt->While.Process);
yyt->While.PUOut=yyt->While.Process->Process.PUOut;
break;

case kSkip:
yyt->Skip.SpecList->SpecList.PUIn=yyt->Skip.PUIn;
yyVisit1 (yyt->Skip.SpecList);
yyt->Skip.PUOut=yyt->Skip.SpecList->SpecList.PUOut;
break;

case kStop:
yyt->Stop.SpecList->SpecList.PUIn=yyt->Stop.PUIn;
yyVisit1 (yyt->Stop.SpecList);
yyt->Stop.PUOut=yyt->Stop.SpecList->SpecList.PUOut;
break;

case kInstance:
yyt->Instance.SpecList->SpecList.PUIn=yyt->Instance.PUIn;
yyVisit1 (yyt->Instance.SpecList);
/* line 829 "" */

    yyt->Instance.ExprList->ExprList.PUIn = DEP(CreateUsage(yyt->Instance.Pos),yyt->Instance.SpecList->SpecList.PUOut);
/* line 936 "" */
AddInstanceLValues(SymbolKind(yyt->Instance.Sym)->DefAProc.Formals,yyt->Instance.ExprList);
/* line 828 "" */
 
    yyt->Instance.ExprList->ExprList.PAR = true;
yyVisit1 (yyt->Instance.ExprList);
/* line 830 "" */

      if (Tree_IsType(SymbolKind(yyt->Instance.Sym),kDefProc)) 
      {
	AllowNoAbbrevInProc(yyt->Instance.Sym,yyt->Instance.Scope, SymbolKind(yyt->Instance.Sym)->DefProc.Process->Process.PUOut,yyt->Instance.Pos);
	yyt->Instance.PUOut = CheckUsage(yyt->Instance.ExprList->ExprList.PUOut,SymbolKind(yyt->Instance.Sym)->DefProc.Process->Process.PUOut,yyt->Instance.Pos,Symbol(yyt->Instance.Sym).Depth);
      } ; 
      yyt->Instance.PUOut = CombineUsage(yyt->Instance.SpecList->SpecList.PUOut,yyt->Instance.ExprList->ExprList.PUOut,BIGDEPTH); 
    
break;

case kCCode:
yyt->CCode.SpecList->SpecList.PUIn=yyt->CCode.PUIn;
yyVisit1 (yyt->CCode.SpecList);
yyt->CCode.PUOut=yyt->CCode.SpecList->SpecList.PUOut;
break;

case kNullProcess:
yyt->NullProcess.SpecList->SpecList.PUIn=yyt->NullProcess.PUIn;
yyVisit1 (yyt->NullProcess.SpecList);
yyt->NullProcess.PUOut=yyt->NullProcess.SpecList->SpecList.PUOut;
break;

case kAltList:
yyt->AltList.PUOut=yyt->AltList.PUIn;
break;

case kNoAlt:
yyt->NoAlt.PUOut=yyt->NoAlt.PUIn;
break;

case kAnAlts:
yyt->AnAlts.SpecList->SpecList.PUIn=yyt->AnAlts.PUIn;
yyVisit1 (yyt->AnAlts.SpecList);
yyt->AnAlts.Next->AltList.PUIn=yyt->AnAlts.SpecList->SpecList.PUOut;
yyVisit1 (yyt->AnAlts.Next);
yyt->AnAlts.PUOut=yyt->AnAlts.Next->AltList.PUOut;
break;

case kAnAlt:
yyt->AnAlt.SpecList->SpecList.PUIn=yyt->AnAlt.PUIn;
yyVisit1 (yyt->AnAlt.SpecList);
yyt->AnAlt.Next->AltList.PUIn=yyt->AnAlt.SpecList->SpecList.PUOut;
yyVisit1 (yyt->AnAlt.Next);
yyt->AnAlt.Guard->Guard.PUIn=yyt->AnAlt.Next->AltList.PUOut;
yyVisit1 (yyt->AnAlt.Guard);
yyt->AnAlt.PUOut=yyt->AnAlt.Guard->Guard.PUOut;
break;

case kANewAlt:
yyt->ANewAlt.SpecList->SpecList.PUIn=yyt->ANewAlt.PUIn;
yyVisit1 (yyt->ANewAlt.SpecList);
yyt->ANewAlt.Next->AltList.PUIn=yyt->ANewAlt.SpecList->SpecList.PUOut;
yyVisit1 (yyt->ANewAlt.Next);
yyt->ANewAlt.AltType->AltType.PUIn=yyt->ANewAlt.Next->AltList.PUOut;
yyVisit1 (yyt->ANewAlt.AltType);
yyt->ANewAlt.PUOut=yyt->ANewAlt.AltType->AltType.PUOut;
break;

case kGuard:
/* line 940 "" */
 yyt->Guard.Expr->Expr.LV = false;
yyt->Guard.Expr->Expr.PUIn=yyt->Guard.PUIn;
yyVisit1 (yyt->Guard.Expr);
yyt->Guard.PUOut=yyt->Guard.Expr->Expr.PUOut;
break;

case kAltInput:
/* line 940 "" */
 yyt->AltInput.Expr->Expr.LV = false;
yyt->AltInput.Expr->Expr.PUIn=yyt->AltInput.PUIn;
yyVisit1 (yyt->AltInput.Expr);
yyt->AltInput.Input->Input.PUIn=yyt->AltInput.Expr->Expr.PUOut;
yyVisit1 (yyt->AltInput.Input);
yyt->AltInput.Process->Process.PUIn=yyt->AltInput.Input->Input.PUOut;
yyVisit1 (yyt->AltInput.Process);
yyt->AltInput.PUOut=yyt->AltInput.Process->Process.PUOut;
break;

case kAltTimer:
/* line 940 "" */
 yyt->AltTimer.Expr->Expr.LV = false;
yyt->AltTimer.Expr->Expr.PUIn=yyt->AltTimer.PUIn;
yyVisit1 (yyt->AltTimer.Expr);
yyt->AltTimer.After->After.PUIn=yyt->AltTimer.Expr->Expr.PUOut;
yyVisit1 (yyt->AltTimer.After);
yyt->AltTimer.Process->Process.PUIn=yyt->AltTimer.After->After.PUOut;
yyVisit1 (yyt->AltTimer.Process);
yyt->AltTimer.PUOut=yyt->AltTimer.Process->Process.PUOut;
break;

case kAltCaseIn:
/* line 940 "" */
 yyt->AltCaseIn.Expr->Expr.LV = false;
yyt->AltCaseIn.Expr->Expr.PUIn=yyt->AltCaseIn.PUIn;
yyVisit1 (yyt->AltCaseIn.Expr);
yyt->AltCaseIn.CaseIn->CaseIn.PUIn=yyt->AltCaseIn.Expr->Expr.PUOut;
yyVisit1 (yyt->AltCaseIn.CaseIn);
yyt->AltCaseIn.PUOut=yyt->AltCaseIn.CaseIn->CaseIn.PUOut;
break;

case kNoInput:
/* line 940 "" */
 yyt->NoInput.Expr->Expr.LV = false;
yyt->NoInput.Expr->Expr.PUIn=yyt->NoInput.PUIn;
yyVisit1 (yyt->NoInput.Expr);
yyt->NoInput.Process->Process.PUIn=yyt->NoInput.Expr->Expr.PUOut;
yyVisit1 (yyt->NoInput.Process);
yyt->NoInput.PUOut=yyt->NoInput.Process->Process.PUOut;
break;

case kChoiceList:
yyt->ChoiceList.PUOut=yyt->ChoiceList.PUIn;
break;

case kNoChoice:
yyt->NoChoice.PUOut=yyt->NoChoice.PUIn;
break;

case kAChoices:
yyt->AChoices.SpecList->SpecList.PUIn=yyt->AChoices.PUIn;
yyVisit1 (yyt->AChoices.SpecList);
yyt->AChoices.Next->ChoiceList.PUIn=yyt->AChoices.SpecList->SpecList.PUOut;
yyVisit1 (yyt->AChoices.Next);
yyt->AChoices.PUOut=yyt->AChoices.Next->ChoiceList.PUOut;
break;

case kAChoice:
yyt->AChoice.SpecList->SpecList.PUIn=yyt->AChoice.PUIn;
yyVisit1 (yyt->AChoice.SpecList);
yyt->AChoice.Next->ChoiceList.PUIn=yyt->AChoice.SpecList->SpecList.PUOut;
yyVisit1 (yyt->AChoice.Next);
yyt->AChoice.ExprList->ExprList.PUIn=yyt->AChoice.Next->ChoiceList.PUOut;
/* line 941 "" */
AddLValues(yyt->AChoice.ExprList,false,false);
/* line 888 "" */
 yyt->AChoice.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->AChoice.ExprList);
yyt->AChoice.Process->Process.PUIn=yyt->AChoice.ExprList->ExprList.PUOut;
yyVisit1 (yyt->AChoice.Process);
yyt->AChoice.PUOut=yyt->AChoice.Process->Process.PUOut;
break;

case kANewIf:
yyt->ANewIf.SpecList->SpecList.PUIn=yyt->ANewIf.PUIn;
yyVisit1 (yyt->ANewIf.SpecList);
yyt->ANewIf.Next->ChoiceList.PUIn=yyt->ANewIf.SpecList->SpecList.PUOut;
yyVisit1 (yyt->ANewIf.Next);
yyt->ANewIf.IfType->IfType.PUIn=yyt->ANewIf.Next->ChoiceList.PUOut;
yyVisit1 (yyt->ANewIf.IfType);
yyt->ANewIf.PUOut=yyt->ANewIf.IfType->IfType.PUOut;
break;

case kProcList:
yyt->ProcList.PUOut=yyt->ProcList.PUIn;
break;

case kNoProcess:
/* line 748 "" */
 
    yyt->NoProcess.PUOut = (yyt->NoProcess.PAR) ? CreateUsage(yyt->NoProcess.Pos) : yyt->NoProcess.PUIn;
break;

case kAProcess:
yyt->AProcess.Next->ProcList.Pos=yyt->AProcess.Pos;
yyt->AProcess.Next->ProcList.PAR=yyt->AProcess.PAR;
yyt->AProcess.Next->ProcList.PUIn=yyt->AProcess.PUIn;
yyVisit1 (yyt->AProcess.Next);
/* line 741 "" */
 
    yyt->AProcess.Process->Process.PUIn = (yyt->AProcess.PAR) ? CreateUsage(yyt->AProcess.Pos) : yyt->AProcess.Next->ProcList.PUOut;
yyVisit1 (yyt->AProcess.Process);
/* line 742 "" */
 
    yyt->AProcess.PUOut = (yyt->AProcess.PAR) ? CheckUsage(yyt->AProcess.Next->ProcList.PUOut,yyt->AProcess.Process->Process.PUOut,yyt->AProcess.Process->Process.Pos,Symbol(yyt->AProcess.Sym).Depth) : yyt->AProcess.Next->ProcList.PUOut;
break;

case kCaseList:
yyt->CaseList.PUOut=yyt->CaseList.PUIn;
break;

case kNoCase:
yyt->NoCase.PUOut=yyt->NoCase.PUIn;
break;

case kACase:
yyt->ACase.Next->CaseList.PUIn=yyt->ACase.PUIn;
yyVisit1 (yyt->ACase.Next);
yyt->ACase.SpecList->SpecList.PUIn=yyt->ACase.Next->CaseList.PUOut;
yyVisit1 (yyt->ACase.SpecList);
yyt->ACase.ExprList->ExprList.PUIn=yyt->ACase.SpecList->SpecList.PUOut;
/* line 942 "" */
AddLValues(yyt->ACase.ExprList,false,false);
/* line 889 "" */
 yyt->ACase.ExprList->ExprList.PAR = false;
yyVisit1 (yyt->ACase.ExprList);
yyt->ACase.Process->Process.PUIn=yyt->ACase.ExprList->ExprList.PUOut;
yyVisit1 (yyt->ACase.Process);
yyt->ACase.PUOut=yyt->ACase.Process->Process.PUOut;
break;

case kAccessList:
break;

case kAnAccess:
yyVisit1 (yyt->AnAccess.ElementList);
yyVisit1 (yyt->AnAccess.Next);
break;

case kNoAccess:
break;

case kElementList:
break;

case kAnElement:
yyVisit1 (yyt->AnElement.Next);
/* line 1093 "" */
 yyt->AnElement.Element->Element.RVL = NoTree;
/* line 945 "" */

/* line 875 "" */
 Message("internal error- traversed AnElement node",xxFatal,yyt->AnElement.Element->Element.Pos); 
yyVisit1 (yyt->AnElement.Element);
break;

case kNoElement:
break;
 default: ;
 }
}

void BeginParUsageAg ()
{
/* line 694 "" */

  BeginParUsage();

/* line 1004 "" */

  InitOmega();

}

void CloseParUsageAg ()
{
/* line 700 "" */

  CloseParUsage();

}
