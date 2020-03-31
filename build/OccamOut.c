# include "OccamOut.h"
# include "yyOccamOut.w"
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

/* line 29 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */

  #include <stdio.h>
  #include <stdlib.h>
  #include <TypeChk.h>

  /*{{{  statics*/
  static int indent = 0;
  static int col = 0;   
  static bool NL = false;  
  extern FILE* yyf;
  static void yyAbort(char *);
  
  /*}}}*/
  /*{{{  defines*/
  #undef yyWrite
  
  static void Indent()     { if (!NL) fputc('\n',yyf); NL=true; indent+=2; }
  static void Outdent()    { if (!NL) fputc('\n',yyf); NL=true; indent-=2; }
  static void NewLine()    { if (!NL) fputc('\n',yyf); NL=true;}
  static void DoNL()       { if (NL==true) { int i; NL = false; for(i=0;i<indent;i++) fputc(' ',yyf); col=indent;} }
  
  #define WriteInt(a)  {DoNL(); col+=fprintf(yyf,"%d",(int)a);}
  #define WriteReal(a) {DoNL(); col+=fprintf(yyf,"%*G",a.RNumber.p,a.RNumber.n);}
  #define WriteByte(a) {DoNL(); col+=fprintf(yyf,"\'*#%02X\'",(unsigned)a);}
  #define WriteBool(a) {DoNL(); col+=fprintf(yyf,"%s",(a)?"TRUE":"FALSE");}
  #define WriteStr(a)  {DoNL(); col+=fprintf(yyf,"%s",a);}
  #define WriteChar(a)  {DoNL(); col+=1; fputc(a,yyf);}
  #define WriteId(a)   { DoNL(); WriteIdent(yyf,a); }
  
  #define SetIndent(a)   (a=indent,indent=col)
  #define ClearIndent(a) (indent=a)
  #define COND_Write(b,s) { if (b) yyWrite(s); }
  
  #define SizeInt      sizeof(int)
  
  #define yyWrite(s)   {DoNL(); col+=fprintf(yyf,"%s",s);}
  
  static void WriteSym(tSymbol s)   
  {
    DoNL(); 
    if (Symbol(s).Id==NoIdent) 
      col+=fprintf(yyf,"tmp.%d",s); 
    else 
    { 
      tStringRef id; 
      id=GetStringRef(Symbol(s).Id); 
      col+=LengthSt(id);
      WriteIdent(yyf,Symbol(s).Id);
    }
  }  
  
  #define WarnNotImp(P,S)    ErrorMessageI(xxNotImplemented,xxWarning,P,xxString,S)
  
  #define IsPrintable(c) (((c)>=' ') && ((c)<='~'))
  
  /*}}}*/
  /*{{{  functions*/
  /*{{{  static void WriteOccStr(char *str,int len)*/
  static void WriteOccStr(char *str,int len)
  {
    int i;
  
    for (i=0;i<len;i++)
    {
      char c = str[i] ;
    
      if (c == '*' || c == '"' || c == '\'')
      {
        fprintf(yyf,"*%c",c) ;
        col+=2;
      }
      else if (IsPrintable(c))
      {
        fputc(c,yyf) ;
        col++ ;
      }
      else if (c == 13)
      {
        fprintf(yyf,"*c") ;
        col+=2;
      }
      else if (c == 10)
      {
        fprintf(yyf,"*n") ;
        col+=2;
      }
      else if (c == '\t')
      {
        fprintf(yyf,"*t") ;
        col+=2;
      }
      else
      {
        fprintf(yyf,"*#%02x",c) ;
        col+=4;
      }
    }
  }
  
  /*}}}*/
  /*{{{  void WriteStrRef(tStringRef r)*/
  void WriteStrRef(tStringRef r)
  {
    char *temp ;
    int len = LengthSt(r) ;
  
    DoNL() ;
  
    if ((temp = malloc((len==0)?1:len+1)) == NULL)
      yyAbort("Out of heap") ;
    else
      {
        int i;
        
        StGetString (r,temp) ;
        WriteOccStr(temp,len);
        free(temp) ;
      }
  }
  /*}}}*/
  /*{{{  void WritetInt(tInt val,bool int64)*/
  static void WritetInt(tInt val,bool int64)
  {
    char str[20];
    int len=0;
    bool neg=(val < 0);
    tInt value=(neg)?-val:val;
    bool hex=(val > 1000000) || (val < -1000000);
  
    DoNL() ;
  
    if (hex)
    {
      int digits = (int64) ? 16 : 8;
      char hexdigits[] = "0123456789ABCDEF";
  
      while(len < digits) { str[len++] = hexdigits[val & 15]; val = val >> 4; }
      while(len>1 && str[len-1]=='0') --len;
  
      fprintf(yyf,"#");
    }
    else
    {
      while(len==0 || value > 0) { str[len++]='0'+(value % 10); value = value / 10; }
      if (neg) fputc('-',yyf);
    }
  
    {
      int i = 0;
      int j=len-1;
      char c;
  
      while(i < j) { c = str[i]; str[i++] = str[j]; str[j--] = c; }
    }
    
    str[len] = 0;
    fprintf(yyf,"%s",str);
  }
    
    
  
  /*}}}*/
  /*{{{  void OutputOccam(char *name, tTree Root)*/
  void OutputOccam(char *name, tTree Root)
  {
    FILE *savef = yyf;
  
    yyf = stdout;
    if (strcmp(name,"-") != 0)
      if ((yyf=fopen(name,"w"))==NULL) MessageI("Cannot open file ",xxFatal,NoPosition,xxString,name);
  
    Code(Root);
    if(ferror(yyf))  
      Message("Error writing output file",xxFatal, NoPosition);  
    if (yyf != stdout)
      if(fclose(yyf))  
        Message("Error writing output file",xxFatal, NoPosition);  
  
  
    yyf = savef;
  }
  /*}}}*/
  /*}}}*/
  /*{{{  field support*/
  #define MAX_FIELD_DEPTH 100
  static tSymbol Fields[MAX_FIELD_DEPTH] ;
  static int Field_Depth=0;
  static void WriteFields()
  {
    int i;
    for (i=0;i<Field_Depth;i++) { WriteStr(".."); WriteSym(Fields[i]); }
  }
  #define PushField(S) Fields[Field_Depth++]=S
  #define PopField()  Field_Depth--
  /*}}}*/



