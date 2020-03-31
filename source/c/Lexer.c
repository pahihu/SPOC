/*{{{  file banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title :  Lexical Analyser
||@(#)   System : Occam 2 C
||@(#) Filename : Lexer.c
||@(#)  Version : 1.3
||@(#)     Date : 3/4/97
\*@(#)====================================================*/
/*}}}*/

/*{{{  ORIGINAL HEADER */
/*
 * OCCAM2 lexical analysis routine
 * 
 * pjmp  HRC  9/3/89 mbh     uk.ac.soton.ecs  4/12/92 modified for GMD tools
 * 
 */

/*
 * This work is in the public domain. It was written by Peter Polkinghorne in
 * 1986 & 1989 at GEC Hirst Research Centre, Wembley, England. No liability
 * is accepted or warranty given by the Author, still less my employers.
 */
/*}}}*/
/*{{{  includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Scanner.h"
#include "Errors.h"
#include "DynArray.h"
#include "Tree.h"
#include "Fopenenv.h"
#include "Symbol.h"

extern bool showinfo;
/*}}}*/
/*{{{  defines*/
#define  Error(s) { fprintf(stderr,"Fatal Error - %s\n",s); exit(-1); }

#define  MAXLINE  256
#define  STARTBUF  1024

#define COMMENT 999
/*}}}*/

/*{{{  global variables*/
static unsigned long lineExtent = STARTBUF;
static char *line = NULL;

char yytext[MAXLINE];
int yylen;

static int llen;          /* how much in line */
static int nolines = 0;   /* no of lines */
static int curind;     /* current indentation */
static int indent = 0; /* this lines indent */

static int idx = 0;  /* token idx */
static int sind = 0;   /* start of current token */

int yylineno = 1;
bool firsttok=true;
bool colon_ended = false;
int tab_size = 8;
bool no_tabs = false;
static bool tab_error = false;
static bool between_brackets = false;

static FILE *infile=NULL;

static char *DV_PRAGMA = "PRAGMA";
static char *DV_OPTION = "OPTION";
static char *DV_COMMENT = "COMMENT";
static char *DV_USE = "USE";
static char *DV_INCLUDE = "INCLUDE";
static char *DV_C = "C";
static char *DV_H = "H";

static enum lexstate
{
  Start, Rest, Eof, DoEol, DoStr, DoCStr, IgnoreIndentedText, IgnoreIndentedTextEol
} state = Start;

/*}}}*/
/*{{{  Tokens */
struct KeyList
{
  char *Name;
  int Val;
};

#include "TokenDefs.h"
#include "KeyTable.h"
#define IsUdtToken(t) ((t>(NoTokens+NoKeywords))&&(t<=(NoTokens+NoKeywords+NoKeywords_udt)))
#define IsImsToken(t) ((t>(NoTokens+NoKeywords+NoKeywords_udt))&&(t<=(NoTokens+NoKeywords+NoKeywords_udt+NoKeywords_ims)))
struct KeyList *KeyStart[26], *KeyEnd[26];

/*}}}*/
/*{{{  Punctuation */
struct KeyList PuncList[] =
{
  {"--", COMMENT},
  {":=", ASSIGN},
  {"<>", NE},
  {"<=", LE},
  {">=", GE},
  {"/\\", BITAND},
  {"\\/", BITOR},
  {"><", BITEOR},
  {"\\", REM},
  {"<<", LSHIFT},
  {">>", RSHIFT},
  {"::", DCOLON},
  {"[",LSB},
  {"]",RSB},
  {"?",QUERY},
  {"!",PLING},
  {"&",AMPERSAND},
  {",",COMMA},
  {":", COLON},
  {"=", EQ},
  {"+", ADD},
  {"-", SUBTRACT},
  {"*", MULTIPLY},
  {"/", DIVIDE},
  {"~", BITNOT},
  {"(", LP},
  {")", RP},
  {"<", LT},
  {">", GT},
  {";", SEMICOLON},
  {0, 0}
};

/*}}}*/

/*{{{  ScanAttribute declaration*/
tScanAttribute Attribute;

void ErrorAttribute(short yyToken,tScanAttribute *yyRepairAttribute)
{
  switch (yyToken)
  {
    case ID:
      yyRepairAttribute->Value.Ident = NoIdent;
      break;
    case NUMBER:
      yyRepairAttribute->Value.Number = 0;
      break;
    case HEXNUMBER:
      yyRepairAttribute->Value.Number = 0;
      break;
    case RNUMBER:
      yyRepairAttribute->Value.RNumber.n = 0.0;
      yyRepairAttribute->Value.RNumber.p = 1;
      break;
    case CHCON:
      yyRepairAttribute->Value.Char = '?';
      break;
    case STR:
      yyRepairAttribute->Value.String = NoString;
      break;
    default:
      yyRepairAttribute->Value.Number = 0;
  }
  yyRepairAttribute->Position=Attribute.Position;
}

