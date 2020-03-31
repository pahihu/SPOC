# ifndef yyTree
# define yyTree

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

/* line 22 "" */

  #include "Scanner.h"
  #include "Errors.h"

# include <stdint.h>


# ifndef bool
# define bool char
# endif
# define NoTree (tTree) 0L
# define kStart 1
# define kSpecList 2
# define kNoSpec 3
# define kASpec 4
# define kSpec 5
# define kDeclType 6
# define kDecl 7
# define kIs 8
# define kAVal 9
# define kVal 10
# define kValRetypes 11
# define kValReshapes 12
# define kRetypes 13
# define kReshapes 14
# define kDefAPro 15
# define kDefSeqPro 16
# define kDefTagPro 17
# define kDefAProc 18
# define kDefProc 19
# define kInlineDefProc 20
# define kPrototypeProc 21
# define kDefAFunc 22
# define kDefFunc 23
# define kInlineDefFunc 24
# define kDefIsFunc 25
# define kInlineDefIsFunc 26
# define kPrototypeFunc 27
# define kNullSpec 28
# define kHCode 29
# define kCallFrame 30
# define kParFrame 31
# define kFormals 32
# define kNoFormal 33
# define kNoFormals 34
# define kAFormal 35
# define kReplicator 36
# define kTagProList 37
# define kNoTagPro 38
# define kATagPro 39
# define kBy 40
# define kByVal 41
# define kByRef 42
# define kByUndef 43
# define kIdList 44
# define kNoId 45
# define kAnId 46
# define kType 47
# define kArray 48
# define kUserArray 49
# define kChan 50
# define kTimer 51
# define kUntyped 52
# define kUntypedByte 53
# define kUntypedReal 54
# define kUntypedInteger 55
# define kUntypedInt 56
# define kUntypedHex 57
# define kPrimitive 58
# define kNumeric 59
# define kInteger 60
# define kInt 61
# define kUserInt 62
# define kInt16 63
# define kUserInt16 64
# define kInt32 65
# define kUserInt32 66
# define kInt64 67
# define kUserInt64 68
# define kReal 69
# define kReal32 70
# define kUserReal32 71
# define kReal64 72
# define kUserReal64 73
# define kByte 74
# define kUserByte 75
# define kBool 76
# define kUserBool 77
# define kUserStructTypes 78
# define kUserDataTypes 79
# define kUserRecord 80
# define kUserPackedRecord 81
# define kUserUnion 82
# define kUserChanRecord 83
# define kTypeList 84
# define kNoType 85
# define kAType 86
# define kFieldList 87
# define kNoField 88
# define kAField 89
# define kProtocol 90
# define kAny 91
# define kNamed 92
# define kSimple 93
# define kCounted 94
# define kSeqProList 95
# define kNoSeqPro 96
# define kASeqPro 97
# define kExpr 98
# define kBinary 99
# define kUnary 100
# define kConst 101
# define kValof 102
# define kFuncCall 103
# define kElementalExpr 104
# define kDefault 105
# define kConvert 106
# define kRound 107
# define kTrunc 108
# define kMostPos 109
# define kMostNeg 110
# define kCountArray 111
# define kUnBound 112
# define kBytesIn 113
# define kBytesInType 114
# define kBytesInExpr 115
# define kSizeOf 116
# define kOffsetOf 117
# define kStringElement 118
# define kString 119
# define kStringSubscript 120
# define kStringRange 121
# define kElemental 122
# define kTableElement 123
# define kTable 124
# define kUserLiteral 125
# define kTableExpr 126
# define kTableSubscript 127
# define kTableRange 128
# define kElement 129
# define kIdentifier 130
# define kUnmappedIdentifier 131
# define kElementAccess 132
# define kSubscript 133
# define kRange 134
# define kExprList 135
# define kNoExpr 136
# define kAnExpr 137
# define kOp 138
# define kRelOp 139
# define kLess 140
# define kGreater 141
# define kLessEqual 142
# define kGreaterEqual 143
# define kEqual 144
# define kNotEqual 145
# define kMathsOp 146
# define kAdd 147
# define kSubtract 148
# define kMultiply 149
# define kDivide 150
# define kRem 151
# define kBitOp 152
# define kBitAnd 153
# define kBitOr 154
# define kBitEor 155
# define kShiftOp 156
# define kLeftShift 157
# define kRightShift 158
# define kUniOp 159
# define kSize 160
# define kBitNot 161
# define kNot 162
# define kClockOp 163
# define kPlus 164
# define kMinus 165
# define kTimes 166
# define kLogOp 167
# define kAnd 168
# define kOr 169
# define kAfterOp 170
# define kProcess 171
# define kAssign 172
# define kAltType 173
# define kAlt 174
# define kAltRep 175
# define kParType 176
# define kPar 177
# define kParRep 178
# define kProcessor 179
# define kIfType 180
# define kIf 181
# define kIfRep 182
# define kInput 183
# define kOutput 184
# define kCaseIn 185
# define kAfter 186
# define kSeqRep 187
# define kSeq 188
# define kCase 189
# define kWhile 190
# define kSkip 191
# define kStop 192
# define kInstance 193
# define kCCode 194
# define kNullProcess 195
# define kAltList 196
# define kNoAlt 197
# define kAnAlts 198
# define kAnAlt 199
# define kANewAlt 200
# define kGuard 201
# define kAltInput 202
# define kAltTimer 203
# define kAltCaseIn 204
# define kNoInput 205
# define kChoiceList 206
# define kNoChoice 207
# define kAChoices 208
# define kAChoice 209
# define kANewIf 210
# define kProcList 211
# define kNoProcess 212
# define kAProcess 213
# define kCaseList 214
# define kNoCase 215
# define kACase 216
# define kAccessList 217
# define kAnAccess 218
# define kNoAccess 219
# define kElementList 220
# define kAnElement 221
# define kNoElement 222