static void yyExit () { Exit (1); }

void (* OccamOut_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module OccamOut, routine %s failed\n", yyFunction);
 OccamOut_Exit ();
}

void Code ARGS((tTree yyP1));
static void CodeDeclList ARGS((tTree yyP2));
static void CodeDecl ARGS((tTree yyP4, tSymbol yyP3));
static void CodeDeclFields ARGS((tSymbol yyP6, tTree yyP5));
static void CodeAbbrev ARGS((tTree yyP8, tTree yyP7));
static void CodeAbbrevFields ARGS((tTree yyP10, tTree yyP9));
static void CodeFormals ARGS((tTree yyP11));
static void CodeFormal ARGS((tTree yyP13, tTree yyP12));
static void CodeFormalFields ARGS((tTree yyP15, tTree yyP14));
static void CodeComplexExpr ARGS((tTree yyP18, tTree yyP17, char yyP16));
static void CodeExprFields ARGS((tTree yyP21, tTree yyP20, char yyP19));
static void CodeUserProtocol ARGS((tTree yyP23, tTree yyP22));
static void CodeUserProtocolFields ARGS((tTree yyP25, tTree yyP24));
static void SemiSep ARGS((tTree yyP26));
static void CodeExpr ARGS((tTree yyP27));
static void CodeElemental ARGS((tTree yyP28));
static void CodeConst ARGS((tTree yyP30, tValue yyP29));
static void CodeBy ARGS((tTree yyP31));
static void CodeOp ARGS((tTree yyP32));
void CodeType ARGS((tTree yyP33));
static void CodeUserType ARGS((tTree yyP35, char yyP34));
static void CodeTypeFields ARGS((tTree yyP37, char yyP36));
static void CodeConstArray ARGS((tTree yyP39, tConstArray yyP38));

void Code
# if defined __STDC__ | defined __cplusplus
(register tTree yyP1)
# else
(yyP1)
 register tTree yyP1;