/*}}}*/

/*{{{  coping with include files */
#define  MAXINCS  10

static struct
{
  tStringRef name;
  int line;
  int indent;
  FILE *ptr;
  int mode;
} file[MAXINCS];

static inclevel = 0;

/*{{{  int LUIncludeLevel(f) */
int LUIncludeLevel(f)
tStringRef f;
{
  int i;

  for (i = 0; i < MAXINCS; i++)
    if (file[i].name == f)
      return (i);
  return (-1);
}

/*}}}*/

static char here[] = "./" ;

/*{{{  FILE *OpenInclude(inc)*/
FILE *OpenInclude(inc)
char *inc;
{
  FILE *fi;
  char *path;
  
  fi = fopenenv(inc,"r",OSEARCH,ODEFAULT_LIB,&path);
  if (showinfo && fi!=NULL)
  {
    fprintf(stderr,"Information - Reading file %s\n",path);
  }
  return fi;
}
/*}}}*/

/*{{{  void CloseInclude(fptr)*/
void CloseInclude(fptr)
FILE *fptr;
{
  if (ferror(fptr)|| fclose(fptr))
    Message("Error reading #INCLUDE file ", xxFatal, Attribute.Position);
}
/*}}}*/

/*{{{  bool EnterInclude(tStringRef incf,int NewMode)*/
bool EnterInclude(tStringRef incf,int NewMode)
{ 
  yylineno += nolines; nolines = 0;
  file[inclevel].line = yylineno;
  file[inclevel].indent = indent;
  file[inclevel].mode = ExtnModes;
  inclevel++;
  file[inclevel].name = incf;

  {  
    char f[256];
    
    StGetString(file[inclevel].name, f);

    if ((file[inclevel].ptr = OpenInclude(f)) == NULL)
    {
      inclevel--;
      return false;
    }
    else
    {
      ExtnModes=NewMode;
      yylineno = 1;
      indent = 0;
      curind = 0;  
      infile = file[inclevel].ptr;
      return true;
    }
  }  
}
/*}}}*/
/*}}}*/
/*{{{  static void UpdatePosition()*/
static void UpdatePosition()
{
  static int old_cntlen = 0;
  static int old_yylineno = 0;
  static int old_col = 0;
  int i = 0;
  int col=0;
  int cntlen= (sind<(llen-1)) ? sind : (llen-1);
  
  if (yylineno==old_yylineno && Attribute.Position.File==file[inclevel].name)
  {    
    i = old_cntlen;
    col = old_col;
    
  }
  else 
    Attribute.Position.Line = yylineno;
  
  for (;i<cntlen;i++)
  {
    if (line[i]=='\n')
    {
      Attribute.Position.Line++;
      col=0;
    }
    else
      col++;
  }
  Attribute.Position.Column = col;
  Attribute.Position.File = file[inclevel].name;
  old_cntlen = cntlen;
  old_col = col;
  old_yylineno = yylineno;
}
/*}}}*/

/*{{{  coping with VAL of indentation */
#define MAXVALOF 100
static int valofi = 0;
static int valofs[MAXVALOF] = {0};  
static int lasttok = -1;

/*{{{  void PushTokenColumn()*/
void PushTokenColumn()
{
  if (++valofi==MAXVALOF)
  {
    Message("Exceeded limits of expression indentation stack",xxFatal,Attribute.Position);
    valofi--;
  }
  if (Attribute.Position.Column<(2*curind))
  {
    Message("Insufficient indentation",xxError,Attribute.Position);
    valofs[valofi]=0;
  }
  else
    valofs[valofi]=Attribute.Position.Column-2*curind;
}
/*}}}*/
/*{{{  void PushColumn(int l)*/
void PushColumn(int l)
{
  if (++valofi==MAXVALOF)
  {
    Message("Exceeded limits of expression indentation buffer",xxFatal,Attribute.Position);
    valofi--;
  }
  if (l<(2*curind))
  {
    Message("Insufficient indentation",xxError,Attribute.Position);
    valofs[valofi]=0;
  }
  else
    valofs[valofi]=l-2*curind;
}
/*}}}*/
/*{{{  void PopColumn()*/
void PopColumn()
{
  if (--valofi<0)
  {
    Message("Attempted pop from empty expression indentation stack",xxFatal,Attribute.Position);
    valofi = 0;
  }
}
/*}}}*/
/*}}}*/
/*{{{  void InitScanner(name,fptr) */
void InitScanner(name, fptr)
char *name;
FILE *fptr;
{
  struct KeyList *p;
  int i, j;

  for (i = 0; i < 26; ++i)
    KeyStart[i] = (struct KeyList *) NULL;

  for (p = &KeyTable[0]; p->Name; ++p)
  {
    j = tolower(p->Name[0]) - 'a';
    if (KeyStart[j] == (struct KeyList *) NULL)
      KeyStart[j] = p;
    KeyEnd[j] = p;
  }

  for (i = 0; i < MAXINCS; i++)
    file[0].name = NULL;

  file[0].name = PutString(name, strlen(name));
  file[0].ptr = fptr;

  infile = fptr;

  Attribute.Position = NoPosition;
  Attribute.Position.File = file[0].name;

  tab_error = false;
  MakeArray(&line,&lineExtent,1);
}

