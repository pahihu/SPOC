# ifndef yyTypeChk
# define yyTypeChk

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"

/* line 112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/typechk.trafo" */

  #define MOSTPOSINT32 ((tInt)((long)0x7FFFFFFFL))
  #define MOSTNEGINT32 (~MOSTPOSINT32)
  #define INT32MASK    ((tInt)((tUInt)0xFFFFFFFFUL))

  #define MOSTPOSINT16 ((tInt)((short)0x7FFFUL))
  #define MOSTNEGINT16 ((tInt)((short)0x8000UL))
  #define INT16MASK    ((tInt)((tUInt)0xFFFFUL))

  #define MOSTPOSINT   (IS_Mode(xx16bitINT) ? MOSTPOSINT16 : MOSTPOSINT32)
  #define MOSTNEGINT   (IS_Mode(xx16bitINT) ? MOSTNEGINT16 : MOSTNEGINT32)
  #define INTMASK      (IS_Mode(xx16bitINT) ? INT16MASK : INT32MASK)

  #define MOSTPOSBYTE  ((tInt)0xff)
  #define MOSTNEGBYTE  ((tInt)0)
  #define BYTEMASK     ((tInt)0xff)
  #define BOOLMASK     ((tInt)0x01)

#ifdef SUPPORT_INT64
  #define MOSTPOSINT64 ((tInt)0x7FFFFFFFFFFFFFFFLL)
  #define MOSTNEGINT64 (~MOSTPOSINT64) 
  #define INT64MASK    ((tInt)~0LL)
  #define RNGCHKINT32(v) (((v)>=MOSTNEGINT) && (v <= MOSTPOSINT))
#else
  #define MOSTPOSINT64 MOSTPOSINT32
  #define MOSTNEGINT64 MOSTNEGINT32
  #define RNGCHKINT32(v) (true)
#endif

  #define USER_ALIGNMENT 8
  
  void RetypeConst(int ss,tConstArray src, int ds, tConstArray dest);


extern void (* TypeChk_Exit) ();

