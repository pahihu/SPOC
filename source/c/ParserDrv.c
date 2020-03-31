/*{{{  file banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Top level driver
||@(#)   System : Occam 2 C
||@(#) Filename : ParserDrv.c
||@(#)  Version : 1.35
||@(#)     Date : 10/4/97
\*@(#)====================================================*/
/*}}}*/
/*{{{  includes*/
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#include "Parser.h"
#include "Tree.h"
#include "OccamOut.h"
#include "PreTypeChkAg.h"
#include "TypeChkAg.h"
#include "Cout.h"
#include "CoutAg.h"
#include "ParUsage.h"
#include "ParUsageAg.h"
#include "Transform.h"
/*}}}*/
/*{{{  externs + defines*/
#define BoolChr(A) ((A)?'t':'f')
#define COMPILATION_FAILED -1
extern char *VersionName;
extern char *VersionDate;

#define DEFAULT_STR(f) (IS_Flag(f,DefaultModes) ? "disable" : "enable")
/*}}}*/
/*{{{  switches*/
bool cgdebug = false;
int yydebug = false;
int ldebug = false;
bool showinfo = false;

int nowarnings = false;

static bool nowarningsatall = false;
static bool nousagewarnings = false;
static bool noparsewarnings = false;

static bool showtree = false;
static bool showsyms = false;
static bool showdecs = false;
static bool doquery = false;
static bool doprettyc = false;
static bool lexonly = false;

static bool dointrinsics = true;
static bool dooccout = false;
static bool dostore = true;
static bool docout = true;
static bool dotransform = true;

static bool dotypechk_ag = true;
static bool docout_ag = true;

static bool do2_1to2 = false;
/*}}}*/

/*{{{  statics*/
char * OARCH = ARCH;
char * OSEARCH = "OSEARCH";
char * OLIB = "lib";
char * ODEFAULT_LIB = NULL;
char * OSPOC = "SPOC";
char * SLASH = "/";

FILE *yyf;
bool Compiler_is_LittleEndian;
static char *SpServerFile   = "spserver.lib";
static char *IntrinsicsFile = "Intrinsics.lib";

/*}}}*/

/*{{{  static void ApplyAttributeGrammars(void)*/
static void ApplyAttributeGrammars(void)
{
  if (ErrorCount>0) return;

  if (!CheckTree(TreeRoot)) { fprintf(stderr,"Fatal-ABSTRACT SYNTAX TREE is INVALID\n"); exit(-1); }
  
  /*{{{  if (dotypechk_ag)*/
  if (dotypechk_ag)
  {
    if (showinfo) fprintf(stderr,"Pre Type checking\n") ;
    BeginPreTypeChkAg();
    PreTypeChkAg(TreeRoot);
    ClosePreTypeChkAg();
    if (ErrorCount>0) return;
  
    if (showinfo) fprintf(stderr,"Type checking\n") ;
    BeginTypeChkAg();
    TypeChkAg(TreeRoot);
    CloseTypeChkAg();
  }
  /*}}}*/

  if (ErrorCount>0) return;
  
  /*{{{  if (IS_Mode(xxNoUsage)*/
  if (!IS_Mode(xxNoUsageChecking))
  {
    nowarnings = nowarningsatall || noparsewarnings;  
    if (showinfo) fprintf(stderr,"Parallel usage and alias checking\n") ;
    BeginParUsageAg();
    ParUsageAg(TreeRoot);
    CloseParUsageAg();
    nowarnings = nowarningsatall;
  }
  /*}}}*/
  
  if (ErrorCount>0) return;

  /*{{{  if (dotransform)*/
  if (dotransform)
  {
    if (showinfo) fprintf(stderr,"Tree transformations\n") ;
    
    Transform(TreeRoot);
  
    if (!CheckTree(TreeRoot)) { fprintf(stderr,"Fatal-ABSTRACT SYNTAX TREE is INVALID\n"); exit(-1); }
    BeginTypeChkAg();
    TypeChkAg(TreeRoot);
    CloseTypeChkAg();
  }
  /*}}}*/

  if (ErrorCount>0) return;

  /*{{{  if (docout_ag)*/
  if (docout_ag)
  {
    if (showinfo) fprintf(stderr,"Computing tree attributes for C code generation\n") ;
  
    BeginCoutAg();
    CoutAg(TreeRoot);
    CloseCoutAg();
  }
  /*}}}*/
}
/*}}}*/