typedef unsigned char Tree_tKind;
typedef unsigned short Tree_tMark;
typedef unsigned short Tree_tLabel;
typedef union Tree_Node * tTree;
typedef void (* Tree_tProcTree) ();
/* line 29 "" */

  #include "Symbol.h"

  typedef enum { xxPrototype, xxCDirectives, xxLibrary, xxUsageWarnings,		/* Compile time modes */
		 xxCheckConversions, xxCheckInternals, xxCheckArrays, xxCheckRanges,	/* Error checking modes */
		 xxOccamLines, xxOccamDebug, xxLinesAsComments, 			/* Debugging */
		 xxSupportINT64, xxLittleEndian, xx16bitINT, xxEmulate,	 		/* Target Architecture */
		 xxSimulatedServer, xxCallF90,
		 xxInmosExtensions, xxUserDataTypes, xxSupportPlaced, xxNoFreeVars, xxPrettyC,
		 xxNoUsageChecking
	       } eExtnModes;

  extern int ExtnModes;
  #define CHK_Mode(x,p) if (!((1<<x)&ExtnModes)) Message("Grammar not allowed under current mode",xxError,p)
  #define ADD_Mode(x)   (ExtnModes|=(1<<x))
  #define REM_Mode(x)   (ExtnModes&=(~(1<<x)))
  #define TOG_Mode(x)   (ExtnModes^=(1<<x))
  #define IS_Mode(x)    ((1<<x)&ExtnModes)
  #define BIT_Mode(x)   (1<<x)
  #define GET_Mode()    (ExtnModes)

  #define SET_Mode(x,b) ((b) ? (ADD_Mode(x)) : (REM_Mode(x)))

  #define ADD_Flag(x,f)   ((f)|=(1<<x))
  #define REM_Flag(x,f)   ((f)&=(~(1<<x)))
  #define TOG_Flag(x,f)   ((f)^=(1<<x))
  #define IS_Flag(x,f)    ((1<<x)&(f))

  #undef putchar
  #undef getchar

/* line 626 "" */

  typedef tTree tType;

  typedef unsigned char *tConstArray;
  #define NoConstArray ((tConstArray)NULL)

  #define DoBndChk(S) S->Subscript.CB
  #define DoLwrBndChk(R) R->Range.CLB
  #define DoUprBndChk(R) R->Range.CUB

  #define ConstFlag(E) E.C
  #define IsConstInt(E) (Tree_IsType(ExprType(E),kInteger) && ConstFlag(E->Expr))
  #define ValConstInt(E) E->Expr.CV.Number

  #define ElementalConst(E) E->Elemental.CA
  #define StringConst(S) S->StringElement.CA

  #define IsConstExpr(E) ConstFlag(E->Expr)
  #define ValConstExpr(E) E->Expr.CV

  #define IsConstIntElement(E) (Tree_IsType(ElementType(E),kInteger) && ConstFlag(E->Element))
  #define ValConstIntElement(E) E->Element.CV.Number

  #define IsConstIntExprList(E) (Tree_IsType(ExprListType(E),kInteger) && ConstFlag(E->ExprList))
  #define IsConstExprList(E)    (ConstFlag(E->ExprList))

  #define ParType(P) (P->ParType.PType)
  #define AltIsPri(P) (P->AltType.Pri)
  typedef enum { xxPar, xxPriPar, xxPlacedPar } tParType;

  #define IsLVIdent(I) I->Identifier.LV
  #define ExprType(E) E->Expr.SynType
  #define ExprListType(E) E->ExprList.SynType
  #define ChoiceListType(E) E->ChoiceList.SynType
  #define ElementType(E) E->Element.SynType
  #define ElementalType(E) E->Elemental.SynType
  #define ExprFieldList(E) E->Expr.FLOut
  
  #define Writable(E) E->Element.W
  #define TempRequired(EL) EL->ExprList.TR

  typedef enum { xxUnused, xxUsed, xxInput, xxOutput, xxInOut } tUsage; 
  extern tUsage UpdateUsage(tUsage old, tUsage new);

  /*{{{  VUsage*/
  enum { xxxAccess,xxxLRead,xxxLWrite,xxxNLRead,xxxNLWrite,xxxAtomic };
  #define xxLRead   (1<<xxxLRead)
  #define xxLWrite  (1<<xxxLWrite)
  #define xxNLRead  (1<<xxxNLRead)
  #define xxNLWrite (1<<xxxNLWrite)
  #define xxAtomic  (1<<xxxAtomic)
  #define xxAccess  (1<<xxxAccess)
  
  #define AddVUsage(S,U)  { SymbolUsage(S) |= (U); }
  
  #define AddRVUsage(S,P) { tVUsage u = (SymbolDepth(S)<=SymbolDepth(P)) ?  xxNLRead :  xxLRead; AddVUsage(S,xxAccess | u); MarkRefParent(S,SymbolKind(P),xxNLRead); }
  #define AddWVUsage(S,P) { tVUsage u = (SymbolDepth(S)<=SymbolDepth(P)) ? xxNLWrite : xxLWrite; AddVUsage(S,xxAccess | u); MarkRefParent(S,SymbolKind(P),xxNLWrite);}
  /*}}}*/

  typedef enum { xxOccam, xxAtomicOccam, xxF90, xxF77, xxC } tLang;

/* line 853 "" */

  typedef tTree tChain;
  typedef tTree tFrame;
  typedef tTree tChoiceList;
  typedef tTree tIfType;

  extern FILE *yyf;

/* line 1017 "" */

  typedef struct tParUsageStruct tParUsageStruct;
  typedef tParUsageStruct* tParUsage;

  typedef struct tLinExpr tLinExpr;
  typedef tLinExpr* pLinExpr;
  #include "Omega.h"
  #include "ParUsage.h"