extern tTree GetTagProtocol ARGS((tTree yyP299));
extern void LookupProtocolTag ARGS((tIdent yyP302, tPosition yyP301, tTree yyP300, tSymbol * yyP303));
extern bool SameBaseType ARGS((tTree yyP117, tTree yyP116));
extern bool SameUnderlyingType ARGS((tTree yyP119, tTree yyP118));
extern int CountExprs ARGS((tTree yyP250));
extern tTree CreateFuncCall ARGS((tPosition yyP290, tSymbol yyP289, tTree yyP288, tInt yyP287));
extern tTree CreateProcCall ARGS((tPosition yyP295, tTree yyP294, tSymbol yyP293, tTree yyP292, tSymbol yyP291));
extern tTree DefaultType ARGS((tTree yyP41));
extern tTree TransformUserLiterals ARGS((tTree yyP59, tTree yyP58));
extern tTree SynthDefType ARGS((tTree yyP37, tTree yyP36, tTree yyP35));
extern tTree SynthDefTableType ARGS((tTree yyP38));
extern tTree SynthDefSubscriptType ARGS((tTree yyP40, tTree yyP39));
extern void InheritFromFormals ARGS((tTree yyP45, tTree yyP44));
extern void InheritFromProtocol ARGS((tTree yyP49, tTree yyP48));
extern void InheritFromTable ARGS((tTree yyP53, tTree yyP52));
extern void InheritFromType ARGS((tTree yyP55, tTree yyP54));
extern tTree InheritLhsType ARGS((tTree yyP43, tTree yyP42));
extern bool CheckValidUserType ARGS((tTree yyP296));
extern tTree GetFieldList ARGS((tTree yyP308));
extern tTree LookupField ARGS((tIdent yyP306, tPosition yyP305, tTree yyP304, tSymbol * yyP307));
extern tTree TypeUserDataType ARGS((tTree yyP3, tSymbol yyP2, tPosition yyP1));
extern bool IsUserDataType ARGS((tTree yyP113));
extern void TypeFormals ARGS((tTree yyP4));
extern void TypeDecl ARGS((tTree yyP6, tTree yyP5));
extern tTree TypeAbbrev ARGS((tTree yyP9, tTree yyP8, tPosition yyP7));
extern tTree TypeSubscript ARGS((tTree yyP13, tTree yyP12, tTree yyP11, tPosition yyP10));
extern tTree TypeRange ARGS((tTree yyP17, tTree yyP16, tTree yyP15, tPosition yyP14));
extern tTree TypeBinary ARGS((tTree yyP21, tTree yyP20, tTree yyP19, tPosition yyP18));
extern tTree TypeUnary ARGS((tTree yyP24, tTree yyP23, tPosition yyP22));
extern tTree TypeExprList ARGS((tTree yyP26));
extern tTree TypeTable ARGS((tTree yyP29, tTree yyP28, tTree yyP27));
extern tTree TypeTableExpr ARGS((tTree yyP30));
extern tTree TypeString ARGS((int yyP33, tTree yyP32, tPosition yyP31));
extern tTree StripTypeList ARGS((tTree yyP34));
extern void TypeCheckAssign ARGS((tTree yyP62, tTree yyP61, tPosition yyP60));
extern void TypeCheckResults ARGS((tTree yyP71, tTree yyP70, tPosition yyP69));
extern void TypeCheckParameters ARGS((tTree yyP75, tTree yyP74, tPosition yyP73, int yyP72));
extern void TypeCheckIfList ARGS((tTree yyP78));
extern void TypeCheckTimerInput ARGS((tTree yyP80, tPosition yyP79));
extern void TypeCheckInputProtocol ARGS((tTree yyP83, tTree yyP82, tPosition yyP81));
extern void TypeCheckOutputProtocol ARGS((tTree yyP88, tTree yyP87, tPosition yyP86));
extern void TypeCheckCaseInputs ARGS((tTree yyP91, tTree yyP90, tPosition yyP89));
extern void TypeCheckCaseList ARGS((tTree yyP103, tTree yyP102, bool yyP101));
extern void TypeCheckRetypes ARGS((tTree yyP107, tTree yyP106, tTree yyP105, tPosition yyP104));
extern bool CheckWritable ARGS((tTree yyP126));
extern bool CheckWritableExprList ARGS((tTree yyP127));
extern bool CheckForVals ARGS((tTree yyP128));
extern bool CheckValofScope ARGS((tTree yyP130, int yyP129));
extern bool IsChanOrTimerType ARGS((tTree yyP131));
extern bool IsTimerType ARGS((tTree yyP134));
extern bool IsChanType ARGS((tTree yyP133));
extern bool IsArrayType ARGS((tTree yyP135));
extern bool IsScalarType ARGS((tTree yyP136));
extern bool IsComputableType ARGS((tTree yyP115));
extern int CalcTypeSize ARGS((tTree yyP269));
extern int CalcOffset ARGS((tTree yyP268, tTree yyP267));
extern bool CheckEvalBiOp ARGS((tTree yyP145, tTree yyP144));
extern bool CheckEvalUniOp ARGS((tTree yyP150, tTree yyP149, tTree yyP148));
extern tValue EvalBiExpr ARGS((tPosition yyP155, tTree yyP154, tValue yyP153, tTree yyP152, tValue yyP151));
extern tValue EvalUniExpr ARGS((tTree yyP178, tTree yyP177, tTree yyP176));
extern tTree ReduceElement ARGS((tTree yyP271, tTree yyP270));
extern tTree ReduceElemental ARGS((tTree yyP272));
extern bool CheckConstConvert ARGS((tTree yyP179));
extern tValue EvalConstConvert ARGS((tPosition yyP182, tTree yyP181, tTree yyP180));
extern tValue CheckConst ARGS((tPosition yyP186, tTree yyP185, tInt yyP184, bool yyP183));
extern bool CheckConstRoundTrunc ARGS((tTree yyP188, tTree yyP187));
extern tValue EvalConstRound ARGS((tPosition yyP196, tTree yyP195, tTree yyP194, tValue yyP193));
extern tValue EvalConstTrunc ARGS((tPosition yyP192, tTree yyP191, tTree yyP190, tValue yyP189));
extern bool CheckConstId ARGS((tTree yyP197));
extern tValue EvalConstId ARGS((tTree yyP198));
extern tTree MakeConstInt ARGS((int yyP245, tPosition yyP244));
extern bool CheckValidDeclType ARGS((tTree yyP142));
extern bool CheckFuncType ARGS((tTree yyP143));
extern tValue EvalConstSubscript ARGS((tTree yyP212, int yyP211, tConstArray yyP210));
extern tInt EvalMostPos ARGS((tTree yyP207));
extern tInt EvalMostNeg ARGS((tTree yyP208));
extern tInt TestIntOVF ARGS((tPosition yyP201, tTree yyP200, tInt yyP199));
extern tInt SignExtendInt ARGS((tTree yyP203, tInt yyP202));
extern void UpdateElementalUsage ARGS((tTree yyP77, tUsage yyP76));
extern tConstArray ConstArrayFromString ARGS((tTree yyP214, tStringRef yyP213));
extern tConstArray ConstArrayFromExprList ARGS((tTree yyP216, tTree yyP215));
extern tConstArray ConstArrayFromDecl ARGS((tTree yyP218, tTree yyP217));
extern tConstArray SubscriptConstArray ARGS((tTree yyP232, tConstArray yyP231, tTree yyP230, tTree yyP229));
extern tConstArray RangeConstArray ARGS((tTree yyP228, tConstArray yyP227, int yyP226));
extern tValue ReadConstArray ARGS((tTree yyP234, tConstArray yyP233));
extern void WriteConstArray ARGS((tTree yyP237, tValue yyP236, tConstArray yyP235));
extern tTree ArrayPrimType ARGS((tTree yyP309));
extern void MarkNonAtomicIf ARGS((tTree IF));

extern void BeginTypeChk ();
extern void CloseTypeChk ();

# endif
