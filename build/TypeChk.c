# include "TypeChk.h"
# include "yyTypeChk.w"
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

/* line 63 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */


  #include "Positions.h"
  #include <stdlib.h>
  #include <math.h>

  #undef MAX
  #define MAX(a,b) ((a > b) ? a : b)
  /*{{{  void RetypeConst(int ss,tConstArray src, int ds, tConstArray dest)*/
  /* Retype a ConstArray section -
  
     ConstArray values are stored in compiler-endianness, to allow direct
     casting for constant folding/output
  
     To retype, is the endianness of the compiler/target architectures differ,
     we must perform two stages.
  
     1. Copy source to temporary, performing source type endianness swap
	This leaves temporary in target machine endianness
     2. Copy temporary to destination, performing destination type endianness swap
	This converts temporary into correct types, in compiler endianness
  */
  
  void RetypeConst(int ss,tConstArray src, int ds, tConstArray dest)
  {
    int sz = (ds > ss) ? ds : ss;
    int i;
    int val = 1;
    bool Compiler_is_LittleEndian = (((char *)&val)[0]) == 1;
    unsigned char tmp[8];
  
    if ((!IS_Mode(xxLittleEndian) && Compiler_is_LittleEndian)
	||(IS_Mode(xxLittleEndian) && !Compiler_is_LittleEndian))
    {
      for(i = 0; i < sz; ++i) tmp[i] = src[i ^ (ss-1)]; 
      for(i = 0; i < sz; ++i) dest[i] = tmp[i ^ (ds-1)]; 
    }
    else
    {
      for(i = 0; i < sz; ++i) dest[i] = src[i];
    }
  }
  
  /*}}}*/
  tSymbol FirstFormal,FirstActual,FunctionInlining;


static void yyExit () { Exit (1); }

void (* TypeChk_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module TypeChk, routine %s failed\n", yyFunction);
 TypeChk_Exit ();
}

tTree TypeUserDataType ARGS((tTree yyP3, tSymbol yyP2, tPosition yyP1));
void TypeFormals ARGS((tTree yyP4));
void TypeDecl ARGS((tTree yyP6, tTree yyP5));
tTree TypeAbbrev ARGS((tTree yyP9, tTree yyP8, tPosition yyP7));
tTree TypeSubscript ARGS((tTree yyP13, tTree yyP12, tTree yyP11, tPosition yyP10));
tTree TypeRange ARGS((tTree yyP17, tTree yyP16, tTree yyP15, tPosition yyP14));
tTree TypeBinary ARGS((tTree yyP21, tTree yyP20, tTree yyP19, tPosition yyP18));
tTree TypeUnary ARGS((tTree yyP24, tTree yyP23, tPosition yyP22));
static tTree TypeExprSubList ARGS((tTree yyP25));
tTree TypeExprList ARGS((tTree yyP26));
tTree TypeTable ARGS((tTree yyP29, tTree yyP28, tTree yyP27));
tTree TypeTableExpr ARGS((tTree yyP30));
tTree TypeString ARGS((int yyP33, tTree yyP32, tPosition yyP31));
tTree StripTypeList ARGS((tTree yyP34));
tTree SynthDefType ARGS((tTree yyP37, tTree yyP36, tTree yyP35));
tTree SynthDefTableType ARGS((tTree yyP38));
tTree SynthDefSubscriptType ARGS((tTree yyP40, tTree yyP39));
tTree DefaultType ARGS((tTree yyP41));
tTree InheritLhsType ARGS((tTree yyP43, tTree yyP42));
void InheritFromFormals ARGS((tTree yyP45, tTree yyP44));
static void InheritFromSimpleProtocol ARGS((tTree yyP47, tTree yyP46));
void InheritFromProtocol ARGS((tTree yyP49, tTree yyP48));
static void InheritFromFieldList ARGS((tTree yyP51, tTree yyP50));
void InheritFromTable ARGS((tTree yyP53, tTree yyP52));
void InheritFromType ARGS((tTree yyP55, tTree yyP54));
static void InheritAttributes ARGS((tTree yyP57, tTree yyP56));
tTree TransformUserLiterals ARGS((tTree yyP59, tTree yyP58));
void TypeCheckAssign ARGS((tTree yyP62, tTree yyP61, tPosition yyP60));
static void TypeCheckAssignList ARGS((tTree yyP65, tTree yyP64, tPosition yyP63));
static void TypeCheckAssignFn ARGS((tTree yyP68, tTree yyP67, tPosition yyP66));
void TypeCheckResults ARGS((tTree yyP71, tTree yyP70, tPosition yyP69));
void TypeCheckParameters ARGS((tTree yyP75, tTree yyP74, tPosition yyP73, int yyP72));
void UpdateElementalUsage ARGS((tTree yyP77, tUsage yyP76));
void TypeCheckIfList ARGS((tTree yyP78));
void TypeCheckTimerInput ARGS((tTree yyP80, tPosition yyP79));
void TypeCheckInputProtocol ARGS((tTree yyP83, tTree yyP82, tPosition yyP81));
static void TypeCheckSimpleProtocol ARGS((tTree yyP85, tTree yyP84));
void TypeCheckOutputProtocol ARGS((tTree yyP88, tTree yyP87, tPosition yyP86));
void TypeCheckCaseInputs ARGS((tTree yyP91, tTree yyP90, tPosition yyP89));
static void TypeCheckCaseOutput ARGS((tTree yyP94, tTree yyP93, tPosition yyP92));
static void TypeCheckInputSeqProtocol ARGS((tTree yyP97, tTree yyP96, tPosition yyP95));
static void TypeCheckOutputSeqProtocol ARGS((tTree yyP100, tTree yyP99, tPosition yyP98));
void TypeCheckCaseList ARGS((tTree yyP103, tTree yyP102, bool yyP101));
void TypeCheckRetypes ARGS((tTree yyP107, tTree yyP106, tTree yyP105, tPosition yyP104));
static void DoTypeCheckRetypes ARGS((int yyP112, tTree yyP111, int yyP110, tTree yyP109, tPosition yyP108));
bool IsUserDataType ARGS((tTree yyP113));
static tSymbol WhichUserDataType ARGS((tTree yyP114));
bool IsComputableType ARGS((tTree yyP115));
bool SameBaseType ARGS((tTree yyP117, tTree yyP116));
bool SameUnderlyingType ARGS((tTree yyP119, tTree yyP118));
static bool SameType ARGS((tTree yyP121, tTree yyP120));
static bool CheckValidTag ARGS((tTree yyP123, tSymbol yyP122));
static bool SameProtocol ARGS((tTree yyP125, tTree yyP124));
bool CheckWritable ARGS((tTree yyP126));
bool CheckWritableExprList ARGS((tTree yyP127));
bool CheckForVals ARGS((tTree yyP128));
bool CheckValofScope ARGS((tTree yyP130, int yyP129));
bool IsChanOrTimerType ARGS((tTree yyP131));
static bool IsDataType ARGS((tTree yyP132));
bool IsChanType ARGS((tTree yyP133));
bool IsTimerType ARGS((tTree yyP134));
bool IsArrayType ARGS((tTree yyP135));
bool IsScalarType ARGS((tTree yyP136));
static bool CheckChoicesDiffer ARGS((tTree yyP138, tTree yyP137));
static bool CheckExprDiffers ARGS((tTree yyP141, tTree yyP140, tTree yyP139));
bool CheckValidDeclType ARGS((tTree yyP142));
bool CheckFuncType ARGS((tTree yyP143));
bool CheckEvalBiOp ARGS((tTree yyP145, tTree yyP144));
static bool CheckEvalBiIntOp ARGS((tTree yyP146));
static bool CheckEvalBiRealOp ARGS((tTree yyP147));
bool CheckEvalUniOp ARGS((tTree yyP150, tTree yyP149, tTree yyP148));
tValue EvalBiExpr ARGS((tPosition yyP155, tTree yyP154, tValue yyP153, tTree yyP152, tValue yyP151));
static tInt EvalBiIntOp ARGS((tPosition yyP160, tTree yyP159, tInt yyP158, tTree yyP157, tInt yyP156));
static int EvalBiBoolOp ARGS((tPosition yyP165, tTree yyP164, tInt yyP163, tTree yyP162, tInt yyP161));
static double EvalBiRealMathsOp ARGS((tPosition yyP170, tTree yyP169, double yyP168, tTree yyP167, double yyP166));
static int EvalBiRealRelOp ARGS((tPosition yyP175, tTree yyP174, double yyP173, tTree yyP172, double yyP171));
tValue EvalUniExpr ARGS((tTree yyP178, tTree yyP177, tTree yyP176));
bool CheckConstConvert ARGS((tTree yyP179));
tValue EvalConstConvert ARGS((tPosition yyP182, tTree yyP181, tTree yyP180));
tValue CheckConst ARGS((tPosition yyP186, tTree yyP185, tInt yyP184, bool yyP183));
bool CheckConstRoundTrunc ARGS((tTree yyP188, tTree yyP187));
tValue EvalConstTrunc ARGS((tPosition yyP192, tTree yyP191, tTree yyP190, tValue yyP189));
tValue EvalConstRound ARGS((tPosition yyP196, tTree yyP195, tTree yyP194, tValue yyP193));
bool CheckConstId ARGS((tTree yyP197));
tValue EvalConstId ARGS((tTree yyP198));
tInt TestIntOVF ARGS((tPosition yyP201, tTree yyP200, tInt yyP199));
tInt SignExtendInt ARGS((tTree yyP203, tInt yyP202));
static int CountTags ARGS((tSymbol yyP204));
static int CountFields ARGS((tSymbol yyP205));
static tInt EvalIntBits ARGS((tTree yyP206));
tInt EvalMostPos ARGS((tTree yyP207));
tInt EvalMostNeg ARGS((tTree yyP208));
static tInt EvalIntMask ARGS((tTree yyP209));
tValue EvalConstSubscript ARGS((tTree yyP212, int yyP211, tConstArray yyP210));
tConstArray ConstArrayFromString ARGS((tTree yyP214, tStringRef yyP213));
tConstArray ConstArrayFromExprList ARGS((tTree yyP216, tTree yyP215));
tConstArray ConstArrayFromDecl ARGS((tTree yyP218, tTree yyP217));
static void FillConstArray ARGS((tTree yyP221, tTree yyP220, tConstArray yyP219));
static void FillConstFields ARGS((tTree yyP225, tTree yyP224, tTree yyP223, tConstArray yyP222));
tConstArray RangeConstArray ARGS((tTree yyP228, tConstArray yyP227, int yyP226));
tConstArray SubscriptConstArray ARGS((tTree yyP232, tConstArray yyP231, tTree yyP230, tTree yyP229));
static tConstArray GetConstArray ARGS((tTree EX));
tValue ReadConstArray ARGS((tTree yyP234, tConstArray yyP233));
void WriteConstArray ARGS((tTree yyP237, tValue yyP236, tConstArray yyP235));
static tConstArray NewConstArray ARGS((int yyP238));
static void RangeCheckSubscript ARGS((tTree yyP240, tTree yyP239));
static void RangeCheckSlice ARGS((tTree yyP243, tTree yyP242, tTree yyP241));
tTree MakeConstInt ARGS((int yyP245, tPosition yyP244));
static tTree MakeAddExpr ARGS((tTree yyP247, tTree yyP246));
static int CountList ARGS((tTree yyP249, tTree yyP248));
int CountExprs ARGS((tTree yyP250));
static int SizeType ARGS((tTree yyP253, tPosition yyP252, bool yyP251, tTree * yyP254));
static int AlignToNext ARGS((int yyP256, int yyP255));
static int CalcRecordSize ARGS((tTree yyP258, int yyP257));
static int CalcPackedRecordSize ARGS((tTree yyP260, int yyP259));
static int CalcRecordOffset ARGS((tTree yyP263, tTree yyP262, int yyP261));
static int CalcPackedRecordOffset ARGS((tTree yyP266, tTree yyP265, int yyP264));
int CalcOffset ARGS((tTree yyP268, tTree yyP267));
int CalcTypeSize ARGS((tTree yyP269));
tTree ReduceElement ARGS((tTree yyP271, tTree yyP270));
tTree ReduceElemental ARGS((tTree yyP272));
static tTree SplatSymbolTableFormals ARGS((tPosition yyP277, tTree yyP276, tSymbol yyP275, int yyP274, tTree yyP273));
static tTree AppendSpecList ARGS((tTree yyP279, tTree yyP278));
static void UpdateFuncSymbol ARGS((tTree yyP280));
static tTree CopyFuncBody ARGS((tTree yyP284, tTree yyP283, int yyP282, tSymbol yyP281));
static bool WithinScope ARGS((tSymbol yyP286, tSymbol yyP285));
tTree CreateFuncCall ARGS((tPosition yyP290, tSymbol yyP289, tTree yyP288, tInt yyP287));
tTree CreateProcCall ARGS((tPosition yyP295, tTree yyP294, tSymbol yyP293, tTree yyP292, tSymbol yyP291));
bool CheckValidUserType ARGS((tTree yyP296));
static bool CheckAllDataFields ARGS((tTree yyP297));
static bool CheckAllChanFields ARGS((tTree yyP298));
tTree GetTagProtocol ARGS((tTree yyP299));
void LookupProtocolTag ARGS((tIdent yyP302, tPosition yyP301, tTree yyP300, tSymbol * yyP303));
tTree LookupField ARGS((tIdent yyP306, tPosition yyP305, tTree yyP304, tSymbol * yyP307));
tTree GetFieldList ARGS((tTree yyP308));
tTree ArrayPrimType ARGS((tTree yyP309));
void MarkNonAtomicIf ARGS((tTree IF));
static void MarkNonAtomicIfList ARGS((tTree IF));

tTree TypeUserDataType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP3, tSymbol yyP2, register tPosition yyP1)
# else
(yyP3, yyP2, yyP1)
 register tTree yyP3;
 tSymbol yyP2;
 register tPosition yyP1;