/*}}}*/
/*{{{  static int LUKeyword(char *s, int n)*/
static int LUKeyword(char *s, int n)
{
  register struct KeyList *pk;
  int k;

  if (!isalpha(*s) || !isupper(*s))
    return (-1);

  k = *s - 'A';

  for (pk = KeyStart[k]; pk && pk <= KeyEnd[k]; ++pk)
  {
    register char *key, *text;
    register int count = 0;

    key = pk->Name;
    text = s;

    while (*key && *key == *text++)
    {
      key++;
      count++;
    }

    if (*key == 0 && count == n)/* Matches table entry */
    {
      if (IsImsToken(pk->Val)&&(!IS_Mode(xxInmosExtensions)))
      {
	UpdatePosition();
	Message("Identifier used as keyword in Inmos occam 2",xxWarning,Attribute.Position);
	return -1;
      }
      else if (IsUdtToken(pk->Val)&&(!IS_Mode(xxUserDataTypes)))
      {
	UpdatePosition();
	Message("Identifier used as keyword for User Data Types language extension",xxWarning,Attribute.Position);
	return -1;
      }
      else
	return pk->Val;
    }
  }
  return -1;
}

/*}}}*/
/*{{{  static bool IsEmptyLine()*/
static bool IsEmptyLine()
{
  int lidx = idx;
  
  while (line[lidx] == ' ')
    lidx++;
  if (line[lidx] == '\n' || line[lidx] == '\0')
    return (true);
  return (false);
}

/*}}}*/
/*{{{  static bool IsBetweenBrackets()*/
static bool IsBetweenBrackets()
{
  int i;
  for (i=sind;i<llen;i++) { if (line[i]==']') return true; }
  return false;
}
/*}}}*/
/*{{{  static bool IsContinued()*/
static bool IsContinued()
{
  int f = 0;
  int t;

  colon_ended = false;

  while (line[f]==' ') f++;
  if (line[f]=='#') return (false);  
  
  f = llen - 1;
  while (line[f] == ' ' || line[f] == '\n')
  {
    f--;
    if (f<0) return false;
  }
  if (isalnum(line[f]) || line[f] == '.')
  {
    int b = f + 1;

    while (line[f] != ' ')
      f--;
    f++;
    if (isalpha(line[f]) && isupper(line[f]))
    {
      t = LUKeyword(&line[f], b - f);
      switch(t)
      {
	case SIZE: case REM :
	case IS: case RETYPES : /* Note retypes not in Occam2Ref */
	case FROM:
	case FOR:
	case AFTER :
	case MOSTNEG : case MOSTPOS :
	case AND: case OR: case NOT:
	case BITAND: case BITNOT: case BITOR:
	case ROUND: case TRUNC:
	case MINUS: case PLUS: case TIMES:
	  return (true);
      }
    }
  }
  else
  {
    switch(line[f])
    {
      case '+':
      case '-':
      case '/':
      case '*':
      case '~':
      case '\\':
      case ',':
      case '=':
      case ';':
      case '<':
      case '>':
	return (true);
      case ':':
	colon_ended = true;
    }
  }
  return (false);
}