# endif
{

  switch (yyP1->Kind) {
  case kASpec:
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ASpec.Spec);
/* line 231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ASpec.Next);
  }
   return;

  case kNoSpec:
/* line 232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kIs:
/* line 234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeAbbrev (yyP1, yyP1->Is.Type);
  }
   return;

  case kVal:
/* line 235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeAbbrev (yyP1, yyP1->Val.Type);
  }
   return;

  case kRetypes:
  case kReshapes:
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP1->Retypes.Type);
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->Retypes.Sym);
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" RETYPES ");
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP1->Retypes.Elemental);
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" :");
/* line 237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kValRetypes:
  case kValReshapes:
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("VAL ");
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP1->ValRetypes.Type);
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->ValRetypes.Sym);
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" RETYPES ");
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->ValRetypes.Expr);
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" :");
/* line 238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kDefProc:
  case kInlineDefProc:
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PROC ");
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DefProc.Sym);
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormals (yyP1->DefProc.Formals);
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefProc.Process);
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kDeclType:
  if (Tree_IsType (yyP1->DeclType.Type, kUserDataTypes)) {
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PROTOCOL ");
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DeclType.Type->UserDataTypes.Sym);
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" IS ");
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP1->DeclType.Type, ';');
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  }
/* line 243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kDecl:
/* line 244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDeclList (yyP1->Decl.IdList);
  }
   return;

  case kDefSeqPro:
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PROTOCOL ");
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DefSeqPro.Sym);
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" IS ");
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefSeqPro.SeqProList);
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kDefTagPro:
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PROTOCOL ");
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DefTagPro.Sym);
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("CASE ");
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefTagPro.TagProList);
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kDefFunc:
  case kInlineDefFunc:
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP1->DefFunc.TypeList, ',');
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("FUNCTION ");
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DefFunc.Sym);
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormals (yyP1->DefFunc.Formals);
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
/* line 248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefFunc.Valof->Valof.SpecList);
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("VALOF");
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefFunc.Valof->Valof.Process);
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("RESULT ");
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefFunc.Valof->Valof.ExprList);
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kDefIsFunc:
  case kInlineDefIsFunc:
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP1->DefIsFunc.TypeList, ',');
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("FUNCTION ");
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->DefIsFunc.Sym);
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormals (yyP1->DefIsFunc.Formals);
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" IS ");
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->DefIsFunc.ExprList);
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kReplicator:
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->Replicator.Sym);
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" = ");
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->Replicator.From);
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FOR ");
/* line 252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->Replicator.For);
  }
   return;

  case kNoTagPro:
/* line 254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kATagPro:
  if (yyP1->ATagPro.SeqProList->Kind == kNoSeqPro) {
/* line 255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->ATagPro.Sym);
/* line 255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
/* line 255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ATagPro.Next);
  }
   return;

  }
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->ATagPro.Sym);
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("; ");
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ATagPro.SeqProList);
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
/* line 256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ATagPro.Next);
  }
   return;

  case kStart:
/* line 258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Start.Process);
/* line 258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
/* line 258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("");
  }
   return;

  case kAny:
/* line 260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("ANY ");
  }
   return;

  case kNamed:
/* line 261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->Named.Sym);
/* line 261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
  }
   return;

  case kSimple:
/* line 262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP1->Simple.Type, ';');
  }
   return;

  case kCounted:
/* line 263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserProtocol (yyP1, yyP1->Counted.Type);
  }
   return;

  case kASeqPro:
  if (yyP1->ASeqPro.Next->Kind == kNoSeqPro) {
/* line 265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ASeqPro.Protocol);
  }
   return;

  }
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ASeqPro.Protocol);
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("; ");
/* line 266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ASeqPro.Next);
  }
   return;

  case kAnExpr:
  if (yyP1->AnExpr.Next->Kind == kNoExpr) {
/* line 268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP1->AnExpr.Expr, ExprType (yyP1->AnExpr.Expr), ',');
  }
   return;

  }
/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP1->AnExpr.Expr, ExprType (yyP1->AnExpr.Expr), ',');
/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (", ");
/* line 269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AnExpr.Next);
  }
   return;

  case kNoCase:
/* line 271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kACase:
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ACase.SpecList);
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SemiSep (yyP1->ACase.ExprList);
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ACase.Process);
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ACase.Next);
  }
   return;

  case kAssign:
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Assign.SpecList);
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Assign.Lval);
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" := ");
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Assign.Rval);
/* line 274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kAlt:
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Alt.SpecList);
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->Alt.Pri, "PRI ");
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("ALT");
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Alt.AltList);
/* line 276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kAltRep:
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltRep.SpecList);
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->AltRep.Pri, "PRI ");
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("ALT");
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltRep.Replicator);
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltRep.AltList);
/* line 277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kInput:
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Input.SpecList);
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP1->Input.Element);
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ? ");
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SemiSep (yyP1->Input.ExprList);
/* line 279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kOutput:
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Output.SpecList);
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP1->Output.Element);
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ! ");
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SemiSep (yyP1->Output.ExprList);
/* line 280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kCaseIn:
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->CaseIn.SpecList);
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP1->CaseIn.Element);
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ? ");
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("CASE");
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->CaseIn.CaseList);
/* line 281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kAfter:
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->After.SpecList);
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP1->After.Element);
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ? ");
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("AFTER ");
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->After.Expr);
/* line 282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kSeq:
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Seq.SpecList);
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("SEQ");
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Seq.ProcList);
/* line 283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kSeqRep:
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->SeqRep.SpecList);
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("SEQ ");
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->SeqRep.Replicator);
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->SeqRep.Process);
/* line 284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kPar:
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Par.SpecList);
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->Par.PType == xxPriPar, "PRI ");
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->Par.PType == xxPlacedPar, "PLACED ");
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PAR");
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Par.ProcList);
/* line 286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kParRep:
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ParRep.SpecList);
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->ParRep.PType == xxPriPar, "PRI ");
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   COND_Write (yyP1->ParRep.PType == xxPlacedPar, "PLACED ");
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PAR ");
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ParRep.Replicator);
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ParRep.Process);
/* line 287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kProcessor:
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Processor.SpecList);
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("PROCESSOR ");
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->Processor.Expr);
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteId (yyP1->Processor.Name);
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Processor.Process);
/* line 289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kIf:
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->If.SpecList);
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("IF");
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->If.ChoiceList);
/* line 291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kIfRep:
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->IfRep.SpecList);
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("IF ");
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->IfRep.Replicator);
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->IfRep.ChoiceList);
/* line 292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kCase:
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Case.SpecList);
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("CASE ");
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Case.Selector);
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Case.ChoiceList);
/* line 293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kWhile:
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->While.SpecList);
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("WHILE ");
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->While.Expr);
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->While.Process);
/* line 294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kSkip:
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Skip.SpecList);
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("SKIP");
/* line 295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kStop:
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Stop.SpecList);
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("STOP");
/* line 296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kCCode:
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->CCode.SpecList);
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("SKIP -- ");
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteStrRef (yyP1->CCode.Code);
/* line 297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kInstance:
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Instance.SpecList);
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP1->Instance.Sym);
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->Instance.ExprList);
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
/* line 298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  case kNullProcess:
/* line 299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->NullProcess.SpecList);
  }
   return;

  case kNoAlt:
/* line 301 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kAnAlt:
/* line 302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AnAlt.SpecList);
/* line 302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AnAlt.Guard);
/* line 302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AnAlt.Next);
  }
   return;

  case kANewAlt:
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewAlt.SpecList);
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewAlt.AltType);
/* line 303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewAlt.Next);
  }
   return;

  case kAltInput:
  if (yyP1->AltInput.Expr->Kind == kConst) {
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((yyP1->AltInput.Expr->Const.Value . Number == true))) goto yyL54;
  {
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltInput.Input);
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltInput.Process);
/* line 305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
  }
   return;
yyL54:;

  }
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->AltInput.Expr);
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" & ");
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltInput.Input);
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltInput.Process);
/* line 306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kAltTimer:
  if (yyP1->AltTimer.Expr->Kind == kConst) {
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((yyP1->AltTimer.Expr->Const.Value . Number == true))) goto yyL56;
  {
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltTimer.After);
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltTimer.Process);
/* line 307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
  }
   return;
yyL56:;

  }
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->AltTimer.Expr);
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" & ");
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltTimer.After);
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltTimer.Process);
/* line 308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kAltCaseIn:
  if (yyP1->AltCaseIn.Expr->Kind == kConst) {
/* line 309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((yyP1->AltCaseIn.Expr->Const.Value . Number == true))) goto yyL58;
  {
/* line 309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltCaseIn.CaseIn);
  }
  }
   return;
yyL58:;

  }
/* line 310 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 310 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->AltCaseIn.Expr);
/* line 310 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" & ");
/* line 310 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AltCaseIn.CaseIn);
  }
   return;

  case kNoInput:
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1->NoInput.Expr);
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" & SKIP ");
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->NoInput.Process);
/* line 311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
  }
   return;

  case kNoChoice:
/* line 313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kAChoice:
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AChoice.SpecList);
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AChoice.ExprList);
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AChoice.Process);
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AChoice.Next);
  }
   return;

  case kANewIf:
/* line 315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewIf.SpecList);
/* line 315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewIf.IfType);
/* line 315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->ANewIf.Next);
  }
   return;

  case kNoProcess:
/* line 317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kAProcess:
/* line 318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AProcess.Process);
/* line 318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP1->AProcess.Next);
  }
   return;

  case kPrototypeFunc:
/* line 320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kPrototypeProc:
/* line 321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kParFrame:
/* line 323 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kCallFrame:
/* line 324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }

/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP1);
  }
   return;

;
}

static void CodeDeclList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP2)
# else
(yyP2)
 register tTree yyP2;
# endif
{
  if (yyP2->Kind == kNoId) {
/* line 333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP2->Kind == kAnId) {
/* line 334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDecl (SymbolType (yyP2->AnId.Sym), yyP2->AnId.Sym);
/* line 334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDeclList (yyP2->AnId.Next);
  }
   return;

  }
;
}

static void CodeDecl
# if defined __STDC__ | defined __cplusplus
(register tTree yyP4, tSymbol yyP3)
# else
(yyP4, yyP3)
 register tTree yyP4;
 tSymbol yyP3;
# endif
{
  if (Tree_IsType (yyP4, kUserStructTypes)) {
/* line 338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDeclFields (yyP3, yyP4->UserStructTypes.FieldList);
  }
   return;

  }
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP4);
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP3);
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteFields (/*yyP3*/);
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

