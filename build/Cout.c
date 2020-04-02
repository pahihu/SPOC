# include "Cout.h"
# include "yyCout.w"
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

/* line 51 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */

/*{{{  includes*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Scanner.h"
#include "Errors.h"
#include "DynArray.h"
#include "Symbol.h"
#include "Transform.h"
#include "Fopenenv.h"
#include "TypeChk.h"
/*}}}*/
/*{{{  statics*/
extern bool cgdebug;

static bool NL = false;
static tPosition POS= {0,0,NULL};
static int indent = 0;
static int line_count = 0;
static char *outname;                  /* base name of generated file */
static tTree CurProc;
extern FILE* yyf;
static void yyAbort(char *);
extern void Code(tTree);

static int CurrentLabel;

static char M_MAIN[]="<<MAIN>>";             static int S_MAIN= sizeof(M_MAIN)-1; 
static char M_HEADER[]="<<HEADER>>";         static int S_HEADER= sizeof(M_HEADER)-1; 
static char M_CODE[]="<<CODE>>";             static int S_CODE= sizeof(M_CODE)-1;
static char M_NAME[]="<<NAME>>";             static int S_NAME= sizeof(M_NAME)-1;
static char M_PROTOTYPES[]="<<PROTOTYPES>>"; static int S_PROTOTYPES=sizeof(M_PROTOTYPES)-1;
static char M_SWITCHES[]="<<SWITCHES>>";     static int S_SWITCHES= sizeof(M_SWITCHES)-1; 
static char M_PARAMETERS[]="<<PARAMETERS>>"; static int S_PARAMETERS=sizeof(M_PARAMETERS)-1;
static char M_DEBUG[]="<<DEBUG>>";           static int S_DEBUG=sizeof(M_DEBUG)-1;
static char M_SERVERWS[]="<<SERVERWS>>";     static int S_SERVERWS=sizeof(M_SERVERWS)-1;

static char PRM[] = "Param";  /* base name of parameters to library units */
static char DIM[] = "_Dim";   /* hidden dimension extension */
static char CHK[] = "CHK";    /* name of checking macro */
static char RNGCHK[] = "RNGCHK";    /* name of checking macro */
static char RNGCHKLB[] = "RNGCHKLB";    /* name of checking macro */
static char RNGCHKUB[] = "RNGCHKUB";    /* name of checking macro */
static char ASSIGN[] = "ASSIGN";    /* name of array copying macro */
static char MSG_ASM[] = "MSG_ASM";
static char MSG_ARM[] = "MSG_ARM";
static char MSG_AIM[] = "MSG_AIM";
static char MSG_CITL[] = "MSG_CITL";
static char CHK_SWITCH[][20] = { "CHECK_CONVERSIONS","CHECK_INTERNALS","CHECK_ARRAYS","CHECK_RANGES" };
static char ERR_MODES[][20] = { "TERMINATE_ON_ERROR","CONTINUE_ON_ERROR","STOPP_ON_ERROR" };

static char M_SPOC_ISERVER[] = "spoc.iserver";
static int  S_SPOC_ISERVER   = sizeof(M_SPOC_ISERVER)-1;

eErrorModes ErrorMode = xxTerminateOnError;

/*}}}*/
/*{{{  static void ShowPosition()*/
#define SetPosition(P) POS=P
static void ShowPosition()  
{  
  if (IS_Mode(xxOccamLines))
  {  
    if (POS.File!=NULL)
    {
      if (IS_Mode(xxLinesAsComments))
      {
	fprintf(yyf,"/*line %4d \"",POS.Line);
	WriteString(yyf,POS.File);
	fprintf(yyf,"\"*/ ");
      }
      else
      {
	fprintf(yyf,"#line %4d \"",POS.Line);
	WriteString(yyf,POS.File);
	fprintf(yyf,"\"\n");
      }  
    }
  }
}
/*}}}*/
/*{{{  output control*/
#undef yyWrite
static int output_text=true;
/*{{{  static void DoNL()*/
static void DoNL()
{
  if (NL==true) 
  { 
    int i; 
    ShowPosition();
    NL = false; 
    for(i=0;i<indent;i++) fputc(' ',yyf);
  }
  output_text=true;
}
/*}}}*/
static void Indent()     { if (!NL) fputc('\n',yyf); NL=true; indent+=2; line_count = 0;}
static void Outdent()    { if (!NL) fputc('\n',yyf); NL=true; indent-=2; line_count = 0;}
static void BigIndent()  { if (!NL) fputc('\n',yyf); NL=true; indent+=10; line_count = 0;}
static void BigOutdent() { if (!NL) fputc('\n',yyf); NL=true; indent-=10; line_count = 0;}
static void NewLine()    { if (!NL) fputc('\n',yyf); NL=true; line_count = 0;}
static void BlankLine()  { NewLine(); fputc('\n',yyf); }
static void ResetIndent() { NL=false; indent=0; }

#define CONDWrite(b,s)   { if (b) yyWrite(s); }
#define COND(b,s)        { if (b) s; }

#define MAX_CONSTS_LINE (20)

#define WriteInt(a)  {DoNL(); fprintf(yyf,"%d",(int)a);}

#define WriteReal(a) {DoNL(); fprintf(yyf,"%.*e",a.RNumber.p,a.RNumber.n);} 
#define WriteByte(a) {DoNL(); fprintf(yyf,"\'\\x%02x\'",(unsigned char)a);}
#define WriteBool(a) {DoNL(); fputs((a)?"true":"false",yyf);}
#define WriteStr(a)  {DoNL(); strlen(a); fputs(a,yyf);}
#define WriteChar(a)  {DoNL(); fputc((char)a,yyf);}

#define GenLabel()    WriteLabel(NewLabel())
#define NewLabel()    (++CurrentLabel)
#define WriteLabel(l) { /*Big*/Outdent(); DoNL(); fprintf(yyf,"CASE(%d):  ",(int)l); /*Big*/Indent(); }
#define WriteLabelValue(l) WriteInt(l)

#define yyWrite(a)   {DoNL(); fputs(a,yyf);}

#define WriteId(s)    {DoNL(); WriteIdent(yyf,s);}  

#define WarnNotImp(P,S)    ErrorMessageI(xxNotImplemented,xxWarning,P,xxString,S)
#define WNYI(S,P)          MessageI("C transformation not implemented for ",xxError,P,xxString,Tree_NodeName[S->Kind])

#define WriteOccSym(s) {DoNL(); WriteIdent(yyf,Symbol(s).Id);}


/*{{{  void WriteSymSuffix(tSymbol s)*/
void WriteSymSuffix(tSymbol s)
{
  if (Tree_IsType(SymbolKind(s),kPrototypeFunc))
  {
    tIdent Id=SymbolKind(s)->PrototypeFunc.Module;
    if (Id!=NoIdent)
    {
      fputc('_',yyf);
      WriteIdent(yyf,Id);
    }
  }
  else if (Tree_IsType(SymbolKind(s),kPrototypeProc))
  {
    tIdent Id=SymbolKind(s)->PrototypeProc.Module;
    if (Id!=NoIdent)
    {
      fputc('_',yyf);
      WriteIdent(yyf,Id);
    }
  }
  else if (Symbol(s).Depth==0)
  {
    fprintf(yyf,"_%s",outname);
  }   
  else
    fprintf(yyf,"_%d",s);
}
/*}}}*/
/*{{{  void WriteStrRef(tStringRef r)*/
static void WriteStrRef(tStringRef r)
{
  #define IsPrintable(c) (((c)>=' ') && ((c)<='~'))
  char *temp ;
  int len = LengthSt(r) ;

  DoNL();
  if ((temp = (char*) malloc((len==0)?1:len+1)) == NULL)
    yyAbort("Out of heap") ;
  else
    {
      int i;
      
      StGetString (r,temp) ;

      for (i=0;i<len;i++)
      {
	char c = temp[i] ;

	if (c == '"')
	  fprintf(yyf,"\\\"") ;
	else if (c == '\\')
	  fprintf(yyf,"\\\\") ;
	else if (c == '\'')
	  fprintf(yyf,"\\'") ;
	else if (c == '\?')
	  fprintf(yyf,"\\?") ;
	else if (IsPrintable(c))
	  fputc(c,yyf) ;
	else if (c == 13)
	  fprintf(yyf,"\\r") ;
	else if (c == 10)
	  fprintf(yyf,"\\n") ;
	else if (c == '\t')
	  fprintf(yyf,"\\t") ;
	else
	  fprintf(yyf,"\\x%02x",c) ;
      }
      free(temp) ;
    }
}
/*}}}*/
/*{{{  void WriteStaticSym(tSymbol sym)*/
void WriteStaticSym(tSymbol sym)
{
  tStringRef r = GetStringRef (Symbol(sym).CId);
  char *temp ;
  int len = LengthSt(r) ;

  DoNL() ;

  if ((temp = (char*) malloc((len==0)?1:len+1)) == NULL)
    yyAbort("Out of heap") ;
  else
    {
      int i;
      
      StGetString (r,temp) ;

      for (i=0;i<len;i++)
      {
	char c = temp[i] ;

	if (c == '.')
	  fprintf(yyf,"_") ;
	else
	  fputc(c,yyf) ;
      }
      free(temp) ;
      WriteSymSuffix(sym);
    }
}
/*}}}*/
/*{{{  void WritetInt(tInt val,bool int64)*/
void WritetInt(tInt val,bool int64)
{
  char str[24];
  int len=0;
  bool hex=(val > 1000000);

#ifndef SUPPORT_INT64
  int64 = false;
#endif
  if (!IS_Mode(xxSupportINT64)) int64=false;

  /* need these to represent MOSTNEG INT correctly in C */
  if (val==MOSTNEGINT32) { fprintf(yyf,"MOSTNEGINT32"); return; }
  if (val==MOSTNEGINT64) { fprintf(yyf,"MOSTNEGINT64"); return; }

  if (hex)
  {
    int digits = (int64) ? 16 : 8;
    char hexdigits[] = "0123456789ABCDEF";

    while(len < digits) { str[len++] = hexdigits[val & 15]; val = val >> 4; }
    while(len>1 && str[len-1]=='0') --len;

    fprintf(yyf,"0x");
  }
  else
  {
    bool neg=(val < 0);
    tInt value= (neg)?-val:val;

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
  if (int64 && len>8) fprintf(yyf,"LL");

}
  
  
/*}}}*/
/*{{{  static void WriteSym(int s)*/
static void WriteSym(int s)
{
  DoNL(); 
  WriteIdent(yyf,Symbol(s).CId);
  WriteSymSuffix(s);
}
/*}}}*/

/*}}}*/
/*{{{  access to back list in IfType*/
#define BackIfType(I)     I->IfType.BackIfType
#define BackChoiceList(I) I->IfType.BackChoiceList
/*}}}*/
/*{{{  Specification handling */
#define TypeDepth(T) (T->Type.D)
#define IsTopLevel(S) (Symbol(S).Depth==0)

#define NeedsUnion(PL) (PL->ProcList.SLC > 1)
#define NeedsStruct(P) (P->Process.SpecList->SpecList.DCL > 0)

typedef struct
{
  tIdent Id;
  tIdent PrefixId;
  bool newSpec;
} tSpec;

static uint32_t SpecExtent = 0;
static tSpec *SpecTable = NULL;
static int SpecLevel;

#define CurSpec() SpecTable[SpecLevel]

/* OpenSSpec opens a structure scope if we are in a union and we
   need a new level. The current scope is then a union only if
   we were in a union before and we did not open a structure.  */

#define OpenSSpec(S,b) (!OpenSpec((S) && (b),"struct") && (S))

/* OpenUSpec opens a union scope if we are in a structure and we
   need a new level. The current scope is then a union if we 
   were in a union before or if we just opened a union.        */

#define OpenUSpec(S,b) (OpenSpec(!(S) && (b),"union") || (S))

void InitSpec()
{
  if (SpecExtent == 0)
  {
    SpecExtent = 20;
    MakeArray((char**)&SpecTable,&SpecExtent,sizeof(tSpec));
  }
  SpecLevel = 0;

  CurSpec().Id = NoIdent;
  CurSpec().PrefixId = NoIdent;
}

bool OpenSpec(bool b,char *name)
{
  tIdent id = CurSpec().Id;
  tIdent pfid = CurSpec().PrefixId;

  if (b)
  {
    id = UniqueId(toupper(name[0]));

    if (pfid)
    {
      pfid = PrefixId(pfid,id,'.');
    }
    else
      pfid = id;

    WriteStr(name); NewLine();
    WriteStr("{"); NewLine();
    Indent();
    output_text=false;
  }

  if (SpecLevel>=SpecExtent) ExtendArray((char**)&SpecTable,&SpecExtent,sizeof(tSpec));

  SpecLevel++;
  CurSpec().PrefixId = pfid;
  CurSpec().Id = id;
  CurSpec().newSpec = b;
  
  return(b);
}

void CloseSpec()
{
  if (CurSpec().newSpec)
  {
    if (!output_text) { WriteStr("INT _P_dummy;"); NewLine(); }
    Outdent();
    WriteStr("} ");
    WriteId(CurSpec().Id);
    WriteStr(";"); NewLine();
  }
  SpecLevel--;
}
/*}}}*/
/*{{{  Blocks*/
static uint32_t BlockExtent = 0;
static bool *BlockTable = NULL;
static int BlockLevel;

#define CurBlock BlockTable[BlockLevel]

void InitBlocks()
{
  if (BlockExtent == 0)
  {
    BlockExtent = 20;
    MakeArray((char**)&BlockTable,&BlockExtent,sizeof(bool));
  }
  BlockLevel = 0;
  indent = 0;
}

#define DoBlock() { if (!CurBlock) { CurBlock = true; WriteStr("{"); Indent(); NewLine(); }} 

void OpenBlock(bool b)
{
  if (BlockLevel>=BlockExtent) ExtendArray((char**)&BlockTable,&BlockExtent,sizeof(bool));

  BlockLevel++;
  CurBlock = b;
}

void CloseBlock()
{
  if (CurBlock)
  {
    Outdent();
    WriteStr("}");
    NewLine();
  }
  BlockLevel--;
}
/*}}}*/
/*{{{  static int CalcDepth(tSymbol ThisSym)*/
static int CalcDepth(tSymbol ThisSym)
{
  tSymbol S=CalcParent(SymbolKind(ThisSym));
  int D=0;
  if (S==NoSymbol) return 0;
  while (Symbol(S).Depth>0)
  {
    if (Tree_IsType(SymbolKind(S),kDefProc)) 
      S=SymbolKind(S)->DefProc.Parent;
    else if (Tree_IsType(SymbolKind(S),kPrototypeProc)) 
      S=SymbolKind(S)->PrototypeProc.Parent;
    else if (Tree_IsType(SymbolKind(S),kDefAFunc)) 
      S=SymbolKind(S)->DefAFunc.Parent;
    else
      Message("Parent is not PROC or FUNCTION",xxFatal,NoPosition);
    D++;
  }
  if ( Tree_IsType(SymbolKind(ThisSym),kDefProc) ||
       Tree_IsType(SymbolKind(ThisSym),kDefFunc) )
    D++;
  return D;
}
/*}}}*/
/*{{{  static void DoSubstitutions(FILE *inf,char *filename, tTree Root)*/
static void DoSubstitutions(FILE *inf,tTree Root)
{  
  DoNL();

  while(!feof(inf))
  {
    char *bp, buff[128];
    
    bp = fgets(buff,128,inf);
    if (bp == NULL) break;
    while(*bp)
    {
      if (strncmp(bp,M_MAIN,S_MAIN)==0)
	/*{{{  insert OuterProc name*/
	{
	  bp += S_MAIN;
	  if (!IS_Mode(xxLibrary))
	  {
	    WriteSym(TreeRoot->Start.OuterProc);
	  }
	}
	/*}}}*/
      else if (strncmp(bp,M_PARAMETERS,S_PARAMETERS)==0)
	/*{{{  insert parameters to outermost proc*/
	{
	  bp += S_PARAMETERS;
	  if (!IS_Mode(xxLibrary))
	  {
	    Indent();
	    CodeMainEntryParameters(SymbolKind(TreeRoot->Start.OuterProc));
	    Outdent();
	  }
	}
	/*}}}*/
      else if (strncmp(bp,M_SWITCHES,S_SWITCHES)==0)
	/*{{{  insert selected switches*/
	{
	  int i;
	  
	  bp += S_SWITCHES;
	
	  if (IS_Mode(xxLibrary)) { WriteStr("#define LIBRARY_UNIT"); NewLine(); }
#if __SIZEOF_POINTER__==8
	  WriteStr("#define BYTES_PER_WORD 8"); NewLine();
#else
	  WriteStr("#define BYTES_PER_WORD 4"); NewLine();
#endif
	
	  if (!IS_Mode(xxSupportINT64)) { WriteStr("#define NO_INT64"); NewLine(); }
	
	  WriteStr("#define ");WriteStr(ERR_MODES[ErrorMode]);NewLine();
	
	  for (i=xxCheckConversions;i<=xxCheckRanges;i++)
	    if (IS_Mode(i)) { WriteStr("#define ");WriteStr(CHK_SWITCH[i-xxCheckConversions]); NewLine(); }
	
	  if (!IS_Mode(xxOccamDebug)) { WriteStr("#define NO_DEBUG"); NewLine(); }
	  WriteStr("#define FASTGOTOS"); NewLine();
	  if (IS_Mode(xxSimulatedServer) && !IS_Mode(xxLibrary)) 
	    if (IsASEntry(SymbolKind(TreeRoot->Start.OuterProc)))
	      { WriteStr("#define USE_SPOC_ASERVER"); NewLine(); }
	    else
	      { WriteStr("#define USE_SPOC_ISERVER"); NewLine(); }    
	  if (IS_Mode(xxLittleEndian)) 
	    { WriteStr("#define SOURCE LITTLE"); NewLine(); }
	  else 
	    { WriteStr("#define SOURCE BIG"); NewLine(); }
	  if (IS_Mode(xxCallF90)) { WriteStr("#define CALL_NAG_F90"); NewLine(); }
	}
	/*}}}*/
      else if (strncmp(bp,M_NAME,S_NAME)==0)
	/*{{{  insert name*/
	{
	  bp += S_NAME;
	  WriteStr(outname);
	}
	/*}}}*/
      else if (strncmp(bp,M_HEADER,S_HEADER) == 0)
	/*{{{  insert structure declarations*/
	{
	  tTree T;
	
	  bp += S_HEADER;
	  
	  T = Root->Start.CGNext;
	  while(T != NoTree)
	    T = GenTypeDef(T);
	
	  T = Root->Start.CGNext;
	  while(T != NoTree)
	  {
	    InitSpec();
	    T = GenDeclStruct(T);
	  }
	  GenStatics(Root);
	}        
	
	/*}}}*/
      else if (strncmp(bp,M_CODE,S_CODE) == 0)
	/*{{{  insert generated code*/
	{
	  tTree T;
	
	  bp += S_CODE;
	  
	  T = Root->Start.CGNext;
	  while(T != NoTree)
	    T = GenCode(T);
	}
	/*}}}*/
      else if (strncmp(bp,M_PROTOTYPES,S_PROTOTYPES)==0)
	/*{{{  insert prototypes*/
	{ 
	  tTree T;
		
	  bp += S_PROTOTYPES;
	  if (IS_Mode(xxLibrary))
	  {
	    Indent();
	    T = Root->Start.CGNext;
	    while(T != NoTree)
	    {
	      T = GenCPrototype(T);
	    }
	  }
	  Outdent();
	}
	/*}}}*/
      else if (strncmp(bp,M_DEBUG,S_DEBUG)==0)
	/*{{{  insert debug*/
	{
	  tSymbol S;
	  bp+= S_DEBUG;
	
	  if (IS_Mode(xxOccamDebug))
	  {
	    WriteStr("#ifndef NO_DEBUG");NewLine();
	    WriteStr("#undef SETSCOPE");NewLine();
	    WriteStr("#define NUMSYMS ");WriteInt(OccSymSize);NewLine();
	    /*{{{  generate Symbols table*/
	    WriteStr("static char *Symbols[NUMSYMS]={\"");
	    for (S=1;S<=OccSymSize;S++) 
	    {
	      WriteOccSym(S);
	      if (S==OccSymSize) 
		WriteStr("\"};")
	      else
		WriteStr("\",\"");
	    } NewLine();
	    /*}}}*/
	    /*{{{  generate Kinds table*/
	    WriteStr("static int Kinds[NUMSYMS]={");
	    for (S=1;S<=OccSymSize;S++) 
	    {
	     
	      WriteInt(MapKinds(SymbolKind(S),SymbolType(S),S));
	      if (S==OccSymSize) 
		WriteStr("};")
	      else
		WriteStr(",");
	    } NewLine();
	    /*}}}*/
	    /*{{{  generate Scopes table*/
	    WriteStr("static int Scopes[NUMSYMS]={");
	    for (S=1;S<=OccSymSize;S++) 
	    {
	      WriteInt(Symbol(S).Scope-1);
	      if (S==OccSymSize) 
		WriteStr("};")
	      else
		WriteStr(",");
	    } NewLine();
	    /*}}}*/
	    /*{{{  generate Depths table*/
	    WriteStr("static int Depths[NUMSYMS]={");
	    for (S=1;S<=OccSymSize;S++) 
	    {
	      WriteInt(CalcDepth(S));
	      if (S==OccSymSize) 
		WriteStr("};")
	      else
		WriteStr(",");
	    } NewLine();
	    /*}}}*/
	    /*{{{  generate Types table*/
	    WriteStr("static char *Types[NUMSYMS]={\"");
	    for (S=1;S<=OccSymSize;S++) 
	    {
	      OccCodeType(SymbolType(S));
	      if (S==OccSymSize) 
		WriteStr("\"};")
	      else
		WriteStr("\",\"");
	    } NewLine();
	    /*}}}*/
	    WriteStr("#include \"Debug.h\"");NewLine();
	    /*{{{  ShowValue function*/
	    WriteStr("static void *ShowValue(void *FP, int S)"); NewLine();
	    WriteStr("{ switch(S)");Indent();
	    WriteStr("{"); Indent();
	    for (S=1;S<=OccSymSize;S++) 
	    {
	      WriteStr("case "); WriteInt(S-1); WriteStr(": "); 
	      if (MapKinds(SymbolKind(S),SymbolType(S),S)>0 && Symbol(S).Id!=NoIdent) GenShowValue(SymbolKind(S),SymbolType(S),S);
	      WriteStr(" break;");
	      NewLine();
	    } 
	    Outdent(); WriteStr("}");
	    Outdent(); WriteStr("}"); NewLine();  
	    /*}}}*/
	    /*{{{  DEBUGOCCAM function*/
	    WriteStr("static void DEBUGOCCAM()");NewLine();
	    WriteStr("{");Indent();
	    WriteStr("Show(MODE,CFP);");NewLine();
	    WriteStr("DEBUGDONE();");Outdent();
	    WriteStr("}");NewLine();
	    /*}}}*/
	    WriteStr("#endif");NewLine();
	  }
	}
	/*}}}*/
      else
	do fputc(*bp++,yyf); while(*bp && *bp!='<');
    }
  }
  
  if (ferror(inf)||fclose(inf))
    Message("Error reading template file",xxFatal,NoPosition);
}

void DoGenCodeOnly(tTree Root)
{
  tTree T;

  InitSpec();
  T = Root->Start.CGNext;
  while(T != NoTree)
    T = GenCode(T);
}

/*}}}*/
/*{{{  static FILE *OpenTemplate(char *name)*/
static FILE *OpenTemplate(char *name)
{
  return(fopenenv(name,"r",OSEARCH,ODEFAULT_LIB,NULL));
}
/*}}}*/
/*{{{  void OutputC(f,t,createc)*/
void OutputC(char *name,tTree Root)
{
  FILE *savef = yyf;
  FILE *inf;
  char *template="Occam2C.c";

  inf = OpenTemplate(template);
  if (inf == NULL) MessageI("Cannot open file ",xxFatal,NoPosition,xxString,template);
  
  yyf = stdout;
  if (strcmp(name,"-") != 0)
  {
    char *dot = strrchr(name,'.');
    char *slash = strrchr(name,'/');

    if (slash == NULL) slash = strrchr(name,'\\');

    if ((yyf=fopen(name,"w"))==NULL) MessageI("Cannot open file ",xxFatal,NoPosition,xxString,name);
    if (dot != NULL) *dot=0;
    if (slash == NULL) outname = name;
    else outname = ++slash;
  }
  else
    outname = "";

  if (!IS_Mode(xxLibrary))
  {
    if (Tree_IsType(Symbol(Root->Start.OuterProc).Kind,kDefProc))
      Symbol(Root->Start.OuterProc).Kind->DefProc.isPar = true;
    else
      Message("Outermost subroutine must be a PROC",xxFatal,Symbol(Root->Start.OuterProc).Kind->DefProc.Pos);
  }
  ResetIndent();
  if (cgdebug)  DoGenCodeOnly(Root);
  else DoSubstitutions(inf,Root);
  if(ferror(yyf))
    Message("Error writing output file",xxFatal, NoPosition);
  if (yyf != stdout) 
    if(fclose(yyf))
      Message("Error writing output file",xxFatal, NoPosition);
  if (IS_Mode(xxLibrary))
  {
    tTree T;
    char *tname = (char *)malloc(strlen(outname)+10);

    if (!tname) Message("Out of Heap",xxFatal,NoPosition);
    strcpy(tname,outname); strcat(tname,".t");
    if ((yyf=fopen(tname,"w"))==NULL) MessageI("Cannot open file ",xxFatal,NoPosition,xxString,tname);

    SetPosition(NoPosition);
    ResetIndent();
    T = Root->Start.CGNext;
    while(T != NoTree)
    {
      T = GenOccPrototypes(T);
    }

    if(ferror(yyf)||fclose(yyf))  
      Message("Error writing output file",xxFatal, NoPosition);  

  }
  /*{{{  if (cgdebug)*/
  if (cgdebug)
  {
    tTree T;
	  
    yyf = stdout;
  
    T = Root->Start.CGNext;
    fprintf(yyf,"\nCodeGenerator Debug\n");
  
    while(T != NoTree)
    {
      T = GenCGDebug(T);
    }
  }
  /*}}}*/
  yyf = savef;
}
/*}}}*/
/*{{{  void WriteCCode(tStringRef S, tPosition P, int scope)*/
void WriteCCode(tStringRef S, tPosition P, int scope)
{
  int l = LengthSt(S);
  int i=0;
  char *str = (char*) malloc((l==0)?1:l+1);
  StGetString(S,str);
  while (i<l)
  {
    if (str[i]=='$')
    {
      tSymbol Sym;
      int start = ++i;
      while (isalnum(str[i])||(str[i]=='.')) i++;
      Sym = LookupSymbolInScope(MakeIdent(&str[start],i-start),P,scope);
      AddVUsage(Sym,xxLRead);
      CShowSym(Symbol(Sym).Kind, Sym);
    }
    else 
      WriteChar(str[i++]);
  }
  free(str);
}
/*}}}*/
/*{{{  void MarkCCodeUsage(tStringRef S, tPosition P, int scope)*/
void MarkCCodeUsage(tStringRef S, tPosition P, int scope)
{
  int l = LengthSt(S);
  int i=0;
  char *str = (char*) malloc((l==0)?1:l+1);
  StGetString(S,str);
  while (i<l)
  {
    if (str[i]=='$')
    {
      tSymbol Sym;
      int start = ++i;
      while (isalnum(str[i])||(str[i]=='.')) i++;
      Sym = LookupSymbolInScope(MakeIdent(&str[start],i-start),P,scope);
      AddVUsage(Sym,xxLRead);
    }
    else ++i;
  }
  free(str);
}
/*}}}*/
/*{{{  Priority Defs.*/
#define HIPRI 0
#define LOPRI 1
/*}}}*/
/*{{{  static bool IsDuplicatedPrototype(tSymbol ThisSym)*/
static bool IsDuplicatedPrototype(tSymbol ThisSym)
{
  tSymbol S=1;
  tIdent M=CalcModule(SymbolKind(ThisSym));
  while (S<=ThisSym)
  {
    if ((M!=NoIdent) && (SymbolKind(S)!=NULL) && 
	(Symbol(S).Id==Symbol(ThisSym).Id) && 
	(CalcModule(SymbolKind(S))==M) &&
	(CalcUsage(SymbolKind(S))==xxUsed))
      break;
    S++;
  }
  return (S!=ThisSym);
}

/*}}}*/
/*{{{  static bool IsDuplicatedHeader(tSymbol ThisSym)*/
static bool IsDuplicatedHeader(tSymbol ThisSym)
{
  tSymbol S=1;
  tStringRef H=CalcHeader(SymbolKind(ThisSym));
  while (S<=ThisSym)
  {
    if ((SymbolKind(S)!=NULL) && IsPrototype(SymbolKind(S)) &&
	(CalcHeader(SymbolKind(S))==H) &&
	(CalcUsage(SymbolKind(S))==xxUsed))
      break;
    S++;
  }
  return (S!=ThisSym);
}

/*}}}*/
/*{{{  static void WriteModuleHeaderName(tStringRef St)*/
static void WriteModuleHeaderName(tStringRef St)
{
  int len = LengthSt(St);
  char *temp = (char*) malloc(len+5);

  if (temp==NULL)
      Message("Out of heap",xxFatal,NoPosition) ;
  else
  {
    int i = len-1;
    StGetString (St,temp);
    
    while(i>1 && temp[i]!='.') --i;
    if (i) temp[i] = 0;
    strcat(temp,".hdr");

    WriteStr(temp);
  }
}
/*}}}*/


static void yyExit () { Exit (1); }

void (* Cout_Exit) () = yyExit;

extern FILE *yyf;

static void yyAbort
# ifdef __cplusplus
 (char * yyFunction)
# else
 (yyFunction) char * yyFunction;
# endif
{
 (void) fprintf (stderr, "Error: module Cout, routine %s failed\n", yyFunction);
 Cout_Exit ();
}

tTree GenTypeDef ARGS((tTree T));
static void GenWS ARGS((tSymbol yyP1));
static void GenProtoFormals ARGS((tTree yyP3, int yyP2));
static void GenProtoArrayFormal ARGS((tTree yyP5, int yyP4));
tTree GenDeclStruct ARGS((tTree T));
static void GenParentRef ARGS((tTree yyP7, tSymbol yyP6));
static void GenDeclFormals ARGS((tTree yyP9, bool yyP8));
static void GenDeclArrayFormal ARGS((tTree yyP12, tSymbol yyP11, bool yyP10));
static void GenDeclProc ARGS((tTree P, bool yyP13));
static void GenDeclList ARGS((tTree T, bool yyP14));
static void GenDeclGuard ARGS((tTree yyP16, bool yyP15));
static void GenDeclAltTypeOuter ARGS((tTree yyP18, bool yyP17));
static void GenDeclAltTypeInner ARGS((tTree yyP19));
static void GenDeclAltList ARGS((tTree yyP20));
static void GenDeclAltRep ARGS((tTree yyP21));
static void GenNLRepState ARGS((tTree yyP22));
static void GenDeclSpecs ARGS((tTree yyP23));
static void GenDeclNLSpec ARGS((tTree yyP24));
static void GenDeclSym ARGS((tTree yyP27, tSymbol yyP26, bool yyP25));
static void GenDeclArraySym ARGS((tTree yyP29, tSymbol yyP28));
static bool IsZeroSizeType ARGS((tTree yyP30));
static bool NeedsSpecs ARGS((tTree yyP31));
static bool NeedsSplit ARGS((tTree yyP32));
static void GenDeclNLIdList ARGS((tTree yyP34, tTree yyP33));
static void GenSpecState ARGS((tTree yyP35));
int CountRepDecls ARGS((tTree yyP36));
int CountAltRepDecls ARGS((tTree yyP37));
static int CountAbbrev ARGS((tTree yyP38));
static int CountIdList ARGS((tTree yyP39));
static int CountDecl ARGS((tTree yyP40));
int CountSpec ARGS((tTree yyP41));
tTree GenCode ARGS((tTree T));
static void GenProcHead ARGS((tTree DP));
static void GenProcEntry ARGS((tTree DP));
static void GenProcTail ARGS((tTree DP));
static void GenFuncHead ARGS((tTree DF));
static void GenFuncEntry ARGS((tTree DF));
static void GenFuncExit ARGS((tTree DF));
static void GenFuncTail ARGS((tTree DF));
static bool GenFormals ARGS((tTree yyP43, bool yyP42));
static void GenArrayFormal ARGS((tTree yyP46, tSymbol yyP45, bool yyP44));
static void GenNLFormals ARGS((tTree yyP47));
static void GenNLArrayFormal ARGS((tTree yyP49, tSymbol yyP48));
static bool AnyNLFormals ARGS((tTree yyP50));
static void GenMVFormals ARGS((tTree yyP53, int yyP52, bool yyP51));
static void GenMVFResult ARGS((tTree yyP55, int yyP54));
static bool NeedsChain ARGS((tTree yyP56));
static bool UsesFreeVars ARGS((tTree yyP57));
static bool HasReadUsage ARGS((tParUsage yyP58));
static bool HasWriteUsage ARGS((tParUsage yyP59));
static void GenProcess ARGS((tTree yyP60));
static void GenProcList ARGS((tTree yyP61));
static void GenParList ARGS((tTree yyP63, int yyP62));
static void GenParProc ARGS((tTree yyP67, tSymbol yyP66, int yyP65, tTree yyP64));
static void GenPlacedList ARGS((tTree yyP70, tSymbol yyP69, tTree yyP68));
static void GenPlaced ARGS((tTree yyP73, tSymbol yyP72, tTree yyP71));
static void CodePlacedExpr ARGS((tTree EX, tSymbol yyP75, tTree yyP74));
static void GenParProcParam ARGS((tTree yyP80, tTree yyP79, tIdent yyP78, tSymbol yyP77, tTree yyP76));
static void GenEnableAltType ARGS((tTree yyP81));
static void GenEnableAltList ARGS((tTree yyP82));
static void GenEnableGuard ARGS((tTree yyP83));
static void GenDisableAltType ARGS((tTree yyP84));
static void GenDisableAltList ARGS((tTree yyP85));
static void GenDisableGuard ARGS((tTree yyP87, int yyP86));
static void GenAssignRepsAltType ARGS((tTree yyP88));
static void GenAssignRepsAltList ARGS((tTree yyP89));
static tTree BuildRevSpecList ARGS((tTree yyP91, tTree yyP90));
static void GenCodeAltType ARGS((tTree yyP94, int yyP93, tTree yyP92));
static void GenCodeAltList ARGS((tTree yyP97, int yyP96, tTree yyP95));
static void GenCodeGuard ARGS((tTree yyP98));
static void GenIfType ARGS((tTree yyP100, int yyP99));
static void GenIfList ARGS((tTree yyP102, int yyP101));
static void GenIfTypeAtomic ARGS((tTree yyP104, bool yyP103));
static void GenIfListAtomic ARGS((tTree yyP107, tTree yyP106, bool yyP105));
static void GenCaseSwitches ARGS((tTree yyP109, bool yyP108));
static void GenCaseExprList ARGS((tTree yyP110));
static void GenCaseCode ARGS((tTree yyP112, int yyP111));
static void GenCaseAtomic ARGS((tTree yyP114, bool yyP113));
static void GenCaseInSwitches ARGS((tTree yyP115));
static void GenCaseInCode ARGS((tTree yyP118, tTree yyP117, int yyP116));
static void GenSetScope ARGS((tSymbol yyP119));
static void GenSetListScope ARGS((tTree yyP120));
static void GenLocalSpecs ARGS((tTree yyP121));
static void GenLocalSpec ARGS((tTree SP));
static void GenLocalRep ARGS((tTree yyP122));
static void GenLocalIdList ARGS((tTree yyP124, tTree yyP123));
static void GenLocalInits ARGS((tTree yyP125));
static void GenLocalInit ARGS((tTree SP));
static void GenInitUserType ARGS((tSymbol yyP128, tTree yyP127, tTree yyP126));
static void GenInitTable ARGS((tSymbol yyP131, int yyP130, tTree yyP129));
static void GenArrayTypeSize ARGS((tTree yyP132));
static void GenEmuRetype ARGS((tTree yyP136, tSymbol yyP135, tTree yyP134, tTree yyP133));
static void CodeRetypeSize ARGS((tTree yyP139, tSymbol yyP138, int yyP137));
static int RetypeSize ARGS((tTree yyP140));
static void GenInitChanList ARGS((tTree yyP141));
static void GenInitChanSubs ARGS((int yyP143, int yyP142));
static void GenInitChanArray ARGS((tTree yyP146, tSymbol yyP145, int yyP144));
static void GenInitChanArrayList ARGS((tTree yyP148, tTree yyP147));
static void GenInitIsArray ARGS((tSymbol yyP153, tTree yyP152, tTree yyP151, tSymbol yyP150, tTree yyP149));
static void GenInitReArray ARGS((tSymbol yyP158, tTree yyP157, tTree yyP156, tSymbol yyP155, tTree yyP154));
static void GenInitMissingDimensions ARGS((tSymbol yyP162, tTree yyP161, tTree yyP160, tTree yyP159));
static void GenArrayUnBoundSize ARGS((tTree yyP163));
static void GenResults ARGS((tTree yyP164));
static void GenResult ARGS((tTree yyP165));
static void GenResultTable ARGS((tSymbol yyP168, int yyP167, tTree yyP166));
static void GenEmuRetypeResult ARGS((tTree yyP172, tSymbol yyP171, tTree yyP170, tTree yyP169));
static int GenRepStart ARGS((tTree yyP173));
static void GenRepEnd ARGS((tTree yyP176, int yyP175, int yyP174));
static void GenAltIndex ARGS((tTree yyP177));
static void GenAltSelect ARGS((tTree yyP178));
static void GenForLoop ARGS((tTree yyP179));
static void GenForCheck ARGS((tTree yyP180));
static void CodeArrayExpr ARGS((tTree yyP182, bool yyP181));
static void CodeExpr ARGS((tTree yyP183));
static void CodeSizeOp ARGS((tTree yyP185, tTree yyP184));
static void CodeConstExpr ARGS((tTree yyP187, tValue yyP186));
static void CodeAssign ARGS((tTree yyP189, tTree yyP188));
static void CodeAssign2 ARGS((tTree yyP191, tTree yyP190));
static void GenChkDims ARGS((tTree yyP195, tTree yyP194, tTree yyP193, tTree yyP192));
static void GenInstance ARGS((tTree DP, tTree yyP197, tIdent yyP196));
static void GenAtomicInstance ARGS((tSymbol yyP200, tTree yyP199, tTree yyP198));
static void GenNonAtomicInstance ARGS((tSymbol yyP204, tTree yyP203, tTree yyP202, tIdent yyP201));
static void GenCInstance ARGS((tSymbol yyP207, tTree yyP206, tTree yyP205));
static void GenCTemps ARGS((tTree yyP210, tTree yyP209, bool yyP208));
static void GenCParams ARGS((tTree yyP213, tTree yyP212, bool yyP211));
static void GenCResults ARGS((tTree yyP216, tTree yyP215, bool yyP214));
static void GenF90Instance ARGS((tSymbol yyP219, tTree yyP218, tTree yyP217));
static void GenF90Temps ARGS((tTree yyP222, tTree yyP221, int yyP220));
static void GenF90Inits ARGS((tTree yyP225, tTree yyP224, int yyP223));
static void GenF90DopeInits ARGS((tTree yyP230, tTree yyP229, tTree yyP228, int yyP227, int yyP226));
static void GenF90Params ARGS((tTree yyP234, tTree yyP233, int yyP232, bool yyP231));
static void GenF90CharParams ARGS((tTree yyP237, tTree yyP236, int yyP235));
static void GenF90Results ARGS((tTree yyP240, tTree yyP239, int yyP238));
static void GenArrayDimSize ARGS((tTree yyP242, tTree yyP241));
static void GenExtParams ARGS((tTree yyP246, tTree yyP245, tIdent yyP244, int yyP243));
static void GenExtArrayParams ARGS((tTree yyP251, tTree yyP250, tTree yyP249, tIdent yyP248, int yyP247));
static void GenIntParams ARGS((tTree yyP255, tTree yyP254, tIdent yyP253, tSymbol yyP252));
static void GenChain ARGS((tIdent yyP258, tSymbol yyP257, bool yyP256));
static void GenIntArrayParams ARGS((tTree yyP264, tTree yyP263, tSymbol yyP262, tTree yyP261, tIdent yyP260, tSymbol yyP259));
static void GenFrameRef ARGS((tIdent yyP266, tSymbol yyP265));
static void GenFuncCall ARGS((tTree yyP269, tTree yyP268, tTree yyP267));
static void CodeMVFParams ARGS((tTree yyP272, bool yyP271, int yyP270));
static void CodeMVFTemps ARGS((tTree yyP274, int yyP273));
static void CodeMVFTempCopies ARGS((tTree yyP276, int yyP275));
static void GenParams ARGS((tTree yyP279, tTree yyP278, bool yyP277));
static void GenArrayParams ARGS((tTree yyP283, tTree yyP282, tTree yyP281, bool yyP280));
static void GenComma ARGS((bool yyP284));
static void GenTimerInput ARGS((tTree yyP285));
static void GenInput ARGS((tTree yyP287, tTree T, tTree yyP286));
static void GenInputToExpr ARGS((tTree yyP289, tTree yyP288));
static void GenOutput ARGS((tTree yyP292, tTree yyP291, tTree yyP290));
static void GenSeqProOutput ARGS((tTree yyP295, tTree yyP294, tTree yyP293));
static void GenSimpleOutput ARGS((tTree yyP298, tTree yyP297, tTree yyP296));
static void GenSimpleOutputOfExpr ARGS((tTree yyP301, tTree yyP300, tTree yyP299));
static void CodeElement ARGS((tTree yyP303, bool yyP302));
static void CodeChannelElement ARGS((tTree yyP305, bool yyP304));
static void GenArrayIndex ARGS((tSymbol yyP308, tTree yyP307, tTree yyP306));
static void GenElementSize ARGS((tTree yyP310, tTree yyP309));
static void GenArraySize ARGS((tSymbol yyP312, tTree yyP311));
static void CodeChkExpr ARGS((bool yyP317, bool yyP316, tTree yyP315, tSymbol yyP314, tTree yyP313));
static void CodeChkUBExpr ARGS((bool yyP323, bool yyP322, tTree yyP321, tTree yyP320, tSymbol yyP319, tTree yyP318));
static void CodeChkLBExpr ARGS((bool yyP325, tTree yyP324));
static tSymbol GetElementSymbol ARGS((tTree yyP326));
void CodeSymRef ARGS((tTree yyP329, tSymbol yyP328, bool yyP327));
static void CodeSym ARGS((tSymbol yyP330));
static void CodeIndirect ARGS((tSymbol yyP332, tTree yyP331));
static void CCodeTypeName ARGS((tTree yyP334, bool yyP333));
static void CCodeType ARGS((tTree yyP335));
static void CCodeTypeId ARGS((tTree yyP336));
static void CCodeTypeIdChkConst ARGS((tTree yyP338, tPosition yyP337));
void OccCodeType ARGS((tTree yyP339));
static void OccCodeProtocol ARGS((tTree yyP340));
static void CodeOp ARGS((tTree yyP341));
static int PrimTypeSize ARGS((tTree yyP342));
static tTree GetPrimType ARGS((tTree yyP343));
static bool IsPrimType ARGS((tTree yyP344));
static bool IsTimer ARGS((tTree yyP345));
static bool IsAtomicProcess ARGS((tTree yyP346));
bool IsAtomicProc ARGS((tTree yyP347));
bool IsParProc ARGS((tTree yyP348));
bool IsPrototype ARGS((tTree yyP349));
static bool IsBoundedType ARGS((tTree yyP350));
static bool IsByteArray ARGS((tTree yyP351));
static int CountArrayDims ARGS((tTree yyP352));
tTree GenOccPrototypes ARGS((tTree yyP353));
static void GenOccPrototypeFormals ARGS((tTree yyP354));
tTree GenCPrototype ARGS((tTree yyP355));
tTree GenCGDebug ARGS((tTree yyP356));
bool IsASEntry ARGS((tTree yyP357));
void CodeMainEntryParameters ARGS((tTree yyP358));
static bool IsSPChan ARGS((tTree yyP359));
static bool IsASChan ARGS((tTree yyP360));
void GenStatics ARGS((tTree yyP361));
static void GenStatic ARGS((tTree yyP362));
static void CodeConstArray ARGS((tTree yyP364, tConstArray yyP363));
static void GenStaticExpr ARGS((tTree yyP365));
static void GenStaticElement ARGS((tTree yyP367, int yyP366));
static void GenStaticExprList ARGS((tTree yyP368));
void GenShowValue ARGS((tTree yyP371, tTree yyP370, tSymbol yyP369));
static void GenShowSizes ARGS((tTree yyP374, tSymbol yyP373, int yyP372));
static void GenShowTests ARGS((tTree yyP377, tSymbol yyP376, int yyP375));
static void GenShowSymRef ARGS((tTree yyP379, tSymbol yyP378));
static void GenShowSym ARGS((tTree yyP381, tSymbol yyP380));
void CShowSym ARGS((tTree yyP383, tSymbol yyP382));
int MapKinds ARGS((tTree yyP386, tTree yyP385, tSymbol yyP384));
tSymbol CalcParent ARGS((tTree T));
tIdent CalcModule ARGS((tTree yyP387));
tStringRef CalcHeader ARGS((tTree yyP388));
tUsage CalcUsage ARGS((tTree yyP389));
static bool IsInlined ARGS((tTree yyP390));
static bool IsFieldExpr ARGS((tTree yyP391));
static int GetFieldOffset ARGS((tTree yyP393, tTree yyP392));
bool IsLocal ARGS((tSymbol yyP394));
bool IsNonLocal ARGS((tSymbol yyP395));
bool IsAtomic ARGS((tTree yyP396));
static bool IsMVFunction ARGS((tTree yyP397));
bool IsSTVFunction ARGS((tTree yyP398));
static bool IsNewSymbol ARGS((tSymbol yyP399));
static void BeginBlock ARGS((tTree yyP400));
static void EndBlock ARGS((tTree yyP401));
static void BeginAtomicBlock ARGS((tTree yyP404, tTree yyP403, bool yyP402));
static void EndAtomicBlock ARGS((tTree yyP405));
void MarkRefParent ARGS((tSymbol yyP408, tTree yyP407, tVUsage yyP406));

tTree GenTypeDef
# if defined __STDC__ | defined __cplusplus
(register tTree T)
# else
(T)
 register tTree T;
# endif
{
  if (T->Kind == kStart) {
/* line 944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  }
  if (Tree_IsType (T, kDefProc)) {
/* line 946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("typedef struct SF_");
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefProc.Sym);
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" tSF_");
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefProc.Sym);
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TreeRoot -> Start . OuterProc != T->DefProc.Sym))) goto yyL2;
  }
   return T -> DefProc . CGNext;
yyL2:;

/* line 951 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcHead (T);
/* line 952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return T -> DefProc . CGNext;

  }
  if (Tree_IsType (T, kDefFunc)) {
/* line 955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("typedef struct SF_");
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefFunc.Sym);
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" tSF_");
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefFunc.Sym);
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 956 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 957 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TreeRoot -> Start . OuterProc != T->DefFunc.Sym))) goto yyL4;
  }
   return T -> DefFunc . CGNext;
yyL4:;

/* line 960 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncHead (T);
/* line 961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return T -> DefFunc . CGNext;

  }
  if (T->Kind == kPrototypeProc) {
/* line 964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsDuplicatedPrototype (T->PrototypeProc.Sym) || CalcUsage (T) != xxUsed))) goto yyL6;
  }
   return T -> PrototypeProc . CGNext;
yyL6:;

/* line 966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsDuplicatedHeader (T->PrototypeProc.Sym)))) goto yyL7;
  {
/* line 967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("#include \"");
/* line 967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteModuleHeaderName (T->PrototypeProc.Pos . File);
/* line 967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\"");
/* line 967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL7;
  }
  }
   return T -> PrototypeProc . CGNext;
yyL7:;

/* line 971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((T -> DefAProc . U == xxUsed))) goto yyL8;
  {
/* line 971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsAtomicProc (T)))) goto yyL8;
  {
/* line 972 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("struct SF_");
/* line 972 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->PrototypeProc.Sym);
/* line 972 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tHeader _Header;");
/* line 974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("void *Chain;");
/* line 975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (T->PrototypeProc.Formals, 0);
/* line 976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenWS (T->PrototypeProc.Sym);
/* line 977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("};");
/* line 978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return T -> PrototypeProc . CGNext;
yyL8:;

/* line 981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> PrototypeProc . CGNext;

  }
  if (T->Kind == kPrototypeFunc) {
/* line 983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsDuplicatedPrototype (T->PrototypeFunc.Sym) || CalcUsage (T) != xxUsed))) goto yyL10;
  }
   return T -> PrototypeFunc . CGNext;
yyL10:;

/* line 985 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 985 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsDuplicatedHeader (T->PrototypeFunc.Sym)))) goto yyL11;
  {
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("#include \"");
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteModuleHeaderName (T->PrototypeFunc.Pos . File);
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\"");
/* line 986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return T -> PrototypeFunc . CGNext;
yyL11:;

/* line 989 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> PrototypeFunc . CGNext;

  }
  if (T->Kind == kHCode) {
/* line 991 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> HCode . CGNext;

  }
 yyAbort ("GenTypeDef");
}

static void GenWS
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP1)
# else
(yyP1)
 tSymbol yyP1;
# endif
{
  {tSymbol yyT; yyT = 0;
  if (equaltSymbol (yyP1, yyT)) {
/* line 994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
/* line 995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("char WS[WS_");
/* line 995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP1);
/* line 995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("];");
/* line 995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenProtoFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP3, register int yyP2)
# else
(yyP3, yyP2)
 register tTree yyP3;
 register int yyP2;
# endif
{
  if (Tree_IsType (yyP3, kNoFormal)) {
/* line 999 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP3->Kind == kAFormal) {
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP3->AFormal.Type)))) goto yyL2;
  {
/* line 1000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (yyP3->AFormal.Next, yyP2 + 1);
  }
  }
   return;
yyL2:;

  if (Tree_IsType (yyP3->AFormal.Type, kArray)) {
/* line 1001 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1001 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoArrayFormal (yyP3->AFormal.Type, yyP2);
/* line 1001 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (yyP3->AFormal.Next, yyP2 + 1);
  }
   return;

  }
  if (Tree_IsType (yyP3->AFormal.Type, kUserStructTypes)) {
/* line 1002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoArrayFormal (yyP3->AFormal.Type, yyP2);
/* line 1002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (yyP3->AFormal.Next, yyP2 + 1);
  }
   return;

  }
  if (yyP3->AFormal.By->Kind == kByVal) {
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP3->AFormal.Type);
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP2);
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (yyP3->AFormal.Next, yyP2 + 1);
  }
   return;

  }
  if (yyP3->AFormal.By->Kind == kByRef) {
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP3->AFormal.Type);
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP2);
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1004 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoFormals (yyP3->AFormal.Next, yyP2 + 1);
  }
   return;

  }
  }
 yyAbort ("GenProtoFormals");
;
}

static void GenProtoArrayFormal
# if defined __STDC__ | defined __cplusplus
(register tTree yyP5, register int yyP4)
# else
(yyP5, yyP4)
 register tTree yyP5;
 register int yyP4;
# endif
{
  if (Tree_IsType (yyP5, kArray)) {
  if (yyP5->Array.Expr->Kind == kUnBound) {
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoArrayFormal (yyP5->Array.Type, yyP4);
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP4);
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP5));
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 1009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProtoArrayFormal (yyP5->Array.Type, yyP4);
  }
   return;

  }
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP5)))) goto yyL3;
  {
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP5);
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" **");
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP4);
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL3:;

/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP5);
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP4);
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

tTree GenDeclStruct
# if defined __STDC__ | defined __cplusplus
(register tTree T)
# else
(T)
 register tTree T;
# endif
{
  if (T->Kind == kStart) {
/* line 1018 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  }
  if (Tree_IsType (T, kDefProc)) {
/* line 1020 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("struct SF_");
/* line 1021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefProc.Sym);
/* line 1021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1023 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tHeader _Header;");
/* line 1023 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParentRef (T, T->DefProc.Sym);
/* line 1024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *Chain;");
/* line 1024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (T->DefProc.Formals, IsAtomicProc (T) && ! IsParProc (T));
/* line 1026 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->DefProc.Process, false);
/* line 1027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("};");
/* line 1027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return T -> DefProc . CGNext;

  }
  if (Tree_IsType (T, kDefFunc)) {
/* line 1030 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("struct SF_");
/* line 1031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (T->DefFunc.Sym);
/* line 1031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tHeader _Header;");
/* line 1033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1034 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParentRef (T, T->DefFunc.Sym);
/* line 1034 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *Chain;");
/* line 1034 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1035 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (T->DefFunc.Formals, true);
/* line 1036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->DefFunc.Valof->Valof.SpecList);
/* line 1037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->DefFunc.Valof->Valof.Process, false);
/* line 1038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("};");
/* line 1038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return T -> DefFunc . CGNext;

  }
  if (Tree_IsType (T, kDefAFunc)) {
/* line 1041 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefAFunc . CGNext;

  }
  if (Tree_IsType (T, kDefAProc)) {
/* line 1042 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefAProc . CGNext;

  }
  if (T->Kind == kHCode) {
/* line 1043 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> HCode . CGNext;

  }
 yyAbort ("GenDeclStruct");
}

static void GenParentRef
# if defined __STDC__ | defined __cplusplus
(register tTree yyP7, tSymbol yyP6)
# else
(yyP7, yyP6)
 register tTree yyP7;
 tSymbol yyP6;
# endif
{
  if (Tree_IsType (yyP7, kDefProc)) {
/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsTopLevel (yyP6)))) goto yyL1;
  {
/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP7 -> DefProc . Parent);
  }
  }
   return;
yyL1:;

  }
  if (Tree_IsType (yyP7, kDefFunc)) {
/* line 1047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsTopLevel (yyP6)))) goto yyL2;
  {
/* line 1047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP7 -> DefFunc . Parent);
  }
  }
   return;
yyL2:;

  }
/* line 1048 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1048 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("void");
  }
   return;

;
}

static void GenDeclFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP9, register bool yyP8)
# else
(yyP9, yyP8)
 register tTree yyP9;
 register bool yyP8;
# endif
{
  if (Tree_IsType (yyP9, kNoFormal)) {
/* line 1053 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP9->Kind == kAFormal) {
/* line 1055 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1055 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP9->AFormal.Type)))) goto yyL2;
  {
/* line 1056 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
  }
   return;
yyL2:;

/* line 1059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP9->AFormal.Sym) && yyP8))) goto yyL3;
  {
/* line 1060 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
  }
   return;
yyL3:;

  if (Tree_IsType (yyP9->AFormal.Type, kUserStructTypes)) {
/* line 1063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArrayFormal (yyP9->AFormal.Type, yyP9->AFormal.Sym, true);
/* line 1065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
   return;

  }
  if (Tree_IsType (yyP9->AFormal.Type, kArray)) {
/* line 1068 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArrayFormal (yyP9->AFormal.Type, yyP9->AFormal.Sym, true);
/* line 1070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
   return;

  }
  if (yyP9->AFormal.By->Kind == kByVal) {
/* line 1073 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP9->AFormal.Type);
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP9->AFormal.Sym);
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1074 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
   return;

  }
  if (yyP9->AFormal.By->Kind == kByRef) {
/* line 1078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP9->AFormal.Type);
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP9->AFormal.Sym);
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclFormals (yyP9->AFormal.Next, yyP8);
  }
   return;

  }
  }
 yyAbort ("GenDeclFormals");
;
}

static void GenDeclArrayFormal
# if defined __STDC__ | defined __cplusplus
(register tTree yyP12, tSymbol yyP11, register bool yyP10)
# else
(yyP12, yyP11, yyP10)
 register tTree yyP12;
 tSymbol yyP11;
 register bool yyP10;
# endif
{
  if (Tree_IsType (yyP12, kArray)) {
  if (yyP12->Array.Expr->Kind == kUnBound) {
  if (equalbool (yyP10, true)) {
/* line 1086 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArrayFormal (yyP12->Array.Type, yyP11, true);
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP11);
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP12));
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 1089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArrayFormal (yyP12->Array.Type, yyP11, false);
  }
   return;

  }
/* line 1092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1093 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArrayFormal (yyP12->Array.Type, yyP11, yyP10);
  }
   return;

  }
/* line 1095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP12)))) goto yyL4;
  {
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP12);
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" **");
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP11);
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

/* line 1098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP12);
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP11);
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenDeclProc
# if defined __STDC__ | defined __cplusplus
(register tTree P, register bool yyP13)
# else
(P, yyP13)
 register tTree P;
 register bool yyP13;
# endif
{

  switch (P->Kind) {
  case kAlt:
/* line 1108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->Alt.SpecList));
/* line 1110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Alt.SpecList);
/* line 1111 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltTypeOuter (P, yyP13);
/* line 1112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenUSpec (yyP13, NeedsSplit (P->Alt.AltList));
/* line 1113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->Alt.AltList, yyP13);
/* line 1114 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1115 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kAltRep:
/* line 1117 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->AltRep.SpecList));
/* line 1119 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->AltRep.SpecList);
/* line 1120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltTypeOuter (P, yyP13);
/* line 1121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->AltRep.AltList, yyP13);
/* line 1122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kCaseIn:
/* line 1124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->CaseIn.SpecList));
/* line 1126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->CaseIn.SpecList);
/* line 1127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenUSpec (yyP13, NeedsSplit (P->CaseIn.CaseList));
/* line 1128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->CaseIn.CaseList, yyP13);
/* line 1129 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kSeq:
/* line 1134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->Seq.SpecList));
/* line 1136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Seq.SpecList);
/* line 1137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenUSpec (yyP13, NeedsSplit (P->Seq.ProcList));
/* line 1138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->Seq.ProcList, yyP13);
/* line 1139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kSeqRep:
/* line 1142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1143 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->SeqRep.SpecList));
/* line 1144 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->SeqRep.SpecList);
/* line 1145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLRepState (P->SeqRep.Replicator);
/* line 1146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (P->SeqRep.Process, yyP13);
/* line 1147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kPar:
/* line 1149 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, (NeedsSpecs (P->Par.SpecList) || NeedsSplit (P->Par.ProcList)));
/* line 1151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Par.SpecList);
/* line 1152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->Par.ProcList, yyP13);
/* line 1153 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kParRep:
/* line 1155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->ParRep.SpecList));
/* line 1157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->ParRep.SpecList);
/* line 1158 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLRepState (P->ParRep.Replicator);
/* line 1159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (P->ParRep.Process, yyP13);
/* line 1160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kProcessor:
/* line 1162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1163 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->Processor.SpecList));
/* line 1164 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Processor.SpecList);
/* line 1165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (P->Processor.Process, yyP13);
/* line 1166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kIf:
/* line 1172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->If.SpecList));
/* line 1174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->If.SpecList);
/* line 1175 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenUSpec (yyP13, NeedsSplit (P->If.ChoiceList));
/* line 1176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->If.ChoiceList, yyP13);
/* line 1177 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kIfRep:
/* line 1180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->IfRep.SpecList));
/* line 1182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->IfRep.SpecList);
/* line 1183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLRepState (P->IfRep.Replicator);
/* line 1184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->IfRep.ChoiceList, yyP13);
/* line 1185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kCase:
/* line 1187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->Case.SpecList));
/* line 1189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Case.SpecList);
/* line 1190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenUSpec (yyP13, NeedsSplit (P->Case.ChoiceList));
/* line 1191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (P->Case.ChoiceList, yyP13);
/* line 1192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kWhile:
/* line 1195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, NeedsSpecs (P->While.SpecList));
/* line 1197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->While.SpecList);
/* line 1198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (P->While.Process, yyP13);
/* line 1199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  case kInstance:
/* line 1212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, P->Instance.SpecList -> SpecList . DCL > 1);
/* line 1214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Instance.SpecList);
/* line 1215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  }

/* line 1220 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP13 = OpenSSpec (yyP13, P->Process.SpecList -> SpecList . DCL > 1);
/* line 1222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (P->Process.SpecList);
/* line 1223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

;
}

static void GenDeclList
# if defined __STDC__ | defined __cplusplus
(register tTree T, register bool yyP14)
# else
(T, yyP14)
 register tTree T;
 register bool yyP14;
# endif
{
/* line 1231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool S2; 

  switch (T->Kind) {
  case kNoProcess:
/* line 1234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kAProcess:
/* line 1236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->AProcess.Process, yyP14);
/* line 1238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->AProcess.Next, yyP14);
  }
   return;

  case kNoAlt:
/* line 1242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kAnAlt:
/* line 1244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   S2 = OpenSSpec (yyP14, NeedsSpecs (T->AnAlt.SpecList));
/* line 1246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->AnAlt.SpecList);
/* line 1247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclGuard (T->AnAlt.Guard, S2);
/* line 1248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->AnAlt.Next, yyP14);
  }
   return;

  case kANewAlt:
/* line 1251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   S2 = OpenSSpec (yyP14, NeedsSpecs (T->ANewAlt.SpecList));
/* line 1253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->ANewAlt.SpecList);
/* line 1254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->ANewAlt.AltType, S2);
/* line 1255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->ANewAlt.Next, yyP14);
  }
   return;

  case kNoChoice:
/* line 1260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kAChoice:
/* line 1262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   S2 = OpenSSpec (yyP14, NeedsSpecs (T->AChoice.SpecList));
/* line 1264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->AChoice.SpecList);
/* line 1265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->AChoice.Process, S2);
/* line 1266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->AChoice.Next, yyP14);
  }
   return;

  case kANewIf:
/* line 1269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   S2 = OpenSSpec (yyP14, NeedsSpecs (T->ANewIf.SpecList));
/* line 1271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->ANewIf.SpecList);
/* line 1272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->ANewIf.IfType, S2);
/* line 1273 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->ANewIf.Next, yyP14);
  }
   return;

  case kNoCase:
/* line 1278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kACase:
/* line 1280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   S2 = OpenSSpec (yyP14, NeedsSpecs (T->ACase.SpecList));
/* line 1282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (T->ACase.SpecList);
/* line 1283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (T->ACase.Process, S2);
/* line 1284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
/* line 1285 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclList (T->ACase.Next, yyP14);
  }
   return;

  }

 yyAbort ("GenDeclList");
;
}

static void GenDeclGuard
# if defined __STDC__ | defined __cplusplus
(register tTree yyP16, register bool yyP15)
# else
(yyP16, yyP15)
 register tTree yyP16;
 register bool yyP15;
# endif
{
  if (yyP16->Kind == kAltInput) {
/* line 1293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP15 = OpenSSpec (yyP15, (NeedsSpecs (yyP16->AltInput.Input->Input.SpecList) || NeedsSplit (yyP16->AltInput.Process)));
/* line 1295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->AltInput.Input, yyP15);
/* line 1296 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->AltInput.Process, yyP15);
/* line 1297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  }
  if (yyP16->Kind == kAltTimer) {
/* line 1299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1300 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP15 = OpenSSpec (yyP15, (NeedsSpecs (yyP16->AltTimer.After->After.SpecList) || NeedsSplit (yyP16->AltTimer.Process)));
/* line 1301 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->AltTimer.After, yyP15);
/* line 1302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->AltTimer.Process, yyP15);
/* line 1303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
   return;

  }
  if (yyP16->Kind == kAltCaseIn) {
/* line 1305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->AltCaseIn.CaseIn, yyP15);
  }
   return;

  }
  if (yyP16->Kind == kNoInput) {
/* line 1308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclProc (yyP16->NoInput.Process, yyP15);
  }
   return;

  }
 yyAbort ("GenDeclGuard");
;
}

static void GenDeclAltTypeOuter
# if defined __STDC__ | defined __cplusplus
(register tTree yyP18, register bool yyP17)
# else
(yyP18, yyP17)
 register tTree yyP18;
 register bool yyP17;
# endif
{
/* line 1317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP18 -> AltType . TLA))) goto yyL1;
  {
/* line 1318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OpenSSpec (yyP17, (yyP18 -> AltType . ARC > 0));
/* line 1319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltTypeInner (yyP18);
/* line 1320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseSpec ();
  }
  }
   return;
yyL1:;

/* line 1321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenDeclAltTypeInner
# if defined __STDC__ | defined __cplusplus
(register tTree yyP19)
# else
(yyP19)
 register tTree yyP19;
# endif
{
  if (yyP19->Kind == kAlt) {
/* line 1326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltList (yyP19->Alt.AltList);
  }
   return;

  }
  if (yyP19->Kind == kAltRep) {
/* line 1328 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1328 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltRep (yyP19->AltRep.Replicator);
/* line 1328 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltList (yyP19->AltRep.AltList);
  }
   return;

  }
 yyAbort ("GenDeclAltTypeInner");
;
}

static void GenDeclAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP20)
# else
(yyP20)
 register tTree yyP20;
# endif
{
  if (yyP20->Kind == kNoAlt) {
/* line 1333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP20->Kind == kAnAlt) {
/* line 1335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltList (yyP20->AnAlt.Next);
  }
   return;

  }
  if (yyP20->Kind == kANewAlt) {
/* line 1337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltTypeInner (yyP20->ANewAlt.AltType);
/* line 1337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclAltList (yyP20->ANewAlt.Next);
  }
   return;

  }
 yyAbort ("GenDeclAltList");
;
}

static void GenDeclAltRep
# if defined __STDC__ | defined __cplusplus
(register tTree yyP21)
# else
(yyP21)
 register tTree yyP21;
# endif
{
/* line 1344 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP21->Replicator.Sym);
/* line 1345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Alt;");
/* line 1345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLRepState (yyP21);
  }
   return;

;
}

static void GenNLRepState
# if defined __STDC__ | defined __cplusplus
(register tTree yyP22)
# else
(yyP22)
 register tTree yyP22;
# endif
{
/* line 1351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsLocal (yyP22->Replicator.Sym)))) goto yyL1;
  {
/* line 1351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP22->Replicator.From)))) goto yyL1;
  {
/* line 1351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP22->Replicator.For)))) goto yyL1;
  {
/* line 1352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP22->Replicator.Sym);
/* line 1352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP22->Replicator.Sym, PrefixId (CurSpec () . PrefixId, Symbol (yyP22->Replicator.Sym) . CId, '.'));
  }
  }
  }
  }
   return;
yyL1:;

/* line 1356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsLocal (yyP22->Replicator.Sym)))) goto yyL2;
  {
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP22->Replicator.Sym);
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP22->Replicator.Sym);
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End;");
/* line 1358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP22->Replicator.Sym, PrefixId (CurSpec () . PrefixId, Symbol (yyP22->Replicator.Sym) . CId, '.'));
  }
  }
   return;
yyL2:;

/* line 1362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenDeclSpecs
# if defined __STDC__ | defined __cplusplus
(register tTree yyP23)
# else
(yyP23)
 register tTree yyP23;
# endif
{
  if (yyP23->Kind == kNoSpec) {
/* line 1366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP23->Kind == kASpec) {
/* line 1367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclNLSpec (yyP23->ASpec.Spec);
/* line 1367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSpecs (yyP23->ASpec.Next);
  }
   return;

  }
 yyAbort ("GenDeclSpecs");
;
}

static void GenDeclNLSpec
# if defined __STDC__ | defined __cplusplus
(register tTree yyP24)
# else
(yyP24)
 register tTree yyP24;
# endif
{

  switch (yyP24->Kind) {
  case kDecl:
/* line 1373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclNLIdList (yyP24->Decl.Type, yyP24->Decl.IdList);
  }
   return;

  case kVal:
/* line 1376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP24->Val.Sym)))) goto yyL2;
  }
   return;
yyL2:;

  break;
  case kValRetypes:
  case kValReshapes:
/* line 1377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP24->ValRetypes.Sym)))) goto yyL3;
  }
   return;
yyL3:;

  break;
  case kRetypes:
  case kReshapes:
/* line 1378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP24->Retypes.Sym)))) goto yyL4;
  }
   return;
yyL4:;

  break;
  case kIs:
/* line 1379 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1379 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP24->Is.Sym)))) goto yyL5;
  }
   return;
yyL5:;

  break;
  case kCallFrame:
/* line 1382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProc (SymbolKind (yyP24->CallFrame.Sym))))) goto yyL6;
  }
   return;
yyL6:;

/* line 1383 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP24->CallFrame.Sym);
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP24->CallFrame.Id);
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP24->CallFrame.Id = PrefixId (CurSpec () . PrefixId, yyP24->CallFrame.Id, '.');
  }
   return;

  case kParFrame:
/* line 1387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP24->ParFrame.Rep > 0))) goto yyL8;
  {
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tTask ");
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP24->ParFrame.TId);
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP24->ParFrame.Rep);
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP24->ParFrame.Sym);
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP24->ParFrame.FId);
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP24->ParFrame.Rep);
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP24->ParFrame.TId = PrefixId (CurSpec () . PrefixId, yyP24->ParFrame.TId, '.');
/* line 1391 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP24->ParFrame.FId = PrefixId (CurSpec () . PrefixId, yyP24->ParFrame.FId, '.');
  }
  }
   return;
yyL8:;

/* line 1394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tTask ");
/* line 1395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP24->ParFrame.TId);
/* line 1395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP24->ParFrame.Sym);
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP24->ParFrame.FId);
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP24->ParFrame.TId = PrefixId (CurSpec () . PrefixId, yyP24->ParFrame.TId, '.');
/* line 1398 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP24->ParFrame.FId = PrefixId (CurSpec () . PrefixId, yyP24->ParFrame.FId, '.');
  }
   return;

  case kDefAProc:
  case kDefProc:
  case kInlineDefProc:
  case kPrototypeProc:
/* line 1401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAFunc:
  case kDefFunc:
  case kInlineDefFunc:
  case kDefIsFunc:
  case kInlineDefIsFunc:
  case kPrototypeFunc:
/* line 1402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAPro:
  case kDefSeqPro:
  case kDefTagPro:
/* line 1403 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kNullSpec:
/* line 1404 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }

/* line 1406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSpecState (yyP24);
  }
   return;

;
}

static void GenDeclSym
# if defined __STDC__ | defined __cplusplus
(register tTree yyP27, tSymbol yyP26, register bool yyP25)
# else
(yyP27, yyP26, yyP25)
 register tTree yyP27;
 tSymbol yyP26;
 register bool yyP25;
# endif
{
/* line 1412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP27)))) goto yyL1;
  }
   return;
yyL1:;

  if (equalbool (yyP25, false)) {
/* line 1415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SymbolType (yyP26))))) goto yyL2;
  {
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP26));
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP26);
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_CHAN");
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (SymbolType (yyP26));
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP26));
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP26);
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (SymbolType (yyP26));
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP26, PrefixId (CurSpec () . PrefixId, Symbol (yyP26) . CId, '.'));
  }
  }
   return;
yyL2:;

  }
  if (equalbool (yyP25, false)) {
/* line 1421 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP26));
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP26);
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (SymbolType (yyP26));
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP26, PrefixId (CurSpec () . PrefixId, Symbol (yyP26) . CId, '.'));
  }
   return;

  }
/* line 1426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP26));
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP26);
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (SymbolType (yyP26));
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP26, PrefixId (CurSpec () . PrefixId, Symbol (yyP26) . CId, '.'));
  }
   return;

;
}

static void GenDeclArraySym
# if defined __STDC__ | defined __cplusplus
(register tTree yyP29, tSymbol yyP28)
# else
(yyP29, yyP28)
 register tTree yyP29;
 tSymbol yyP28;
# endif
{
/* line 1434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP29)))) goto yyL1;
  }
   return;
yyL1:;

  if (Tree_IsType (yyP29, kArray)) {
  if (yyP29->Array.Expr->Kind == kUnBound) {
/* line 1437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP28);
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP29));
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP29->Array.Type, yyP28);
  }
   return;

  }
/* line 1442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP29->Array.Expr)))) goto yyL3;
  {
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP28);
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP29));
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("/*SMW*/");
/* line 1443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP29->Array.Type, yyP28);
  }
  }
   return;
yyL3:;

/* line 1447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP29->Array.Type, yyP28);
  }
   return;

  }
/* line 1451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SymbolType (yyP28))))) goto yyL5;
  {
/* line 1452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP28));
/* line 1452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" **");
/* line 1452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP28);
/* line 1452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP28, PrefixId (CurSpec () . PrefixId, Symbol (yyP28) . CId, '.'));
  }
  }
   return;
yyL5:;

/* line 1456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (SymbolType (yyP28));
/* line 1457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP28);
/* line 1457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP28, PrefixId (CurSpec () . PrefixId, Symbol (yyP28) . CId, '.'));
  }
   return;

;
}

static bool IsZeroSizeType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP30)
# else
(yyP30)
 register tTree yyP30;
# endif
{
  if (Tree_IsType (yyP30, kArray)) {
/* line 1466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (((IsConstExpr (yyP30->Array.Expr)) && (ValConstInt (yyP30->Array.Expr) == 0)))) goto yyL1;
  }
   return true;
yyL1:;

/* line 1468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsZeroSizeType (yyP30->Array.Type);

  }
/* line 1470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return false;

}

static bool NeedsSpecs
# if defined __STDC__ | defined __cplusplus
(register tTree yyP31)
# else
(yyP31)
 register tTree yyP31;
# endif
{
/* line 1475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP31 -> SpecList . DCL > 0);

}

static bool NeedsSplit
# if defined __STDC__ | defined __cplusplus
(register tTree yyP32)
# else
(yyP32)
 register tTree yyP32;
# endif
{
  if (Tree_IsType (yyP32, kProcess)) {
/* line 1479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> Process . SLC > 1);

  }
  if (Tree_IsType (yyP32, kProcList)) {
/* line 1480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> ProcList . SLC > 1);

  }
  if (Tree_IsType (yyP32, kChoiceList)) {
/* line 1481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> ChoiceList . SLC > 1);

  }
  if (Tree_IsType (yyP32, kAltList)) {
/* line 1482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> AltList . SLC > 1);

  }
  if (Tree_IsType (yyP32, kGuard)) {
/* line 1483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> Guard . SLC > 1);

  }
  if (Tree_IsType (yyP32, kCaseList)) {
/* line 1484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP32 -> CaseList . SLC > 1);

  }
 yyAbort ("NeedsSplit");
}

static void GenDeclNLIdList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP34, register tTree yyP33)
# else
(yyP34, yyP33)
 register tTree yyP34;
 register tTree yyP33;
# endif
{
  if (yyP33->Kind == kAnId) {
/* line 1489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP33->AnId.Sym)))) goto yyL1;
  {
/* line 1489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclNLIdList (yyP34, yyP33->AnId.Next);
  }
  }
   return;
yyL1:;

/* line 1490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP34, yyP33->AnId.Sym, false);
/* line 1490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclNLIdList (yyP34, yyP33->AnId.Next);
  }
   return;

  }
  if (yyP33->Kind == kNoId) {
/* line 1491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenDeclNLIdList");
;
}

static void GenSpecState
# if defined __STDC__ | defined __cplusplus
(register tTree yyP35)
# else
(yyP35)
 register tTree yyP35;
# endif
{
  if (yyP35->Kind == kVal) {
  if (Tree_IsType (yyP35->Val.Expr, kStringElement)) {
/* line 1499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP35->Val.Expr)))) goto yyL1;
  {
/* line 1499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (ExprType (yyP35->Val.Expr))))) goto yyL1;
  {
/* line 1500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->Val.Type, yyP35->Val.Sym);
  }
  }
  }
   return;
yyL1:;

/* line 1521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected string element", xxError, yyP35->Val.Pos);
  }
   return;

  }
  if (yyP35->Val.Expr->Kind == kElementalExpr) {
/* line 1502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP35->Val.Expr)))) goto yyL2;
  {
/* line 1502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (ExprType (yyP35->Val.Expr))))) goto yyL2;
  {
/* line 1503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->Val.Type, yyP35->Val.Sym);
  }
  }
  }
   return;
yyL2:;

  if (Tree_IsType (yyP35->Val.Expr->ElementalExpr.Elemental, kTable)) {
/* line 1512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->Val.Type, yyP35->Val.Sym, false);
  }
   return;

  }
  if (Tree_IsType (yyP35->Val.Expr->ElementalExpr.Elemental, kTableElement)) {
/* line 1518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected table element", xxError, yyP35->Val.Pos);
  }
   return;

  }
  }
  if (yyP35->Val.Expr->Kind == kFuncCall) {
/* line 1515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsSTVFunction (SymbolKind (yyP35->Val.Expr->FuncCall.Sym))))) goto yyL6;
  {
/* line 1516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->Val.Type, yyP35->Val.Sym, false);
  }
  }
   return;
yyL6:;

  }
/* line 1563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP35->Val.Sym))))) goto yyL18;
  {
/* line 1563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP35->Val.Expr)))) goto yyL18;
  {
/* line 1563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsNewSymbol (yyP35->Val.Sym)))) goto yyL18;
  }
  }
  }
   return;
yyL18:;

/* line 1565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP35->Val.Sym))))) goto yyL19;
  {
/* line 1566 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (SymbolType (yyP35->Val.Sym), yyP35->Val.Sym, false);
  }
  }
   return;
yyL19:;

/* line 1569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1570 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (SymbolType (yyP35->Val.Sym), yyP35->Val.Sym);
  }
   return;

  }
  if (Tree_IsType (yyP35, kValRetypes)) {
  if (Tree_IsType (yyP35->ValRetypes.Expr, kStringElement)) {
/* line 1505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP35->ValRetypes.Expr)))) goto yyL3;
  {
/* line 1505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP35->ValRetypes.Type)))) goto yyL3;
  {
/* line 1506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->ValRetypes.Type, yyP35->ValRetypes.Sym);
  }
  }
  }
   return;
yyL3:;

/* line 1527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected string retypes", xxError, yyP35->ValRetypes.Pos);
  }
   return;

  }
  if (yyP35->ValRetypes.Expr->Kind == kElementalExpr) {
/* line 1508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP35->ValRetypes.Expr)))) goto yyL4;
  {
/* line 1508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP35->ValRetypes.Type)))) goto yyL4;
  {
/* line 1508 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IS_Mode (xxEmulate)))) goto yyL4;
  {
/* line 1509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->ValRetypes.Type, yyP35->ValRetypes.Sym);
  }
  }
  }
  }
   return;
yyL4:;

  if (Tree_IsType (yyP35->ValRetypes.Expr->ElementalExpr.Elemental, kTableElement)) {
/* line 1524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected table retypes", xxError, yyP35->ValRetypes.Pos);
  }
   return;

  }
  }
/* line 1573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP35->ValRetypes.Type)))) goto yyL21;
  {
/* line 1574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->ValRetypes.Type, yyP35->ValRetypes.Sym, false);
  }
  }
   return;
yyL21:;

/* line 1577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL22;
  {
/* line 1577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP35->ValRetypes.Type) != RetypeSize (ExprType (yyP35->ValRetypes.Expr))))) goto yyL22;
  {
/* line 1577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsBoundedType (yyP35->ValRetypes.Type)))) goto yyL22;
  {
/* line 1578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->ValRetypes.Type, yyP35->ValRetypes.Sym, false);
  }
  }
  }
  }
   return;
yyL22:;

/* line 1581 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->ValRetypes.Type, yyP35->ValRetypes.Sym);
  }
   return;

  }
  if (yyP35->Kind == kIs) {
  if (Tree_IsType (yyP35->Is.Elemental, kTable)) {
/* line 1530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->Is.Type, yyP35->Is.Sym, false);
  }
   return;

  }
  if (Tree_IsType (yyP35->Is.Elemental, kTableElement)) {
/* line 1533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected table element", xxError, yyP35->Is.Pos);
  }
   return;

  }
/* line 1538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP35->Is.Sym))))) goto yyL13;
  {
/* line 1539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (SymbolType (yyP35->Is.Sym), yyP35->Is.Sym, false);
  }
  }
   return;
yyL13:;

/* line 1542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (SymbolType (yyP35->Is.Sym), yyP35->Is.Sym);
  }
   return;

  }
  if (Tree_IsType (yyP35, kRetypes)) {
/* line 1546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP35->Retypes.Type)))) goto yyL15;
  {
/* line 1547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->Retypes.Type, yyP35->Retypes.Sym, false);
  }
  }
   return;
yyL15:;

/* line 1550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL16;
  {
/* line 1550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsChanType (yyP35->Retypes.Type)))) goto yyL16;
  {
/* line 1551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsChanType (ElementType (yyP35->Retypes.Elemental))))) goto yyL16;
  {
/* line 1552 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP35->Retypes.Type) != RetypeSize (ElementType (yyP35->Retypes.Elemental))))) goto yyL16;
  {
/* line 1553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsBoundedType (yyP35->Retypes.Type)))) goto yyL16;
  {
/* line 1554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP35->Retypes.Type, yyP35->Retypes.Sym, false);
  }
  }
  }
  }
  }
  }
   return;
yyL16:;

/* line 1557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclArraySym (yyP35->Retypes.Type, yyP35->Retypes.Sym);
  }
   return;

  }
  if (yyP35->Kind == kDeclType) {
/* line 1586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
/* line 1588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP35, yyP35->Spec.Pos);
  }
   return;

;
}

int CountRepDecls
# if defined __STDC__ | defined __cplusplus
(register tTree yyP36)
# else
(yyP36)
 register tTree yyP36;
# endif
{
/* line 1598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP36->Replicator.Sym)))) goto yyL1;
  }
   return 0;
yyL1:;

/* line 1599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP36->Replicator.From)))) goto yyL2;
  {
/* line 1599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP36->Replicator.For)))) goto yyL2;
  }
  }
   return 1;
yyL2:;

/* line 1600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 2;

}

int CountAltRepDecls
# if defined __STDC__ | defined __cplusplus
(register tTree yyP37)
# else
(yyP37)
 register tTree yyP37;
# endif
{
/* line 1607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return CountRepDecls (yyP37) + 1;

}

static int CountAbbrev
# if defined __STDC__ | defined __cplusplus
(register tTree yyP38)
# else
(yyP38)
 register tTree yyP38;
# endif
{
  if (Tree_IsType (yyP38, kArray)) {
  if (yyP38->Array.Expr->Kind == kUnBound) {
/* line 1612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1 + CountAbbrev (yyP38->Array.Type);

  }
/* line 1614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP38->Array.Expr)))) goto yyL2;
  }
   return 1 + CountAbbrev (yyP38->Array.Type);
yyL2:;

/* line 1616 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return CountAbbrev (yyP38->Array.Type);

  }
/* line 1618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1;

}

static int CountIdList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP39)
# else
(yyP39)
 register tTree yyP39;
# endif
{
  if (yyP39->Kind == kNoId) {
/* line 1624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  }
  if (yyP39->Kind == kAnId) {
/* line 1625 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (IsNonLocal(yyP39->AnId.Sym)?CountIdList(yyP39->AnId.Next)+1:CountIdList(yyP39->AnId.Next));

  }
 yyAbort ("CountIdList");
}

static int CountDecl
# if defined __STDC__ | defined __cplusplus
(register tTree yyP40)
# else
(yyP40)
 register tTree yyP40;
# endif
{
/* line 1630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP40->Decl.Type)))) goto yyL1;
  }
   return 0;
yyL1:;

/* line 1631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP40->Decl.Type)))) goto yyL2;
  }
   return 2 * CountIdList (yyP40->Decl.IdList);
yyL2:;

/* line 1632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return CountIdList (yyP40->Decl.IdList);

}

int CountSpec
# if defined __STDC__ | defined __cplusplus
(register tTree yyP41)
# else
(yyP41)
 register tTree yyP41;
# endif
{

  switch (yyP41->Kind) {
  case kDefAProc:
  case kDefProc:
  case kInlineDefProc:
  case kPrototypeProc:
/* line 1637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  case kDefAFunc:
  case kDefFunc:
  case kInlineDefFunc:
  case kDefIsFunc:
  case kInlineDefIsFunc:
  case kPrototypeFunc:
/* line 1638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  case kParFrame:
/* line 1640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 2;

  case kCallFrame:
/* line 1641 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1;

  case kDecl:
/* line 1643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return CountDecl (yyP41);

  case kVal:
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP41->Val.Sym))))) goto yyL6;
  {
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP41->Val.Expr)))) goto yyL6;
  {
/* line 1645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsNewSymbol (yyP41->Val.Sym)))) goto yyL6;
  }
  }
  }
   return 0;
yyL6:;

/* line 1649 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsNonLocal(yyP41->Val.Sym)?CountAbbrev(SymbolType(yyP41->Val.Sym)):0;

  case kIs:
/* line 1647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsNonLocal(yyP41->Is.Sym)?CountAbbrev(SymbolType(yyP41->Is.Sym)):0;

  case kRetypes:
  case kReshapes:
/* line 1648 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsNonLocal(yyP41->Retypes.Sym)?CountAbbrev(SymbolType(yyP41->Retypes.Sym)):0;

  case kValRetypes:
  case kValReshapes:
/* line 1650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsNonLocal(yyP41->ValRetypes.Sym)?CountAbbrev(SymbolType(yyP41->ValRetypes.Sym)):0;

  }

/* line 1652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1;

}

tTree GenCode
# if defined __STDC__ | defined __cplusplus
(register tTree T)
# else
(T)
 register tTree T;
# endif
{

  switch (T->Kind) {
  case kStart:
/* line 1662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  case kDefFunc:
  case kInlineDefFunc:
/* line 1664 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (T->DefFunc.Pos);
/* line 1666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CurProc = T;
/* line 1667 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   InitBlocks ();
/* line 1668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncHead (T);
/* line 1668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncEntry (T);
/* line 1670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (T->DefFunc.Valof->Valof.SpecList, NoTree, false);
/* line 1671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (T->DefFunc.Valof->Valof.Process);
/* line 1672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncExit (T);
/* line 1673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (T->DefFunc.Valof->Valof.SpecList);
/* line 1674 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncTail (T);
  }
   return T -> DefFunc . CGNext;

  case kDefProc:
  case kInlineDefProc:
/* line 1677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (T->DefProc.Pos);
/* line 1679 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CurProc = T;
/* line 1680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   InitBlocks ();
/* line 1681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcHead (T);
/* line 1681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcEntry (T);
/* line 1683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (T->DefProc.Process);
/* line 1684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcTail (T);
  }
   return T -> DefProc . CGNext;

  case kDefIsFunc:
  case kInlineDefIsFunc:
/* line 1687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (T, T->DefIsFunc.Pos);
  }
   return T -> DefIsFunc . CGNext;

  case kDefAFunc:
  case kPrototypeFunc:
/* line 1691 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefAFunc . CGNext;

  case kDefAProc:
  case kPrototypeProc:
/* line 1692 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefAProc . CGNext;

  case kHCode:
/* line 1693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1694 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteCCode (T->HCode.Code, T->HCode.Pos, T -> HCode . Scope);
/* line 1695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return T -> HCode . CGNext;

  }

 yyAbort ("GenCode");
}

static void GenProcHead
# if defined __STDC__ | defined __cplusplus
(register tTree DP)
# else
(DP)
 register tTree DP;
# endif
{
/* line 1702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma = false; 
/* line 1704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (! IsTopLevel (DP->DefProc.Sym), "static ");
/* line 1706 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("void ");
/* line 1707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsInlined (DP), "inline ");
/* line 1708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DP->DefProc.Sym);
/* line 1708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" (");
/* line 1709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
   return;
yyL1:;

/* line 1712 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1712 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsAtomicProc (DP) || IsParProc (DP)))) goto yyL2;
  {
/* line 1713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DP->DefProc.Sym);
/* line 1713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *FP)");
  }
  }
   return;
yyL2:;

/* line 1716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (DP)))) goto yyL3;
  {
/* line 1717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DP -> DefProc . Parent);
/* line 1717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *PP");
/* line 1718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = true;
/* line 1719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL3;
  }
  }
   return;
yyL3:;

/* line 1722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = GenFormals (DP->DefProc.Formals, comma);
/* line 1724 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (! comma, "void");
/* line 1725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void GenProcEntry
# if defined __STDC__ | defined __cplusplus
(register tTree DP)
# else
(DP)
 register tTree DP;
# endif
{
/* line 1734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsAtomicProc (DP)))) goto yyL1;
  {
/* line 1735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("while(true)");
/* line 1736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("switch(FP->_Header.IP)");
/* line 1738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CurrentLabel = - 1;
/* line 1740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLabel ();
/* line 1741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (DP->DefProc.Formals);
  }
  }
   return;
yyL1:;

/* line 1744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsParProc (DP)))) goto yyL2;
  {
/* line 1745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (DP->DefProc.Formals);
  }
  }
   return;
yyL2:;

/* line 1749 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1750 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1750 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1750 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DP->DefProc.Sym);
/* line 1751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" SF;");
/* line 1751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DP->DefProc.Sym);
/* line 1752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *FP = &SF;");
/* line 1752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (DP->DefProc.Formals);
/* line 1754 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (DP->DefProc.Formals);
/* line 1755 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (DP)))) goto yyL3;
  {
/* line 1756 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->Chain = PP;");
/* line 1756 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL3:;

/* line 1759 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenProcTail
# if defined __STDC__ | defined __cplusplus
(register tTree DP)
# else
(DP)
 register tTree DP;
# endif
{
/* line 1771 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1771 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP -> DefProc . isPar))) goto yyL1;
  {
/* line 1772 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENDP();");
/* line 1773 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 1776 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1776 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProc (DP)))) goto yyL2;
  {
/* line 1777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 1777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BlankLine ();
  }
  }
   return;
yyL2:;

/* line 1781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! DP -> DefProc . isPar))) goto yyL3;
  {
/* line 1782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("RETURN();");
/* line 1783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL3;
  }
  }
   return;
yyL3:;

/* line 1786 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: SETERR(MSG_IP);");
/* line 1788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 1789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 1790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1791 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1791 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 1791 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1792 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BlankLine ();
  }
   return;

;
}

static void GenFuncHead
# if defined __STDC__ | defined __cplusplus
(register tTree DF)
# else
(DF)
 register tTree DF;
# endif
{
/* line 1799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma = false; 
  if (DF->DefFunc.TypeList->Kind == kAType) {
/* line 1801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsMVFunction (DF) && ! IsSTVFunction (DF)))) goto yyL1;
  {
/* line 1802 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite ((IsInlined (DF)) || ! IsTopLevel (DF->DefFunc.Sym), "static ");
/* line 1803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (DF->DefFunc.TypeList->AType.Type);
/* line 1803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1804 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsInlined (DF), "inline ");
/* line 1805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DF->DefFunc.Sym);
/* line 1805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 1806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

  }
/* line 1809 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1809 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (DF) || IsSTVFunction (DF)))) goto yyL2;
  {
/* line 1810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (! IsTopLevel (DF->DefFunc.Sym), "static ");
/* line 1811 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("void ");
/* line 1812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsInlined (DF), "inline ");
/* line 1813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DF->DefFunc.Sym);
/* line 1813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 1814 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
  }
   return;
yyL2:;

/* line 1817 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1817 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (DF)))) goto yyL3;
  {
/* line 1818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DF -> DefFunc . Parent);
/* line 1818 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *PP");
/* line 1819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = true;
/* line 1820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL3;
  }
  }
   return;
yyL3:;

/* line 1823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1824 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = GenFormals (DF->DefFunc.Formals, comma);
/* line 1825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL4;
  }
   return;
yyL4:;

/* line 1828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (DF) || IsSTVFunction (DF)))) goto yyL5;
  {
/* line 1829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenMVFormals (DF->DefFunc.TypeList, 0, comma);
/* line 1830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
  }
   return;
yyL5:;

/* line 1833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1834 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (! comma, "void ");
/* line 1835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void GenFuncEntry
# if defined __STDC__ | defined __cplusplus
(register tTree DF)
# else
(DF)
 register tTree DF;
# endif
{
/* line 1842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 1843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 1843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsMVFunction (DF) && ! IsSTVFunction (DF)))) goto yyL1;
  {
/* line 1845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (DF->DefFunc.TypeList -> AType . Type);
/* line 1845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" _Result;");
/* line 1845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 1849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DF->DefFunc.Sym);
/* line 1850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" SF;");
/* line 1850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("tSF_");
/* line 1851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (DF->DefFunc.Sym);
/* line 1851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *FP = &SF;");
/* line 1851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (DF->DefFunc.Formals);
/* line 1853 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (DF->DefFunc.Formals);
/* line 1854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (DF)))) goto yyL2;
  {
/* line 1855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->Chain = PP;");
/* line 1855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL2:;

/* line 1858 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenFuncExit
# if defined __STDC__ | defined __cplusplus
(register tTree DF)
# else
(DF)
 register tTree DF;
# endif
{
/* line 1864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (DF) || IsSTVFunction (DF)))) goto yyL1;
  {
/* line 1865 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenMVFResult (DF->DefFunc.Valof->Valof.ExprList, 0);
  }
  }
   return;
yyL1:;

  if (DF->DefFunc.Valof->Valof.ExprList->Kind == kAnExpr) {
  if (DF->DefFunc.Valof->Valof.ExprList->AnExpr.Next->Kind == kNoExpr) {
/* line 1868 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Result = ");
/* line 1869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (DF->DefFunc.Valof->Valof.ExprList->AnExpr.Expr);
/* line 1869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
 yyAbort ("GenFuncExit");
;
}

static void GenFuncTail
# if defined __STDC__ | defined __cplusplus
(register tTree DF)
# else
(DF)
 register tTree DF;
# endif
{
/* line 1876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1876 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsMVFunction (DF) && ! IsSTVFunction (DF)))) goto yyL1;
  {
/* line 1877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("return _Result;");
/* line 1877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 1881 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 1882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 1882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1883 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BlankLine ();
  }
   return;

;
}

static bool GenFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP43, register bool yyP42)
# else
(yyP43, yyP42)
 register tTree yyP43;
 register bool yyP42;
# endif
{
/* line 1890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma; 
  if (Tree_IsType (yyP43, kNoFormal)) {
/* line 1892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP42;

  }
  if (yyP43->Kind == kAFormal) {
/* line 1894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP43->AFormal.Type)))) goto yyL2;
  {
/* line 1895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = GenFormals (yyP43->AFormal.Next, false);
  }
  }
   return comma;
yyL2:;

  if (Tree_IsType (yyP43->AFormal.Type, kUserStructTypes)) {
/* line 1898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayFormal (yyP43->AFormal.Type, yyP43->AFormal.Sym, yyP42);
/* line 1900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (GenFormals (yyP43->AFormal.Next, true))) goto yyL3;
  }
   return true;
yyL3:;

  }
  if (Tree_IsType (yyP43->AFormal.Type, kArray)) {
/* line 1903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayFormal (yyP43->AFormal.Type, yyP43->AFormal.Sym, yyP42);
/* line 1905 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (GenFormals (yyP43->AFormal.Next, true))) goto yyL4;
  }
   return true;
yyL4:;

  }
  if (yyP43->AFormal.By->Kind == kByVal) {
/* line 1908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP42);
/* line 1910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP43->AFormal.Type);
/* line 1910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 1910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP43->AFormal.Sym);
/* line 1911 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (GenFormals (yyP43->AFormal.Next, true))) goto yyL5;
  }
   return true;
yyL5:;

  }
  if (yyP43->AFormal.By->Kind == kByRef) {
/* line 1914 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP42);
/* line 1916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP43->AFormal.Type);
/* line 1916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP43->AFormal.Sym);
/* line 1917 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (GenFormals (yyP43->AFormal.Next, true))) goto yyL6;
  }
   return true;
yyL6:;

  }
  }
 yyAbort ("GenFormals");
}

static void GenArrayFormal
# if defined __STDC__ | defined __cplusplus
(register tTree yyP46, tSymbol yyP45, register bool yyP44)
# else
(yyP46, yyP45, yyP44)
 register tTree yyP46;
 tSymbol yyP45;
 register bool yyP44;
# endif
{
  if (Tree_IsType (yyP46, kArray)) {
/* line 1924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP46->Array.Expr)))) goto yyL1;
  {
/* line 1925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP44);
/* line 1926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 1926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP45);
/* line 1926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP46));
/* line 1927 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayFormal (yyP46->Array.Type, yyP45, true);
  }
  }
   return;
yyL1:;

/* line 1930 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1931 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayFormal (yyP46->Array.Type, yyP45, yyP44);
  }
   return;

  }
/* line 1934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP46)))) goto yyL3;
  {
/* line 1935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP44);
/* line 1936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP46);
/* line 1936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" **");
/* line 1936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP45);
  }
  }
   return;
yyL3:;

/* line 1939 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1940 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP44);
/* line 1941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP46);
/* line 1941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 1941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP45);
  }
   return;

;
}

static void GenNLFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP47)
# else
(yyP47)
 register tTree yyP47;
# endif
{
  if (yyP47->Kind == kAFormal) {
/* line 1948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP47->AFormal.Type)))) goto yyL1;
  {
/* line 1949 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (yyP47->AFormal.Next);
  }
  }
   return;
yyL1:;

  if (Tree_IsType (yyP47->AFormal.Type, kArray)) {
/* line 1952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP47->AFormal.Sym)))) goto yyL2;
  {
/* line 1953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLArrayFormal (yyP47->AFormal.Type, yyP47->AFormal.Sym);
/* line 1954 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (yyP47->AFormal.Next);
  }
  }
   return;
yyL2:;

  }
/* line 1957 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1957 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP47->AFormal.Sym)))) goto yyL3;
  {
/* line 1958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP47->AFormal.Sym);
/* line 1958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 1958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP47->AFormal.Sym);
/* line 1958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1959 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (yyP47->AFormal.Next);
  }
  }
   return;
yyL3:;

/* line 1962 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1963 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLFormals (yyP47->AFormal.Next);
  }
   return;

  }
  if (Tree_IsType (yyP47, kNoFormal)) {
/* line 1966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenNLFormals");
;
}

static void GenNLArrayFormal
# if defined __STDC__ | defined __cplusplus
(register tTree yyP49, tSymbol yyP48)
# else
(yyP49, yyP48)
 register tTree yyP49;
 tSymbol yyP48;
# endif
{
  if (Tree_IsType (yyP49, kArray)) {
  if (yyP49->Array.Expr->Kind == kUnBound) {
/* line 1972 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP48);
/* line 1973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP49));
/* line 1973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 1974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP48);
/* line 1974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 1974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP49));
/* line 1974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 1975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLArrayFormal (yyP49->Array.Type, yyP48);
  }
   return;

  }
/* line 1977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNLArrayFormal (yyP49->Array.Type, yyP48);
  }
   return;

  }
/* line 1980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP48);
/* line 1981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 1981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP48);
/* line 1981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 1981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static bool AnyNLFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP50)
# else
(yyP50)
 register tTree yyP50;
# endif
{
  if (yyP50->Kind == kAFormal) {
/* line 1986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP50->AFormal.Sym)))) goto yyL1;
  }
   return true;
yyL1:;

/* line 1987 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1987 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((AnyNLFormals (yyP50->AFormal.Next)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  return false;
}

static void GenMVFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP53, register int yyP52, register bool yyP51)
# else
(yyP53, yyP52, yyP51)
 register tTree yyP53;
 register int yyP52;
 register bool yyP51;
# endif
{
  if (yyP53->Kind == kNoType) {
/* line 1994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP53->Kind == kAType) {
  if (equalbool (yyP51, true)) {
/* line 1996 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 1997 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 1998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
   return;
yyL2:;

  }
/* line 2001 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP53->AType.Type);
/* line 2002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 2002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 2002 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP52);
/* line 2003 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenMVFormals (yyP53->AType.Next, yyP52 + 1, true);
  }
   return;

  }
 yyAbort ("GenMVFormals");
;
}

static void GenMVFResult
# if defined __STDC__ | defined __cplusplus
(register tTree yyP55, register int yyP54)
# else
(yyP55, yyP54)
 register tTree yyP55;
 register int yyP54;
# endif
{
  if (yyP55->Kind == kNoExpr) {
/* line 2008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP55->Kind == kAnExpr) {
/* line 2009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (ExprType (yyP55->AnExpr.Expr))))) goto yyL2;
  {
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ASSIGN(");
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP54);
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP55->AnExpr.Expr);
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcTypeSize (ExprType (yyP55->AnExpr.Expr)));
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenMVFResult (yyP55->AnExpr.Next, yyP54 + 1);
  }
  }
   return;
yyL2:;

/* line 2013 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP54);
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP55->AnExpr.Expr);
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 2014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2015 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenMVFResult (yyP55->AnExpr.Next, yyP54 + 1);
  }
   return;

  }
 yyAbort ("GenMVFResult");
;
}

static bool NeedsChain
# if defined __STDC__ | defined __cplusplus
(register tTree yyP56)
# else
(yyP56)
 register tTree yyP56;
# endif
{
  if (Tree_IsType (yyP56, kDefProc)) {
/* line 2021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((UsesFreeVars (yyP56)))) goto yyL1;
  {
/* line 2021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsTopLevel (yyP56->DefProc.Sym)))) goto yyL1;
  }
  }
   return true;
yyL1:;

  }
  if (Tree_IsType (yyP56, kDefFunc)) {
/* line 2022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((UsesFreeVars (yyP56)))) goto yyL2;
  {
/* line 2022 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsTopLevel (yyP56->DefFunc.Sym)))) goto yyL2;
  }
  }
   return true;
yyL2:;

  }
  return false;
}

static bool UsesFreeVars
# if defined __STDC__ | defined __cplusplus
(register tTree yyP57)
# else
(yyP57)
 register tTree yyP57;
# endif
{
/* line 2027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxNoUsageChecking)))) goto yyL1;
  }
   return true;
yyL1:;

  if (Tree_IsType (yyP57, kDefProc)) {
/* line 2028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((HasReadUsage (yyP57->DefProc.Process -> Process . PUOut) || HasWriteUsage (yyP57->DefProc.Process -> Process . PUOut)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (Tree_IsType (yyP57, kDefFunc)) {
/* line 2030 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2030 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((HasReadUsage (yyP57->DefFunc.Valof->Valof.ExprList -> ExprList . PUOut) || HasWriteUsage (yyP57->DefFunc.Valof->Valof.ExprList -> ExprList . PUOut)))) goto yyL3;
  }
   return true;
yyL3:;

  }
  if (Tree_IsType (yyP57, kDefIsFunc)) {
/* line 2031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((HasReadUsage (yyP57->DefIsFunc.ExprList -> ExprList . PUOut) || HasWriteUsage (yyP57->DefIsFunc.ExprList -> ExprList . PUOut)))) goto yyL4;
  }
   return true;
yyL4:;

  }
  if (yyP57->Kind == kPar) {
/* line 2032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((HasReadUsage (yyP57->Par.ProcList -> ProcList . PUOut) || HasWriteUsage (yyP57->Par.ProcList -> ProcList . PUOut)))) goto yyL5;
  }
   return true;
yyL5:;

  }
  if (yyP57->Kind == kParRep) {
/* line 2033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((HasReadUsage (yyP57->ParRep.Process -> Process . PUOut) || HasWriteUsage (yyP57->ParRep.Process -> Process . PUOut)))) goto yyL6;
  }
   return true;
yyL6:;

  }
  return false;
}

static bool HasReadUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP58)
# else
(yyP58)
 tParUsage yyP58;
# endif
{
  if (yyP58 == NULL) {
/* line 2036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return false;
  }

  }
/* line 2037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsEmpty (& yyP58 -> R)))) goto yyL2;
  }
   return true;
yyL2:;

/* line 2038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP58 -> RAL, kAnAccess)))) goto yyL3;
  }
   return true;
yyL3:;

  return false;
}

static bool HasWriteUsage
# if defined __STDC__ | defined __cplusplus
(tParUsage yyP59)
# else
(yyP59)
 tParUsage yyP59;
# endif
{
  if (yyP59 == NULL) {
/* line 2041 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2041 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return false;
  }

  }
/* line 2042 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2042 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsEmpty (& yyP59 -> W)))) goto yyL2;
  }
   return true;
yyL2:;

/* line 2043 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2043 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP59 -> WAL, kAnAccess)))) goto yyL3;
  }
   return true;
yyL3:;

  return false;
}

static void GenProcess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP60)
# else
(yyP60)
 register tTree yyP60;
# endif
{
/* line 2052 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int le,ls,lab,endlab; 

  switch (yyP60->Kind) {
  case kAssign:
/* line 2055 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2057 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Assign.SpecList, NoTree, false);
/* line 2058 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Assign.Pos);
/* line 2059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP60->Assign.Lval, yyP60->Assign.Rval);
/* line 2060 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Assign.SpecList);
/* line 2061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  case kCCode:
/* line 2065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2067 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->CCode.SpecList, NoTree, false);
/* line 2068 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->CCode.Pos);
/* line 2069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteCCode (yyP60->CCode.Code, yyP60->CCode.Pos, yyP60 -> CCode . Scope);
/* line 2070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->CCode.SpecList);
/* line 2071 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  case kInstance:
/* line 2075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2077 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Instance.SpecList, NoTree, false);
/* line 2078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Instance.Pos);
/* line 2079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInstance (SymbolKind (yyP60->Instance.Sym), yyP60->Instance.ExprList, yyP60 -> Instance . Frame -> CallFrame . Id);
/* line 2080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Instance.SpecList);
  }
   return;

  case kWhile:
/* line 2084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProcess (yyP60)))) goto yyL4;
  {
/* line 2086 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->While.SpecList, NoTree, false);
/* line 2087 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->While.Pos);
/* line 2088 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("while (");
/* line 2088 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP60->While.Expr);
/* line 2088 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 2088 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP60->While.Process);
/* line 2091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->While.Pos);
/* line 2092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2093 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->While.SpecList);
/* line 2094 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

/* line 2098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   le = NewLabel ();
/* line 2100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ls = NewLabel ();
/* line 2101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->While.SpecList);
/* line 2102 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->While.Pos);
/* line 2103 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 2103 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (le);
/* line 2103 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2103 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (ls);
/* line 2105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP60->While.Process);
/* line 2106 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->While.Pos);
/* line 2107 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (le);
/* line 2108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 2108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP60->While.Expr);
/* line 2108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 2108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 2110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (ls);
/* line 2110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2111 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2111 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2111 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->While.SpecList);
  }
   return;

  case kCase:
/* line 2116 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2116 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProcess (yyP60)))) goto yyL6;
  {
/* line 2118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Case.SpecList, NoTree, false);
/* line 2119 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Case.Pos);
/* line 2120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("switch(");
/* line 2120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP60->Case.Selector);
/* line 2120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 2120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseAtomic (yyP60->Case.ChoiceList, false);
/* line 2123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Case.SpecList);
  }
  }
   return;
yyL6:;

/* line 2128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Case.SpecList);
/* line 2132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Case.Pos);
/* line 2133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("switch(");
/* line 2133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP60->Case.Selector);
/* line 2133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 2133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseSwitches (yyP60->Case.ChoiceList, false);
/* line 2136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseCode (yyP60->Case.ChoiceList, lab);
/* line 2139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Case.SpecList);
  }
   return;

  case kIfType:
  case kIf:
  case kIfRep:
/* line 2144 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2144 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProcess (yyP60)))) goto yyL8;
  {
/* line 2146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BackIfType (yyP60) = NULL;
/* line 2147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfTypeAtomic (yyP60, false);
  }
  }
   return;
yyL8:;

/* line 2151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2153 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2154 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfType (yyP60, lab);
/* line 2155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("SETERR(MSG_IF);");
/* line 2155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
   return;

  case kSkip:
/* line 2160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Skip.SpecList, NoTree, false);
/* line 2163 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Skip.Pos);
/* line 2164 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Skip.SpecList);
  }
   return;

  case kStop:
/* line 2168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Stop.SpecList, NoTree, false);
/* line 2171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Stop.Pos);
/* line 2172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("SETERR(MSG_STOP);");
/* line 2173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Stop.SpecList);
  }
   return;

  case kSeq:
/* line 2178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProcess (yyP60)))) goto yyL12;
  {
/* line 2180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Seq.SpecList, NoTree, false);
/* line 2181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Seq.Pos);
/* line 2182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcList (yyP60->Seq.ProcList);
/* line 2184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Seq.SpecList);
  }
  }
   return;
yyL12:;

/* line 2189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Seq.SpecList);
/* line 2192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Seq.Pos);
/* line 2193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcList (yyP60->Seq.ProcList);
/* line 2194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Seq.SpecList);
  }
   return;

  case kSeqRep:
/* line 2198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP60->SeqRep.Replicator->Replicator.For)))) goto yyL14;
  {
/* line 2198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((ValConstInt (yyP60->SeqRep.Replicator->Replicator.For) == 0))) goto yyL14;
  }
  }
   return;
yyL14:;

/* line 2201 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2201 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProcess (yyP60)))) goto yyL15;
  {
/* line 2203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->SeqRep.SpecList, yyP60->SeqRep.Replicator, false);
/* line 2204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->SeqRep.Pos);
/* line 2205 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP60->SeqRep.Replicator);
/* line 2206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP60->SeqRep.Process);
/* line 2208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->SeqRep.Pos);
/* line 2210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->SeqRep.SpecList);
  }
  }
   return;
yyL15:;

/* line 2214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->SeqRep.SpecList);
/* line 2217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->SeqRep.Pos);
/* line 2218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   le = GenRepStart (yyP60->SeqRep.Replicator);
/* line 2219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ls = NewLabel ();
/* line 2220 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (ls);
/* line 2221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP60->SeqRep.Process);
/* line 2222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->SeqRep.Pos);
/* line 2223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenRepEnd (yyP60->SeqRep.Replicator, ls, le);
/* line 2224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->SeqRep.SpecList);
  }
   return;

  case kInput:
/* line 2228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (ElementType (yyP60->Input.Element))))) goto yyL17;
  {
/* line 2230 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->Input.SpecList, NoTree, false);
/* line 2231 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Input.Pos);
/* line 2232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenTimerInput (yyP60->Input.ExprList);
/* line 2233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->Input.SpecList);
  }
  }
   return;
yyL17:;

/* line 2249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Input.SpecList);
/* line 2249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Input.Pos);
/* line 2249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP60->Input.Element, ElementType (yyP60->Input.Element), yyP60->Input.ExprList);
/* line 2249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Input.SpecList);
  }
   return;

  case kAfter:
/* line 2240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->After.SpecList);
/* line 2242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->After.Pos);
/* line 2243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DELAY(");
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP60->After.Expr);
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->After.SpecList);
  }
   return;

  case kOutput:
/* line 2251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Output.SpecList);
/* line 2251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Output.Pos);
/* line 2251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOutput (yyP60->Output.Element, ElementType (yyP60->Output.Element) -> Chan . Protocol, yyP60->Output.ExprList);
/* line 2251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Output.SpecList);
  }
   return;

  case kAltType:
  case kAlt:
  case kAltRep:
/* line 2253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 {
  register tTree yyV1;
  {
/* line 2255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->AltType.SpecList);
/* line 2256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->AltType.Pos);
/* line 2257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ALT();");
/* line 2257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltType (yyP60);
/* line 2260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ALTWT(");
/* line 2261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BOOL TMP = false;");
/* line 2265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2266 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltType (yyP60);
/* line 2267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltType (yyP60);
/* line 2268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ALTEND();");
/* line 2272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   endlab = NewLabel ();
/* line 2273 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kNoSpec)
   GenCodeAltType (yyP60, endlab, yyV1);
/* line 2274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (endlab);
/* line 2275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->AltType.SpecList);
  }
   return;
 }

  case kCaseIn:
/* line 2279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   le = NewLabel ();
/* line 2282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->CaseIn.SpecList);
/* line 2283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->CaseIn.Pos);
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INPUT1(");
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP60->CaseIn.Element, false);
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->_Header.Temp.VBYTE,");
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (le);
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (le);
/* line 2287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   le = NewLabel ();
/* line 2288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("switch(FP->_Header.Temp.VBYTE)");
/* line 2288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2290 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2290 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInSwitches (yyP60->CaseIn.CaseList);
/* line 2291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2293 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInCode (yyP60->CaseIn.CaseList, yyP60->CaseIn.Element, le);
/* line 2294 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (le);
/* line 2295 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->CaseIn.SpecList);
  }
   return;

  case kPar:
  if (yyP60->Par.ProcList->Kind == kNoProcess) {
/* line 2299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2301 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Par.SpecList);
/* line 2302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Par.Pos);
/* line 2303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Par.SpecList);
  }
   return;

  }
/* line 2306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP60->Par.PType == xxPlacedPar))) goto yyL24;
  {
/* line 2306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((UsesFreeVars (yyP60)))) goto yyL24;
  {
/* line 2306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxNoFreeVars)))) goto yyL24;
  {
/* line 2308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("PLACED PAR uses free variables", xxError, yyP60->Par.Pos);
  }
  }
  }
  }
   return;
yyL24:;

/* line 2312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP60->Par.PType == xxPlacedPar))) goto yyL25;
  {
/* line 2314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Par.SpecList);
/* line 2316 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Par.Pos);
/* line 2317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlacedList (yyP60->Par.ProcList, NoSymbol, NoTree);
/* line 2318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("WAITP(");
/* line 2318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Par.SpecList);
  }
  }
   return;
yyL25:;

/* line 2324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2326 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2327 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP60->Par.SpecList);
/* line 2328 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->Par.Pos);
/* line 2329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParList (yyP60->Par.ProcList, yyP60->Par.PType ? HIPRI : LOPRI);
/* line 2330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("WAITP(");
/* line 2330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2331 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2332 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->Par.SpecList);
  }
   return;

  case kParRep:
/* line 2336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP60->ParRep.Replicator->Replicator.For)))) goto yyL27;
  {
/* line 2336 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((ValConstInt (yyP60->ParRep.Replicator->Replicator.For) == 0))) goto yyL27;
  }
  }
   return;
yyL27:;

/* line 2339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP60->ParRep.PType == xxPlacedPar))) goto yyL28;
  {
/* line 2339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((UsesFreeVars (yyP60)))) goto yyL28;
  {
/* line 2339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxNoFreeVars)))) goto yyL28;
  {
/* line 2341 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("PLACED PAR uses free variables", xxError, yyP60->ParRep.Pos);
  }
  }
  }
  }
   return;
yyL28:;

/* line 2345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP60->ParRep.PType == xxPlacedPar))) goto yyL29;
  {
/* line 2347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->ParRep.SpecList, yyP60->ParRep.Replicator, false);
/* line 2349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->ParRep.Pos);
/* line 2350 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP60->ParRep.Replicator);
/* line 2351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlaced (yyP60->ParRep.Process, yyP60->ParRep.Replicator->Replicator.Sym, yyP60->ParRep.Replicator->Replicator.From);
/* line 2353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->ParRep.Pos);
/* line 2354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->ParRep.SpecList);
/* line 2356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("WAITP(");
/* line 2356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2356 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2357 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->ParRep.SpecList);
  }
  }
   return;
yyL29:;

  if (yyP60->ParRep.Process->Kind == kInstance) {
/* line 2362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP60->ParRep.SpecList, yyP60->ParRep.Replicator, false);
/* line 2366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->ParRep.Pos);
/* line 2367 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP60->ParRep.Replicator);
/* line 2368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2369 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP60->ParRep.PType == xxPriPar))) goto yyL30;
  {
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP60->ParRep.Replicator->Replicator.Sym);
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (ValConstInt (yyP60->ParRep.Replicator->Replicator.From));
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") ");
/* line 2370 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParProc (yyP60->ParRep.Process, yyP60->ParRep.Replicator->Replicator.Sym, HIPRI, yyP60->ParRep.Replicator->Replicator.From);
/* line 2373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->ParRep.Pos);
/* line 2375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("else ");
/* line 2375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL30;
  }
  }
   return;
yyL30:;

/* line 2380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2383 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParProc (yyP60->ParRep.Process, yyP60->ParRep.Replicator->Replicator.Sym, LOPRI, yyP60->ParRep.Replicator->Replicator.From);
/* line 2384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP60->ParRep.Pos);
/* line 2385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2385 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP60->ParRep.SpecList);
/* line 2387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("WAITP(");
/* line 2387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2388 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2389 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP60->ParRep.SpecList);
  }
   return;

  }
  break;
  }

/* line 2396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP60, yyP60->Process.Pos);
  }
   return;

;
}

static void GenProcList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP61)
# else
(yyP61)
 register tTree yyP61;
# endif
{
  if (yyP61->Kind == kAProcess) {
/* line 2400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP61->AProcess.Process);
/* line 2400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcList (yyP61->AProcess.Next);
  }
   return;

  }
  if (yyP61->Kind == kNoProcess) {
/* line 2401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenProcList");
;
}

static void GenParList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP63, register int yyP62)
# else
(yyP63, yyP62)
 register tTree yyP63;
 register int yyP62;
# endif
{
  if (yyP63->Kind == kAProcess) {
  if (yyP63->AProcess.Process->Kind == kInstance) {
/* line 2405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2406 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParProc (yyP63->AProcess.Process, NoSymbol, yyP62, NoTree);
/* line 2407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParList (yyP63->AProcess.Next, LOPRI);
  }
   return;

  }
  }
  if (yyP63->Kind == kNoProcess) {
/* line 2410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenParList");
;
}

static void GenParProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP67, tSymbol yyP66, register int yyP65, register tTree yyP64)
# else
(yyP67, yyP66, yyP65, yyP64)
 register tTree yyP67;
 tSymbol yyP66;
 register int yyP65;
 register tTree yyP64;
# endif
{
/* line 2415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tTree Frame; 
/* line 2417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP66 != NoSymbol))) goto yyL1;
  {
/* line 2418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParProcParam (Symbol (yyP67->Instance.Sym) . Kind -> DefProc . Formals, yyP67->Instance.ExprList, yyP67 -> Instance . Frame -> ParFrame . FId, yyP66, yyP64);
/* line 2419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 2422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Frame = yyP67 -> Instance . Frame;
/* line 2424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP67->Instance.Pos);
/* line 2425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("STARTP(");
/* line 2426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
   return;
yyL2:;

/* line 2429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP66 == NoSymbol))) goto yyL3;
  {
/* line 2430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&FP->");
/* line 2430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . TId);
/* line 2430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP67->Instance.Sym);
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->");
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . FId);
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",\"");
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (Frame -> CallFrame . Sym);
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\",");
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP65);
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL3:;

/* line 2434 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&FP->");
/* line 2435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . TId);
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP66) . Kind, yyP66, false);
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP64);
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP67->Instance.Sym);
/* line 2437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->");
/* line 2437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . FId);
/* line 2438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 2438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP66) . Kind, yyP66, false);
/* line 2438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP64);
/* line 2438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",\"");
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (Frame -> CallFrame . Sym);
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\",");
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP65);
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenPlacedList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP70, tSymbol yyP69, register tTree yyP68)
# else
(yyP70, yyP69, yyP68)
 register tTree yyP70;
 tSymbol yyP69;
 register tTree yyP68;
# endif
{
  if (yyP70->Kind == kAProcess) {
/* line 2446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlaced (yyP70->AProcess.Process, yyP69, yyP68);
/* line 2446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlacedList (yyP70->AProcess.Next, yyP69, yyP68);
  }
   return;

  }
  if (yyP70->Kind == kNoProcess) {
/* line 2447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenPlacedList");
;
}

static void GenPlaced
# if defined __STDC__ | defined __cplusplus
(register tTree yyP73, tSymbol yyP72, register tTree yyP71)
# else
(yyP73, yyP72, yyP71)
 register tTree yyP73;
 tSymbol yyP72;
 register tTree yyP71;
# endif
{
/* line 2451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tFrame Frame; 
  if (yyP73->Kind == kPar) {
/* line 2453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP73->Par.SpecList);
/* line 2456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP73->Par.Pos);
/* line 2457 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlacedList (yyP73->Par.ProcList, yyP72, yyP71);
/* line 2458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP73->Par.SpecList);
  }
   return;

  }
  if (yyP73->Kind == kParRep) {
/* line 2462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP72 == NoSymbol))) goto yyL2;
  {
/* line 2464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP73->ParRep.SpecList, yyP73->ParRep.Replicator, false);
/* line 2465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP73->ParRep.Pos);
/* line 2466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP73->ParRep.Replicator);
/* line 2467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenPlaced (yyP73->ParRep.Process, yyP73->ParRep.Replicator->Replicator.Sym, yyP73->ParRep.Replicator->Replicator.From);
/* line 2469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP73->ParRep.Pos);
/* line 2470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP73->ParRep.SpecList);
  }
  }
   return;
yyL2:;

/* line 2475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2476 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP73, yyP73->ParRep.Pos);
  }
   return;

  }
  if (yyP73->Kind == kProcessor) {
  if (yyP73->Processor.Process->Kind == kInstance) {
/* line 2479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP72 != NoSymbol))) goto yyL4;
  {
/* line 2480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParProcParam (Symbol (yyP73->Processor.Process->Instance.Sym) . Kind -> DefProc . Formals, yyP73->Processor.Process->Instance.ExprList, yyP73->Processor.Process -> Instance . Frame -> ParFrame . FId, yyP72, yyP71);
/* line 2481 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL4;
  }
  }
   return;
yyL4:;

/* line 2484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Frame = yyP73->Processor.Process -> Instance . Frame;
/* line 2486 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP73->Processor.Process->Instance.Pos);
/* line 2487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("PLACED_STARTP(");
/* line 2488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodePlacedExpr (yyP73->Processor.Expr, yyP72, yyP71);
/* line 2488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL5;
  }
   return;
yyL5:;

/* line 2492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP72 == NoSymbol))) goto yyL6;
  {
/* line 2493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&FP->");
/* line 2493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . TId);
/* line 2493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP73->Processor.Process->Instance.Sym);
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->");
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . FId);
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",\"");
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (Frame -> CallFrame . Sym);
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\");");
/* line 2494 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL6:;

/* line 2497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&FP->");
/* line 2498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . TId);
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP72);
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP71);
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP73->Processor.Process->Instance.Sym);
/* line 2500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->");
/* line 2500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Frame -> ParFrame . FId);
/* line 2501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 2501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP72);
/* line 2501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP71);
/* line 2501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 2502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",\"");
/* line 2502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (Frame -> CallFrame . Sym);
/* line 2502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\");");
/* line 2502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
/* line 2505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP73, yyP73->Process.Pos);
  }
   return;

;
}

static void CodePlacedExpr
# if defined __STDC__ | defined __cplusplus
(register tTree EX, tSymbol yyP75, register tTree yyP74)
# else
(EX, yyP75, yyP74)
 register tTree EX;
 tSymbol yyP75;
 register tTree yyP74;
# endif
{
  if (EX->Kind == kElementalExpr) {
  if (Tree_IsType (EX->ElementalExpr.Elemental, kIdentifier)) {
/* line 2510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP75);
/* line 2511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP74);
  }
   return;

  }
  }
  if (EX->Kind == kConst) {
/* line 2514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2515 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (EX);
  }
   return;

  }
/* line 2518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (EX, EX->Expr.Pos);
  }
   return;

;
}

static void GenParProcParam
# if defined __STDC__ | defined __cplusplus
(register tTree yyP80, register tTree yyP79, register tIdent yyP78, tSymbol yyP77, register tTree yyP76)
# else
(yyP80, yyP79, yyP78, yyP77, yyP76)
 register tTree yyP80;
 register tTree yyP79;
 register tIdent yyP78;
 tSymbol yyP77;
 register tTree yyP76;
# endif
{
  if (yyP80->Kind == kAFormal) {
  if (yyP80->AFormal.By->Kind == kByVal) {
  if (yyP79->Kind == kAnExpr) {
/* line 2526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP78);
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP77) . Kind, yyP77, false);
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("-");
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP76);
/* line 2527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP80->AFormal.Sym);
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP79->AnExpr.Expr);
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 2528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
  }
 yyAbort ("GenParProcParam");
;
}

static void GenEnableAltType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP81)
# else
(yyP81)
 register tTree yyP81;
# endif
{
  if (yyP81->Kind == kAlt) {
/* line 2536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltList (yyP81->Alt.AltList);
  }
   return;

  }
  if (yyP81->Kind == kAltRep) {
/* line 2538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP81->AltRep.Pos);
/* line 2540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP81->AltRep.Replicator);
/* line 2541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2542 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltList (yyP81->AltRep.AltList);
/* line 2543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP81->AltRep.Pos);
/* line 2544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenEnableAltType");
;
}

static void GenEnableAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP82)
# else
(yyP82)
 register tTree yyP82;
# endif
{
  if (yyP82->Kind == kNoAlt) {
/* line 2550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP82->Kind == kAnAlt) {
/* line 2552 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP82->AnAlt.SpecList);
/* line 2554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableGuard (yyP82->AnAlt.Guard);
/* line 2555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltList (yyP82->AnAlt.Next);
/* line 2556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP82->AnAlt.SpecList);
  }
   return;

  }
  if (yyP82->Kind == kANewAlt) {
/* line 2559 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP82->ANewAlt.SpecList);
/* line 2561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (yyP82->ANewAlt.AltType->AltType.SpecList);
/* line 2562 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltType (yyP82->ANewAlt.AltType);
/* line 2563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEnableAltList (yyP82->ANewAlt.Next);
/* line 2564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (yyP82->ANewAlt.AltType->AltType.SpecList);
/* line 2565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP82->ANewAlt.SpecList);
  }
   return;

  }
 yyAbort ("GenEnableAltList");
;
}

static void GenEnableGuard
# if defined __STDC__ | defined __cplusplus
(register tTree yyP83)
# else
(yyP83)
 register tTree yyP83;
# endif
{
  if (yyP83->Kind == kAltInput) {
/* line 2571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimer (yyP83->AltInput.Input->Input.Element)))) goto yyL1;
  {
/* line 2572 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENBS(");
/* line 2572 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->AltInput.Expr);
/* line 2573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL1:;

/* line 2576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENBC(");
/* line 2577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->AltInput.Expr);
/* line 2578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP83->AltInput.Input->Input.Element, false);
/* line 2579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP83->Kind == kAltTimer) {
/* line 2582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENBT(");
/* line 2583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->AltTimer.Expr);
/* line 2584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->AltTimer.After->After.Expr);
/* line 2585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP83->Kind == kAltCaseIn) {
/* line 2588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENBC(");
/* line 2589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->AltCaseIn.Expr);
/* line 2590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP83->AltCaseIn.CaseIn->CaseIn.Element, false);
/* line 2591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP83->Kind == kNoInput) {
/* line 2594 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ENBS(");
/* line 2595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP83->NoInput.Expr);
/* line 2596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenEnableGuard");
;
}

static void GenDisableAltType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP84)
# else
(yyP84)
 register tTree yyP84;
# endif
{
  if (yyP84->Kind == kAlt) {
/* line 2603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltList (yyP84->Alt.AltList);
  }
   return;

  }
  if (yyP84->Kind == kAltRep) {
/* line 2605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BOOL OLDTMP = TMP;");
/* line 2607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP84->AltRep.Replicator);
/* line 2609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltList (yyP84->AltRep.AltList);
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (TMP && (!OLDTMP))");
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAltSelect (yyP84->AltRep.Replicator);
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAltIndex (yyP84->AltRep.Replicator);
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 2611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OLDTMP=TMP;");
/* line 2612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenDisableAltType");
;
}

static void GenDisableAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP85)
# else
(yyP85)
 register tTree yyP85;
# endif
{
  if (yyP85->Kind == kNoAlt) {
/* line 2621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP85->Kind == kAnAlt) {
/* line 2623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP85->AnAlt.SpecList);
/* line 2625 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP85 -> AnAlt . Label = NewLabel ();
/* line 2626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableGuard (yyP85->AnAlt.Guard, yyP85 -> AnAlt . Label);
/* line 2627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltList (yyP85->AnAlt.Next);
/* line 2628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP85->AnAlt.SpecList);
  }
   return;

  }
  if (yyP85->Kind == kANewAlt) {
/* line 2631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP85->ANewAlt.SpecList);
/* line 2633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (yyP85->ANewAlt.AltType->AltType.SpecList);
/* line 2634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltType (yyP85->ANewAlt.AltType);
/* line 2635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDisableAltList (yyP85->ANewAlt.Next);
/* line 2636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (yyP85->ANewAlt.AltType->AltType.SpecList);
/* line 2637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP85->ANewAlt.SpecList);
  }
   return;

  }
 yyAbort ("GenDisableAltList");
;
}

static void GenDisableGuard
# if defined __STDC__ | defined __cplusplus
(register tTree yyP87, register int yyP86)
# else
(yyP87, yyP86)
 register tTree yyP87;
 register int yyP86;
# endif
{
  if (yyP87->Kind == kAltInput) {
/* line 2643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimer (yyP87->AltInput.Input->Input.Element)))) goto yyL1;
  {
/* line 2644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP |= ");
/* line 2645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DISS(");
/* line 2645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP86);
/* line 2646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->AltInput.Expr);
/* line 2647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL1:;

/* line 2650 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2651 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP |= ");
/* line 2652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DISC(");
/* line 2652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP86);
/* line 2653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->AltInput.Expr);
/* line 2654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP87->AltInput.Input->Input.Element, false);
/* line 2655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP87->Kind == kAltTimer) {
/* line 2658 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP |= ");
/* line 2660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DIST(");
/* line 2660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP86);
/* line 2661 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2661 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->AltTimer.Expr);
/* line 2662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->AltTimer.After->After.Expr);
/* line 2663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP87->Kind == kAltCaseIn) {
/* line 2666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2667 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP |= ");
/* line 2668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DISC(");
/* line 2668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP86);
/* line 2669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->AltCaseIn.Expr);
/* line 2670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP87->AltCaseIn.CaseIn->CaseIn.Element, false);
/* line 2671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP87->Kind == kNoInput) {
/* line 2674 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP |= ");
/* line 2676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("DISS(");
/* line 2676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP86);
/* line 2677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 2677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP87->NoInput.Expr);
/* line 2678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenDisableGuard");
;
}

static void GenAssignRepsAltType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP88)
# else
(yyP88)
 register tTree yyP88;
# endif
{
  if (yyP88->Kind == kAlt) {
/* line 2685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltList (yyP88->Alt.AltList);
  }
   return;

  }
  if (yyP88->Kind == kAltRep) {
/* line 2687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAltIndex (yyP88->AltRep.Replicator);
/* line 2688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 2688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAltSelect (yyP88->AltRep.Replicator);
/* line 2688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 2688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltList (yyP88->AltRep.AltList);
  }
   return;

  }
 yyAbort ("GenAssignRepsAltType");
;
}

static void GenAssignRepsAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP89)
# else
(yyP89)
 register tTree yyP89;
# endif
{
  if (yyP89->Kind == kNoAlt) {
/* line 2695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP89->Kind == kAnAlt) {
/* line 2697 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltList (yyP89->AnAlt.Next);
  }
   return;

  }
  if (yyP89->Kind == kANewAlt) {
/* line 2701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltType (yyP89->ANewAlt.AltType);
/* line 2703 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAssignRepsAltList (yyP89->ANewAlt.Next);
  }
   return;

  }
 yyAbort ("GenAssignRepsAltList");
;
}

static tTree BuildRevSpecList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP91, register tTree yyP90)
# else
(yyP91, yyP90)
 register tTree yyP91;
 register tTree yyP90;
# endif
{
  if (yyP90->Kind == kNoSpec) {
/* line 2713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP91;

  }
  if (yyP90->Kind == kASpec) {
/* line 2714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 {
  {
  register tTree yyV1;
   yyALLOC (tTree,Tree_PoolFreePtr,Tree_PoolMaxPtr,Tree_Alloc,Tree_NodeSize,MakeTree,yyV1,kASpec)
    yyV1->ASpec.Spec = yyP90->ASpec.Spec;
    yyV1->ASpec.Next = yyP91;
   return BuildRevSpecList (yyV1, yyP90->ASpec.Next);
  }
 }

  }
 yyAbort ("BuildRevSpecList");
}

static void GenCodeAltType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP94, register int yyP93, register tTree yyP92)
# else
(yyP94, yyP93, yyP92)
 register tTree yyP94;
 register int yyP93;
 register tTree yyP92;
# endif
{
  if (yyP94->Kind == kAlt) {
/* line 2720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeAltList (yyP94->Alt.AltList, yyP93, yyP92);
  }
   return;

  }
  if (yyP94->Kind == kAltRep) {
/* line 2722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeAltList (yyP94->AltRep.AltList, yyP93, yyP92);
  }
   return;

  }
 yyAbort ("GenCodeAltType");
;
}

static void GenCodeAltList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP97, register int yyP96, register tTree yyP95)
# else
(yyP97, yyP96, yyP95)
 register tTree yyP97;
 register int yyP96;
 register tTree yyP95;
# endif
{
/* line 2734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tTree UnreversedSL; 
  if (yyP97->Kind == kNoAlt) {
/* line 2736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP97->Kind == kAnAlt) {
/* line 2738 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (yyP97 -> AnAlt . Label);
/* line 2740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   UnreversedSL = ReverseTree (yyP95);
/* line 2741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (UnreversedSL);
/* line 2742 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP97->AnAlt.SpecList);
/* line 2743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeGuard (yyP97->AnAlt.Guard);
/* line 2744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP97->AnAlt.SpecList);
/* line 2745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (UnreversedSL);
/* line 2746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 2746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP96);
/* line 2746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReverseTree (UnreversedSL);
/* line 2748 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeAltList (yyP97->AnAlt.Next, yyP96, yyP95);
  }
   return;

  }
  if (yyP97->Kind == kANewAlt) {
/* line 2751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2752 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeAltType (yyP97->ANewAlt.AltType, yyP96, BuildRevSpecList (BuildRevSpecList (yyP95, yyP97->ANewAlt.SpecList), yyP97->ANewAlt.AltType->AltType.SpecList));
/* line 2753 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCodeAltList (yyP97->ANewAlt.Next, yyP96, yyP95);
  }
   return;

  }
 yyAbort ("GenCodeAltList");
;
}

static void GenCodeGuard
# if defined __STDC__ | defined __cplusplus
(register tTree yyP98)
# else
(yyP98)
 register tTree yyP98;
# endif
{
/* line 2758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
  if (yyP98->Kind == kAltInput) {
/* line 2760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP98->AltInput.Input->Input.SpecList);
/* line 2762 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP98->AltInput.Input->Input.Element, ElementType (yyP98->AltInput.Input->Input.Element), yyP98->AltInput.Input->Input.ExprList);
/* line 2763 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP98->AltInput.Process);
/* line 2764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP98->AltInput.Input->Input.SpecList);
  }
   return;

  }
  if (yyP98->Kind == kAltTimer) {
/* line 2766 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2767 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP98->AltTimer.After->After.SpecList);
/* line 2768 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP98->AltTimer.Process);
/* line 2769 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP98->AltTimer.After->After.SpecList);
  }
   return;

  }
  if (yyP98->Kind == kAltCaseIn) {
/* line 2771 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2772 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2773 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP98->AltCaseIn.CaseIn->CaseIn.SpecList);
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INPUT1(");
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP98->AltCaseIn.CaseIn->CaseIn.Element, false);
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->_Header.Temp.VBYTE,");
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2776 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 2778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("switch(FP->_Header.Temp.VBYTE)");
/* line 2778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2779 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2779 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInSwitches (yyP98->AltCaseIn.CaseIn->CaseIn.CaseList);
/* line 2781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInCode (yyP98->AltCaseIn.CaseIn->CaseIn.CaseList, yyP98->AltCaseIn.CaseIn->CaseIn.Element, lab);
/* line 2784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 2785 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP98->AltCaseIn.CaseIn->CaseIn.SpecList);
  }
   return;

  }
  if (yyP98->Kind == kNoInput) {
/* line 2787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP98->NoInput.Process);
  }
   return;

  }
 yyAbort ("GenCodeGuard");
;
}

static void GenIfType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP100, register int yyP99)
# else
(yyP100, yyP99)
 register tTree yyP100;
 register int yyP99;
# endif
{
/* line 2797 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int le,ls; 
  if (yyP100->Kind == kIf) {
/* line 2799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2800 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP100->If.SpecList);
/* line 2801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP100->If.Pos);
/* line 2802 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfList (yyP100->If.ChoiceList, yyP99);
/* line 2803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP100->If.SpecList);
  }
   return;

  }
  if (yyP100->Kind == kIfRep) {
/* line 2806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   le = GenRepStart (yyP100->IfRep.Replicator);
/* line 2808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ls = NewLabel ();
/* line 2809 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (ls);
/* line 2810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP100->IfRep.SpecList);
/* line 2811 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP100->IfRep.Pos);
/* line 2812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfList (yyP100->IfRep.ChoiceList, yyP99);
/* line 2813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenRepEnd (yyP100->IfRep.Replicator, ls, le);
/* line 2814 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP100->IfRep.SpecList);
  }
   return;

  }
 yyAbort ("GenIfType");
;
}

static void GenIfList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP102, register int yyP101)
# else
(yyP102, yyP101)
 register tTree yyP102;
 register int yyP101;
# endif
{
/* line 2819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int nlab; 
  if (yyP102->Kind == kAChoice) {
  if (yyP102->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP102->AChoice.ExprList->AnExpr.Next->Kind == kNoExpr) {
/* line 2821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2822 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   nlab = NewLabel ();
/* line 2823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP102->AChoice.SpecList);
/* line 2824 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP102->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (!");
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP102->AChoice.ExprList->AnExpr.Expr);
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") GOTO(");
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (nlab);
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP102->AChoice.Process);
/* line 2827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP102->AChoice.SpecList);
/* line 2828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 2828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP101);
/* line 2828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (nlab);
/* line 2830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfList (yyP102->AChoice.Next, yyP101);
  }
   return;

  }
  }
  }
  if (yyP102->Kind == kANewIf) {
/* line 2833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2834 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP102->ANewIf.SpecList);
/* line 2835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfType (yyP102->ANewIf.IfType, yyP101);
/* line 2836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfList (yyP102->ANewIf.Next, yyP101);
/* line 2837 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP102->ANewIf.SpecList);
  }
   return;

  }
  if (yyP102->Kind == kNoChoice) {
/* line 2840 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenIfList");
;
}

static void GenIfTypeAtomic
# if defined __STDC__ | defined __cplusplus
(register tTree yyP104, register bool yyP103)
# else
(yyP104, yyP103)
 register tTree yyP104;
 register bool yyP103;
# endif
{
  if (yyP104->Kind == kIf) {
/* line 2849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP104->If.SpecList, NoTree, false);
/* line 2851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP104->If.Pos);
/* line 2852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfListAtomic (yyP104, yyP104->If.ChoiceList, yyP103);
/* line 2853 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP104->If.SpecList);
  }
   return;

  }
  if (yyP104->Kind == kIfRep) {
/* line 2856 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP104->IfRep.SpecList, yyP104->IfRep.Replicator, false);
/* line 2858 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP104->IfRep.Pos);
/* line 2859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForLoop (yyP104->IfRep.Replicator);
/* line 2860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfListAtomic (yyP104, yyP104->IfRep.ChoiceList, true);
/* line 2862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenForCheck (yyP104->IfRep.Replicator);
/* line 2865 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
   return;
yyL2:;

/* line 2867 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2867 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((BackIfType (yyP104) != NULL))) goto yyL3;
  {
/* line 2868 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfListAtomic (BackIfType (yyP104), BackChoiceList (yyP104), yyP103);
/* line 2869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP104->IfRep.SpecList);
  }
  }
   return;
yyL3:;

  if (equalbool (yyP103, false)) {
/* line 2872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("SETERR(MSG_IF);");
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2873 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP104->IfRep.SpecList);
  }
   return;

  }
  if (equalbool (yyP103, true)) {
/* line 2877 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("continue;");
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP104->IfRep.SpecList);
  }
   return;

  }
  }
 yyAbort ("GenIfTypeAtomic");
;
}

static void GenIfListAtomic
# if defined __STDC__ | defined __cplusplus
(register tTree yyP107, register tTree yyP106, register bool yyP105)
# else
(yyP107, yyP106, yyP105)
 register tTree yyP107;
 register tTree yyP106;
 register bool yyP105;
# endif
{
  if (yyP106->Kind == kAChoice) {
  if (yyP106->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP106->AChoice.ExprList->AnExpr.Next->Kind == kNoExpr) {
/* line 2888 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP106->AChoice.SpecList, NoTree, false);
/* line 2890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP106->AChoice.ExprList->AnExpr.Expr->Expr.Pos);
/* line 2891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 2891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP106->AChoice.ExprList->AnExpr.Expr);
/* line 2891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 2891 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP106->AChoice.Process);
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2892 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2893 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("else");
/* line 2893 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfListAtomic (yyP107, yyP106->AChoice.Next, yyP105);
/* line 2895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP106->AChoice.SpecList);
  }
   return;

  }
  }
  }
  if (yyP106->Kind == kANewIf) {
  if (yyP106->ANewIf.IfType->Kind == kIf) {
/* line 2898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP106->ANewIf.SpecList, NoTree, false);
/* line 2900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP106->ANewIf.IfType->If.Pos);
/* line 2901 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BackIfType (yyP106->ANewIf.IfType) = yyP107;
/* line 2902 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BackChoiceList (yyP106->ANewIf.IfType) = yyP106->ANewIf.Next;
/* line 2903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfTypeAtomic (yyP106->ANewIf.IfType, yyP105);
/* line 2904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP106->ANewIf.SpecList);
  }
   return;

  }
  if (yyP106->ANewIf.IfType->Kind == kIfRep) {
/* line 2907 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP106->ANewIf.SpecList, NoTree, false);
/* line 2909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP106->ANewIf.IfType->IfRep.Pos);
/* line 2910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BackIfType (yyP106->ANewIf.IfType) = yyP107;
/* line 2911 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BackChoiceList (yyP106->ANewIf.IfType) = yyP106->ANewIf.Next;
/* line 2912 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfTypeAtomic (yyP106->ANewIf.IfType, yyP105);
/* line 2913 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP106->ANewIf.SpecList);
  }
   return;

  }
  }
  if (yyP106->Kind == kNoChoice) {
/* line 2916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((BackIfType (yyP107) != NULL))) goto yyL4;
  {
/* line 2916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP107, kIf)))) goto yyL4;
  {
/* line 2917 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIfListAtomic (BackIfType (yyP107), BackChoiceList (yyP107), yyP105);
  }
  }
  }
   return;
yyL4:;

  if (equalbool (yyP105, false)) {
/* line 2920 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("SETERR(MSG_IF);");
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (equalbool (yyP105, true)) {
/* line 2924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("continue;");
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 2925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
 yyAbort ("GenIfListAtomic");
;
}

static void GenCaseSwitches
# if defined __STDC__ | defined __cplusplus
(register tTree yyP109, register bool yyP108)
# else
(yyP109, yyP108)
 register tTree yyP109;
 register bool yyP108;
# endif
{
/* line 2932 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int nlab; 
  if (yyP109->Kind == kAChoice) {
  if (yyP109->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP109->AChoice.ExprList->AnExpr.Expr->Kind == kDefault) {
/* line 2934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   nlab = NewLabel ();
/* line 2936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: GOTO(");
/* line 2936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (nlab);
/* line 2936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP109 -> AChoice . Label = nlab;
/* line 2938 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseSwitches (yyP109->AChoice.Next, true);
  }
   return;

  }
  }
/* line 2941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2942 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   nlab = NewLabel ();
/* line 2943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseExprList (yyP109->AChoice.ExprList);
/* line 2944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO (");
/* line 2944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (nlab);
/* line 2944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP109 -> AChoice . Label = nlab;
/* line 2946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseSwitches (yyP109->AChoice.Next, yyP108);
  }
   return;

  }
  if (yyP109->Kind == kNoChoice) {
  if (equalbool (yyP108, false)) {
/* line 2949 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2950 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: SETERR(MSG_CASE);");
/* line 2950 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 2952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenCaseSwitches");
;
}

static void GenCaseExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP110)
# else
(yyP110)
 register tTree yyP110;
# endif
{
  if (yyP110->Kind == kAnExpr) {
/* line 2957 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("case ");
/* line 2958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP110->AnExpr.Expr);
/* line 2958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (": ");
/* line 2959 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseExprList (yyP110->AnExpr.Next);
  }
   return;

  }
  if (yyP110->Kind == kNoExpr) {
/* line 2961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenCaseExprList");
;
}

static void GenCaseCode
# if defined __STDC__ | defined __cplusplus
(register tTree yyP112, register int yyP111)
# else
(yyP112, yyP111)
 register tTree yyP112;
 register int yyP111;
# endif
{
  if (yyP112->Kind == kAChoice) {
/* line 2965 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (yyP112 -> AChoice . Label);
/* line 2967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP112->AChoice.SpecList);
/* line 2968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP112->AChoice.Process);
/* line 2969 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP112->AChoice.SpecList);
/* line 2970 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 2970 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP111);
/* line 2970 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 2970 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseCode (yyP112->AChoice.Next, yyP111);
  }
   return;

  }
  if (yyP112->Kind == kNoChoice) {
/* line 2974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenCaseCode");
;
}

static void GenCaseAtomic
# if defined __STDC__ | defined __cplusplus
(register tTree yyP114, register bool yyP113)
# else
(yyP114, yyP113)
 register tTree yyP114;
 register bool yyP113;
# endif
{
  if (yyP114->Kind == kAChoice) {
  if (yyP114->AChoice.ExprList->Kind == kAnExpr) {
  if (yyP114->AChoice.ExprList->AnExpr.Expr->Kind == kDefault) {
/* line 2979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: ");
/* line 2980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP114->AChoice.SpecList, NoTree, false);
/* line 2982 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP114->AChoice.Process);
/* line 2983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP114->AChoice.SpecList);
/* line 2984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2985 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseAtomic (yyP114->AChoice.Next, true);
  }
   return;

  }
  }
/* line 2988 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2989 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseExprList (yyP114->AChoice.ExprList);
/* line 2989 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2990 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginAtomicBlock (yyP114->AChoice.SpecList, NoTree, false);
/* line 2991 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP114->AChoice.Process);
/* line 2992 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndAtomicBlock (yyP114->AChoice.SpecList);
/* line 2993 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("break;");
/* line 2993 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 2994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseAtomic (yyP114->AChoice.Next, yyP113);
  }
   return;

  }
  if (yyP114->Kind == kNoChoice) {
  if (equalbool (yyP113, false)) {
/* line 2997 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 2998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: SETERR(MSG_CASE);");
/* line 2998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 3000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenCaseAtomic");
;
}

static void GenCaseInSwitches
# if defined __STDC__ | defined __cplusplus
(register tTree yyP115)
# else
(yyP115)
 register tTree yyP115;
# endif
{
/* line 3005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int nlab; 
  if (yyP115->Kind == kACase) {
  if (yyP115->ACase.Next->Kind == kNoCase) {
  if (yyP115->ACase.ExprList->Kind == kAnExpr) {
  if (yyP115->ACase.ExprList->AnExpr.Expr->Kind == kDefault) {
/* line 3010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   nlab = NewLabel ();
/* line 3012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: GOTO(");
/* line 3012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (nlab);
/* line 3012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3013 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP115 -> ACase . Label = nlab;
  }
   return;

  }
  }
  }
  if (yyP115->ACase.ExprList->Kind == kAnExpr) {
  if (yyP115->ACase.ExprList->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP115->ACase.ExprList->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3016 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3017 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   nlab = NewLabel ();
/* line 3018 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("case ");
/* line 3018 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP115->ACase.ExprList->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 3018 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (": ");
/* line 3019 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO (");
/* line 3019 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (nlab);
/* line 3019 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3019 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3020 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyP115 -> ACase . Label = nlab;
/* line 3021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInSwitches (yyP115->ACase.Next);
  }
   return;

  }
  }
  }
  }
  if (yyP115->Kind == kNoCase) {
/* line 3024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("default: SETERR(MSG_CASEIN);");
/* line 3025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenCaseInSwitches");
;
}

static void GenCaseInCode
# if defined __STDC__ | defined __cplusplus
(register tTree yyP118, register tTree yyP117, register int yyP116)
# else
(yyP118, yyP117, yyP116)
 register tTree yyP118;
 register tTree yyP117;
 register int yyP116;
# endif
{
  if (yyP118->Kind == kACase) {
  if (yyP118->ACase.ExprList->Kind == kAnExpr) {
/* line 3031 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (yyP118 -> ACase . Label);
/* line 3033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   BeginBlock (yyP118->ACase.SpecList);
/* line 3034 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP118->ACase.ExprList->AnExpr.Expr->Expr.Pos);
/* line 3035 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP117, ElementType (yyP117), yyP118->ACase.ExprList->AnExpr.Next);
/* line 3036 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcess (yyP118->ACase.Process);
/* line 3037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   EndBlock (yyP118->ACase.SpecList);
/* line 3038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 3038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP116);
/* line 3038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3039 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCaseInCode (yyP118->ACase.Next, yyP117, yyP116);
  }
   return;

  }
  }
  if (yyP118->Kind == kNoCase) {
/* line 3042 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenCaseInCode");
;
}

static void GenSetScope
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP119)
# else
(yyP119)
 tSymbol yyP119;
# endif
{
/* line 3049 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3049 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxOccamDebug)))) goto yyL1;
  {
/* line 3049 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP119 <= OccSymSize))) goto yyL1;
  {
/* line 3050 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr ("SETSCOPE(");
/* line 3051 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP119 - 1);
/* line 3052 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3053 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL1:;

/* line 3055 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenSetListScope
# if defined __STDC__ | defined __cplusplus
(register tTree yyP120)
# else
(yyP120)
 register tTree yyP120;
# endif
{
  if (yyP120->Kind == kAnId) {
  if (yyP120->AnId.Next->Kind == kNoId) {
/* line 3059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP120->AnId.Sym);
  }
   return;

  }
/* line 3060 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3060 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (yyP120->AnId.Next);
  }
   return;

  }
  if (yyP120->Kind == kNoId) {
/* line 3061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP120->Kind == kAFormal) {
  if (Tree_IsType (yyP120->AFormal.Next, kNoFormal)) {
/* line 3062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP120->AFormal.Sym);
  }
   return;

  }
/* line 3063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (yyP120->AFormal.Next);
  }
   return;

  }
  if (Tree_IsType (yyP120, kNoFormal)) {
/* line 3064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenSetListScope");
;
}

static void GenLocalSpecs
# if defined __STDC__ | defined __cplusplus
(register tTree yyP121)
# else
(yyP121)
 register tTree yyP121;
# endif
{
  if (yyP121->Kind == kASpec) {
/* line 3069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP121->ASpec.Spec->Spec.Pos);
/* line 3069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalSpec (yyP121->ASpec.Spec);
/* line 3069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalSpecs (yyP121->ASpec.Next);
  }
   return;

  }
  if (yyP121->Kind == kNoSpec) {
/* line 3070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenLocalSpecs");
;
}

static void GenLocalSpec
# if defined __STDC__ | defined __cplusplus
(register tTree SP)
# else
(SP)
 register tTree SP;
# endif
{
  if (SP->Kind == kDecl) {
/* line 3076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalIdList (SP->Decl.Type, SP->Decl.IdList);
/* line 3076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
   return;
yyL1:;

  }
  if (SP->Kind == kVal) {
/* line 3078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (SP->Val.Sym)))) goto yyL2;
  {
/* line 3078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSpecState (SP);
  }
  }
   return;
yyL2:;

  }
  if (Tree_IsType (SP, kValRetypes)) {
/* line 3079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (SP->ValRetypes.Sym)))) goto yyL3;
  {
/* line 3079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSpecState (SP);
  }
  }
   return;
yyL3:;

  }
  if (Tree_IsType (SP, kRetypes)) {
/* line 3080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (SP->Retypes.Sym)))) goto yyL4;
  {
/* line 3080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSpecState (SP);
  }
  }
   return;
yyL4:;

  }
  if (SP->Kind == kIs) {
/* line 3081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (SP->Is.Sym)))) goto yyL5;
  {
/* line 3081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSpecState (SP);
  }
  }
   return;
yyL5:;

  }
/* line 3084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenLocalRep
# if defined __STDC__ | defined __cplusplus
(register tTree yyP122)
# else
(yyP122)
 register tTree yyP122;
# endif
{
/* line 3089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP122->Replicator.Sym)))) goto yyL1;
  {
/* line 3089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP122->Replicator.From)))) goto yyL1;
  {
/* line 3089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP122->Replicator.For)))) goto yyL1;
  {
/* line 3090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 3091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP122->Replicator.Sym);
/* line 3091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP122->Replicator.Sym, PrefixId (CurSpec () . PrefixId, Symbol (yyP122->Replicator.Sym) . CId, '.'));
  }
  }
  }
  }
   return;
yyL1:;

/* line 3095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP122->Replicator.Sym)))) goto yyL2;
  {
/* line 3096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3097 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 3097 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP122->Replicator.Sym);
/* line 3097 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3097 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT ");
/* line 3098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP122->Replicator.Sym);
/* line 3098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End;");
/* line 3098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   ReplaceSymId (yyP122->Replicator.Sym, PrefixId (CurSpec () . PrefixId, Symbol (yyP122->Replicator.Sym) . CId, '.'));
  }
  }
   return;
yyL2:;

 yyAbort ("GenLocalRep");
;
}

static void GenLocalIdList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP124, register tTree yyP123)
# else
(yyP124, yyP123)
 register tTree yyP124;
 register tTree yyP123;
# endif
{
  if (yyP123->Kind == kAnId) {
/* line 3104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP123->AnId.Sym)))) goto yyL1;
  {
/* line 3104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenDeclSym (yyP124, yyP123->AnId.Sym, false);
/* line 3104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalIdList (yyP124, yyP123->AnId.Next);
  }
  }
   return;
yyL1:;

/* line 3105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalIdList (yyP124, yyP123->AnId.Next);
  }
   return;

  }
  if (yyP123->Kind == kNoId) {
/* line 3106 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenLocalIdList");
;
}

static void GenLocalInits
# if defined __STDC__ | defined __cplusplus
(register tTree yyP125)
# else
(yyP125)
 register tTree yyP125;
# endif
{
  if (yyP125->Kind == kASpec) {
/* line 3112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   SetPosition (yyP125->ASpec.Spec->Spec.Pos);
/* line 3112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInit (yyP125->ASpec.Spec);
/* line 3112 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (yyP125->ASpec.Next);
  }
   return;

  }
  if (yyP125->Kind == kNoSpec) {
/* line 3113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenLocalInits");
;
}

static void GenLocalInit
# if defined __STDC__ | defined __cplusplus
(register tTree SP)
# else
(SP)
 register tTree SP;
# endif
{
  if (SP->Kind == kDecl) {
  if (SP->Decl.Type->Kind == kChan) {
/* line 3118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (SP->Decl.IdList);
/* line 3118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanList (SP->Decl.IdList);
  }
   return;

  }
  if (Tree_IsType (SP->Decl.Type, kArray)) {
/* line 3120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SP->Decl.Type)))) goto yyL2;
  {
/* line 3120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (SP->Decl.IdList);
/* line 3120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanArrayList (SP->Decl.Type, SP->Decl.IdList);
  }
  }
   return;
yyL2:;

  }
/* line 3122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3122 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetListScope (SP->Decl.IdList);
  }
   return;

  }
  if (Tree_IsType (SP, kAVal)) {
/* line 3125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsZeroSizeType (ExprType (SP->AVal.Expr))))) goto yyL4;
  {
/* line 3126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->AVal.Sym);
/* line 3127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->AVal.Sym);
/* line 3127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = NULL;");
/* line 3127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

  }

  switch (SP->Kind) {
  case kIs:
/* line 3129 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3129 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsZeroSizeType (ExprType (SP->Is.Elemental))))) goto yyL5;
  {
/* line 3130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Is.Sym);
/* line 3131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = NULL;");
/* line 3131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL5:;

/* line 3253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (SymbolType (SP->Is.Sym))))) goto yyL26;
  }
   return;
yyL26:;

  if (Tree_IsType (SP->Is.Type, kUserStructTypes)) {
  if (Tree_IsType (SP->Is.Elemental, kTable)) {
/* line 3255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitUserType (SP->Is.Sym, SP->Is.Type->UserStructTypes.FieldList, SP->Is.Elemental->Table.ExprList);
  }
   return;

  }
  }
  if (Tree_IsType (SP->Is.Elemental, kTable)) {
/* line 3259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitTable (SP->Is.Sym, 0, SP->Is.Elemental->Table.ExprList);
  }
   return;

  }
  if (Tree_IsType (SP->Is.Elemental, kElement)) {
/* line 3263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (SP->Is.Sym))))) goto yyL29;
  {
/* line 3263 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SymbolType (SP->Is.Sym))))) goto yyL29;
  {
/* line 3264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Is.Sym);
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SymbolType (SP->Is.Sym), false);
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (SP->Is.Elemental, false);
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL29:;

/* line 3267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SymbolType (SP->Is.Sym))))) goto yyL30;
  {
/* line 3268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Is.Sym);
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SymbolType (SP->Is.Sym), true);
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (SP->Is.Elemental, true);
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3269 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (SP->Is.Sym, SymbolType (SP->Is.Sym), SP->Is.Elemental, GetElementSymbol (SP->Is.Elemental), ElementType (SP->Is.Elemental));
  }
  }
   return;
yyL30:;

/* line 3272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (SP->Is.Sym))))) goto yyL31;
  {
/* line 3273 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Is.Sym);
/* line 3274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (SP->Is.Elemental, false);
/* line 3274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL31:;

/* line 3276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3277 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Is.Sym);
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Is.Sym);
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SymbolType (SP->Is.Sym), true);
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (SP->Is.Elemental, true);
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (SP->Is.Sym, SymbolType (SP->Is.Sym), SP->Is.Elemental, GetElementSymbol (SP->Is.Elemental), ElementType (SP->Is.Elemental));
  }
   return;

  }
  break;
  case kRetypes:
  case kReshapes:
/* line 3133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsZeroSizeType (SP->Retypes.Type)))) goto yyL6;
  {
/* line 3134 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Retypes.Sym);
/* line 3135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Retypes.Sym);
/* line 3135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = NULL;");
/* line 3135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL6:;

  if (Tree_IsType (SP->Retypes.Elemental, kElement)) {
/* line 3140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SP->Retypes.Type)))) goto yyL7;
  {
/* line 3140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SP->Retypes.Type)))) goto yyL7;
  {
/* line 3141 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Retypes.Sym);
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Retypes.Sym);
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->Retypes.Type, false);
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (SP->Retypes.Elemental, false);
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL7:;

/* line 3145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SP->Retypes.Type)))) goto yyL8;
  {
/* line 3146 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Retypes.Sym);
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Retypes.Sym);
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->Retypes.Type, true);
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (SP->Retypes.Elemental, true);
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3148 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (SP->Retypes.Sym, SP->Retypes.Type, SP->Retypes.Elemental, GetElementSymbol (SP->Retypes.Elemental), ElementType (SP->Retypes.Elemental));
  }
  }
   return;
yyL8:;

/* line 3151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL9;
  {
/* line 3152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEmuRetype (SP->Retypes.Type, SP->Retypes.Sym, ElementType (SP->Retypes.Elemental), SP->Retypes.Elemental);
  }
  }
   return;
yyL9:;

/* line 3155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SP->Retypes.Type)))) goto yyL10;
  {
/* line 3156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Retypes.Sym);
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Retypes.Sym);
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = *");
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->Retypes.Type, true);
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (SP->Retypes.Elemental, true);
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL10:;

/* line 3160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Retypes.Sym);
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Retypes.Sym);
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->Retypes.Type, true);
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (SP->Retypes.Elemental, true);
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3163 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (SP->Retypes.Sym, SP->Retypes.Type, SP->Retypes.Elemental, GetElementSymbol (SP->Retypes.Elemental), ElementType (SP->Retypes.Elemental));
  }
   return;

  }
  break;
  case kValRetypes:
  case kValReshapes:
/* line 3168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL12;
  {
/* line 3168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (SP->ValRetypes.Expr))))) goto yyL12;
  {
/* line 3168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (SP->ValRetypes.Expr)))) goto yyL12;
  {
/* line 3169 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3169 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (SP->ValRetypes.Expr));
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP; TMP = ");
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (SP->ValRetypes.Expr);
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->ValRetypes.Sym);
/* line 3172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("RETYPE(");
/* line 3172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (SP->ValRetypes.Type));
/* line 3172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("^");
/* line 3172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (ExprType (SP->ValRetypes.Expr)));
/* line 3172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (SP->ValRetypes.Type, SP->ValRetypes.Sym, 1);
/* line 3173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (SymbolKind (SP->ValRetypes.Sym), SP->ValRetypes.Sym, true);
/* line 3174 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3175 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&TMP);");
/* line 3175 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3176 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
   return;
yyL12:;

  if (SP->ValRetypes.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (SP->ValRetypes.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3179 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL13;
  {
/* line 3180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEmuRetype (SP->ValRetypes.Type, SP->ValRetypes.Sym, ExprType (SP->ValRetypes.Expr), SP->ValRetypes.Expr->ElementalExpr.Elemental);
  }
  }
   return;
yyL13:;

  }
  }
/* line 3183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SP->ValRetypes.Type)))) goto yyL14;
  {
/* line 3183 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (SP->ValRetypes.Expr))))) goto yyL14;
  {
/* line 3184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->ValRetypes.Sym);
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*(");
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (ExprType (SP->ValRetypes.Expr), true);
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (SymbolKind (SP->ValRetypes.Sym), SP->ValRetypes.Sym, true);
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") = ");
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (SP->ValRetypes.Expr);
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL14:;

/* line 3188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (SP->ValRetypes.Expr))))) goto yyL15;
  {
/* line 3188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (SP->ValRetypes.Expr)))) goto yyL15;
  {
/* line 3189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (SP->ValRetypes.Expr));
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP; TMP = ");
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (SP->ValRetypes.Expr);
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->ValRetypes.Sym);
/* line 3192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->ValRetypes.Sym);
/* line 3192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->ValRetypes.Type, true);
/* line 3192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&TMP;");
/* line 3192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL15:;

  if (SP->ValRetypes.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (SP->ValRetypes.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SP->ValRetypes.Type)))) goto yyL16;
  {
/* line 3197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->ValRetypes.Sym);
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->ValRetypes.Sym);
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = *(");
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->ValRetypes.Type, true);
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (SP->ValRetypes.Expr, true);
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (SP->ValRetypes.Sym, SP->ValRetypes.Type, SP->ValRetypes.Expr->ElementalExpr.Elemental, GetElementSymbol (SP->ValRetypes.Expr->ElementalExpr.Elemental), ExprType (SP->ValRetypes.Expr->ElementalExpr.Elemental));
  }
  }
   return;
yyL16:;

/* line 3202 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->ValRetypes.Sym);
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->ValRetypes.Sym);
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SP->ValRetypes.Type, true);
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (SP->ValRetypes.Expr, true);
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3205 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (SP->ValRetypes.Sym, SP->ValRetypes.Type, SP->ValRetypes.Expr->ElementalExpr.Elemental, GetElementSymbol (SP->ValRetypes.Expr->ElementalExpr.Elemental), ExprType (SP->ValRetypes.Expr->ElementalExpr.Elemental));
  }
   return;

  }
  }
  break;
  case kVal:
  if (SP->Val.Expr->Kind == kElementalExpr) {
/* line 3212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (SP->Val.Expr)))) goto yyL18;
  {
/* line 3212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (ExprType (SP->Val.Expr))))) goto yyL18;
  {
/* line 3213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (SP->Val.Expr));
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (ExprType (SP->Val.Expr));
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (ExprType (SP->Val.Expr->ElementalExpr.Elemental), SP->Val.Expr->ElementalExpr.Elemental -> Elemental . CA);
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3215 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
/* line 3216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Val.Sym);
/* line 3216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = (");
/* line 3216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (SP->Val.Expr));
/* line 3216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)TMP;");
/* line 3216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL18:;

  }
  if (Tree_IsType (SP->Val.Expr, kStringElement)) {
/* line 3219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (SP->Val.Expr)))) goto yyL19;
  {
/* line 3219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsArrayType (ExprType (SP->Val.Expr))))) goto yyL19;
  {
/* line 3220 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3220 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (SP->Val.Expr));
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (ExprType (SP->Val.Expr));
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (ExprType (SP->Val.Expr), SP->Val.Expr -> StringElement . CA);
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
/* line 3223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Val.Sym);
/* line 3223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = TMP;");
/* line 3223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL19:;

  }
/* line 3226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (SP->Val.Expr)))) goto yyL20;
  {
/* line 3226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsNewSymbol (SP->Val.Sym)))) goto yyL20;
  }
  }
   return;
yyL20:;

  if (Tree_IsType (SP->Val.Type, kUserStructTypes)) {
  if (SP->Val.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (SP->Val.Expr->ElementalExpr.Elemental, kTable)) {
/* line 3228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3229 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitUserType (SP->Val.Sym, SP->Val.Type->UserStructTypes.FieldList, SP->Val.Expr->ElementalExpr.Elemental->Table.ExprList);
/* line 3230 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
  }
   return;

  }
  }
  }
  if (SP->Val.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (SP->Val.Expr->ElementalExpr.Elemental, kTable)) {
/* line 3232 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitTable (SP->Val.Sym, 0, SP->Val.Expr->ElementalExpr.Elemental->Table.ExprList);
/* line 3234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
  }
   return;

  }
  }
  if (SP->Val.Expr->Kind == kFuncCall) {
/* line 3236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsSTVFunction (SymbolKind (SP->Val.Expr->FuncCall.Sym))))) goto yyL23;
  {
/* line 3237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncCall (SymbolKind (SP->Val.Expr->FuncCall.Sym), SP->Val.Expr, SP);
/* line 3237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
  }
  }
   return;
yyL23:;

  }
/* line 3240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (SP->Val.Sym))))) goto yyL24;
  {
/* line 3241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Val.Sym);
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SymbolType (SP->Val.Sym), false);
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (SP->Val.Expr);
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL24:;

  if (SP->Val.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (SP->Val.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (SP->Val.Sym);
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (SP->Val.Sym);
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (SymbolType (SP->Val.Sym), true);
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (SP->Val.Expr, true);
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3246 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (SP->Val.Sym, SymbolType (SP->Val.Sym), SP->Val.Expr->ElementalExpr.Elemental, GetElementSymbol (SP->Val.Expr->ElementalExpr.Elemental), ExprType (SP->Val.Expr));
  }
   return;

  }
  }
  break;
  case kCallFrame:
/* line 3283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kParFrame:
/* line 3284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAProc:
  case kDefProc:
  case kInlineDefProc:
  case kPrototypeProc:
/* line 3285 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAFunc:
  case kDefFunc:
  case kInlineDefFunc:
  case kDefIsFunc:
  case kInlineDefIsFunc:
  case kPrototypeFunc:
/* line 3286 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAPro:
  case kDefSeqPro:
  case kDefTagPro:
/* line 3287 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kNullSpec:
/* line 3288 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDeclType:
/* line 3289 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }

/* line 3291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (SP, SP->Spec.Pos);
  }
   return;

;
}

static void GenInitUserType
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP128, register tTree yyP127, register tTree yyP126)
# else
(yyP128, yyP127, yyP126)
 tSymbol yyP128;
 register tTree yyP127;
 register tTree yyP126;
# endif
{
  if (yyP127->Kind == kAField) {
  if (yyP126->Kind == kAnExpr) {
/* line 3297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP127->AField.Type)))) goto yyL1;
  {
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*((");
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP127->AField.Type);
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)(((BYTE*)");
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP128);
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")+");
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcOffset (SymbolType (yyP128), yyP127));
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")) = ");
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP126->AnExpr.Expr);
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitUserType (yyP128, yyP127->AField.Next, yyP126->AnExpr.Next);
  }
  }
   return;
yyL1:;

/* line 3301 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(((BYTE*)");
/* line 3303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP128);
/* line 3303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")+");
/* line 3303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcOffset (SymbolType (yyP128), yyP127));
/* line 3303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
/* line 3304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (yyP126->AnExpr.Expr, true);
/* line 3304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayTypeSize (ExprType (yyP126->AnExpr.Expr));
/* line 3305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("); ");
/* line 3305 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitUserType (yyP128, yyP127->AField.Next, yyP126->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP126->Kind == kNoExpr) {
/* line 3308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenInitUserType");
;
}

static void GenInitTable
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP131, register int yyP130, register tTree yyP129)
# else
(yyP131, yyP130, yyP129)
 tSymbol yyP131;
 register int yyP130;
 register tTree yyP129;
# endif
{
  if (yyP129->Kind == kAnExpr) {
/* line 3313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (yyP129->AnExpr.Expr))))) goto yyL1;
  {
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP131);
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP130);
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("] = ");
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP129->AnExpr.Expr);
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitTable (yyP131, yyP130 + 1, yyP129->AnExpr.Next);
  }
  }
   return;
yyL1:;

/* line 3317 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP131);
/* line 3319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 3319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP130);
/* line 3319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("],");
/* line 3320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (yyP129->AnExpr.Expr, true);
/* line 3320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayTypeSize (ExprType (yyP129->AnExpr.Expr));
/* line 3321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("); ");
/* line 3321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitTable (yyP131, yyP130 + 1, yyP129->AnExpr.Next);
  }
   return;

  }
  if (yyP129->Kind == kNoExpr) {
/* line 3324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenInitTable");
;
}

static void GenArrayTypeSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP132)
# else
(yyP132)
 register tTree yyP132;
# endif
{
  if (Tree_IsType (yyP132, kArray)) {
/* line 3329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP132->Array.Expr);
/* line 3330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 3330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayTypeSize (yyP132->Array.Type);
/* line 3330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
/* line 3333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP132)))) goto yyL2;
  {
/* line 3334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("sizeof(");
/* line 3334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP132);
/* line 3334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *)");
  }
  }
   return;
yyL2:;

/* line 3337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (yyP132));
  }
   return;

;
}

static void GenEmuRetype
# if defined __STDC__ | defined __cplusplus
(register tTree yyP136, tSymbol yyP135, register tTree yyP134, register tTree yyP133)
# else
(yyP136, yyP135, yyP134, yyP133)
 register tTree yyP136;
 tSymbol yyP135;
 register tTree yyP134;
 register tTree yyP133;
# endif
{
/* line 3346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP136) == RetypeSize (yyP134)))) goto yyL1;
  {
/* line 3346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP136)))) goto yyL1;
  {
/* line 3347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP135);
/* line 3347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (yyP136, true);
/* line 3348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP133, true);
/* line 3348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3349 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (yyP135, SymbolType (yyP135), yyP133, GetElementSymbol (yyP133), yyP134);
  }
  }
  }
   return;
yyL1:;

/* line 3352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP136)))) goto yyL2;
  {
/* line 3353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitReArray (yyP135, SymbolType (yyP135), yyP133, GetElementSymbol (yyP133), yyP134);
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ALLOCRETYPE(");
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP135);
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP136);
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*,");
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (yyP136, yyP135, 1);
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
  }
   return;
yyL2:;

/* line 3358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP135);
/* line 3360 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("RETYPE(");
/* line 3361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (yyP136));
/* line 3361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("^");
/* line 3361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (yyP134));
/* line 3361 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (yyP136, yyP135, 1);
/* line 3362 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (SymbolKind (yyP135), yyP135, true);
/* line 3364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3365 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP133, true);
/* line 3366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3366 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void CodeRetypeSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP139, tSymbol yyP138, register int yyP137)
# else
(yyP139, yyP138, yyP137)
 register tTree yyP139;
 tSymbol yyP138;
 register int yyP137;
# endif
{
  if (Tree_IsType (yyP139, kArray)) {
  if (yyP139->Array.Expr->Kind == kUnBound) {
/* line 3372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP138);
/* line 3373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Dim");
/* line 3373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP139));
/* line 3373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 3374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (yyP139->Array.Type, yyP138, yyP137);
  }
   return;

  }
/* line 3376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (yyP139->Array.Type, yyP138, yyP137 * ValConstInt (yyP139->Array.Expr));
  }
   return;

  }
/* line 3380 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3381 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP137 * PrimTypeSize (yyP139));
  }
   return;

;
}

static int RetypeSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP140)
# else
(yyP140)
 register tTree yyP140;
# endif
{
  if (Tree_IsType (yyP140, kArray)) {
/* line 3386 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return RetypeSize (yyP140->Array.Type);

  }
/* line 3387 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return PrimTypeSize (yyP140) - 1;

}

static void GenInitChanList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP141)
# else
(yyP141)
 register tTree yyP141;
# endif
{
  if (yyP141->Kind == kAnId) {
/* line 3393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP141->AnId.Sym);
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = &");
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP141->AnId.Sym);
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_CHAN");
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INITCH(");
/* line 3395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP141->AnId.Sym);
/* line 3395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanList (yyP141->AnId.Next);
  }
   return;

  }
  if (yyP141->Kind == kNoId) {
/* line 3397 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenInitChanList");
;
}

static void GenInitChanSubs
# if defined __STDC__ | defined __cplusplus
(register int yyP143, register int yyP142)
# else
(yyP143, yyP142)
 register int yyP143;
 register int yyP142;
# endif
{
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP143 < yyP142))) goto yyL1;
  {
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[TMP");
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP143);
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 3401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanSubs (yyP143 + 1, yyP142);
  }
  }
   return;
yyL1:;

/* line 3402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenInitChanArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP146, tSymbol yyP145, register int yyP144)
# else
(yyP146, yyP145, yyP144)
 register tTree yyP146;
 tSymbol yyP145;
 register int yyP144;
# endif
{
  if (Tree_IsType (yyP146, kArray)) {
/* line 3407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("int TMP");
/* line 3409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP144);
/* line 3409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("for (TMP");
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP144);
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=0;TMP");
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP144);
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("<");
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP146->Array.Expr);
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";TMP");
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP144);
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("++)");
/* line 3410 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3411 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanArray (yyP146->Array.Type, yyP145, yyP144 + 1);
/* line 3413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP146->Kind == kChan) {
/* line 3416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP145);
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanSubs (0, yyP144);
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = &");
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP145);
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_CHAN");
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanSubs (0, yyP144);
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INITCH(");
/* line 3418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP145);
/* line 3418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanSubs (0, yyP144);
/* line 3418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenInitChanArray");
;
}

static void GenInitChanArrayList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP148, register tTree yyP147)
# else
(yyP148, yyP147)
 register tTree yyP148;
 register tTree yyP147;
# endif
{
  if (yyP147->Kind == kAnId) {
/* line 3422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanArray (yyP148, yyP147->AnId.Sym, 0);
/* line 3422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitChanArrayList (yyP148, yyP147->AnId.Next);
  }
   return;

  }
  if (yyP147->Kind == kNoId) {
/* line 3423 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenInitChanArrayList");
;
}

static void GenInitIsArray
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP153, register tTree yyP152, register tTree yyP151, tSymbol yyP150, register tTree yyP149)
# else
(yyP153, yyP152, yyP151, yyP150, yyP149)
 tSymbol yyP153;
 register tTree yyP152;
 register tTree yyP151;
 tSymbol yyP150;
 register tTree yyP149;
# endif
{
  if (Tree_IsType (yyP152, kArray)) {
  if (yyP151->Kind == kRange) {
  if (Tree_IsType (yyP149, kArray)) {
/* line 3429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP152->Array.Expr)))) goto yyL1;
  {
/* line 3430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (yyP153, yyP152->Array.Type, yyP151->Range.Next, yyP150, yyP149->Array.Type);
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP153);
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP152));
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP151->Range.For);
/* line 3431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL1:;

/* line 3435 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (yyP153, yyP152->Array.Type, yyP151->Range.Next, yyP150, yyP149->Array.Type);
/* line 3437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 3437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP152->Array.Expr);
/* line 3438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == ");
/* line 3439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP151->Range.For);
/* line 3440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_AIM);
/* line 3440 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
  if (Tree_IsType (yyP149, kArray)) {
/* line 3444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP152->Array.Expr)))) goto yyL3;
  {
/* line 3445 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (yyP153, yyP152->Array.Type, yyP151, yyP150, yyP149->Array.Type);
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP153);
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP152));
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP151, yyP149);
/* line 3446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL3:;

/* line 3450 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3450 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP149->Array.Expr)))) goto yyL4;
  {
/* line 3451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (yyP153, yyP152->Array.Type, yyP151, yyP150, yyP149->Array.Type);
/* line 3452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 3452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP152->Array.Expr);
/* line 3453 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == ");
/* line 3454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP150);
/* line 3454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP149));
/* line 3455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_AIM);
/* line 3455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

/* line 3459 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitIsArray (yyP153, yyP152->Array.Type, yyP151, yyP150, yyP149->Array.Type);
  }
   return;

  }
  }
/* line 3462 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenInitReArray
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP158, register tTree yyP157, register tTree yyP156, tSymbol yyP155, register tTree yyP154)
# else
(yyP158, yyP157, yyP156, yyP155, yyP154)
 tSymbol yyP158;
 register tTree yyP157;
 register tTree yyP156;
 tSymbol yyP155;
 register tTree yyP154;
# endif
{
  if (Tree_IsType (yyP154, kArray)) {
/* line 3467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((CheckValidDeclType (yyP157)))) goto yyL1;
  {
/* line 3467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((CheckValidDeclType (yyP154)))) goto yyL1;
  }
  }
   return;
yyL1:;

  }
/* line 3469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitMissingDimensions (yyP158, yyP157, yyP156, yyP154);
/* line 3471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 3471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (yyP158, yyP157);
/* line 3471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == ");
/* line 3471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP156, yyP154);
/* line 3472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ARM);
/* line 3472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenInitMissingDimensions
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP162, register tTree yyP161, register tTree yyP160, register tTree yyP159)
# else
(yyP162, yyP161, yyP160, yyP159)
 tSymbol yyP162;
 register tTree yyP161;
 register tTree yyP160;
 register tTree yyP159;
# endif
{
  if (Tree_IsType (yyP161, kArray)) {
  if (yyP161->Array.Expr->Kind == kUnBound) {
/* line 3477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP162);
/* line 3478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP161));
/* line 3478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP160, yyP159);
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")/(");
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayUnBoundSize (SymbolType (yyP162));
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")) ;");
/* line 3479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitMissingDimensions (yyP162, yyP161->Array.Type, yyP160, yyP159);
  }
   return;

  }
/* line 3482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP161->Array.Expr)))) goto yyL2;
  {
/* line 3483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP162);
/* line 3483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP161));
/* line 3483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP161->Array.Expr);
/* line 3484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3485 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitMissingDimensions (yyP162, yyP161->Array.Type, yyP160, yyP159);
  }
  }
   return;
yyL2:;

/* line 3487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInitMissingDimensions (yyP162, yyP161->Array.Type, yyP160, yyP159);
  }
   return;

  }
/* line 3490 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenArrayUnBoundSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP163)
# else
(yyP163)
 register tTree yyP163;
# endif
{
  if (Tree_IsType (yyP163, kArray)) {
  if (yyP163->Array.Expr->Kind == kUnBound) {
/* line 3496 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayUnBoundSize (yyP163->Array.Type);
  }
   return;

  }
/* line 3500 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP163->Array.Expr);
/* line 3502 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" * ");
/* line 3503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayUnBoundSize (yyP163->Array.Type);
  }
   return;

  }
  if (Tree_IsType (yyP163, kUserStructTypes)) {
/* line 3506 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcTypeSize (yyP163));
  }
   return;

  }
/* line 3509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3509 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP163)))) goto yyL4;
  {
/* line 3510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("sizeof(");
/* line 3510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP163);
/* line 3510 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *)");
  }
  }
   return;
yyL4:;

/* line 3512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("sizeof(");
/* line 3513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP163);
/* line 3513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void GenResults
# if defined __STDC__ | defined __cplusplus
(register tTree yyP164)
# else
(yyP164)
 register tTree yyP164;
# endif
{
  if (yyP164->Kind == kASpec) {
/* line 3519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (yyP164->ASpec.Next);
/* line 3519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResult (yyP164->ASpec.Spec);
  }
   return;

  }
  if (yyP164->Kind == kNoSpec) {
/* line 3520 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenResults");
;
}

static void GenResult
# if defined __STDC__ | defined __cplusplus
(register tTree yyP165)
# else
(yyP165)
 register tTree yyP165;
# endif
{
  if (yyP165->Kind == kIs) {
/* line 3525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanOrTimerType (SymbolType (yyP165->Is.Sym))))) goto yyL1;
  }
   return;
yyL1:;

  if (Tree_IsType (yyP165->Is.Elemental, kTable)) {
/* line 3531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResultTable (yyP165->Is.Sym, 0, yyP165->Is.Elemental->Table.ExprList);
  }
   return;

  }
  if (Tree_IsType (yyP165->Is.Elemental, kElement)) {
/* line 3535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP165->Is.Sym))))) goto yyL4;
  {
/* line 3536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP165->Is.Elemental, false);
/* line 3536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP165->Is.Sym);
/* line 3536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

  }
  }
  if (Tree_IsType (yyP165, kRetypes)) {
/* line 3528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanOrTimerType (SymbolType (yyP165->Retypes.Sym))))) goto yyL2;
  }
   return;
yyL2:;

/* line 3543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL6;
  {
/* line 3544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenEmuRetypeResult (yyP165->Retypes.Type, yyP165->Retypes.Sym, ElementType (yyP165->Retypes.Elemental), yyP165->Retypes.Elemental);
  }
  }
   return;
yyL6:;

  if (Tree_IsType (yyP165->Retypes.Elemental, kElement)) {
/* line 3547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP165->Retypes.Type)))) goto yyL7;
  {
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (yyP165->Retypes.Type, true);
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP165->Retypes.Elemental, true);
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP165->Retypes.Sym);
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL7:;

  }
  }
  if (Tree_IsType (yyP165, kValRetypes)) {
  if (yyP165->ValRetypes.Expr->Kind == kElementalExpr) {
/* line 3539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxEmulate)))) goto yyL5;
  {
/* line 3539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP165->ValRetypes.Type) != RetypeSize (ElementType (yyP165->ValRetypes.Expr->ElementalExpr.Elemental))))) goto yyL5;
  {
/* line 3539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP165->ValRetypes.Type)))) goto yyL5;
  {
/* line 3539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP165->ValRetypes.Type)))) goto yyL5;
  {
/* line 3540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FREERETYPE(");
/* line 3540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP165->ValRetypes.Sym);
/* line 3540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3540 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
  }
   return;
yyL5:;

  }
  }
/* line 3550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenResultTable
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP168, register int yyP167, register tTree yyP166)
# else
(yyP168, yyP167, yyP166)
 tSymbol yyP168;
 register int yyP167;
 register tTree yyP166;
# endif
{
  if (yyP166->Kind == kAnExpr) {
/* line 3556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (yyP166->AnExpr.Expr))))) goto yyL1;
  {
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP166->AnExpr.Expr);
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP168);
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP167);
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("];");
/* line 3557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResultTable (yyP168, yyP167 + 1, yyP166->AnExpr.Next);
  }
  }
   return;
yyL1:;

/* line 3560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3562 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (yyP166->AnExpr.Expr, true);
/* line 3562 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP168);
/* line 3563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 3563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP167);
/* line 3563 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("],");
/* line 3564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayTypeSize (ExprType (yyP166->AnExpr.Expr));
/* line 3564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("); ");
/* line 3564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResultTable (yyP168, yyP167 + 1, yyP166->AnExpr.Next);
  }
   return;

  }
 yyAbort ("GenResultTable");
;
}

static void GenEmuRetypeResult
# if defined __STDC__ | defined __cplusplus
(register tTree yyP172, tSymbol yyP171, register tTree yyP170, register tTree yyP169)
# else
(yyP172, yyP171, yyP170, yyP169)
 register tTree yyP172;
 tSymbol yyP171;
 register tTree yyP170;
 register tTree yyP169;
# endif
{
/* line 3569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP172) == RetypeSize (yyP170)))) goto yyL1;
  }
   return;
yyL1:;

  if (Tree_IsType (yyP169, kElement)) {
/* line 3571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3572 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP171);
/* line 3573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("RETYPE(");
/* line 3574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (yyP172));
/* line 3574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("^");
/* line 3574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (RetypeSize (yyP170));
/* line 3574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeRetypeSize (yyP172, yyP171, 1);
/* line 3575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP169, true);
/* line 3577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (SymbolKind (yyP171), yyP171, true);
/* line 3579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
   return;
yyL2:;

/* line 3583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((RetypeSize (yyP172) != RetypeSize (ElementType (yyP169))))) goto yyL3;
  {
/* line 3583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP172)))) goto yyL3;
  {
/* line 3583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP172)))) goto yyL3;
  {
/* line 3584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FREERETYPE(");
/* line 3584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP171);
/* line 3584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
   return;
yyL3:;

/* line 3587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenEmuRetypeResult");
;
}

static int GenRepStart
# if defined __STDC__ | defined __cplusplus
(register tTree yyP173)
# else
(yyP173)
 register tTree yyP173;
# endif
{
/* line 3595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
/* line 3596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP173->Replicator.From)))) goto yyL1;
  {
/* line 3596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP173->Replicator.For)))) goto yyL1;
  {
/* line 3597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 3598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP173->Replicator.Sym);
/* line 3599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP173->Replicator.Sym) . Kind, yyP173->Replicator.Sym, false);
/* line 3599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP173->Replicator.From);
/* line 3599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 3600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 3600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return lab;
yyL1:;

/* line 3602 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 3604 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP173->Replicator.Sym);
/* line 3605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP173->Replicator.Sym) . Kind, yyP173->Replicator.Sym, false);
/* line 3605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP173->Replicator.From);
/* line 3605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3605 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP173->Replicator.Sym) . Kind, yyP173->Replicator.Sym, false);
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End = ");
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP173->Replicator.Sym) . Kind, yyP173->Replicator.Sym, false);
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP173->Replicator.For);
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3606 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("GOTO(");
/* line 3607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 3607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return lab;

}

static void GenRepEnd
# if defined __STDC__ | defined __cplusplus
(register tTree yyP176, register int yyP175, register int yyP174)
# else
(yyP176, yyP175, yyP174)
 register tTree yyP176;
 register int yyP175;
 register int yyP174;
# endif
{
/* line 3612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP176->Replicator.From)))) goto yyL1;
  {
/* line 3612 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP176->Replicator.For)))) goto yyL1;
  {
/* line 3613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP176->Replicator.Sym) . Kind, yyP176->Replicator.Sym, false);
/* line 3613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("++;");
/* line 3613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3614 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (yyP174);
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP176->Replicator.Sym) . Kind, yyP176->Replicator.Sym, false);
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" != (");
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP176->Replicator.From);
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("+");
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP176->Replicator.For);
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")) GOTO(");
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP175);
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3615 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL1:;

/* line 3617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP176->Replicator.Sym) . Kind, yyP176->Replicator.Sym, false);
/* line 3618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("++;");
/* line 3618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (yyP174);
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP176->Replicator.Sym) . Kind, yyP176->Replicator.Sym, false);
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" != ");
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP176->Replicator.Sym) . Kind, yyP176->Replicator.Sym, false);
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End) GOTO(");
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (yyP175);
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenAltIndex
# if defined __STDC__ | defined __cplusplus
(register tTree yyP177)
# else
(yyP177)
 register tTree yyP177;
# endif
{
/* line 3626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP177->Replicator.Sym) . Kind, yyP177->Replicator.Sym, false);
  }
   return;

;
}

static void GenAltSelect
# if defined __STDC__ | defined __cplusplus
(register tTree yyP178)
# else
(yyP178)
 register tTree yyP178;
# endif
{
/* line 3631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP178->Replicator.Sym) . Kind, yyP178->Replicator.Sym, false);
/* line 3632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Alt");
  }
   return;

;
}

static void GenForLoop
# if defined __STDC__ | defined __cplusplus
(register tTree yyP179)
# else
(yyP179)
 register tTree yyP179;
# endif
{
/* line 3637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP179->Replicator.From)))) goto yyL1;
  {
/* line 3637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP179->Replicator.For)))) goto yyL1;
  {
/* line 3638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP179->Replicator.Sym);
/* line 3639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("for (");
/* line 3639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP179->Replicator.From);
/* line 3639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" != ");
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP179->Replicator.From);
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP179->Replicator.For);
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("++)");
/* line 3640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL1:;

/* line 3643 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSetScope (yyP179->Replicator.Sym);
/* line 3645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP179->Replicator.From);
/* line 3645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End = ");
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP179->Replicator.For);
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("for (; ");
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" != ");
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End; ");
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP179->Replicator.Sym) . Kind, yyP179->Replicator.Sym, false);
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("++)");
/* line 3647 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenForCheck
# if defined __STDC__ | defined __cplusplus
(register tTree yyP180)
# else
(yyP180)
 register tTree yyP180;
# endif
{
/* line 3652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP180->Replicator.From)))) goto yyL1;
  {
/* line 3652 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP180->Replicator.For)))) goto yyL1;
  {
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP180->Replicator.Sym) . Kind, yyP180->Replicator.Sym, false);
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == (");
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP180->Replicator.From);
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP180->Replicator.For);
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
/* line 3653 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL1:;

/* line 3655 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (");
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP180->Replicator.Sym) . Kind, yyP180->Replicator.Sym, false);
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == ");
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP180->Replicator.Sym) . Kind, yyP180->Replicator.Sym, false);
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_End) ");
/* line 3656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void CodeArrayExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP182, register bool yyP181)
# else
(yyP182, yyP181)
 register tTree yyP182;
 register bool yyP181;
# endif
{
  if (yyP182->Kind == kElementalExpr) {
  if (Tree_IsType (yyP182->ElementalExpr.Elemental, kElement)) {
/* line 3665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP182->ElementalExpr.Elemental))))) goto yyL1;
  {
/* line 3666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP182->ElementalExpr.Elemental, yyP181);
  }
  }
   return;
yyL1:;

/* line 3668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP182->ElementalExpr.Elemental, yyP181);
  }
   return;

  }
  if (Tree_IsType (yyP182->ElementalExpr.Elemental, kTableElement)) {
/* line 3671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected table element", xxError, yyP182->ElementalExpr.Elemental->TableElement.Pos);
  }
   return;

  }
  }
  if (Tree_IsType (yyP182, kStringElement)) {
/* line 3674 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected string element", xxError, yyP182->StringElement.Pos);
  }
   return;

  }
 yyAbort ("CodeArrayExpr");
;
}

static void CodeExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP183)
# else
(yyP183)
 register tTree yyP183;
# endif
{
/* line 3681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP183)))) goto yyL1;
  {
/* line 3681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ExprType (yyP183))))) goto yyL1;
  {
/* line 3681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (ExprType (yyP183), ValConstExpr (yyP183));
  }
  }
  }
   return;
yyL1:;


  switch (yyP183->Kind) {
  case kBinary:
  if (yyP183->Binary.Op->Kind == kLeftShift) {
/* line 3683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("LOGSHIFTLEFT(");
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP183->Binary.Lop));
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Lop);
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Rop);
/* line 3684 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  if (yyP183->Binary.Op->Kind == kRightShift) {
/* line 3686 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("LOGSHIFTRIGHT(");
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP183->Binary.Lop));
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Lop);
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Rop);
/* line 3687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  if (yyP183->Binary.Op->Kind == kAfterOp) {
/* line 3689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("AFTER(");
/* line 3690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Lop);
/* line 3690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Rop);
/* line 3690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  if (yyP183->Binary.Op->Kind == kRem) {
/* line 3692 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3692 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (ExprType (yyP183), kReal)))) goto yyL5;
  {
/* line 3693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("( remainder((");
/* line 3693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Lop);
/* line 3693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),(");
/* line 3693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Rop);
/* line 3693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")))");
  }
  }
   return;
yyL5:;

  }
/* line 3695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Lop);
/* line 3696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeOp (yyP183->Binary.Op);
/* line 3696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Binary.Rop);
/* line 3696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  case kUnary:
  if (yyP183->Unary.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP183->Unary.Expr->ElementalExpr.Elemental, kElement)) {
  if (yyP183->Unary.Op->Kind == kSize) {
/* line 3698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP183->Unary.Expr->ElementalExpr.Elemental, ExprType (yyP183->Unary.Expr));
/* line 3699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  }
  }
/* line 3701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeOp (yyP183->Unary.Op);
/* line 3702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Unary.Expr);
/* line 3702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  case kBytesInType:
/* line 3704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (NoSymbol, yyP183->BytesInType.Type);
  }
   return;

  case kBytesInExpr:
  if (yyP183->BytesInExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP183->BytesInExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP183->BytesInExpr.Expr->ElementalExpr.Elemental, ElementType (yyP183->BytesInExpr.Expr->ElementalExpr.Elemental));
  }
   return;

  }
  }
  break;
  case kElementalExpr:
  if (Tree_IsType (yyP183->ElementalExpr.Elemental, kElement)) {
/* line 3710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP183->ElementalExpr.Elemental))))) goto yyL11;
  {
/* line 3710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (ElementType (yyP183->ElementalExpr.Elemental))))) goto yyL11;
  {
/* line 3711 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP183->ElementalExpr.Elemental, false);
  }
  }
  }
   return;
yyL11:;

/* line 3713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP183->ElementalExpr.Elemental))))) goto yyL12;
  {
/* line 3714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP183->ElementalExpr.Elemental, true);
  }
  }
   return;
yyL12:;

/* line 3716 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP183->ElementalExpr.Elemental, false);
  }
   return;

  }
  break;
  case kConvert:
/* line 3719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP183->Convert.Type);
/* line 3720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP183->Convert.Expr));
/* line 3720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Convert.Expr);
/* line 3720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  case kRound:
/* line 3722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP183->Round.Type);
/* line 3723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP183->Round.Expr));
/* line 3723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ROUND(");
/* line 3723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Round.Expr);
/* line 3723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  case kTrunc:
/* line 3725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP183->Trunc.Type);
/* line 3726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP183->Trunc.Expr));
/* line 3726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TRUNC(");
/* line 3726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP183->Trunc.Expr);
/* line 3726 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  case kMostPos:
/* line 3728 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MOSTPOS");
/* line 3729 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP183->MostPos.Type);
  }
   return;

  case kMostNeg:
/* line 3731 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3732 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MOSTNEG");
/* line 3732 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP183->MostNeg.Type);
  }
   return;

  case kFuncCall:
/* line 3734 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncCall (SymbolKind (yyP183->FuncCall.Sym), yyP183, NoTree);
  }
   return;

  }

/* line 3737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP183, yyP183->Expr.Pos);
  }
   return;

;
}

static void CodeSizeOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP185, register tTree yyP184)
# else
(yyP185, yyP184)
 register tTree yyP185;
 register tTree yyP184;
# endif
{
  if (yyP185->Kind == kRange) {
  if (Tree_IsType (yyP184, kArray)) {
/* line 3742 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3742 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP184->Array.Expr)))) goto yyL1;
  {
/* line 3743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP185->Range.For);
  }
  }
   return;
yyL1:;

  }
  }
  if (Tree_IsType (yyP184, kArray)) {
/* line 3746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP184->Array.Expr)))) goto yyL2;
  {
/* line 3747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP185));
/* line 3747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 3747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP184));
  }
  }
   return;
yyL2:;

/* line 3750 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3751 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP184->Array.Expr);
  }
   return;

  }
 yyAbort ("CodeSizeOp");
;
}

static void CodeConstExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP187, tValue yyP186)
# else
(yyP187, yyP186)
 register tTree yyP187;
 tValue yyP186;
# endif
{
  if (Tree_IsType (yyP187, kInt64)) {
/* line 3758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3758 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 WritetInt(yyP186.Number,true); 
  }
   return;

  }
  if (Tree_IsType (yyP187, kInteger)) {
/* line 3759 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3759 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 WritetInt(yyP186.Number,false); 
  }
   return;

  }
  if (Tree_IsType (yyP187, kByte)) {
/* line 3760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
WriteInt((unsigned char)yyP186.Number);
  }
   return;

  }
  if (Tree_IsType (yyP187, kBool)) {
/* line 3761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP186 . Number))) goto yyL4;
  {
/* line 3761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("true");
  }
  }
   return;
yyL4:;

/* line 3762 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3762 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("false");
  }
   return;

  }
  if (Tree_IsType (yyP187, kReal32)) {
/* line 3764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteReal (yyP186);
  }
   return;

  }
  if (Tree_IsType (yyP187, kReal64)) {
/* line 3765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteReal (yyP186);
  }
   return;

  }
/* line 3766 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3766 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP187, NoPosition);
  }
   return;

;
}

static void CodeAssign
# if defined __STDC__ | defined __cplusplus
(register tTree yyP189, register tTree yyP188)
# else
(yyP189, yyP188)
 register tTree yyP189;
 register tTree yyP188;
# endif
{
  if (yyP189->Kind == kNoExpr) {
  if (yyP188->Kind == kNoExpr) {
/* line 3773 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
  if (yyP189->Kind == kAnExpr) {
  if (yyP189->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP189->AnExpr.Expr->ElementalExpr.Elemental, kTableElement)) {
/* line 3775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3776 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected table elements on LHS of assign", xxError, yyP189->AnExpr.Expr->ElementalExpr.Pos);
  }
   return;

  }
  }
  if (yyP188->Kind == kAnExpr) {
  if (yyP188->AnExpr.Next->Kind == kAnExpr) {
  if (yyP188->AnExpr.Expr->Kind == kFuncCall) {
/* line 3778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (SymbolKind (yyP188->AnExpr.Expr->FuncCall.Sym))))) goto yyL3;
  {
/* line 3779 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OpenBlock (false);
/* line 3779 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTemps (yyP189, 0);
/* line 3781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncCall (SymbolKind (yyP188->AnExpr.Expr->FuncCall.Sym), yyP188->AnExpr.Expr, yyP189);
/* line 3781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3782 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTempCopies (yyP189, 0);
/* line 3783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseBlock ();
/* line 3784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP189->AnExpr.Next, yyP188->AnExpr.Next);
  }
  }
   return;
yyL3:;

  }
  }
  if (yyP188->AnExpr.Expr->Kind == kFuncCall) {
/* line 3787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (SymbolKind (yyP188->AnExpr.Expr->FuncCall.Sym))))) goto yyL4;
  {
/* line 3788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OpenBlock (false);
/* line 3788 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   DoBlock ();
/* line 3789 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTemps (yyP189, 0);
/* line 3790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncCall (SymbolKind (yyP188->AnExpr.Expr->FuncCall.Sym), yyP188->AnExpr.Expr, yyP189);
/* line 3790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3790 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3791 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTempCopies (yyP189, 0);
/* line 3792 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseBlock ();
  }
  }
   return;
yyL4:;

  }
  }
  }
/* line 3795 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign2 (yyP189, yyP188);
  }
   return;

;
}

static void CodeAssign2
# if defined __STDC__ | defined __cplusplus
(register tTree yyP191, register tTree yyP190)
# else
(yyP191, yyP190)
 register tTree yyP191;
 register tTree yyP190;
# endif
{
/* line 3801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tTree T;
  if (yyP191->Kind == kAnExpr) {
  if (yyP191->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP191->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
  if (yyP190->Kind == kAnExpr) {
  if (yyP190->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP190->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TempRequired (yyP191)))) goto yyL1;
  {
/* line 3803 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (T = ElementType (yyP191->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL1;
  {
/* line 3804 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3804 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP191->AnExpr.Expr->ElementalExpr.Elemental));
/* line 3805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 3805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeIdChkConst (ElementType (yyP191->AnExpr.Expr->ElementalExpr.Elemental), yyP191->AnExpr.Expr->ElementalExpr.Pos);
/* line 3805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 3805 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT len = ");
/* line 3806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP191->AnExpr.Expr->ElementalExpr.Elemental, T);
/* line 3806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3806 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChkDims (yyP191->AnExpr.Expr->ElementalExpr.Elemental, T, yyP190->AnExpr.Expr->ElementalExpr.Elemental, ExprType (yyP190->AnExpr.Expr));
/* line 3808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(TMP,");
/* line 3808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (yyP190->AnExpr.Expr, true);
/* line 3808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",len); ");
/* line 3808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3809 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP191->AnExpr.Next, yyP190->AnExpr.Next);
/* line 3810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP191->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 3810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",TMP,len); ");
/* line 3810 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3811 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3811 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL1:;

/* line 3813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3813 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (T = ElementType (yyP191->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL2;
  {
/* line 3814 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChkDims (yyP191->AnExpr.Expr->ElementalExpr.Elemental, T, yyP190->AnExpr.Expr->ElementalExpr.Elemental, ExprType (yyP190->AnExpr.Expr));
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP191->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeArrayExpr (yyP190->AnExpr.Expr, true);
/* line 3815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3816 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP191->AnExpr.Expr->ElementalExpr.Elemental, T);
/* line 3816 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("); ");
/* line 3816 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3817 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP191->AnExpr.Next, yyP190->AnExpr.Next);
  }
  }
   return;
yyL2:;

  }
  }
/* line 3819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3819 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! TempRequired (yyP191)))) goto yyL3;
  {
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP191->AnExpr.Next, yyP190->AnExpr.Next);
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP191->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP190->AnExpr.Expr);
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL3:;

  if (yyP190->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP190->AnExpr.Expr->ElementalExpr.Elemental, kIdentifier)) {
/* line 3822 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3822 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP190->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym), kAVal)))) goto yyL4;
  {
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP191->AnExpr.Next, yyP190->AnExpr.Next);
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP191->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP190->AnExpr.Expr);
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3823 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

  }
  }
/* line 3825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 3826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 3827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP191->AnExpr.Expr->ElementalExpr.Elemental));
/* line 3827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP = ");
/* line 3827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP190->AnExpr.Expr);
/* line 3827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("; ");
/* line 3827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3828 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeAssign (yyP191->AnExpr.Next, yyP190->AnExpr.Next);
/* line 3829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP191->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 3829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = TMP;");
/* line 3829 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 3830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 3830 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
  }
  }
/* line 3832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyAbort ("Unbalanced assignment list");
  }
   return;

;
}

static void GenChkDims
# if defined __STDC__ | defined __cplusplus
(register tTree yyP195, register tTree yyP194, register tTree yyP193, register tTree yyP192)
# else
(yyP195, yyP194, yyP193, yyP192)
 register tTree yyP195;
 register tTree yyP194;
 register tTree yyP193;
 register tTree yyP192;
# endif
{
  if (Tree_IsType (yyP194, kArray)) {
  if (yyP193->Kind == kRange) {
  if (Tree_IsType (yyP192, kArray)) {
/* line 3837 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP195, yyP194);
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP193, yyP192);
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChkDims (yyP195, yyP194->Array.Type, yyP193->Range.Next, yyP192->Array.Type);
  }
   return;

  }
  }
  if (Tree_IsType (yyP192, kArray)) {
/* line 3842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP194->Array.Expr)))) goto yyL2;
  {
/* line 3842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP192->Array.Expr)))) goto yyL2;
  {
/* line 3843 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChkDims (yyP195, yyP194->Array.Type, yyP193, yyP192->Array.Type);
  }
  }
  }
   return;
yyL2:;

/* line 3846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP195, yyP194);
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP193, yyP192);
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3848 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChkDims (yyP195, yyP194->Array.Type, yyP193, yyP192->Array.Type);
  }
   return;

  }
  }
/* line 3850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenInstance
# if defined __STDC__ | defined __cplusplus
(register tTree DP, register tTree yyP197, register tIdent yyP196)
# else
(DP, yyP197, yyP196)
 register tTree DP;
 register tTree yyP197;
 register tIdent yyP196;
# endif
{
/* line 3860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma = false; int lab; 
  if (Tree_IsType (DP, kDefProc)) {
/* line 3862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProc (DP)))) goto yyL1;
  {
/* line 3863 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAtomicInstance (DP->DefProc.Sym, DP->DefProc.Formals, yyP197);
  }
  }
   return;
yyL1:;

/* line 3866 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3867 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNonAtomicInstance (DP->DefProc.Sym, DP->DefProc.Formals, yyP197, yyP196);
  }
   return;

  }
  if (DP->Kind == kPrototypeProc) {
/* line 3870 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3870 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP->PrototypeProc.Lang == xxOccam))) goto yyL3;
  {
/* line 3871 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenNonAtomicInstance (DP->PrototypeProc.Sym, DP->PrototypeProc.Formals, yyP197, yyP196);
  }
  }
   return;
yyL3:;

/* line 3874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP->PrototypeProc.Lang == xxAtomicOccam))) goto yyL4;
  {
/* line 3875 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenAtomicInstance (DP->PrototypeProc.Sym, DP->PrototypeProc.Formals, yyP197);
  }
  }
   return;
yyL4:;

/* line 3878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP->PrototypeProc.Lang == xxC))) goto yyL5;
  {
/* line 3879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCInstance (DP->PrototypeProc.Sym, DP->PrototypeProc.Formals, yyP197);
  }
  }
   return;
yyL5:;

/* line 3882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3882 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP->PrototypeProc.Lang == xxF90))) goto yyL6;
  {
/* line 3883 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Instance (DP->PrototypeProc.Sym, DP->PrototypeProc.Formals, yyP197);
  }
  }
   return;
yyL6:;

/* line 3886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((DP->PrototypeProc.Lang == xxF77))) goto yyL7;
  {
/* line 3887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Fortran 77 interface not yet implemented", xxError, DP->PrototypeProc.Pos);
  }
  }
   return;
yyL7:;

  }
 yyAbort ("GenInstance");
;
}

static void GenAtomicInstance
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP200, register tTree yyP199, register tTree yyP198)
# else
(yyP200, yyP199, yyP198)
 tSymbol yyP200;
 register tTree yyP199;
 register tTree yyP198;
# endif
{
/* line 3893 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma = false; 
/* line 3895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP200);
/* line 3896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3897 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (SymbolKind (yyP200))))) goto yyL1;
  {
/* line 3898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP");
/* line 3898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP200, CurProc);
/* line 3899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = true;
/* line 3900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 3903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP199, yyP198, comma);
/* line 3905 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3905 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenNonAtomicInstance
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP204, register tTree yyP203, register tTree yyP202, register tIdent yyP201)
# else
(yyP204, yyP203, yyP202, yyP201)
 tSymbol yyP204;
 register tTree yyP203;
 register tTree yyP202;
 register tIdent yyP201;
# endif
{
/* line 3910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
/* line 3912 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3912 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrototype (SymbolKind (yyP204))))) goto yyL1;
  {
/* line 3913 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP203, yyP202, yyP201, 0);
/* line 3914 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChain (yyP201, yyP204, true);
/* line 3915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 3918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsPrototype (SymbolKind (yyP204))))) goto yyL2;
  {
/* line 3919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP203, yyP202, yyP201, NoSymbol);
/* line 3920 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenChain (yyP201, yyP204, (IsTopLevel (yyP204)));
/* line 3921 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
  }
   return;
yyL2:;

/* line 3924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("CALL(");
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP204);
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->");
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP201);
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (lab);
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",\"");
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP204);
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\");");
/* line 3926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3927 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
   return;

;
}

static void GenCInstance
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP207, register tTree yyP206, register tTree yyP205)
# else
(yyP207, yyP206, yyP205)
 tSymbol yyP207;
 register tTree yyP206;
 register tTree yyP205;
# endif
{
/* line 3933 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCTemps (yyP206, yyP205, 0);
/* line 3935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Symbol (yyP207) . Id);
/* line 3935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCParams (yyP206, yyP205, false);
/* line 3937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 3937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 3938 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCResults (yyP206, yyP205, 0);
  }
   return;

;
}

static void GenCTemps
# if defined __STDC__ | defined __cplusplus
(register tTree yyP210, register tTree yyP209, register bool yyP208)
# else
(yyP210, yyP209, yyP208)
 register tTree yyP210;
 register tTree yyP209;
 register bool yyP208;
# endif
{
  if (yyP209->Kind == kNoExpr) {
/* line 3945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP210->Kind == kAFormal) {
  if (yyP209->Kind == kAnExpr) {
  if (yyP209->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP209->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP210->AFormal.Type)))) goto yyL2;
  {
/* line 3948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCTemps (yyP210->AFormal.Next, yyP209->AnExpr.Next, yyP208 + 1);
  }
  }
   return;
yyL2:;

  }
  }
/* line 3951 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCTemps (yyP210->AFormal.Next, yyP209->AnExpr.Next, yyP208 + 1);
  }
   return;

  }
  }
 yyAbort ("GenCTemps");
;
}

static void GenCParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP213, register tTree yyP212, register bool yyP211)
# else
(yyP213, yyP212, yyP211)
 register tTree yyP213;
 register tTree yyP212;
 register bool yyP211;
# endif
{
  if (yyP212->Kind == kNoExpr) {
/* line 3958 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP213->Kind == kAFormal) {
  if (Tree_IsType (yyP213->AFormal.Type, kArray)) {
  if (yyP212->Kind == kAnExpr) {
  if (yyP212->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP212->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3960 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3960 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ArrayPrimType (yyP213->AFormal.Type))))) goto yyL2;
  {
/* line 3961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ArrayPrimType (yyP213->AFormal.Type));
/* line 3961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
/* line 3961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP212->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 3962 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCParams (yyP213->AFormal.Next, yyP212->AnExpr.Next, true);
  }
  }
   return;
yyL2:;

/* line 3965 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP211);
/* line 3967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 3967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP213->AFormal.Type);
/* line 3967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 3967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP212->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 3968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCParams (yyP213->AFormal.Next, yyP212->AnExpr.Next, true);
  }
   return;

  }
  }
  }
  }
  if (yyP213->AFormal.By->Kind == kByRef) {
  if (yyP212->Kind == kAnExpr) {
  if (yyP212->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP212->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP212->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL4;
  {
/* line 3972 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP211);
/* line 3973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP212->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 3974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCParams (yyP213->AFormal.Next, yyP212->AnExpr.Next, true);
  }
  }
   return;
yyL4:;

/* line 3977 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP211);
/* line 3979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP212->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 3980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP213->AFormal.Next, yyP212->AnExpr.Next, true);
  }
   return;

  }
  }
  }
  }
  if (yyP213->AFormal.By->Kind == kByVal) {
  if (yyP212->Kind == kAnExpr) {
/* line 3983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP211);
/* line 3985 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP212->AnExpr.Expr);
/* line 3986 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP213->AFormal.Next, yyP212->AnExpr.Next, true);
  }
   return;

  }
  }
  }
 yyAbort ("GenCParams");
;
}

static void GenCResults
# if defined __STDC__ | defined __cplusplus
(register tTree yyP216, register tTree yyP215, register bool yyP214)
# else
(yyP216, yyP215, yyP214)
 register tTree yyP216;
 register tTree yyP215;
 register bool yyP214;
# endif
{
  if (yyP215->Kind == kNoExpr) {
/* line 3992 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP216->Kind == kAFormal) {
  if (yyP216->AFormal.By->Kind == kByRef) {
  if (yyP215->Kind == kAnExpr) {
  if (yyP215->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP215->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 3994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP216->AFormal.Type)))) goto yyL2;
  {
/* line 3995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCResults (yyP216->AFormal.Next, yyP215->AnExpr.Next, yyP214 + 1);
  }
  }
   return;
yyL2:;

  }
  }
  }
  }
  if (yyP215->Kind == kAnExpr) {
/* line 3998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 3999 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenCResults (yyP216->AFormal.Next, yyP215->AnExpr.Next, yyP214 + 1);
  }
   return;

  }
  }
 yyAbort ("GenCResults");
;
}

static void GenF90Instance
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP219, register tTree yyP218, register tTree yyP217)
# else
(yyP219, yyP218, yyP217)
 tSymbol yyP219;
 register tTree yyP218;
 register tTree yyP217;
# endif
{
/* line 4005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 4006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 4006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4007 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP218, yyP217, 0);
/* line 4008 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Inits (yyP218, yyP217, 0);
/* line 4009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (CalcModule (SymbolKind (yyP219)));
/* line 4009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_MP_");
/* line 4009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (Symbol (yyP219) . Id);
/* line 4009 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP218, yyP217, 0, false);
/* line 4011 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90CharParams (yyP218, yyP217, 0);
/* line 4012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4012 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4013 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Results (yyP218, yyP217, 0);
/* line 4014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 4014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 4014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenF90Temps
# if defined __STDC__ | defined __cplusplus
(register tTree yyP222, register tTree yyP221, register int yyP220)
# else
(yyP222, yyP221, yyP220)
 register tTree yyP222;
 register tTree yyP221;
 register int yyP220;
# endif
{
  if (yyP221->Kind == kNoExpr) {
/* line 4021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP222->Kind == kAFormal) {
  if (Tree_IsType (yyP222->AFormal.Type, kArray)) {
  if (Tree_IsType (yyP222->AFormal.Type->Array.Type, kByte)) {
  if (yyP221->Kind == kAnExpr) {
  if (yyP221->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP221->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4023 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4024 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
   return;

  }
  }
  }
  }
  if (yyP221->Kind == kAnExpr) {
  if (yyP221->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP221->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP222->AFormal.Type)))) goto yyL3;
  {
/* line 4027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP222->AFormal.Type)))) goto yyL3;
  {
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ChDope");
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CountArrayDims (yyP222->AFormal.Type) - 1);
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP220);
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4028 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4029 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
  }
  }
   return;
yyL3:;

/* line 4032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP222->AFormal.Type)))) goto yyL4;
  {
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("Dope");
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CountArrayDims (yyP222->AFormal.Type));
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP220);
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4033 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4034 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
  }
   return;
yyL4:;

  }
  }
  }
  }
  if (yyP221->Kind == kAnExpr) {
  if (yyP221->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP221->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP222->AFormal.Type)))) goto yyL5;
  {
/* line 4037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP221->AnExpr.Expr)))) goto yyL5;
  {
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP222->AFormal.Type);
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP220);
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP221->AnExpr.Expr);
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4039 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
  }
  }
   return;
yyL5:;

/* line 4042 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4043 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
   return;

  }
  }
/* line 4046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP222->AFormal.Type);
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" TMP");
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP220);
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP221->AnExpr.Expr);
/* line 4047 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4048 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Temps (yyP222->AFormal.Next, yyP221->AnExpr.Next, yyP220 + 1);
  }
   return;

  }
  }
 yyAbort ("GenF90Temps");
;
}

static void GenF90Inits
# if defined __STDC__ | defined __cplusplus
(register tTree yyP225, register tTree yyP224, register int yyP223)
# else
(yyP225, yyP224, yyP223)
 register tTree yyP225;
 register tTree yyP224;
 register int yyP223;
# endif
{
/* line 4053 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int tmp; 
  if (yyP224->Kind == kNoExpr) {
/* line 4055 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP225->Kind == kAFormal) {
  if (Tree_IsType (yyP225->AFormal.Type, kArray)) {
  if (Tree_IsType (yyP225->AFormal.Type->Array.Type, kByte)) {
  if (yyP224->Kind == kAnExpr) {
/* line 4057 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4058 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Inits (yyP225->AFormal.Next, yyP224->AnExpr.Next, yyP223 + 1);
  }
   return;

  }
  }
  if (yyP224->Kind == kAnExpr) {
  if (yyP224->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP224->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP225->AFormal.Type)))) goto yyL3;
  {
/* line 4061 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP225->AFormal.Type)))) goto yyL3;
  {
/* line 4062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP223);
/* line 4062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".offset = 0; ");
/* line 4062 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP223);
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".addr = (Pointer)");
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP224->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4063 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4064 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90DopeInits (yyP225->AFormal.Type, ExprType (yyP224->AnExpr.Expr), yyP224->AnExpr.Expr->ElementalExpr.Elemental, yyP223, CountArrayDims (yyP225->AFormal.Type) - 2);
/* line 4065 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Inits (yyP225->AFormal.Next, yyP224->AnExpr.Next, yyP223 + 1);
  }
  }
  }
   return;
yyL3:;

/* line 4068 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4068 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsBoundedType (yyP225->AFormal.Type)))) goto yyL4;
  {
/* line 4069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP223);
/* line 4069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".offset = 0; ");
/* line 4069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP223);
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".addr = (Pointer)");
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP224->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4070 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4071 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90DopeInits (yyP225->AFormal.Type, ExprType (yyP224->AnExpr.Expr), yyP224->AnExpr.Expr->ElementalExpr.Elemental, yyP223, CountArrayDims (yyP225->AFormal.Type) - 1);
/* line 4072 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Inits (yyP225->AFormal.Next, yyP224->AnExpr.Next, yyP223 + 1);
  }
  }
   return;
yyL4:;

  }
  }
  }
  }
  if (yyP224->Kind == kAnExpr) {
/* line 4075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Inits (yyP225->AFormal.Next, yyP224->AnExpr.Next, yyP223 + 1);
  }
   return;

  }
  }
 yyAbort ("GenF90Inits");
;
}

static void GenF90DopeInits
# if defined __STDC__ | defined __cplusplus
(register tTree yyP230, register tTree yyP229, register tTree yyP228, register int yyP227, register int yyP226)
# else
(yyP230, yyP229, yyP228, yyP227, yyP226)
 register tTree yyP230;
 register tTree yyP229;
 register tTree yyP228;
 register int yyP227;
 register int yyP226;
# endif
{
/* line 4081 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int extent; 
/* line 4083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[");
/* line 4084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP226);
/* line 4084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("].lower = 0;");
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[");
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP226);
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("].extent = ");
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayDimSize (yyP229, yyP228);
/* line 4085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4086 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
   return;
yyL1:;

  if (Tree_IsType (yyP229, kArray)) {
/* line 4089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP226 == 0))) goto yyL2;
  {
/* line 4089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP229->Array.Type)))) goto yyL2;
  {
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".len = ");
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayDimSize (yyP229->Array.Type, yyP228);
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[0].mult = ");
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayDimSize (yyP229->Array.Type, yyP228);
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4091 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return;
yyL2:;

/* line 4094 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4094 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP226 > 0))) goto yyL3;
  {
/* line 4095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90DopeInits (yyP230, yyP229->Array.Type, yyP228, yyP227, yyP226 - 1);
/* line 4096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL3;
  }
  }
   return;
yyL3:;

/* line 4099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP226 > 0))) goto yyL4;
  {
/* line 4100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[");
/* line 4100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP226);
/* line 4100 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("].mult = ");
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[");
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP226 - 1);
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("].mult * ");
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayDimSize (yyP229->Array.Type, yyP228);
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL4:;

/* line 4104 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP227);
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".dim[");
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP226);
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("].mult = sizeof(");
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP229->Array.Type);
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 4108 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenF90Params
# if defined __STDC__ | defined __cplusplus
(register tTree yyP234, register tTree yyP233, register int yyP232, register bool yyP231)
# else
(yyP234, yyP233, yyP232, yyP231)
 register tTree yyP234;
 register tTree yyP233;
 register int yyP232;
 register bool yyP231;
# endif
{
  if (yyP233->Kind == kNoExpr) {
/* line 4116 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP234->Kind == kAFormal) {
  if (Tree_IsType (yyP234->AFormal.Type, kArray)) {
  if (Tree_IsType (yyP234->AFormal.Type->Array.Type, kByte)) {
  if (yyP233->Kind == kAnExpr) {
  if (yyP233->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP233->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4118 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4119 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ArrayPrimType (yyP234->AFormal.Type));
/* line 4120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4120 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP233->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
   return;

  }
  }
  }
  }
  if (yyP233->Kind == kAnExpr) {
  if (yyP233->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP233->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4124 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsBoundedType (yyP234->AFormal.Type)))) goto yyL3;
  {
/* line 4125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ArrayPrimType (yyP234->AFormal.Type));
/* line 4126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4126 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP233->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
  }
   return;
yyL3:;

/* line 4130 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&TMP");
/* line 4132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP232);
/* line 4133 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
   return;

  }
  }
  }
  }
  if (yyP233->Kind == kAnExpr) {
  if (yyP233->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP233->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP234->AFormal.Type)))) goto yyL5;
  {
/* line 4136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP233->AnExpr.Expr)))) goto yyL5;
  {
/* line 4137 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&TMP");
/* line 4138 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP232);
/* line 4139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
  }
  }
   return;
yyL5:;

/* line 4142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4143 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4144 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP233->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4145 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
   return;

  }
  }
/* line 4148 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4149 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP231);
/* line 4150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&TMP");
/* line 4150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP232);
/* line 4151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Params (yyP234->AFormal.Next, yyP233->AnExpr.Next, yyP232 + 1, true);
  }
   return;

  }
  }
 yyAbort ("GenF90Params");
;
}

static void GenF90CharParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP237, register tTree yyP236, register int yyP235)
# else
(yyP237, yyP236, yyP235)
 register tTree yyP237;
 register tTree yyP236;
 register int yyP235;
# endif
{
  if (yyP236->Kind == kNoExpr) {
/* line 4157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP237->Kind == kAFormal) {
  if (Tree_IsType (yyP237->AFormal.Type, kArray)) {
  if (Tree_IsType (yyP237->AFormal.Type->Array.Type, kByte)) {
  if (yyP236->Kind == kAnExpr) {
  if (yyP236->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP236->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4159 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (true);
/* line 4161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayDimSize (ExprType (yyP236->AnExpr.Expr), yyP236->AnExpr.Expr->ElementalExpr.Elemental);
/* line 4162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90CharParams (yyP237->AFormal.Next, yyP236->AnExpr.Next, yyP235 + 1);
  }
   return;

  }
  }
  }
  }
  if (yyP236->Kind == kAnExpr) {
  if (yyP236->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP236->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP237->AFormal.Type)))) goto yyL3;
  {
/* line 4166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (true);
/* line 4167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TMP");
/* line 4167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP235);
/* line 4167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".len");
/* line 4168 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90CharParams (yyP237->AFormal.Next, yyP236->AnExpr.Next, yyP235 + 1);
  }
  }
   return;
yyL3:;

  }
  }
  }
  }
  if (yyP236->Kind == kAnExpr) {
/* line 4171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4172 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90CharParams (yyP237->AFormal.Next, yyP236->AnExpr.Next, yyP235 + 1);
  }
   return;

  }
  }
 yyAbort ("GenF90CharParams");
;
}

static void GenF90Results
# if defined __STDC__ | defined __cplusplus
(register tTree yyP240, register tTree yyP239, register int yyP238)
# else
(yyP240, yyP239, yyP238)
 register tTree yyP240;
 register tTree yyP239;
 register int yyP238;
# endif
{
  if (yyP239->Kind == kNoExpr) {
/* line 4178 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP240->Kind == kAFormal) {
  if (yyP239->Kind == kAnExpr) {
  if (yyP239->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP239->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsBoundedType (yyP240->AFormal.Type)))) goto yyL2;
  {
/* line 4181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Results (yyP240->AFormal.Next, yyP239->AnExpr.Next, yyP238 + 1);
  }
  }
   return;
yyL2:;

  }
  }
/* line 4188 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Results (yyP240->AFormal.Next, yyP239->AnExpr.Next, yyP238 + 1);
  }
   return;

  }
  if (Tree_IsType (yyP240->AFormal.Type, kArray)) {
  if (yyP239->Kind == kAnExpr) {
  if (yyP239->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP239->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4184 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4185 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenF90Results (yyP240->AFormal.Next, yyP239->AnExpr.Next, yyP238 + 1);
  }
   return;

  }
  }
  }
  }
  }
 yyAbort ("GenF90Results");
;
}

static void GenArrayDimSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP242, register tTree yyP241)
# else
(yyP242, yyP241)
 register tTree yyP242;
 register tTree yyP241;
# endif
{
  if (Tree_IsType (yyP242, kArray)) {
  if (yyP242->Array.Expr->Kind == kUnBound) {
/* line 4194 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP241));
/* line 4195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4195 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP242));
  }
   return;

  }
/* line 4198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (ValConstInt (yyP242->Array.Expr));
  }
   return;

  }
 yyAbort ("GenArrayDimSize");
;
}

static void GenExtParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP246, register tTree yyP245, register tIdent yyP244, register int yyP243)
# else
(yyP246, yyP245, yyP244, yyP243)
 register tTree yyP246;
 register tTree yyP245;
 register tIdent yyP244;
 register int yyP243;
# endif
{
  if (Tree_IsType (yyP246, kNoFormal)) {
  if (yyP245->Kind == kNoExpr) {
/* line 4207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
  if (yyP246->Kind == kAFormal) {
  if (yyP245->Kind == kAnExpr) {
/* line 4209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP246->AFormal.Type)))) goto yyL2;
  {
/* line 4210 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP246->AFormal.Next, yyP245->AnExpr.Next, yyP244, yyP243);
  }
  }
   return;
yyL2:;

  }
  if (Tree_IsType (yyP246->AFormal.Type, kArray)) {
  if (yyP245->Kind == kAnExpr) {
/* line 4212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP246->AFormal.Type, ExprType (yyP245->AnExpr.Expr), yyP245->AnExpr.Expr, yyP244, yyP243);
/* line 4214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP246->AFormal.Next, yyP245->AnExpr.Next, yyP244, yyP243 + 1);
  }
   return;

  }
  }
  if (yyP246->AFormal.By->Kind == kByRef) {
  if (yyP245->Kind == kAnExpr) {
  if (yyP245->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP245->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4216 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP245->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL4;
  {
/* line 4217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP244);
/* line 4217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP243);
/* line 4218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP245->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 4218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4218 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP246->AFormal.Next, yyP245->AnExpr.Next, yyP244, yyP243 + 1);
  }
  }
   return;
yyL4:;

/* line 4221 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP244);
/* line 4222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4222 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP243);
/* line 4223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP245->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4223 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP246->AFormal.Next, yyP245->AnExpr.Next, yyP244, yyP243 + 1);
  }
   return;

  }
  }
  }
  }
  if (yyP246->AFormal.By->Kind == kByVal) {
  if (yyP245->Kind == kAnExpr) {
/* line 4226 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP244);
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP243);
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP245->AnExpr.Expr);
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4227 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtParams (yyP246->AFormal.Next, yyP245->AnExpr.Next, yyP244, yyP243 + 1);
  }
   return;

  }
  }
  }
 yyAbort ("GenExtParams");
;
}

static void GenExtArrayParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP251, register tTree yyP250, register tTree yyP249, register tIdent yyP248, register int yyP247)
# else
(yyP251, yyP250, yyP249, yyP248, yyP247)
 register tTree yyP251;
 register tTree yyP250;
 register tTree yyP249;
 register tIdent yyP248;
 register int yyP247;
# endif
{
  if (Tree_IsType (yyP251, kArray)) {
  if (yyP251->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP250, kArray)) {
  if (yyP250->Array.Expr->Kind == kUnBound) {
  if (yyP249->Kind == kElementalExpr) {
  if (Tree_IsType (yyP249->ElementalExpr.Elemental, kElement)) {
/* line 4233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP248);
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP247);
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP251));
/* line 4234 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP249->ElementalExpr.Elemental));
/* line 4235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP250));
/* line 4235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4236 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP251->Array.Type, yyP250->Array.Type, yyP249, yyP248, yyP247);
  }
   return;

  }
  }
  }
/* line 4238 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP248);
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP247);
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP251));
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP250->Array.Expr);
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4240 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP251->Array.Type, yyP250->Array.Type, yyP249, yyP248, yyP247);
  }
   return;

  }
  }
  if (Tree_IsType (yyP250, kArray)) {
  if (yyP250->Array.Expr->Kind == kUnBound) {
  if (yyP249->Kind == kElementalExpr) {
  if (Tree_IsType (yyP249->ElementalExpr.Elemental, kElement)) {
/* line 4242 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 4243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP251->Array.Expr);
/* line 4243 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP249->ElementalExpr.Elemental));
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP250));
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4244 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4245 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP251->Array.Type, yyP250->Array.Type, yyP249, yyP248, yyP247);
  }
   return;

  }
  }
  }
/* line 4247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP251->Array.Expr)))) goto yyL4;
  {
/* line 4247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP250->Array.Expr)))) goto yyL4;
  {
/* line 4248 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP251->Array.Type, yyP250->Array.Type, yyP249, yyP248, yyP247);
  }
  }
  }
   return;
yyL4:;

/* line 4250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP251->Array.Expr);
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP250->Array.Expr);
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4251 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenExtArrayParams (yyP251->Array.Type, yyP250->Array.Type, yyP249, yyP248, yyP247);
  }
   return;

  }
  }
  if (yyP249->Kind == kElementalExpr) {
  if (Tree_IsType (yyP249->ElementalExpr.Elemental, kElement)) {
/* line 4254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4254 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP251)))) goto yyL6;
  {
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP248);
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP247);
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= (");
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP251);
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP249->ElementalExpr.Elemental, true);
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL6:;

/* line 4257 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP248);
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (PRM);
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP247);
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= (");
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP251);
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP249->ElementalExpr.Elemental, true);
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4258 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
/* line 4260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4260 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP249, yyP249->Expr.Pos);
  }
   return;

;
}

static void GenIntParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP255, register tTree yyP254, register tIdent yyP253, tSymbol yyP252)
# else
(yyP255, yyP254, yyP253, yyP252)
 register tTree yyP255;
 register tTree yyP254;
 register tIdent yyP253;
 tSymbol yyP252;
# endif
{
  if (Tree_IsType (yyP255, kNoFormal)) {
  if (yyP254->Kind == kNoExpr) {
/* line 4265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
  if (yyP255->Kind == kAFormal) {
  if (yyP254->Kind == kAnExpr) {
/* line 4267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP255->AFormal.Type)))) goto yyL2;
  {
/* line 4268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP255->AFormal.Next, yyP254->AnExpr.Next, yyP253, yyP252);
  }
  }
   return;
yyL2:;

  }
  if (Tree_IsType (yyP255->AFormal.Type, kArray)) {
  if (yyP254->Kind == kAnExpr) {
/* line 4270 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP255->AFormal.Type, ExprType (yyP254->AnExpr.Expr), yyP255->AFormal.Sym, yyP254->AnExpr.Expr, yyP253, yyP252);
/* line 4272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP255->AFormal.Next, yyP254->AnExpr.Next, yyP253, yyP252);
  }
   return;

  }
  }
  if (yyP255->AFormal.By->Kind == kByRef) {
  if (yyP254->Kind == kAnExpr) {
  if (yyP254->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP254->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4274 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP254->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL4;
  {
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP253, yyP252);
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP255->AFormal.Sym);
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP254->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4275 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4276 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP255->AFormal.Next, yyP254->AnExpr.Next, yyP253, yyP252);
  }
  }
   return;
yyL4:;

/* line 4278 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP253, yyP252);
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP255->AFormal.Sym);
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP254->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4279 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4280 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP255->AFormal.Next, yyP254->AnExpr.Next, yyP253, yyP252);
  }
   return;

  }
  }
  }
  }
  if (yyP255->AFormal.By->Kind == kByVal) {
  if (yyP254->Kind == kAnExpr) {
/* line 4282 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP253, yyP252);
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP255->AFormal.Sym);
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP254->AnExpr.Expr);
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4283 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntParams (yyP255->AFormal.Next, yyP254->AnExpr.Next, yyP253, yyP252);
  }
   return;

  }
  }
  }
 yyAbort ("GenIntParams");
;
}

static void GenChain
# if defined __STDC__ | defined __cplusplus
(register tIdent yyP258, tSymbol yyP257, register bool yyP256)
# else
(yyP258, yyP257, yyP256)
 register tIdent yyP258;
 tSymbol yyP257;
 register bool yyP256;
# endif
{
  if (equalbool (yyP256, true)) {
/* line 4291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP258);
/* line 4291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".Chain = NULL;");
/* line 4291 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP258);
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".Chain = ");
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP");
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP257, CurProc);
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4292 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenIntArrayParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP264, register tTree yyP263, tSymbol yyP262, register tTree yyP261, register tIdent yyP260, tSymbol yyP259)
# else
(yyP264, yyP263, yyP262, yyP261, yyP260, yyP259)
 register tTree yyP264;
 register tTree yyP263;
 tSymbol yyP262;
 register tTree yyP261;
 register tIdent yyP260;
 tSymbol yyP259;
# endif
{
  if (Tree_IsType (yyP264, kArray)) {
  if (yyP264->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP263, kArray)) {
  if (yyP263->Array.Expr->Kind == kUnBound) {
  if (yyP261->Kind == kElementalExpr) {
  if (Tree_IsType (yyP261->ElementalExpr.Elemental, kElement)) {
/* line 4297 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP260, yyP259);
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP262);
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP264));
/* line 4298 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP261->ElementalExpr.Elemental));
/* line 4299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP263));
/* line 4299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4299 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4300 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP264->Array.Type, yyP263->Array.Type, yyP262, yyP261, yyP260, yyP259);
  }
   return;

  }
  }
  }
/* line 4302 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP260, yyP259);
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP262);
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP264));
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP263->Array.Expr);
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4303 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4304 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP264->Array.Type, yyP263->Array.Type, yyP262, yyP261, yyP260, yyP259);
  }
   return;

  }
  }
  if (Tree_IsType (yyP263, kArray)) {
  if (yyP263->Array.Expr->Kind == kUnBound) {
  if (yyP261->Kind == kElementalExpr) {
  if (Tree_IsType (yyP261->ElementalExpr.Elemental, kElement)) {
/* line 4306 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 4307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP264->Array.Expr);
/* line 4307 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP261->ElementalExpr.Elemental));
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP263));
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP264->Array.Type, yyP263->Array.Type, yyP262, yyP261, yyP260, yyP259);
  }
   return;

  }
  }
  }
/* line 4311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP264->Array.Expr)))) goto yyL4;
  {
/* line 4311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP263->Array.Expr)))) goto yyL4;
  {
/* line 4312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP264->Array.Type, yyP263->Array.Type, yyP262, yyP261, yyP260, yyP259);
  }
  }
  }
   return;
yyL4:;

/* line 4314 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP264->Array.Expr);
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("==");
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP263->Array.Expr);
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_ASM);
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4316 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenIntArrayParams (yyP264->Array.Type, yyP263->Array.Type, yyP262, yyP261, yyP260, yyP259);
  }
   return;

  }
  }
  if (yyP261->Kind == kElementalExpr) {
  if (Tree_IsType (yyP261->ElementalExpr.Elemental, kElement)) {
/* line 4318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4318 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP264)))) goto yyL6;
  {
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP260, yyP259);
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP262);
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= (");
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP264);
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP261->ElementalExpr.Elemental, true);
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4319 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL6:;

/* line 4321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFrameRef (yyP260, yyP259);
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (".");
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP262);
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= (");
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP264);
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP261->ElementalExpr.Elemental, true);
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
/* line 4324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP261, yyP261->Expr.Pos);
  }
   return;

;
}

static void GenFrameRef
# if defined __STDC__ | defined __cplusplus
(register tIdent yyP266, tSymbol yyP265)
# else
(yyP266, yyP265)
 register tIdent yyP266;
 tSymbol yyP265;
# endif
{
  {tSymbol yyT; yyT = NoSymbol;
  if (equaltSymbol (yyP265, yyT)) {
/* line 4329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4329 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP266);
  }
   return;

  }
  }
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->");
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteId (yyP266);
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP265) . Kind, yyP265, false);
/* line 4330 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
  }
   return;

;
}

static void GenFuncCall
# if defined __STDC__ | defined __cplusplus
(register tTree yyP269, register tTree yyP268, register tTree yyP267)
# else
(yyP269, yyP268, yyP267)
 register tTree yyP269;
 register tTree yyP268;
 register tTree yyP267;
# endif
{
/* line 4335 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 bool comma = false; 
/* line 4337 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP268->FuncCall.Sym);
/* line 4338 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4339 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((NeedsChain (yyP269)))) goto yyL1;
  {
/* line 4340 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP");
/* line 4340 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP268->FuncCall.Sym, CurProc);
/* line 4341 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = true;
/* line 4342 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

  if (yyP269->DefFunc.Formals->Kind == kAFormal) {
/* line 4345 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4346 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP269->DefFunc.Formals, yyP268->FuncCall.ExprList, comma);
/* line 4347 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   comma = true;
/* line 4348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
   return;
yyL2:;

  }
/* line 4351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (yyP269) || IsSTVFunction (yyP269)))) goto yyL3;
  {
/* line 4351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP267, kVal)))) goto yyL3;
  {
/* line 4352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (comma, ",");
/* line 4353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP268));
/* line 4353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4354 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP267 -> Val . Sym);
/* line 4355 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL3;
  }
  }
  }
   return;
yyL3:;

/* line 4358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsMVFunction (yyP269)))) goto yyL4;
  {
/* line 4358 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP267, kExprList)))) goto yyL4;
  {
/* line 4359 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 CodeMVFParams(yyP267,comma,0); 
/* line 4360 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL4;
  }
  }
  }
   return;
yyL4:;

/* line 4363 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4364 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void CodeMVFParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP272, register bool yyP271, register int yyP270)
# else
(yyP272, yyP271, yyP270)
 register tTree yyP272;
 register bool yyP271;
 register int yyP270;
# endif
{
  if (yyP272->Kind == kNoExpr) {
/* line 4372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP272->Kind == kAnExpr) {
  if (yyP272->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP272->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4374 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TempRequired (yyP272)))) goto yyL2;
  {
/* line 4375 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP271);
/* line 4376 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsScalarType (ElementType (yyP272->AnExpr.Expr->ElementalExpr.Elemental)), "&");
/* line 4377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_TMP");
/* line 4377 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP270);
/* line 4378 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFParams (yyP272->AnExpr.Next, true, yyP270 + 1);
  }
  }
   return;
yyL2:;

/* line 4381 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP271);
/* line 4383 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP272->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4384 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFParams (yyP272->AnExpr.Next, true, yyP270 + 1);
  }
   return;

  }
  }
  }
 yyAbort ("CodeMVFParams");
;
}

static void CodeMVFTemps
# if defined __STDC__ | defined __cplusplus
(register tTree yyP274, register int yyP273)
# else
(yyP274, yyP273)
 register tTree yyP274;
 register int yyP273;
# endif
{
  if (yyP274->Kind == kNoExpr) {
/* line 4391 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP274->Kind == kAnExpr) {
  if (yyP274->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP274->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4393 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TempRequired (yyP274)))) goto yyL2;
  {
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP274->AnExpr.Expr->ElementalExpr.Elemental));
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" _TMP");
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP273);
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (ElementType (yyP274->AnExpr.Expr->ElementalExpr.Elemental));
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4395 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTemps (yyP274->AnExpr.Next, yyP273 + 1);
  }
  }
   return;
yyL2:;

/* line 4398 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4399 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTemps (yyP274->AnExpr.Next, yyP273 + 1);
  }
   return;

  }
  }
  }
 yyAbort ("CodeMVFTemps");
;
}

static void CodeMVFTempCopies
# if defined __STDC__ | defined __cplusplus
(register tTree yyP276, register int yyP275)
# else
(yyP276, yyP275)
 register tTree yyP276;
 register int yyP275;
# endif
{
  if (yyP276->Kind == kNoExpr) {
/* line 4405 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP276->Kind == kAnExpr) {
  if (yyP276->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP276->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TempRequired (yyP276)))) goto yyL2;
  {
/* line 4407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (ElementType (yyP276->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL2;
  {
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (ASSIGN);
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP276->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (", _TMP");
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP275);
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP276->AnExpr.Expr->ElementalExpr.Elemental, ElementType (yyP276->AnExpr.Expr->ElementalExpr.Elemental));
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4409 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTempCopies (yyP276->AnExpr.Next, yyP275 + 1);
  }
  }
  }
   return;
yyL2:;

/* line 4412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4412 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((TempRequired (yyP276)))) goto yyL3;
  {
/* line 4413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP276->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 4413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = _TMP");
/* line 4413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP275);
/* line 4413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTempCopies (yyP276->AnExpr.Next, yyP275 + 1);
  }
  }
   return;
yyL3:;

/* line 4417 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4418 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeMVFTempCopies (yyP276->AnExpr.Next, yyP275 + 1);
  }
   return;

  }
  }
  }
 yyAbort ("CodeMVFTempCopies");
;
}

static void GenParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP279, register tTree yyP278, register bool yyP277)
# else
(yyP279, yyP278, yyP277)
 register tTree yyP279;
 register tTree yyP278;
 register bool yyP277;
# endif
{
  if (yyP278->Kind == kNoExpr) {
/* line 4425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP279->Kind == kAFormal) {
  if (yyP278->Kind == kAnExpr) {
/* line 4427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTimerType (yyP279->AFormal.Type)))) goto yyL2;
  {
/* line 4428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP279->AFormal.Next, yyP278->AnExpr.Next, yyP277);
  }
  }
   return;
yyL2:;

  }
  if (Tree_IsType (yyP279->AFormal.Type, kArray)) {
  if (yyP278->Kind == kAnExpr) {
/* line 4431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP279->AFormal.Type, ExprType (yyP278->AnExpr.Expr), yyP278->AnExpr.Expr, yyP277);
/* line 4433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP279->AFormal.Next, yyP278->AnExpr.Next, true);
  }
   return;

  }
  }
  if (yyP279->AFormal.By->Kind == kByRef) {
  if (yyP278->Kind == kAnExpr) {
  if (yyP278->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP278->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP278->AnExpr.Expr->ElementalExpr.Elemental))))) goto yyL4;
  {
/* line 4437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP277);
/* line 4438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP278->AnExpr.Expr->ElementalExpr.Elemental, false);
/* line 4439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP279->AFormal.Next, yyP278->AnExpr.Next, true);
  }
  }
   return;
yyL4:;

/* line 4441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP277);
/* line 4443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP278->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP279->AFormal.Next, yyP278->AnExpr.Next, true);
  }
   return;

  }
  }
  }
  }
  if (yyP279->AFormal.By->Kind == kByVal) {
  if (yyP278->Kind == kAnExpr) {
/* line 4446 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4447 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP277);
/* line 4448 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP278->AnExpr.Expr);
/* line 4449 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenParams (yyP279->AFormal.Next, yyP278->AnExpr.Next, true);
  }
   return;

  }
  }
  }
 yyAbort ("GenParams");
;
}

static void GenArrayParams
# if defined __STDC__ | defined __cplusplus
(register tTree yyP283, register tTree yyP282, register tTree yyP281, register bool yyP280)
# else
(yyP283, yyP282, yyP281, yyP280)
 register tTree yyP283;
 register tTree yyP282;
 register tTree yyP281;
 register bool yyP280;
# endif
{
  if (Tree_IsType (yyP283, kArray)) {
  if (yyP283->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP282, kArray)) {
  if (yyP282->Array.Expr->Kind == kUnBound) {
  if (yyP281->Kind == kElementalExpr) {
  if (Tree_IsType (yyP281->ElementalExpr.Elemental, kElement)) {
/* line 4458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4459 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP280);
/* line 4460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (GetElementSymbol (yyP281->ElementalExpr.Elemental));
/* line 4460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP282));
/* line 4461 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP283->Array.Type, yyP282->Array.Type, yyP281, true);
  }
   return;

  }
  }
  }
/* line 4463 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4464 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP280);
/* line 4465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP282->Array.Expr);
/* line 4466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP283->Array.Type, yyP282->Array.Type, yyP281, true);
  }
   return;

  }
  }
  if (Tree_IsType (yyP282, kArray)) {
/* line 4469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP283->Array.Expr)))) goto yyL3;
  {
/* line 4469 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP282->Array.Expr)))) goto yyL3;
  {
/* line 4470 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP283->Array.Type, yyP282->Array.Type, yyP281, yyP280);
  }
  }
  }
   return;
yyL3:;

  if (yyP282->Array.Expr->Kind == kUnBound) {
  if (yyP281->Kind == kElementalExpr) {
  if (Tree_IsType (yyP281->ElementalExpr.Elemental, kElement)) {
/* line 4473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4475 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP283->Array.Type, yyP282->Array.Type, yyP281, yyP280);
  }
   return;

  }
  }
  }
/* line 4477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayParams (yyP283->Array.Type, yyP282->Array.Type, yyP281, yyP280);
  }
   return;

  }
  }
  if (yyP281->Kind == kElementalExpr) {
  if (Tree_IsType (yyP281->ElementalExpr.Elemental, kElement)) {
/* line 4482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4482 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP283)))) goto yyL6;
  {
/* line 4483 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP280);
/* line 4484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP283);
/* line 4484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
/* line 4484 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP281->ElementalExpr.Elemental, true);
  }
  }
   return;
yyL6:;

/* line 4487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4488 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenComma (yyP280);
/* line 4489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP283);
/* line 4489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 4489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP281->ElementalExpr.Elemental, true);
  }
   return;

  }
  }
/* line 4492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4492 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP281, yyP281->Expr.Pos);
  }
   return;

;
}

static void GenComma
# if defined __STDC__ | defined __cplusplus
(register bool yyP284)
# else
(yyP284)
 register bool yyP284;
# endif
{
  if (equalbool (yyP284, true)) {
/* line 4497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4498 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
  }
   return;

  }
/* line 4501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void GenTimerInput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP285)
# else
(yyP285)
 register tTree yyP285;
# endif
{
  if (yyP285->Kind == kAnExpr) {
/* line 4511 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP285->AnExpr.Expr);
/* line 4512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ReadTimer();");
/* line 4512 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenTimerInput");
;
}

static void GenInput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP287, register tTree T, register tTree yyP286)
# else
(yyP287, T, yyP286)
 register tTree yyP287;
 register tTree T;
 register tTree yyP286;
# endif
{
/* line 4518 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
  if (yyP286->Kind == kNoExpr) {
/* line 4519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP286->Kind == kAnExpr) {
  if (yyP286->AnExpr.Expr->Kind == kCountArray) {
  if (yyP286->AnExpr.Expr->CountArray.Length->Kind == kElementalExpr) {
  if (yyP286->AnExpr.Expr->CountArray.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP286->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4521 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInputToExpr (yyP287, yyP286->AnExpr.Expr->CountArray.Length);
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (CHK);
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP286->AnExpr.Expr->CountArray.Length);
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("<=");
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP286->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental, ExprType (yyP286->AnExpr.Expr->CountArray.Expr));
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (MSG_CITL);
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4523 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INPUT");
/* line 4525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP287, false);
/* line 4525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP286->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental, true);
/* line 4525 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP286->AnExpr.Expr->CountArray.Length);
/* line 4526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")*(");
/* line 4526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (GetElementSymbol (yyP286->AnExpr.Expr->CountArray.Expr->ElementalExpr.Elemental), ExprType (yyP286->AnExpr.Expr->CountArray.Expr) -> Array . Type);
/* line 4526 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
/* line 4527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4527 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 4528 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP287, T, yyP286->AnExpr.Next);
  }
   return;

  }
  }
  }
  }
/* line 4530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP286->AnExpr.Expr))))) goto yyL3;
  {
/* line 4531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInputToExpr (yyP287, yyP286->AnExpr.Expr);
/* line 4532 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP287, T, yyP286->AnExpr.Next);
  }
  }
   return;
yyL3:;

  if (yyP286->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP286->AnExpr.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4535 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INPUT");
/* line 4536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP287, false);
/* line 4536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP286->AnExpr.Expr->ElementalExpr.Elemental, true);
/* line 4536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP286->AnExpr.Expr->ElementalExpr.Elemental, ExprType (yyP286->AnExpr.Expr));
/* line 4537 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4538 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 4539 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenInput (yyP287, T, yyP286->AnExpr.Next);
  }
   return;

  }
  }
  }
/* line 4541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP287, yyP287->Element.Pos);
  }
   return;

;
}

static void GenInputToExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP289, register tTree yyP288)
# else
(yyP289, yyP288)
 register tTree yyP289;
 register tTree yyP288;
# endif
{
/* line 4544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
  if (yyP288->Kind == kElementalExpr) {
  if (Tree_IsType (yyP288->ElementalExpr.Elemental, kElement)) {
/* line 4545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP288))))) goto yyL1;
  {
/* line 4546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INPUT");
/* line 4546 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (ExprType (yyP288)));
/* line 4547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP289, false);
/* line 4547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP288->ElementalExpr.Elemental, true);
/* line 4548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4548 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
  }
   return;
yyL1:;

  }
  }
 yyAbort ("GenInputToExpr");
;
}

static void GenOutput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP292, register tTree yyP291, register tTree yyP290)
# else
(yyP292, yyP291, yyP290)
 register tTree yyP292;
 register tTree yyP291;
 register tTree yyP290;
# endif
{
  if (yyP290->Kind == kNoExpr) {
/* line 4555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP291->Kind == kNamed) {
/* line 4557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP291->Named.Sym), kDefSeqPro)))) goto yyL2;
  {
/* line 4558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSeqProOutput (yyP292, SymbolKind (yyP291->Named.Sym) -> DefSeqPro . SeqProList, yyP290);
  }
  }
   return;
yyL2:;

  if (yyP290->Kind == kAnExpr) {
  if (yyP290->AnExpr.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP290->AnExpr.Expr->ElementalExpr.Elemental, kIdentifier)) {
/* line 4560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP291->Named.Sym), kDefTagPro)))) goto yyL3;
  {
/* line 4561 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutputOfExpr (yyP292, yyP290->AnExpr.Expr, ExprType (yyP290->AnExpr.Expr));
/* line 4562 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSeqProOutput (yyP292, SymbolKind (yyP290->AnExpr.Expr->ElementalExpr.Elemental->Identifier.Sym) -> ATagPro . SeqProList, yyP290->AnExpr.Next);
  }
  }
   return;
yyL3:;

  }
  }
  }
  }
  if (yyP291->Kind == kAny) {
  if (yyP290->Kind == kAnExpr) {
/* line 4564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutput (yyP292, yyP291, yyP290->AnExpr.Expr);
/* line 4564 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOutput (yyP292, yyP291, yyP290->AnExpr.Next);
  }
   return;

  }
  if (yyP290->Kind == kNoExpr) {
/* line 4565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
  if (yyP291->Kind == kSimple) {
  if (yyP290->Kind == kAnExpr) {
  if (yyP290->AnExpr.Next->Kind == kNoExpr) {
/* line 4567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutput (yyP292, yyP291, yyP290->AnExpr.Expr);
  }
   return;

  }
  }
  }
  if (yyP291->Kind == kCounted) {
  if (yyP290->Kind == kAnExpr) {
  if (yyP290->AnExpr.Next->Kind == kNoExpr) {
/* line 4569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutput (yyP292, yyP291, yyP290->AnExpr.Expr);
  }
   return;

  }
  }
  }
  if (yyP290->Kind == kAnExpr) {
/* line 4571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP290->AnExpr.Expr, yyP290->AnExpr.Expr->Expr.Pos);
  }
   return;

  }
 yyAbort ("GenOutput");
;
}

static void GenSeqProOutput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP295, register tTree yyP294, register tTree yyP293)
# else
(yyP295, yyP294, yyP293)
 register tTree yyP295;
 register tTree yyP294;
 register tTree yyP293;
# endif
{
  if (yyP294->Kind == kASeqPro) {
  if (yyP293->Kind == kAnExpr) {
/* line 4574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutput (yyP295, yyP294->ASeqPro.Protocol, yyP293->AnExpr.Expr);
/* line 4576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSeqProOutput (yyP295, yyP294->ASeqPro.Next, yyP293->AnExpr.Next);
  }
   return;

  }
  }
  if (yyP294->Kind == kNoSeqPro) {
  if (yyP293->Kind == kNoExpr) {
/* line 4578 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  }
 yyAbort ("GenSeqProOutput");
;
}

static void GenSimpleOutput
# if defined __STDC__ | defined __cplusplus
(register tTree yyP298, register tTree yyP297, register tTree yyP296)
# else
(yyP298, yyP297, yyP296)
 register tTree yyP298;
 register tTree yyP297;
 register tTree yyP296;
# endif
{
/* line 4581 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
  if (yyP297->Kind == kCounted) {
  if (yyP296->Kind == kCountArray) {
  if (yyP296->CountArray.Expr->Kind == kElementalExpr) {
  if (Tree_IsType (yyP296->CountArray.Expr->ElementalExpr.Elemental, kElement)) {
/* line 4583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4584 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutputOfExpr (yyP298, yyP296->CountArray.Length, ExprType (yyP296->CountArray.Length));
/* line 4585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP298, false);
/* line 4586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP296->CountArray.Expr->ElementalExpr.Elemental, true);
/* line 4586 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP296->CountArray.Length);
/* line 4587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")*(");
/* line 4587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (GetElementSymbol (yyP296->CountArray.Expr->ElementalExpr.Elemental), ExprType (yyP296->CountArray.Expr) -> Array . Type);
/* line 4587 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
/* line 4588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4588 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
   return;

  }
  }
  }
  if (yyP296->Kind == kElementalExpr) {
  if (Tree_IsType (yyP296->ElementalExpr.Elemental, kElement)) {
/* line 4591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4592 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->_Header.Temp.V");
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP297->Counted.Type);
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSizeOp (yyP296->ElementalExpr.Elemental, ExprType (yyP296));
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4593 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4594 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4594 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (yyP297->Counted.Type));
/* line 4595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP298, false);
/* line 4595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->_Header.Temp.V");
/* line 4595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP297->Counted.Type);
/* line 4596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
/* line 4597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP298, false);
/* line 4598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP296->ElementalExpr.Elemental, true);
/* line 4598 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP296->ElementalExpr.Elemental, ExprType (yyP296));
/* line 4599 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
/* line 4600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4600 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
   return;

  }
  }
  }
/* line 4603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4603 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP296))))) goto yyL3;
  {
/* line 4604 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenSimpleOutputOfExpr (yyP298, yyP296, ExprType (yyP296));
  }
  }
   return;
yyL3:;

  if (yyP296->Kind == kElementalExpr) {
  if (Tree_IsType (yyP296->ElementalExpr.Elemental, kElement)) {
/* line 4607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP298, false);
/* line 4609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP296->ElementalExpr.Elemental, true);
/* line 4609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenElementSize (yyP296->ElementalExpr.Elemental, ExprType (yyP296));
/* line 4610 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4611 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
   return;

  }
  }
/* line 4613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP296, yyP296->Expr.Pos);
  }
   return;

;
}

static void GenSimpleOutputOfExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP301, register tTree yyP300, register tTree yyP299)
# else
(yyP301, yyP300, yyP299)
 register tTree yyP301;
 register tTree yyP300;
 register tTree yyP299;
# endif
{
/* line 4616 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 int lab; 
/* line 4618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP300))))) goto yyL1;
  {
/* line 4618 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP300)))) goto yyL1;
  {
/* line 4619 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->_Header.Temp.V");
/* line 4620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP299);
/* line 4620 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 4621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP300);
/* line 4621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4622 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4622 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (yyP299));
/* line 4623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP301, false);
/* line 4623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->_Header.Temp.V");
/* line 4623 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP299);
/* line 4624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4624 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
  }
  }
   return;
yyL1:;

  if (yyP300->Kind == kElementalExpr) {
  if (Tree_IsType (yyP300->ElementalExpr.Elemental, kElement)) {
/* line 4627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP300))))) goto yyL2;
  {
/* line 4628 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4629 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4629 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (yyP299));
/* line 4630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP301, false);
/* line 4630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4630 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP300->ElementalExpr.Elemental, true);
/* line 4631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4631 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
  }
   return;
yyL2:;

  }
  }
/* line 4634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP300))))) goto yyL3;
  {
/* line 4635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   lab = NewLabel ();
/* line 4636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP->_Header.Temp.V");
/* line 4636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP299);
/* line 4636 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 4637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP300);
/* line 4637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 4637 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("OUTPUT");
/* line 4638 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (PrimTypeSize (yyP299));
/* line 4639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChannelElement (yyP301, false);
/* line 4639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",&FP->_Header.Temp.V");
/* line 4639 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP299);
/* line 4640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabelValue (lab);
/* line 4640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 4640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 4640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteLabel (lab);
  }
  }
   return;
yyL3:;

/* line 4644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4644 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP300, yyP300->Expr.Pos);
  }
   return;

;
}

static void CodeElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP303, register bool yyP302)
# else
(yyP303, yyP302)
 register tTree yyP303;
 register bool yyP302;
# endif
{
/* line 4654 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tSymbol Sym; 
/* line 4656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4656 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (ElementType (yyP303))))) goto yyL1;
  {
/* line 4657 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Unexpected channel element", xxError, yyP303->Element.Pos);
  }
  }
   return;
yyL1:;

  if (Tree_IsType (yyP303, kIdentifier)) {
  if (equalbool (yyP302, false)) {
/* line 4659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4659 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((ConstFlag (yyP303 -> Element) && IsScalarType (ElementType (yyP303))))) goto yyL2;
  {
/* line 4660 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (ElementType (yyP303), yyP303 -> Element . CV);
  }
  }
   return;
yyL2:;

  }
/* line 4662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4662 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolType (yyP303->Identifier.Sym), kArray)))) goto yyL3;
  {
/* line 4663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP303->Identifier.Sym);
  }
  }
   return;
yyL3:;

/* line 4665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4665 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolType (yyP303->Identifier.Sym), kUserStructTypes)))) goto yyL4;
  {
/* line 4666 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP303->Identifier.Sym);
  }
  }
   return;
yyL4:;

/* line 4668 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4669 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (Symbol (yyP303->Identifier.Sym) . Kind, yyP303->Identifier.Sym, yyP302);
  }
   return;

  }
  if (yyP303->Kind == kSubscript) {
/* line 4671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! yyP302))) goto yyL6;
  {
/* line 4671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! Tree_IsType (ElementType (yyP303), kArray)))) goto yyL6;
  {
/* line 4671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! Tree_IsType (ElementType (yyP303), kUserStructTypes)))) goto yyL6;
  {
/* line 4672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeElement (yyP303, true);
  }
  }
  }
  }
   return;
yyL6:;

/* line 4675 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Sym = GetElementSymbol (yyP303);
/* line 4677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 4677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP303));
/* line 4677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)(((BYTE*)");
/* line 4677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (Sym);
/* line 4677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (Sym, ElementType (yyP303), yyP303);
/* line 4678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
  }
   return;

  }
  if (yyP303->Kind == kRange) {
/* line 4680 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Sym = GetElementSymbol (yyP303);
/* line 4682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 4682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP303));
/* line 4682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)(((BYTE*)");
/* line 4682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (Sym);
/* line 4682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (Sym, ElementType (yyP303) -> Array . Type, yyP303);
/* line 4683 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
  }
   return;

  }
/* line 4685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4685 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP303, yyP303->Element.Pos);
  }
   return;

;
}

static void CodeChannelElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP305, register bool yyP304)
# else
(yyP305, yyP304)
 register tTree yyP305;
 register bool yyP304;
# endif
{
/* line 4690 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 tSymbol Sym; 
/* line 4692 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4692 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsChanType (ElementType (yyP305))))) goto yyL1;
  {
/* line 4693 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Expected channel element", xxError, yyP305->Element.Pos);
  }
  }
   return;
yyL1:;

  if (Tree_IsType (yyP305, kIdentifier)) {
  if (equalbool (yyP304, false)) {
/* line 4695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolType (yyP305->Identifier.Sym), kArray)))) goto yyL2;
  {
/* line 4696 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Can only take reference to channel arrays", xxError, yyP305->Identifier.Pos);
  }
  }
   return;
yyL2:;

  }
  if (equalbool (yyP304, true)) {
/* line 4698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolType (yyP305->Identifier.Sym), kArray)))) goto yyL3;
  {
/* line 4699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP305->Identifier.Sym);
  }
  }
   return;
yyL3:;

  }
  if (equalbool (yyP304, true)) {
/* line 4701 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(&");
/* line 4702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP305->Identifier.Sym);
/* line 4702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
/* line 4704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4705 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP305->Identifier.Sym);
  }
   return;

  }
  if (yyP305->Kind == kSubscript) {
  if (equalbool (yyP304, false)) {
/* line 4707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4707 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! Tree_IsType (ElementType (yyP305), kArray)))) goto yyL6;
  {
/* line 4708 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Sym = GetElementSymbol (yyP305);
/* line 4709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*((");
/* line 4709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP305));
/* line 4709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)(((BYTE*)");
/* line 4709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (Sym);
/* line 4709 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (Sym, ElementType (yyP305), yyP305);
/* line 4710 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
  }
  }
   return;
yyL6:;

  }
/* line 4712 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4713 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Sym = GetElementSymbol (yyP305);
/* line 4714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 4714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP305));
/* line 4714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)(((BYTE*)");
/* line 4714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (Sym);
/* line 4714 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4715 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (Sym, ElementType (yyP305), yyP305);
/* line 4715 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
  }
   return;

  }
  if (yyP305->Kind == kRange) {
  if (equalbool (yyP304, true)) {
/* line 4717 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4718 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Sym = GetElementSymbol (yyP305);
/* line 4719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 4719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ElementType (yyP305));
/* line 4719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)(((BYTE*)");
/* line 4719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (Sym);
/* line 4719 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (Sym, ElementType (yyP305) -> Array . Type, yyP305);
/* line 4720 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("))");
  }
   return;

  }
/* line 4722 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4723 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Can only take reference to channel arrays", xxError, yyP305->Range.Pos);
  }
   return;

  }
/* line 4725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4725 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP305, yyP305->Element.Pos);
  }
   return;

;
}

static void GenArrayIndex
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP308, register tTree yyP307, register tTree yyP306)
# else
(yyP308, yyP307, yyP306)
 tSymbol yyP308;
 register tTree yyP307;
 register tTree yyP306;
# endif
{
  if (yyP306->Kind == kSubscript) {
/* line 4735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4735 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (ElementType (yyP306->Subscript.Next), kUserStructTypes)))) goto yyL1;
  {
/* line 4736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("+");
/* line 4736 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (GetFieldOffset (ElementType (yyP306->Subscript.Next), yyP306->Subscript.Expr));
/* line 4737 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (yyP308, yyP307, yyP306->Subscript.Next);
  }
  }
   return;
yyL1:;

/* line 4739 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("+(");
/* line 4740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChkExpr (DoBndChk (yyP306), (Tree_IsType (ElementType (yyP306->Subscript.Next) -> Array . Expr, kUnBound)), yyP306->Subscript.Expr, yyP308, ElementType (yyP306->Subscript.Next));
/* line 4740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (yyP308, ElementType (yyP306));
/* line 4740 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4741 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (yyP308, yyP307, yyP306->Subscript.Next);
  }
   return;

  }
  if (yyP306->Kind == kRange) {
/* line 4743 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4744 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("+(");
/* line 4745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4745 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChkUBExpr (DoUprBndChk (yyP306), (Tree_IsType (ElementType (yyP306->Range.Next) -> Array . Expr, kUnBound)), yyP306->Range.From, yyP306->Range.For, yyP308, ElementType (yyP306->Range.Next));
/* line 4746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeChkLBExpr (DoLwrBndChk (yyP306), yyP306->Range.From);
/* line 4746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (yyP308, ElementType (yyP306) -> Array . Type);
/* line 4746 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
/* line 4747 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArrayIndex (yyP308, yyP307, yyP306->Range.Next);
  }
   return;

  }
  if (Tree_IsType (yyP306, kIdentifier)) {
/* line 4749 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenArrayIndex");
;
}

static void GenElementSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP310, register tTree yyP309)
# else
(yyP310, yyP309)
 register tTree yyP310;
 register tTree yyP309;
# endif
{
  if (yyP310->Kind == kRange) {
  if (Tree_IsType (yyP309, kArray)) {
/* line 4760 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP310->Range.For);
/* line 4761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (GetElementSymbol (yyP310), yyP309->Array.Type);
/* line 4761 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  }
/* line 4764 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4765 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (GetElementSymbol (yyP310), yyP309);
  }
   return;

;
}

static void GenArraySize
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP312, register tTree yyP311)
# else
(yyP312, yyP311)
 tSymbol yyP312;
 register tTree yyP311;
# endif
{
  if (Tree_IsType (yyP311, kArray)) {
/* line 4774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4774 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstExpr (yyP311->Array.Expr)))) goto yyL1;
  {
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP312);
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP311));
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (yyP312, yyP311->Array.Type);
/* line 4775 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
  }
   return;
yyL1:;

/* line 4777 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP311->Array.Expr);
/* line 4778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenArraySize (yyP312, yyP311->Array.Type);
/* line 4778 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  if (Tree_IsType (yyP311, kUserStructTypes)) {
/* line 4780 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4781 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcTypeSize (yyP311));
  }
   return;

  }
/* line 4783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4783 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP311)))) goto yyL4;
  {
/* line 4784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("sizeof(");
/* line 4784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP311);
/* line 4784 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *)");
  }
  }
   return;
yyL4:;

/* line 4786 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("sizeof(");
/* line 4787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP311);
/* line 4787 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void CodeChkExpr
# if defined __STDC__ | defined __cplusplus
(register bool yyP317, register bool yyP316, register tTree yyP315, tSymbol yyP314, register tTree yyP313)
# else
(yyP317, yyP316, yyP315, yyP314, yyP313)
 register bool yyP317;
 register bool yyP316;
 register tTree yyP315;
 tSymbol yyP314;
 register tTree yyP313;
# endif
{
  if (equalbool (yyP317, true)) {
  if (equalbool (yyP316, true)) {
/* line 4795 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (RNGCHK);
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP315);
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP314);
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP313));
/* line 4796 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  }
  if (equalbool (yyP317, true)) {
  if (Tree_IsType (yyP313, kArray)) {
/* line 4798 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (RNGCHK);
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP315);
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP313->Array.Expr);
/* line 4799 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
  }
/* line 4801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4801 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP315);
  }
   return;

;
}

static void CodeChkUBExpr
# if defined __STDC__ | defined __cplusplus
(register bool yyP323, register bool yyP322, register tTree yyP321, register tTree yyP320, tSymbol yyP319, register tTree yyP318)
# else
(yyP323, yyP322, yyP321, yyP320, yyP319, yyP318)
 register bool yyP323;
 register bool yyP322;
 register tTree yyP321;
 register tTree yyP320;
 tSymbol yyP319;
 register tTree yyP318;
# endif
{
  if (equalbool (yyP323, true)) {
  if (equalbool (yyP322, true)) {
  if (Tree_IsType (yyP318, kArray)) {
/* line 4807 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (RNGCHKUB);
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP321);
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP320);
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP319);
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP318));
/* line 4808 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
  }
   return;

  }
  }
  }
  if (equalbool (yyP323, true)) {
  if (Tree_IsType (yyP318, kArray)) {
/* line 4811 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (RNGCHKUB);
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP321);
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP320);
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP318->Array.Expr);
/* line 4812 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
  }
   return;

  }
  }
/* line 4815 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void CodeChkLBExpr
# if defined __STDC__ | defined __cplusplus
(register bool yyP325, register tTree yyP324)
# else
(yyP325, yyP324)
 register bool yyP325;
 register tTree yyP324;
# endif
{
  if (equalbool (yyP325, true)) {
/* line 4820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (RNGCHKLB);
/* line 4820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP324);
/* line 4820 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

  }
/* line 4821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4821 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP324);
  }
   return;

;
}

static tSymbol GetElementSymbol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP326)
# else
(yyP326)
 register tTree yyP326;
# endif
{
  if (Tree_IsType (yyP326, kIdentifier)) {
/* line 4825 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP326->Identifier.Sym;

  }
  if (yyP326->Kind == kSubscript) {
/* line 4826 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return GetElementSymbol (yyP326->Subscript.Next);

  }
  if (yyP326->Kind == kRange) {
/* line 4827 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return GetElementSymbol (yyP326->Range.Next);

  }
 yyAbort ("GetElementSymbol");
}

void CodeSymRef
# if defined __STDC__ | defined __cplusplus
(register tTree yyP329, tSymbol yyP328, register bool yyP327)
# else
(yyP329, yyP328, yyP327)
 register tTree yyP329;
 tSymbol yyP328;
 register bool yyP327;
# endif
{

  switch (yyP329->Kind) {
  case kAFormal:
  if (yyP329->AFormal.By->Kind == kByRef) {
  if (equalbool (yyP327, true)) {
/* line 4832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4832 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4833 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
  if (yyP329->AFormal.By->Kind == kByVal) {
  if (equalbool (yyP327, true)) {
/* line 4835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4835 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4836 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
  break;
  case kAnId:
  if (equalbool (yyP327, true)) {
/* line 4838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4838 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4839 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  case kReplicator:
  if (equalbool (yyP327, true)) {
/* line 4841 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4841 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4841 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4842 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  case kIs:
  if (equalbool (yyP327, true)) {
/* line 4844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP328))))) goto yyL9;
  {
/* line 4844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4844 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL9:;

  }
/* line 4845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP328))))) goto yyL10;
  {
/* line 4845 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL10:;

  if (equalbool (yyP327, true)) {
/* line 4846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4846 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4847 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  case kRetypes:
  case kReshapes:
  if (equalbool (yyP327, true)) {
/* line 4849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP329->Retypes.Type)))) goto yyL13;
  {
/* line 4849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4849 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL13:;

  }
/* line 4850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP329->Retypes.Type)))) goto yyL14;
  {
/* line 4850 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL14:;

  if (equalbool (yyP327, true)) {
/* line 4851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4851 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4852 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  case kVal:
  if (equalbool (yyP327, true)) {
/* line 4854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP328))))) goto yyL17;
  {
/* line 4854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4854 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL17:;

  }
/* line 4855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (SymbolType (yyP328))))) goto yyL18;
  {
/* line 4855 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL18:;

  if (equalbool (yyP327, true)) {
/* line 4856 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4856 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4857 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  case kValRetypes:
  case kValReshapes:
  if (equalbool (yyP327, true)) {
/* line 4859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP329->ValRetypes.Type)))) goto yyL21;
  {
/* line 4859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 4859 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL21:;

  }
/* line 4860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsScalarType (yyP329->ValRetypes.Type)))) goto yyL22;
  {
/* line 4860 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
  }
   return;
yyL22:;

  if (equalbool (yyP327, true)) {
/* line 4861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4861 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }
/* line 4862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 4862 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSym (yyP328);
  }
   return;

  }

  if (Tree_IsType (yyP329, kSpec)) {
/* line 4864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4864 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WNYI (yyP329, yyP329->Spec.Pos);
  }
   return;

  }
 yyAbort ("CodeSymRef");
;
}

static void CodeSym
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP330)
# else
(yyP330)
 tSymbol yyP330;
# endif
{
/* line 4869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4869 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP330) . Depth == 0))) goto yyL1;
  {
/* line 4870 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP330);
  }
  }
   return;
yyL1:;

/* line 4872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP330)))) goto yyL2;
  {
/* line 4872 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP330);
  }
  }
   return;
yyL2:;

/* line 4874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FP");
/* line 4874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP330, CurProc);
/* line 4874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("->");
/* line 4874 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP330);
  }
   return;

;
}

static void CodeIndirect
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP332, register tTree yyP331)
# else
(yyP332, yyP331)
 tSymbol yyP332;
 register tTree yyP331;
# endif
{
  if (Tree_IsType (yyP331, kDefProc)) {
/* line 4878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP332) . Depth <= Symbol (yyP331->DefProc.Sym) . Depth))) goto yyL1;
  {
/* line 4878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("->Chain");
/* line 4878 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP332, Symbol (yyP331 -> DefProc . Parent) . Kind);
  }
  }
   return;
yyL1:;

  }
  if (Tree_IsType (yyP331, kDefFunc)) {
/* line 4879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP332) . Depth <= Symbol (yyP331->DefFunc.Sym) . Depth))) goto yyL2;
  {
/* line 4879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("->Chain");
/* line 4879 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeIndirect (yyP332, Symbol (yyP331 -> DefFunc . Parent) . Kind);
  }
  }
   return;
yyL2:;

  }
/* line 4880 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void CCodeTypeName
# if defined __STDC__ | defined __cplusplus
(register tTree yyP334, register bool yyP333)
# else
(yyP334, yyP333)
 register tTree yyP334;
 register bool yyP333;
# endif
{
  if (equalbool (yyP333, true)) {
/* line 4886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP334)))) goto yyL1;
  {
/* line 4886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP334);
/* line 4886 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
  }
  }
   return;
yyL1:;

  }
/* line 4887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (yyP334)))) goto yyL2;
  {
/* line 4887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP334);
/* line 4887 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
  }
  }
   return;
yyL2:;

  if (equalbool (yyP333, true)) {
/* line 4889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP334);
/* line 4889 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
  }
   return;

  }
/* line 4890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 4890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP334);
/* line 4890 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")");
  }
   return;

;
}

static void CCodeType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP335)
# else
(yyP335)
 register tTree yyP335;
# endif
{

  switch (yyP335->Kind) {
  case kInt:
  case kUserInt:
/* line 4894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4894 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT");
  }
   return;

  case kInt64:
  case kUserInt64:
/* line 4895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4895 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT64");
  }
   return;

  case kInt32:
  case kUserInt32:
/* line 4896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4896 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT32");
  }
   return;

  case kInt16:
  case kUserInt16:
/* line 4897 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4897 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT16");
  }
   return;

  case kReal32:
  case kUserReal32:
/* line 4898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4898 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("REAL32");
  }
   return;

  case kReal64:
  case kUserReal64:
/* line 4899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4899 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("REAL64");
  }
   return;

  case kBool:
  case kUserBool:
/* line 4900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4900 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BOOL");
  }
   return;

  case kByte:
  case kUserByte:
/* line 4901 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4901 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BYTE");
  }
   return;

  case kArray:
  case kUserArray:
/* line 4902 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4902 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP335->Array.Type);
  }
   return;

  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 4903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4903 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("USER");
  }
   return;

  case kChan:
/* line 4904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4904 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("CHAN");
  }
   return;

  }

 yyAbort ("CCodeType");
;
}

static void CCodeTypeId
# if defined __STDC__ | defined __cplusplus
(register tTree yyP336)
# else
(yyP336)
 register tTree yyP336;
# endif
{
  if (Tree_IsType (yyP336, kArray)) {
/* line 4908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 4908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP336->Array.Expr);
/* line 4908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 4908 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP336->Array.Type);
  }
   return;

  }
  if (Tree_IsType (yyP336, kUserStructTypes)) {
/* line 4909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[SIZEUSER(");
/* line 4909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcTypeSize (yyP336));
/* line 4909 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")]");
  }
   return;

  }
/* line 4910 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

static void CCodeTypeIdChkConst
# if defined __STDC__ | defined __cplusplus
(register tTree yyP338, register tPosition yyP337)
# else
(yyP338, yyP337)
 register tTree yyP338;
 register tPosition yyP337;
# endif
{
  if (Tree_IsType (yyP338, kArray)) {
/* line 4914 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4914 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsConstInt (yyP338->Array.Expr)))) goto yyL1;
  {
/* line 4915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[]");
/* line 4915 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP338->Array.Type);
/* line 4916 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Implementation restriction: arrays may not be unknown size", xxError, yyP337);
  }
  }
   return;
yyL1:;

/* line 4918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 4918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP338->Array.Expr);
/* line 4918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 4918 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP338->Array.Type);
  }
   return;

  }
  if (Tree_IsType (yyP338, kUserStructTypes)) {
/* line 4919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[SIZEUSER(");
/* line 4919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (CalcTypeSize (yyP338));
/* line 4919 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")]");
  }
   return;

  }
 yyAbort ("CCodeTypeIdChkConst");
;
}

void OccCodeType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP339)
# else
(yyP339)
 register tTree yyP339;
# endif
{
  if (yyP339 == NULL) {
/* line 4924 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }

  switch (yyP339->Kind) {
  case kAType:
  if (yyP339->AType.Next->Kind == kNoType) {
/* line 4925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4925 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP339->AType.Type);
  }
   return;

  }
/* line 4926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP339->AType.Type);
/* line 4926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 4926 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP339->AType.Next);
  }
   return;

  case kUserArray:
/* line 4927 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4927 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserArray.Sym);
  }
   return;

  case kUserInt:
/* line 4928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4928 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserInt.Sym);
  }
   return;

  case kUserInt16:
/* line 4929 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4929 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserInt16.Sym);
  }
   return;

  case kUserInt32:
/* line 4930 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4930 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserInt32.Sym);
  }
   return;

  case kUserInt64:
/* line 4931 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4931 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserInt64.Sym);
  }
   return;

  case kUserReal32:
/* line 4932 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4932 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserReal32.Sym);
  }
   return;

  case kUserReal64:
/* line 4933 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4933 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserReal64.Sym);
  }
   return;

  case kUserByte:
/* line 4934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4934 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserByte.Sym);
  }
   return;

  case kUserBool:
/* line 4935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4935 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserBool.Sym);
  }
   return;

  case kUserStructTypes:
  case kUserDataTypes:
  case kUserRecord:
  case kUserPackedRecord:
  case kUserUnion:
  case kUserChanRecord:
/* line 4936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4936 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP339->UserStructTypes.Sym);
  }
   return;

  case kInt:
/* line 4937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4937 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT");
  }
   return;

  case kInt16:
/* line 4938 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4938 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT16");
  }
   return;

  case kInt32:
/* line 4939 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4939 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT32");
  }
   return;

  case kInt64:
/* line 4940 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4940 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("INT64");
  }
   return;

  case kReal32:
/* line 4941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4941 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("REAL32");
  }
   return;

  case kReal64:
/* line 4942 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4942 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("REAL64");
  }
   return;

  case kBool:
/* line 4943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4943 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BOOL");
  }
   return;

  case kByte:
/* line 4944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4944 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("BYTE");
  }
   return;

  case kArray:
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstInt (yyP339->Array.Expr)))) goto yyL22;
  {
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (ValConstInt (yyP339->Array.Expr));
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
/* line 4945 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP339->Array.Type);
  }
  }
   return;
yyL22:;

/* line 4946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[]");
/* line 4946 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP339->Array.Type);
  }
   return;

  case kChan:
/* line 4947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("CHAN OF ");
/* line 4947 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeProtocol (yyP339->Chan.Protocol);
  }
   return;

  case kTimer:
/* line 4948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4948 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("TIMER");
  }
   return;

  }

 yyAbort ("OccCodeType");
;
}

static void OccCodeProtocol
# if defined __STDC__ | defined __cplusplus
(register tTree yyP340)
# else
(yyP340)
 register tTree yyP340;
# endif
{
  if (yyP340->Kind == kAny) {
/* line 4952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4952 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ANY");
  }
   return;

  }
  if (yyP340->Kind == kNamed) {
/* line 4953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4953 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP340->Named.Sym);
  }
   return;

  }
  if (yyP340->Kind == kSimple) {
/* line 4954 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4954 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP340->Simple.Type);
  }
   return;

  }
  if (yyP340->Kind == kCounted) {
/* line 4955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP340->Counted.Length);
/* line 4955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (":: ");
/* line 4955 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP340->Counted.Type);
  }
   return;

  }
 yyAbort ("OccCodeProtocol");
;
}

static void CodeOp
# if defined __STDC__ | defined __cplusplus
(register tTree yyP341)
# else
(yyP341)
 register tTree yyP341;
# endif
{

  switch (yyP341->Kind) {
  case kLess:
/* line 4960 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4960 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" < ");
  }
   return;

  case kGreater:
/* line 4961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4961 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" > ");
  }
   return;

  case kLessEqual:
/* line 4962 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4962 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" <= ");
  }
   return;

  case kGreaterEqual:
/* line 4963 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4963 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" >= ");
  }
   return;

  case kEqual:
/* line 4964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4964 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" == ");
  }
   return;

  case kNotEqual:
/* line 4965 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4965 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" != ");
  }
   return;

  case kAdd:
/* line 4966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4966 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
  }
   return;

  case kSubtract:
/* line 4967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4967 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" - ");
  }
   return;

  case kMultiply:
/* line 4968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4968 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" * ");
  }
   return;

  case kDivide:
/* line 4969 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4969 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" / ");
  }
   return;

  case kRem:
/* line 4971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4971 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" % ");
  }
   return;

  case kBitAnd:
/* line 4973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4973 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" & ");
  }
   return;

  case kBitOr:
/* line 4974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4974 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" | ");
  }
   return;

  case kBitEor:
/* line 4975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4975 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ^ ");
  }
   return;

  case kBitNot:
/* line 4976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4976 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ~ ");
  }
   return;

  case kSize:
/* line 4978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4978 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" sizeof ");
  }
   return;

  case kPlus:
/* line 4979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4979 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" + ");
  }
   return;

  case kMinus:
/* line 4980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4980 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" - ");
  }
   return;

  case kTimes:
/* line 4981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4981 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" * ");
  }
   return;

  case kAnd:
/* line 4982 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4982 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" && ");
  }
   return;

  case kOr:
/* line 4983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4983 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" || ");
  }
   return;

  case kNot:
/* line 4984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 4984 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ! ");
  }
   return;

  }

 yyAbort ("CodeOp");
;
}

static int PrimTypeSize
# if defined __STDC__ | defined __cplusplus
(register tTree yyP342)
# else
(yyP342)
 register tTree yyP342;
# endif
{

  switch (yyP342->Kind) {
  case kUserRecord:
/* line 4992 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return CalcTypeSize (yyP342);

  case kInt:
  case kUserInt:
/* line 4993 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return  (IS_Mode(xx16bitINT)) ? 2 : 4;

  case kInt16:
  case kUserInt16:
/* line 4994 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 2;

  case kInt32:
  case kUserInt32:
/* line 4995 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 4;

  case kInt64:
  case kUserInt64:
/* line 4996 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 8;

  case kReal32:
  case kUserReal32:
/* line 4997 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 4;

  case kReal64:
  case kUserReal64:
/* line 4998 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 8;

  case kByte:
  case kUserByte:
/* line 4999 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1;

  case kBool:
  case kUserBool:
/* line 5000 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1;

  }

 yyAbort ("PrimTypeSize");
}

static tTree GetPrimType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP343)
# else
(yyP343)
 register tTree yyP343;
# endif
{
  if (Tree_IsType (yyP343, kArray)) {
/* line 5005 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return GetPrimType (yyP343->Array.Type);

  }
/* line 5006 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP343;

}

static bool IsPrimType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP344)
# else
(yyP344)
 register tTree yyP344;
# endif
{
  if (Tree_IsType (yyP344, kPrimitive)) {
/* line 5010 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  return false;
}

static bool IsTimer
# if defined __STDC__ | defined __cplusplus
(register tTree yyP345)
# else
(yyP345)
 register tTree yyP345;
# endif
{
  if (Tree_IsType (yyP345, kIdentifier)) {
/* line 5014 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return Tree_IsType (SymbolType (yyP345->Identifier.Sym), kTimer);

  }
  if (yyP345->Kind == kSubscript) {
/* line 5015 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsTimer (yyP345->Subscript.Next);

  }
  if (yyP345->Kind == kRange) {
/* line 5016 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsTimer (yyP345->Range.Next);

  }
 yyAbort ("IsTimer");
}

static bool IsAtomicProcess
# if defined __STDC__ | defined __cplusplus
(register tTree yyP346)
# else
(yyP346)
 register tTree yyP346;
# endif
{
/* line 5021 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return (yyP346 -> Process . ATOMIC || IS_Mode (xxPrettyC));

}

bool IsAtomicProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP347)
# else
(yyP347)
 register tTree yyP347;
# endif
{
  if (yyP347->Kind == kPrototypeProc) {
/* line 5025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5025 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP347 -> PrototypeProc . Lang != xxOccam))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (Tree_IsType (yyP347, kDefProc)) {
/* line 5026 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5026 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP347 -> DefProc . ATOMIC || IS_Mode (xxPrettyC)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (Tree_IsType (yyP347, kDefAFunc)) {
/* line 5027 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  return false;
}

bool IsParProc
# if defined __STDC__ | defined __cplusplus
(register tTree yyP348)
# else
(yyP348)
 register tTree yyP348;
# endif
{
  if (Tree_IsType (yyP348, kDefProc)) {
/* line 5032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5032 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP348 -> DefProc . isPar || (yyP348->DefProc.Sym == TreeRoot -> Start . OuterProc && ! IS_Mode (xxLibrary))))) goto yyL1;
  }
   return true;
yyL1:;

  }
  return false;
}

bool IsPrototype
# if defined __STDC__ | defined __cplusplus
(register tTree yyP349)
# else
(yyP349)
 register tTree yyP349;
# endif
{
  if (yyP349->Kind == kPrototypeProc) {
/* line 5037 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  if (yyP349->Kind == kPrototypeFunc) {
/* line 5038 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  return false;
}

static bool IsBoundedType
# if defined __STDC__ | defined __cplusplus
(register tTree yyP350)
# else
(yyP350)
 register tTree yyP350;
# endif
{
  if (Tree_IsType (yyP350, kArray)) {
  if (yyP350->Array.Expr->Kind == kUnBound) {
/* line 5044 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return false;

  }
/* line 5046 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return IsBoundedType (yyP350->Array.Type);

  }
/* line 5048 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

}

static bool IsByteArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP351)
# else
(yyP351)
 register tTree yyP351;
# endif
{
  if (Tree_IsType (yyP351, kArray)) {
  if (Tree_IsType (yyP351->Array.Type, kByte)) {
/* line 5053 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
/* line 5054 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5054 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsByteArray (yyP351->Array.Type)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  return false;
}

static int CountArrayDims
# if defined __STDC__ | defined __cplusplus
(register tTree yyP352)
# else
(yyP352)
 register tTree yyP352;
# endif
{
  if (Tree_IsType (yyP352, kArray)) {
/* line 5059 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 1 + CountArrayDims (yyP352->Array.Type);

  }
/* line 5060 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

}

tTree GenOccPrototypes
# if defined __STDC__ | defined __cplusplus
(register tTree yyP353)
# else
(yyP353)
 register tTree yyP353;
# endif
{
  if (yyP353->Kind == kStart) {
/* line 5069 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  }
  if (yyP353->Kind == kPrototypeProc) {
/* line 5071 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP353 -> PrototypeProc . CGNext;

  }
  if (yyP353->Kind == kPrototypeFunc) {
/* line 5073 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP353 -> PrototypeFunc . CGNext;

  }
  if (Tree_IsType (yyP353, kDefProc)) {
/* line 5075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP353->DefProc.Sym)))) goto yyL4;
  {
/* line 5075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsAtomicProc (yyP353))) goto yyL4;
  {
/* line 5075 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP353)))) goto yyL4;
  {
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("EXTERN_ATOMIC PROC ");
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP353->DefProc.Sym);
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP353->DefProc.Formals);
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") : ");
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (outname);
/* line 5076 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
   return yyP353 -> DefProc . CGNext;
yyL4:;

/* line 5078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP353->DefProc.Sym)))) goto yyL5;
  {
/* line 5078 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP353)))) goto yyL5;
  {
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("EXTERN_OCCAM PROC ");
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP353->DefProc.Sym);
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP353->DefProc.Formals);
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") : ");
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (outname);
/* line 5079 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return yyP353 -> DefProc . CGNext;
yyL5:;

/* line 5080 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP353 -> DefProc . CGNext;

  }
  if (Tree_IsType (yyP353, kDefAFunc)) {
/* line 5082 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5082 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP353->DefAFunc.Sym)))) goto yyL7;
  {
/* line 5082 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP353)))) goto yyL7;
  {
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("EXTERN_ATOMIC ");
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (yyP353->DefAFunc.TypeList);
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" FUNCTION ");
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteOccSym (yyP353->DefAFunc.Sym);
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP353->DefAFunc.Formals);
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (") : ");
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (outname);
/* line 5083 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return yyP353 -> DefAFunc . CGNext;
yyL7:;

/* line 5084 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP353 -> DefAFunc . CGNext;

  }
  if (yyP353->Kind == kHCode) {
/* line 5085 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP353 -> HCode . CGNext;

  }
 yyAbort ("GenOccPrototypes");
}

static void GenOccPrototypeFormals
# if defined __STDC__ | defined __cplusplus
(register tTree yyP354)
# else
(yyP354)
 register tTree yyP354;
# endif
{
  if (yyP354->Kind == kAFormal) {
  if (Tree_IsType (yyP354->AFormal.Next, kNoFormal)) {
/* line 5089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5089 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP354 -> AFormal . U == xxOutput))) goto yyL1;
  {
/* line 5090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5090 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" out");
  }
  }
   return;
yyL1:;

/* line 5092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5092 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP354 -> AFormal . U == xxInput))) goto yyL2;
  {
/* line 5093 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5093 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" in");
  }
  }
   return;
yyL2:;

  if (yyP354->AFormal.By->Kind == kByRef) {
/* line 5095 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5096 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
  }
   return;

  }
  if (yyP354->AFormal.By->Kind == kByVal) {
/* line 5098 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("VAL ");
/* line 5099 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
  }
   return;

  }
  }
/* line 5101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5101 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP354 -> AFormal . U == xxOutput))) goto yyL5;
  {
/* line 5102 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5102 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" out,");
/* line 5103 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP354->AFormal.Next);
  }
  }
   return;
yyL5:;

/* line 5105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5105 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP354 -> AFormal . U == xxInput))) goto yyL6;
  {
/* line 5106 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5106 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" in,");
/* line 5107 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP354->AFormal.Next);
  }
  }
   return;
yyL6:;

  if (yyP354->AFormal.By->Kind == kByRef) {
/* line 5109 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5110 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 5111 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP354->AFormal.Next);
  }
   return;

  }
  if (yyP354->AFormal.By->Kind == kByVal) {
/* line 5113 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5114 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("VAL ");
/* line 5114 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OccCodeType (SymbolType (yyP354->AFormal.Sym));
/* line 5114 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 5115 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenOccPrototypeFormals (yyP354->AFormal.Next);
  }
   return;

  }
  }
  if (Tree_IsType (yyP354, kNoFormal)) {
/* line 5117 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
 yyAbort ("GenOccPrototypeFormals");
;
}

tTree GenCPrototype
# if defined __STDC__ | defined __cplusplus
(register tTree yyP355)
# else
(yyP355)
 register tTree yyP355;
# endif
{
  if (yyP355->Kind == kStart) {
/* line 5121 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  }
  if (yyP355->Kind == kPrototypeProc) {
/* line 5123 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP355 -> PrototypeProc . CGNext;

  }
  if (yyP355->Kind == kPrototypeFunc) {
/* line 5125 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP355 -> PrototypeFunc . CGNext;

  }
  if (Tree_IsType (yyP355, kDefFunc)) {
/* line 5127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP355->DefFunc.Sym)))) goto yyL4;
  {
/* line 5127 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP355)))) goto yyL4;
  {
/* line 5128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{\"");
/* line 5128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenFuncHead (yyP355);
/* line 5128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";\",NULL,0},");
/* line 5128 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return yyP355 -> DefFunc . CGNext;
yyL4:;

  }
  if (Tree_IsType (yyP355, kDefProc)) {
/* line 5131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP355->DefProc.Sym)))) goto yyL5;
  {
/* line 5131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsAtomicProc (yyP355)))) goto yyL5;
  {
/* line 5131 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP355)))) goto yyL5;
  {
/* line 5132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{\"");
/* line 5132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcHead (yyP355);
/* line 5132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";\",NULL,0},");
/* line 5132 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
   return yyP355 -> DefProc . CGNext;
yyL5:;

/* line 5135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP355->DefProc.Sym)))) goto yyL6;
  {
/* line 5135 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsInlined (yyP355)))) goto yyL6;
  {
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{\"");
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenProcHead (yyP355);
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";\",\"");
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP355->DefProc.Sym);
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\",sizeof(tSF_");
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP355->DefProc.Sym);
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")},");
/* line 5136 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
   return yyP355 -> DefProc . CGNext;
yyL6:;

/* line 5140 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP355 -> DefProc . CGNext;

  }
  if (Tree_IsType (yyP355, kDefAFunc)) {
/* line 5139 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP355 -> DefAFunc . CGNext;

  }
  if (yyP355->Kind == kHCode) {
/* line 5142 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP355 -> HCode . CGNext;

  }
 yyAbort ("GenCPrototype");
}

tTree GenCGDebug
# if defined __STDC__ | defined __cplusplus
(register tTree yyP356)
# else
(yyP356)
 register tTree yyP356;
# endif
{
  if (yyP356->Kind == kStart) {
/* line 5147 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoTree;

  }
  if (Tree_IsType (yyP356, kDefFunc)) {
/* line 5149 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("FUNC ");
/* line 5150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP356->DefFunc.Sym);
/* line 5150 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("() ");
/* line 5151 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (NeedsChain (yyP356), "CHAIN ");
/* line 5152 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsTopLevel (yyP356->DefFunc.Sym), "TOP ");
/* line 5153 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsMVFunction (yyP356), "MV ");
/* line 5154 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsSTVFunction (yyP356), "STV ");
/* line 5155 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (UsesFreeVars (yyP356), "UFV ");
/* line 5156 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsInlined (yyP356), "INLINE ");
/* line 5157 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\n");
  }
   return yyP356 -> DefFunc . CGNext;

  }
  if (Tree_IsType (yyP356, kDefProc)) {
/* line 5160 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("PROC ");
/* line 5161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP356->DefProc.Sym);
/* line 5161 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("() ");
/* line 5162 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsAtomicProc (yyP356), "ATOMIC ");
/* line 5163 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (NeedsChain (yyP356), "CHAIN ");
/* line 5164 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsTopLevel (yyP356->DefProc.Sym), "TOP ");
/* line 5165 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (UsesFreeVars (yyP356), "UFV ");
/* line 5166 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CONDWrite (IsInlined (yyP356), "INLINE ");
/* line 5167 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\n");
  }
   return yyP356 -> DefProc . CGNext;

  }
  if (Tree_IsType (yyP356, kDefAFunc)) {
/* line 5170 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP356 -> DefAFunc . CGNext;

  }
  if (Tree_IsType (yyP356, kDefAProc)) {
/* line 5171 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP356 -> DefAProc . CGNext;

  }
  if (yyP356->Kind == kHCode) {
/* line 5173 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP356 -> HCode . CGNext;

  }
 yyAbort ("GenCGDebug");
}

bool IsASEntry
# if defined __STDC__ | defined __cplusplus
(register tTree yyP357)
# else
(yyP357)
 register tTree yyP357;
# endif
{
  if (Tree_IsType (yyP357, kDefProc)) {
  if (yyP357->DefProc.Formals->Kind == kAFormal) {
  if (yyP357->DefProc.Formals->AFormal.Next->Kind == kAFormal) {
  if (Tree_IsType (yyP357->DefProc.Formals->AFormal.Next->AFormal.Next, kNoFormal)) {
/* line 5180 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsASChan (SymbolType (yyP357->DefProc.Formals->AFormal.Sym)))) goto yyL1;
  {
/* line 5181 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsASChan (SymbolType (yyP357->DefProc.Formals->AFormal.Next->AFormal.Sym)))) goto yyL1;
  {
/* line 5182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP357->DefProc.Formals -> AFormal . U != xxOutput))) goto yyL1;
  {
/* line 5182 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP357->DefProc.Formals->AFormal.Next -> AFormal . U != xxInput))) goto yyL1;
  }
  }
  }
  }
   return true;
yyL1:;

  }
  }
  }
  }
  return false;
}

void CodeMainEntryParameters
# if defined __STDC__ | defined __cplusplus
(register tTree yyP358)
# else
(yyP358)
 register tTree yyP358;
# endif
{
  if (Tree_IsType (yyP358, kDefProc)) {
  if (Tree_IsType (yyP358->DefProc.Formals, kNoFormal)) {
/* line 5187 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP358->DefProc.Formals->Kind == kAFormal) {
  if (yyP358->DefProc.Formals->AFormal.Next->Kind == kAFormal) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next, kNoFormal)) {
/* line 5189 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsASChan (SymbolType (yyP358->DefProc.Formals->AFormal.Sym)))) goto yyL2;
  {
/* line 5190 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsASChan (SymbolType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym)))) goto yyL2;
  {
/* line 5191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals -> AFormal . U != xxOutput))) goto yyL2;
  {
/* line 5191 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals->AFormal.Next -> AFormal . U != xxInput))) goto yyL2;
  {
/* line 5192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Sym);
/* line 5192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&fs;");
/* line 5192 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym);
/* line 5193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&ts;");
/* line 5193 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
  }
   return;
yyL2:;

/* line 5196 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsSPChan (SymbolType (yyP358->DefProc.Formals->AFormal.Sym)))) goto yyL3;
  {
/* line 5197 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsSPChan (SymbolType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym)))) goto yyL3;
  {
/* line 5198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals -> AFormal . U != xxOutput))) goto yyL3;
  {
/* line 5198 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals->AFormal.Next -> AFormal . U != xxInput))) goto yyL3;
  {
/* line 5199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Sym);
/* line 5199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&fs;");
/* line 5199 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5200 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5200 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym);
/* line 5200 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&ts;");
/* line 5200 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
  }
   return;
yyL3:;

  }
  if (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->Kind == kAFormal) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Next, kNoFormal)) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Type, kArray)) {
  if (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Type->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Type->Array.Type, kInt)) {
/* line 5203 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsSPChan (SymbolType (yyP358->DefProc.Formals->AFormal.Sym)))) goto yyL4;
  {
/* line 5204 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsSPChan (SymbolType (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym)))) goto yyL4;
  {
/* line 5205 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals -> AFormal . U != xxOutput))) goto yyL4;
  {
/* line 5205 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP358->DefProc.Formals->AFormal.Next -> AFormal . U != xxInput))) goto yyL4;
  {
/* line 5206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Sym);
/* line 5206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&fs;");
/* line 5206 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Next->AFormal.Sym);
/* line 5207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=&ts;");
/* line 5207 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Sym);
/* line 5208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=mem;");
/* line 5208 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Next->AFormal.Next->AFormal.Sym);
/* line 5209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Dim0=mem_size;");
/* line 5209 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
  }
  }
  }
   return;
yyL4:;

  }
  }
  }
  }
  }
  }
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Next, kNoFormal)) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Type, kArray)) {
  if (yyP358->DefProc.Formals->AFormal.Type->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP358->DefProc.Formals->AFormal.Type->Array.Type, kInt)) {
/* line 5212 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Sym);
/* line 5213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=mem;");
/* line 5213 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("MAINFP->");
/* line 5214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP358->DefProc.Formals->AFormal.Sym);
/* line 5214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("_Dim0=mem_size;");
/* line 5214 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
  }
  }
  }
/* line 5217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5217 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Main entry point does not have acceptable parameter list", xxError, yyP358->DefProc.Pos);
  }
   return;

  }
/* line 5219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5219 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Main Entry Point must be a PROC", xxError, yyP358->Spec.Pos);
  }
   return;

;
}

static bool IsSPChan
# if defined __STDC__ | defined __cplusplus
(register tTree yyP359)
# else
(yyP359)
 register tTree yyP359;
# endif
{
  if (yyP359->Kind == kChan) {
  if (yyP359->Chan.Protocol->Kind == kNamed) {
/* line 5224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5224 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP359->Chan.Protocol->Named.Sym) . Id == MakeIdent ("SP", 2)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  }
  return false;
}

static bool IsASChan
# if defined __STDC__ | defined __cplusplus
(register tTree yyP360)
# else
(yyP360)
 register tTree yyP360;
# endif
{
  if (yyP360->Kind == kChan) {
  if (yyP360->Chan.Protocol->Kind == kNamed) {
/* line 5228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5228 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP360->Chan.Protocol->Named.Sym) . Id == MakeIdent ("AS", 2)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  }
  return false;
}

void GenStatics
# if defined __STDC__ | defined __cplusplus
(register tTree yyP361)
# else
(yyP361)
 register tTree yyP361;
# endif
{
  if (yyP361->Kind == kStart) {
/* line 5233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5233 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStatics (yyP361->Start.Process);
  }
   return;

  }
  if (yyP361->Kind == kNullProcess) {
/* line 5235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5235 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStatics (yyP361->NullProcess.SpecList);
  }
   return;

  }
  if (yyP361->Kind == kNoSpec) {
/* line 5237 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP361->Kind == kASpec) {
/* line 5239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStatic (yyP361->ASpec.Spec);
/* line 5239 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStatics (yyP361->ASpec.Next);
  }
   return;

  }
/* line 5241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("Error in GenStatics - ");
/* line 5241 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteTreeNode (yyf, yyP361);
  }
   return;

;
}

static void GenStatic
# if defined __STDC__ | defined __cplusplus
(register tTree yyP362)
# else
(yyP362)
 register tTree yyP362;
# endif
{
  if (yyP362->Kind == kVal) {
/* line 5247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5247 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP362->Val.Expr))))) goto yyL1;
  }
   return;
yyL1:;

/* line 5249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5249 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsZeroSizeType (ExprType (yyP362->Val.Expr))))) goto yyL2;
  {
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP362->Val.Expr));
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->Val.Sym);
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = NULL;");
/* line 5250 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL2:;

  if (Tree_IsType (yyP362->Val.Expr, kStringElement)) {
/* line 5252 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static BYTE ");
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->Val.Sym);
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[] = ");
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (ExprType (yyP362->Val.Expr), yyP362->Val.Expr -> StringElement . CA);
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5253 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP362->Val.Expr->Kind == kElementalExpr) {
/* line 5255 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (ExprType (yyP362->Val.Expr));
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->Val.Sym);
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (ExprType (yyP362->Val.Expr));
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (ExprType (yyP362->Val.Expr->ElementalExpr.Elemental), yyP362->Val.Expr->ElementalExpr.Elemental -> Elemental . CA);
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5256 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
  if (Tree_IsType (yyP362, kValRetypes)) {
/* line 5259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5259 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (yyP362->ValRetypes.Type)))) goto yyL5;
  }
   return;
yyL5:;

/* line 5261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5261 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsZeroSizeType (ExprType (yyP362->ValRetypes.Expr))))) goto yyL6;
  {
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP362->ValRetypes.Type);
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *");
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->ValRetypes.Sym);
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = NULL;");
/* line 5262 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL6:;

  if (Tree_IsType (yyP362->ValRetypes.Expr, kStringElement)) {
/* line 5264 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP362->ValRetypes.Type);
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->ValRetypes.Sym);
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP362->ValRetypes.Type);
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (yyP362->ValRetypes.Type, yyP362->ValRetypes.Expr -> StringElement . CA);
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5265 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (yyP362->ValRetypes.Expr->Kind == kElementalExpr) {
/* line 5267 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP362->ValRetypes.Type);
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->ValRetypes.Sym);
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP362->ValRetypes.Type);
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstArray (yyP362->ValRetypes.Type, yyP362->ValRetypes.Expr->ElementalExpr.Elemental -> Elemental . CA);
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5268 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 5271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5271 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! (IsPrimType (ExprType (yyP362->ValRetypes.Expr)))) goto yyL9;
  {
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("static ");
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP362->ValRetypes.Type);
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" ");
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP362->ValRetypes.Sym);
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeId (yyP362->ValRetypes.Type);
/* line 5272 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" = ");
/* line 5273 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
{
      unsigned char tmp[8];
      tConstArray CA = (tConstArray)tmp;

      WriteConstArray(ExprType(yyP362->ValRetypes.Expr),ValConstExpr(yyP362->ValRetypes.Expr),CA);
      RetypeConst(PrimTypeSize(ExprType(yyP362->ValRetypes.Expr)),CA,PrimTypeSize(ArrayPrimType(yyP362->ValRetypes.Type)),CA);
      CodeConstArray(yyP362->ValRetypes.Type,CA);
    }
/* line 5281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5281 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
  }
   return;
yyL9:;

  }

  switch (yyP362->Kind) {
  case kAVal:
  case kVal:
  case kValRetypes:
  case kValReshapes:
/* line 5284 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5285 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("VAL constant cannot be calculated at compile time", xxError, yyP362->AVal.Pos);
  }
   return;

  case kDefAProc:
  case kDefProc:
  case kInlineDefProc:
  case kPrototypeProc:
/* line 5308 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAFunc:
  case kDefFunc:
  case kInlineDefFunc:
  case kDefIsFunc:
  case kInlineDefIsFunc:
  case kPrototypeFunc:
/* line 5309 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kHCode:
/* line 5310 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kNullSpec:
/* line 5311 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDefAPro:
  case kDefSeqPro:
  case kDefTagPro:
/* line 5312 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kDeclType:
/* line 5313 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }

/* line 5315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("Error in GenStatic - ");
/* line 5315 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteTreeNode (yyf, yyP362);
  }
   return;

;
}

static void CodeConstArray
# if defined __STDC__ | defined __cplusplus
(register tTree yyP364, tConstArray yyP363)
# else
(yyP364, yyP363)
 register tTree yyP364;
 tConstArray yyP363;
# endif
{
/* line 5320 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
 static bool dowarn=true; 
  if (Tree_IsType (yyP364, kUserStructTypes)) {
/* line 5321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5321 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((dowarn))) goto yyL1;
  {
/* line 5322 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("Code generator has assumed target endianness is compiler endianness", xxWarning, yyP364->UserStructTypes.Pos);
/* line 5323 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   dowarn = false;
/* line 5324 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
  }
   return;
yyL1:;

/* line 5332 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5333 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 5334 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */

      { int i;
	int sz = CalcTypeSize(yyP364);
	line_count = 0;

	for(i = 0; i < sz; i+=USER_ALIGNMENT)
	{
	  if (i) WriteChar(',');
	    
	  WritetInt(*((tInt *)(yyP363+i)),true);
	  line_count++;
	}
      }      
    
/* line 5348 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
  }
   return;

  }
  if (Tree_IsType (yyP364, kArray)) {
  if (Tree_IsType (yyP364->Array.Type, kPrimitive)) {
/* line 5351 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5352 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 5353 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
{ int i;
       int sz = PrimTypeSize(yyP364->Array.Type);
       for(i = 0; i < ValConstInt(yyP364->Array.Expr); ++i)
       {
	  if (i) WriteChar(',');
	  if (line_count == MAX_CONSTS_LINE) 
	  {
	    line_count=0;
	    NewLine();
	    WriteStr("   ");
	  }
	  CodeConstExpr(yyP364->Array.Type,ReadConstArray(yyP364->Array.Type,yyP363+(sz*i)));
	  line_count++;
	}
    }
/* line 5368 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
  }
   return;

  }
/* line 5371 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5372 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 5373 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
{ int i,sz;
	sz = CalcTypeSize(yyP364->Array.Type);
	for(i = 0; i < ValConstInt(yyP364->Array.Expr); ++i)
	{
	  if (i) WriteChar(',');
	  CodeConstArray(yyP364->Array.Type,yyP363+i*sz);
	  line_count++;
	}
    }
/* line 5382 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
  }
   return;

  }
 yyAbort ("CodeConstArray");
;
}

static void GenStaticExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP365)
# else
(yyP365)
 register tTree yyP365;
# endif
{
  if (yyP365->Kind == kElementalExpr) {
  if (Tree_IsType (yyP365->ElementalExpr.Elemental, kTable)) {
/* line 5390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{ ");
/* line 5390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticExprList (yyP365->ElementalExpr.Elemental->Table.ExprList);
/* line 5390 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" }");
  }
   return;

  }
  }
  if (yyP365->Kind == kString) {
/* line 5392 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5392 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\"");
/* line 5392 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStrRef (yyP365->String.Value);
/* line 5392 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\"");
  }
   return;

  }
/* line 5394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP365)))) goto yyL3;
  {
/* line 5394 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP365);
  }
  }
   return;
yyL3:;

/* line 5396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5396 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Message ("VAL constant cannot be calculated at compile time", xxError, yyP365->Expr.Pos);
  }
   return;

;
}

static void GenStaticElement
# if defined __STDC__ | defined __cplusplus
(register tTree yyP367, register int yyP366)
# else
(yyP367, yyP366)
 register tTree yyP367;
 register int yyP366;
# endif
{
  if (Tree_IsType (yyP367, kIdentifier)) {
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("&");
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStaticSym (yyP367->Identifier.Sym);
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP366);
/* line 5400 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP367->Kind == kSubscript) {
/* line 5401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticElement (yyP367->Subscript.Next, ValConstInt (yyP367->Subscript.Expr));
/* line 5401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("[");
/* line 5401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP366);
/* line 5401 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("]");
  }
   return;

  }
  if (yyP367->Kind == kRange) {
/* line 5402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5402 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticElement (yyP367->Range.Next, ValConstInt (yyP367->Range.From) + yyP366);
  }
   return;

  }
 yyAbort ("GenStaticElement");
;
}

static void GenStaticExprList
# if defined __STDC__ | defined __cplusplus
(register tTree yyP368)
# else
(yyP368)
 register tTree yyP368;
# endif
{
  if (yyP368->Kind == kAnExpr) {
  if (yyP368->AnExpr.Next->Kind == kNoExpr) {
/* line 5407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5407 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticExpr (yyP368->AnExpr.Expr);
  }
   return;

  }
/* line 5408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticExpr (yyP368->AnExpr.Expr);
/* line 5408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (", ");
/* line 5408 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenStaticExprList (yyP368->AnExpr.Next);
  }
   return;

  }
 yyAbort ("GenStaticExprList");
;
}

void GenShowValue
# if defined __STDC__ | defined __cplusplus
(register tTree yyP371, register tTree yyP370, tSymbol yyP369)
# else
(yyP371, yyP370, yyP369)
 register tTree yyP371;
 register tTree yyP370;
 tSymbol yyP369;
# endif
{
  if (yyP371->Kind == kATagPro) {
/* line 5413 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP371->Kind == kDefTagPro) {
/* line 5414 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
/* line 5415 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5416 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (yyP371, kAFormal) && (Tree_IsType (SymbolKind (CalcParent (SymbolKind (yyP369))), kPrototypeProc) || Tree_IsType (SymbolKind (CalcParent (SymbolKind (yyP369))), kPrototypeFunc))))) goto yyL3;
  }
   return;
yyL3:;

  if (Tree_IsType (yyP371, kDefAProc)) {
/* line 5419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("printf(\"");
/* line 5419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP369);
/* line 5419 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\");");
  }
   return;

  }
  if (Tree_IsType (yyP371, kDefAFunc)) {
/* line 5420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("printf(\"");
/* line 5420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP369);
/* line 5420 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("\");");
  }
   return;

  }
  if (yyP370 == NULL) {
/* line 5422 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (Tree_IsType (yyP370, kArray)) {
  if (Tree_IsType (yyP370->Array.Type, kArray)) {
/* line 5424 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5425 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{ int i;");
/* line 5426 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 5427 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSizes (yyP370, yyP369, 0);
/* line 5428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("for(i=0;i<s0;i++)");
/* line 5428 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("{");
/* line 5429 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Indent ();
/* line 5430 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowTests (yyP370, yyP369, 0);
/* line 5431 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("else putchar(',');");
/* line 5432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 5432 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
/* line 5433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   Outdent ();
/* line 5433 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("}");
  }
   return;

  }
  if (yyP370->Array.Expr->Kind == kUnBound) {
/* line 5436 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ShowArray");
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP370->Array.Type);
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((");
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeTypeName (yyP370->Array.Type, true);
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP369) . Kind, yyP369);
/* line 5437 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("),");
/* line 5438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP369) . Kind, yyP369);
/* line 5438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 5438 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP370));
/* line 5439 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
  }
   return;

  }
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsConstExpr (yyP370->Array.Expr)))) goto yyL9;
  {
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ShowArray");
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP370->Array.Type);
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP369) . Kind, yyP369);
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (",");
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP370->Array.Expr);
/* line 5441 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
  }
  }
   return;
yyL9:;

/* line 5442 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  if (yyP370->Kind == kTimer) {
/* line 5443 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("Show");
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP370);
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(");
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP369) . Kind, yyP369);
/* line 5444 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
  }
   return;

;
}

static void GenShowSizes
# if defined __STDC__ | defined __cplusplus
(register tTree yyP374, tSymbol yyP373, register int yyP372)
# else
(yyP374, yyP373, yyP372)
 register tTree yyP374;
 tSymbol yyP373;
 register int yyP372;
# endif
{
  if (Tree_IsType (yyP374, kArray)) {
  if (yyP374->Array.Expr->Kind == kUnBound) {
  if (Tree_IsType (yyP374->Array.Type, kArray)) {
  if (Tree_IsType (yyP374->Array.Type->Array.Type, kArray)) {
/* line 5450 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5451 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSizes (yyP374->Array.Type, yyP373, yyP372 + 1);
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("int s");
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372);
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= s");
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372 + 1);
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP373) . Kind, yyP373);
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP374));
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5452 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSizes (yyP374->Array.Type, yyP373, yyP372 + 1);
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("int s");
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372);
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP373) . Kind, yyP373);
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteStr (DIM);
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (TypeDepth (yyP374));
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5458 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  if (Tree_IsType (yyP374->Array.Type, kArray)) {
  if (Tree_IsType (yyP374->Array.Type->Array.Type, kArray)) {
/* line 5454 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5455 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSizes (yyP374->Array.Type, yyP373, yyP372 + 1);
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("int s");
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372);
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("= s");
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372 + 1);
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP374->Array.Expr);
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5456 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
  }
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSizes (yyP374->Array.Type, yyP373, yyP372 + 1);
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("int s");
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP372);
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("=");
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeExpr (yyP374->Array.Expr);
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (";");
/* line 5460 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenShowSizes");
;
}

static void GenShowTests
# if defined __STDC__ | defined __cplusplus
(register tTree yyP377, tSymbol yyP376, register int yyP375)
# else
(yyP377, yyP376, yyP375)
 register tTree yyP377;
 tSymbol yyP376;
 register int yyP375;
# endif
{
  if (Tree_IsType (yyP377, kArray)) {
  if (Tree_IsType (yyP377->Array.Type, kArray)) {
/* line 5465 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if ((i%s");
/* line 5466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5466 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")==0) LP; ");
/* line 5467 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowTests (yyP377->Array.Type, yyP376, yyP375 + 1);
/* line 5468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("if (((i+1)%s");
/* line 5468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5468 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")==0) RP; ");
  }
   return;

  }
  if (yyP377->Array.Type->Kind == kChan) {
/* line 5471 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5472 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ShowArray");
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP377->Array.Type);
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(((");
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP377->Array.Type);
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("**)");
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP376) . Kind, yyP376);
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")+(i*s");
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("), s");
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5473 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 5474 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
/* line 5477 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5478 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("ShowArray");
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP377->Array.Type);
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("(((");
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CCodeType (yyP377->Array.Type);
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*)");
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSym (Symbol (yyP376) . Kind, yyP376);
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (")+(i*s");
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("), s");
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteInt (yyP375);
/* line 5479 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (");");
/* line 5480 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

  }
 yyAbort ("GenShowTests");
;
}

static void GenShowSymRef
# if defined __STDC__ | defined __cplusplus
(register tTree yyP379, tSymbol yyP378)
# else
(yyP379, yyP378)
 register tTree yyP379;
 tSymbol yyP378;
# endif
{
/* line 5487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5487 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (SymbolType (yyP378))))) goto yyL1;
  }
   return;
yyL1:;

/* line 5489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5489 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsChanType (SymbolType (yyP378))))) goto yyL2;
  }
   return;
yyL2:;


  switch (yyP379->Kind) {
  case kAFormal:
  if (yyP379->AFormal.By->Kind == kByRef) {
/* line 5491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5491 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("*");
  }
   return;

  }
  if (yyP379->AFormal.By->Kind == kByVal) {
/* line 5493 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }
  break;
  case kAnId:
/* line 5495 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kReplicator:
/* line 5497 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kIs:
/* line 5499 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kRetypes:
  case kReshapes:
/* line 5501 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kVal:
/* line 5503 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  case kValRetypes:
  case kValReshapes:
/* line 5505 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

  }

/* line 5507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("** Error in GenShowSymRef -");
/* line 5507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteTreeNode (yyf, yyP379);
/* line 5507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" **");
/* line 5507 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   NewLine ();
  }
   return;

;
}

static void GenShowSym
# if defined __STDC__ | defined __cplusplus
(register tTree yyP381, tSymbol yyP380)
# else
(yyP381, yyP380)
 register tTree yyP381;
 tSymbol yyP380;
# endif
{
  if (yyP381->Kind == kVal) {
/* line 5513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5513 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP381->Val.Expr)) && IsConstExpr (yyP381->Val.Expr)))) goto yyL1;
  {
/* line 5514 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (ExprType (yyP381->Val.Expr), ValConstExpr (yyP381->Val.Expr));
  }
  }
   return;
yyL1:;

  }
  if (Tree_IsType (yyP381, kValRetypes)) {
/* line 5516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5516 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (yyP381->ValRetypes.Type) && IsConstExpr (yyP381->ValRetypes.Expr)))) goto yyL2;
  {
/* line 5517 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (yyP381->ValRetypes.Type, EvalConstId (yyP381));
  }
  }
   return;
yyL2:;

  }
/* line 5519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5519 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Symbol (yyP380) . Depth == 0))) goto yyL3;
  {
/* line 5520 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP380);
  }
  }
   return;
yyL3:;

/* line 5522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5522 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsLocal (yyP380)))) goto yyL4;
  }
   return;
yyL4:;

/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenShowSymRef (yyP381, yyP380);
/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite ("((tSF_");
/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (CalcParent (SymbolKind (yyP380)));
/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   yyWrite (" *)FP)->");
/* line 5524 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteSym (yyP380);
  }
   return;

;
}

void CShowSym
# if defined __STDC__ | defined __cplusplus
(register tTree yyP383, tSymbol yyP382)
# else
(yyP383, yyP382)
 register tTree yyP383;
 tSymbol yyP382;
# endif
{
  if (yyP383->Kind == kVal) {
/* line 5530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5530 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (ExprType (yyP383->Val.Expr)) && IsConstExpr (yyP383->Val.Expr)))) goto yyL1;
  {
/* line 5531 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (ExprType (yyP383->Val.Expr), ValConstExpr (yyP383->Val.Expr));
  }
  }
   return;
yyL1:;

  }
  if (Tree_IsType (yyP383, kValRetypes)) {
/* line 5533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5533 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsPrimType (yyP383->ValRetypes.Type) && IsConstExpr (yyP383->ValRetypes.Expr)))) goto yyL2;
  {
/* line 5534 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeConstExpr (yyP383->ValRetypes.Type, EvalConstId (yyP383));
  }
  }
   return;
yyL2:;

  }
/* line 5536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5536 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CodeSymRef (yyP383, yyP382, false);
  }
   return;

;
}

int MapKinds
# if defined __STDC__ | defined __cplusplus
(register tTree yyP386, register tTree yyP385, tSymbol yyP384)
# else
(yyP386, yyP385, yyP384)
 register tTree yyP386;
 register tTree yyP385;
 tSymbol yyP384;
# endif
{
  if (yyP386 == NULL) {
/* line 5541 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  }
  if (Tree_IsType (yyP386, kDefProc)) {
/* line 5543 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 3;

  }
  if (yyP386->Kind == kPrototypeProc) {
/* line 5544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5544 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP386 -> DefAProc . U == xxUsed))) goto yyL3;
  }
   return 10;
yyL3:;

/* line 5545 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  }
  if (yyP385 == NULL) {
/* line 5547 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  }

  switch (yyP386->Kind) {
  case kDefFunc:
  case kInlineDefFunc:
/* line 5549 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 2;

  case kPrototypeFunc:
/* line 5550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5550 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP386 -> DefAFunc . U == xxUsed))) goto yyL7;
  }
   return 10;
yyL7:;

/* line 5551 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

  case kAFormal:
/* line 5553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5553 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL9;
  }
   return 1;
yyL9:;

  break;
  case kAnId:
/* line 5554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5554 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL10;
  }
   return 4;
yyL10:;

  break;
  case kVal:
/* line 5555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5555 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL11;
  }
   return 5;
yyL11:;

  break;
  case kValRetypes:
  case kValReshapes:
/* line 5556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5556 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL12;
  }
   return 6;
yyL12:;

  break;
  case kIs:
/* line 5557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5557 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL13;
  }
   return 7;
yyL13:;

  break;
  case kRetypes:
  case kReshapes:
/* line 5558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5558 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL14;
  }
   return 8;
yyL14:;

  break;
  case kReplicator:
/* line 5559 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5559 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsNonLocal (yyP384)))) goto yyL15;
  }
   return 9;
yyL15:;

  break;
  }

/* line 5560 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return 0;

}

tSymbol CalcParent
# if defined __STDC__ | defined __cplusplus
(register tTree T)
# else
(T)
 register tTree T;
# endif
{
  if (T == NULL) {
/* line 5565 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoSymbol;

  }

  switch (T->Kind) {
  case kATagPro:
/* line 5566 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoSymbol;

  case kDefTagPro:
/* line 5567 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoSymbol;

  case kDefSeqPro:
/* line 5568 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoSymbol;

  case kTableElement:
  case kTable:
  case kUserLiteral:
  case kTableExpr:
  case kTableSubscript:
  case kTableRange:
/* line 5569 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoSymbol;

  case kAnId:
/* line 5571 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> AnId . Parent;

  case kAVal:
  case kVal:
  case kValRetypes:
  case kValReshapes:
/* line 5572 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> AVal . Parent;

  case kIs:
/* line 5573 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> Is . Parent;

  case kReplicator:
/* line 5574 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> Replicator . Parent;

  case kRetypes:
  case kReshapes:
/* line 5575 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> Retypes . Parent;

  case kAFormal:
/* line 5576 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> AFormal . Parent;

  case kValof:
/* line 5577 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> Valof . Parent;

  case kDefProc:
  case kInlineDefProc:
/* line 5579 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefProc . Parent;

  case kDefFunc:
  case kInlineDefFunc:
/* line 5580 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> DefFunc . Parent;

  case kPrototypeProc:
/* line 5582 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> PrototypeProc . Parent;

  case kPrototypeFunc:
/* line 5583 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return T -> PrototypeFunc . Parent;

  }

/* line 5585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   fprintf (stderr, "Fell through CalcParent with\n");
/* line 5585 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   WriteTreeNode (stderr, T);
  }
   return 0;

}

tIdent CalcModule
# if defined __STDC__ | defined __cplusplus
(register tTree yyP387)
# else
(yyP387)
 register tTree yyP387;
# endif
{
  if (yyP387->Kind == kPrototypeFunc) {
/* line 5589 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP387 -> PrototypeFunc . Module;

  }
  if (yyP387->Kind == kPrototypeProc) {
/* line 5590 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP387 -> PrototypeProc . Module;

  }
/* line 5591 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NoIdent;

}

tStringRef CalcHeader
# if defined __STDC__ | defined __cplusplus
(register tTree yyP388)
# else
(yyP388)
 register tTree yyP388;
# endif
{
  if (yyP388->Kind == kPrototypeFunc) {
/* line 5595 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP388->PrototypeFunc.Pos . File;

  }
  if (yyP388->Kind == kPrototypeProc) {
/* line 5596 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP388->PrototypeProc.Pos . File;

  }
/* line 5597 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return NULL;

}

tUsage CalcUsage
# if defined __STDC__ | defined __cplusplus
(register tTree yyP389)
# else
(yyP389)
 register tTree yyP389;
# endif
{
  if (yyP389->Kind == kPrototypeFunc) {
/* line 5601 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP389 -> PrototypeFunc . U;

  }
  if (yyP389->Kind == kPrototypeProc) {
/* line 5602 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return yyP389 -> PrototypeProc . U;

  }
 yyAbort ("CalcUsage");
}

static bool IsInlined
# if defined __STDC__ | defined __cplusplus
(register tTree yyP390)
# else
(yyP390)
 register tTree yyP390;
# endif
{
  if (yyP390->Kind == kInlineDefProc) {
/* line 5607 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  if (yyP390->Kind == kInlineDefFunc) {
/* line 5608 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  if (yyP390->Kind == kInlineDefIsFunc) {
/* line 5609 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  return false;
}

static bool IsFieldExpr
# if defined __STDC__ | defined __cplusplus
(register tTree yyP391)
# else
(yyP391)
 register tTree yyP391;
# endif
{
  if (yyP391->Kind == kElementalExpr) {
  if (Tree_IsType (yyP391->ElementalExpr.Elemental, kIdentifier)) {
/* line 5613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5613 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP391->ElementalExpr.Elemental->Identifier.Sym), kAField)))) goto yyL1;
  }
   return true;
yyL1:;

  }
  }
  return false;
}

static int GetFieldOffset
# if defined __STDC__ | defined __cplusplus
(register tTree yyP393, register tTree yyP392)
# else
(yyP393, yyP392)
 register tTree yyP393;
 register tTree yyP392;
# endif
{
  if (yyP392->Kind == kElementalExpr) {
  if (Tree_IsType (yyP392->ElementalExpr.Elemental, kIdentifier)) {
/* line 5617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5617 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((Tree_IsType (SymbolKind (yyP392->ElementalExpr.Elemental->Identifier.Sym), kAField)))) goto yyL1;
  }
   return CalcOffset (yyP393, SymbolKind (yyP392->ElementalExpr.Elemental->Identifier.Sym));
yyL1:;

  }
  }
 yyAbort ("GetFieldOffset");
}

bool IsLocal
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP394)
# else
(yyP394)
 tSymbol yyP394;
# endif
{
/* line 5621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5621 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsNonLocal (yyP394)))) goto yyL1;
  }
   return true;
yyL1:;

  return false;
}

bool IsNonLocal
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP395)
# else
(yyP395)
 tSymbol yyP395;
# endif
{
/* line 5626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5626 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((SymbolUsage (yyP395) & (xxNLRead | xxNLWrite)))) goto yyL1;
  }
   return true;
yyL1:;

/* line 5627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5627 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! (SymbolUsage (yyP395) & xxAtomic)))) goto yyL2;
  }
   return true;
yyL2:;

  return false;
}

bool IsAtomic
# if defined __STDC__ | defined __cplusplus
(register tTree yyP396)
# else
(yyP396)
 register tTree yyP396;
# endif
{
/* line 5632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5632 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IS_Mode (xxPrettyC)))) goto yyL1;
  }
   return true;
yyL1:;

  if (Tree_IsType (yyP396, kProcess)) {
/* line 5633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5633 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP396 -> Process . ATOMIC))) goto yyL2;
  }
   return true;
yyL2:;

  }
  if (Tree_IsType (yyP396, kAChoices)) {
/* line 5634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5634 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP396 -> AChoices . ATOMIC))) goto yyL3;
  }
   return true;
yyL3:;

  }
  if (yyP396->Kind == kReplicator) {
/* line 5635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5635 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP396 -> Replicator . AtomicSpec))) goto yyL4;
  }
   return true;
yyL4:;

  }
  return false;
}

static bool IsMVFunction
# if defined __STDC__ | defined __cplusplus
(register tTree yyP397)
# else
(yyP397)
 register tTree yyP397;
# endif
{
  if (yyP397->DefFunc.TypeList->Kind == kAType) {
  if (yyP397->DefFunc.TypeList->AType.Next->Kind == kAType) {
/* line 5640 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return true;

  }
  }
  return false;
}

bool IsSTVFunction
# if defined __STDC__ | defined __cplusplus
(register tTree yyP398)
# else
(yyP398)
 register tTree yyP398;
# endif
{
  if (yyP398 == NULL) {
/* line 5645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5645 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((1))) goto yyL1;
  }
   return true;
yyL1:;

  }
  if (yyP398->DefFunc.TypeList->Kind == kAType) {
  if (yyP398->DefFunc.TypeList->AType.Next->Kind == kNoType) {
/* line 5646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5646 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((! IsScalarType (yyP398->DefFunc.TypeList->AType.Type)))) goto yyL2;
  }
   return true;
yyL2:;

  }
  }
  return false;
}

static bool IsNewSymbol
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP399)
# else
(yyP399)
 tSymbol yyP399;
# endif
{
/* line 5651 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5651 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((SymTable[yyP399].Id == NoIdent))) goto yyL1;
  }
   return true;
yyL1:;

  return false;
}

static void BeginBlock
# if defined __STDC__ | defined __cplusplus
(register tTree yyP400)
# else
(yyP400)
 register tTree yyP400;
# endif
{
/* line 5657 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5658 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (yyP400);
  }
   return;

;
}

static void EndBlock
# if defined __STDC__ | defined __cplusplus
(register tTree yyP401)
# else
(yyP401)
 register tTree yyP401;
# endif
{
/* line 5663 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5664 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (yyP401);
  }
   return;

;
}

static void BeginAtomicBlock
# if defined __STDC__ | defined __cplusplus
(register tTree yyP404, register tTree yyP403, register bool yyP402)
# else
(yyP404, yyP403, yyP402)
 register tTree yyP404;
 register tTree yyP403;
 register bool yyP402;
# endif
{
/* line 5670 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5671 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   OpenBlock (yyP402);
/* line 5672 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalSpecs (yyP404);
/* line 5673 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL1;
  }
   return;
yyL1:;

/* line 5676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5676 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((yyP403 != NoTree))) goto yyL2;
  {
/* line 5677 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalRep (yyP403);
/* line 5678 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((false))) goto yyL2;
  }
  }
   return;
yyL2:;

/* line 5681 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5682 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenLocalInits (yyP404);
  }
   return;

;
}

static void EndAtomicBlock
# if defined __STDC__ | defined __cplusplus
(register tTree yyP405)
# else
(yyP405)
 register tTree yyP405;
# endif
{
/* line 5687 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5688 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   GenResults (yyP405);
/* line 5689 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   CloseBlock ();
  }
   return;

;
}

void MarkRefParent
# if defined __STDC__ | defined __cplusplus
(tSymbol yyP408, register tTree yyP407, tVUsage yyP406)
# else
(yyP408, yyP407, yyP406)
 tSymbol yyP408;
 register tTree yyP407;
 tVUsage yyP406;
# endif
{
/* line 5695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5695 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((IsTopLevel (yyP408)))) goto yyL1;
  }
   return;
yyL1:;

  if (Tree_IsType (yyP407, kDefAProc)) {
/* line 5697 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5697 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((SymbolDepth (yyP408) <= SymbolDepth (yyP407->DefAProc.Sym)))) goto yyL2;
  {
/* line 5698 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   AddVUsage (yyP407->DefAProc.Sym, yyP406);
/* line 5699 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   MarkRefParent (yyP408, SymbolKind (yyP407 -> DefAProc . Parent), yyP406);
  }
  }
   return;
yyL2:;

  }
  if (Tree_IsType (yyP407, kDefAFunc)) {
/* line 5702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
  {
/* line 5702 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   if (! ((SymbolDepth (yyP408) <= SymbolDepth (yyP407->DefAFunc.Sym)))) goto yyL3;
  {
/* line 5703 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   AddVUsage (yyP407->DefAFunc.Sym, yyP406);
/* line 5704 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   MarkRefParent (yyP408, SymbolKind (yyP407 -> DefAFunc . Parent), yyP406);
  }
  }
   return;
yyL3:;

  }
/* line 5706 "/home/hpcc/SPOC/spoc.1.3/source/gmd/c.trafo" */
   return;

;
}

void BeginCout ()
{
}

void CloseCout ()
{
}