/*}}}*/
/*{{{  char *fgets_padding_tabs(char *s, int n, FILE *stream,bool *full)*/
char *fgets_padding_tabs(char *s, int n, FILE *stream,bool *full)
{
  int l=0;
  unsigned char c = '\0';
  n = n-tab_size;
  if (feof(stream))
    return NULL;
  else
  {
    while (!feof(stream) && l<(n-1) && c!='\n')
    {
      c=(unsigned char)fgetc(stream);
      
      if (c < ' ')
      {
	switch(c)
	{
	  case '\t':
	    {
	      if (no_tabs) 
	      {
		if (!tab_error) Message("No tabs allowed in this mode",xxError,Attribute.Position);
		tab_error = true;
	      }
	      if (l%tab_size == 0) s[l++] = ' ';
	      while(l%tab_size != 0)       
		s[l++]=' ';
	    }
	    break;
	  case '\n':
	    s[l++]=c; break;
	  case 13:
	  case 26:
	    break;
	  default:
	    MessageI("Invalid character in source",xxWarning,Attribute.Position,xxCharacter,(char *)&c);
	    break;
	}
      }
      else
	s[l++]=c;
    }
    if (feof(stream)) s[--l]='\0'; else s[l++]='\0';
    *full = ((l>=n) && (c!='\n'));
    return s;
  }
}
/*}}}*/
/*{{{  static bool RestIsEmpty(char *l)*/
static bool RestIsEmpty(char *l)
{
  while (true)
  {
    if (*l=='\n') return true; 
    if (*l=='\0') return true; 
    if (*l=='-' && *(l+1)=='-') return true;
    if (*l!=' ') return false;
    l++; 
  }
}
/*}}}*/
/*{{{  static int GetLine()*/
static int GetLine()
{
  bool outside_string = true ;
  int oldlen = 0;
  bool full = false;
  nolines = 0;
  llen = 0;
  

  do
  {
    nolines++;
    if (fgets_padding_tabs(&line[llen], lineExtent - llen, infile,&full) == NULL && nolines==1)
    { 
      state = Eof;
      return (Eof);
    }
    llen += strlen(&line[llen]);
    while (full)
    {
      ExtendArray(&line,&lineExtent,1);
      fgets_padding_tabs(&line[llen], lineExtent - llen, infile,&full);
      llen += strlen(&line[llen]);
    }          

    if (nolines == 1) /* Don't mess with # lines */
    {
      int i = 0;
      while(line[i] == ' ') ++i;
      if (line[i] == '#') break;
    }

    /*{{{  goes through new bit of string looking for comments and continuations*/
    {
      int i;
    
      for (i = oldlen; i < llen; i++)
      {
	if (line[i] == '"') outside_string = !outside_string ;
    
	else if (outside_string)
	{  
	  if (line[i] == '-' && line[i + 1] == '-')
	  {
	    line[i++] = '\n';
	    line[i] = 0;
	    llen = i;
	    break;
	  }
	  if (line[i] == '\'')
	  {
	    if (line[i+1] == '*')
	    {
	      if (line[i+2] == '#') i+=2;
	      else i++;
	    }
	    i+=2;
	  }
	}  
	else
	{  
	  if (line[i]=='*')
	  {
	    if (line[i+1]=='*') i++;
	    else if (line[i+1]=='"') i++;
	    else if (RestIsEmpty(&line[i+1]))
	    {
	      i++;
	      line[i++]='\n';
	      line[i] = 0;
	      llen = i; 
	      break;
	    }
	  }
	}  
    
      }
      oldlen = llen;
    }
    /*}}}*/
  } while (llen >0 && IsContinued());

#ifdef YYDEBUG
  if (ldebug)  fprintf(stderr,"%s",line);
#endif
  return (-1);
}

/*}}}*/
/*{{{  static int HexDigit(int c)*/
static int HexDigit(int c)
{
  if (c>='0' && c<='9')
    return c - '0' ;
  else if ((c>='A') && (c<='F'))
    return c - 'A' + 10 ;
  else
    {
      Message("Invalid hexadecimal character",xxError,Attribute.Position);
      return 0 ;
    }
}
/*}}}*/
/*{{{  static int MapEscapedCharacter (char *text, int len, char *cout)*/
static int MapEscapedCharacter (char *text, int len, char *cout)
{  
  if (len < 2 || text[0] != '*')
    Message("Invalid escaped character sequence",xxError,Attribute.Position);

  switch (text[1])
  {
    case 'c':
    case 'C':   *cout = 13 ;
		return 2 ;
    case 'n':
    case 'N':	*cout = 10 ;
		return 2 ;
    case 't':
    case 'T':	*cout = '\t' ;
		return 2 ;
    case 's':
    case 'S':	*cout = ' ' ;
		return 2 ;

    case '\'':
    case '"':
    case '*':   *cout = text[1] ;
		return 2 ;
    
    case '#':	if (len < 4)
		  Message("Invalid escaped character sequence",xxError,Attribute.Position);
		else
		{
		  *cout = (HexDigit(text[2]) * 16) + HexDigit(text[3]) ;
		  return 4 ;
		}
		
    default:	{
		  Message("Invalid escaped character",xxWarning,Attribute.Position); 
		  *cout = text[1] ;
		  return 2 ;
		}
  }
}
/*}}}*/
/*{{{  static void CalculateIndentation()*/
static void CalculateIndentation()
{
    for (; idx < valofs[valofi]; idx++)
      if (line[idx] != ' ')
      {
	UpdatePosition();
	Message("Incorrect indentation", xxError, Attribute.Position);
	break;
      }
      else
	sind++;

    while (line[idx] == ' ' && line[idx + 1] == ' ')
    {    
      indent++;
      idx += 2;
      sind += 2;
    }
    UpdatePosition();
    if (line[idx] == ' ' && line[idx + 1] != ' ')
    {  
      Attribute.Position.Column++;
      Message("Incorrect indentation", xxError, Attribute.Position);
      idx++;
      sind++;
    }  

}
/*}}}*/

