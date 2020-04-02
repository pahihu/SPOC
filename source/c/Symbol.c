/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title :  Symbol Table Module
||@(#)   System : Occam 2 C
||@(#) Filename : Symbol.c
||@(#)  Version : 1.8
||@(#)     Date : 2/18/94
\*@(#)====================================================*/
/*}}}*/

/*{{{  includes static and defines*/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "DynArray.h"
#include "Tree.h"
#include "OccamOut.h"

/* #define DEBUG */

static uint32_t SymExtent = 100;
static uint32_t ScopeExtent = 20;

int SymSize = -1;
int OccSymSize=0;

tSymEntry* SymTable = NULL;
static tSymbol* Scope = NULL; 

static tSymEntry NullSymbol= NULLSYMBOL; 
static int UniqueCount[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int Depth=-1;
static bool LUfail=false;

#define  Fatal(s) Message(s,xxFatal,Attribute.Position)
/*}}}*/

/*{{{  void InitSymbols()*/
void InitSymbols()
{
  MakeArray((char**)&SymTable,&SymExtent,sizeof(tSymEntry));
  MakeArray((char**)&Scope,&ScopeExtent,sizeof(tSymbol));

  ResetUniqueIds();
  SymSize=0;
  SymTable[SymSize] = NullSymbol;
  SymTable[SymSize].Id = NoIdent;
  SymTable[SymSize].Depth = Depth;
}
/*}}}*/
/*{{{  void EndSymbols()*/
void EndSymbols()
{
  ReleaseArray((char**)&SymTable,&SymExtent,sizeof(tSymEntry));
  ReleaseArray((char**)&Scope,&ScopeExtent,sizeof(tSymbol));
}
/*}}}*/

/*{{{  static tIdent OccIdent2C(tIdent I)*/
static tIdent OccIdent2C(tIdent I)
{
  if (I!=NoIdent)
  {
    tStringRef  sr = GetStringRef(I);
    int len = LengthSt(sr);
    char *temp;
   
    if ((temp = (char*) malloc(len+1)) == NULL)
      Fatal("Out of heap") ;
    else
    {
      int i;
      tIdent CId;

      StGetString (sr,temp);
      for(i=0;i<len;i++)
	if (temp[i]=='.') temp[i]='_';
      CId = MakeIdent(temp,len);  
      free(temp);
      return(CId);
    }
  }
  return (NoIdent);
}
/*}}}*/
/*{{{  tSymbol AddSymbol(tIdent Id)*/
tSymbol AddSymbol(tIdent Id)
{
#ifdef DEBUG
  { int i; for (i=0;i<Depth;i++) printf("  "); printf("ADDSYMBOL "); WriteIdent(stdout,Id); printf("\n");}
#endif

  if (SymSize<0) Fatal("Symbol Table Not Initialised");
  if (Depth<0)   Fatal("Invalid Scope Depth");

  SymSize++;
  if (SymSize==SymExtent) ExtendArray((char**)&SymTable,&SymExtent,sizeof(tSymEntry));
 
  SymTable[SymSize] = NullSymbol;
  SymTable[SymSize].Id = Id;
  if ((!LUfail) && Id!=NoIdent) SymTable[SymSize].CId = OccIdent2C(Id);
  else SymTable[SymSize].CId = NoIdent;
  
  SymTable[SymSize].Scope = Scope[Depth];
  SymTable[SymSize].Depth = Depth;
  SymTable[SymSize].Out = SymSize;
  SymTable[SymSize].Usage = 0;
  
  Scope[Depth]=SymSize;
  LUfail=false;
  return(SymSize);
}
/*}}}*/
/*{{{  tSymbol TempSymbol(int Depth)*/
tSymbol TempSymbol(int Depth)
{
  SymSize++;
  if (SymSize==SymExtent) ExtendArray((char**)&SymTable,&SymExtent,sizeof(tSymEntry));
 
  SymTable[SymSize] = NullSymbol;
  SymTable[SymSize].Id = NoIdent;
  SymTable[SymSize].CId = NoIdent;
  SymTable[SymSize].Scope = 0;
  SymTable[SymSize].Depth = Depth;
  SymTable[SymSize].Usage = 0;
  return(SymSize);
}
/*}}}*/
/*{{{  tSymbol AddChanSymbol(tIdent Id)*/
tSymbol AddChanSymbol(tIdent Id)
{
  tSymbol Sout=AddSymbol(Id);
  tSymbol S=AddSymbol(Id);
  Symbol(S).Out = Sout;
  return (S);
}
/*}}}*/
/*{{{  tIdent UniqueId(char type)*/
tIdent UniqueId(char type)
{
  char str[16];
  int letter = toupper(type) - 'A';

  if (letter < 0 || letter > 26) return 0;
  
  sprintf(str,"_%c%d",type,UniqueCount[letter]++);
  return MakeIdent(str,strlen(str));
}  
/*}}}*/
/*{{{  tIdent PrefixId(tIdent pfid,tIdent id,char sep)*/
tIdent PrefixId(tIdent pfid,tIdent id,char sep)
{
  char str[256];
  int l = 0;

  if (pfid != NoIdent)
  {
    GetString(pfid,&str[l]);
    l += strlen(str);
    if (sep!=0 && id!=0 && id!=NoIdent) str[l++] = sep;
  }
  if (id!=0 && id!=NoIdent) GetString(id,&str[l]);
  return MakeIdent(str,strlen(str));
}
/*}}}*/
/*{{{  void ResetUniqueIds()*/
void ResetUniqueIds()
{
  int i;

  for(i = 0; i < 26; ++i)
    UniqueCount[i] = 0;
}    
  
/*}}}*/

/*{{{  int CountSameNames(tSymbol s)*/
int CountSameNames(tSymbol s)
{
  tIdent Id = SymTable[s].Id;
  int count = 0;
  tSymbol i;
  for (i=0;i<s;i++) 
  {
    if (SymTable[i].Id == Id) count++;  
  }
  return count++;
}
/*}}}*/
/*{{{  tSymbol LookupSymbolWithNoFail(tIdent Id)*/
tSymbol LookupSymbolWithNoFail(tIdent Id)
{
  tSymbol s;
  if (Id==NoIdent) return NoSymbol;
  if (SymSize<0) Fatal("Symbol Table Not Initialised");
  
  s = Scope[Depth];
  while (s!=0)
   if (Symbol(s).Id==Id)
    {
      return(s);
    }
    else
      s=Symbol(s).Scope;
  return NoSymbol;  
}
/*}}}*/
/*{{{  tSymbol LookupSymbol(tScanAttribute Scan)*/
tSymbol LookupSymbol(tScanAttribute Scan)
{
  tSymbol s;
  tIdent Id = Scan.Value.Ident;

  if (Id==NoIdent) return NoSymbol;
#ifdef DEBUG
  { int i; for (i=0;i<Depth;i++) printf("  "); printf("LOOKUP "); WriteIdent(stdout,Id); printf("\n");}
#endif
  
  if (SymSize<0) Fatal("Symbol Table Not Initialised");
  
  s = Scope[Depth];
  while (s!=0)
   if (Symbol(s).Id==Id)
    {
      return(s);
    }
    else
      s=Symbol(s).Scope;
  LUfail=true;
  Message("Could not find identifier",xxError,Scan.Position);
  return AddSymbol(Id);
}
/*}}}*/
/*{{{  tSymbol LookupSymbolInScope(tIdent Id, tPosition Pos, tSymbol scope)*/
tSymbol LookupSymbolInScope(tIdent Id, tPosition Pos, tSymbol scope)
{
  tSymbol s;

  if (Id==NoIdent) return NoSymbol;
#ifdef DEBUG
  { int i; for (i=0;i<Depth;i++) printf("  "); printf("LOOKUP "); WriteIdent(stdout,Id); printf("\n");}
#endif
  
  if (SymSize<0) Fatal("Symbol Table Not Initialised");
  if (scope>SymSize) Fatal("Attempted symbol lookup with invalid scope");
  s = scope;
  while (s!=0)
   if (Symbol(s).Id==Id)
    {
      return(s);
    }
    else
      s=Symbol(s).Scope;
  LUfail=true;
  Message("Could not find identifier",xxError,Pos);
  /* Make up a symbol to get valid symtable table entry */
  OpenScope();
  s = AddSymbol(Id);
  CloseScope();
  return s;
}
/*}}}*/
/*{{{  void WriteSymTable(bool ShowScope)*/
void WriteSymTable()
{
  int index;
  
  printf("\n\n");
  printf("|Entry|Scope|Depth|Out|Usage|Kind         |Type        Identifier         C-Identifier \n");
  printf("+-----+-----+-----+---+-----+-------------+-----------+------------------+-----------------\n");

  for (index=1;index<=SymSize;index++)
  {
    int pos;
    
    printf("|%5d|%5d|%5d|%3d|",index,
       SymTable[index].Scope,
       SymTable[index].Depth,
       SymTable[index].Out);
    
    if (SymTable[index].Kind == NoTree) printf("     |No Kind      |");
    else
    {
      putchar((SymTable[index].Usage & xxAtomic)?'A':' ');
      putchar((SymTable[index].Usage & xxLRead)?'r':' ');
      putchar((SymTable[index].Usage & xxLWrite)?'w':' ');
      putchar((SymTable[index].Usage & xxNLRead)?'R':' ');
      putchar((SymTable[index].Usage & xxNLWrite)?'W':' ');
      printf("|%-13s|",Tree_NodeName[SymTable[index].Kind->Kind] );
    }

    CodeType(SymTable[index].Type);

    printf(" \t");

    WriteIdent(stdout,SymTable[index].Id);    
    pos = (SymTable[index].Id != 0) ? LengthSt(GetStringRef(SymTable[index].Id)) : 0;
    while (pos++ < 20) putchar(' '); 
    putchar(' ');
    WriteIdent(stdout,SymTable[index].CId);    
    pos = (SymTable[index].CId != 0) ?  LengthSt(GetStringRef(SymTable[index].CId)) : 0;
    printf("\n");
  }
  printf("%d Entries in Symbol Table %s\n\n",SymSize,(Depth!=-1)?": ERROR Open/Close Mismatch":"");
}
/*}}}*/
/*{{{  tSymbol CurrentScope()*/
tSymbol CurrentScope()
{
  return Scope[Depth];
}
/*}}}*/
/*{{{  void OpenAtScope(tSymbol s)*/
void OpenAtScope(tSymbol s)
{
  OpenScope();
  Scope[Depth] = s;
}  
/*}}}*/
/*{{{  void OpenScope()*/
void OpenScope()
{
#ifdef DEBUG
  { int i; for (i=0;i<Depth;i++) printf("  "); printf("OPEN\n"); }
#endif
  Depth++;
  if (SymSize<0) Fatal("Symbol Table Not Initialised");
  if (Depth>=ScopeExtent) ExtendArray((char**)&Scope,&ScopeExtent,sizeof(tSymbol));
  Scope[Depth] = (Depth==0) ? 0 : Scope[Depth-1];
}  
/*}}}*/
/*{{{  void CloseScope()*/
void CloseScope()
{
  Depth--;
#ifdef DEBUG
  { int i; for (i=0;i<Depth;i++) printf("  "); printf("CLOSE\n"); }
#endif
}
/*}}}*/

/*{{{  TEST module*/
#ifdef TEST

#define ADD(x)    printf("ADD    %4s %4d\n",x,AddSymbol(MakeIdent(x,strlen(x))))
#define LOOKUP(x) printf("LOOKUP %4s %4d\n",x,LookupSymbol(MakeIdent(x,strlen(x))))
#define OPEN()   printf("OPEN\n");OpenScope()
#define CLOSE()   printf("CLOSE\n");CloseScope()

tScanAttribute  Attribute;
int main()
{
  InitIdents();
  InitSymbols();
  OPEN();
    ADD("b");
    ADD("a");
    ADD("a");
      OPEN();
	LOOKUP("a");
	ADD("a");
	LOOKUP("b");
	LOOKUP("c");
	LOOKUP("f");
	OPEN();
	  LOOKUP("a");
	  ADD("a");
	  LOOKUP("b");
	  OPEN();
	    LOOKUP("a");
	    ADD("a");
	    LOOKUP("b");
	  CLOSE();
	CLOSE();
      CLOSE();
      OPEN();
	LOOKUP("a");
	ADD("a");
	LOOKUP("b");
      CLOSE();
    LOOKUP("a");
    ADD("a");
  CLOSE();
  WriteSymTable();
}

#endif
/*}}}*/