# endif
{

  switch (yyP3->Kind) {
  case kArray:
  case kUserArray:
/* line 150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserArray)
    yyV1->UserArray.Expr = yyP3->Array.Expr;
    yyV1->UserArray.Type = yyP3->Array.Type;
    yyV1->UserArray.Pos = yyP1;
    yyV1->UserArray.Sym = yyP2;
   return yyV1;
  }
 }

  case kInt:
  case kUserInt:
/* line 151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserInt)
    yyV1->UserInt.Pos = yyP1;
    yyV1->UserInt.Sym = yyP2;
   return yyV1;
  }
 }

  case kInt16:
  case kUserInt16:
/* line 152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserInt16)
    yyV1->UserInt16.Pos = yyP1;
    yyV1->UserInt16.Sym = yyP2;
   return yyV1;
  }
 }

  case kInt32:
  case kUserInt32:
/* line 153 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserInt32)
    yyV1->UserInt32.Pos = yyP1;
    yyV1->UserInt32.Sym = yyP2;
   return yyV1;
  }
 }

  case kInt64:
  case kUserInt64:
/* line 154 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserInt64)
    yyV1->UserInt64.Pos = yyP1;
    yyV1->UserInt64.Sym = yyP2;
   return yyV1;
  }
 }

  case kReal32:
  case kUserReal32:
/* line 155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserReal32)
    yyV1->UserReal32.Pos = yyP1;
    yyV1->UserReal32.Sym = yyP2;
   return yyV1;
  }
 }

  case kReal64:
  case kUserReal64:
/* line 156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserReal64)
    yyV1->UserReal64.Pos = yyP1;
    yyV1->UserReal64.Sym = yyP2;
   return yyV1;
  }
 }

  case kByte:
  case kUserByte:
/* line 157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserByte)
    yyV1->UserByte.Pos = yyP1;
    yyV1->UserByte.Sym = yyP2;
   return yyV1;
  }
 }

  case kBool:
  case kUserBool:
/* line 158 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kUserBool)
    yyV1->UserBool.Pos = yyP1;
    yyV1->UserBool.Sym = yyP2;
   return yyV1;
  }
 }

  }

/* line 159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Not a valid user data type", xxError, yyP1);
  }
   return yyP3;

}

void TypeFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP4)
# else
(yyP4)
 register tTree yyP4;
# endif
{
/* line 164 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 static tTree by=NULL; static tTree type=NULL; 
  if (Tree_IsType (yyP4, kNoFormal)) {
/* line 165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP4->Kind == kAFormal) {
  if (yyP4->AFormal.By->Kind == kByUndef) {
  if (Tree_IsType (yyP4->AFormal.Type, kUntyped)) {
/* line 166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP4->AFormal.By = by;
/* line 166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP4->AFormal.Type = type;
/* line 166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP4->AFormal.Sym) = type;
/* line 166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeFormals (yyP4->AFormal.Next);
  }
   return;

  }
  }
/* line 167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   by = yyP4->AFormal.By;
/* line 167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   type = yyP4->AFormal.Type;
/* line 167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP4->AFormal.Sym) = yyP4->AFormal.Type;
/* line 167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeFormals (yyP4->AFormal.Next);
  }
   return;

  }
;
}

void TypeDecl
# if defined __STDC__ | defined __cplusplus
(register tTree yyP6, register tTree yyP5)
# else
(yyP6, yyP5)
 register tTree yyP6;
 register tTree yyP5;
# endif
{
  if (yyP5->Kind == kNoId) {
/* line 171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP5->Kind == kAnId) {
/* line 172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP5->AnId.Sym) = yyP6;
/* line 172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeDecl (yyP6, yyP5->AnId.Next);
  }
   return;

  }
;
}

tTree TypeAbbrev
# if defined __STDC__ | defined __cplusplus
(register tTree yyP9, register tTree yyP8, register tPosition yyP7)
# else
(yyP9, yyP8, yyP7)
 register tTree yyP9;
 register tTree yyP8;
 register tPosition yyP7;
# endif
{
  if (Tree_IsType (yyP9, kUntyped)) {
/* line 176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP8;

  }
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsUserDataType (yyP9)))) goto yyL2;
  {
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsUserDataType (yyP8)))) goto yyL2;
  {
/* line 177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((WhichUserDataType (yyP9) == WhichUserDataType (yyP8)))) goto yyL2;
  }
  }
  }
   return yyP9;
yyL2:;

/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsUserDataType (yyP8)))) goto yyL3;
  {
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsUserDataType (yyP8)))) goto yyL3;
  {
/* line 178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("User type mismatch in abbreviation", xxError, yyP7);
  }
  }
  }
   return yyP9;
yyL3:;

  if (Tree_IsType (yyP9, kPrimitive)) {
  if (Tree_IsType (yyP8, kPrimitive)) {
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (yyP9, yyP8)))) goto yyL4;
  }
   return yyP9;
yyL4:;

  }
  }
  if (yyP9->Kind == kTimer) {
  if (yyP8->Kind == kTimer) {
/* line 180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP9;

  }
  }
  if (yyP9->Kind == kChan) {
  if (yyP8->Kind == kChan) {
/* line 181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameProtocol (yyP9->Chan.Protocol, yyP8->Chan.Protocol))) goto yyL6;
  }
   return yyP9;
yyL6:;

  }
  }
  if (Tree_IsType (yyP9, kArray)) {
  if (yyP9->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP8, kArray)) {
  if (yyP8->Array.Expr->Kind == kUnBound) {
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Type = TypeAbbrev (yyP9->Array.Type, yyP8->Array.Type, yyP7);
  }
   return yyP9;

  }
  }
  }
  if (Tree_IsType (yyP8, kArray)) {
/* line 183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (yyP9->Array.Expr, kUnBound)))) goto yyL8;
  {
/* line 183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Expr = yyP8->Array.Expr;
/* line 183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Type = TypeAbbrev (yyP9->Array.Type, yyP8->Array.Type, yyP7);
  }
  }
   return yyP9;
yyL8:;

/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP9->Array.Expr)))) goto yyL9;
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP8->Array.Expr)))) goto yyL9;
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP9->Array.Expr) == ValConstInt (yyP8->Array.Expr)))) goto yyL9;
  {
/* line 184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Type = TypeAbbrev (yyP9->Array.Type, yyP8->Array.Type, yyP7);
  }
  }
  }
  }
   return yyP9;
yyL9:;

/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP9->Array.Expr)))) goto yyL10;
  {
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP8->Array.Expr)))) goto yyL10;
  {
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Array size mismatch in abbreviation", xxError, yyP7);
/* line 185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Type = TypeAbbrev (yyP9->Array.Type, yyP8->Array.Type, yyP7);
  }
  }
  }
   return yyP9;
yyL10:;

/* line 186 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 186 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP9->Array.Type = TypeAbbrev (yyP9->Array.Type, yyP8->Array.Type, yyP7);
  }
   return yyP9;

  }
  }
/* line 187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch in abbreviation", xxError, yyP7);
  }
   return yyP9;

}

tTree TypeSubscript
# if defined __STDC__ | defined __cplusplus
(register tTree yyP13, register tTree yyP12, register tTree yyP11, register tPosition yyP10)
# else
(yyP13, yyP12, yyP11, yyP10)
 register tTree yyP13;
 register tTree yyP12;
 register tTree yyP11;
 register tPosition yyP10;
# endif
{
  if (Tree_IsType (yyP12, kArray)) {
  if (yyP11 == NULL) {
/* line 192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! Tree_IsType (ExprType (yyP13), kInt)))) goto yyL1;
  {
/* line 193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Subscript not of type INT", xxError, yyP10);
  }
  }
   return yyP12->Array.Type;
yyL1:;

/* line 195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   RangeCheckSubscript (yyP13, yyP12->Array.Expr);
  }
   return yyP12->Array.Type;

  }
  }
  if (Tree_IsType (yyP12, kUserStructTypes)) {
  if (yyP11 == NULL) {
/* line 196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Expected field name not an expression", xxError, yyP10);
  }
   return yyP12;

  }
  }
  if (yyP11 == NULL) {
/* line 197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Subscripted non-array type", xxError, yyP10);
  }
   return yyP12;

  }
  if (yyP11->Kind == kNoField) {
/* line 198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Invalid field name for this user type", xxError, yyP10);
  }
   return yyP12;

  }
  if (yyP11->Kind == kAField) {
/* line 199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP11->AField.Type;

  }
 yyAbort ("TypeSubscript");
}

tTree TypeRange
# if defined __STDC__ | defined __cplusplus
(register tTree yyP17, register tTree yyP16, register tTree yyP15, register tPosition yyP14)
# else
(yyP17, yyP16, yyP15, yyP14)
 register tTree yyP17;
 register tTree yyP16;
 register tTree yyP15;
 register tPosition yyP14;
# endif
{
  if (Tree_IsType (yyP15, kArray)) {
/* line 203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   RangeCheckSlice (yyP17, yyP16, yyP15->Array.Expr);
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
    yyV1->Array.Expr = yyP16;
    yyV1->Array.Type = yyP15->Array.Type;
   return yyV1;
  }
 }

  }
/* line 204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Took slice of non-array type", xxError, yyP14);
  }
   return yyP15;

}

tTree TypeBinary
# if defined __STDC__ | defined __cplusplus
(register tTree yyP21, register tTree yyP20, register tTree yyP19, register tPosition yyP18)
# else
(yyP21, yyP20, yyP19, yyP18)
 register tTree yyP21;
 register tTree yyP20;
 register tTree yyP19;
 register tPosition yyP18;
# endif
{
  if (Tree_IsType (yyP21, kBool)) {
  if (Tree_IsType (yyP20, kLogOp)) {
  if (Tree_IsType (yyP19, kBool)) {
/* line 208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL1;
  }
   return yyP21;
yyL1:;

  }
  }
  if (Tree_IsType (yyP20, kRelOp)) {
  if (Tree_IsType (yyP19, kBool)) {
/* line 209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL2;
  }
   return yyP21;
yyL2:;

  }
  }
  }
  if (Tree_IsType (yyP21, kByte)) {
  if (Tree_IsType (yyP20, kRelOp)) {
  if (Tree_IsType (yyP19, kByte)) {
/* line 210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL3;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kBool)
   return yyV1;
  }
 }
yyL3:;

  }
  }
  }
  if (Tree_IsType (yyP21, kReal)) {
  if (Tree_IsType (yyP20, kMathsOp)) {
  if (Tree_IsType (yyP19, kReal)) {
/* line 211 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 211 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL4;
  }
   return yyP21;
yyL4:;

  }
  }
  }
  if (yyP20->Kind == kAfterOp) {
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP21))) goto yyL5;
  {
/* line 212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL5;
  }
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kBool)
   return yyV1;
  }
 }
yyL5:;

  }
  if (Tree_IsType (yyP21, kNumeric)) {
  if (Tree_IsType (yyP20, kRelOp)) {
  if (Tree_IsType (yyP19, kNumeric)) {
/* line 213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL6;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kBool)
   return yyV1;
  }
 }
yyL6:;

  }
  }
  }
  if (Tree_IsType (yyP20, kMathsOp)) {
/* line 214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP21))) goto yyL7;
  {
/* line 214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL7;
  }
  }
   return yyP21;
yyL7:;

  }
  if (Tree_IsType (yyP20, kBitOp)) {
/* line 215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP21))) goto yyL8;
  {
/* line 215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL8;
  }
  }
   return yyP21;
yyL8:;

  }
  if (Tree_IsType (yyP20, kClockOp)) {
/* line 216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP21))) goto yyL9;
  {
/* line 216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP21, yyP19))) goto yyL9;
  }
  }
   return yyP21;
yyL9:;

  }
  if (Tree_IsType (yyP20, kShiftOp)) {
  if (Tree_IsType (yyP19, kInt)) {
/* line 217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP21))) goto yyL10;
  }
   return yyP21;
yyL10:;

  }
  }
/* line 219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch in binary operation", xxError, yyP18);
  }
   return yyP21;

}

tTree TypeUnary
# if defined __STDC__ | defined __cplusplus
(register tTree yyP24, register tTree yyP23, register tPosition yyP22)
# else
(yyP24, yyP23, yyP22)
 register tTree yyP24;
 register tTree yyP23;
 register tPosition yyP22;
# endif
{

  switch (yyP24->Kind) {
  case kSubtract:
  if (Tree_IsType (yyP23, kNumeric)) {
/* line 223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP23;

  }
  break;
  case kMinus:
/* line 224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP23))) goto yyL2;
  }
   return yyP23;
yyL2:;

  break;
  case kAdd:
  if (Tree_IsType (yyP23, kNumeric)) {
/* line 225 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP23;

  }
  break;
  case kPlus:
/* line 226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP23))) goto yyL4;
  }
   return yyP23;
yyL4:;

  break;
  case kNot:
  if (Tree_IsType (yyP23, kBool)) {
/* line 227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP23;

  }
  break;
  case kSize:
  if (Tree_IsType (yyP23, kArray)) {
/* line 228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }

  }
  break;
  case kBitNot:
/* line 229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsComputableType (yyP23))) goto yyL7;
  }
   return yyP23;
yyL7:;

  break;
  }

/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch in unary operation", xxError, yyP22);
  }
   return yyP23;

}

static tTree TypeExprSubList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP25)
# else
(yyP25)
 register tTree yyP25;
# endif
{
  if (yyP25->Kind == kNoExpr) {
/* line 235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoType)
   return yyV1;
  }
 }

  }
  if (yyP25->Kind == kAnExpr) {
/* line 236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAType)
    yyV1->AType.Next = TypeExprSubList (yyP25->AnExpr.Next);
    yyV1->AType.Type = ExprType (yyP25->AnExpr.Expr);
   return yyV1;
  }
 }

  }
 yyAbort ("TypeExprSubList");
}

tTree TypeExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP26)
# else
(yyP26)
 register tTree yyP26;
# endif
{
  if (yyP26->Kind == kNoExpr) {
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoType)
   return yyV1;
  }
 }

  }
  if (yyP26->Kind == kAnExpr) {
  if (yyP26->AnExpr.Next->Kind == kNoExpr) {
/* line 241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return ExprType (yyP26->AnExpr.Expr);

  }
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kAType)
    yyV1->AType.Next = TypeExprSubList (yyP26->AnExpr.Next);
    yyV1->AType.Type = ExprType (yyP26->AnExpr.Expr);
   return yyV1;
  }
 }

  }
 yyAbort ("TypeExprList");
}

tTree TypeTable
# if defined __STDC__ | defined __cplusplus
(register tTree yyP29, register tTree yyP28, register tTree yyP27)
# else
(yyP29, yyP28, yyP27)
 register tTree yyP29;
 register tTree yyP28;
 register tTree yyP27;
# endif
{
  if (yyP29->Kind == kAnExpr) {
  if (yyP27 == NULL) {
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
    yyV1->Array.Expr = MakeConstInt (CountList (yyP29->AnExpr.Next, yyP28), yyP29->AnExpr.Expr->Expr.Pos);
    yyV1->Array.Type = yyP28;
   return yyV1;
  }
 }

  }
  if (yyP27->Kind == kUserArray) {
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP27->UserArray.Expr)))) goto yyL2;
  {
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP27->UserArray.Expr) == CountList (yyP29->AnExpr.Next, yyP27->UserArray.Type)))) goto yyL2;
  }
  }
   return yyP27;
yyL2:;

  }
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
    yyV1->Array.Expr = MakeConstInt (CountList (yyP29->AnExpr.Next, yyP28), yyP29->AnExpr.Expr->Expr.Pos);
    yyV1->Array.Type = yyP28;
   return yyV1;
  }
 }

  }
 yyAbort ("TypeTable");
}

tTree TypeTableExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP30)
# else
(yyP30)
 register tTree yyP30;
# endif
{
  if (yyP30->Kind == kElementalExpr) {
  if (yyP30->ElementalExpr.Elemental->Kind == kTableExpr) {
/* line 257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP30->ElementalExpr.Elemental;

  }
  }
/* line 258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kTableExpr)
    yyV1->TableExpr.Pos = yyP30->Expr.Pos;
    yyV1->TableExpr.Expr = yyP30;
    yyV1->TableExpr.Sym = AddSymbol (NoIdent);
   return yyV1;
  }
 }

}

tTree TypeString
# if defined __STDC__ | defined __cplusplus
(register int yyP33, register tTree yyP32, register tPosition yyP31)
# else
(yyP33, yyP32, yyP31)
 register int yyP33;
 register tTree yyP32;
 register tPosition yyP31;
# endif
{
  if (yyP32->Kind == kUserArray) {
  if (Tree_IsType (yyP32->UserArray.Type, kByte)) {
/* line 262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP32->UserArray.Expr)))) goto yyL1;
  {
/* line 264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP33 == ValConstInt (yyP32->UserArray.Expr)))) goto yyL1;
  }
  }
   return yyP32;
yyL1:;

  }
  }
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
    yyV1->Array.Expr = MakeConstInt (yyP33, yyP31);
    yyV1->Array.Type = mByte ();
   return yyV1;
  }
 }

}

tTree StripTypeList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP34)
# else
(yyP34)
 register tTree yyP34;
# endif
{
  if (yyP34->Kind == kAType) {
  if (yyP34->AType.Next->Kind == kNoType) {
/* line 271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP34->AType.Type;

  }
  }
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP34;

}

tTree SynthDefType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP37, register tTree yyP36, register tTree yyP35)
# else
(yyP37, yyP36, yyP35)
 register tTree yyP37;
 register tTree yyP36;
 register tTree yyP35;
# endif
{
  if (Tree_IsType (yyP35, kShiftOp)) {
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP37;

  }
  if (yyP37 == NULL) {
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP36;

  }
  if (yyP36 == NULL) {
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP37;

  }
  if (Tree_IsType (yyP37, kUntyped)) {
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP36;

  }
  if (Tree_IsType (yyP36, kUntyped)) {
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP37;

  }
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP37;

}

tTree SynthDefTableType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP38)
# else
(yyP38)
 register tTree yyP38;
# endif
{
  if (yyP38 == NULL) {
/* line 288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP38;

  }
  if (Tree_IsType (yyP38, kUntyped)) {
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP38;

  }
/* line 290 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kArray)
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kUnBound)
    yyV1->Array.Expr = yyW1;
    yyW1->UnBound.Pos = NoPosition;
   }
    yyV1->Array.Type = yyP38;
   return yyV1;
  }
 }

}

tTree SynthDefSubscriptType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP40, register tTree yyP39)
# else
(yyP40, yyP39)
 register tTree yyP40;
 register tTree yyP39;
# endif
{
  if (yyP40 == NULL) {
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP40;

  }
  if (Tree_IsType (yyP40, kUntyped)) {
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP40;

  }
  if (Tree_IsType (yyP40, kArray)) {
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP40->Array.Type;

  }
  if (yyP39 == NULL) {
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP40;

  }
  if (yyP39->Kind == kAField) {
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP39->AField.Type;

  }
/* line 299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NULL;

}

tTree DefaultType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP41)
# else
(yyP41)
 register tTree yyP41;
# endif
{
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IS_Mode (xxUserDataTypes)))) goto yyL1;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }
yyL1:;

/* line 304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP41 -> Expr . DefType == NoTree))) goto yyL2;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }
yyL2:;

/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (yyP41 -> Expr . DefType, kUntyped)))) goto yyL3;
  }
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }
yyL3:;

/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP41 -> Expr . DefType;

}

tTree InheritLhsType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP43, register tTree yyP42)
# else
(yyP43, yyP42)
 register tTree yyP43;
 register tTree yyP42;
# endif
{
  if (yyP43 == NULL) {
/* line 312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xxUserDataTypes)))) goto yyL1;
  }
   return yyP42;
yyL1:;

/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }

  }
  if (Tree_IsType (yyP43, kUntyped)) {
/* line 313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xxUserDataTypes)))) goto yyL2;
  }
   return yyP42;
yyL2:;

  }
  if (yyP43->Kind == kUntypedByte) {
/* line 315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kByte)
   return yyV1;
  }
 }

  }
  if (Tree_IsType (yyP43, kUntyped)) {
/* line 316 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kInt)
   return yyV1;
  }
 }

  }
/* line 317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP43;

}

void InheritFromFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP45, register tTree yyP44)
# else
(yyP45, yyP44)
 register tTree yyP45;
 register tTree yyP44;
# endif
{
  if (Tree_IsType (yyP45, kDefAProc)) {
/* line 321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFormals (yyP45->DefAProc.Formals, yyP44);
  }
   return;

  }
  if (Tree_IsType (yyP45, kDefAFunc)) {
/* line 322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFormals (yyP45->DefAFunc.Formals, yyP44);
  }
   return;

  }
  if (Tree_IsType (yyP45, kNoFormal)) {
  if (yyP44->Kind == kNoExpr) {
/* line 323 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  }
  if (Tree_IsType (yyP45, kFormals)) {
  if (yyP44->Kind == kNoExpr) {
/* line 324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  }
  if (yyP45->Kind == kAFormal) {
  if (yyP44->Kind == kAnExpr) {
/* line 325 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 325 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP44->AnExpr.Expr -> Expr . InhType = yyP45->AFormal.Type;
/* line 325 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFormals (yyP45->AFormal.Next, yyP44->AnExpr.Next);
  }
   return;

  }
  }
  if (Tree_IsType (yyP45, kNoFormal)) {
  if (yyP44->Kind == kAnExpr) {
/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP44->AnExpr.Expr -> Expr . InhType = NoTree;
/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFormals (yyP45, yyP44->AnExpr.Next);
  }
   return;

  }
  }
;
}

static void InheritFromSimpleProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP47, register tTree yyP46)
# else
(yyP47, yyP46)
 register tTree yyP47;
 register tTree yyP46;
# endif
{
  if (yyP47->Kind == kSimple) {
/* line 330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP46 -> Expr . InhType = yyP47->Simple.Type;
  }
   return;

  }
  if (yyP47->Kind == kCounted) {
  if (yyP46->Kind == kCountArray) {
/* line 331 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 332 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP46 -> CountArray . InhType = yyP47->Counted.Type;
/* line 333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP46 -> CountArray . Length -> Expr . InhType = yyP47->Counted.Length;
  }
   return;

  }
/* line 335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP46 -> Expr . InhType = yyP47->Counted.Type;
  }
   return;

  }
;
}

void InheritFromProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP49, register tTree yyP48)
# else
(yyP49, yyP48)
 register tTree yyP49;
 register tTree yyP48;
# endif
{
  if (yyP48->Kind == kNoExpr) {
/* line 341 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP49->Kind == kChan) {
/* line 342 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 342 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (yyP49->Chan.Protocol, yyP48);
  }
   return;

  }
  if (yyP49->Kind == kNamed) {
/* line 343 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 343 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP49->Named.Sym), kDefSeqPro)))) goto yyL3;
  {
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (SymbolKind (yyP49->Named.Sym) -> DefSeqPro . SeqProList, yyP48);
  }
  }
   return;
yyL3:;

/* line 346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP49->Named.Sym), kDefTagPro)))) goto yyL4;
  {
/* line 347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (SymbolKind (yyP49->Named.Sym) -> DefTagPro . TagProList, yyP48);
  }
  }
   return;
yyL4:;

  }
  if (yyP49->Kind == kATagPro) {
  if (yyP48->Kind == kAnExpr) {
  if (yyP48->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP48->AnExpr.Expr->ElementalExpr.Elemental, kIdentifier)) {
/* line 349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((CheckValidTag (yyP49, yyP48->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym)))) goto yyL5;
  {
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP48->AnExpr.Expr -> Expr . InhType = mInt ();
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (SymbolKind (yyP48->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym) -> ATagPro . SeqProList, yyP48->AnExpr.Next);
  }
  }
   return;
yyL5:;

  }
  }
  }
  }
  if (yyP49->Kind == kASeqPro) {
  if (yyP48->Kind == kAnExpr) {
/* line 353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromSimpleProtocol (yyP49->ASeqPro.Protocol, yyP48->AnExpr.Expr);
/* line 355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (yyP49->ASeqPro.Next, yyP48->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP49->Kind == kSimple) {
  if (yyP48->Kind == kAnExpr) {
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromSimpleProtocol (yyP49, yyP48->AnExpr.Expr);
/* line 359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (yyP49, yyP48->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP49->Kind == kCounted) {
  if (yyP48->Kind == kAnExpr) {
/* line 361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromSimpleProtocol (yyP49, yyP48->AnExpr.Expr);
/* line 363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (yyP49, yyP48->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP48->Kind == kAnExpr) {
/* line 365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP48->AnExpr.Expr -> Expr . InhType = mInt ();
/* line 367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromProtocol (yyP49, yyP48->AnExpr.Next);
  }
   return;

  }
;
}

static void InheritFromFieldList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP51, register tTree yyP50)
# else
(yyP51, yyP50)
 register tTree yyP51;
 register tTree yyP50;
# endif
{
  if (yyP51->Kind == kAField) {
  if (yyP50->Kind == kAnExpr) {
/* line 372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP50->AnExpr.Expr -> Expr . InhType = yyP51->AField.Type;
/* line 372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFieldList (yyP51->AField.Next, yyP50->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP51->Kind == kNoField) {
  if (yyP50->Kind == kAnExpr) {
/* line 373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP50->AnExpr.Expr -> Expr . InhType = NoTree;
/* line 373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFieldList (yyP51, yyP50->AnExpr.Next);
  }
   return;

  }
  }
/* line 374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

;
}

void InheritFromTable
# if defined __STDC__ | defined __cplusplus
(register tTree yyP53, register tTree yyP52)
# else
(yyP53, yyP52)
 register tTree yyP53;
 register tTree yyP52;
# endif
{
  if (Tree_IsType (yyP53, kArray)) {
/* line 378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (yyP53->Array.Type, yyP52);
  }
   return;

  }
  if (Tree_IsType (yyP53, kUserDataTypes)) {
/* line 379 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 379 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromFieldList (yyP53->UserDataTypes.FieldList, yyP52);
  }
   return;

  }
/* line 380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (yyP53, yyP52);
  }
   return;

;
}

void InheritFromType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP55, register tTree yyP54)
# else
(yyP55, yyP54)
 register tTree yyP55;
 register tTree yyP54;
# endif
{
  if (yyP54->Kind == kNoExpr) {
/* line 384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP55 == NULL) {
  if (yyP54->Kind == kAnExpr) {
/* line 385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP54->AnExpr.Expr -> Expr . InhType = NULL;
/* line 385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (NULL, yyP54->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP55->Kind == kAType) {
  if (yyP54->Kind == kAnExpr) {
  if (yyP54->AnExpr.Expr->Kind == kValof) {
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP54->AnExpr.Expr -> Expr . InhType = yyP55;
  }
   return;

  }
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP54->AnExpr.Expr -> Expr . InhType = yyP55->AType.Type;
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (yyP55->AType.Next, yyP54->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP55->Kind == kNoType) {
  if (yyP54->Kind == kAnExpr) {
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP54->AnExpr.Expr -> Expr . InhType = NoTree;
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (yyP55, yyP54->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP54->Kind == kAnExpr) {
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP54->AnExpr.Expr -> Expr . InhType = yyP55;
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritFromType (yyP55, yyP54->AnExpr.Next);
  }
   return;

  }
;
}

static void InheritAttributes
# if defined __STDC__ | defined __cplusplus
(register tTree yyP57, register tTree yyP56)
# else
(yyP57, yyP56)
 register tTree yyP57;
 register tTree yyP56;
# endif
{
/* line 393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Tree_tKind K; 
/* line 394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (yyP56, yyP57 -> Kind)))) goto yyL1;
  {
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   K = yyP56 -> Kind;
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   memcpy (yyP56, yyP57, Tree_NodeSize [yyP57 -> Kind]);
/* line 397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP56 -> Kind = K;
  }
  }
   return;
yyL1:;

/* line 399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Attempt to inherit attributes from non-parental node", xxError, NoPosition);
  }
   return;

;
}

tTree TransformUserLiterals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP59, register tTree yyP58)
# else
(yyP59, yyP58)
 register tTree yyP59;
 register tTree yyP58;
# endif
{
/* line 403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree NEW; 
  if (Tree_IsType (yyP59, kUserDataTypes)) {
  if (Tree_IsType (yyP58, kTable)) {
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   InheritAttributes (yyP58, NEW = MakeTree (kUserLiteral));
/* line 404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NEW -> UserLiteral . Type = CopyTree (yyP59);
  }
   return NEW;

  }
  }
/* line 405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP58;

}

void TypeCheckAssign
# if defined __STDC__ | defined __cplusplus
(register tTree yyP62, register tTree yyP61, register tPosition yyP60)
# else
(yyP62, yyP61, yyP60)
 register tTree yyP62;
 register tTree yyP61;
 register tPosition yyP60;
# endif
{
  if (yyP62->Kind == kAnExpr) {
  if (yyP62->AnExpr.Expr->Kind == kElementalExpr) {
  if (yyP61->Kind == kAnExpr) {
/* line 413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (ExprType (yyP61->AnExpr.Expr), kAType)))) goto yyL1;
  {
/* line 414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignFn (yyP62, ExprType (yyP61->AnExpr.Expr), yyP62->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL1:;

  }
  }
  }
/* line 415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP62, yyP61, yyP60);
  }
   return;

;
}

static void TypeCheckAssignList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP65, register tTree yyP64, register tPosition yyP63)
# else
(yyP65, yyP64, yyP63)
 register tTree yyP65;
 register tTree yyP64;
 register tPosition yyP63;
# endif
{
  if (yyP65->Kind == kNoExpr) {
  if (yyP64->Kind == kNoExpr) {
/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many expressions on assignment RHS", xxError, yyP63);
  }
   return;

  }
  if (yyP64->Kind == kNoExpr) {
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few expressions on assignment RHS", xxError, yyP63);
  }
   return;

  }
  if (yyP65->Kind == kAnExpr) {
  if (yyP65->AnExpr.Expr->Kind == kElementalExpr) {
  if (yyP64->Kind == kAnExpr) {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsChanOrTimerType (ElementType (yyP65->AnExpr.Expr))))) goto yyL4;
  {
/* line 423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cannot assign to channel or timer", xxError, yyP65->AnExpr.Expr->ElementalExpr.Pos);
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP65->AnExpr.Next, yyP64->AnExpr.Next, yyP65->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL4:;

/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP65->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL5;
  {
/* line 425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (ExprType (yyP65->AnExpr.Expr), ExprType (yyP64->AnExpr.Expr))))) goto yyL5;
  {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP65->AnExpr.Next, yyP64->AnExpr.Next, yyP65->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
  }
   return;
yyL5:;

/* line 427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP65->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL6;
  {
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch during assignment", xxError, yyP65->AnExpr.Expr->ElementalExpr.Pos);
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP65->AnExpr.Next, yyP64->AnExpr.Next, yyP65->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL6:;

/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cannot make assignment to variable", xxError, yyP65->AnExpr.Expr->ElementalExpr.Pos);
/* line 432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP65->AnExpr.Next, yyP64->AnExpr.Next, yyP65->AnExpr.Expr->ElementalExpr.Pos);
  }
   return;

  }
  }
  if (yyP64->Kind == kAnExpr) {
/* line 433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("LHS must be an assignable variable", xxError, yyP63);
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignList (yyP65->AnExpr.Next, yyP64->AnExpr.Next, yyP63);
  }
   return;

  }
  }
;
}

static void TypeCheckAssignFn
# if defined __STDC__ | defined __cplusplus
(register tTree yyP68, register tTree yyP67, register tPosition yyP66)
# else
(yyP68, yyP67, yyP66)
 register tTree yyP68;
 register tTree yyP67;
 register tPosition yyP66;
# endif
{
  if (yyP68->Kind == kNoExpr) {
  if (yyP67->Kind == kNoType) {
/* line 439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few variables to assign to from function", xxError, yyP66);
  }
   return;

  }
  if (yyP67->Kind == kNoType) {
/* line 441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many variables to assign to from function", xxError, yyP66);
  }
   return;

  }
  if (yyP68->Kind == kAnExpr) {
  if (yyP68->AnExpr.Expr->Kind == kElementalExpr) {
  if (yyP67->Kind == kAType) {
/* line 442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP68->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL4;
  {
/* line 442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (ExprType (yyP68->AnExpr.Expr), yyP67->AType.Type))) goto yyL4;
  {
/* line 443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignFn (yyP68->AnExpr.Next, yyP67->AType.Next, yyP68->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
  }
   return;
yyL4:;

/* line 444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP68->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL5;
  {
/* line 445 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch during function assignment", xxError, yyP68->AnExpr.Expr->ElementalExpr.Pos);
/* line 446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignFn (yyP68->AnExpr.Next, yyP67->AType.Next, yyP68->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL5:;

/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cannot make assignment to variable", xxError, yyP68->AnExpr.Expr->ElementalExpr.Pos);
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckAssignFn (yyP68->AnExpr.Next, yyP67->AType.Next, yyP68->AnExpr.Expr->ElementalExpr.Pos);
  }
   return;

  }
  }
  }
;
}

void TypeCheckResults
# if defined __STDC__ | defined __cplusplus
(register tTree yyP71, register tTree yyP70, register tPosition yyP69)
# else
(yyP71, yyP70, yyP69)
 register tTree yyP71;
 register tTree yyP70;
 register tPosition yyP69;
# endif
{
  if (yyP71->Kind == kNoType) {
  if (yyP70->Kind == kNoExpr) {
/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many expressions in result list", xxError, yyP69);
  }
   return;

  }
  if (yyP70->Kind == kNoExpr) {
/* line 455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few expressions in result list", xxError, yyP69);
  }
   return;

  }
  if (yyP71->Kind == kAType) {
  if (yyP70->Kind == kAnExpr) {
/* line 456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (yyP71->AType.Type, ExprType (yyP70->AnExpr.Expr))))) goto yyL4;
  {
/* line 456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckResults (yyP71->AType.Next, yyP70->AnExpr.Next, yyP70->AnExpr.Expr->Expr.Pos);
  }
  }
   return;
yyL4:;

/* line 457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch in result list", xxError, yyP70->AnExpr.Expr->Expr.Pos);
/* line 457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckResults (yyP71->AType.Next, yyP70->AnExpr.Next, yyP70->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
;
}

void TypeCheckParameters
# if defined __STDC__ | defined __cplusplus
(register tTree yyP75, register tTree yyP74, register tPosition yyP73, register int yyP72)
# else
(yyP75, yyP74, yyP73, yyP72)
 register tTree yyP75;
 register tTree yyP74;
 register tPosition yyP73;
 register int yyP72;
# endif
{
  if (Tree_IsType (yyP75, kNoFormal)) {
  if (yyP74->Kind == kNoExpr) {
/* line 461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many parameters", xxError, yyP73);
  }
   return;

  }
  if (yyP74->Kind == kNoExpr) {
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few parameters", xxError, yyP73);
  }
   return;

  }
  if (yyP75->Kind == kAFormal) {
  if (yyP75->AFormal.By->Kind == kByRef) {
  if (yyP74->Kind == kAnExpr) {
  if (yyP74->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP74->AnExpr.Expr->ElementalExpr.Elemental, kTable)) {
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP74->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL4;
  {
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP75->AFormal.Type, ExprType (yyP74->AnExpr.Expr)))) goto yyL4;
  {
/* line 465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP74->AnExpr.Expr->ElementalExpr.Elemental, yyP75 -> AFormal . U);
/* line 466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckParameters (yyP75->AFormal.Next, yyP74->AnExpr.Next, yyP73, yyP72);
  }
  }
  }
   return;
yyL4:;

  }
  if (Tree_IsType (yyP74->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP74->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL5;
  {
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP75->AFormal.Type, ExprType (yyP74->AnExpr.Expr)))) goto yyL5;
  {
/* line 469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (!CheckValofScope(yyP74->AnExpr.Expr->ElementalExpr.Elemental,yyP72)) Message("Attempted to pass free variable by reference within VALOF",xxError,yyP73); 
/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP74->AnExpr.Expr->ElementalExpr.Elemental, yyP75 -> AFormal . U);
/* line 471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckParameters (yyP75->AFormal.Next, yyP74->AnExpr.Next, yyP73, yyP72);
  }
  }
  }
   return;
yyL5:;

  }
  }
/* line 476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP75->AFormal.Type, ExprType (yyP74->AnExpr.Expr)))) goto yyL7;
  {
/* line 477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Actual parameter cannot be passed by reference", xxError, yyP73);
/* line 478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckParameters (yyP75->AFormal.Next, yyP74->AnExpr.Next, yyP73, yyP72);
  }
  }
   return;
yyL7:;

  }
  }
  if (yyP75->AFormal.By->Kind == kByVal) {
  if (yyP74->Kind == kAnExpr) {
/* line 473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP75->AFormal.Type, ExprType (yyP74->AnExpr.Expr)))) goto yyL6;
  {
/* line 474 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckParameters (yyP75->AFormal.Next, yyP74->AnExpr.Next, yyP73, yyP72);
  }
  }
   return;
yyL6:;

  }
  }
  if (yyP74->Kind == kAnExpr) {
/* line 480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Actual parameter does not match formal description", xxError, yyP74->AnExpr.Expr->Expr.Pos);
/* line 482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckParameters (yyP75->AFormal.Next, yyP74->AnExpr.Next, yyP73, yyP72);
  }
   return;

  }
  }
;
}

void UpdateElementalUsage
# if defined __STDC__ | defined __cplusplus
(register tTree yyP77, tUsage yyP76)
# else
(yyP77, yyP76)
 register tTree yyP77;
 tUsage yyP76;
# endif
{
/* line 487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tUsage tmp; 

  switch (yyP77->Kind) {
  case kIdentifier:
  case kUnmappedIdentifier:
/* line 488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP77->Identifier.Sym), kAFormal)))) goto yyL1;
  {
/* line 489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Identifier . U = yyP76;
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP77->Identifier.Sym) -> AFormal . U = UpdateUsage (SymbolKind (yyP77->Identifier.Sym) -> AFormal . U, yyP76);
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (SymbolKind(yyP77->Identifier.Sym)->AFormal.U==xxInOut) Message("Passed channel used for both input and output",xxError,yyP77->Identifier.Pos); 
  }
  }
   return;
yyL1:;

/* line 493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP77->Identifier.Sym), kIs)))) goto yyL2;
  {
/* line 494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Identifier . U = yyP76;
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP77->Identifier.Sym) -> Is . U = UpdateUsage (SymbolKind (yyP77->Identifier.Sym) -> Is . U, yyP76);
/* line 496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   tmp = SymbolKind (yyP77->Identifier.Sym) -> Is . U;
/* line 497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (tmp==xxInput||tmp==xxOutput||tmp==xxInOut) UpdateElementalUsage(SymbolKind(yyP77->Identifier.Sym)->Is.Elemental,yyP76);
/* line 498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (SymbolKind(yyP77->Identifier.Sym)->Is.U==xxInOut) Message("Abbreviated channel used for both input and output",(IS_Mode(xxUsageWarnings))?xxWarning:xxError,yyP77->Identifier.Pos); 
  }
  }
   return;
yyL2:;

/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP77->Identifier.Sym), kAnId)))) goto yyL3;
  {
/* line 501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Identifier . U = yyP76;
/* line 502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP77->Identifier.Sym) -> AnId . U = UpdateUsage (SymbolKind (yyP77->Identifier.Sym) -> AnId . U, yyP76);
  }
  }
   return;
yyL3:;

  break;
  case kRange:
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Range . U = yyP76;
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->Range.Next, yyP76);
  }
   return;

  case kSubscript:
/* line 505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Subscript . U = yyP76;
/* line 505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->Subscript.Next, yyP76);
  }
   return;

  case kTableRange:
/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> TableRange . U = yyP76;
/* line 506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->TableRange.Next, yyP76);
  }
   return;

  case kTableSubscript:
/* line 507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> TableSubscript . U = yyP76;
/* line 507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->TableSubscript.Next, yyP76);
  }
   return;

  case kTable:
  case kUserLiteral:
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP77 -> Table . U = yyP76;
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->Table.ExprList, yyP76);
  }
   return;

  case kAnExpr:
  if (yyP77->AnExpr.Expr->Kind == kElementalExpr) {
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->AnExpr.Expr->ElementalExpr.Elemental, yyP76);
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->AnExpr.Next, yyP76);
  }
   return;

  }
/* line 510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   UpdateElementalUsage (yyP77->AnExpr.Next, yyP76);
  }
   return;

  }

;
}

void TypeCheckIfList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP78)
# else
(yyP78)
 register tTree yyP78;
# endif
{
  if (yyP78->Kind == kNoChoice) {
/* line 514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP78->Kind == kAChoice) {
  if (yyP78->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP78->AChoice.ExprList->AnExpr.Next->Kind == kNoExpr) {
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (ExprType (yyP78->AChoice.ExprList->AnExpr.Expr), kBool)))) goto yyL2;
  {
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->AChoice.Next);
  }
  }
   return;
yyL2:;

/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Conditional not of type BOOL", xxError, yyP78->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->AChoice.Next);
  }
   return;

  }
  }
  }
  if (yyP78->Kind == kANewIf) {
  if (yyP78->ANewIf.IfType->Kind == kIf) {
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->ANewIf.IfType->If.ChoiceList);
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->ANewIf.Next);
  }
   return;

  }
  if (yyP78->ANewIf.IfType->Kind == kIfRep) {
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->ANewIf.IfType->IfRep.ChoiceList);
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckIfList (yyP78->ANewIf.Next);
  }
   return;

  }
  }
;
}

void TypeCheckTimerInput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP80, register tPosition yyP79)
# else
(yyP80, yyP79)
 register tTree yyP80;
 register tPosition yyP79;
# endif
{
  if (yyP80->Kind == kAnExpr) {
  if (yyP80->AnExpr.Next->Kind == kNoExpr) {
  if (yyP80->AnExpr.Expr->Kind == kElementalExpr) {
/* line 522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP80->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL1;
  {
/* line 522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (ExprType (yyP80->AnExpr.Expr), kInt)))) goto yyL1;
  }
  }
   return;
yyL1:;

  }
/* line 523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Timer input not to writable INT variable", xxError, yyP80->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
/* line 524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Only single input allowed from TIMER", xxError, yyP79);
  }
   return;

  }
  if (yyP80->Kind == kNoExpr) {
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Missing variable for timer input", xxError, yyP79);
  }
   return;

  }
;
}

void TypeCheckInputProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP83, register tTree yyP82, register tPosition yyP81)
# else
(yyP83, yyP82, yyP81)
 register tTree yyP83;
 register tTree yyP82;
 register tPosition yyP81;
# endif
{
  if (yyP83->Kind == kAny) {
  if (yyP82->Kind == kAnExpr) {
  if (yyP82->AnExpr.Next->Kind == kNoExpr) {
  if (yyP82->AnExpr.Expr->Kind == kElementalExpr) {
/* line 529 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 529 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL1;
  }
   return;
yyL1:;

  }
  if (yyP82->AnExpr.Expr->Kind == kCountArray) {
  if (yyP82->AnExpr.Expr->CountArray.Length->Kind == kElementalExpr) {
  if (yyP82->AnExpr.Expr->CountArray.Expr->Kind == kElementalExpr) {
/* line 531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Length->ElementalExpr.Elemental)))) goto yyL3;
  {
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental)))) goto yyL3;
  {
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (yyP82->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental -> Element . SynType, kArray)))) goto yyL3;
  }
  }
  }
   return;
yyL3:;

  }
  }
  }
/* line 535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Input variable not writable", xxError, yyP82->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  if (yyP82->AnExpr.Expr->Kind == kElementalExpr) {
/* line 530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL2;
  {
/* line 530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputProtocol (yyP83, yyP82->AnExpr.Next, yyP82->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL2:;

  }
  if (yyP82->AnExpr.Expr->Kind == kCountArray) {
  if (yyP82->AnExpr.Expr->CountArray.Length->Kind == kElementalExpr) {
  if (yyP82->AnExpr.Expr->CountArray.Expr->Kind == kElementalExpr) {
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Length->ElementalExpr.Elemental)))) goto yyL4;
  {
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental)))) goto yyL4;
  {
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (yyP82->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental -> Element . SynType, kArray)))) goto yyL4;
  {
/* line 534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputProtocol (yyP83, yyP82->AnExpr.Next, yyP82->AnExpr.Expr->CountArray.Pos);
  }
  }
  }
  }
   return;
yyL4:;

  }
  }
  }
/* line 536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Input variable not writable", xxError, yyP82->AnExpr.Expr->Expr.Pos);
/* line 536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputProtocol (yyP83, yyP82->AnExpr.Next, yyP82->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
  if (yyP82->Kind == kNoExpr) {
/* line 538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("No expression list present for channel input", xxError, yyP81);
  }
   return;

  }
  if (yyP83->Kind == kNamed) {
/* line 540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP83->Named.Sym), kDefSeqPro)))) goto yyL8;
  {
/* line 540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputSeqProtocol (SymbolKind (yyP83->Named.Sym) -> DefSeqPro . SeqProList, yyP82, yyP81);
  }
  }
   return;
yyL8:;

/* line 541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Invalid form of variant protocol input", xxError, yyP81);
  }
   return;

  }
  if (yyP82->Kind == kAnExpr) {
  if (yyP82->AnExpr.Next->Kind == kNoExpr) {
  if (yyP82->AnExpr.Expr->Kind == kElementalExpr) {
/* line 542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL10;
  {
/* line 542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP83, yyP82->AnExpr.Expr);
  }
  }
   return;
yyL10:;

  }
  if (yyP82->AnExpr.Expr->Kind == kCountArray) {
  if (yyP82->AnExpr.Expr->CountArray.Length->Kind == kElementalExpr) {
  if (yyP82->AnExpr.Expr->CountArray.Expr->Kind == kElementalExpr) {
/* line 543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Length->ElementalExpr.Elemental)))) goto yyL11;
  {
/* line 543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP82->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental)))) goto yyL11;
  {
/* line 543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP83, yyP82->AnExpr.Expr);
  }
  }
  }
   return;
yyL11:;

  }
  }
  }
/* line 544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Input item not assignable", xxError, yyP82->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
/* line 545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many inputs for a channel with a simple protocol", xxError, yyP82->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
;
}

static void TypeCheckSimpleProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP85, register tTree yyP84)
# else
(yyP85, yyP84)
 register tTree yyP85;
 register tTree yyP84;
# endif
{
  if (yyP85->Kind == kSimple) {
  if (yyP84->Kind == kCountArray) {
/* line 550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Protocol does not expect a counted array", xxError, yyP84->CountArray.Pos);
  }
   return;

  }
/* line 551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP85->Simple.Type, ExprType (yyP84)))) goto yyL2;
  }
   return;
yyL2:;

/* line 552 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 552 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Variable is not of correct type for protocol", xxError, yyP84->Expr.Pos);
  }
   return;

  }
  if (yyP85->Kind == kCounted) {
  if (yyP84->Kind == kCountArray) {
/* line 553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (yyP85->Counted.Length, ExprType (yyP84->CountArray.Length))))) goto yyL4;
  {
/* line 553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (yyP85->Counted.Type, ExprType (yyP84->CountArray.Expr))))) goto yyL4;
  }
  }
   return;
yyL4:;

/* line 554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Counted array expression types do not match protocol", xxError, yyP84->CountArray.Pos);
  }
   return;

  }
/* line 555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SameType (yyP85->Counted.Type, ExprType (yyP84))))) goto yyL6;
  }
   return;
yyL6:;

/* line 556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Output expression does not match protocol", xxError, yyP84->Expr.Pos);
  }
   return;

  }
;
}

void TypeCheckOutputProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP88, register tTree yyP87, register tPosition yyP86)
# else
(yyP88, yyP87, yyP86)
 register tTree yyP88;
 register tTree yyP87;
 register tPosition yyP86;
# endif
{
  if (yyP88->Kind == kAny) {
  if (yyP87->Kind == kAnExpr) {
  if (yyP87->AnExpr.Next->Kind == kNoExpr) {
/* line 563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckOutputProtocol (yyP88, yyP87->AnExpr.Next, yyP87->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
  if (yyP87->Kind == kNoExpr) {
/* line 567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 568 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("No expression list present for channel output", xxError, yyP86);
  }
   return;

  }
  if (yyP88->Kind == kNamed) {
/* line 570 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 570 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP88->Named.Sym), kDefSeqPro)))) goto yyL4;
  {
/* line 571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckOutputSeqProtocol (SymbolKind (yyP88->Named.Sym) -> DefSeqPro . SeqProList, yyP87, yyP86);
  }
  }
   return;
yyL4:;

/* line 573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP88->Named.Sym), kDefTagPro)))) goto yyL5;
  {
/* line 574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseOutput (SymbolKind (yyP88->Named.Sym) -> DefTagPro . TagProList, yyP87, yyP86);
  }
  }
   return;
yyL5:;

/* line 576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Not a valid named", xxError, yyP86);
  }
   return;

  }
  if (yyP87->Kind == kAnExpr) {
  if (yyP87->AnExpr.Next->Kind == kNoExpr) {
/* line 579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP88, yyP87->AnExpr.Expr);
  }
   return;

  }
/* line 582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many outputs for a channel with a simple protocol", xxError, yyP87->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
;
}

void TypeCheckCaseInputs
# if defined __STDC__ | defined __cplusplus
(register tTree yyP91, register tTree yyP90, register tPosition yyP89)
# else
(yyP91, yyP90, yyP89)
 register tTree yyP91;
 register tTree yyP90;
 register tPosition yyP89;
# endif
{
  if (yyP90->Kind == kNoCase) {
/* line 588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP90->Kind == kACase) {
  if (yyP90->ACase.ExprList->Kind == kAnExpr) {
  if (yyP90->ACase.ExprList->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP90->ACase.ExprList->AnExpr.Expr->ElementalExpr.Elemental, kIdentifier)) {
/* line 589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((CheckValidTag (yyP91, yyP90->ACase.ExprList->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym)))) goto yyL2;
  {
/* line 589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputSeqProtocol (SymbolKind (yyP90->ACase.ExprList->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym) -> ATagPro . SeqProList, yyP90->ACase.ExprList->AnExpr.Next, yyP89);
/* line 589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseInputs (yyP91, yyP90->ACase.Next, yyP89);
  }
  }
   return;
yyL2:;

  }
  }
/* line 590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Variant tag is not valid for this protocol", xxError, yyP90->ACase.ExprList->AnExpr.Expr->Expr.Pos);
/* line 590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseInputs (yyP91, yyP90->ACase.Next, yyP89);
  }
   return;

  }
  if (yyP90->ACase.ExprList->Kind == kNoExpr) {
/* line 591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cased input missing variant protocol", xxError, yyP89);
/* line 591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseInputs (yyP91, yyP90->ACase.Next, yyP89);
  }
   return;

  }
  }
;
}

static void TypeCheckCaseOutput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP94, register tTree yyP93, register tPosition yyP92)
# else
(yyP94, yyP93, yyP92)
 register tTree yyP94;
 register tTree yyP93;
 register tPosition yyP92;
# endif
{
  if (yyP93->Kind == kAnExpr) {
  if (yyP93->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP93->AnExpr.Expr->ElementalExpr.Elemental, kIdentifier)) {
/* line 595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((CheckValidTag (yyP94, yyP93->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym)))) goto yyL1;
  {
/* line 596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckOutputSeqProtocol (SymbolKind (yyP93->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym) -> ATagPro . SeqProList, yyP93->AnExpr.Next, yyP92);
  }
  }
   return;
yyL1:;

  }
  }
/* line 598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Variant tag is not valid for this protocol", xxError, yyP93->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  if (yyP93->Kind == kNoExpr) {
/* line 601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 602 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cased output missing variant protocol", xxError, yyP92);
  }
   return;

  }
;
}

static void TypeCheckInputSeqProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP97, register tTree yyP96, register tPosition yyP95)
# else
(yyP97, yyP96, yyP95)
 register tTree yyP97;
 register tTree yyP96;
 register tPosition yyP95;
# endif
{
  if (yyP97->Kind == kNoSeqPro) {
  if (yyP96->Kind == kNoExpr) {
/* line 607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP96->Kind == kAnExpr) {
/* line 608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many input expressions for channel protocol", xxError, yyP96->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
  if (yyP96->Kind == kNoExpr) {
/* line 609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few input expressions for channel protocol", xxError, yyP95);
  }
   return;

  }
  if (yyP97->Kind == kASeqPro) {
  if (yyP96->Kind == kAnExpr) {
  if (yyP96->AnExpr.Expr->Kind == kElementalExpr) {
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Writable (yyP96->AnExpr.Expr->ElementalExpr.Elemental)))) goto yyL4;
  {
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP97->ASeqPro.Protocol, yyP96->AnExpr.Expr);
/* line 610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputSeqProtocol (yyP97->ASeqPro.Next, yyP96->AnExpr.Next, yyP96->AnExpr.Expr->ElementalExpr.Pos);
  }
  }
   return;
yyL4:;

  }
  if (yyP96->AnExpr.Expr->Kind == kCountArray) {
/* line 611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP97->ASeqPro.Protocol, yyP96->AnExpr.Expr);
/* line 611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputSeqProtocol (yyP97->ASeqPro.Next, yyP96->AnExpr.Next, yyP96->AnExpr.Expr->CountArray.Pos);
  }
   return;

  }
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Not an assignable expression", xxError, yyP96->AnExpr.Expr->Expr.Pos);
/* line 612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckInputSeqProtocol (yyP97->ASeqPro.Next, yyP96->AnExpr.Next, yyP96->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
;
}

static void TypeCheckOutputSeqProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP100, register tTree yyP99, register tPosition yyP98)
# else
(yyP100, yyP99, yyP98)
 register tTree yyP100;
 register tTree yyP99;
 register tPosition yyP98;
# endif
{
  if (yyP100->Kind == kNoSeqPro) {
  if (yyP99->Kind == kNoExpr) {
/* line 616 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP99->Kind == kAnExpr) {
/* line 617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many output expressions for channel protocol", xxError, yyP99->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
  if (yyP99->Kind == kNoExpr) {
/* line 618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few output expressions for channel protocol", xxError, yyP98);
  }
   return;

  }
  if (yyP100->Kind == kASeqPro) {
  if (yyP99->Kind == kAnExpr) {
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckSimpleProtocol (yyP100->ASeqPro.Protocol, yyP99->AnExpr.Expr);
/* line 619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckOutputSeqProtocol (yyP100->ASeqPro.Next, yyP99->AnExpr.Next, yyP99->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
;
}

void TypeCheckCaseList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP103, register tTree yyP102, register bool yyP101)
# else
(yyP103, yyP102, yyP101)
 register tTree yyP103;
 register tTree yyP102;
 register bool yyP101;
# endif
{
  if (yyP102->Kind == kNoChoice) {
/* line 623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
  if (yyP102->Kind == kAChoice) {
  if (yyP102->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP102->AChoice.ExprList->AnExpr.Next->Kind == kNoExpr) {
  if (yyP102->AChoice.ExprList->AnExpr.Expr->Kind == kDefault) {
  if (equalbool (yyP101, false)) {
/* line 624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseList (yyP103, yyP102->AChoice.Next, true);
  }
   return;

  }
/* line 625 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 625 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Multiple ELSE options in selection", xxError, yyP102->AChoice.ExprList->AnExpr.Expr->Default.Pos);
  }
   return;

  }
  }
/* line 626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 CountList(yyP102->AChoice.ExprList,yyP103);
/* line 628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TypeCheckCaseList (yyP103, yyP102->AChoice.Next, yyP101);
/* line 629 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (!ConstFlag(yyP102->AChoice.ExprList->ExprList))
	Message("CASE options must be constant",xxError,yyP102->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
      if (!(Tree_IsType(ExprListType(yyP102->AChoice.ExprList),kInteger)||Tree_IsType(ExprListType(yyP102->AChoice.ExprList),kByte)||Tree_IsType(ExprListType(yyP102->AChoice.ExprList),kBool)))
	Message("CASE options must be integer or BYTE (or BOOL)",xxError,yyP102->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
      else 
	if (!CheckChoicesDiffer(yyP102->AChoice.ExprList,yyP102->AChoice.Next)) Message("CASE options must be distinct",xxError,yyP102->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
  }
;
}

void TypeCheckRetypes
# if defined __STDC__ | defined __cplusplus
(register tTree yyP107, register tTree yyP106, register tTree yyP105, register tPosition yyP104)
# else
(yyP107, yyP106, yyP105, yyP104)
 register tTree yyP107;
 register tTree yyP106;
 register tTree yyP105;
 register tPosition yyP104;
# endif
{
/* line 638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int LS,RS; 
  if (yyP107->Kind == kReshapes) {
/* line 639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tTree yyV1;
  tTree yyV2;
  {
/* line 640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   LS = SizeType (yyP106, yyP104, true, & yyV1);
/* line 641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   RS = SizeType (yyP105, yyP104, false, & yyV2);
/* line 642 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((LS >= 0))) goto yyL1;
  {
/* line 643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   DoTypeCheckRetypes (LS, yyV1, RS, yyV2, yyP104);
  }
  }
   return;
 }
yyL1:;

/* line 644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tTree yyV1;
  tTree yyV2;
  {
/* line 647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   LS = SizeType (yyP106, yyP104, true, & yyV1);
/* line 648 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   RS = SizeType (yyP105, yyP104, false, & yyV2);
/* line 649 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   DoTypeCheckRetypes (LS, yyV1, RS, yyV2, yyP104);
  }
   return;
 }

;
}

static void DoTypeCheckRetypes
# if defined __STDC__ | defined __cplusplus
(register int yyP112, register tTree yyP111, register int yyP110, register tTree yyP109, register tPosition yyP108)
# else
(yyP112, yyP111, yyP110, yyP109, yyP108)
 register int yyP112;
 register tTree yyP111;
 register int yyP110;
 register tTree yyP109;
 register tPosition yyP108;
# endif
{
  if (equalint (yyP110, - 1)) {
/* line 652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
/* line 653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP109 != NoTree))) goto yyL2;
  }
   return;
yyL2:;

  if (yyP111 == NULL) {
  if (yyP109 == NULL) {
/* line 654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP112 == yyP110))) goto yyL3;
  }
   return;
yyL3:;

/* line 655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Size of retype does not match expression size", xxError, yyP108);
  }
   return;

  }
  }
  if (yyP109 == NULL) {
/* line 656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (((yyP110 % yyP112) == 0))) goto yyL5;
  {
/* line 656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP111 -> Array . Expr = MakeConstInt (yyP110 / yyP112, yyP108);
  }
  }
   return;
yyL5:;

/* line 657 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 657 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Size of retype cannot match expression size", xxError, yyP108);
  }
   return;

  }
;
}

bool IsUserDataType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP113)
# else
(yyP113)
 register tTree yyP113;
# endif
{
  if (yyP113 == NULL) {
/* line 664 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 664 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }

  switch (yyP113->Kind) {
  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserArray:
/* line 666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserInt:
/* line 667 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserInt16:
/* line 668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserInt32:
/* line 669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserInt64:
/* line 670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserReal32:
/* line 671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserReal64:
/* line 672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserByte:
/* line 673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  case kUserBool:
/* line 674 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }

  return false;
}

static tSymbol WhichUserDataType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP114)
# else
(yyP114)
 register tTree yyP114;
# endif
{
  if (yyP114 == NULL) {
/* line 678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NoSymbol;

  }

  switch (yyP114->Kind) {
  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 679 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserStructTypes.Sym;

  case kUserArray:
/* line 680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserArray.Sym;

  case kUserInt:
/* line 681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserInt.Sym;

  case kUserInt16:
/* line 682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserInt16.Sym;

  case kUserInt32:
/* line 683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserInt32.Sym;

  case kUserInt64:
/* line 684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserInt64.Sym;

  case kUserReal32:
/* line 685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserReal32.Sym;

  case kUserReal64:
/* line 686 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserReal64.Sym;

  case kUserByte:
/* line 687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserByte.Sym;

  case kUserBool:
/* line 688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP114->UserBool.Sym;

  }

/* line 689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NoSymbol;

}

bool IsComputableType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP115)
# else
(yyP115)
 register tTree yyP115;
# endif
{
  if (Tree_IsType (yyP115, kInteger)) {
/* line 693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP115, kByte)) {
/* line 694 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 694 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xxUserDataTypes)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  return false;
}

bool SameBaseType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP117, register tTree yyP116)
# else
(yyP117, yyP116)
 register tTree yyP117;
 register tTree yyP116;
# endif
{
  if (Tree_IsType (yyP117, kArray)) {
/* line 698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameBaseType (yyP117->Array.Type, yyP116))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (Tree_IsType (yyP116, kArray)) {
/* line 699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameBaseType (yyP117, yyP116->Array.Type))) goto yyL2;
  }
   return true;
yyL2:;

  }
/* line 700 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 700 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP117, yyP116))) goto yyL3;
  }
   return true;
yyL3:;

  return false;
}

bool SameUnderlyingType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP119, register tTree yyP118)
# else
(yyP119, yyP118)
 register tTree yyP119;
 register tTree yyP118;
# endif
{

  switch (yyP119->Kind) {
  case kInt16:
  case kUserInt16:
  if (Tree_IsType (yyP118, kInt16)) {
/* line 704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kInt32:
  case kUserInt32:
  if (Tree_IsType (yyP118, kInt32)) {
/* line 705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kInt64:
  case kUserInt64:
  if (Tree_IsType (yyP118, kInt64)) {
/* line 706 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kInt:
  case kUserInt:
  if (Tree_IsType (yyP118, kInt)) {
/* line 707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kBool:
  case kUserBool:
  if (Tree_IsType (yyP118, kBool)) {
/* line 708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kReal32:
  case kUserReal32:
  if (Tree_IsType (yyP118, kReal32)) {
/* line 709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kReal64:
  case kUserReal64:
  if (Tree_IsType (yyP118, kReal64)) {
/* line 710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kByte:
  case kUserByte:
  if (Tree_IsType (yyP118, kByte)) {
/* line 711 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kTimer:
  if (yyP118->Kind == kTimer) {
/* line 712 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  break;
  case kChan:
  if (yyP118->Kind == kChan) {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameProtocol (yyP119->Chan.Protocol, yyP118->Chan.Protocol))) goto yyL10;
  }
   return true;
yyL10:;

  }
  break;
  case kArray:
  case kUserArray:
  if (yyP119->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP118, kArray)) {
/* line 714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP119->Array.Type, yyP118->Array.Type))) goto yyL11;
  }
   return true;
yyL11:;

/* line 715 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 715 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }
  }
  if (Tree_IsType (yyP118, kArray)) {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP119->Array.Expr)))) goto yyL13;
  {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP118->Array.Expr)))) goto yyL13;
  {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP119->Array.Expr) == ValConstInt (yyP118->Array.Expr)))) goto yyL13;
  {
/* line 716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP119->Array.Type, yyP118->Array.Type))) goto yyL13;
  }
  }
  }
  }
   return true;
yyL13:;

/* line 717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsConstInt (yyP118->Array.Expr)))) goto yyL14;
  {
/* line 717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP119->Array.Type, yyP118->Array.Type))) goto yyL14;
  }
  }
   return true;
yyL14:;

/* line 718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsConstInt (yyP119->Array.Expr)))) goto yyL15;
  {
/* line 718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP119->Array.Type, yyP118->Array.Type))) goto yyL15;
  }
  }
   return true;
yyL15:;

  }
  break;
  }

  return false;
}

static bool SameType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP121, register tTree yyP120)
# else
(yyP121, yyP120)
 register tTree yyP121;
 register tTree yyP120;
# endif
{
/* line 722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tSymbol S1=NoSymbol,S2=NoSymbol; 
/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (((S1 = WhichUserDataType (yyP121)) != NoSymbol))) goto yyL1;
  {
/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (((S2 = WhichUserDataType (yyP120)) != NoSymbol))) goto yyL1;
  {
/* line 723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((S1 == S2))) goto yyL1;
  }
  }
  }
   return true;
yyL1:;

/* line 724 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 724 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((S1 != NoSymbol))) goto yyL2;
  {
/* line 724 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }
  }
yyL2:;

/* line 725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((S2 != NoSymbol))) goto yyL3;
  {
/* line 725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }
  }
yyL3:;

/* line 726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameUnderlyingType (yyP121, yyP120))) goto yyL4;
  }
   return true;
yyL4:;

  return false;
}

static bool CheckValidTag
# if defined __STDC__ | defined __cplusplus
(register tTree yyP123, tSymbol yyP122)
# else
(yyP123, yyP122)
 register tTree yyP123;
 tSymbol yyP122;
# endif
{
  if (yyP123->Kind == kATagPro) {
/* line 730 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 730 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP123->ATagPro.Sym == yyP122))) goto yyL1;
  }
   return true;
yyL1:;

/* line 731 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 731 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValidTag (yyP123->ATagPro.Next, yyP122))) goto yyL2;
  }
   return true;
yyL2:;

  }
  return false;
}

static bool SameProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP125, register tTree yyP124)
# else
(yyP125, yyP124)
 register tTree yyP125;
 register tTree yyP124;
# endif
{
  if (yyP125->Kind == kAny) {
/* line 735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP124->Kind == kAny) {
/* line 736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP125->Kind == kNamed) {
  if (yyP124->Kind == kNamed) {
/* line 737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP125->Named.Sym == yyP124->Named.Sym))) goto yyL3;
  }
   return true;
yyL3:;

  }
  }
  if (yyP125->Kind == kSimple) {
  if (yyP124->Kind == kSimple) {
/* line 738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP125->Simple.Type, yyP124->Simple.Type))) goto yyL4;
  }
   return true;
yyL4:;

  }
  }
  if (yyP125->Kind == kCounted) {
  if (yyP124->Kind == kCounted) {
/* line 739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP125->Counted.Length, yyP124->Counted.Length))) goto yyL5;
  {
/* line 739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP125->Counted.Type, yyP124->Counted.Type))) goto yyL5;
  }
  }
   return true;
yyL5:;

  }
  }
  return false;
}

bool CheckWritable
# if defined __STDC__ | defined __cplusplus
(register tTree yyP126)
# else
(yyP126)
 register tTree yyP126;
# endif
{
  if (yyP126->Kind == kAnId) {
/* line 743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP126->Kind == kAFormal) {
  if (yyP126->AFormal.By->Kind == kByRef) {
/* line 744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  }
  if (yyP126->Kind == kIs) {
/* line 745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP126, kRetypes)) {
/* line 746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool CheckWritableExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP127)
# else
(yyP127)
 register tTree yyP127;
# endif
{
  if (yyP127->Kind == kNoExpr) {
/* line 751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP127->Kind == kAnExpr) {
  if (yyP127->AnExpr.Expr->Kind == kElementalExpr) {
/* line 752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CheckWritableExprList (yyP127->AnExpr.Next) && Writable (yyP127->AnExpr.Expr->ElementalExpr.Elemental);

  }
/* line 753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;

  }
 yyAbort ("CheckWritableExprList");
}

bool CheckForVals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP128)
# else
(yyP128)
 register tTree yyP128;
# endif
{
  if (Tree_IsType (yyP128, kNoFormal)) {
/* line 757 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP128->Kind == kAFormal) {
  if (yyP128->AFormal.By->Kind == kByVal) {
/* line 758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckForVals (yyP128->AFormal.Next))) goto yyL2;
  }
   return true;
yyL2:;

  }
  }
  return false;
}

bool CheckValofScope
# if defined __STDC__ | defined __cplusplus
(register tTree yyP130, register int yyP129)
# else
(yyP130, yyP129)
 register tTree yyP130;
 register int yyP129;
# endif
{
  if (yyP130->Kind == kNoExpr) {
/* line 762 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP130->Kind == kAnExpr) {
  if (yyP130->AnExpr.Expr->Kind == kElementalExpr) {
/* line 763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->AnExpr.Expr->ElementalExpr.Elemental, yyP129))) goto yyL2;
  {
/* line 763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->AnExpr.Next, yyP129))) goto yyL2;
  }
  }
   return true;
yyL2:;

/* line 764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }
  if (yyP130->AnExpr.Expr->Kind == kCountArray) {
  if (yyP130->AnExpr.Expr->CountArray.Length->Kind == kElementalExpr) {
  if (yyP130->AnExpr.Expr->CountArray.Expr->Kind == kElementalExpr) {
/* line 765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental, yyP129))) goto yyL4;
  {
/* line 765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->AnExpr.Next, yyP129))) goto yyL4;
  }
  }
   return true;
yyL4:;

/* line 766 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 766 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }
  }
  }
/* line 767 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 767 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->AnExpr.Next, yyP129))) goto yyL6;
  }
   return true;
yyL6:;

  }
  if (yyP130->Kind == kRange) {
/* line 768 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 768 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->Range.Next, yyP129))) goto yyL7;
  }
   return true;
yyL7:;

  }
  if (yyP130->Kind == kSubscript) {
/* line 769 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 769 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValofScope (yyP130->Subscript.Next, yyP129))) goto yyL8;
  }
   return true;
yyL8:;

  }
  if (Tree_IsType (yyP130, kIdentifier)) {
/* line 770 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 770 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Symbol (yyP130->Identifier.Sym) . Depth >= yyP129))) goto yyL9;
  }
   return true;
yyL9:;

  }
  return false;
}

bool IsChanOrTimerType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP131)
# else
(yyP131)
 register tTree yyP131;
# endif
{
  if (Tree_IsType (yyP131, kArray)) {
/* line 774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsChanOrTimerType (yyP131->Array.Type)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP131->Kind == kUserChanRecord) {
/* line 775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP131->Kind == kChan) {
/* line 776 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP131->Kind == kTimer) {
/* line 777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

static bool IsDataType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP132)
# else
(yyP132)
 register tTree yyP132;
# endif
{
  if (Tree_IsType (yyP132, kArray)) {
/* line 781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsDataType (yyP132->Array.Type)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (Tree_IsType (yyP132, kPrimitive)) {
/* line 782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP132, kUserDataTypes)) {
/* line 783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool IsChanType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP133)
# else
(yyP133)
 register tTree yyP133;
# endif
{
  if (Tree_IsType (yyP133, kArray)) {
/* line 787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsChanType (yyP133->Array.Type)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP133->Kind == kUserChanRecord) {
/* line 788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP133->Kind == kChan) {
/* line 789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool IsTimerType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP134)
# else
(yyP134)
 register tTree yyP134;
# endif
{
  if (Tree_IsType (yyP134, kArray)) {
/* line 793 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 793 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsTimerType (yyP134->Array.Type)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP134->Kind == kTimer) {
/* line 795 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool IsArrayType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP135)
# else
(yyP135)
 register tTree yyP135;
# endif
{
  if (Tree_IsType (yyP135, kArray)) {
/* line 801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool IsScalarType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP136)
# else
(yyP136)
 register tTree yyP136;
# endif
{
  if (Tree_IsType (yyP136, kArray)) {
/* line 806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;

  }
  if (Tree_IsType (yyP136, kUserStructTypes)) {
/* line 807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;

  }
/* line 808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

}

static bool CheckChoicesDiffer
# if defined __STDC__ | defined __cplusplus
(register tTree yyP138, register tTree yyP137)
# else
(yyP138, yyP137)
 register tTree yyP138;
 register tTree yyP137;
# endif
{
  if (yyP138->Kind == kNoExpr) {
/* line 812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP138->Kind == kAnExpr) {
/* line 813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckExprDiffers (yyP138->AnExpr.Expr, yyP138->AnExpr.Next, yyP137))) goto yyL2;
  {
/* line 813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckChoicesDiffer (yyP138->AnExpr.Next, yyP137))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckExprDiffers
# if defined __STDC__ | defined __cplusplus
(register tTree yyP141, register tTree yyP140, register tTree yyP139)
# else
(yyP141, yyP140, yyP139)
 register tTree yyP141;
 register tTree yyP140;
 register tTree yyP139;
# endif
{
  if (yyP140->Kind == kNoExpr) {
  if (yyP139->Kind == kNoChoice) {
/* line 817 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP139->Kind == kAChoice) {
/* line 818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckExprDiffers (yyP141, yyP139->AChoice.ExprList, yyP139->AChoice.Next))) goto yyL2;
  }
   return true;
yyL2:;

  }
  }
  if (yyP140->Kind == kAnExpr) {
/* line 819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsConstInt (yyP141)))) goto yyL3;
  }
   return true;
yyL3:;

/* line 820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsConstInt (yyP140->AnExpr.Expr)))) goto yyL4;
  {
/* line 820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckExprDiffers (yyP141, yyP140->AnExpr.Next, yyP139))) goto yyL4;
  }
  }
   return true;
yyL4:;

/* line 821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP141) != ValConstInt (yyP140->AnExpr.Expr)))) goto yyL5;
  {
/* line 821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckExprDiffers (yyP141, yyP140->AnExpr.Next, yyP139))) goto yyL5;
  }
  }
   return true;
yyL5:;

  }
  return false;
}

bool CheckValidDeclType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP142)
# else
(yyP142)
 register tTree yyP142;
# endif
{
  if (Tree_IsType (yyP142, kArray)) {
/* line 825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP142->Array.Expr)))) goto yyL1;
  {
/* line 825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP142->Array.Expr) > 0))) goto yyL1;
  {
/* line 825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckValidDeclType (yyP142->Array.Type))) goto yyL1;
  }
  }
  }
   return true;
yyL1:;

/* line 826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }
/* line 827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

}

bool CheckFuncType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP143)
# else
(yyP143)
 register tTree yyP143;
# endif
{
  if (yyP143->Kind == kAType) {
/* line 831 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 831 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xxUserDataTypes)))) goto yyL1;
  {
/* line 832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((CheckValidDeclType (yyP143->AType.Type)))) goto yyL1;
  {
/* line 833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsDataType (yyP143->AType.Type)))) goto yyL1;
  {
/* line 834 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((CheckFuncType (yyP143->AType.Next)))) goto yyL1;
  }
  }
  }
  }
   return true;
yyL1:;

  if (Tree_IsType (yyP143->AType.Type, kArray)) {
/* line 836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IS_Mode (xxUserDataTypes)))) goto yyL2;
  {
/* line 837 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }
  }
yyL2:;

  }
/* line 839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP143->Kind == kNoType) {
/* line 840 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool CheckEvalBiOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP145, register tTree yyP144)
# else
(yyP145, yyP144)
 register tTree yyP145;
 register tTree yyP144;
# endif
{
  if (Tree_IsType (yyP144, kInteger)) {
/* line 846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckEvalBiIntOp (yyP145))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (Tree_IsType (yyP144, kReal)) {
/* line 848 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 848 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckEvalBiRealOp (yyP145))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (Tree_IsType (yyP144, kByte)) {
/* line 850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckEvalBiIntOp (yyP145))) goto yyL3;
  }
   return true;
yyL3:;

  }
  if (Tree_IsType (yyP145, kLogOp)) {
  if (Tree_IsType (yyP144, kBool)) {
/* line 852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  }
  if (Tree_IsType (yyP145, kRelOp)) {
  if (Tree_IsType (yyP144, kBool)) {
/* line 853 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  }
  if (yyP145->Kind == kAfterOp) {
  if (Tree_IsType (yyP144, kBool)) {
/* line 854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  }
  return false;
}

static bool CheckEvalBiIntOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP146)
# else
(yyP146)
 register tTree yyP146;
# endif
{
  if (Tree_IsType (yyP146, kMathsOp)) {
/* line 857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP146, kClockOp)) {
/* line 858 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP146, kRelOp)) {
/* line 859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP146, kBitOp)) {
/* line 860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP146, kShiftOp)) {
/* line 861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP146->Kind == kAfterOp) {
/* line 862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

static bool CheckEvalBiRealOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP147)
# else
(yyP147)
 register tTree yyP147;
# endif
{
  if (Tree_IsType (yyP147, kMathsOp)) {
/* line 865 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP147, kRelOp)) {
/* line 866 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

bool CheckEvalUniOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP150, register tTree yyP149, register tTree yyP148)
# else
(yyP150, yyP149, yyP148)
 register tTree yyP150;
 register tTree yyP149;
 register tTree yyP148;
# endif
{
  if (Tree_IsType (yyP150, kInteger)) {
  if (yyP149->Kind == kBitNot) {
/* line 872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP149->Kind == kSubtract) {
/* line 874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL3;
  }
   return true;
yyL3:;

  }
  if (yyP149->Kind == kMinus) {
/* line 875 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 875 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL4;
  }
   return true;
yyL4:;

  }
  if (yyP149->Kind == kPlus) {
/* line 876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL5;
  }
   return true;
yyL5:;

  }
  if (yyP149->Kind == kAdd) {
/* line 877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL6;
  }
   return true;
yyL6:;

  }
  }
  if (Tree_IsType (yyP150, kBool)) {
  if (yyP149->Kind == kNot) {
/* line 873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  }
  if (Tree_IsType (yyP150, kReal)) {
  if (yyP149->Kind == kSubtract) {
/* line 878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL7;
  }
   return true;
yyL7:;

  }
  if (yyP149->Kind == kAdd) {
/* line 879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP148)))) goto yyL8;
  }
   return true;
yyL8:;

  }
  }
  if (Tree_IsType (yyP150, kArray)) {
  if (yyP149->Kind == kSize) {
/* line 880 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 880 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP150->Array.Expr)))) goto yyL9;
  }
   return true;
yyL9:;

  }
  }
  return false;
}

tValue EvalBiExpr
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP155, register tTree yyP154, tValue yyP153, register tTree yyP152, tValue yyP151)
# else
(yyP155, yyP154, yyP153, yyP152, yyP151)
 register tPosition yyP155;
 register tTree yyP154;
 tValue yyP153;
 register tTree yyP152;
 tValue yyP151;
# endif
{
/* line 885 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (Tree_IsType (yyP154, kInteger)) {
/* line 887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = EvalBiIntOp (yyP155, yyP154, yyP153 . Number, yyP152, yyP151 . Number);
  }
   return Res;

  }
  if (Tree_IsType (yyP154, kByte)) {
/* line 888 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 888 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = EvalBiIntOp (yyP155, yyP154, yyP153 . Number, yyP152, yyP151 . Number);
  }
   return Res;

  }
  if (Tree_IsType (yyP154, kBool)) {
/* line 889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = EvalBiBoolOp (yyP155, yyP154, yyP153 . Number, yyP152, yyP151 . Number);
  }
   return Res;

  }
  if (Tree_IsType (yyP154, kReal)) {
  if (Tree_IsType (yyP152, kMathsOp)) {
/* line 890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . n = EvalBiRealMathsOp (yyP155, yyP154, yyP153 . RNumber . n, yyP152, yyP151 . RNumber . n);
/* line 890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . p = MAX (yyP153 . RNumber . p, yyP151 . RNumber . p);
  }
   return Res;

  }
  if (Tree_IsType (yyP152, kRelOp)) {
/* line 891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = EvalBiRealRelOp (yyP155, yyP154, yyP153 . RNumber . n, yyP152, yyP151 . RNumber . n);
  }
   return Res;

  }
  }
 yyAbort ("EvalBiExpr");
}

static tInt EvalBiIntOp
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP160, register tTree yyP159, tInt yyP158, register tTree yyP157, tInt yyP156)
# else
(yyP160, yyP159, yyP158, yyP157, yyP156)
 register tPosition yyP160;
 register tTree yyP159;
 tInt yyP158;
 register tTree yyP157;
 tInt yyP156;
# endif
{
/* line 895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tInt res; 

  switch (yyP157->Kind) {
  case kAdd:
/* line 897 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return TestIntOVF (yyP160, yyP159, yyP158 + yyP156);

  case kSubtract:
/* line 898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return TestIntOVF (yyP160, yyP159, yyP158 - yyP156);

  case kMultiply:
/* line 899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return TestIntOVF (yyP160, yyP159, yyP158 * yyP156);

  case kDivide:
/* line 901 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 901 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP156 != 0))) goto yyL4;
  }
   return TestIntOVF (yyP160, yyP159, yyP158 / yyP156);
yyL4:;

/* line 902 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return TestIntOVF (yyP160, mUntyped (), 1);

  case kRem:
/* line 903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP156 != 0))) goto yyL6;
  }
   return TestIntOVF (yyP160, yyP159, yyP158 % yyP156);
yyL6:;

/* line 904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return TestIntOVF (yyP160, mUntyped (), 1);

  case kEqual:
/* line 906 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 == yyP156;

  case kNotEqual:
/* line 907 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 != yyP156;

  case kLess:
/* line 908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 < yyP156;

  case kGreater:
/* line 909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 > yyP156;

  case kLessEqual:
/* line 910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 <= yyP156;

  case kGreaterEqual:
/* line 911 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 >= yyP156;

  case kBitAnd:
/* line 913 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 & yyP156;

  case kBitOr:
/* line 914 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP158 | yyP156;

  case kBitEor:
/* line 915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 res = yyP158^yyP156;
  }
   return res;

  case kShiftOp:
  case kLeftShift:
  case kRightShift:
/* line 917 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 917 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP156 == 0))) goto yyL17;
  }
   return yyP158;
yyL17:;

/* line 918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP156 > EvalIntBits (yyP159)))) goto yyL18;
  {
/* line 918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Shift by invalid value for Integer size. Result is always zero", xxWarning, yyP160);
  }
  }
   return 0;
yyL18:;

/* line 919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP156 < 0))) goto yyL19;
  {
/* line 919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Shift by invalid (negative) value", xxError, yyP160);
  }
  }
   return 0;
yyL19:;

  break;
  }

  if (yyP157->Kind == kLeftShift) {
/* line 921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 res = SignExtendInt(yyP159,yyP158<<yyP156); 
  }
   return res;

  }
  if (yyP157->Kind == kRightShift) {
/* line 922 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 922 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 res = SignExtendInt(yyP159,(tInt)(((tUInt)(EvalIntMask(yyP159)&yyP158)) >> yyP156)); 
  }
   return res;

  }
  if (yyP157->Kind == kPlus) {
/* line 924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return SignExtendInt (yyP159, yyP158 + yyP156);

  }
  if (yyP157->Kind == kMinus) {
/* line 925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return SignExtendInt (yyP159, yyP158 - yyP156);

  }
  if (yyP157->Kind == kTimes) {
/* line 926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return SignExtendInt (yyP159, yyP158 * yyP156);

  }
  if (yyP157->Kind == kAfterOp) {
/* line 928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 res = ((tUInt)yyP158 > (tUInt)yyP156); 
  }
   return res;

  }
 yyAbort ("EvalBiIntOp");
}

static int EvalBiBoolOp
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP165, register tTree yyP164, tInt yyP163, register tTree yyP162, tInt yyP161)
# else
(yyP165, yyP164, yyP163, yyP162, yyP161)
 register tPosition yyP165;
 register tTree yyP164;
 tInt yyP163;
 register tTree yyP162;
 tInt yyP161;
# endif
{

  switch (yyP162->Kind) {
  case kEqual:
/* line 935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 == yyP161;

  case kNotEqual:
/* line 936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 != yyP161;

  case kLess:
/* line 937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 < yyP161;

  case kGreater:
/* line 938 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 > yyP161;

  case kLessEqual:
/* line 939 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 <= yyP161;

  case kGreaterEqual:
/* line 940 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 >= yyP161;

  case kAnd:
/* line 942 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 && yyP161;

  case kOr:
/* line 943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP163 || yyP161;

  }

 yyAbort ("EvalBiBoolOp");
}

static double EvalBiRealMathsOp
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP170, register tTree yyP169, register double yyP168, register tTree yyP167, register double yyP166)
# else
(yyP170, yyP169, yyP168, yyP167, yyP166)
 register tPosition yyP170;
 register tTree yyP169;
 register double yyP168;
 register tTree yyP167;
 register double yyP166;
# endif
{
/* line 948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 double Res = 0; 
  if (yyP167->Kind == kAdd) {
/* line 950 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP168 + yyP166;

  }
  if (yyP167->Kind == kSubtract) {
/* line 951 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP168 - yyP166;

  }
  if (yyP167->Kind == kMultiply) {
/* line 952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP168 * yyP166;

  }
  if (yyP167->Kind == kDivide) {
/* line 953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP166 != 0))) goto yyL4;
  }
   return yyP168 / yyP166;
yyL4:;

/* line 954 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 954 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 TestIntOVF(yyP170,mUntyped(),1); 
  }
   return Res;

  }
  if (yyP167->Kind == kRem) {
/* line 955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP166 != 0))) goto yyL6;
  {
/* line 955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res = yyP168-(yyP166*(int)(yyP168/yyP166)); 
  }
  }
   return Res;
yyL6:;

/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 TestIntOVF(yyP170,mUntyped(),1); 
  }
   return Res;

  }
 yyAbort ("EvalBiRealMathsOp");
}

static int EvalBiRealRelOp
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP175, register tTree yyP174, register double yyP173, register tTree yyP172, register double yyP171)
# else
(yyP175, yyP174, yyP173, yyP172, yyP171)
 register tPosition yyP175;
 register tTree yyP174;
 register double yyP173;
 register tTree yyP172;
 register double yyP171;
# endif
{
  if (yyP172->Kind == kEqual) {
/* line 962 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 == yyP171;

  }
  if (yyP172->Kind == kNotEqual) {
/* line 963 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 != yyP171;

  }
  if (yyP172->Kind == kLess) {
/* line 964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 < yyP171;

  }
  if (yyP172->Kind == kGreater) {
/* line 965 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 > yyP171;

  }
  if (yyP172->Kind == kLessEqual) {
/* line 966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 <= yyP171;

  }
  if (yyP172->Kind == kGreaterEqual) {
/* line 967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP173 >= yyP171;

  }
 yyAbort ("EvalBiRealRelOp");
}

tValue EvalUniExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP178, register tTree yyP177, register tTree yyP176)
# else
(yyP178, yyP177, yyP176)
 register tTree yyP178;
 register tTree yyP177;
 register tTree yyP176;
# endif
{
/* line 974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (Tree_IsType (yyP178, kInteger)) {
  if (yyP177->Kind == kBitNot) {
/* line 976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = ~ ValConstInt (yyP176);
  }
   return Res;

  }
  if (yyP177->Kind == kSubtract) {
/* line 977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = - ValConstInt (yyP176);
  }
   return Res;

  }
  if (yyP177->Kind == kMinus) {
/* line 978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = - ValConstInt (yyP176);
  }
   return Res;

  }
  if (yyP177->Kind == kPlus) {
/* line 979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = ValConstInt (yyP176);
  }
   return Res;

  }
  if (yyP177->Kind == kAdd) {
/* line 980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = ValConstInt (yyP176);
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP178, kBool)) {
  if (yyP177->Kind == kNot) {
/* line 982 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 982 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = ! (ValConstInt (yyP176));
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP178, kReal)) {
  if (yyP177->Kind == kSubtract) {
/* line 983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . n = - ValConstExpr (yyP176) . RNumber . n;
/* line 983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . p = ValConstExpr (yyP176) . RNumber . p;
  }
   return Res;

  }
  if (yyP177->Kind == kAdd) {
/* line 984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . n = - ValConstExpr (yyP176) . RNumber . n;
/* line 984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . p = ValConstExpr (yyP176) . RNumber . p;
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP178, kArray)) {
  if (yyP177->Kind == kSize) {
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = ValConstInt (yyP178->Array.Expr);
  }
   return Res;

  }
  }
 yyAbort ("EvalUniExpr");
}

bool CheckConstConvert
# if defined __STDC__ | defined __cplusplus
(register tTree yyP179)
# else
(yyP179)
 register tTree yyP179;
# endif
{
  if (Tree_IsType (yyP179, kInteger)) {
/* line 992 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP179, kBool)) {
/* line 993 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP179, kByte)) {
/* line 994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

tValue EvalConstConvert
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP182, register tTree yyP181, register tTree yyP180)
# else
(yyP182, yyP181, yyP180)
 register tPosition yyP182;
 register tTree yyP181;
 register tTree yyP180;
# endif
{
/* line 998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = TestIntOVF (yyP182, yyP181, ValConstInt (yyP180));
  }
   return Res;

}

tValue CheckConst
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP186, register tTree yyP185, tInt yyP184, register bool yyP183)
# else
(yyP186, yyP185, yyP184, yyP183)
 register tPosition yyP186;
 register tTree yyP185;
 tInt yyP184;
 register bool yyP183;
# endif
{
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (Tree_IsType (yyP185, kUntyped)) {
/* line 1005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = yyP184; 
  }
   return Res;

  }
/* line 1006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP183 == false))) goto yyL2;
  {
/* line 1006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = TestIntOVF(yyP186,yyP185,yyP184); 
  }
  }
   return Res;
yyL2:;

/* line 1007 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1007 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = SignExtendInt(yyP185,yyP184); 
  }
   return Res;

}

bool CheckConstRoundTrunc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP188, register tTree yyP187)
# else
(yyP188, yyP187)
 register tTree yyP188;
 register tTree yyP187;
# endif
{
  if (Tree_IsType (yyP188, kReal)) {
/* line 1013 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (Tree_IsType (yyP187, kReal)) {
/* line 1014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

tValue EvalConstTrunc
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP192, register tTree yyP191, register tTree yyP190, tValue yyP189)
# else
(yyP192, yyP191, yyP190, yyP189)
 register tPosition yyP192;
 register tTree yyP191;
 register tTree yyP190;
 tValue yyP189;
# endif
{
/* line 1033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (Tree_IsType (yyP191, kReal)) {
  if (Tree_IsType (yyP190, kReal)) {
/* line 1035 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1035 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = floor(yyP189.RNumber.n); Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n)); 
  }
   return Res;

  }
  if (Tree_IsType (yyP190, kInteger)) {
/* line 1038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = (double)yyP189.Number; Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n)); 
  }
   return Res;

  }
  if (Tree_IsType (yyP190, kByte)) {
/* line 1039 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1039 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = (double)yyP189.Number; Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n)); 
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP191, kInteger)) {
  if (Tree_IsType (yyP190, kReal)) {
/* line 1036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = TestIntOVF(yyP192,yyP191,(tInt)yyP189.RNumber.n); 
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP191, kByte)) {
  if (Tree_IsType (yyP190, kReal)) {
/* line 1037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = TestIntOVF(yyP192,yyP191,(tInt)yyP189.RNumber.n); 
  }
   return Res;

  }
  }
 yyAbort ("EvalConstTrunc");
}

tValue EvalConstRound
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP196, register tTree yyP195, register tTree yyP194, tValue yyP193)
# else
(yyP196, yyP195, yyP194, yyP193)
 register tPosition yyP196;
 register tTree yyP195;
 register tTree yyP194;
 tValue yyP193;
# endif
{
/* line 1059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (Tree_IsType (yyP195, kReal)) {
  if (Tree_IsType (yyP194, kReal)) {
/* line 1061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = floor(yyP193.RNumber.n + 0.5); Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n));; 
  }
   return Res;

  }
  if (Tree_IsType (yyP194, kInteger)) {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = (double)yyP193.Number; Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n));; 
  }
   return Res;

  }
  if (Tree_IsType (yyP194, kByte)) {
/* line 1065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.RNumber.n = (double)yyP193.Number; Res.RNumber.p = 1.0+log10(fabs(Res.RNumber.n));; 
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP195, kInteger)) {
  if (Tree_IsType (yyP194, kReal)) {
/* line 1062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = TestIntOVF(yyP196,yyP195,(tInt)(yyP193.RNumber.n + 0.5)); 
  }
   return Res;

  }
  }
  if (Tree_IsType (yyP195, kByte)) {
  if (Tree_IsType (yyP194, kReal)) {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 Res.Number = TestIntOVF(yyP196,yyP195,(tInt)(yyP193.RNumber.n + 0.5)); 
  }
   return Res;

  }
  }
 yyAbort ("EvalConstRound");
}

bool CheckConstId
# if defined __STDC__ | defined __cplusplus
(register tTree yyP197)
# else
(yyP197)
 register tTree yyP197;
# endif
{
  if (yyP197 == NULL) {
/* line 1072 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1072 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }

  }
  if (Tree_IsType (yyP197, kAVal)) {
/* line 1073 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1073 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP197->AVal.Expr)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (yyP197->Kind == kATagPro) {
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP197->Kind == kAField) {
/* line 1075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  return false;
}

tValue EvalConstId
# if defined __STDC__ | defined __cplusplus
(register tTree yyP198)
# else
(yyP198)
 register tTree yyP198;
# endif
{
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; 
  if (yyP198->Kind == kVal) {
/* line 1080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return ValConstExpr (yyP198->Val.Expr);

  }
  if (Tree_IsType (yyP198, kValRetypes)) {
/* line 1082 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1082 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (yyP198->ValRetypes.Type)))) goto yyL2;
  }
   return ValConstExpr (yyP198->ValRetypes.Expr);
yyL2:;

/* line 1084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (ExprType (yyP198->ValRetypes.Expr))))) goto yyL3;
  {
/* line 1085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 
    unsigned char tmp[8];
    tConstArray D = (tConstArray)tmp;
    int SS = CalcTypeSize(ArrayPrimType(ExprType(yyP198->ValRetypes.Expr)));
    int DS = CalcTypeSize(yyP198->ValRetypes.Type);

    RetypeConst(SS,GetConstArray(yyP198->ValRetypes.Expr),DS,D);
    Res = ReadConstArray(yyP198->ValRetypes.Type,D);
  
  }
  }
   return Res;
yyL3:;

/* line 1095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 
    unsigned char tmp1[8],tmp2[8];
    tConstArray D = (tConstArray)tmp1;
    tConstArray S = (tConstArray)tmp2;
    int SS = CalcTypeSize(ArrayPrimType(ExprType(yyP198->ValRetypes.Expr)));
    int DS = CalcTypeSize(yyP198->ValRetypes.Type);

    WriteConstArray(ExprType(yyP198->ValRetypes.Expr),ValConstExpr(yyP198->ValRetypes.Expr),S);
    RetypeConst(SS,S,DS,D);
    Res = ReadConstArray(yyP198->ValRetypes.Type,D);
  
  }
   return Res;

  }
  if (yyP198->Kind == kATagPro) {
/* line 1109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = CountTags (yyP198->ATagPro.Sym);
  }
   return Res;

  }
  if (yyP198->Kind == kAField) {
/* line 1113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . Number = CountFields (yyP198->AField.Sym);
  }
   return Res;

  }
 yyAbort ("EvalConstId");
}

tInt TestIntOVF
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP201, register tTree yyP200, tInt yyP199)
# else
(yyP201, yyP200, yyP199)
 register tPosition yyP201;
 register tTree yyP200;
 tInt yyP199;
# endif
{
  if (Tree_IsType (yyP200, kInt64)) {
/* line 1121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP199;

  }
  if (Tree_IsType (yyP200, kInt)) {
/* line 1122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP199 >= MOSTNEGINT && yyP199 <= MOSTPOSINT))) goto yyL2;
  }
   return yyP199;
yyL2:;

  }
  if (Tree_IsType (yyP200, kInt32)) {
/* line 1123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP199 >= MOSTNEGINT32 && yyP199 <= MOSTPOSINT32))) goto yyL3;
  }
   return yyP199;
yyL3:;

  }
  if (Tree_IsType (yyP200, kInt16)) {
/* line 1124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP199 >= MOSTNEGINT16 && yyP199 <= MOSTPOSINT16))) goto yyL4;
  }
   return yyP199;
yyL4:;

  }
  if (Tree_IsType (yyP200, kByte)) {
/* line 1125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP199 & (~ BYTEMASK)) == 0)) goto yyL5;
  }
   return yyP199;
yyL5:;

  }
  if (Tree_IsType (yyP200, kBool)) {
/* line 1126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP199 & (~ BOOLMASK)) == 0)) goto yyL6;
  }
   return yyP199;
yyL6:;

  }
/* line 1128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP199 = 0;
/* line 1128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Overflow when evaluating constant expression", xxError, yyP201);
  }
   return yyP199;

}

tInt SignExtendInt
# if defined __STDC__ | defined __cplusplus
(register tTree yyP203, tInt yyP202)
# else
(yyP203, yyP202)
 register tTree yyP203;
 tInt yyP202;
# endif
{
  if (Tree_IsType (yyP203, kInt64)) {
/* line 1133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP202;

  }
  if (Tree_IsType (yyP203, kInt)) {
/* line 1134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xx16bitINT)))) goto yyL2;
  {
/* line 1134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 yyP202 = (tInt)((short)yyP202); 
  }
  }
   return yyP202;
yyL2:;

/* line 1135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 yyP202 = ((int)yyP202); 
  }
   return yyP202;

  }
  if (Tree_IsType (yyP203, kInt32)) {
/* line 1136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 yyP202 = ((int)yyP202); 
  }
   return yyP202;

  }
  if (Tree_IsType (yyP203, kInt16)) {
/* line 1137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 yyP202 = ((short)yyP202); 
  }
   return yyP202;

  }
  if (Tree_IsType (yyP203, kByte)) {
/* line 1138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP202;

  }
  if (Tree_IsType (yyP203, kBool)) {
/* line 1139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP202;

  }
/* line 1141 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP202;

}

static int CountTags
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP204)
# else
(yyP204)
 tSymbol yyP204;
# endif
{
/* line 1146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! Tree_IsType (SymbolKind (yyP204 - 1), kATagPro)))) goto yyL1;
  }
   return 0;
yyL1:;

/* line 1147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 1 + CountTags (yyP204 - 1);

}

static int CountFields
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP205)
# else
(yyP205)
 tSymbol yyP205;
# endif
{
/* line 1151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! Tree_IsType (SymbolKind (yyP205 - 1), kAField)))) goto yyL1;
  }
   return 0;
yyL1:;

/* line 1152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 1 + CountFields (yyP205 - 1);

}

static tInt EvalIntBits
# if defined __STDC__ | defined __cplusplus
(register tTree yyP206)
# else
(yyP206)
 register tTree yyP206;
# endif
{
  if (Tree_IsType (yyP206, kInt)) {
/* line 1157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 32;

  }
  if (Tree_IsType (yyP206, kByte)) {
/* line 1158 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 8;

  }
  if (Tree_IsType (yyP206, kInt16)) {
/* line 1159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 16;

  }
  if (Tree_IsType (yyP206, kInt32)) {
/* line 1160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 32;

  }
  if (Tree_IsType (yyP206, kInt64)) {
/* line 1161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 64;

  }
 yyAbort ("EvalIntBits");
}

tInt EvalMostPos
# if defined __STDC__ | defined __cplusplus
(register tTree yyP207)
# else
(yyP207)
 register tTree yyP207;
# endif
{
  if (Tree_IsType (yyP207, kByte)) {
/* line 1165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTPOSBYTE;

  }
  if (Tree_IsType (yyP207, kInt)) {
/* line 1166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTPOSINT;

  }
  if (Tree_IsType (yyP207, kInt16)) {
/* line 1167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTPOSINT16;

  }
  if (Tree_IsType (yyP207, kInt32)) {
/* line 1168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTPOSINT32;

  }
  if (Tree_IsType (yyP207, kInt64)) {
/* line 1169 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTPOSINT64;

  }
 yyAbort ("EvalMostPos");
}

tInt EvalMostNeg
# if defined __STDC__ | defined __cplusplus
(register tTree yyP208)
# else
(yyP208)
 register tTree yyP208;
# endif
{
  if (Tree_IsType (yyP208, kByte)) {
/* line 1173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTNEGBYTE;

  }
  if (Tree_IsType (yyP208, kInt)) {
/* line 1174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTNEGINT;

  }
  if (Tree_IsType (yyP208, kInt16)) {
/* line 1175 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTNEGINT16;

  }
  if (Tree_IsType (yyP208, kInt32)) {
/* line 1176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTNEGINT32;

  }
  if (Tree_IsType (yyP208, kInt64)) {
/* line 1177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return MOSTNEGINT64;

  }
 yyAbort ("EvalMostNeg");
}

static tInt EvalIntMask
# if defined __STDC__ | defined __cplusplus
(register tTree yyP209)
# else
(yyP209)
 register tTree yyP209;
# endif
{
  if (Tree_IsType (yyP209, kByte)) {
/* line 1181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return BYTEMASK;

  }
  if (Tree_IsType (yyP209, kInt)) {
/* line 1182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return INTMASK;

  }
  if (Tree_IsType (yyP209, kInt16)) {
/* line 1183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return INT16MASK;

  }
  if (Tree_IsType (yyP209, kInt32)) {
/* line 1184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return INT32MASK;

  }
  if (Tree_IsType (yyP209, kInt64)) {
/* line 1185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return INT64MASK;

  }
 yyAbort ("EvalIntMask");
}

tValue EvalConstSubscript
# if defined __STDC__ | defined __cplusplus
(register tTree yyP212, register int yyP211, tConstArray yyP210)
# else
(yyP212, yyP211, yyP210)
 register tTree yyP212;
 register int yyP211;
 tConstArray yyP210;
# endif
{
/* line 1190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue Res; tTree E; 
  if (Tree_IsType (yyP212, kReal32)) {
/* line 1192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . n =  ((float *)yyP210)[yyP211] ;
/* line 1192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . p = 0;
  }
   return Res;

  }
  if (Tree_IsType (yyP212, kReal64)) {
/* line 1193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . n =  ((double *)yyP210)[yyP211] ;
/* line 1193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Res . RNumber . p = 0;
  }
   return Res;

  }
  if (Tree_IsType (yyP212, kPrimitive)) {
/* line 1194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      switch(CalcTypeSize(yyP212))
      {
	case 1:  Res.Number = ((unsigned char *)yyP210)[yyP211]; break;
	case 2:  Res.Number = ((short *)yyP210)[yyP211]; break;
	case 4:  Res.Number = ((int *)yyP210)[yyP211]; break;
	case 8:  Res.Number = ((tInt *)yyP210)[yyP211]; break;
      }
    
  }
   return Res;

  }
 yyAbort ("EvalConstSubscript");
}

tConstArray ConstArrayFromString
# if defined __STDC__ | defined __cplusplus
(register tTree yyP214, register tStringRef yyP213)
# else
(yyP214, yyP213)
 register tTree yyP214;
 register tStringRef yyP213;
# endif
{
/* line 1209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tConstArray CA; 
/* line 1211 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1211 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   CA = NewConstArray (LengthSt (yyP213));
/* line 1212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 StGetString(yyP213,CA); 
  }
   return CA;

}

tConstArray ConstArrayFromExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP216, register tTree yyP215)
# else
(yyP216, yyP215)
 register tTree yyP216;
 register tTree yyP215;
# endif
{
/* line 1216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tConstArray CA; 
  if (Tree_IsType (yyP216, kArray)) {
/* line 1217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   CA = NewConstArray (CalcTypeSize (yyP216));
/* line 1217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   FillConstArray (yyP216->Array.Type, yyP215, CA);
  }
   return CA;

  }
  if (Tree_IsType (yyP216, kUserDataTypes)) {
/* line 1218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   CA = NewConstArray (CalcTypeSize (yyP216));
/* line 1218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   FillConstFields (yyP216, yyP216->UserDataTypes.FieldList, yyP215, CA);
  }
   return CA;

  }
 yyAbort ("ConstArrayFromExprList");
}

tConstArray ConstArrayFromDecl
# if defined __STDC__ | defined __cplusplus
(register tTree yyP218, register tTree yyP217)
# else
(yyP218, yyP217)
 register tTree yyP218;
 register tTree yyP217;
# endif
{
/* line 1223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tConstArray CA; 
  if (yyP217->Kind == kAField) {
/* line 1224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NoConstArray;

  }
  if (yyP217->Kind == kVal) {
  if (yyP217->Val.Expr->Kind == kElementalExpr) {
/* line 1225 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1225 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsScalarType (ExprType (yyP217->Val.Expr))))) goto yyL2;
  }
   return  yyP217->Val.Expr->ElementalExpr.Elemental->Elemental.CA ;
yyL2:;

  }
  if (Tree_IsType (yyP217->Val.Expr, kStringElement)) {
/* line 1226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (ExprType (yyP217->Val.Expr))))) goto yyL3;
  }
   return  yyP217->Val.Expr->StringElement.CA ;
yyL3:;

  }
  if (yyP217->Val.Expr->Kind == kValof) {
/* line 1227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsScalarType (ExprType (yyP217->Val.Expr))))) goto yyL4;
  }
   return ConstArrayFromDecl (yyP218, yyP217->Val.Expr);
yyL4:;

  }
  }
  if (yyP217->Kind == kValof) {
  if (yyP217->Valof.ExprList->Kind == kAnExpr) {
  if (yyP217->Valof.ExprList->AnExpr.Expr->Kind == kValof) {
/* line 1228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsScalarType (ExprType (yyP217->Valof.ExprList->AnExpr.Expr))))) goto yyL5;
  }
   return ConstArrayFromDecl (yyP218, yyP217->Valof.ExprList->AnExpr.Expr);
yyL5:;

  }
  if (yyP217->Valof.ExprList->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsScalarType (ExprType (yyP217->Valof.ExprList->AnExpr.Expr))))) goto yyL6;
  }
   return  yyP217->Valof.ExprList->AnExpr.Expr->ElementalExpr.Elemental->Elemental.CA ;
yyL6:;

  }
  if (Tree_IsType (yyP217->Valof.ExprList->AnExpr.Expr, kStringElement)) {
/* line 1230 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1230 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsScalarType (ExprType (yyP217->Valof.ExprList->AnExpr.Expr))))) goto yyL7;
  }
   return  yyP217->Valof.ExprList->AnExpr.Expr->StringElement.CA ;
yyL7:;

  }
  }
  }
  if (Tree_IsType (yyP218, kArray)) {
  if (Tree_IsType (yyP217, kValRetypes)) {
  if (yyP217->ValRetypes.Expr->Kind == kElementalExpr) {
/* line 1232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (ExprType (yyP217->ValRetypes.Expr))))) goto yyL8;
  {
/* line 1233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      { tConstArray S,D;
	int SS = CalcTypeSize(ArrayPrimType(ExprType(yyP217->ValRetypes.Expr)));
	int DS = CalcTypeSize(ArrayPrimType(yyP218));
	int sz = CalcTypeSize(yyP218);
	int psz = CalcTypeSize(ArrayPrimType(yyP218));
	int count = sz/psz;

	CA = NewConstArray(sz);
	S = GetConstArray(yyP217->ValRetypes.Expr);
	D = CA;

	while(count-- > 0)
	{
	  RetypeConst(SS,S,DS,D);
	  D += DS; S += DS;
	}
      }
    
  }
  }
   return CA;
yyL8:;

  }
/* line 1254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      { tConstArray S,D;
	int SS = CalcTypeSize(ExprType(yyP217->ValRetypes.Expr));
	int DS = CalcTypeSize(ArrayPrimType(yyP218));

	CA = NewConstArray(CalcTypeSize(yyP218));
	S = CA; D = CA;
	WriteConstArray(ExprType(yyP217->ValRetypes.Expr),ValConstExpr(yyP217->ValRetypes.Expr),CA);
	RetypeConst(SS,S,DS,D);
      }
    
  }
   return CA;

  }
  }
  if (Tree_IsType (yyP217, kSpec)) {
/* line 1268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Implementation restriction: Construct Not Supported", xxError, yyP217->Spec.Pos);
  }
   return NoConstArray;

  }
/* line 1269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyWrite ("ConstArrayFromDecl-");
/* line 1269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   WriteTreeNode (yyf, yyP217);
  }
   return NoConstArray;

}

static void FillConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP221, register tTree yyP220, tConstArray yyP219)
# else
(yyP221, yyP220, yyP219)
 register tTree yyP221;
 register tTree yyP220;
 tConstArray yyP219;
# endif
{
  if (Tree_IsType (yyP221, kUserDataTypes)) {
  if (yyP220->Kind == kAnExpr) {
  if (yyP220->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      int sz=CalcTypeSize(yyP221);
      memcpy(yyP219,yyP220->AnExpr.Expr->ElementalExpr.Elemental->Elemental.CA,sz);
      FillConstArray(yyP221,yyP220->AnExpr.Next,yyP219+sz);
    
  }
   return;

  }
  }
  }
  if (Tree_IsType (yyP221, kArray)) {
  if (yyP220->Kind == kAnExpr) {
  if (yyP220->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      int sz=CalcTypeSize(yyP221);
      memcpy(yyP219,yyP220->AnExpr.Expr->ElementalExpr.Elemental->Elemental.CA,sz);
      FillConstArray(yyP221,yyP220->AnExpr.Next,yyP219+sz);
    
  }
   return;

  }
  if (Tree_IsType (yyP220->AnExpr.Expr, kStringElement)) {
/* line 1288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      int sz=CalcTypeSize(yyP221);
      memcpy(yyP219,yyP220->AnExpr.Expr->StringElement.CA,sz);
      FillConstArray(yyP221,yyP220->AnExpr.Next,yyP219+sz);
    
  }
   return;

  }
  }
  }
  if (Tree_IsType (yyP221, kPrimitive)) {
  if (yyP220->Kind == kAnExpr) {
/* line 1296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      WriteConstArray(yyP221,ValConstExpr(yyP220->AnExpr.Expr),yyP219);
      FillConstArray(yyP221,yyP220->AnExpr.Next,yyP219+CalcTypeSize(yyP221));
    
  }
   return;

  }
  }
;
}

static void FillConstFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP225, register tTree yyP224, register tTree yyP223, tConstArray yyP222)
# else
(yyP225, yyP224, yyP223, yyP222)
 register tTree yyP225;
 register tTree yyP224;
 register tTree yyP223;
 tConstArray yyP222;
# endif
{
  if (yyP224->Kind == kAField) {
  if (Tree_IsType (yyP224->AField.Type, kUserDataTypes)) {
  if (yyP223->Kind == kAnExpr) {
  if (yyP223->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      memcpy(yyP222+CalcOffset(yyP225,yyP224),yyP223->AnExpr.Expr->ElementalExpr.Elemental->Elemental.CA,CalcTypeSize(yyP224->AField.Type));
      FillConstFields(yyP225,yyP224->AField.Next,yyP223->AnExpr.Next,yyP222);
    
  }
   return;

  }
  }
  }
  if (Tree_IsType (yyP224->AField.Type, kArray)) {
  if (yyP223->Kind == kAnExpr) {
  if (yyP223->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      memcpy(yyP222+CalcOffset(yyP225,yyP224),yyP223->AnExpr.Expr->ElementalExpr.Elemental->Elemental.CA,CalcTypeSize(yyP224->AField.Type));
      FillConstFields(yyP225,yyP224->AField.Next,yyP223->AnExpr.Next,yyP222);
    
  }
   return;

  }
  if (Tree_IsType (yyP223->AnExpr.Expr, kStringElement)) {
/* line 1317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      memcpy(yyP222+CalcOffset(yyP225,yyP224),yyP223->AnExpr.Expr->StringElement.CA,CalcTypeSize(yyP224->AField.Type));
      FillConstFields(yyP225,yyP224->AField.Next,yyP223->AnExpr.Next,yyP222);
    
  }
   return;

  }
  }
  }
  if (Tree_IsType (yyP224->AField.Type, kPrimitive)) {
  if (yyP223->Kind == kAnExpr) {
/* line 1324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1325 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

      WriteConstArray(yyP224->AField.Type,ValConstExpr(yyP223->AnExpr.Expr),yyP222+CalcOffset(yyP225,yyP224));
      FillConstFields(yyP225,yyP224->AField.Next,yyP223->AnExpr.Next,yyP222);
    
  }
   return;

  }
  }
  }
;
}

tConstArray RangeConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP228, tConstArray yyP227, register int yyP226)
# else
(yyP228, yyP227, yyP226)
 register tTree yyP228;
 tConstArray yyP227;
 register int yyP226;
# endif
{
  if (Tree_IsType (yyP228, kArray)) {
/* line 1335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP227 = yyP227 +  (yyP226*CalcTypeSize(yyP228->Array.Type));
/* line 1336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */


    
  }
   return yyP227;

  }
 yyAbort ("RangeConstArray");
}