/*{{{  int GetToken() */
int GetToken()
{
  int tok = -1;
  
  sind = idx;    

  while (tok < 0)
  {
    switch (state)
    {
      /*{{{  case Start:*/
      case Start:
      {
	yylineno+=nolines;
	GetLine(); 
	if (state == Eof)
	  break;
	idx = 0; sind = 0; indent = 0;
	firsttok=true;
	if (!IsEmptyLine())
	{    
	  CalculateIndentation();
	  state = Rest;
	}    
	break;
      }
      /*}}}*/
      /*{{{  case Rest:*/
      case Rest:
      {
	/* do we have some indentation to adjust for ... */
	if (curind > indent)
	{    
	  curind--;
	  tok = END;
	  break;
	}    
	else if (curind < indent)
	{    
	  curind++;
	  tok = BEGIN;
	  break;
	}    
      
	while ((line[idx] == ' ' || line[idx] == '\n') && idx < llen)
	{    
	  sind++;
	  idx++;
	}    
      
	if (idx<llen)
	{    
	  char c = line[idx];
      
	  /*{{{  if (isdigit(c))*/
	  if (isdigit(c))
	  {
	    /* gobble up digits */
	    idx++;
	    while (isdigit(line[idx]))
	      idx++;
	    if (line[idx] == '.')
	    {  
	      idx++;
	      while (isdigit(line[idx]) || line[idx] == 'E' || ((line[idx] == '-' || line[idx] == '+') && line[idx - 1] == 'E'))
		idx++;
	     tok = RNUMBER;
	    }  
	    else
	      tok = NUMBER;
	    break;
	  }
	  /*}}}*/
	  /*{{{  else if (isalpha(c))*/
	  else if (isalpha(c))
	  {
	    int wlen = 1;
	    bool has_ = false;
	   
	    idx++;
	    /* gobble up associated chs */
	    while (isalpha(line[idx])
		|| isdigit(line[idx])
		|| line[idx] == '.'
		|| line[idx] == '_'
		|| line[idx] == '%')
	    {  
	      wlen++;
	      has_ |= (line[idx] == '_');
	      idx++;
	      
	    }
	    
	    /* now check against reserved word list */
	    if (isupper(c)) tok = LUKeyword(&line[idx - wlen], wlen);
	    if (tok==ASM || tok==GUY)
	    {
	      UpdatePosition();
	      Message("Ignoring assembler directives",xxWarning,Attribute.Position);
	      state = IgnoreIndentedTextEol;
	      tok=SKIP;
	    }
	    else if (tok < 0)
	      tok = ID;
	    break;
	  }
	      
	  /*}}}*/
	  /*{{{  else if (c == '#')*/
	  else if (c == '#')
	  {
	    /*{{{  INCLUDE actions*/
	    if (strncmp(&line[idx + 1], DV_INCLUDE, strlen(DV_INCLUDE)) == 0)
	    {
	      int s;
	      
	      while ((line[idx++] != '\"') && (idx < llen));
	    
	      s = idx;
	    
	      while ((line[++idx] != '\"') && (idx < llen));
	    
	      if (idx >= llen)
		Message("Expected quoted filename", xxError, Attribute.Position);
	      else 
		if (!EnterInclude(PutString(&line[s], idx - s),ExtnModes))
		  Message("Could not open #INCLUDE file ", xxFatal, Attribute.Position);
		   
	      state = Start;
	      break;
	    }
	    
	    /*}}}*/
	    /*{{{  USE*/
	    if (strncmp(&line[idx + 1], DV_USE, strlen(DV_USE)) == 0)
	    {
	      int s;
	      
	      while ((line[idx++] != '\"') && (idx < llen));
	    
	      s = idx;
	    
	      while ((line[++idx] != '\"') && (idx < llen));
	    
	      if (idx >= llen)
		Message("Expected quoted filename",xxError, Attribute.Position);
	      else
		/*{{{  open file*/
		{
		  char module_string[] = ".t";
		  int  size_module_string = sizeof(module_string)-1;
		  char library_string[] = ".lib";
		  int  size_library_string = sizeof(library_string)-1;
		  int  dot, size;
		  bool found = false;
		
		  for (dot=idx-1;dot>=s;dot--)
		    if (line[dot] == '.')
		      break ;
		  if (dot < s) dot = idx;
		
		  size=dot-s;
		
		  if (size_library_string==(idx-dot) && strncmp(&line[dot],library_string,size_library_string)==0)
		    /*{{{  attempt to open .lib file*/
		    found = EnterInclude(PutString(&line[s], idx - s),ExtnModes|BIT_Mode(xxPrototype));
		    /*}}}*/
		
		  if (!found)
		  {
		    if (size_module_string ==(idx-dot) && strncmp(&line[dot],module_string,size_module_string)==0)
		      /*{{{  attempt to open .t file*/
		      found = EnterInclude(PutString(&line[s], idx - s),ExtnModes|BIT_Mode(xxPrototype));
		      /*}}}*/
		    else
		      /*{{{  convert extension to .t and attempt to open*/
		      {
			char *tmp = malloc (size+size_module_string+1);
			if (tmp == NULL) Message("Out of heap",xxError,Attribute.Position);
			strncpy (tmp,&line[s],size);
			strcpy  (tmp+size,module_string);
			found = EnterInclude(PutString(tmp, size+size_module_string),ExtnModes|BIT_Mode(xxPrototype));
			free(tmp);
		      }
		      /*}}}*/
		  }
		
		  if (!found)
		    Message("Could not open #USE file ", xxError, Attribute.Position);
		}
		/*}}}*/
		   
	      state = Start;
	      break;
	    }
	    /*}}}*/
	    if (strncmp(&line[idx + 1], DV_COMMENT, strlen(DV_COMMENT)) == 0)
	    {
	      UpdatePosition();
	      state = Start;
	       break;
	    }
	    if (strncmp(&line[idx + 1], DV_PRAGMA, strlen(DV_PRAGMA)) == 0   ||
		strncmp(&line[idx + 1], DV_OPTION, strlen(DV_OPTION)) == 0   )
		{
		  UpdatePosition();
		  Message("Ignored Compiler Directive",xxWarning,Attribute.Position);
		  state = Start;
		  break;
		}
	    if (firsttok && strncmp(&line[idx + 1], DV_C,strlen(DV_C)) == 0)
	      /*{{{  #C directive*/
	      {
		if (IS_Mode(xxCDirectives))
		{    
		  tok = HASHC;
		  idx++;
		  idx += strlen(DV_C);
		  state=DoCStr;
		  break;
		}    
		else 
		{    
		  UpdatePosition();
		  Message("In mode which ignores C compiler directives",xxWarning,Attribute.Position);
		  tok= SKIP;
		  state=DoEol;
		  break;
		}    
	      }
	      /*}}}*/
	    else if (firsttok && strncmp(&line[idx + 1], DV_H,strlen(DV_H)) == 0)
	      /*{{{  #H directive*/
	      {
		if (IS_Mode(xxCDirectives))
		{    
		  tok = HASHH;
		  idx++;
		  idx += strlen(DV_H);
		  state=DoCStr;
		  break;
		}    
		else 
		{    
		  UpdatePosition();
		  Message("In mode which ignores C compiler directives",xxWarning,Attribute.Position);
		  tok= -1;
		  state=Start;
		  break;
		}    
	      }
	      /*}}}*/
	    if (isxdigit(line[idx + 1]))
	    {
	      /* gobble up hex digits */
	      idx++;
	      while (isxdigit(line[idx])) idx++;
	      
	      tok = HEXNUMBER;
	    }
	    else
	      tok = -1;
	  }
	  
	  /*}}}*/
	  /*{{{  else if (c == '\n')*/
	  else if (c == '\n')
	  {
	    break;
	  }
	  /*}}}*/
	  /*{{{  else if (c == '\'')*/
	  else if (c == '\'')
	  {
	    if (line[idx + 1] == '*')
	    {
	      if (line[idx + 2] == '#')
	      {
		if (isxdigit(line[idx + 3]) && isxdigit(line[idx + 4]) && line[idx + 5] == '\'')
		{
		  idx += 6;
		  tok = CHCON;
		}
	      }
	      else if (line[idx + 3] == '\'')
	      {
		idx += 4;
		tok = CHCON;
	      }
	    }
	    else if (line[idx + 2] == '\'')
	    {
	      idx += 3;
	      tok = CHCON;
	    }
	  
	    if (tok != CHCON)
	    {
	      int lidx = idx + 1;
	  
	      Message("Badly formed character constant",xxError,Attribute.Position);
	  
	      while (line[lidx] != '\'' && lidx <= llen)
		lidx++;
	  
	      if (line[lidx] == '\'')
	      {
		idx = lidx + 1;
		tok = CHCON ;
	      }
	      else
	      {
		Message("Unterminated character constant, skipping rest of line",xxError,Attribute.Position);
		idx = lidx+1 ;
	      }
	    }
	    
	    break;
	  }
	  
	  /*}}}*/
	  /*{{{  else if (c == '"')*/
	  else if (c == '"')
	  {
	    int lidx = idx + 1;
	  
	    while (true)
	    {
	      if (line[lidx]=='*' && line[lidx+1]=='*')
		lidx+=2;
	      else if (line[lidx]=='*' && line[lidx+1]=='"')
		lidx+=2;
	      else if (line[lidx]=='*' && line[lidx+1]=='\n')
	      {
		lidx+=2;
		while (line[lidx]==' ') lidx++;
		if (line[lidx++]!='*') Message("Multiline string missing continuation character",xxError,Attribute.Position);
	      }
	      else if (line[lidx]!='"' && lidx <= llen)
		lidx++;
	      else
		break;
	    }
	    
	    if (line[lidx] == '"')
	    {
	      idx = lidx + 1;
	      tok = STR;
	    }
	    else
	    {
	      Message("Unterminated string constant, skipping rest of line",xxError,Attribute.Position);
	      idx = lidx+1 ;
	      tok = STR;
	      state = DoEol;
	    }
	  
	    break;
	  }
	  
	  /*}}}*/
	  /*{{{  else*/
	  else
	  {
	    struct KeyList *kp=&PuncList[0];
	  
	    while(kp && kp->Name)
	    {
	      register char *key,*text;
	      register int count=0;
	      
	      key = kp->Name;
	      text = &line[idx];
	  
	      while(*key && *key==*text++) { key++; count++; }
	      if (*key == 0)
	      {
		tok = kp->Val;
		idx += count;
		break;
	      }
	      kp++;
	    }
	  }
	  /*}}}*/
	}    
	else 
	{    
	  tok = EOL;
	  state = Start;
	}    
	/*{{{  if (tok==-1)*/
	if (tok==-1)
	{
	  Message("Invalid character", xxError, Attribute.Position);
	  idx++;
	}
	/*}}}*/
       break;
      }
      /*}}}*/
      /*{{{  case Eof:*/
      case Eof:
      {
	/* do we have some indentation to adjust for ... */
	if (curind > 0)
	{    
	  curind--;
	  tok = END;
	}    
	else if (inclevel > 0)
	{    
	  CloseInclude(file[inclevel].ptr);
	  inclevel--;
	  curind = file[inclevel].indent;
	  indent = file[inclevel].indent;
	  yylineno = file[inclevel].line; nolines = 0;
	  infile = file[inclevel].ptr;
	  ExtnModes = file[inclevel].mode;
	  state = Start;
	}    
	else 
	  tok = 0;
	     
	break;
      }
      /*}}}*/
      /*{{{  case DoEol:*/
      case DoEol:
	state = Start;
	tok = EOL;
	break;
      /*}}}*/
      case DoStr:
	sind = idx;
	while (line[idx]!='\0' && line[idx]!='\n') idx++;
	idx++;
	tok = STR;
	state = DoEol;
	break;
      case DoCStr:
	sind = idx;
	while (line[idx]!='\0' && line[idx]!='\n') idx++;
	idx++;
	tok = CSTR;
	state = DoEol;
	break;
      case IgnoreIndentedTextEol:
	state = IgnoreIndentedText;
	tok = EOL;
	break;
      case IgnoreIndentedText:
	yylineno+=nolines;
	GetLine();
	if (state == Eof)
	  break;
	idx = 0; sind = 0; indent = 0;
	if (!IsEmptyLine())
	{
	  CalculateIndentation();
	  if (indent<=curind)
	    state=Rest;
	}            
	break;
    }
    UpdatePosition();
  }
  /*{{{  setup yytext yylen*/
  if (idx > sind)
  {
    int i;
  
    if (tok!=STR)
    {
      yylen = idx - sind;
      for (i = 0; i < yylen; i++)
      {
	yytext[i] = line[sind + i];
      }
    }
    else
    {
      yylen=0;
      for (i = 0; i < idx-sind; i++)
      {
	if (line[sind+i]=='*' && line[sind+i+1]=='\n')
	{
	  i++;
	  while (line[sind+i]==' ' || line[sind+i]=='\n') i++;
	  if (line[sind+i]!='*') Message("Expected continuation * in string constant",xxError,Attribute.Position); 
	}
	else
	  yytext[yylen++] = line[sind+i];
      }
    }  
    yytext[yylen] = '\0';
  }
  else
  {
    yylen = 0;
    yytext[0] = '\0';
  }
  /*}}}*/

  /*{{{  setup token attributes*/
  switch (tok)
  {
  case LSB : between_brackets = IsBetweenBrackets(); break;
  case RSB : between_brackets = false; break;
  case COMMA :
    /*{{{  */
    if (colon_ended && (!between_brackets)) tok = DECLCOMMA;
    break;
    /*}}}*/
  case STR:
    /*{{{  */
    {
      char *temp,*t ;
    
      if ((temp=malloc((yylen==0)?1:yylen+1)) == NULL)
	Message("Out of heap",xxError,Attribute.Position);
      else
      {
	int i = 1 ;
	t=temp;
	   
	while (i<yylen-1)
	{  
	  if (yytext[i] == '*' && (state!=DoEol))
	    i += MapEscapedCharacter (&yytext[i],yylen-1-i,t) ;
	  else
	    *t = yytext[i++] ;
	  t++ ;
	}  
	   
	Attribute.Value.String = PutString(temp, t-temp);
	   
	free(temp) ;
      }
    }
      
    break;
    /*}}}*/
  case CSTR:
    /*{{{  */
    {
      char *temp,*t ;
    
      if ((temp=malloc((yylen==0)?1:yylen+1)) == NULL)
	Message("Out of heap",xxError,Attribute.Position);
      else
      {
	int i = 1 ;
	t=temp;
	   
	while (i<yylen-1)
	{  
	  *t++ = yytext[i++] ;
	}  
	   
	Attribute.Value.String = PutString(temp, t-temp);
	   
	free(temp) ;
      }
    }
      
    break;
    /*}}}*/
  case HEXNUMBER:
    if (IS_Mode(xxSupportINT64))
    {
  #ifdef SUPPORT_INT64
      tInt val = 0;
      int i = 1 ;
	 
      while (i<yylen)
      {
	val = (val << 4) + (isdigit(yytext[i]) ? (yytext[i++]-'0') : (toupper(yytext[i++]) - 'A' + 10));
      }
      if (yylen > 17) Message("Hex constant too large",xxError,Attribute.Position);
      Attribute.Value.Number = val;
  #else
      Message("Internal Error - Scanner - INT64",xxFatal,NoPosition);
  #endif
    }
    else
      sscanf(&yytext[1], "%X", (unsigned int *)&Attribute.Value.Number);
    
    break;
  
  case NUMBER:
    if (IS_Mode(xxSupportINT64))
    {
  #ifdef SUPPORT_INT64
      tInt val = 0;
      int i = 0;
      tInt max = 0x7fffffffffffffffLL;
	 
      while (i<yylen)
      {
	if (val >= max/10) Message("INT constant too large",xxError,Attribute.Position);
	val = (val * 10) + (yytext[i++] - '0');
      }
      Attribute.Value.Number = val;
  #else 
      Message("Internal Error - Scanner - INT64",xxFatal,NoPosition);
  #endif
    }
    else
      Attribute.Value.Number = atoi(yytext);
    
    break;
    
  case RNUMBER:
    /*{{{  */
    Attribute.Value.RNumber.n = atof(yytext);
    Attribute.Value.RNumber.p = yylen - 2;
    break;
    /*}}}*/
  case CHCON:
    /*{{{  */
    if (yylen == 3)
      Attribute.Value.Char = yytext[1];
    else
      MapEscapedCharacter (&yytext[1],yylen-2,&Attribute.Value.Char) ;
    break ;
    /*}}}*/
  case ID:
    /*{{{  */
    {
      tSymbol S;
      Attribute.Value.Ident = MakeIdent(yytext, yylen);
      S = LookupSymbolWithNoFail(Attribute.Value.Ident);
      if (Tree_IsType(SymbolKind(S),kDeclType)) tok = TYPEID;
    }
    break;
    /*}}}*/
  }
  /*}}}*/

  if (tok!=BEGIN && tok!=END) firsttok = false;
  lasttok = tok;
/*{{{  DEBUG token */
#ifdef YYDEBUG
if (ldebug)
{
  fprintf(stderr, "DEBUG: at %2d,%2d (VD:I=%d:%d,L=%d,CE=%c) token is %3d ",
     Attribute.Position.Line, Attribute.Position.Column, valofi, valofs[valofi],llen,(colon_ended)?'T':'F',tok);
  switch (tok)
  {
  case BEGIN:
    fprintf(stderr, "BEGIN\n");
    break;
  case END:
    fprintf(stderr, "END\n");
    break;
  case EOL:
    fprintf(stderr, "EOL\n");
    break;
  case ID:
    fprintf(stderr, "ID:<%s>\n", yytext);
    break;
  case NUMBER:
    fprintf(stderr,"NUMBER:<%s>\n",yytext);
    break;
  case RNUMBER:
    fprintf(stderr,"RNUMBER:<%s>\n",yytext);
    break;
  case CHCON:
    fprintf(stderr,"CHCON:<%s>\n",yytext);
    break;
  case STR:
    fprintf(stderr,"STR:<%s>\n",yytext);
    break;
  case TYPEID:
    fprintf(stderr,"TYPEID:<%s>\n",yytext);
    break;
  default:
    fprintf(stderr, "<%s>\n", yytext);
    break;
  }
}
#endif
/*}}}*/
  return (tok);
}

/*}}}*/
