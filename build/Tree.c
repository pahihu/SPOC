# include "Tree.h"
# define yyALLOC(ptr, size)	if ((ptr = (tTree) Tree_PoolFreePtr) >= (tTree) Tree_PoolMaxPtr) \
  ptr = Tree_Alloc (); \
  Tree_PoolFreePtr += size;
# define yyFREE(ptr, size)	
# ifdef __cplusplus
extern "C" {
# include <stdio.h>
# include "yyTree.w"
# include "System.h"
# include "General.h"
# include "Memory.h"
# include "DynArray.h"
# include "StringMem.h"
# include "Idents.h"
# include "Sets.h"
# include "Positions.h"
}
# else
# include <stdio.h>
# include "yyTree.w"
# include "System.h"
# include "General.h"
# include "Memory.h"
# include "DynArray.h"
# include "StringMem.h"
# include "Idents.h"
# include "Sets.h"
# include "Positions.h"
# endif

/* line 63 "" */
 
  int ExtnModes = 0;

  #undef  writetIdent
  #define writetIdent(a)  { if (a != NoIdent && a != 0) WriteIdent(yyf,a); else fputs("NoIdent",yyf); }

  #undef  writetSymbol
  #define writetSymbol(a) { if (Symbol(a).Id != NoIdent && Symbol(a).Id != 0) WriteIdent(yyf,Symbol(a).Id); else fputs("NoSymbol",yyf); }

  #undef  writetStringRef
  #define writetStringRef(a) { WriteStrRef(a); }

  #undef  writechar
  #define writechar(a)       { fprintf(yyf,"\'*#%02x\'",a);}

  #undef writetType  
  #define writetType(a)   { if (a) { /* fputs(Tree_NodeName[a->Kind],yyf); */ CodeType(a);}}
  #undef begintType
  #define begintType(a)   { a = NoTree; }

  #undef beginbool
  #define beginbool(a) { a = 0; }
  
  #undef writetChain
  #define writetChain(a)       { if (a) fputs(Tree_NodeName[a->Kind],yyf); }

  #undef writetFrame
  #define writetFrame(a)       { if (a) fputs(Tree_NodeName[a->Kind],yyf); }

  #undef writetChoiceList
  #define writetChoiceList(a)  { if (a) fputs(Tree_NodeName[a->Kind],yyf); }

  #undef writetIfType
  #define writetIfType(a)      { if (a) fputs(Tree_NodeName[a->Kind],yyf); }

/* line 689 "" */

  /*{{{  tUsage UpdateUsage(tUsage old, tUsage new)*/
  tUsage UpdateUsage(tUsage old, tUsage new)
  {
    if (old==xxInput && new==xxOutput) return xxInOut;
    if (new==xxInput && old==xxOutput) return xxInOut;
    if (old==xxInput || old==xxOutput) return old;
    return(new);  
  }
  /*}}}*/

/* line 1028 "" */

  #undef writepLinExpr
  #define writepLinExpr(a) WriteLinExpr(yyf,a);


static void yyExit () { Exit (1); }

void (* Tree_Exit) () = yyExit;

# define yyBlockSize 20480

typedef struct yysBlock {
 char yyBlock [yyBlockSize];
 struct yysBlock * yySuccessor;
} yytBlock, * yytBlockPtr;

tTree TreeRoot;
unsigned long Tree_HeapUsed = 0;

static yytBlockPtr yyBlockList	= (yytBlockPtr) NoTree;
char * Tree_PoolFreePtr	= (char *) NoTree;
char * Tree_PoolMaxPtr	= (char *) NoTree;
static unsigned short yyMaxSize	= 0;
unsigned short Tree_NodeSize [222 + 1] = { 0,
 sizeof (yStart),
 sizeof (ySpecList),
 sizeof (yNoSpec),
 sizeof (yASpec),
 sizeof (ySpec),
 sizeof (yDeclType),
 sizeof (yDecl),
 sizeof (yIs),
 sizeof (yAVal),
 sizeof (yVal),
 sizeof (yValRetypes),
 sizeof (yValReshapes),
 sizeof (yRetypes),
 sizeof (yReshapes),
 sizeof (yDefAPro),
 sizeof (yDefSeqPro),
 sizeof (yDefTagPro),
 sizeof (yDefAProc),
 sizeof (yDefProc),
 sizeof (yInlineDefProc),
 sizeof (yPrototypeProc),
 sizeof (yDefAFunc),
 sizeof (yDefFunc),
 sizeof (yInlineDefFunc),
 sizeof (yDefIsFunc),
 sizeof (yInlineDefIsFunc),
 sizeof (yPrototypeFunc),
 sizeof (yNullSpec),
 sizeof (yHCode),
 sizeof (yCallFrame),
 sizeof (yParFrame),
 sizeof (yFormals),
 sizeof (yNoFormal),
 sizeof (yNoFormals),
 sizeof (yAFormal),
 sizeof (yReplicator),
 sizeof (yTagProList),
 sizeof (yNoTagPro),
 sizeof (yATagPro),
 sizeof (yBy),
 sizeof (yByVal),
 sizeof (yByRef),
 sizeof (yByUndef),
 sizeof (yIdList),
 sizeof (yNoId),
 sizeof (yAnId),
 sizeof (yType),
 sizeof (yArray),
 sizeof (yUserArray),
 sizeof (yChan),
 sizeof (yTimer),
 sizeof (yUntyped),
 sizeof (yUntypedByte),
 sizeof (yUntypedReal),
 sizeof (yUntypedInteger),
 sizeof (yUntypedInt),
 sizeof (yUntypedHex),
 sizeof (yPrimitive),
 sizeof (yNumeric),
 sizeof (yInteger),
 sizeof (yInt),
 sizeof (yUserInt),
 sizeof (yInt16),
 sizeof (yUserInt16),
 sizeof (yInt32),
 sizeof (yUserInt32),
 sizeof (yInt64),
 sizeof (yUserInt64),
 sizeof (yReal),
 sizeof (yReal32),
 sizeof (yUserReal32),
 sizeof (yReal64),
 sizeof (yUserReal64),
 sizeof (yByte),
 sizeof (yUserByte),
 sizeof (yBool),
 sizeof (yUserBool),
 sizeof (yUserStructTypes),
 sizeof (yUserDataTypes),
 sizeof (yUserRecord),
 sizeof (yUserPackedRecord),
 sizeof (yUserUnion),
 sizeof (yUserChanRecord),
 sizeof (yTypeList),
 sizeof (yNoType),
 sizeof (yAType),
 sizeof (yFieldList),
 sizeof (yNoField),
 sizeof (yAField),
 sizeof (yProtocol),
 sizeof (yAny),
 sizeof (yNamed),
 sizeof (ySimple),
 sizeof (yCounted),
 sizeof (ySeqProList),
 sizeof (yNoSeqPro),
 sizeof (yASeqPro),
 sizeof (yExpr),
 sizeof (yBinary),
 sizeof (yUnary),
 sizeof (yConst),
 sizeof (yValof),
 sizeof (yFuncCall),
 sizeof (yElementalExpr),
 sizeof (yDefault),
 sizeof (yConvert),
 sizeof (yRound),
 sizeof (yTrunc),
 sizeof (yMostPos),
 sizeof (yMostNeg),
 sizeof (yCountArray),
 sizeof (yUnBound),
 sizeof (yBytesIn),
 sizeof (yBytesInType),
 sizeof (yBytesInExpr),
 sizeof (ySizeOf),
 sizeof (yOffsetOf),
 sizeof (yStringElement),
 sizeof (yString),
 sizeof (yStringSubscript),
 sizeof (yStringRange),
 sizeof (yElemental),
 sizeof (yTableElement),
 sizeof (yTable),
 sizeof (yUserLiteral),
 sizeof (yTableExpr),
 sizeof (yTableSubscript),
 sizeof (yTableRange),
 sizeof (yElement),
 sizeof (yIdentifier),
 sizeof (yUnmappedIdentifier),
 sizeof (yElementAccess),
 sizeof (ySubscript),
 sizeof (yRange),
 sizeof (yExprList),
 sizeof (yNoExpr),
 sizeof (yAnExpr),
 sizeof (yOp),
 sizeof (yRelOp),
 sizeof (yLess),
 sizeof (yGreater),
 sizeof (yLessEqual),
 sizeof (yGreaterEqual),
 sizeof (yEqual),
 sizeof (yNotEqual),
 sizeof (yMathsOp),
 sizeof (yAdd),
 sizeof (ySubtract),
 sizeof (yMultiply),
 sizeof (yDivide),
 sizeof (yRem),
 sizeof (yBitOp),
 sizeof (yBitAnd),
 sizeof (yBitOr),
 sizeof (yBitEor),
 sizeof (yShiftOp),
 sizeof (yLeftShift),
 sizeof (yRightShift),
 sizeof (yUniOp),
 sizeof (ySize),
 sizeof (yBitNot),
 sizeof (yNot),
 sizeof (yClockOp),
 sizeof (yPlus),
 sizeof (yMinus),
 sizeof (yTimes),
 sizeof (yLogOp),
 sizeof (yAnd),
 sizeof (yOr),
 sizeof (yAfterOp),
 sizeof (yProcess),
 sizeof (yAssign),
 sizeof (yAltType),
 sizeof (yAlt),
 sizeof (yAltRep),
 sizeof (yParType),
 sizeof (yPar),
 sizeof (yParRep),
 sizeof (yProcessor),
 sizeof (yIfType),
 sizeof (yIf),
 sizeof (yIfRep),
 sizeof (yInput),
 sizeof (yOutput),
 sizeof (yCaseIn),
 sizeof (yAfter),
 sizeof (ySeqRep),
 sizeof (ySeq),
 sizeof (yCase),
 sizeof (yWhile),
 sizeof (ySkip),
 sizeof (yStop),
 sizeof (yInstance),
 sizeof (yCCode),
 sizeof (yNullProcess),
 sizeof (yAltList),
 sizeof (yNoAlt),
 sizeof (yAnAlts),
 sizeof (yAnAlt),
 sizeof (yANewAlt),
 sizeof (yGuard),
 sizeof (yAltInput),
 sizeof (yAltTimer),
 sizeof (yAltCaseIn),
 sizeof (yNoInput),
 sizeof (yChoiceList),
 sizeof (yNoChoice),
 sizeof (yAChoices),
 sizeof (yAChoice),
 sizeof (yANewIf),
 sizeof (yProcList),
 sizeof (yNoProcess),
 sizeof (yAProcess),
 sizeof (yCaseList),
 sizeof (yNoCase),
 sizeof (yACase),
 sizeof (yAccessList),
 sizeof (yAnAccess),
 sizeof (yNoAccess),
 sizeof (yElementList),
 sizeof (yAnElement),
 sizeof (yNoElement),
};
char * Tree_NodeName [222 + 1] = {
 "NoTree",
 "Start",
 "SpecList",
 "NoSpec",
 "ASpec",
 "Spec",
 "DeclType",
 "Decl",
 "Is",
 "AVal",
 "Val",
 "ValRetypes",
 "ValReshapes",
 "Retypes",
 "Reshapes",
 "DefAPro",
 "DefSeqPro",
 "DefTagPro",
 "DefAProc",
 "DefProc",
 "InlineDefProc",
 "PrototypeProc",
 "DefAFunc",
 "DefFunc",
 "InlineDefFunc",
 "DefIsFunc",
 "InlineDefIsFunc",
 "PrototypeFunc",
 "NullSpec",
 "HCode",
 "CallFrame",
 "ParFrame",
 "Formals",
 "NoFormal",
 "NoFormals",
 "AFormal",
 "Replicator",
 "TagProList",
 "NoTagPro",
 "ATagPro",
 "By",
 "ByVal",
 "ByRef",
 "ByUndef",
 "IdList",
 "NoId",
 "AnId",
 "Type",
 "Array",
 "UserArray",
 "Chan",
 "Timer",
 "Untyped",
 "UntypedByte",
 "UntypedReal",
 "UntypedInteger",
 "UntypedInt",
 "UntypedHex",
 "Primitive",
 "Numeric",
 "Integer",
 "Int",
 "UserInt",
 "Int16",
 "UserInt16",
 "Int32",
 "UserInt32",
 "Int64",
 "UserInt64",
 "Real",
 "Real32",
 "UserReal32",
 "Real64",
 "UserReal64",
 "Byte",
 "UserByte",
 "Bool",
 "UserBool",
 "UserStructTypes",
 "UserDataTypes",
 "UserRecord",
 "UserPackedRecord",
 "UserUnion",
 "UserChanRecord",
 "TypeList",
 "NoType",
 "AType",
 "FieldList",
 "NoField",
 "AField",
 "Protocol",
 "Any",
 "Named",
 "Simple",
 "Counted",
 "SeqProList",
 "NoSeqPro",
 "ASeqPro",
 "Expr",
 "Binary",
 "Unary",
 "Const",
 "Valof",
 "FuncCall",
 "ElementalExpr",
 "Default",
 "Convert",
 "Round",
 "Trunc",
 "MostPos",
 "MostNeg",
 "CountArray",
 "UnBound",
 "BytesIn",
 "BytesInType",
 "BytesInExpr",
 "SizeOf",
 "OffsetOf",
 "StringElement",
 "String",
 "StringSubscript",
 "StringRange",
 "Elemental",
 "TableElement",
 "Table",
 "UserLiteral",
 "TableExpr",
 "TableSubscript",
 "TableRange",
 "Element",
 "Identifier",
 "UnmappedIdentifier",
 "ElementAccess",
 "Subscript",
 "Range",
 "ExprList",
 "NoExpr",
 "AnExpr",
 "Op",
 "RelOp",
 "Less",
 "Greater",
 "LessEqual",
 "GreaterEqual",
 "Equal",
 "NotEqual",
 "MathsOp",
 "Add",
 "Subtract",
 "Multiply",
 "Divide",
 "Rem",
 "BitOp",
 "BitAnd",
 "BitOr",
 "BitEor",
 "ShiftOp",
 "LeftShift",
 "RightShift",
 "UniOp",
 "Size",
 "BitNot",
 "Not",
 "ClockOp",
 "Plus",
 "Minus",
 "Times",
 "LogOp",
 "And",
 "Or",
 "AfterOp",
 "Process",
 "Assign",
 "AltType",
 "Alt",
 "AltRep",
 "ParType",
 "Par",
 "ParRep",
 "Processor",
 "IfType",
 "If",
 "IfRep",
 "Input",
 "Output",
 "CaseIn",
 "After",
 "SeqRep",
 "Seq",
 "Case",
 "While",
 "Skip",
 "Stop",
 "Instance",
 "CCode",
 "NullProcess",
 "AltList",
 "NoAlt",
 "AnAlts",
 "AnAlt",
 "ANewAlt",
 "Guard",
 "AltInput",
 "AltTimer",
 "AltCaseIn",
 "NoInput",
 "ChoiceList",
 "NoChoice",
 "AChoices",
 "AChoice",
 "ANewIf",
 "ProcList",
 "NoProcess",
 "AProcess",
 "CaseList",
 "NoCase",
 "ACase",
 "AccessList",
 "AnAccess",
 "NoAccess",
 "ElementList",
 "AnElement",
 "NoElement",
};
static Tree_tKind yyTypeRange [222 + 1] = { 0,
 kStart,
 kASpec,
 kNoSpec,
 kASpec,
 kParFrame,
 kDeclType,
 kDecl,
 kIs,
 kValReshapes,
 kVal,
 kValReshapes,
 kValReshapes,
 kReshapes,
 kReshapes,
 kDefTagPro,
 kDefSeqPro,
 kDefTagPro,
 kPrototypeProc,
 kInlineDefProc,
 kInlineDefProc,
 kPrototypeProc,
 kPrototypeFunc,
 kInlineDefFunc,
 kInlineDefFunc,
 kInlineDefIsFunc,
 kInlineDefIsFunc,
 kPrototypeFunc,
 kNullSpec,
 kHCode,
 kCallFrame,
 kParFrame,
 kAFormal,
 kNoFormals,
 kNoFormals,
 kAFormal,
 kReplicator,
 kATagPro,
 kNoTagPro,
 kATagPro,
 kByUndef,
 kByVal,
 kByRef,
 kByUndef,
 kAnId,
 kNoId,
 kAnId,
 kAType,
 kUserArray,
 kUserArray,
 kChan,
 kTimer,
 kUntypedHex,
 kUntypedByte,
 kUntypedReal,
 kUntypedHex,
 kUntypedInt,
 kUntypedHex,
 kUserBool,
 kUserReal64,
 kUserInt64,
 kUserInt,
 kUserInt,
 kUserInt16,
 kUserInt16,
 kUserInt32,
 kUserInt32,
 kUserInt64,
 kUserInt64,
 kUserReal64,
 kUserReal32,
 kUserReal32,
 kUserReal64,
 kUserReal64,
 kUserByte,
 kUserByte,
 kUserBool,
 kUserBool,
 kUserChanRecord,
 kUserUnion,
 kUserRecord,
 kUserPackedRecord,
 kUserUnion,
 kUserChanRecord,
 kAType,
 kNoType,
 kAType,
 kAField,
 kNoField,
 kAField,
 kCounted,
 kAny,
 kNamed,
 kSimple,
 kCounted,
 kASeqPro,
 kNoSeqPro,
 kASeqPro,
 kStringRange,
 kBinary,
 kUnary,
 kConst,
 kValof,
 kFuncCall,
 kElementalExpr,
 kDefault,
 kConvert,
 kRound,
 kTrunc,
 kMostPos,
 kMostNeg,
 kCountArray,
 kUnBound,
 kBytesInExpr,
 kBytesInType,
 kBytesInExpr,
 kSizeOf,
 kOffsetOf,
 kStringRange,
 kString,
 kStringSubscript,
 kStringRange,
 kRange,
 kTableRange,
 kUserLiteral,
 kUserLiteral,
 kTableExpr,
 kTableSubscript,
 kTableRange,
 kRange,
 kUnmappedIdentifier,
 kUnmappedIdentifier,
 kRange,
 kSubscript,
 kRange,
 kAnExpr,
 kNoExpr,
 kAnExpr,
 kAfterOp,
 kNotEqual,
 kLess,
 kGreater,
 kLessEqual,
 kGreaterEqual,
 kEqual,
 kNotEqual,
 kRem,
 kAdd,
 kSubtract,
 kMultiply,
 kDivide,
 kRem,
 kBitEor,
 kBitAnd,
 kBitOr,
 kBitEor,
 kRightShift,
 kLeftShift,
 kRightShift,
 kNot,
 kSize,
 kBitNot,
 kNot,
 kTimes,
 kPlus,
 kMinus,
 kTimes,
 kOr,
 kAnd,
 kOr,
 kAfterOp,
 kNullProcess,
 kAssign,
 kAltRep,
 kAlt,
 kAltRep,
 kParRep,
 kPar,
 kParRep,
 kProcessor,
 kIfRep,
 kIf,
 kIfRep,
 kInput,
 kOutput,
 kCaseIn,
 kAfter,
 kSeqRep,
 kSeq,
 kCase,
 kWhile,
 kSkip,
 kStop,
 kInstance,
 kCCode,
 kNullProcess,
 kANewAlt,
 kNoAlt,
 kANewAlt,
 kAnAlt,
 kANewAlt,
 kNoInput,
 kAltInput,
 kAltTimer,
 kAltCaseIn,
 kNoInput,
 kANewIf,
 kNoChoice,
 kANewIf,
 kAChoice,
 kANewIf,
 kAProcess,
 kNoProcess,
 kAProcess,
 kACase,
 kNoCase,
 kACase,
 kNoAccess,
 kAnAccess,
 kNoAccess,
 kNoElement,
 kAnElement,
 kNoElement,
};

tTree Tree_Alloc ()
{
 register yytBlockPtr yyBlockPtr = yyBlockList;
 register int i;

 if (yyMaxSize == 0)
  for (i = 1; i <= 222; i ++) {
   Tree_NodeSize [i] = (Tree_NodeSize [i] + yyMaxAlign - 1) & yyAlignMasks [yyMaxAlign];
   yyMaxSize = Max (Tree_NodeSize [i], yyMaxSize);
  }
 yyBlockList = (yytBlockPtr) Alloc (sizeof (yytBlock));
 yyBlockList->yySuccessor = yyBlockPtr;
 Tree_PoolFreePtr = yyBlockList->yyBlock;
 Tree_PoolMaxPtr = Tree_PoolFreePtr + yyBlockSize - yyMaxSize + 1;
 Tree_HeapUsed += yyBlockSize;
 return (tTree) Tree_PoolFreePtr;
}

tTree MakeTree
# if defined __STDC__ | defined __cplusplus
 (Tree_tKind yyKind)
# else
 (yyKind) Tree_tKind yyKind;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [yyKind])
 yyt->Kind = yyKind;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

bool Tree_IsType
# if defined __STDC__ | defined __cplusplus
 (register tTree yyt, register Tree_tKind yyKind)
# else
 (yyt, yyKind) register tTree yyt; register Tree_tKind yyKind;
# endif
{
 return yyt != NoTree && yyKind <= yyt->Kind && yyt->Kind <= yyTypeRange [yyKind];
}


tTree mStart
# if defined __STDC__ | defined __cplusplus
(tTree pProcess)
# else
(pProcess)
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kStart])
 yyt->Kind = kStart;
 yyt->yyHead.yyMark = 0;
 yyt->Start.Process = pProcess;
 begintChain(yyt->Start.CGNext)
 begintSymbol(yyt->Start.OuterProc)
 return yyt;
}

tTree mSpecList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSpecList])
 yyt->Kind = kSpecList;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->SpecList.V1In)
 beginint(yyt->SpecList.V1Out)
 beginint(yyt->SpecList.VS)
 beginint(yyt->SpecList.VIn)
 beginint(yyt->SpecList.VOut)
 beginint(yyt->SpecList.DCL)
 begintSymbol(yyt->SpecList.Parent)
 beginbool(yyt->SpecList.AtomicSpec)
 begintParUsage(yyt->SpecList.PUIn)
 begintParUsage(yyt->SpecList.PUOut)
 return yyt;
}

tTree mNoSpec
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoSpec])
 yyt->Kind = kNoSpec;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoSpec.V1In)
 beginint(yyt->NoSpec.V1Out)
 beginint(yyt->NoSpec.VS)
 beginint(yyt->NoSpec.VIn)
 beginint(yyt->NoSpec.VOut)
 beginint(yyt->NoSpec.DCL)
 begintSymbol(yyt->NoSpec.Parent)
 beginbool(yyt->NoSpec.AtomicSpec)
 begintParUsage(yyt->NoSpec.PUIn)
 begintParUsage(yyt->NoSpec.PUOut)
 return yyt;
}

tTree mASpec
# if defined __STDC__ | defined __cplusplus
(tTree pSpec, tTree pNext)
# else
(pSpec, pNext)
tTree pSpec;
tTree pNext;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kASpec])
 yyt->Kind = kASpec;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->ASpec.V1In)
 beginint(yyt->ASpec.V1Out)
 beginint(yyt->ASpec.VS)
 beginint(yyt->ASpec.VIn)
 beginint(yyt->ASpec.VOut)
 beginint(yyt->ASpec.DCL)
 begintSymbol(yyt->ASpec.Parent)
 beginbool(yyt->ASpec.AtomicSpec)
 begintParUsage(yyt->ASpec.PUIn)
 begintParUsage(yyt->ASpec.PUOut)
 yyt->ASpec.Spec = pSpec;
 yyt->ASpec.Next = pNext;
 return yyt;
}

tTree mSpec
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSpec])
 yyt->Kind = kSpec;
 yyt->yyHead.yyMark = 0;
 yyt->Spec.Pos = pPos;
 beginint(yyt->Spec.V1In)
 beginint(yyt->Spec.V1Out)
 beginint(yyt->Spec.VS)
 beginint(yyt->Spec.VIn)
 beginint(yyt->Spec.VOut)
 begintSymbol(yyt->Spec.Parent)
 beginbool(yyt->Spec.AtomicSpec)
 begintParUsage(yyt->Spec.PUIn)
 begintParUsage(yyt->Spec.PUOut)
 return yyt;
}

tTree mDeclType
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDeclType])
 yyt->Kind = kDeclType;
 yyt->yyHead.yyMark = 0;
 yyt->DeclType.Pos = pPos;
 beginint(yyt->DeclType.V1In)
 beginint(yyt->DeclType.V1Out)
 beginint(yyt->DeclType.VS)
 beginint(yyt->DeclType.VIn)
 beginint(yyt->DeclType.VOut)
 begintSymbol(yyt->DeclType.Parent)
 beginbool(yyt->DeclType.AtomicSpec)
 begintParUsage(yyt->DeclType.PUIn)
 begintParUsage(yyt->DeclType.PUOut)
 yyt->DeclType.Type = pType;
 return yyt;
}

tTree mDecl
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tTree pIdList)
# else
(pPos, pType, pIdList)
tPosition pPos;
tTree pType;
tTree pIdList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDecl])
 yyt->Kind = kDecl;
 yyt->yyHead.yyMark = 0;
 yyt->Decl.Pos = pPos;
 beginint(yyt->Decl.V1In)
 beginint(yyt->Decl.V1Out)
 beginint(yyt->Decl.VS)
 beginint(yyt->Decl.VIn)
 beginint(yyt->Decl.VOut)
 begintSymbol(yyt->Decl.Parent)
 beginbool(yyt->Decl.AtomicSpec)
 begintParUsage(yyt->Decl.PUIn)
 begintParUsage(yyt->Decl.PUOut)
 yyt->Decl.Type = pType;
 yyt->Decl.IdList = pIdList;
 return yyt;
}

tTree mIs
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental)
# else
(pPos, pType, pSym, pElemental)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pElemental;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIs])
 yyt->Kind = kIs;
 yyt->yyHead.yyMark = 0;
 yyt->Is.Pos = pPos;
 beginint(yyt->Is.V1In)
 beginint(yyt->Is.V1Out)
 beginint(yyt->Is.VS)
 beginint(yyt->Is.VIn)
 beginint(yyt->Is.VOut)
 begintSymbol(yyt->Is.Parent)
 beginbool(yyt->Is.AtomicSpec)
 begintParUsage(yyt->Is.PUIn)
 begintParUsage(yyt->Is.PUOut)
 yyt->Is.Type = pType;
 yyt->Is.Sym = pSym;
 yyt->Is.Elemental = pElemental;
 begintUsage(yyt->Is.U)
 return yyt;
}

tTree mAVal
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr)
# else
(pPos, pType, pSym, pExpr)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAVal])
 yyt->Kind = kAVal;
 yyt->yyHead.yyMark = 0;
 yyt->AVal.Pos = pPos;
 beginint(yyt->AVal.V1In)
 beginint(yyt->AVal.V1Out)
 beginint(yyt->AVal.VS)
 beginint(yyt->AVal.VIn)
 beginint(yyt->AVal.VOut)
 begintSymbol(yyt->AVal.Parent)
 beginbool(yyt->AVal.AtomicSpec)
 begintParUsage(yyt->AVal.PUIn)
 begintParUsage(yyt->AVal.PUOut)
 yyt->AVal.Type = pType;
 yyt->AVal.Sym = pSym;
 yyt->AVal.Expr = pExpr;
 return yyt;
}

tTree mVal
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr)
# else
(pPos, pType, pSym, pExpr)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kVal])
 yyt->Kind = kVal;
 yyt->yyHead.yyMark = 0;
 yyt->Val.Pos = pPos;
 beginint(yyt->Val.V1In)
 beginint(yyt->Val.V1Out)
 beginint(yyt->Val.VS)
 beginint(yyt->Val.VIn)
 beginint(yyt->Val.VOut)
 begintSymbol(yyt->Val.Parent)
 beginbool(yyt->Val.AtomicSpec)
 begintParUsage(yyt->Val.PUIn)
 begintParUsage(yyt->Val.PUOut)
 yyt->Val.Type = pType;
 yyt->Val.Sym = pSym;
 yyt->Val.Expr = pExpr;
 return yyt;
}

tTree mValRetypes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr)
# else
(pPos, pType, pSym, pExpr)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kValRetypes])
 yyt->Kind = kValRetypes;
 yyt->yyHead.yyMark = 0;
 yyt->ValRetypes.Pos = pPos;
 beginint(yyt->ValRetypes.V1In)
 beginint(yyt->ValRetypes.V1Out)
 beginint(yyt->ValRetypes.VS)
 beginint(yyt->ValRetypes.VIn)
 beginint(yyt->ValRetypes.VOut)
 begintSymbol(yyt->ValRetypes.Parent)
 beginbool(yyt->ValRetypes.AtomicSpec)
 begintParUsage(yyt->ValRetypes.PUIn)
 begintParUsage(yyt->ValRetypes.PUOut)
 yyt->ValRetypes.Type = pType;
 yyt->ValRetypes.Sym = pSym;
 yyt->ValRetypes.Expr = pExpr;
 return yyt;
}

tTree mValReshapes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr)
# else
(pPos, pType, pSym, pExpr)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kValReshapes])
 yyt->Kind = kValReshapes;
 yyt->yyHead.yyMark = 0;
 yyt->ValReshapes.Pos = pPos;
 beginint(yyt->ValReshapes.V1In)
 beginint(yyt->ValReshapes.V1Out)
 beginint(yyt->ValReshapes.VS)
 beginint(yyt->ValReshapes.VIn)
 beginint(yyt->ValReshapes.VOut)
 begintSymbol(yyt->ValReshapes.Parent)
 beginbool(yyt->ValReshapes.AtomicSpec)
 begintParUsage(yyt->ValReshapes.PUIn)
 begintParUsage(yyt->ValReshapes.PUOut)
 yyt->ValReshapes.Type = pType;
 yyt->ValReshapes.Sym = pSym;
 yyt->ValReshapes.Expr = pExpr;
 return yyt;
}

tTree mRetypes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental)
# else
(pPos, pType, pSym, pElemental)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pElemental;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRetypes])
 yyt->Kind = kRetypes;
 yyt->yyHead.yyMark = 0;
 yyt->Retypes.Pos = pPos;
 beginint(yyt->Retypes.V1In)
 beginint(yyt->Retypes.V1Out)
 beginint(yyt->Retypes.VS)
 beginint(yyt->Retypes.VIn)
 beginint(yyt->Retypes.VOut)
 begintSymbol(yyt->Retypes.Parent)
 beginbool(yyt->Retypes.AtomicSpec)
 begintParUsage(yyt->Retypes.PUIn)
 begintParUsage(yyt->Retypes.PUOut)
 yyt->Retypes.Type = pType;
 yyt->Retypes.Sym = pSym;
 yyt->Retypes.Elemental = pElemental;
 return yyt;
}

tTree mReshapes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental)
# else
(pPos, pType, pSym, pElemental)
tPosition pPos;
tTree pType;
tSymbol pSym;
tTree pElemental;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kReshapes])
 yyt->Kind = kReshapes;
 yyt->yyHead.yyMark = 0;
 yyt->Reshapes.Pos = pPos;
 beginint(yyt->Reshapes.V1In)
 beginint(yyt->Reshapes.V1Out)
 beginint(yyt->Reshapes.VS)
 beginint(yyt->Reshapes.VIn)
 beginint(yyt->Reshapes.VOut)
 begintSymbol(yyt->Reshapes.Parent)
 beginbool(yyt->Reshapes.AtomicSpec)
 begintParUsage(yyt->Reshapes.PUIn)
 begintParUsage(yyt->Reshapes.PUOut)
 yyt->Reshapes.Type = pType;
 yyt->Reshapes.Sym = pSym;
 yyt->Reshapes.Elemental = pElemental;
 return yyt;
}

tTree mDefAPro
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefAPro])
 yyt->Kind = kDefAPro;
 yyt->yyHead.yyMark = 0;
 yyt->DefAPro.Pos = pPos;
 beginint(yyt->DefAPro.V1In)
 beginint(yyt->DefAPro.V1Out)
 beginint(yyt->DefAPro.VS)
 beginint(yyt->DefAPro.VIn)
 beginint(yyt->DefAPro.VOut)
 begintSymbol(yyt->DefAPro.Parent)
 beginbool(yyt->DefAPro.AtomicSpec)
 begintParUsage(yyt->DefAPro.PUIn)
 begintParUsage(yyt->DefAPro.PUOut)
 return yyt;
}

tTree mDefSeqPro
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pSeqProList)
# else
(pPos, pSym, pSeqProList)
tPosition pPos;
tSymbol pSym;
tTree pSeqProList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefSeqPro])
 yyt->Kind = kDefSeqPro;
 yyt->yyHead.yyMark = 0;
 yyt->DefSeqPro.Pos = pPos;
 beginint(yyt->DefSeqPro.V1In)
 beginint(yyt->DefSeqPro.V1Out)
 beginint(yyt->DefSeqPro.VS)
 beginint(yyt->DefSeqPro.VIn)
 beginint(yyt->DefSeqPro.VOut)
 begintSymbol(yyt->DefSeqPro.Parent)
 beginbool(yyt->DefSeqPro.AtomicSpec)
 begintParUsage(yyt->DefSeqPro.PUIn)
 begintParUsage(yyt->DefSeqPro.PUOut)
 yyt->DefSeqPro.Sym = pSym;
 yyt->DefSeqPro.SeqProList = pSeqProList;
 return yyt;
}

tTree mDefTagPro
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTagProList)
# else
(pPos, pSym, pTagProList)
tPosition pPos;
tSymbol pSym;
tTree pTagProList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefTagPro])
 yyt->Kind = kDefTagPro;
 yyt->yyHead.yyMark = 0;
 yyt->DefTagPro.Pos = pPos;
 beginint(yyt->DefTagPro.V1In)
 beginint(yyt->DefTagPro.V1Out)
 beginint(yyt->DefTagPro.VS)
 beginint(yyt->DefTagPro.VIn)
 beginint(yyt->DefTagPro.VOut)
 begintSymbol(yyt->DefTagPro.Parent)
 beginbool(yyt->DefTagPro.AtomicSpec)
 begintParUsage(yyt->DefTagPro.PUIn)
 begintParUsage(yyt->DefTagPro.PUOut)
 yyt->DefTagPro.Sym = pSym;
 yyt->DefTagPro.TagProList = pTagProList;
 return yyt;
}

tTree mDefAProc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFormals)
# else
(pPos, pSym, pFormals)
tPosition pPos;
tSymbol pSym;
tTree pFormals;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefAProc])
 yyt->Kind = kDefAProc;
 yyt->yyHead.yyMark = 0;
 yyt->DefAProc.Pos = pPos;
 beginint(yyt->DefAProc.V1In)
 beginint(yyt->DefAProc.V1Out)
 beginint(yyt->DefAProc.VS)
 beginint(yyt->DefAProc.VIn)
 beginint(yyt->DefAProc.VOut)
 begintSymbol(yyt->DefAProc.Parent)
 beginbool(yyt->DefAProc.AtomicSpec)
 begintParUsage(yyt->DefAProc.PUIn)
 begintParUsage(yyt->DefAProc.PUOut)
 yyt->DefAProc.Sym = pSym;
 yyt->DefAProc.Formals = pFormals;
 begintUsage(yyt->DefAProc.U)
 begintChain(yyt->DefAProc.CGNext)
 return yyt;
}

tTree mDefProc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFormals, tTree pProcess)
# else
(pPos, pSym, pFormals, pProcess)
tPosition pPos;
tSymbol pSym;
tTree pFormals;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefProc])
 yyt->Kind = kDefProc;
 yyt->yyHead.yyMark = 0;
 yyt->DefProc.Pos = pPos;
 beginint(yyt->DefProc.V1In)
 beginint(yyt->DefProc.V1Out)
 beginint(yyt->DefProc.VS)
 beginint(yyt->DefProc.VIn)
 beginint(yyt->DefProc.VOut)
 begintSymbol(yyt->DefProc.Parent)
 beginbool(yyt->DefProc.AtomicSpec)
 begintParUsage(yyt->DefProc.PUIn)
 begintParUsage(yyt->DefProc.PUOut)
 yyt->DefProc.Sym = pSym;
 yyt->DefProc.Formals = pFormals;
 begintUsage(yyt->DefProc.U)
 begintChain(yyt->DefProc.CGNext)
 yyt->DefProc.Process = pProcess;
 beginbool(yyt->DefProc.ATOMIC)
 beginbool(yyt->DefProc.isPar)
 return yyt;
}

tTree mInlineDefProc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFormals, tTree pProcess)
# else
(pPos, pSym, pFormals, pProcess)
tPosition pPos;
tSymbol pSym;
tTree pFormals;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInlineDefProc])
 yyt->Kind = kInlineDefProc;
 yyt->yyHead.yyMark = 0;
 yyt->InlineDefProc.Pos = pPos;
 beginint(yyt->InlineDefProc.V1In)
 beginint(yyt->InlineDefProc.V1Out)
 beginint(yyt->InlineDefProc.VS)
 beginint(yyt->InlineDefProc.VIn)
 beginint(yyt->InlineDefProc.VOut)
 begintSymbol(yyt->InlineDefProc.Parent)
 beginbool(yyt->InlineDefProc.AtomicSpec)
 begintParUsage(yyt->InlineDefProc.PUIn)
 begintParUsage(yyt->InlineDefProc.PUOut)
 yyt->InlineDefProc.Sym = pSym;
 yyt->InlineDefProc.Formals = pFormals;
 begintUsage(yyt->InlineDefProc.U)
 begintChain(yyt->InlineDefProc.CGNext)
 yyt->InlineDefProc.Process = pProcess;
 beginbool(yyt->InlineDefProc.ATOMIC)
 beginbool(yyt->InlineDefProc.isPar)
 return yyt;
}

tTree mPrototypeProc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFormals, tIdent pModule, tLang pLang)
# else
(pPos, pSym, pFormals, pModule, pLang)
tPosition pPos;
tSymbol pSym;
tTree pFormals;
tIdent pModule;
tLang pLang;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kPrototypeProc])
 yyt->Kind = kPrototypeProc;
 yyt->yyHead.yyMark = 0;
 yyt->PrototypeProc.Pos = pPos;
 beginint(yyt->PrototypeProc.V1In)
 beginint(yyt->PrototypeProc.V1Out)
 beginint(yyt->PrototypeProc.VS)
 beginint(yyt->PrototypeProc.VIn)
 beginint(yyt->PrototypeProc.VOut)
 begintSymbol(yyt->PrototypeProc.Parent)
 beginbool(yyt->PrototypeProc.AtomicSpec)
 begintParUsage(yyt->PrototypeProc.PUIn)
 begintParUsage(yyt->PrototypeProc.PUOut)
 yyt->PrototypeProc.Sym = pSym;
 yyt->PrototypeProc.Formals = pFormals;
 begintUsage(yyt->PrototypeProc.U)
 begintChain(yyt->PrototypeProc.CGNext)
 yyt->PrototypeProc.Module = pModule;
 yyt->PrototypeProc.Lang = pLang;
 return yyt;
}

tTree mDefAFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals)
# else
(pPos, pSym, pTypeList, pFormals)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefAFunc])
 yyt->Kind = kDefAFunc;
 yyt->yyHead.yyMark = 0;
 yyt->DefAFunc.Pos = pPos;
 beginint(yyt->DefAFunc.V1In)
 beginint(yyt->DefAFunc.V1Out)
 beginint(yyt->DefAFunc.VS)
 beginint(yyt->DefAFunc.VIn)
 beginint(yyt->DefAFunc.VOut)
 begintSymbol(yyt->DefAFunc.Parent)
 beginbool(yyt->DefAFunc.AtomicSpec)
 begintParUsage(yyt->DefAFunc.PUIn)
 begintParUsage(yyt->DefAFunc.PUOut)
 yyt->DefAFunc.Sym = pSym;
 yyt->DefAFunc.TypeList = pTypeList;
 yyt->DefAFunc.Formals = pFormals;
 begintUsage(yyt->DefAFunc.U)
 begintChain(yyt->DefAFunc.CGNext)
 return yyt;
}

tTree mDefFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pValof)
# else
(pPos, pSym, pTypeList, pFormals, pValof)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
tTree pValof;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefFunc])
 yyt->Kind = kDefFunc;
 yyt->yyHead.yyMark = 0;
 yyt->DefFunc.Pos = pPos;
 beginint(yyt->DefFunc.V1In)
 beginint(yyt->DefFunc.V1Out)
 beginint(yyt->DefFunc.VS)
 beginint(yyt->DefFunc.VIn)
 beginint(yyt->DefFunc.VOut)
 begintSymbol(yyt->DefFunc.Parent)
 beginbool(yyt->DefFunc.AtomicSpec)
 begintParUsage(yyt->DefFunc.PUIn)
 begintParUsage(yyt->DefFunc.PUOut)
 yyt->DefFunc.Sym = pSym;
 yyt->DefFunc.TypeList = pTypeList;
 yyt->DefFunc.Formals = pFormals;
 begintUsage(yyt->DefFunc.U)
 begintChain(yyt->DefFunc.CGNext)
 yyt->DefFunc.Valof = pValof;
 beginbool(yyt->DefFunc.ATOMIC)
 return yyt;
}

tTree mInlineDefFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pValof)
# else
(pPos, pSym, pTypeList, pFormals, pValof)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
tTree pValof;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInlineDefFunc])
 yyt->Kind = kInlineDefFunc;
 yyt->yyHead.yyMark = 0;
 yyt->InlineDefFunc.Pos = pPos;
 beginint(yyt->InlineDefFunc.V1In)
 beginint(yyt->InlineDefFunc.V1Out)
 beginint(yyt->InlineDefFunc.VS)
 beginint(yyt->InlineDefFunc.VIn)
 beginint(yyt->InlineDefFunc.VOut)
 begintSymbol(yyt->InlineDefFunc.Parent)
 beginbool(yyt->InlineDefFunc.AtomicSpec)
 begintParUsage(yyt->InlineDefFunc.PUIn)
 begintParUsage(yyt->InlineDefFunc.PUOut)
 yyt->InlineDefFunc.Sym = pSym;
 yyt->InlineDefFunc.TypeList = pTypeList;
 yyt->InlineDefFunc.Formals = pFormals;
 begintUsage(yyt->InlineDefFunc.U)
 begintChain(yyt->InlineDefFunc.CGNext)
 yyt->InlineDefFunc.Valof = pValof;
 beginbool(yyt->InlineDefFunc.ATOMIC)
 return yyt;
}

tTree mDefIsFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pExprList)
# else
(pPos, pSym, pTypeList, pFormals, pExprList)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
tTree pExprList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefIsFunc])
 yyt->Kind = kDefIsFunc;
 yyt->yyHead.yyMark = 0;
 yyt->DefIsFunc.Pos = pPos;
 beginint(yyt->DefIsFunc.V1In)
 beginint(yyt->DefIsFunc.V1Out)
 beginint(yyt->DefIsFunc.VS)
 beginint(yyt->DefIsFunc.VIn)
 beginint(yyt->DefIsFunc.VOut)
 begintSymbol(yyt->DefIsFunc.Parent)
 beginbool(yyt->DefIsFunc.AtomicSpec)
 begintParUsage(yyt->DefIsFunc.PUIn)
 begintParUsage(yyt->DefIsFunc.PUOut)
 yyt->DefIsFunc.Sym = pSym;
 yyt->DefIsFunc.TypeList = pTypeList;
 yyt->DefIsFunc.Formals = pFormals;
 begintUsage(yyt->DefIsFunc.U)
 begintChain(yyt->DefIsFunc.CGNext)
 yyt->DefIsFunc.ExprList = pExprList;
 beginbool(yyt->DefIsFunc.ATOMIC)
 return yyt;
}

tTree mInlineDefIsFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pExprList)
# else
(pPos, pSym, pTypeList, pFormals, pExprList)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
tTree pExprList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInlineDefIsFunc])
 yyt->Kind = kInlineDefIsFunc;
 yyt->yyHead.yyMark = 0;
 yyt->InlineDefIsFunc.Pos = pPos;
 beginint(yyt->InlineDefIsFunc.V1In)
 beginint(yyt->InlineDefIsFunc.V1Out)
 beginint(yyt->InlineDefIsFunc.VS)
 beginint(yyt->InlineDefIsFunc.VIn)
 beginint(yyt->InlineDefIsFunc.VOut)
 begintSymbol(yyt->InlineDefIsFunc.Parent)
 beginbool(yyt->InlineDefIsFunc.AtomicSpec)
 begintParUsage(yyt->InlineDefIsFunc.PUIn)
 begintParUsage(yyt->InlineDefIsFunc.PUOut)
 yyt->InlineDefIsFunc.Sym = pSym;
 yyt->InlineDefIsFunc.TypeList = pTypeList;
 yyt->InlineDefIsFunc.Formals = pFormals;
 begintUsage(yyt->InlineDefIsFunc.U)
 begintChain(yyt->InlineDefIsFunc.CGNext)
 yyt->InlineDefIsFunc.ExprList = pExprList;
 beginbool(yyt->InlineDefIsFunc.ATOMIC)
 return yyt;
}

tTree mPrototypeFunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tIdent pModule, tLang pLang)
# else
(pPos, pSym, pTypeList, pFormals, pModule, pLang)
tPosition pPos;
tSymbol pSym;
tTree pTypeList;
tTree pFormals;
tIdent pModule;
tLang pLang;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kPrototypeFunc])
 yyt->Kind = kPrototypeFunc;
 yyt->yyHead.yyMark = 0;
 yyt->PrototypeFunc.Pos = pPos;
 beginint(yyt->PrototypeFunc.V1In)
 beginint(yyt->PrototypeFunc.V1Out)
 beginint(yyt->PrototypeFunc.VS)
 beginint(yyt->PrototypeFunc.VIn)
 beginint(yyt->PrototypeFunc.VOut)
 begintSymbol(yyt->PrototypeFunc.Parent)
 beginbool(yyt->PrototypeFunc.AtomicSpec)
 begintParUsage(yyt->PrototypeFunc.PUIn)
 begintParUsage(yyt->PrototypeFunc.PUOut)
 yyt->PrototypeFunc.Sym = pSym;
 yyt->PrototypeFunc.TypeList = pTypeList;
 yyt->PrototypeFunc.Formals = pFormals;
 begintUsage(yyt->PrototypeFunc.U)
 begintChain(yyt->PrototypeFunc.CGNext)
 yyt->PrototypeFunc.Module = pModule;
 yyt->PrototypeFunc.Lang = pLang;
 return yyt;
}

tTree mNullSpec
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNullSpec])
 yyt->Kind = kNullSpec;
 yyt->yyHead.yyMark = 0;
 yyt->NullSpec.Pos = pPos;
 beginint(yyt->NullSpec.V1In)
 beginint(yyt->NullSpec.V1Out)
 beginint(yyt->NullSpec.VS)
 beginint(yyt->NullSpec.VIn)
 beginint(yyt->NullSpec.VOut)
 begintSymbol(yyt->NullSpec.Parent)
 beginbool(yyt->NullSpec.AtomicSpec)
 begintParUsage(yyt->NullSpec.PUIn)
 begintParUsage(yyt->NullSpec.PUOut)
 return yyt;
}

tTree mHCode
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tStringRef pCode, tSymbol pScope)
# else
(pPos, pCode, pScope)
tPosition pPos;
tStringRef pCode;
tSymbol pScope;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kHCode])
 yyt->Kind = kHCode;
 yyt->yyHead.yyMark = 0;
 yyt->HCode.Pos = pPos;
 beginint(yyt->HCode.V1In)
 beginint(yyt->HCode.V1Out)
 beginint(yyt->HCode.VS)
 beginint(yyt->HCode.VIn)
 beginint(yyt->HCode.VOut)
 begintSymbol(yyt->HCode.Parent)
 beginbool(yyt->HCode.AtomicSpec)
 begintParUsage(yyt->HCode.PUIn)
 begintParUsage(yyt->HCode.PUOut)
 yyt->HCode.Code = pCode;
 yyt->HCode.Scope = pScope;
 begintChain(yyt->HCode.CGNext)
 return yyt;
}

tTree mCallFrame
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tIdent pId, tSymbol pSym)
# else
(pPos, pId, pSym)
tPosition pPos;
tIdent pId;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCallFrame])
 yyt->Kind = kCallFrame;
 yyt->yyHead.yyMark = 0;
 yyt->CallFrame.Pos = pPos;
 beginint(yyt->CallFrame.V1In)
 beginint(yyt->CallFrame.V1Out)
 beginint(yyt->CallFrame.VS)
 beginint(yyt->CallFrame.VIn)
 beginint(yyt->CallFrame.VOut)
 begintSymbol(yyt->CallFrame.Parent)
 beginbool(yyt->CallFrame.AtomicSpec)
 begintParUsage(yyt->CallFrame.PUIn)
 begintParUsage(yyt->CallFrame.PUOut)
 yyt->CallFrame.Id = pId;
 yyt->CallFrame.Sym = pSym;
 return yyt;
}

tTree mParFrame
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tIdent pTId, tIdent pFId, tSymbol pSym, int pRep)
# else
(pPos, pTId, pFId, pSym, pRep)
tPosition pPos;
tIdent pTId;
tIdent pFId;
tSymbol pSym;
int pRep;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kParFrame])
 yyt->Kind = kParFrame;
 yyt->yyHead.yyMark = 0;
 yyt->ParFrame.Pos = pPos;
 beginint(yyt->ParFrame.V1In)
 beginint(yyt->ParFrame.V1Out)
 beginint(yyt->ParFrame.VS)
 beginint(yyt->ParFrame.VIn)
 beginint(yyt->ParFrame.VOut)
 begintSymbol(yyt->ParFrame.Parent)
 beginbool(yyt->ParFrame.AtomicSpec)
 begintParUsage(yyt->ParFrame.PUIn)
 begintParUsage(yyt->ParFrame.PUOut)
 yyt->ParFrame.TId = pTId;
 yyt->ParFrame.FId = pFId;
 yyt->ParFrame.Sym = pSym;
 yyt->ParFrame.Rep = pRep;
 return yyt;
}

tTree mFormals
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kFormals])
 yyt->Kind = kFormals;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Formals.V1In)
 beginint(yyt->Formals.V1Out)
 beginint(yyt->Formals.VIn)
 beginint(yyt->Formals.VOut)
 beginbool(yyt->Formals.InPrototype)
 begintSymbol(yyt->Formals.Parent)
 beginbool(yyt->Formals.AtomicSpec)
 begintParUsage(yyt->Formals.PUIn)
 begintParUsage(yyt->Formals.PUOut)
 return yyt;
}

tTree mNoFormal
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoFormal])
 yyt->Kind = kNoFormal;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoFormal.V1In)
 beginint(yyt->NoFormal.V1Out)
 beginint(yyt->NoFormal.VIn)
 beginint(yyt->NoFormal.VOut)
 beginbool(yyt->NoFormal.InPrototype)
 begintSymbol(yyt->NoFormal.Parent)
 beginbool(yyt->NoFormal.AtomicSpec)
 begintParUsage(yyt->NoFormal.PUIn)
 begintParUsage(yyt->NoFormal.PUOut)
 return yyt;
}

tTree mNoFormals
# if defined __STDC__ | defined __cplusplus
(tSymbol pSym)
# else
(pSym)
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoFormals])
 yyt->Kind = kNoFormals;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoFormals.V1In)
 beginint(yyt->NoFormals.V1Out)
 beginint(yyt->NoFormals.VIn)
 beginint(yyt->NoFormals.VOut)
 beginbool(yyt->NoFormals.InPrototype)
 begintSymbol(yyt->NoFormals.Parent)
 beginbool(yyt->NoFormals.AtomicSpec)
 begintParUsage(yyt->NoFormals.PUIn)
 begintParUsage(yyt->NoFormals.PUOut)
 yyt->NoFormals.Sym = pSym;
 return yyt;
}

tTree mAFormal
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tPosition pPos, tTree pBy, tTree pType, tSymbol pSym)
# else
(pNext, pPos, pBy, pType, pSym)
tTree pNext;
tPosition pPos;
tTree pBy;
tTree pType;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAFormal])
 yyt->Kind = kAFormal;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AFormal.V1In)
 beginint(yyt->AFormal.V1Out)
 beginint(yyt->AFormal.VIn)
 beginint(yyt->AFormal.VOut)
 beginbool(yyt->AFormal.InPrototype)
 begintSymbol(yyt->AFormal.Parent)
 beginbool(yyt->AFormal.AtomicSpec)
 begintParUsage(yyt->AFormal.PUIn)
 begintParUsage(yyt->AFormal.PUOut)
 yyt->AFormal.Next = pNext;
 yyt->AFormal.Pos = pPos;
 yyt->AFormal.By = pBy;
 yyt->AFormal.Type = pType;
 yyt->AFormal.Sym = pSym;
 begintUsage(yyt->AFormal.U)
 return yyt;
}

tTree mReplicator
# if defined __STDC__ | defined __cplusplus
(tSymbol pSym, tTree pFrom, tTree pFor)
# else
(pSym, pFrom, pFor)
tSymbol pSym;
tTree pFrom;
tTree pFor;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kReplicator])
 yyt->Kind = kReplicator;
 yyt->yyHead.yyMark = 0;
 yyt->Replicator.Sym = pSym;
 yyt->Replicator.From = pFrom;
 yyt->Replicator.For = pFor;
 beginint(yyt->Replicator.V1In)
 beginint(yyt->Replicator.V1Out)
 beginint(yyt->Replicator.VIn)
 beginint(yyt->Replicator.VOut)
 begintSymbol(yyt->Replicator.Parent)
 beginbool(yyt->Replicator.AtomicSpec)
 begintParUsage(yyt->Replicator.PUIn)
 begintParUsage(yyt->Replicator.PUOut)
 beginbool(yyt->Replicator.SE)
 beginpLinExpr(yyt->Replicator.FromLE)
 beginpLinExpr(yyt->Replicator.ForLE)
 return yyt;
}

tTree mTagProList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTagProList])
 yyt->Kind = kTagProList;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->TagProList.PUIn)
 begintParUsage(yyt->TagProList.PUOut)
 return yyt;
}

tTree mNoTagPro
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoTagPro])
 yyt->Kind = kNoTagPro;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->NoTagPro.PUIn)
 begintParUsage(yyt->NoTagPro.PUOut)
 return yyt;
}

tTree mATagPro
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tSymbol pSym, tTree pSeqProList)
# else
(pNext, pSym, pSeqProList)
tTree pNext;
tSymbol pSym;
tTree pSeqProList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kATagPro])
 yyt->Kind = kATagPro;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->ATagPro.PUIn)
 begintParUsage(yyt->ATagPro.PUOut)
 yyt->ATagPro.Next = pNext;
 yyt->ATagPro.Sym = pSym;
 yyt->ATagPro.SeqProList = pSeqProList;
 return yyt;
}

tTree mBy
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBy])
 yyt->Kind = kBy;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mByVal
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kByVal])
 yyt->Kind = kByVal;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mByRef
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kByRef])
 yyt->Kind = kByRef;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mByUndef
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kByUndef])
 yyt->Kind = kByUndef;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mIdList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIdList])
 yyt->Kind = kIdList;
 yyt->yyHead.yyMark = 0;
 begintSymbol(yyt->IdList.Parent)
 beginbool(yyt->IdList.AtomicSpec)
 return yyt;
}

tTree mNoId
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoId])
 yyt->Kind = kNoId;
 yyt->yyHead.yyMark = 0;
 begintSymbol(yyt->NoId.Parent)
 beginbool(yyt->NoId.AtomicSpec)
 return yyt;
}

tTree mAnId
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tSymbol pSym)
# else
(pNext, pSym)
tTree pNext;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnId])
 yyt->Kind = kAnId;
 yyt->yyHead.yyMark = 0;
 begintSymbol(yyt->AnId.Parent)
 beginbool(yyt->AnId.AtomicSpec)
 yyt->AnId.Next = pNext;
 yyt->AnId.Sym = pSym;
 begintUsage(yyt->AnId.U)
 return yyt;
}

tTree mType
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kType])
 yyt->Kind = kType;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Type.V1In)
 beginint(yyt->Type.V1Out)
 beginint(yyt->Type.VIn)
 beginint(yyt->Type.VOut)
 beginint(yyt->Type.D)
 begintParUsage(yyt->Type.PUIn)
 begintParUsage(yyt->Type.PUOut)
 return yyt;
}

tTree mArray
# if defined __STDC__ | defined __cplusplus
(tTree pExpr, tTree pType)
# else
(pExpr, pType)
tTree pExpr;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kArray])
 yyt->Kind = kArray;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Array.V1In)
 beginint(yyt->Array.V1Out)
 beginint(yyt->Array.VIn)
 beginint(yyt->Array.VOut)
 beginint(yyt->Array.D)
 begintParUsage(yyt->Array.PUIn)
 begintParUsage(yyt->Array.PUOut)
 yyt->Array.Expr = pExpr;
 yyt->Array.Type = pType;
 beginbool(yyt->Array.SE)
 beginpLinExpr(yyt->Array.LE)
 return yyt;
}

tTree mUserArray
# if defined __STDC__ | defined __cplusplus
(tTree pExpr, tTree pType, tPosition pPos, tSymbol pSym)
# else
(pExpr, pType, pPos, pSym)
tTree pExpr;
tTree pType;
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserArray])
 yyt->Kind = kUserArray;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserArray.V1In)
 beginint(yyt->UserArray.V1Out)
 beginint(yyt->UserArray.VIn)
 beginint(yyt->UserArray.VOut)
 beginint(yyt->UserArray.D)
 begintParUsage(yyt->UserArray.PUIn)
 begintParUsage(yyt->UserArray.PUOut)
 yyt->UserArray.Expr = pExpr;
 yyt->UserArray.Type = pType;
 beginbool(yyt->UserArray.SE)
 beginpLinExpr(yyt->UserArray.LE)
 yyt->UserArray.Pos = pPos;
 yyt->UserArray.Sym = pSym;
 return yyt;
}

tTree mChan
# if defined __STDC__ | defined __cplusplus
(tTree pProtocol)
# else
(pProtocol)
tTree pProtocol;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kChan])
 yyt->Kind = kChan;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Chan.V1In)
 beginint(yyt->Chan.V1Out)
 beginint(yyt->Chan.VIn)
 beginint(yyt->Chan.VOut)
 beginint(yyt->Chan.D)
 begintParUsage(yyt->Chan.PUIn)
 begintParUsage(yyt->Chan.PUOut)
 yyt->Chan.Protocol = pProtocol;
 return yyt;
}

tTree mTimer
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTimer])
 yyt->Kind = kTimer;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Timer.V1In)
 beginint(yyt->Timer.V1Out)
 beginint(yyt->Timer.VIn)
 beginint(yyt->Timer.VOut)
 beginint(yyt->Timer.D)
 begintParUsage(yyt->Timer.PUIn)
 begintParUsage(yyt->Timer.PUOut)
 return yyt;
}

tTree mUntyped
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntyped])
 yyt->Kind = kUntyped;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Untyped.V1In)
 beginint(yyt->Untyped.V1Out)
 beginint(yyt->Untyped.VIn)
 beginint(yyt->Untyped.VOut)
 beginint(yyt->Untyped.D)
 begintParUsage(yyt->Untyped.PUIn)
 begintParUsage(yyt->Untyped.PUOut)
 return yyt;
}

tTree mUntypedByte
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntypedByte])
 yyt->Kind = kUntypedByte;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UntypedByte.V1In)
 beginint(yyt->UntypedByte.V1Out)
 beginint(yyt->UntypedByte.VIn)
 beginint(yyt->UntypedByte.VOut)
 beginint(yyt->UntypedByte.D)
 begintParUsage(yyt->UntypedByte.PUIn)
 begintParUsage(yyt->UntypedByte.PUOut)
 return yyt;
}

tTree mUntypedReal
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntypedReal])
 yyt->Kind = kUntypedReal;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UntypedReal.V1In)
 beginint(yyt->UntypedReal.V1Out)
 beginint(yyt->UntypedReal.VIn)
 beginint(yyt->UntypedReal.VOut)
 beginint(yyt->UntypedReal.D)
 begintParUsage(yyt->UntypedReal.PUIn)
 begintParUsage(yyt->UntypedReal.PUOut)
 return yyt;
}

tTree mUntypedInteger
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntypedInteger])
 yyt->Kind = kUntypedInteger;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UntypedInteger.V1In)
 beginint(yyt->UntypedInteger.V1Out)
 beginint(yyt->UntypedInteger.VIn)
 beginint(yyt->UntypedInteger.VOut)
 beginint(yyt->UntypedInteger.D)
 begintParUsage(yyt->UntypedInteger.PUIn)
 begintParUsage(yyt->UntypedInteger.PUOut)
 return yyt;
}

tTree mUntypedInt
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntypedInt])
 yyt->Kind = kUntypedInt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UntypedInt.V1In)
 beginint(yyt->UntypedInt.V1Out)
 beginint(yyt->UntypedInt.VIn)
 beginint(yyt->UntypedInt.VOut)
 beginint(yyt->UntypedInt.D)
 begintParUsage(yyt->UntypedInt.PUIn)
 begintParUsage(yyt->UntypedInt.PUOut)
 return yyt;
}

tTree mUntypedHex
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUntypedHex])
 yyt->Kind = kUntypedHex;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UntypedHex.V1In)
 beginint(yyt->UntypedHex.V1Out)
 beginint(yyt->UntypedHex.VIn)
 beginint(yyt->UntypedHex.VOut)
 beginint(yyt->UntypedHex.D)
 begintParUsage(yyt->UntypedHex.PUIn)
 begintParUsage(yyt->UntypedHex.PUOut)
 return yyt;
}

tTree mPrimitive
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kPrimitive])
 yyt->Kind = kPrimitive;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Primitive.V1In)
 beginint(yyt->Primitive.V1Out)
 beginint(yyt->Primitive.VIn)
 beginint(yyt->Primitive.VOut)
 beginint(yyt->Primitive.D)
 begintParUsage(yyt->Primitive.PUIn)
 begintParUsage(yyt->Primitive.PUOut)
 return yyt;
}

tTree mNumeric
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNumeric])
 yyt->Kind = kNumeric;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Numeric.V1In)
 beginint(yyt->Numeric.V1Out)
 beginint(yyt->Numeric.VIn)
 beginint(yyt->Numeric.VOut)
 beginint(yyt->Numeric.D)
 begintParUsage(yyt->Numeric.PUIn)
 begintParUsage(yyt->Numeric.PUOut)
 return yyt;
}

tTree mInteger
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInteger])
 yyt->Kind = kInteger;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Integer.V1In)
 beginint(yyt->Integer.V1Out)
 beginint(yyt->Integer.VIn)
 beginint(yyt->Integer.VOut)
 beginint(yyt->Integer.D)
 begintParUsage(yyt->Integer.PUIn)
 begintParUsage(yyt->Integer.PUOut)
 return yyt;
}

tTree mInt
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInt])
 yyt->Kind = kInt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Int.V1In)
 beginint(yyt->Int.V1Out)
 beginint(yyt->Int.VIn)
 beginint(yyt->Int.VOut)
 beginint(yyt->Int.D)
 begintParUsage(yyt->Int.PUIn)
 begintParUsage(yyt->Int.PUOut)
 return yyt;
}

tTree mUserInt
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserInt])
 yyt->Kind = kUserInt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserInt.V1In)
 beginint(yyt->UserInt.V1Out)
 beginint(yyt->UserInt.VIn)
 beginint(yyt->UserInt.VOut)
 beginint(yyt->UserInt.D)
 begintParUsage(yyt->UserInt.PUIn)
 begintParUsage(yyt->UserInt.PUOut)
 yyt->UserInt.Pos = pPos;
 yyt->UserInt.Sym = pSym;
 return yyt;
}

tTree mInt16
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInt16])
 yyt->Kind = kInt16;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Int16.V1In)
 beginint(yyt->Int16.V1Out)
 beginint(yyt->Int16.VIn)
 beginint(yyt->Int16.VOut)
 beginint(yyt->Int16.D)
 begintParUsage(yyt->Int16.PUIn)
 begintParUsage(yyt->Int16.PUOut)
 return yyt;
}

tTree mUserInt16
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserInt16])
 yyt->Kind = kUserInt16;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserInt16.V1In)
 beginint(yyt->UserInt16.V1Out)
 beginint(yyt->UserInt16.VIn)
 beginint(yyt->UserInt16.VOut)
 beginint(yyt->UserInt16.D)
 begintParUsage(yyt->UserInt16.PUIn)
 begintParUsage(yyt->UserInt16.PUOut)
 yyt->UserInt16.Pos = pPos;
 yyt->UserInt16.Sym = pSym;
 return yyt;
}

tTree mInt32
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInt32])
 yyt->Kind = kInt32;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Int32.V1In)
 beginint(yyt->Int32.V1Out)
 beginint(yyt->Int32.VIn)
 beginint(yyt->Int32.VOut)
 beginint(yyt->Int32.D)
 begintParUsage(yyt->Int32.PUIn)
 begintParUsage(yyt->Int32.PUOut)
 return yyt;
}

tTree mUserInt32
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserInt32])
 yyt->Kind = kUserInt32;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserInt32.V1In)
 beginint(yyt->UserInt32.V1Out)
 beginint(yyt->UserInt32.VIn)
 beginint(yyt->UserInt32.VOut)
 beginint(yyt->UserInt32.D)
 begintParUsage(yyt->UserInt32.PUIn)
 begintParUsage(yyt->UserInt32.PUOut)
 yyt->UserInt32.Pos = pPos;
 yyt->UserInt32.Sym = pSym;
 return yyt;
}

tTree mInt64
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInt64])
 yyt->Kind = kInt64;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Int64.V1In)
 beginint(yyt->Int64.V1Out)
 beginint(yyt->Int64.VIn)
 beginint(yyt->Int64.VOut)
 beginint(yyt->Int64.D)
 begintParUsage(yyt->Int64.PUIn)
 begintParUsage(yyt->Int64.PUOut)
 return yyt;
}

tTree mUserInt64
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserInt64])
 yyt->Kind = kUserInt64;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserInt64.V1In)
 beginint(yyt->UserInt64.V1Out)
 beginint(yyt->UserInt64.VIn)
 beginint(yyt->UserInt64.VOut)
 beginint(yyt->UserInt64.D)
 begintParUsage(yyt->UserInt64.PUIn)
 begintParUsage(yyt->UserInt64.PUOut)
 yyt->UserInt64.Pos = pPos;
 yyt->UserInt64.Sym = pSym;
 return yyt;
}

tTree mReal
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kReal])
 yyt->Kind = kReal;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Real.V1In)
 beginint(yyt->Real.V1Out)
 beginint(yyt->Real.VIn)
 beginint(yyt->Real.VOut)
 beginint(yyt->Real.D)
 begintParUsage(yyt->Real.PUIn)
 begintParUsage(yyt->Real.PUOut)
 return yyt;
}

tTree mReal32
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kReal32])
 yyt->Kind = kReal32;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Real32.V1In)
 beginint(yyt->Real32.V1Out)
 beginint(yyt->Real32.VIn)
 beginint(yyt->Real32.VOut)
 beginint(yyt->Real32.D)
 begintParUsage(yyt->Real32.PUIn)
 begintParUsage(yyt->Real32.PUOut)
 return yyt;
}

tTree mUserReal32
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserReal32])
 yyt->Kind = kUserReal32;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserReal32.V1In)
 beginint(yyt->UserReal32.V1Out)
 beginint(yyt->UserReal32.VIn)
 beginint(yyt->UserReal32.VOut)
 beginint(yyt->UserReal32.D)
 begintParUsage(yyt->UserReal32.PUIn)
 begintParUsage(yyt->UserReal32.PUOut)
 yyt->UserReal32.Pos = pPos;
 yyt->UserReal32.Sym = pSym;
 return yyt;
}

tTree mReal64
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kReal64])
 yyt->Kind = kReal64;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Real64.V1In)
 beginint(yyt->Real64.V1Out)
 beginint(yyt->Real64.VIn)
 beginint(yyt->Real64.VOut)
 beginint(yyt->Real64.D)
 begintParUsage(yyt->Real64.PUIn)
 begintParUsage(yyt->Real64.PUOut)
 return yyt;
}

tTree mUserReal64
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserReal64])
 yyt->Kind = kUserReal64;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserReal64.V1In)
 beginint(yyt->UserReal64.V1Out)
 beginint(yyt->UserReal64.VIn)
 beginint(yyt->UserReal64.VOut)
 beginint(yyt->UserReal64.D)
 begintParUsage(yyt->UserReal64.PUIn)
 begintParUsage(yyt->UserReal64.PUOut)
 yyt->UserReal64.Pos = pPos;
 yyt->UserReal64.Sym = pSym;
 return yyt;
}

tTree mByte
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kByte])
 yyt->Kind = kByte;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Byte.V1In)
 beginint(yyt->Byte.V1Out)
 beginint(yyt->Byte.VIn)
 beginint(yyt->Byte.VOut)
 beginint(yyt->Byte.D)
 begintParUsage(yyt->Byte.PUIn)
 begintParUsage(yyt->Byte.PUOut)
 return yyt;
}

tTree mUserByte
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserByte])
 yyt->Kind = kUserByte;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserByte.V1In)
 beginint(yyt->UserByte.V1Out)
 beginint(yyt->UserByte.VIn)
 beginint(yyt->UserByte.VOut)
 beginint(yyt->UserByte.D)
 begintParUsage(yyt->UserByte.PUIn)
 begintParUsage(yyt->UserByte.PUOut)
 yyt->UserByte.Pos = pPos;
 yyt->UserByte.Sym = pSym;
 return yyt;
}

tTree mBool
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBool])
 yyt->Kind = kBool;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->Bool.V1In)
 beginint(yyt->Bool.V1Out)
 beginint(yyt->Bool.VIn)
 beginint(yyt->Bool.VOut)
 beginint(yyt->Bool.D)
 begintParUsage(yyt->Bool.PUIn)
 begintParUsage(yyt->Bool.PUOut)
 return yyt;
}

tTree mUserBool
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserBool])
 yyt->Kind = kUserBool;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserBool.V1In)
 beginint(yyt->UserBool.V1Out)
 beginint(yyt->UserBool.VIn)
 beginint(yyt->UserBool.VOut)
 beginint(yyt->UserBool.D)
 begintParUsage(yyt->UserBool.PUIn)
 begintParUsage(yyt->UserBool.PUOut)
 yyt->UserBool.Pos = pPos;
 yyt->UserBool.Sym = pSym;
 return yyt;
}

tTree mUserStructTypes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserStructTypes])
 yyt->Kind = kUserStructTypes;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserStructTypes.V1In)
 beginint(yyt->UserStructTypes.V1Out)
 beginint(yyt->UserStructTypes.VIn)
 beginint(yyt->UserStructTypes.VOut)
 beginint(yyt->UserStructTypes.D)
 begintParUsage(yyt->UserStructTypes.PUIn)
 begintParUsage(yyt->UserStructTypes.PUOut)
 yyt->UserStructTypes.Pos = pPos;
 yyt->UserStructTypes.Sym = pSym;
 yyt->UserStructTypes.FieldList = pFieldList;
 return yyt;
}

tTree mUserDataTypes
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserDataTypes])
 yyt->Kind = kUserDataTypes;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserDataTypes.V1In)
 beginint(yyt->UserDataTypes.V1Out)
 beginint(yyt->UserDataTypes.VIn)
 beginint(yyt->UserDataTypes.VOut)
 beginint(yyt->UserDataTypes.D)
 begintParUsage(yyt->UserDataTypes.PUIn)
 begintParUsage(yyt->UserDataTypes.PUOut)
 yyt->UserDataTypes.Pos = pPos;
 yyt->UserDataTypes.Sym = pSym;
 yyt->UserDataTypes.FieldList = pFieldList;
 return yyt;
}

tTree mUserRecord
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserRecord])
 yyt->Kind = kUserRecord;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserRecord.V1In)
 beginint(yyt->UserRecord.V1Out)
 beginint(yyt->UserRecord.VIn)
 beginint(yyt->UserRecord.VOut)
 beginint(yyt->UserRecord.D)
 begintParUsage(yyt->UserRecord.PUIn)
 begintParUsage(yyt->UserRecord.PUOut)
 yyt->UserRecord.Pos = pPos;
 yyt->UserRecord.Sym = pSym;
 yyt->UserRecord.FieldList = pFieldList;
 return yyt;
}

tTree mUserPackedRecord
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserPackedRecord])
 yyt->Kind = kUserPackedRecord;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserPackedRecord.V1In)
 beginint(yyt->UserPackedRecord.V1Out)
 beginint(yyt->UserPackedRecord.VIn)
 beginint(yyt->UserPackedRecord.VOut)
 beginint(yyt->UserPackedRecord.D)
 begintParUsage(yyt->UserPackedRecord.PUIn)
 begintParUsage(yyt->UserPackedRecord.PUOut)
 yyt->UserPackedRecord.Pos = pPos;
 yyt->UserPackedRecord.Sym = pSym;
 yyt->UserPackedRecord.FieldList = pFieldList;
 return yyt;
}

tTree mUserUnion
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserUnion])
 yyt->Kind = kUserUnion;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserUnion.V1In)
 beginint(yyt->UserUnion.V1Out)
 beginint(yyt->UserUnion.VIn)
 beginint(yyt->UserUnion.VOut)
 beginint(yyt->UserUnion.D)
 begintParUsage(yyt->UserUnion.PUIn)
 begintParUsage(yyt->UserUnion.PUOut)
 yyt->UserUnion.Pos = pPos;
 yyt->UserUnion.Sym = pSym;
 yyt->UserUnion.FieldList = pFieldList;
 return yyt;
}

tTree mUserChanRecord
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pFieldList)
# else
(pPos, pSym, pFieldList)
tPosition pPos;
tSymbol pSym;
tTree pFieldList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserChanRecord])
 yyt->Kind = kUserChanRecord;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->UserChanRecord.V1In)
 beginint(yyt->UserChanRecord.V1Out)
 beginint(yyt->UserChanRecord.VIn)
 beginint(yyt->UserChanRecord.VOut)
 beginint(yyt->UserChanRecord.D)
 begintParUsage(yyt->UserChanRecord.PUIn)
 begintParUsage(yyt->UserChanRecord.PUOut)
 yyt->UserChanRecord.Pos = pPos;
 yyt->UserChanRecord.Sym = pSym;
 yyt->UserChanRecord.FieldList = pFieldList;
 return yyt;
}

tTree mTypeList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTypeList])
 yyt->Kind = kTypeList;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->TypeList.V1In)
 beginint(yyt->TypeList.V1Out)
 beginint(yyt->TypeList.VIn)
 beginint(yyt->TypeList.VOut)
 beginint(yyt->TypeList.D)
 begintParUsage(yyt->TypeList.PUIn)
 begintParUsage(yyt->TypeList.PUOut)
 return yyt;
}

tTree mNoType
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoType])
 yyt->Kind = kNoType;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoType.V1In)
 beginint(yyt->NoType.V1Out)
 beginint(yyt->NoType.VIn)
 beginint(yyt->NoType.VOut)
 beginint(yyt->NoType.D)
 begintParUsage(yyt->NoType.PUIn)
 begintParUsage(yyt->NoType.PUOut)
 return yyt;
}

tTree mAType
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pType)
# else
(pNext, pType)
tTree pNext;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAType])
 yyt->Kind = kAType;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AType.V1In)
 beginint(yyt->AType.V1Out)
 beginint(yyt->AType.VIn)
 beginint(yyt->AType.VOut)
 beginint(yyt->AType.D)
 begintParUsage(yyt->AType.PUIn)
 begintParUsage(yyt->AType.PUOut)
 yyt->AType.Next = pNext;
 yyt->AType.Type = pType;
 return yyt;
}

tTree mFieldList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kFieldList])
 yyt->Kind = kFieldList;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->FieldList.PUIn)
 begintParUsage(yyt->FieldList.PUOut)
 return yyt;
}

tTree mNoField
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoField])
 yyt->Kind = kNoField;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->NoField.PUIn)
 begintParUsage(yyt->NoField.PUOut)
 return yyt;
}

tTree mAField
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tSymbol pSym, tTree pType)
# else
(pNext, pSym, pType)
tTree pNext;
tSymbol pSym;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAField])
 yyt->Kind = kAField;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->AField.PUIn)
 begintParUsage(yyt->AField.PUOut)
 yyt->AField.Next = pNext;
 yyt->AField.Sym = pSym;
 yyt->AField.Type = pType;
 return yyt;
}

tTree mProtocol
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kProtocol])
 yyt->Kind = kProtocol;
 yyt->yyHead.yyMark = 0;
 yyt->Protocol.Pos = pPos;
 begintParUsage(yyt->Protocol.PUIn)
 begintParUsage(yyt->Protocol.PUOut)
 return yyt;
}

tTree mAny
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAny])
 yyt->Kind = kAny;
 yyt->yyHead.yyMark = 0;
 yyt->Any.Pos = pPos;
 begintParUsage(yyt->Any.PUIn)
 begintParUsage(yyt->Any.PUOut)
 return yyt;
}

tTree mNamed
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym)
# else
(pPos, pSym)
tPosition pPos;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNamed])
 yyt->Kind = kNamed;
 yyt->yyHead.yyMark = 0;
 yyt->Named.Pos = pPos;
 begintParUsage(yyt->Named.PUIn)
 begintParUsage(yyt->Named.PUOut)
 yyt->Named.Sym = pSym;
 return yyt;
}

tTree mSimple
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSimple])
 yyt->Kind = kSimple;
 yyt->yyHead.yyMark = 0;
 yyt->Simple.Pos = pPos;
 begintParUsage(yyt->Simple.PUIn)
 begintParUsage(yyt->Simple.PUOut)
 yyt->Simple.Type = pType;
 return yyt;
}

tTree mCounted
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pLength, tTree pType)
# else
(pPos, pLength, pType)
tPosition pPos;
tTree pLength;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCounted])
 yyt->Kind = kCounted;
 yyt->yyHead.yyMark = 0;
 yyt->Counted.Pos = pPos;
 begintParUsage(yyt->Counted.PUIn)
 begintParUsage(yyt->Counted.PUOut)
 yyt->Counted.Length = pLength;
 yyt->Counted.Type = pType;
 return yyt;
}

tTree mSeqProList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSeqProList])
 yyt->Kind = kSeqProList;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->SeqProList.PUIn)
 begintParUsage(yyt->SeqProList.PUOut)
 return yyt;
}

tTree mNoSeqPro
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoSeqPro])
 yyt->Kind = kNoSeqPro;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->NoSeqPro.PUIn)
 begintParUsage(yyt->NoSeqPro.PUOut)
 return yyt;
}

tTree mASeqPro
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pProtocol)
# else
(pNext, pProtocol)
tTree pNext;
tTree pProtocol;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kASeqPro])
 yyt->Kind = kASeqPro;
 yyt->yyHead.yyMark = 0;
 begintParUsage(yyt->ASeqPro.PUIn)
 begintParUsage(yyt->ASeqPro.PUOut)
 yyt->ASeqPro.Next = pNext;
 yyt->ASeqPro.Protocol = pProtocol;
 return yyt;
}

tTree mExpr
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kExpr])
 yyt->Kind = kExpr;
 yyt->yyHead.yyMark = 0;
 yyt->Expr.Pos = pPos;
 begintType(yyt->Expr.DefType)
 begintType(yyt->Expr.FLIn)
 begintType(yyt->Expr.FLOut)
 beginint(yyt->Expr.V1In)
 beginint(yyt->Expr.V1Out)
 begintType(yyt->Expr.SynType)
 begintType(yyt->Expr.InhType)
 beginint(yyt->Expr.VIn)
 beginint(yyt->Expr.VOut)
 beginbool(yyt->Expr.C)
 begintValue(yyt->Expr.CV)
 begintSymbol(yyt->Expr.Parent)
 begintParUsage(yyt->Expr.PUIn)
 begintParUsage(yyt->Expr.PUOut)
 beginbool(yyt->Expr.LV)
 return yyt;
}

tTree mBinary
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pLop, tTree pRop, tTree pOp)
# else
(pPos, pLop, pRop, pOp)
tPosition pPos;
tTree pLop;
tTree pRop;
tTree pOp;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBinary])
 yyt->Kind = kBinary;
 yyt->yyHead.yyMark = 0;
 yyt->Binary.Pos = pPos;
 begintType(yyt->Binary.DefType)
 begintType(yyt->Binary.FLIn)
 begintType(yyt->Binary.FLOut)
 beginint(yyt->Binary.V1In)
 beginint(yyt->Binary.V1Out)
 begintType(yyt->Binary.SynType)
 begintType(yyt->Binary.InhType)
 beginint(yyt->Binary.VIn)
 beginint(yyt->Binary.VOut)
 beginbool(yyt->Binary.C)
 begintValue(yyt->Binary.CV)
 begintSymbol(yyt->Binary.Parent)
 begintParUsage(yyt->Binary.PUIn)
 begintParUsage(yyt->Binary.PUOut)
 beginbool(yyt->Binary.LV)
 yyt->Binary.Lop = pLop;
 yyt->Binary.Rop = pRop;
 yyt->Binary.Op = pOp;
 return yyt;
}

tTree mUnary
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tTree pOp)
# else
(pPos, pExpr, pOp)
tPosition pPos;
tTree pExpr;
tTree pOp;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUnary])
 yyt->Kind = kUnary;
 yyt->yyHead.yyMark = 0;
 yyt->Unary.Pos = pPos;
 begintType(yyt->Unary.DefType)
 begintType(yyt->Unary.FLIn)
 begintType(yyt->Unary.FLOut)
 beginint(yyt->Unary.V1In)
 beginint(yyt->Unary.V1Out)
 begintType(yyt->Unary.SynType)
 begintType(yyt->Unary.InhType)
 beginint(yyt->Unary.VIn)
 beginint(yyt->Unary.VOut)
 beginbool(yyt->Unary.C)
 begintValue(yyt->Unary.CV)
 begintSymbol(yyt->Unary.Parent)
 begintParUsage(yyt->Unary.PUIn)
 begintParUsage(yyt->Unary.PUOut)
 beginbool(yyt->Unary.LV)
 yyt->Unary.Expr = pExpr;
 yyt->Unary.Op = pOp;
 return yyt;
}

tTree mConst
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tValue pValue, tTree pType)
# else
(pPos, pValue, pType)
tPosition pPos;
tValue pValue;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kConst])
 yyt->Kind = kConst;
 yyt->yyHead.yyMark = 0;
 yyt->Const.Pos = pPos;
 begintType(yyt->Const.DefType)
 begintType(yyt->Const.FLIn)
 begintType(yyt->Const.FLOut)
 beginint(yyt->Const.V1In)
 beginint(yyt->Const.V1Out)
 begintType(yyt->Const.SynType)
 begintType(yyt->Const.InhType)
 beginint(yyt->Const.VIn)
 beginint(yyt->Const.VOut)
 beginbool(yyt->Const.C)
 begintValue(yyt->Const.CV)
 begintSymbol(yyt->Const.Parent)
 begintParUsage(yyt->Const.PUIn)
 begintParUsage(yyt->Const.PUOut)
 beginbool(yyt->Const.LV)
 yyt->Const.Value = pValue;
 yyt->Const.Type = pType;
 return yyt;
}

tTree mValof
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pProcess, tTree pExprList, tSymbol pSym, int pVS)
# else
(pPos, pSpecList, pProcess, pExprList, pSym, pVS)
tPosition pPos;
tTree pSpecList;
tTree pProcess;
tTree pExprList;
tSymbol pSym;
int pVS;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kValof])
 yyt->Kind = kValof;
 yyt->yyHead.yyMark = 0;
 yyt->Valof.Pos = pPos;
 begintType(yyt->Valof.DefType)
 begintType(yyt->Valof.FLIn)
 begintType(yyt->Valof.FLOut)
 beginint(yyt->Valof.V1In)
 beginint(yyt->Valof.V1Out)
 begintType(yyt->Valof.SynType)
 begintType(yyt->Valof.InhType)
 beginint(yyt->Valof.VIn)
 beginint(yyt->Valof.VOut)
 beginbool(yyt->Valof.C)
 begintValue(yyt->Valof.CV)
 begintSymbol(yyt->Valof.Parent)
 begintParUsage(yyt->Valof.PUIn)
 begintParUsage(yyt->Valof.PUOut)
 beginbool(yyt->Valof.LV)
 yyt->Valof.SpecList = pSpecList;
 yyt->Valof.Process = pProcess;
 yyt->Valof.ExprList = pExprList;
 yyt->Valof.Sym = pSym;
 begintTree(yyt->Valof.FuncCall)
 yyt->Valof.VS = pVS;
 return yyt;
}

tTree mFuncCall
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tTree pExprList, int pDepth)
# else
(pPos, pSym, pExprList, pDepth)
tPosition pPos;
tSymbol pSym;
tTree pExprList;
int pDepth;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kFuncCall])
 yyt->Kind = kFuncCall;
 yyt->yyHead.yyMark = 0;
 yyt->FuncCall.Pos = pPos;
 begintType(yyt->FuncCall.DefType)
 begintType(yyt->FuncCall.FLIn)
 begintType(yyt->FuncCall.FLOut)
 beginint(yyt->FuncCall.V1In)
 beginint(yyt->FuncCall.V1Out)
 begintType(yyt->FuncCall.SynType)
 begintType(yyt->FuncCall.InhType)
 beginint(yyt->FuncCall.VIn)
 beginint(yyt->FuncCall.VOut)
 beginbool(yyt->FuncCall.C)
 begintValue(yyt->FuncCall.CV)
 begintSymbol(yyt->FuncCall.Parent)
 begintParUsage(yyt->FuncCall.PUIn)
 begintParUsage(yyt->FuncCall.PUOut)
 beginbool(yyt->FuncCall.LV)
 yyt->FuncCall.Sym = pSym;
 yyt->FuncCall.ExprList = pExprList;
 yyt->FuncCall.Depth = pDepth;
 begintFrame(yyt->FuncCall.Frame)
 return yyt;
}

tTree mElementalExpr
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pElemental)
# else
(pPos, pElemental)
tPosition pPos;
tTree pElemental;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kElementalExpr])
 yyt->Kind = kElementalExpr;
 yyt->yyHead.yyMark = 0;
 yyt->ElementalExpr.Pos = pPos;
 begintType(yyt->ElementalExpr.DefType)
 begintType(yyt->ElementalExpr.FLIn)
 begintType(yyt->ElementalExpr.FLOut)
 beginint(yyt->ElementalExpr.V1In)
 beginint(yyt->ElementalExpr.V1Out)
 begintType(yyt->ElementalExpr.SynType)
 begintType(yyt->ElementalExpr.InhType)
 beginint(yyt->ElementalExpr.VIn)
 beginint(yyt->ElementalExpr.VOut)
 beginbool(yyt->ElementalExpr.C)
 begintValue(yyt->ElementalExpr.CV)
 begintSymbol(yyt->ElementalExpr.Parent)
 begintParUsage(yyt->ElementalExpr.PUIn)
 begintParUsage(yyt->ElementalExpr.PUOut)
 beginbool(yyt->ElementalExpr.LV)
 yyt->ElementalExpr.Elemental = pElemental;
 beginbool(yyt->ElementalExpr.PAR)
 return yyt;
}

tTree mDefault
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDefault])
 yyt->Kind = kDefault;
 yyt->yyHead.yyMark = 0;
 yyt->Default.Pos = pPos;
 begintType(yyt->Default.DefType)
 begintType(yyt->Default.FLIn)
 begintType(yyt->Default.FLOut)
 beginint(yyt->Default.V1In)
 beginint(yyt->Default.V1Out)
 begintType(yyt->Default.SynType)
 begintType(yyt->Default.InhType)
 beginint(yyt->Default.VIn)
 beginint(yyt->Default.VOut)
 beginbool(yyt->Default.C)
 begintValue(yyt->Default.CV)
 begintSymbol(yyt->Default.Parent)
 begintParUsage(yyt->Default.PUIn)
 begintParUsage(yyt->Default.PUOut)
 beginbool(yyt->Default.LV)
 return yyt;
}

tTree mConvert
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tTree pExpr)
# else
(pPos, pType, pExpr)
tPosition pPos;
tTree pType;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kConvert])
 yyt->Kind = kConvert;
 yyt->yyHead.yyMark = 0;
 yyt->Convert.Pos = pPos;
 begintType(yyt->Convert.DefType)
 begintType(yyt->Convert.FLIn)
 begintType(yyt->Convert.FLOut)
 beginint(yyt->Convert.V1In)
 beginint(yyt->Convert.V1Out)
 begintType(yyt->Convert.SynType)
 begintType(yyt->Convert.InhType)
 beginint(yyt->Convert.VIn)
 beginint(yyt->Convert.VOut)
 beginbool(yyt->Convert.C)
 begintValue(yyt->Convert.CV)
 begintSymbol(yyt->Convert.Parent)
 begintParUsage(yyt->Convert.PUIn)
 begintParUsage(yyt->Convert.PUOut)
 beginbool(yyt->Convert.LV)
 yyt->Convert.Type = pType;
 yyt->Convert.Expr = pExpr;
 return yyt;
}

tTree mRound
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tTree pExpr)
# else
(pPos, pType, pExpr)
tPosition pPos;
tTree pType;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRound])
 yyt->Kind = kRound;
 yyt->yyHead.yyMark = 0;
 yyt->Round.Pos = pPos;
 begintType(yyt->Round.DefType)
 begintType(yyt->Round.FLIn)
 begintType(yyt->Round.FLOut)
 beginint(yyt->Round.V1In)
 beginint(yyt->Round.V1Out)
 begintType(yyt->Round.SynType)
 begintType(yyt->Round.InhType)
 beginint(yyt->Round.VIn)
 beginint(yyt->Round.VOut)
 beginbool(yyt->Round.C)
 begintValue(yyt->Round.CV)
 begintSymbol(yyt->Round.Parent)
 begintParUsage(yyt->Round.PUIn)
 begintParUsage(yyt->Round.PUOut)
 beginbool(yyt->Round.LV)
 yyt->Round.Type = pType;
 yyt->Round.Expr = pExpr;
 return yyt;
}

tTree mTrunc
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tTree pExpr)
# else
(pPos, pType, pExpr)
tPosition pPos;
tTree pType;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTrunc])
 yyt->Kind = kTrunc;
 yyt->yyHead.yyMark = 0;
 yyt->Trunc.Pos = pPos;
 begintType(yyt->Trunc.DefType)
 begintType(yyt->Trunc.FLIn)
 begintType(yyt->Trunc.FLOut)
 beginint(yyt->Trunc.V1In)
 beginint(yyt->Trunc.V1Out)
 begintType(yyt->Trunc.SynType)
 begintType(yyt->Trunc.InhType)
 beginint(yyt->Trunc.VIn)
 beginint(yyt->Trunc.VOut)
 beginbool(yyt->Trunc.C)
 begintValue(yyt->Trunc.CV)
 begintSymbol(yyt->Trunc.Parent)
 begintParUsage(yyt->Trunc.PUIn)
 begintParUsage(yyt->Trunc.PUOut)
 beginbool(yyt->Trunc.LV)
 yyt->Trunc.Type = pType;
 yyt->Trunc.Expr = pExpr;
 return yyt;
}

tTree mMostPos
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kMostPos])
 yyt->Kind = kMostPos;
 yyt->yyHead.yyMark = 0;
 yyt->MostPos.Pos = pPos;
 begintType(yyt->MostPos.DefType)
 begintType(yyt->MostPos.FLIn)
 begintType(yyt->MostPos.FLOut)
 beginint(yyt->MostPos.V1In)
 beginint(yyt->MostPos.V1Out)
 begintType(yyt->MostPos.SynType)
 begintType(yyt->MostPos.InhType)
 beginint(yyt->MostPos.VIn)
 beginint(yyt->MostPos.VOut)
 beginbool(yyt->MostPos.C)
 begintValue(yyt->MostPos.CV)
 begintSymbol(yyt->MostPos.Parent)
 begintParUsage(yyt->MostPos.PUIn)
 begintParUsage(yyt->MostPos.PUOut)
 beginbool(yyt->MostPos.LV)
 yyt->MostPos.Type = pType;
 return yyt;
}

tTree mMostNeg
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kMostNeg])
 yyt->Kind = kMostNeg;
 yyt->yyHead.yyMark = 0;
 yyt->MostNeg.Pos = pPos;
 begintType(yyt->MostNeg.DefType)
 begintType(yyt->MostNeg.FLIn)
 begintType(yyt->MostNeg.FLOut)
 beginint(yyt->MostNeg.V1In)
 beginint(yyt->MostNeg.V1Out)
 begintType(yyt->MostNeg.SynType)
 begintType(yyt->MostNeg.InhType)
 beginint(yyt->MostNeg.VIn)
 beginint(yyt->MostNeg.VOut)
 beginbool(yyt->MostNeg.C)
 begintValue(yyt->MostNeg.CV)
 begintSymbol(yyt->MostNeg.Parent)
 begintParUsage(yyt->MostNeg.PUIn)
 begintParUsage(yyt->MostNeg.PUOut)
 beginbool(yyt->MostNeg.LV)
 yyt->MostNeg.Type = pType;
 return yyt;
}

tTree mCountArray
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pLength, tTree pExpr)
# else
(pPos, pLength, pExpr)
tPosition pPos;
tTree pLength;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCountArray])
 yyt->Kind = kCountArray;
 yyt->yyHead.yyMark = 0;
 yyt->CountArray.Pos = pPos;
 begintType(yyt->CountArray.DefType)
 begintType(yyt->CountArray.FLIn)
 begintType(yyt->CountArray.FLOut)
 beginint(yyt->CountArray.V1In)
 beginint(yyt->CountArray.V1Out)
 begintType(yyt->CountArray.SynType)
 begintType(yyt->CountArray.InhType)
 beginint(yyt->CountArray.VIn)
 beginint(yyt->CountArray.VOut)
 beginbool(yyt->CountArray.C)
 begintValue(yyt->CountArray.CV)
 begintSymbol(yyt->CountArray.Parent)
 begintParUsage(yyt->CountArray.PUIn)
 begintParUsage(yyt->CountArray.PUOut)
 beginbool(yyt->CountArray.LV)
 yyt->CountArray.Length = pLength;
 yyt->CountArray.Expr = pExpr;
 return yyt;
}

tTree mUnBound
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUnBound])
 yyt->Kind = kUnBound;
 yyt->yyHead.yyMark = 0;
 yyt->UnBound.Pos = pPos;
 begintType(yyt->UnBound.DefType)
 begintType(yyt->UnBound.FLIn)
 begintType(yyt->UnBound.FLOut)
 beginint(yyt->UnBound.V1In)
 beginint(yyt->UnBound.V1Out)
 begintType(yyt->UnBound.SynType)
 begintType(yyt->UnBound.InhType)
 beginint(yyt->UnBound.VIn)
 beginint(yyt->UnBound.VOut)
 beginbool(yyt->UnBound.C)
 begintValue(yyt->UnBound.CV)
 begintSymbol(yyt->UnBound.Parent)
 begintParUsage(yyt->UnBound.PUIn)
 begintParUsage(yyt->UnBound.PUOut)
 beginbool(yyt->UnBound.LV)
 return yyt;
}

tTree mBytesIn
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBytesIn])
 yyt->Kind = kBytesIn;
 yyt->yyHead.yyMark = 0;
 yyt->BytesIn.Pos = pPos;
 begintType(yyt->BytesIn.DefType)
 begintType(yyt->BytesIn.FLIn)
 begintType(yyt->BytesIn.FLOut)
 beginint(yyt->BytesIn.V1In)
 beginint(yyt->BytesIn.V1Out)
 begintType(yyt->BytesIn.SynType)
 begintType(yyt->BytesIn.InhType)
 beginint(yyt->BytesIn.VIn)
 beginint(yyt->BytesIn.VOut)
 beginbool(yyt->BytesIn.C)
 begintValue(yyt->BytesIn.CV)
 begintSymbol(yyt->BytesIn.Parent)
 begintParUsage(yyt->BytesIn.PUIn)
 begintParUsage(yyt->BytesIn.PUOut)
 beginbool(yyt->BytesIn.LV)
 return yyt;
}

tTree mBytesInType
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBytesInType])
 yyt->Kind = kBytesInType;
 yyt->yyHead.yyMark = 0;
 yyt->BytesInType.Pos = pPos;
 begintType(yyt->BytesInType.DefType)
 begintType(yyt->BytesInType.FLIn)
 begintType(yyt->BytesInType.FLOut)
 beginint(yyt->BytesInType.V1In)
 beginint(yyt->BytesInType.V1Out)
 begintType(yyt->BytesInType.SynType)
 begintType(yyt->BytesInType.InhType)
 beginint(yyt->BytesInType.VIn)
 beginint(yyt->BytesInType.VOut)
 beginbool(yyt->BytesInType.C)
 begintValue(yyt->BytesInType.CV)
 begintSymbol(yyt->BytesInType.Parent)
 begintParUsage(yyt->BytesInType.PUIn)
 begintParUsage(yyt->BytesInType.PUOut)
 beginbool(yyt->BytesInType.LV)
 yyt->BytesInType.Type = pType;
 return yyt;
}

tTree mBytesInExpr
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr)
# else
(pPos, pExpr)
tPosition pPos;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBytesInExpr])
 yyt->Kind = kBytesInExpr;
 yyt->yyHead.yyMark = 0;
 yyt->BytesInExpr.Pos = pPos;
 begintType(yyt->BytesInExpr.DefType)
 begintType(yyt->BytesInExpr.FLIn)
 begintType(yyt->BytesInExpr.FLOut)
 beginint(yyt->BytesInExpr.V1In)
 beginint(yyt->BytesInExpr.V1Out)
 begintType(yyt->BytesInExpr.SynType)
 begintType(yyt->BytesInExpr.InhType)
 beginint(yyt->BytesInExpr.VIn)
 beginint(yyt->BytesInExpr.VOut)
 beginbool(yyt->BytesInExpr.C)
 begintValue(yyt->BytesInExpr.CV)
 begintSymbol(yyt->BytesInExpr.Parent)
 begintParUsage(yyt->BytesInExpr.PUIn)
 begintParUsage(yyt->BytesInExpr.PUOut)
 beginbool(yyt->BytesInExpr.LV)
 yyt->BytesInExpr.Expr = pExpr;
 return yyt;
}

tTree mSizeOf
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType)
# else
(pPos, pType)
tPosition pPos;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSizeOf])
 yyt->Kind = kSizeOf;
 yyt->yyHead.yyMark = 0;
 yyt->SizeOf.Pos = pPos;
 begintType(yyt->SizeOf.DefType)
 begintType(yyt->SizeOf.FLIn)
 begintType(yyt->SizeOf.FLOut)
 beginint(yyt->SizeOf.V1In)
 beginint(yyt->SizeOf.V1Out)
 begintType(yyt->SizeOf.SynType)
 begintType(yyt->SizeOf.InhType)
 beginint(yyt->SizeOf.VIn)
 beginint(yyt->SizeOf.VOut)
 beginbool(yyt->SizeOf.C)
 begintValue(yyt->SizeOf.CV)
 begintSymbol(yyt->SizeOf.Parent)
 begintParUsage(yyt->SizeOf.PUIn)
 begintParUsage(yyt->SizeOf.PUOut)
 beginbool(yyt->SizeOf.LV)
 yyt->SizeOf.Type = pType;
 return yyt;
}

tTree mOffsetOf
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pType, tTree pUnmappedIdentifier)
# else
(pPos, pType, pUnmappedIdentifier)
tPosition pPos;
tTree pType;
tTree pUnmappedIdentifier;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kOffsetOf])
 yyt->Kind = kOffsetOf;
 yyt->yyHead.yyMark = 0;
 yyt->OffsetOf.Pos = pPos;
 begintType(yyt->OffsetOf.DefType)
 begintType(yyt->OffsetOf.FLIn)
 begintType(yyt->OffsetOf.FLOut)
 beginint(yyt->OffsetOf.V1In)
 beginint(yyt->OffsetOf.V1Out)
 begintType(yyt->OffsetOf.SynType)
 begintType(yyt->OffsetOf.InhType)
 beginint(yyt->OffsetOf.VIn)
 beginint(yyt->OffsetOf.VOut)
 beginbool(yyt->OffsetOf.C)
 begintValue(yyt->OffsetOf.CV)
 begintSymbol(yyt->OffsetOf.Parent)
 begintParUsage(yyt->OffsetOf.PUIn)
 begintParUsage(yyt->OffsetOf.PUOut)
 beginbool(yyt->OffsetOf.LV)
 yyt->OffsetOf.Type = pType;
 yyt->OffsetOf.UnmappedIdentifier = pUnmappedIdentifier;
 return yyt;
}

tTree mStringElement
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kStringElement])
 yyt->Kind = kStringElement;
 yyt->yyHead.yyMark = 0;
 yyt->StringElement.Pos = pPos;
 begintType(yyt->StringElement.DefType)
 begintType(yyt->StringElement.FLIn)
 begintType(yyt->StringElement.FLOut)
 beginint(yyt->StringElement.V1In)
 beginint(yyt->StringElement.V1Out)
 begintType(yyt->StringElement.SynType)
 begintType(yyt->StringElement.InhType)
 beginint(yyt->StringElement.VIn)
 beginint(yyt->StringElement.VOut)
 beginbool(yyt->StringElement.C)
 begintValue(yyt->StringElement.CV)
 begintSymbol(yyt->StringElement.Parent)
 begintParUsage(yyt->StringElement.PUIn)
 begintParUsage(yyt->StringElement.PUOut)
 beginbool(yyt->StringElement.LV)
 begintConstArray(yyt->StringElement.CA)
 beginbool(yyt->StringElement.SE)
 return yyt;
}

tTree mString
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tStringRef pValue, tTree pType, int pDepth, tSymbol pSym)
# else
(pPos, pValue, pType, pDepth, pSym)
tPosition pPos;
tStringRef pValue;
tTree pType;
int pDepth;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kString])
 yyt->Kind = kString;
 yyt->yyHead.yyMark = 0;
 yyt->String.Pos = pPos;
 begintType(yyt->String.DefType)
 begintType(yyt->String.FLIn)
 begintType(yyt->String.FLOut)
 beginint(yyt->String.V1In)
 beginint(yyt->String.V1Out)
 begintType(yyt->String.SynType)
 begintType(yyt->String.InhType)
 beginint(yyt->String.VIn)
 beginint(yyt->String.VOut)
 beginbool(yyt->String.C)
 begintValue(yyt->String.CV)
 begintSymbol(yyt->String.Parent)
 begintParUsage(yyt->String.PUIn)
 begintParUsage(yyt->String.PUOut)
 beginbool(yyt->String.LV)
 begintConstArray(yyt->String.CA)
 beginbool(yyt->String.SE)
 yyt->String.Value = pValue;
 yyt->String.Type = pType;
 yyt->String.Depth = pDepth;
 yyt->String.Sym = pSym;
 beginpLinExpr(yyt->String.LE)
 return yyt;
}

tTree mStringSubscript
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pExpr)
# else
(pPos, pNext, pExpr)
tPosition pPos;
tTree pNext;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kStringSubscript])
 yyt->Kind = kStringSubscript;
 yyt->yyHead.yyMark = 0;
 yyt->StringSubscript.Pos = pPos;
 begintType(yyt->StringSubscript.DefType)
 begintType(yyt->StringSubscript.FLIn)
 begintType(yyt->StringSubscript.FLOut)
 beginint(yyt->StringSubscript.V1In)
 beginint(yyt->StringSubscript.V1Out)
 begintType(yyt->StringSubscript.SynType)
 begintType(yyt->StringSubscript.InhType)
 beginint(yyt->StringSubscript.VIn)
 beginint(yyt->StringSubscript.VOut)
 beginbool(yyt->StringSubscript.C)
 begintValue(yyt->StringSubscript.CV)
 begintSymbol(yyt->StringSubscript.Parent)
 begintParUsage(yyt->StringSubscript.PUIn)
 begintParUsage(yyt->StringSubscript.PUOut)
 beginbool(yyt->StringSubscript.LV)
 begintConstArray(yyt->StringSubscript.CA)
 beginbool(yyt->StringSubscript.SE)
 yyt->StringSubscript.Next = pNext;
 yyt->StringSubscript.Expr = pExpr;
 beginpLinExpr(yyt->StringSubscript.LE)
 return yyt;
}

tTree mStringRange
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pFrom, tTree pFor)
# else
(pPos, pNext, pFrom, pFor)
tPosition pPos;
tTree pNext;
tTree pFrom;
tTree pFor;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kStringRange])
 yyt->Kind = kStringRange;
 yyt->yyHead.yyMark = 0;
 yyt->StringRange.Pos = pPos;
 begintType(yyt->StringRange.DefType)
 begintType(yyt->StringRange.FLIn)
 begintType(yyt->StringRange.FLOut)
 beginint(yyt->StringRange.V1In)
 beginint(yyt->StringRange.V1Out)
 begintType(yyt->StringRange.SynType)
 begintType(yyt->StringRange.InhType)
 beginint(yyt->StringRange.VIn)
 beginint(yyt->StringRange.VOut)
 beginbool(yyt->StringRange.C)
 begintValue(yyt->StringRange.CV)
 begintSymbol(yyt->StringRange.Parent)
 begintParUsage(yyt->StringRange.PUIn)
 begintParUsage(yyt->StringRange.PUOut)
 beginbool(yyt->StringRange.LV)
 begintConstArray(yyt->StringRange.CA)
 beginbool(yyt->StringRange.SE)
 yyt->StringRange.Next = pNext;
 yyt->StringRange.From = pFrom;
 yyt->StringRange.For = pFor;
 beginpLinExpr(yyt->StringRange.FromLE)
 beginpLinExpr(yyt->StringRange.ForLE)
 return yyt;
}

tTree mElemental
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kElemental])
 yyt->Kind = kElemental;
 yyt->yyHead.yyMark = 0;
 yyt->Elemental.Pos = pPos;
 begintType(yyt->Elemental.DefType)
 begintType(yyt->Elemental.FLIn)
 begintType(yyt->Elemental.FLOut)
 beginint(yyt->Elemental.V1In)
 beginint(yyt->Elemental.V1Out)
 begintType(yyt->Elemental.SynType)
 begintType(yyt->Elemental.InhType)
 beginint(yyt->Elemental.W)
 beginint(yyt->Elemental.VIn)
 beginint(yyt->Elemental.VOut)
 beginbool(yyt->Elemental.C)
 begintValue(yyt->Elemental.CV)
 begintConstArray(yyt->Elemental.CA)
 begintUsage(yyt->Elemental.U)
 begintSymbol(yyt->Elemental.Parent)
 begintParUsage(yyt->Elemental.PUIn)
 begintParUsage(yyt->Elemental.PUOut)
 beginbool(yyt->Elemental.LV)
 beginbool(yyt->Elemental.SE)
 begintTree(yyt->Elemental.RVL)
 return yyt;
}

tTree mTableElement
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTableElement])
 yyt->Kind = kTableElement;
 yyt->yyHead.yyMark = 0;
 yyt->TableElement.Pos = pPos;
 begintType(yyt->TableElement.DefType)
 begintType(yyt->TableElement.FLIn)
 begintType(yyt->TableElement.FLOut)
 beginint(yyt->TableElement.V1In)
 beginint(yyt->TableElement.V1Out)
 begintType(yyt->TableElement.SynType)
 begintType(yyt->TableElement.InhType)
 beginint(yyt->TableElement.W)
 beginint(yyt->TableElement.VIn)
 beginint(yyt->TableElement.VOut)
 beginbool(yyt->TableElement.C)
 begintValue(yyt->TableElement.CV)
 begintConstArray(yyt->TableElement.CA)
 begintUsage(yyt->TableElement.U)
 begintSymbol(yyt->TableElement.Parent)
 begintParUsage(yyt->TableElement.PUIn)
 begintParUsage(yyt->TableElement.PUOut)
 beginbool(yyt->TableElement.LV)
 beginbool(yyt->TableElement.SE)
 begintTree(yyt->TableElement.RVL)
 return yyt;
}

tTree mTable
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExprList, int pDepth, tSymbol pSym)
# else
(pPos, pExprList, pDepth, pSym)
tPosition pPos;
tTree pExprList;
int pDepth;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTable])
 yyt->Kind = kTable;
 yyt->yyHead.yyMark = 0;
 yyt->Table.Pos = pPos;
 begintType(yyt->Table.DefType)
 begintType(yyt->Table.FLIn)
 begintType(yyt->Table.FLOut)
 beginint(yyt->Table.V1In)
 beginint(yyt->Table.V1Out)
 begintType(yyt->Table.SynType)
 begintType(yyt->Table.InhType)
 beginint(yyt->Table.W)
 beginint(yyt->Table.VIn)
 beginint(yyt->Table.VOut)
 beginbool(yyt->Table.C)
 begintValue(yyt->Table.CV)
 begintConstArray(yyt->Table.CA)
 begintUsage(yyt->Table.U)
 begintSymbol(yyt->Table.Parent)
 begintParUsage(yyt->Table.PUIn)
 begintParUsage(yyt->Table.PUOut)
 beginbool(yyt->Table.LV)
 beginbool(yyt->Table.SE)
 begintTree(yyt->Table.RVL)
 yyt->Table.ExprList = pExprList;
 yyt->Table.Depth = pDepth;
 yyt->Table.Sym = pSym;
 beginpLinExpr(yyt->Table.LE)
 return yyt;
}

tTree mUserLiteral
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExprList, int pDepth, tSymbol pSym, tTree pType)
# else
(pPos, pExprList, pDepth, pSym, pType)
tPosition pPos;
tTree pExprList;
int pDepth;
tSymbol pSym;
tTree pType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUserLiteral])
 yyt->Kind = kUserLiteral;
 yyt->yyHead.yyMark = 0;
 yyt->UserLiteral.Pos = pPos;
 begintType(yyt->UserLiteral.DefType)
 begintType(yyt->UserLiteral.FLIn)
 begintType(yyt->UserLiteral.FLOut)
 beginint(yyt->UserLiteral.V1In)
 beginint(yyt->UserLiteral.V1Out)
 begintType(yyt->UserLiteral.SynType)
 begintType(yyt->UserLiteral.InhType)
 beginint(yyt->UserLiteral.W)
 beginint(yyt->UserLiteral.VIn)
 beginint(yyt->UserLiteral.VOut)
 beginbool(yyt->UserLiteral.C)
 begintValue(yyt->UserLiteral.CV)
 begintConstArray(yyt->UserLiteral.CA)
 begintUsage(yyt->UserLiteral.U)
 begintSymbol(yyt->UserLiteral.Parent)
 begintParUsage(yyt->UserLiteral.PUIn)
 begintParUsage(yyt->UserLiteral.PUOut)
 beginbool(yyt->UserLiteral.LV)
 beginbool(yyt->UserLiteral.SE)
 begintTree(yyt->UserLiteral.RVL)
 yyt->UserLiteral.ExprList = pExprList;
 yyt->UserLiteral.Depth = pDepth;
 yyt->UserLiteral.Sym = pSym;
 beginpLinExpr(yyt->UserLiteral.LE)
 yyt->UserLiteral.Type = pType;
 return yyt;
}

tTree mTableExpr
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tSymbol pSym)
# else
(pPos, pExpr, pSym)
tPosition pPos;
tTree pExpr;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTableExpr])
 yyt->Kind = kTableExpr;
 yyt->yyHead.yyMark = 0;
 yyt->TableExpr.Pos = pPos;
 begintType(yyt->TableExpr.DefType)
 begintType(yyt->TableExpr.FLIn)
 begintType(yyt->TableExpr.FLOut)
 beginint(yyt->TableExpr.V1In)
 beginint(yyt->TableExpr.V1Out)
 begintType(yyt->TableExpr.SynType)
 begintType(yyt->TableExpr.InhType)
 beginint(yyt->TableExpr.W)
 beginint(yyt->TableExpr.VIn)
 beginint(yyt->TableExpr.VOut)
 beginbool(yyt->TableExpr.C)
 begintValue(yyt->TableExpr.CV)
 begintConstArray(yyt->TableExpr.CA)
 begintUsage(yyt->TableExpr.U)
 begintSymbol(yyt->TableExpr.Parent)
 begintParUsage(yyt->TableExpr.PUIn)
 begintParUsage(yyt->TableExpr.PUOut)
 beginbool(yyt->TableExpr.LV)
 beginbool(yyt->TableExpr.SE)
 begintTree(yyt->TableExpr.RVL)
 yyt->TableExpr.Expr = pExpr;
 yyt->TableExpr.Sym = pSym;
 return yyt;
}

tTree mTableSubscript
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pExpr)
# else
(pPos, pNext, pExpr)
tPosition pPos;
tTree pNext;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTableSubscript])
 yyt->Kind = kTableSubscript;
 yyt->yyHead.yyMark = 0;
 yyt->TableSubscript.Pos = pPos;
 begintType(yyt->TableSubscript.DefType)
 begintType(yyt->TableSubscript.FLIn)
 begintType(yyt->TableSubscript.FLOut)
 beginint(yyt->TableSubscript.V1In)
 beginint(yyt->TableSubscript.V1Out)
 begintType(yyt->TableSubscript.SynType)
 begintType(yyt->TableSubscript.InhType)
 beginint(yyt->TableSubscript.W)
 beginint(yyt->TableSubscript.VIn)
 beginint(yyt->TableSubscript.VOut)
 beginbool(yyt->TableSubscript.C)
 begintValue(yyt->TableSubscript.CV)
 begintConstArray(yyt->TableSubscript.CA)
 begintUsage(yyt->TableSubscript.U)
 begintSymbol(yyt->TableSubscript.Parent)
 begintParUsage(yyt->TableSubscript.PUIn)
 begintParUsage(yyt->TableSubscript.PUOut)
 beginbool(yyt->TableSubscript.LV)
 beginbool(yyt->TableSubscript.SE)
 begintTree(yyt->TableSubscript.RVL)
 yyt->TableSubscript.Next = pNext;
 yyt->TableSubscript.Expr = pExpr;
 beginpLinExpr(yyt->TableSubscript.LE)
 return yyt;
}

tTree mTableRange
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pFrom, tTree pFor)
# else
(pPos, pNext, pFrom, pFor)
tPosition pPos;
tTree pNext;
tTree pFrom;
tTree pFor;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTableRange])
 yyt->Kind = kTableRange;
 yyt->yyHead.yyMark = 0;
 yyt->TableRange.Pos = pPos;
 begintType(yyt->TableRange.DefType)
 begintType(yyt->TableRange.FLIn)
 begintType(yyt->TableRange.FLOut)
 beginint(yyt->TableRange.V1In)
 beginint(yyt->TableRange.V1Out)
 begintType(yyt->TableRange.SynType)
 begintType(yyt->TableRange.InhType)
 beginint(yyt->TableRange.W)
 beginint(yyt->TableRange.VIn)
 beginint(yyt->TableRange.VOut)
 beginbool(yyt->TableRange.C)
 begintValue(yyt->TableRange.CV)
 begintConstArray(yyt->TableRange.CA)
 begintUsage(yyt->TableRange.U)
 begintSymbol(yyt->TableRange.Parent)
 begintParUsage(yyt->TableRange.PUIn)
 begintParUsage(yyt->TableRange.PUOut)
 beginbool(yyt->TableRange.LV)
 beginbool(yyt->TableRange.SE)
 begintTree(yyt->TableRange.RVL)
 yyt->TableRange.Next = pNext;
 yyt->TableRange.From = pFrom;
 yyt->TableRange.For = pFor;
 beginpLinExpr(yyt->TableRange.FromLE)
 beginpLinExpr(yyt->TableRange.ForLE)
 return yyt;
}

tTree mElement
# if defined __STDC__ | defined __cplusplus
(tPosition pPos)
# else
(pPos)
tPosition pPos;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kElement])
 yyt->Kind = kElement;
 yyt->yyHead.yyMark = 0;
 yyt->Element.Pos = pPos;
 begintType(yyt->Element.DefType)
 begintType(yyt->Element.FLIn)
 begintType(yyt->Element.FLOut)
 beginint(yyt->Element.V1In)
 beginint(yyt->Element.V1Out)
 begintType(yyt->Element.SynType)
 begintType(yyt->Element.InhType)
 beginint(yyt->Element.W)
 beginint(yyt->Element.VIn)
 beginint(yyt->Element.VOut)
 beginbool(yyt->Element.C)
 begintValue(yyt->Element.CV)
 begintConstArray(yyt->Element.CA)
 begintUsage(yyt->Element.U)
 begintSymbol(yyt->Element.Parent)
 begintParUsage(yyt->Element.PUIn)
 begintParUsage(yyt->Element.PUOut)
 beginbool(yyt->Element.LV)
 beginbool(yyt->Element.SE)
 begintTree(yyt->Element.RVL)
 return yyt;
}

tTree mIdentifier
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tSymbol pScope)
# else
(pPos, pSym, pScope)
tPosition pPos;
tSymbol pSym;
tSymbol pScope;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIdentifier])
 yyt->Kind = kIdentifier;
 yyt->yyHead.yyMark = 0;
 yyt->Identifier.Pos = pPos;
 begintType(yyt->Identifier.DefType)
 begintType(yyt->Identifier.FLIn)
 begintType(yyt->Identifier.FLOut)
 beginint(yyt->Identifier.V1In)
 beginint(yyt->Identifier.V1Out)
 begintType(yyt->Identifier.SynType)
 begintType(yyt->Identifier.InhType)
 beginint(yyt->Identifier.W)
 beginint(yyt->Identifier.VIn)
 beginint(yyt->Identifier.VOut)
 beginbool(yyt->Identifier.C)
 begintValue(yyt->Identifier.CV)
 begintConstArray(yyt->Identifier.CA)
 begintUsage(yyt->Identifier.U)
 begintSymbol(yyt->Identifier.Parent)
 begintParUsage(yyt->Identifier.PUIn)
 begintParUsage(yyt->Identifier.PUOut)
 beginbool(yyt->Identifier.LV)
 beginbool(yyt->Identifier.SE)
 begintTree(yyt->Identifier.RVL)
 yyt->Identifier.Sym = pSym;
 yyt->Identifier.Scope = pScope;
 return yyt;
}

tTree mUnmappedIdentifier
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tSymbol pSym, tSymbol pScope, tIdent pIdent)
# else
(pPos, pSym, pScope, pIdent)
tPosition pPos;
tSymbol pSym;
tSymbol pScope;
tIdent pIdent;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUnmappedIdentifier])
 yyt->Kind = kUnmappedIdentifier;
 yyt->yyHead.yyMark = 0;
 yyt->UnmappedIdentifier.Pos = pPos;
 begintType(yyt->UnmappedIdentifier.DefType)
 begintType(yyt->UnmappedIdentifier.FLIn)
 begintType(yyt->UnmappedIdentifier.FLOut)
 beginint(yyt->UnmappedIdentifier.V1In)
 beginint(yyt->UnmappedIdentifier.V1Out)
 begintType(yyt->UnmappedIdentifier.SynType)
 begintType(yyt->UnmappedIdentifier.InhType)
 beginint(yyt->UnmappedIdentifier.W)
 beginint(yyt->UnmappedIdentifier.VIn)
 beginint(yyt->UnmappedIdentifier.VOut)
 beginbool(yyt->UnmappedIdentifier.C)
 begintValue(yyt->UnmappedIdentifier.CV)
 begintConstArray(yyt->UnmappedIdentifier.CA)
 begintUsage(yyt->UnmappedIdentifier.U)
 begintSymbol(yyt->UnmappedIdentifier.Parent)
 begintParUsage(yyt->UnmappedIdentifier.PUIn)
 begintParUsage(yyt->UnmappedIdentifier.PUOut)
 beginbool(yyt->UnmappedIdentifier.LV)
 beginbool(yyt->UnmappedIdentifier.SE)
 begintTree(yyt->UnmappedIdentifier.RVL)
 yyt->UnmappedIdentifier.Sym = pSym;
 yyt->UnmappedIdentifier.Scope = pScope;
 yyt->UnmappedIdentifier.Ident = pIdent;
 return yyt;
}

tTree mElementAccess
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext)
# else
(pPos, pNext)
tPosition pPos;
tTree pNext;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kElementAccess])
 yyt->Kind = kElementAccess;
 yyt->yyHead.yyMark = 0;
 yyt->ElementAccess.Pos = pPos;
 begintType(yyt->ElementAccess.DefType)
 begintType(yyt->ElementAccess.FLIn)
 begintType(yyt->ElementAccess.FLOut)
 beginint(yyt->ElementAccess.V1In)
 beginint(yyt->ElementAccess.V1Out)
 begintType(yyt->ElementAccess.SynType)
 begintType(yyt->ElementAccess.InhType)
 beginint(yyt->ElementAccess.W)
 beginint(yyt->ElementAccess.VIn)
 beginint(yyt->ElementAccess.VOut)
 beginbool(yyt->ElementAccess.C)
 begintValue(yyt->ElementAccess.CV)
 begintConstArray(yyt->ElementAccess.CA)
 begintUsage(yyt->ElementAccess.U)
 begintSymbol(yyt->ElementAccess.Parent)
 begintParUsage(yyt->ElementAccess.PUIn)
 begintParUsage(yyt->ElementAccess.PUOut)
 beginbool(yyt->ElementAccess.LV)
 beginbool(yyt->ElementAccess.SE)
 begintTree(yyt->ElementAccess.RVL)
 yyt->ElementAccess.Next = pNext;
 return yyt;
}

tTree mSubscript
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pExpr)
# else
(pPos, pNext, pExpr)
tPosition pPos;
tTree pNext;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSubscript])
 yyt->Kind = kSubscript;
 yyt->yyHead.yyMark = 0;
 yyt->Subscript.Pos = pPos;
 begintType(yyt->Subscript.DefType)
 begintType(yyt->Subscript.FLIn)
 begintType(yyt->Subscript.FLOut)
 beginint(yyt->Subscript.V1In)
 beginint(yyt->Subscript.V1Out)
 begintType(yyt->Subscript.SynType)
 begintType(yyt->Subscript.InhType)
 beginint(yyt->Subscript.W)
 beginint(yyt->Subscript.VIn)
 beginint(yyt->Subscript.VOut)
 beginbool(yyt->Subscript.C)
 begintValue(yyt->Subscript.CV)
 begintConstArray(yyt->Subscript.CA)
 begintUsage(yyt->Subscript.U)
 begintSymbol(yyt->Subscript.Parent)
 begintParUsage(yyt->Subscript.PUIn)
 begintParUsage(yyt->Subscript.PUOut)
 beginbool(yyt->Subscript.LV)
 beginbool(yyt->Subscript.SE)
 begintTree(yyt->Subscript.RVL)
 yyt->Subscript.Next = pNext;
 yyt->Subscript.Expr = pExpr;
 beginbool(yyt->Subscript.CB)
 beginpLinExpr(yyt->Subscript.LE)
 return yyt;
}

tTree mRange
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pNext, tTree pFrom, tTree pFor)
# else
(pPos, pNext, pFrom, pFor)
tPosition pPos;
tTree pNext;
tTree pFrom;
tTree pFor;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRange])
 yyt->Kind = kRange;
 yyt->yyHead.yyMark = 0;
 yyt->Range.Pos = pPos;
 begintType(yyt->Range.DefType)
 begintType(yyt->Range.FLIn)
 begintType(yyt->Range.FLOut)
 beginint(yyt->Range.V1In)
 beginint(yyt->Range.V1Out)
 begintType(yyt->Range.SynType)
 begintType(yyt->Range.InhType)
 beginint(yyt->Range.W)
 beginint(yyt->Range.VIn)
 beginint(yyt->Range.VOut)
 beginbool(yyt->Range.C)
 begintValue(yyt->Range.CV)
 begintConstArray(yyt->Range.CA)
 begintUsage(yyt->Range.U)
 begintSymbol(yyt->Range.Parent)
 begintParUsage(yyt->Range.PUIn)
 begintParUsage(yyt->Range.PUOut)
 beginbool(yyt->Range.LV)
 beginbool(yyt->Range.SE)
 begintTree(yyt->Range.RVL)
 yyt->Range.Next = pNext;
 yyt->Range.From = pFrom;
 yyt->Range.For = pFor;
 beginbool(yyt->Range.CLB)
 beginbool(yyt->Range.CUB)
 beginpLinExpr(yyt->Range.FromLE)
 beginpLinExpr(yyt->Range.ForLE)
 return yyt;
}

tTree mExprList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kExprList])
 yyt->Kind = kExprList;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->ExprList.DefType)
 begintType(yyt->ExprList.FLIn)
 begintType(yyt->ExprList.FLOut)
 beginint(yyt->ExprList.V1In)
 beginint(yyt->ExprList.V1Out)
 begintType(yyt->ExprList.SynType)
 begintType(yyt->ExprList.InhType)
 beginint(yyt->ExprList.VIn)
 beginint(yyt->ExprList.VOut)
 beginbool(yyt->ExprList.C)
 beginbool(yyt->ExprList.TR)
 begintSymbol(yyt->ExprList.Parent)
 begintParUsage(yyt->ExprList.PUIn)
 begintParUsage(yyt->ExprList.PUOut)
 beginbool(yyt->ExprList.PAR)
 beginbool(yyt->ExprList.LV)
 return yyt;
}

tTree mNoExpr
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoExpr])
 yyt->Kind = kNoExpr;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->NoExpr.DefType)
 begintType(yyt->NoExpr.FLIn)
 begintType(yyt->NoExpr.FLOut)
 beginint(yyt->NoExpr.V1In)
 beginint(yyt->NoExpr.V1Out)
 begintType(yyt->NoExpr.SynType)
 begintType(yyt->NoExpr.InhType)
 beginint(yyt->NoExpr.VIn)
 beginint(yyt->NoExpr.VOut)
 beginbool(yyt->NoExpr.C)
 beginbool(yyt->NoExpr.TR)
 begintSymbol(yyt->NoExpr.Parent)
 begintParUsage(yyt->NoExpr.PUIn)
 begintParUsage(yyt->NoExpr.PUOut)
 beginbool(yyt->NoExpr.PAR)
 beginbool(yyt->NoExpr.LV)
 return yyt;
}

tTree mAnExpr
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pExpr)
# else
(pNext, pExpr)
tTree pNext;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnExpr])
 yyt->Kind = kAnExpr;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->AnExpr.DefType)
 begintType(yyt->AnExpr.FLIn)
 begintType(yyt->AnExpr.FLOut)
 beginint(yyt->AnExpr.V1In)
 beginint(yyt->AnExpr.V1Out)
 begintType(yyt->AnExpr.SynType)
 begintType(yyt->AnExpr.InhType)
 beginint(yyt->AnExpr.VIn)
 beginint(yyt->AnExpr.VOut)
 beginbool(yyt->AnExpr.C)
 beginbool(yyt->AnExpr.TR)
 begintSymbol(yyt->AnExpr.Parent)
 begintParUsage(yyt->AnExpr.PUIn)
 begintParUsage(yyt->AnExpr.PUOut)
 beginbool(yyt->AnExpr.PAR)
 beginbool(yyt->AnExpr.LV)
 yyt->AnExpr.Next = pNext;
 yyt->AnExpr.Expr = pExpr;
 return yyt;
}

tTree mOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kOp])
 yyt->Kind = kOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mRelOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRelOp])
 yyt->Kind = kRelOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mLess
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kLess])
 yyt->Kind = kLess;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mGreater
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kGreater])
 yyt->Kind = kGreater;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mLessEqual
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kLessEqual])
 yyt->Kind = kLessEqual;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mGreaterEqual
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kGreaterEqual])
 yyt->Kind = kGreaterEqual;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mEqual
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kEqual])
 yyt->Kind = kEqual;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mNotEqual
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNotEqual])
 yyt->Kind = kNotEqual;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mMathsOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kMathsOp])
 yyt->Kind = kMathsOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mAdd
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAdd])
 yyt->Kind = kAdd;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mSubtract
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSubtract])
 yyt->Kind = kSubtract;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mMultiply
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kMultiply])
 yyt->Kind = kMultiply;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mDivide
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kDivide])
 yyt->Kind = kDivide;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mRem
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRem])
 yyt->Kind = kRem;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mBitOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBitOp])
 yyt->Kind = kBitOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mBitAnd
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBitAnd])
 yyt->Kind = kBitAnd;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mBitOr
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBitOr])
 yyt->Kind = kBitOr;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mBitEor
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBitEor])
 yyt->Kind = kBitEor;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mShiftOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kShiftOp])
 yyt->Kind = kShiftOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mLeftShift
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kLeftShift])
 yyt->Kind = kLeftShift;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mRightShift
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kRightShift])
 yyt->Kind = kRightShift;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mUniOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kUniOp])
 yyt->Kind = kUniOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mSize
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSize])
 yyt->Kind = kSize;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mBitNot
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kBitNot])
 yyt->Kind = kBitNot;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mNot
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNot])
 yyt->Kind = kNot;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mClockOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kClockOp])
 yyt->Kind = kClockOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mPlus
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kPlus])
 yyt->Kind = kPlus;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mMinus
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kMinus])
 yyt->Kind = kMinus;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mTimes
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kTimes])
 yyt->Kind = kTimes;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mLogOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kLogOp])
 yyt->Kind = kLogOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mAnd
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnd])
 yyt->Kind = kAnd;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mOr
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kOr])
 yyt->Kind = kOr;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mAfterOp
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAfterOp])
 yyt->Kind = kAfterOp;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mProcess
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList)
# else
(pPos, pSpecList)
tPosition pPos;
tTree pSpecList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kProcess])
 yyt->Kind = kProcess;
 yyt->yyHead.yyMark = 0;
 yyt->Process.Pos = pPos;
 yyt->Process.SpecList = pSpecList;
 beginint(yyt->Process.V1In)
 beginint(yyt->Process.V1Out)
 beginint(yyt->Process.VS)
 beginint(yyt->Process.VIn)
 beginint(yyt->Process.VOut)
 beginbool(yyt->Process.ATOMIC)
 beginint(yyt->Process.SLC)
 beginbool(yyt->Process.TLA)
 begintSymbol(yyt->Process.Parent)
 begintParUsage(yyt->Process.PUIn)
 begintParUsage(yyt->Process.PUOut)
 return yyt;
}

tTree mAssign
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pLval, tTree pRval)
# else
(pPos, pSpecList, pLval, pRval)
tPosition pPos;
tTree pSpecList;
tTree pLval;
tTree pRval;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAssign])
 yyt->Kind = kAssign;
 yyt->yyHead.yyMark = 0;
 yyt->Assign.Pos = pPos;
 yyt->Assign.SpecList = pSpecList;
 beginint(yyt->Assign.V1In)
 beginint(yyt->Assign.V1Out)
 beginint(yyt->Assign.VS)
 beginint(yyt->Assign.VIn)
 beginint(yyt->Assign.VOut)
 beginbool(yyt->Assign.ATOMIC)
 beginint(yyt->Assign.SLC)
 beginbool(yyt->Assign.TLA)
 begintSymbol(yyt->Assign.Parent)
 begintParUsage(yyt->Assign.PUIn)
 begintParUsage(yyt->Assign.PUOut)
 yyt->Assign.Lval = pLval;
 yyt->Assign.Rval = pRval;
 return yyt;
}

tTree mAltType
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, bool pPri)
# else
(pPos, pSpecList, pPri)
tPosition pPos;
tTree pSpecList;
bool pPri;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltType])
 yyt->Kind = kAltType;
 yyt->yyHead.yyMark = 0;
 yyt->AltType.Pos = pPos;
 yyt->AltType.SpecList = pSpecList;
 beginint(yyt->AltType.V1In)
 beginint(yyt->AltType.V1Out)
 beginint(yyt->AltType.VS)
 beginint(yyt->AltType.VIn)
 beginint(yyt->AltType.VOut)
 beginbool(yyt->AltType.ATOMIC)
 beginint(yyt->AltType.SLC)
 beginbool(yyt->AltType.TLA)
 begintSymbol(yyt->AltType.Parent)
 begintParUsage(yyt->AltType.PUIn)
 begintParUsage(yyt->AltType.PUOut)
 yyt->AltType.Pri = pPri;
 beginint(yyt->AltType.ARC)
 return yyt;
}

tTree mAlt
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, bool pPri, tTree pAltList)
# else
(pPos, pSpecList, pPri, pAltList)
tPosition pPos;
tTree pSpecList;
bool pPri;
tTree pAltList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAlt])
 yyt->Kind = kAlt;
 yyt->yyHead.yyMark = 0;
 yyt->Alt.Pos = pPos;
 yyt->Alt.SpecList = pSpecList;
 beginint(yyt->Alt.V1In)
 beginint(yyt->Alt.V1Out)
 beginint(yyt->Alt.VS)
 beginint(yyt->Alt.VIn)
 beginint(yyt->Alt.VOut)
 beginbool(yyt->Alt.ATOMIC)
 beginint(yyt->Alt.SLC)
 beginbool(yyt->Alt.TLA)
 begintSymbol(yyt->Alt.Parent)
 begintParUsage(yyt->Alt.PUIn)
 begintParUsage(yyt->Alt.PUOut)
 yyt->Alt.Pri = pPri;
 beginint(yyt->Alt.ARC)
 yyt->Alt.AltList = pAltList;
 return yyt;
}

tTree mAltRep
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, bool pPri, tTree pReplicator, tTree pAltList)
# else
(pPos, pSpecList, pPri, pReplicator, pAltList)
tPosition pPos;
tTree pSpecList;
bool pPri;
tTree pReplicator;
tTree pAltList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltRep])
 yyt->Kind = kAltRep;
 yyt->yyHead.yyMark = 0;
 yyt->AltRep.Pos = pPos;
 yyt->AltRep.SpecList = pSpecList;
 beginint(yyt->AltRep.V1In)
 beginint(yyt->AltRep.V1Out)
 beginint(yyt->AltRep.VS)
 beginint(yyt->AltRep.VIn)
 beginint(yyt->AltRep.VOut)
 beginbool(yyt->AltRep.ATOMIC)
 beginint(yyt->AltRep.SLC)
 beginbool(yyt->AltRep.TLA)
 begintSymbol(yyt->AltRep.Parent)
 begintParUsage(yyt->AltRep.PUIn)
 begintParUsage(yyt->AltRep.PUOut)
 yyt->AltRep.Pri = pPri;
 beginint(yyt->AltRep.ARC)
 yyt->AltRep.Replicator = pReplicator;
 yyt->AltRep.AltList = pAltList;
 return yyt;
}

tTree mParType
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tParType pPType)
# else
(pPos, pSpecList, pPType)
tPosition pPos;
tTree pSpecList;
tParType pPType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kParType])
 yyt->Kind = kParType;
 yyt->yyHead.yyMark = 0;
 yyt->ParType.Pos = pPos;
 yyt->ParType.SpecList = pSpecList;
 beginint(yyt->ParType.V1In)
 beginint(yyt->ParType.V1Out)
 beginint(yyt->ParType.VS)
 beginint(yyt->ParType.VIn)
 beginint(yyt->ParType.VOut)
 beginbool(yyt->ParType.ATOMIC)
 beginint(yyt->ParType.SLC)
 beginbool(yyt->ParType.TLA)
 begintSymbol(yyt->ParType.Parent)
 begintParUsage(yyt->ParType.PUIn)
 begintParUsage(yyt->ParType.PUOut)
 yyt->ParType.PType = pPType;
 return yyt;
}

tTree mPar
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tParType pPType, tTree pProcList)
# else
(pPos, pSpecList, pPType, pProcList)
tPosition pPos;
tTree pSpecList;
tParType pPType;
tTree pProcList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kPar])
 yyt->Kind = kPar;
 yyt->yyHead.yyMark = 0;
 yyt->Par.Pos = pPos;
 yyt->Par.SpecList = pSpecList;
 beginint(yyt->Par.V1In)
 beginint(yyt->Par.V1Out)
 beginint(yyt->Par.VS)
 beginint(yyt->Par.VIn)
 beginint(yyt->Par.VOut)
 beginbool(yyt->Par.ATOMIC)
 beginint(yyt->Par.SLC)
 beginbool(yyt->Par.TLA)
 begintSymbol(yyt->Par.Parent)
 begintParUsage(yyt->Par.PUIn)
 begintParUsage(yyt->Par.PUOut)
 yyt->Par.PType = pPType;
 yyt->Par.ProcList = pProcList;
 return yyt;
}

tTree mParRep
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tParType pPType, tTree pReplicator, tTree pProcess, tSymbol pSym)
# else
(pPos, pSpecList, pPType, pReplicator, pProcess, pSym)
tPosition pPos;
tTree pSpecList;
tParType pPType;
tTree pReplicator;
tTree pProcess;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kParRep])
 yyt->Kind = kParRep;
 yyt->yyHead.yyMark = 0;
 yyt->ParRep.Pos = pPos;
 yyt->ParRep.SpecList = pSpecList;
 beginint(yyt->ParRep.V1In)
 beginint(yyt->ParRep.V1Out)
 beginint(yyt->ParRep.VS)
 beginint(yyt->ParRep.VIn)
 beginint(yyt->ParRep.VOut)
 beginbool(yyt->ParRep.ATOMIC)
 beginint(yyt->ParRep.SLC)
 beginbool(yyt->ParRep.TLA)
 begintSymbol(yyt->ParRep.Parent)
 begintParUsage(yyt->ParRep.PUIn)
 begintParUsage(yyt->ParRep.PUOut)
 yyt->ParRep.PType = pPType;
 yyt->ParRep.Replicator = pReplicator;
 yyt->ParRep.Process = pProcess;
 yyt->ParRep.Sym = pSym;
 return yyt;
}

tTree mProcessor
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pExpr, tIdent pName, tTree pProcess)
# else
(pPos, pSpecList, pExpr, pName, pProcess)
tPosition pPos;
tTree pSpecList;
tTree pExpr;
tIdent pName;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kProcessor])
 yyt->Kind = kProcessor;
 yyt->yyHead.yyMark = 0;
 yyt->Processor.Pos = pPos;
 yyt->Processor.SpecList = pSpecList;
 beginint(yyt->Processor.V1In)
 beginint(yyt->Processor.V1Out)
 beginint(yyt->Processor.VS)
 beginint(yyt->Processor.VIn)
 beginint(yyt->Processor.VOut)
 beginbool(yyt->Processor.ATOMIC)
 beginint(yyt->Processor.SLC)
 beginbool(yyt->Processor.TLA)
 begintSymbol(yyt->Processor.Parent)
 begintParUsage(yyt->Processor.PUIn)
 begintParUsage(yyt->Processor.PUOut)
 yyt->Processor.Expr = pExpr;
 yyt->Processor.Name = pName;
 yyt->Processor.Process = pProcess;
 return yyt;
}

tTree mIfType
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList)
# else
(pPos, pSpecList)
tPosition pPos;
tTree pSpecList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIfType])
 yyt->Kind = kIfType;
 yyt->yyHead.yyMark = 0;
 yyt->IfType.Pos = pPos;
 yyt->IfType.SpecList = pSpecList;
 beginint(yyt->IfType.V1In)
 beginint(yyt->IfType.V1Out)
 beginint(yyt->IfType.VS)
 beginint(yyt->IfType.VIn)
 beginint(yyt->IfType.VOut)
 beginbool(yyt->IfType.ATOMIC)
 beginint(yyt->IfType.SLC)
 beginbool(yyt->IfType.TLA)
 begintSymbol(yyt->IfType.Parent)
 begintParUsage(yyt->IfType.PUIn)
 begintParUsage(yyt->IfType.PUOut)
 begintChoiceList(yyt->IfType.BackChoiceList)
 begintIfType(yyt->IfType.BackIfType)
 return yyt;
}

tTree mIf
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pChoiceList)
# else
(pPos, pSpecList, pChoiceList)
tPosition pPos;
tTree pSpecList;
tTree pChoiceList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIf])
 yyt->Kind = kIf;
 yyt->yyHead.yyMark = 0;
 yyt->If.Pos = pPos;
 yyt->If.SpecList = pSpecList;
 beginint(yyt->If.V1In)
 beginint(yyt->If.V1Out)
 beginint(yyt->If.VS)
 beginint(yyt->If.VIn)
 beginint(yyt->If.VOut)
 beginbool(yyt->If.ATOMIC)
 beginint(yyt->If.SLC)
 beginbool(yyt->If.TLA)
 begintSymbol(yyt->If.Parent)
 begintParUsage(yyt->If.PUIn)
 begintParUsage(yyt->If.PUOut)
 begintChoiceList(yyt->If.BackChoiceList)
 begintIfType(yyt->If.BackIfType)
 yyt->If.ChoiceList = pChoiceList;
 return yyt;
}

tTree mIfRep
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pReplicator, tTree pChoiceList)
# else
(pPos, pSpecList, pReplicator, pChoiceList)
tPosition pPos;
tTree pSpecList;
tTree pReplicator;
tTree pChoiceList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kIfRep])
 yyt->Kind = kIfRep;
 yyt->yyHead.yyMark = 0;
 yyt->IfRep.Pos = pPos;
 yyt->IfRep.SpecList = pSpecList;
 beginint(yyt->IfRep.V1In)
 beginint(yyt->IfRep.V1Out)
 beginint(yyt->IfRep.VS)
 beginint(yyt->IfRep.VIn)
 beginint(yyt->IfRep.VOut)
 beginbool(yyt->IfRep.ATOMIC)
 beginint(yyt->IfRep.SLC)
 beginbool(yyt->IfRep.TLA)
 begintSymbol(yyt->IfRep.Parent)
 begintParUsage(yyt->IfRep.PUIn)
 begintParUsage(yyt->IfRep.PUOut)
 begintChoiceList(yyt->IfRep.BackChoiceList)
 begintIfType(yyt->IfRep.BackIfType)
 yyt->IfRep.Replicator = pReplicator;
 yyt->IfRep.ChoiceList = pChoiceList;
 return yyt;
}

tTree mInput
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pElement, tTree pExprList)
# else
(pPos, pSpecList, pElement, pExprList)
tPosition pPos;
tTree pSpecList;
tTree pElement;
tTree pExprList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInput])
 yyt->Kind = kInput;
 yyt->yyHead.yyMark = 0;
 yyt->Input.Pos = pPos;
 yyt->Input.SpecList = pSpecList;
 beginint(yyt->Input.V1In)
 beginint(yyt->Input.V1Out)
 beginint(yyt->Input.VS)
 beginint(yyt->Input.VIn)
 beginint(yyt->Input.VOut)
 beginbool(yyt->Input.ATOMIC)
 beginint(yyt->Input.SLC)
 beginbool(yyt->Input.TLA)
 begintSymbol(yyt->Input.Parent)
 begintParUsage(yyt->Input.PUIn)
 begintParUsage(yyt->Input.PUOut)
 yyt->Input.Element = pElement;
 yyt->Input.ExprList = pExprList;
 return yyt;
}

tTree mOutput
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pElement, tTree pExprList)
# else
(pPos, pSpecList, pElement, pExprList)
tPosition pPos;
tTree pSpecList;
tTree pElement;
tTree pExprList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kOutput])
 yyt->Kind = kOutput;
 yyt->yyHead.yyMark = 0;
 yyt->Output.Pos = pPos;
 yyt->Output.SpecList = pSpecList;
 beginint(yyt->Output.V1In)
 beginint(yyt->Output.V1Out)
 beginint(yyt->Output.VS)
 beginint(yyt->Output.VIn)
 beginint(yyt->Output.VOut)
 beginbool(yyt->Output.ATOMIC)
 beginint(yyt->Output.SLC)
 beginbool(yyt->Output.TLA)
 begintSymbol(yyt->Output.Parent)
 begintParUsage(yyt->Output.PUIn)
 begintParUsage(yyt->Output.PUOut)
 yyt->Output.Element = pElement;
 yyt->Output.ExprList = pExprList;
 return yyt;
}

tTree mCaseIn
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pElement, tTree pCaseList)
# else
(pPos, pSpecList, pElement, pCaseList)
tPosition pPos;
tTree pSpecList;
tTree pElement;
tTree pCaseList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCaseIn])
 yyt->Kind = kCaseIn;
 yyt->yyHead.yyMark = 0;
 yyt->CaseIn.Pos = pPos;
 yyt->CaseIn.SpecList = pSpecList;
 beginint(yyt->CaseIn.V1In)
 beginint(yyt->CaseIn.V1Out)
 beginint(yyt->CaseIn.VS)
 beginint(yyt->CaseIn.VIn)
 beginint(yyt->CaseIn.VOut)
 beginbool(yyt->CaseIn.ATOMIC)
 beginint(yyt->CaseIn.SLC)
 beginbool(yyt->CaseIn.TLA)
 begintSymbol(yyt->CaseIn.Parent)
 begintParUsage(yyt->CaseIn.PUIn)
 begintParUsage(yyt->CaseIn.PUOut)
 yyt->CaseIn.Element = pElement;
 yyt->CaseIn.CaseList = pCaseList;
 return yyt;
}

tTree mAfter
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pElement, tTree pExpr)
# else
(pPos, pSpecList, pElement, pExpr)
tPosition pPos;
tTree pSpecList;
tTree pElement;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAfter])
 yyt->Kind = kAfter;
 yyt->yyHead.yyMark = 0;
 yyt->After.Pos = pPos;
 yyt->After.SpecList = pSpecList;
 beginint(yyt->After.V1In)
 beginint(yyt->After.V1Out)
 beginint(yyt->After.VS)
 beginint(yyt->After.VIn)
 beginint(yyt->After.VOut)
 beginbool(yyt->After.ATOMIC)
 beginint(yyt->After.SLC)
 beginbool(yyt->After.TLA)
 begintSymbol(yyt->After.Parent)
 begintParUsage(yyt->After.PUIn)
 begintParUsage(yyt->After.PUOut)
 yyt->After.Element = pElement;
 yyt->After.Expr = pExpr;
 return yyt;
}

tTree mSeqRep
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pReplicator, tTree pProcess)
# else
(pPos, pSpecList, pReplicator, pProcess)
tPosition pPos;
tTree pSpecList;
tTree pReplicator;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSeqRep])
 yyt->Kind = kSeqRep;
 yyt->yyHead.yyMark = 0;
 yyt->SeqRep.Pos = pPos;
 yyt->SeqRep.SpecList = pSpecList;
 beginint(yyt->SeqRep.V1In)
 beginint(yyt->SeqRep.V1Out)
 beginint(yyt->SeqRep.VS)
 beginint(yyt->SeqRep.VIn)
 beginint(yyt->SeqRep.VOut)
 beginbool(yyt->SeqRep.ATOMIC)
 beginint(yyt->SeqRep.SLC)
 beginbool(yyt->SeqRep.TLA)
 begintSymbol(yyt->SeqRep.Parent)
 begintParUsage(yyt->SeqRep.PUIn)
 begintParUsage(yyt->SeqRep.PUOut)
 yyt->SeqRep.Replicator = pReplicator;
 yyt->SeqRep.Process = pProcess;
 return yyt;
}

tTree mSeq
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pProcList)
# else
(pPos, pSpecList, pProcList)
tPosition pPos;
tTree pSpecList;
tTree pProcList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSeq])
 yyt->Kind = kSeq;
 yyt->yyHead.yyMark = 0;
 yyt->Seq.Pos = pPos;
 yyt->Seq.SpecList = pSpecList;
 beginint(yyt->Seq.V1In)
 beginint(yyt->Seq.V1Out)
 beginint(yyt->Seq.VS)
 beginint(yyt->Seq.VIn)
 beginint(yyt->Seq.VOut)
 beginbool(yyt->Seq.ATOMIC)
 beginint(yyt->Seq.SLC)
 beginbool(yyt->Seq.TLA)
 begintSymbol(yyt->Seq.Parent)
 begintParUsage(yyt->Seq.PUIn)
 begintParUsage(yyt->Seq.PUOut)
 yyt->Seq.ProcList = pProcList;
 return yyt;
}

tTree mCase
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pSelector, tTree pChoiceList)
# else
(pPos, pSpecList, pSelector, pChoiceList)
tPosition pPos;
tTree pSpecList;
tTree pSelector;
tTree pChoiceList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCase])
 yyt->Kind = kCase;
 yyt->yyHead.yyMark = 0;
 yyt->Case.Pos = pPos;
 yyt->Case.SpecList = pSpecList;
 beginint(yyt->Case.V1In)
 beginint(yyt->Case.V1Out)
 beginint(yyt->Case.VS)
 beginint(yyt->Case.VIn)
 beginint(yyt->Case.VOut)
 beginbool(yyt->Case.ATOMIC)
 beginint(yyt->Case.SLC)
 beginbool(yyt->Case.TLA)
 begintSymbol(yyt->Case.Parent)
 begintParUsage(yyt->Case.PUIn)
 begintParUsage(yyt->Case.PUOut)
 yyt->Case.Selector = pSelector;
 yyt->Case.ChoiceList = pChoiceList;
 return yyt;
}

tTree mWhile
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tTree pExpr, tTree pProcess)
# else
(pPos, pSpecList, pExpr, pProcess)
tPosition pPos;
tTree pSpecList;
tTree pExpr;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kWhile])
 yyt->Kind = kWhile;
 yyt->yyHead.yyMark = 0;
 yyt->While.Pos = pPos;
 yyt->While.SpecList = pSpecList;
 beginint(yyt->While.V1In)
 beginint(yyt->While.V1Out)
 beginint(yyt->While.VS)
 beginint(yyt->While.VIn)
 beginint(yyt->While.VOut)
 beginbool(yyt->While.ATOMIC)
 beginint(yyt->While.SLC)
 beginbool(yyt->While.TLA)
 begintSymbol(yyt->While.Parent)
 begintParUsage(yyt->While.PUIn)
 begintParUsage(yyt->While.PUOut)
 yyt->While.Expr = pExpr;
 yyt->While.Process = pProcess;
 return yyt;
}

tTree mSkip
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList)
# else
(pPos, pSpecList)
tPosition pPos;
tTree pSpecList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kSkip])
 yyt->Kind = kSkip;
 yyt->yyHead.yyMark = 0;
 yyt->Skip.Pos = pPos;
 yyt->Skip.SpecList = pSpecList;
 beginint(yyt->Skip.V1In)
 beginint(yyt->Skip.V1Out)
 beginint(yyt->Skip.VS)
 beginint(yyt->Skip.VIn)
 beginint(yyt->Skip.VOut)
 beginbool(yyt->Skip.ATOMIC)
 beginint(yyt->Skip.SLC)
 beginbool(yyt->Skip.TLA)
 begintSymbol(yyt->Skip.Parent)
 begintParUsage(yyt->Skip.PUIn)
 begintParUsage(yyt->Skip.PUOut)
 return yyt;
}

tTree mStop
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList)
# else
(pPos, pSpecList)
tPosition pPos;
tTree pSpecList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kStop])
 yyt->Kind = kStop;
 yyt->yyHead.yyMark = 0;
 yyt->Stop.Pos = pPos;
 yyt->Stop.SpecList = pSpecList;
 beginint(yyt->Stop.V1In)
 beginint(yyt->Stop.V1Out)
 beginint(yyt->Stop.VS)
 beginint(yyt->Stop.VIn)
 beginint(yyt->Stop.VOut)
 beginbool(yyt->Stop.ATOMIC)
 beginint(yyt->Stop.SLC)
 beginbool(yyt->Stop.TLA)
 begintSymbol(yyt->Stop.Parent)
 begintParUsage(yyt->Stop.PUIn)
 begintParUsage(yyt->Stop.PUOut)
 return yyt;
}

tTree mInstance
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tSymbol pSym, tTree pExprList, tSymbol pScope)
# else
(pPos, pSpecList, pSym, pExprList, pScope)
tPosition pPos;
tTree pSpecList;
tSymbol pSym;
tTree pExprList;
tSymbol pScope;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kInstance])
 yyt->Kind = kInstance;
 yyt->yyHead.yyMark = 0;
 yyt->Instance.Pos = pPos;
 yyt->Instance.SpecList = pSpecList;
 beginint(yyt->Instance.V1In)
 beginint(yyt->Instance.V1Out)
 beginint(yyt->Instance.VS)
 beginint(yyt->Instance.VIn)
 beginint(yyt->Instance.VOut)
 beginbool(yyt->Instance.ATOMIC)
 beginint(yyt->Instance.SLC)
 beginbool(yyt->Instance.TLA)
 begintSymbol(yyt->Instance.Parent)
 begintParUsage(yyt->Instance.PUIn)
 begintParUsage(yyt->Instance.PUOut)
 yyt->Instance.Sym = pSym;
 yyt->Instance.ExprList = pExprList;
 yyt->Instance.Scope = pScope;
 begintFrame(yyt->Instance.Frame)
 return yyt;
}

tTree mCCode
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList, tStringRef pCode, tSymbol pScope)
# else
(pPos, pSpecList, pCode, pScope)
tPosition pPos;
tTree pSpecList;
tStringRef pCode;
tSymbol pScope;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCCode])
 yyt->Kind = kCCode;
 yyt->yyHead.yyMark = 0;
 yyt->CCode.Pos = pPos;
 yyt->CCode.SpecList = pSpecList;
 beginint(yyt->CCode.V1In)
 beginint(yyt->CCode.V1Out)
 beginint(yyt->CCode.VS)
 beginint(yyt->CCode.VIn)
 beginint(yyt->CCode.VOut)
 beginbool(yyt->CCode.ATOMIC)
 beginint(yyt->CCode.SLC)
 beginbool(yyt->CCode.TLA)
 begintSymbol(yyt->CCode.Parent)
 begintParUsage(yyt->CCode.PUIn)
 begintParUsage(yyt->CCode.PUOut)
 yyt->CCode.Code = pCode;
 yyt->CCode.Scope = pScope;
 return yyt;
}

tTree mNullProcess
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pSpecList)
# else
(pPos, pSpecList)
tPosition pPos;
tTree pSpecList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNullProcess])
 yyt->Kind = kNullProcess;
 yyt->yyHead.yyMark = 0;
 yyt->NullProcess.Pos = pPos;
 yyt->NullProcess.SpecList = pSpecList;
 beginint(yyt->NullProcess.V1In)
 beginint(yyt->NullProcess.V1Out)
 beginint(yyt->NullProcess.VS)
 beginint(yyt->NullProcess.VIn)
 beginint(yyt->NullProcess.VOut)
 beginbool(yyt->NullProcess.ATOMIC)
 beginint(yyt->NullProcess.SLC)
 beginbool(yyt->NullProcess.TLA)
 begintSymbol(yyt->NullProcess.Parent)
 begintParUsage(yyt->NullProcess.PUIn)
 begintParUsage(yyt->NullProcess.PUOut)
 return yyt;
}

tTree mAltList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltList])
 yyt->Kind = kAltList;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AltList.V1In)
 beginint(yyt->AltList.V1Out)
 beginint(yyt->AltList.VS)
 beginint(yyt->AltList.VIn)
 beginint(yyt->AltList.VOut)
 beginint(yyt->AltList.SLC)
 beginint(yyt->AltList.ARC)
 begintSymbol(yyt->AltList.Parent)
 begintParUsage(yyt->AltList.PUIn)
 begintParUsage(yyt->AltList.PUOut)
 return yyt;
}

tTree mNoAlt
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoAlt])
 yyt->Kind = kNoAlt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoAlt.V1In)
 beginint(yyt->NoAlt.V1Out)
 beginint(yyt->NoAlt.VS)
 beginint(yyt->NoAlt.VIn)
 beginint(yyt->NoAlt.VOut)
 beginint(yyt->NoAlt.SLC)
 beginint(yyt->NoAlt.ARC)
 begintSymbol(yyt->NoAlt.Parent)
 begintParUsage(yyt->NoAlt.PUIn)
 begintParUsage(yyt->NoAlt.PUOut)
 return yyt;
}

tTree mAnAlts
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext)
# else
(pSpecList, pNext)
tTree pSpecList;
tTree pNext;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnAlts])
 yyt->Kind = kAnAlts;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AnAlts.V1In)
 beginint(yyt->AnAlts.V1Out)
 beginint(yyt->AnAlts.VS)
 beginint(yyt->AnAlts.VIn)
 beginint(yyt->AnAlts.VOut)
 beginint(yyt->AnAlts.SLC)
 beginint(yyt->AnAlts.ARC)
 begintSymbol(yyt->AnAlts.Parent)
 begintParUsage(yyt->AnAlts.PUIn)
 begintParUsage(yyt->AnAlts.PUOut)
 yyt->AnAlts.SpecList = pSpecList;
 yyt->AnAlts.Next = pNext;
 return yyt;
}

tTree mAnAlt
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext, tTree pGuard)
# else
(pSpecList, pNext, pGuard)
tTree pSpecList;
tTree pNext;
tTree pGuard;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnAlt])
 yyt->Kind = kAnAlt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AnAlt.V1In)
 beginint(yyt->AnAlt.V1Out)
 beginint(yyt->AnAlt.VS)
 beginint(yyt->AnAlt.VIn)
 beginint(yyt->AnAlt.VOut)
 beginint(yyt->AnAlt.SLC)
 beginint(yyt->AnAlt.ARC)
 begintSymbol(yyt->AnAlt.Parent)
 begintParUsage(yyt->AnAlt.PUIn)
 begintParUsage(yyt->AnAlt.PUOut)
 yyt->AnAlt.SpecList = pSpecList;
 yyt->AnAlt.Next = pNext;
 yyt->AnAlt.Guard = pGuard;
 beginint(yyt->AnAlt.Label)
 return yyt;
}

tTree mANewAlt
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext, tTree pAltType)
# else
(pSpecList, pNext, pAltType)
tTree pSpecList;
tTree pNext;
tTree pAltType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kANewAlt])
 yyt->Kind = kANewAlt;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->ANewAlt.V1In)
 beginint(yyt->ANewAlt.V1Out)
 beginint(yyt->ANewAlt.VS)
 beginint(yyt->ANewAlt.VIn)
 beginint(yyt->ANewAlt.VOut)
 beginint(yyt->ANewAlt.SLC)
 beginint(yyt->ANewAlt.ARC)
 begintSymbol(yyt->ANewAlt.Parent)
 begintParUsage(yyt->ANewAlt.PUIn)
 begintParUsage(yyt->ANewAlt.PUOut)
 yyt->ANewAlt.SpecList = pSpecList;
 yyt->ANewAlt.Next = pNext;
 yyt->ANewAlt.AltType = pAltType;
 return yyt;
}

tTree mGuard
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr)
# else
(pPos, pExpr)
tPosition pPos;
tTree pExpr;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kGuard])
 yyt->Kind = kGuard;
 yyt->yyHead.yyMark = 0;
 yyt->Guard.Pos = pPos;
 yyt->Guard.Expr = pExpr;
 beginint(yyt->Guard.V1In)
 beginint(yyt->Guard.V1Out)
 beginint(yyt->Guard.VS)
 beginint(yyt->Guard.VIn)
 beginint(yyt->Guard.VOut)
 beginint(yyt->Guard.SLC)
 begintSymbol(yyt->Guard.Parent)
 begintParUsage(yyt->Guard.PUIn)
 begintParUsage(yyt->Guard.PUOut)
 return yyt;
}

tTree mAltInput
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tTree pInput, tTree pProcess)
# else
(pPos, pExpr, pInput, pProcess)
tPosition pPos;
tTree pExpr;
tTree pInput;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltInput])
 yyt->Kind = kAltInput;
 yyt->yyHead.yyMark = 0;
 yyt->AltInput.Pos = pPos;
 yyt->AltInput.Expr = pExpr;
 beginint(yyt->AltInput.V1In)
 beginint(yyt->AltInput.V1Out)
 beginint(yyt->AltInput.VS)
 beginint(yyt->AltInput.VIn)
 beginint(yyt->AltInput.VOut)
 beginint(yyt->AltInput.SLC)
 begintSymbol(yyt->AltInput.Parent)
 begintParUsage(yyt->AltInput.PUIn)
 begintParUsage(yyt->AltInput.PUOut)
 yyt->AltInput.Input = pInput;
 yyt->AltInput.Process = pProcess;
 return yyt;
}

tTree mAltTimer
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tTree pAfter, tTree pProcess)
# else
(pPos, pExpr, pAfter, pProcess)
tPosition pPos;
tTree pExpr;
tTree pAfter;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltTimer])
 yyt->Kind = kAltTimer;
 yyt->yyHead.yyMark = 0;
 yyt->AltTimer.Pos = pPos;
 yyt->AltTimer.Expr = pExpr;
 beginint(yyt->AltTimer.V1In)
 beginint(yyt->AltTimer.V1Out)
 beginint(yyt->AltTimer.VS)
 beginint(yyt->AltTimer.VIn)
 beginint(yyt->AltTimer.VOut)
 beginint(yyt->AltTimer.SLC)
 begintSymbol(yyt->AltTimer.Parent)
 begintParUsage(yyt->AltTimer.PUIn)
 begintParUsage(yyt->AltTimer.PUOut)
 yyt->AltTimer.After = pAfter;
 yyt->AltTimer.Process = pProcess;
 return yyt;
}

tTree mAltCaseIn
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tTree pCaseIn)
# else
(pPos, pExpr, pCaseIn)
tPosition pPos;
tTree pExpr;
tTree pCaseIn;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAltCaseIn])
 yyt->Kind = kAltCaseIn;
 yyt->yyHead.yyMark = 0;
 yyt->AltCaseIn.Pos = pPos;
 yyt->AltCaseIn.Expr = pExpr;
 beginint(yyt->AltCaseIn.V1In)
 beginint(yyt->AltCaseIn.V1Out)
 beginint(yyt->AltCaseIn.VS)
 beginint(yyt->AltCaseIn.VIn)
 beginint(yyt->AltCaseIn.VOut)
 beginint(yyt->AltCaseIn.SLC)
 begintSymbol(yyt->AltCaseIn.Parent)
 begintParUsage(yyt->AltCaseIn.PUIn)
 begintParUsage(yyt->AltCaseIn.PUOut)
 yyt->AltCaseIn.CaseIn = pCaseIn;
 return yyt;
}

tTree mNoInput
# if defined __STDC__ | defined __cplusplus
(tPosition pPos, tTree pExpr, tTree pProcess)
# else
(pPos, pExpr, pProcess)
tPosition pPos;
tTree pExpr;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoInput])
 yyt->Kind = kNoInput;
 yyt->yyHead.yyMark = 0;
 yyt->NoInput.Pos = pPos;
 yyt->NoInput.Expr = pExpr;
 beginint(yyt->NoInput.V1In)
 beginint(yyt->NoInput.V1Out)
 beginint(yyt->NoInput.VS)
 beginint(yyt->NoInput.VIn)
 beginint(yyt->NoInput.VOut)
 beginint(yyt->NoInput.SLC)
 begintSymbol(yyt->NoInput.Parent)
 begintParUsage(yyt->NoInput.PUIn)
 begintParUsage(yyt->NoInput.PUOut)
 yyt->NoInput.Process = pProcess;
 return yyt;
}

tTree mChoiceList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kChoiceList])
 yyt->Kind = kChoiceList;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->ChoiceList.DefType)
 beginint(yyt->ChoiceList.V1In)
 beginint(yyt->ChoiceList.V1Out)
 begintType(yyt->ChoiceList.SynType)
 begintType(yyt->ChoiceList.InhType)
 beginint(yyt->ChoiceList.VS)
 beginint(yyt->ChoiceList.VIn)
 beginint(yyt->ChoiceList.VOut)
 beginbool(yyt->ChoiceList.ATOMIC)
 beginint(yyt->ChoiceList.SLC)
 begintSymbol(yyt->ChoiceList.Parent)
 begintParUsage(yyt->ChoiceList.PUIn)
 begintParUsage(yyt->ChoiceList.PUOut)
 return yyt;
}

tTree mNoChoice
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoChoice])
 yyt->Kind = kNoChoice;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->NoChoice.DefType)
 beginint(yyt->NoChoice.V1In)
 beginint(yyt->NoChoice.V1Out)
 begintType(yyt->NoChoice.SynType)
 begintType(yyt->NoChoice.InhType)
 beginint(yyt->NoChoice.VS)
 beginint(yyt->NoChoice.VIn)
 beginint(yyt->NoChoice.VOut)
 beginbool(yyt->NoChoice.ATOMIC)
 beginint(yyt->NoChoice.SLC)
 begintSymbol(yyt->NoChoice.Parent)
 begintParUsage(yyt->NoChoice.PUIn)
 begintParUsage(yyt->NoChoice.PUOut)
 return yyt;
}

tTree mAChoices
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext)
# else
(pSpecList, pNext)
tTree pSpecList;
tTree pNext;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAChoices])
 yyt->Kind = kAChoices;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->AChoices.DefType)
 beginint(yyt->AChoices.V1In)
 beginint(yyt->AChoices.V1Out)
 begintType(yyt->AChoices.SynType)
 begintType(yyt->AChoices.InhType)
 beginint(yyt->AChoices.VS)
 beginint(yyt->AChoices.VIn)
 beginint(yyt->AChoices.VOut)
 beginbool(yyt->AChoices.ATOMIC)
 beginint(yyt->AChoices.SLC)
 begintSymbol(yyt->AChoices.Parent)
 begintParUsage(yyt->AChoices.PUIn)
 begintParUsage(yyt->AChoices.PUOut)
 yyt->AChoices.SpecList = pSpecList;
 yyt->AChoices.Next = pNext;
 return yyt;
}

tTree mAChoice
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext, tTree pExprList, tTree pProcess)
# else
(pSpecList, pNext, pExprList, pProcess)
tTree pSpecList;
tTree pNext;
tTree pExprList;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAChoice])
 yyt->Kind = kAChoice;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->AChoice.DefType)
 beginint(yyt->AChoice.V1In)
 beginint(yyt->AChoice.V1Out)
 begintType(yyt->AChoice.SynType)
 begintType(yyt->AChoice.InhType)
 beginint(yyt->AChoice.VS)
 beginint(yyt->AChoice.VIn)
 beginint(yyt->AChoice.VOut)
 beginbool(yyt->AChoice.ATOMIC)
 beginint(yyt->AChoice.SLC)
 begintSymbol(yyt->AChoice.Parent)
 begintParUsage(yyt->AChoice.PUIn)
 begintParUsage(yyt->AChoice.PUOut)
 yyt->AChoice.SpecList = pSpecList;
 yyt->AChoice.Next = pNext;
 yyt->AChoice.ExprList = pExprList;
 yyt->AChoice.Process = pProcess;
 beginint(yyt->AChoice.Label)
 return yyt;
}

tTree mANewIf
# if defined __STDC__ | defined __cplusplus
(tTree pSpecList, tTree pNext, tTree pIfType)
# else
(pSpecList, pNext, pIfType)
tTree pSpecList;
tTree pNext;
tTree pIfType;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kANewIf])
 yyt->Kind = kANewIf;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->ANewIf.DefType)
 beginint(yyt->ANewIf.V1In)
 beginint(yyt->ANewIf.V1Out)
 begintType(yyt->ANewIf.SynType)
 begintType(yyt->ANewIf.InhType)
 beginint(yyt->ANewIf.VS)
 beginint(yyt->ANewIf.VIn)
 beginint(yyt->ANewIf.VOut)
 beginbool(yyt->ANewIf.ATOMIC)
 beginint(yyt->ANewIf.SLC)
 begintSymbol(yyt->ANewIf.Parent)
 begintParUsage(yyt->ANewIf.PUIn)
 begintParUsage(yyt->ANewIf.PUOut)
 yyt->ANewIf.SpecList = pSpecList;
 yyt->ANewIf.Next = pNext;
 yyt->ANewIf.IfType = pIfType;
 return yyt;
}

tTree mProcList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kProcList])
 yyt->Kind = kProcList;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->ProcList.V1In)
 beginint(yyt->ProcList.V1Out)
 beginint(yyt->ProcList.VS)
 beginint(yyt->ProcList.VIn)
 beginint(yyt->ProcList.VOut)
 beginbool(yyt->ProcList.ATOMIC)
 beginint(yyt->ProcList.SLC)
 begintSymbol(yyt->ProcList.Parent)
 begintParUsage(yyt->ProcList.PUIn)
 begintParUsage(yyt->ProcList.PUOut)
 beginbool(yyt->ProcList.PAR)
 begintPosition(yyt->ProcList.Pos)
 return yyt;
}

tTree mNoProcess
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoProcess])
 yyt->Kind = kNoProcess;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->NoProcess.V1In)
 beginint(yyt->NoProcess.V1Out)
 beginint(yyt->NoProcess.VS)
 beginint(yyt->NoProcess.VIn)
 beginint(yyt->NoProcess.VOut)
 beginbool(yyt->NoProcess.ATOMIC)
 beginint(yyt->NoProcess.SLC)
 begintSymbol(yyt->NoProcess.Parent)
 begintParUsage(yyt->NoProcess.PUIn)
 begintParUsage(yyt->NoProcess.PUOut)
 beginbool(yyt->NoProcess.PAR)
 begintPosition(yyt->NoProcess.Pos)
 return yyt;
}

tTree mAProcess
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pProcess, tSymbol pSym)
# else
(pNext, pProcess, pSym)
tTree pNext;
tTree pProcess;
tSymbol pSym;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAProcess])
 yyt->Kind = kAProcess;
 yyt->yyHead.yyMark = 0;
 beginint(yyt->AProcess.V1In)
 beginint(yyt->AProcess.V1Out)
 beginint(yyt->AProcess.VS)
 beginint(yyt->AProcess.VIn)
 beginint(yyt->AProcess.VOut)
 beginbool(yyt->AProcess.ATOMIC)
 beginint(yyt->AProcess.SLC)
 begintSymbol(yyt->AProcess.Parent)
 begintParUsage(yyt->AProcess.PUIn)
 begintParUsage(yyt->AProcess.PUOut)
 beginbool(yyt->AProcess.PAR)
 begintPosition(yyt->AProcess.Pos)
 yyt->AProcess.Next = pNext;
 yyt->AProcess.Process = pProcess;
 yyt->AProcess.Sym = pSym;
 return yyt;
}

tTree mCaseList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kCaseList])
 yyt->Kind = kCaseList;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->CaseList.FLIn)
 begintType(yyt->CaseList.FLOut)
 beginint(yyt->CaseList.V1In)
 beginint(yyt->CaseList.V1Out)
 begintType(yyt->CaseList.InhType)
 beginint(yyt->CaseList.VS)
 beginint(yyt->CaseList.VIn)
 beginint(yyt->CaseList.VOut)
 beginint(yyt->CaseList.SLC)
 begintSymbol(yyt->CaseList.Parent)
 begintParUsage(yyt->CaseList.PUIn)
 begintParUsage(yyt->CaseList.PUOut)
 return yyt;
}

tTree mNoCase
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoCase])
 yyt->Kind = kNoCase;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->NoCase.FLIn)
 begintType(yyt->NoCase.FLOut)
 beginint(yyt->NoCase.V1In)
 beginint(yyt->NoCase.V1Out)
 begintType(yyt->NoCase.InhType)
 beginint(yyt->NoCase.VS)
 beginint(yyt->NoCase.VIn)
 beginint(yyt->NoCase.VOut)
 beginint(yyt->NoCase.SLC)
 begintSymbol(yyt->NoCase.Parent)
 begintParUsage(yyt->NoCase.PUIn)
 begintParUsage(yyt->NoCase.PUOut)
 return yyt;
}

tTree mACase
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pSpecList, tTree pExprList, tTree pProcess)
# else
(pNext, pSpecList, pExprList, pProcess)
tTree pNext;
tTree pSpecList;
tTree pExprList;
tTree pProcess;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kACase])
 yyt->Kind = kACase;
 yyt->yyHead.yyMark = 0;
 begintType(yyt->ACase.FLIn)
 begintType(yyt->ACase.FLOut)
 beginint(yyt->ACase.V1In)
 beginint(yyt->ACase.V1Out)
 begintType(yyt->ACase.InhType)
 beginint(yyt->ACase.VS)
 beginint(yyt->ACase.VIn)
 beginint(yyt->ACase.VOut)
 beginint(yyt->ACase.SLC)
 begintSymbol(yyt->ACase.Parent)
 begintParUsage(yyt->ACase.PUIn)
 begintParUsage(yyt->ACase.PUOut)
 yyt->ACase.Next = pNext;
 yyt->ACase.SpecList = pSpecList;
 yyt->ACase.ExprList = pExprList;
 yyt->ACase.Process = pProcess;
 beginint(yyt->ACase.Label)
 return yyt;
}

tTree mAccessList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAccessList])
 yyt->Kind = kAccessList;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mAnAccess
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tSymbol pSym, tTree pElementList)
# else
(pNext, pSym, pElementList)
tTree pNext;
tSymbol pSym;
tTree pElementList;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnAccess])
 yyt->Kind = kAnAccess;
 yyt->yyHead.yyMark = 0;
 yyt->AnAccess.Next = pNext;
 yyt->AnAccess.Sym = pSym;
 yyt->AnAccess.ElementList = pElementList;
 return yyt;
}

tTree mNoAccess
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoAccess])
 yyt->Kind = kNoAccess;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mElementList
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kElementList])
 yyt->Kind = kElementList;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

tTree mAnElement
# if defined __STDC__ | defined __cplusplus
(tTree pNext, tTree pElement)
# else
(pNext, pElement)
tTree pNext;
tTree pElement;
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kAnElement])
 yyt->Kind = kAnElement;
 yyt->yyHead.yyMark = 0;
 yyt->AnElement.Next = pNext;
 yyt->AnElement.Element = pElement;
 return yyt;
}

tTree mNoElement
# if defined __STDC__ | defined __cplusplus
()
# else
()
# endif
{
 register tTree yyt;
 yyALLOC (yyt, Tree_NodeSize [kNoElement])
 yyt->Kind = kNoElement;
 yyt->yyHead.yyMark = 0;
 return yyt;
}

typedef tTree * yyPtrtTree;

extern FILE *yyf;

static void yyMark
# if defined __STDC__ | defined __cplusplus
 (register tTree yyt)
# else
 (yyt) register tTree yyt;
# endif
{
 for (;;) {
  if (yyt == NoTree || ++ yyt->yyHead.yyMark > 1) return;

  switch (yyt->Kind) {
case kStart:
yyt = yyt->Start.Process; break;
case kASpec:
yyMark (yyt->ASpec.Spec);
yyt = yyt->ASpec.Next; break;
case kDeclType:
yyt = yyt->DeclType.Type; break;
case kDecl:
yyMark (yyt->Decl.Type);
yyt = yyt->Decl.IdList; break;
case kIs:
yyMark (yyt->Is.Type);
yyt = yyt->Is.Elemental; break;
case kAVal:
yyMark (yyt->AVal.Type);
yyt = yyt->AVal.Expr; break;
case kVal:
yyMark (yyt->Val.Type);
yyt = yyt->Val.Expr; break;
case kValRetypes:
yyMark (yyt->ValRetypes.Type);
yyt = yyt->ValRetypes.Expr; break;
case kValReshapes:
yyMark (yyt->ValReshapes.Type);
yyt = yyt->ValReshapes.Expr; break;
case kRetypes:
yyMark (yyt->Retypes.Type);
yyt = yyt->Retypes.Elemental; break;
case kReshapes:
yyMark (yyt->Reshapes.Type);
yyt = yyt->Reshapes.Elemental; break;
case kDefSeqPro:
yyt = yyt->DefSeqPro.SeqProList; break;
case kDefTagPro:
yyt = yyt->DefTagPro.TagProList; break;
case kDefAProc:
yyt = yyt->DefAProc.Formals; break;
case kDefProc:
yyMark (yyt->DefProc.Formals);
yyt = yyt->DefProc.Process; break;
case kInlineDefProc:
yyMark (yyt->InlineDefProc.Formals);
yyt = yyt->InlineDefProc.Process; break;
case kPrototypeProc:
yyt = yyt->PrototypeProc.Formals; break;
case kDefAFunc:
yyMark (yyt->DefAFunc.TypeList);
yyt = yyt->DefAFunc.Formals; break;
case kDefFunc:
yyMark (yyt->DefFunc.TypeList);
yyMark (yyt->DefFunc.Formals);
yyt = yyt->DefFunc.Valof; break;
case kInlineDefFunc:
yyMark (yyt->InlineDefFunc.TypeList);
yyMark (yyt->InlineDefFunc.Formals);
yyt = yyt->InlineDefFunc.Valof; break;
case kDefIsFunc:
yyMark (yyt->DefIsFunc.TypeList);
yyMark (yyt->DefIsFunc.Formals);
yyt = yyt->DefIsFunc.ExprList; break;
case kInlineDefIsFunc:
yyMark (yyt->InlineDefIsFunc.TypeList);
yyMark (yyt->InlineDefIsFunc.Formals);
yyt = yyt->InlineDefIsFunc.ExprList; break;
case kPrototypeFunc:
yyMark (yyt->PrototypeFunc.TypeList);
yyt = yyt->PrototypeFunc.Formals; break;
case kAFormal:
yyMark (yyt->AFormal.Next);
yyMark (yyt->AFormal.By);
yyt = yyt->AFormal.Type; break;
case kReplicator:
yyMark (yyt->Replicator.From);
yyt = yyt->Replicator.For; break;
case kATagPro:
yyMark (yyt->ATagPro.SeqProList);
yyt = yyt->ATagPro.Next; break;
case kAnId:
yyt = yyt->AnId.Next; break;
case kArray:
yyMark (yyt->Array.Expr);
yyt = yyt->Array.Type; break;
case kUserArray:
yyMark (yyt->UserArray.Expr);
yyt = yyt->UserArray.Type; break;
case kChan:
yyt = yyt->Chan.Protocol; break;
case kUserStructTypes:
yyt = yyt->UserStructTypes.FieldList; break;
case kUserDataTypes:
yyt = yyt->UserDataTypes.FieldList; break;
case kUserRecord:
yyt = yyt->UserRecord.FieldList; break;
case kUserPackedRecord:
yyt = yyt->UserPackedRecord.FieldList; break;
case kUserUnion:
yyt = yyt->UserUnion.FieldList; break;
case kUserChanRecord:
yyt = yyt->UserChanRecord.FieldList; break;
case kAType:
yyMark (yyt->AType.Type);
yyt = yyt->AType.Next; break;
case kAField:
yyMark (yyt->AField.Type);
yyt = yyt->AField.Next; break;
case kSimple:
yyt = yyt->Simple.Type; break;
case kCounted:
yyMark (yyt->Counted.Length);
yyt = yyt->Counted.Type; break;
case kASeqPro:
yyMark (yyt->ASeqPro.Protocol);
yyt = yyt->ASeqPro.Next; break;
case kBinary:
yyMark (yyt->Binary.Lop);
yyMark (yyt->Binary.Rop);
yyt = yyt->Binary.Op; break;
case kUnary:
yyMark (yyt->Unary.Expr);
yyt = yyt->Unary.Op; break;
case kConst:
yyt = yyt->Const.Type; break;
case kValof:
yyMark (yyt->Valof.SpecList);
yyMark (yyt->Valof.Process);
yyt = yyt->Valof.ExprList; break;
case kFuncCall:
yyt = yyt->FuncCall.ExprList; break;
case kElementalExpr:
yyt = yyt->ElementalExpr.Elemental; break;
case kConvert:
yyMark (yyt->Convert.Type);
yyt = yyt->Convert.Expr; break;
case kRound:
yyMark (yyt->Round.Type);
yyt = yyt->Round.Expr; break;
case kTrunc:
yyMark (yyt->Trunc.Type);
yyt = yyt->Trunc.Expr; break;
case kMostPos:
yyt = yyt->MostPos.Type; break;
case kMostNeg:
yyt = yyt->MostNeg.Type; break;
case kCountArray:
yyMark (yyt->CountArray.Length);
yyt = yyt->CountArray.Expr; break;
case kBytesInType:
yyt = yyt->BytesInType.Type; break;
case kBytesInExpr:
yyt = yyt->BytesInExpr.Expr; break;
case kSizeOf:
yyt = yyt->SizeOf.Type; break;
case kOffsetOf:
yyMark (yyt->OffsetOf.Type);
yyt = yyt->OffsetOf.UnmappedIdentifier; break;
case kString:
yyt = yyt->String.Type; break;
case kStringSubscript:
yyMark (yyt->StringSubscript.Next);
yyt = yyt->StringSubscript.Expr; break;
case kStringRange:
yyMark (yyt->StringRange.Next);
yyMark (yyt->StringRange.From);
yyt = yyt->StringRange.For; break;
case kTable:
yyt = yyt->Table.ExprList; break;
case kUserLiteral:
yyMark (yyt->UserLiteral.ExprList);
yyt = yyt->UserLiteral.Type; break;
case kTableExpr:
yyt = yyt->TableExpr.Expr; break;
case kTableSubscript:
yyMark (yyt->TableSubscript.Next);
yyt = yyt->TableSubscript.Expr; break;
case kTableRange:
yyMark (yyt->TableRange.Next);
yyMark (yyt->TableRange.From);
yyt = yyt->TableRange.For; break;
case kElementAccess:
yyt = yyt->ElementAccess.Next; break;
case kSubscript:
yyMark (yyt->Subscript.Next);
yyt = yyt->Subscript.Expr; break;
case kRange:
yyMark (yyt->Range.Next);
yyMark (yyt->Range.From);
yyt = yyt->Range.For; break;
case kAnExpr:
yyMark (yyt->AnExpr.Expr);
yyt = yyt->AnExpr.Next; break;
case kProcess:
yyt = yyt->Process.SpecList; break;
case kAssign:
yyMark (yyt->Assign.SpecList);
yyMark (yyt->Assign.Lval);
yyt = yyt->Assign.Rval; break;
case kAltType:
yyt = yyt->AltType.SpecList; break;
case kAlt:
yyMark (yyt->Alt.SpecList);
yyt = yyt->Alt.AltList; break;
case kAltRep:
yyMark (yyt->AltRep.SpecList);
yyMark (yyt->AltRep.Replicator);
yyt = yyt->AltRep.AltList; break;
case kParType:
yyt = yyt->ParType.SpecList; break;
case kPar:
yyMark (yyt->Par.SpecList);
yyt = yyt->Par.ProcList; break;
case kParRep:
yyMark (yyt->ParRep.SpecList);
yyMark (yyt->ParRep.Replicator);
yyt = yyt->ParRep.Process; break;
case kProcessor:
yyMark (yyt->Processor.SpecList);
yyMark (yyt->Processor.Expr);
yyt = yyt->Processor.Process; break;
case kIfType:
yyt = yyt->IfType.SpecList; break;
case kIf:
yyMark (yyt->If.SpecList);
yyt = yyt->If.ChoiceList; break;
case kIfRep:
yyMark (yyt->IfRep.SpecList);
yyMark (yyt->IfRep.Replicator);
yyt = yyt->IfRep.ChoiceList; break;
case kInput:
yyMark (yyt->Input.SpecList);
yyMark (yyt->Input.Element);
yyt = yyt->Input.ExprList; break;
case kOutput:
yyMark (yyt->Output.SpecList);
yyMark (yyt->Output.Element);
yyt = yyt->Output.ExprList; break;
case kCaseIn:
yyMark (yyt->CaseIn.SpecList);
yyMark (yyt->CaseIn.Element);
yyt = yyt->CaseIn.CaseList; break;
case kAfter:
yyMark (yyt->After.SpecList);
yyMark (yyt->After.Element);
yyt = yyt->After.Expr; break;
case kSeqRep:
yyMark (yyt->SeqRep.SpecList);
yyMark (yyt->SeqRep.Replicator);
yyt = yyt->SeqRep.Process; break;
case kSeq:
yyMark (yyt->Seq.SpecList);
yyt = yyt->Seq.ProcList; break;
case kCase:
yyMark (yyt->Case.SpecList);
yyMark (yyt->Case.Selector);
yyt = yyt->Case.ChoiceList; break;
case kWhile:
yyMark (yyt->While.SpecList);
yyMark (yyt->While.Expr);
yyt = yyt->While.Process; break;
case kSkip:
yyt = yyt->Skip.SpecList; break;
case kStop:
yyt = yyt->Stop.SpecList; break;
case kInstance:
yyMark (yyt->Instance.SpecList);
yyt = yyt->Instance.ExprList; break;
case kCCode:
yyt = yyt->CCode.SpecList; break;
case kNullProcess:
yyt = yyt->NullProcess.SpecList; break;
case kAnAlts:
yyMark (yyt->AnAlts.SpecList);
yyt = yyt->AnAlts.Next; break;
case kAnAlt:
yyMark (yyt->AnAlt.SpecList);
yyMark (yyt->AnAlt.Guard);
yyt = yyt->AnAlt.Next; break;
case kANewAlt:
yyMark (yyt->ANewAlt.SpecList);
yyMark (yyt->ANewAlt.AltType);
yyt = yyt->ANewAlt.Next; break;
case kGuard:
yyt = yyt->Guard.Expr; break;
case kAltInput:
yyMark (yyt->AltInput.Expr);
yyMark (yyt->AltInput.Input);
yyt = yyt->AltInput.Process; break;
case kAltTimer:
yyMark (yyt->AltTimer.Expr);
yyMark (yyt->AltTimer.After);
yyt = yyt->AltTimer.Process; break;
case kAltCaseIn:
yyMark (yyt->AltCaseIn.Expr);
yyt = yyt->AltCaseIn.CaseIn; break;
case kNoInput:
yyMark (yyt->NoInput.Expr);
yyt = yyt->NoInput.Process; break;
case kAChoices:
yyMark (yyt->AChoices.SpecList);
yyt = yyt->AChoices.Next; break;
case kAChoice:
yyMark (yyt->AChoice.SpecList);
yyMark (yyt->AChoice.ExprList);
yyMark (yyt->AChoice.Process);
yyt = yyt->AChoice.Next; break;
case kANewIf:
yyMark (yyt->ANewIf.SpecList);
yyMark (yyt->ANewIf.IfType);
yyt = yyt->ANewIf.Next; break;
case kAProcess:
yyMark (yyt->AProcess.Process);
yyt = yyt->AProcess.Next; break;
case kACase:
yyMark (yyt->ACase.SpecList);
yyMark (yyt->ACase.ExprList);
yyMark (yyt->ACase.Process);
yyt = yyt->ACase.Next; break;
case kAnAccess:
yyMark (yyt->AnAccess.Next);
yyt = yyt->AnAccess.ElementList; break;
case kAnElement:
yyMark (yyt->AnElement.Next);
yyt = yyt->AnElement.Element; break;
  default: return;
  }
 }
}

# define yyInitTreeStoreSize 32
# define yyMapToTree(yyLabel) yyTreeStorePtr [yyLabel]

static unsigned long yyTreeStoreSize = yyInitTreeStoreSize;
static tTree yyTreeStore [yyInitTreeStoreSize];
static tTree * yyTreeStorePtr = yyTreeStore;
static int yyLabelCount;
static short yyRecursionLevel = 0;

static Tree_tLabel yyMapToLabel
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 register int yyi;
 for (yyi = 1; yyi <= yyLabelCount; yyi ++) if (yyTreeStorePtr [yyi] == yyt) return yyi;
 if (++ yyLabelCount == yyTreeStoreSize)
  ExtendArray ((char * *) & yyTreeStorePtr, & yyTreeStoreSize, sizeof (tTree));
 yyTreeStorePtr [yyLabelCount] = yyt;
 return yyLabelCount;
}

static void yyWriteTree ();

static void yyWriteNl () { (void) putc ('\n', yyf); }

static void yyWriteSelector
# if defined __STDC__ | defined __cplusplus
 (char * yys)
# else
 (yys) char * yys;
# endif
{
 register int yyi = 16 - strlen (yys);
 (void) fputs (yys, yyf);
 while (yyi -- > 0) (void) putc (' ', yyf);
 (void) fputs (" = ", yyf);
}

static void yyWriteHex
# if defined __STDC__ | defined __cplusplus
 (unsigned char * yyx, int yysize)
# else
 (yyx, yysize) unsigned char * yyx; int yysize;
# endif
{ register int yyi; for (yyi = 0; yyi < yysize; yyi ++) (void) fprintf (yyf, "%02x ", yyx [yyi]); }

static void yyWriteAdr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 if (yyt == NoTree) (void) fputs ("NoTree", yyf);
 else yyWriteHex ((unsigned char *) & yyt, sizeof (yyt));
 yyWriteNl ();
}

static void yWriteNodeStart
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Process"); yyWriteAdr (yyt->Start.Process);
 yyWriteSelector ("CGNext"); writetChain (yyt->Start.CGNext) yyWriteNl ();
 yyWriteSelector ("OuterProc"); writetSymbol (yyt->Start.OuterProc) yyWriteNl ();
}

static void yWriteNodeSpecList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("V1In"); writeint (yyt->SpecList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->SpecList.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->SpecList.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->SpecList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->SpecList.VOut) yyWriteNl ();
 yyWriteSelector ("DCL"); writeint (yyt->SpecList.DCL) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->SpecList.Parent) yyWriteNl ();
 yyWriteSelector ("AtomicSpec"); writebool (yyt->SpecList.AtomicSpec) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->SpecList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->SpecList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpecList (yyt); 
}

static void yWriteNodeASpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpecList (yyt); 
 yyWriteSelector ("Spec"); yyWriteAdr (yyt->ASpec.Spec);
 yyWriteSelector ("Next"); yyWriteAdr (yyt->ASpec.Next);
}

static void yWriteNodeSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Spec.Pos) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->Spec.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Spec.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->Spec.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Spec.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Spec.VOut) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Spec.Parent) yyWriteNl ();
 yyWriteSelector ("AtomicSpec"); writebool (yyt->Spec.AtomicSpec) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Spec.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Spec.PUOut) yyWriteNl ();
}

static void yWriteNodeDeclType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->DeclType.Type);
}

static void yWriteNodeDecl
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Decl.Type);
 yyWriteSelector ("IdList"); yyWriteAdr (yyt->Decl.IdList);
}

static void yWriteNodeIs
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Is.Type);
 yyWriteSelector ("Sym"); writetSymbol (yyt->Is.Sym) yyWriteNl ();
 yyWriteSelector ("Elemental"); yyWriteAdr (yyt->Is.Elemental);
 yyWriteSelector ("U"); writetUsage (yyt->Is.U) yyWriteNl ();
}

static void yWriteNodeAVal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->AVal.Type);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AVal.Sym) yyWriteNl ();
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->AVal.Expr);
}

static void yWriteNodeVal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAVal (yyt); 
}

static void yWriteNodeValRetypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAVal (yyt); 
}

static void yWriteNodeValReshapes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeValRetypes (yyt); 
}

static void yWriteNodeRetypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Retypes.Type);
 yyWriteSelector ("Sym"); writetSymbol (yyt->Retypes.Sym) yyWriteNl ();
 yyWriteSelector ("Elemental"); yyWriteAdr (yyt->Retypes.Elemental);
}

static void yWriteNodeReshapes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeRetypes (yyt); 
}

static void yWriteNodeDefAPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
}

static void yWriteNodeDefSeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAPro (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->DefSeqPro.Sym) yyWriteNl ();
 yyWriteSelector ("SeqProList"); yyWriteAdr (yyt->DefSeqPro.SeqProList);
}

static void yWriteNodeDefTagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAPro (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->DefTagPro.Sym) yyWriteNl ();
 yyWriteSelector ("TagProList"); yyWriteAdr (yyt->DefTagPro.TagProList);
}

static void yWriteNodeDefAProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->DefAProc.Sym) yyWriteNl ();
 yyWriteSelector ("Formals"); yyWriteAdr (yyt->DefAProc.Formals);
 yyWriteSelector ("U"); writetUsage (yyt->DefAProc.U) yyWriteNl ();
 yyWriteSelector ("CGNext"); writetChain (yyt->DefAProc.CGNext) yyWriteNl ();
}

static void yWriteNodeDefProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAProc (yyt); 
 yyWriteSelector ("Process"); yyWriteAdr (yyt->DefProc.Process);
 yyWriteSelector ("ATOMIC"); writebool (yyt->DefProc.ATOMIC) yyWriteNl ();
 yyWriteSelector ("isPar"); writebool (yyt->DefProc.isPar) yyWriteNl ();
}

static void yWriteNodeInlineDefProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefProc (yyt); 
}

static void yWriteNodePrototypeProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAProc (yyt); 
 yyWriteSelector ("Module"); writetIdent (yyt->PrototypeProc.Module) yyWriteNl ();
 yyWriteSelector ("Lang"); writetLang (yyt->PrototypeProc.Lang) yyWriteNl ();
}

static void yWriteNodeDefAFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->DefAFunc.Sym) yyWriteNl ();
 yyWriteSelector ("TypeList"); yyWriteAdr (yyt->DefAFunc.TypeList);
 yyWriteSelector ("Formals"); yyWriteAdr (yyt->DefAFunc.Formals);
 yyWriteSelector ("U"); writetUsage (yyt->DefAFunc.U) yyWriteNl ();
 yyWriteSelector ("CGNext"); writetChain (yyt->DefAFunc.CGNext) yyWriteNl ();
}

static void yWriteNodeDefFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAFunc (yyt); 
 yyWriteSelector ("Valof"); yyWriteAdr (yyt->DefFunc.Valof);
 yyWriteSelector ("ATOMIC"); writebool (yyt->DefFunc.ATOMIC) yyWriteNl ();
}

static void yWriteNodeInlineDefFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefFunc (yyt); 
}

static void yWriteNodeDefIsFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAFunc (yyt); 
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->DefIsFunc.ExprList);
 yyWriteSelector ("ATOMIC"); writebool (yyt->DefIsFunc.ATOMIC) yyWriteNl ();
}

static void yWriteNodeInlineDefIsFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefIsFunc (yyt); 
}

static void yWriteNodePrototypeFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeDefAFunc (yyt); 
 yyWriteSelector ("Module"); writetIdent (yyt->PrototypeFunc.Module) yyWriteNl ();
 yyWriteSelector ("Lang"); writetLang (yyt->PrototypeFunc.Lang) yyWriteNl ();
}

static void yWriteNodeNullSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
}

static void yWriteNodeHCode
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Code"); writetStringRef (yyt->HCode.Code) yyWriteNl ();
 yyWriteSelector ("Scope"); writetSymbol (yyt->HCode.Scope) yyWriteNl ();
 yyWriteSelector ("CGNext"); writetChain (yyt->HCode.CGNext) yyWriteNl ();
}

static void yWriteNodeCallFrame
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("Id"); writetIdent (yyt->CallFrame.Id) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->CallFrame.Sym) yyWriteNl ();
}

static void yWriteNodeParFrame
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSpec (yyt); 
 yyWriteSelector ("TId"); writetIdent (yyt->ParFrame.TId) yyWriteNl ();
 yyWriteSelector ("FId"); writetIdent (yyt->ParFrame.FId) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->ParFrame.Sym) yyWriteNl ();
 yyWriteSelector ("Rep"); writeint (yyt->ParFrame.Rep) yyWriteNl ();
}

static void yWriteNodeFormals
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("V1In"); writeint (yyt->Formals.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Formals.V1Out) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Formals.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Formals.VOut) yyWriteNl ();
 yyWriteSelector ("InPrototype"); writebool (yyt->Formals.InPrototype) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Formals.Parent) yyWriteNl ();
 yyWriteSelector ("AtomicSpec"); writebool (yyt->Formals.AtomicSpec) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Formals.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Formals.PUOut) yyWriteNl ();
}

static void yWriteNodeNoFormal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeFormals (yyt); 
}

static void yWriteNodeNoFormals
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeNoFormal (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->NoFormals.Sym) yyWriteNl ();
}

static void yWriteNodeAFormal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeFormals (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AFormal.Next);
 yyWriteSelector ("Pos"); writetPosition (yyt->AFormal.Pos) yyWriteNl ();
 yyWriteSelector ("By"); yyWriteAdr (yyt->AFormal.By);
 yyWriteSelector ("Type"); yyWriteAdr (yyt->AFormal.Type);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AFormal.Sym) yyWriteNl ();
 yyWriteSelector ("U"); writetUsage (yyt->AFormal.U) yyWriteNl ();
}

static void yWriteNodeReplicator
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Sym"); writetSymbol (yyt->Replicator.Sym) yyWriteNl ();
 yyWriteSelector ("From"); yyWriteAdr (yyt->Replicator.From);
 yyWriteSelector ("For"); yyWriteAdr (yyt->Replicator.For);
 yyWriteSelector ("V1In"); writeint (yyt->Replicator.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Replicator.V1Out) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Replicator.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Replicator.VOut) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Replicator.Parent) yyWriteNl ();
 yyWriteSelector ("AtomicSpec"); writebool (yyt->Replicator.AtomicSpec) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Replicator.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Replicator.PUOut) yyWriteNl ();
 yyWriteSelector ("SE"); writebool (yyt->Replicator.SE) yyWriteNl ();
 yyWriteSelector ("FromLE"); writepLinExpr (yyt->Replicator.FromLE) yyWriteNl ();
 yyWriteSelector ("ForLE"); writepLinExpr (yyt->Replicator.ForLE) yyWriteNl ();
}

static void yWriteNodeTagProList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("PUIn"); writetParUsage (yyt->TagProList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->TagProList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoTagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTagProList (yyt); 
}

static void yWriteNodeATagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTagProList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->ATagPro.Next);
 yyWriteSelector ("Sym"); writetSymbol (yyt->ATagPro.Sym) yyWriteNl ();
 yyWriteSelector ("SeqProList"); yyWriteAdr (yyt->ATagPro.SeqProList);
}

static void yWriteNodeIdList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Parent"); writetSymbol (yyt->IdList.Parent) yyWriteNl ();
 yyWriteSelector ("AtomicSpec"); writebool (yyt->IdList.AtomicSpec) yyWriteNl ();
}

static void yWriteNodeNoId
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeIdList (yyt); 
}

static void yWriteNodeAnId
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeIdList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AnId.Next);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AnId.Sym) yyWriteNl ();
 yyWriteSelector ("U"); writetUsage (yyt->AnId.U) yyWriteNl ();
}

static void yWriteNodeType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("V1In"); writeint (yyt->Type.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Type.V1Out) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Type.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Type.VOut) yyWriteNl ();
 yyWriteSelector ("D"); writeint (yyt->Type.D) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Type.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Type.PUOut) yyWriteNl ();
}

static void yWriteNodeArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Array.Expr);
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Array.Type);
 yyWriteSelector ("SE"); writebool (yyt->Array.SE) yyWriteNl ();
 yyWriteSelector ("LE"); writepLinExpr (yyt->Array.LE) yyWriteNl ();
}

static void yWriteNodeUserArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeArray (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserArray.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserArray.Sym) yyWriteNl ();
}

static void yWriteNodeChan
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
 yyWriteSelector ("Protocol"); yyWriteAdr (yyt->Chan.Protocol);
}

static void yWriteNodeTimer
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
}

static void yWriteNodeUntyped
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
}

static void yWriteNodeUntypedByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUntyped (yyt); 
}

static void yWriteNodeUntypedReal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUntyped (yyt); 
}

static void yWriteNodeUntypedInteger
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUntyped (yyt); 
}

static void yWriteNodeUntypedInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUntypedInteger (yyt); 
}

static void yWriteNodeUntypedHex
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUntypedInteger (yyt); 
}

static void yWriteNodePrimitive
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
}

static void yWriteNodeNumeric
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodePrimitive (yyt); 
}

static void yWriteNodeInteger
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeNumeric (yyt); 
}

static void yWriteNodeInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInteger (yyt); 
}

static void yWriteNodeUserInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInt (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserInt.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserInt.Sym) yyWriteNl ();
}

static void yWriteNodeInt16
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInteger (yyt); 
}

static void yWriteNodeUserInt16
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInt16 (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserInt16.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserInt16.Sym) yyWriteNl ();
}

static void yWriteNodeInt32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInteger (yyt); 
}

static void yWriteNodeUserInt32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInt32 (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserInt32.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserInt32.Sym) yyWriteNl ();
}

static void yWriteNodeInt64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInteger (yyt); 
}

static void yWriteNodeUserInt64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeInt64 (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserInt64.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserInt64.Sym) yyWriteNl ();
}

static void yWriteNodeReal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeNumeric (yyt); 
}

static void yWriteNodeReal32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeReal (yyt); 
}

static void yWriteNodeUserReal32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeReal32 (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserReal32.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserReal32.Sym) yyWriteNl ();
}

static void yWriteNodeReal64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeReal (yyt); 
}

static void yWriteNodeUserReal64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeReal64 (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserReal64.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserReal64.Sym) yyWriteNl ();
}

static void yWriteNodeByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodePrimitive (yyt); 
}

static void yWriteNodeUserByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeByte (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserByte.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserByte.Sym) yyWriteNl ();
}

static void yWriteNodeBool
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodePrimitive (yyt); 
}

static void yWriteNodeUserBool
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeBool (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserBool.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserBool.Sym) yyWriteNl ();
}

static void yWriteNodeUserStructTypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
 yyWriteSelector ("Pos"); writetPosition (yyt->UserStructTypes.Pos) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->UserStructTypes.Sym) yyWriteNl ();
 yyWriteSelector ("FieldList"); yyWriteAdr (yyt->UserStructTypes.FieldList);
}

static void yWriteNodeUserDataTypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUserStructTypes (yyt); 
}

static void yWriteNodeUserRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUserDataTypes (yyt); 
}

static void yWriteNodeUserPackedRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUserDataTypes (yyt); 
}

static void yWriteNodeUserUnion
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUserDataTypes (yyt); 
}

static void yWriteNodeUserChanRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeUserStructTypes (yyt); 
}

static void yWriteNodeTypeList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeType (yyt); 
}

static void yWriteNodeNoType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTypeList (yyt); 
}

static void yWriteNodeAType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTypeList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AType.Next);
 yyWriteSelector ("Type"); yyWriteAdr (yyt->AType.Type);
}

static void yWriteNodeFieldList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("PUIn"); writetParUsage (yyt->FieldList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->FieldList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoField
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeFieldList (yyt); 
}

static void yWriteNodeAField
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeFieldList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AField.Next);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AField.Sym) yyWriteNl ();
 yyWriteSelector ("Type"); yyWriteAdr (yyt->AField.Type);
}

static void yWriteNodeProtocol
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Protocol.Pos) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Protocol.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Protocol.PUOut) yyWriteNl ();
}

static void yWriteNodeAny
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProtocol (yyt); 
}

static void yWriteNodeNamed
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProtocol (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->Named.Sym) yyWriteNl ();
}

static void yWriteNodeSimple
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProtocol (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Simple.Type);
}

static void yWriteNodeCounted
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProtocol (yyt); 
 yyWriteSelector ("Length"); yyWriteAdr (yyt->Counted.Length);
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Counted.Type);
}

static void yWriteNodeSeqProList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("PUIn"); writetParUsage (yyt->SeqProList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->SeqProList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoSeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSeqProList (yyt); 
}

static void yWriteNodeASeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeSeqProList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->ASeqPro.Next);
 yyWriteSelector ("Protocol"); yyWriteAdr (yyt->ASeqPro.Protocol);
}

static void yWriteNodeExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Expr.Pos) yyWriteNl ();
 yyWriteSelector ("DefType"); writetType (yyt->Expr.DefType) yyWriteNl ();
 yyWriteSelector ("FLIn"); writetType (yyt->Expr.FLIn) yyWriteNl ();
 yyWriteSelector ("FLOut"); writetType (yyt->Expr.FLOut) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->Expr.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Expr.V1Out) yyWriteNl ();
 yyWriteSelector ("SynType"); writetType (yyt->Expr.SynType) yyWriteNl ();
 yyWriteSelector ("InhType"); writetType (yyt->Expr.InhType) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Expr.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Expr.VOut) yyWriteNl ();
 yyWriteSelector ("C"); writebool (yyt->Expr.C) yyWriteNl ();
 yyWriteSelector ("CV"); writetValue (yyt->Expr.CV) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Expr.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Expr.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Expr.PUOut) yyWriteNl ();
 yyWriteSelector ("LV"); writebool (yyt->Expr.LV) yyWriteNl ();
}

static void yWriteNodeBinary
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Lop"); yyWriteAdr (yyt->Binary.Lop);
 yyWriteSelector ("Rop"); yyWriteAdr (yyt->Binary.Rop);
 yyWriteSelector ("Op"); yyWriteAdr (yyt->Binary.Op);
}

static void yWriteNodeUnary
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Unary.Expr);
 yyWriteSelector ("Op"); yyWriteAdr (yyt->Unary.Op);
}

static void yWriteNodeConst
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Value"); writetValue (yyt->Const.Value) yyWriteNl ();
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Const.Type);
}

static void yWriteNodeValof
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("SpecList"); yyWriteAdr (yyt->Valof.SpecList);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->Valof.Process);
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->Valof.ExprList);
 yyWriteSelector ("Sym"); writetSymbol (yyt->Valof.Sym) yyWriteNl ();
 yyWriteSelector ("FuncCall"); writetTree (yyt->Valof.FuncCall) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->Valof.VS) yyWriteNl ();
}

static void yWriteNodeFuncCall
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->FuncCall.Sym) yyWriteNl ();
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->FuncCall.ExprList);
 yyWriteSelector ("Depth"); writeint (yyt->FuncCall.Depth) yyWriteNl ();
 yyWriteSelector ("Frame"); writetFrame (yyt->FuncCall.Frame) yyWriteNl ();
}

static void yWriteNodeElementalExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Elemental"); yyWriteAdr (yyt->ElementalExpr.Elemental);
 yyWriteSelector ("PAR"); writebool (yyt->ElementalExpr.PAR) yyWriteNl ();
}

static void yWriteNodeDefault
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
}

static void yWriteNodeConvert
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Convert.Type);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Convert.Expr);
}

static void yWriteNodeRound
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Round.Type);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Round.Expr);
}

static void yWriteNodeTrunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->Trunc.Type);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Trunc.Expr);
}

static void yWriteNodeMostPos
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->MostPos.Type);
}

static void yWriteNodeMostNeg
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->MostNeg.Type);
}

static void yWriteNodeCountArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Length"); yyWriteAdr (yyt->CountArray.Length);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->CountArray.Expr);
}

static void yWriteNodeUnBound
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
}

static void yWriteNodeBytesIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
}

static void yWriteNodeBytesInType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeBytesIn (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->BytesInType.Type);
}

static void yWriteNodeBytesInExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeBytesIn (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->BytesInExpr.Expr);
}

static void yWriteNodeSizeOf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->SizeOf.Type);
}

static void yWriteNodeOffsetOf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->OffsetOf.Type);
 yyWriteSelector ("UnmappedIdentifier"); yyWriteAdr (yyt->OffsetOf.UnmappedIdentifier);
}

static void yWriteNodeStringElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExpr (yyt); 
 yyWriteSelector ("CA"); writetConstArray (yyt->StringElement.CA) yyWriteNl ();
 yyWriteSelector ("SE"); writebool (yyt->StringElement.SE) yyWriteNl ();
}

static void yWriteNodeString
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeStringElement (yyt); 
 yyWriteSelector ("Value"); writetStringRef (yyt->String.Value) yyWriteNl ();
 yyWriteSelector ("Type"); yyWriteAdr (yyt->String.Type);
 yyWriteSelector ("Depth"); writeint (yyt->String.Depth) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->String.Sym) yyWriteNl ();
 yyWriteSelector ("LE"); writepLinExpr (yyt->String.LE) yyWriteNl ();
}

static void yWriteNodeStringSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeStringElement (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->StringSubscript.Next);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->StringSubscript.Expr);
 yyWriteSelector ("LE"); writepLinExpr (yyt->StringSubscript.LE) yyWriteNl ();
}

static void yWriteNodeStringRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeStringElement (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->StringRange.Next);
 yyWriteSelector ("From"); yyWriteAdr (yyt->StringRange.From);
 yyWriteSelector ("For"); yyWriteAdr (yyt->StringRange.For);
 yyWriteSelector ("FromLE"); writepLinExpr (yyt->StringRange.FromLE) yyWriteNl ();
 yyWriteSelector ("ForLE"); writepLinExpr (yyt->StringRange.ForLE) yyWriteNl ();
}

static void yWriteNodeElemental
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Elemental.Pos) yyWriteNl ();
 yyWriteSelector ("DefType"); writetType (yyt->Elemental.DefType) yyWriteNl ();
 yyWriteSelector ("FLIn"); writetType (yyt->Elemental.FLIn) yyWriteNl ();
 yyWriteSelector ("FLOut"); writetType (yyt->Elemental.FLOut) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->Elemental.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Elemental.V1Out) yyWriteNl ();
 yyWriteSelector ("SynType"); writetType (yyt->Elemental.SynType) yyWriteNl ();
 yyWriteSelector ("InhType"); writetType (yyt->Elemental.InhType) yyWriteNl ();
 yyWriteSelector ("W"); writeint (yyt->Elemental.W) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Elemental.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Elemental.VOut) yyWriteNl ();
 yyWriteSelector ("C"); writebool (yyt->Elemental.C) yyWriteNl ();
 yyWriteSelector ("CV"); writetValue (yyt->Elemental.CV) yyWriteNl ();
 yyWriteSelector ("CA"); writetConstArray (yyt->Elemental.CA) yyWriteNl ();
 yyWriteSelector ("U"); writetUsage (yyt->Elemental.U) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Elemental.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Elemental.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Elemental.PUOut) yyWriteNl ();
 yyWriteSelector ("LV"); writebool (yyt->Elemental.LV) yyWriteNl ();
 yyWriteSelector ("SE"); writebool (yyt->Elemental.SE) yyWriteNl ();
 yyWriteSelector ("RVL"); writetTree (yyt->Elemental.RVL) yyWriteNl ();
}

static void yWriteNodeTableElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElemental (yyt); 
}

static void yWriteNodeTable
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTableElement (yyt); 
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->Table.ExprList);
 yyWriteSelector ("Depth"); writeint (yyt->Table.Depth) yyWriteNl ();
 yyWriteSelector ("Sym"); writetSymbol (yyt->Table.Sym) yyWriteNl ();
 yyWriteSelector ("LE"); writepLinExpr (yyt->Table.LE) yyWriteNl ();
}

static void yWriteNodeUserLiteral
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTable (yyt); 
 yyWriteSelector ("Type"); yyWriteAdr (yyt->UserLiteral.Type);
}

static void yWriteNodeTableExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTableElement (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->TableExpr.Expr);
 yyWriteSelector ("Sym"); writetSymbol (yyt->TableExpr.Sym) yyWriteNl ();
}

static void yWriteNodeTableSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTableElement (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->TableSubscript.Next);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->TableSubscript.Expr);
 yyWriteSelector ("LE"); writepLinExpr (yyt->TableSubscript.LE) yyWriteNl ();
}

static void yWriteNodeTableRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeTableElement (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->TableRange.Next);
 yyWriteSelector ("From"); yyWriteAdr (yyt->TableRange.From);
 yyWriteSelector ("For"); yyWriteAdr (yyt->TableRange.For);
 yyWriteSelector ("FromLE"); writepLinExpr (yyt->TableRange.FromLE) yyWriteNl ();
 yyWriteSelector ("ForLE"); writepLinExpr (yyt->TableRange.ForLE) yyWriteNl ();
}

static void yWriteNodeElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElemental (yyt); 
}

static void yWriteNodeIdentifier
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElement (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->Identifier.Sym) yyWriteNl ();
 yyWriteSelector ("Scope"); writetSymbol (yyt->Identifier.Scope) yyWriteNl ();
}

static void yWriteNodeUnmappedIdentifier
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeIdentifier (yyt); 
 yyWriteSelector ("Ident"); writetIdent (yyt->UnmappedIdentifier.Ident) yyWriteNl ();
}

static void yWriteNodeElementAccess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElement (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->ElementAccess.Next);
}

static void yWriteNodeSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElementAccess (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Subscript.Expr);
 yyWriteSelector ("CB"); writebool (yyt->Subscript.CB) yyWriteNl ();
 yyWriteSelector ("LE"); writepLinExpr (yyt->Subscript.LE) yyWriteNl ();
}

static void yWriteNodeRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeElementAccess (yyt); 
 yyWriteSelector ("From"); yyWriteAdr (yyt->Range.From);
 yyWriteSelector ("For"); yyWriteAdr (yyt->Range.For);
 yyWriteSelector ("CLB"); writebool (yyt->Range.CLB) yyWriteNl ();
 yyWriteSelector ("CUB"); writebool (yyt->Range.CUB) yyWriteNl ();
 yyWriteSelector ("FromLE"); writepLinExpr (yyt->Range.FromLE) yyWriteNl ();
 yyWriteSelector ("ForLE"); writepLinExpr (yyt->Range.ForLE) yyWriteNl ();
}

static void yWriteNodeExprList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("DefType"); writetType (yyt->ExprList.DefType) yyWriteNl ();
 yyWriteSelector ("FLIn"); writetType (yyt->ExprList.FLIn) yyWriteNl ();
 yyWriteSelector ("FLOut"); writetType (yyt->ExprList.FLOut) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->ExprList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->ExprList.V1Out) yyWriteNl ();
 yyWriteSelector ("SynType"); writetType (yyt->ExprList.SynType) yyWriteNl ();
 yyWriteSelector ("InhType"); writetType (yyt->ExprList.InhType) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->ExprList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->ExprList.VOut) yyWriteNl ();
 yyWriteSelector ("C"); writebool (yyt->ExprList.C) yyWriteNl ();
 yyWriteSelector ("TR"); writebool (yyt->ExprList.TR) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->ExprList.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->ExprList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->ExprList.PUOut) yyWriteNl ();
 yyWriteSelector ("PAR"); writebool (yyt->ExprList.PAR) yyWriteNl ();
 yyWriteSelector ("LV"); writebool (yyt->ExprList.LV) yyWriteNl ();
}

static void yWriteNodeNoExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExprList (yyt); 
}

static void yWriteNodeAnExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeExprList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AnExpr.Next);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->AnExpr.Expr);
}

static void yWriteNodeProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Process.Pos) yyWriteNl ();
 yyWriteSelector ("SpecList"); yyWriteAdr (yyt->Process.SpecList);
 yyWriteSelector ("V1In"); writeint (yyt->Process.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Process.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->Process.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Process.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Process.VOut) yyWriteNl ();
 yyWriteSelector ("ATOMIC"); writebool (yyt->Process.ATOMIC) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->Process.SLC) yyWriteNl ();
 yyWriteSelector ("TLA"); writebool (yyt->Process.TLA) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Process.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Process.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Process.PUOut) yyWriteNl ();
}

static void yWriteNodeAssign
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Lval"); yyWriteAdr (yyt->Assign.Lval);
 yyWriteSelector ("Rval"); yyWriteAdr (yyt->Assign.Rval);
}

static void yWriteNodeAltType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Pri"); writebool (yyt->AltType.Pri) yyWriteNl ();
 yyWriteSelector ("ARC"); writeint (yyt->AltType.ARC) yyWriteNl ();
}

static void yWriteNodeAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAltType (yyt); 
 yyWriteSelector ("AltList"); yyWriteAdr (yyt->Alt.AltList);
}

static void yWriteNodeAltRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAltType (yyt); 
 yyWriteSelector ("Replicator"); yyWriteAdr (yyt->AltRep.Replicator);
 yyWriteSelector ("AltList"); yyWriteAdr (yyt->AltRep.AltList);
}

static void yWriteNodeParType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("PType"); writetParType (yyt->ParType.PType) yyWriteNl ();
}

static void yWriteNodePar
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeParType (yyt); 
 yyWriteSelector ("ProcList"); yyWriteAdr (yyt->Par.ProcList);
}

static void yWriteNodeParRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeParType (yyt); 
 yyWriteSelector ("Replicator"); yyWriteAdr (yyt->ParRep.Replicator);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->ParRep.Process);
 yyWriteSelector ("Sym"); writetSymbol (yyt->ParRep.Sym) yyWriteNl ();
}

static void yWriteNodeProcessor
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Processor.Expr);
 yyWriteSelector ("Name"); writetIdent (yyt->Processor.Name) yyWriteNl ();
 yyWriteSelector ("Process"); yyWriteAdr (yyt->Processor.Process);
}

static void yWriteNodeIfType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("BackChoiceList"); writetChoiceList (yyt->IfType.BackChoiceList) yyWriteNl ();
 yyWriteSelector ("BackIfType"); writetIfType (yyt->IfType.BackIfType) yyWriteNl ();
}

static void yWriteNodeIf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeIfType (yyt); 
 yyWriteSelector ("ChoiceList"); yyWriteAdr (yyt->If.ChoiceList);
}

static void yWriteNodeIfRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeIfType (yyt); 
 yyWriteSelector ("Replicator"); yyWriteAdr (yyt->IfRep.Replicator);
 yyWriteSelector ("ChoiceList"); yyWriteAdr (yyt->IfRep.ChoiceList);
}

static void yWriteNodeInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Element"); yyWriteAdr (yyt->Input.Element);
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->Input.ExprList);
}

static void yWriteNodeOutput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Element"); yyWriteAdr (yyt->Output.Element);
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->Output.ExprList);
}

static void yWriteNodeCaseIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Element"); yyWriteAdr (yyt->CaseIn.Element);
 yyWriteSelector ("CaseList"); yyWriteAdr (yyt->CaseIn.CaseList);
}

static void yWriteNodeAfter
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Element"); yyWriteAdr (yyt->After.Element);
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->After.Expr);
}

static void yWriteNodeSeqRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Replicator"); yyWriteAdr (yyt->SeqRep.Replicator);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->SeqRep.Process);
}

static void yWriteNodeSeq
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("ProcList"); yyWriteAdr (yyt->Seq.ProcList);
}

static void yWriteNodeCase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Selector"); yyWriteAdr (yyt->Case.Selector);
 yyWriteSelector ("ChoiceList"); yyWriteAdr (yyt->Case.ChoiceList);
}

static void yWriteNodeWhile
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->While.Expr);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->While.Process);
}

static void yWriteNodeSkip
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
}

static void yWriteNodeStop
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
}

static void yWriteNodeInstance
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Sym"); writetSymbol (yyt->Instance.Sym) yyWriteNl ();
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->Instance.ExprList);
 yyWriteSelector ("Scope"); writetSymbol (yyt->Instance.Scope) yyWriteNl ();
 yyWriteSelector ("Frame"); writetFrame (yyt->Instance.Frame) yyWriteNl ();
}

static void yWriteNodeCCode
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
 yyWriteSelector ("Code"); writetStringRef (yyt->CCode.Code) yyWriteNl ();
 yyWriteSelector ("Scope"); writetSymbol (yyt->CCode.Scope) yyWriteNl ();
}

static void yWriteNodeNullProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcess (yyt); 
}

static void yWriteNodeAltList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("V1In"); writeint (yyt->AltList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->AltList.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->AltList.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->AltList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->AltList.VOut) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->AltList.SLC) yyWriteNl ();
 yyWriteSelector ("ARC"); writeint (yyt->AltList.ARC) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->AltList.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->AltList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->AltList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAltList (yyt); 
}

static void yWriteNodeAnAlts
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAltList (yyt); 
 yyWriteSelector ("SpecList"); yyWriteAdr (yyt->AnAlts.SpecList);
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AnAlts.Next);
}

static void yWriteNodeAnAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAnAlts (yyt); 
 yyWriteSelector ("Guard"); yyWriteAdr (yyt->AnAlt.Guard);
 yyWriteSelector ("Label"); writeint (yyt->AnAlt.Label) yyWriteNl ();
}

static void yWriteNodeANewAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAnAlts (yyt); 
 yyWriteSelector ("AltType"); yyWriteAdr (yyt->ANewAlt.AltType);
}

static void yWriteNodeGuard
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Pos"); writetPosition (yyt->Guard.Pos) yyWriteNl ();
 yyWriteSelector ("Expr"); yyWriteAdr (yyt->Guard.Expr);
 yyWriteSelector ("V1In"); writeint (yyt->Guard.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->Guard.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->Guard.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->Guard.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->Guard.VOut) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->Guard.SLC) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->Guard.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->Guard.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->Guard.PUOut) yyWriteNl ();
}

static void yWriteNodeAltInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeGuard (yyt); 
 yyWriteSelector ("Input"); yyWriteAdr (yyt->AltInput.Input);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->AltInput.Process);
}

static void yWriteNodeAltTimer
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeGuard (yyt); 
 yyWriteSelector ("After"); yyWriteAdr (yyt->AltTimer.After);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->AltTimer.Process);
}

static void yWriteNodeAltCaseIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeGuard (yyt); 
 yyWriteSelector ("CaseIn"); yyWriteAdr (yyt->AltCaseIn.CaseIn);
}

static void yWriteNodeNoInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeGuard (yyt); 
 yyWriteSelector ("Process"); yyWriteAdr (yyt->NoInput.Process);
}

static void yWriteNodeChoiceList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("DefType"); writetType (yyt->ChoiceList.DefType) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->ChoiceList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->ChoiceList.V1Out) yyWriteNl ();
 yyWriteSelector ("SynType"); writetType (yyt->ChoiceList.SynType) yyWriteNl ();
 yyWriteSelector ("InhType"); writetType (yyt->ChoiceList.InhType) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->ChoiceList.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->ChoiceList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->ChoiceList.VOut) yyWriteNl ();
 yyWriteSelector ("ATOMIC"); writebool (yyt->ChoiceList.ATOMIC) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->ChoiceList.SLC) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->ChoiceList.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->ChoiceList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->ChoiceList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoChoice
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeChoiceList (yyt); 
}

static void yWriteNodeAChoices
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeChoiceList (yyt); 
 yyWriteSelector ("SpecList"); yyWriteAdr (yyt->AChoices.SpecList);
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AChoices.Next);
}

static void yWriteNodeAChoice
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAChoices (yyt); 
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->AChoice.ExprList);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->AChoice.Process);
 yyWriteSelector ("Label"); writeint (yyt->AChoice.Label) yyWriteNl ();
}

static void yWriteNodeANewIf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeAChoices (yyt); 
 yyWriteSelector ("IfType"); yyWriteAdr (yyt->ANewIf.IfType);
}

static void yWriteNodeProcList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("V1In"); writeint (yyt->ProcList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->ProcList.V1Out) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->ProcList.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->ProcList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->ProcList.VOut) yyWriteNl ();
 yyWriteSelector ("ATOMIC"); writebool (yyt->ProcList.ATOMIC) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->ProcList.SLC) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->ProcList.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->ProcList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->ProcList.PUOut) yyWriteNl ();
 yyWriteSelector ("PAR"); writebool (yyt->ProcList.PAR) yyWriteNl ();
 yyWriteSelector ("Pos"); writetPosition (yyt->ProcList.Pos) yyWriteNl ();
}

static void yWriteNodeNoProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcList (yyt); 
}

static void yWriteNodeAProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeProcList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AProcess.Next);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->AProcess.Process);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AProcess.Sym) yyWriteNl ();
}

static void yWriteNodeCaseList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("FLIn"); writetType (yyt->CaseList.FLIn) yyWriteNl ();
 yyWriteSelector ("FLOut"); writetType (yyt->CaseList.FLOut) yyWriteNl ();
 yyWriteSelector ("V1In"); writeint (yyt->CaseList.V1In) yyWriteNl ();
 yyWriteSelector ("V1Out"); writeint (yyt->CaseList.V1Out) yyWriteNl ();
 yyWriteSelector ("InhType"); writetType (yyt->CaseList.InhType) yyWriteNl ();
 yyWriteSelector ("VS"); writeint (yyt->CaseList.VS) yyWriteNl ();
 yyWriteSelector ("VIn"); writeint (yyt->CaseList.VIn) yyWriteNl ();
 yyWriteSelector ("VOut"); writeint (yyt->CaseList.VOut) yyWriteNl ();
 yyWriteSelector ("SLC"); writeint (yyt->CaseList.SLC) yyWriteNl ();
 yyWriteSelector ("Parent"); writetSymbol (yyt->CaseList.Parent) yyWriteNl ();
 yyWriteSelector ("PUIn"); writetParUsage (yyt->CaseList.PUIn) yyWriteNl ();
 yyWriteSelector ("PUOut"); writetParUsage (yyt->CaseList.PUOut) yyWriteNl ();
}

static void yWriteNodeNoCase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeCaseList (yyt); 
}

static void yWriteNodeACase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yWriteNodeCaseList (yyt); 
 yyWriteSelector ("Next"); yyWriteAdr (yyt->ACase.Next);
 yyWriteSelector ("SpecList"); yyWriteAdr (yyt->ACase.SpecList);
 yyWriteSelector ("ExprList"); yyWriteAdr (yyt->ACase.ExprList);
 yyWriteSelector ("Process"); yyWriteAdr (yyt->ACase.Process);
 yyWriteSelector ("Label"); writeint (yyt->ACase.Label) yyWriteNl ();
}

static void yWriteNodeAnAccess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AnAccess.Next);
 yyWriteSelector ("Sym"); writetSymbol (yyt->AnAccess.Sym) yyWriteNl ();
 yyWriteSelector ("ElementList"); yyWriteAdr (yyt->AnAccess.ElementList);
}

static void yWriteNodeAnElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Next"); yyWriteAdr (yyt->AnElement.Next);
 yyWriteSelector ("Element"); yyWriteAdr (yyt->AnElement.Element);
}

void WriteTreeNode
# if defined __STDC__ | defined __cplusplus
 (FILE * yyyf, tTree yyt)
# else
 (yyyf, yyt) FILE * yyyf; tTree yyt;
# endif
{
 yyf = yyyf;
 if (yyt == NoTree) { (void) fputs ("NoTree\n", yyf); return; }

 switch (yyt->Kind) {
case kStart: (void) fputs (Tree_NodeName [kStart], yyf); (void) fputc ('\n', yyf);
 yWriteNodeStart (yyt); break;
case kSpecList: (void) fputs (Tree_NodeName [kSpecList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSpecList (yyt); break;
case kNoSpec: (void) fputs (Tree_NodeName [kNoSpec], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoSpec (yyt); break;
case kASpec: (void) fputs (Tree_NodeName [kASpec], yyf); (void) fputc ('\n', yyf);
 yWriteNodeASpec (yyt); break;
case kSpec: (void) fputs (Tree_NodeName [kSpec], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSpec (yyt); break;
case kDeclType: (void) fputs (Tree_NodeName [kDeclType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDeclType (yyt); break;
case kDecl: (void) fputs (Tree_NodeName [kDecl], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDecl (yyt); break;
case kIs: (void) fputs (Tree_NodeName [kIs], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIs (yyt); break;
case kAVal: (void) fputs (Tree_NodeName [kAVal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAVal (yyt); break;
case kVal: (void) fputs (Tree_NodeName [kVal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeVal (yyt); break;
case kValRetypes: (void) fputs (Tree_NodeName [kValRetypes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeValRetypes (yyt); break;
case kValReshapes: (void) fputs (Tree_NodeName [kValReshapes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeValReshapes (yyt); break;
case kRetypes: (void) fputs (Tree_NodeName [kRetypes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeRetypes (yyt); break;
case kReshapes: (void) fputs (Tree_NodeName [kReshapes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeReshapes (yyt); break;
case kDefAPro: (void) fputs (Tree_NodeName [kDefAPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefAPro (yyt); break;
case kDefSeqPro: (void) fputs (Tree_NodeName [kDefSeqPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefSeqPro (yyt); break;
case kDefTagPro: (void) fputs (Tree_NodeName [kDefTagPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefTagPro (yyt); break;
case kDefAProc: (void) fputs (Tree_NodeName [kDefAProc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefAProc (yyt); break;
case kDefProc: (void) fputs (Tree_NodeName [kDefProc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefProc (yyt); break;
case kInlineDefProc: (void) fputs (Tree_NodeName [kInlineDefProc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInlineDefProc (yyt); break;
case kPrototypeProc: (void) fputs (Tree_NodeName [kPrototypeProc], yyf); (void) fputc ('\n', yyf);
 yWriteNodePrototypeProc (yyt); break;
case kDefAFunc: (void) fputs (Tree_NodeName [kDefAFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefAFunc (yyt); break;
case kDefFunc: (void) fputs (Tree_NodeName [kDefFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefFunc (yyt); break;
case kInlineDefFunc: (void) fputs (Tree_NodeName [kInlineDefFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInlineDefFunc (yyt); break;
case kDefIsFunc: (void) fputs (Tree_NodeName [kDefIsFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefIsFunc (yyt); break;
case kInlineDefIsFunc: (void) fputs (Tree_NodeName [kInlineDefIsFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInlineDefIsFunc (yyt); break;
case kPrototypeFunc: (void) fputs (Tree_NodeName [kPrototypeFunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodePrototypeFunc (yyt); break;
case kNullSpec: (void) fputs (Tree_NodeName [kNullSpec], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNullSpec (yyt); break;
case kHCode: (void) fputs (Tree_NodeName [kHCode], yyf); (void) fputc ('\n', yyf);
 yWriteNodeHCode (yyt); break;
case kCallFrame: (void) fputs (Tree_NodeName [kCallFrame], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCallFrame (yyt); break;
case kParFrame: (void) fputs (Tree_NodeName [kParFrame], yyf); (void) fputc ('\n', yyf);
 yWriteNodeParFrame (yyt); break;
case kFormals: (void) fputs (Tree_NodeName [kFormals], yyf); (void) fputc ('\n', yyf);
 yWriteNodeFormals (yyt); break;
case kNoFormal: (void) fputs (Tree_NodeName [kNoFormal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoFormal (yyt); break;
case kNoFormals: (void) fputs (Tree_NodeName [kNoFormals], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoFormals (yyt); break;
case kAFormal: (void) fputs (Tree_NodeName [kAFormal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAFormal (yyt); break;
case kReplicator: (void) fputs (Tree_NodeName [kReplicator], yyf); (void) fputc ('\n', yyf);
 yWriteNodeReplicator (yyt); break;
case kTagProList: (void) fputs (Tree_NodeName [kTagProList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTagProList (yyt); break;
case kNoTagPro: (void) fputs (Tree_NodeName [kNoTagPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoTagPro (yyt); break;
case kATagPro: (void) fputs (Tree_NodeName [kATagPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeATagPro (yyt); break;
case kBy: (void) fputs (Tree_NodeName [kBy], yyf); (void) fputc ('\n', yyf);
 break;
case kByVal: (void) fputs (Tree_NodeName [kByVal], yyf); (void) fputc ('\n', yyf);
 break;
case kByRef: (void) fputs (Tree_NodeName [kByRef], yyf); (void) fputc ('\n', yyf);
 break;
case kByUndef: (void) fputs (Tree_NodeName [kByUndef], yyf); (void) fputc ('\n', yyf);
 break;
case kIdList: (void) fputs (Tree_NodeName [kIdList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIdList (yyt); break;
case kNoId: (void) fputs (Tree_NodeName [kNoId], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoId (yyt); break;
case kAnId: (void) fputs (Tree_NodeName [kAnId], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnId (yyt); break;
case kType: (void) fputs (Tree_NodeName [kType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeType (yyt); break;
case kArray: (void) fputs (Tree_NodeName [kArray], yyf); (void) fputc ('\n', yyf);
 yWriteNodeArray (yyt); break;
case kUserArray: (void) fputs (Tree_NodeName [kUserArray], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserArray (yyt); break;
case kChan: (void) fputs (Tree_NodeName [kChan], yyf); (void) fputc ('\n', yyf);
 yWriteNodeChan (yyt); break;
case kTimer: (void) fputs (Tree_NodeName [kTimer], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTimer (yyt); break;
case kUntyped: (void) fputs (Tree_NodeName [kUntyped], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntyped (yyt); break;
case kUntypedByte: (void) fputs (Tree_NodeName [kUntypedByte], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntypedByte (yyt); break;
case kUntypedReal: (void) fputs (Tree_NodeName [kUntypedReal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntypedReal (yyt); break;
case kUntypedInteger: (void) fputs (Tree_NodeName [kUntypedInteger], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntypedInteger (yyt); break;
case kUntypedInt: (void) fputs (Tree_NodeName [kUntypedInt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntypedInt (yyt); break;
case kUntypedHex: (void) fputs (Tree_NodeName [kUntypedHex], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUntypedHex (yyt); break;
case kPrimitive: (void) fputs (Tree_NodeName [kPrimitive], yyf); (void) fputc ('\n', yyf);
 yWriteNodePrimitive (yyt); break;
case kNumeric: (void) fputs (Tree_NodeName [kNumeric], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNumeric (yyt); break;
case kInteger: (void) fputs (Tree_NodeName [kInteger], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInteger (yyt); break;
case kInt: (void) fputs (Tree_NodeName [kInt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInt (yyt); break;
case kUserInt: (void) fputs (Tree_NodeName [kUserInt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserInt (yyt); break;
case kInt16: (void) fputs (Tree_NodeName [kInt16], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInt16 (yyt); break;
case kUserInt16: (void) fputs (Tree_NodeName [kUserInt16], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserInt16 (yyt); break;
case kInt32: (void) fputs (Tree_NodeName [kInt32], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInt32 (yyt); break;
case kUserInt32: (void) fputs (Tree_NodeName [kUserInt32], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserInt32 (yyt); break;
case kInt64: (void) fputs (Tree_NodeName [kInt64], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInt64 (yyt); break;
case kUserInt64: (void) fputs (Tree_NodeName [kUserInt64], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserInt64 (yyt); break;
case kReal: (void) fputs (Tree_NodeName [kReal], yyf); (void) fputc ('\n', yyf);
 yWriteNodeReal (yyt); break;
case kReal32: (void) fputs (Tree_NodeName [kReal32], yyf); (void) fputc ('\n', yyf);
 yWriteNodeReal32 (yyt); break;
case kUserReal32: (void) fputs (Tree_NodeName [kUserReal32], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserReal32 (yyt); break;
case kReal64: (void) fputs (Tree_NodeName [kReal64], yyf); (void) fputc ('\n', yyf);
 yWriteNodeReal64 (yyt); break;
case kUserReal64: (void) fputs (Tree_NodeName [kUserReal64], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserReal64 (yyt); break;
case kByte: (void) fputs (Tree_NodeName [kByte], yyf); (void) fputc ('\n', yyf);
 yWriteNodeByte (yyt); break;
case kUserByte: (void) fputs (Tree_NodeName [kUserByte], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserByte (yyt); break;
case kBool: (void) fputs (Tree_NodeName [kBool], yyf); (void) fputc ('\n', yyf);
 yWriteNodeBool (yyt); break;
case kUserBool: (void) fputs (Tree_NodeName [kUserBool], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserBool (yyt); break;
case kUserStructTypes: (void) fputs (Tree_NodeName [kUserStructTypes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserStructTypes (yyt); break;
case kUserDataTypes: (void) fputs (Tree_NodeName [kUserDataTypes], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserDataTypes (yyt); break;
case kUserRecord: (void) fputs (Tree_NodeName [kUserRecord], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserRecord (yyt); break;
case kUserPackedRecord: (void) fputs (Tree_NodeName [kUserPackedRecord], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserPackedRecord (yyt); break;
case kUserUnion: (void) fputs (Tree_NodeName [kUserUnion], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserUnion (yyt); break;
case kUserChanRecord: (void) fputs (Tree_NodeName [kUserChanRecord], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserChanRecord (yyt); break;
case kTypeList: (void) fputs (Tree_NodeName [kTypeList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTypeList (yyt); break;
case kNoType: (void) fputs (Tree_NodeName [kNoType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoType (yyt); break;
case kAType: (void) fputs (Tree_NodeName [kAType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAType (yyt); break;
case kFieldList: (void) fputs (Tree_NodeName [kFieldList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeFieldList (yyt); break;
case kNoField: (void) fputs (Tree_NodeName [kNoField], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoField (yyt); break;
case kAField: (void) fputs (Tree_NodeName [kAField], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAField (yyt); break;
case kProtocol: (void) fputs (Tree_NodeName [kProtocol], yyf); (void) fputc ('\n', yyf);
 yWriteNodeProtocol (yyt); break;
case kAny: (void) fputs (Tree_NodeName [kAny], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAny (yyt); break;
case kNamed: (void) fputs (Tree_NodeName [kNamed], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNamed (yyt); break;
case kSimple: (void) fputs (Tree_NodeName [kSimple], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSimple (yyt); break;
case kCounted: (void) fputs (Tree_NodeName [kCounted], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCounted (yyt); break;
case kSeqProList: (void) fputs (Tree_NodeName [kSeqProList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSeqProList (yyt); break;
case kNoSeqPro: (void) fputs (Tree_NodeName [kNoSeqPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoSeqPro (yyt); break;
case kASeqPro: (void) fputs (Tree_NodeName [kASeqPro], yyf); (void) fputc ('\n', yyf);
 yWriteNodeASeqPro (yyt); break;
case kExpr: (void) fputs (Tree_NodeName [kExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeExpr (yyt); break;
case kBinary: (void) fputs (Tree_NodeName [kBinary], yyf); (void) fputc ('\n', yyf);
 yWriteNodeBinary (yyt); break;
case kUnary: (void) fputs (Tree_NodeName [kUnary], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUnary (yyt); break;
case kConst: (void) fputs (Tree_NodeName [kConst], yyf); (void) fputc ('\n', yyf);
 yWriteNodeConst (yyt); break;
case kValof: (void) fputs (Tree_NodeName [kValof], yyf); (void) fputc ('\n', yyf);
 yWriteNodeValof (yyt); break;
case kFuncCall: (void) fputs (Tree_NodeName [kFuncCall], yyf); (void) fputc ('\n', yyf);
 yWriteNodeFuncCall (yyt); break;
case kElementalExpr: (void) fputs (Tree_NodeName [kElementalExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeElementalExpr (yyt); break;
case kDefault: (void) fputs (Tree_NodeName [kDefault], yyf); (void) fputc ('\n', yyf);
 yWriteNodeDefault (yyt); break;
case kConvert: (void) fputs (Tree_NodeName [kConvert], yyf); (void) fputc ('\n', yyf);
 yWriteNodeConvert (yyt); break;
case kRound: (void) fputs (Tree_NodeName [kRound], yyf); (void) fputc ('\n', yyf);
 yWriteNodeRound (yyt); break;
case kTrunc: (void) fputs (Tree_NodeName [kTrunc], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTrunc (yyt); break;
case kMostPos: (void) fputs (Tree_NodeName [kMostPos], yyf); (void) fputc ('\n', yyf);
 yWriteNodeMostPos (yyt); break;
case kMostNeg: (void) fputs (Tree_NodeName [kMostNeg], yyf); (void) fputc ('\n', yyf);
 yWriteNodeMostNeg (yyt); break;
case kCountArray: (void) fputs (Tree_NodeName [kCountArray], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCountArray (yyt); break;
case kUnBound: (void) fputs (Tree_NodeName [kUnBound], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUnBound (yyt); break;
case kBytesIn: (void) fputs (Tree_NodeName [kBytesIn], yyf); (void) fputc ('\n', yyf);
 yWriteNodeBytesIn (yyt); break;
case kBytesInType: (void) fputs (Tree_NodeName [kBytesInType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeBytesInType (yyt); break;
case kBytesInExpr: (void) fputs (Tree_NodeName [kBytesInExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeBytesInExpr (yyt); break;
case kSizeOf: (void) fputs (Tree_NodeName [kSizeOf], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSizeOf (yyt); break;
case kOffsetOf: (void) fputs (Tree_NodeName [kOffsetOf], yyf); (void) fputc ('\n', yyf);
 yWriteNodeOffsetOf (yyt); break;
case kStringElement: (void) fputs (Tree_NodeName [kStringElement], yyf); (void) fputc ('\n', yyf);
 yWriteNodeStringElement (yyt); break;
case kString: (void) fputs (Tree_NodeName [kString], yyf); (void) fputc ('\n', yyf);
 yWriteNodeString (yyt); break;
case kStringSubscript: (void) fputs (Tree_NodeName [kStringSubscript], yyf); (void) fputc ('\n', yyf);
 yWriteNodeStringSubscript (yyt); break;
case kStringRange: (void) fputs (Tree_NodeName [kStringRange], yyf); (void) fputc ('\n', yyf);
 yWriteNodeStringRange (yyt); break;
case kElemental: (void) fputs (Tree_NodeName [kElemental], yyf); (void) fputc ('\n', yyf);
 yWriteNodeElemental (yyt); break;
case kTableElement: (void) fputs (Tree_NodeName [kTableElement], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTableElement (yyt); break;
case kTable: (void) fputs (Tree_NodeName [kTable], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTable (yyt); break;
case kUserLiteral: (void) fputs (Tree_NodeName [kUserLiteral], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUserLiteral (yyt); break;
case kTableExpr: (void) fputs (Tree_NodeName [kTableExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTableExpr (yyt); break;
case kTableSubscript: (void) fputs (Tree_NodeName [kTableSubscript], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTableSubscript (yyt); break;
case kTableRange: (void) fputs (Tree_NodeName [kTableRange], yyf); (void) fputc ('\n', yyf);
 yWriteNodeTableRange (yyt); break;
case kElement: (void) fputs (Tree_NodeName [kElement], yyf); (void) fputc ('\n', yyf);
 yWriteNodeElement (yyt); break;
case kIdentifier: (void) fputs (Tree_NodeName [kIdentifier], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIdentifier (yyt); break;
case kUnmappedIdentifier: (void) fputs (Tree_NodeName [kUnmappedIdentifier], yyf); (void) fputc ('\n', yyf);
 yWriteNodeUnmappedIdentifier (yyt); break;
case kElementAccess: (void) fputs (Tree_NodeName [kElementAccess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeElementAccess (yyt); break;
case kSubscript: (void) fputs (Tree_NodeName [kSubscript], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSubscript (yyt); break;
case kRange: (void) fputs (Tree_NodeName [kRange], yyf); (void) fputc ('\n', yyf);
 yWriteNodeRange (yyt); break;
case kExprList: (void) fputs (Tree_NodeName [kExprList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeExprList (yyt); break;
case kNoExpr: (void) fputs (Tree_NodeName [kNoExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoExpr (yyt); break;
case kAnExpr: (void) fputs (Tree_NodeName [kAnExpr], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnExpr (yyt); break;
case kOp: (void) fputs (Tree_NodeName [kOp], yyf); (void) fputc ('\n', yyf);
 break;
case kRelOp: (void) fputs (Tree_NodeName [kRelOp], yyf); (void) fputc ('\n', yyf);
 break;
case kLess: (void) fputs (Tree_NodeName [kLess], yyf); (void) fputc ('\n', yyf);
 break;
case kGreater: (void) fputs (Tree_NodeName [kGreater], yyf); (void) fputc ('\n', yyf);
 break;
case kLessEqual: (void) fputs (Tree_NodeName [kLessEqual], yyf); (void) fputc ('\n', yyf);
 break;
case kGreaterEqual: (void) fputs (Tree_NodeName [kGreaterEqual], yyf); (void) fputc ('\n', yyf);
 break;
case kEqual: (void) fputs (Tree_NodeName [kEqual], yyf); (void) fputc ('\n', yyf);
 break;
case kNotEqual: (void) fputs (Tree_NodeName [kNotEqual], yyf); (void) fputc ('\n', yyf);
 break;
case kMathsOp: (void) fputs (Tree_NodeName [kMathsOp], yyf); (void) fputc ('\n', yyf);
 break;
case kAdd: (void) fputs (Tree_NodeName [kAdd], yyf); (void) fputc ('\n', yyf);
 break;
case kSubtract: (void) fputs (Tree_NodeName [kSubtract], yyf); (void) fputc ('\n', yyf);
 break;
case kMultiply: (void) fputs (Tree_NodeName [kMultiply], yyf); (void) fputc ('\n', yyf);
 break;
case kDivide: (void) fputs (Tree_NodeName [kDivide], yyf); (void) fputc ('\n', yyf);
 break;
case kRem: (void) fputs (Tree_NodeName [kRem], yyf); (void) fputc ('\n', yyf);
 break;
case kBitOp: (void) fputs (Tree_NodeName [kBitOp], yyf); (void) fputc ('\n', yyf);
 break;
case kBitAnd: (void) fputs (Tree_NodeName [kBitAnd], yyf); (void) fputc ('\n', yyf);
 break;
case kBitOr: (void) fputs (Tree_NodeName [kBitOr], yyf); (void) fputc ('\n', yyf);
 break;
case kBitEor: (void) fputs (Tree_NodeName [kBitEor], yyf); (void) fputc ('\n', yyf);
 break;
case kShiftOp: (void) fputs (Tree_NodeName [kShiftOp], yyf); (void) fputc ('\n', yyf);
 break;
case kLeftShift: (void) fputs (Tree_NodeName [kLeftShift], yyf); (void) fputc ('\n', yyf);
 break;
case kRightShift: (void) fputs (Tree_NodeName [kRightShift], yyf); (void) fputc ('\n', yyf);
 break;
case kUniOp: (void) fputs (Tree_NodeName [kUniOp], yyf); (void) fputc ('\n', yyf);
 break;
case kSize: (void) fputs (Tree_NodeName [kSize], yyf); (void) fputc ('\n', yyf);
 break;
case kBitNot: (void) fputs (Tree_NodeName [kBitNot], yyf); (void) fputc ('\n', yyf);
 break;
case kNot: (void) fputs (Tree_NodeName [kNot], yyf); (void) fputc ('\n', yyf);
 break;
case kClockOp: (void) fputs (Tree_NodeName [kClockOp], yyf); (void) fputc ('\n', yyf);
 break;
case kPlus: (void) fputs (Tree_NodeName [kPlus], yyf); (void) fputc ('\n', yyf);
 break;
case kMinus: (void) fputs (Tree_NodeName [kMinus], yyf); (void) fputc ('\n', yyf);
 break;
case kTimes: (void) fputs (Tree_NodeName [kTimes], yyf); (void) fputc ('\n', yyf);
 break;
case kLogOp: (void) fputs (Tree_NodeName [kLogOp], yyf); (void) fputc ('\n', yyf);
 break;
case kAnd: (void) fputs (Tree_NodeName [kAnd], yyf); (void) fputc ('\n', yyf);
 break;
case kOr: (void) fputs (Tree_NodeName [kOr], yyf); (void) fputc ('\n', yyf);
 break;
case kAfterOp: (void) fputs (Tree_NodeName [kAfterOp], yyf); (void) fputc ('\n', yyf);
 break;
case kProcess: (void) fputs (Tree_NodeName [kProcess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeProcess (yyt); break;
case kAssign: (void) fputs (Tree_NodeName [kAssign], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAssign (yyt); break;
case kAltType: (void) fputs (Tree_NodeName [kAltType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltType (yyt); break;
case kAlt: (void) fputs (Tree_NodeName [kAlt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAlt (yyt); break;
case kAltRep: (void) fputs (Tree_NodeName [kAltRep], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltRep (yyt); break;
case kParType: (void) fputs (Tree_NodeName [kParType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeParType (yyt); break;
case kPar: (void) fputs (Tree_NodeName [kPar], yyf); (void) fputc ('\n', yyf);
 yWriteNodePar (yyt); break;
case kParRep: (void) fputs (Tree_NodeName [kParRep], yyf); (void) fputc ('\n', yyf);
 yWriteNodeParRep (yyt); break;
case kProcessor: (void) fputs (Tree_NodeName [kProcessor], yyf); (void) fputc ('\n', yyf);
 yWriteNodeProcessor (yyt); break;
case kIfType: (void) fputs (Tree_NodeName [kIfType], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIfType (yyt); break;
case kIf: (void) fputs (Tree_NodeName [kIf], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIf (yyt); break;
case kIfRep: (void) fputs (Tree_NodeName [kIfRep], yyf); (void) fputc ('\n', yyf);
 yWriteNodeIfRep (yyt); break;
case kInput: (void) fputs (Tree_NodeName [kInput], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInput (yyt); break;
case kOutput: (void) fputs (Tree_NodeName [kOutput], yyf); (void) fputc ('\n', yyf);
 yWriteNodeOutput (yyt); break;
case kCaseIn: (void) fputs (Tree_NodeName [kCaseIn], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCaseIn (yyt); break;
case kAfter: (void) fputs (Tree_NodeName [kAfter], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAfter (yyt); break;
case kSeqRep: (void) fputs (Tree_NodeName [kSeqRep], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSeqRep (yyt); break;
case kSeq: (void) fputs (Tree_NodeName [kSeq], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSeq (yyt); break;
case kCase: (void) fputs (Tree_NodeName [kCase], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCase (yyt); break;
case kWhile: (void) fputs (Tree_NodeName [kWhile], yyf); (void) fputc ('\n', yyf);
 yWriteNodeWhile (yyt); break;
case kSkip: (void) fputs (Tree_NodeName [kSkip], yyf); (void) fputc ('\n', yyf);
 yWriteNodeSkip (yyt); break;
case kStop: (void) fputs (Tree_NodeName [kStop], yyf); (void) fputc ('\n', yyf);
 yWriteNodeStop (yyt); break;
case kInstance: (void) fputs (Tree_NodeName [kInstance], yyf); (void) fputc ('\n', yyf);
 yWriteNodeInstance (yyt); break;
case kCCode: (void) fputs (Tree_NodeName [kCCode], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCCode (yyt); break;
case kNullProcess: (void) fputs (Tree_NodeName [kNullProcess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNullProcess (yyt); break;
case kAltList: (void) fputs (Tree_NodeName [kAltList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltList (yyt); break;
case kNoAlt: (void) fputs (Tree_NodeName [kNoAlt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoAlt (yyt); break;
case kAnAlts: (void) fputs (Tree_NodeName [kAnAlts], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnAlts (yyt); break;
case kAnAlt: (void) fputs (Tree_NodeName [kAnAlt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnAlt (yyt); break;
case kANewAlt: (void) fputs (Tree_NodeName [kANewAlt], yyf); (void) fputc ('\n', yyf);
 yWriteNodeANewAlt (yyt); break;
case kGuard: (void) fputs (Tree_NodeName [kGuard], yyf); (void) fputc ('\n', yyf);
 yWriteNodeGuard (yyt); break;
case kAltInput: (void) fputs (Tree_NodeName [kAltInput], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltInput (yyt); break;
case kAltTimer: (void) fputs (Tree_NodeName [kAltTimer], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltTimer (yyt); break;
case kAltCaseIn: (void) fputs (Tree_NodeName [kAltCaseIn], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAltCaseIn (yyt); break;
case kNoInput: (void) fputs (Tree_NodeName [kNoInput], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoInput (yyt); break;
case kChoiceList: (void) fputs (Tree_NodeName [kChoiceList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeChoiceList (yyt); break;
case kNoChoice: (void) fputs (Tree_NodeName [kNoChoice], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoChoice (yyt); break;
case kAChoices: (void) fputs (Tree_NodeName [kAChoices], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAChoices (yyt); break;
case kAChoice: (void) fputs (Tree_NodeName [kAChoice], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAChoice (yyt); break;
case kANewIf: (void) fputs (Tree_NodeName [kANewIf], yyf); (void) fputc ('\n', yyf);
 yWriteNodeANewIf (yyt); break;
case kProcList: (void) fputs (Tree_NodeName [kProcList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeProcList (yyt); break;
case kNoProcess: (void) fputs (Tree_NodeName [kNoProcess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoProcess (yyt); break;
case kAProcess: (void) fputs (Tree_NodeName [kAProcess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAProcess (yyt); break;
case kCaseList: (void) fputs (Tree_NodeName [kCaseList], yyf); (void) fputc ('\n', yyf);
 yWriteNodeCaseList (yyt); break;
case kNoCase: (void) fputs (Tree_NodeName [kNoCase], yyf); (void) fputc ('\n', yyf);
 yWriteNodeNoCase (yyt); break;
case kACase: (void) fputs (Tree_NodeName [kACase], yyf); (void) fputc ('\n', yyf);
 yWriteNodeACase (yyt); break;
case kAccessList: (void) fputs (Tree_NodeName [kAccessList], yyf); (void) fputc ('\n', yyf);
 break;
case kAnAccess: (void) fputs (Tree_NodeName [kAnAccess], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnAccess (yyt); break;
case kNoAccess: (void) fputs (Tree_NodeName [kNoAccess], yyf); (void) fputc ('\n', yyf);
 break;
case kElementList: (void) fputs (Tree_NodeName [kElementList], yyf); (void) fputc ('\n', yyf);
 break;
case kAnElement: (void) fputs (Tree_NodeName [kAnElement], yyf); (void) fputc ('\n', yyf);
 yWriteNodeAnElement (yyt); break;
case kNoElement: (void) fputs (Tree_NodeName [kNoElement], yyf); (void) fputc ('\n', yyf);
 break;
 default: ;
 }
}

static short yyIndentLevel;

void WriteTree
# if defined __STDC__ | defined __cplusplus
 (FILE * yyyf, tTree yyt)
# else
 (yyyf, yyt) FILE * yyyf; tTree yyt;
# endif
{
 short yySaveLevel = yyIndentLevel;
 yyf = yyyf;
 if (yyRecursionLevel ++ == 0) yyLabelCount = 0;
 yyMark (yyt);
 yyIndentLevel = 0;
 yyWriteTree (yyt);
 yyIndentLevel = yySaveLevel;
 yyRecursionLevel --;
}

static void yyIndentSelector
# if defined __STDC__ | defined __cplusplus
 (char * yys)
# else
 (yys) char * yys;
# endif
{
 register int yyi;
 for (yyi = 1; yyi <= yyIndentLevel; yyi ++) (void) putc (' ', yyf);
 yyWriteSelector (yys);
}

static void yyIndentSelectorTree
# if defined __STDC__ | defined __cplusplus
 (char * yys, tTree yyt)
# else
 (yys, yyt) char * yys; tTree yyt;
# endif
{ yyIndentSelector (yys); writetTree (yyt) }

static void yWriteStart
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kStart], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("CGNext"); writetChain (yyt->Start.CGNext) yyWriteNl ();
 yyIndentSelector ("OuterProc"); writetSymbol (yyt->Start.OuterProc) yyWriteNl ();
}

static void yWriteSpecList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSpecList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->SpecList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->SpecList.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->SpecList.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->SpecList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->SpecList.VOut) yyWriteNl ();
 yyIndentSelector ("DCL"); writeint (yyt->SpecList.DCL) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->SpecList.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->SpecList.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->SpecList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->SpecList.PUOut) yyWriteNl ();
}

static void yWriteNoSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoSpec], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoSpec.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoSpec.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoSpec.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoSpec.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoSpec.VOut) yyWriteNl ();
 yyIndentSelector ("DCL"); writeint (yyt->NoSpec.DCL) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoSpec.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->NoSpec.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoSpec.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoSpec.PUOut) yyWriteNl ();
}

static void yWriteASpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kASpec], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->ASpec.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ASpec.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ASpec.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ASpec.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ASpec.VOut) yyWriteNl ();
 yyIndentSelector ("DCL"); writeint (yyt->ASpec.DCL) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ASpec.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->ASpec.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ASpec.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ASpec.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Spec", yyt->ASpec.Spec);
}

static void yWriteSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSpec], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Spec.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Spec.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Spec.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Spec.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Spec.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Spec.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Spec.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Spec.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Spec.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Spec.PUOut) yyWriteNl ();
}

static void yWriteDeclType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDeclType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DeclType.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DeclType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DeclType.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DeclType.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DeclType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DeclType.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DeclType.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DeclType.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DeclType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DeclType.PUOut) yyWriteNl ();
}

static void yWriteDecl
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDecl], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Decl.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Decl.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Decl.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Decl.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Decl.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Decl.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Decl.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Decl.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Decl.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Decl.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Decl.Type);
}

static void yWriteIs
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIs], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Is.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Is.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Is.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Is.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Is.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Is.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Is.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Is.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Is.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Is.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Is.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Is.Sym) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Is.U) yyWriteNl ();
}

static void yWriteAVal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAVal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AVal.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->AVal.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AVal.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AVal.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AVal.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AVal.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AVal.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->AVal.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AVal.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AVal.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->AVal.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->AVal.Sym) yyWriteNl ();
}

static void yWriteVal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kVal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Val.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Val.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Val.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Val.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Val.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Val.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Val.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Val.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Val.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Val.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Val.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Val.Sym) yyWriteNl ();
}

static void yWriteValRetypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kValRetypes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ValRetypes.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ValRetypes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ValRetypes.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ValRetypes.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ValRetypes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ValRetypes.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ValRetypes.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->ValRetypes.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ValRetypes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ValRetypes.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->ValRetypes.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->ValRetypes.Sym) yyWriteNl ();
}

static void yWriteValReshapes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kValReshapes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ValReshapes.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ValReshapes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ValReshapes.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ValReshapes.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ValReshapes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ValReshapes.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ValReshapes.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->ValReshapes.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ValReshapes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ValReshapes.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->ValReshapes.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->ValReshapes.Sym) yyWriteNl ();
}

static void yWriteRetypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kRetypes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Retypes.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Retypes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Retypes.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Retypes.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Retypes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Retypes.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Retypes.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Retypes.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Retypes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Retypes.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Retypes.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Retypes.Sym) yyWriteNl ();
}

static void yWriteReshapes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kReshapes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Reshapes.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Reshapes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Reshapes.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Reshapes.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Reshapes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Reshapes.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Reshapes.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Reshapes.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Reshapes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Reshapes.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Reshapes.Type);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Reshapes.Sym) yyWriteNl ();
}

static void yWriteDefAPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefAPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefAPro.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefAPro.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefAPro.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefAPro.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefAPro.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefAPro.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefAPro.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefAPro.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefAPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefAPro.PUOut) yyWriteNl ();
}

static void yWriteDefSeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefSeqPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefSeqPro.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefSeqPro.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefSeqPro.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefSeqPro.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefSeqPro.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefSeqPro.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefSeqPro.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefSeqPro.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefSeqPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefSeqPro.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefSeqPro.Sym) yyWriteNl ();
}

static void yWriteDefTagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefTagPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefTagPro.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefTagPro.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefTagPro.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefTagPro.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefTagPro.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefTagPro.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefTagPro.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefTagPro.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefTagPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefTagPro.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefTagPro.Sym) yyWriteNl ();
}

static void yWriteDefAProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefAProc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefAProc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefAProc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefAProc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefAProc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefAProc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefAProc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefAProc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefAProc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefAProc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefAProc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefAProc.Sym) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->DefAProc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->DefAProc.CGNext) yyWriteNl ();
}

static void yWriteDefProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefProc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefProc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefProc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefProc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefProc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefProc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefProc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefProc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefProc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefProc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefProc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefProc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("Formals", yyt->DefProc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->DefProc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->DefProc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->DefProc.ATOMIC) yyWriteNl ();
 yyIndentSelector ("isPar"); writebool (yyt->DefProc.isPar) yyWriteNl ();
}

static void yWriteInlineDefProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInlineDefProc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->InlineDefProc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->InlineDefProc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->InlineDefProc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->InlineDefProc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->InlineDefProc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->InlineDefProc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->InlineDefProc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->InlineDefProc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->InlineDefProc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->InlineDefProc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->InlineDefProc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("Formals", yyt->InlineDefProc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->InlineDefProc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->InlineDefProc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->InlineDefProc.ATOMIC) yyWriteNl ();
 yyIndentSelector ("isPar"); writebool (yyt->InlineDefProc.isPar) yyWriteNl ();
}

static void yWritePrototypeProc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kPrototypeProc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->PrototypeProc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->PrototypeProc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->PrototypeProc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->PrototypeProc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->PrototypeProc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->PrototypeProc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->PrototypeProc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->PrototypeProc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->PrototypeProc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->PrototypeProc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->PrototypeProc.Sym) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->PrototypeProc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->PrototypeProc.CGNext) yyWriteNl ();
 yyIndentSelector ("Module"); writetIdent (yyt->PrototypeProc.Module) yyWriteNl ();
 yyIndentSelector ("Lang"); writetLang (yyt->PrototypeProc.Lang) yyWriteNl ();
}

static void yWriteDefAFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefAFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefAFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefAFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefAFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefAFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefAFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefAFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefAFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefAFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefAFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefAFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefAFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->DefAFunc.TypeList);
 yyIndentSelector ("U"); writetUsage (yyt->DefAFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->DefAFunc.CGNext) yyWriteNl ();
}

static void yWriteDefFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->DefFunc.TypeList);
 yyIndentSelectorTree ("Formals", yyt->DefFunc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->DefFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->DefFunc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->DefFunc.ATOMIC) yyWriteNl ();
}

static void yWriteInlineDefFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInlineDefFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->InlineDefFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->InlineDefFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->InlineDefFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->InlineDefFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->InlineDefFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->InlineDefFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->InlineDefFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->InlineDefFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->InlineDefFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->InlineDefFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->InlineDefFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->InlineDefFunc.TypeList);
 yyIndentSelectorTree ("Formals", yyt->InlineDefFunc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->InlineDefFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->InlineDefFunc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->InlineDefFunc.ATOMIC) yyWriteNl ();
}

static void yWriteDefIsFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefIsFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->DefIsFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->DefIsFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->DefIsFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->DefIsFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->DefIsFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->DefIsFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->DefIsFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->DefIsFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->DefIsFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->DefIsFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->DefIsFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->DefIsFunc.TypeList);
 yyIndentSelectorTree ("Formals", yyt->DefIsFunc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->DefIsFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->DefIsFunc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->DefIsFunc.ATOMIC) yyWriteNl ();
}

static void yWriteInlineDefIsFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInlineDefIsFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->InlineDefIsFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->InlineDefIsFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->InlineDefIsFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->InlineDefIsFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->InlineDefIsFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->InlineDefIsFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->InlineDefIsFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->InlineDefIsFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->InlineDefIsFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->InlineDefIsFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->InlineDefIsFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->InlineDefIsFunc.TypeList);
 yyIndentSelectorTree ("Formals", yyt->InlineDefIsFunc.Formals);
 yyIndentSelector ("U"); writetUsage (yyt->InlineDefIsFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->InlineDefIsFunc.CGNext) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->InlineDefIsFunc.ATOMIC) yyWriteNl ();
}

static void yWritePrototypeFunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kPrototypeFunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->PrototypeFunc.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->PrototypeFunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->PrototypeFunc.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->PrototypeFunc.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->PrototypeFunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->PrototypeFunc.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->PrototypeFunc.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->PrototypeFunc.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->PrototypeFunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->PrototypeFunc.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->PrototypeFunc.Sym) yyWriteNl ();
 yyIndentSelectorTree ("TypeList", yyt->PrototypeFunc.TypeList);
 yyIndentSelector ("U"); writetUsage (yyt->PrototypeFunc.U) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->PrototypeFunc.CGNext) yyWriteNl ();
 yyIndentSelector ("Module"); writetIdent (yyt->PrototypeFunc.Module) yyWriteNl ();
 yyIndentSelector ("Lang"); writetLang (yyt->PrototypeFunc.Lang) yyWriteNl ();
}

static void yWriteNullSpec
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNullSpec], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->NullSpec.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->NullSpec.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NullSpec.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NullSpec.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NullSpec.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NullSpec.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NullSpec.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->NullSpec.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NullSpec.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NullSpec.PUOut) yyWriteNl ();
}

static void yWriteHCode
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kHCode], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->HCode.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->HCode.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->HCode.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->HCode.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->HCode.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->HCode.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->HCode.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->HCode.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->HCode.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->HCode.PUOut) yyWriteNl ();
 yyIndentSelector ("Code"); writetStringRef (yyt->HCode.Code) yyWriteNl ();
 yyIndentSelector ("Scope"); writetSymbol (yyt->HCode.Scope) yyWriteNl ();
 yyIndentSelector ("CGNext"); writetChain (yyt->HCode.CGNext) yyWriteNl ();
}

static void yWriteCallFrame
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCallFrame], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->CallFrame.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->CallFrame.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->CallFrame.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->CallFrame.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->CallFrame.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->CallFrame.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->CallFrame.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->CallFrame.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->CallFrame.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->CallFrame.PUOut) yyWriteNl ();
 yyIndentSelector ("Id"); writetIdent (yyt->CallFrame.Id) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->CallFrame.Sym) yyWriteNl ();
}

static void yWriteParFrame
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kParFrame], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ParFrame.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ParFrame.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ParFrame.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ParFrame.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ParFrame.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ParFrame.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ParFrame.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->ParFrame.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ParFrame.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ParFrame.PUOut) yyWriteNl ();
 yyIndentSelector ("TId"); writetIdent (yyt->ParFrame.TId) yyWriteNl ();
 yyIndentSelector ("FId"); writetIdent (yyt->ParFrame.FId) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->ParFrame.Sym) yyWriteNl ();
 yyIndentSelector ("Rep"); writeint (yyt->ParFrame.Rep) yyWriteNl ();
}

static void yWriteFormals
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kFormals], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Formals.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Formals.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Formals.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Formals.VOut) yyWriteNl ();
 yyIndentSelector ("InPrototype"); writebool (yyt->Formals.InPrototype) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Formals.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Formals.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Formals.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Formals.PUOut) yyWriteNl ();
}

static void yWriteNoFormal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoFormal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoFormal.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoFormal.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoFormal.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoFormal.VOut) yyWriteNl ();
 yyIndentSelector ("InPrototype"); writebool (yyt->NoFormal.InPrototype) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoFormal.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->NoFormal.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoFormal.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoFormal.PUOut) yyWriteNl ();
}

static void yWriteNoFormals
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoFormals], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoFormals.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoFormals.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoFormals.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoFormals.VOut) yyWriteNl ();
 yyIndentSelector ("InPrototype"); writebool (yyt->NoFormals.InPrototype) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoFormals.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->NoFormals.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoFormals.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoFormals.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->NoFormals.Sym) yyWriteNl ();
}

static void yWriteAFormal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAFormal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AFormal.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AFormal.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AFormal.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AFormal.VOut) yyWriteNl ();
 yyIndentSelector ("InPrototype"); writebool (yyt->AFormal.InPrototype) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AFormal.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->AFormal.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AFormal.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AFormal.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->AFormal.Next);
 yyIndentSelector ("Pos"); writetPosition (yyt->AFormal.Pos) yyWriteNl ();
 yyIndentSelectorTree ("By", yyt->AFormal.By);
 yyIndentSelector ("Sym"); writetSymbol (yyt->AFormal.Sym) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->AFormal.U) yyWriteNl ();
}

static void yWriteReplicator
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kReplicator], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Replicator.Sym) yyWriteNl ();
 yyIndentSelectorTree ("From", yyt->Replicator.From);
 yyIndentSelector ("V1In"); writeint (yyt->Replicator.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Replicator.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Replicator.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Replicator.VOut) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Replicator.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->Replicator.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Replicator.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Replicator.PUOut) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Replicator.SE) yyWriteNl ();
 yyIndentSelector ("FromLE"); writepLinExpr (yyt->Replicator.FromLE) yyWriteNl ();
 yyIndentSelector ("ForLE"); writepLinExpr (yyt->Replicator.ForLE) yyWriteNl ();
}

static void yWriteTagProList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTagProList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TagProList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TagProList.PUOut) yyWriteNl ();
}

static void yWriteNoTagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoTagPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoTagPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoTagPro.PUOut) yyWriteNl ();
}

static void yWriteATagPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kATagPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ATagPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ATagPro.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->ATagPro.Sym) yyWriteNl ();
 yyIndentSelectorTree ("SeqProList", yyt->ATagPro.SeqProList);
}

static void yWriteIdList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIdList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Parent"); writetSymbol (yyt->IdList.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->IdList.AtomicSpec) yyWriteNl ();
}

static void yWriteNoId
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoId], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoId.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->NoId.AtomicSpec) yyWriteNl ();
}

static void yWriteAnId
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnId], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Parent"); writetSymbol (yyt->AnId.Parent) yyWriteNl ();
 yyIndentSelector ("AtomicSpec"); writebool (yyt->AnId.AtomicSpec) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->AnId.Sym) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->AnId.U) yyWriteNl ();
}

static void yWriteType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Type.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Type.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Type.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Type.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Type.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Type.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Type.PUOut) yyWriteNl ();
}

static void yWriteArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kArray], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Array.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Array.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Array.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Array.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Array.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Array.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Array.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->Array.Expr);
 yyIndentSelector ("SE"); writebool (yyt->Array.SE) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->Array.LE) yyWriteNl ();
}

static void yWriteUserArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserArray], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserArray.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserArray.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserArray.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserArray.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserArray.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserArray.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserArray.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->UserArray.Expr);
 yyIndentSelector ("SE"); writebool (yyt->UserArray.SE) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->UserArray.LE) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserArray.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserArray.Sym) yyWriteNl ();
}

static void yWriteChan
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kChan], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Chan.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Chan.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Chan.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Chan.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Chan.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Chan.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Chan.PUOut) yyWriteNl ();
}

static void yWriteTimer
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTimer], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Timer.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Timer.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Timer.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Timer.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Timer.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Timer.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Timer.PUOut) yyWriteNl ();
}

static void yWriteUntyped
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntyped], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Untyped.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Untyped.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Untyped.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Untyped.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Untyped.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Untyped.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Untyped.PUOut) yyWriteNl ();
}

static void yWriteUntypedByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntypedByte], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UntypedByte.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UntypedByte.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UntypedByte.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UntypedByte.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UntypedByte.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UntypedByte.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UntypedByte.PUOut) yyWriteNl ();
}

static void yWriteUntypedReal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntypedReal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UntypedReal.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UntypedReal.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UntypedReal.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UntypedReal.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UntypedReal.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UntypedReal.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UntypedReal.PUOut) yyWriteNl ();
}

static void yWriteUntypedInteger
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntypedInteger], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UntypedInteger.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UntypedInteger.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UntypedInteger.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UntypedInteger.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UntypedInteger.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UntypedInteger.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UntypedInteger.PUOut) yyWriteNl ();
}

static void yWriteUntypedInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntypedInt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UntypedInt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UntypedInt.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UntypedInt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UntypedInt.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UntypedInt.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UntypedInt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UntypedInt.PUOut) yyWriteNl ();
}

static void yWriteUntypedHex
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUntypedHex], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UntypedHex.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UntypedHex.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UntypedHex.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UntypedHex.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UntypedHex.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UntypedHex.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UntypedHex.PUOut) yyWriteNl ();
}

static void yWritePrimitive
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kPrimitive], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Primitive.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Primitive.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Primitive.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Primitive.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Primitive.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Primitive.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Primitive.PUOut) yyWriteNl ();
}

static void yWriteNumeric
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNumeric], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Numeric.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Numeric.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Numeric.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Numeric.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Numeric.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Numeric.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Numeric.PUOut) yyWriteNl ();
}

static void yWriteInteger
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInteger], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Integer.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Integer.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Integer.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Integer.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Integer.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Integer.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Integer.PUOut) yyWriteNl ();
}

static void yWriteInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Int.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Int.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Int.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Int.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Int.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Int.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Int.PUOut) yyWriteNl ();
}

static void yWriteUserInt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserInt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserInt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserInt.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserInt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserInt.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserInt.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserInt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserInt.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserInt.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserInt.Sym) yyWriteNl ();
}

static void yWriteInt16
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInt16], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Int16.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Int16.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Int16.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Int16.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Int16.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Int16.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Int16.PUOut) yyWriteNl ();
}

static void yWriteUserInt16
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserInt16], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserInt16.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserInt16.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserInt16.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserInt16.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserInt16.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserInt16.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserInt16.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserInt16.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserInt16.Sym) yyWriteNl ();
}

static void yWriteInt32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInt32], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Int32.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Int32.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Int32.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Int32.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Int32.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Int32.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Int32.PUOut) yyWriteNl ();
}

static void yWriteUserInt32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserInt32], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserInt32.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserInt32.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserInt32.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserInt32.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserInt32.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserInt32.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserInt32.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserInt32.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserInt32.Sym) yyWriteNl ();
}

static void yWriteInt64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInt64], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Int64.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Int64.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Int64.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Int64.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Int64.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Int64.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Int64.PUOut) yyWriteNl ();
}

static void yWriteUserInt64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserInt64], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserInt64.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserInt64.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserInt64.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserInt64.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserInt64.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserInt64.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserInt64.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserInt64.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserInt64.Sym) yyWriteNl ();
}

static void yWriteReal
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kReal], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Real.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Real.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Real.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Real.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Real.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Real.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Real.PUOut) yyWriteNl ();
}

static void yWriteReal32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kReal32], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Real32.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Real32.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Real32.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Real32.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Real32.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Real32.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Real32.PUOut) yyWriteNl ();
}

static void yWriteUserReal32
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserReal32], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserReal32.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserReal32.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserReal32.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserReal32.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserReal32.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserReal32.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserReal32.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserReal32.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserReal32.Sym) yyWriteNl ();
}

static void yWriteReal64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kReal64], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Real64.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Real64.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Real64.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Real64.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Real64.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Real64.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Real64.PUOut) yyWriteNl ();
}

static void yWriteUserReal64
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserReal64], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserReal64.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserReal64.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserReal64.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserReal64.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserReal64.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserReal64.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserReal64.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserReal64.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserReal64.Sym) yyWriteNl ();
}

static void yWriteByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kByte], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Byte.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Byte.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Byte.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Byte.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Byte.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Byte.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Byte.PUOut) yyWriteNl ();
}

static void yWriteUserByte
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserByte], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserByte.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserByte.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserByte.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserByte.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserByte.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserByte.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserByte.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserByte.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserByte.Sym) yyWriteNl ();
}

static void yWriteBool
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kBool], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->Bool.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Bool.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Bool.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Bool.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->Bool.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Bool.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Bool.PUOut) yyWriteNl ();
}

static void yWriteUserBool
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserBool], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserBool.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserBool.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserBool.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserBool.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserBool.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserBool.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserBool.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserBool.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserBool.Sym) yyWriteNl ();
}

static void yWriteUserStructTypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserStructTypes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserStructTypes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserStructTypes.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserStructTypes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserStructTypes.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserStructTypes.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserStructTypes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserStructTypes.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserStructTypes.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserStructTypes.Sym) yyWriteNl ();
}

static void yWriteUserDataTypes
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserDataTypes], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserDataTypes.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserDataTypes.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserDataTypes.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserDataTypes.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserDataTypes.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserDataTypes.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserDataTypes.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserDataTypes.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserDataTypes.Sym) yyWriteNl ();
}

static void yWriteUserRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserRecord], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserRecord.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserRecord.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserRecord.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserRecord.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserRecord.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserRecord.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserRecord.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserRecord.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserRecord.Sym) yyWriteNl ();
}

static void yWriteUserPackedRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserPackedRecord], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserPackedRecord.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserPackedRecord.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserPackedRecord.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserPackedRecord.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserPackedRecord.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserPackedRecord.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserPackedRecord.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserPackedRecord.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserPackedRecord.Sym) yyWriteNl ();
}

static void yWriteUserUnion
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserUnion], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserUnion.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserUnion.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserUnion.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserUnion.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserUnion.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserUnion.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserUnion.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserUnion.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserUnion.Sym) yyWriteNl ();
}

static void yWriteUserChanRecord
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserChanRecord], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->UserChanRecord.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserChanRecord.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserChanRecord.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserChanRecord.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->UserChanRecord.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserChanRecord.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserChanRecord.PUOut) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->UserChanRecord.Pos) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserChanRecord.Sym) yyWriteNl ();
}

static void yWriteTypeList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTypeList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->TypeList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->TypeList.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->TypeList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->TypeList.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->TypeList.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TypeList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TypeList.PUOut) yyWriteNl ();
}

static void yWriteNoType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoType.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoType.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->NoType.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoType.PUOut) yyWriteNl ();
}

static void yWriteAType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AType.V1Out) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AType.VOut) yyWriteNl ();
 yyIndentSelector ("D"); writeint (yyt->AType.D) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AType.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->AType.Type);
}

static void yWriteFieldList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kFieldList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->FieldList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->FieldList.PUOut) yyWriteNl ();
}

static void yWriteNoField
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoField], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoField.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoField.PUOut) yyWriteNl ();
}

static void yWriteAField
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAField], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AField.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AField.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->AField.Sym) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->AField.Type);
}

static void yWriteProtocol
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kProtocol], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Protocol.Pos) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Protocol.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Protocol.PUOut) yyWriteNl ();
}

static void yWriteAny
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAny], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Any.Pos) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Any.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Any.PUOut) yyWriteNl ();
}

static void yWriteNamed
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNamed], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Named.Pos) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Named.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Named.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->Named.Sym) yyWriteNl ();
}

static void yWriteSimple
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSimple], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Simple.Pos) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Simple.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Simple.PUOut) yyWriteNl ();
}

static void yWriteCounted
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCounted], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Counted.Pos) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Counted.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Counted.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Length", yyt->Counted.Length);
}

static void yWriteSeqProList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSeqProList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->SeqProList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->SeqProList.PUOut) yyWriteNl ();
}

static void yWriteNoSeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoSeqPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoSeqPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoSeqPro.PUOut) yyWriteNl ();
}

static void yWriteASeqPro
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kASeqPro], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ASeqPro.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ASeqPro.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Protocol", yyt->ASeqPro.Protocol);
}

static void yWriteExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Expr.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Expr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Expr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Expr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Expr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Expr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Expr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Expr.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Expr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Expr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Expr.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Expr.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Expr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Expr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Expr.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Expr.LV) yyWriteNl ();
}

static void yWriteBinary
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kBinary], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Binary.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Binary.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Binary.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Binary.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Binary.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Binary.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Binary.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Binary.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Binary.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Binary.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Binary.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Binary.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Binary.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Binary.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Binary.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Binary.LV) yyWriteNl ();
 yyIndentSelectorTree ("Lop", yyt->Binary.Lop);
 yyIndentSelectorTree ("Rop", yyt->Binary.Rop);
}

static void yWriteUnary
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUnary], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Unary.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Unary.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Unary.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Unary.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Unary.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Unary.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Unary.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Unary.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Unary.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Unary.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Unary.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Unary.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Unary.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Unary.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Unary.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Unary.LV) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->Unary.Expr);
}

static void yWriteConst
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kConst], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Const.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Const.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Const.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Const.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Const.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Const.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Const.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Const.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Const.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Const.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Const.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Const.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Const.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Const.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Const.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Const.LV) yyWriteNl ();
 yyIndentSelector ("Value"); writetValue (yyt->Const.Value) yyWriteNl ();
}

static void yWriteValof
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kValof], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Valof.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Valof.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Valof.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Valof.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Valof.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Valof.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Valof.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Valof.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Valof.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Valof.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Valof.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Valof.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Valof.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Valof.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Valof.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Valof.LV) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Valof.SpecList);
 yyIndentSelectorTree ("Process", yyt->Valof.Process);
 yyIndentSelector ("Sym"); writetSymbol (yyt->Valof.Sym) yyWriteNl ();
 yyIndentSelector ("FuncCall"); writetTree (yyt->Valof.FuncCall) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Valof.VS) yyWriteNl ();
}

static void yWriteFuncCall
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kFuncCall], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->FuncCall.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->FuncCall.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->FuncCall.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->FuncCall.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->FuncCall.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->FuncCall.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->FuncCall.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->FuncCall.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->FuncCall.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->FuncCall.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->FuncCall.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->FuncCall.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->FuncCall.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->FuncCall.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->FuncCall.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->FuncCall.LV) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->FuncCall.Sym) yyWriteNl ();
 yyIndentSelector ("Depth"); writeint (yyt->FuncCall.Depth) yyWriteNl ();
 yyIndentSelector ("Frame"); writetFrame (yyt->FuncCall.Frame) yyWriteNl ();
}

static void yWriteElementalExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kElementalExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ElementalExpr.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->ElementalExpr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->ElementalExpr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->ElementalExpr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ElementalExpr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ElementalExpr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->ElementalExpr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ElementalExpr.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ElementalExpr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ElementalExpr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->ElementalExpr.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->ElementalExpr.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ElementalExpr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ElementalExpr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ElementalExpr.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->ElementalExpr.LV) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->ElementalExpr.PAR) yyWriteNl ();
}

static void yWriteDefault
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kDefault], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Default.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Default.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Default.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Default.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Default.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Default.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Default.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Default.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Default.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Default.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Default.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Default.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Default.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Default.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Default.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Default.LV) yyWriteNl ();
}

static void yWriteConvert
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kConvert], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Convert.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Convert.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Convert.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Convert.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Convert.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Convert.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Convert.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Convert.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Convert.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Convert.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Convert.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Convert.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Convert.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Convert.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Convert.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Convert.LV) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Convert.Type);
}

static void yWriteRound
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kRound], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Round.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Round.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Round.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Round.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Round.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Round.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Round.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Round.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Round.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Round.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Round.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Round.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Round.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Round.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Round.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Round.LV) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Round.Type);
}

static void yWriteTrunc
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTrunc], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Trunc.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Trunc.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Trunc.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Trunc.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Trunc.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Trunc.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Trunc.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Trunc.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Trunc.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Trunc.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Trunc.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Trunc.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Trunc.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Trunc.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Trunc.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Trunc.LV) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->Trunc.Type);
}

static void yWriteMostPos
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kMostPos], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->MostPos.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->MostPos.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->MostPos.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->MostPos.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->MostPos.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->MostPos.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->MostPos.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->MostPos.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->MostPos.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->MostPos.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->MostPos.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->MostPos.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->MostPos.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->MostPos.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->MostPos.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->MostPos.LV) yyWriteNl ();
}

static void yWriteMostNeg
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kMostNeg], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->MostNeg.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->MostNeg.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->MostNeg.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->MostNeg.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->MostNeg.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->MostNeg.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->MostNeg.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->MostNeg.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->MostNeg.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->MostNeg.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->MostNeg.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->MostNeg.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->MostNeg.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->MostNeg.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->MostNeg.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->MostNeg.LV) yyWriteNl ();
}

static void yWriteCountArray
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCountArray], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->CountArray.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->CountArray.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->CountArray.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->CountArray.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->CountArray.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->CountArray.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->CountArray.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->CountArray.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->CountArray.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->CountArray.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->CountArray.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->CountArray.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->CountArray.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->CountArray.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->CountArray.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->CountArray.LV) yyWriteNl ();
 yyIndentSelectorTree ("Length", yyt->CountArray.Length);
}

static void yWriteUnBound
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUnBound], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->UnBound.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->UnBound.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->UnBound.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->UnBound.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->UnBound.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UnBound.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->UnBound.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->UnBound.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UnBound.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UnBound.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->UnBound.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->UnBound.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->UnBound.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UnBound.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UnBound.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->UnBound.LV) yyWriteNl ();
}

static void yWriteBytesIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kBytesIn], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->BytesIn.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->BytesIn.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->BytesIn.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->BytesIn.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->BytesIn.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->BytesIn.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->BytesIn.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->BytesIn.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->BytesIn.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->BytesIn.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->BytesIn.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->BytesIn.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->BytesIn.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->BytesIn.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->BytesIn.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->BytesIn.LV) yyWriteNl ();
}

static void yWriteBytesInType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kBytesInType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->BytesInType.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->BytesInType.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->BytesInType.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->BytesInType.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->BytesInType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->BytesInType.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->BytesInType.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->BytesInType.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->BytesInType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->BytesInType.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->BytesInType.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->BytesInType.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->BytesInType.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->BytesInType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->BytesInType.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->BytesInType.LV) yyWriteNl ();
}

static void yWriteBytesInExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kBytesInExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->BytesInExpr.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->BytesInExpr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->BytesInExpr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->BytesInExpr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->BytesInExpr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->BytesInExpr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->BytesInExpr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->BytesInExpr.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->BytesInExpr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->BytesInExpr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->BytesInExpr.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->BytesInExpr.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->BytesInExpr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->BytesInExpr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->BytesInExpr.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->BytesInExpr.LV) yyWriteNl ();
}

static void yWriteSizeOf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSizeOf], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->SizeOf.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->SizeOf.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->SizeOf.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->SizeOf.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->SizeOf.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->SizeOf.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->SizeOf.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->SizeOf.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->SizeOf.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->SizeOf.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->SizeOf.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->SizeOf.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->SizeOf.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->SizeOf.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->SizeOf.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->SizeOf.LV) yyWriteNl ();
}

static void yWriteOffsetOf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kOffsetOf], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->OffsetOf.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->OffsetOf.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->OffsetOf.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->OffsetOf.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->OffsetOf.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->OffsetOf.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->OffsetOf.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->OffsetOf.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->OffsetOf.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->OffsetOf.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->OffsetOf.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->OffsetOf.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->OffsetOf.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->OffsetOf.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->OffsetOf.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->OffsetOf.LV) yyWriteNl ();
 yyIndentSelectorTree ("Type", yyt->OffsetOf.Type);
}

static void yWriteStringElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kStringElement], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->StringElement.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->StringElement.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->StringElement.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->StringElement.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->StringElement.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->StringElement.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->StringElement.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->StringElement.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->StringElement.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->StringElement.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->StringElement.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->StringElement.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->StringElement.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->StringElement.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->StringElement.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->StringElement.LV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->StringElement.CA) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->StringElement.SE) yyWriteNl ();
}

static void yWriteString
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kString], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->String.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->String.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->String.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->String.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->String.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->String.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->String.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->String.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->String.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->String.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->String.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->String.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->String.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->String.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->String.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->String.LV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->String.CA) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->String.SE) yyWriteNl ();
 yyIndentSelector ("Value"); writetStringRef (yyt->String.Value) yyWriteNl ();
 yyIndentSelector ("Depth"); writeint (yyt->String.Depth) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->String.Sym) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->String.LE) yyWriteNl ();
}

static void yWriteStringSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kStringSubscript], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->StringSubscript.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->StringSubscript.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->StringSubscript.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->StringSubscript.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->StringSubscript.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->StringSubscript.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->StringSubscript.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->StringSubscript.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->StringSubscript.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->StringSubscript.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->StringSubscript.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->StringSubscript.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->StringSubscript.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->StringSubscript.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->StringSubscript.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->StringSubscript.LV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->StringSubscript.CA) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->StringSubscript.SE) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->StringSubscript.Next);
 yyIndentSelector ("LE"); writepLinExpr (yyt->StringSubscript.LE) yyWriteNl ();
}

static void yWriteStringRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kStringRange], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->StringRange.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->StringRange.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->StringRange.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->StringRange.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->StringRange.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->StringRange.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->StringRange.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->StringRange.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->StringRange.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->StringRange.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->StringRange.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->StringRange.CV) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->StringRange.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->StringRange.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->StringRange.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->StringRange.LV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->StringRange.CA) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->StringRange.SE) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->StringRange.Next);
 yyIndentSelectorTree ("From", yyt->StringRange.From);
 yyIndentSelector ("FromLE"); writepLinExpr (yyt->StringRange.FromLE) yyWriteNl ();
 yyIndentSelector ("ForLE"); writepLinExpr (yyt->StringRange.ForLE) yyWriteNl ();
}

static void yWriteElemental
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kElemental], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Elemental.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Elemental.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Elemental.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Elemental.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Elemental.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Elemental.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Elemental.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Elemental.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Elemental.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Elemental.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Elemental.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Elemental.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Elemental.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Elemental.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Elemental.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Elemental.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Elemental.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Elemental.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Elemental.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Elemental.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Elemental.RVL) yyWriteNl ();
}

static void yWriteTableElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTableElement], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->TableElement.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->TableElement.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->TableElement.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->TableElement.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->TableElement.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->TableElement.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->TableElement.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->TableElement.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->TableElement.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->TableElement.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->TableElement.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->TableElement.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->TableElement.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->TableElement.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->TableElement.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->TableElement.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TableElement.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TableElement.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->TableElement.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->TableElement.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->TableElement.RVL) yyWriteNl ();
}

static void yWriteTable
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTable], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Table.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Table.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Table.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Table.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Table.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Table.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Table.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Table.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Table.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Table.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Table.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Table.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Table.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Table.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Table.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Table.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Table.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Table.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Table.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Table.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Table.RVL) yyWriteNl ();
 yyIndentSelector ("Depth"); writeint (yyt->Table.Depth) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->Table.Sym) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->Table.LE) yyWriteNl ();
}

static void yWriteUserLiteral
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUserLiteral], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->UserLiteral.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->UserLiteral.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->UserLiteral.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->UserLiteral.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->UserLiteral.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UserLiteral.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->UserLiteral.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->UserLiteral.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->UserLiteral.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UserLiteral.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UserLiteral.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->UserLiteral.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->UserLiteral.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->UserLiteral.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->UserLiteral.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->UserLiteral.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UserLiteral.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UserLiteral.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->UserLiteral.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->UserLiteral.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->UserLiteral.RVL) yyWriteNl ();
 yyIndentSelectorTree ("ExprList", yyt->UserLiteral.ExprList);
 yyIndentSelector ("Depth"); writeint (yyt->UserLiteral.Depth) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UserLiteral.Sym) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->UserLiteral.LE) yyWriteNl ();
}

static void yWriteTableExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTableExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->TableExpr.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->TableExpr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->TableExpr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->TableExpr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->TableExpr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->TableExpr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->TableExpr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->TableExpr.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->TableExpr.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->TableExpr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->TableExpr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->TableExpr.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->TableExpr.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->TableExpr.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->TableExpr.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->TableExpr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TableExpr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TableExpr.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->TableExpr.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->TableExpr.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->TableExpr.RVL) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->TableExpr.Sym) yyWriteNl ();
}

static void yWriteTableSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTableSubscript], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->TableSubscript.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->TableSubscript.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->TableSubscript.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->TableSubscript.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->TableSubscript.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->TableSubscript.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->TableSubscript.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->TableSubscript.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->TableSubscript.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->TableSubscript.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->TableSubscript.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->TableSubscript.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->TableSubscript.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->TableSubscript.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->TableSubscript.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->TableSubscript.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TableSubscript.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TableSubscript.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->TableSubscript.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->TableSubscript.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->TableSubscript.RVL) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->TableSubscript.Next);
 yyIndentSelector ("LE"); writepLinExpr (yyt->TableSubscript.LE) yyWriteNl ();
}

static void yWriteTableRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kTableRange], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->TableRange.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->TableRange.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->TableRange.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->TableRange.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->TableRange.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->TableRange.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->TableRange.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->TableRange.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->TableRange.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->TableRange.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->TableRange.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->TableRange.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->TableRange.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->TableRange.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->TableRange.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->TableRange.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->TableRange.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->TableRange.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->TableRange.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->TableRange.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->TableRange.RVL) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->TableRange.Next);
 yyIndentSelectorTree ("From", yyt->TableRange.From);
 yyIndentSelector ("FromLE"); writepLinExpr (yyt->TableRange.FromLE) yyWriteNl ();
 yyIndentSelector ("ForLE"); writepLinExpr (yyt->TableRange.ForLE) yyWriteNl ();
}

static void yWriteElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kElement], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Element.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Element.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Element.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Element.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Element.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Element.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Element.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Element.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Element.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Element.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Element.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Element.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Element.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Element.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Element.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Element.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Element.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Element.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Element.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Element.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Element.RVL) yyWriteNl ();
}

static void yWriteIdentifier
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIdentifier], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Identifier.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Identifier.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Identifier.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Identifier.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Identifier.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Identifier.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Identifier.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Identifier.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Identifier.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Identifier.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Identifier.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Identifier.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Identifier.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Identifier.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Identifier.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Identifier.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Identifier.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Identifier.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Identifier.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Identifier.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Identifier.RVL) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->Identifier.Sym) yyWriteNl ();
 yyIndentSelector ("Scope"); writetSymbol (yyt->Identifier.Scope) yyWriteNl ();
}

static void yWriteUnmappedIdentifier
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kUnmappedIdentifier], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->UnmappedIdentifier.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->UnmappedIdentifier.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->UnmappedIdentifier.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->UnmappedIdentifier.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->UnmappedIdentifier.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->UnmappedIdentifier.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->UnmappedIdentifier.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->UnmappedIdentifier.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->UnmappedIdentifier.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->UnmappedIdentifier.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->UnmappedIdentifier.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->UnmappedIdentifier.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->UnmappedIdentifier.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->UnmappedIdentifier.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->UnmappedIdentifier.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->UnmappedIdentifier.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->UnmappedIdentifier.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->UnmappedIdentifier.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->UnmappedIdentifier.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->UnmappedIdentifier.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->UnmappedIdentifier.RVL) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->UnmappedIdentifier.Sym) yyWriteNl ();
 yyIndentSelector ("Scope"); writetSymbol (yyt->UnmappedIdentifier.Scope) yyWriteNl ();
 yyIndentSelector ("Ident"); writetIdent (yyt->UnmappedIdentifier.Ident) yyWriteNl ();
}

static void yWriteElementAccess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kElementAccess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ElementAccess.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->ElementAccess.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->ElementAccess.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->ElementAccess.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ElementAccess.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ElementAccess.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->ElementAccess.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ElementAccess.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->ElementAccess.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ElementAccess.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ElementAccess.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->ElementAccess.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->ElementAccess.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->ElementAccess.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->ElementAccess.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ElementAccess.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ElementAccess.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ElementAccess.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->ElementAccess.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->ElementAccess.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->ElementAccess.RVL) yyWriteNl ();
}

static void yWriteSubscript
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSubscript], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Subscript.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Subscript.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Subscript.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Subscript.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Subscript.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Subscript.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Subscript.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Subscript.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Subscript.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Subscript.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Subscript.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Subscript.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Subscript.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Subscript.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Subscript.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Subscript.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Subscript.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Subscript.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Subscript.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Subscript.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Subscript.RVL) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->Subscript.Next);
 yyIndentSelector ("CB"); writebool (yyt->Subscript.CB) yyWriteNl ();
 yyIndentSelector ("LE"); writepLinExpr (yyt->Subscript.LE) yyWriteNl ();
}

static void yWriteRange
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kRange], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Range.Pos) yyWriteNl ();
 yyIndentSelector ("DefType"); writetType (yyt->Range.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->Range.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->Range.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Range.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Range.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->Range.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->Range.InhType) yyWriteNl ();
 yyIndentSelector ("W"); writeint (yyt->Range.W) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Range.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Range.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->Range.C) yyWriteNl ();
 yyIndentSelector ("CV"); writetValue (yyt->Range.CV) yyWriteNl ();
 yyIndentSelector ("CA"); writetConstArray (yyt->Range.CA) yyWriteNl ();
 yyIndentSelector ("U"); writetUsage (yyt->Range.U) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Range.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Range.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Range.PUOut) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->Range.LV) yyWriteNl ();
 yyIndentSelector ("SE"); writebool (yyt->Range.SE) yyWriteNl ();
 yyIndentSelector ("RVL"); writetTree (yyt->Range.RVL) yyWriteNl ();
 yyIndentSelectorTree ("Next", yyt->Range.Next);
 yyIndentSelectorTree ("From", yyt->Range.From);
 yyIndentSelector ("CLB"); writebool (yyt->Range.CLB) yyWriteNl ();
 yyIndentSelector ("CUB"); writebool (yyt->Range.CUB) yyWriteNl ();
 yyIndentSelector ("FromLE"); writepLinExpr (yyt->Range.FromLE) yyWriteNl ();
 yyIndentSelector ("ForLE"); writepLinExpr (yyt->Range.ForLE) yyWriteNl ();
}

static void yWriteExprList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kExprList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->ExprList.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->ExprList.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->ExprList.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ExprList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ExprList.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->ExprList.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ExprList.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ExprList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ExprList.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->ExprList.C) yyWriteNl ();
 yyIndentSelector ("TR"); writebool (yyt->ExprList.TR) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ExprList.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ExprList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ExprList.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->ExprList.PAR) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->ExprList.LV) yyWriteNl ();
}

static void yWriteNoExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->NoExpr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->NoExpr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->NoExpr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->NoExpr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoExpr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->NoExpr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->NoExpr.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoExpr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoExpr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->NoExpr.C) yyWriteNl ();
 yyIndentSelector ("TR"); writebool (yyt->NoExpr.TR) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoExpr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoExpr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoExpr.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->NoExpr.PAR) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->NoExpr.LV) yyWriteNl ();
}

static void yWriteAnExpr
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnExpr], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->AnExpr.DefType) yyWriteNl ();
 yyIndentSelector ("FLIn"); writetType (yyt->AnExpr.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->AnExpr.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->AnExpr.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AnExpr.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->AnExpr.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->AnExpr.InhType) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AnExpr.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AnExpr.VOut) yyWriteNl ();
 yyIndentSelector ("C"); writebool (yyt->AnExpr.C) yyWriteNl ();
 yyIndentSelector ("TR"); writebool (yyt->AnExpr.TR) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AnExpr.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AnExpr.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AnExpr.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->AnExpr.PAR) yyWriteNl ();
 yyIndentSelector ("LV"); writebool (yyt->AnExpr.LV) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->AnExpr.Expr);
}

static void yWriteProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kProcess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Process.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Process.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Process.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Process.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Process.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Process.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Process.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Process.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Process.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Process.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Process.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Process.PUOut) yyWriteNl ();
}

static void yWriteAssign
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAssign], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Assign.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Assign.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Assign.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Assign.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Assign.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Assign.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Assign.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Assign.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Assign.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Assign.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Assign.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Assign.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Assign.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Lval", yyt->Assign.Lval);
}

static void yWriteAltType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AltType.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->AltType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltType.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltType.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltType.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->AltType.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltType.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->AltType.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltType.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltType.PUOut) yyWriteNl ();
 yyIndentSelector ("Pri"); writebool (yyt->AltType.Pri) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->AltType.ARC) yyWriteNl ();
}

static void yWriteAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAlt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Alt.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Alt.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Alt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Alt.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Alt.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Alt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Alt.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Alt.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Alt.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Alt.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Alt.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Alt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Alt.PUOut) yyWriteNl ();
 yyIndentSelector ("Pri"); writebool (yyt->Alt.Pri) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->Alt.ARC) yyWriteNl ();
}

static void yWriteAltRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltRep], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AltRep.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->AltRep.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->AltRep.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltRep.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltRep.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltRep.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltRep.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->AltRep.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltRep.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->AltRep.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltRep.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltRep.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltRep.PUOut) yyWriteNl ();
 yyIndentSelector ("Pri"); writebool (yyt->AltRep.Pri) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->AltRep.ARC) yyWriteNl ();
 yyIndentSelectorTree ("Replicator", yyt->AltRep.Replicator);
}

static void yWriteParType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kParType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ParType.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ParType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ParType.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ParType.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ParType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ParType.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->ParType.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ParType.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->ParType.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ParType.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ParType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ParType.PUOut) yyWriteNl ();
 yyIndentSelector ("PType"); writetParType (yyt->ParType.PType) yyWriteNl ();
}

static void yWritePar
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kPar], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Par.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Par.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Par.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Par.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Par.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Par.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Par.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Par.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Par.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Par.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Par.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Par.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Par.PUOut) yyWriteNl ();
 yyIndentSelector ("PType"); writetParType (yyt->Par.PType) yyWriteNl ();
}

static void yWriteParRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kParRep], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->ParRep.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->ParRep.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->ParRep.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ParRep.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ParRep.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ParRep.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ParRep.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->ParRep.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ParRep.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->ParRep.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ParRep.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ParRep.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ParRep.PUOut) yyWriteNl ();
 yyIndentSelector ("PType"); writetParType (yyt->ParRep.PType) yyWriteNl ();
 yyIndentSelectorTree ("Replicator", yyt->ParRep.Replicator);
 yyIndentSelector ("Sym"); writetSymbol (yyt->ParRep.Sym) yyWriteNl ();
}

static void yWriteProcessor
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kProcessor], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Processor.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Processor.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Processor.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Processor.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Processor.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Processor.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Processor.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Processor.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Processor.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Processor.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Processor.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Processor.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Processor.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->Processor.Expr);
 yyIndentSelector ("Name"); writetIdent (yyt->Processor.Name) yyWriteNl ();
}

static void yWriteIfType
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIfType], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->IfType.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->IfType.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->IfType.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->IfType.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->IfType.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->IfType.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->IfType.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->IfType.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->IfType.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->IfType.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->IfType.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->IfType.PUOut) yyWriteNl ();
 yyIndentSelector ("BackChoiceList"); writetChoiceList (yyt->IfType.BackChoiceList) yyWriteNl ();
 yyIndentSelector ("BackIfType"); writetIfType (yyt->IfType.BackIfType) yyWriteNl ();
}

static void yWriteIf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIf], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->If.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->If.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->If.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->If.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->If.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->If.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->If.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->If.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->If.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->If.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->If.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->If.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->If.PUOut) yyWriteNl ();
 yyIndentSelector ("BackChoiceList"); writetChoiceList (yyt->If.BackChoiceList) yyWriteNl ();
 yyIndentSelector ("BackIfType"); writetIfType (yyt->If.BackIfType) yyWriteNl ();
}

static void yWriteIfRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kIfRep], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->IfRep.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->IfRep.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->IfRep.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->IfRep.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->IfRep.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->IfRep.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->IfRep.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->IfRep.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->IfRep.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->IfRep.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->IfRep.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->IfRep.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->IfRep.PUOut) yyWriteNl ();
 yyIndentSelector ("BackChoiceList"); writetChoiceList (yyt->IfRep.BackChoiceList) yyWriteNl ();
 yyIndentSelector ("BackIfType"); writetIfType (yyt->IfRep.BackIfType) yyWriteNl ();
 yyIndentSelectorTree ("Replicator", yyt->IfRep.Replicator);
}

static void yWriteInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInput], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Input.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Input.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Input.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Input.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Input.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Input.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Input.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Input.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Input.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Input.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Input.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Input.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Input.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Element", yyt->Input.Element);
}

static void yWriteOutput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kOutput], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Output.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Output.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Output.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Output.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Output.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Output.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Output.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Output.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Output.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Output.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Output.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Output.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Output.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Element", yyt->Output.Element);
}

static void yWriteCaseIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCaseIn], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->CaseIn.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->CaseIn.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->CaseIn.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->CaseIn.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->CaseIn.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->CaseIn.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->CaseIn.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->CaseIn.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->CaseIn.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->CaseIn.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->CaseIn.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->CaseIn.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->CaseIn.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Element", yyt->CaseIn.Element);
}

static void yWriteAfter
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAfter], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->After.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->After.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->After.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->After.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->After.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->After.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->After.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->After.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->After.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->After.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->After.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->After.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->After.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Element", yyt->After.Element);
}

static void yWriteSeqRep
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSeqRep], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->SeqRep.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->SeqRep.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->SeqRep.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->SeqRep.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->SeqRep.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->SeqRep.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->SeqRep.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->SeqRep.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->SeqRep.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->SeqRep.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->SeqRep.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->SeqRep.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->SeqRep.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Replicator", yyt->SeqRep.Replicator);
}

static void yWriteSeq
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSeq], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Seq.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Seq.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Seq.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Seq.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Seq.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Seq.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Seq.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Seq.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Seq.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Seq.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Seq.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Seq.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Seq.PUOut) yyWriteNl ();
}

static void yWriteCase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCase], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Case.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Case.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Case.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Case.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Case.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Case.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Case.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Case.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Case.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Case.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Case.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Case.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Case.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Selector", yyt->Case.Selector);
}

static void yWriteWhile
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kWhile], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->While.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->While.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->While.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->While.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->While.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->While.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->While.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->While.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->While.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->While.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->While.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->While.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->While.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->While.Expr);
}

static void yWriteSkip
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kSkip], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Skip.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Skip.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Skip.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Skip.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Skip.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Skip.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Skip.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Skip.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Skip.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Skip.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Skip.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Skip.PUOut) yyWriteNl ();
}

static void yWriteStop
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kStop], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Stop.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Stop.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Stop.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Stop.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Stop.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Stop.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Stop.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Stop.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Stop.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Stop.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Stop.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Stop.PUOut) yyWriteNl ();
}

static void yWriteInstance
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kInstance], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Instance.Pos) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->Instance.SpecList);
 yyIndentSelector ("V1In"); writeint (yyt->Instance.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Instance.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Instance.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Instance.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Instance.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->Instance.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Instance.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->Instance.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Instance.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Instance.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Instance.PUOut) yyWriteNl ();
 yyIndentSelector ("Sym"); writetSymbol (yyt->Instance.Sym) yyWriteNl ();
 yyIndentSelector ("Scope"); writetSymbol (yyt->Instance.Scope) yyWriteNl ();
 yyIndentSelector ("Frame"); writetFrame (yyt->Instance.Frame) yyWriteNl ();
}

static void yWriteCCode
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCCode], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->CCode.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->CCode.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->CCode.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->CCode.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->CCode.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->CCode.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->CCode.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->CCode.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->CCode.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->CCode.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->CCode.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->CCode.PUOut) yyWriteNl ();
 yyIndentSelector ("Code"); writetStringRef (yyt->CCode.Code) yyWriteNl ();
 yyIndentSelector ("Scope"); writetSymbol (yyt->CCode.Scope) yyWriteNl ();
}

static void yWriteNullProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNullProcess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->NullProcess.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->NullProcess.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NullProcess.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NullProcess.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NullProcess.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NullProcess.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->NullProcess.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NullProcess.SLC) yyWriteNl ();
 yyIndentSelector ("TLA"); writebool (yyt->NullProcess.TLA) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NullProcess.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NullProcess.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NullProcess.PUOut) yyWriteNl ();
}

static void yWriteAltList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AltList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltList.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltList.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltList.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltList.SLC) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->AltList.ARC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltList.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltList.PUOut) yyWriteNl ();
}

static void yWriteNoAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoAlt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoAlt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoAlt.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoAlt.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoAlt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoAlt.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NoAlt.SLC) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->NoAlt.ARC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoAlt.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoAlt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoAlt.PUOut) yyWriteNl ();
}

static void yWriteAnAlts
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnAlts], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AnAlts.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AnAlts.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AnAlts.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AnAlts.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AnAlts.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AnAlts.SLC) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->AnAlts.ARC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AnAlts.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AnAlts.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AnAlts.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->AnAlts.SpecList);
}

static void yWriteAnAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnAlt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AnAlt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AnAlt.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AnAlt.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AnAlt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AnAlt.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AnAlt.SLC) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->AnAlt.ARC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AnAlt.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AnAlt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AnAlt.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->AnAlt.SpecList);
 yyIndentSelectorTree ("Guard", yyt->AnAlt.Guard);
 yyIndentSelector ("Label"); writeint (yyt->AnAlt.Label) yyWriteNl ();
}

static void yWriteANewAlt
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kANewAlt], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->ANewAlt.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ANewAlt.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ANewAlt.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ANewAlt.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ANewAlt.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ANewAlt.SLC) yyWriteNl ();
 yyIndentSelector ("ARC"); writeint (yyt->ANewAlt.ARC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ANewAlt.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ANewAlt.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ANewAlt.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->ANewAlt.SpecList);
 yyIndentSelectorTree ("AltType", yyt->ANewAlt.AltType);
}

static void yWriteGuard
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kGuard], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->Guard.Pos) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->Guard.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->Guard.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->Guard.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->Guard.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->Guard.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->Guard.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->Guard.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->Guard.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->Guard.PUOut) yyWriteNl ();
}

static void yWriteAltInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltInput], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AltInput.Pos) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->AltInput.Expr);
 yyIndentSelector ("V1In"); writeint (yyt->AltInput.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltInput.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltInput.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltInput.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltInput.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltInput.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltInput.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltInput.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltInput.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("Input", yyt->AltInput.Input);
}

static void yWriteAltTimer
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltTimer], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AltTimer.Pos) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->AltTimer.Expr);
 yyIndentSelector ("V1In"); writeint (yyt->AltTimer.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltTimer.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltTimer.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltTimer.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltTimer.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltTimer.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltTimer.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltTimer.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltTimer.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("After", yyt->AltTimer.After);
}

static void yWriteAltCaseIn
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAltCaseIn], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->AltCaseIn.Pos) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->AltCaseIn.Expr);
 yyIndentSelector ("V1In"); writeint (yyt->AltCaseIn.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AltCaseIn.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AltCaseIn.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AltCaseIn.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AltCaseIn.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AltCaseIn.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AltCaseIn.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AltCaseIn.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AltCaseIn.PUOut) yyWriteNl ();
}

static void yWriteNoInput
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoInput], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("Pos"); writetPosition (yyt->NoInput.Pos) yyWriteNl ();
 yyIndentSelectorTree ("Expr", yyt->NoInput.Expr);
 yyIndentSelector ("V1In"); writeint (yyt->NoInput.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoInput.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoInput.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoInput.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoInput.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NoInput.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoInput.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoInput.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoInput.PUOut) yyWriteNl ();
}

static void yWriteChoiceList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kChoiceList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->ChoiceList.DefType) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ChoiceList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ChoiceList.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->ChoiceList.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ChoiceList.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ChoiceList.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ChoiceList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ChoiceList.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->ChoiceList.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ChoiceList.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ChoiceList.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ChoiceList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ChoiceList.PUOut) yyWriteNl ();
}

static void yWriteNoChoice
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoChoice], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->NoChoice.DefType) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->NoChoice.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoChoice.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->NoChoice.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->NoChoice.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoChoice.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoChoice.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoChoice.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->NoChoice.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NoChoice.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoChoice.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoChoice.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoChoice.PUOut) yyWriteNl ();
}

static void yWriteAChoices
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAChoices], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->AChoices.DefType) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->AChoices.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AChoices.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->AChoices.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->AChoices.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AChoices.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AChoices.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AChoices.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->AChoices.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AChoices.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AChoices.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AChoices.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AChoices.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->AChoices.SpecList);
}

static void yWriteAChoice
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAChoice], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->AChoice.DefType) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->AChoice.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AChoice.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->AChoice.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->AChoice.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AChoice.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AChoice.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AChoice.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->AChoice.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AChoice.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AChoice.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AChoice.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AChoice.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->AChoice.SpecList);
 yyIndentSelectorTree ("ExprList", yyt->AChoice.ExprList);
 yyIndentSelectorTree ("Process", yyt->AChoice.Process);
 yyIndentSelector ("Label"); writeint (yyt->AChoice.Label) yyWriteNl ();
}

static void yWriteANewIf
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kANewIf], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("DefType"); writetType (yyt->ANewIf.DefType) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ANewIf.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ANewIf.V1Out) yyWriteNl ();
 yyIndentSelector ("SynType"); writetType (yyt->ANewIf.SynType) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ANewIf.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ANewIf.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ANewIf.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ANewIf.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->ANewIf.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ANewIf.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ANewIf.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ANewIf.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ANewIf.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->ANewIf.SpecList);
 yyIndentSelectorTree ("IfType", yyt->ANewIf.IfType);
}

static void yWriteProcList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kProcList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->ProcList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ProcList.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ProcList.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ProcList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ProcList.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->ProcList.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ProcList.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ProcList.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ProcList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ProcList.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->ProcList.PAR) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->ProcList.Pos) yyWriteNl ();
}

static void yWriteNoProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoProcess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->NoProcess.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoProcess.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoProcess.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoProcess.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoProcess.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->NoProcess.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NoProcess.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoProcess.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoProcess.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoProcess.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->NoProcess.PAR) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->NoProcess.Pos) yyWriteNl ();
}

static void yWriteAProcess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAProcess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("V1In"); writeint (yyt->AProcess.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->AProcess.V1Out) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->AProcess.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->AProcess.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->AProcess.VOut) yyWriteNl ();
 yyIndentSelector ("ATOMIC"); writebool (yyt->AProcess.ATOMIC) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->AProcess.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->AProcess.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->AProcess.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->AProcess.PUOut) yyWriteNl ();
 yyIndentSelector ("PAR"); writebool (yyt->AProcess.PAR) yyWriteNl ();
 yyIndentSelector ("Pos"); writetPosition (yyt->AProcess.Pos) yyWriteNl ();
 yyIndentSelectorTree ("Process", yyt->AProcess.Process);
 yyIndentSelector ("Sym"); writetSymbol (yyt->AProcess.Sym) yyWriteNl ();
}

static void yWriteCaseList
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kCaseList], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("FLIn"); writetType (yyt->CaseList.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->CaseList.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->CaseList.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->CaseList.V1Out) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->CaseList.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->CaseList.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->CaseList.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->CaseList.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->CaseList.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->CaseList.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->CaseList.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->CaseList.PUOut) yyWriteNl ();
}

static void yWriteNoCase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kNoCase], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("FLIn"); writetType (yyt->NoCase.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->NoCase.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->NoCase.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->NoCase.V1Out) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->NoCase.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->NoCase.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->NoCase.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->NoCase.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->NoCase.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->NoCase.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->NoCase.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->NoCase.PUOut) yyWriteNl ();
}

static void yWriteACase
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kACase], yyf); (void) fputc ('\n', yyf);
 yyIndentSelector ("FLIn"); writetType (yyt->ACase.FLIn) yyWriteNl ();
 yyIndentSelector ("FLOut"); writetType (yyt->ACase.FLOut) yyWriteNl ();
 yyIndentSelector ("V1In"); writeint (yyt->ACase.V1In) yyWriteNl ();
 yyIndentSelector ("V1Out"); writeint (yyt->ACase.V1Out) yyWriteNl ();
 yyIndentSelector ("InhType"); writetType (yyt->ACase.InhType) yyWriteNl ();
 yyIndentSelector ("VS"); writeint (yyt->ACase.VS) yyWriteNl ();
 yyIndentSelector ("VIn"); writeint (yyt->ACase.VIn) yyWriteNl ();
 yyIndentSelector ("VOut"); writeint (yyt->ACase.VOut) yyWriteNl ();
 yyIndentSelector ("SLC"); writeint (yyt->ACase.SLC) yyWriteNl ();
 yyIndentSelector ("Parent"); writetSymbol (yyt->ACase.Parent) yyWriteNl ();
 yyIndentSelector ("PUIn"); writetParUsage (yyt->ACase.PUIn) yyWriteNl ();
 yyIndentSelector ("PUOut"); writetParUsage (yyt->ACase.PUOut) yyWriteNl ();
 yyIndentSelectorTree ("SpecList", yyt->ACase.SpecList);
 yyIndentSelectorTree ("ExprList", yyt->ACase.ExprList);
 yyIndentSelectorTree ("Process", yyt->ACase.Process);
 yyIndentSelector ("Label"); writeint (yyt->ACase.Label) yyWriteNl ();
}

static void yWriteAnAccess
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnAccess], yyf); (void) fputc ('\n', yyf);
 yyIndentSelectorTree ("Next", yyt->AnAccess.Next);
 yyIndentSelector ("Sym"); writetSymbol (yyt->AnAccess.Sym) yyWriteNl ();
}

static void yWriteAnElement
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 (void) fputs (Tree_NodeName [kAnElement], yyf); (void) fputc ('\n', yyf);
 yyIndentSelectorTree ("Next", yyt->AnElement.Next);
}

static void yyWriteTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{ unsigned short yyLevel = yyIndentLevel;
 for (;;) {
  if (yyt == NoTree) { (void) fputs (" NoTree\n", yyf); goto yyExit;
  } else if (yyt->yyHead.yyMark == 0) { (void) fprintf (yyf, "^%d\n", yyMapToLabel (yyt)); goto yyExit;
  } else if (yyt->yyHead.yyMark > 1) {
   register int yyi;
   (void) fprintf (yyf, "\n%06d:", yyMapToLabel (yyt));
   for (yyi = 8; yyi <= yyIndentLevel; yyi ++) (void) putc (' ', yyf);
  } else (void) putc (' ', yyf);
  yyt->yyHead.yyMark = 0;
  yyIndentLevel += 2;

  switch (yyt->Kind) {
case kStart: yWriteStart (yyt); yyIndentSelector ("Process"); yyt = yyt->Start.Process; break;
case kSpecList: yWriteSpecList (yyt); goto yyExit;
case kNoSpec: yWriteNoSpec (yyt); goto yyExit;
case kASpec: yWriteASpec (yyt); yyIndentSelector ("Next"); yyt = yyt->ASpec.Next; break;
case kSpec: yWriteSpec (yyt); goto yyExit;
case kDeclType: yWriteDeclType (yyt); yyIndentSelector ("Type"); yyt = yyt->DeclType.Type; break;
case kDecl: yWriteDecl (yyt); yyIndentSelector ("IdList"); yyt = yyt->Decl.IdList; break;
case kIs: yWriteIs (yyt); yyIndentSelector ("Elemental"); yyt = yyt->Is.Elemental; break;
case kAVal: yWriteAVal (yyt); yyIndentSelector ("Expr"); yyt = yyt->AVal.Expr; break;
case kVal: yWriteVal (yyt); yyIndentSelector ("Expr"); yyt = yyt->Val.Expr; break;
case kValRetypes: yWriteValRetypes (yyt); yyIndentSelector ("Expr"); yyt = yyt->ValRetypes.Expr; break;
case kValReshapes: yWriteValReshapes (yyt); yyIndentSelector ("Expr"); yyt = yyt->ValReshapes.Expr; break;
case kRetypes: yWriteRetypes (yyt); yyIndentSelector ("Elemental"); yyt = yyt->Retypes.Elemental; break;
case kReshapes: yWriteReshapes (yyt); yyIndentSelector ("Elemental"); yyt = yyt->Reshapes.Elemental; break;
case kDefAPro: yWriteDefAPro (yyt); goto yyExit;
case kDefSeqPro: yWriteDefSeqPro (yyt); yyIndentSelector ("SeqProList"); yyt = yyt->DefSeqPro.SeqProList; break;
case kDefTagPro: yWriteDefTagPro (yyt); yyIndentSelector ("TagProList"); yyt = yyt->DefTagPro.TagProList; break;
case kDefAProc: yWriteDefAProc (yyt); yyIndentSelector ("Formals"); yyt = yyt->DefAProc.Formals; break;
case kDefProc: yWriteDefProc (yyt); yyIndentSelector ("Process"); yyt = yyt->DefProc.Process; break;
case kInlineDefProc: yWriteInlineDefProc (yyt); yyIndentSelector ("Process"); yyt = yyt->InlineDefProc.Process; break;
case kPrototypeProc: yWritePrototypeProc (yyt); yyIndentSelector ("Formals"); yyt = yyt->PrototypeProc.Formals; break;
case kDefAFunc: yWriteDefAFunc (yyt); yyIndentSelector ("Formals"); yyt = yyt->DefAFunc.Formals; break;
case kDefFunc: yWriteDefFunc (yyt); yyIndentSelector ("Valof"); yyt = yyt->DefFunc.Valof; break;
case kInlineDefFunc: yWriteInlineDefFunc (yyt); yyIndentSelector ("Valof"); yyt = yyt->InlineDefFunc.Valof; break;
case kDefIsFunc: yWriteDefIsFunc (yyt); yyIndentSelector ("ExprList"); yyt = yyt->DefIsFunc.ExprList; break;
case kInlineDefIsFunc: yWriteInlineDefIsFunc (yyt); yyIndentSelector ("ExprList"); yyt = yyt->InlineDefIsFunc.ExprList; break;
case kPrototypeFunc: yWritePrototypeFunc (yyt); yyIndentSelector ("Formals"); yyt = yyt->PrototypeFunc.Formals; break;
case kNullSpec: yWriteNullSpec (yyt); goto yyExit;
case kHCode: yWriteHCode (yyt); goto yyExit;
case kCallFrame: yWriteCallFrame (yyt); goto yyExit;
case kParFrame: yWriteParFrame (yyt); goto yyExit;
case kFormals: yWriteFormals (yyt); goto yyExit;
case kNoFormal: yWriteNoFormal (yyt); goto yyExit;
case kNoFormals: yWriteNoFormals (yyt); goto yyExit;
case kAFormal: yWriteAFormal (yyt); yyIndentSelector ("Type"); yyt = yyt->AFormal.Type; break;
case kReplicator: yWriteReplicator (yyt); yyIndentSelector ("For"); yyt = yyt->Replicator.For; break;
case kTagProList: yWriteTagProList (yyt); goto yyExit;
case kNoTagPro: yWriteNoTagPro (yyt); goto yyExit;
case kATagPro: yWriteATagPro (yyt); yyIndentSelector ("Next"); yyt = yyt->ATagPro.Next; break;
case kBy: (void) fputs (Tree_NodeName [kBy], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kByVal: (void) fputs (Tree_NodeName [kByVal], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kByRef: (void) fputs (Tree_NodeName [kByRef], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kByUndef: (void) fputs (Tree_NodeName [kByUndef], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kIdList: yWriteIdList (yyt); goto yyExit;
case kNoId: yWriteNoId (yyt); goto yyExit;
case kAnId: yWriteAnId (yyt); yyIndentSelector ("Next"); yyt = yyt->AnId.Next; break;
case kType: yWriteType (yyt); goto yyExit;
case kArray: yWriteArray (yyt); yyIndentSelector ("Type"); yyt = yyt->Array.Type; break;
case kUserArray: yWriteUserArray (yyt); yyIndentSelector ("Type"); yyt = yyt->UserArray.Type; break;
case kChan: yWriteChan (yyt); yyIndentSelector ("Protocol"); yyt = yyt->Chan.Protocol; break;
case kTimer: yWriteTimer (yyt); goto yyExit;
case kUntyped: yWriteUntyped (yyt); goto yyExit;
case kUntypedByte: yWriteUntypedByte (yyt); goto yyExit;
case kUntypedReal: yWriteUntypedReal (yyt); goto yyExit;
case kUntypedInteger: yWriteUntypedInteger (yyt); goto yyExit;
case kUntypedInt: yWriteUntypedInt (yyt); goto yyExit;
case kUntypedHex: yWriteUntypedHex (yyt); goto yyExit;
case kPrimitive: yWritePrimitive (yyt); goto yyExit;
case kNumeric: yWriteNumeric (yyt); goto yyExit;
case kInteger: yWriteInteger (yyt); goto yyExit;
case kInt: yWriteInt (yyt); goto yyExit;
case kUserInt: yWriteUserInt (yyt); goto yyExit;
case kInt16: yWriteInt16 (yyt); goto yyExit;
case kUserInt16: yWriteUserInt16 (yyt); goto yyExit;
case kInt32: yWriteInt32 (yyt); goto yyExit;
case kUserInt32: yWriteUserInt32 (yyt); goto yyExit;
case kInt64: yWriteInt64 (yyt); goto yyExit;
case kUserInt64: yWriteUserInt64 (yyt); goto yyExit;
case kReal: yWriteReal (yyt); goto yyExit;
case kReal32: yWriteReal32 (yyt); goto yyExit;
case kUserReal32: yWriteUserReal32 (yyt); goto yyExit;
case kReal64: yWriteReal64 (yyt); goto yyExit;
case kUserReal64: yWriteUserReal64 (yyt); goto yyExit;
case kByte: yWriteByte (yyt); goto yyExit;
case kUserByte: yWriteUserByte (yyt); goto yyExit;
case kBool: yWriteBool (yyt); goto yyExit;
case kUserBool: yWriteUserBool (yyt); goto yyExit;
case kUserStructTypes: yWriteUserStructTypes (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserStructTypes.FieldList; break;
case kUserDataTypes: yWriteUserDataTypes (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserDataTypes.FieldList; break;
case kUserRecord: yWriteUserRecord (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserRecord.FieldList; break;
case kUserPackedRecord: yWriteUserPackedRecord (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserPackedRecord.FieldList; break;
case kUserUnion: yWriteUserUnion (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserUnion.FieldList; break;
case kUserChanRecord: yWriteUserChanRecord (yyt); yyIndentSelector ("FieldList"); yyt = yyt->UserChanRecord.FieldList; break;
case kTypeList: yWriteTypeList (yyt); goto yyExit;
case kNoType: yWriteNoType (yyt); goto yyExit;
case kAType: yWriteAType (yyt); yyIndentSelector ("Next"); yyt = yyt->AType.Next; break;
case kFieldList: yWriteFieldList (yyt); goto yyExit;
case kNoField: yWriteNoField (yyt); goto yyExit;
case kAField: yWriteAField (yyt); yyIndentSelector ("Next"); yyt = yyt->AField.Next; break;
case kProtocol: yWriteProtocol (yyt); goto yyExit;
case kAny: yWriteAny (yyt); goto yyExit;
case kNamed: yWriteNamed (yyt); goto yyExit;
case kSimple: yWriteSimple (yyt); yyIndentSelector ("Type"); yyt = yyt->Simple.Type; break;
case kCounted: yWriteCounted (yyt); yyIndentSelector ("Type"); yyt = yyt->Counted.Type; break;
case kSeqProList: yWriteSeqProList (yyt); goto yyExit;
case kNoSeqPro: yWriteNoSeqPro (yyt); goto yyExit;
case kASeqPro: yWriteASeqPro (yyt); yyIndentSelector ("Next"); yyt = yyt->ASeqPro.Next; break;
case kExpr: yWriteExpr (yyt); goto yyExit;
case kBinary: yWriteBinary (yyt); yyIndentSelector ("Op"); yyt = yyt->Binary.Op; break;
case kUnary: yWriteUnary (yyt); yyIndentSelector ("Op"); yyt = yyt->Unary.Op; break;
case kConst: yWriteConst (yyt); yyIndentSelector ("Type"); yyt = yyt->Const.Type; break;
case kValof: yWriteValof (yyt); yyIndentSelector ("ExprList"); yyt = yyt->Valof.ExprList; break;
case kFuncCall: yWriteFuncCall (yyt); yyIndentSelector ("ExprList"); yyt = yyt->FuncCall.ExprList; break;
case kElementalExpr: yWriteElementalExpr (yyt); yyIndentSelector ("Elemental"); yyt = yyt->ElementalExpr.Elemental; break;
case kDefault: yWriteDefault (yyt); goto yyExit;
case kConvert: yWriteConvert (yyt); yyIndentSelector ("Expr"); yyt = yyt->Convert.Expr; break;
case kRound: yWriteRound (yyt); yyIndentSelector ("Expr"); yyt = yyt->Round.Expr; break;
case kTrunc: yWriteTrunc (yyt); yyIndentSelector ("Expr"); yyt = yyt->Trunc.Expr; break;
case kMostPos: yWriteMostPos (yyt); yyIndentSelector ("Type"); yyt = yyt->MostPos.Type; break;
case kMostNeg: yWriteMostNeg (yyt); yyIndentSelector ("Type"); yyt = yyt->MostNeg.Type; break;
case kCountArray: yWriteCountArray (yyt); yyIndentSelector ("Expr"); yyt = yyt->CountArray.Expr; break;
case kUnBound: yWriteUnBound (yyt); goto yyExit;
case kBytesIn: yWriteBytesIn (yyt); goto yyExit;
case kBytesInType: yWriteBytesInType (yyt); yyIndentSelector ("Type"); yyt = yyt->BytesInType.Type; break;
case kBytesInExpr: yWriteBytesInExpr (yyt); yyIndentSelector ("Expr"); yyt = yyt->BytesInExpr.Expr; break;
case kSizeOf: yWriteSizeOf (yyt); yyIndentSelector ("Type"); yyt = yyt->SizeOf.Type; break;
case kOffsetOf: yWriteOffsetOf (yyt); yyIndentSelector ("UnmappedIdentifier"); yyt = yyt->OffsetOf.UnmappedIdentifier; break;
case kStringElement: yWriteStringElement (yyt); goto yyExit;
case kString: yWriteString (yyt); yyIndentSelector ("Type"); yyt = yyt->String.Type; break;
case kStringSubscript: yWriteStringSubscript (yyt); yyIndentSelector ("Expr"); yyt = yyt->StringSubscript.Expr; break;
case kStringRange: yWriteStringRange (yyt); yyIndentSelector ("For"); yyt = yyt->StringRange.For; break;
case kElemental: yWriteElemental (yyt); goto yyExit;
case kTableElement: yWriteTableElement (yyt); goto yyExit;
case kTable: yWriteTable (yyt); yyIndentSelector ("ExprList"); yyt = yyt->Table.ExprList; break;
case kUserLiteral: yWriteUserLiteral (yyt); yyIndentSelector ("Type"); yyt = yyt->UserLiteral.Type; break;
case kTableExpr: yWriteTableExpr (yyt); yyIndentSelector ("Expr"); yyt = yyt->TableExpr.Expr; break;
case kTableSubscript: yWriteTableSubscript (yyt); yyIndentSelector ("Expr"); yyt = yyt->TableSubscript.Expr; break;
case kTableRange: yWriteTableRange (yyt); yyIndentSelector ("For"); yyt = yyt->TableRange.For; break;
case kElement: yWriteElement (yyt); goto yyExit;
case kIdentifier: yWriteIdentifier (yyt); goto yyExit;
case kUnmappedIdentifier: yWriteUnmappedIdentifier (yyt); goto yyExit;
case kElementAccess: yWriteElementAccess (yyt); yyIndentSelector ("Next"); yyt = yyt->ElementAccess.Next; break;
case kSubscript: yWriteSubscript (yyt); yyIndentSelector ("Expr"); yyt = yyt->Subscript.Expr; break;
case kRange: yWriteRange (yyt); yyIndentSelector ("For"); yyt = yyt->Range.For; break;
case kExprList: yWriteExprList (yyt); goto yyExit;
case kNoExpr: yWriteNoExpr (yyt); goto yyExit;
case kAnExpr: yWriteAnExpr (yyt); yyIndentSelector ("Next"); yyt = yyt->AnExpr.Next; break;
case kOp: (void) fputs (Tree_NodeName [kOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kRelOp: (void) fputs (Tree_NodeName [kRelOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kLess: (void) fputs (Tree_NodeName [kLess], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kGreater: (void) fputs (Tree_NodeName [kGreater], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kLessEqual: (void) fputs (Tree_NodeName [kLessEqual], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kGreaterEqual: (void) fputs (Tree_NodeName [kGreaterEqual], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kEqual: (void) fputs (Tree_NodeName [kEqual], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kNotEqual: (void) fputs (Tree_NodeName [kNotEqual], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kMathsOp: (void) fputs (Tree_NodeName [kMathsOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kAdd: (void) fputs (Tree_NodeName [kAdd], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kSubtract: (void) fputs (Tree_NodeName [kSubtract], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kMultiply: (void) fputs (Tree_NodeName [kMultiply], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kDivide: (void) fputs (Tree_NodeName [kDivide], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kRem: (void) fputs (Tree_NodeName [kRem], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kBitOp: (void) fputs (Tree_NodeName [kBitOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kBitAnd: (void) fputs (Tree_NodeName [kBitAnd], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kBitOr: (void) fputs (Tree_NodeName [kBitOr], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kBitEor: (void) fputs (Tree_NodeName [kBitEor], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kShiftOp: (void) fputs (Tree_NodeName [kShiftOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kLeftShift: (void) fputs (Tree_NodeName [kLeftShift], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kRightShift: (void) fputs (Tree_NodeName [kRightShift], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kUniOp: (void) fputs (Tree_NodeName [kUniOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kSize: (void) fputs (Tree_NodeName [kSize], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kBitNot: (void) fputs (Tree_NodeName [kBitNot], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kNot: (void) fputs (Tree_NodeName [kNot], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kClockOp: (void) fputs (Tree_NodeName [kClockOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kPlus: (void) fputs (Tree_NodeName [kPlus], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kMinus: (void) fputs (Tree_NodeName [kMinus], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kTimes: (void) fputs (Tree_NodeName [kTimes], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kLogOp: (void) fputs (Tree_NodeName [kLogOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kAnd: (void) fputs (Tree_NodeName [kAnd], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kOr: (void) fputs (Tree_NodeName [kOr], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kAfterOp: (void) fputs (Tree_NodeName [kAfterOp], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kProcess: yWriteProcess (yyt); yyIndentSelector ("SpecList"); yyt = yyt->Process.SpecList; break;
case kAssign: yWriteAssign (yyt); yyIndentSelector ("Rval"); yyt = yyt->Assign.Rval; break;
case kAltType: yWriteAltType (yyt); yyIndentSelector ("SpecList"); yyt = yyt->AltType.SpecList; break;
case kAlt: yWriteAlt (yyt); yyIndentSelector ("AltList"); yyt = yyt->Alt.AltList; break;
case kAltRep: yWriteAltRep (yyt); yyIndentSelector ("AltList"); yyt = yyt->AltRep.AltList; break;
case kParType: yWriteParType (yyt); yyIndentSelector ("SpecList"); yyt = yyt->ParType.SpecList; break;
case kPar: yWritePar (yyt); yyIndentSelector ("ProcList"); yyt = yyt->Par.ProcList; break;
case kParRep: yWriteParRep (yyt); yyIndentSelector ("Process"); yyt = yyt->ParRep.Process; break;
case kProcessor: yWriteProcessor (yyt); yyIndentSelector ("Process"); yyt = yyt->Processor.Process; break;
case kIfType: yWriteIfType (yyt); yyIndentSelector ("SpecList"); yyt = yyt->IfType.SpecList; break;
case kIf: yWriteIf (yyt); yyIndentSelector ("ChoiceList"); yyt = yyt->If.ChoiceList; break;
case kIfRep: yWriteIfRep (yyt); yyIndentSelector ("ChoiceList"); yyt = yyt->IfRep.ChoiceList; break;
case kInput: yWriteInput (yyt); yyIndentSelector ("ExprList"); yyt = yyt->Input.ExprList; break;
case kOutput: yWriteOutput (yyt); yyIndentSelector ("ExprList"); yyt = yyt->Output.ExprList; break;
case kCaseIn: yWriteCaseIn (yyt); yyIndentSelector ("CaseList"); yyt = yyt->CaseIn.CaseList; break;
case kAfter: yWriteAfter (yyt); yyIndentSelector ("Expr"); yyt = yyt->After.Expr; break;
case kSeqRep: yWriteSeqRep (yyt); yyIndentSelector ("Process"); yyt = yyt->SeqRep.Process; break;
case kSeq: yWriteSeq (yyt); yyIndentSelector ("ProcList"); yyt = yyt->Seq.ProcList; break;
case kCase: yWriteCase (yyt); yyIndentSelector ("ChoiceList"); yyt = yyt->Case.ChoiceList; break;
case kWhile: yWriteWhile (yyt); yyIndentSelector ("Process"); yyt = yyt->While.Process; break;
case kSkip: yWriteSkip (yyt); yyIndentSelector ("SpecList"); yyt = yyt->Skip.SpecList; break;
case kStop: yWriteStop (yyt); yyIndentSelector ("SpecList"); yyt = yyt->Stop.SpecList; break;
case kInstance: yWriteInstance (yyt); yyIndentSelector ("ExprList"); yyt = yyt->Instance.ExprList; break;
case kCCode: yWriteCCode (yyt); yyIndentSelector ("SpecList"); yyt = yyt->CCode.SpecList; break;
case kNullProcess: yWriteNullProcess (yyt); yyIndentSelector ("SpecList"); yyt = yyt->NullProcess.SpecList; break;
case kAltList: yWriteAltList (yyt); goto yyExit;
case kNoAlt: yWriteNoAlt (yyt); goto yyExit;
case kAnAlts: yWriteAnAlts (yyt); yyIndentSelector ("Next"); yyt = yyt->AnAlts.Next; break;
case kAnAlt: yWriteAnAlt (yyt); yyIndentSelector ("Next"); yyt = yyt->AnAlt.Next; break;
case kANewAlt: yWriteANewAlt (yyt); yyIndentSelector ("Next"); yyt = yyt->ANewAlt.Next; break;
case kGuard: yWriteGuard (yyt); yyIndentSelector ("Expr"); yyt = yyt->Guard.Expr; break;
case kAltInput: yWriteAltInput (yyt); yyIndentSelector ("Process"); yyt = yyt->AltInput.Process; break;
case kAltTimer: yWriteAltTimer (yyt); yyIndentSelector ("Process"); yyt = yyt->AltTimer.Process; break;
case kAltCaseIn: yWriteAltCaseIn (yyt); yyIndentSelector ("CaseIn"); yyt = yyt->AltCaseIn.CaseIn; break;
case kNoInput: yWriteNoInput (yyt); yyIndentSelector ("Process"); yyt = yyt->NoInput.Process; break;
case kChoiceList: yWriteChoiceList (yyt); goto yyExit;
case kNoChoice: yWriteNoChoice (yyt); goto yyExit;
case kAChoices: yWriteAChoices (yyt); yyIndentSelector ("Next"); yyt = yyt->AChoices.Next; break;
case kAChoice: yWriteAChoice (yyt); yyIndentSelector ("Next"); yyt = yyt->AChoice.Next; break;
case kANewIf: yWriteANewIf (yyt); yyIndentSelector ("Next"); yyt = yyt->ANewIf.Next; break;
case kProcList: yWriteProcList (yyt); goto yyExit;
case kNoProcess: yWriteNoProcess (yyt); goto yyExit;
case kAProcess: yWriteAProcess (yyt); yyIndentSelector ("Next"); yyt = yyt->AProcess.Next; break;
case kCaseList: yWriteCaseList (yyt); goto yyExit;
case kNoCase: yWriteNoCase (yyt); goto yyExit;
case kACase: yWriteACase (yyt); yyIndentSelector ("Next"); yyt = yyt->ACase.Next; break;
case kAccessList: (void) fputs (Tree_NodeName [kAccessList], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kAnAccess: yWriteAnAccess (yyt); yyIndentSelector ("ElementList"); yyt = yyt->AnAccess.ElementList; break;
case kNoAccess: (void) fputs (Tree_NodeName [kNoAccess], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kElementList: (void) fputs (Tree_NodeName [kElementList], yyf); (void) fputc ('\n', yyf); goto yyExit;
case kAnElement: yWriteAnElement (yyt); yyIndentSelector ("Element"); yyt = yyt->AnElement.Element; break;
case kNoElement: (void) fputs (Tree_NodeName [kNoElement], yyf); (void) fputc ('\n', yyf); goto yyExit;
  default: goto yyExit;
  }
 }
yyExit:
 yyIndentLevel = yyLevel;
}

# define yyNil	0374
# define yyNoLabel	0375
# define yyLabelDef	0376
# define yyLabelUse	0377

static Tree_tProcTree yyProc;

static void yyTraverseTreeTD
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 for (;;) {
  if (yyt == NoTree || yyt->yyHead.yyMark == 0) return;
  yyt->yyHead.yyMark = 0;
  yyProc (yyt);

  switch (yyt->Kind) {
case kStart:
yyt = yyt->Start.Process; break;
case kASpec:
yyTraverseTreeTD (yyt->ASpec.Spec);
yyt = yyt->ASpec.Next; break;
case kDeclType:
yyt = yyt->DeclType.Type; break;
case kDecl:
yyTraverseTreeTD (yyt->Decl.Type);
yyt = yyt->Decl.IdList; break;
case kIs:
yyTraverseTreeTD (yyt->Is.Type);
yyt = yyt->Is.Elemental; break;
case kAVal:
yyTraverseTreeTD (yyt->AVal.Type);
yyt = yyt->AVal.Expr; break;
case kVal:
yyTraverseTreeTD (yyt->Val.Type);
yyt = yyt->Val.Expr; break;
case kValRetypes:
yyTraverseTreeTD (yyt->ValRetypes.Type);
yyt = yyt->ValRetypes.Expr; break;
case kValReshapes:
yyTraverseTreeTD (yyt->ValReshapes.Type);
yyt = yyt->ValReshapes.Expr; break;
case kRetypes:
yyTraverseTreeTD (yyt->Retypes.Type);
yyt = yyt->Retypes.Elemental; break;
case kReshapes:
yyTraverseTreeTD (yyt->Reshapes.Type);
yyt = yyt->Reshapes.Elemental; break;
case kDefSeqPro:
yyt = yyt->DefSeqPro.SeqProList; break;
case kDefTagPro:
yyt = yyt->DefTagPro.TagProList; break;
case kDefAProc:
yyt = yyt->DefAProc.Formals; break;
case kDefProc:
yyTraverseTreeTD (yyt->DefProc.Formals);
yyt = yyt->DefProc.Process; break;
case kInlineDefProc:
yyTraverseTreeTD (yyt->InlineDefProc.Formals);
yyt = yyt->InlineDefProc.Process; break;
case kPrototypeProc:
yyt = yyt->PrototypeProc.Formals; break;
case kDefAFunc:
yyTraverseTreeTD (yyt->DefAFunc.TypeList);
yyt = yyt->DefAFunc.Formals; break;
case kDefFunc:
yyTraverseTreeTD (yyt->DefFunc.TypeList);
yyTraverseTreeTD (yyt->DefFunc.Formals);
yyt = yyt->DefFunc.Valof; break;
case kInlineDefFunc:
yyTraverseTreeTD (yyt->InlineDefFunc.TypeList);
yyTraverseTreeTD (yyt->InlineDefFunc.Formals);
yyt = yyt->InlineDefFunc.Valof; break;
case kDefIsFunc:
yyTraverseTreeTD (yyt->DefIsFunc.TypeList);
yyTraverseTreeTD (yyt->DefIsFunc.Formals);
yyt = yyt->DefIsFunc.ExprList; break;
case kInlineDefIsFunc:
yyTraverseTreeTD (yyt->InlineDefIsFunc.TypeList);
yyTraverseTreeTD (yyt->InlineDefIsFunc.Formals);
yyt = yyt->InlineDefIsFunc.ExprList; break;
case kPrototypeFunc:
yyTraverseTreeTD (yyt->PrototypeFunc.TypeList);
yyt = yyt->PrototypeFunc.Formals; break;
case kAFormal:
yyTraverseTreeTD (yyt->AFormal.Next);
yyTraverseTreeTD (yyt->AFormal.By);
yyt = yyt->AFormal.Type; break;
case kReplicator:
yyTraverseTreeTD (yyt->Replicator.From);
yyt = yyt->Replicator.For; break;
case kATagPro:
yyTraverseTreeTD (yyt->ATagPro.SeqProList);
yyt = yyt->ATagPro.Next; break;
case kAnId:
yyt = yyt->AnId.Next; break;
case kArray:
yyTraverseTreeTD (yyt->Array.Expr);
yyt = yyt->Array.Type; break;
case kUserArray:
yyTraverseTreeTD (yyt->UserArray.Expr);
yyt = yyt->UserArray.Type; break;
case kChan:
yyt = yyt->Chan.Protocol; break;
case kUserStructTypes:
yyt = yyt->UserStructTypes.FieldList; break;
case kUserDataTypes:
yyt = yyt->UserDataTypes.FieldList; break;
case kUserRecord:
yyt = yyt->UserRecord.FieldList; break;
case kUserPackedRecord:
yyt = yyt->UserPackedRecord.FieldList; break;
case kUserUnion:
yyt = yyt->UserUnion.FieldList; break;
case kUserChanRecord:
yyt = yyt->UserChanRecord.FieldList; break;
case kAType:
yyTraverseTreeTD (yyt->AType.Type);
yyt = yyt->AType.Next; break;
case kAField:
yyTraverseTreeTD (yyt->AField.Type);
yyt = yyt->AField.Next; break;
case kSimple:
yyt = yyt->Simple.Type; break;
case kCounted:
yyTraverseTreeTD (yyt->Counted.Length);
yyt = yyt->Counted.Type; break;
case kASeqPro:
yyTraverseTreeTD (yyt->ASeqPro.Protocol);
yyt = yyt->ASeqPro.Next; break;
case kBinary:
yyTraverseTreeTD (yyt->Binary.Lop);
yyTraverseTreeTD (yyt->Binary.Rop);
yyt = yyt->Binary.Op; break;
case kUnary:
yyTraverseTreeTD (yyt->Unary.Expr);
yyt = yyt->Unary.Op; break;
case kConst:
yyt = yyt->Const.Type; break;
case kValof:
yyTraverseTreeTD (yyt->Valof.SpecList);
yyTraverseTreeTD (yyt->Valof.Process);
yyt = yyt->Valof.ExprList; break;
case kFuncCall:
yyt = yyt->FuncCall.ExprList; break;
case kElementalExpr:
yyt = yyt->ElementalExpr.Elemental; break;
case kConvert:
yyTraverseTreeTD (yyt->Convert.Type);
yyt = yyt->Convert.Expr; break;
case kRound:
yyTraverseTreeTD (yyt->Round.Type);
yyt = yyt->Round.Expr; break;
case kTrunc:
yyTraverseTreeTD (yyt->Trunc.Type);
yyt = yyt->Trunc.Expr; break;
case kMostPos:
yyt = yyt->MostPos.Type; break;
case kMostNeg:
yyt = yyt->MostNeg.Type; break;
case kCountArray:
yyTraverseTreeTD (yyt->CountArray.Length);
yyt = yyt->CountArray.Expr; break;
case kBytesInType:
yyt = yyt->BytesInType.Type; break;
case kBytesInExpr:
yyt = yyt->BytesInExpr.Expr; break;
case kSizeOf:
yyt = yyt->SizeOf.Type; break;
case kOffsetOf:
yyTraverseTreeTD (yyt->OffsetOf.Type);
yyt = yyt->OffsetOf.UnmappedIdentifier; break;
case kString:
yyt = yyt->String.Type; break;
case kStringSubscript:
yyTraverseTreeTD (yyt->StringSubscript.Next);
yyt = yyt->StringSubscript.Expr; break;
case kStringRange:
yyTraverseTreeTD (yyt->StringRange.Next);
yyTraverseTreeTD (yyt->StringRange.From);
yyt = yyt->StringRange.For; break;
case kTable:
yyt = yyt->Table.ExprList; break;
case kUserLiteral:
yyTraverseTreeTD (yyt->UserLiteral.ExprList);
yyt = yyt->UserLiteral.Type; break;
case kTableExpr:
yyt = yyt->TableExpr.Expr; break;
case kTableSubscript:
yyTraverseTreeTD (yyt->TableSubscript.Next);
yyt = yyt->TableSubscript.Expr; break;
case kTableRange:
yyTraverseTreeTD (yyt->TableRange.Next);
yyTraverseTreeTD (yyt->TableRange.From);
yyt = yyt->TableRange.For; break;
case kElementAccess:
yyt = yyt->ElementAccess.Next; break;
case kSubscript:
yyTraverseTreeTD (yyt->Subscript.Next);
yyt = yyt->Subscript.Expr; break;
case kRange:
yyTraverseTreeTD (yyt->Range.Next);
yyTraverseTreeTD (yyt->Range.From);
yyt = yyt->Range.For; break;
case kAnExpr:
yyTraverseTreeTD (yyt->AnExpr.Expr);
yyt = yyt->AnExpr.Next; break;
case kProcess:
yyt = yyt->Process.SpecList; break;
case kAssign:
yyTraverseTreeTD (yyt->Assign.SpecList);
yyTraverseTreeTD (yyt->Assign.Lval);
yyt = yyt->Assign.Rval; break;
case kAltType:
yyt = yyt->AltType.SpecList; break;
case kAlt:
yyTraverseTreeTD (yyt->Alt.SpecList);
yyt = yyt->Alt.AltList; break;
case kAltRep:
yyTraverseTreeTD (yyt->AltRep.SpecList);
yyTraverseTreeTD (yyt->AltRep.Replicator);
yyt = yyt->AltRep.AltList; break;
case kParType:
yyt = yyt->ParType.SpecList; break;
case kPar:
yyTraverseTreeTD (yyt->Par.SpecList);
yyt = yyt->Par.ProcList; break;
case kParRep:
yyTraverseTreeTD (yyt->ParRep.SpecList);
yyTraverseTreeTD (yyt->ParRep.Replicator);
yyt = yyt->ParRep.Process; break;
case kProcessor:
yyTraverseTreeTD (yyt->Processor.SpecList);
yyTraverseTreeTD (yyt->Processor.Expr);
yyt = yyt->Processor.Process; break;
case kIfType:
yyt = yyt->IfType.SpecList; break;
case kIf:
yyTraverseTreeTD (yyt->If.SpecList);
yyt = yyt->If.ChoiceList; break;
case kIfRep:
yyTraverseTreeTD (yyt->IfRep.SpecList);
yyTraverseTreeTD (yyt->IfRep.Replicator);
yyt = yyt->IfRep.ChoiceList; break;
case kInput:
yyTraverseTreeTD (yyt->Input.SpecList);
yyTraverseTreeTD (yyt->Input.Element);
yyt = yyt->Input.ExprList; break;
case kOutput:
yyTraverseTreeTD (yyt->Output.SpecList);
yyTraverseTreeTD (yyt->Output.Element);
yyt = yyt->Output.ExprList; break;
case kCaseIn:
yyTraverseTreeTD (yyt->CaseIn.SpecList);
yyTraverseTreeTD (yyt->CaseIn.Element);
yyt = yyt->CaseIn.CaseList; break;
case kAfter:
yyTraverseTreeTD (yyt->After.SpecList);
yyTraverseTreeTD (yyt->After.Element);
yyt = yyt->After.Expr; break;
case kSeqRep:
yyTraverseTreeTD (yyt->SeqRep.SpecList);
yyTraverseTreeTD (yyt->SeqRep.Replicator);
yyt = yyt->SeqRep.Process; break;
case kSeq:
yyTraverseTreeTD (yyt->Seq.SpecList);
yyt = yyt->Seq.ProcList; break;
case kCase:
yyTraverseTreeTD (yyt->Case.SpecList);
yyTraverseTreeTD (yyt->Case.Selector);
yyt = yyt->Case.ChoiceList; break;
case kWhile:
yyTraverseTreeTD (yyt->While.SpecList);
yyTraverseTreeTD (yyt->While.Expr);
yyt = yyt->While.Process; break;
case kSkip:
yyt = yyt->Skip.SpecList; break;
case kStop:
yyt = yyt->Stop.SpecList; break;
case kInstance:
yyTraverseTreeTD (yyt->Instance.SpecList);
yyt = yyt->Instance.ExprList; break;
case kCCode:
yyt = yyt->CCode.SpecList; break;
case kNullProcess:
yyt = yyt->NullProcess.SpecList; break;
case kAnAlts:
yyTraverseTreeTD (yyt->AnAlts.SpecList);
yyt = yyt->AnAlts.Next; break;
case kAnAlt:
yyTraverseTreeTD (yyt->AnAlt.SpecList);
yyTraverseTreeTD (yyt->AnAlt.Guard);
yyt = yyt->AnAlt.Next; break;
case kANewAlt:
yyTraverseTreeTD (yyt->ANewAlt.SpecList);
yyTraverseTreeTD (yyt->ANewAlt.AltType);
yyt = yyt->ANewAlt.Next; break;
case kGuard:
yyt = yyt->Guard.Expr; break;
case kAltInput:
yyTraverseTreeTD (yyt->AltInput.Expr);
yyTraverseTreeTD (yyt->AltInput.Input);
yyt = yyt->AltInput.Process; break;
case kAltTimer:
yyTraverseTreeTD (yyt->AltTimer.Expr);
yyTraverseTreeTD (yyt->AltTimer.After);
yyt = yyt->AltTimer.Process; break;
case kAltCaseIn:
yyTraverseTreeTD (yyt->AltCaseIn.Expr);
yyt = yyt->AltCaseIn.CaseIn; break;
case kNoInput:
yyTraverseTreeTD (yyt->NoInput.Expr);
yyt = yyt->NoInput.Process; break;
case kAChoices:
yyTraverseTreeTD (yyt->AChoices.SpecList);
yyt = yyt->AChoices.Next; break;
case kAChoice:
yyTraverseTreeTD (yyt->AChoice.SpecList);
yyTraverseTreeTD (yyt->AChoice.ExprList);
yyTraverseTreeTD (yyt->AChoice.Process);
yyt = yyt->AChoice.Next; break;
case kANewIf:
yyTraverseTreeTD (yyt->ANewIf.SpecList);
yyTraverseTreeTD (yyt->ANewIf.IfType);
yyt = yyt->ANewIf.Next; break;
case kAProcess:
yyTraverseTreeTD (yyt->AProcess.Process);
yyt = yyt->AProcess.Next; break;
case kACase:
yyTraverseTreeTD (yyt->ACase.SpecList);
yyTraverseTreeTD (yyt->ACase.ExprList);
yyTraverseTreeTD (yyt->ACase.Process);
yyt = yyt->ACase.Next; break;
case kAnAccess:
yyTraverseTreeTD (yyt->AnAccess.Next);
yyt = yyt->AnAccess.ElementList; break;
case kAnElement:
yyTraverseTreeTD (yyt->AnElement.Next);
yyt = yyt->AnElement.Element; break;
  default: return;
  }
 }
}

void TraverseTreeTD
# if defined __STDC__ | defined __cplusplus
 (tTree yyt, Tree_tProcTree yyyProc)
# else
 (yyt, yyyProc) tTree yyt; Tree_tProcTree yyyProc;
# endif
{
 yyMark (yyt);
 yyProc = yyyProc;
 yyTraverseTreeTD (yyt);
}

static void yyTraverseTreeBU
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 if (yyt == NoTree || yyt->yyHead.yyMark == 0) return;
 yyt->yyHead.yyMark = 0;

 switch (yyt->Kind) {
case kStart:
yyTraverseTreeBU (yyt->Start.Process); break;
case kASpec:
yyTraverseTreeBU (yyt->ASpec.Spec);
yyTraverseTreeBU (yyt->ASpec.Next); break;
case kDeclType:
yyTraverseTreeBU (yyt->DeclType.Type); break;
case kDecl:
yyTraverseTreeBU (yyt->Decl.Type);
yyTraverseTreeBU (yyt->Decl.IdList); break;
case kIs:
yyTraverseTreeBU (yyt->Is.Type);
yyTraverseTreeBU (yyt->Is.Elemental); break;
case kAVal:
yyTraverseTreeBU (yyt->AVal.Type);
yyTraverseTreeBU (yyt->AVal.Expr); break;
case kVal:
yyTraverseTreeBU (yyt->Val.Type);
yyTraverseTreeBU (yyt->Val.Expr); break;
case kValRetypes:
yyTraverseTreeBU (yyt->ValRetypes.Type);
yyTraverseTreeBU (yyt->ValRetypes.Expr); break;
case kValReshapes:
yyTraverseTreeBU (yyt->ValReshapes.Type);
yyTraverseTreeBU (yyt->ValReshapes.Expr); break;
case kRetypes:
yyTraverseTreeBU (yyt->Retypes.Type);
yyTraverseTreeBU (yyt->Retypes.Elemental); break;
case kReshapes:
yyTraverseTreeBU (yyt->Reshapes.Type);
yyTraverseTreeBU (yyt->Reshapes.Elemental); break;
case kDefSeqPro:
yyTraverseTreeBU (yyt->DefSeqPro.SeqProList); break;
case kDefTagPro:
yyTraverseTreeBU (yyt->DefTagPro.TagProList); break;
case kDefAProc:
yyTraverseTreeBU (yyt->DefAProc.Formals); break;
case kDefProc:
yyTraverseTreeBU (yyt->DefProc.Formals);
yyTraverseTreeBU (yyt->DefProc.Process); break;
case kInlineDefProc:
yyTraverseTreeBU (yyt->InlineDefProc.Formals);
yyTraverseTreeBU (yyt->InlineDefProc.Process); break;
case kPrototypeProc:
yyTraverseTreeBU (yyt->PrototypeProc.Formals); break;
case kDefAFunc:
yyTraverseTreeBU (yyt->DefAFunc.TypeList);
yyTraverseTreeBU (yyt->DefAFunc.Formals); break;
case kDefFunc:
yyTraverseTreeBU (yyt->DefFunc.TypeList);
yyTraverseTreeBU (yyt->DefFunc.Formals);
yyTraverseTreeBU (yyt->DefFunc.Valof); break;
case kInlineDefFunc:
yyTraverseTreeBU (yyt->InlineDefFunc.TypeList);
yyTraverseTreeBU (yyt->InlineDefFunc.Formals);
yyTraverseTreeBU (yyt->InlineDefFunc.Valof); break;
case kDefIsFunc:
yyTraverseTreeBU (yyt->DefIsFunc.TypeList);
yyTraverseTreeBU (yyt->DefIsFunc.Formals);
yyTraverseTreeBU (yyt->DefIsFunc.ExprList); break;
case kInlineDefIsFunc:
yyTraverseTreeBU (yyt->InlineDefIsFunc.TypeList);
yyTraverseTreeBU (yyt->InlineDefIsFunc.Formals);
yyTraverseTreeBU (yyt->InlineDefIsFunc.ExprList); break;
case kPrototypeFunc:
yyTraverseTreeBU (yyt->PrototypeFunc.TypeList);
yyTraverseTreeBU (yyt->PrototypeFunc.Formals); break;
case kAFormal:
yyTraverseTreeBU (yyt->AFormal.Next);
yyTraverseTreeBU (yyt->AFormal.By);
yyTraverseTreeBU (yyt->AFormal.Type); break;
case kReplicator:
yyTraverseTreeBU (yyt->Replicator.From);
yyTraverseTreeBU (yyt->Replicator.For); break;
case kATagPro:
yyTraverseTreeBU (yyt->ATagPro.SeqProList);
yyTraverseTreeBU (yyt->ATagPro.Next); break;
case kAnId:
yyTraverseTreeBU (yyt->AnId.Next); break;
case kArray:
yyTraverseTreeBU (yyt->Array.Expr);
yyTraverseTreeBU (yyt->Array.Type); break;
case kUserArray:
yyTraverseTreeBU (yyt->UserArray.Expr);
yyTraverseTreeBU (yyt->UserArray.Type); break;
case kChan:
yyTraverseTreeBU (yyt->Chan.Protocol); break;
case kUserStructTypes:
yyTraverseTreeBU (yyt->UserStructTypes.FieldList); break;
case kUserDataTypes:
yyTraverseTreeBU (yyt->UserDataTypes.FieldList); break;
case kUserRecord:
yyTraverseTreeBU (yyt->UserRecord.FieldList); break;
case kUserPackedRecord:
yyTraverseTreeBU (yyt->UserPackedRecord.FieldList); break;
case kUserUnion:
yyTraverseTreeBU (yyt->UserUnion.FieldList); break;
case kUserChanRecord:
yyTraverseTreeBU (yyt->UserChanRecord.FieldList); break;
case kAType:
yyTraverseTreeBU (yyt->AType.Type);
yyTraverseTreeBU (yyt->AType.Next); break;
case kAField:
yyTraverseTreeBU (yyt->AField.Type);
yyTraverseTreeBU (yyt->AField.Next); break;
case kSimple:
yyTraverseTreeBU (yyt->Simple.Type); break;
case kCounted:
yyTraverseTreeBU (yyt->Counted.Length);
yyTraverseTreeBU (yyt->Counted.Type); break;
case kASeqPro:
yyTraverseTreeBU (yyt->ASeqPro.Protocol);
yyTraverseTreeBU (yyt->ASeqPro.Next); break;
case kBinary:
yyTraverseTreeBU (yyt->Binary.Lop);
yyTraverseTreeBU (yyt->Binary.Rop);
yyTraverseTreeBU (yyt->Binary.Op); break;
case kUnary:
yyTraverseTreeBU (yyt->Unary.Expr);
yyTraverseTreeBU (yyt->Unary.Op); break;
case kConst:
yyTraverseTreeBU (yyt->Const.Type); break;
case kValof:
yyTraverseTreeBU (yyt->Valof.SpecList);
yyTraverseTreeBU (yyt->Valof.Process);
yyTraverseTreeBU (yyt->Valof.ExprList); break;
case kFuncCall:
yyTraverseTreeBU (yyt->FuncCall.ExprList); break;
case kElementalExpr:
yyTraverseTreeBU (yyt->ElementalExpr.Elemental); break;
case kConvert:
yyTraverseTreeBU (yyt->Convert.Type);
yyTraverseTreeBU (yyt->Convert.Expr); break;
case kRound:
yyTraverseTreeBU (yyt->Round.Type);
yyTraverseTreeBU (yyt->Round.Expr); break;
case kTrunc:
yyTraverseTreeBU (yyt->Trunc.Type);
yyTraverseTreeBU (yyt->Trunc.Expr); break;
case kMostPos:
yyTraverseTreeBU (yyt->MostPos.Type); break;
case kMostNeg:
yyTraverseTreeBU (yyt->MostNeg.Type); break;
case kCountArray:
yyTraverseTreeBU (yyt->CountArray.Length);
yyTraverseTreeBU (yyt->CountArray.Expr); break;
case kBytesInType:
yyTraverseTreeBU (yyt->BytesInType.Type); break;
case kBytesInExpr:
yyTraverseTreeBU (yyt->BytesInExpr.Expr); break;
case kSizeOf:
yyTraverseTreeBU (yyt->SizeOf.Type); break;
case kOffsetOf:
yyTraverseTreeBU (yyt->OffsetOf.Type);
yyTraverseTreeBU (yyt->OffsetOf.UnmappedIdentifier); break;
case kString:
yyTraverseTreeBU (yyt->String.Type); break;
case kStringSubscript:
yyTraverseTreeBU (yyt->StringSubscript.Next);
yyTraverseTreeBU (yyt->StringSubscript.Expr); break;
case kStringRange:
yyTraverseTreeBU (yyt->StringRange.Next);
yyTraverseTreeBU (yyt->StringRange.From);
yyTraverseTreeBU (yyt->StringRange.For); break;
case kTable:
yyTraverseTreeBU (yyt->Table.ExprList); break;
case kUserLiteral:
yyTraverseTreeBU (yyt->UserLiteral.ExprList);
yyTraverseTreeBU (yyt->UserLiteral.Type); break;
case kTableExpr:
yyTraverseTreeBU (yyt->TableExpr.Expr); break;
case kTableSubscript:
yyTraverseTreeBU (yyt->TableSubscript.Next);
yyTraverseTreeBU (yyt->TableSubscript.Expr); break;
case kTableRange:
yyTraverseTreeBU (yyt->TableRange.Next);
yyTraverseTreeBU (yyt->TableRange.From);
yyTraverseTreeBU (yyt->TableRange.For); break;
case kElementAccess:
yyTraverseTreeBU (yyt->ElementAccess.Next); break;
case kSubscript:
yyTraverseTreeBU (yyt->Subscript.Next);
yyTraverseTreeBU (yyt->Subscript.Expr); break;
case kRange:
yyTraverseTreeBU (yyt->Range.Next);
yyTraverseTreeBU (yyt->Range.From);
yyTraverseTreeBU (yyt->Range.For); break;
case kAnExpr:
yyTraverseTreeBU (yyt->AnExpr.Expr);
yyTraverseTreeBU (yyt->AnExpr.Next); break;
case kProcess:
yyTraverseTreeBU (yyt->Process.SpecList); break;
case kAssign:
yyTraverseTreeBU (yyt->Assign.SpecList);
yyTraverseTreeBU (yyt->Assign.Lval);
yyTraverseTreeBU (yyt->Assign.Rval); break;
case kAltType:
yyTraverseTreeBU (yyt->AltType.SpecList); break;
case kAlt:
yyTraverseTreeBU (yyt->Alt.SpecList);
yyTraverseTreeBU (yyt->Alt.AltList); break;
case kAltRep:
yyTraverseTreeBU (yyt->AltRep.SpecList);
yyTraverseTreeBU (yyt->AltRep.Replicator);
yyTraverseTreeBU (yyt->AltRep.AltList); break;
case kParType:
yyTraverseTreeBU (yyt->ParType.SpecList); break;
case kPar:
yyTraverseTreeBU (yyt->Par.SpecList);
yyTraverseTreeBU (yyt->Par.ProcList); break;
case kParRep:
yyTraverseTreeBU (yyt->ParRep.SpecList);
yyTraverseTreeBU (yyt->ParRep.Replicator);
yyTraverseTreeBU (yyt->ParRep.Process); break;
case kProcessor:
yyTraverseTreeBU (yyt->Processor.SpecList);
yyTraverseTreeBU (yyt->Processor.Expr);
yyTraverseTreeBU (yyt->Processor.Process); break;
case kIfType:
yyTraverseTreeBU (yyt->IfType.SpecList); break;
case kIf:
yyTraverseTreeBU (yyt->If.SpecList);
yyTraverseTreeBU (yyt->If.ChoiceList); break;
case kIfRep:
yyTraverseTreeBU (yyt->IfRep.SpecList);
yyTraverseTreeBU (yyt->IfRep.Replicator);
yyTraverseTreeBU (yyt->IfRep.ChoiceList); break;
case kInput:
yyTraverseTreeBU (yyt->Input.SpecList);
yyTraverseTreeBU (yyt->Input.Element);
yyTraverseTreeBU (yyt->Input.ExprList); break;
case kOutput:
yyTraverseTreeBU (yyt->Output.SpecList);
yyTraverseTreeBU (yyt->Output.Element);
yyTraverseTreeBU (yyt->Output.ExprList); break;
case kCaseIn:
yyTraverseTreeBU (yyt->CaseIn.SpecList);
yyTraverseTreeBU (yyt->CaseIn.Element);
yyTraverseTreeBU (yyt->CaseIn.CaseList); break;
case kAfter:
yyTraverseTreeBU (yyt->After.SpecList);
yyTraverseTreeBU (yyt->After.Element);
yyTraverseTreeBU (yyt->After.Expr); break;
case kSeqRep:
yyTraverseTreeBU (yyt->SeqRep.SpecList);
yyTraverseTreeBU (yyt->SeqRep.Replicator);
yyTraverseTreeBU (yyt->SeqRep.Process); break;
case kSeq:
yyTraverseTreeBU (yyt->Seq.SpecList);
yyTraverseTreeBU (yyt->Seq.ProcList); break;
case kCase:
yyTraverseTreeBU (yyt->Case.SpecList);
yyTraverseTreeBU (yyt->Case.Selector);
yyTraverseTreeBU (yyt->Case.ChoiceList); break;
case kWhile:
yyTraverseTreeBU (yyt->While.SpecList);
yyTraverseTreeBU (yyt->While.Expr);
yyTraverseTreeBU (yyt->While.Process); break;
case kSkip:
yyTraverseTreeBU (yyt->Skip.SpecList); break;
case kStop:
yyTraverseTreeBU (yyt->Stop.SpecList); break;
case kInstance:
yyTraverseTreeBU (yyt->Instance.SpecList);
yyTraverseTreeBU (yyt->Instance.ExprList); break;
case kCCode:
yyTraverseTreeBU (yyt->CCode.SpecList); break;
case kNullProcess:
yyTraverseTreeBU (yyt->NullProcess.SpecList); break;
case kAnAlts:
yyTraverseTreeBU (yyt->AnAlts.SpecList);
yyTraverseTreeBU (yyt->AnAlts.Next); break;
case kAnAlt:
yyTraverseTreeBU (yyt->AnAlt.SpecList);
yyTraverseTreeBU (yyt->AnAlt.Guard);
yyTraverseTreeBU (yyt->AnAlt.Next); break;
case kANewAlt:
yyTraverseTreeBU (yyt->ANewAlt.SpecList);
yyTraverseTreeBU (yyt->ANewAlt.AltType);
yyTraverseTreeBU (yyt->ANewAlt.Next); break;
case kGuard:
yyTraverseTreeBU (yyt->Guard.Expr); break;
case kAltInput:
yyTraverseTreeBU (yyt->AltInput.Expr);
yyTraverseTreeBU (yyt->AltInput.Input);
yyTraverseTreeBU (yyt->AltInput.Process); break;
case kAltTimer:
yyTraverseTreeBU (yyt->AltTimer.Expr);
yyTraverseTreeBU (yyt->AltTimer.After);
yyTraverseTreeBU (yyt->AltTimer.Process); break;
case kAltCaseIn:
yyTraverseTreeBU (yyt->AltCaseIn.Expr);
yyTraverseTreeBU (yyt->AltCaseIn.CaseIn); break;
case kNoInput:
yyTraverseTreeBU (yyt->NoInput.Expr);
yyTraverseTreeBU (yyt->NoInput.Process); break;
case kAChoices:
yyTraverseTreeBU (yyt->AChoices.SpecList);
yyTraverseTreeBU (yyt->AChoices.Next); break;
case kAChoice:
yyTraverseTreeBU (yyt->AChoice.SpecList);
yyTraverseTreeBU (yyt->AChoice.ExprList);
yyTraverseTreeBU (yyt->AChoice.Process);
yyTraverseTreeBU (yyt->AChoice.Next); break;
case kANewIf:
yyTraverseTreeBU (yyt->ANewIf.SpecList);
yyTraverseTreeBU (yyt->ANewIf.IfType);
yyTraverseTreeBU (yyt->ANewIf.Next); break;
case kAProcess:
yyTraverseTreeBU (yyt->AProcess.Process);
yyTraverseTreeBU (yyt->AProcess.Next); break;
case kACase:
yyTraverseTreeBU (yyt->ACase.SpecList);
yyTraverseTreeBU (yyt->ACase.ExprList);
yyTraverseTreeBU (yyt->ACase.Process);
yyTraverseTreeBU (yyt->ACase.Next); break;
case kAnAccess:
yyTraverseTreeBU (yyt->AnAccess.Next);
yyTraverseTreeBU (yyt->AnAccess.ElementList); break;
case kAnElement:
yyTraverseTreeBU (yyt->AnElement.Next);
yyTraverseTreeBU (yyt->AnElement.Element); break;
 default: ;
 }
 yyProc (yyt);
}

void TraverseTreeBU
# if defined __STDC__ | defined __cplusplus
 (tTree yyt, Tree_tProcTree yyyProc)
# else
 (yyt, yyyProc) tTree yyt; Tree_tProcTree yyyProc;
# endif
{
 yyMark (yyt);
 yyProc = yyyProc;
 yyTraverseTreeBU (yyt);
}

tTree ReverseTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyOld)
# else
 (yyOld) tTree yyOld;
# endif
{
 register tTree yyNew, yyNext, yyTail;
 yyNew = yyOld;
 yyTail = yyOld;
 for (;;) {
  switch (yyOld->Kind) {
case kASpec: yyNext = yyOld->ASpec.Next; yyOld->ASpec.Next = yyNew; break;
case kATagPro: yyNext = yyOld->ATagPro.Next; yyOld->ATagPro.Next = yyNew; break;
case kAnId: yyNext = yyOld->AnId.Next; yyOld->AnId.Next = yyNew; break;
case kAType: yyNext = yyOld->AType.Next; yyOld->AType.Next = yyNew; break;
case kAField: yyNext = yyOld->AField.Next; yyOld->AField.Next = yyNew; break;
case kASeqPro: yyNext = yyOld->ASeqPro.Next; yyOld->ASeqPro.Next = yyNew; break;
case kAnExpr: yyNext = yyOld->AnExpr.Next; yyOld->AnExpr.Next = yyNew; break;
case kAnAlts: yyNext = yyOld->AnAlts.Next; yyOld->AnAlts.Next = yyNew; break;
case kAnAlt: yyNext = yyOld->AnAlt.Next; yyOld->AnAlt.Next = yyNew; break;
case kANewAlt: yyNext = yyOld->ANewAlt.Next; yyOld->ANewAlt.Next = yyNew; break;
case kAChoices: yyNext = yyOld->AChoices.Next; yyOld->AChoices.Next = yyNew; break;
case kAChoice: yyNext = yyOld->AChoice.Next; yyOld->AChoice.Next = yyNew; break;
case kANewIf: yyNext = yyOld->ANewIf.Next; yyOld->ANewIf.Next = yyNew; break;
case kAProcess: yyNext = yyOld->AProcess.Next; yyOld->AProcess.Next = yyNew; break;
case kACase: yyNext = yyOld->ACase.Next; yyOld->ACase.Next = yyNew; break;
  default: goto yyExit;
  }
  yyNew = yyOld;
  yyOld = yyNext;
 }
yyExit:
 switch (yyTail->Kind) {
case kASpec: yyTail->ASpec.Next = yyOld; break;
case kATagPro: yyTail->ATagPro.Next = yyOld; break;
case kAnId: yyTail->AnId.Next = yyOld; break;
case kAType: yyTail->AType.Next = yyOld; break;
case kAField: yyTail->AField.Next = yyOld; break;
case kASeqPro: yyTail->ASeqPro.Next = yyOld; break;
case kAnExpr: yyTail->AnExpr.Next = yyOld; break;
case kAnAlts: yyTail->AnAlts.Next = yyOld; break;
case kAnAlt: yyTail->AnAlt.Next = yyOld; break;
case kANewAlt: yyTail->ANewAlt.Next = yyOld; break;
case kAChoices: yyTail->AChoices.Next = yyOld; break;
case kAChoice: yyTail->AChoice.Next = yyOld; break;
case kANewIf: yyTail->ANewIf.Next = yyOld; break;
case kAProcess: yyTail->AProcess.Next = yyOld; break;
case kACase: yyTail->ACase.Next = yyOld; break;
 default: ;
 }
 return yyNew;
}

# define yyInitOldToNewStoreSize 32

typedef struct { tTree yyOld, yyNew; } yytOldToNew;
static unsigned long yyOldToNewStoreSize = yyInitOldToNewStoreSize;
static yytOldToNew yyOldToNewStore [yyInitOldToNewStoreSize];
static yytOldToNew * yyOldToNewStorePtr = yyOldToNewStore;
static int yyOldToNewCount;

static void yyStoreOldToNew
# if defined __STDC__ | defined __cplusplus
 (tTree yyOld, tTree yyNew)
# else
 (yyOld, yyNew) tTree yyOld, yyNew;
# endif
{
 if (++ yyOldToNewCount == yyOldToNewStoreSize)
  ExtendArray ((char * *) & yyOldToNewStorePtr, & yyOldToNewStoreSize, sizeof (yytOldToNew));
 yyOldToNewStorePtr [yyOldToNewCount].yyOld = yyOld;
 yyOldToNewStorePtr [yyOldToNewCount].yyNew = yyNew;
}

static tTree yyMapOldToNew
# if defined __STDC__ | defined __cplusplus
 (tTree yyOld)
# else
 (yyOld) tTree yyOld;
# endif
{
 register int yyi;
 for (yyi = 1; yyi <= yyOldToNewCount; yyi ++)
  if (yyOldToNewStorePtr [yyi].yyOld == yyOld) return yyOldToNewStorePtr [yyi].yyNew;
}

static tTree yyCopyTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt, yyPtrtTree yyNew)
# else
 (yyt, yyNew) tTree yyt; yyPtrtTree yyNew;
# endif
{
 for (;;) {
  if (yyt == NoTree) { * yyNew = NoTree; return; }
  if (yyt->yyHead.yyMark == 0) { * yyNew = yyMapOldToNew (yyt); return; }
  yyALLOC (* yyNew, Tree_NodeSize [yyt->Kind])
  if (yyt->yyHead.yyMark > 1) { yyStoreOldToNew (yyt, * yyNew); }
  yyt->yyHead.yyMark = 0;

  switch (yyt->Kind) {
case kStart: (* yyNew)->Start = yyt->Start;
copytChain ((* yyNew)->Start.CGNext, yyt->Start.CGNext)
copytSymbol ((* yyNew)->Start.OuterProc, yyt->Start.OuterProc)
yyt = yyt->Start.Process;
yyNew = & (* yyNew)->Start.Process; break;
case kSpecList: (* yyNew)->SpecList = yyt->SpecList;
copyint ((* yyNew)->SpecList.V1In, yyt->SpecList.V1In)
copyint ((* yyNew)->SpecList.V1Out, yyt->SpecList.V1Out)
copyint ((* yyNew)->SpecList.VS, yyt->SpecList.VS)
copyint ((* yyNew)->SpecList.VIn, yyt->SpecList.VIn)
copyint ((* yyNew)->SpecList.VOut, yyt->SpecList.VOut)
copyint ((* yyNew)->SpecList.DCL, yyt->SpecList.DCL)
copytSymbol ((* yyNew)->SpecList.Parent, yyt->SpecList.Parent)
copybool ((* yyNew)->SpecList.AtomicSpec, yyt->SpecList.AtomicSpec)
copytParUsage ((* yyNew)->SpecList.PUIn, yyt->SpecList.PUIn)
copytParUsage ((* yyNew)->SpecList.PUOut, yyt->SpecList.PUOut)
return;
case kNoSpec: (* yyNew)->NoSpec = yyt->NoSpec;
copyint ((* yyNew)->NoSpec.V1In, yyt->NoSpec.V1In)
copyint ((* yyNew)->NoSpec.V1Out, yyt->NoSpec.V1Out)
copyint ((* yyNew)->NoSpec.VS, yyt->NoSpec.VS)
copyint ((* yyNew)->NoSpec.VIn, yyt->NoSpec.VIn)
copyint ((* yyNew)->NoSpec.VOut, yyt->NoSpec.VOut)
copyint ((* yyNew)->NoSpec.DCL, yyt->NoSpec.DCL)
copytSymbol ((* yyNew)->NoSpec.Parent, yyt->NoSpec.Parent)
copybool ((* yyNew)->NoSpec.AtomicSpec, yyt->NoSpec.AtomicSpec)
copytParUsage ((* yyNew)->NoSpec.PUIn, yyt->NoSpec.PUIn)
copytParUsage ((* yyNew)->NoSpec.PUOut, yyt->NoSpec.PUOut)
return;
case kASpec: (* yyNew)->ASpec = yyt->ASpec;
copyint ((* yyNew)->ASpec.V1In, yyt->ASpec.V1In)
copyint ((* yyNew)->ASpec.V1Out, yyt->ASpec.V1Out)
copyint ((* yyNew)->ASpec.VS, yyt->ASpec.VS)
copyint ((* yyNew)->ASpec.VIn, yyt->ASpec.VIn)
copyint ((* yyNew)->ASpec.VOut, yyt->ASpec.VOut)
copyint ((* yyNew)->ASpec.DCL, yyt->ASpec.DCL)
copytSymbol ((* yyNew)->ASpec.Parent, yyt->ASpec.Parent)
copybool ((* yyNew)->ASpec.AtomicSpec, yyt->ASpec.AtomicSpec)
copytParUsage ((* yyNew)->ASpec.PUIn, yyt->ASpec.PUIn)
copytParUsage ((* yyNew)->ASpec.PUOut, yyt->ASpec.PUOut)
copytTree ((* yyNew)->ASpec.Spec, yyt->ASpec.Spec)
yyt = yyt->ASpec.Next;
yyNew = & (* yyNew)->ASpec.Next; break;
case kSpec: (* yyNew)->Spec = yyt->Spec;
copytPosition ((* yyNew)->Spec.Pos, yyt->Spec.Pos)
copyint ((* yyNew)->Spec.V1In, yyt->Spec.V1In)
copyint ((* yyNew)->Spec.V1Out, yyt->Spec.V1Out)
copyint ((* yyNew)->Spec.VS, yyt->Spec.VS)
copyint ((* yyNew)->Spec.VIn, yyt->Spec.VIn)
copyint ((* yyNew)->Spec.VOut, yyt->Spec.VOut)
copytSymbol ((* yyNew)->Spec.Parent, yyt->Spec.Parent)
copybool ((* yyNew)->Spec.AtomicSpec, yyt->Spec.AtomicSpec)
copytParUsage ((* yyNew)->Spec.PUIn, yyt->Spec.PUIn)
copytParUsage ((* yyNew)->Spec.PUOut, yyt->Spec.PUOut)
return;
case kDeclType: (* yyNew)->DeclType = yyt->DeclType;
copytPosition ((* yyNew)->DeclType.Pos, yyt->DeclType.Pos)
copyint ((* yyNew)->DeclType.V1In, yyt->DeclType.V1In)
copyint ((* yyNew)->DeclType.V1Out, yyt->DeclType.V1Out)
copyint ((* yyNew)->DeclType.VS, yyt->DeclType.VS)
copyint ((* yyNew)->DeclType.VIn, yyt->DeclType.VIn)
copyint ((* yyNew)->DeclType.VOut, yyt->DeclType.VOut)
copytSymbol ((* yyNew)->DeclType.Parent, yyt->DeclType.Parent)
copybool ((* yyNew)->DeclType.AtomicSpec, yyt->DeclType.AtomicSpec)
copytParUsage ((* yyNew)->DeclType.PUIn, yyt->DeclType.PUIn)
copytParUsage ((* yyNew)->DeclType.PUOut, yyt->DeclType.PUOut)
yyt = yyt->DeclType.Type;
yyNew = & (* yyNew)->DeclType.Type; break;
case kDecl: (* yyNew)->Decl = yyt->Decl;
copytPosition ((* yyNew)->Decl.Pos, yyt->Decl.Pos)
copyint ((* yyNew)->Decl.V1In, yyt->Decl.V1In)
copyint ((* yyNew)->Decl.V1Out, yyt->Decl.V1Out)
copyint ((* yyNew)->Decl.VS, yyt->Decl.VS)
copyint ((* yyNew)->Decl.VIn, yyt->Decl.VIn)
copyint ((* yyNew)->Decl.VOut, yyt->Decl.VOut)
copytSymbol ((* yyNew)->Decl.Parent, yyt->Decl.Parent)
copybool ((* yyNew)->Decl.AtomicSpec, yyt->Decl.AtomicSpec)
copytParUsage ((* yyNew)->Decl.PUIn, yyt->Decl.PUIn)
copytParUsage ((* yyNew)->Decl.PUOut, yyt->Decl.PUOut)
copytTree ((* yyNew)->Decl.Type, yyt->Decl.Type)
yyt = yyt->Decl.IdList;
yyNew = & (* yyNew)->Decl.IdList; break;
case kIs: (* yyNew)->Is = yyt->Is;
copytPosition ((* yyNew)->Is.Pos, yyt->Is.Pos)
copyint ((* yyNew)->Is.V1In, yyt->Is.V1In)
copyint ((* yyNew)->Is.V1Out, yyt->Is.V1Out)
copyint ((* yyNew)->Is.VS, yyt->Is.VS)
copyint ((* yyNew)->Is.VIn, yyt->Is.VIn)
copyint ((* yyNew)->Is.VOut, yyt->Is.VOut)
copytSymbol ((* yyNew)->Is.Parent, yyt->Is.Parent)
copybool ((* yyNew)->Is.AtomicSpec, yyt->Is.AtomicSpec)
copytParUsage ((* yyNew)->Is.PUIn, yyt->Is.PUIn)
copytParUsage ((* yyNew)->Is.PUOut, yyt->Is.PUOut)
copytTree ((* yyNew)->Is.Type, yyt->Is.Type)
copytSymbol ((* yyNew)->Is.Sym, yyt->Is.Sym)
copytUsage ((* yyNew)->Is.U, yyt->Is.U)
yyt = yyt->Is.Elemental;
yyNew = & (* yyNew)->Is.Elemental; break;
case kAVal: (* yyNew)->AVal = yyt->AVal;
copytPosition ((* yyNew)->AVal.Pos, yyt->AVal.Pos)
copyint ((* yyNew)->AVal.V1In, yyt->AVal.V1In)
copyint ((* yyNew)->AVal.V1Out, yyt->AVal.V1Out)
copyint ((* yyNew)->AVal.VS, yyt->AVal.VS)
copyint ((* yyNew)->AVal.VIn, yyt->AVal.VIn)
copyint ((* yyNew)->AVal.VOut, yyt->AVal.VOut)
copytSymbol ((* yyNew)->AVal.Parent, yyt->AVal.Parent)
copybool ((* yyNew)->AVal.AtomicSpec, yyt->AVal.AtomicSpec)
copytParUsage ((* yyNew)->AVal.PUIn, yyt->AVal.PUIn)
copytParUsage ((* yyNew)->AVal.PUOut, yyt->AVal.PUOut)
copytTree ((* yyNew)->AVal.Type, yyt->AVal.Type)
copytSymbol ((* yyNew)->AVal.Sym, yyt->AVal.Sym)
yyt = yyt->AVal.Expr;
yyNew = & (* yyNew)->AVal.Expr; break;
case kVal: (* yyNew)->Val = yyt->Val;
copytPosition ((* yyNew)->Val.Pos, yyt->Val.Pos)
copyint ((* yyNew)->Val.V1In, yyt->Val.V1In)
copyint ((* yyNew)->Val.V1Out, yyt->Val.V1Out)
copyint ((* yyNew)->Val.VS, yyt->Val.VS)
copyint ((* yyNew)->Val.VIn, yyt->Val.VIn)
copyint ((* yyNew)->Val.VOut, yyt->Val.VOut)
copytSymbol ((* yyNew)->Val.Parent, yyt->Val.Parent)
copybool ((* yyNew)->Val.AtomicSpec, yyt->Val.AtomicSpec)
copytParUsage ((* yyNew)->Val.PUIn, yyt->Val.PUIn)
copytParUsage ((* yyNew)->Val.PUOut, yyt->Val.PUOut)
copytTree ((* yyNew)->Val.Type, yyt->Val.Type)
copytSymbol ((* yyNew)->Val.Sym, yyt->Val.Sym)
yyt = yyt->Val.Expr;
yyNew = & (* yyNew)->Val.Expr; break;
case kValRetypes: (* yyNew)->ValRetypes = yyt->ValRetypes;
copytPosition ((* yyNew)->ValRetypes.Pos, yyt->ValRetypes.Pos)
copyint ((* yyNew)->ValRetypes.V1In, yyt->ValRetypes.V1In)
copyint ((* yyNew)->ValRetypes.V1Out, yyt->ValRetypes.V1Out)
copyint ((* yyNew)->ValRetypes.VS, yyt->ValRetypes.VS)
copyint ((* yyNew)->ValRetypes.VIn, yyt->ValRetypes.VIn)
copyint ((* yyNew)->ValRetypes.VOut, yyt->ValRetypes.VOut)
copytSymbol ((* yyNew)->ValRetypes.Parent, yyt->ValRetypes.Parent)
copybool ((* yyNew)->ValRetypes.AtomicSpec, yyt->ValRetypes.AtomicSpec)
copytParUsage ((* yyNew)->ValRetypes.PUIn, yyt->ValRetypes.PUIn)
copytParUsage ((* yyNew)->ValRetypes.PUOut, yyt->ValRetypes.PUOut)
copytTree ((* yyNew)->ValRetypes.Type, yyt->ValRetypes.Type)
copytSymbol ((* yyNew)->ValRetypes.Sym, yyt->ValRetypes.Sym)
yyt = yyt->ValRetypes.Expr;
yyNew = & (* yyNew)->ValRetypes.Expr; break;
case kValReshapes: (* yyNew)->ValReshapes = yyt->ValReshapes;
copytPosition ((* yyNew)->ValReshapes.Pos, yyt->ValReshapes.Pos)
copyint ((* yyNew)->ValReshapes.V1In, yyt->ValReshapes.V1In)
copyint ((* yyNew)->ValReshapes.V1Out, yyt->ValReshapes.V1Out)
copyint ((* yyNew)->ValReshapes.VS, yyt->ValReshapes.VS)
copyint ((* yyNew)->ValReshapes.VIn, yyt->ValReshapes.VIn)
copyint ((* yyNew)->ValReshapes.VOut, yyt->ValReshapes.VOut)
copytSymbol ((* yyNew)->ValReshapes.Parent, yyt->ValReshapes.Parent)
copybool ((* yyNew)->ValReshapes.AtomicSpec, yyt->ValReshapes.AtomicSpec)
copytParUsage ((* yyNew)->ValReshapes.PUIn, yyt->ValReshapes.PUIn)
copytParUsage ((* yyNew)->ValReshapes.PUOut, yyt->ValReshapes.PUOut)
copytTree ((* yyNew)->ValReshapes.Type, yyt->ValReshapes.Type)
copytSymbol ((* yyNew)->ValReshapes.Sym, yyt->ValReshapes.Sym)
yyt = yyt->ValReshapes.Expr;
yyNew = & (* yyNew)->ValReshapes.Expr; break;
case kRetypes: (* yyNew)->Retypes = yyt->Retypes;
copytPosition ((* yyNew)->Retypes.Pos, yyt->Retypes.Pos)
copyint ((* yyNew)->Retypes.V1In, yyt->Retypes.V1In)
copyint ((* yyNew)->Retypes.V1Out, yyt->Retypes.V1Out)
copyint ((* yyNew)->Retypes.VS, yyt->Retypes.VS)
copyint ((* yyNew)->Retypes.VIn, yyt->Retypes.VIn)
copyint ((* yyNew)->Retypes.VOut, yyt->Retypes.VOut)
copytSymbol ((* yyNew)->Retypes.Parent, yyt->Retypes.Parent)
copybool ((* yyNew)->Retypes.AtomicSpec, yyt->Retypes.AtomicSpec)
copytParUsage ((* yyNew)->Retypes.PUIn, yyt->Retypes.PUIn)
copytParUsage ((* yyNew)->Retypes.PUOut, yyt->Retypes.PUOut)
copytTree ((* yyNew)->Retypes.Type, yyt->Retypes.Type)
copytSymbol ((* yyNew)->Retypes.Sym, yyt->Retypes.Sym)
yyt = yyt->Retypes.Elemental;
yyNew = & (* yyNew)->Retypes.Elemental; break;
case kReshapes: (* yyNew)->Reshapes = yyt->Reshapes;
copytPosition ((* yyNew)->Reshapes.Pos, yyt->Reshapes.Pos)
copyint ((* yyNew)->Reshapes.V1In, yyt->Reshapes.V1In)
copyint ((* yyNew)->Reshapes.V1Out, yyt->Reshapes.V1Out)
copyint ((* yyNew)->Reshapes.VS, yyt->Reshapes.VS)
copyint ((* yyNew)->Reshapes.VIn, yyt->Reshapes.VIn)
copyint ((* yyNew)->Reshapes.VOut, yyt->Reshapes.VOut)
copytSymbol ((* yyNew)->Reshapes.Parent, yyt->Reshapes.Parent)
copybool ((* yyNew)->Reshapes.AtomicSpec, yyt->Reshapes.AtomicSpec)
copytParUsage ((* yyNew)->Reshapes.PUIn, yyt->Reshapes.PUIn)
copytParUsage ((* yyNew)->Reshapes.PUOut, yyt->Reshapes.PUOut)
copytTree ((* yyNew)->Reshapes.Type, yyt->Reshapes.Type)
copytSymbol ((* yyNew)->Reshapes.Sym, yyt->Reshapes.Sym)
yyt = yyt->Reshapes.Elemental;
yyNew = & (* yyNew)->Reshapes.Elemental; break;
case kDefAPro: (* yyNew)->DefAPro = yyt->DefAPro;
copytPosition ((* yyNew)->DefAPro.Pos, yyt->DefAPro.Pos)
copyint ((* yyNew)->DefAPro.V1In, yyt->DefAPro.V1In)
copyint ((* yyNew)->DefAPro.V1Out, yyt->DefAPro.V1Out)
copyint ((* yyNew)->DefAPro.VS, yyt->DefAPro.VS)
copyint ((* yyNew)->DefAPro.VIn, yyt->DefAPro.VIn)
copyint ((* yyNew)->DefAPro.VOut, yyt->DefAPro.VOut)
copytSymbol ((* yyNew)->DefAPro.Parent, yyt->DefAPro.Parent)
copybool ((* yyNew)->DefAPro.AtomicSpec, yyt->DefAPro.AtomicSpec)
copytParUsage ((* yyNew)->DefAPro.PUIn, yyt->DefAPro.PUIn)
copytParUsage ((* yyNew)->DefAPro.PUOut, yyt->DefAPro.PUOut)
return;
case kDefSeqPro: (* yyNew)->DefSeqPro = yyt->DefSeqPro;
copytPosition ((* yyNew)->DefSeqPro.Pos, yyt->DefSeqPro.Pos)
copyint ((* yyNew)->DefSeqPro.V1In, yyt->DefSeqPro.V1In)
copyint ((* yyNew)->DefSeqPro.V1Out, yyt->DefSeqPro.V1Out)
copyint ((* yyNew)->DefSeqPro.VS, yyt->DefSeqPro.VS)
copyint ((* yyNew)->DefSeqPro.VIn, yyt->DefSeqPro.VIn)
copyint ((* yyNew)->DefSeqPro.VOut, yyt->DefSeqPro.VOut)
copytSymbol ((* yyNew)->DefSeqPro.Parent, yyt->DefSeqPro.Parent)
copybool ((* yyNew)->DefSeqPro.AtomicSpec, yyt->DefSeqPro.AtomicSpec)
copytParUsage ((* yyNew)->DefSeqPro.PUIn, yyt->DefSeqPro.PUIn)
copytParUsage ((* yyNew)->DefSeqPro.PUOut, yyt->DefSeqPro.PUOut)
copytSymbol ((* yyNew)->DefSeqPro.Sym, yyt->DefSeqPro.Sym)
yyt = yyt->DefSeqPro.SeqProList;
yyNew = & (* yyNew)->DefSeqPro.SeqProList; break;
case kDefTagPro: (* yyNew)->DefTagPro = yyt->DefTagPro;
copytPosition ((* yyNew)->DefTagPro.Pos, yyt->DefTagPro.Pos)
copyint ((* yyNew)->DefTagPro.V1In, yyt->DefTagPro.V1In)
copyint ((* yyNew)->DefTagPro.V1Out, yyt->DefTagPro.V1Out)
copyint ((* yyNew)->DefTagPro.VS, yyt->DefTagPro.VS)
copyint ((* yyNew)->DefTagPro.VIn, yyt->DefTagPro.VIn)
copyint ((* yyNew)->DefTagPro.VOut, yyt->DefTagPro.VOut)
copytSymbol ((* yyNew)->DefTagPro.Parent, yyt->DefTagPro.Parent)
copybool ((* yyNew)->DefTagPro.AtomicSpec, yyt->DefTagPro.AtomicSpec)
copytParUsage ((* yyNew)->DefTagPro.PUIn, yyt->DefTagPro.PUIn)
copytParUsage ((* yyNew)->DefTagPro.PUOut, yyt->DefTagPro.PUOut)
copytSymbol ((* yyNew)->DefTagPro.Sym, yyt->DefTagPro.Sym)
yyt = yyt->DefTagPro.TagProList;
yyNew = & (* yyNew)->DefTagPro.TagProList; break;
case kDefAProc: (* yyNew)->DefAProc = yyt->DefAProc;
copytPosition ((* yyNew)->DefAProc.Pos, yyt->DefAProc.Pos)
copyint ((* yyNew)->DefAProc.V1In, yyt->DefAProc.V1In)
copyint ((* yyNew)->DefAProc.V1Out, yyt->DefAProc.V1Out)
copyint ((* yyNew)->DefAProc.VS, yyt->DefAProc.VS)
copyint ((* yyNew)->DefAProc.VIn, yyt->DefAProc.VIn)
copyint ((* yyNew)->DefAProc.VOut, yyt->DefAProc.VOut)
copytSymbol ((* yyNew)->DefAProc.Parent, yyt->DefAProc.Parent)
copybool ((* yyNew)->DefAProc.AtomicSpec, yyt->DefAProc.AtomicSpec)
copytParUsage ((* yyNew)->DefAProc.PUIn, yyt->DefAProc.PUIn)
copytParUsage ((* yyNew)->DefAProc.PUOut, yyt->DefAProc.PUOut)
copytSymbol ((* yyNew)->DefAProc.Sym, yyt->DefAProc.Sym)
copytUsage ((* yyNew)->DefAProc.U, yyt->DefAProc.U)
copytChain ((* yyNew)->DefAProc.CGNext, yyt->DefAProc.CGNext)
yyt = yyt->DefAProc.Formals;
yyNew = & (* yyNew)->DefAProc.Formals; break;
case kDefProc: (* yyNew)->DefProc = yyt->DefProc;
copytPosition ((* yyNew)->DefProc.Pos, yyt->DefProc.Pos)
copyint ((* yyNew)->DefProc.V1In, yyt->DefProc.V1In)
copyint ((* yyNew)->DefProc.V1Out, yyt->DefProc.V1Out)
copyint ((* yyNew)->DefProc.VS, yyt->DefProc.VS)
copyint ((* yyNew)->DefProc.VIn, yyt->DefProc.VIn)
copyint ((* yyNew)->DefProc.VOut, yyt->DefProc.VOut)
copytSymbol ((* yyNew)->DefProc.Parent, yyt->DefProc.Parent)
copybool ((* yyNew)->DefProc.AtomicSpec, yyt->DefProc.AtomicSpec)
copytParUsage ((* yyNew)->DefProc.PUIn, yyt->DefProc.PUIn)
copytParUsage ((* yyNew)->DefProc.PUOut, yyt->DefProc.PUOut)
copytSymbol ((* yyNew)->DefProc.Sym, yyt->DefProc.Sym)
copytTree ((* yyNew)->DefProc.Formals, yyt->DefProc.Formals)
copytUsage ((* yyNew)->DefProc.U, yyt->DefProc.U)
copytChain ((* yyNew)->DefProc.CGNext, yyt->DefProc.CGNext)
copybool ((* yyNew)->DefProc.ATOMIC, yyt->DefProc.ATOMIC)
copybool ((* yyNew)->DefProc.isPar, yyt->DefProc.isPar)
yyt = yyt->DefProc.Process;
yyNew = & (* yyNew)->DefProc.Process; break;
case kInlineDefProc: (* yyNew)->InlineDefProc = yyt->InlineDefProc;
copytPosition ((* yyNew)->InlineDefProc.Pos, yyt->InlineDefProc.Pos)
copyint ((* yyNew)->InlineDefProc.V1In, yyt->InlineDefProc.V1In)
copyint ((* yyNew)->InlineDefProc.V1Out, yyt->InlineDefProc.V1Out)
copyint ((* yyNew)->InlineDefProc.VS, yyt->InlineDefProc.VS)
copyint ((* yyNew)->InlineDefProc.VIn, yyt->InlineDefProc.VIn)
copyint ((* yyNew)->InlineDefProc.VOut, yyt->InlineDefProc.VOut)
copytSymbol ((* yyNew)->InlineDefProc.Parent, yyt->InlineDefProc.Parent)
copybool ((* yyNew)->InlineDefProc.AtomicSpec, yyt->InlineDefProc.AtomicSpec)
copytParUsage ((* yyNew)->InlineDefProc.PUIn, yyt->InlineDefProc.PUIn)
copytParUsage ((* yyNew)->InlineDefProc.PUOut, yyt->InlineDefProc.PUOut)
copytSymbol ((* yyNew)->InlineDefProc.Sym, yyt->InlineDefProc.Sym)
copytTree ((* yyNew)->InlineDefProc.Formals, yyt->InlineDefProc.Formals)
copytUsage ((* yyNew)->InlineDefProc.U, yyt->InlineDefProc.U)
copytChain ((* yyNew)->InlineDefProc.CGNext, yyt->InlineDefProc.CGNext)
copybool ((* yyNew)->InlineDefProc.ATOMIC, yyt->InlineDefProc.ATOMIC)
copybool ((* yyNew)->InlineDefProc.isPar, yyt->InlineDefProc.isPar)
yyt = yyt->InlineDefProc.Process;
yyNew = & (* yyNew)->InlineDefProc.Process; break;
case kPrototypeProc: (* yyNew)->PrototypeProc = yyt->PrototypeProc;
copytPosition ((* yyNew)->PrototypeProc.Pos, yyt->PrototypeProc.Pos)
copyint ((* yyNew)->PrototypeProc.V1In, yyt->PrototypeProc.V1In)
copyint ((* yyNew)->PrototypeProc.V1Out, yyt->PrototypeProc.V1Out)
copyint ((* yyNew)->PrototypeProc.VS, yyt->PrototypeProc.VS)
copyint ((* yyNew)->PrototypeProc.VIn, yyt->PrototypeProc.VIn)
copyint ((* yyNew)->PrototypeProc.VOut, yyt->PrototypeProc.VOut)
copytSymbol ((* yyNew)->PrototypeProc.Parent, yyt->PrototypeProc.Parent)
copybool ((* yyNew)->PrototypeProc.AtomicSpec, yyt->PrototypeProc.AtomicSpec)
copytParUsage ((* yyNew)->PrototypeProc.PUIn, yyt->PrototypeProc.PUIn)
copytParUsage ((* yyNew)->PrototypeProc.PUOut, yyt->PrototypeProc.PUOut)
copytSymbol ((* yyNew)->PrototypeProc.Sym, yyt->PrototypeProc.Sym)
copytUsage ((* yyNew)->PrototypeProc.U, yyt->PrototypeProc.U)
copytChain ((* yyNew)->PrototypeProc.CGNext, yyt->PrototypeProc.CGNext)
copytIdent ((* yyNew)->PrototypeProc.Module, yyt->PrototypeProc.Module)
copytLang ((* yyNew)->PrototypeProc.Lang, yyt->PrototypeProc.Lang)
yyt = yyt->PrototypeProc.Formals;
yyNew = & (* yyNew)->PrototypeProc.Formals; break;
case kDefAFunc: (* yyNew)->DefAFunc = yyt->DefAFunc;
copytPosition ((* yyNew)->DefAFunc.Pos, yyt->DefAFunc.Pos)
copyint ((* yyNew)->DefAFunc.V1In, yyt->DefAFunc.V1In)
copyint ((* yyNew)->DefAFunc.V1Out, yyt->DefAFunc.V1Out)
copyint ((* yyNew)->DefAFunc.VS, yyt->DefAFunc.VS)
copyint ((* yyNew)->DefAFunc.VIn, yyt->DefAFunc.VIn)
copyint ((* yyNew)->DefAFunc.VOut, yyt->DefAFunc.VOut)
copytSymbol ((* yyNew)->DefAFunc.Parent, yyt->DefAFunc.Parent)
copybool ((* yyNew)->DefAFunc.AtomicSpec, yyt->DefAFunc.AtomicSpec)
copytParUsage ((* yyNew)->DefAFunc.PUIn, yyt->DefAFunc.PUIn)
copytParUsage ((* yyNew)->DefAFunc.PUOut, yyt->DefAFunc.PUOut)
copytSymbol ((* yyNew)->DefAFunc.Sym, yyt->DefAFunc.Sym)
copytTree ((* yyNew)->DefAFunc.TypeList, yyt->DefAFunc.TypeList)
copytUsage ((* yyNew)->DefAFunc.U, yyt->DefAFunc.U)
copytChain ((* yyNew)->DefAFunc.CGNext, yyt->DefAFunc.CGNext)
yyt = yyt->DefAFunc.Formals;
yyNew = & (* yyNew)->DefAFunc.Formals; break;
case kDefFunc: (* yyNew)->DefFunc = yyt->DefFunc;
copytPosition ((* yyNew)->DefFunc.Pos, yyt->DefFunc.Pos)
copyint ((* yyNew)->DefFunc.V1In, yyt->DefFunc.V1In)
copyint ((* yyNew)->DefFunc.V1Out, yyt->DefFunc.V1Out)
copyint ((* yyNew)->DefFunc.VS, yyt->DefFunc.VS)
copyint ((* yyNew)->DefFunc.VIn, yyt->DefFunc.VIn)
copyint ((* yyNew)->DefFunc.VOut, yyt->DefFunc.VOut)
copytSymbol ((* yyNew)->DefFunc.Parent, yyt->DefFunc.Parent)
copybool ((* yyNew)->DefFunc.AtomicSpec, yyt->DefFunc.AtomicSpec)
copytParUsage ((* yyNew)->DefFunc.PUIn, yyt->DefFunc.PUIn)
copytParUsage ((* yyNew)->DefFunc.PUOut, yyt->DefFunc.PUOut)
copytSymbol ((* yyNew)->DefFunc.Sym, yyt->DefFunc.Sym)
copytTree ((* yyNew)->DefFunc.TypeList, yyt->DefFunc.TypeList)
copytTree ((* yyNew)->DefFunc.Formals, yyt->DefFunc.Formals)
copytUsage ((* yyNew)->DefFunc.U, yyt->DefFunc.U)
copytChain ((* yyNew)->DefFunc.CGNext, yyt->DefFunc.CGNext)
copybool ((* yyNew)->DefFunc.ATOMIC, yyt->DefFunc.ATOMIC)
yyt = yyt->DefFunc.Valof;
yyNew = & (* yyNew)->DefFunc.Valof; break;
case kInlineDefFunc: (* yyNew)->InlineDefFunc = yyt->InlineDefFunc;
copytPosition ((* yyNew)->InlineDefFunc.Pos, yyt->InlineDefFunc.Pos)
copyint ((* yyNew)->InlineDefFunc.V1In, yyt->InlineDefFunc.V1In)
copyint ((* yyNew)->InlineDefFunc.V1Out, yyt->InlineDefFunc.V1Out)
copyint ((* yyNew)->InlineDefFunc.VS, yyt->InlineDefFunc.VS)
copyint ((* yyNew)->InlineDefFunc.VIn, yyt->InlineDefFunc.VIn)
copyint ((* yyNew)->InlineDefFunc.VOut, yyt->InlineDefFunc.VOut)
copytSymbol ((* yyNew)->InlineDefFunc.Parent, yyt->InlineDefFunc.Parent)
copybool ((* yyNew)->InlineDefFunc.AtomicSpec, yyt->InlineDefFunc.AtomicSpec)
copytParUsage ((* yyNew)->InlineDefFunc.PUIn, yyt->InlineDefFunc.PUIn)
copytParUsage ((* yyNew)->InlineDefFunc.PUOut, yyt->InlineDefFunc.PUOut)
copytSymbol ((* yyNew)->InlineDefFunc.Sym, yyt->InlineDefFunc.Sym)
copytTree ((* yyNew)->InlineDefFunc.TypeList, yyt->InlineDefFunc.TypeList)
copytTree ((* yyNew)->InlineDefFunc.Formals, yyt->InlineDefFunc.Formals)
copytUsage ((* yyNew)->InlineDefFunc.U, yyt->InlineDefFunc.U)
copytChain ((* yyNew)->InlineDefFunc.CGNext, yyt->InlineDefFunc.CGNext)
copybool ((* yyNew)->InlineDefFunc.ATOMIC, yyt->InlineDefFunc.ATOMIC)
yyt = yyt->InlineDefFunc.Valof;
yyNew = & (* yyNew)->InlineDefFunc.Valof; break;
case kDefIsFunc: (* yyNew)->DefIsFunc = yyt->DefIsFunc;
copytPosition ((* yyNew)->DefIsFunc.Pos, yyt->DefIsFunc.Pos)
copyint ((* yyNew)->DefIsFunc.V1In, yyt->DefIsFunc.V1In)
copyint ((* yyNew)->DefIsFunc.V1Out, yyt->DefIsFunc.V1Out)
copyint ((* yyNew)->DefIsFunc.VS, yyt->DefIsFunc.VS)
copyint ((* yyNew)->DefIsFunc.VIn, yyt->DefIsFunc.VIn)
copyint ((* yyNew)->DefIsFunc.VOut, yyt->DefIsFunc.VOut)
copytSymbol ((* yyNew)->DefIsFunc.Parent, yyt->DefIsFunc.Parent)
copybool ((* yyNew)->DefIsFunc.AtomicSpec, yyt->DefIsFunc.AtomicSpec)
copytParUsage ((* yyNew)->DefIsFunc.PUIn, yyt->DefIsFunc.PUIn)
copytParUsage ((* yyNew)->DefIsFunc.PUOut, yyt->DefIsFunc.PUOut)
copytSymbol ((* yyNew)->DefIsFunc.Sym, yyt->DefIsFunc.Sym)
copytTree ((* yyNew)->DefIsFunc.TypeList, yyt->DefIsFunc.TypeList)
copytTree ((* yyNew)->DefIsFunc.Formals, yyt->DefIsFunc.Formals)
copytUsage ((* yyNew)->DefIsFunc.U, yyt->DefIsFunc.U)
copytChain ((* yyNew)->DefIsFunc.CGNext, yyt->DefIsFunc.CGNext)
copybool ((* yyNew)->DefIsFunc.ATOMIC, yyt->DefIsFunc.ATOMIC)
yyt = yyt->DefIsFunc.ExprList;
yyNew = & (* yyNew)->DefIsFunc.ExprList; break;
case kInlineDefIsFunc: (* yyNew)->InlineDefIsFunc = yyt->InlineDefIsFunc;
copytPosition ((* yyNew)->InlineDefIsFunc.Pos, yyt->InlineDefIsFunc.Pos)
copyint ((* yyNew)->InlineDefIsFunc.V1In, yyt->InlineDefIsFunc.V1In)
copyint ((* yyNew)->InlineDefIsFunc.V1Out, yyt->InlineDefIsFunc.V1Out)
copyint ((* yyNew)->InlineDefIsFunc.VS, yyt->InlineDefIsFunc.VS)
copyint ((* yyNew)->InlineDefIsFunc.VIn, yyt->InlineDefIsFunc.VIn)
copyint ((* yyNew)->InlineDefIsFunc.VOut, yyt->InlineDefIsFunc.VOut)
copytSymbol ((* yyNew)->InlineDefIsFunc.Parent, yyt->InlineDefIsFunc.Parent)
copybool ((* yyNew)->InlineDefIsFunc.AtomicSpec, yyt->InlineDefIsFunc.AtomicSpec)
copytParUsage ((* yyNew)->InlineDefIsFunc.PUIn, yyt->InlineDefIsFunc.PUIn)
copytParUsage ((* yyNew)->InlineDefIsFunc.PUOut, yyt->InlineDefIsFunc.PUOut)
copytSymbol ((* yyNew)->InlineDefIsFunc.Sym, yyt->InlineDefIsFunc.Sym)
copytTree ((* yyNew)->InlineDefIsFunc.TypeList, yyt->InlineDefIsFunc.TypeList)
copytTree ((* yyNew)->InlineDefIsFunc.Formals, yyt->InlineDefIsFunc.Formals)
copytUsage ((* yyNew)->InlineDefIsFunc.U, yyt->InlineDefIsFunc.U)
copytChain ((* yyNew)->InlineDefIsFunc.CGNext, yyt->InlineDefIsFunc.CGNext)
copybool ((* yyNew)->InlineDefIsFunc.ATOMIC, yyt->InlineDefIsFunc.ATOMIC)
yyt = yyt->InlineDefIsFunc.ExprList;
yyNew = & (* yyNew)->InlineDefIsFunc.ExprList; break;
case kPrototypeFunc: (* yyNew)->PrototypeFunc = yyt->PrototypeFunc;
copytPosition ((* yyNew)->PrototypeFunc.Pos, yyt->PrototypeFunc.Pos)
copyint ((* yyNew)->PrototypeFunc.V1In, yyt->PrototypeFunc.V1In)
copyint ((* yyNew)->PrototypeFunc.V1Out, yyt->PrototypeFunc.V1Out)
copyint ((* yyNew)->PrototypeFunc.VS, yyt->PrototypeFunc.VS)
copyint ((* yyNew)->PrototypeFunc.VIn, yyt->PrototypeFunc.VIn)
copyint ((* yyNew)->PrototypeFunc.VOut, yyt->PrototypeFunc.VOut)
copytSymbol ((* yyNew)->PrototypeFunc.Parent, yyt->PrototypeFunc.Parent)
copybool ((* yyNew)->PrototypeFunc.AtomicSpec, yyt->PrototypeFunc.AtomicSpec)
copytParUsage ((* yyNew)->PrototypeFunc.PUIn, yyt->PrototypeFunc.PUIn)
copytParUsage ((* yyNew)->PrototypeFunc.PUOut, yyt->PrototypeFunc.PUOut)
copytSymbol ((* yyNew)->PrototypeFunc.Sym, yyt->PrototypeFunc.Sym)
copytTree ((* yyNew)->PrototypeFunc.TypeList, yyt->PrototypeFunc.TypeList)
copytUsage ((* yyNew)->PrototypeFunc.U, yyt->PrototypeFunc.U)
copytChain ((* yyNew)->PrototypeFunc.CGNext, yyt->PrototypeFunc.CGNext)
copytIdent ((* yyNew)->PrototypeFunc.Module, yyt->PrototypeFunc.Module)
copytLang ((* yyNew)->PrototypeFunc.Lang, yyt->PrototypeFunc.Lang)
yyt = yyt->PrototypeFunc.Formals;
yyNew = & (* yyNew)->PrototypeFunc.Formals; break;
case kNullSpec: (* yyNew)->NullSpec = yyt->NullSpec;
copytPosition ((* yyNew)->NullSpec.Pos, yyt->NullSpec.Pos)
copyint ((* yyNew)->NullSpec.V1In, yyt->NullSpec.V1In)
copyint ((* yyNew)->NullSpec.V1Out, yyt->NullSpec.V1Out)
copyint ((* yyNew)->NullSpec.VS, yyt->NullSpec.VS)
copyint ((* yyNew)->NullSpec.VIn, yyt->NullSpec.VIn)
copyint ((* yyNew)->NullSpec.VOut, yyt->NullSpec.VOut)
copytSymbol ((* yyNew)->NullSpec.Parent, yyt->NullSpec.Parent)
copybool ((* yyNew)->NullSpec.AtomicSpec, yyt->NullSpec.AtomicSpec)
copytParUsage ((* yyNew)->NullSpec.PUIn, yyt->NullSpec.PUIn)
copytParUsage ((* yyNew)->NullSpec.PUOut, yyt->NullSpec.PUOut)
return;
case kHCode: (* yyNew)->HCode = yyt->HCode;
copytPosition ((* yyNew)->HCode.Pos, yyt->HCode.Pos)
copyint ((* yyNew)->HCode.V1In, yyt->HCode.V1In)
copyint ((* yyNew)->HCode.V1Out, yyt->HCode.V1Out)
copyint ((* yyNew)->HCode.VS, yyt->HCode.VS)
copyint ((* yyNew)->HCode.VIn, yyt->HCode.VIn)
copyint ((* yyNew)->HCode.VOut, yyt->HCode.VOut)
copytSymbol ((* yyNew)->HCode.Parent, yyt->HCode.Parent)
copybool ((* yyNew)->HCode.AtomicSpec, yyt->HCode.AtomicSpec)
copytParUsage ((* yyNew)->HCode.PUIn, yyt->HCode.PUIn)
copytParUsage ((* yyNew)->HCode.PUOut, yyt->HCode.PUOut)
copytStringRef ((* yyNew)->HCode.Code, yyt->HCode.Code)
copytSymbol ((* yyNew)->HCode.Scope, yyt->HCode.Scope)
copytChain ((* yyNew)->HCode.CGNext, yyt->HCode.CGNext)
return;
case kCallFrame: (* yyNew)->CallFrame = yyt->CallFrame;
copytPosition ((* yyNew)->CallFrame.Pos, yyt->CallFrame.Pos)
copyint ((* yyNew)->CallFrame.V1In, yyt->CallFrame.V1In)
copyint ((* yyNew)->CallFrame.V1Out, yyt->CallFrame.V1Out)
copyint ((* yyNew)->CallFrame.VS, yyt->CallFrame.VS)
copyint ((* yyNew)->CallFrame.VIn, yyt->CallFrame.VIn)
copyint ((* yyNew)->CallFrame.VOut, yyt->CallFrame.VOut)
copytSymbol ((* yyNew)->CallFrame.Parent, yyt->CallFrame.Parent)
copybool ((* yyNew)->CallFrame.AtomicSpec, yyt->CallFrame.AtomicSpec)
copytParUsage ((* yyNew)->CallFrame.PUIn, yyt->CallFrame.PUIn)
copytParUsage ((* yyNew)->CallFrame.PUOut, yyt->CallFrame.PUOut)
copytIdent ((* yyNew)->CallFrame.Id, yyt->CallFrame.Id)
copytSymbol ((* yyNew)->CallFrame.Sym, yyt->CallFrame.Sym)
return;
case kParFrame: (* yyNew)->ParFrame = yyt->ParFrame;
copytPosition ((* yyNew)->ParFrame.Pos, yyt->ParFrame.Pos)
copyint ((* yyNew)->ParFrame.V1In, yyt->ParFrame.V1In)
copyint ((* yyNew)->ParFrame.V1Out, yyt->ParFrame.V1Out)
copyint ((* yyNew)->ParFrame.VS, yyt->ParFrame.VS)
copyint ((* yyNew)->ParFrame.VIn, yyt->ParFrame.VIn)
copyint ((* yyNew)->ParFrame.VOut, yyt->ParFrame.VOut)
copytSymbol ((* yyNew)->ParFrame.Parent, yyt->ParFrame.Parent)
copybool ((* yyNew)->ParFrame.AtomicSpec, yyt->ParFrame.AtomicSpec)
copytParUsage ((* yyNew)->ParFrame.PUIn, yyt->ParFrame.PUIn)
copytParUsage ((* yyNew)->ParFrame.PUOut, yyt->ParFrame.PUOut)
copytIdent ((* yyNew)->ParFrame.TId, yyt->ParFrame.TId)
copytIdent ((* yyNew)->ParFrame.FId, yyt->ParFrame.FId)
copytSymbol ((* yyNew)->ParFrame.Sym, yyt->ParFrame.Sym)
copyint ((* yyNew)->ParFrame.Rep, yyt->ParFrame.Rep)
return;
case kFormals: (* yyNew)->Formals = yyt->Formals;
copyint ((* yyNew)->Formals.V1In, yyt->Formals.V1In)
copyint ((* yyNew)->Formals.V1Out, yyt->Formals.V1Out)
copyint ((* yyNew)->Formals.VIn, yyt->Formals.VIn)
copyint ((* yyNew)->Formals.VOut, yyt->Formals.VOut)
copybool ((* yyNew)->Formals.InPrototype, yyt->Formals.InPrototype)
copytSymbol ((* yyNew)->Formals.Parent, yyt->Formals.Parent)
copybool ((* yyNew)->Formals.AtomicSpec, yyt->Formals.AtomicSpec)
copytParUsage ((* yyNew)->Formals.PUIn, yyt->Formals.PUIn)
copytParUsage ((* yyNew)->Formals.PUOut, yyt->Formals.PUOut)
return;
case kNoFormal: (* yyNew)->NoFormal = yyt->NoFormal;
copyint ((* yyNew)->NoFormal.V1In, yyt->NoFormal.V1In)
copyint ((* yyNew)->NoFormal.V1Out, yyt->NoFormal.V1Out)
copyint ((* yyNew)->NoFormal.VIn, yyt->NoFormal.VIn)
copyint ((* yyNew)->NoFormal.VOut, yyt->NoFormal.VOut)
copybool ((* yyNew)->NoFormal.InPrototype, yyt->NoFormal.InPrototype)
copytSymbol ((* yyNew)->NoFormal.Parent, yyt->NoFormal.Parent)
copybool ((* yyNew)->NoFormal.AtomicSpec, yyt->NoFormal.AtomicSpec)
copytParUsage ((* yyNew)->NoFormal.PUIn, yyt->NoFormal.PUIn)
copytParUsage ((* yyNew)->NoFormal.PUOut, yyt->NoFormal.PUOut)
return;
case kNoFormals: (* yyNew)->NoFormals = yyt->NoFormals;
copyint ((* yyNew)->NoFormals.V1In, yyt->NoFormals.V1In)
copyint ((* yyNew)->NoFormals.V1Out, yyt->NoFormals.V1Out)
copyint ((* yyNew)->NoFormals.VIn, yyt->NoFormals.VIn)
copyint ((* yyNew)->NoFormals.VOut, yyt->NoFormals.VOut)
copybool ((* yyNew)->NoFormals.InPrototype, yyt->NoFormals.InPrototype)
copytSymbol ((* yyNew)->NoFormals.Parent, yyt->NoFormals.Parent)
copybool ((* yyNew)->NoFormals.AtomicSpec, yyt->NoFormals.AtomicSpec)
copytParUsage ((* yyNew)->NoFormals.PUIn, yyt->NoFormals.PUIn)
copytParUsage ((* yyNew)->NoFormals.PUOut, yyt->NoFormals.PUOut)
copytSymbol ((* yyNew)->NoFormals.Sym, yyt->NoFormals.Sym)
return;
case kAFormal: (* yyNew)->AFormal = yyt->AFormal;
copyint ((* yyNew)->AFormal.V1In, yyt->AFormal.V1In)
copyint ((* yyNew)->AFormal.V1Out, yyt->AFormal.V1Out)
copyint ((* yyNew)->AFormal.VIn, yyt->AFormal.VIn)
copyint ((* yyNew)->AFormal.VOut, yyt->AFormal.VOut)
copybool ((* yyNew)->AFormal.InPrototype, yyt->AFormal.InPrototype)
copytSymbol ((* yyNew)->AFormal.Parent, yyt->AFormal.Parent)
copybool ((* yyNew)->AFormal.AtomicSpec, yyt->AFormal.AtomicSpec)
copytParUsage ((* yyNew)->AFormal.PUIn, yyt->AFormal.PUIn)
copytParUsage ((* yyNew)->AFormal.PUOut, yyt->AFormal.PUOut)
copytTree ((* yyNew)->AFormal.Next, yyt->AFormal.Next)
copytPosition ((* yyNew)->AFormal.Pos, yyt->AFormal.Pos)
copytTree ((* yyNew)->AFormal.By, yyt->AFormal.By)
copytSymbol ((* yyNew)->AFormal.Sym, yyt->AFormal.Sym)
copytUsage ((* yyNew)->AFormal.U, yyt->AFormal.U)
yyt = yyt->AFormal.Type;
yyNew = & (* yyNew)->AFormal.Type; break;
case kReplicator: (* yyNew)->Replicator = yyt->Replicator;
copytSymbol ((* yyNew)->Replicator.Sym, yyt->Replicator.Sym)
copytTree ((* yyNew)->Replicator.From, yyt->Replicator.From)
copyint ((* yyNew)->Replicator.V1In, yyt->Replicator.V1In)
copyint ((* yyNew)->Replicator.V1Out, yyt->Replicator.V1Out)
copyint ((* yyNew)->Replicator.VIn, yyt->Replicator.VIn)
copyint ((* yyNew)->Replicator.VOut, yyt->Replicator.VOut)
copytSymbol ((* yyNew)->Replicator.Parent, yyt->Replicator.Parent)
copybool ((* yyNew)->Replicator.AtomicSpec, yyt->Replicator.AtomicSpec)
copytParUsage ((* yyNew)->Replicator.PUIn, yyt->Replicator.PUIn)
copytParUsage ((* yyNew)->Replicator.PUOut, yyt->Replicator.PUOut)
copybool ((* yyNew)->Replicator.SE, yyt->Replicator.SE)
copypLinExpr ((* yyNew)->Replicator.FromLE, yyt->Replicator.FromLE)
copypLinExpr ((* yyNew)->Replicator.ForLE, yyt->Replicator.ForLE)
yyt = yyt->Replicator.For;
yyNew = & (* yyNew)->Replicator.For; break;
case kTagProList: (* yyNew)->TagProList = yyt->TagProList;
copytParUsage ((* yyNew)->TagProList.PUIn, yyt->TagProList.PUIn)
copytParUsage ((* yyNew)->TagProList.PUOut, yyt->TagProList.PUOut)
return;
case kNoTagPro: (* yyNew)->NoTagPro = yyt->NoTagPro;
copytParUsage ((* yyNew)->NoTagPro.PUIn, yyt->NoTagPro.PUIn)
copytParUsage ((* yyNew)->NoTagPro.PUOut, yyt->NoTagPro.PUOut)
return;
case kATagPro: (* yyNew)->ATagPro = yyt->ATagPro;
copytParUsage ((* yyNew)->ATagPro.PUIn, yyt->ATagPro.PUIn)
copytParUsage ((* yyNew)->ATagPro.PUOut, yyt->ATagPro.PUOut)
copytSymbol ((* yyNew)->ATagPro.Sym, yyt->ATagPro.Sym)
copytTree ((* yyNew)->ATagPro.SeqProList, yyt->ATagPro.SeqProList)
yyt = yyt->ATagPro.Next;
yyNew = & (* yyNew)->ATagPro.Next; break;
case kBy: (* yyNew)->By = yyt->By;
return;
case kByVal: (* yyNew)->ByVal = yyt->ByVal;
return;
case kByRef: (* yyNew)->ByRef = yyt->ByRef;
return;
case kByUndef: (* yyNew)->ByUndef = yyt->ByUndef;
return;
case kIdList: (* yyNew)->IdList = yyt->IdList;
copytSymbol ((* yyNew)->IdList.Parent, yyt->IdList.Parent)
copybool ((* yyNew)->IdList.AtomicSpec, yyt->IdList.AtomicSpec)
return;
case kNoId: (* yyNew)->NoId = yyt->NoId;
copytSymbol ((* yyNew)->NoId.Parent, yyt->NoId.Parent)
copybool ((* yyNew)->NoId.AtomicSpec, yyt->NoId.AtomicSpec)
return;
case kAnId: (* yyNew)->AnId = yyt->AnId;
copytSymbol ((* yyNew)->AnId.Parent, yyt->AnId.Parent)
copybool ((* yyNew)->AnId.AtomicSpec, yyt->AnId.AtomicSpec)
copytSymbol ((* yyNew)->AnId.Sym, yyt->AnId.Sym)
copytUsage ((* yyNew)->AnId.U, yyt->AnId.U)
yyt = yyt->AnId.Next;
yyNew = & (* yyNew)->AnId.Next; break;
case kType: (* yyNew)->Type = yyt->Type;
copyint ((* yyNew)->Type.V1In, yyt->Type.V1In)
copyint ((* yyNew)->Type.V1Out, yyt->Type.V1Out)
copyint ((* yyNew)->Type.VIn, yyt->Type.VIn)
copyint ((* yyNew)->Type.VOut, yyt->Type.VOut)
copyint ((* yyNew)->Type.D, yyt->Type.D)
copytParUsage ((* yyNew)->Type.PUIn, yyt->Type.PUIn)
copytParUsage ((* yyNew)->Type.PUOut, yyt->Type.PUOut)
return;
case kArray: (* yyNew)->Array = yyt->Array;
copyint ((* yyNew)->Array.V1In, yyt->Array.V1In)
copyint ((* yyNew)->Array.V1Out, yyt->Array.V1Out)
copyint ((* yyNew)->Array.VIn, yyt->Array.VIn)
copyint ((* yyNew)->Array.VOut, yyt->Array.VOut)
copyint ((* yyNew)->Array.D, yyt->Array.D)
copytParUsage ((* yyNew)->Array.PUIn, yyt->Array.PUIn)
copytParUsage ((* yyNew)->Array.PUOut, yyt->Array.PUOut)
copytTree ((* yyNew)->Array.Expr, yyt->Array.Expr)
copybool ((* yyNew)->Array.SE, yyt->Array.SE)
copypLinExpr ((* yyNew)->Array.LE, yyt->Array.LE)
yyt = yyt->Array.Type;
yyNew = & (* yyNew)->Array.Type; break;
case kUserArray: (* yyNew)->UserArray = yyt->UserArray;
copyint ((* yyNew)->UserArray.V1In, yyt->UserArray.V1In)
copyint ((* yyNew)->UserArray.V1Out, yyt->UserArray.V1Out)
copyint ((* yyNew)->UserArray.VIn, yyt->UserArray.VIn)
copyint ((* yyNew)->UserArray.VOut, yyt->UserArray.VOut)
copyint ((* yyNew)->UserArray.D, yyt->UserArray.D)
copytParUsage ((* yyNew)->UserArray.PUIn, yyt->UserArray.PUIn)
copytParUsage ((* yyNew)->UserArray.PUOut, yyt->UserArray.PUOut)
copytTree ((* yyNew)->UserArray.Expr, yyt->UserArray.Expr)
copybool ((* yyNew)->UserArray.SE, yyt->UserArray.SE)
copypLinExpr ((* yyNew)->UserArray.LE, yyt->UserArray.LE)
copytPosition ((* yyNew)->UserArray.Pos, yyt->UserArray.Pos)
copytSymbol ((* yyNew)->UserArray.Sym, yyt->UserArray.Sym)
yyt = yyt->UserArray.Type;
yyNew = & (* yyNew)->UserArray.Type; break;
case kChan: (* yyNew)->Chan = yyt->Chan;
copyint ((* yyNew)->Chan.V1In, yyt->Chan.V1In)
copyint ((* yyNew)->Chan.V1Out, yyt->Chan.V1Out)
copyint ((* yyNew)->Chan.VIn, yyt->Chan.VIn)
copyint ((* yyNew)->Chan.VOut, yyt->Chan.VOut)
copyint ((* yyNew)->Chan.D, yyt->Chan.D)
copytParUsage ((* yyNew)->Chan.PUIn, yyt->Chan.PUIn)
copytParUsage ((* yyNew)->Chan.PUOut, yyt->Chan.PUOut)
yyt = yyt->Chan.Protocol;
yyNew = & (* yyNew)->Chan.Protocol; break;
case kTimer: (* yyNew)->Timer = yyt->Timer;
copyint ((* yyNew)->Timer.V1In, yyt->Timer.V1In)
copyint ((* yyNew)->Timer.V1Out, yyt->Timer.V1Out)
copyint ((* yyNew)->Timer.VIn, yyt->Timer.VIn)
copyint ((* yyNew)->Timer.VOut, yyt->Timer.VOut)
copyint ((* yyNew)->Timer.D, yyt->Timer.D)
copytParUsage ((* yyNew)->Timer.PUIn, yyt->Timer.PUIn)
copytParUsage ((* yyNew)->Timer.PUOut, yyt->Timer.PUOut)
return;
case kUntyped: (* yyNew)->Untyped = yyt->Untyped;
copyint ((* yyNew)->Untyped.V1In, yyt->Untyped.V1In)
copyint ((* yyNew)->Untyped.V1Out, yyt->Untyped.V1Out)
copyint ((* yyNew)->Untyped.VIn, yyt->Untyped.VIn)
copyint ((* yyNew)->Untyped.VOut, yyt->Untyped.VOut)
copyint ((* yyNew)->Untyped.D, yyt->Untyped.D)
copytParUsage ((* yyNew)->Untyped.PUIn, yyt->Untyped.PUIn)
copytParUsage ((* yyNew)->Untyped.PUOut, yyt->Untyped.PUOut)
return;
case kUntypedByte: (* yyNew)->UntypedByte = yyt->UntypedByte;
copyint ((* yyNew)->UntypedByte.V1In, yyt->UntypedByte.V1In)
copyint ((* yyNew)->UntypedByte.V1Out, yyt->UntypedByte.V1Out)
copyint ((* yyNew)->UntypedByte.VIn, yyt->UntypedByte.VIn)
copyint ((* yyNew)->UntypedByte.VOut, yyt->UntypedByte.VOut)
copyint ((* yyNew)->UntypedByte.D, yyt->UntypedByte.D)
copytParUsage ((* yyNew)->UntypedByte.PUIn, yyt->UntypedByte.PUIn)
copytParUsage ((* yyNew)->UntypedByte.PUOut, yyt->UntypedByte.PUOut)
return;
case kUntypedReal: (* yyNew)->UntypedReal = yyt->UntypedReal;
copyint ((* yyNew)->UntypedReal.V1In, yyt->UntypedReal.V1In)
copyint ((* yyNew)->UntypedReal.V1Out, yyt->UntypedReal.V1Out)
copyint ((* yyNew)->UntypedReal.VIn, yyt->UntypedReal.VIn)
copyint ((* yyNew)->UntypedReal.VOut, yyt->UntypedReal.VOut)
copyint ((* yyNew)->UntypedReal.D, yyt->UntypedReal.D)
copytParUsage ((* yyNew)->UntypedReal.PUIn, yyt->UntypedReal.PUIn)
copytParUsage ((* yyNew)->UntypedReal.PUOut, yyt->UntypedReal.PUOut)
return;
case kUntypedInteger: (* yyNew)->UntypedInteger = yyt->UntypedInteger;
copyint ((* yyNew)->UntypedInteger.V1In, yyt->UntypedInteger.V1In)
copyint ((* yyNew)->UntypedInteger.V1Out, yyt->UntypedInteger.V1Out)
copyint ((* yyNew)->UntypedInteger.VIn, yyt->UntypedInteger.VIn)
copyint ((* yyNew)->UntypedInteger.VOut, yyt->UntypedInteger.VOut)
copyint ((* yyNew)->UntypedInteger.D, yyt->UntypedInteger.D)
copytParUsage ((* yyNew)->UntypedInteger.PUIn, yyt->UntypedInteger.PUIn)
copytParUsage ((* yyNew)->UntypedInteger.PUOut, yyt->UntypedInteger.PUOut)
return;
case kUntypedInt: (* yyNew)->UntypedInt = yyt->UntypedInt;
copyint ((* yyNew)->UntypedInt.V1In, yyt->UntypedInt.V1In)
copyint ((* yyNew)->UntypedInt.V1Out, yyt->UntypedInt.V1Out)
copyint ((* yyNew)->UntypedInt.VIn, yyt->UntypedInt.VIn)
copyint ((* yyNew)->UntypedInt.VOut, yyt->UntypedInt.VOut)
copyint ((* yyNew)->UntypedInt.D, yyt->UntypedInt.D)
copytParUsage ((* yyNew)->UntypedInt.PUIn, yyt->UntypedInt.PUIn)
copytParUsage ((* yyNew)->UntypedInt.PUOut, yyt->UntypedInt.PUOut)
return;
case kUntypedHex: (* yyNew)->UntypedHex = yyt->UntypedHex;
copyint ((* yyNew)->UntypedHex.V1In, yyt->UntypedHex.V1In)
copyint ((* yyNew)->UntypedHex.V1Out, yyt->UntypedHex.V1Out)
copyint ((* yyNew)->UntypedHex.VIn, yyt->UntypedHex.VIn)
copyint ((* yyNew)->UntypedHex.VOut, yyt->UntypedHex.VOut)
copyint ((* yyNew)->UntypedHex.D, yyt->UntypedHex.D)
copytParUsage ((* yyNew)->UntypedHex.PUIn, yyt->UntypedHex.PUIn)
copytParUsage ((* yyNew)->UntypedHex.PUOut, yyt->UntypedHex.PUOut)
return;
case kPrimitive: (* yyNew)->Primitive = yyt->Primitive;
copyint ((* yyNew)->Primitive.V1In, yyt->Primitive.V1In)
copyint ((* yyNew)->Primitive.V1Out, yyt->Primitive.V1Out)
copyint ((* yyNew)->Primitive.VIn, yyt->Primitive.VIn)
copyint ((* yyNew)->Primitive.VOut, yyt->Primitive.VOut)
copyint ((* yyNew)->Primitive.D, yyt->Primitive.D)
copytParUsage ((* yyNew)->Primitive.PUIn, yyt->Primitive.PUIn)
copytParUsage ((* yyNew)->Primitive.PUOut, yyt->Primitive.PUOut)
return;
case kNumeric: (* yyNew)->Numeric = yyt->Numeric;
copyint ((* yyNew)->Numeric.V1In, yyt->Numeric.V1In)
copyint ((* yyNew)->Numeric.V1Out, yyt->Numeric.V1Out)
copyint ((* yyNew)->Numeric.VIn, yyt->Numeric.VIn)
copyint ((* yyNew)->Numeric.VOut, yyt->Numeric.VOut)
copyint ((* yyNew)->Numeric.D, yyt->Numeric.D)
copytParUsage ((* yyNew)->Numeric.PUIn, yyt->Numeric.PUIn)
copytParUsage ((* yyNew)->Numeric.PUOut, yyt->Numeric.PUOut)
return;
case kInteger: (* yyNew)->Integer = yyt->Integer;
copyint ((* yyNew)->Integer.V1In, yyt->Integer.V1In)
copyint ((* yyNew)->Integer.V1Out, yyt->Integer.V1Out)
copyint ((* yyNew)->Integer.VIn, yyt->Integer.VIn)
copyint ((* yyNew)->Integer.VOut, yyt->Integer.VOut)
copyint ((* yyNew)->Integer.D, yyt->Integer.D)
copytParUsage ((* yyNew)->Integer.PUIn, yyt->Integer.PUIn)
copytParUsage ((* yyNew)->Integer.PUOut, yyt->Integer.PUOut)
return;
case kInt: (* yyNew)->Int = yyt->Int;
copyint ((* yyNew)->Int.V1In, yyt->Int.V1In)
copyint ((* yyNew)->Int.V1Out, yyt->Int.V1Out)
copyint ((* yyNew)->Int.VIn, yyt->Int.VIn)
copyint ((* yyNew)->Int.VOut, yyt->Int.VOut)
copyint ((* yyNew)->Int.D, yyt->Int.D)
copytParUsage ((* yyNew)->Int.PUIn, yyt->Int.PUIn)
copytParUsage ((* yyNew)->Int.PUOut, yyt->Int.PUOut)
return;
case kUserInt: (* yyNew)->UserInt = yyt->UserInt;
copyint ((* yyNew)->UserInt.V1In, yyt->UserInt.V1In)
copyint ((* yyNew)->UserInt.V1Out, yyt->UserInt.V1Out)
copyint ((* yyNew)->UserInt.VIn, yyt->UserInt.VIn)
copyint ((* yyNew)->UserInt.VOut, yyt->UserInt.VOut)
copyint ((* yyNew)->UserInt.D, yyt->UserInt.D)
copytParUsage ((* yyNew)->UserInt.PUIn, yyt->UserInt.PUIn)
copytParUsage ((* yyNew)->UserInt.PUOut, yyt->UserInt.PUOut)
copytPosition ((* yyNew)->UserInt.Pos, yyt->UserInt.Pos)
copytSymbol ((* yyNew)->UserInt.Sym, yyt->UserInt.Sym)
return;
case kInt16: (* yyNew)->Int16 = yyt->Int16;
copyint ((* yyNew)->Int16.V1In, yyt->Int16.V1In)
copyint ((* yyNew)->Int16.V1Out, yyt->Int16.V1Out)
copyint ((* yyNew)->Int16.VIn, yyt->Int16.VIn)
copyint ((* yyNew)->Int16.VOut, yyt->Int16.VOut)
copyint ((* yyNew)->Int16.D, yyt->Int16.D)
copytParUsage ((* yyNew)->Int16.PUIn, yyt->Int16.PUIn)
copytParUsage ((* yyNew)->Int16.PUOut, yyt->Int16.PUOut)
return;
case kUserInt16: (* yyNew)->UserInt16 = yyt->UserInt16;
copyint ((* yyNew)->UserInt16.V1In, yyt->UserInt16.V1In)
copyint ((* yyNew)->UserInt16.V1Out, yyt->UserInt16.V1Out)
copyint ((* yyNew)->UserInt16.VIn, yyt->UserInt16.VIn)
copyint ((* yyNew)->UserInt16.VOut, yyt->UserInt16.VOut)
copyint ((* yyNew)->UserInt16.D, yyt->UserInt16.D)
copytParUsage ((* yyNew)->UserInt16.PUIn, yyt->UserInt16.PUIn)
copytParUsage ((* yyNew)->UserInt16.PUOut, yyt->UserInt16.PUOut)
copytPosition ((* yyNew)->UserInt16.Pos, yyt->UserInt16.Pos)
copytSymbol ((* yyNew)->UserInt16.Sym, yyt->UserInt16.Sym)
return;
case kInt32: (* yyNew)->Int32 = yyt->Int32;
copyint ((* yyNew)->Int32.V1In, yyt->Int32.V1In)
copyint ((* yyNew)->Int32.V1Out, yyt->Int32.V1Out)
copyint ((* yyNew)->Int32.VIn, yyt->Int32.VIn)
copyint ((* yyNew)->Int32.VOut, yyt->Int32.VOut)
copyint ((* yyNew)->Int32.D, yyt->Int32.D)
copytParUsage ((* yyNew)->Int32.PUIn, yyt->Int32.PUIn)
copytParUsage ((* yyNew)->Int32.PUOut, yyt->Int32.PUOut)
return;
case kUserInt32: (* yyNew)->UserInt32 = yyt->UserInt32;
copyint ((* yyNew)->UserInt32.V1In, yyt->UserInt32.V1In)
copyint ((* yyNew)->UserInt32.V1Out, yyt->UserInt32.V1Out)
copyint ((* yyNew)->UserInt32.VIn, yyt->UserInt32.VIn)
copyint ((* yyNew)->UserInt32.VOut, yyt->UserInt32.VOut)
copyint ((* yyNew)->UserInt32.D, yyt->UserInt32.D)
copytParUsage ((* yyNew)->UserInt32.PUIn, yyt->UserInt32.PUIn)
copytParUsage ((* yyNew)->UserInt32.PUOut, yyt->UserInt32.PUOut)
copytPosition ((* yyNew)->UserInt32.Pos, yyt->UserInt32.Pos)
copytSymbol ((* yyNew)->UserInt32.Sym, yyt->UserInt32.Sym)
return;
case kInt64: (* yyNew)->Int64 = yyt->Int64;
copyint ((* yyNew)->Int64.V1In, yyt->Int64.V1In)
copyint ((* yyNew)->Int64.V1Out, yyt->Int64.V1Out)
copyint ((* yyNew)->Int64.VIn, yyt->Int64.VIn)
copyint ((* yyNew)->Int64.VOut, yyt->Int64.VOut)
copyint ((* yyNew)->Int64.D, yyt->Int64.D)
copytParUsage ((* yyNew)->Int64.PUIn, yyt->Int64.PUIn)
copytParUsage ((* yyNew)->Int64.PUOut, yyt->Int64.PUOut)
return;
case kUserInt64: (* yyNew)->UserInt64 = yyt->UserInt64;
copyint ((* yyNew)->UserInt64.V1In, yyt->UserInt64.V1In)
copyint ((* yyNew)->UserInt64.V1Out, yyt->UserInt64.V1Out)
copyint ((* yyNew)->UserInt64.VIn, yyt->UserInt64.VIn)
copyint ((* yyNew)->UserInt64.VOut, yyt->UserInt64.VOut)
copyint ((* yyNew)->UserInt64.D, yyt->UserInt64.D)
copytParUsage ((* yyNew)->UserInt64.PUIn, yyt->UserInt64.PUIn)
copytParUsage ((* yyNew)->UserInt64.PUOut, yyt->UserInt64.PUOut)
copytPosition ((* yyNew)->UserInt64.Pos, yyt->UserInt64.Pos)
copytSymbol ((* yyNew)->UserInt64.Sym, yyt->UserInt64.Sym)
return;
case kReal: (* yyNew)->Real = yyt->Real;
copyint ((* yyNew)->Real.V1In, yyt->Real.V1In)
copyint ((* yyNew)->Real.V1Out, yyt->Real.V1Out)
copyint ((* yyNew)->Real.VIn, yyt->Real.VIn)
copyint ((* yyNew)->Real.VOut, yyt->Real.VOut)
copyint ((* yyNew)->Real.D, yyt->Real.D)
copytParUsage ((* yyNew)->Real.PUIn, yyt->Real.PUIn)
copytParUsage ((* yyNew)->Real.PUOut, yyt->Real.PUOut)
return;
case kReal32: (* yyNew)->Real32 = yyt->Real32;
copyint ((* yyNew)->Real32.V1In, yyt->Real32.V1In)
copyint ((* yyNew)->Real32.V1Out, yyt->Real32.V1Out)
copyint ((* yyNew)->Real32.VIn, yyt->Real32.VIn)
copyint ((* yyNew)->Real32.VOut, yyt->Real32.VOut)
copyint ((* yyNew)->Real32.D, yyt->Real32.D)
copytParUsage ((* yyNew)->Real32.PUIn, yyt->Real32.PUIn)
copytParUsage ((* yyNew)->Real32.PUOut, yyt->Real32.PUOut)
return;
case kUserReal32: (* yyNew)->UserReal32 = yyt->UserReal32;
copyint ((* yyNew)->UserReal32.V1In, yyt->UserReal32.V1In)
copyint ((* yyNew)->UserReal32.V1Out, yyt->UserReal32.V1Out)
copyint ((* yyNew)->UserReal32.VIn, yyt->UserReal32.VIn)
copyint ((* yyNew)->UserReal32.VOut, yyt->UserReal32.VOut)
copyint ((* yyNew)->UserReal32.D, yyt->UserReal32.D)
copytParUsage ((* yyNew)->UserReal32.PUIn, yyt->UserReal32.PUIn)
copytParUsage ((* yyNew)->UserReal32.PUOut, yyt->UserReal32.PUOut)
copytPosition ((* yyNew)->UserReal32.Pos, yyt->UserReal32.Pos)
copytSymbol ((* yyNew)->UserReal32.Sym, yyt->UserReal32.Sym)
return;
case kReal64: (* yyNew)->Real64 = yyt->Real64;
copyint ((* yyNew)->Real64.V1In, yyt->Real64.V1In)
copyint ((* yyNew)->Real64.V1Out, yyt->Real64.V1Out)
copyint ((* yyNew)->Real64.VIn, yyt->Real64.VIn)
copyint ((* yyNew)->Real64.VOut, yyt->Real64.VOut)
copyint ((* yyNew)->Real64.D, yyt->Real64.D)
copytParUsage ((* yyNew)->Real64.PUIn, yyt->Real64.PUIn)
copytParUsage ((* yyNew)->Real64.PUOut, yyt->Real64.PUOut)
return;
case kUserReal64: (* yyNew)->UserReal64 = yyt->UserReal64;
copyint ((* yyNew)->UserReal64.V1In, yyt->UserReal64.V1In)
copyint ((* yyNew)->UserReal64.V1Out, yyt->UserReal64.V1Out)
copyint ((* yyNew)->UserReal64.VIn, yyt->UserReal64.VIn)
copyint ((* yyNew)->UserReal64.VOut, yyt->UserReal64.VOut)
copyint ((* yyNew)->UserReal64.D, yyt->UserReal64.D)
copytParUsage ((* yyNew)->UserReal64.PUIn, yyt->UserReal64.PUIn)
copytParUsage ((* yyNew)->UserReal64.PUOut, yyt->UserReal64.PUOut)
copytPosition ((* yyNew)->UserReal64.Pos, yyt->UserReal64.Pos)
copytSymbol ((* yyNew)->UserReal64.Sym, yyt->UserReal64.Sym)
return;
case kByte: (* yyNew)->Byte = yyt->Byte;
copyint ((* yyNew)->Byte.V1In, yyt->Byte.V1In)
copyint ((* yyNew)->Byte.V1Out, yyt->Byte.V1Out)
copyint ((* yyNew)->Byte.VIn, yyt->Byte.VIn)
copyint ((* yyNew)->Byte.VOut, yyt->Byte.VOut)
copyint ((* yyNew)->Byte.D, yyt->Byte.D)
copytParUsage ((* yyNew)->Byte.PUIn, yyt->Byte.PUIn)
copytParUsage ((* yyNew)->Byte.PUOut, yyt->Byte.PUOut)
return;
case kUserByte: (* yyNew)->UserByte = yyt->UserByte;
copyint ((* yyNew)->UserByte.V1In, yyt->UserByte.V1In)
copyint ((* yyNew)->UserByte.V1Out, yyt->UserByte.V1Out)
copyint ((* yyNew)->UserByte.VIn, yyt->UserByte.VIn)
copyint ((* yyNew)->UserByte.VOut, yyt->UserByte.VOut)
copyint ((* yyNew)->UserByte.D, yyt->UserByte.D)
copytParUsage ((* yyNew)->UserByte.PUIn, yyt->UserByte.PUIn)
copytParUsage ((* yyNew)->UserByte.PUOut, yyt->UserByte.PUOut)
copytPosition ((* yyNew)->UserByte.Pos, yyt->UserByte.Pos)
copytSymbol ((* yyNew)->UserByte.Sym, yyt->UserByte.Sym)
return;
case kBool: (* yyNew)->Bool = yyt->Bool;
copyint ((* yyNew)->Bool.V1In, yyt->Bool.V1In)
copyint ((* yyNew)->Bool.V1Out, yyt->Bool.V1Out)
copyint ((* yyNew)->Bool.VIn, yyt->Bool.VIn)
copyint ((* yyNew)->Bool.VOut, yyt->Bool.VOut)
copyint ((* yyNew)->Bool.D, yyt->Bool.D)
copytParUsage ((* yyNew)->Bool.PUIn, yyt->Bool.PUIn)
copytParUsage ((* yyNew)->Bool.PUOut, yyt->Bool.PUOut)
return;
case kUserBool: (* yyNew)->UserBool = yyt->UserBool;
copyint ((* yyNew)->UserBool.V1In, yyt->UserBool.V1In)
copyint ((* yyNew)->UserBool.V1Out, yyt->UserBool.V1Out)
copyint ((* yyNew)->UserBool.VIn, yyt->UserBool.VIn)
copyint ((* yyNew)->UserBool.VOut, yyt->UserBool.VOut)
copyint ((* yyNew)->UserBool.D, yyt->UserBool.D)
copytParUsage ((* yyNew)->UserBool.PUIn, yyt->UserBool.PUIn)
copytParUsage ((* yyNew)->UserBool.PUOut, yyt->UserBool.PUOut)
copytPosition ((* yyNew)->UserBool.Pos, yyt->UserBool.Pos)
copytSymbol ((* yyNew)->UserBool.Sym, yyt->UserBool.Sym)
return;
case kUserStructTypes: (* yyNew)->UserStructTypes = yyt->UserStructTypes;
copyint ((* yyNew)->UserStructTypes.V1In, yyt->UserStructTypes.V1In)
copyint ((* yyNew)->UserStructTypes.V1Out, yyt->UserStructTypes.V1Out)
copyint ((* yyNew)->UserStructTypes.VIn, yyt->UserStructTypes.VIn)
copyint ((* yyNew)->UserStructTypes.VOut, yyt->UserStructTypes.VOut)
copyint ((* yyNew)->UserStructTypes.D, yyt->UserStructTypes.D)
copytParUsage ((* yyNew)->UserStructTypes.PUIn, yyt->UserStructTypes.PUIn)
copytParUsage ((* yyNew)->UserStructTypes.PUOut, yyt->UserStructTypes.PUOut)
copytPosition ((* yyNew)->UserStructTypes.Pos, yyt->UserStructTypes.Pos)
copytSymbol ((* yyNew)->UserStructTypes.Sym, yyt->UserStructTypes.Sym)
yyt = yyt->UserStructTypes.FieldList;
yyNew = & (* yyNew)->UserStructTypes.FieldList; break;
case kUserDataTypes: (* yyNew)->UserDataTypes = yyt->UserDataTypes;
copyint ((* yyNew)->UserDataTypes.V1In, yyt->UserDataTypes.V1In)
copyint ((* yyNew)->UserDataTypes.V1Out, yyt->UserDataTypes.V1Out)
copyint ((* yyNew)->UserDataTypes.VIn, yyt->UserDataTypes.VIn)
copyint ((* yyNew)->UserDataTypes.VOut, yyt->UserDataTypes.VOut)
copyint ((* yyNew)->UserDataTypes.D, yyt->UserDataTypes.D)
copytParUsage ((* yyNew)->UserDataTypes.PUIn, yyt->UserDataTypes.PUIn)
copytParUsage ((* yyNew)->UserDataTypes.PUOut, yyt->UserDataTypes.PUOut)
copytPosition ((* yyNew)->UserDataTypes.Pos, yyt->UserDataTypes.Pos)
copytSymbol ((* yyNew)->UserDataTypes.Sym, yyt->UserDataTypes.Sym)
yyt = yyt->UserDataTypes.FieldList;
yyNew = & (* yyNew)->UserDataTypes.FieldList; break;
case kUserRecord: (* yyNew)->UserRecord = yyt->UserRecord;
copyint ((* yyNew)->UserRecord.V1In, yyt->UserRecord.V1In)
copyint ((* yyNew)->UserRecord.V1Out, yyt->UserRecord.V1Out)
copyint ((* yyNew)->UserRecord.VIn, yyt->UserRecord.VIn)
copyint ((* yyNew)->UserRecord.VOut, yyt->UserRecord.VOut)
copyint ((* yyNew)->UserRecord.D, yyt->UserRecord.D)
copytParUsage ((* yyNew)->UserRecord.PUIn, yyt->UserRecord.PUIn)
copytParUsage ((* yyNew)->UserRecord.PUOut, yyt->UserRecord.PUOut)
copytPosition ((* yyNew)->UserRecord.Pos, yyt->UserRecord.Pos)
copytSymbol ((* yyNew)->UserRecord.Sym, yyt->UserRecord.Sym)
yyt = yyt->UserRecord.FieldList;
yyNew = & (* yyNew)->UserRecord.FieldList; break;
case kUserPackedRecord: (* yyNew)->UserPackedRecord = yyt->UserPackedRecord;
copyint ((* yyNew)->UserPackedRecord.V1In, yyt->UserPackedRecord.V1In)
copyint ((* yyNew)->UserPackedRecord.V1Out, yyt->UserPackedRecord.V1Out)
copyint ((* yyNew)->UserPackedRecord.VIn, yyt->UserPackedRecord.VIn)
copyint ((* yyNew)->UserPackedRecord.VOut, yyt->UserPackedRecord.VOut)
copyint ((* yyNew)->UserPackedRecord.D, yyt->UserPackedRecord.D)
copytParUsage ((* yyNew)->UserPackedRecord.PUIn, yyt->UserPackedRecord.PUIn)
copytParUsage ((* yyNew)->UserPackedRecord.PUOut, yyt->UserPackedRecord.PUOut)
copytPosition ((* yyNew)->UserPackedRecord.Pos, yyt->UserPackedRecord.Pos)
copytSymbol ((* yyNew)->UserPackedRecord.Sym, yyt->UserPackedRecord.Sym)
yyt = yyt->UserPackedRecord.FieldList;
yyNew = & (* yyNew)->UserPackedRecord.FieldList; break;
case kUserUnion: (* yyNew)->UserUnion = yyt->UserUnion;
copyint ((* yyNew)->UserUnion.V1In, yyt->UserUnion.V1In)
copyint ((* yyNew)->UserUnion.V1Out, yyt->UserUnion.V1Out)
copyint ((* yyNew)->UserUnion.VIn, yyt->UserUnion.VIn)
copyint ((* yyNew)->UserUnion.VOut, yyt->UserUnion.VOut)
copyint ((* yyNew)->UserUnion.D, yyt->UserUnion.D)
copytParUsage ((* yyNew)->UserUnion.PUIn, yyt->UserUnion.PUIn)
copytParUsage ((* yyNew)->UserUnion.PUOut, yyt->UserUnion.PUOut)
copytPosition ((* yyNew)->UserUnion.Pos, yyt->UserUnion.Pos)
copytSymbol ((* yyNew)->UserUnion.Sym, yyt->UserUnion.Sym)
yyt = yyt->UserUnion.FieldList;
yyNew = & (* yyNew)->UserUnion.FieldList; break;
case kUserChanRecord: (* yyNew)->UserChanRecord = yyt->UserChanRecord;
copyint ((* yyNew)->UserChanRecord.V1In, yyt->UserChanRecord.V1In)
copyint ((* yyNew)->UserChanRecord.V1Out, yyt->UserChanRecord.V1Out)
copyint ((* yyNew)->UserChanRecord.VIn, yyt->UserChanRecord.VIn)
copyint ((* yyNew)->UserChanRecord.VOut, yyt->UserChanRecord.VOut)
copyint ((* yyNew)->UserChanRecord.D, yyt->UserChanRecord.D)
copytParUsage ((* yyNew)->UserChanRecord.PUIn, yyt->UserChanRecord.PUIn)
copytParUsage ((* yyNew)->UserChanRecord.PUOut, yyt->UserChanRecord.PUOut)
copytPosition ((* yyNew)->UserChanRecord.Pos, yyt->UserChanRecord.Pos)
copytSymbol ((* yyNew)->UserChanRecord.Sym, yyt->UserChanRecord.Sym)
yyt = yyt->UserChanRecord.FieldList;
yyNew = & (* yyNew)->UserChanRecord.FieldList; break;
case kTypeList: (* yyNew)->TypeList = yyt->TypeList;
copyint ((* yyNew)->TypeList.V1In, yyt->TypeList.V1In)
copyint ((* yyNew)->TypeList.V1Out, yyt->TypeList.V1Out)
copyint ((* yyNew)->TypeList.VIn, yyt->TypeList.VIn)
copyint ((* yyNew)->TypeList.VOut, yyt->TypeList.VOut)
copyint ((* yyNew)->TypeList.D, yyt->TypeList.D)
copytParUsage ((* yyNew)->TypeList.PUIn, yyt->TypeList.PUIn)
copytParUsage ((* yyNew)->TypeList.PUOut, yyt->TypeList.PUOut)
return;
case kNoType: (* yyNew)->NoType = yyt->NoType;
copyint ((* yyNew)->NoType.V1In, yyt->NoType.V1In)
copyint ((* yyNew)->NoType.V1Out, yyt->NoType.V1Out)
copyint ((* yyNew)->NoType.VIn, yyt->NoType.VIn)
copyint ((* yyNew)->NoType.VOut, yyt->NoType.VOut)
copyint ((* yyNew)->NoType.D, yyt->NoType.D)
copytParUsage ((* yyNew)->NoType.PUIn, yyt->NoType.PUIn)
copytParUsage ((* yyNew)->NoType.PUOut, yyt->NoType.PUOut)
return;
case kAType: (* yyNew)->AType = yyt->AType;
copyint ((* yyNew)->AType.V1In, yyt->AType.V1In)
copyint ((* yyNew)->AType.V1Out, yyt->AType.V1Out)
copyint ((* yyNew)->AType.VIn, yyt->AType.VIn)
copyint ((* yyNew)->AType.VOut, yyt->AType.VOut)
copyint ((* yyNew)->AType.D, yyt->AType.D)
copytParUsage ((* yyNew)->AType.PUIn, yyt->AType.PUIn)
copytParUsage ((* yyNew)->AType.PUOut, yyt->AType.PUOut)
copytTree ((* yyNew)->AType.Type, yyt->AType.Type)
yyt = yyt->AType.Next;
yyNew = & (* yyNew)->AType.Next; break;
case kFieldList: (* yyNew)->FieldList = yyt->FieldList;
copytParUsage ((* yyNew)->FieldList.PUIn, yyt->FieldList.PUIn)
copytParUsage ((* yyNew)->FieldList.PUOut, yyt->FieldList.PUOut)
return;
case kNoField: (* yyNew)->NoField = yyt->NoField;
copytParUsage ((* yyNew)->NoField.PUIn, yyt->NoField.PUIn)
copytParUsage ((* yyNew)->NoField.PUOut, yyt->NoField.PUOut)
return;
case kAField: (* yyNew)->AField = yyt->AField;
copytParUsage ((* yyNew)->AField.PUIn, yyt->AField.PUIn)
copytParUsage ((* yyNew)->AField.PUOut, yyt->AField.PUOut)
copytSymbol ((* yyNew)->AField.Sym, yyt->AField.Sym)
copytTree ((* yyNew)->AField.Type, yyt->AField.Type)
yyt = yyt->AField.Next;
yyNew = & (* yyNew)->AField.Next; break;
case kProtocol: (* yyNew)->Protocol = yyt->Protocol;
copytPosition ((* yyNew)->Protocol.Pos, yyt->Protocol.Pos)
copytParUsage ((* yyNew)->Protocol.PUIn, yyt->Protocol.PUIn)
copytParUsage ((* yyNew)->Protocol.PUOut, yyt->Protocol.PUOut)
return;
case kAny: (* yyNew)->Any = yyt->Any;
copytPosition ((* yyNew)->Any.Pos, yyt->Any.Pos)
copytParUsage ((* yyNew)->Any.PUIn, yyt->Any.PUIn)
copytParUsage ((* yyNew)->Any.PUOut, yyt->Any.PUOut)
return;
case kNamed: (* yyNew)->Named = yyt->Named;
copytPosition ((* yyNew)->Named.Pos, yyt->Named.Pos)
copytParUsage ((* yyNew)->Named.PUIn, yyt->Named.PUIn)
copytParUsage ((* yyNew)->Named.PUOut, yyt->Named.PUOut)
copytSymbol ((* yyNew)->Named.Sym, yyt->Named.Sym)
return;
case kSimple: (* yyNew)->Simple = yyt->Simple;
copytPosition ((* yyNew)->Simple.Pos, yyt->Simple.Pos)
copytParUsage ((* yyNew)->Simple.PUIn, yyt->Simple.PUIn)
copytParUsage ((* yyNew)->Simple.PUOut, yyt->Simple.PUOut)
yyt = yyt->Simple.Type;
yyNew = & (* yyNew)->Simple.Type; break;
case kCounted: (* yyNew)->Counted = yyt->Counted;
copytPosition ((* yyNew)->Counted.Pos, yyt->Counted.Pos)
copytParUsage ((* yyNew)->Counted.PUIn, yyt->Counted.PUIn)
copytParUsage ((* yyNew)->Counted.PUOut, yyt->Counted.PUOut)
copytTree ((* yyNew)->Counted.Length, yyt->Counted.Length)
yyt = yyt->Counted.Type;
yyNew = & (* yyNew)->Counted.Type; break;
case kSeqProList: (* yyNew)->SeqProList = yyt->SeqProList;
copytParUsage ((* yyNew)->SeqProList.PUIn, yyt->SeqProList.PUIn)
copytParUsage ((* yyNew)->SeqProList.PUOut, yyt->SeqProList.PUOut)
return;
case kNoSeqPro: (* yyNew)->NoSeqPro = yyt->NoSeqPro;
copytParUsage ((* yyNew)->NoSeqPro.PUIn, yyt->NoSeqPro.PUIn)
copytParUsage ((* yyNew)->NoSeqPro.PUOut, yyt->NoSeqPro.PUOut)
return;
case kASeqPro: (* yyNew)->ASeqPro = yyt->ASeqPro;
copytParUsage ((* yyNew)->ASeqPro.PUIn, yyt->ASeqPro.PUIn)
copytParUsage ((* yyNew)->ASeqPro.PUOut, yyt->ASeqPro.PUOut)
copytTree ((* yyNew)->ASeqPro.Protocol, yyt->ASeqPro.Protocol)
yyt = yyt->ASeqPro.Next;
yyNew = & (* yyNew)->ASeqPro.Next; break;
case kExpr: (* yyNew)->Expr = yyt->Expr;
copytPosition ((* yyNew)->Expr.Pos, yyt->Expr.Pos)
copytType ((* yyNew)->Expr.DefType, yyt->Expr.DefType)
copytType ((* yyNew)->Expr.FLIn, yyt->Expr.FLIn)
copytType ((* yyNew)->Expr.FLOut, yyt->Expr.FLOut)
copyint ((* yyNew)->Expr.V1In, yyt->Expr.V1In)
copyint ((* yyNew)->Expr.V1Out, yyt->Expr.V1Out)
copytType ((* yyNew)->Expr.SynType, yyt->Expr.SynType)
copytType ((* yyNew)->Expr.InhType, yyt->Expr.InhType)
copyint ((* yyNew)->Expr.VIn, yyt->Expr.VIn)
copyint ((* yyNew)->Expr.VOut, yyt->Expr.VOut)
copybool ((* yyNew)->Expr.C, yyt->Expr.C)
copytValue ((* yyNew)->Expr.CV, yyt->Expr.CV)
copytSymbol ((* yyNew)->Expr.Parent, yyt->Expr.Parent)
copytParUsage ((* yyNew)->Expr.PUIn, yyt->Expr.PUIn)
copytParUsage ((* yyNew)->Expr.PUOut, yyt->Expr.PUOut)
copybool ((* yyNew)->Expr.LV, yyt->Expr.LV)
return;
case kBinary: (* yyNew)->Binary = yyt->Binary;
copytPosition ((* yyNew)->Binary.Pos, yyt->Binary.Pos)
copytType ((* yyNew)->Binary.DefType, yyt->Binary.DefType)
copytType ((* yyNew)->Binary.FLIn, yyt->Binary.FLIn)
copytType ((* yyNew)->Binary.FLOut, yyt->Binary.FLOut)
copyint ((* yyNew)->Binary.V1In, yyt->Binary.V1In)
copyint ((* yyNew)->Binary.V1Out, yyt->Binary.V1Out)
copytType ((* yyNew)->Binary.SynType, yyt->Binary.SynType)
copytType ((* yyNew)->Binary.InhType, yyt->Binary.InhType)
copyint ((* yyNew)->Binary.VIn, yyt->Binary.VIn)
copyint ((* yyNew)->Binary.VOut, yyt->Binary.VOut)
copybool ((* yyNew)->Binary.C, yyt->Binary.C)
copytValue ((* yyNew)->Binary.CV, yyt->Binary.CV)
copytSymbol ((* yyNew)->Binary.Parent, yyt->Binary.Parent)
copytParUsage ((* yyNew)->Binary.PUIn, yyt->Binary.PUIn)
copytParUsage ((* yyNew)->Binary.PUOut, yyt->Binary.PUOut)
copybool ((* yyNew)->Binary.LV, yyt->Binary.LV)
copytTree ((* yyNew)->Binary.Lop, yyt->Binary.Lop)
copytTree ((* yyNew)->Binary.Rop, yyt->Binary.Rop)
yyt = yyt->Binary.Op;
yyNew = & (* yyNew)->Binary.Op; break;
case kUnary: (* yyNew)->Unary = yyt->Unary;
copytPosition ((* yyNew)->Unary.Pos, yyt->Unary.Pos)
copytType ((* yyNew)->Unary.DefType, yyt->Unary.DefType)
copytType ((* yyNew)->Unary.FLIn, yyt->Unary.FLIn)
copytType ((* yyNew)->Unary.FLOut, yyt->Unary.FLOut)
copyint ((* yyNew)->Unary.V1In, yyt->Unary.V1In)
copyint ((* yyNew)->Unary.V1Out, yyt->Unary.V1Out)
copytType ((* yyNew)->Unary.SynType, yyt->Unary.SynType)
copytType ((* yyNew)->Unary.InhType, yyt->Unary.InhType)
copyint ((* yyNew)->Unary.VIn, yyt->Unary.VIn)
copyint ((* yyNew)->Unary.VOut, yyt->Unary.VOut)
copybool ((* yyNew)->Unary.C, yyt->Unary.C)
copytValue ((* yyNew)->Unary.CV, yyt->Unary.CV)
copytSymbol ((* yyNew)->Unary.Parent, yyt->Unary.Parent)
copytParUsage ((* yyNew)->Unary.PUIn, yyt->Unary.PUIn)
copytParUsage ((* yyNew)->Unary.PUOut, yyt->Unary.PUOut)
copybool ((* yyNew)->Unary.LV, yyt->Unary.LV)
copytTree ((* yyNew)->Unary.Expr, yyt->Unary.Expr)
yyt = yyt->Unary.Op;
yyNew = & (* yyNew)->Unary.Op; break;
case kConst: (* yyNew)->Const = yyt->Const;
copytPosition ((* yyNew)->Const.Pos, yyt->Const.Pos)
copytType ((* yyNew)->Const.DefType, yyt->Const.DefType)
copytType ((* yyNew)->Const.FLIn, yyt->Const.FLIn)
copytType ((* yyNew)->Const.FLOut, yyt->Const.FLOut)
copyint ((* yyNew)->Const.V1In, yyt->Const.V1In)
copyint ((* yyNew)->Const.V1Out, yyt->Const.V1Out)
copytType ((* yyNew)->Const.SynType, yyt->Const.SynType)
copytType ((* yyNew)->Const.InhType, yyt->Const.InhType)
copyint ((* yyNew)->Const.VIn, yyt->Const.VIn)
copyint ((* yyNew)->Const.VOut, yyt->Const.VOut)
copybool ((* yyNew)->Const.C, yyt->Const.C)
copytValue ((* yyNew)->Const.CV, yyt->Const.CV)
copytSymbol ((* yyNew)->Const.Parent, yyt->Const.Parent)
copytParUsage ((* yyNew)->Const.PUIn, yyt->Const.PUIn)
copytParUsage ((* yyNew)->Const.PUOut, yyt->Const.PUOut)
copybool ((* yyNew)->Const.LV, yyt->Const.LV)
copytValue ((* yyNew)->Const.Value, yyt->Const.Value)
yyt = yyt->Const.Type;
yyNew = & (* yyNew)->Const.Type; break;
case kValof: (* yyNew)->Valof = yyt->Valof;
copytPosition ((* yyNew)->Valof.Pos, yyt->Valof.Pos)
copytType ((* yyNew)->Valof.DefType, yyt->Valof.DefType)
copytType ((* yyNew)->Valof.FLIn, yyt->Valof.FLIn)
copytType ((* yyNew)->Valof.FLOut, yyt->Valof.FLOut)
copyint ((* yyNew)->Valof.V1In, yyt->Valof.V1In)
copyint ((* yyNew)->Valof.V1Out, yyt->Valof.V1Out)
copytType ((* yyNew)->Valof.SynType, yyt->Valof.SynType)
copytType ((* yyNew)->Valof.InhType, yyt->Valof.InhType)
copyint ((* yyNew)->Valof.VIn, yyt->Valof.VIn)
copyint ((* yyNew)->Valof.VOut, yyt->Valof.VOut)
copybool ((* yyNew)->Valof.C, yyt->Valof.C)
copytValue ((* yyNew)->Valof.CV, yyt->Valof.CV)
copytSymbol ((* yyNew)->Valof.Parent, yyt->Valof.Parent)
copytParUsage ((* yyNew)->Valof.PUIn, yyt->Valof.PUIn)
copytParUsage ((* yyNew)->Valof.PUOut, yyt->Valof.PUOut)
copybool ((* yyNew)->Valof.LV, yyt->Valof.LV)
copytTree ((* yyNew)->Valof.SpecList, yyt->Valof.SpecList)
copytTree ((* yyNew)->Valof.Process, yyt->Valof.Process)
copytSymbol ((* yyNew)->Valof.Sym, yyt->Valof.Sym)
copytTree ((* yyNew)->Valof.FuncCall, yyt->Valof.FuncCall)
copyint ((* yyNew)->Valof.VS, yyt->Valof.VS)
yyt = yyt->Valof.ExprList;
yyNew = & (* yyNew)->Valof.ExprList; break;
case kFuncCall: (* yyNew)->FuncCall = yyt->FuncCall;
copytPosition ((* yyNew)->FuncCall.Pos, yyt->FuncCall.Pos)
copytType ((* yyNew)->FuncCall.DefType, yyt->FuncCall.DefType)
copytType ((* yyNew)->FuncCall.FLIn, yyt->FuncCall.FLIn)
copytType ((* yyNew)->FuncCall.FLOut, yyt->FuncCall.FLOut)
copyint ((* yyNew)->FuncCall.V1In, yyt->FuncCall.V1In)
copyint ((* yyNew)->FuncCall.V1Out, yyt->FuncCall.V1Out)
copytType ((* yyNew)->FuncCall.SynType, yyt->FuncCall.SynType)
copytType ((* yyNew)->FuncCall.InhType, yyt->FuncCall.InhType)
copyint ((* yyNew)->FuncCall.VIn, yyt->FuncCall.VIn)
copyint ((* yyNew)->FuncCall.VOut, yyt->FuncCall.VOut)
copybool ((* yyNew)->FuncCall.C, yyt->FuncCall.C)
copytValue ((* yyNew)->FuncCall.CV, yyt->FuncCall.CV)
copytSymbol ((* yyNew)->FuncCall.Parent, yyt->FuncCall.Parent)
copytParUsage ((* yyNew)->FuncCall.PUIn, yyt->FuncCall.PUIn)
copytParUsage ((* yyNew)->FuncCall.PUOut, yyt->FuncCall.PUOut)
copybool ((* yyNew)->FuncCall.LV, yyt->FuncCall.LV)
copytSymbol ((* yyNew)->FuncCall.Sym, yyt->FuncCall.Sym)
copyint ((* yyNew)->FuncCall.Depth, yyt->FuncCall.Depth)
copytFrame ((* yyNew)->FuncCall.Frame, yyt->FuncCall.Frame)
yyt = yyt->FuncCall.ExprList;
yyNew = & (* yyNew)->FuncCall.ExprList; break;
case kElementalExpr: (* yyNew)->ElementalExpr = yyt->ElementalExpr;
copytPosition ((* yyNew)->ElementalExpr.Pos, yyt->ElementalExpr.Pos)
copytType ((* yyNew)->ElementalExpr.DefType, yyt->ElementalExpr.DefType)
copytType ((* yyNew)->ElementalExpr.FLIn, yyt->ElementalExpr.FLIn)
copytType ((* yyNew)->ElementalExpr.FLOut, yyt->ElementalExpr.FLOut)
copyint ((* yyNew)->ElementalExpr.V1In, yyt->ElementalExpr.V1In)
copyint ((* yyNew)->ElementalExpr.V1Out, yyt->ElementalExpr.V1Out)
copytType ((* yyNew)->ElementalExpr.SynType, yyt->ElementalExpr.SynType)
copytType ((* yyNew)->ElementalExpr.InhType, yyt->ElementalExpr.InhType)
copyint ((* yyNew)->ElementalExpr.VIn, yyt->ElementalExpr.VIn)
copyint ((* yyNew)->ElementalExpr.VOut, yyt->ElementalExpr.VOut)
copybool ((* yyNew)->ElementalExpr.C, yyt->ElementalExpr.C)
copytValue ((* yyNew)->ElementalExpr.CV, yyt->ElementalExpr.CV)
copytSymbol ((* yyNew)->ElementalExpr.Parent, yyt->ElementalExpr.Parent)
copytParUsage ((* yyNew)->ElementalExpr.PUIn, yyt->ElementalExpr.PUIn)
copytParUsage ((* yyNew)->ElementalExpr.PUOut, yyt->ElementalExpr.PUOut)
copybool ((* yyNew)->ElementalExpr.LV, yyt->ElementalExpr.LV)
copybool ((* yyNew)->ElementalExpr.PAR, yyt->ElementalExpr.PAR)
yyt = yyt->ElementalExpr.Elemental;
yyNew = & (* yyNew)->ElementalExpr.Elemental; break;
case kDefault: (* yyNew)->Default = yyt->Default;
copytPosition ((* yyNew)->Default.Pos, yyt->Default.Pos)
copytType ((* yyNew)->Default.DefType, yyt->Default.DefType)
copytType ((* yyNew)->Default.FLIn, yyt->Default.FLIn)
copytType ((* yyNew)->Default.FLOut, yyt->Default.FLOut)
copyint ((* yyNew)->Default.V1In, yyt->Default.V1In)
copyint ((* yyNew)->Default.V1Out, yyt->Default.V1Out)
copytType ((* yyNew)->Default.SynType, yyt->Default.SynType)
copytType ((* yyNew)->Default.InhType, yyt->Default.InhType)
copyint ((* yyNew)->Default.VIn, yyt->Default.VIn)
copyint ((* yyNew)->Default.VOut, yyt->Default.VOut)
copybool ((* yyNew)->Default.C, yyt->Default.C)
copytValue ((* yyNew)->Default.CV, yyt->Default.CV)
copytSymbol ((* yyNew)->Default.Parent, yyt->Default.Parent)
copytParUsage ((* yyNew)->Default.PUIn, yyt->Default.PUIn)
copytParUsage ((* yyNew)->Default.PUOut, yyt->Default.PUOut)
copybool ((* yyNew)->Default.LV, yyt->Default.LV)
return;
case kConvert: (* yyNew)->Convert = yyt->Convert;
copytPosition ((* yyNew)->Convert.Pos, yyt->Convert.Pos)
copytType ((* yyNew)->Convert.DefType, yyt->Convert.DefType)
copytType ((* yyNew)->Convert.FLIn, yyt->Convert.FLIn)
copytType ((* yyNew)->Convert.FLOut, yyt->Convert.FLOut)
copyint ((* yyNew)->Convert.V1In, yyt->Convert.V1In)
copyint ((* yyNew)->Convert.V1Out, yyt->Convert.V1Out)
copytType ((* yyNew)->Convert.SynType, yyt->Convert.SynType)
copytType ((* yyNew)->Convert.InhType, yyt->Convert.InhType)
copyint ((* yyNew)->Convert.VIn, yyt->Convert.VIn)
copyint ((* yyNew)->Convert.VOut, yyt->Convert.VOut)
copybool ((* yyNew)->Convert.C, yyt->Convert.C)
copytValue ((* yyNew)->Convert.CV, yyt->Convert.CV)
copytSymbol ((* yyNew)->Convert.Parent, yyt->Convert.Parent)
copytParUsage ((* yyNew)->Convert.PUIn, yyt->Convert.PUIn)
copytParUsage ((* yyNew)->Convert.PUOut, yyt->Convert.PUOut)
copybool ((* yyNew)->Convert.LV, yyt->Convert.LV)
copytTree ((* yyNew)->Convert.Type, yyt->Convert.Type)
yyt = yyt->Convert.Expr;
yyNew = & (* yyNew)->Convert.Expr; break;
case kRound: (* yyNew)->Round = yyt->Round;
copytPosition ((* yyNew)->Round.Pos, yyt->Round.Pos)
copytType ((* yyNew)->Round.DefType, yyt->Round.DefType)
copytType ((* yyNew)->Round.FLIn, yyt->Round.FLIn)
copytType ((* yyNew)->Round.FLOut, yyt->Round.FLOut)
copyint ((* yyNew)->Round.V1In, yyt->Round.V1In)
copyint ((* yyNew)->Round.V1Out, yyt->Round.V1Out)
copytType ((* yyNew)->Round.SynType, yyt->Round.SynType)
copytType ((* yyNew)->Round.InhType, yyt->Round.InhType)
copyint ((* yyNew)->Round.VIn, yyt->Round.VIn)
copyint ((* yyNew)->Round.VOut, yyt->Round.VOut)
copybool ((* yyNew)->Round.C, yyt->Round.C)
copytValue ((* yyNew)->Round.CV, yyt->Round.CV)
copytSymbol ((* yyNew)->Round.Parent, yyt->Round.Parent)
copytParUsage ((* yyNew)->Round.PUIn, yyt->Round.PUIn)
copytParUsage ((* yyNew)->Round.PUOut, yyt->Round.PUOut)
copybool ((* yyNew)->Round.LV, yyt->Round.LV)
copytTree ((* yyNew)->Round.Type, yyt->Round.Type)
yyt = yyt->Round.Expr;
yyNew = & (* yyNew)->Round.Expr; break;
case kTrunc: (* yyNew)->Trunc = yyt->Trunc;
copytPosition ((* yyNew)->Trunc.Pos, yyt->Trunc.Pos)
copytType ((* yyNew)->Trunc.DefType, yyt->Trunc.DefType)
copytType ((* yyNew)->Trunc.FLIn, yyt->Trunc.FLIn)
copytType ((* yyNew)->Trunc.FLOut, yyt->Trunc.FLOut)
copyint ((* yyNew)->Trunc.V1In, yyt->Trunc.V1In)
copyint ((* yyNew)->Trunc.V1Out, yyt->Trunc.V1Out)
copytType ((* yyNew)->Trunc.SynType, yyt->Trunc.SynType)
copytType ((* yyNew)->Trunc.InhType, yyt->Trunc.InhType)
copyint ((* yyNew)->Trunc.VIn, yyt->Trunc.VIn)
copyint ((* yyNew)->Trunc.VOut, yyt->Trunc.VOut)
copybool ((* yyNew)->Trunc.C, yyt->Trunc.C)
copytValue ((* yyNew)->Trunc.CV, yyt->Trunc.CV)
copytSymbol ((* yyNew)->Trunc.Parent, yyt->Trunc.Parent)
copytParUsage ((* yyNew)->Trunc.PUIn, yyt->Trunc.PUIn)
copytParUsage ((* yyNew)->Trunc.PUOut, yyt->Trunc.PUOut)
copybool ((* yyNew)->Trunc.LV, yyt->Trunc.LV)
copytTree ((* yyNew)->Trunc.Type, yyt->Trunc.Type)
yyt = yyt->Trunc.Expr;
yyNew = & (* yyNew)->Trunc.Expr; break;
case kMostPos: (* yyNew)->MostPos = yyt->MostPos;
copytPosition ((* yyNew)->MostPos.Pos, yyt->MostPos.Pos)
copytType ((* yyNew)->MostPos.DefType, yyt->MostPos.DefType)
copytType ((* yyNew)->MostPos.FLIn, yyt->MostPos.FLIn)
copytType ((* yyNew)->MostPos.FLOut, yyt->MostPos.FLOut)
copyint ((* yyNew)->MostPos.V1In, yyt->MostPos.V1In)
copyint ((* yyNew)->MostPos.V1Out, yyt->MostPos.V1Out)
copytType ((* yyNew)->MostPos.SynType, yyt->MostPos.SynType)
copytType ((* yyNew)->MostPos.InhType, yyt->MostPos.InhType)
copyint ((* yyNew)->MostPos.VIn, yyt->MostPos.VIn)
copyint ((* yyNew)->MostPos.VOut, yyt->MostPos.VOut)
copybool ((* yyNew)->MostPos.C, yyt->MostPos.C)
copytValue ((* yyNew)->MostPos.CV, yyt->MostPos.CV)
copytSymbol ((* yyNew)->MostPos.Parent, yyt->MostPos.Parent)
copytParUsage ((* yyNew)->MostPos.PUIn, yyt->MostPos.PUIn)
copytParUsage ((* yyNew)->MostPos.PUOut, yyt->MostPos.PUOut)
copybool ((* yyNew)->MostPos.LV, yyt->MostPos.LV)
yyt = yyt->MostPos.Type;
yyNew = & (* yyNew)->MostPos.Type; break;
case kMostNeg: (* yyNew)->MostNeg = yyt->MostNeg;
copytPosition ((* yyNew)->MostNeg.Pos, yyt->MostNeg.Pos)
copytType ((* yyNew)->MostNeg.DefType, yyt->MostNeg.DefType)
copytType ((* yyNew)->MostNeg.FLIn, yyt->MostNeg.FLIn)
copytType ((* yyNew)->MostNeg.FLOut, yyt->MostNeg.FLOut)
copyint ((* yyNew)->MostNeg.V1In, yyt->MostNeg.V1In)
copyint ((* yyNew)->MostNeg.V1Out, yyt->MostNeg.V1Out)
copytType ((* yyNew)->MostNeg.SynType, yyt->MostNeg.SynType)
copytType ((* yyNew)->MostNeg.InhType, yyt->MostNeg.InhType)
copyint ((* yyNew)->MostNeg.VIn, yyt->MostNeg.VIn)
copyint ((* yyNew)->MostNeg.VOut, yyt->MostNeg.VOut)
copybool ((* yyNew)->MostNeg.C, yyt->MostNeg.C)
copytValue ((* yyNew)->MostNeg.CV, yyt->MostNeg.CV)
copytSymbol ((* yyNew)->MostNeg.Parent, yyt->MostNeg.Parent)
copytParUsage ((* yyNew)->MostNeg.PUIn, yyt->MostNeg.PUIn)
copytParUsage ((* yyNew)->MostNeg.PUOut, yyt->MostNeg.PUOut)
copybool ((* yyNew)->MostNeg.LV, yyt->MostNeg.LV)
yyt = yyt->MostNeg.Type;
yyNew = & (* yyNew)->MostNeg.Type; break;
case kCountArray: (* yyNew)->CountArray = yyt->CountArray;
copytPosition ((* yyNew)->CountArray.Pos, yyt->CountArray.Pos)
copytType ((* yyNew)->CountArray.DefType, yyt->CountArray.DefType)
copytType ((* yyNew)->CountArray.FLIn, yyt->CountArray.FLIn)
copytType ((* yyNew)->CountArray.FLOut, yyt->CountArray.FLOut)
copyint ((* yyNew)->CountArray.V1In, yyt->CountArray.V1In)
copyint ((* yyNew)->CountArray.V1Out, yyt->CountArray.V1Out)
copytType ((* yyNew)->CountArray.SynType, yyt->CountArray.SynType)
copytType ((* yyNew)->CountArray.InhType, yyt->CountArray.InhType)
copyint ((* yyNew)->CountArray.VIn, yyt->CountArray.VIn)
copyint ((* yyNew)->CountArray.VOut, yyt->CountArray.VOut)
copybool ((* yyNew)->CountArray.C, yyt->CountArray.C)
copytValue ((* yyNew)->CountArray.CV, yyt->CountArray.CV)
copytSymbol ((* yyNew)->CountArray.Parent, yyt->CountArray.Parent)
copytParUsage ((* yyNew)->CountArray.PUIn, yyt->CountArray.PUIn)
copytParUsage ((* yyNew)->CountArray.PUOut, yyt->CountArray.PUOut)
copybool ((* yyNew)->CountArray.LV, yyt->CountArray.LV)
copytTree ((* yyNew)->CountArray.Length, yyt->CountArray.Length)
yyt = yyt->CountArray.Expr;
yyNew = & (* yyNew)->CountArray.Expr; break;
case kUnBound: (* yyNew)->UnBound = yyt->UnBound;
copytPosition ((* yyNew)->UnBound.Pos, yyt->UnBound.Pos)
copytType ((* yyNew)->UnBound.DefType, yyt->UnBound.DefType)
copytType ((* yyNew)->UnBound.FLIn, yyt->UnBound.FLIn)
copytType ((* yyNew)->UnBound.FLOut, yyt->UnBound.FLOut)
copyint ((* yyNew)->UnBound.V1In, yyt->UnBound.V1In)
copyint ((* yyNew)->UnBound.V1Out, yyt->UnBound.V1Out)
copytType ((* yyNew)->UnBound.SynType, yyt->UnBound.SynType)
copytType ((* yyNew)->UnBound.InhType, yyt->UnBound.InhType)
copyint ((* yyNew)->UnBound.VIn, yyt->UnBound.VIn)
copyint ((* yyNew)->UnBound.VOut, yyt->UnBound.VOut)
copybool ((* yyNew)->UnBound.C, yyt->UnBound.C)
copytValue ((* yyNew)->UnBound.CV, yyt->UnBound.CV)
copytSymbol ((* yyNew)->UnBound.Parent, yyt->UnBound.Parent)
copytParUsage ((* yyNew)->UnBound.PUIn, yyt->UnBound.PUIn)
copytParUsage ((* yyNew)->UnBound.PUOut, yyt->UnBound.PUOut)
copybool ((* yyNew)->UnBound.LV, yyt->UnBound.LV)
return;
case kBytesIn: (* yyNew)->BytesIn = yyt->BytesIn;
copytPosition ((* yyNew)->BytesIn.Pos, yyt->BytesIn.Pos)
copytType ((* yyNew)->BytesIn.DefType, yyt->BytesIn.DefType)
copytType ((* yyNew)->BytesIn.FLIn, yyt->BytesIn.FLIn)
copytType ((* yyNew)->BytesIn.FLOut, yyt->BytesIn.FLOut)
copyint ((* yyNew)->BytesIn.V1In, yyt->BytesIn.V1In)
copyint ((* yyNew)->BytesIn.V1Out, yyt->BytesIn.V1Out)
copytType ((* yyNew)->BytesIn.SynType, yyt->BytesIn.SynType)
copytType ((* yyNew)->BytesIn.InhType, yyt->BytesIn.InhType)
copyint ((* yyNew)->BytesIn.VIn, yyt->BytesIn.VIn)
copyint ((* yyNew)->BytesIn.VOut, yyt->BytesIn.VOut)
copybool ((* yyNew)->BytesIn.C, yyt->BytesIn.C)
copytValue ((* yyNew)->BytesIn.CV, yyt->BytesIn.CV)
copytSymbol ((* yyNew)->BytesIn.Parent, yyt->BytesIn.Parent)
copytParUsage ((* yyNew)->BytesIn.PUIn, yyt->BytesIn.PUIn)
copytParUsage ((* yyNew)->BytesIn.PUOut, yyt->BytesIn.PUOut)
copybool ((* yyNew)->BytesIn.LV, yyt->BytesIn.LV)
return;
case kBytesInType: (* yyNew)->BytesInType = yyt->BytesInType;
copytPosition ((* yyNew)->BytesInType.Pos, yyt->BytesInType.Pos)
copytType ((* yyNew)->BytesInType.DefType, yyt->BytesInType.DefType)
copytType ((* yyNew)->BytesInType.FLIn, yyt->BytesInType.FLIn)
copytType ((* yyNew)->BytesInType.FLOut, yyt->BytesInType.FLOut)
copyint ((* yyNew)->BytesInType.V1In, yyt->BytesInType.V1In)
copyint ((* yyNew)->BytesInType.V1Out, yyt->BytesInType.V1Out)
copytType ((* yyNew)->BytesInType.SynType, yyt->BytesInType.SynType)
copytType ((* yyNew)->BytesInType.InhType, yyt->BytesInType.InhType)
copyint ((* yyNew)->BytesInType.VIn, yyt->BytesInType.VIn)
copyint ((* yyNew)->BytesInType.VOut, yyt->BytesInType.VOut)
copybool ((* yyNew)->BytesInType.C, yyt->BytesInType.C)
copytValue ((* yyNew)->BytesInType.CV, yyt->BytesInType.CV)
copytSymbol ((* yyNew)->BytesInType.Parent, yyt->BytesInType.Parent)
copytParUsage ((* yyNew)->BytesInType.PUIn, yyt->BytesInType.PUIn)
copytParUsage ((* yyNew)->BytesInType.PUOut, yyt->BytesInType.PUOut)
copybool ((* yyNew)->BytesInType.LV, yyt->BytesInType.LV)
yyt = yyt->BytesInType.Type;
yyNew = & (* yyNew)->BytesInType.Type; break;
case kBytesInExpr: (* yyNew)->BytesInExpr = yyt->BytesInExpr;
copytPosition ((* yyNew)->BytesInExpr.Pos, yyt->BytesInExpr.Pos)
copytType ((* yyNew)->BytesInExpr.DefType, yyt->BytesInExpr.DefType)
copytType ((* yyNew)->BytesInExpr.FLIn, yyt->BytesInExpr.FLIn)
copytType ((* yyNew)->BytesInExpr.FLOut, yyt->BytesInExpr.FLOut)
copyint ((* yyNew)->BytesInExpr.V1In, yyt->BytesInExpr.V1In)
copyint ((* yyNew)->BytesInExpr.V1Out, yyt->BytesInExpr.V1Out)
copytType ((* yyNew)->BytesInExpr.SynType, yyt->BytesInExpr.SynType)
copytType ((* yyNew)->BytesInExpr.InhType, yyt->BytesInExpr.InhType)
copyint ((* yyNew)->BytesInExpr.VIn, yyt->BytesInExpr.VIn)
copyint ((* yyNew)->BytesInExpr.VOut, yyt->BytesInExpr.VOut)
copybool ((* yyNew)->BytesInExpr.C, yyt->BytesInExpr.C)
copytValue ((* yyNew)->BytesInExpr.CV, yyt->BytesInExpr.CV)
copytSymbol ((* yyNew)->BytesInExpr.Parent, yyt->BytesInExpr.Parent)
copytParUsage ((* yyNew)->BytesInExpr.PUIn, yyt->BytesInExpr.PUIn)
copytParUsage ((* yyNew)->BytesInExpr.PUOut, yyt->BytesInExpr.PUOut)
copybool ((* yyNew)->BytesInExpr.LV, yyt->BytesInExpr.LV)
yyt = yyt->BytesInExpr.Expr;
yyNew = & (* yyNew)->BytesInExpr.Expr; break;
case kSizeOf: (* yyNew)->SizeOf = yyt->SizeOf;
copytPosition ((* yyNew)->SizeOf.Pos, yyt->SizeOf.Pos)
copytType ((* yyNew)->SizeOf.DefType, yyt->SizeOf.DefType)
copytType ((* yyNew)->SizeOf.FLIn, yyt->SizeOf.FLIn)
copytType ((* yyNew)->SizeOf.FLOut, yyt->SizeOf.FLOut)
copyint ((* yyNew)->SizeOf.V1In, yyt->SizeOf.V1In)
copyint ((* yyNew)->SizeOf.V1Out, yyt->SizeOf.V1Out)
copytType ((* yyNew)->SizeOf.SynType, yyt->SizeOf.SynType)
copytType ((* yyNew)->SizeOf.InhType, yyt->SizeOf.InhType)
copyint ((* yyNew)->SizeOf.VIn, yyt->SizeOf.VIn)
copyint ((* yyNew)->SizeOf.VOut, yyt->SizeOf.VOut)
copybool ((* yyNew)->SizeOf.C, yyt->SizeOf.C)
copytValue ((* yyNew)->SizeOf.CV, yyt->SizeOf.CV)
copytSymbol ((* yyNew)->SizeOf.Parent, yyt->SizeOf.Parent)
copytParUsage ((* yyNew)->SizeOf.PUIn, yyt->SizeOf.PUIn)
copytParUsage ((* yyNew)->SizeOf.PUOut, yyt->SizeOf.PUOut)
copybool ((* yyNew)->SizeOf.LV, yyt->SizeOf.LV)
yyt = yyt->SizeOf.Type;
yyNew = & (* yyNew)->SizeOf.Type; break;
case kOffsetOf: (* yyNew)->OffsetOf = yyt->OffsetOf;
copytPosition ((* yyNew)->OffsetOf.Pos, yyt->OffsetOf.Pos)
copytType ((* yyNew)->OffsetOf.DefType, yyt->OffsetOf.DefType)
copytType ((* yyNew)->OffsetOf.FLIn, yyt->OffsetOf.FLIn)
copytType ((* yyNew)->OffsetOf.FLOut, yyt->OffsetOf.FLOut)
copyint ((* yyNew)->OffsetOf.V1In, yyt->OffsetOf.V1In)
copyint ((* yyNew)->OffsetOf.V1Out, yyt->OffsetOf.V1Out)
copytType ((* yyNew)->OffsetOf.SynType, yyt->OffsetOf.SynType)
copytType ((* yyNew)->OffsetOf.InhType, yyt->OffsetOf.InhType)
copyint ((* yyNew)->OffsetOf.VIn, yyt->OffsetOf.VIn)
copyint ((* yyNew)->OffsetOf.VOut, yyt->OffsetOf.VOut)
copybool ((* yyNew)->OffsetOf.C, yyt->OffsetOf.C)
copytValue ((* yyNew)->OffsetOf.CV, yyt->OffsetOf.CV)
copytSymbol ((* yyNew)->OffsetOf.Parent, yyt->OffsetOf.Parent)
copytParUsage ((* yyNew)->OffsetOf.PUIn, yyt->OffsetOf.PUIn)
copytParUsage ((* yyNew)->OffsetOf.PUOut, yyt->OffsetOf.PUOut)
copybool ((* yyNew)->OffsetOf.LV, yyt->OffsetOf.LV)
copytTree ((* yyNew)->OffsetOf.Type, yyt->OffsetOf.Type)
yyt = yyt->OffsetOf.UnmappedIdentifier;
yyNew = & (* yyNew)->OffsetOf.UnmappedIdentifier; break;
case kStringElement: (* yyNew)->StringElement = yyt->StringElement;
copytPosition ((* yyNew)->StringElement.Pos, yyt->StringElement.Pos)
copytType ((* yyNew)->StringElement.DefType, yyt->StringElement.DefType)
copytType ((* yyNew)->StringElement.FLIn, yyt->StringElement.FLIn)
copytType ((* yyNew)->StringElement.FLOut, yyt->StringElement.FLOut)
copyint ((* yyNew)->StringElement.V1In, yyt->StringElement.V1In)
copyint ((* yyNew)->StringElement.V1Out, yyt->StringElement.V1Out)
copytType ((* yyNew)->StringElement.SynType, yyt->StringElement.SynType)
copytType ((* yyNew)->StringElement.InhType, yyt->StringElement.InhType)
copyint ((* yyNew)->StringElement.VIn, yyt->StringElement.VIn)
copyint ((* yyNew)->StringElement.VOut, yyt->StringElement.VOut)
copybool ((* yyNew)->StringElement.C, yyt->StringElement.C)
copytValue ((* yyNew)->StringElement.CV, yyt->StringElement.CV)
copytSymbol ((* yyNew)->StringElement.Parent, yyt->StringElement.Parent)
copytParUsage ((* yyNew)->StringElement.PUIn, yyt->StringElement.PUIn)
copytParUsage ((* yyNew)->StringElement.PUOut, yyt->StringElement.PUOut)
copybool ((* yyNew)->StringElement.LV, yyt->StringElement.LV)
copytConstArray ((* yyNew)->StringElement.CA, yyt->StringElement.CA)
copybool ((* yyNew)->StringElement.SE, yyt->StringElement.SE)
return;
case kString: (* yyNew)->String = yyt->String;
copytPosition ((* yyNew)->String.Pos, yyt->String.Pos)
copytType ((* yyNew)->String.DefType, yyt->String.DefType)
copytType ((* yyNew)->String.FLIn, yyt->String.FLIn)
copytType ((* yyNew)->String.FLOut, yyt->String.FLOut)
copyint ((* yyNew)->String.V1In, yyt->String.V1In)
copyint ((* yyNew)->String.V1Out, yyt->String.V1Out)
copytType ((* yyNew)->String.SynType, yyt->String.SynType)
copytType ((* yyNew)->String.InhType, yyt->String.InhType)
copyint ((* yyNew)->String.VIn, yyt->String.VIn)
copyint ((* yyNew)->String.VOut, yyt->String.VOut)
copybool ((* yyNew)->String.C, yyt->String.C)
copytValue ((* yyNew)->String.CV, yyt->String.CV)
copytSymbol ((* yyNew)->String.Parent, yyt->String.Parent)
copytParUsage ((* yyNew)->String.PUIn, yyt->String.PUIn)
copytParUsage ((* yyNew)->String.PUOut, yyt->String.PUOut)
copybool ((* yyNew)->String.LV, yyt->String.LV)
copytConstArray ((* yyNew)->String.CA, yyt->String.CA)
copybool ((* yyNew)->String.SE, yyt->String.SE)
copytStringRef ((* yyNew)->String.Value, yyt->String.Value)
copyint ((* yyNew)->String.Depth, yyt->String.Depth)
copytSymbol ((* yyNew)->String.Sym, yyt->String.Sym)
copypLinExpr ((* yyNew)->String.LE, yyt->String.LE)
yyt = yyt->String.Type;
yyNew = & (* yyNew)->String.Type; break;
case kStringSubscript: (* yyNew)->StringSubscript = yyt->StringSubscript;
copytPosition ((* yyNew)->StringSubscript.Pos, yyt->StringSubscript.Pos)
copytType ((* yyNew)->StringSubscript.DefType, yyt->StringSubscript.DefType)
copytType ((* yyNew)->StringSubscript.FLIn, yyt->StringSubscript.FLIn)
copytType ((* yyNew)->StringSubscript.FLOut, yyt->StringSubscript.FLOut)
copyint ((* yyNew)->StringSubscript.V1In, yyt->StringSubscript.V1In)
copyint ((* yyNew)->StringSubscript.V1Out, yyt->StringSubscript.V1Out)
copytType ((* yyNew)->StringSubscript.SynType, yyt->StringSubscript.SynType)
copytType ((* yyNew)->StringSubscript.InhType, yyt->StringSubscript.InhType)
copyint ((* yyNew)->StringSubscript.VIn, yyt->StringSubscript.VIn)
copyint ((* yyNew)->StringSubscript.VOut, yyt->StringSubscript.VOut)
copybool ((* yyNew)->StringSubscript.C, yyt->StringSubscript.C)
copytValue ((* yyNew)->StringSubscript.CV, yyt->StringSubscript.CV)
copytSymbol ((* yyNew)->StringSubscript.Parent, yyt->StringSubscript.Parent)
copytParUsage ((* yyNew)->StringSubscript.PUIn, yyt->StringSubscript.PUIn)
copytParUsage ((* yyNew)->StringSubscript.PUOut, yyt->StringSubscript.PUOut)
copybool ((* yyNew)->StringSubscript.LV, yyt->StringSubscript.LV)
copytConstArray ((* yyNew)->StringSubscript.CA, yyt->StringSubscript.CA)
copybool ((* yyNew)->StringSubscript.SE, yyt->StringSubscript.SE)
copytTree ((* yyNew)->StringSubscript.Next, yyt->StringSubscript.Next)
copypLinExpr ((* yyNew)->StringSubscript.LE, yyt->StringSubscript.LE)
yyt = yyt->StringSubscript.Expr;
yyNew = & (* yyNew)->StringSubscript.Expr; break;
case kStringRange: (* yyNew)->StringRange = yyt->StringRange;
copytPosition ((* yyNew)->StringRange.Pos, yyt->StringRange.Pos)
copytType ((* yyNew)->StringRange.DefType, yyt->StringRange.DefType)
copytType ((* yyNew)->StringRange.FLIn, yyt->StringRange.FLIn)
copytType ((* yyNew)->StringRange.FLOut, yyt->StringRange.FLOut)
copyint ((* yyNew)->StringRange.V1In, yyt->StringRange.V1In)
copyint ((* yyNew)->StringRange.V1Out, yyt->StringRange.V1Out)
copytType ((* yyNew)->StringRange.SynType, yyt->StringRange.SynType)
copytType ((* yyNew)->StringRange.InhType, yyt->StringRange.InhType)
copyint ((* yyNew)->StringRange.VIn, yyt->StringRange.VIn)
copyint ((* yyNew)->StringRange.VOut, yyt->StringRange.VOut)
copybool ((* yyNew)->StringRange.C, yyt->StringRange.C)
copytValue ((* yyNew)->StringRange.CV, yyt->StringRange.CV)
copytSymbol ((* yyNew)->StringRange.Parent, yyt->StringRange.Parent)
copytParUsage ((* yyNew)->StringRange.PUIn, yyt->StringRange.PUIn)
copytParUsage ((* yyNew)->StringRange.PUOut, yyt->StringRange.PUOut)
copybool ((* yyNew)->StringRange.LV, yyt->StringRange.LV)
copytConstArray ((* yyNew)->StringRange.CA, yyt->StringRange.CA)
copybool ((* yyNew)->StringRange.SE, yyt->StringRange.SE)
copytTree ((* yyNew)->StringRange.Next, yyt->StringRange.Next)
copytTree ((* yyNew)->StringRange.From, yyt->StringRange.From)
copypLinExpr ((* yyNew)->StringRange.FromLE, yyt->StringRange.FromLE)
copypLinExpr ((* yyNew)->StringRange.ForLE, yyt->StringRange.ForLE)
yyt = yyt->StringRange.For;
yyNew = & (* yyNew)->StringRange.For; break;
case kElemental: (* yyNew)->Elemental = yyt->Elemental;
copytPosition ((* yyNew)->Elemental.Pos, yyt->Elemental.Pos)
copytType ((* yyNew)->Elemental.DefType, yyt->Elemental.DefType)
copytType ((* yyNew)->Elemental.FLIn, yyt->Elemental.FLIn)
copytType ((* yyNew)->Elemental.FLOut, yyt->Elemental.FLOut)
copyint ((* yyNew)->Elemental.V1In, yyt->Elemental.V1In)
copyint ((* yyNew)->Elemental.V1Out, yyt->Elemental.V1Out)
copytType ((* yyNew)->Elemental.SynType, yyt->Elemental.SynType)
copytType ((* yyNew)->Elemental.InhType, yyt->Elemental.InhType)
copyint ((* yyNew)->Elemental.W, yyt->Elemental.W)
copyint ((* yyNew)->Elemental.VIn, yyt->Elemental.VIn)
copyint ((* yyNew)->Elemental.VOut, yyt->Elemental.VOut)
copybool ((* yyNew)->Elemental.C, yyt->Elemental.C)
copytValue ((* yyNew)->Elemental.CV, yyt->Elemental.CV)
copytConstArray ((* yyNew)->Elemental.CA, yyt->Elemental.CA)
copytUsage ((* yyNew)->Elemental.U, yyt->Elemental.U)
copytSymbol ((* yyNew)->Elemental.Parent, yyt->Elemental.Parent)
copytParUsage ((* yyNew)->Elemental.PUIn, yyt->Elemental.PUIn)
copytParUsage ((* yyNew)->Elemental.PUOut, yyt->Elemental.PUOut)
copybool ((* yyNew)->Elemental.LV, yyt->Elemental.LV)
copybool ((* yyNew)->Elemental.SE, yyt->Elemental.SE)
copytTree ((* yyNew)->Elemental.RVL, yyt->Elemental.RVL)
return;
case kTableElement: (* yyNew)->TableElement = yyt->TableElement;
copytPosition ((* yyNew)->TableElement.Pos, yyt->TableElement.Pos)
copytType ((* yyNew)->TableElement.DefType, yyt->TableElement.DefType)
copytType ((* yyNew)->TableElement.FLIn, yyt->TableElement.FLIn)
copytType ((* yyNew)->TableElement.FLOut, yyt->TableElement.FLOut)
copyint ((* yyNew)->TableElement.V1In, yyt->TableElement.V1In)
copyint ((* yyNew)->TableElement.V1Out, yyt->TableElement.V1Out)
copytType ((* yyNew)->TableElement.SynType, yyt->TableElement.SynType)
copytType ((* yyNew)->TableElement.InhType, yyt->TableElement.InhType)
copyint ((* yyNew)->TableElement.W, yyt->TableElement.W)
copyint ((* yyNew)->TableElement.VIn, yyt->TableElement.VIn)
copyint ((* yyNew)->TableElement.VOut, yyt->TableElement.VOut)
copybool ((* yyNew)->TableElement.C, yyt->TableElement.C)
copytValue ((* yyNew)->TableElement.CV, yyt->TableElement.CV)
copytConstArray ((* yyNew)->TableElement.CA, yyt->TableElement.CA)
copytUsage ((* yyNew)->TableElement.U, yyt->TableElement.U)
copytSymbol ((* yyNew)->TableElement.Parent, yyt->TableElement.Parent)
copytParUsage ((* yyNew)->TableElement.PUIn, yyt->TableElement.PUIn)
copytParUsage ((* yyNew)->TableElement.PUOut, yyt->TableElement.PUOut)
copybool ((* yyNew)->TableElement.LV, yyt->TableElement.LV)
copybool ((* yyNew)->TableElement.SE, yyt->TableElement.SE)
copytTree ((* yyNew)->TableElement.RVL, yyt->TableElement.RVL)
return;
case kTable: (* yyNew)->Table = yyt->Table;
copytPosition ((* yyNew)->Table.Pos, yyt->Table.Pos)
copytType ((* yyNew)->Table.DefType, yyt->Table.DefType)
copytType ((* yyNew)->Table.FLIn, yyt->Table.FLIn)
copytType ((* yyNew)->Table.FLOut, yyt->Table.FLOut)
copyint ((* yyNew)->Table.V1In, yyt->Table.V1In)
copyint ((* yyNew)->Table.V1Out, yyt->Table.V1Out)
copytType ((* yyNew)->Table.SynType, yyt->Table.SynType)
copytType ((* yyNew)->Table.InhType, yyt->Table.InhType)
copyint ((* yyNew)->Table.W, yyt->Table.W)
copyint ((* yyNew)->Table.VIn, yyt->Table.VIn)
copyint ((* yyNew)->Table.VOut, yyt->Table.VOut)
copybool ((* yyNew)->Table.C, yyt->Table.C)
copytValue ((* yyNew)->Table.CV, yyt->Table.CV)
copytConstArray ((* yyNew)->Table.CA, yyt->Table.CA)
copytUsage ((* yyNew)->Table.U, yyt->Table.U)
copytSymbol ((* yyNew)->Table.Parent, yyt->Table.Parent)
copytParUsage ((* yyNew)->Table.PUIn, yyt->Table.PUIn)
copytParUsage ((* yyNew)->Table.PUOut, yyt->Table.PUOut)
copybool ((* yyNew)->Table.LV, yyt->Table.LV)
copybool ((* yyNew)->Table.SE, yyt->Table.SE)
copytTree ((* yyNew)->Table.RVL, yyt->Table.RVL)
copyint ((* yyNew)->Table.Depth, yyt->Table.Depth)
copytSymbol ((* yyNew)->Table.Sym, yyt->Table.Sym)
copypLinExpr ((* yyNew)->Table.LE, yyt->Table.LE)
yyt = yyt->Table.ExprList;
yyNew = & (* yyNew)->Table.ExprList; break;
case kUserLiteral: (* yyNew)->UserLiteral = yyt->UserLiteral;
copytPosition ((* yyNew)->UserLiteral.Pos, yyt->UserLiteral.Pos)
copytType ((* yyNew)->UserLiteral.DefType, yyt->UserLiteral.DefType)
copytType ((* yyNew)->UserLiteral.FLIn, yyt->UserLiteral.FLIn)
copytType ((* yyNew)->UserLiteral.FLOut, yyt->UserLiteral.FLOut)
copyint ((* yyNew)->UserLiteral.V1In, yyt->UserLiteral.V1In)
copyint ((* yyNew)->UserLiteral.V1Out, yyt->UserLiteral.V1Out)
copytType ((* yyNew)->UserLiteral.SynType, yyt->UserLiteral.SynType)
copytType ((* yyNew)->UserLiteral.InhType, yyt->UserLiteral.InhType)
copyint ((* yyNew)->UserLiteral.W, yyt->UserLiteral.W)
copyint ((* yyNew)->UserLiteral.VIn, yyt->UserLiteral.VIn)
copyint ((* yyNew)->UserLiteral.VOut, yyt->UserLiteral.VOut)
copybool ((* yyNew)->UserLiteral.C, yyt->UserLiteral.C)
copytValue ((* yyNew)->UserLiteral.CV, yyt->UserLiteral.CV)
copytConstArray ((* yyNew)->UserLiteral.CA, yyt->UserLiteral.CA)
copytUsage ((* yyNew)->UserLiteral.U, yyt->UserLiteral.U)
copytSymbol ((* yyNew)->UserLiteral.Parent, yyt->UserLiteral.Parent)
copytParUsage ((* yyNew)->UserLiteral.PUIn, yyt->UserLiteral.PUIn)
copytParUsage ((* yyNew)->UserLiteral.PUOut, yyt->UserLiteral.PUOut)
copybool ((* yyNew)->UserLiteral.LV, yyt->UserLiteral.LV)
copybool ((* yyNew)->UserLiteral.SE, yyt->UserLiteral.SE)
copytTree ((* yyNew)->UserLiteral.RVL, yyt->UserLiteral.RVL)
copytTree ((* yyNew)->UserLiteral.ExprList, yyt->UserLiteral.ExprList)
copyint ((* yyNew)->UserLiteral.Depth, yyt->UserLiteral.Depth)
copytSymbol ((* yyNew)->UserLiteral.Sym, yyt->UserLiteral.Sym)
copypLinExpr ((* yyNew)->UserLiteral.LE, yyt->UserLiteral.LE)
yyt = yyt->UserLiteral.Type;
yyNew = & (* yyNew)->UserLiteral.Type; break;
case kTableExpr: (* yyNew)->TableExpr = yyt->TableExpr;
copytPosition ((* yyNew)->TableExpr.Pos, yyt->TableExpr.Pos)
copytType ((* yyNew)->TableExpr.DefType, yyt->TableExpr.DefType)
copytType ((* yyNew)->TableExpr.FLIn, yyt->TableExpr.FLIn)
copytType ((* yyNew)->TableExpr.FLOut, yyt->TableExpr.FLOut)
copyint ((* yyNew)->TableExpr.V1In, yyt->TableExpr.V1In)
copyint ((* yyNew)->TableExpr.V1Out, yyt->TableExpr.V1Out)
copytType ((* yyNew)->TableExpr.SynType, yyt->TableExpr.SynType)
copytType ((* yyNew)->TableExpr.InhType, yyt->TableExpr.InhType)
copyint ((* yyNew)->TableExpr.W, yyt->TableExpr.W)
copyint ((* yyNew)->TableExpr.VIn, yyt->TableExpr.VIn)
copyint ((* yyNew)->TableExpr.VOut, yyt->TableExpr.VOut)
copybool ((* yyNew)->TableExpr.C, yyt->TableExpr.C)
copytValue ((* yyNew)->TableExpr.CV, yyt->TableExpr.CV)
copytConstArray ((* yyNew)->TableExpr.CA, yyt->TableExpr.CA)
copytUsage ((* yyNew)->TableExpr.U, yyt->TableExpr.U)
copytSymbol ((* yyNew)->TableExpr.Parent, yyt->TableExpr.Parent)
copytParUsage ((* yyNew)->TableExpr.PUIn, yyt->TableExpr.PUIn)
copytParUsage ((* yyNew)->TableExpr.PUOut, yyt->TableExpr.PUOut)
copybool ((* yyNew)->TableExpr.LV, yyt->TableExpr.LV)
copybool ((* yyNew)->TableExpr.SE, yyt->TableExpr.SE)
copytTree ((* yyNew)->TableExpr.RVL, yyt->TableExpr.RVL)
copytSymbol ((* yyNew)->TableExpr.Sym, yyt->TableExpr.Sym)
yyt = yyt->TableExpr.Expr;
yyNew = & (* yyNew)->TableExpr.Expr; break;
case kTableSubscript: (* yyNew)->TableSubscript = yyt->TableSubscript;
copytPosition ((* yyNew)->TableSubscript.Pos, yyt->TableSubscript.Pos)
copytType ((* yyNew)->TableSubscript.DefType, yyt->TableSubscript.DefType)
copytType ((* yyNew)->TableSubscript.FLIn, yyt->TableSubscript.FLIn)
copytType ((* yyNew)->TableSubscript.FLOut, yyt->TableSubscript.FLOut)
copyint ((* yyNew)->TableSubscript.V1In, yyt->TableSubscript.V1In)
copyint ((* yyNew)->TableSubscript.V1Out, yyt->TableSubscript.V1Out)
copytType ((* yyNew)->TableSubscript.SynType, yyt->TableSubscript.SynType)
copytType ((* yyNew)->TableSubscript.InhType, yyt->TableSubscript.InhType)
copyint ((* yyNew)->TableSubscript.W, yyt->TableSubscript.W)
copyint ((* yyNew)->TableSubscript.VIn, yyt->TableSubscript.VIn)
copyint ((* yyNew)->TableSubscript.VOut, yyt->TableSubscript.VOut)
copybool ((* yyNew)->TableSubscript.C, yyt->TableSubscript.C)
copytValue ((* yyNew)->TableSubscript.CV, yyt->TableSubscript.CV)
copytConstArray ((* yyNew)->TableSubscript.CA, yyt->TableSubscript.CA)
copytUsage ((* yyNew)->TableSubscript.U, yyt->TableSubscript.U)
copytSymbol ((* yyNew)->TableSubscript.Parent, yyt->TableSubscript.Parent)
copytParUsage ((* yyNew)->TableSubscript.PUIn, yyt->TableSubscript.PUIn)
copytParUsage ((* yyNew)->TableSubscript.PUOut, yyt->TableSubscript.PUOut)
copybool ((* yyNew)->TableSubscript.LV, yyt->TableSubscript.LV)
copybool ((* yyNew)->TableSubscript.SE, yyt->TableSubscript.SE)
copytTree ((* yyNew)->TableSubscript.RVL, yyt->TableSubscript.RVL)
copytTree ((* yyNew)->TableSubscript.Next, yyt->TableSubscript.Next)
copypLinExpr ((* yyNew)->TableSubscript.LE, yyt->TableSubscript.LE)
yyt = yyt->TableSubscript.Expr;
yyNew = & (* yyNew)->TableSubscript.Expr; break;
case kTableRange: (* yyNew)->TableRange = yyt->TableRange;
copytPosition ((* yyNew)->TableRange.Pos, yyt->TableRange.Pos)
copytType ((* yyNew)->TableRange.DefType, yyt->TableRange.DefType)
copytType ((* yyNew)->TableRange.FLIn, yyt->TableRange.FLIn)
copytType ((* yyNew)->TableRange.FLOut, yyt->TableRange.FLOut)
copyint ((* yyNew)->TableRange.V1In, yyt->TableRange.V1In)
copyint ((* yyNew)->TableRange.V1Out, yyt->TableRange.V1Out)
copytType ((* yyNew)->TableRange.SynType, yyt->TableRange.SynType)
copytType ((* yyNew)->TableRange.InhType, yyt->TableRange.InhType)
copyint ((* yyNew)->TableRange.W, yyt->TableRange.W)
copyint ((* yyNew)->TableRange.VIn, yyt->TableRange.VIn)
copyint ((* yyNew)->TableRange.VOut, yyt->TableRange.VOut)
copybool ((* yyNew)->TableRange.C, yyt->TableRange.C)
copytValue ((* yyNew)->TableRange.CV, yyt->TableRange.CV)
copytConstArray ((* yyNew)->TableRange.CA, yyt->TableRange.CA)
copytUsage ((* yyNew)->TableRange.U, yyt->TableRange.U)
copytSymbol ((* yyNew)->TableRange.Parent, yyt->TableRange.Parent)
copytParUsage ((* yyNew)->TableRange.PUIn, yyt->TableRange.PUIn)
copytParUsage ((* yyNew)->TableRange.PUOut, yyt->TableRange.PUOut)
copybool ((* yyNew)->TableRange.LV, yyt->TableRange.LV)
copybool ((* yyNew)->TableRange.SE, yyt->TableRange.SE)
copytTree ((* yyNew)->TableRange.RVL, yyt->TableRange.RVL)
copytTree ((* yyNew)->TableRange.Next, yyt->TableRange.Next)
copytTree ((* yyNew)->TableRange.From, yyt->TableRange.From)
copypLinExpr ((* yyNew)->TableRange.FromLE, yyt->TableRange.FromLE)
copypLinExpr ((* yyNew)->TableRange.ForLE, yyt->TableRange.ForLE)
yyt = yyt->TableRange.For;
yyNew = & (* yyNew)->TableRange.For; break;
case kElement: (* yyNew)->Element = yyt->Element;
copytPosition ((* yyNew)->Element.Pos, yyt->Element.Pos)
copytType ((* yyNew)->Element.DefType, yyt->Element.DefType)
copytType ((* yyNew)->Element.FLIn, yyt->Element.FLIn)
copytType ((* yyNew)->Element.FLOut, yyt->Element.FLOut)
copyint ((* yyNew)->Element.V1In, yyt->Element.V1In)
copyint ((* yyNew)->Element.V1Out, yyt->Element.V1Out)
copytType ((* yyNew)->Element.SynType, yyt->Element.SynType)
copytType ((* yyNew)->Element.InhType, yyt->Element.InhType)
copyint ((* yyNew)->Element.W, yyt->Element.W)
copyint ((* yyNew)->Element.VIn, yyt->Element.VIn)
copyint ((* yyNew)->Element.VOut, yyt->Element.VOut)
copybool ((* yyNew)->Element.C, yyt->Element.C)
copytValue ((* yyNew)->Element.CV, yyt->Element.CV)
copytConstArray ((* yyNew)->Element.CA, yyt->Element.CA)
copytUsage ((* yyNew)->Element.U, yyt->Element.U)
copytSymbol ((* yyNew)->Element.Parent, yyt->Element.Parent)
copytParUsage ((* yyNew)->Element.PUIn, yyt->Element.PUIn)
copytParUsage ((* yyNew)->Element.PUOut, yyt->Element.PUOut)
copybool ((* yyNew)->Element.LV, yyt->Element.LV)
copybool ((* yyNew)->Element.SE, yyt->Element.SE)
copytTree ((* yyNew)->Element.RVL, yyt->Element.RVL)
return;
case kIdentifier: (* yyNew)->Identifier = yyt->Identifier;
copytPosition ((* yyNew)->Identifier.Pos, yyt->Identifier.Pos)
copytType ((* yyNew)->Identifier.DefType, yyt->Identifier.DefType)
copytType ((* yyNew)->Identifier.FLIn, yyt->Identifier.FLIn)
copytType ((* yyNew)->Identifier.FLOut, yyt->Identifier.FLOut)
copyint ((* yyNew)->Identifier.V1In, yyt->Identifier.V1In)
copyint ((* yyNew)->Identifier.V1Out, yyt->Identifier.V1Out)
copytType ((* yyNew)->Identifier.SynType, yyt->Identifier.SynType)
copytType ((* yyNew)->Identifier.InhType, yyt->Identifier.InhType)
copyint ((* yyNew)->Identifier.W, yyt->Identifier.W)
copyint ((* yyNew)->Identifier.VIn, yyt->Identifier.VIn)
copyint ((* yyNew)->Identifier.VOut, yyt->Identifier.VOut)
copybool ((* yyNew)->Identifier.C, yyt->Identifier.C)
copytValue ((* yyNew)->Identifier.CV, yyt->Identifier.CV)
copytConstArray ((* yyNew)->Identifier.CA, yyt->Identifier.CA)
copytUsage ((* yyNew)->Identifier.U, yyt->Identifier.U)
copytSymbol ((* yyNew)->Identifier.Parent, yyt->Identifier.Parent)
copytParUsage ((* yyNew)->Identifier.PUIn, yyt->Identifier.PUIn)
copytParUsage ((* yyNew)->Identifier.PUOut, yyt->Identifier.PUOut)
copybool ((* yyNew)->Identifier.LV, yyt->Identifier.LV)
copybool ((* yyNew)->Identifier.SE, yyt->Identifier.SE)
copytTree ((* yyNew)->Identifier.RVL, yyt->Identifier.RVL)
copytSymbol ((* yyNew)->Identifier.Sym, yyt->Identifier.Sym)
copytSymbol ((* yyNew)->Identifier.Scope, yyt->Identifier.Scope)
return;
case kUnmappedIdentifier: (* yyNew)->UnmappedIdentifier = yyt->UnmappedIdentifier;
copytPosition ((* yyNew)->UnmappedIdentifier.Pos, yyt->UnmappedIdentifier.Pos)
copytType ((* yyNew)->UnmappedIdentifier.DefType, yyt->UnmappedIdentifier.DefType)
copytType ((* yyNew)->UnmappedIdentifier.FLIn, yyt->UnmappedIdentifier.FLIn)
copytType ((* yyNew)->UnmappedIdentifier.FLOut, yyt->UnmappedIdentifier.FLOut)
copyint ((* yyNew)->UnmappedIdentifier.V1In, yyt->UnmappedIdentifier.V1In)
copyint ((* yyNew)->UnmappedIdentifier.V1Out, yyt->UnmappedIdentifier.V1Out)
copytType ((* yyNew)->UnmappedIdentifier.SynType, yyt->UnmappedIdentifier.SynType)
copytType ((* yyNew)->UnmappedIdentifier.InhType, yyt->UnmappedIdentifier.InhType)
copyint ((* yyNew)->UnmappedIdentifier.W, yyt->UnmappedIdentifier.W)
copyint ((* yyNew)->UnmappedIdentifier.VIn, yyt->UnmappedIdentifier.VIn)
copyint ((* yyNew)->UnmappedIdentifier.VOut, yyt->UnmappedIdentifier.VOut)
copybool ((* yyNew)->UnmappedIdentifier.C, yyt->UnmappedIdentifier.C)
copytValue ((* yyNew)->UnmappedIdentifier.CV, yyt->UnmappedIdentifier.CV)
copytConstArray ((* yyNew)->UnmappedIdentifier.CA, yyt->UnmappedIdentifier.CA)
copytUsage ((* yyNew)->UnmappedIdentifier.U, yyt->UnmappedIdentifier.U)
copytSymbol ((* yyNew)->UnmappedIdentifier.Parent, yyt->UnmappedIdentifier.Parent)
copytParUsage ((* yyNew)->UnmappedIdentifier.PUIn, yyt->UnmappedIdentifier.PUIn)
copytParUsage ((* yyNew)->UnmappedIdentifier.PUOut, yyt->UnmappedIdentifier.PUOut)
copybool ((* yyNew)->UnmappedIdentifier.LV, yyt->UnmappedIdentifier.LV)
copybool ((* yyNew)->UnmappedIdentifier.SE, yyt->UnmappedIdentifier.SE)
copytTree ((* yyNew)->UnmappedIdentifier.RVL, yyt->UnmappedIdentifier.RVL)
copytSymbol ((* yyNew)->UnmappedIdentifier.Sym, yyt->UnmappedIdentifier.Sym)
copytSymbol ((* yyNew)->UnmappedIdentifier.Scope, yyt->UnmappedIdentifier.Scope)
copytIdent ((* yyNew)->UnmappedIdentifier.Ident, yyt->UnmappedIdentifier.Ident)
return;
case kElementAccess: (* yyNew)->ElementAccess = yyt->ElementAccess;
copytPosition ((* yyNew)->ElementAccess.Pos, yyt->ElementAccess.Pos)
copytType ((* yyNew)->ElementAccess.DefType, yyt->ElementAccess.DefType)
copytType ((* yyNew)->ElementAccess.FLIn, yyt->ElementAccess.FLIn)
copytType ((* yyNew)->ElementAccess.FLOut, yyt->ElementAccess.FLOut)
copyint ((* yyNew)->ElementAccess.V1In, yyt->ElementAccess.V1In)
copyint ((* yyNew)->ElementAccess.V1Out, yyt->ElementAccess.V1Out)
copytType ((* yyNew)->ElementAccess.SynType, yyt->ElementAccess.SynType)
copytType ((* yyNew)->ElementAccess.InhType, yyt->ElementAccess.InhType)
copyint ((* yyNew)->ElementAccess.W, yyt->ElementAccess.W)
copyint ((* yyNew)->ElementAccess.VIn, yyt->ElementAccess.VIn)
copyint ((* yyNew)->ElementAccess.VOut, yyt->ElementAccess.VOut)
copybool ((* yyNew)->ElementAccess.C, yyt->ElementAccess.C)
copytValue ((* yyNew)->ElementAccess.CV, yyt->ElementAccess.CV)
copytConstArray ((* yyNew)->ElementAccess.CA, yyt->ElementAccess.CA)
copytUsage ((* yyNew)->ElementAccess.U, yyt->ElementAccess.U)
copytSymbol ((* yyNew)->ElementAccess.Parent, yyt->ElementAccess.Parent)
copytParUsage ((* yyNew)->ElementAccess.PUIn, yyt->ElementAccess.PUIn)
copytParUsage ((* yyNew)->ElementAccess.PUOut, yyt->ElementAccess.PUOut)
copybool ((* yyNew)->ElementAccess.LV, yyt->ElementAccess.LV)
copybool ((* yyNew)->ElementAccess.SE, yyt->ElementAccess.SE)
copytTree ((* yyNew)->ElementAccess.RVL, yyt->ElementAccess.RVL)
yyt = yyt->ElementAccess.Next;
yyNew = & (* yyNew)->ElementAccess.Next; break;
case kSubscript: (* yyNew)->Subscript = yyt->Subscript;
copytPosition ((* yyNew)->Subscript.Pos, yyt->Subscript.Pos)
copytType ((* yyNew)->Subscript.DefType, yyt->Subscript.DefType)
copytType ((* yyNew)->Subscript.FLIn, yyt->Subscript.FLIn)
copytType ((* yyNew)->Subscript.FLOut, yyt->Subscript.FLOut)
copyint ((* yyNew)->Subscript.V1In, yyt->Subscript.V1In)
copyint ((* yyNew)->Subscript.V1Out, yyt->Subscript.V1Out)
copytType ((* yyNew)->Subscript.SynType, yyt->Subscript.SynType)
copytType ((* yyNew)->Subscript.InhType, yyt->Subscript.InhType)
copyint ((* yyNew)->Subscript.W, yyt->Subscript.W)
copyint ((* yyNew)->Subscript.VIn, yyt->Subscript.VIn)
copyint ((* yyNew)->Subscript.VOut, yyt->Subscript.VOut)
copybool ((* yyNew)->Subscript.C, yyt->Subscript.C)
copytValue ((* yyNew)->Subscript.CV, yyt->Subscript.CV)
copytConstArray ((* yyNew)->Subscript.CA, yyt->Subscript.CA)
copytUsage ((* yyNew)->Subscript.U, yyt->Subscript.U)
copytSymbol ((* yyNew)->Subscript.Parent, yyt->Subscript.Parent)
copytParUsage ((* yyNew)->Subscript.PUIn, yyt->Subscript.PUIn)
copytParUsage ((* yyNew)->Subscript.PUOut, yyt->Subscript.PUOut)
copybool ((* yyNew)->Subscript.LV, yyt->Subscript.LV)
copybool ((* yyNew)->Subscript.SE, yyt->Subscript.SE)
copytTree ((* yyNew)->Subscript.RVL, yyt->Subscript.RVL)
copytTree ((* yyNew)->Subscript.Next, yyt->Subscript.Next)
copybool ((* yyNew)->Subscript.CB, yyt->Subscript.CB)
copypLinExpr ((* yyNew)->Subscript.LE, yyt->Subscript.LE)
yyt = yyt->Subscript.Expr;
yyNew = & (* yyNew)->Subscript.Expr; break;
case kRange: (* yyNew)->Range = yyt->Range;
copytPosition ((* yyNew)->Range.Pos, yyt->Range.Pos)
copytType ((* yyNew)->Range.DefType, yyt->Range.DefType)
copytType ((* yyNew)->Range.FLIn, yyt->Range.FLIn)
copytType ((* yyNew)->Range.FLOut, yyt->Range.FLOut)
copyint ((* yyNew)->Range.V1In, yyt->Range.V1In)
copyint ((* yyNew)->Range.V1Out, yyt->Range.V1Out)
copytType ((* yyNew)->Range.SynType, yyt->Range.SynType)
copytType ((* yyNew)->Range.InhType, yyt->Range.InhType)
copyint ((* yyNew)->Range.W, yyt->Range.W)
copyint ((* yyNew)->Range.VIn, yyt->Range.VIn)
copyint ((* yyNew)->Range.VOut, yyt->Range.VOut)
copybool ((* yyNew)->Range.C, yyt->Range.C)
copytValue ((* yyNew)->Range.CV, yyt->Range.CV)
copytConstArray ((* yyNew)->Range.CA, yyt->Range.CA)
copytUsage ((* yyNew)->Range.U, yyt->Range.U)
copytSymbol ((* yyNew)->Range.Parent, yyt->Range.Parent)
copytParUsage ((* yyNew)->Range.PUIn, yyt->Range.PUIn)
copytParUsage ((* yyNew)->Range.PUOut, yyt->Range.PUOut)
copybool ((* yyNew)->Range.LV, yyt->Range.LV)
copybool ((* yyNew)->Range.SE, yyt->Range.SE)
copytTree ((* yyNew)->Range.RVL, yyt->Range.RVL)
copytTree ((* yyNew)->Range.Next, yyt->Range.Next)
copytTree ((* yyNew)->Range.From, yyt->Range.From)
copybool ((* yyNew)->Range.CLB, yyt->Range.CLB)
copybool ((* yyNew)->Range.CUB, yyt->Range.CUB)
copypLinExpr ((* yyNew)->Range.FromLE, yyt->Range.FromLE)
copypLinExpr ((* yyNew)->Range.ForLE, yyt->Range.ForLE)
yyt = yyt->Range.For;
yyNew = & (* yyNew)->Range.For; break;
case kExprList: (* yyNew)->ExprList = yyt->ExprList;
copytType ((* yyNew)->ExprList.DefType, yyt->ExprList.DefType)
copytType ((* yyNew)->ExprList.FLIn, yyt->ExprList.FLIn)
copytType ((* yyNew)->ExprList.FLOut, yyt->ExprList.FLOut)
copyint ((* yyNew)->ExprList.V1In, yyt->ExprList.V1In)
copyint ((* yyNew)->ExprList.V1Out, yyt->ExprList.V1Out)
copytType ((* yyNew)->ExprList.SynType, yyt->ExprList.SynType)
copytType ((* yyNew)->ExprList.InhType, yyt->ExprList.InhType)
copyint ((* yyNew)->ExprList.VIn, yyt->ExprList.VIn)
copyint ((* yyNew)->ExprList.VOut, yyt->ExprList.VOut)
copybool ((* yyNew)->ExprList.C, yyt->ExprList.C)
copybool ((* yyNew)->ExprList.TR, yyt->ExprList.TR)
copytSymbol ((* yyNew)->ExprList.Parent, yyt->ExprList.Parent)
copytParUsage ((* yyNew)->ExprList.PUIn, yyt->ExprList.PUIn)
copytParUsage ((* yyNew)->ExprList.PUOut, yyt->ExprList.PUOut)
copybool ((* yyNew)->ExprList.PAR, yyt->ExprList.PAR)
copybool ((* yyNew)->ExprList.LV, yyt->ExprList.LV)
return;
case kNoExpr: (* yyNew)->NoExpr = yyt->NoExpr;
copytType ((* yyNew)->NoExpr.DefType, yyt->NoExpr.DefType)
copytType ((* yyNew)->NoExpr.FLIn, yyt->NoExpr.FLIn)
copytType ((* yyNew)->NoExpr.FLOut, yyt->NoExpr.FLOut)
copyint ((* yyNew)->NoExpr.V1In, yyt->NoExpr.V1In)
copyint ((* yyNew)->NoExpr.V1Out, yyt->NoExpr.V1Out)
copytType ((* yyNew)->NoExpr.SynType, yyt->NoExpr.SynType)
copytType ((* yyNew)->NoExpr.InhType, yyt->NoExpr.InhType)
copyint ((* yyNew)->NoExpr.VIn, yyt->NoExpr.VIn)
copyint ((* yyNew)->NoExpr.VOut, yyt->NoExpr.VOut)
copybool ((* yyNew)->NoExpr.C, yyt->NoExpr.C)
copybool ((* yyNew)->NoExpr.TR, yyt->NoExpr.TR)
copytSymbol ((* yyNew)->NoExpr.Parent, yyt->NoExpr.Parent)
copytParUsage ((* yyNew)->NoExpr.PUIn, yyt->NoExpr.PUIn)
copytParUsage ((* yyNew)->NoExpr.PUOut, yyt->NoExpr.PUOut)
copybool ((* yyNew)->NoExpr.PAR, yyt->NoExpr.PAR)
copybool ((* yyNew)->NoExpr.LV, yyt->NoExpr.LV)
return;
case kAnExpr: (* yyNew)->AnExpr = yyt->AnExpr;
copytType ((* yyNew)->AnExpr.DefType, yyt->AnExpr.DefType)
copytType ((* yyNew)->AnExpr.FLIn, yyt->AnExpr.FLIn)
copytType ((* yyNew)->AnExpr.FLOut, yyt->AnExpr.FLOut)
copyint ((* yyNew)->AnExpr.V1In, yyt->AnExpr.V1In)
copyint ((* yyNew)->AnExpr.V1Out, yyt->AnExpr.V1Out)
copytType ((* yyNew)->AnExpr.SynType, yyt->AnExpr.SynType)
copytType ((* yyNew)->AnExpr.InhType, yyt->AnExpr.InhType)
copyint ((* yyNew)->AnExpr.VIn, yyt->AnExpr.VIn)
copyint ((* yyNew)->AnExpr.VOut, yyt->AnExpr.VOut)
copybool ((* yyNew)->AnExpr.C, yyt->AnExpr.C)
copybool ((* yyNew)->AnExpr.TR, yyt->AnExpr.TR)
copytSymbol ((* yyNew)->AnExpr.Parent, yyt->AnExpr.Parent)
copytParUsage ((* yyNew)->AnExpr.PUIn, yyt->AnExpr.PUIn)
copytParUsage ((* yyNew)->AnExpr.PUOut, yyt->AnExpr.PUOut)
copybool ((* yyNew)->AnExpr.PAR, yyt->AnExpr.PAR)
copybool ((* yyNew)->AnExpr.LV, yyt->AnExpr.LV)
copytTree ((* yyNew)->AnExpr.Expr, yyt->AnExpr.Expr)
yyt = yyt->AnExpr.Next;
yyNew = & (* yyNew)->AnExpr.Next; break;
case kOp: (* yyNew)->Op = yyt->Op;
return;
case kRelOp: (* yyNew)->RelOp = yyt->RelOp;
return;
case kLess: (* yyNew)->Less = yyt->Less;
return;
case kGreater: (* yyNew)->Greater = yyt->Greater;
return;
case kLessEqual: (* yyNew)->LessEqual = yyt->LessEqual;
return;
case kGreaterEqual: (* yyNew)->GreaterEqual = yyt->GreaterEqual;
return;
case kEqual: (* yyNew)->Equal = yyt->Equal;
return;
case kNotEqual: (* yyNew)->NotEqual = yyt->NotEqual;
return;
case kMathsOp: (* yyNew)->MathsOp = yyt->MathsOp;
return;
case kAdd: (* yyNew)->Add = yyt->Add;
return;
case kSubtract: (* yyNew)->Subtract = yyt->Subtract;
return;
case kMultiply: (* yyNew)->Multiply = yyt->Multiply;
return;
case kDivide: (* yyNew)->Divide = yyt->Divide;
return;
case kRem: (* yyNew)->Rem = yyt->Rem;
return;
case kBitOp: (* yyNew)->BitOp = yyt->BitOp;
return;
case kBitAnd: (* yyNew)->BitAnd = yyt->BitAnd;
return;
case kBitOr: (* yyNew)->BitOr = yyt->BitOr;
return;
case kBitEor: (* yyNew)->BitEor = yyt->BitEor;
return;
case kShiftOp: (* yyNew)->ShiftOp = yyt->ShiftOp;
return;
case kLeftShift: (* yyNew)->LeftShift = yyt->LeftShift;
return;
case kRightShift: (* yyNew)->RightShift = yyt->RightShift;
return;
case kUniOp: (* yyNew)->UniOp = yyt->UniOp;
return;
case kSize: (* yyNew)->Size = yyt->Size;
return;
case kBitNot: (* yyNew)->BitNot = yyt->BitNot;
return;
case kNot: (* yyNew)->Not = yyt->Not;
return;
case kClockOp: (* yyNew)->ClockOp = yyt->ClockOp;
return;
case kPlus: (* yyNew)->Plus = yyt->Plus;
return;
case kMinus: (* yyNew)->Minus = yyt->Minus;
return;
case kTimes: (* yyNew)->Times = yyt->Times;
return;
case kLogOp: (* yyNew)->LogOp = yyt->LogOp;
return;
case kAnd: (* yyNew)->And = yyt->And;
return;
case kOr: (* yyNew)->Or = yyt->Or;
return;
case kAfterOp: (* yyNew)->AfterOp = yyt->AfterOp;
return;
case kProcess: (* yyNew)->Process = yyt->Process;
copytPosition ((* yyNew)->Process.Pos, yyt->Process.Pos)
copyint ((* yyNew)->Process.V1In, yyt->Process.V1In)
copyint ((* yyNew)->Process.V1Out, yyt->Process.V1Out)
copyint ((* yyNew)->Process.VS, yyt->Process.VS)
copyint ((* yyNew)->Process.VIn, yyt->Process.VIn)
copyint ((* yyNew)->Process.VOut, yyt->Process.VOut)
copybool ((* yyNew)->Process.ATOMIC, yyt->Process.ATOMIC)
copyint ((* yyNew)->Process.SLC, yyt->Process.SLC)
copybool ((* yyNew)->Process.TLA, yyt->Process.TLA)
copytSymbol ((* yyNew)->Process.Parent, yyt->Process.Parent)
copytParUsage ((* yyNew)->Process.PUIn, yyt->Process.PUIn)
copytParUsage ((* yyNew)->Process.PUOut, yyt->Process.PUOut)
yyt = yyt->Process.SpecList;
yyNew = & (* yyNew)->Process.SpecList; break;
case kAssign: (* yyNew)->Assign = yyt->Assign;
copytPosition ((* yyNew)->Assign.Pos, yyt->Assign.Pos)
copytTree ((* yyNew)->Assign.SpecList, yyt->Assign.SpecList)
copyint ((* yyNew)->Assign.V1In, yyt->Assign.V1In)
copyint ((* yyNew)->Assign.V1Out, yyt->Assign.V1Out)
copyint ((* yyNew)->Assign.VS, yyt->Assign.VS)
copyint ((* yyNew)->Assign.VIn, yyt->Assign.VIn)
copyint ((* yyNew)->Assign.VOut, yyt->Assign.VOut)
copybool ((* yyNew)->Assign.ATOMIC, yyt->Assign.ATOMIC)
copyint ((* yyNew)->Assign.SLC, yyt->Assign.SLC)
copybool ((* yyNew)->Assign.TLA, yyt->Assign.TLA)
copytSymbol ((* yyNew)->Assign.Parent, yyt->Assign.Parent)
copytParUsage ((* yyNew)->Assign.PUIn, yyt->Assign.PUIn)
copytParUsage ((* yyNew)->Assign.PUOut, yyt->Assign.PUOut)
copytTree ((* yyNew)->Assign.Lval, yyt->Assign.Lval)
yyt = yyt->Assign.Rval;
yyNew = & (* yyNew)->Assign.Rval; break;
case kAltType: (* yyNew)->AltType = yyt->AltType;
copytPosition ((* yyNew)->AltType.Pos, yyt->AltType.Pos)
copyint ((* yyNew)->AltType.V1In, yyt->AltType.V1In)
copyint ((* yyNew)->AltType.V1Out, yyt->AltType.V1Out)
copyint ((* yyNew)->AltType.VS, yyt->AltType.VS)
copyint ((* yyNew)->AltType.VIn, yyt->AltType.VIn)
copyint ((* yyNew)->AltType.VOut, yyt->AltType.VOut)
copybool ((* yyNew)->AltType.ATOMIC, yyt->AltType.ATOMIC)
copyint ((* yyNew)->AltType.SLC, yyt->AltType.SLC)
copybool ((* yyNew)->AltType.TLA, yyt->AltType.TLA)
copytSymbol ((* yyNew)->AltType.Parent, yyt->AltType.Parent)
copytParUsage ((* yyNew)->AltType.PUIn, yyt->AltType.PUIn)
copytParUsage ((* yyNew)->AltType.PUOut, yyt->AltType.PUOut)
copybool ((* yyNew)->AltType.Pri, yyt->AltType.Pri)
copyint ((* yyNew)->AltType.ARC, yyt->AltType.ARC)
yyt = yyt->AltType.SpecList;
yyNew = & (* yyNew)->AltType.SpecList; break;
case kAlt: (* yyNew)->Alt = yyt->Alt;
copytPosition ((* yyNew)->Alt.Pos, yyt->Alt.Pos)
copytTree ((* yyNew)->Alt.SpecList, yyt->Alt.SpecList)
copyint ((* yyNew)->Alt.V1In, yyt->Alt.V1In)
copyint ((* yyNew)->Alt.V1Out, yyt->Alt.V1Out)
copyint ((* yyNew)->Alt.VS, yyt->Alt.VS)
copyint ((* yyNew)->Alt.VIn, yyt->Alt.VIn)
copyint ((* yyNew)->Alt.VOut, yyt->Alt.VOut)
copybool ((* yyNew)->Alt.ATOMIC, yyt->Alt.ATOMIC)
copyint ((* yyNew)->Alt.SLC, yyt->Alt.SLC)
copybool ((* yyNew)->Alt.TLA, yyt->Alt.TLA)
copytSymbol ((* yyNew)->Alt.Parent, yyt->Alt.Parent)
copytParUsage ((* yyNew)->Alt.PUIn, yyt->Alt.PUIn)
copytParUsage ((* yyNew)->Alt.PUOut, yyt->Alt.PUOut)
copybool ((* yyNew)->Alt.Pri, yyt->Alt.Pri)
copyint ((* yyNew)->Alt.ARC, yyt->Alt.ARC)
yyt = yyt->Alt.AltList;
yyNew = & (* yyNew)->Alt.AltList; break;
case kAltRep: (* yyNew)->AltRep = yyt->AltRep;
copytPosition ((* yyNew)->AltRep.Pos, yyt->AltRep.Pos)
copytTree ((* yyNew)->AltRep.SpecList, yyt->AltRep.SpecList)
copyint ((* yyNew)->AltRep.V1In, yyt->AltRep.V1In)
copyint ((* yyNew)->AltRep.V1Out, yyt->AltRep.V1Out)
copyint ((* yyNew)->AltRep.VS, yyt->AltRep.VS)
copyint ((* yyNew)->AltRep.VIn, yyt->AltRep.VIn)
copyint ((* yyNew)->AltRep.VOut, yyt->AltRep.VOut)
copybool ((* yyNew)->AltRep.ATOMIC, yyt->AltRep.ATOMIC)
copyint ((* yyNew)->AltRep.SLC, yyt->AltRep.SLC)
copybool ((* yyNew)->AltRep.TLA, yyt->AltRep.TLA)
copytSymbol ((* yyNew)->AltRep.Parent, yyt->AltRep.Parent)
copytParUsage ((* yyNew)->AltRep.PUIn, yyt->AltRep.PUIn)
copytParUsage ((* yyNew)->AltRep.PUOut, yyt->AltRep.PUOut)
copybool ((* yyNew)->AltRep.Pri, yyt->AltRep.Pri)
copyint ((* yyNew)->AltRep.ARC, yyt->AltRep.ARC)
copytTree ((* yyNew)->AltRep.Replicator, yyt->AltRep.Replicator)
yyt = yyt->AltRep.AltList;
yyNew = & (* yyNew)->AltRep.AltList; break;
case kParType: (* yyNew)->ParType = yyt->ParType;
copytPosition ((* yyNew)->ParType.Pos, yyt->ParType.Pos)
copyint ((* yyNew)->ParType.V1In, yyt->ParType.V1In)
copyint ((* yyNew)->ParType.V1Out, yyt->ParType.V1Out)
copyint ((* yyNew)->ParType.VS, yyt->ParType.VS)
copyint ((* yyNew)->ParType.VIn, yyt->ParType.VIn)
copyint ((* yyNew)->ParType.VOut, yyt->ParType.VOut)
copybool ((* yyNew)->ParType.ATOMIC, yyt->ParType.ATOMIC)
copyint ((* yyNew)->ParType.SLC, yyt->ParType.SLC)
copybool ((* yyNew)->ParType.TLA, yyt->ParType.TLA)
copytSymbol ((* yyNew)->ParType.Parent, yyt->ParType.Parent)
copytParUsage ((* yyNew)->ParType.PUIn, yyt->ParType.PUIn)
copytParUsage ((* yyNew)->ParType.PUOut, yyt->ParType.PUOut)
copytParType ((* yyNew)->ParType.PType, yyt->ParType.PType)
yyt = yyt->ParType.SpecList;
yyNew = & (* yyNew)->ParType.SpecList; break;
case kPar: (* yyNew)->Par = yyt->Par;
copytPosition ((* yyNew)->Par.Pos, yyt->Par.Pos)
copytTree ((* yyNew)->Par.SpecList, yyt->Par.SpecList)
copyint ((* yyNew)->Par.V1In, yyt->Par.V1In)
copyint ((* yyNew)->Par.V1Out, yyt->Par.V1Out)
copyint ((* yyNew)->Par.VS, yyt->Par.VS)
copyint ((* yyNew)->Par.VIn, yyt->Par.VIn)
copyint ((* yyNew)->Par.VOut, yyt->Par.VOut)
copybool ((* yyNew)->Par.ATOMIC, yyt->Par.ATOMIC)
copyint ((* yyNew)->Par.SLC, yyt->Par.SLC)
copybool ((* yyNew)->Par.TLA, yyt->Par.TLA)
copytSymbol ((* yyNew)->Par.Parent, yyt->Par.Parent)
copytParUsage ((* yyNew)->Par.PUIn, yyt->Par.PUIn)
copytParUsage ((* yyNew)->Par.PUOut, yyt->Par.PUOut)
copytParType ((* yyNew)->Par.PType, yyt->Par.PType)
yyt = yyt->Par.ProcList;
yyNew = & (* yyNew)->Par.ProcList; break;
case kParRep: (* yyNew)->ParRep = yyt->ParRep;
copytPosition ((* yyNew)->ParRep.Pos, yyt->ParRep.Pos)
copytTree ((* yyNew)->ParRep.SpecList, yyt->ParRep.SpecList)
copyint ((* yyNew)->ParRep.V1In, yyt->ParRep.V1In)
copyint ((* yyNew)->ParRep.V1Out, yyt->ParRep.V1Out)
copyint ((* yyNew)->ParRep.VS, yyt->ParRep.VS)
copyint ((* yyNew)->ParRep.VIn, yyt->ParRep.VIn)
copyint ((* yyNew)->ParRep.VOut, yyt->ParRep.VOut)
copybool ((* yyNew)->ParRep.ATOMIC, yyt->ParRep.ATOMIC)
copyint ((* yyNew)->ParRep.SLC, yyt->ParRep.SLC)
copybool ((* yyNew)->ParRep.TLA, yyt->ParRep.TLA)
copytSymbol ((* yyNew)->ParRep.Parent, yyt->ParRep.Parent)
copytParUsage ((* yyNew)->ParRep.PUIn, yyt->ParRep.PUIn)
copytParUsage ((* yyNew)->ParRep.PUOut, yyt->ParRep.PUOut)
copytParType ((* yyNew)->ParRep.PType, yyt->ParRep.PType)
copytTree ((* yyNew)->ParRep.Replicator, yyt->ParRep.Replicator)
copytSymbol ((* yyNew)->ParRep.Sym, yyt->ParRep.Sym)
yyt = yyt->ParRep.Process;
yyNew = & (* yyNew)->ParRep.Process; break;
case kProcessor: (* yyNew)->Processor = yyt->Processor;
copytPosition ((* yyNew)->Processor.Pos, yyt->Processor.Pos)
copytTree ((* yyNew)->Processor.SpecList, yyt->Processor.SpecList)
copyint ((* yyNew)->Processor.V1In, yyt->Processor.V1In)
copyint ((* yyNew)->Processor.V1Out, yyt->Processor.V1Out)
copyint ((* yyNew)->Processor.VS, yyt->Processor.VS)
copyint ((* yyNew)->Processor.VIn, yyt->Processor.VIn)
copyint ((* yyNew)->Processor.VOut, yyt->Processor.VOut)
copybool ((* yyNew)->Processor.ATOMIC, yyt->Processor.ATOMIC)
copyint ((* yyNew)->Processor.SLC, yyt->Processor.SLC)
copybool ((* yyNew)->Processor.TLA, yyt->Processor.TLA)
copytSymbol ((* yyNew)->Processor.Parent, yyt->Processor.Parent)
copytParUsage ((* yyNew)->Processor.PUIn, yyt->Processor.PUIn)
copytParUsage ((* yyNew)->Processor.PUOut, yyt->Processor.PUOut)
copytTree ((* yyNew)->Processor.Expr, yyt->Processor.Expr)
copytIdent ((* yyNew)->Processor.Name, yyt->Processor.Name)
yyt = yyt->Processor.Process;
yyNew = & (* yyNew)->Processor.Process; break;
case kIfType: (* yyNew)->IfType = yyt->IfType;
copytPosition ((* yyNew)->IfType.Pos, yyt->IfType.Pos)
copyint ((* yyNew)->IfType.V1In, yyt->IfType.V1In)
copyint ((* yyNew)->IfType.V1Out, yyt->IfType.V1Out)
copyint ((* yyNew)->IfType.VS, yyt->IfType.VS)
copyint ((* yyNew)->IfType.VIn, yyt->IfType.VIn)
copyint ((* yyNew)->IfType.VOut, yyt->IfType.VOut)
copybool ((* yyNew)->IfType.ATOMIC, yyt->IfType.ATOMIC)
copyint ((* yyNew)->IfType.SLC, yyt->IfType.SLC)
copybool ((* yyNew)->IfType.TLA, yyt->IfType.TLA)
copytSymbol ((* yyNew)->IfType.Parent, yyt->IfType.Parent)
copytParUsage ((* yyNew)->IfType.PUIn, yyt->IfType.PUIn)
copytParUsage ((* yyNew)->IfType.PUOut, yyt->IfType.PUOut)
copytChoiceList ((* yyNew)->IfType.BackChoiceList, yyt->IfType.BackChoiceList)
copytIfType ((* yyNew)->IfType.BackIfType, yyt->IfType.BackIfType)
yyt = yyt->IfType.SpecList;
yyNew = & (* yyNew)->IfType.SpecList; break;
case kIf: (* yyNew)->If = yyt->If;
copytPosition ((* yyNew)->If.Pos, yyt->If.Pos)
copytTree ((* yyNew)->If.SpecList, yyt->If.SpecList)
copyint ((* yyNew)->If.V1In, yyt->If.V1In)
copyint ((* yyNew)->If.V1Out, yyt->If.V1Out)
copyint ((* yyNew)->If.VS, yyt->If.VS)
copyint ((* yyNew)->If.VIn, yyt->If.VIn)
copyint ((* yyNew)->If.VOut, yyt->If.VOut)
copybool ((* yyNew)->If.ATOMIC, yyt->If.ATOMIC)
copyint ((* yyNew)->If.SLC, yyt->If.SLC)
copybool ((* yyNew)->If.TLA, yyt->If.TLA)
copytSymbol ((* yyNew)->If.Parent, yyt->If.Parent)
copytParUsage ((* yyNew)->If.PUIn, yyt->If.PUIn)
copytParUsage ((* yyNew)->If.PUOut, yyt->If.PUOut)
copytChoiceList ((* yyNew)->If.BackChoiceList, yyt->If.BackChoiceList)
copytIfType ((* yyNew)->If.BackIfType, yyt->If.BackIfType)
yyt = yyt->If.ChoiceList;
yyNew = & (* yyNew)->If.ChoiceList; break;
case kIfRep: (* yyNew)->IfRep = yyt->IfRep;
copytPosition ((* yyNew)->IfRep.Pos, yyt->IfRep.Pos)
copytTree ((* yyNew)->IfRep.SpecList, yyt->IfRep.SpecList)
copyint ((* yyNew)->IfRep.V1In, yyt->IfRep.V1In)
copyint ((* yyNew)->IfRep.V1Out, yyt->IfRep.V1Out)
copyint ((* yyNew)->IfRep.VS, yyt->IfRep.VS)
copyint ((* yyNew)->IfRep.VIn, yyt->IfRep.VIn)
copyint ((* yyNew)->IfRep.VOut, yyt->IfRep.VOut)
copybool ((* yyNew)->IfRep.ATOMIC, yyt->IfRep.ATOMIC)
copyint ((* yyNew)->IfRep.SLC, yyt->IfRep.SLC)
copybool ((* yyNew)->IfRep.TLA, yyt->IfRep.TLA)
copytSymbol ((* yyNew)->IfRep.Parent, yyt->IfRep.Parent)
copytParUsage ((* yyNew)->IfRep.PUIn, yyt->IfRep.PUIn)
copytParUsage ((* yyNew)->IfRep.PUOut, yyt->IfRep.PUOut)
copytChoiceList ((* yyNew)->IfRep.BackChoiceList, yyt->IfRep.BackChoiceList)
copytIfType ((* yyNew)->IfRep.BackIfType, yyt->IfRep.BackIfType)
copytTree ((* yyNew)->IfRep.Replicator, yyt->IfRep.Replicator)
yyt = yyt->IfRep.ChoiceList;
yyNew = & (* yyNew)->IfRep.ChoiceList; break;
case kInput: (* yyNew)->Input = yyt->Input;
copytPosition ((* yyNew)->Input.Pos, yyt->Input.Pos)
copytTree ((* yyNew)->Input.SpecList, yyt->Input.SpecList)
copyint ((* yyNew)->Input.V1In, yyt->Input.V1In)
copyint ((* yyNew)->Input.V1Out, yyt->Input.V1Out)
copyint ((* yyNew)->Input.VS, yyt->Input.VS)
copyint ((* yyNew)->Input.VIn, yyt->Input.VIn)
copyint ((* yyNew)->Input.VOut, yyt->Input.VOut)
copybool ((* yyNew)->Input.ATOMIC, yyt->Input.ATOMIC)
copyint ((* yyNew)->Input.SLC, yyt->Input.SLC)
copybool ((* yyNew)->Input.TLA, yyt->Input.TLA)
copytSymbol ((* yyNew)->Input.Parent, yyt->Input.Parent)
copytParUsage ((* yyNew)->Input.PUIn, yyt->Input.PUIn)
copytParUsage ((* yyNew)->Input.PUOut, yyt->Input.PUOut)
copytTree ((* yyNew)->Input.Element, yyt->Input.Element)
yyt = yyt->Input.ExprList;
yyNew = & (* yyNew)->Input.ExprList; break;
case kOutput: (* yyNew)->Output = yyt->Output;
copytPosition ((* yyNew)->Output.Pos, yyt->Output.Pos)
copytTree ((* yyNew)->Output.SpecList, yyt->Output.SpecList)
copyint ((* yyNew)->Output.V1In, yyt->Output.V1In)
copyint ((* yyNew)->Output.V1Out, yyt->Output.V1Out)
copyint ((* yyNew)->Output.VS, yyt->Output.VS)
copyint ((* yyNew)->Output.VIn, yyt->Output.VIn)
copyint ((* yyNew)->Output.VOut, yyt->Output.VOut)
copybool ((* yyNew)->Output.ATOMIC, yyt->Output.ATOMIC)
copyint ((* yyNew)->Output.SLC, yyt->Output.SLC)
copybool ((* yyNew)->Output.TLA, yyt->Output.TLA)
copytSymbol ((* yyNew)->Output.Parent, yyt->Output.Parent)
copytParUsage ((* yyNew)->Output.PUIn, yyt->Output.PUIn)
copytParUsage ((* yyNew)->Output.PUOut, yyt->Output.PUOut)
copytTree ((* yyNew)->Output.Element, yyt->Output.Element)
yyt = yyt->Output.ExprList;
yyNew = & (* yyNew)->Output.ExprList; break;
case kCaseIn: (* yyNew)->CaseIn = yyt->CaseIn;
copytPosition ((* yyNew)->CaseIn.Pos, yyt->CaseIn.Pos)
copytTree ((* yyNew)->CaseIn.SpecList, yyt->CaseIn.SpecList)
copyint ((* yyNew)->CaseIn.V1In, yyt->CaseIn.V1In)
copyint ((* yyNew)->CaseIn.V1Out, yyt->CaseIn.V1Out)
copyint ((* yyNew)->CaseIn.VS, yyt->CaseIn.VS)
copyint ((* yyNew)->CaseIn.VIn, yyt->CaseIn.VIn)
copyint ((* yyNew)->CaseIn.VOut, yyt->CaseIn.VOut)
copybool ((* yyNew)->CaseIn.ATOMIC, yyt->CaseIn.ATOMIC)
copyint ((* yyNew)->CaseIn.SLC, yyt->CaseIn.SLC)
copybool ((* yyNew)->CaseIn.TLA, yyt->CaseIn.TLA)
copytSymbol ((* yyNew)->CaseIn.Parent, yyt->CaseIn.Parent)
copytParUsage ((* yyNew)->CaseIn.PUIn, yyt->CaseIn.PUIn)
copytParUsage ((* yyNew)->CaseIn.PUOut, yyt->CaseIn.PUOut)
copytTree ((* yyNew)->CaseIn.Element, yyt->CaseIn.Element)
yyt = yyt->CaseIn.CaseList;
yyNew = & (* yyNew)->CaseIn.CaseList; break;
case kAfter: (* yyNew)->After = yyt->After;
copytPosition ((* yyNew)->After.Pos, yyt->After.Pos)
copytTree ((* yyNew)->After.SpecList, yyt->After.SpecList)
copyint ((* yyNew)->After.V1In, yyt->After.V1In)
copyint ((* yyNew)->After.V1Out, yyt->After.V1Out)
copyint ((* yyNew)->After.VS, yyt->After.VS)
copyint ((* yyNew)->After.VIn, yyt->After.VIn)
copyint ((* yyNew)->After.VOut, yyt->After.VOut)
copybool ((* yyNew)->After.ATOMIC, yyt->After.ATOMIC)
copyint ((* yyNew)->After.SLC, yyt->After.SLC)
copybool ((* yyNew)->After.TLA, yyt->After.TLA)
copytSymbol ((* yyNew)->After.Parent, yyt->After.Parent)
copytParUsage ((* yyNew)->After.PUIn, yyt->After.PUIn)
copytParUsage ((* yyNew)->After.PUOut, yyt->After.PUOut)
copytTree ((* yyNew)->After.Element, yyt->After.Element)
yyt = yyt->After.Expr;
yyNew = & (* yyNew)->After.Expr; break;
case kSeqRep: (* yyNew)->SeqRep = yyt->SeqRep;
copytPosition ((* yyNew)->SeqRep.Pos, yyt->SeqRep.Pos)
copytTree ((* yyNew)->SeqRep.SpecList, yyt->SeqRep.SpecList)
copyint ((* yyNew)->SeqRep.V1In, yyt->SeqRep.V1In)
copyint ((* yyNew)->SeqRep.V1Out, yyt->SeqRep.V1Out)
copyint ((* yyNew)->SeqRep.VS, yyt->SeqRep.VS)
copyint ((* yyNew)->SeqRep.VIn, yyt->SeqRep.VIn)
copyint ((* yyNew)->SeqRep.VOut, yyt->SeqRep.VOut)
copybool ((* yyNew)->SeqRep.ATOMIC, yyt->SeqRep.ATOMIC)
copyint ((* yyNew)->SeqRep.SLC, yyt->SeqRep.SLC)
copybool ((* yyNew)->SeqRep.TLA, yyt->SeqRep.TLA)
copytSymbol ((* yyNew)->SeqRep.Parent, yyt->SeqRep.Parent)
copytParUsage ((* yyNew)->SeqRep.PUIn, yyt->SeqRep.PUIn)
copytParUsage ((* yyNew)->SeqRep.PUOut, yyt->SeqRep.PUOut)
copytTree ((* yyNew)->SeqRep.Replicator, yyt->SeqRep.Replicator)
yyt = yyt->SeqRep.Process;
yyNew = & (* yyNew)->SeqRep.Process; break;
case kSeq: (* yyNew)->Seq = yyt->Seq;
copytPosition ((* yyNew)->Seq.Pos, yyt->Seq.Pos)
copytTree ((* yyNew)->Seq.SpecList, yyt->Seq.SpecList)
copyint ((* yyNew)->Seq.V1In, yyt->Seq.V1In)
copyint ((* yyNew)->Seq.V1Out, yyt->Seq.V1Out)
copyint ((* yyNew)->Seq.VS, yyt->Seq.VS)
copyint ((* yyNew)->Seq.VIn, yyt->Seq.VIn)
copyint ((* yyNew)->Seq.VOut, yyt->Seq.VOut)
copybool ((* yyNew)->Seq.ATOMIC, yyt->Seq.ATOMIC)
copyint ((* yyNew)->Seq.SLC, yyt->Seq.SLC)
copybool ((* yyNew)->Seq.TLA, yyt->Seq.TLA)
copytSymbol ((* yyNew)->Seq.Parent, yyt->Seq.Parent)
copytParUsage ((* yyNew)->Seq.PUIn, yyt->Seq.PUIn)
copytParUsage ((* yyNew)->Seq.PUOut, yyt->Seq.PUOut)
yyt = yyt->Seq.ProcList;
yyNew = & (* yyNew)->Seq.ProcList; break;
case kCase: (* yyNew)->Case = yyt->Case;
copytPosition ((* yyNew)->Case.Pos, yyt->Case.Pos)
copytTree ((* yyNew)->Case.SpecList, yyt->Case.SpecList)
copyint ((* yyNew)->Case.V1In, yyt->Case.V1In)
copyint ((* yyNew)->Case.V1Out, yyt->Case.V1Out)
copyint ((* yyNew)->Case.VS, yyt->Case.VS)
copyint ((* yyNew)->Case.VIn, yyt->Case.VIn)
copyint ((* yyNew)->Case.VOut, yyt->Case.VOut)
copybool ((* yyNew)->Case.ATOMIC, yyt->Case.ATOMIC)
copyint ((* yyNew)->Case.SLC, yyt->Case.SLC)
copybool ((* yyNew)->Case.TLA, yyt->Case.TLA)
copytSymbol ((* yyNew)->Case.Parent, yyt->Case.Parent)
copytParUsage ((* yyNew)->Case.PUIn, yyt->Case.PUIn)
copytParUsage ((* yyNew)->Case.PUOut, yyt->Case.PUOut)
copytTree ((* yyNew)->Case.Selector, yyt->Case.Selector)
yyt = yyt->Case.ChoiceList;
yyNew = & (* yyNew)->Case.ChoiceList; break;
case kWhile: (* yyNew)->While = yyt->While;
copytPosition ((* yyNew)->While.Pos, yyt->While.Pos)
copytTree ((* yyNew)->While.SpecList, yyt->While.SpecList)
copyint ((* yyNew)->While.V1In, yyt->While.V1In)
copyint ((* yyNew)->While.V1Out, yyt->While.V1Out)
copyint ((* yyNew)->While.VS, yyt->While.VS)
copyint ((* yyNew)->While.VIn, yyt->While.VIn)
copyint ((* yyNew)->While.VOut, yyt->While.VOut)
copybool ((* yyNew)->While.ATOMIC, yyt->While.ATOMIC)
copyint ((* yyNew)->While.SLC, yyt->While.SLC)
copybool ((* yyNew)->While.TLA, yyt->While.TLA)
copytSymbol ((* yyNew)->While.Parent, yyt->While.Parent)
copytParUsage ((* yyNew)->While.PUIn, yyt->While.PUIn)
copytParUsage ((* yyNew)->While.PUOut, yyt->While.PUOut)
copytTree ((* yyNew)->While.Expr, yyt->While.Expr)
yyt = yyt->While.Process;
yyNew = & (* yyNew)->While.Process; break;
case kSkip: (* yyNew)->Skip = yyt->Skip;
copytPosition ((* yyNew)->Skip.Pos, yyt->Skip.Pos)
copyint ((* yyNew)->Skip.V1In, yyt->Skip.V1In)
copyint ((* yyNew)->Skip.V1Out, yyt->Skip.V1Out)
copyint ((* yyNew)->Skip.VS, yyt->Skip.VS)
copyint ((* yyNew)->Skip.VIn, yyt->Skip.VIn)
copyint ((* yyNew)->Skip.VOut, yyt->Skip.VOut)
copybool ((* yyNew)->Skip.ATOMIC, yyt->Skip.ATOMIC)
copyint ((* yyNew)->Skip.SLC, yyt->Skip.SLC)
copybool ((* yyNew)->Skip.TLA, yyt->Skip.TLA)
copytSymbol ((* yyNew)->Skip.Parent, yyt->Skip.Parent)
copytParUsage ((* yyNew)->Skip.PUIn, yyt->Skip.PUIn)
copytParUsage ((* yyNew)->Skip.PUOut, yyt->Skip.PUOut)
yyt = yyt->Skip.SpecList;
yyNew = & (* yyNew)->Skip.SpecList; break;
case kStop: (* yyNew)->Stop = yyt->Stop;
copytPosition ((* yyNew)->Stop.Pos, yyt->Stop.Pos)
copyint ((* yyNew)->Stop.V1In, yyt->Stop.V1In)
copyint ((* yyNew)->Stop.V1Out, yyt->Stop.V1Out)
copyint ((* yyNew)->Stop.VS, yyt->Stop.VS)
copyint ((* yyNew)->Stop.VIn, yyt->Stop.VIn)
copyint ((* yyNew)->Stop.VOut, yyt->Stop.VOut)
copybool ((* yyNew)->Stop.ATOMIC, yyt->Stop.ATOMIC)
copyint ((* yyNew)->Stop.SLC, yyt->Stop.SLC)
copybool ((* yyNew)->Stop.TLA, yyt->Stop.TLA)
copytSymbol ((* yyNew)->Stop.Parent, yyt->Stop.Parent)
copytParUsage ((* yyNew)->Stop.PUIn, yyt->Stop.PUIn)
copytParUsage ((* yyNew)->Stop.PUOut, yyt->Stop.PUOut)
yyt = yyt->Stop.SpecList;
yyNew = & (* yyNew)->Stop.SpecList; break;
case kInstance: (* yyNew)->Instance = yyt->Instance;
copytPosition ((* yyNew)->Instance.Pos, yyt->Instance.Pos)
copytTree ((* yyNew)->Instance.SpecList, yyt->Instance.SpecList)
copyint ((* yyNew)->Instance.V1In, yyt->Instance.V1In)
copyint ((* yyNew)->Instance.V1Out, yyt->Instance.V1Out)
copyint ((* yyNew)->Instance.VS, yyt->Instance.VS)
copyint ((* yyNew)->Instance.VIn, yyt->Instance.VIn)
copyint ((* yyNew)->Instance.VOut, yyt->Instance.VOut)
copybool ((* yyNew)->Instance.ATOMIC, yyt->Instance.ATOMIC)
copyint ((* yyNew)->Instance.SLC, yyt->Instance.SLC)
copybool ((* yyNew)->Instance.TLA, yyt->Instance.TLA)
copytSymbol ((* yyNew)->Instance.Parent, yyt->Instance.Parent)
copytParUsage ((* yyNew)->Instance.PUIn, yyt->Instance.PUIn)
copytParUsage ((* yyNew)->Instance.PUOut, yyt->Instance.PUOut)
copytSymbol ((* yyNew)->Instance.Sym, yyt->Instance.Sym)
copytSymbol ((* yyNew)->Instance.Scope, yyt->Instance.Scope)
copytFrame ((* yyNew)->Instance.Frame, yyt->Instance.Frame)
yyt = yyt->Instance.ExprList;
yyNew = & (* yyNew)->Instance.ExprList; break;
case kCCode: (* yyNew)->CCode = yyt->CCode;
copytPosition ((* yyNew)->CCode.Pos, yyt->CCode.Pos)
copyint ((* yyNew)->CCode.V1In, yyt->CCode.V1In)
copyint ((* yyNew)->CCode.V1Out, yyt->CCode.V1Out)
copyint ((* yyNew)->CCode.VS, yyt->CCode.VS)
copyint ((* yyNew)->CCode.VIn, yyt->CCode.VIn)
copyint ((* yyNew)->CCode.VOut, yyt->CCode.VOut)
copybool ((* yyNew)->CCode.ATOMIC, yyt->CCode.ATOMIC)
copyint ((* yyNew)->CCode.SLC, yyt->CCode.SLC)
copybool ((* yyNew)->CCode.TLA, yyt->CCode.TLA)
copytSymbol ((* yyNew)->CCode.Parent, yyt->CCode.Parent)
copytParUsage ((* yyNew)->CCode.PUIn, yyt->CCode.PUIn)
copytParUsage ((* yyNew)->CCode.PUOut, yyt->CCode.PUOut)
copytStringRef ((* yyNew)->CCode.Code, yyt->CCode.Code)
copytSymbol ((* yyNew)->CCode.Scope, yyt->CCode.Scope)
yyt = yyt->CCode.SpecList;
yyNew = & (* yyNew)->CCode.SpecList; break;
case kNullProcess: (* yyNew)->NullProcess = yyt->NullProcess;
copytPosition ((* yyNew)->NullProcess.Pos, yyt->NullProcess.Pos)
copyint ((* yyNew)->NullProcess.V1In, yyt->NullProcess.V1In)
copyint ((* yyNew)->NullProcess.V1Out, yyt->NullProcess.V1Out)
copyint ((* yyNew)->NullProcess.VS, yyt->NullProcess.VS)
copyint ((* yyNew)->NullProcess.VIn, yyt->NullProcess.VIn)
copyint ((* yyNew)->NullProcess.VOut, yyt->NullProcess.VOut)
copybool ((* yyNew)->NullProcess.ATOMIC, yyt->NullProcess.ATOMIC)
copyint ((* yyNew)->NullProcess.SLC, yyt->NullProcess.SLC)
copybool ((* yyNew)->NullProcess.TLA, yyt->NullProcess.TLA)
copytSymbol ((* yyNew)->NullProcess.Parent, yyt->NullProcess.Parent)
copytParUsage ((* yyNew)->NullProcess.PUIn, yyt->NullProcess.PUIn)
copytParUsage ((* yyNew)->NullProcess.PUOut, yyt->NullProcess.PUOut)
yyt = yyt->NullProcess.SpecList;
yyNew = & (* yyNew)->NullProcess.SpecList; break;
case kAltList: (* yyNew)->AltList = yyt->AltList;
copyint ((* yyNew)->AltList.V1In, yyt->AltList.V1In)
copyint ((* yyNew)->AltList.V1Out, yyt->AltList.V1Out)
copyint ((* yyNew)->AltList.VS, yyt->AltList.VS)
copyint ((* yyNew)->AltList.VIn, yyt->AltList.VIn)
copyint ((* yyNew)->AltList.VOut, yyt->AltList.VOut)
copyint ((* yyNew)->AltList.SLC, yyt->AltList.SLC)
copyint ((* yyNew)->AltList.ARC, yyt->AltList.ARC)
copytSymbol ((* yyNew)->AltList.Parent, yyt->AltList.Parent)
copytParUsage ((* yyNew)->AltList.PUIn, yyt->AltList.PUIn)
copytParUsage ((* yyNew)->AltList.PUOut, yyt->AltList.PUOut)
return;
case kNoAlt: (* yyNew)->NoAlt = yyt->NoAlt;
copyint ((* yyNew)->NoAlt.V1In, yyt->NoAlt.V1In)
copyint ((* yyNew)->NoAlt.V1Out, yyt->NoAlt.V1Out)
copyint ((* yyNew)->NoAlt.VS, yyt->NoAlt.VS)
copyint ((* yyNew)->NoAlt.VIn, yyt->NoAlt.VIn)
copyint ((* yyNew)->NoAlt.VOut, yyt->NoAlt.VOut)
copyint ((* yyNew)->NoAlt.SLC, yyt->NoAlt.SLC)
copyint ((* yyNew)->NoAlt.ARC, yyt->NoAlt.ARC)
copytSymbol ((* yyNew)->NoAlt.Parent, yyt->NoAlt.Parent)
copytParUsage ((* yyNew)->NoAlt.PUIn, yyt->NoAlt.PUIn)
copytParUsage ((* yyNew)->NoAlt.PUOut, yyt->NoAlt.PUOut)
return;
case kAnAlts: (* yyNew)->AnAlts = yyt->AnAlts;
copyint ((* yyNew)->AnAlts.V1In, yyt->AnAlts.V1In)
copyint ((* yyNew)->AnAlts.V1Out, yyt->AnAlts.V1Out)
copyint ((* yyNew)->AnAlts.VS, yyt->AnAlts.VS)
copyint ((* yyNew)->AnAlts.VIn, yyt->AnAlts.VIn)
copyint ((* yyNew)->AnAlts.VOut, yyt->AnAlts.VOut)
copyint ((* yyNew)->AnAlts.SLC, yyt->AnAlts.SLC)
copyint ((* yyNew)->AnAlts.ARC, yyt->AnAlts.ARC)
copytSymbol ((* yyNew)->AnAlts.Parent, yyt->AnAlts.Parent)
copytParUsage ((* yyNew)->AnAlts.PUIn, yyt->AnAlts.PUIn)
copytParUsage ((* yyNew)->AnAlts.PUOut, yyt->AnAlts.PUOut)
copytTree ((* yyNew)->AnAlts.SpecList, yyt->AnAlts.SpecList)
yyt = yyt->AnAlts.Next;
yyNew = & (* yyNew)->AnAlts.Next; break;
case kAnAlt: (* yyNew)->AnAlt = yyt->AnAlt;
copyint ((* yyNew)->AnAlt.V1In, yyt->AnAlt.V1In)
copyint ((* yyNew)->AnAlt.V1Out, yyt->AnAlt.V1Out)
copyint ((* yyNew)->AnAlt.VS, yyt->AnAlt.VS)
copyint ((* yyNew)->AnAlt.VIn, yyt->AnAlt.VIn)
copyint ((* yyNew)->AnAlt.VOut, yyt->AnAlt.VOut)
copyint ((* yyNew)->AnAlt.SLC, yyt->AnAlt.SLC)
copyint ((* yyNew)->AnAlt.ARC, yyt->AnAlt.ARC)
copytSymbol ((* yyNew)->AnAlt.Parent, yyt->AnAlt.Parent)
copytParUsage ((* yyNew)->AnAlt.PUIn, yyt->AnAlt.PUIn)
copytParUsage ((* yyNew)->AnAlt.PUOut, yyt->AnAlt.PUOut)
copytTree ((* yyNew)->AnAlt.SpecList, yyt->AnAlt.SpecList)
copytTree ((* yyNew)->AnAlt.Guard, yyt->AnAlt.Guard)
copyint ((* yyNew)->AnAlt.Label, yyt->AnAlt.Label)
yyt = yyt->AnAlt.Next;
yyNew = & (* yyNew)->AnAlt.Next; break;
case kANewAlt: (* yyNew)->ANewAlt = yyt->ANewAlt;
copyint ((* yyNew)->ANewAlt.V1In, yyt->ANewAlt.V1In)
copyint ((* yyNew)->ANewAlt.V1Out, yyt->ANewAlt.V1Out)
copyint ((* yyNew)->ANewAlt.VS, yyt->ANewAlt.VS)
copyint ((* yyNew)->ANewAlt.VIn, yyt->ANewAlt.VIn)
copyint ((* yyNew)->ANewAlt.VOut, yyt->ANewAlt.VOut)
copyint ((* yyNew)->ANewAlt.SLC, yyt->ANewAlt.SLC)
copyint ((* yyNew)->ANewAlt.ARC, yyt->ANewAlt.ARC)
copytSymbol ((* yyNew)->ANewAlt.Parent, yyt->ANewAlt.Parent)
copytParUsage ((* yyNew)->ANewAlt.PUIn, yyt->ANewAlt.PUIn)
copytParUsage ((* yyNew)->ANewAlt.PUOut, yyt->ANewAlt.PUOut)
copytTree ((* yyNew)->ANewAlt.SpecList, yyt->ANewAlt.SpecList)
copytTree ((* yyNew)->ANewAlt.AltType, yyt->ANewAlt.AltType)
yyt = yyt->ANewAlt.Next;
yyNew = & (* yyNew)->ANewAlt.Next; break;
case kGuard: (* yyNew)->Guard = yyt->Guard;
copytPosition ((* yyNew)->Guard.Pos, yyt->Guard.Pos)
copyint ((* yyNew)->Guard.V1In, yyt->Guard.V1In)
copyint ((* yyNew)->Guard.V1Out, yyt->Guard.V1Out)
copyint ((* yyNew)->Guard.VS, yyt->Guard.VS)
copyint ((* yyNew)->Guard.VIn, yyt->Guard.VIn)
copyint ((* yyNew)->Guard.VOut, yyt->Guard.VOut)
copyint ((* yyNew)->Guard.SLC, yyt->Guard.SLC)
copytSymbol ((* yyNew)->Guard.Parent, yyt->Guard.Parent)
copytParUsage ((* yyNew)->Guard.PUIn, yyt->Guard.PUIn)
copytParUsage ((* yyNew)->Guard.PUOut, yyt->Guard.PUOut)
yyt = yyt->Guard.Expr;
yyNew = & (* yyNew)->Guard.Expr; break;
case kAltInput: (* yyNew)->AltInput = yyt->AltInput;
copytPosition ((* yyNew)->AltInput.Pos, yyt->AltInput.Pos)
copytTree ((* yyNew)->AltInput.Expr, yyt->AltInput.Expr)
copyint ((* yyNew)->AltInput.V1In, yyt->AltInput.V1In)
copyint ((* yyNew)->AltInput.V1Out, yyt->AltInput.V1Out)
copyint ((* yyNew)->AltInput.VS, yyt->AltInput.VS)
copyint ((* yyNew)->AltInput.VIn, yyt->AltInput.VIn)
copyint ((* yyNew)->AltInput.VOut, yyt->AltInput.VOut)
copyint ((* yyNew)->AltInput.SLC, yyt->AltInput.SLC)
copytSymbol ((* yyNew)->AltInput.Parent, yyt->AltInput.Parent)
copytParUsage ((* yyNew)->AltInput.PUIn, yyt->AltInput.PUIn)
copytParUsage ((* yyNew)->AltInput.PUOut, yyt->AltInput.PUOut)
copytTree ((* yyNew)->AltInput.Input, yyt->AltInput.Input)
yyt = yyt->AltInput.Process;
yyNew = & (* yyNew)->AltInput.Process; break;
case kAltTimer: (* yyNew)->AltTimer = yyt->AltTimer;
copytPosition ((* yyNew)->AltTimer.Pos, yyt->AltTimer.Pos)
copytTree ((* yyNew)->AltTimer.Expr, yyt->AltTimer.Expr)
copyint ((* yyNew)->AltTimer.V1In, yyt->AltTimer.V1In)
copyint ((* yyNew)->AltTimer.V1Out, yyt->AltTimer.V1Out)
copyint ((* yyNew)->AltTimer.VS, yyt->AltTimer.VS)
copyint ((* yyNew)->AltTimer.VIn, yyt->AltTimer.VIn)
copyint ((* yyNew)->AltTimer.VOut, yyt->AltTimer.VOut)
copyint ((* yyNew)->AltTimer.SLC, yyt->AltTimer.SLC)
copytSymbol ((* yyNew)->AltTimer.Parent, yyt->AltTimer.Parent)
copytParUsage ((* yyNew)->AltTimer.PUIn, yyt->AltTimer.PUIn)
copytParUsage ((* yyNew)->AltTimer.PUOut, yyt->AltTimer.PUOut)
copytTree ((* yyNew)->AltTimer.After, yyt->AltTimer.After)
yyt = yyt->AltTimer.Process;
yyNew = & (* yyNew)->AltTimer.Process; break;
case kAltCaseIn: (* yyNew)->AltCaseIn = yyt->AltCaseIn;
copytPosition ((* yyNew)->AltCaseIn.Pos, yyt->AltCaseIn.Pos)
copytTree ((* yyNew)->AltCaseIn.Expr, yyt->AltCaseIn.Expr)
copyint ((* yyNew)->AltCaseIn.V1In, yyt->AltCaseIn.V1In)
copyint ((* yyNew)->AltCaseIn.V1Out, yyt->AltCaseIn.V1Out)
copyint ((* yyNew)->AltCaseIn.VS, yyt->AltCaseIn.VS)
copyint ((* yyNew)->AltCaseIn.VIn, yyt->AltCaseIn.VIn)
copyint ((* yyNew)->AltCaseIn.VOut, yyt->AltCaseIn.VOut)
copyint ((* yyNew)->AltCaseIn.SLC, yyt->AltCaseIn.SLC)
copytSymbol ((* yyNew)->AltCaseIn.Parent, yyt->AltCaseIn.Parent)
copytParUsage ((* yyNew)->AltCaseIn.PUIn, yyt->AltCaseIn.PUIn)
copytParUsage ((* yyNew)->AltCaseIn.PUOut, yyt->AltCaseIn.PUOut)
yyt = yyt->AltCaseIn.CaseIn;
yyNew = & (* yyNew)->AltCaseIn.CaseIn; break;
case kNoInput: (* yyNew)->NoInput = yyt->NoInput;
copytPosition ((* yyNew)->NoInput.Pos, yyt->NoInput.Pos)
copytTree ((* yyNew)->NoInput.Expr, yyt->NoInput.Expr)
copyint ((* yyNew)->NoInput.V1In, yyt->NoInput.V1In)
copyint ((* yyNew)->NoInput.V1Out, yyt->NoInput.V1Out)
copyint ((* yyNew)->NoInput.VS, yyt->NoInput.VS)
copyint ((* yyNew)->NoInput.VIn, yyt->NoInput.VIn)
copyint ((* yyNew)->NoInput.VOut, yyt->NoInput.VOut)
copyint ((* yyNew)->NoInput.SLC, yyt->NoInput.SLC)
copytSymbol ((* yyNew)->NoInput.Parent, yyt->NoInput.Parent)
copytParUsage ((* yyNew)->NoInput.PUIn, yyt->NoInput.PUIn)
copytParUsage ((* yyNew)->NoInput.PUOut, yyt->NoInput.PUOut)
yyt = yyt->NoInput.Process;
yyNew = & (* yyNew)->NoInput.Process; break;
case kChoiceList: (* yyNew)->ChoiceList = yyt->ChoiceList;
copytType ((* yyNew)->ChoiceList.DefType, yyt->ChoiceList.DefType)
copyint ((* yyNew)->ChoiceList.V1In, yyt->ChoiceList.V1In)
copyint ((* yyNew)->ChoiceList.V1Out, yyt->ChoiceList.V1Out)
copytType ((* yyNew)->ChoiceList.SynType, yyt->ChoiceList.SynType)
copytType ((* yyNew)->ChoiceList.InhType, yyt->ChoiceList.InhType)
copyint ((* yyNew)->ChoiceList.VS, yyt->ChoiceList.VS)
copyint ((* yyNew)->ChoiceList.VIn, yyt->ChoiceList.VIn)
copyint ((* yyNew)->ChoiceList.VOut, yyt->ChoiceList.VOut)
copybool ((* yyNew)->ChoiceList.ATOMIC, yyt->ChoiceList.ATOMIC)
copyint ((* yyNew)->ChoiceList.SLC, yyt->ChoiceList.SLC)
copytSymbol ((* yyNew)->ChoiceList.Parent, yyt->ChoiceList.Parent)
copytParUsage ((* yyNew)->ChoiceList.PUIn, yyt->ChoiceList.PUIn)
copytParUsage ((* yyNew)->ChoiceList.PUOut, yyt->ChoiceList.PUOut)
return;
case kNoChoice: (* yyNew)->NoChoice = yyt->NoChoice;
copytType ((* yyNew)->NoChoice.DefType, yyt->NoChoice.DefType)
copyint ((* yyNew)->NoChoice.V1In, yyt->NoChoice.V1In)
copyint ((* yyNew)->NoChoice.V1Out, yyt->NoChoice.V1Out)
copytType ((* yyNew)->NoChoice.SynType, yyt->NoChoice.SynType)
copytType ((* yyNew)->NoChoice.InhType, yyt->NoChoice.InhType)
copyint ((* yyNew)->NoChoice.VS, yyt->NoChoice.VS)
copyint ((* yyNew)->NoChoice.VIn, yyt->NoChoice.VIn)
copyint ((* yyNew)->NoChoice.VOut, yyt->NoChoice.VOut)
copybool ((* yyNew)->NoChoice.ATOMIC, yyt->NoChoice.ATOMIC)
copyint ((* yyNew)->NoChoice.SLC, yyt->NoChoice.SLC)
copytSymbol ((* yyNew)->NoChoice.Parent, yyt->NoChoice.Parent)
copytParUsage ((* yyNew)->NoChoice.PUIn, yyt->NoChoice.PUIn)
copytParUsage ((* yyNew)->NoChoice.PUOut, yyt->NoChoice.PUOut)
return;
case kAChoices: (* yyNew)->AChoices = yyt->AChoices;
copytType ((* yyNew)->AChoices.DefType, yyt->AChoices.DefType)
copyint ((* yyNew)->AChoices.V1In, yyt->AChoices.V1In)
copyint ((* yyNew)->AChoices.V1Out, yyt->AChoices.V1Out)
copytType ((* yyNew)->AChoices.SynType, yyt->AChoices.SynType)
copytType ((* yyNew)->AChoices.InhType, yyt->AChoices.InhType)
copyint ((* yyNew)->AChoices.VS, yyt->AChoices.VS)
copyint ((* yyNew)->AChoices.VIn, yyt->AChoices.VIn)
copyint ((* yyNew)->AChoices.VOut, yyt->AChoices.VOut)
copybool ((* yyNew)->AChoices.ATOMIC, yyt->AChoices.ATOMIC)
copyint ((* yyNew)->AChoices.SLC, yyt->AChoices.SLC)
copytSymbol ((* yyNew)->AChoices.Parent, yyt->AChoices.Parent)
copytParUsage ((* yyNew)->AChoices.PUIn, yyt->AChoices.PUIn)
copytParUsage ((* yyNew)->AChoices.PUOut, yyt->AChoices.PUOut)
copytTree ((* yyNew)->AChoices.SpecList, yyt->AChoices.SpecList)
yyt = yyt->AChoices.Next;
yyNew = & (* yyNew)->AChoices.Next; break;
case kAChoice: (* yyNew)->AChoice = yyt->AChoice;
copytType ((* yyNew)->AChoice.DefType, yyt->AChoice.DefType)
copyint ((* yyNew)->AChoice.V1In, yyt->AChoice.V1In)
copyint ((* yyNew)->AChoice.V1Out, yyt->AChoice.V1Out)
copytType ((* yyNew)->AChoice.SynType, yyt->AChoice.SynType)
copytType ((* yyNew)->AChoice.InhType, yyt->AChoice.InhType)
copyint ((* yyNew)->AChoice.VS, yyt->AChoice.VS)
copyint ((* yyNew)->AChoice.VIn, yyt->AChoice.VIn)
copyint ((* yyNew)->AChoice.VOut, yyt->AChoice.VOut)
copybool ((* yyNew)->AChoice.ATOMIC, yyt->AChoice.ATOMIC)
copyint ((* yyNew)->AChoice.SLC, yyt->AChoice.SLC)
copytSymbol ((* yyNew)->AChoice.Parent, yyt->AChoice.Parent)
copytParUsage ((* yyNew)->AChoice.PUIn, yyt->AChoice.PUIn)
copytParUsage ((* yyNew)->AChoice.PUOut, yyt->AChoice.PUOut)
copytTree ((* yyNew)->AChoice.SpecList, yyt->AChoice.SpecList)
copytTree ((* yyNew)->AChoice.ExprList, yyt->AChoice.ExprList)
copytTree ((* yyNew)->AChoice.Process, yyt->AChoice.Process)
copyint ((* yyNew)->AChoice.Label, yyt->AChoice.Label)
yyt = yyt->AChoice.Next;
yyNew = & (* yyNew)->AChoice.Next; break;
case kANewIf: (* yyNew)->ANewIf = yyt->ANewIf;
copytType ((* yyNew)->ANewIf.DefType, yyt->ANewIf.DefType)
copyint ((* yyNew)->ANewIf.V1In, yyt->ANewIf.V1In)
copyint ((* yyNew)->ANewIf.V1Out, yyt->ANewIf.V1Out)
copytType ((* yyNew)->ANewIf.SynType, yyt->ANewIf.SynType)
copytType ((* yyNew)->ANewIf.InhType, yyt->ANewIf.InhType)
copyint ((* yyNew)->ANewIf.VS, yyt->ANewIf.VS)
copyint ((* yyNew)->ANewIf.VIn, yyt->ANewIf.VIn)
copyint ((* yyNew)->ANewIf.VOut, yyt->ANewIf.VOut)
copybool ((* yyNew)->ANewIf.ATOMIC, yyt->ANewIf.ATOMIC)
copyint ((* yyNew)->ANewIf.SLC, yyt->ANewIf.SLC)
copytSymbol ((* yyNew)->ANewIf.Parent, yyt->ANewIf.Parent)
copytParUsage ((* yyNew)->ANewIf.PUIn, yyt->ANewIf.PUIn)
copytParUsage ((* yyNew)->ANewIf.PUOut, yyt->ANewIf.PUOut)
copytTree ((* yyNew)->ANewIf.SpecList, yyt->ANewIf.SpecList)
copytTree ((* yyNew)->ANewIf.IfType, yyt->ANewIf.IfType)
yyt = yyt->ANewIf.Next;
yyNew = & (* yyNew)->ANewIf.Next; break;
case kProcList: (* yyNew)->ProcList = yyt->ProcList;
copyint ((* yyNew)->ProcList.V1In, yyt->ProcList.V1In)
copyint ((* yyNew)->ProcList.V1Out, yyt->ProcList.V1Out)
copyint ((* yyNew)->ProcList.VS, yyt->ProcList.VS)
copyint ((* yyNew)->ProcList.VIn, yyt->ProcList.VIn)
copyint ((* yyNew)->ProcList.VOut, yyt->ProcList.VOut)
copybool ((* yyNew)->ProcList.ATOMIC, yyt->ProcList.ATOMIC)
copyint ((* yyNew)->ProcList.SLC, yyt->ProcList.SLC)
copytSymbol ((* yyNew)->ProcList.Parent, yyt->ProcList.Parent)
copytParUsage ((* yyNew)->ProcList.PUIn, yyt->ProcList.PUIn)
copytParUsage ((* yyNew)->ProcList.PUOut, yyt->ProcList.PUOut)
copybool ((* yyNew)->ProcList.PAR, yyt->ProcList.PAR)
copytPosition ((* yyNew)->ProcList.Pos, yyt->ProcList.Pos)
return;
case kNoProcess: (* yyNew)->NoProcess = yyt->NoProcess;
copyint ((* yyNew)->NoProcess.V1In, yyt->NoProcess.V1In)
copyint ((* yyNew)->NoProcess.V1Out, yyt->NoProcess.V1Out)
copyint ((* yyNew)->NoProcess.VS, yyt->NoProcess.VS)
copyint ((* yyNew)->NoProcess.VIn, yyt->NoProcess.VIn)
copyint ((* yyNew)->NoProcess.VOut, yyt->NoProcess.VOut)
copybool ((* yyNew)->NoProcess.ATOMIC, yyt->NoProcess.ATOMIC)
copyint ((* yyNew)->NoProcess.SLC, yyt->NoProcess.SLC)
copytSymbol ((* yyNew)->NoProcess.Parent, yyt->NoProcess.Parent)
copytParUsage ((* yyNew)->NoProcess.PUIn, yyt->NoProcess.PUIn)
copytParUsage ((* yyNew)->NoProcess.PUOut, yyt->NoProcess.PUOut)
copybool ((* yyNew)->NoProcess.PAR, yyt->NoProcess.PAR)
copytPosition ((* yyNew)->NoProcess.Pos, yyt->NoProcess.Pos)
return;
case kAProcess: (* yyNew)->AProcess = yyt->AProcess;
copyint ((* yyNew)->AProcess.V1In, yyt->AProcess.V1In)
copyint ((* yyNew)->AProcess.V1Out, yyt->AProcess.V1Out)
copyint ((* yyNew)->AProcess.VS, yyt->AProcess.VS)
copyint ((* yyNew)->AProcess.VIn, yyt->AProcess.VIn)
copyint ((* yyNew)->AProcess.VOut, yyt->AProcess.VOut)
copybool ((* yyNew)->AProcess.ATOMIC, yyt->AProcess.ATOMIC)
copyint ((* yyNew)->AProcess.SLC, yyt->AProcess.SLC)
copytSymbol ((* yyNew)->AProcess.Parent, yyt->AProcess.Parent)
copytParUsage ((* yyNew)->AProcess.PUIn, yyt->AProcess.PUIn)
copytParUsage ((* yyNew)->AProcess.PUOut, yyt->AProcess.PUOut)
copybool ((* yyNew)->AProcess.PAR, yyt->AProcess.PAR)
copytPosition ((* yyNew)->AProcess.Pos, yyt->AProcess.Pos)
copytTree ((* yyNew)->AProcess.Process, yyt->AProcess.Process)
copytSymbol ((* yyNew)->AProcess.Sym, yyt->AProcess.Sym)
yyt = yyt->AProcess.Next;
yyNew = & (* yyNew)->AProcess.Next; break;
case kCaseList: (* yyNew)->CaseList = yyt->CaseList;
copytType ((* yyNew)->CaseList.FLIn, yyt->CaseList.FLIn)
copytType ((* yyNew)->CaseList.FLOut, yyt->CaseList.FLOut)
copyint ((* yyNew)->CaseList.V1In, yyt->CaseList.V1In)
copyint ((* yyNew)->CaseList.V1Out, yyt->CaseList.V1Out)
copytType ((* yyNew)->CaseList.InhType, yyt->CaseList.InhType)
copyint ((* yyNew)->CaseList.VS, yyt->CaseList.VS)
copyint ((* yyNew)->CaseList.VIn, yyt->CaseList.VIn)
copyint ((* yyNew)->CaseList.VOut, yyt->CaseList.VOut)
copyint ((* yyNew)->CaseList.SLC, yyt->CaseList.SLC)
copytSymbol ((* yyNew)->CaseList.Parent, yyt->CaseList.Parent)
copytParUsage ((* yyNew)->CaseList.PUIn, yyt->CaseList.PUIn)
copytParUsage ((* yyNew)->CaseList.PUOut, yyt->CaseList.PUOut)
return;
case kNoCase: (* yyNew)->NoCase = yyt->NoCase;
copytType ((* yyNew)->NoCase.FLIn, yyt->NoCase.FLIn)
copytType ((* yyNew)->NoCase.FLOut, yyt->NoCase.FLOut)
copyint ((* yyNew)->NoCase.V1In, yyt->NoCase.V1In)
copyint ((* yyNew)->NoCase.V1Out, yyt->NoCase.V1Out)
copytType ((* yyNew)->NoCase.InhType, yyt->NoCase.InhType)
copyint ((* yyNew)->NoCase.VS, yyt->NoCase.VS)
copyint ((* yyNew)->NoCase.VIn, yyt->NoCase.VIn)
copyint ((* yyNew)->NoCase.VOut, yyt->NoCase.VOut)
copyint ((* yyNew)->NoCase.SLC, yyt->NoCase.SLC)
copytSymbol ((* yyNew)->NoCase.Parent, yyt->NoCase.Parent)
copytParUsage ((* yyNew)->NoCase.PUIn, yyt->NoCase.PUIn)
copytParUsage ((* yyNew)->NoCase.PUOut, yyt->NoCase.PUOut)
return;
case kACase: (* yyNew)->ACase = yyt->ACase;
copytType ((* yyNew)->ACase.FLIn, yyt->ACase.FLIn)
copytType ((* yyNew)->ACase.FLOut, yyt->ACase.FLOut)
copyint ((* yyNew)->ACase.V1In, yyt->ACase.V1In)
copyint ((* yyNew)->ACase.V1Out, yyt->ACase.V1Out)
copytType ((* yyNew)->ACase.InhType, yyt->ACase.InhType)
copyint ((* yyNew)->ACase.VS, yyt->ACase.VS)
copyint ((* yyNew)->ACase.VIn, yyt->ACase.VIn)
copyint ((* yyNew)->ACase.VOut, yyt->ACase.VOut)
copyint ((* yyNew)->ACase.SLC, yyt->ACase.SLC)
copytSymbol ((* yyNew)->ACase.Parent, yyt->ACase.Parent)
copytParUsage ((* yyNew)->ACase.PUIn, yyt->ACase.PUIn)
copytParUsage ((* yyNew)->ACase.PUOut, yyt->ACase.PUOut)
copytTree ((* yyNew)->ACase.SpecList, yyt->ACase.SpecList)
copytTree ((* yyNew)->ACase.ExprList, yyt->ACase.ExprList)
copytTree ((* yyNew)->ACase.Process, yyt->ACase.Process)
copyint ((* yyNew)->ACase.Label, yyt->ACase.Label)
yyt = yyt->ACase.Next;
yyNew = & (* yyNew)->ACase.Next; break;
case kAccessList: (* yyNew)->AccessList = yyt->AccessList;
return;
case kAnAccess: (* yyNew)->AnAccess = yyt->AnAccess;
copytTree ((* yyNew)->AnAccess.Next, yyt->AnAccess.Next)
copytSymbol ((* yyNew)->AnAccess.Sym, yyt->AnAccess.Sym)
yyt = yyt->AnAccess.ElementList;
yyNew = & (* yyNew)->AnAccess.ElementList; break;
case kNoAccess: (* yyNew)->NoAccess = yyt->NoAccess;
return;
case kElementList: (* yyNew)->ElementList = yyt->ElementList;
return;
case kAnElement: (* yyNew)->AnElement = yyt->AnElement;
copytTree ((* yyNew)->AnElement.Next, yyt->AnElement.Next)
yyt = yyt->AnElement.Element;
yyNew = & (* yyNew)->AnElement.Element; break;
case kNoElement: (* yyNew)->NoElement = yyt->NoElement;
return;
  default: ;
  }
 }
}

tTree CopyTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 tTree yyNew;
 yyMark (yyt);
 yyOldToNewCount = 0;
 yyCopyTree (yyt, & yyNew);
 return yyNew;
}

static bool yyCheckTree ARGS((tTree yyt));

bool CheckTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyMark (yyt);
 return yyCheckTree (yyt);
}

static bool yyCheckChild
# if defined __STDC__ | defined __cplusplus
 (tTree yyParent, tTree yyyChild, Tree_tKind yyType, char * yySelector)
# else
 (yyParent, yyyChild, yyType, yySelector)
 tTree yyParent, yyyChild;
 Tree_tKind yyType;
 char * yySelector;
# endif
{
 bool yySuccess = Tree_IsType (yyyChild, yyType);
 if (! yySuccess) {
  (void) fputs ("CheckTree: parent = ", stderr);
  WriteTreeNode (stderr, yyParent);
  (void) fprintf (stderr, "\nselector: %s child = ", yySelector);
  WriteTreeNode (stderr, yyyChild);
  (void) fputc ('\n', stderr);
 }
 return yyCheckTree (yyyChild) && yySuccess;
}

static bool yyCheckTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 bool yyResult;
 if (yyt == NoTree) return false;
 else if (yyt->yyHead.yyMark == 0) return true;
 yyt->yyHead.yyMark = 0;

 yyResult = true;
 switch (yyt->Kind) {
case kStart:
yyResult = yyCheckChild (yyt, yyt->Start.Process, kProcess, "Process") && yyResult;
break;
case kASpec:
yyResult = yyCheckChild (yyt, yyt->ASpec.Spec, kSpec, "Spec") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ASpec.Next, kSpecList, "Next") && yyResult;
break;
case kDeclType:
yyResult = yyCheckChild (yyt, yyt->DeclType.Type, kType, "Type") && yyResult;
break;
case kDecl:
yyResult = yyCheckChild (yyt, yyt->Decl.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Decl.IdList, kIdList, "IdList") && yyResult;
break;
case kIs:
yyResult = yyCheckChild (yyt, yyt->Is.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Is.Elemental, kElemental, "Elemental") && yyResult;
break;
case kAVal:
yyResult = yyCheckChild (yyt, yyt->AVal.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AVal.Expr, kExpr, "Expr") && yyResult;
break;
case kVal:
yyResult = yyCheckChild (yyt, yyt->Val.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Val.Expr, kExpr, "Expr") && yyResult;
break;
case kValRetypes:
yyResult = yyCheckChild (yyt, yyt->ValRetypes.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ValRetypes.Expr, kExpr, "Expr") && yyResult;
break;
case kValReshapes:
yyResult = yyCheckChild (yyt, yyt->ValReshapes.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ValReshapes.Expr, kExpr, "Expr") && yyResult;
break;
case kRetypes:
yyResult = yyCheckChild (yyt, yyt->Retypes.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Retypes.Elemental, kElemental, "Elemental") && yyResult;
break;
case kReshapes:
yyResult = yyCheckChild (yyt, yyt->Reshapes.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Reshapes.Elemental, kElemental, "Elemental") && yyResult;
break;
case kDefSeqPro:
yyResult = yyCheckChild (yyt, yyt->DefSeqPro.SeqProList, kSeqProList, "SeqProList") && yyResult;
break;
case kDefTagPro:
yyResult = yyCheckChild (yyt, yyt->DefTagPro.TagProList, kTagProList, "TagProList") && yyResult;
break;
case kDefAProc:
yyResult = yyCheckChild (yyt, yyt->DefAProc.Formals, kFormals, "Formals") && yyResult;
break;
case kDefProc:
yyResult = yyCheckChild (yyt, yyt->DefProc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefProc.Process, kProcess, "Process") && yyResult;
break;
case kInlineDefProc:
yyResult = yyCheckChild (yyt, yyt->InlineDefProc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->InlineDefProc.Process, kProcess, "Process") && yyResult;
break;
case kPrototypeProc:
yyResult = yyCheckChild (yyt, yyt->PrototypeProc.Formals, kFormals, "Formals") && yyResult;
break;
case kDefAFunc:
yyResult = yyCheckChild (yyt, yyt->DefAFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefAFunc.Formals, kFormals, "Formals") && yyResult;
break;
case kDefFunc:
yyResult = yyCheckChild (yyt, yyt->DefFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefFunc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefFunc.Valof, kValof, "Valof") && yyResult;
break;
case kInlineDefFunc:
yyResult = yyCheckChild (yyt, yyt->InlineDefFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->InlineDefFunc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->InlineDefFunc.Valof, kValof, "Valof") && yyResult;
break;
case kDefIsFunc:
yyResult = yyCheckChild (yyt, yyt->DefIsFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefIsFunc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->DefIsFunc.ExprList, kExprList, "ExprList") && yyResult;
break;
case kInlineDefIsFunc:
yyResult = yyCheckChild (yyt, yyt->InlineDefIsFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->InlineDefIsFunc.Formals, kFormals, "Formals") && yyResult;
yyResult = yyCheckChild (yyt, yyt->InlineDefIsFunc.ExprList, kExprList, "ExprList") && yyResult;
break;
case kPrototypeFunc:
yyResult = yyCheckChild (yyt, yyt->PrototypeFunc.TypeList, kTypeList, "TypeList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->PrototypeFunc.Formals, kFormals, "Formals") && yyResult;
break;
case kAFormal:
yyResult = yyCheckChild (yyt, yyt->AFormal.Next, kFormals, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AFormal.By, kBy, "By") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AFormal.Type, kType, "Type") && yyResult;
break;
case kReplicator:
yyResult = yyCheckChild (yyt, yyt->Replicator.From, kExpr, "From") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Replicator.For, kExpr, "For") && yyResult;
break;
case kATagPro:
yyResult = yyCheckChild (yyt, yyt->ATagPro.Next, kTagProList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ATagPro.SeqProList, kSeqProList, "SeqProList") && yyResult;
break;
case kAnId:
yyResult = yyCheckChild (yyt, yyt->AnId.Next, kIdList, "Next") && yyResult;
break;
case kArray:
yyResult = yyCheckChild (yyt, yyt->Array.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Array.Type, kType, "Type") && yyResult;
break;
case kUserArray:
yyResult = yyCheckChild (yyt, yyt->UserArray.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->UserArray.Type, kType, "Type") && yyResult;
break;
case kChan:
yyResult = yyCheckChild (yyt, yyt->Chan.Protocol, kProtocol, "Protocol") && yyResult;
break;
case kUserStructTypes:
yyResult = yyCheckChild (yyt, yyt->UserStructTypes.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kUserDataTypes:
yyResult = yyCheckChild (yyt, yyt->UserDataTypes.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kUserRecord:
yyResult = yyCheckChild (yyt, yyt->UserRecord.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kUserPackedRecord:
yyResult = yyCheckChild (yyt, yyt->UserPackedRecord.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kUserUnion:
yyResult = yyCheckChild (yyt, yyt->UserUnion.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kUserChanRecord:
yyResult = yyCheckChild (yyt, yyt->UserChanRecord.FieldList, kFieldList, "FieldList") && yyResult;
break;
case kAType:
yyResult = yyCheckChild (yyt, yyt->AType.Next, kTypeList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AType.Type, kType, "Type") && yyResult;
break;
case kAField:
yyResult = yyCheckChild (yyt, yyt->AField.Next, kFieldList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AField.Type, kType, "Type") && yyResult;
break;
case kSimple:
yyResult = yyCheckChild (yyt, yyt->Simple.Type, kType, "Type") && yyResult;
break;
case kCounted:
yyResult = yyCheckChild (yyt, yyt->Counted.Length, kType, "Length") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Counted.Type, kType, "Type") && yyResult;
break;
case kASeqPro:
yyResult = yyCheckChild (yyt, yyt->ASeqPro.Next, kSeqProList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ASeqPro.Protocol, kProtocol, "Protocol") && yyResult;
break;
case kBinary:
yyResult = yyCheckChild (yyt, yyt->Binary.Lop, kExpr, "Lop") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Binary.Rop, kExpr, "Rop") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Binary.Op, kOp, "Op") && yyResult;
break;
case kUnary:
yyResult = yyCheckChild (yyt, yyt->Unary.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Unary.Op, kOp, "Op") && yyResult;
break;
case kConst:
yyResult = yyCheckChild (yyt, yyt->Const.Type, kType, "Type") && yyResult;
break;
case kValof:
yyResult = yyCheckChild (yyt, yyt->Valof.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Valof.Process, kProcess, "Process") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Valof.ExprList, kExprList, "ExprList") && yyResult;
break;
case kFuncCall:
yyResult = yyCheckChild (yyt, yyt->FuncCall.ExprList, kExprList, "ExprList") && yyResult;
break;
case kElementalExpr:
yyResult = yyCheckChild (yyt, yyt->ElementalExpr.Elemental, kElemental, "Elemental") && yyResult;
break;
case kConvert:
yyResult = yyCheckChild (yyt, yyt->Convert.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Convert.Expr, kExpr, "Expr") && yyResult;
break;
case kRound:
yyResult = yyCheckChild (yyt, yyt->Round.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Round.Expr, kExpr, "Expr") && yyResult;
break;
case kTrunc:
yyResult = yyCheckChild (yyt, yyt->Trunc.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Trunc.Expr, kExpr, "Expr") && yyResult;
break;
case kMostPos:
yyResult = yyCheckChild (yyt, yyt->MostPos.Type, kType, "Type") && yyResult;
break;
case kMostNeg:
yyResult = yyCheckChild (yyt, yyt->MostNeg.Type, kType, "Type") && yyResult;
break;
case kCountArray:
yyResult = yyCheckChild (yyt, yyt->CountArray.Length, kExpr, "Length") && yyResult;
yyResult = yyCheckChild (yyt, yyt->CountArray.Expr, kExpr, "Expr") && yyResult;
break;
case kBytesInType:
yyResult = yyCheckChild (yyt, yyt->BytesInType.Type, kType, "Type") && yyResult;
break;
case kBytesInExpr:
yyResult = yyCheckChild (yyt, yyt->BytesInExpr.Expr, kExpr, "Expr") && yyResult;
break;
case kSizeOf:
yyResult = yyCheckChild (yyt, yyt->SizeOf.Type, kType, "Type") && yyResult;
break;
case kOffsetOf:
yyResult = yyCheckChild (yyt, yyt->OffsetOf.Type, kType, "Type") && yyResult;
yyResult = yyCheckChild (yyt, yyt->OffsetOf.UnmappedIdentifier, kUnmappedIdentifier, "UnmappedIdentifier") && yyResult;
break;
case kString:
yyResult = yyCheckChild (yyt, yyt->String.Type, kType, "Type") && yyResult;
break;
case kStringSubscript:
yyResult = yyCheckChild (yyt, yyt->StringSubscript.Next, kStringElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->StringSubscript.Expr, kExpr, "Expr") && yyResult;
break;
case kStringRange:
yyResult = yyCheckChild (yyt, yyt->StringRange.Next, kStringElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->StringRange.From, kExpr, "From") && yyResult;
yyResult = yyCheckChild (yyt, yyt->StringRange.For, kExpr, "For") && yyResult;
break;
case kTable:
yyResult = yyCheckChild (yyt, yyt->Table.ExprList, kExprList, "ExprList") && yyResult;
break;
case kUserLiteral:
yyResult = yyCheckChild (yyt, yyt->UserLiteral.ExprList, kExprList, "ExprList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->UserLiteral.Type, kType, "Type") && yyResult;
break;
case kTableExpr:
yyResult = yyCheckChild (yyt, yyt->TableExpr.Expr, kExpr, "Expr") && yyResult;
break;
case kTableSubscript:
yyResult = yyCheckChild (yyt, yyt->TableSubscript.Next, kTableElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->TableSubscript.Expr, kExpr, "Expr") && yyResult;
break;
case kTableRange:
yyResult = yyCheckChild (yyt, yyt->TableRange.Next, kTableElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->TableRange.From, kExpr, "From") && yyResult;
yyResult = yyCheckChild (yyt, yyt->TableRange.For, kExpr, "For") && yyResult;
break;
case kElementAccess:
yyResult = yyCheckChild (yyt, yyt->ElementAccess.Next, kElement, "Next") && yyResult;
break;
case kSubscript:
yyResult = yyCheckChild (yyt, yyt->Subscript.Next, kElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Subscript.Expr, kExpr, "Expr") && yyResult;
break;
case kRange:
yyResult = yyCheckChild (yyt, yyt->Range.Next, kElement, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Range.From, kExpr, "From") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Range.For, kExpr, "For") && yyResult;
break;
case kAnExpr:
yyResult = yyCheckChild (yyt, yyt->AnExpr.Next, kExprList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnExpr.Expr, kExpr, "Expr") && yyResult;
break;
case kProcess:
yyResult = yyCheckChild (yyt, yyt->Process.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kAssign:
yyResult = yyCheckChild (yyt, yyt->Assign.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Assign.Lval, kExprList, "Lval") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Assign.Rval, kExprList, "Rval") && yyResult;
break;
case kAltType:
yyResult = yyCheckChild (yyt, yyt->AltType.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kAlt:
yyResult = yyCheckChild (yyt, yyt->Alt.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Alt.AltList, kAltList, "AltList") && yyResult;
break;
case kAltRep:
yyResult = yyCheckChild (yyt, yyt->AltRep.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltRep.Replicator, kReplicator, "Replicator") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltRep.AltList, kAltList, "AltList") && yyResult;
break;
case kParType:
yyResult = yyCheckChild (yyt, yyt->ParType.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kPar:
yyResult = yyCheckChild (yyt, yyt->Par.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Par.ProcList, kProcList, "ProcList") && yyResult;
break;
case kParRep:
yyResult = yyCheckChild (yyt, yyt->ParRep.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ParRep.Replicator, kReplicator, "Replicator") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ParRep.Process, kProcess, "Process") && yyResult;
break;
case kProcessor:
yyResult = yyCheckChild (yyt, yyt->Processor.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Processor.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Processor.Process, kProcess, "Process") && yyResult;
break;
case kIfType:
yyResult = yyCheckChild (yyt, yyt->IfType.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kIf:
yyResult = yyCheckChild (yyt, yyt->If.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->If.ChoiceList, kChoiceList, "ChoiceList") && yyResult;
break;
case kIfRep:
yyResult = yyCheckChild (yyt, yyt->IfRep.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->IfRep.Replicator, kReplicator, "Replicator") && yyResult;
yyResult = yyCheckChild (yyt, yyt->IfRep.ChoiceList, kChoiceList, "ChoiceList") && yyResult;
break;
case kInput:
yyResult = yyCheckChild (yyt, yyt->Input.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Input.Element, kElement, "Element") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Input.ExprList, kExprList, "ExprList") && yyResult;
break;
case kOutput:
yyResult = yyCheckChild (yyt, yyt->Output.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Output.Element, kElement, "Element") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Output.ExprList, kExprList, "ExprList") && yyResult;
break;
case kCaseIn:
yyResult = yyCheckChild (yyt, yyt->CaseIn.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->CaseIn.Element, kElement, "Element") && yyResult;
yyResult = yyCheckChild (yyt, yyt->CaseIn.CaseList, kCaseList, "CaseList") && yyResult;
break;
case kAfter:
yyResult = yyCheckChild (yyt, yyt->After.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->After.Element, kElement, "Element") && yyResult;
yyResult = yyCheckChild (yyt, yyt->After.Expr, kExpr, "Expr") && yyResult;
break;
case kSeqRep:
yyResult = yyCheckChild (yyt, yyt->SeqRep.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->SeqRep.Replicator, kReplicator, "Replicator") && yyResult;
yyResult = yyCheckChild (yyt, yyt->SeqRep.Process, kProcess, "Process") && yyResult;
break;
case kSeq:
yyResult = yyCheckChild (yyt, yyt->Seq.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Seq.ProcList, kProcList, "ProcList") && yyResult;
break;
case kCase:
yyResult = yyCheckChild (yyt, yyt->Case.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Case.Selector, kExpr, "Selector") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Case.ChoiceList, kChoiceList, "ChoiceList") && yyResult;
break;
case kWhile:
yyResult = yyCheckChild (yyt, yyt->While.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->While.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->While.Process, kProcess, "Process") && yyResult;
break;
case kSkip:
yyResult = yyCheckChild (yyt, yyt->Skip.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kStop:
yyResult = yyCheckChild (yyt, yyt->Stop.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kInstance:
yyResult = yyCheckChild (yyt, yyt->Instance.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->Instance.ExprList, kExprList, "ExprList") && yyResult;
break;
case kCCode:
yyResult = yyCheckChild (yyt, yyt->CCode.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kNullProcess:
yyResult = yyCheckChild (yyt, yyt->NullProcess.SpecList, kSpecList, "SpecList") && yyResult;
break;
case kAnAlts:
yyResult = yyCheckChild (yyt, yyt->AnAlts.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnAlts.Next, kAltList, "Next") && yyResult;
break;
case kAnAlt:
yyResult = yyCheckChild (yyt, yyt->AnAlt.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnAlt.Next, kAltList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnAlt.Guard, kGuard, "Guard") && yyResult;
break;
case kANewAlt:
yyResult = yyCheckChild (yyt, yyt->ANewAlt.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ANewAlt.Next, kAltList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ANewAlt.AltType, kAltType, "AltType") && yyResult;
break;
case kGuard:
yyResult = yyCheckChild (yyt, yyt->Guard.Expr, kExpr, "Expr") && yyResult;
break;
case kAltInput:
yyResult = yyCheckChild (yyt, yyt->AltInput.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltInput.Input, kInput, "Input") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltInput.Process, kProcess, "Process") && yyResult;
break;
case kAltTimer:
yyResult = yyCheckChild (yyt, yyt->AltTimer.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltTimer.After, kAfter, "After") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltTimer.Process, kProcess, "Process") && yyResult;
break;
case kAltCaseIn:
yyResult = yyCheckChild (yyt, yyt->AltCaseIn.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AltCaseIn.CaseIn, kCaseIn, "CaseIn") && yyResult;
break;
case kNoInput:
yyResult = yyCheckChild (yyt, yyt->NoInput.Expr, kExpr, "Expr") && yyResult;
yyResult = yyCheckChild (yyt, yyt->NoInput.Process, kProcess, "Process") && yyResult;
break;
case kAChoices:
yyResult = yyCheckChild (yyt, yyt->AChoices.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AChoices.Next, kChoiceList, "Next") && yyResult;
break;
case kAChoice:
yyResult = yyCheckChild (yyt, yyt->AChoice.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AChoice.Next, kChoiceList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AChoice.ExprList, kExprList, "ExprList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AChoice.Process, kProcess, "Process") && yyResult;
break;
case kANewIf:
yyResult = yyCheckChild (yyt, yyt->ANewIf.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ANewIf.Next, kChoiceList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ANewIf.IfType, kIfType, "IfType") && yyResult;
break;
case kAProcess:
yyResult = yyCheckChild (yyt, yyt->AProcess.Next, kProcList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AProcess.Process, kProcess, "Process") && yyResult;
break;
case kACase:
yyResult = yyCheckChild (yyt, yyt->ACase.Next, kCaseList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ACase.SpecList, kSpecList, "SpecList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ACase.ExprList, kExprList, "ExprList") && yyResult;
yyResult = yyCheckChild (yyt, yyt->ACase.Process, kProcess, "Process") && yyResult;
break;
case kAnAccess:
yyResult = yyCheckChild (yyt, yyt->AnAccess.Next, kAccessList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnAccess.ElementList, kElementList, "ElementList") && yyResult;
break;
case kAnElement:
yyResult = yyCheckChild (yyt, yyt->AnElement.Next, kElementList, "Next") && yyResult;
yyResult = yyCheckChild (yyt, yyt->AnElement.Element, kElement, "Element") && yyResult;
break;
 default: ;
 }
 return yyResult;
}

# define yyyWrite	1
# define yyyRead	2
# define yyyQuit	3

static char yyyString [32], yyCh;
static int yyLength, yyState;

static bool yyyIsEqual
# if defined __STDC__ | defined __cplusplus
 (char * yya)
# else
 (yya) char * yya;
# endif
{
 register int yyi;
 if (yyLength >= 0 && yyyString [yyLength] == ' ') {
  if (yyLength != strlen (yya)) return false;
  for (yyi = 0; yyi < yyLength; yyi ++)
   if (yyyString [yyi] != yya [yyi]) return false;
 } else {
  if (yyLength >= strlen (yya)) return false;
  for (yyi = 0; yyi <= yyLength; yyi ++)
   if (yyyString [yyi] != yya [yyi]) return false;
 }
 return true;
}

void QueryTree
# if defined __STDC__ | defined __cplusplus
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyState = yyyWrite;
 for (;;) {
  switch (yyState) {
  case yyyQuit : return;
  case yyyWrite: WriteTreeNode (stdout, yyt); yyState = yyyRead;
  case yyyRead : (void) printf ("? "); yyLength = -1; yyCh = getc (stdin);
   while (yyCh != '\n' && yyCh > 0)
    { yyyString [++ yyLength] = yyCh; yyCh = getc (stdin); }
   if (yyCh < 0) { (void) fputs ("QueryTree: eof reached\n", stderr);
    yyState = yyyQuit; return; }
   if      (yyyIsEqual ("parent")) { yyState = yyyWrite; return; }
   else if (yyyIsEqual ("quit"  )) { yyState = yyyQuit ; return; }
   else if (yyt != NoTree) {
    switch (yyt->Kind) {
case kStart: if (false) ;
else if (yyyIsEqual ("Process")) QueryTree (yyt->Start.Process);
break;
case kASpec: if (false) ;
else if (yyyIsEqual ("Spec")) QueryTree (yyt->ASpec.Spec);
else if (yyyIsEqual ("Next")) QueryTree (yyt->ASpec.Next);
break;
case kDeclType: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->DeclType.Type);
break;
case kDecl: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Decl.Type);
else if (yyyIsEqual ("IdList")) QueryTree (yyt->Decl.IdList);
break;
case kIs: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Is.Type);
else if (yyyIsEqual ("Elemental")) QueryTree (yyt->Is.Elemental);
break;
case kAVal: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->AVal.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->AVal.Expr);
break;
case kVal: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Val.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Val.Expr);
break;
case kValRetypes: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->ValRetypes.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->ValRetypes.Expr);
break;
case kValReshapes: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->ValReshapes.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->ValReshapes.Expr);
break;
case kRetypes: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Retypes.Type);
else if (yyyIsEqual ("Elemental")) QueryTree (yyt->Retypes.Elemental);
break;
case kReshapes: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Reshapes.Type);
else if (yyyIsEqual ("Elemental")) QueryTree (yyt->Reshapes.Elemental);
break;
case kDefSeqPro: if (false) ;
else if (yyyIsEqual ("SeqProList")) QueryTree (yyt->DefSeqPro.SeqProList);
break;
case kDefTagPro: if (false) ;
else if (yyyIsEqual ("TagProList")) QueryTree (yyt->DefTagPro.TagProList);
break;
case kDefAProc: if (false) ;
else if (yyyIsEqual ("Formals")) QueryTree (yyt->DefAProc.Formals);
break;
case kDefProc: if (false) ;
else if (yyyIsEqual ("Formals")) QueryTree (yyt->DefProc.Formals);
else if (yyyIsEqual ("Process")) QueryTree (yyt->DefProc.Process);
break;
case kInlineDefProc: if (false) ;
else if (yyyIsEqual ("Formals")) QueryTree (yyt->InlineDefProc.Formals);
else if (yyyIsEqual ("Process")) QueryTree (yyt->InlineDefProc.Process);
break;
case kPrototypeProc: if (false) ;
else if (yyyIsEqual ("Formals")) QueryTree (yyt->PrototypeProc.Formals);
break;
case kDefAFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->DefAFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->DefAFunc.Formals);
break;
case kDefFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->DefFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->DefFunc.Formals);
else if (yyyIsEqual ("Valof")) QueryTree (yyt->DefFunc.Valof);
break;
case kInlineDefFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->InlineDefFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->InlineDefFunc.Formals);
else if (yyyIsEqual ("Valof")) QueryTree (yyt->InlineDefFunc.Valof);
break;
case kDefIsFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->DefIsFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->DefIsFunc.Formals);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->DefIsFunc.ExprList);
break;
case kInlineDefIsFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->InlineDefIsFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->InlineDefIsFunc.Formals);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->InlineDefIsFunc.ExprList);
break;
case kPrototypeFunc: if (false) ;
else if (yyyIsEqual ("TypeList")) QueryTree (yyt->PrototypeFunc.TypeList);
else if (yyyIsEqual ("Formals")) QueryTree (yyt->PrototypeFunc.Formals);
break;
case kAFormal: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AFormal.Next);
else if (yyyIsEqual ("By")) QueryTree (yyt->AFormal.By);
else if (yyyIsEqual ("Type")) QueryTree (yyt->AFormal.Type);
break;
case kReplicator: if (false) ;
else if (yyyIsEqual ("From")) QueryTree (yyt->Replicator.From);
else if (yyyIsEqual ("For")) QueryTree (yyt->Replicator.For);
break;
case kATagPro: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->ATagPro.Next);
else if (yyyIsEqual ("SeqProList")) QueryTree (yyt->ATagPro.SeqProList);
break;
case kAnId: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnId.Next);
break;
case kArray: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Array.Expr);
else if (yyyIsEqual ("Type")) QueryTree (yyt->Array.Type);
break;
case kUserArray: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->UserArray.Expr);
else if (yyyIsEqual ("Type")) QueryTree (yyt->UserArray.Type);
break;
case kChan: if (false) ;
else if (yyyIsEqual ("Protocol")) QueryTree (yyt->Chan.Protocol);
break;
case kUserStructTypes: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserStructTypes.FieldList);
break;
case kUserDataTypes: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserDataTypes.FieldList);
break;
case kUserRecord: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserRecord.FieldList);
break;
case kUserPackedRecord: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserPackedRecord.FieldList);
break;
case kUserUnion: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserUnion.FieldList);
break;
case kUserChanRecord: if (false) ;
else if (yyyIsEqual ("FieldList")) QueryTree (yyt->UserChanRecord.FieldList);
break;
case kAType: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AType.Next);
else if (yyyIsEqual ("Type")) QueryTree (yyt->AType.Type);
break;
case kAField: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AField.Next);
else if (yyyIsEqual ("Type")) QueryTree (yyt->AField.Type);
break;
case kSimple: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Simple.Type);
break;
case kCounted: if (false) ;
else if (yyyIsEqual ("Length")) QueryTree (yyt->Counted.Length);
else if (yyyIsEqual ("Type")) QueryTree (yyt->Counted.Type);
break;
case kASeqPro: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->ASeqPro.Next);
else if (yyyIsEqual ("Protocol")) QueryTree (yyt->ASeqPro.Protocol);
break;
case kBinary: if (false) ;
else if (yyyIsEqual ("Lop")) QueryTree (yyt->Binary.Lop);
else if (yyyIsEqual ("Rop")) QueryTree (yyt->Binary.Rop);
else if (yyyIsEqual ("Op")) QueryTree (yyt->Binary.Op);
break;
case kUnary: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Unary.Expr);
else if (yyyIsEqual ("Op")) QueryTree (yyt->Unary.Op);
break;
case kConst: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Const.Type);
break;
case kValof: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Valof.SpecList);
else if (yyyIsEqual ("Process")) QueryTree (yyt->Valof.Process);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->Valof.ExprList);
break;
case kFuncCall: if (false) ;
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->FuncCall.ExprList);
break;
case kElementalExpr: if (false) ;
else if (yyyIsEqual ("Elemental")) QueryTree (yyt->ElementalExpr.Elemental);
break;
case kConvert: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Convert.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Convert.Expr);
break;
case kRound: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Round.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Round.Expr);
break;
case kTrunc: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->Trunc.Type);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Trunc.Expr);
break;
case kMostPos: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->MostPos.Type);
break;
case kMostNeg: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->MostNeg.Type);
break;
case kCountArray: if (false) ;
else if (yyyIsEqual ("Length")) QueryTree (yyt->CountArray.Length);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->CountArray.Expr);
break;
case kBytesInType: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->BytesInType.Type);
break;
case kBytesInExpr: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->BytesInExpr.Expr);
break;
case kSizeOf: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->SizeOf.Type);
break;
case kOffsetOf: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->OffsetOf.Type);
else if (yyyIsEqual ("UnmappedIdentifier")) QueryTree (yyt->OffsetOf.UnmappedIdentifier);
break;
case kString: if (false) ;
else if (yyyIsEqual ("Type")) QueryTree (yyt->String.Type);
break;
case kStringSubscript: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->StringSubscript.Next);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->StringSubscript.Expr);
break;
case kStringRange: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->StringRange.Next);
else if (yyyIsEqual ("From")) QueryTree (yyt->StringRange.From);
else if (yyyIsEqual ("For")) QueryTree (yyt->StringRange.For);
break;
case kTable: if (false) ;
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->Table.ExprList);
break;
case kUserLiteral: if (false) ;
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->UserLiteral.ExprList);
else if (yyyIsEqual ("Type")) QueryTree (yyt->UserLiteral.Type);
break;
case kTableExpr: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->TableExpr.Expr);
break;
case kTableSubscript: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->TableSubscript.Next);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->TableSubscript.Expr);
break;
case kTableRange: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->TableRange.Next);
else if (yyyIsEqual ("From")) QueryTree (yyt->TableRange.From);
else if (yyyIsEqual ("For")) QueryTree (yyt->TableRange.For);
break;
case kElementAccess: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->ElementAccess.Next);
break;
case kSubscript: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->Subscript.Next);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Subscript.Expr);
break;
case kRange: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->Range.Next);
else if (yyyIsEqual ("From")) QueryTree (yyt->Range.From);
else if (yyyIsEqual ("For")) QueryTree (yyt->Range.For);
break;
case kAnExpr: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnExpr.Next);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->AnExpr.Expr);
break;
case kProcess: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Process.SpecList);
break;
case kAssign: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Assign.SpecList);
else if (yyyIsEqual ("Lval")) QueryTree (yyt->Assign.Lval);
else if (yyyIsEqual ("Rval")) QueryTree (yyt->Assign.Rval);
break;
case kAltType: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AltType.SpecList);
break;
case kAlt: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Alt.SpecList);
else if (yyyIsEqual ("AltList")) QueryTree (yyt->Alt.AltList);
break;
case kAltRep: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AltRep.SpecList);
else if (yyyIsEqual ("Replicator")) QueryTree (yyt->AltRep.Replicator);
else if (yyyIsEqual ("AltList")) QueryTree (yyt->AltRep.AltList);
break;
case kParType: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->ParType.SpecList);
break;
case kPar: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Par.SpecList);
else if (yyyIsEqual ("ProcList")) QueryTree (yyt->Par.ProcList);
break;
case kParRep: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->ParRep.SpecList);
else if (yyyIsEqual ("Replicator")) QueryTree (yyt->ParRep.Replicator);
else if (yyyIsEqual ("Process")) QueryTree (yyt->ParRep.Process);
break;
case kProcessor: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Processor.SpecList);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Processor.Expr);
else if (yyyIsEqual ("Process")) QueryTree (yyt->Processor.Process);
break;
case kIfType: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->IfType.SpecList);
break;
case kIf: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->If.SpecList);
else if (yyyIsEqual ("ChoiceList")) QueryTree (yyt->If.ChoiceList);
break;
case kIfRep: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->IfRep.SpecList);
else if (yyyIsEqual ("Replicator")) QueryTree (yyt->IfRep.Replicator);
else if (yyyIsEqual ("ChoiceList")) QueryTree (yyt->IfRep.ChoiceList);
break;
case kInput: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Input.SpecList);
else if (yyyIsEqual ("Element")) QueryTree (yyt->Input.Element);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->Input.ExprList);
break;
case kOutput: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Output.SpecList);
else if (yyyIsEqual ("Element")) QueryTree (yyt->Output.Element);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->Output.ExprList);
break;
case kCaseIn: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->CaseIn.SpecList);
else if (yyyIsEqual ("Element")) QueryTree (yyt->CaseIn.Element);
else if (yyyIsEqual ("CaseList")) QueryTree (yyt->CaseIn.CaseList);
break;
case kAfter: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->After.SpecList);
else if (yyyIsEqual ("Element")) QueryTree (yyt->After.Element);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->After.Expr);
break;
case kSeqRep: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->SeqRep.SpecList);
else if (yyyIsEqual ("Replicator")) QueryTree (yyt->SeqRep.Replicator);
else if (yyyIsEqual ("Process")) QueryTree (yyt->SeqRep.Process);
break;
case kSeq: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Seq.SpecList);
else if (yyyIsEqual ("ProcList")) QueryTree (yyt->Seq.ProcList);
break;
case kCase: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Case.SpecList);
else if (yyyIsEqual ("Selector")) QueryTree (yyt->Case.Selector);
else if (yyyIsEqual ("ChoiceList")) QueryTree (yyt->Case.ChoiceList);
break;
case kWhile: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->While.SpecList);
else if (yyyIsEqual ("Expr")) QueryTree (yyt->While.Expr);
else if (yyyIsEqual ("Process")) QueryTree (yyt->While.Process);
break;
case kSkip: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Skip.SpecList);
break;
case kStop: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Stop.SpecList);
break;
case kInstance: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->Instance.SpecList);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->Instance.ExprList);
break;
case kCCode: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->CCode.SpecList);
break;
case kNullProcess: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->NullProcess.SpecList);
break;
case kAnAlts: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AnAlts.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnAlts.Next);
break;
case kAnAlt: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AnAlt.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnAlt.Next);
else if (yyyIsEqual ("Guard")) QueryTree (yyt->AnAlt.Guard);
break;
case kANewAlt: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->ANewAlt.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->ANewAlt.Next);
else if (yyyIsEqual ("AltType")) QueryTree (yyt->ANewAlt.AltType);
break;
case kGuard: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->Guard.Expr);
break;
case kAltInput: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->AltInput.Expr);
else if (yyyIsEqual ("Input")) QueryTree (yyt->AltInput.Input);
else if (yyyIsEqual ("Process")) QueryTree (yyt->AltInput.Process);
break;
case kAltTimer: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->AltTimer.Expr);
else if (yyyIsEqual ("After")) QueryTree (yyt->AltTimer.After);
else if (yyyIsEqual ("Process")) QueryTree (yyt->AltTimer.Process);
break;
case kAltCaseIn: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->AltCaseIn.Expr);
else if (yyyIsEqual ("CaseIn")) QueryTree (yyt->AltCaseIn.CaseIn);
break;
case kNoInput: if (false) ;
else if (yyyIsEqual ("Expr")) QueryTree (yyt->NoInput.Expr);
else if (yyyIsEqual ("Process")) QueryTree (yyt->NoInput.Process);
break;
case kAChoices: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AChoices.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->AChoices.Next);
break;
case kAChoice: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->AChoice.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->AChoice.Next);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->AChoice.ExprList);
else if (yyyIsEqual ("Process")) QueryTree (yyt->AChoice.Process);
break;
case kANewIf: if (false) ;
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->ANewIf.SpecList);
else if (yyyIsEqual ("Next")) QueryTree (yyt->ANewIf.Next);
else if (yyyIsEqual ("IfType")) QueryTree (yyt->ANewIf.IfType);
break;
case kAProcess: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AProcess.Next);
else if (yyyIsEqual ("Process")) QueryTree (yyt->AProcess.Process);
break;
case kACase: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->ACase.Next);
else if (yyyIsEqual ("SpecList")) QueryTree (yyt->ACase.SpecList);
else if (yyyIsEqual ("ExprList")) QueryTree (yyt->ACase.ExprList);
else if (yyyIsEqual ("Process")) QueryTree (yyt->ACase.Process);
break;
case kAnAccess: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnAccess.Next);
else if (yyyIsEqual ("ElementList")) QueryTree (yyt->AnAccess.ElementList);
break;
case kAnElement: if (false) ;
else if (yyyIsEqual ("Next")) QueryTree (yyt->AnElement.Next);
else if (yyyIsEqual ("Element")) QueryTree (yyt->AnElement.Element);
break;
    default: ;
    }
   }
  }
 }
}

void BeginTree ()
{
}

void CloseTree ()
{
}