tConstArray SubscriptConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP232, tConstArray yyP231, register tTree yyP230, register tTree yyP229)
# else
(yyP232, yyP231, yyP230, yyP229)
 register tTree yyP232;
 tConstArray yyP231;
 register tTree yyP230;
 register tTree yyP229;
# endif
{
  if (Tree_IsType (yyP229, kUserDataTypes)) {
/* line 1344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP231 = yyP231 + CalcOffset (yyP229, ExprFieldList (yyP230));
  }
   return yyP231;

  }
/* line 1347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP231 = yyP231 + (ValConstInt (yyP230) * CalcTypeSize (yyP232));
/* line 1349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

     
    
  }
   return yyP231;

}

static tConstArray GetConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree EX)
# else
(EX)
 register tTree EX;
# endif
{
  if (EX->Kind == kElementalExpr) {
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (ExprType (EX))))) goto yyL1;
  }
   return EX->ElementalExpr.Elemental -> Elemental . CA;
yyL1:;

  }
  if (Tree_IsType (EX, kStringElement)) {
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsArrayType (ExprType (EX))))) goto yyL2;
  }
   return EX -> StringElement . CA;
yyL2:;

  }
/* line 1359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Error in ExprConstArray()", xxFatal, EX->Expr.Pos);
  }
   return (tConstArray)NULL;

}

tValue ReadConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP234, tConstArray yyP233)
# else
(yyP234, yyP233)
 register tTree yyP234;
 tConstArray yyP233;
# endif
{
/* line 1363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tValue V; 

  switch (yyP234->Kind) {
  case kByte:
  case kUserByte:
/* line 1365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
V.Number = *((unsigned char *)yyP233); 
  }
   return V;

  case kBool:
  case kUserBool:
/* line 1366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
V.Number = *((unsigned char *)yyP233); 
  }
   return V;

  case kInt:
  case kUserInt:
/* line 1368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xx16bitINT)))) goto yyL3;
  {
/* line 1369 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
short v; memcpy(&v,yyP233,2); V.Number = v; 
  }
  }
   return V;
yyL3:;

/* line 1373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
int v; memcpy(&v,yyP233,4); V.Number = v; 
  }
   return V;

  case kInt16:
  case kUserInt16:
/* line 1370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
short v; memcpy(&v,yyP233,2); V.Number = v; 
  }
   return V;

  case kInt32:
  case kUserInt32:
/* line 1375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
int v; memcpy(&v,yyP233,4); V.Number = v; 
  }
   return V;

  case kReal32:
  case kUserReal32:
/* line 1377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
float v; memcpy(&v,yyP233,4); V.RNumber.n = v; V.RNumber.p = 7; 
  }
   return V;

  case kInt64:
  case kUserInt64:
/* line 1380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1381 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
tInt v; memcpy(&v,yyP233,8); V.Number = v; 
  }
   return V;

  case kReal64:
  case kUserReal64:
/* line 1382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1383 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
double v; memcpy(&v,yyP233,8); V.RNumber.n = v; V.RNumber.p = 14; 
  }
   return V;

  }

 yyAbort ("ReadConstArray");
}

void WriteConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP237, tValue yyP236, tConstArray yyP235)
# else
(yyP237, yyP236, yyP235)
 register tTree yyP237;
 tValue yyP236;
 tConstArray yyP235;
# endif
{

  switch (yyP237->Kind) {
  case kByte:
  case kUserByte:
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
*((char *)yyP235) = yyP236.Number; 
  }
   return;

  case kBool:
  case kUserBool:
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
*((char *)yyP235) = yyP236.Number; 
  }
   return;

  case kInt:
  case kUserInt:
/* line 1391 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1391 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IS_Mode (xx16bitINT)))) goto yyL3;
  {
/* line 1392 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
short v = yyP236.Number; memcpy(yyP235,&v,2); 
  }
  }
   return;
yyL3:;

/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
int v = yyP236.Number; memcpy(yyP235,&v,4); 
  }
   return;

  case kInt16:
  case kUserInt16:
/* line 1393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
short v = yyP236.Number; memcpy(yyP235,&v,2); 
  }
   return;

  case kInt32:
  case kUserInt32:
/* line 1398 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
int v = yyP236.Number; memcpy(yyP235,&v,4); 
  }
   return;

  case kReal32:
  case kUserReal32:
/* line 1400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
float v = yyP236.RNumber.n; memcpy(yyP235,&v,4); 
  }
   return;

  case kInt64:
  case kUserInt64:
/* line 1403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
tInt v = yyP236.Number; memcpy(yyP235,&v,8); 
  }
   return;

  case kReal64:
  case kUserReal64:
/* line 1405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
double v = yyP236.RNumber.n; memcpy(yyP235,&v,8); 
  }
   return;

  }

;
}

static tConstArray NewConstArray
# if defined __STDC__ | defined __cplusplus
(register int yyP238)
# else
(yyP238)
 register int yyP238;
# endif
{
/* line 1410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tConstArray CA; 
/* line 1412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

    if (!(CA = (tConstArray)malloc((yyP238==0)?1:yyP238))) Message("Out of heap allocating array",xxError,NoPosition);
  
  }
   return CA;

}

static void RangeCheckSubscript
# if defined __STDC__ | defined __cplusplus
(register tTree yyP240, register tTree yyP239)
# else
(yyP240, yyP239)
 register tTree yyP240;
 register tTree yyP239;
# endif
{
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP240)))) goto yyL1;
  {
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP239)))) goto yyL1;
  {
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP240) >= 0))) goto yyL1;
  {
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP240) < ValConstInt (yyP239)))) goto yyL1;
  }
  }
  }
  }
   return;
yyL1:;

/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP240)))) goto yyL2;
  {
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP239)))) goto yyL2;
  {
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Subscript out of range", xxError, yyP240->Expr.Pos);
  }
  }
  }
   return;
yyL2:;

/* line 1423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP240)))) goto yyL3;
  {
/* line 1423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP240) >= 0))) goto yyL3;
  }
  }
   return;
yyL3:;

/* line 1424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP240)))) goto yyL4;
  {
/* line 1424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Subscript out of range", xxError, yyP240->Expr.Pos);
  }
  }
   return;
yyL4:;

/* line 1425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

;
}

static void RangeCheckSlice
# if defined __STDC__ | defined __cplusplus
(register tTree yyP243, register tTree yyP242, register tTree yyP241)
# else
(yyP243, yyP242, yyP241)
 register tTree yyP243;
 register tTree yyP242;
 register tTree yyP241;
# endif
{
/* line 1429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP243)))) goto yyL1;
  {
/* line 1429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP242)))) goto yyL1;
  {
/* line 1429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP241)))) goto yyL1;
  {
/* line 1430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP243) >= 0))) goto yyL1;
  {
/* line 1430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((ValConstInt (yyP242) >= 0))) goto yyL1;
  {
/* line 1430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (((ValConstInt (yyP243) + ValConstInt (yyP242)) <= ValConstInt (yyP241)))) goto yyL1;
  }
  }
  }
  }
  }
  }
   return;
yyL1:;

/* line 1431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP243)))) goto yyL2;
  {
/* line 1431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP242)))) goto yyL2;
  {
/* line 1431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP241)))) goto yyL2;
  {
/* line 1432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Array slice exceeds array bounds", xxError, yyP243->Expr.Pos);
  }
  }
  }
  }
   return;
yyL2:;

/* line 1433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   RangeCheckSubscript (yyP243, yyP241);
  }
   return;

;
}

tTree MakeConstInt
# if defined __STDC__ | defined __cplusplus
(register int yyP245, register tPosition yyP244)
# else
(yyP245, yyP244)
 register int yyP245;
 register tPosition yyP244;
# endif
{
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree T; tValue TMP; 
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   TMP . Number = yyP245;
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   T = mConst (yyP244, TMP, mInt ());
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ExprType (T) = mInt ();
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ConstFlag (T -> Expr) = true;
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ValConstInt (T) = yyP245;
  }
   return T;

}

static tTree MakeAddExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP247, register tTree yyP246)
# else
(yyP247, yyP246)
 register tTree yyP247;
 register tTree yyP246;
# endif
{
/* line 1442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree T; 
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP247)))) goto yyL1;
  {
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstInt (yyP246)))) goto yyL1;
  {
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   T = mBinary (yyP247->Expr.Pos, yyP247, yyP246, mAdd ());
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ExprType (T) = ExprType (yyP247);
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ConstFlag (T -> Expr) = true;
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ValConstInt (T) = ValConstInt (yyP247) + ValConstInt (yyP246);
  }
  }
  }
   return T;
yyL1:;

/* line 1444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   T = mBinary (yyP247->Expr.Pos, yyP247, yyP246, mAdd ());
/* line 1444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ExprType (T) = ExprType (yyP247);
/* line 1444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   ConstFlag (T -> Expr) = false;
  }
   return T;

}

static int CountList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP249, register tTree yyP248)
# else
(yyP249, yyP248)
 register tTree yyP249;
 register tTree yyP248;
# endif
{
  if (yyP249->Kind == kAnExpr) {
/* line 1449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SameType (yyP248, ExprType (yyP249->AnExpr.Expr)))) goto yyL1;
  }
   return CountList (yyP249->AnExpr.Next, yyP248) + 1;
yyL1:;

/* line 1451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Type mismatch in expression list", xxError, yyP249->AnExpr.Expr->Expr.Pos);
  }
   return CountList (yyP249->AnExpr.Next, yyP248) + 1;

  }
  if (yyP249->Kind == kNoExpr) {
/* line 1450 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 1;

  }
 yyAbort ("CountList");
}

int CountExprs
# if defined __STDC__ | defined __cplusplus
(register tTree yyP250)
# else
(yyP250)
 register tTree yyP250;
# endif
{
  if (yyP250->Kind == kAnExpr) {
/* line 1455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CountExprs (yyP250->AnExpr.Next) + 1;

  }
  if (yyP250->Kind == kNoExpr) {
/* line 1456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 0;

  }
 yyAbort ("CountExprs");
}

static int SizeType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP253, register tPosition yyP252, register bool yyP251, register tTree * yyP254)
# else
(yyP253, yyP252, yyP251, yyP254)
 register tTree yyP253;
 register tPosition yyP252;
 register bool yyP251;
 register tTree * yyP254;
# endif
{
/* line 1460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int V; 

  switch (yyP253->Kind) {
  case kArray:
  case kUserArray:
  if (yyP253->Array.Expr->Kind == kUnBound) {
 {
  tTree yyV1;
  if (equalbool (yyP251, true)) {
/* line 1461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   V = SizeType (yyP253->Array.Type, yyP252, true, & yyV1);
/* line 1463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 if (yyV1!=NoTree) Message("Too many unspecified dimensions to retype",xxError,yyP252); 
  }
   * yyP254 = yyP253;
  {
   return V;
  }

  }
 }
  }
/* line 1465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tTree yyV1;
  {
/* line 1465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! IsConstInt (yyP253->Array.Expr)))) goto yyL2;
  {
/* line 1465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (SizeType (yyP253->Array.Type, yyP252, yyP251, & yyV1))) goto yyL2;
  }
  }
   * yyP254 = yyV1;
  {
   return - 1;
  }
 }
yyL2:;

/* line 1466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tTree yyV1;
  {
/* line 1466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   V = SizeType (yyP253->Array.Type, yyP252, yyP251, & yyV1);
  }
   * yyP254 = yyV1;
  {
   return (V<0)?V:V*ValConstInt(yyP253->Array.Expr);
  }
 }

  case kInt:
  case kUserInt:
/* line 1467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return IS_Mode(xx16bitINT) ? 2 : 4 ;

  case kInt16:
  case kUserInt16:
/* line 1468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 2;

  case kInt32:
  case kUserInt32:
/* line 1469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 4;

  case kInt64:
  case kUserInt64:
/* line 1470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 8;

  case kReal32:
  case kUserReal32:
/* line 1471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 4;

  case kReal64:
  case kUserReal64:
/* line 1472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 8;

  case kBool:
  case kUserBool:
/* line 1473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 1;

  case kByte:
  case kUserByte:
/* line 1474 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return 1;

  case kChan:
/* line 1475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return sizeof(void*);

  case kUserRecord:
/* line 1476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP254 = NULL;
   return CalcRecordSize (yyP253->UserRecord.FieldList, 0);

  }

/* line 1477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Cannot perform retype involving this base type", xxError, yyP252);
  }
   * yyP254 = NULL;
   return - 1;

}

static int AlignToNext
# if defined __STDC__ | defined __cplusplus
(register int yyP256, register int yyP255)
# else
(yyP256, yyP255)
 register int yyP256;
 register int yyP255;
# endif
{
/* line 1481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return ((yyP256 + (yyP255 - 1)) / yyP255) * yyP255;

}

static int CalcRecordSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP258, register int yyP257)
# else
(yyP258, yyP257)
 register tTree yyP258;
 register int yyP257;
# endif
{
/* line 1485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int BTS; 
  if (yyP258->Kind == kNoField) {
/* line 1486 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return AlignToNext (yyP257, USER_ALIGNMENT);

  }
  if (yyP258->Kind == kAField) {
  if (Tree_IsType (yyP258->AField.Type, kPrimitive)) {
/* line 1487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   BTS = CalcTypeSize (yyP258->AField.Type);
  }
   return CalcRecordSize (yyP258->AField.Next, AlignToNext (yyP257, BTS) + BTS);

  }
/* line 1488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcRecordSize (yyP258->AField.Next, AlignToNext (yyP257, USER_ALIGNMENT) + CalcTypeSize (yyP258->AField.Type));

  }
 yyAbort ("CalcRecordSize");
}

static int CalcPackedRecordSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP260, register int yyP259)
# else
(yyP260, yyP259)
 register tTree yyP260;
 register int yyP259;
# endif
{
/* line 1492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int BTS; 
  if (yyP260->Kind == kNoField) {
/* line 1493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP259;

  }
  if (yyP260->Kind == kAField) {
/* line 1494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcPackedRecordSize (yyP260->AField.Next, yyP259 + CalcTypeSize (yyP260->AField.Type));

  }
 yyAbort ("CalcPackedRecordSize");
}

static int CalcRecordOffset
# if defined __STDC__ | defined __cplusplus
(register tTree yyP263, register tTree yyP262, register int yyP261)
# else
(yyP263, yyP262, yyP261)
 register tTree yyP263;
 register tTree yyP262;
 register int yyP261;
# endif
{
/* line 1498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int BTS; 
  if (yyP263->Kind == kAField) {
  if (Tree_IsType (yyP263->AField.Type, kPrimitive)) {
  if (yyP262->Kind == kAField) {
/* line 1499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP263->AField.Sym == yyP262->AField.Sym))) goto yyL1;
  }
   return AlignToNext (yyP261, CalcTypeSize (yyP263->AField.Type));
yyL1:;

  }
/* line 1501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   BTS = CalcTypeSize (yyP263->AField.Type);
  }
   return CalcRecordOffset (yyP263->AField.Next, yyP262, AlignToNext (yyP261, BTS) + BTS);

  }
  if (yyP262->Kind == kAField) {
/* line 1500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP263->AField.Sym == yyP262->AField.Sym))) goto yyL2;
  }
   return AlignToNext (yyP261, USER_ALIGNMENT);
yyL2:;

  }
/* line 1502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcRecordOffset (yyP263->AField.Next, yyP262, AlignToNext (yyP261, USER_ALIGNMENT) + CalcTypeSize (yyP263->AField.Type));

  }
 yyAbort ("CalcRecordOffset");
}

static int CalcPackedRecordOffset
# if defined __STDC__ | defined __cplusplus
(register tTree yyP266, register tTree yyP265, register int yyP264)
# else
(yyP266, yyP265, yyP264)
 register tTree yyP266;
 register tTree yyP265;
 register int yyP264;
# endif
{
/* line 1506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 int BTS; 
  if (yyP266->Kind == kAField) {
  if (yyP265->Kind == kAField) {
/* line 1507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP266->AField.Sym == yyP265->AField.Sym))) goto yyL1;
  }
   return yyP264;
yyL1:;

  }
/* line 1508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcPackedRecordOffset (yyP266->AField.Next, yyP265, yyP264 + CalcTypeSize (yyP266->AField.Type));

  }
 yyAbort ("CalcPackedRecordOffset");
}

int CalcOffset
# if defined __STDC__ | defined __cplusplus
(register tTree yyP268, register tTree yyP267)
# else
(yyP268, yyP267)
 register tTree yyP268;
 register tTree yyP267;
# endif
{
  if (yyP268->Kind == kUserRecord) {
/* line 1512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcRecordOffset (yyP268->UserRecord.FieldList, yyP267, 0);

  }
  if (yyP268->Kind == kUserPackedRecord) {
/* line 1513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcPackedRecordOffset (yyP268->UserPackedRecord.FieldList, yyP267, 0);

  }
 yyAbort ("CalcOffset");
}

int CalcTypeSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP269)
# else
(yyP269)
 register tTree yyP269;
# endif
{

  switch (yyP269->Kind) {
  case kArray:
  case kUserArray:
/* line 1517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsConstExpr (yyP269->Array.Expr)))) goto yyL1;
  }
   return  ValConstInt(yyP269->Array.Expr) * CalcTypeSize(yyP269->Array.Type) ;
yyL1:;

  break;
  case kUserRecord:
/* line 1518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcRecordSize (yyP269->UserRecord.FieldList, 0);

  case kUserPackedRecord:
/* line 1519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return CalcPackedRecordSize (yyP269->UserPackedRecord.FieldList, 0);

  case kInt:
  case kUserInt:
/* line 1520 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return IS_Mode(xx16bitINT) ? 2 : 4 ;

  case kInt16:
  case kUserInt16:
/* line 1521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 2;

  case kInt32:
  case kUserInt32:
/* line 1522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 4;

  case kInt64:
  case kUserInt64:
/* line 1523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 8;

  case kReal32:
  case kUserReal32:
/* line 1524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 4;

  case kReal64:
  case kUserReal64:
/* line 1525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 8;

  case kBool:
  case kUserBool:
/* line 1526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 1;

  case kByte:
  case kUserByte:
/* line 1527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return 1;

  }

/* line 1528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Internal Error - ConstArray Evaluation", xxError, NoPosition);
  }
   return - 1;

}

tTree ReduceElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP271, register tTree yyP270)
# else
(yyP271, yyP270)
 register tTree yyP271;
 register tTree yyP270;
# endif
{
  if (Tree_IsType (yyP271, kIdentifier)) {
/* line 1536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP271;

  }
  if (yyP271->Kind == kRange) {
  if (yyP270 == NULL) {
/* line 1537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP271->Range.Next = ReduceElement (yyP271->Range.Next, yyP271);
  }
   return yyP271;

  }
  if (yyP270->Kind == kRange) {
/* line 1538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP270->Range.From = MakeAddExpr (yyP270->Range.From, yyP271->Range.From);
  }
   return ReduceElement (yyP271->Range.Next, yyP270);

  }
  if (yyP270->Kind == kSubscript) {
/* line 1539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP270->Subscript.Expr = MakeAddExpr (yyP270->Subscript.Expr, yyP271->Range.From);
  }
   return ReduceElement (yyP271->Range.Next, yyP270);

  }
  }
  if (yyP271->Kind == kSubscript) {
/* line 1540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP271->Subscript.Next = ReduceElement (yyP271->Subscript.Next, yyP271);
  }
   return yyP271;

  }
 yyAbort ("ReduceElement");
}

tTree ReduceElemental
# if defined __STDC__ | defined __cplusplus
(register tTree yyP272)
# else
(yyP272)
 register tTree yyP272;
# endif
{
  if (Tree_IsType (yyP272, kElement)) {
/* line 1545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return ReduceElement (yyP272, NoTree);

  }
  if (Tree_IsType (yyP272, kTableElement)) {
/* line 1547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP272;

  }
 yyAbort ("ReduceElemental");
}

static tTree SplatSymbolTableFormals
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP277, register tTree yyP276, tSymbol yyP275, register int yyP274, register tTree yyP273)
# else
(yyP277, yyP276, yyP275, yyP274, yyP273)
 register tPosition yyP277;
 register tTree yyP276;
 tSymbol yyP275;
 register int yyP274;
 register tTree yyP273;
# endif
{
  if (yyP276->Kind == kAnExpr) {
/* line 1551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1552 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP274 == SymbolDepth (yyP275)))) goto yyL1;
  {
/* line 1553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275), kAFormal)))) goto yyL1;
  {
/* line 1554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275) -> AFormal . By, kByVal)))) goto yyL1;
  {
/* line 1555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP275) = mVal (yyP276->AnExpr.Expr->Expr.Pos, CopyTree (SymbolType (yyP275)), yyP275, yyP276->AnExpr.Expr);
  }
  }
  }
  }
   return SplatSymbolTableFormals (yyP277, yyP276->AnExpr.Next, yyP275 + 1, yyP274, mASpec (SymbolKind (yyP275), yyP273));
yyL1:;

  if (yyP276->AnExpr.Expr->Kind == kElementalExpr) {
/* line 1557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP274 == SymbolDepth (yyP275)))) goto yyL2;
  {
/* line 1559 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275), kAFormal)))) goto yyL2;
  {
/* line 1560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275) -> AFormal . By, kByRef)))) goto yyL2;
  {
/* line 1561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP275) = mIs (yyP276->AnExpr.Expr->ElementalExpr.Elemental->Elemental.Pos, CopyTree (SymbolType (yyP275)), yyP275, yyP276->AnExpr.Expr->ElementalExpr.Elemental);
  }
  }
  }
  }
   return SplatSymbolTableFormals (yyP277, yyP276->AnExpr.Next, yyP275 + 1, yyP274, mASpec (SymbolKind (yyP275), yyP273));
yyL2:;

  }
/* line 1563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP274 == SymbolDepth (yyP275)))) goto yyL3;
  {
/* line 1565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275), kAFormal)))) goto yyL3;
  {
/* line 1566 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Expected pass by reference argument", xxError, yyP277);
  }
  }
  }
   return yyP273;
yyL3:;

/* line 1568 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too many arguments to inline function", xxError, yyP277);
  }
   return yyP273;

  }
  if (yyP276->Kind == kNoExpr) {
/* line 1571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1572 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP274 == SymbolDepth (yyP275)))) goto yyL5;
  {
/* line 1573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP275), kAFormal)))) goto yyL5;
  {
/* line 1574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   Message ("Too few arguments to inline function", xxError, yyP277);
  }
  }
  }
   return yyP273;
yyL5:;

/* line 1576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP273;

  }
 yyAbort ("SplatSymbolTableFormals");
}

static tTree AppendSpecList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP279, register tTree yyP278)
# else
(yyP279, yyP278)
 register tTree yyP279;
 register tTree yyP278;
# endif
{
  if (yyP279->Kind == kASpec) {
/* line 1581 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP279->ASpec.Next = AppendSpecList (yyP279->ASpec.Next, yyP278);
  }
   return yyP279;

  }
  if (yyP279->Kind == kNoSpec) {
/* line 1584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP278;

  }
 yyAbort ("AppendSpecList");
}

static void UpdateFuncSymbol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP280)
# else
(yyP280)
 register tTree yyP280;
# endif
{

  switch (yyP280->Kind) {
  case kParRep:
/* line 1589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> ParRep . Sym >= FirstFormal))) goto yyL1;
  {
/* line 1589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> ParRep . Sym < FunctionInlining))) goto yyL1;
  {
/* line 1590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> ParRep . Sym = yyP280 -> ParRep . Sym + (FirstActual - FirstFormal);
/* line 1591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280 -> ParRep . Sym) = yyP280;
  }
  }
  }
   return;
yyL1:;

  break;
  case kAProcess:
/* line 1593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> AProcess . Sym >= FirstFormal))) goto yyL2;
  {
/* line 1593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> AProcess . Sym < FunctionInlining))) goto yyL2;
  {
/* line 1594 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> AProcess . Sym = yyP280 -> AProcess . Sym + (FirstActual - FirstFormal);
/* line 1595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280 -> AProcess . Sym) = yyP280;
  }
  }
  }
   return;
yyL2:;

  break;
  case kValof:
/* line 1597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Valof . Sym >= FirstFormal))) goto yyL3;
  {
/* line 1597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Valof . Sym < FunctionInlining))) goto yyL3;
  {
/* line 1598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> Valof . Sym = yyP280 -> Valof . Sym + (FirstActual - FirstFormal);
/* line 1599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280 -> Valof . Sym) = yyP280;
  }
  }
  }
   return;
yyL3:;

  break;
  case kTable:
  case kUserLiteral:
/* line 1601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Table . Sym >= FirstFormal))) goto yyL4;
  {
/* line 1601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Table . Sym < FunctionInlining))) goto yyL4;
  {
/* line 1602 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> Table . Sym = yyP280 -> Table . Sym + (FirstActual - FirstFormal);
/* line 1603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280 -> Table . Sym) = yyP280;
  }
  }
  }
   return;
yyL4:;

  break;
  case kString:
/* line 1605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> String . Sym >= FirstFormal))) goto yyL5;
  {
/* line 1605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> String . Sym < FunctionInlining))) goto yyL5;
  {
/* line 1606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> String . Sym = yyP280 -> String . Sym + (FirstActual - FirstFormal);
/* line 1607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280 -> String . Sym) = yyP280;
  }
  }
  }
   return;
yyL5:;

  break;
  case kCCode:
/* line 1609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> CCode . Scope >= FirstFormal))) goto yyL6;
  {
/* line 1609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> CCode . Scope < FunctionInlining))) goto yyL6;
  {
/* line 1610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> CCode . Scope = yyP280 -> CCode . Scope + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL6:;

  break;
  case kHCode:
/* line 1612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> HCode . Scope >= FirstFormal))) goto yyL7;
  {
/* line 1612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> HCode . Scope < FunctionInlining))) goto yyL7;
  {
/* line 1613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> HCode . Scope = yyP280 -> HCode . Scope + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL7:;

  break;
  case kIdentifier:
  case kUnmappedIdentifier:
/* line 1615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Identifier . Scope >= FirstFormal))) goto yyL8;
  {
/* line 1615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280 -> Identifier . Scope < FunctionInlining))) goto yyL8;
  {
/* line 1616 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> Identifier . Scope = yyP280 -> Identifier . Scope + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL8:;

  break;
  case kIs:
/* line 1618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Is.Sym >= FirstFormal))) goto yyL9;
  {
/* line 1618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Is.Sym < FunctionInlining))) goto yyL9;
  {
/* line 1619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->Is.Sym = yyP280->Is.Sym + (FirstActual - FirstFormal);
/* line 1620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->Is.Sym) = yyP280;
/* line 1621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->Is.Sym) = yyP280->Is.Type;
  }
  }
  }
   return;
yyL9:;

  break;
  case kAVal:
  case kVal:
  case kValRetypes:
  case kValReshapes:
/* line 1623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AVal.Sym >= FirstFormal))) goto yyL10;
  {
/* line 1623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AVal.Sym < FunctionInlining))) goto yyL10;
  {
/* line 1624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->AVal.Sym = yyP280->AVal.Sym + (FirstActual - FirstFormal);
/* line 1625 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->AVal.Sym) = yyP280;
/* line 1626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->AVal.Sym) = yyP280->AVal.Type;
  }
  }
  }
   return;
yyL10:;

  break;
  case kRetypes:
  case kReshapes:
/* line 1628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Retypes.Sym >= FirstFormal))) goto yyL11;
  {
/* line 1628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Retypes.Sym < FunctionInlining))) goto yyL11;
  {
/* line 1629 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->Retypes.Sym = yyP280->Retypes.Sym + (FirstActual - FirstFormal);
/* line 1630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->Retypes.Sym) = yyP280;
/* line 1631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->Retypes.Sym) = yyP280->Retypes.Type;
  }
  }
  }
   return;
yyL11:;

  break;
  case kDefSeqPro:
/* line 1633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefSeqPro.Sym >= FirstFormal))) goto yyL12;
  {
/* line 1633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefSeqPro.Sym < FunctionInlining))) goto yyL12;
  {
/* line 1634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->DefSeqPro.Sym = yyP280->DefSeqPro.Sym + (FirstActual - FirstFormal);
/* line 1635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->DefSeqPro.Sym) = yyP280;
  }
  }
  }
   return;
yyL12:;

  break;
  case kDefTagPro:
/* line 1637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefTagPro.Sym >= FirstFormal))) goto yyL13;
  {
/* line 1637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefTagPro.Sym < FunctionInlining))) goto yyL13;
  {
/* line 1638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->DefTagPro.Sym = yyP280->DefTagPro.Sym + (FirstActual - FirstFormal);
/* line 1639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->DefTagPro.Sym) = yyP280;
  }
  }
  }
   return;
yyL13:;

  break;
  case kDefAProc:
  case kDefProc:
  case kInlineDefProc:
  case kPrototypeProc:
/* line 1641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefAProc.Sym >= FirstFormal))) goto yyL14;
  {
/* line 1641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefAProc.Sym < FunctionInlining))) goto yyL14;
  {
/* line 1642 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->DefAProc.Sym = yyP280->DefAProc.Sym + (FirstActual - FirstFormal);
/* line 1643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->DefAProc.Sym) = yyP280;
  }
  }
  }
   return;
yyL14:;

  break;
  case kDefAFunc:
  case kDefFunc:
  case kInlineDefFunc:
  case kDefIsFunc:
  case kInlineDefIsFunc:
  case kPrototypeFunc:
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefAFunc.Sym >= FirstFormal))) goto yyL15;
  {
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->DefAFunc.Sym < FunctionInlining))) goto yyL15;
  {
/* line 1646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->DefAFunc.Sym = yyP280->DefAFunc.Sym + (FirstActual - FirstFormal);
/* line 1647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->DefAFunc.Sym) = yyP280;
/* line 1648 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->DefAFunc.Sym) = yyP280->DefAFunc.TypeList;
  }
  }
  }
   return;
yyL15:;

  break;
  case kAFormal:
/* line 1650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AFormal.Sym >= FirstFormal))) goto yyL16;
  {
/* line 1650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AFormal.Sym < FunctionInlining))) goto yyL16;
  {
/* line 1651 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->AFormal.Sym = yyP280->AFormal.Sym + (FirstActual - FirstFormal);
/* line 1652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->AFormal.Sym) = yyP280;
  }
  }
  }
   return;
yyL16:;

  break;
  case kReplicator:
/* line 1654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Replicator.Sym >= FirstFormal))) goto yyL17;
  {
/* line 1654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Replicator.Sym < FunctionInlining))) goto yyL17;
  {
/* line 1655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->Replicator.Sym = yyP280->Replicator.Sym + (FirstActual - FirstFormal);
/* line 1656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->Replicator.Sym) = yyP280;
/* line 1657 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->Replicator.Sym) = mInt ();
  }
  }
  }
   return;
yyL17:;

  break;
  case kATagPro:
/* line 1659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->ATagPro.Sym >= FirstFormal))) goto yyL18;
  {
/* line 1659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->ATagPro.Sym < FunctionInlining))) goto yyL18;
  {
/* line 1660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->ATagPro.Sym = yyP280->ATagPro.Sym + (FirstActual - FirstFormal);
/* line 1661 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->ATagPro.Sym) = yyP280;
  }
  }
  }
   return;
yyL18:;

  break;
  case kAnId:
/* line 1663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AnId.Sym >= FirstFormal))) goto yyL19;
  {
/* line 1663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AnId.Sym < FunctionInlining))) goto yyL19;
  {
/* line 1664 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->AnId.Sym = yyP280->AnId.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL19:;

  break;
  case kUserArray:
/* line 1666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserArray.Sym >= FirstFormal))) goto yyL20;
  {
/* line 1666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserArray.Sym < FunctionInlining))) goto yyL20;
  {
/* line 1667 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserArray.Sym = yyP280->UserArray.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL20:;

  break;
  case kUserInt:
/* line 1669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt.Sym >= FirstFormal))) goto yyL21;
  {
/* line 1669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt.Sym < FunctionInlining))) goto yyL21;
  {
/* line 1670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserInt.Sym = yyP280->UserInt.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL21:;

  break;
  case kUserInt16:
/* line 1672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt16.Sym >= FirstFormal))) goto yyL22;
  {
/* line 1672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt16.Sym < FunctionInlining))) goto yyL22;
  {
/* line 1673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserInt16.Sym = yyP280->UserInt16.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL22:;

  break;
  case kUserInt32:
/* line 1675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt32.Sym >= FirstFormal))) goto yyL23;
  {
/* line 1675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt32.Sym < FunctionInlining))) goto yyL23;
  {
/* line 1676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserInt32.Sym = yyP280->UserInt32.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL23:;

  break;
  case kUserInt64:
/* line 1678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt64.Sym >= FirstFormal))) goto yyL24;
  {
/* line 1678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserInt64.Sym < FunctionInlining))) goto yyL24;
  {
/* line 1679 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserInt64.Sym = yyP280->UserInt64.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL24:;

  break;
  case kUserReal32:
/* line 1681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserReal32.Sym >= FirstFormal))) goto yyL25;
  {
/* line 1681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserReal32.Sym < FunctionInlining))) goto yyL25;
  {
/* line 1682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserReal32.Sym = yyP280->UserReal32.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL25:;

  break;
  case kUserReal64:
/* line 1684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserReal64.Sym >= FirstFormal))) goto yyL26;
  {
/* line 1684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserReal64.Sym < FunctionInlining))) goto yyL26;
  {
/* line 1685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserReal64.Sym = yyP280->UserReal64.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL26:;

  break;
  case kUserByte:
/* line 1687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserByte.Sym >= FirstFormal))) goto yyL27;
  {
/* line 1687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserByte.Sym < FunctionInlining))) goto yyL27;
  {
/* line 1688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserByte.Sym = yyP280->UserByte.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL27:;

  break;
  case kUserBool:
/* line 1690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserBool.Sym >= FirstFormal))) goto yyL28;
  {
/* line 1690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserBool.Sym < FunctionInlining))) goto yyL28;
  {
/* line 1691 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserBool.Sym = yyP280->UserBool.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL28:;

  break;
  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 1693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserStructTypes.Sym >= FirstFormal))) goto yyL29;
  {
/* line 1693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->UserStructTypes.Sym < FunctionInlining))) goto yyL29;
  {
/* line 1694 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->UserStructTypes.Sym = yyP280->UserStructTypes.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL29:;

  break;
  case kAField:
/* line 1696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AField.Sym >= FirstFormal))) goto yyL30;
  {
/* line 1696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->AField.Sym < FunctionInlining))) goto yyL30;
  {
/* line 1697 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->AField.Sym = yyP280->AField.Sym + (FirstActual - FirstFormal);
/* line 1698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (yyP280->AField.Sym) = yyP280;
/* line 1699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolType (yyP280->AField.Sym) = yyP280->AField.Type;
  }
  }
  }
   return;
yyL30:;

  break;
  case kNamed:
/* line 1701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Named.Sym >= FirstFormal))) goto yyL31;
  {
/* line 1701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Named.Sym < FunctionInlining))) goto yyL31;
  {
/* line 1702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->Named.Sym = yyP280->Named.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL31:;

  break;
  case kFuncCall:
/* line 1704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->FuncCall.Sym >= FirstFormal))) goto yyL32;
  {
/* line 1704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->FuncCall.Sym < FunctionInlining))) goto yyL32;
  {
/* line 1705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->FuncCall.Sym = yyP280->FuncCall.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL32:;

  break;
  case kInstance:
/* line 1707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Instance.Sym >= FirstFormal))) goto yyL33;
  {
/* line 1707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP280->Instance.Sym < FunctionInlining))) goto yyL33;
  {
/* line 1708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280 -> Instance . Scope = yyP280 -> Instance . Scope + (FirstActual - FirstFormal);
/* line 1709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyP280->Instance.Sym = yyP280->Instance.Sym + (FirstActual - FirstFormal);
  }
  }
  }
   return;
yyL33:;

  break;
  }

/* line 1711 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

;
}

static tTree CopyFuncBody
# if defined __STDC__ | defined __cplusplus
(register tTree yyP284, register tTree yyP283, register int yyP282, tSymbol yyP281)
# else
(yyP284, yyP283, yyP282, yyP281)
 register tTree yyP284;
 register tTree yyP283;
 register int yyP282;
 tSymbol yyP281;
# endif
{
/* line 1715 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree NODE; 
  if (yyP284->Kind == kInlineDefFunc) {
/* line 1716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NODE = CopyTree (yyP284->InlineDefFunc.Valof);
/* line 1718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NODE -> Valof . SpecList = AppendSpecList (yyP283, NODE -> Valof . SpecList);
/* line 1719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NODE -> Valof . VS = yyP282;
/* line 1720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NODE -> Valof . Sym = yyP281;
  }
   return NODE;

  }
  if (Tree_IsType (yyP284, kDefIsFunc)) {
/* line 1722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  register tTree yyV1;
  {
/* line 1723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kSkip)
    yyV1->Skip.Pos = yyP284->DefIsFunc.Pos;
   {register tTree yyW1;
    yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyW1,kNoSpec)
    yyV1->Skip.SpecList = yyW1;
   }
   NODE = mValof (yyP284->DefIsFunc.Pos, yyP283, yyV1, CopyTree (yyP284->DefIsFunc.ExprList), yyP281, yyP282);
  }
  {
   return NODE;
  }
 }

  }
 yyAbort ("CopyFuncBody");
}

static bool WithinScope
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP286, tSymbol yyP285)
# else
(yyP286, yyP285)
 tSymbol yyP286;
 tSymbol yyP285;
# endif
{
/* line 1728 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1728 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP286 > yyP285))) goto yyL1;
  {
/* line 1728 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((SymbolScope (yyP286) == yyP285))) goto yyL1;
  }
  }
   return true;
yyL1:;

/* line 1729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP286 > yyP285))) goto yyL2;
  {
/* line 1729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (WithinScope (SymbolScope (yyP286), yyP285))) goto yyL2;
  }
  }
   return true;
yyL2:;

  return false;
}

tTree CreateFuncCall
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP290, tSymbol yyP289, register tTree yyP288, tInt yyP287)
# else
(yyP290, yyP289, yyP288, yyP287)
 register tPosition yyP290;
 tSymbol yyP289;
 register tTree yyP288;
 tInt yyP287;
# endif
{
/* line 1733 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree NODE; tSymbol NS; 
/* line 1735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((IsSTVFunction (SymbolKind (yyP289))))) goto yyL1;
  {
/* line 1736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NS = AddSymbol (NoIdent);
/* line 1737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   NODE = mElementalExpr (yyP290, mTableExpr (yyP290, mFuncCall (yyP290, yyP289, yyP288, yyP287), NS));
/* line 1738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   SymbolKind (NS) = NODE;
  }
  }
   return NODE;
yyL1:;

/* line 1741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP289), kInlineDefFunc) || Tree_IsType (SymbolKind (yyP289), kDefIsFunc)))) goto yyL2;
  {
/* line 1742 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   OpenAtScope (SymbolScope (yyP289));
/* line 1743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
{
      int depth = SymbolDepth(yyP289)+1;
      int i;
      tTree formals = SymbolKind(yyP289)->InlineDefFunc.Formals;
      if (Tree_IsType(formals,kNoFormals))         
	FirstFormal = formals->NoFormals.Sym;
      else if (Tree_IsType(formals,kNoFormal))
	Message("Internal failure- couldn't find start of formal list",xxFatal,yyP290);
      else
	FirstFormal = formals->AFormal.Sym;
      FunctionInlining = yyP289;
      FirstActual = FunctionInlining;
      i = FirstFormal;
      while (i<yyP289) 
      {
	while (depth<SymbolDepth(i)) { OpenScope(); depth++; }
	while (depth>SymbolDepth(i)) { CloseScope(); depth--; }
	{
	  tSymbol tmp=AddSymbol(Symbol(i).Id);
	  if (FirstActual==FunctionInlining) FirstActual = tmp;
	  SymbolType(tmp) = SymbolType(i);
	  SymbolKind(tmp) = SymbolKind(i);
	}
	i++;
      }
      while (depth>SymbolDepth(i)) { CloseScope(); depth--; } ;
      NODE = CopyFuncBody(SymbolKind(yyP289),SplatSymbolTableFormals(yyP290,yyP288,FirstActual,SymbolDepth(FirstActual),mNoSpec()),yyP287,AddSymbol(NoIdent));
      NODE->Valof.FuncCall = mFuncCall(yyP290,yyP289,yyP288,yyP287);
      SymbolKind(NODE->Valof.Sym) = NODE;
      TraverseTreeTD(NODE,UpdateFuncSymbol);      
    }
  }
  }
   return NODE;
yyL2:;

/* line 1775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return mFuncCall (yyP290, yyP289, yyP288, yyP287);

}

tTree CreateProcCall
# if defined __STDC__ | defined __cplusplus
(register tPosition yyP295, register tTree yyP294, tSymbol yyP293, register tTree yyP292, tSymbol yyP291)
# else
(yyP295, yyP294, yyP293, yyP292, yyP291)
 register tPosition yyP295;
 register tTree yyP294;
 tSymbol yyP293;
 register tTree yyP292;
 tSymbol yyP291;
# endif
{
/* line 1779 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree NODE; 
/* line 1780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP293), kInlineDefProc)))) goto yyL1;
  {
/* line 1781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   OpenAtScope (SymbolScope (yyP293));
/* line 1782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
{
      int depth = SymbolDepth(yyP293)+1;
      int i;
      tTree formals = SymbolKind(yyP293)->InlineDefProc.Formals;
      if (Tree_IsType(formals,kNoFormals))         
	FirstFormal = formals->NoFormals.Sym;
      else if (Tree_IsType(formals,kNoFormal))
	Message("Internal failure- couldn't find start of formal list",xxFatal,yyP295);
      else 
	FirstFormal = formals->AFormal.Sym;

      FunctionInlining = yyP293;
      FirstActual = FunctionInlining;
      i = FirstFormal;
      while (i<yyP293) 
      {
	while (depth<SymbolDepth(i)) { OpenScope(); depth++; }
	while (depth>SymbolDepth(i)) { CloseScope(); depth--; }
	{
	  tSymbol tmp=AddSymbol(Symbol(i).Id);
	  if (FirstActual==FunctionInlining) FirstActual = tmp;
	  SymbolType(tmp) = SymbolType(i);
	  SymbolKind(tmp) = SymbolKind(i);
	}
	i++;
      }
      while (depth>SymbolDepth(i)) { CloseScope(); depth--; } ;
      NODE = CopyTree(SymbolKind(yyP293)->InlineDefProc.Process);
      NODE->Process.SpecList = AppendSpecList(SplatSymbolTableFormals(yyP295,yyP292,FirstActual,SymbolDepth(FirstActual),mNoSpec()),NODE->Process.SpecList);
      TraverseTreeTD(NODE,UpdateFuncSymbol);
    }
  }
  }
   return NODE;
yyL1:;

/* line 1814 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return mInstance (yyP295, yyP294, yyP293, yyP292, yyP291);

}

bool CheckValidUserType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP296)
# else
(yyP296)
 register tTree yyP296;
# endif
{
  if (Tree_IsType (yyP296, kUserDataTypes)) {
/* line 1819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! CheckAllDataFields (yyP296->UserDataTypes.FieldList)))) goto yyL1;
  {
/* line 1819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }
  }
yyL1:;

  }
  if (yyP296->Kind == kUserChanRecord) {
/* line 1820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((! CheckAllChanFields (yyP296->UserChanRecord.FieldList)))) goto yyL2;
  {
/* line 1820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return false;
  }
  }
yyL2:;

  }
/* line 1821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

}

static bool CheckAllDataFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP297)
# else
(yyP297)
 register tTree yyP297;
# endif
{
  if (yyP297->Kind == kNoField) {
/* line 1824 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP297->Kind == kAField) {
/* line 1825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsDataType (yyP297->AField.Type))) goto yyL2;
  {
/* line 1825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckAllDataFields (yyP297->AField.Next))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool CheckAllChanFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP298)
# else
(yyP298)
 register tTree yyP298;
# endif
{
  if (yyP298->Kind == kNoField) {
/* line 1828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return true;

  }
  if (yyP298->Kind == kAField) {
/* line 1829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (IsChanType (yyP298->AField.Type))) goto yyL2;
  {
/* line 1829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! (CheckAllChanFields (yyP298->AField.Next))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

tTree GetTagProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP299)
# else
(yyP299)
 register tTree yyP299;
# endif
{
  if (yyP299 == NULL) {
/* line 1832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NULL;

  }
  if (yyP299->Kind == kChan) {
  if (yyP299->Chan.Protocol->Kind == kNamed) {
/* line 1833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP299->Chan.Protocol->Named.Sym), kDefTagPro)))) goto yyL2;
  }
   return SymbolKind (yyP299->Chan.Protocol->Named.Sym) -> DefTagPro . TagProList;
yyL2:;

  }
  }
/* line 1834 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NULL;

}

void LookupProtocolTag
# if defined __STDC__ | defined __cplusplus
(register tIdent yyP302, register tPosition yyP301, register tTree yyP300, tSymbol * yyP303)
# else
(yyP302, yyP301, yyP300, yyP303)
 register tIdent yyP302;
 register tPosition yyP301;
 register tTree yyP300;
 tSymbol * yyP303;
# endif
{
  if (yyP300->Kind == kNoTagPro) {
/* line 1837 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP303 = NoSymbol;
   return;

  }
  if (yyP300->Kind == kATagPro) {
/* line 1838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP302 == Symbol (yyP300->ATagPro.Sym) . Id))) goto yyL2;
  }
   * yyP303 = yyP300->ATagPro.Sym;
   return;
yyL2:;

/* line 1839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tSymbol yyV1;
  {
/* line 1839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   LookupProtocolTag (yyP302, yyP301, yyP300->ATagPro.Next, & yyV1);
  }
   * yyP303 = yyV1;
   return;
 }

  }
;
}

tTree LookupField
# if defined __STDC__ | defined __cplusplus
(register tIdent yyP306, register tPosition yyP305, register tTree yyP304, tSymbol * yyP307)
# else
(yyP306, yyP305, yyP304, yyP307)
 register tIdent yyP306;
 register tPosition yyP305;
 register tTree yyP304;
 tSymbol * yyP307;
# endif
{
/* line 1842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 tTree FL; 
  if (yyP304->Kind == kNoField) {
/* line 1843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   * yyP307 = NoSymbol;
   return yyP304;

  }
  if (yyP304->Kind == kAField) {
/* line 1844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   if (! ((yyP306 == Symbol (yyP304->AField.Sym) . Id))) goto yyL2;
  }
   * yyP307 = yyP304->AField.Sym;
   return yyP304;
yyL2:;

/* line 1845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
 {
  tSymbol yyV1;
  {
/* line 1845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   FL = LookupField (yyP306, yyP305, yyP304->AField.Next, & yyV1);
  }
   * yyP307 = yyV1;
  {
   return FL;
  }
 }

  }
 yyAbort ("LookupField");
}

tTree GetFieldList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP308)
# else
(yyP308)
 register tTree yyP308;
# endif
{
  if (yyP308 == NULL) {
/* line 1848 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NoTree;

  }
  if (Tree_IsType (yyP308, kUserStructTypes)) {
/* line 1849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP308->UserStructTypes.FieldList;

  }
/* line 1850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return NoTree;

}

tTree ArrayPrimType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP309)
# else
(yyP309)
 register tTree yyP309;
# endif
{
  if (Tree_IsType (yyP309, kArray)) {
/* line 1855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return ArrayPrimType (yyP309->Array.Type);

  }
  if (Tree_IsType (yyP309, kPrimitive)) {
/* line 1856 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return yyP309;

  }
 yyAbort ("ArrayPrimType");
}

void MarkNonAtomicIf
# if defined __STDC__ | defined __cplusplus
(register tTree IF)
# else
(IF)
 register tTree IF;
# endif
{
  if (IF->Kind == kIf) {
/* line 1864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1865 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
IF->If.ATOMIC = false; 
/* line 1866 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   MarkNonAtomicIfList (IF->If.ChoiceList);
  }
   return;

  }
  if (IF->Kind == kIfRep) {
/* line 1869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1870 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
IF->IfRep.ATOMIC = false; 
/* line 1871 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   MarkNonAtomicIfList (IF->IfRep.ChoiceList);
  }
   return;

  }
;
}

static void MarkNonAtomicIfList
# if defined __STDC__ | defined __cplusplus
(register tTree IF)
# else
(IF)
 register tTree IF;
# endif
{
  if (IF->Kind == kAChoice) {
/* line 1876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
IF->AChoice.ATOMIC = false; 
/* line 1878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   MarkNonAtomicIfList (IF->AChoice.Next);
  }
   return;

  }
  if (IF->Kind == kANewIf) {
/* line 1881 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
  {
/* line 1882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
IF->ANewIf.ATOMIC = false; 
/* line 1883 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   MarkNonAtomicIf (IF->ANewIf.IfType);
/* line 1884 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   MarkNonAtomicIfList (IF->ANewIf.Next);
  }
   return;

  }
  if (IF->Kind == kNoChoice) {
/* line 1887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */
   return;

  }
;
}

void BeginTypeChk ()
{
}

void CloseTypeChk ()
{
}