/*{{{  #ifndef CORE_DUMP*/
#ifndef CORE_DUMP
void  FatalError(int signal)
{
  Message("Internal Error (please report to dan@ecs.soton.ac.uk)",xxFatal,Attribute.Position);
}
#endif
/*}}}*/
int main (int argc,char **argv)
{
  FILE *infile=stdin;
  char *filename= "stdin";
  char *outfile = "";
  int DefaultModes;
  char *ospoc = getenv(OSPOC);

  ldebug = false;
  yyf = stdout;
  
#ifndef CORE_DUMP
  signal(SIGSEGV,FatalError);
#endif

  /*{{{  default modes*/
  {
    int val = 1;
    Compiler_is_LittleEndian = (((char *)&val)[0]) == 1;
  }
  ADD_Mode(xxOccamLines);
  ADD_Mode(xxCheckRanges);
  ADD_Mode(xxCheckArrays);
  ADD_Mode(xxCheckConversions);
  ADD_Mode(xxInmosExtensions);
  ADD_Mode(xxNoFreeVars);
  if (Compiler_is_LittleEndian) ADD_Mode(xxLittleEndian);
  /*ADD_Mode(xxEmulate);*/
  
  #ifdef SUPPORT_INT64
  ADD_Mode(xxSupportINT64);
  #endif
  
  DefaultModes = GET_Mode();
  /*}}}*/
  /*{{{  check arguments*/
  {
    int i;
    for(i=1;i<argc;i++)
    {
      if (argv[i][0]=='-')
      {
	int j=1;
	while (argv[i][j]!='\0')
	  switch (argv[i][j])
	  {
	    case '2': 
		      /*{{{  choose occam version*/
		      j++;
		      switch (argv[i][j])
		      {
			case '1' : 
			  SET_Mode(xxUserDataTypes,true); 
			  j++;
			  break;
			case '\0' : do2_1to2 = true; break;
			default : MessageI("Unknown -2 Option",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
		      }
		      break;
		      /*}}}*/
	    case 'c': doprettyc = true;
		      SET_Mode(xxOccamLines,false);
		      SET_Mode(xxPrettyC,true); 
		      SET_Mode(xxSupportINT64,false); 
		      if (!IS_Flag(xxLittleEndian,DefaultModes)) { SET_Mode(xxLittleEndian,true); }
		      SET_Mode(xxLibrary,true); 
		      j++; break;
	    case 'C': j++; break;  /* compatibility */
	    case 'd': SET_Mode(xxCDirectives,!IS_Flag(xxCDirectives,DefaultModes)); j++; break;
	    case 'e':
		      /*{{{  error mode*/
		      j++;
		      switch (argv[i][j])
		      {
			case 's' : ErrorMode=xxStoppOnError; j++; break;
			case 't' : ErrorMode=xxTerminateOnError; j++; break;
			case 'c' : ErrorMode=xxContinueOnError; j++; break;
			default :  MessageI("Unknown -e Option",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
		      }
		      break;
		      /*}}}*/
	    case 'g': SET_Mode(xxOccamDebug,!IS_Flag(xxOccamDebug,DefaultModes)); j++; break;
	    case 'h': case 'H' : case 'V' :
		       /*{{{  Print help messages and options*/
		       fprintf(stderr,"SPOC : Southampton's Portable Occam Compiler, Version %s, %s\n",VersionName,VersionDate);
		       fprintf(stderr,"M. Debbage, X. Fu, M. Hill, D. Nicole and S. Wykes  University Of Southampton, ESPRIT GPMIMD P5404\n\n");
		       if (argv[i][j]=='V') exit(0);
		       fprintf(stderr,"Usage: %s [-options] [filename]\n",argv[0]);
		       fprintf(stderr,"  -2 : perform occam 2.1 to occam 2 translation (experimental)\n");
		       fprintf(stderr,"  -21 : accept occam 2.1\n");
		       fprintf(stderr,"  -c : generate pretty C for conversion to D7314 (experimental)\n");
		       fprintf(stderr,"  -d : enable C Directives\n");
		       fprintf(stderr,"  -e : select Error mode, suffix by one of:\n");
		       fprintf(stderr,"           c : Continue on error\n");
		       fprintf(stderr,"           s : Stop process on error\n");
		       fprintf(stderr,"           t : Terminate application on error (default)\n");
		       fprintf(stderr,"  -g : enable occam debuGging \n");
		       fprintf(stderr,"  -i : disable inclusion of Intrinsics\n");
		       fprintf(stderr,"  -l : generate Library unit\n");
		       fprintf(stderr,"  -m : change code generation Mode, suffix by any combination of:\n");
			 /*{{{  -m modes*/
			   fprintf(stderr,"           a : disable Array checks\n");
			   fprintf(stderr,"           c : disable Conversion checks\n");
			 
			 if (IS_Flag(xxLittleEndian,DefaultModes))
			   fprintf(stderr,"           e : select big-Endian source code\n");
			 else
			   fprintf(stderr,"           e : select little-Endian source code\n");
			 
			   fprintf(stderr,"           f : enable calls for NAG F90 code\n");
			   
			   fprintf(stderr,"           i : generate target Independent code\n");
			 #ifdef SUPPORT_INT64     
			   fprintf(stderr,"           l : disable long-long INT64\n");
			 #endif                   
			   fprintf(stderr,"           m : enable coMmunication checks\n");
			   fprintf(stderr,"           n : disable generation of occam line Numbers\n");
			   fprintf(stderr,"           p : enable PLACED PAR support\n");
			   fprintf(stderr,"           r : disable Range checks\n");
			   fprintf(stderr,"           v : enable free Var access from within PLACED PAR\n");
			   fprintf(stderr,"           w : enable INT as 16-bit Word\n");
			 /*}}}*/
		       fprintf(stderr,"  -o <file> : specify new output filename (use - for stdout)\n");
		       if (argv[i][j]=='H')
		       {
			 fprintf(stderr,"  -p : control internal execution Phases, suffix by any combination of: \n");
			 /*{{{  phase components*/
			 fprintf(stderr,"           t : disable Type checking calculation phase\n");
			 fprintf(stderr,"           p : disable Parallel usage calculation phase\n");
			 fprintf(stderr,"           s : disable code Simplification/transformation phase\n");
			 fprintf(stderr,"           a : disable Attribute calculation phase\n");
			 fprintf(stderr,"           c : disable C code generation phase\n");
			 fprintf(stderr,"           o : enable Occam code generation phase (disables C generation)\n");
			 
			 /*}}}*/
		       }
		       else
			 fprintf(stderr,"  -p : control internal execution Phases (use option -H for details)\n");
		       fprintf(stderr,"  -s : enable simulated Server\n");
		       fprintf(stderr,"  -t#: Allows tabs (of size #) in source code (no size turns off source tabs)\n");
		       fprintf(stderr,"  -u : Usage warnings instead of errors\n");
		       fprintf(stderr,"  -v : Verbose compilation information\n");
		       fprintf(stderr,"  -w : suppress all Warnings, or suffix by any combination of:\n");
		       fprintf(stderr,"           p : disable Parser warnings\n");
		       fprintf(stderr,"           u : disable Usage warnings\n");
		       fprintf(stderr,"  -x : select language eXtensions, suffix by any combination of:\n");
		       fprintf(stderr,"           i : disable standard Inmos extensions\n");
		       fprintf(stderr,"           u : User data type extensions\n");
		       if (argv[i][j]=='H')
		       {
			 fprintf(stderr,"  -z :     Internal debugging options, suffix by any combination of:\n");
			 /*{{{  */
			 fprintf(stderr,"           c : Debug code generator\n");
			 fprintf(stderr,"           d : Debug parser\n");
			 fprintf(stderr,"           e : don't sort Error messages\n");
			 fprintf(stderr,"           l : Lex only\n");
			 fprintf(stderr,"           n : occam liNe numbers are comments\n");
			 fprintf(stderr,"           q : Query syntax tree\n");
			 fprintf(stderr,"           s : show Symbol table\n");
			 fprintf(stderr,"           t : show abstract syntax Tree\n");
			 fprintf(stderr,"           v : show Variable node details\n");
			 fprintf(stderr,"           x : leXical debug\n\n");
			 /*}}}*/
		       }
		       else
			 fprintf(stderr,"  -z :     enable/disable internal debugging options (use option -H for details)\n\n");
		       exit(0);
		       /*}}}*/
	    case 'i': dointrinsics = false; j++; break;
	    case 'l': SET_Mode(xxLibrary,!IS_Flag(xxLibrary,DefaultModes)); j++; break;
	    case 'm':
		      /*{{{  code generation mode*/
			j++; 
			while (argv[i][j]!='\0')
			{    
			switch (argv[i][j])
			{    
			  case 'a' : SET_Mode(xxCheckArrays,!IS_Flag(xxCheckArrays,DefaultModes)); j++; break;
			  case 'c' : SET_Mode(xxCheckConversions,!IS_Flag(xxCheckConversions,DefaultModes)); j++; break;
			  case 'e' : SET_Mode(xxLittleEndian,!IS_Flag(xxLittleEndian,DefaultModes)); j++; break;
			  case 'i' : SET_Mode(xxEmulate,!IS_Flag(xxEmulate,DefaultModes)); j++; break;
			  case 'f' : SET_Mode(xxCallF90,!IS_Flag(xxCallF90,DefaultModes)); j++; break;
		      #ifdef SUPPORT_INT64  
			  case 'l' : SET_Mode(xxSupportINT64,!IS_Flag(xxSupportINT64,DefaultModes)); j++; break;
		      #endif                
			  case 'm' : SET_Mode(xxCheckInternals,!IS_Flag(xxCheckInternals,DefaultModes)); j++; break;
			  case 'n' : SET_Mode(xxOccamLines,!IS_Flag(xxOccamLines,DefaultModes)); j++; break;
			  case 'p' : SET_Mode(xxSupportPlaced,!IS_Flag(xxSupportPlaced,DefaultModes)); j++; break;
			  case 'r' : SET_Mode(xxCheckRanges,!IS_Flag(xxCheckRanges,DefaultModes)); j++; break;
			  case 'v' : SET_Mode(xxNoFreeVars,!IS_Flag(xxNoFreeVars,DefaultModes)); j++; break;
			  case 'w' : SET_Mode(xx16bitINT,!IS_Flag(xx16bitINT,DefaultModes)); j++; break;
			  default :  MessageI("Unknown -m Option",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
			}    
		      }
		      break;
		      /*}}}*/
	    case 'o':
		      /*{{{  output file name*/
		      if (strlen(outfile) == 0 && i < argc-1)
		      {
			outfile = argv[++i]; j = strlen(argv[i]);
		      }
		      else
		      {
			Message("Invalid -o option",xxError,NoPosition);
		      }
		      break;
		      /*}}}*/
	    case 'p':
		      /*{{{  compiler phase control*/
		      j++;
		      while (argv[i][j]!='\0')
		      {
			switch (argv[i][j])
			{    
			  case 'a': docout_ag = false; docout = false; j++; break;
			  case 'c': docout = false; j++; break;
			  case 'o': dooccout = true; docout = false; j++; break;
			  case 'p': SET_Mode(xxNoUsageChecking,!IS_Flag(xxNoUsageChecking,DefaultModes)); j++; break;
			  case 's': dotransform = false; docout = false; j++; break;
			  case 't': dotypechk_ag = false; docout = false; j++; break;
			  default :  MessageI("Unknown -p Option",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
			}    
		      }
		      break;
		      /*}}}*/
	    case 's': SET_Mode(xxSimulatedServer,!IS_Flag(xxSimulatedServer,DefaultModes)); j++; break;
	    case 't':
		      /*{{{  tabs*/
		      j++;
		      {
			char c=argv[i][j];
			if (isdigit(c))
			{    
			  no_tabs=false;
			  tab_size=c-'0';
			  c=argv[i][++j];
			  while (isdigit(c))
			  {  
			    tab_size = (c*10)+(c-'0');
			    c=argv[i][++j];
			  }  
			}    
			else 
			  no_tabs=true;
		      };
		      break;
		      /*}}}*/
	    case 'u': SET_Mode(xxUsageWarnings,!IS_Flag(xxUsageWarnings,DefaultModes)); j++; break;
	    case 'v': showinfo = true; j++; break;
	    case 'w': j++;
		      if (argv[i][j]=='\0')
			nowarningsatall = true;
		      else
			while (argv[i][j]!='\0')
			{
			  switch (argv[i][j])
			  {
			    case 'u': nousagewarnings=true; j++; break;
			    case 'p': noparsewarnings=true; j++; break;
			  }
			}
		      break;
	    case 'x':
		      /*{{{  select extension modes*/
		      j++;
		      while (argv[i][j]!='\0')
		      {
			switch (argv[i][j])
			{    
			  case 'i' : SET_Mode(xxInmosExtensions,!IS_Flag(xxInmosExtensions,DefaultModes)); j++; break;
			  case 'u' : SET_Mode(xxUserDataTypes,!IS_Flag(xxUserDataTypes,DefaultModes)); j++; break;
			}    
		      }
		      break;
		      /*}}}*/
	    case 'z':
		      /*{{{  internal debugging stuff*/
		      j++;
		      while (argv[i][j]!='\0')
		      {
			 switch (argv[i][j])
			 {   
			    case 'c' : SET_Mode(xxOccamLines,false); cgdebug = true; dostore = false; outfile = "-"; j++; break;
			    case 'd' : yydebug = true; j++; break;
			    case 'e' : dostore = false; j++; break;
			    case 'l' : lexonly = true; j++; break;
			    case 'n' : SET_Mode(xxLinesAsComments,true); j++; break;
			    case 'q' : doquery = true; j++; break;
			    case 's' : showsyms = true; j++; break;
			    case 't' : showtree = true; j++; break;
			    case 'v' : showdecs = true; j++; break;
			    case 'x' : ldebug = true; j++; break;
		      
			    default :  MessageI("Unknown -z Option",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
			 }   
		      }
		      break;
		      /*}}}*/
	    case '-': j++; break;
	    default : MessageI("Unknown Option : ",xxWarning,NoPosition,xxCharacter,&argv[i][j]); j++;
	  };
      }
      else if (infile==stdin)
      {
	filename = argv[i];
	if ((infile = fopen(filename,"r"))==NULL)
	{
	  Attribute.Position.File = PutString(filename,strlen(filename));
	  Message("File does not exist",xxFatal,Attribute.Position);
	  exit(-1);
	}
      }
    }
  }
  /*}}}*/

  if (do2_1to2)
  {
     ADD_Mode(xxUserDataTypes);
     dotransform = false;
  }
  if (IS_Mode(xxOccamDebug)) 
  {
    ADD_Mode(xxOccamLines);  
    if (IS_Mode(xxUserDataTypes)) 
    {
      Message("No occam debug support for user data types",xxWarning,NoPosition);
      ADD_Mode(xxLinesAsComments);
    }
  }
  if (dostore) StoreMessages(true);

  /*{{{  set up DEFAULT search path*/
  if (ospoc == NULL)
    Message("Could not get SPOC environment variable",xxError,NoPosition);
  
  if ((ODEFAULT_LIB = malloc (strlen(ospoc)+strlen(OLIB)+strlen(OARCH)+4)) == NULL)
    Message("Out of heap",xxFatal,NoPosition);
  else
    {
      strcpy (ODEFAULT_LIB,ospoc);
      strcat (ODEFAULT_LIB,SLASH);
      strcat (ODEFAULT_LIB,OLIB);
      strcat (ODEFAULT_LIB,SLASH);
      strcat (ODEFAULT_LIB,OARCH);
      strcat (ODEFAULT_LIB,SLASH);
    }
  /*}}}*/

  /*{{{  call init for gmd modules*/
  InitScanner(filename,infile);
  InitStringMemory();
  InitIdents();
  InitSymbols();
  /*}}}*/
  /*{{{  include intrinsic libraries*/
  /* When the simulated server is used, the back-end assumes that
     its prototype can be found at the beginning of the symbol table.
     The following two lines are reversed to set up the includes
     stack appropriately.
  */
     
  if (dointrinsics)
    if (!EnterInclude(PutString(IntrinsicsFile,strlen(IntrinsicsFile)),ExtnModes|BIT_Mode(xxPrototype)))
      MessageI("Could not open intrinsics file ", xxError, Attribute.Position, xxString, IntrinsicsFile);
  
  if (IS_Mode(xxSimulatedServer) && !IS_Mode(xxLibrary))
    if (!EnterInclude(PutString(SpServerFile,strlen(SpServerFile)),ExtnModes|BIT_Mode(xxPrototype)))
      MessageI("Could not open spserver file ", xxError, Attribute.Position, xxString, SpServerFile);
  
  /*}}}*/

  if (lexonly)
  {
    while(GetToken() != 0);
  }
  else
  {
    nowarnings= nowarningsatall || noparsewarnings;
    if (showinfo) fprintf(stderr,"Parsing code\n") ;
    (void) Parser	();
    CloseParser	();
    nowarnings= nowarningsatall;
    
    OccSymSize=SymSize;
    ApplyAttributeGrammars();

    /*{{{  if (showtree)*/
    if (showtree)
    {
      if (showinfo) fprintf(stderr,"Writing tree\n") ;
      WriteTree(stdout,TreeRoot);
    }
    /*}}}*/
    /*{{{  if (showsyms)*/
    if (showsyms)
    {
      if (showinfo) fprintf(stderr,"Writing symbols\n") ;
      WriteSymTable() ;
    }
    /*}}}*/
    /*{{{  if (showdecs)*/
    if (showdecs)
    {
      int i;
      for (i=1;i<=SymSize;i++)
      {
	printf("ENTRY %d scope %d ",i,SymTable[i].Scope);WriteIdent(stdout,SymTable[i].Id);
	printf("\n");
	WriteTreeNode(stdout,SymTable[i].Kind);
      }
    }
    /*}}}*/

    if (doquery) QueryTree(TreeRoot);

    if (dooccout || (do2_1to2 && ErrorCount==0))
      /*{{{  outputoccam*/
      {
	if (showinfo) fprintf(stderr,"Generating occam output\n") ;
	OutputOccam(strlen(outfile)==0 ? "-" : outfile,TreeRoot);
      }
      /*}}}*/
    else if (ErrorCount==0 && docout)
      /*{{{  output C*/
      {
	if (strlen(outfile) == 0)
	{    
	  char *dot = strrchr(filename,'.');
      
	  if (dot != NULL) *dot = 0;
	  outfile = malloc(strlen(filename) + 3);
	  strcpy(outfile,filename); strcat(outfile,".c");
	}    
	     
      
	if (dotypechk_ag && dotransform && docout_ag)
	{    
	  if (showinfo) fprintf(stderr,"Generating C output\n") ;
	  OutputC(outfile,TreeRoot);
	}    
	else 
	  Message("Unable to generate code - attribute grammer or transformation disabled",xxWarning,Attribute.Position);
      }
      /*}}}*/

    if (dostore) WriteMessages(stderr);
    if (ErrorCount>0) fprintf(stderr,"%d Errors encountered.\n",ErrorCount);
  }

  if (showinfo) fprintf(stderr,"Terminating\n") ;
 
  if (infile!=stdin) fclose(infile);

  return (ErrorCount>0)?COMPILATION_FAILED:0;
}