# ifndef Tree_NodeHead
# define Tree_NodeHead
# endif
typedef struct { Tree_tKind yyKind; Tree_tMark yyMark; Tree_NodeHead } Tree_tNodeHead;
typedef struct { Tree_tNodeHead yyHead; tTree Process; tChain CGNext; tSymbol OuterProc; } yStart;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int DCL; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } ySpecList;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int DCL; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } yNoSpec;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int DCL; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Spec; tTree Next; } yASpec;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } ySpec;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; } yDeclType;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tTree IdList; } yDecl;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Elemental; tUsage U; } yIs;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Expr; } yAVal;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Expr; } yVal;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Expr; } yValRetypes;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Expr; } yValReshapes;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Elemental; } yRetypes;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Type; tSymbol Sym; tTree Elemental; } yReshapes;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } yDefAPro;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree SeqProList; } yDefSeqPro;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TagProList; } yDefTagPro;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree Formals; tUsage U; tChain CGNext; } yDefAProc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree Formals; tUsage U; tChain CGNext; tTree Process; bool ATOMIC; bool isPar; } yDefProc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree Formals; tUsage U; tChain CGNext; tTree Process; bool ATOMIC; bool isPar; } yInlineDefProc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree Formals; tUsage U; tChain CGNext; tIdent Module; tLang Lang; } yPrototypeProc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; } yDefAFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; tTree Valof; bool ATOMIC; } yDefFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; tTree Valof; bool ATOMIC; } yInlineDefFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; tTree ExprList; bool ATOMIC; } yDefIsFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; tTree ExprList; bool ATOMIC; } yInlineDefIsFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree TypeList; tTree Formals; tUsage U; tChain CGNext; tIdent Module; tLang Lang; } yPrototypeFunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } yNullSpec;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tStringRef Code; tSymbol Scope; tChain CGNext; } yHCode;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tIdent Id; tSymbol Sym; } yCallFrame;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; int V1In; int V1Out; int VS; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tIdent TId; tIdent FId; tSymbol Sym; int Rep; } yParFrame;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; bool InPrototype; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } yFormals;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; bool InPrototype; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; } yNoFormal;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; bool InPrototype; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; } yNoFormals;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; bool InPrototype; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; tTree Next; tPosition Pos; tTree By; tTree Type; tSymbol Sym; tUsage U; } yAFormal;
typedef struct { Tree_tNodeHead yyHead; tSymbol Sym; tTree From; tTree For; int V1In; int V1Out; int VIn; int VOut; tSymbol Parent; bool AtomicSpec; tParUsage PUIn; tParUsage PUOut; bool SE; pLinExpr FromLE; pLinExpr ForLE; } yReplicator;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } yTagProList;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } yNoTagPro;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; tTree Next; tSymbol Sym; tTree SeqProList; } yATagPro;
typedef struct { Tree_tNodeHead yyHead; } yBy;
typedef struct { Tree_tNodeHead yyHead; } yByVal;
typedef struct { Tree_tNodeHead yyHead; } yByRef;
typedef struct { Tree_tNodeHead yyHead; } yByUndef;
typedef struct { Tree_tNodeHead yyHead; tSymbol Parent; bool AtomicSpec; } yIdList;
typedef struct { Tree_tNodeHead yyHead; tSymbol Parent; bool AtomicSpec; } yNoId;
typedef struct { Tree_tNodeHead yyHead; tSymbol Parent; bool AtomicSpec; tTree Next; tSymbol Sym; tUsage U; } yAnId;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yType;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tTree Expr; tTree Type; bool SE; pLinExpr LE; } yArray;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tTree Expr; tTree Type; bool SE; pLinExpr LE; tPosition Pos; tSymbol Sym; } yUserArray;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tTree Protocol; } yChan;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yTimer;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntyped;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntypedByte;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntypedReal;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntypedInteger;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntypedInt;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yUntypedHex;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yPrimitive;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yNumeric;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yInteger;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yInt;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserInt;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yInt16;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserInt16;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yInt32;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserInt32;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yInt64;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserInt64;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yReal;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yReal32;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserReal32;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yReal64;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserReal64;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yByte;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserByte;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yBool;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; } yUserBool;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserStructTypes;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserDataTypes;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserRecord;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserPackedRecord;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserUnion;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tPosition Pos; tSymbol Sym; tTree FieldList; } yUserChanRecord;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yTypeList;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; } yNoType;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VIn; int VOut; int D; tParUsage PUIn; tParUsage PUOut; tTree Next; tTree Type; } yAType;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } yFieldList;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } yNoField;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; tTree Next; tSymbol Sym; tTree Type; } yAField;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tParUsage PUIn; tParUsage PUOut; } yProtocol;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tParUsage PUIn; tParUsage PUOut; } yAny;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; } yNamed;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tParUsage PUIn; tParUsage PUOut; tTree Type; } ySimple;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tParUsage PUIn; tParUsage PUOut; tTree Length; tTree Type; } yCounted;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } ySeqProList;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; } yNoSeqPro;
typedef struct { Tree_tNodeHead yyHead; tParUsage PUIn; tParUsage PUOut; tTree Next; tTree Protocol; } yASeqPro;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; } yExpr;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Lop; tTree Rop; tTree Op; } yBinary;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Expr; tTree Op; } yUnary;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tValue Value; tTree Type; } yConst;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree SpecList; tTree Process; tTree ExprList; tSymbol Sym; tTree FuncCall; int VS; } yValof;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tSymbol Sym; tTree ExprList; int Depth; tFrame Frame; } yFuncCall;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Elemental; bool PAR; } yElementalExpr;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; } yDefault;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; tTree Expr; } yConvert;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; tTree Expr; } yRound;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; tTree Expr; } yTrunc;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; } yMostPos;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; } yMostNeg;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Length; tTree Expr; } yCountArray;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; } yUnBound;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; } yBytesIn;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; } yBytesInType;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Expr; } yBytesInExpr;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; } ySizeOf;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tTree Type; tTree UnmappedIdentifier; } yOffsetOf;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tConstArray CA; bool SE; } yStringElement;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tConstArray CA; bool SE; tStringRef Value; tTree Type; int Depth; tSymbol Sym; pLinExpr LE; } yString;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tConstArray CA; bool SE; tTree Next; tTree Expr; pLinExpr LE; } yStringSubscript;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; tValue CV; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; tConstArray CA; bool SE; tTree Next; tTree From; tTree For; pLinExpr FromLE; pLinExpr ForLE; } yStringRange;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; } yElemental;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; } yTableElement;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree ExprList; int Depth; tSymbol Sym; pLinExpr LE; } yTable;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree ExprList; int Depth; tSymbol Sym; pLinExpr LE; tTree Type; } yUserLiteral;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Expr; tSymbol Sym; } yTableExpr;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Next; tTree Expr; pLinExpr LE; } yTableSubscript;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Next; tTree From; tTree For; pLinExpr FromLE; pLinExpr ForLE; } yTableRange;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; } yElement;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tSymbol Sym; tSymbol Scope; } yIdentifier;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tSymbol Sym; tSymbol Scope; tIdent Ident; } yUnmappedIdentifier;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Next; } yElementAccess;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Next; tTree Expr; bool CB; pLinExpr LE; } ySubscript;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int W; int VIn; int VOut; bool C; tValue CV; tConstArray CA; tUsage U; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool LV; bool SE; tTree RVL; tTree Next; tTree From; tTree For; bool CLB; bool CUB; pLinExpr FromLE; pLinExpr ForLE; } yRange;
typedef struct { Tree_tNodeHead yyHead; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; bool TR; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; bool LV; } yExprList;
typedef struct { Tree_tNodeHead yyHead; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; bool TR; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; bool LV; } yNoExpr;
typedef struct { Tree_tNodeHead yyHead; tType DefType; tType FLIn; tType FLOut; int V1In; int V1Out; tType SynType; tType InhType; int VIn; int VOut; bool C; bool TR; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; bool LV; tTree Next; tTree Expr; } yAnExpr;
typedef struct { Tree_tNodeHead yyHead; } yOp;
typedef struct { Tree_tNodeHead yyHead; } yRelOp;
typedef struct { Tree_tNodeHead yyHead; } yLess;
typedef struct { Tree_tNodeHead yyHead; } yGreater;
typedef struct { Tree_tNodeHead yyHead; } yLessEqual;
typedef struct { Tree_tNodeHead yyHead; } yGreaterEqual;
typedef struct { Tree_tNodeHead yyHead; } yEqual;
typedef struct { Tree_tNodeHead yyHead; } yNotEqual;
typedef struct { Tree_tNodeHead yyHead; } yMathsOp;
typedef struct { Tree_tNodeHead yyHead; } yAdd;
typedef struct { Tree_tNodeHead yyHead; } ySubtract;
typedef struct { Tree_tNodeHead yyHead; } yMultiply;
typedef struct { Tree_tNodeHead yyHead; } yDivide;
typedef struct { Tree_tNodeHead yyHead; } yRem;
typedef struct { Tree_tNodeHead yyHead; } yBitOp;
typedef struct { Tree_tNodeHead yyHead; } yBitAnd;
typedef struct { Tree_tNodeHead yyHead; } yBitOr;
typedef struct { Tree_tNodeHead yyHead; } yBitEor;
typedef struct { Tree_tNodeHead yyHead; } yShiftOp;
typedef struct { Tree_tNodeHead yyHead; } yLeftShift;
typedef struct { Tree_tNodeHead yyHead; } yRightShift;
typedef struct { Tree_tNodeHead yyHead; } yUniOp;
typedef struct { Tree_tNodeHead yyHead; } ySize;
typedef struct { Tree_tNodeHead yyHead; } yBitNot;
typedef struct { Tree_tNodeHead yyHead; } yNot;
typedef struct { Tree_tNodeHead yyHead; } yClockOp;
typedef struct { Tree_tNodeHead yyHead; } yPlus;
typedef struct { Tree_tNodeHead yyHead; } yMinus;
typedef struct { Tree_tNodeHead yyHead; } yTimes;
typedef struct { Tree_tNodeHead yyHead; } yLogOp;
typedef struct { Tree_tNodeHead yyHead; } yAnd;
typedef struct { Tree_tNodeHead yyHead; } yOr;
typedef struct { Tree_tNodeHead yyHead; } yAfterOp;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yProcess;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Lval; tTree Rval; } yAssign;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool Pri; int ARC; } yAltType;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool Pri; int ARC; tTree AltList; } yAlt;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool Pri; int ARC; tTree Replicator; tTree AltList; } yAltRep;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tParType PType; } yParType;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tParType PType; tTree ProcList; } yPar;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tParType PType; tTree Replicator; tTree Process; tSymbol Sym; } yParRep;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Expr; tIdent Name; tTree Process; } yProcessor;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tChoiceList BackChoiceList; tIfType BackIfType; } yIfType;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tChoiceList BackChoiceList; tIfType BackIfType; tTree ChoiceList; } yIf;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tChoiceList BackChoiceList; tIfType BackIfType; tTree Replicator; tTree ChoiceList; } yIfRep;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Element; tTree ExprList; } yInput;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Element; tTree ExprList; } yOutput;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Element; tTree CaseList; } yCaseIn;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Element; tTree Expr; } yAfter;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Replicator; tTree Process; } ySeqRep;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree ProcList; } ySeq;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Selector; tTree ChoiceList; } yCase;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Expr; tTree Process; } yWhile;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } ySkip;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yStop;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tSymbol Sym; tTree ExprList; tSymbol Scope; tFrame Frame; } yInstance;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tStringRef Code; tSymbol Scope; } yCCode;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree SpecList; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; bool TLA; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yNullProcess;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; int ARC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yAltList;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; int ARC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yNoAlt;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; int ARC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; } yAnAlts;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; int ARC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; tTree Guard; int Label; } yAnAlt;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; int ARC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; tTree AltType; } yANewAlt;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree Expr; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yGuard;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree Expr; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Input; tTree Process; } yAltInput;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree Expr; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree After; tTree Process; } yAltTimer;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree Expr; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree CaseIn; } yAltCaseIn;
typedef struct { Tree_tNodeHead yyHead; tPosition Pos; tTree Expr; int V1In; int V1Out; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Process; } yNoInput;
typedef struct { Tree_tNodeHead yyHead; tType DefType; int V1In; int V1Out; tType SynType; tType InhType; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yChoiceList;
typedef struct { Tree_tNodeHead yyHead; tType DefType; int V1In; int V1Out; tType SynType; tType InhType; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yNoChoice;
typedef struct { Tree_tNodeHead yyHead; tType DefType; int V1In; int V1Out; tType SynType; tType InhType; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; } yAChoices;
typedef struct { Tree_tNodeHead yyHead; tType DefType; int V1In; int V1Out; tType SynType; tType InhType; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; tTree ExprList; tTree Process; int Label; } yAChoice;
typedef struct { Tree_tNodeHead yyHead; tType DefType; int V1In; int V1Out; tType SynType; tType InhType; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree SpecList; tTree Next; tTree IfType; } yANewIf;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; tPosition Pos; } yProcList;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; tPosition Pos; } yNoProcess;
typedef struct { Tree_tNodeHead yyHead; int V1In; int V1Out; int VS; int VIn; int VOut; bool ATOMIC; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; bool PAR; tPosition Pos; tTree Next; tTree Process; tSymbol Sym; } yAProcess;
typedef struct { Tree_tNodeHead yyHead; tType FLIn; tType FLOut; int V1In; int V1Out; tType InhType; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yCaseList;
typedef struct { Tree_tNodeHead yyHead; tType FLIn; tType FLOut; int V1In; int V1Out; tType InhType; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; } yNoCase;
typedef struct { Tree_tNodeHead yyHead; tType FLIn; tType FLOut; int V1In; int V1Out; tType InhType; int VS; int VIn; int VOut; int SLC; tSymbol Parent; tParUsage PUIn; tParUsage PUOut; tTree Next; tTree SpecList; tTree ExprList; tTree Process; int Label; } yACase;
typedef struct { Tree_tNodeHead yyHead; } yAccessList;
typedef struct { Tree_tNodeHead yyHead; tTree Next; tSymbol Sym; tTree ElementList; } yAnAccess;
typedef struct { Tree_tNodeHead yyHead; } yNoAccess;
typedef struct { Tree_tNodeHead yyHead; } yElementList;
typedef struct { Tree_tNodeHead yyHead; tTree Next; tTree Element; } yAnElement;
typedef struct { Tree_tNodeHead yyHead; } yNoElement;

union Tree_Node {
 Tree_tKind Kind;
 Tree_tNodeHead yyHead;
 yStart Start;
 ySpecList SpecList;
 yNoSpec NoSpec;
 yASpec ASpec;
 ySpec Spec;
 yDeclType DeclType;
 yDecl Decl;
 yIs Is;
 yAVal AVal;
 yVal Val;
 yValRetypes ValRetypes;
 yValReshapes ValReshapes;
 yRetypes Retypes;
 yReshapes Reshapes;
 yDefAPro DefAPro;
 yDefSeqPro DefSeqPro;
 yDefTagPro DefTagPro;
 yDefAProc DefAProc;
 yDefProc DefProc;
 yInlineDefProc InlineDefProc;
 yPrototypeProc PrototypeProc;
 yDefAFunc DefAFunc;
 yDefFunc DefFunc;
 yInlineDefFunc InlineDefFunc;
 yDefIsFunc DefIsFunc;
 yInlineDefIsFunc InlineDefIsFunc;
 yPrototypeFunc PrototypeFunc;
 yNullSpec NullSpec;
 yHCode HCode;
 yCallFrame CallFrame;
 yParFrame ParFrame;
 yFormals Formals;
 yNoFormal NoFormal;
 yNoFormals NoFormals;
 yAFormal AFormal;
 yReplicator Replicator;
 yTagProList TagProList;
 yNoTagPro NoTagPro;
 yATagPro ATagPro;
 yBy By;
 yByVal ByVal;
 yByRef ByRef;
 yByUndef ByUndef;
 yIdList IdList;
 yNoId NoId;
 yAnId AnId;
 yType Type;
 yArray Array;
 yUserArray UserArray;
 yChan Chan;
 yTimer Timer;
 yUntyped Untyped;
 yUntypedByte UntypedByte;
 yUntypedReal UntypedReal;
 yUntypedInteger UntypedInteger;
 yUntypedInt UntypedInt;
 yUntypedHex UntypedHex;
 yPrimitive Primitive;
 yNumeric Numeric;
 yInteger Integer;
 yInt Int;
 yUserInt UserInt;
 yInt16 Int16;
 yUserInt16 UserInt16;
 yInt32 Int32;
 yUserInt32 UserInt32;
 yInt64 Int64;
 yUserInt64 UserInt64;
 yReal Real;
 yReal32 Real32;
 yUserReal32 UserReal32;
 yReal64 Real64;
 yUserReal64 UserReal64;
 yByte Byte;
 yUserByte UserByte;
 yBool Bool;
 yUserBool UserBool;
 yUserStructTypes UserStructTypes;
 yUserDataTypes UserDataTypes;
 yUserRecord UserRecord;
 yUserPackedRecord UserPackedRecord;
 yUserUnion UserUnion;
 yUserChanRecord UserChanRecord;
 yTypeList TypeList;
 yNoType NoType;
 yAType AType;
 yFieldList FieldList;
 yNoField NoField;
 yAField AField;
 yProtocol Protocol;
 yAny Any;
 yNamed Named;
 ySimple Simple;
 yCounted Counted;
 ySeqProList SeqProList;
 yNoSeqPro NoSeqPro;
 yASeqPro ASeqPro;
 yExpr Expr;
 yBinary Binary;
 yUnary Unary;
 yConst Const;
 yValof Valof;
 yFuncCall FuncCall;
 yElementalExpr ElementalExpr;
 yDefault Default;
 yConvert Convert;
 yRound Round;
 yTrunc Trunc;
 yMostPos MostPos;
 yMostNeg MostNeg;
 yCountArray CountArray;
 yUnBound UnBound;
 yBytesIn BytesIn;
 yBytesInType BytesInType;
 yBytesInExpr BytesInExpr;
 ySizeOf SizeOf;
 yOffsetOf OffsetOf;
 yStringElement StringElement;
 yString String;
 yStringSubscript StringSubscript;
 yStringRange StringRange;
 yElemental Elemental;
 yTableElement TableElement;
 yTable Table;
 yUserLiteral UserLiteral;
 yTableExpr TableExpr;
 yTableSubscript TableSubscript;
 yTableRange TableRange;
 yElement Element;
 yIdentifier Identifier;
 yUnmappedIdentifier UnmappedIdentifier;
 yElementAccess ElementAccess;
 ySubscript Subscript;
 yRange Range;
 yExprList ExprList;
 yNoExpr NoExpr;
 yAnExpr AnExpr;
 yOp Op;
 yRelOp RelOp;
 yLess Less;
 yGreater Greater;
 yLessEqual LessEqual;
 yGreaterEqual GreaterEqual;
 yEqual Equal;
 yNotEqual NotEqual;
 yMathsOp MathsOp;
 yAdd Add;
 ySubtract Subtract;
 yMultiply Multiply;
 yDivide Divide;
 yRem Rem;
 yBitOp BitOp;
 yBitAnd BitAnd;
 yBitOr BitOr;
 yBitEor BitEor;
 yShiftOp ShiftOp;
 yLeftShift LeftShift;
 yRightShift RightShift;
 yUniOp UniOp;
 ySize Size;
 yBitNot BitNot;
 yNot Not;
 yClockOp ClockOp;
 yPlus Plus;
 yMinus Minus;
 yTimes Times;
 yLogOp LogOp;
 yAnd And;
 yOr Or;
 yAfterOp AfterOp;
 yProcess Process;
 yAssign Assign;
 yAltType AltType;
 yAlt Alt;
 yAltRep AltRep;
 yParType ParType;
 yPar Par;
 yParRep ParRep;
 yProcessor Processor;
 yIfType IfType;
 yIf If;
 yIfRep IfRep;
 yInput Input;
 yOutput Output;
 yCaseIn CaseIn;
 yAfter After;
 ySeqRep SeqRep;
 ySeq Seq;
 yCase Case;
 yWhile While;
 ySkip Skip;
 yStop Stop;
 yInstance Instance;
 yCCode CCode;
 yNullProcess NullProcess;
 yAltList AltList;
 yNoAlt NoAlt;
 yAnAlts AnAlts;
 yAnAlt AnAlt;
 yANewAlt ANewAlt;
 yGuard Guard;
 yAltInput AltInput;
 yAltTimer AltTimer;
 yAltCaseIn AltCaseIn;
 yNoInput NoInput;
 yChoiceList ChoiceList;
 yNoChoice NoChoice;
 yAChoices AChoices;
 yAChoice AChoice;
 yANewIf ANewIf;
 yProcList ProcList;
 yNoProcess NoProcess;
 yAProcess AProcess;
 yCaseList CaseList;
 yNoCase NoCase;
 yACase ACase;
 yAccessList AccessList;
 yAnAccess AnAccess;
 yNoAccess NoAccess;
 yElementList ElementList;
 yAnElement AnElement;
 yNoElement NoElement;
};

extern tTree TreeRoot;
extern uint32_t Tree_HeapUsed;
extern char * Tree_PoolFreePtr, * Tree_PoolMaxPtr;
extern unsigned short Tree_NodeSize [222 + 1];
extern char * Tree_NodeName [222 + 1];

extern void (* Tree_Exit) ();
extern tTree Tree_Alloc ();
extern tTree MakeTree ARGS((Tree_tKind yyKind));
extern bool Tree_IsType ARGS((register tTree yyt, register Tree_tKind yyKind));

extern tTree mStart ARGS((tTree pProcess));
extern tTree mSpecList ARGS(());
extern tTree mNoSpec ARGS(());
extern tTree mASpec ARGS((tTree pSpec, tTree pNext));
extern tTree mSpec ARGS((tPosition pPos));
extern tTree mDeclType ARGS((tPosition pPos, tTree pType));
extern tTree mDecl ARGS((tPosition pPos, tTree pType, tTree pIdList));
extern tTree mIs ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental));
extern tTree mAVal ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr));
extern tTree mVal ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr));
extern tTree mValRetypes ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr));
extern tTree mValReshapes ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pExpr));
extern tTree mRetypes ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental));
extern tTree mReshapes ARGS((tPosition pPos, tTree pType, tSymbol pSym, tTree pElemental));
extern tTree mDefAPro ARGS((tPosition pPos));
extern tTree mDefSeqPro ARGS((tPosition pPos, tSymbol pSym, tTree pSeqProList));
extern tTree mDefTagPro ARGS((tPosition pPos, tSymbol pSym, tTree pTagProList));
extern tTree mDefAProc ARGS((tPosition pPos, tSymbol pSym, tTree pFormals));
extern tTree mDefProc ARGS((tPosition pPos, tSymbol pSym, tTree pFormals, tTree pProcess));
extern tTree mInlineDefProc ARGS((tPosition pPos, tSymbol pSym, tTree pFormals, tTree pProcess));
extern tTree mPrototypeProc ARGS((tPosition pPos, tSymbol pSym, tTree pFormals, tIdent pModule, tLang pLang));
extern tTree mDefAFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals));
extern tTree mDefFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pValof));
extern tTree mInlineDefFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pValof));
extern tTree mDefIsFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pExprList));
extern tTree mInlineDefIsFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tTree pExprList));
extern tTree mPrototypeFunc ARGS((tPosition pPos, tSymbol pSym, tTree pTypeList, tTree pFormals, tIdent pModule, tLang pLang));
extern tTree mNullSpec ARGS((tPosition pPos));
extern tTree mHCode ARGS((tPosition pPos, tStringRef pCode, tSymbol pScope));
extern tTree mCallFrame ARGS((tPosition pPos, tIdent pId, tSymbol pSym));
extern tTree mParFrame ARGS((tPosition pPos, tIdent pTId, tIdent pFId, tSymbol pSym, int pRep));
extern tTree mFormals ARGS(());
extern tTree mNoFormal ARGS(());
extern tTree mNoFormals ARGS((tSymbol pSym));
extern tTree mAFormal ARGS((tTree pNext, tPosition pPos, tTree pBy, tTree pType, tSymbol pSym));
extern tTree mReplicator ARGS((tSymbol pSym, tTree pFrom, tTree pFor));
extern tTree mTagProList ARGS(());
extern tTree mNoTagPro ARGS(());
extern tTree mATagPro ARGS((tTree pNext, tSymbol pSym, tTree pSeqProList));
extern tTree mBy ARGS(());
extern tTree mByVal ARGS(());
extern tTree mByRef ARGS(());
extern tTree mByUndef ARGS(());
extern tTree mIdList ARGS(());
extern tTree mNoId ARGS(());
extern tTree mAnId ARGS((tTree pNext, tSymbol pSym));
extern tTree mType ARGS(());
extern tTree mArray ARGS((tTree pExpr, tTree pType));
extern tTree mUserArray ARGS((tTree pExpr, tTree pType, tPosition pPos, tSymbol pSym));
extern tTree mChan ARGS((tTree pProtocol));
extern tTree mTimer ARGS(());
extern tTree mUntyped ARGS(());
extern tTree mUntypedByte ARGS(());
extern tTree mUntypedReal ARGS(());
extern tTree mUntypedInteger ARGS(());
extern tTree mUntypedInt ARGS(());
extern tTree mUntypedHex ARGS(());
extern tTree mPrimitive ARGS(());
extern tTree mNumeric ARGS(());
extern tTree mInteger ARGS(());
extern tTree mInt ARGS(());
extern tTree mUserInt ARGS((tPosition pPos, tSymbol pSym));
extern tTree mInt16 ARGS(());
extern tTree mUserInt16 ARGS((tPosition pPos, tSymbol pSym));
extern tTree mInt32 ARGS(());
extern tTree mUserInt32 ARGS((tPosition pPos, tSymbol pSym));
extern tTree mInt64 ARGS(());
extern tTree mUserInt64 ARGS((tPosition pPos, tSymbol pSym));
extern tTree mReal ARGS(());
extern tTree mReal32 ARGS(());
extern tTree mUserReal32 ARGS((tPosition pPos, tSymbol pSym));
extern tTree mReal64 ARGS(());
extern tTree mUserReal64 ARGS((tPosition pPos, tSymbol pSym));
extern tTree mByte ARGS(());
extern tTree mUserByte ARGS((tPosition pPos, tSymbol pSym));
extern tTree mBool ARGS(());
extern tTree mUserBool ARGS((tPosition pPos, tSymbol pSym));
extern tTree mUserStructTypes ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mUserDataTypes ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mUserRecord ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mUserPackedRecord ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mUserUnion ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mUserChanRecord ARGS((tPosition pPos, tSymbol pSym, tTree pFieldList));
extern tTree mTypeList ARGS(());
extern tTree mNoType ARGS(());
extern tTree mAType ARGS((tTree pNext, tTree pType));
extern tTree mFieldList ARGS(());
extern tTree mNoField ARGS(());
extern tTree mAField ARGS((tTree pNext, tSymbol pSym, tTree pType));
extern tTree mProtocol ARGS((tPosition pPos));
extern tTree mAny ARGS((tPosition pPos));
extern tTree mNamed ARGS((tPosition pPos, tSymbol pSym));
extern tTree mSimple ARGS((tPosition pPos, tTree pType));
extern tTree mCounted ARGS((tPosition pPos, tTree pLength, tTree pType));
extern tTree mSeqProList ARGS(());
extern tTree mNoSeqPro ARGS(());
extern tTree mASeqPro ARGS((tTree pNext, tTree pProtocol));
extern tTree mExpr ARGS((tPosition pPos));
extern tTree mBinary ARGS((tPosition pPos, tTree pLop, tTree pRop, tTree pOp));
extern tTree mUnary ARGS((tPosition pPos, tTree pExpr, tTree pOp));
extern tTree mConst ARGS((tPosition pPos, tValue pValue, tTree pType));
extern tTree mValof ARGS((tPosition pPos, tTree pSpecList, tTree pProcess, tTree pExprList, tSymbol pSym, int pVS));
extern tTree mFuncCall ARGS((tPosition pPos, tSymbol pSym, tTree pExprList, int pDepth));
extern tTree mElementalExpr ARGS((tPosition pPos, tTree pElemental));
extern tTree mDefault ARGS((tPosition pPos));
extern tTree mConvert ARGS((tPosition pPos, tTree pType, tTree pExpr));
extern tTree mRound ARGS((tPosition pPos, tTree pType, tTree pExpr));
extern tTree mTrunc ARGS((tPosition pPos, tTree pType, tTree pExpr));
extern tTree mMostPos ARGS((tPosition pPos, tTree pType));
extern tTree mMostNeg ARGS((tPosition pPos, tTree pType));
extern tTree mCountArray ARGS((tPosition pPos, tTree pLength, tTree pExpr));
extern tTree mUnBound ARGS((tPosition pPos));
extern tTree mBytesIn ARGS((tPosition pPos));
extern tTree mBytesInType ARGS((tPosition pPos, tTree pType));
extern tTree mBytesInExpr ARGS((tPosition pPos, tTree pExpr));
extern tTree mSizeOf ARGS((tPosition pPos, tTree pType));
extern tTree mOffsetOf ARGS((tPosition pPos, tTree pType, tTree pUnmappedIdentifier));
extern tTree mStringElement ARGS((tPosition pPos));
extern tTree mString ARGS((tPosition pPos, tStringRef pValue, tTree pType, int pDepth, tSymbol pSym));
extern tTree mStringSubscript ARGS((tPosition pPos, tTree pNext, tTree pExpr));
extern tTree mStringRange ARGS((tPosition pPos, tTree pNext, tTree pFrom, tTree pFor));
extern tTree mElemental ARGS((tPosition pPos));
extern tTree mTableElement ARGS((tPosition pPos));
extern tTree mTable ARGS((tPosition pPos, tTree pExprList, int pDepth, tSymbol pSym));
extern tTree mUserLiteral ARGS((tPosition pPos, tTree pExprList, int pDepth, tSymbol pSym, tTree pType));
extern tTree mTableExpr ARGS((tPosition pPos, tTree pExpr, tSymbol pSym));
extern tTree mTableSubscript ARGS((tPosition pPos, tTree pNext, tTree pExpr));
extern tTree mTableRange ARGS((tPosition pPos, tTree pNext, tTree pFrom, tTree pFor));
extern tTree mElement ARGS((tPosition pPos));
extern tTree mIdentifier ARGS((tPosition pPos, tSymbol pSym, tSymbol pScope));
extern tTree mUnmappedIdentifier ARGS((tPosition pPos, tSymbol pSym, tSymbol pScope, tIdent pIdent));
extern tTree mElementAccess ARGS((tPosition pPos, tTree pNext));
extern tTree mSubscript ARGS((tPosition pPos, tTree pNext, tTree pExpr));
extern tTree mRange ARGS((tPosition pPos, tTree pNext, tTree pFrom, tTree pFor));
extern tTree mExprList ARGS(());
extern tTree mNoExpr ARGS(());
extern tTree mAnExpr ARGS((tTree pNext, tTree pExpr));
extern tTree mOp ARGS(());
extern tTree mRelOp ARGS(());
extern tTree mLess ARGS(());
extern tTree mGreater ARGS(());
extern tTree mLessEqual ARGS(());
extern tTree mGreaterEqual ARGS(());
extern tTree mEqual ARGS(());
extern tTree mNotEqual ARGS(());
extern tTree mMathsOp ARGS(());
extern tTree mAdd ARGS(());
extern tTree mSubtract ARGS(());
extern tTree mMultiply ARGS(());
extern tTree mDivide ARGS(());
extern tTree mRem ARGS(());
extern tTree mBitOp ARGS(());
extern tTree mBitAnd ARGS(());
extern tTree mBitOr ARGS(());
extern tTree mBitEor ARGS(());
extern tTree mShiftOp ARGS(());
extern tTree mLeftShift ARGS(());
extern tTree mRightShift ARGS(());
extern tTree mUniOp ARGS(());
extern tTree mSize ARGS(());
extern tTree mBitNot ARGS(());
extern tTree mNot ARGS(());
extern tTree mClockOp ARGS(());
extern tTree mPlus ARGS(());
extern tTree mMinus ARGS(());
extern tTree mTimes ARGS(());
extern tTree mLogOp ARGS(());
extern tTree mAnd ARGS(());
extern tTree mOr ARGS(());
extern tTree mAfterOp ARGS(());
extern tTree mProcess ARGS((tPosition pPos, tTree pSpecList));
extern tTree mAssign ARGS((tPosition pPos, tTree pSpecList, tTree pLval, tTree pRval));
extern tTree mAltType ARGS((tPosition pPos, tTree pSpecList, bool pPri));
extern tTree mAlt ARGS((tPosition pPos, tTree pSpecList, bool pPri, tTree pAltList));
extern tTree mAltRep ARGS((tPosition pPos, tTree pSpecList, bool pPri, tTree pReplicator, tTree pAltList));
extern tTree mParType ARGS((tPosition pPos, tTree pSpecList, tParType pPType));
extern tTree mPar ARGS((tPosition pPos, tTree pSpecList, tParType pPType, tTree pProcList));
extern tTree mParRep ARGS((tPosition pPos, tTree pSpecList, tParType pPType, tTree pReplicator, tTree pProcess, tSymbol pSym));
extern tTree mProcessor ARGS((tPosition pPos, tTree pSpecList, tTree pExpr, tIdent pName, tTree pProcess));
extern tTree mIfType ARGS((tPosition pPos, tTree pSpecList));
extern tTree mIf ARGS((tPosition pPos, tTree pSpecList, tTree pChoiceList));
extern tTree mIfRep ARGS((tPosition pPos, tTree pSpecList, tTree pReplicator, tTree pChoiceList));
extern tTree mInput ARGS((tPosition pPos, tTree pSpecList, tTree pElement, tTree pExprList));
extern tTree mOutput ARGS((tPosition pPos, tTree pSpecList, tTree pElement, tTree pExprList));
extern tTree mCaseIn ARGS((tPosition pPos, tTree pSpecList, tTree pElement, tTree pCaseList));
extern tTree mAfter ARGS((tPosition pPos, tTree pSpecList, tTree pElement, tTree pExpr));
extern tTree mSeqRep ARGS((tPosition pPos, tTree pSpecList, tTree pReplicator, tTree pProcess));
extern tTree mSeq ARGS((tPosition pPos, tTree pSpecList, tTree pProcList));
extern tTree mCase ARGS((tPosition pPos, tTree pSpecList, tTree pSelector, tTree pChoiceList));
extern tTree mWhile ARGS((tPosition pPos, tTree pSpecList, tTree pExpr, tTree pProcess));
extern tTree mSkip ARGS((tPosition pPos, tTree pSpecList));
extern tTree mStop ARGS((tPosition pPos, tTree pSpecList));
extern tTree mInstance ARGS((tPosition pPos, tTree pSpecList, tSymbol pSym, tTree pExprList, tSymbol pScope));
extern tTree mCCode ARGS((tPosition pPos, tTree pSpecList, tStringRef pCode, tSymbol pScope));
extern tTree mNullProcess ARGS((tPosition pPos, tTree pSpecList));
extern tTree mAltList ARGS(());
extern tTree mNoAlt ARGS(());
extern tTree mAnAlts ARGS((tTree pSpecList, tTree pNext));
extern tTree mAnAlt ARGS((tTree pSpecList, tTree pNext, tTree pGuard));
extern tTree mANewAlt ARGS((tTree pSpecList, tTree pNext, tTree pAltType));
extern tTree mGuard ARGS((tPosition pPos, tTree pExpr));
extern tTree mAltInput ARGS((tPosition pPos, tTree pExpr, tTree pInput, tTree pProcess));
extern tTree mAltTimer ARGS((tPosition pPos, tTree pExpr, tTree pAfter, tTree pProcess));
extern tTree mAltCaseIn ARGS((tPosition pPos, tTree pExpr, tTree pCaseIn));
extern tTree mNoInput ARGS((tPosition pPos, tTree pExpr, tTree pProcess));
extern tTree mChoiceList ARGS(());
extern tTree mNoChoice ARGS(());
extern tTree mAChoices ARGS((tTree pSpecList, tTree pNext));
extern tTree mAChoice ARGS((tTree pSpecList, tTree pNext, tTree pExprList, tTree pProcess));
extern tTree mANewIf ARGS((tTree pSpecList, tTree pNext, tTree pIfType));
extern tTree mProcList ARGS(());
extern tTree mNoProcess ARGS(());
extern tTree mAProcess ARGS((tTree pNext, tTree pProcess, tSymbol pSym));
extern tTree mCaseList ARGS(());
extern tTree mNoCase ARGS(());
extern tTree mACase ARGS((tTree pNext, tTree pSpecList, tTree pExprList, tTree pProcess));
extern tTree mAccessList ARGS(());
extern tTree mAnAccess ARGS((tTree pNext, tSymbol pSym, tTree pElementList));
extern tTree mNoAccess ARGS(());
extern tTree mElementList ARGS(());
extern tTree mAnElement ARGS((tTree pNext, tTree pElement));
extern tTree mNoElement ARGS(());

extern void WriteTreeNode ARGS((FILE * yyyf, tTree yyt));
extern void WriteTree ARGS((FILE * yyyf, tTree yyt));
extern void TraverseTreeTD ARGS((tTree yyt, Tree_tProcTree yyyProc));
extern void TraverseTreeBU ARGS((tTree yyt, Tree_tProcTree yyyProc));
extern tTree ReverseTree ARGS((tTree yyOld));
extern tTree CopyTree ARGS((tTree yyt));
extern bool CheckTree ARGS((tTree yyt));
extern void QueryTree ARGS((tTree yyt));
extern void BeginTree ();
extern void CloseTree ();

# endif
