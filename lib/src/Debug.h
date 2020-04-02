/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Debug header file for generated C
||@(#)   System : Occam 2 C
||@(#) Filename : Debug.h
||@(#)  Version : 1.1
||@(#)     Date : 12/10/93
\*@(#)====================================================*/
/*}}}*/

extern void DEBUGDONE(void);
#define SETSCOPE(S) FP->_Header.Scope=S
#include <string.h>
#include <ctype.h>
static char *ID=NULL;
static void *CFP=NULL;
static int DEPTH=0;
typedef enum { oid, oall, oframe, olistsubs, onomode } tShowMode;
typedef struct { tHeader _Header; void *Chain; } tFrame;

typedef enum { kNoKind,kFormal,kProcedure,kFunction,kVariable,kVal,kValRetypes,kIs,kRetypes,kReplicator,kExternal} tKinds;
static char *KindNames[] = { "","Formal","FUNCTION","PROC","Variable","VAL","VAL RETYPES","IS","RETYPES","Replicator","External" };

static MODE=onomode;

static ShowName(char *K,char *T, char *N)
{
  int w;
  w=printf("%10s %s ",K,T);
  printf("%*s = ",((50-w)<0)?0:(50-w),N);
}
static Show(tShowMode M, void *FP)
{
  static ShowValue(void *FP, int Symbol);
  if (FP==NULL)
    fprintf(stderr,"Occam frame pointer is NULL\n");
  else
    switch(M)
    {
      case oid:
        /*{{{  show identifier specified in ID static*/
        {
          int Scope=((tFrame*)FP)->_Header.Scope;
          int Symbol=Scope;
          if (ID==NULL)
            printf("Error- first select identifier <name> by 'set ID=\"<name>\"' command\n");
          else
          {
            int found=false;
            printf("IDENTIFIER SET {%s} SEARCH:\n\n",ID);
            while (Symbol>=0 && Symbol<NUMSYMS)
            {
              char *s,*e;
              s = strstr(ID,Symbols[Symbol]);
              e = s+strlen(Symbols[Symbol]);
              if (s!=NULL && ((s==ID)||(*(s-1)==' ')) && ((*e=='\0')||(*e==' ')))
              {
                tFrame *SFP=FP;
                int Depth=Depths[Scope];
                while (Depths[Symbol]<Depth && SFP!=NULL) { SFP=SFP->Chain; Depth--;}
                if (Kinds[Symbol]!=0)
                {
                  found=true;
                  ShowName(KindNames[Kinds[Symbol]],Types[Symbol],Symbols[Symbol]);
                  ShowValue(SFP,Symbol);
                  printf("\n");
                }
              }  
              Symbol=Scopes[Symbol];
            }
            if (!found) printf("Error- Could not find any member of identifier set {%s} at scope %d\n",ID,Scope);
            printf("\n");
          }
          break;
        }
        /*}}}*/
      case oall:
        /*{{{  show all symbols currently in scope*/
        {
          int Scope=((tFrame*)FP)->_Header.Scope;
          int Symbol=Scope;
          tFrame *SFP=FP;
          int Depth=Depths[Scope];
          printf("SYMBOLS in SCOPE at scoping level %d\n\n",Scope);
          while (Symbol>=0 && Symbol<NUMSYMS && Kinds[Symbol]!=kNoKind)
          {
            while (Depths[Symbol]<Depth && SFP!=NULL) { SFP=SFP->Chain; Depth--;}
            if (SFP==NULL)
              printf("Error- Inconsistent frame and scope\n");
            else
              {
                ShowName(KindNames[Kinds[Symbol]],Types[Symbol],Symbols[Symbol]);
                ShowValue(SFP,Symbol);
                printf("\n");
              }
            Symbol=Scopes[Symbol];                              
          }
          printf("\n");
          break;
        }
        /*}}}*/
      case oframe:
        /*{{{  show all symbols in scope within selected frame*/
        {
          int Scope=((tFrame*)FP)->_Header.Scope;
          int Symbol=Scope;
          int Depth;
        
          if (Scope<0 || Scope>NUMSYMS) { printf("Error- Invalid scope (%d) \n",Scope); break; }
          if (DEPTH>Depths[Scope]) DEPTH=Depths[Scope];
          if (DEPTH<0) DEPTH=0;
          Depth=Depths[Scope]-DEPTH;
          
          printf("SYMBOLS in SCOPE at frame %d\n\n",Depth);
          while (Symbol>=0 && Symbol<NUMSYMS && Depth<Depths[Symbol]) Symbol=Scopes[Symbol];
          while (Symbol>=0 && Symbol<NUMSYMS && Depth==Depths[Symbol] && Kinds[Symbol]!=kNoKind)
          {
            ShowName(KindNames[Kinds[Symbol]],Types[Symbol],Symbols[Symbol]);
            ShowValue(FP,Symbol);
            Symbol=Scopes[Symbol];                              
            printf("\n");
          }
          printf("\n");
          break;
        }
        /*}}}*/
      case olistsubs:
        /*{{{  list c identifiers for all subroutines*/
        {
          int Symbol;
          printf("C IDENTIFIERS FOR occam SUBROUTINES:\n\n");
          for (Symbol=NUMSYMS-1;Symbol>=0;Symbol--)
          {
            if ((Kinds[Symbol]==kFunction || Kinds[Symbol]==kProcedure || Kinds[Symbol]==kExternal)&& Symbols[Symbol][0]!='\0')
            {
              int w=printf("%*c %s %s",(Depths[Symbol]+1)*2,' ',KindNames[Kinds[Symbol]],Symbols[Symbol]);
              printf("%-*c = ",(w>35)?0:(35-w),' ');
              ShowValue(NULL,Symbol);
              putchar('\n');
            }
          }
          putchar('\n');
        }
        /*}}}*/
      default:
    }  
} 

/*{{{  ShowValues*/
#if (BYTES_PER_WORD==8)
  #define ShowINT(v) ShowINT64(v)
  #define ShowArrayINT(p,l) ShowArrayINT64(p,l)
#elif (BYTES_PER_WORD==4)
  #define ShowINT(v) ShowINT32(v)
  #define ShowArrayINT(p,l) ShowArrayINT32(p,l)
#elif (BYTES_PER_WORD==2)
  #define ShowArrayINT(p,l) ShowArrayINT16(p,l)
#else
  #error Compilation Error- Unsupported word length
#endif

#define LP putchar('[')
#define RP putchar(']')
#define SMALLCNT 20
#ifndef NO_INT64
#define ShowINT64(v) printf("%ld",v);
/*{{{  static ShowArrayINT64(INT64 *p,int size)*/
static ShowArrayINT64(INT64 *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowINT64(*p);
    for (i=1;i<size;i++)
    {
      INT64 *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowINT64(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowINT64(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowINT64(*p);
    }
  }
  RP;
}
/*}}}*/
#endif
#define ShowINT32(v) printf("%d",v)          
/*{{{  static ShowArrayINT32(INT32 *p,int size)*/
static ShowArrayINT32(INT32 *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowINT32(*p);
    for (i=1;i<size;i++)
    {
      INT32 *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowINT32(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowINT32(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowINT32(*p);
    }
  }
  RP;
}
/*}}}*/
#define ShowINT16(v) printf("%d",v)
/*{{{  static ShowArrayINT16(INT16 *p,int size)*/
static ShowArrayINT16(INT16 *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowINT16(*p);
    for (i=1;i<size;i++)
    {
      INT16 *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowINT16(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowINT16(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowINT16(*p);
    }
  }
  RP;
}
/*}}}*/
#define ShowBOOL(v)  printf("%c",(v)?'T':'F')
/*{{{  static ShowArrayBOOL(BOOL *p,int size)*/
static ShowArrayBOOL(BOOL *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowBOOL(*p);
    for (i=1;i<size;i++)
    {
      BOOL *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowBOOL(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowBOOL(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowBOOL(*p);
    }
  }
  RP;
}
/*}}}*/
#define ShowBYTE(v)  if (isprint(v)) printf("'%c'",v); else printf("'*#%02X'",v)
/*{{{  static ShowArrayBYTE(BYTE *p,int size)*/
static ShowArrayBYTE(BYTE *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowBYTE(*p);
    for (i=1;i<size;i++)
    {
      BYTE *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowBYTE(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowBYTE(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowBYTE(*p);
    }
  }
  RP;
}
/*}}}*/
#define ShowREAL32(v) printf("%G",v); 
/*{{{  static ShowArrayREAL32(REAL32 *p,int size)*/
static ShowArrayREAL32(REAL32 *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowREAL32(*p);
    for (i=1;i<size;i++)
    {
      REAL32 *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowREAL32(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowREAL32(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowREAL32(*p);
    }
  }
  RP;
}
/*}}}*/
#ifndef NO_REAL64
#define ShowREAL64(v) printf("%G",v); 
/*{{{  static ShowArrayREAL64(REAL64 *p,int size)*/
static ShowArrayREAL64(REAL64 *p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowREAL64(*p);
    for (i=1;i<size;i++)
    {
      REAL64 *oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowREAL64(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowREAL64(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowREAL64(*p);
    }
  }
  RP;
}
/*}}}*/
#endif
/*{{{  static void ShowCHAN(CHAN *c)*/
static void ShowCHAN(CHAN *c)
{
  if (c==NULL || c->Task==NoTask)
    printf("Idle");
  else
    switch(c->Task->AltMode)
    {
      case NotAlting_p: printf("Active");break;
      case Enabling_p: printf("Enabled");break;
      case Waiting_p: printf("Waiting");break;
      case Ready_p: printf("Ready");break;
      default: printf("Invalid");break;
    }
}
/*}}}*/
/*{{{  static ShowArrayCHAN(CHAN **p,int size) */
static ShowArrayCHAN(CHAN **p,int size) 
{
  int cnt=1;
  int i;
  LP;
  if (size>0 && p!=NULL)
  {
    ShowCHAN(*p);
    for (i=1;i<size;i++)
    {
      CHAN **oldp=p;
      if (*p==*++p)
        cnt++;
      else
      {
        if (cnt>SMALLCNT)
          printf( " <repeats %d times>",cnt);
        else if (cnt>1)
        {
          int c;
          for (c=1;c<cnt;c++)
            { putchar(','); ShowCHAN(*oldp); }
        }
        cnt=1;
        putchar(',');
        ShowCHAN(*p);
      }
    }
  }
  if (cnt>SMALLCNT)
    printf( " <repeats %d times>",cnt);
  else if (cnt>1)
  {
    int c;
    for (c=1;c<cnt;c++)
    {
      putchar(',');ShowCHAN(*p);
    }
  }
  RP;
}
/*}}}*/
#define ShowTIMER(v)
#define ShowArrayTIMER(p,l)
/*}}}*/