;
}

static void CodeDeclFields
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP6, register tTree yyP5)
# else
(yyP6, yyP5)
 tSymbol yyP6;
 register tTree yyP5;
# endif
{
  if (yyP5->Kind == kNoField) {
/* line 343 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP5->Kind == kAField) {
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP5->AField.Sym);
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDecl (yyP5->AField.Type, yyP6);
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
/* line 344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeDeclFields (yyP6, yyP5->AField.Next);
  }
   return;

  }
;
}

static void CodeAbbrev
# if defined __STDC__ | defined __cplusplus
(register tTree yyP8, register tTree yyP7)
# else
(yyP8, yyP7)
 register tTree yyP8;
 register tTree yyP7;
# endif
{
  if (Tree_IsType (yyP7, kUserStructTypes)) {
/* line 349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeAbbrevFields (yyP8, yyP7->UserStructTypes.FieldList);
  }
   return;

  }
  if (yyP8->Kind == kIs) {
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP7);
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP8->Is.Sym);
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" IS ");
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP8->Is.Elemental);
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP8->Kind == kVal) {
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("VAL ");
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP7);
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP8->Val.Sym);
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" IS ");
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP8->Val.Expr);
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (":");
/* line 351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

  }
;
}

static void CodeAbbrevFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP10, register tTree yyP9)
# else
(yyP10, yyP9)
 register tTree yyP10;
 register tTree yyP9;
# endif
{
  if (yyP9->Kind == kNoField) {
/* line 356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP9->Kind == kAField) {
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP9->AField.Sym);
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeAbbrev (yyP10, yyP9->AField.Type);
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
/* line 357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeAbbrevFields (yyP10, yyP9->AField.Next);
  }
   return;

  }
;
}

static void CodeFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP11)
# else
(yyP11)
 register tTree yyP11;
# endif
{
  if (Tree_IsType (yyP11, kNoFormal)) {
/* line 362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP11->Kind == kAFormal) {
  if (Tree_IsType (yyP11->AFormal.Next, kNoFormal)) {
/* line 363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormal (yyP11, yyP11->AFormal.Type);
  }
   return;

  }
/* line 364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormal (yyP11, yyP11->AFormal.Type);
/* line 364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (",");
/* line 364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormals (yyP11->AFormal.Next);
  }
   return;

  }
;
}

static void CodeFormal
# if defined __STDC__ | defined __cplusplus
(register tTree yyP13, register tTree yyP12)
# else
(yyP13, yyP12)
 register tTree yyP13;
 register tTree yyP12;
# endif
{
  if (Tree_IsType (yyP12, kUserStructTypes)) {
/* line 369 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 369 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormalFields (yyP13, yyP12->UserStructTypes.FieldList);
  }
   return;

  }
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeBy (yyP13->AFormal.By);
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP12);
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP13->AFormal.Sym);
/* line 370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteFields ();
  }
   return;

;
}

static void CodeFormalFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP15, register tTree yyP14)
# else
(yyP15, yyP14)
 register tTree yyP15;
 register tTree yyP14;
# endif
{
  if (yyP14->Kind == kNoField) {
/* line 374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP14->Kind == kAField) {
  if (yyP14->AField.Next->Kind == kNoField) {
/* line 375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP14->AField.Sym);
/* line 375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormal (yyP15, yyP14->AField.Type);
/* line 375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
  }
   return;

  }
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP14->AField.Sym);
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormal (yyP15, yyP14->AField.Type);
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (",");
/* line 376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeFormalFields (yyP15, yyP14->AField.Next);
  }
   return;

  }
;
}

static void CodeComplexExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP18, register tTree yyP17, register char yyP16)
# else
(yyP18, yyP17, yyP16)
 register tTree yyP18;
 register tTree yyP17;
 register char yyP16;
# endif
{
  if (Tree_IsType (yyP17, kUserStructTypes)) {
/* line 381 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 381 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExprFields (yyP18, yyP17->UserStructTypes.FieldList, yyP16);
  }
   return;

  }
/* line 382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP18);
  }
   return;

;
}

static void CodeExprFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP21, register tTree yyP20, register char yyP19)
# else
(yyP21, yyP20, yyP19)
 register tTree yyP21;
 register tTree yyP20;
 register char yyP19;
# endif
{
  if (yyP21->Kind == kElementalExpr) {
  if (yyP21->ElementalExpr.Elemental->Kind == kUserLiteral) {
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP21->ElementalExpr.Elemental->UserLiteral.ExprList);
  }
   return;

  }
  }
  if (yyP20->Kind == kNoField) {
/* line 387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP20->Kind == kAField) {
  if (yyP20->AField.Next->Kind == kNoField) {
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP20->AField.Sym);
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP21, yyP20->AField.Type, yyP19);
/* line 388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
  }
   return;

  }
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PushField (yyP20->AField.Sym);
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP21, yyP20->AField.Type, yyP19);
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   PopField ();
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteChar (yyP19);
/* line 389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExprFields (yyP21, yyP20->AField.Next, yyP19);
  }
   return;

  }
;
}

static void CodeUserProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP23, register tTree yyP22)
# else
(yyP23, yyP22)
 register tTree yyP23;
 register tTree yyP22;
# endif
{
  if (yyP23->Kind == kCounted) {
  if (Tree_IsType (yyP22, kUserStructTypes)) {
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserProtocolFields (yyP23, yyP22->UserStructTypes.FieldList);
  }
   return;

  }
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP23->Counted.Length);
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("::");
/* line 396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP22);
  }
   return;

  }
;
}

static void CodeUserProtocolFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP25, register tTree yyP24)
# else
(yyP25, yyP24)
 register tTree yyP25;
 register tTree yyP24;
# endif
{
  if (yyP24->Kind == kNoField) {
/* line 400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP24->Kind == kAField) {
  if (yyP24->AField.Next->Kind == kNoField) {
/* line 401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserProtocol (yyP25, yyP24->AField.Type);
  }
   return;

  }
/* line 402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserProtocol (yyP25, yyP24->AField.Type);
/* line 402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (";");
/* line 402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserProtocolFields (yyP25, yyP24->AField.Next);
  }
   return;

  }
;
}

static void SemiSep
# if defined __STDC__ | defined __cplusplus
(register tTree yyP26)
# else
(yyP26)
 register tTree yyP26;
# endif
{
  if (yyP26->Kind == kAnExpr) {
  if (yyP26->AnExpr.Next->Kind == kNoExpr) {
/* line 408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP26->AnExpr.Expr, ExprType (yyP26->AnExpr.Expr), ';');
  }
   return;

  }
/* line 409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP26->AnExpr.Expr, ExprType (yyP26->AnExpr.Expr), ';');
/* line 409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("; ");
/* line 409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SemiSep (yyP26->AnExpr.Next);
  }
   return;

  }
/* line 411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyAbort ("SemiSep");
  }
   return;

;
}

static void CodeExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP27)
# else
(yyP27)
 register tTree yyP27;
# endif
{
/* line 416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
 int lpindent, valindent;
  if (Tree_IsType (yyP27, kStringElement)) {
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsArrayType (ExprType (yyP27))))) goto yyL1;
  {
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP27)))) goto yyL1;
  {
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("\"");
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteOccStr ((char*) (yyP27 -> StringElement . CA), CalcTypeSize (ExprType (yyP27)));
/* line 417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("\"");
  }
  }
  }
   return;
yyL1:;

  }
  if (yyP27->Kind == kElementalExpr) {
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsArrayType (ExprType (yyP27->ElementalExpr.Elemental))))) goto yyL2;
  {
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP27)))) goto yyL2;
  {
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeConstArray (ExprType (yyP27->ElementalExpr.Elemental), yyP27->ElementalExpr.Elemental -> Elemental . CA);
/* line 418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
  }
  }
   return;
yyL2:;

  }
  if (yyP27->Kind == kStringSubscript) {
/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP27)))) goto yyL3;
  {
/* line 419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeConst (ExprType (yyP27), ValConstExpr (yyP27));
  }
  }
   return;
yyL3:;

/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP27->StringSubscript.Next);
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP27->StringSubscript.Expr);
/* line 421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP27->Kind == kStringRange) {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP27->StringRange.Next);
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FROM ");
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP27->StringRange.From);
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FOR ");
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP27->StringRange.For);
/* line 422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP27->Kind == kConvert) {
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP27->Convert.Type);
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Convert.Expr);
/* line 424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  }
  if (Tree_IsType (yyP27, kExpr)) {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP27)))) goto yyL7;
  {
/* line 426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeConst (ExprType (yyP27), ValConstExpr (yyP27));
  }
  }
   return;
yyL7:;

  }

  switch (yyP27->Kind) {
  case kValof:
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SetIndent (lpindent);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("( ");
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   SetIndent (valindent);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Valof.SpecList);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("VALOF");
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Indent ();
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Valof.Process);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("RESULT ");
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Valof.ExprList);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Outdent ();
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   ClearIndent (valindent);
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
/* line 428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   ClearIndent (lpindent);
  }
   return;

  case kBinary:
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Binary.Lop);
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeOp (yyP27->Binary.Op);
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Binary.Rop);
/* line 429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  case kUnary:
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeOp (yyP27->Unary.Op);
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Unary.Expr);
/* line 430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  case kFuncCall:
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP27->FuncCall.Sym);
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->FuncCall.ExprList);
/* line 431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  case kDefault:
/* line 432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("ELSE");
  }
   return;

  case kElementalExpr:
/* line 433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP27->ElementalExpr.Elemental);
  }
   return;

  case kRound:
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP27->Round.Type);
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("ROUND ");
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Round.Expr);
/* line 434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  case kTrunc:
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(");
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP27->Trunc.Type);
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("TRUNC ");
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP27->Trunc.Expr);
/* line 435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (")");
  }
   return;

  case kCountArray:
/* line 436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeComplexExpr (yyP27, ExprType (yyP27->CountArray.Expr), ';');
  }
   return;

  case kUnBound:
/* line 437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  case kNoExpr:
/* line 439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }

/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   fprintf (stderr, "Fell Through CodeExpr with -\n");
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteTreeNode (stderr, yyP27);
/* line 440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

;
}

static void CodeElemental
# if defined __STDC__ | defined __cplusplus
(register tTree yyP28)
# else
(yyP28)
 register tTree yyP28;
# endif
{
  if (yyP28->Kind == kTableSubscript) {
/* line 445 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 445 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP28)))) goto yyL1;
  {
/* line 445 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeConst (ExprType (yyP28), ValConstExpr (yyP28));
  }
  }
   return;
yyL1:;

/* line 446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((IsConstExpr (yyP28)))) goto yyL2;
  {
/* line 446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeConst (ExprType (yyP28), ValConstExpr (yyP28));
  }
  }
   return;
yyL2:;

/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP28->TableSubscript.Next);
/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->TableSubscript.Expr);
/* line 448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (Tree_IsType (yyP28, kTable)) {
/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP28->Table.ExprList);
/* line 447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP28->Kind == kTableRange) {
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP28->TableRange.Next);
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FROM ");
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->TableRange.From);
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FOR ");
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->TableRange.For);
/* line 449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (Tree_IsType (yyP28, kIdentifier)) {
/* line 451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP28->Identifier.Sym);
/* line 451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteFields ();
  }
   return;

  }
  if (yyP28->Kind == kSubscript) {
/* line 452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   if (! ((Tree_IsType (ElementalType (yyP28->Subscript.Next), kUserStructTypes)))) goto yyL7;
  {
/* line 452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP28->Subscript.Next);
/* line 452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (".#.");
/* line 452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->Subscript.Expr);
  }
  }
   return;
yyL7:;

/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP28->Subscript.Next);
/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->Subscript.Expr);
/* line 453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP28->Kind == kRange) {
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeElemental (yyP28->Range.Next);
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FROM ");
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->Range.From);
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" FOR ");
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeExpr (yyP28->Range.For);
/* line 454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
  }
   return;

  }
;
}

static void CodeConst
# if defined __STDC__ | defined __cplusplus
(register tTree yyP30, tValue yyP29)
# else
(yyP30, yyP29)
 register tTree yyP30;
 tValue yyP29;
# endif
{

  switch (yyP30->Kind) {
  case kInt64:
  case kUserInt64:
/* line 461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WritetInt (yyP29 . Number, true);
/* line 461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(INT64)");
  }
   return;

  case kInt:
  case kUserInt:
/* line 462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WritetInt (yyP29 . Number, false);
  }
   return;

  case kInt16:
  case kUserInt16:
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WritetInt (yyP29 . Number, false);
/* line 463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(INT16)");
  }
   return;

  case kInt32:
  case kUserInt32:
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WritetInt (yyP29 . Number, false);
/* line 464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(INT32)");
  }
   return;

  case kReal32:
  case kUserReal32:
/* line 465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteReal (yyP29);
/* line 465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(REAL32)");
  }
   return;

  case kReal64:
  case kUserReal64:
/* line 466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteReal (yyP29);
/* line 466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("(REAL64)");
  }
   return;

  case kByte:
  case kUserByte:
/* line 467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteByte (yyP29 . Number);
  }
   return;

  case kBool:
  case kUserBool:
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteBool (yyP29 . Number);
  }
   return;

  }

/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   fprintf (stderr, "Fell Through CodeConst with -\n");
/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteTreeNode (stderr, yyP30);
/* line 470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   NewLine ();
  }
   return;

;
}

static void CodeBy
# if defined __STDC__ | defined __cplusplus
(register tTree yyP31)
# else
(yyP31)
 register tTree yyP31;
# endif
{
  if (yyP31->Kind == kByVal) {
/* line 475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("VAL ");
  }
   return;

  }
  if (yyP31->Kind == kByRef) {
/* line 476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
;
}

static void CodeOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP32)
# else
(yyP32)
 register tTree yyP32;
# endif
{

  switch (yyP32->Kind) {
  case kLess:
/* line 480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("<");
  }
   return;

  case kGreater:
/* line 481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (">");
  }
   return;

  case kLessEqual:
/* line 482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("<=");
  }
   return;

  case kGreaterEqual:
/* line 483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (">=");
  }
   return;

  case kEqual:
/* line 484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("=");
  }
   return;

  case kNotEqual:
/* line 485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("<>");
  }
   return;

  case kAdd:
/* line 486 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 486 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("+");
  }
   return;

  case kSubtract:
/* line 487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("-");
  }
   return;

  case kMultiply:
/* line 488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("*");
  }
   return;

  case kDivide:
/* line 489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("/");
  }
   return;

  case kRem:
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("\\");
  }
   return;

  case kBitAnd:
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("/\\");
  }
   return;

  case kBitOr:
/* line 492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("\\/");
  }
   return;

  case kBitEor:
/* line 493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("><");
  }
   return;

  case kBitNot:
/* line 494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("~");
  }
   return;

  case kLeftShift:
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("<<");
  }
   return;

  case kRightShift:
/* line 496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (">>");
  }
   return;

  case kSize:
/* line 497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("SIZE ");
  }
   return;

  case kPlus:
/* line 498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" PLUS ");
  }
   return;

  case kMinus:
/* line 499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" MINUS ");
  }
   return;

  case kTimes:
/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" TIMES ");
  }
   return;

  case kAfterOp:
/* line 501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" AFTER ");
  }
   return;

  case kAnd:
/* line 502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" AND ");
  }
   return;

  case kOr:
/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" OR ");
  }
   return;

  case kNot:
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 504 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" NOT ");
  }
   return;

  }

;
}

void CodeType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP33)
# else
(yyP33)
 register tTree yyP33;
# endif
{
  if (yyP33 == NULL) {
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("NoType");
  }
   return;

  }

  switch (yyP33->Kind) {
  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteSym (yyP33->UserStructTypes.Sym);
/* line 509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite (" ");
  }
   return;

  case kInt:
  case kUserInt:
/* line 510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("INT ");
  }
   return;

  case kInt16:
  case kUserInt16:
/* line 511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("INT16 ");
  }
   return;

  case kInt32:
  case kUserInt32:
/* line 512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("INT32 ");
  }
   return;

  case kInt64:
  case kUserInt64:
/* line 513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("INT64 ");
  }
   return;

  case kReal32:
  case kUserReal32:
/* line 514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("REAL32 ");
  }
   return;

  case kReal64:
  case kUserReal64:
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("REAL64 ");
  }
   return;

  case kBool:
  case kUserBool:
/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("BOOL ");
  }
   return;

  case kByte:
  case kUserByte:
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("BYTE ");
  }
   return;

  case kChan:
  if (yyP33->Chan.Protocol->Kind == kSimple) {
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("CHAN OF ");
/* line 518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP33->Chan.Protocol->Simple.Type);
  }
   return;

  }
/* line 519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("CHAN OF ");
/* line 519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP33->Chan.Protocol);
  }
   return;

  case kTimer:
/* line 520 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 520 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("TIMER ");
  }
   return;

  case kArray:
  case kUserArray:
/* line 521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP33->Array.Expr);
/* line 521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
/* line 521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP33->Array.Type);
  }
   return;

  }

;
}

static void CodeUserType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP35, register char yyP34)
# else
(yyP35, yyP34)
 register tTree yyP35;
 register char yyP34;
# endif
{
  if (Tree_IsType (yyP35, kUserStructTypes)) {
/* line 524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeTypeFields (yyP35->UserStructTypes.FieldList, yyP34);
  }
   return;

  }
  if (yyP35->Kind == kAType) {
  if (yyP35->AType.Next->Kind == kNoType) {
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP35->AType.Type, yyP34);
  }
   return;

  }
/* line 526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP35->AType.Type, yyP34);
/* line 526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteChar (yyP34);
/* line 526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP35->AType.Next, yyP34);
  }
   return;

  }
  if (Tree_IsType (yyP35, kArray)) {
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("[");
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   Code (yyP35->Array.Expr);
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   yyWrite ("]");
/* line 527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP35->Array.Type, yyP34);
  }
   return;

  }
/* line 528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeType (yyP35);
  }
   return;

;
}

static void CodeTypeFields
# if defined __STDC__ | defined __cplusplus
(register tTree yyP37, register char yyP36)
# else
(yyP37, yyP36)
 register tTree yyP37;
 register char yyP36;
# endif
{
  if (yyP37->Kind == kNoField) {
/* line 531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   return;

  }
  if (yyP37->Kind == kAField) {
  if (yyP37->AField.Next->Kind == kNoField) {
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP37->AField.Type, yyP36);
  }
   return;

  }
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeUserType (yyP37->AField.Type, yyP36);
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   WriteChar (yyP36);
/* line 533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
   CodeTypeFields (yyP37->AField.Next, yyP36);
  }
   return;

  }
;
}

static void CodeConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP39, tConstArray yyP38)
# else
(yyP39, yyP38)
 register tTree yyP39;
 tConstArray yyP38;
# endif
{
  if (Tree_IsType (yyP39, kArray)) {
  if (Tree_IsType (yyP39->Array.Type, kArray)) {
/* line 539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */

      { int i,sz;
        sz = CalcTypeSize(yyP39->Array.Type);
        for(i = 0; i < ValConstInt(yyP39->Array.Expr); ++i)
        {
          if (i) WriteChar(',');
          WriteStr("[");
          CodeConstArray(yyP39->Array.Type,yyP38+i*sz);
          WriteStr("]");
        }
      }
    
  }
   return;

  }
/* line 554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */
  {
/* line 555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */

      { int i;
        int sz = CalcTypeSize(yyP39->Array.Type);
        
        for(i = 0; i < ValConstInt(yyP39->Array.Expr); ++i)
        {
          if (i) WriteChar(',');
          CodeConst(yyP39->Array.Type,ReadConstArray(yyP39->Array.Type,yyP38+(sz*i)));
        }
      }
    
  }
   return;

  }
;
}

void BeginOccamOut ()
{
}

void CloseOccamOut ()
{
}
