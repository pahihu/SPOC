/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||
||@(#)    Title : Occam tree viewer
||@(#)   System : occam 2 c
||@(#) Filename : Occam2C.h
||@(#)  Version : 1.3
||@(#)     Date : 26/3/97  
\*@(#)====================================================*/

/*{{{  includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <setjmp.h>
#include <math.h>

#ifndef __GNUC__
#define inline
#endif
/*}}}*/
/*{{{  architectures and operating systems*/
#define TRANSPUTER 0
#define PC   1
#define VAX  2
#define SUN3 3
#define SUN4 4

#define LITTLE 0
#define BIG 1

#define NONE  0
#define DOS   1
#define VMS   2
#define SUNOS 3

/*}}}*/
/*{{{  endianness*/
#if (HOST == TRANSPUTER || HOST==PC || HOST==VAX)

#if (SOURCE == LITTLE)
#define LITTLE_AND_LITTLE
#else
#define LITTLE_AND_BIG
#endif

#else

#if (SOURCE == LITTLE)
#define BIG_AND_LITTLE
#else
#define BIG_AND_BIG
#endif

#endif
/*}}}*/
/*{{{  maths support*/
#ifdef linux
#define remainder(x,y) drem(x,y)
#endif
#define NaN (1./0.-1./0.)
#define Inf (1./0.)
#define BITS_PER_CHAR (8)
/*}}}*/
 
extern jmp_buf STOPP_env;

/*{{{  typedef tFuncPtr, CHAN, tTask, bool, tTimer*/
typedef const void * (*tFuncPtr)();

typedef struct Task tTask;
#define NoTask (tTask *)0

typedef struct { tTask *Task; void *Data; } CHAN;

#define false 0
#define true (!false)
#define SETSCOPE(S)

typedef long tTimer;
#define AFTER(a,b) ((a-b)>0)
/*}}}*/
/*{{{  task states and priorities*/
#define NUM_PRIORITY_LEVELS 2
typedef enum
{
  HighPriority=0,
  LowPriority=1
} tTaskPri;

typedef enum
{
  OUT_p=1,
  IN_p=2,
  EXEC_p=3,
  WAIT_p=4,
  ALTING_p=5,
  TALTING_p=6,
  TWAIT_p=7,
  TSOCK_p=8
} tTaskMode;


typedef enum
{
  NotAlting_p = -1,
  Enabling_p  = -2,
  Waiting_p   = -3,
  Ready_p     = -4,
  TReady_p    = -5
} tAltMode;

typedef enum
{
  TimeNotSet_p = -2,
  TimeSet_p = -3
} tTimerMode;

/*}}}*/
/*{{{  struct Task*/
struct Task
{
  tTask *Next;      /* Linked List of Active Tasks */
  tTask *Parent;    /* Parent Task */
  
  tFuncPtr Func;    /* Current Function being executed by this task */
  void *FP;         /* Frame Pointer for this task */

  tTaskMode state;
  int Children;     /* Active Children Count */

  char *name;       /* Task Name */

  union
  {
    struct
    {
      long len;         /* Comms */
      void *data;       /* Comms */
      char buff[4];     /* Comms */
    } io;
    struct
    {
      tTimerMode TimerMode;  /* Timer Alternation */
      tTimer time;
    } timer;
  } comms;
      
  tAltMode AltMode;      /* Alternation */
  int TaskId;       /* Number identifying task, for debugging */
  int TaskPriority;
};
/*}}}*/
/*{{{  scheduling queue*/
extern tTask *QFP[NUM_PRIORITY_LEVELS],*QBP[NUM_PRIORITY_LEVELS],*QTM,*CURTASK;
extern tTimer TIMER;

/*}}}*/
/*{{{  runtime error handling*/
static char* MSG_NULL = NULL;
static char* MSG_EFROL = "ERROR FLAG raised on line %d of file %s\n";
static char* MSG_TA = "Terminating application.\n";
static char* MSG_CA = "Continuing application.\n";
static char* MSG_SP = "STOPPing process %s.\n";
static char* MSG_IF = "\nAll IF branches failed.\n";
static char* MSG_CASE = "\nNo CASE option matched.\n";
static char* MSG_CASEIN = "\nInput tag does not match CASE options.\n";
static char* MSG_STOP = "\nExecuted STOP process\n";
static char* MSG_PRI = "\nInvalid priority level\n";
static char* MSG_IP = "\nInvalid Instruction Pointer\n";

#if (defined STOPP_ON_ERROR)
#define SETERR_PROCEED  fprintf(stderr,MSG_SP,ThisTask()->name); longjmp(STOPP_env,0);
#elif (defined CONTINUE_ON_ERROR)
#define SETERR_PROCEED  fprintf(stderr,MSG_CA);
#else
#define SETERR_PROCEED  fprintf(stderr,MSG_TA);exit(-1)
#endif

#define SETERR(M)	seterr(M,__LINE__,__FILE__)

extern void seterr(char *M,int L,char *F);
/*}}}*/

/*{{{  primitive types*/
#undef INT
#undef INT16
#undef INT32
#undef INT64
#undef BYTE
#undef USER
#undef SIZEUSER
#undef REAL32
#undef REAL64
#undef BOOL
#undef TEMPVAL

#define INT   long int
#define INT32 long int

#ifndef NO_INT64
#if (defined __GNUC__) || (defined cham)
#define INT64 long long
#else
#define INT64 long
#endif
#endif

#define INT16 short int
#define BYTE  unsigned char
#define USER  INT64
#define SIZEUSER(s) ((s+sizeof(USER)-1)/sizeof(USER))
#define REAL32 float

#ifndef NO_REAL64
#define REAL64 double
#endif

#define BOOL char
typedef union
{
  BOOL VBOOL;
  INT VINT;
  INT32 VINT32;
  INT16 VINT16;
#ifndef NO_INT64
  INT64 VINT64;
#endif
  BYTE VBYTE;
  REAL32 VREAL32;
#ifndef NO_REAL64
  REAL64 VREAL64;
#endif
} TEMPVAL;

#define MOSTPOSBYTE  ((BYTE)255)
#define MOSTNEGBYTE  ((BYTE)0)
#define MOSTPOSINT16 ((INT16)0x7FFF)
#define MOSTNEGINT16 ((INT16)0x8000)
#define MOSTPOSINT32 ((INT32)0x7FFFFFFF)
#define MOSTNEGINT32 (~MOSTPOSINT32)

#ifndef NO_INT64
#ifdef __GNUC__
#define MOSTPOSINT64 ((INT64)0x7FFFFFFFFFFFFFFFll)
#define MOSTNEGINT64 (~MOSTPOSINT64)
#else
#define MOSTPOSINT64 MOSTPOSINT32
#define MOSTNEGINT64 MOSTNEGINT32
#endif
#endif

#define UINT   unsigned INT
#define UINT16 unsigned INT16
#define UINT32 unsigned INT32
#define UINT64 unsigned INT64
#define UBYTE  BYTE

/*{{{  casting down primitives*/
static void inline FailConvert(int V,int L, char *F)
{
  fprintf(stderr,"Error- Type conversion failed range check (value was %d)\n",V);
  seterr(MSG_NULL,L,F);
}
#ifdef CHECK_CONVERSIONS
#ifdef NO_INT64
#define downtoINT32(v) ((INT32)(v))
#else
#define downtoINT64(v) ((INT64)(v))
#define downtoINT32(v) ((((v)<MOSTNEGINT32)||((v)>MOSTPOSINT32))?(FailConvert((v),__LINE__,__FILE__),((INT32)(v))):((INT32)(v)))
#endif
#define downtoINT16(v) ((((v)<MOSTNEGINT16)||((v)>MOSTPOSINT16))?(FailConvert((v),__LINE__,__FILE__),((INT16)(v))):((INT16)(v)))
#define downtoBYTE(v) (((v)>255)?(FailConvert((v),__LINE__,__FILE__),((BYTE)(v))):((BYTE)(v)))
#define downtoBOOL(v) (((v)>1)?(FailConvert((v),__LINE__,__FILE__),((BOOL)(v))):((BOOL)(v)))
#define downtoREAL32(v) ((REAL32)(v))
#else
#ifndef NO_INT64
#define downtoINT64(v) ((INT64)(v))
#endif
#define downtoINT32(v) ((INT32)(v))
#define downtoINT16(v) ((INT16)(v))
#define downtoBYTE(v) ((BYTE)(v))
#define downtoBOOL(v) ((BOOL)(v))
#define downtoREAL32(v) ((REAL32)(v))
#endif
/*}}}*/
/*{{{  fixed word size integer conversions*/
#ifndef NO_INT64
#define INT64INT64(v) ((INT64) (v))
#define INT64INT32(v) ((INT64) (v))
#define INT64INT16(v) ((INT64) (v))
#define INT64BYTE(v) ((INT64) (v))
#define INT64BOOL(v) ((INT64) ((v)?1:0))
#define INT32INT64(v) downtoINT32(v)
#define INT16INT64(v) downtoINT16(v)
#define BYTEINT64(v) downtoBYTE((UINT64)v)
#define BOOLINT64(v) downtoBOOL((UINT64)v)
#endif

#define INT32INT32(v) (v)
#define INT32INT16(v) ((INT32) (v))
#define INT32BYTE(v) ((INT32) (v))
#define INT32BOOL(v) ((INT32) ((v)?1:0))
#define INT16INT32(v) downtoINT16(v)
#define BYTEINT32(v) downtoBYTE((UINT32)v)
#define BOOLINT32(v) downtoBOOL((UINT32)v)

#define INT16INT16(v) (v)
#define INT16BYTE(v) ((INT16) (v))
#define INT16BOOL(v) ((INT16) ((v)?1:0))
#define BYTEINT16(v) downtoBYTE((UINT16)v)
#define BOOLINT16(v) downtoBOOL((UINT16)v)

#define BYTEBYTE(v)  (v)
#define BYTEBOOL(v)  ((BYTE) ((v)?1:0))
#define BOOLBYTE(v)  downtoBOOL(v)

#define BOOLBOOL(v)  (v)
/*}}}*/
/*{{{  fixed word size real conversion*/
/*{{{  REAL32 conversions*/
#ifndef NO_INT64
#define INT64REAL32TRUNC(v) downtoINT64(v)
#define REAL32INT64TRUNC(v) ((REAL32) (v))
#define INT64REAL32ROUND(v) downtoINT64((v)+0.5)
#define REAL32INT64ROUND(v) ((REAL32) (v))
#endif

#define INT32REAL32TRUNC(v) downtoINT32(v)
#define REAL32INT32TRUNC(v) ((REAL32) (v))
#define INT32REAL32ROUND(v) downtoINT32((v)+0.5)
#define REAL32INT32ROUND(v) ((REAL32) (v))

#define INT16REAL32TRUNC(v) downtoINT16(v)
#define REAL32INT16TRUNC(v) ((REAL32) (v))
#define INT16REAL32ROUND(v) downtoINT16((v)+0.5)
#define REAL32INT16ROUND(v) ((REAL32) (v))

#define BYTEREAL32TRUNC(v) downtoBYTE(v)
#define REAL32BYTETRUNC(v) ((REAL32) (v))
#define BYTEREAL32ROUND(v) downtoBYTE((v)+0.5)
#define REAL32BYTEROUND(v) ((REAL32) (v))

#define BOOLREAL32TRUNC(v) downtoBOOL(v)
#define REAL32BOOLTRUNC(v) ((REAL32) (v))
#define BOOLREAL32ROUND(v) downtoBOOL((v)+0.5)
#define REAL32BOOLROUND(v) ((REAL32) (v))

#define REAL32REAL32(v)      (v)
#define REAL32REAL32ROUND(v) (v)
#define REAL32REAL32TRUNC(v) (v)

/*}}}*/
#ifndef NO_REAL64
/*{{{  REAL64 conversions*/
#ifndef NO_INT64
#define INT64REAL64TRUNC(v) downtoINT64(v)
#define REAL64INT64TRUNC(v) ((REAL64) (v))
#define INT64REAL64ROUND(v) downtoINT64((v)+0.5)
#define REAL64INT64ROUND(v) ((REAL64) (v))
#endif

#define INT32REAL64TRUNC(v) downtoINT32(v)
#define REAL64INT32TRUNC(v) ((REAL64) (v))
#define INT32REAL64ROUND(v) downtoINT32((v)+0.5)
#define REAL64INT32ROUND(v) ((REAL64) (v))

#define INT16REAL64TRUNC(v) downtoINT16(v)
#define REAL64INT16TRUNC(v) ((REAL64) (v))
#define INT16REAL64ROUND(v) downtoINT16((v)+0.5)
#define REAL64INT16ROUND(v) ((REAL64) (v))

#define BYTEREAL64TRUNC(v) downtoBYTE(v)
#define REAL64BYTETRUNC(v) ((REAL64) (v))
#define BYTEREAL64ROUND(v) downtoBYTE((v)+0.5)
#define REAL64BYTEROUND(v) ((REAL64) (v))

#define BOOLREAL64TRUNC(v) downtoBOOL(v)
#define REAL64BOOLTRUNC(v) ((REAL64) (v))
#define BOOLREAL64ROUND(v) downtoBOOL((v)+0.5)
#define REAL64BOOLROUND(v) ((REAL64) (v))

#define REAL64REAL32ROUND(v) ((REAL64) (v))
#define REAL32REAL64ROUND(v) downtoREAL32((v)+0.5)
#define REAL64REAL32TRUNC(v) ((REAL64) (v))
#define REAL32REAL64TRUNC(v) downtoREAL32(v)
#define REAL64REAL32(v)      ((REAL64) (v))

#define REAL64REAL64(v)      (v)
#define REAL64REAL64ROUND(v) (v)
#define REAL64REAL64TRUNC(v) (v)

/*}}}*/
#endif
/*}}}*/
/*{{{  conversions involving INT*/
#if (BYTES_PER_WORD==4)
#define MOSTPOSINT MOSTPOSINT32
#define MOSTNEGINT MOSTNEGINT32

#define INT16INT(v) INT16INT32(v)
#define INT32INT(v) INT32INT32(v)

#define INTINT(v)   INT32INT(v)
#define INTINT16(v) INT32INT16(v)
#define INTINT32(v) INT32INT32(v)
#define INTBOOL(v)  INT32BOOL(v)
#define INTBYTE(v)  INT32BYTE(v)
#define BYTEINT(v)  BYTEINT32(v)
#define BOOLINT(v)  BOOLINT32(v)

#ifndef NO_INT64
#define INTINT64(v) INT32INT64(v)
#define INT64INT(v) INT64INT32(v)
#endif

#define INTREAL32TRUNC(v) INT32REAL32TRUNC(v)
#define REAL32INTTRUNC(v) REAL32INT32TRUNC(v)
#define INTREAL32ROUND(v) INT32REAL32ROUND(v)
#define REAL32INTROUND(v) REAL32INT32ROUND(v)

#ifndef NO_REAL64
#define INTREAL64TRUNC(v) INT32REAL64TRUNC(v)
#define REAL64INTTRUNC(v) REAL64INT32TRUNC(v)
#define INTREAL64ROUND(v) INT32REAL64ROUND(v)
#define REAL64INTROUND(v) REAL64INT32ROUND(v)
#endif

#elif (BYTES_PER_WORD==2)
#define MOSTPOSINT MOSTPOSINT16
#define MOSTNEGINT MOSTNEGINT16

#define INT16INT(v) INT16INT16(v)
#define INT32INT(v) INT32INT16(v)
#define INTINT(v)   INT16INT16(v)
#define INTINT16(v) INT16INT16(v)
#define INTINT32(v) INT16INT32(v)
#define INTBOOL(v)  INT16BOOL(v)
#define INTBYTE(v)  INT16BYTE(v)
#define BYTEINT(v)  BYTEINT16(v)

#ifndef NO_INT64
#define INTINT64(v) INT16INT64(v)
#define INT64INT(v) INT64INT16(v)
#endif

#define INTREAL32TRUNC(v) INT16REAL32TRUNC(v)
#define REAL32INTTRUNC(v) REAL32INT16TRUNC(v)
#define INTREAL32ROUND(v) INT16REAL32ROUND(v)
#define REAL32INTROUND(v) REAL32INT16ROUND(v)

#ifndef NO_REAL64
#define INTREAL64TRUNC(v) INT16REAL64TRUNC(v)
#define REAL64INTTRUNC(v) REAL64INT16TRUNC(v)
#define INTREAL64ROUND(v) INT16REAL64ROUND(v)
#define REAL64INTROUND(v) REAL64INT16ROUND(v)
#endif

#else
#error Compilation Error- Unsupported word length
#endif
/*}}}*/
/*}}}*/
/*{{{  static void inline CheckPrimitiveSizes(void)*/
static void inline CheckPrimitiveSizes(void)
{
  char *failedtype=NULL;
  
  if (sizeof(INT)!=BYTES_PER_WORD) failedtype="INT"; 
#ifndef NO_INT64
  if (sizeof(INT64)!= 8)
#ifdef __GNUC__
    failedtype="INT64";
#else
    fprintf(stderr,"Compatibility warning: INT64 held in %d bytes\n", sizeof(INT64));
#endif
#endif
  if (sizeof(INT32)!= 4) failedtype="INT32";
  if (sizeof(INT16)!=2) failedtype="INT16";
  if (sizeof(BOOL)!=1) failedtype="BOOL";
  if (sizeof(BYTE)!=1) failedtype="BYTE";
  if (sizeof(REAL32)!=4) failedtype="REAL32";
#ifndef NO_REAL64
  if (sizeof(REAL64)!=8) failedtype="REAL64";
#endif
  if (failedtype!=NULL)
  {
    fprintf(stderr,"\nInternal Error - C primitive type used for %s is not of expected size\n",failedtype);
    exit(-1);
  }
}
/*}}}*/
/*{{{  frame header*/
typedef struct
{
  void *ReturnFP;
  tFuncPtr ReturnFunc;
  int IP;
  TEMPVAL Temp;
  int Scope;
  char *ProcName;
} tHeader;

/*}}}*/
/*{{{  scheduling / communication primitives*/
#define INPUT(c,m,s,l)	 	{FP->_Header.IP = l;  if (ChanIn  (c,m,s)) return;}
#define INPUT1(c,m,l)		{FP->_Header.IP = l;  if (ChanIn1 (c,m)) return;}
#define INPUT2(c,m,l)		{FP->_Header.IP = l;  if (ChanIn2 (c,m)) return;}
#define INPUT4(c,m,l)		{FP->_Header.IP = l;  if (ChanIn4 (c,m)) return;}
#define INPUT8(c,m,l)		INPUT(c,m,8,l)

#define OUTPUT(c,m,s,l)		{FP->_Header.IP = l;  if (ChanOut (c,m,s)) return;}
#define OUTPUT1(c,m,l)		{FP->_Header.IP = l;  if (ChanOut1(c,m)) return;}
#define OUTPUT2(c,m,l)		{FP->_Header.IP = l;  if (ChanOut2(c,m)) return;}
#define OUTPUT4(c,m,l)		{FP->_Header.IP = l;  if (ChanOut4(c,m)) return;}
#define OUTPUT8(c,m,l)		OUTPUT(c,m,8,l)

#define ALT()       { AltStart(); }
#define ENBC(f,c)   { if (f) EnableChannel(c); }
#define ENBS(f)     { if (f) EnableSkip(); }
#define ENBT(f,t)   { if (f) EnableTimer(t); }
#define ALTWT(l)    { FP->_Header.IP = l; if (AltWait()) return; }
#define DISC(l,f,c) ( (f) ? DisableChannel(l,c) : (false) )
#define DISS(l,f)   ( (f) ? DisableSkip(l)      : (false) )
#define DIST(l,f,t) ( (f) ? DisableTimer(l,t)   : (false) )
#define ALTEND()    { FP->_Header.IP = AltFinish(); break; }
#define DELAY(t,l)  { FP->_Header.IP = l; WaitOnTimer(t); return; }
  
#ifdef FASTGOTOS
#define CASE(x) case x: label_ ## x
#define GOTO(x) goto label_ ## x;
#else
#define CASE(x) case x
#define GOTO(x) {FP->_Header.IP = x;break;}
#endif

#define STARTP(t,f,s,n,p) {(s)->Chain = FP;  (s)->_Header.IP = 0; Start_Process(t,(tFuncPtr)f,s,n,p);}
#define PLACED_STARTP(proc,t,f,s,n) {(s)->Chain = FP;  (s)->_Header.IP = 0; Start_Placed(t,(tFuncPtr)f,s,n,proc); }

#define ENDP()		{End_Process(); return; }
#define WAITP(l)	{FP->_Header.IP = l; DeSchedule(WAIT_p); return; }
#define INITCH(c)	{(c)->Task = NoTask; (c)->Data = NULL; }

#define CALL(p,s,l,n)   {(s)->_Header.ReturnFunc=CURTASK->Func;\
			 (s)->_Header.ReturnFP=FP;\
			 FP->_Header.IP = l;\
			 (s)->_Header.ProcName = n;\
			 CURTASK->Func=(tFuncPtr)p;\
			 CURTASK->FP=(void *)s;\
			 IDENTIFYCALL(n);\
			 (s)->_Header.IP = 0;\
			 return;\
			}

			
#define RETURN()        {CURTASK->Func=FP->_Header.ReturnFunc;\
			 CURTASK->FP = FP->_Header.ReturnFP;\
			 return;\
			}

#define WAITSOCKET(s)  { CURTASK->Func=FP->_Header.ReturnFunc;\
			 CURTASK->FP = FP->_Header.ReturnFP;\
			 WaitSocket(s); return; }

/*}}}*/
/*{{{  scheduling / communication routines*/
/*{{{  static void inline DeSchedule(register tTaskMode state)*/
static void inline DeSchedule(register tTaskMode state)
{
  register tTask *task = CURTASK;
  
  task->state = state;
  QFP[task->TaskPriority] = CURTASK->Next;
  if (QFP[task->TaskPriority] == NoTask) QBP[task->TaskPriority] = NoTask;
}   
/*}}}*/
/*{{{  static void inline ReSchedule(register tTask *task)*/
static void inline ReSchedule(register tTask *task)
{
#ifdef DEBUG
  printf("ReSchedule task (%d) from state %d\n",task->TaskId,task->state);
#endif

/*{{{  #ifdef USESIGTIMER*/
#ifdef USESIGTIMER
  if (task->state == TWAIT_p || task->state == TALTING_p)
  {
    tTask *prev = NoTask;
    tTask *this = QTM;

#ifdef DEBUG
    printf("Removing task from timer queue\n");
#endif

    while(this != NoTask && this != task)
    {
      prev = this;
      this = this->Next;
    }

    if (prev != NoTask) prev->Next = task->Next;
    else QTM = task->Next;

  }
#endif
/*}}}*/

  task->Next = NoTask;

  if (QFP[task->TaskPriority] != NoTask)
    QBP[task->TaskPriority]->Next = task;
  else
    QFP[task->TaskPriority] = task;

  task->state = EXEC_p;
  QBP[task->TaskPriority] = task;
}
/*}}}*/

/*{{{  static int inline ChanIn (register CHAN *chan,register void *data,register long len)*/
static int inline ChanIn (register CHAN *chan,register void *data,register long len)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
#ifdef DEBUG
    printf("Input completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (len != task->comms.io.len) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

    if (len > 0) memcpy(data,task->comms.io.data,(size_t)len);

    chan->Task = NoTask;
    ReSchedule(task);
    return 0;
  }    

#ifdef DEBUG
  printf("Input blocks\n");
#endif
  
  chan->Task = CURTASK;
  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = len;
  DeSchedule(IN_p);
  return 1;
}

/*}}}*/
/*{{{  static int inline ChanIn1(register CHAN *chan,register void *data)*/
static int inline ChanIn1(register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
#ifdef DEBUG
    printf("Input completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 1) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

    *((BYTE *)data) = *((BYTE *)task->comms.io.data);

    chan->Task = NoTask;
    ReSchedule(task);
    return 0;
  }    

#ifdef DEBUG
  printf("Input blocks\n");
#endif
  
  chan->Task = CURTASK;
  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 1;
  DeSchedule(IN_p);
  return 1;
}

/*}}}*/
/*{{{  static int inline ChanIn2(register CHAN *chan,register void *data)*/
static int inline ChanIn2(register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
#ifdef DEBUG
    printf("Input completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 2) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

    *((INT16 *)data) = *((INT16 *)task->comms.io.data);

    chan->Task = NoTask;
    ReSchedule(task);
    return 0;
  }    

#ifdef DEBUG
  printf("Input blocks\n");
#endif
  
  chan->Task = CURTASK;
  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 2;
  DeSchedule(IN_p);
  return 1;
}

/*}}}*/
/*{{{  static int inline ChanIn4(register CHAN *chan,register void *data)*/
static int inline ChanIn4(register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
#ifdef DEBUG
    printf("Input completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 4) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

    *((INT32 *)data) = *((INT32 *)task->comms.io.data);

    chan->Task = NoTask;
    ReSchedule(task);
    return 0;
  }    

#ifdef DEBUG
  printf("Input blocks\n");
#endif
  
  chan->Task = CURTASK;
  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 4;
  DeSchedule(IN_p);
  return 1;
}

/*}}}*/
/*{{{  static int inline ChanOut (register CHAN *chan,register void *data,register long len)*/
static int inline ChanOut (register CHAN *chan,register void *data,register long len)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
    tAltMode mode = task->AltMode;

    if (mode == NotAlting_p)
    {
#ifdef DEBUG
      printf("Output completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (len != task->comms.io.len) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

      if (len>0) memcpy(task->comms.io.data,data,(size_t)len);
      
      chan->Task = NoTask;
      ReSchedule(task);
      return 0;
    }
    else if (mode == Waiting_p)
    {
#ifdef DEBUG
      printf("Output restarts alt\n");
#endif

      task->AltMode = Ready_p;
      ReSchedule(task);
    }
   }

#ifdef DEBUG
  printf("Output blocks\n");
#endif
   
  chan->Task = CURTASK;

  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = len;
  DeSchedule(OUT_p);
  return 1;
}
/*}}}*/
/*{{{  static int inline ChanOut1 (register CHAN *chan,register void *data)*/
static int inline ChanOut1 (register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
    tAltMode mode = task->AltMode;

    if (mode == NotAlting_p)
    {
#ifdef DEBUG
      printf("Output completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 1) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

      *((BYTE *)task->comms.io.data) = *((BYTE *)data);
      
      chan->Task = NoTask;
      ReSchedule(task);
      return 0;
    }
    else if (mode == Waiting_p)
    {
#ifdef DEBUG
      printf("Output restarts alt\n");
#endif

      task->AltMode = Ready_p;
      ReSchedule(task);
    }
   }

#ifdef DEBUG
  printf("Output blocks\n");
#endif
   
  chan->Task = CURTASK;

  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 1;
  DeSchedule(OUT_p);
  return 1;
}
/*}}}*/
/*{{{  static int inline ChanOut2 (register CHAN *chan,register void *data)*/
static int inline ChanOut2 (register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
    tAltMode mode = task->AltMode;

    if (mode == NotAlting_p)
    {
#ifdef DEBUG
      printf("Output completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 2) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

      *((INT16 *)task->comms.io.data) = *((INT16 *)data);
      
      chan->Task = NoTask;
      ReSchedule(task);
      return 0;
    }
    else if (mode == Waiting_p)
    {
#ifdef DEBUG
      printf("Output restarts alt\n");
#endif

      task->AltMode = Ready_p;
      ReSchedule(task);
    }
   }

#ifdef DEBUG
  printf("Output blocks\n");
#endif
   
  chan->Task = CURTASK;

  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 2;
  DeSchedule(OUT_p);
  return 1;
}
/*}}}*/
/*{{{  static int inline ChanOut4 (register CHAN *chan,register void *data)*/
static int inline ChanOut4 (register CHAN *chan,register void *data)
{
  register tTask *task = chan->Task;

  if (task != NoTask)
  {
    tAltMode mode = task->AltMode;

    if (mode == NotAlting_p)
    {
#ifdef DEBUG
      printf("Output completes\n");
#endif

#ifdef CHECK_INTERNALS
    if (task->comms.io.len != 4) fprintf(stderr,"Internal Error-I/O length mismatch\n");
#endif

      *((INT32 *)task->comms.io.data) = *((INT32 *)data);
      
      chan->Task = NoTask;
      ReSchedule(task);
      return 0;
    }
    else if (mode == Waiting_p)
    {
#ifdef DEBUG
      printf("Output restarts alt\n");
#endif

      task->AltMode = Ready_p;
      ReSchedule(task);
    }
   }

#ifdef DEBUG
  printf("Output blocks\n");
#endif
   
  chan->Task = CURTASK;

  CURTASK->comms.io.data = data;
  CURTASK->comms.io.len = 4;
  DeSchedule(OUT_p);
  return 1;
}
/*}}}*/

/*{{{  static void inline TimerWait(tTimer when)*/
static void inline TimerWait(tTimer when)
{
#ifdef USESIGTIMER
  tTask *prev = NoTask;
  tTask *tq = QTM;

#ifdef DEBUG
  printf("Adding process to timer queue, for time %d\n",when);
#endif
  CURTASK->comms.timer.time = when;
  while(tq != NoTask && !AFTER(tq->comms.timer.time,when))
  {
    prev = tq;
    tq = tq->Next;
  }

  if (prev == NoTask)
  {
    CURTASK->Next = QTM;
    QTM=CURTASK;
  }
  else
  {
    CURTASK->Next = prev->Next;
    prev->Next = CURTASK;
  }
#endif
}
/*}}}*/

/*{{{  static void inline AltStart (void)*/
static void inline AltStart (void)
{
#ifdef DEBUG
  printf("Alt start\n");
#endif
  
#ifdef CHECK_INTERNALS
  if (CURTASK->AltMode != NotAlting_p)
  {
    fprintf(stderr,"Internal Error-Invalid alt mode at start\n");
    exit(-1);
  }
#endif

  CURTASK->AltMode = Enabling_p;

  CURTASK->comms.timer.TimerMode = TimeNotSet_p;
}
/*}}}*/
/*{{{  static void inline EnableChannel (register CHAN *chan)*/
static void inline EnableChannel (register CHAN *chan)
{
#ifdef DEBUG
  printf("Enable channel %x\n",chan);
#endif

  if (chan->Task != CURTASK)
  {
    if (chan->Task != NoTask)
    {
#ifdef DEBUG
      printf("Enable active channel\n");
#endif

      CURTASK->AltMode = Ready_p;
    }
    else
    {
#ifdef DEBUG
      printf("Enable idle channel\n");
#endif

      chan->Task = CURTASK;
    }
  }
}
/*}}}*/
/*{{{  static void inline EnableSkip (void)*/
static void inline EnableSkip (void)
{
#ifdef DEBUG
  printf("Enable skip\n");
#endif

  CURTASK->AltMode = Ready_p;
}
/*}}}*/
/*{{{  static void inline EnableTimer (register long t)*/
static void inline EnableTimer (register long t)
{
#ifdef DEBUG
  printf("Enable timer for %d\n",t);
#endif

#ifdef USESIGTIMER
  if (CURTASK->comms.timer.TimerMode == TimeNotSet_p)
  {
    CURTASK->comms.timer.TimerMode = TimeSet_p;
    CURTASK->comms.timer.time = t;
#ifdef DEBUG
    printf("Enabling first timer\n");
#endif
  }
  else
  {
    if (AFTER(CURTASK->comms.timer.time,t))
    {
      CURTASK->comms.timer.time = t;
#ifdef DEBUG
      printf("Enabling as shortest timer\n");
#endif
    }
  }  
#endif

}
/*}}}*/
/*{{{  static int  inline AltWait (void)*/
static int  inline AltWait (void)
{
#ifdef DEBUG
  printf("Alt wait\n");
#endif

#ifdef USESIGTIMER
  if ((CURTASK->comms.timer.TimerMode == TimeSet_p) &&
      AFTER(TIMER,CURTASK->comms.timer.time))
  {
#ifdef DEBUG
    printf("Timer alternative selected before waiting\n");
#endif
    CURTASK->AltMode = TReady_p;
  }
#endif
  
  if (CURTASK->AltMode == Enabling_p)
  {
#ifdef DEBUG
    printf("Alt blocks\n");
#endif

    CURTASK->AltMode = Waiting_p;
#ifdef USESIGTIMER
    if (CURTASK->comms.timer.TimerMode == TimeSet_p)
    {
      DeSchedule(TALTING_p);
      TimerWait(CURTASK->comms.timer.time);
    }
    else
#endif
      DeSchedule(ALTING_p);

    return 1;
  }

#ifdef DEBUG
  printf("Alt completes\n");
#endif
  
  return 0;
}
/*}}}*/
/*{{{  static BOOL inline DisableChannel (register int label, register CHAN *chan)*/
static BOOL inline DisableChannel (register int label, register CHAN *chan)
{
#ifdef DEBUG
  printf("Disable channel %x with label %d\n",chan,label);
#endif

  if (chan->Task != NoTask)
  {
    if (chan->Task == CURTASK)
    {
#ifdef DEBUG
      printf("Disable idle channel\n");
#endif

      chan->Task = NoTask;
    }
    else
    {
      if (CURTASK->AltMode < 0)
      {
#ifdef DEBUG
	printf("Disable active channel (selected)\n");
#endif
    
	CURTASK->AltMode = label;

	return(true);
      }
#ifdef DEBUG
      else
	printf("Disable active channel (not selected)\n");
#endif
    }
  }
  return(false);
}
/*}}}*/
/*{{{  static BOOL inline DisableSkip (register int label)*/
static BOOL inline DisableSkip (register int label)
{
#ifdef DEBUG
  printf("Disable skip with label %d\n",label);
#endif

  if (CURTASK->AltMode < 0)
  {
#ifdef DEBUG
    printf("Disable skip (selected)\n");
#endif
    
    CURTASK->AltMode = label;

    return(true);
  }
#ifdef DEBUG
  else
    printf("Disable skip (not selected)\n");
#endif

  return(false);
}
/*}}}*/
/*{{{  static BOOL inline DisableTimer (register int label, register long t)*/
static BOOL inline DisableTimer (register int label, register long t)
{
#ifdef DEBUG
  printf("Disable timer for %x with label %d\n",t,label);
#endif

  if (CURTASK->AltMode == TReady_p && CURTASK->comms.timer.time == t)
  {    
#ifdef DEBUG
    printf("Disable timer (selected)\n");
#endif
    
    CURTASK->AltMode = label;

    return(true);
  }

#ifdef DEBUG
  printf("Disable timer (not selected)\n");
#endif
  return(false);
}
/*}}}*/
/*{{{  static int  inline AltFinish (void)*/
static int  inline AltFinish (void)
{
  tAltMode mode = CURTASK->AltMode;
  
#ifdef DEBUG
  printf("Alt finish\n");
#endif

#ifdef CHECK_INTERNALS
  if (mode < 0)
  {
    fprintf(stderr,"Internal Error-Invalid alt mode at finish\n");
    exit(-1);
  }
  else
#endif
    CURTASK->AltMode = NotAlting_p;

  return mode;
}
/*}}}*/

/*{{{  static void WaitOnTimer(int when)*/
static void WaitOnTimer(tTimer when)
{
  DeSchedule(TWAIT_p);

#ifdef USESIGTIMER
  TimerWait(when);
#else
  ReSchedule(CURTASK);
#endif
}
/*}}}*/
/*{{{  static clock_t inline ReadTimer(void)*/
#ifdef USESIGTIMER
/*{{{  static tTimer inline ReadTimer(void)*/
static tTimer inline ReadTimer(void)
{
  register tTask *task = CURTASK;

  return (task->TaskPriority==0) ? TIMER<<6 : TIMER;
}
/*}}}*/
#else
/*{{{  static tTimer inline ReadTimer(void)*/
static tTimer inline ReadTimer(void)
{
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif
  
  register tTask *task = CURTASK;
  int microsecs_per_clock = 1000000 / CLOCKS_PER_SEC;
  tTimer microsecs = (tTimer)clock() * microsecs_per_clock;

  return (task->TaskPriority==0) ? microsecs : microsecs>>6;
}
/*}}}*/
#endif
/*}}}*/

extern void WaitSocket (int socket);
extern void Start_Process(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Pri);
extern void Start_Placed(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Processor);

/*}}}*/
/*{{{  retyping primitives*/
/*{{{  heap management*/
static char *MSG_HEAP = "Error- Out of heap storage\n"; 

static inline void *ALLOCVEC(int s)
{
  void *vec = malloc(s);
  if (!vec) SETERR(MSG_HEAP);

  return vec;
}

static inline void FREEVEC(void *vec)
{
  free(vec);
}
/*}}}*/
/*{{{  retype defines*/
#define ALLOCRETYPE(symb,type,size) symb=(type)ALLOCVEC(size)
#define FREERETYPE(s)          FREEVEC(s)

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_BIG)
static inline void RETYPE(int swap,int l,void *d,void *s)
{
  int i;
  char *from=(char *)s;
  char *to=(char *)d;

  for(i = 0; i < l; ++i)
    to[i ^ swap] = *from++;
}
#else
#define RETYPE(swap,l,d,s) memcpy((char*)d,(char*)s,l);
#endif

/*}}}*/


/*}}}*/
/*{{{  other primitives*/
#define ASSIGN(d,s,n) memcpy(d,s,(size_t)n)

static char *MSG_ASM = "Array size mismatch";
static char *MSG_ARM = "Array RETYPES abbreviation mismatch";
static char *MSG_AIM = "Array IS abbreviation mismatch";
static char *MSG_CITL = "Counted input too long";
static char *MSG_EACF = "Error- Array check failed : %s\n";
static char *MSG_ERCF = "Error- Range check (0<=%d<%d) failed\n"; 
static char *MSG_ELBCF = "Error- Lower bound check (0<=%d) failed\n"; 
static char *MSG_EUBCF = "Error- Upper bound check (0<=%d,%d<=%d) failed\n"; 

#ifdef FAST_SHIFTS
#define LOGSHIFTLEFT(t,x,y)   ((t)(((U##t)(x))<<(y)))
#define LOGSHIFTRIGHT(t,x,y)  ((t)(((U##t)(x))>>(y)))
#else
#define LOGSHIFTLEFT(t,x,y)   (((y)>=(sizeof(t)*BITS_PER_CHAR))?0:((t)(((U##t)(x))<<(y))))
#define LOGSHIFTRIGHT(t,x,y)  (((y)>=(sizeof(t)*BITS_PER_CHAR))?0:((t)(((U##t)(x))>>(y))))
#endif

#ifdef CHECK_ARRAYS
#define CHK(c,s) if (!(c)) {fprintf(stderr,MSG_EACF,s); SETERR(MSG_NULL);} 
#else
#define CHK(c,s)
#endif

#ifdef CHECK_RANGES
#ifdef USE_RANGE_MACROS
#define RNGCHK(v,s) ((((unsigned INT)(v))>=(s))?(fprintf(stderr,MSG_ERCF,v,s),seterr(MSG_NULL,__LINE__,__FILE__),v):v)
#define RNGCHKLB(v) (((v)<0)?(fprintf(stderr,MSG_ELBCF,v),seterr(MSG_NULL,__LINE__,__FILE__),v):v)
#define RNGCHKUB(b,f,s) ((((f)<0)||(((b)+(f))>(s)))?(fprintf(stderr,MSG_EUBCF,f,(b)+(f),s),seterr(MSG_NULL,__LINE__,__FILE__),b):b)
#else
#define RNGCHK(v,s) rngchk(v,s,__LINE__,__FILE__)
static INT inline rngchk(INT v, INT s, int line, char *file)
{
  if ((unsigned INT) v >= (unsigned INT)s) { fprintf(stderr,MSG_ERCF,v,s); seterr(MSG_NULL,line,file); }
  return v;
}
#define RNGCHKLB(v) rngchklb(v,__LINE__,__FILE__)
static INT inline rngchklb(INT v, int line, char *file)
{
  if (v<0) { fprintf(stderr,MSG_ELBCF,v); seterr(MSG_NULL,line,file); }
  return v;
}
#define RNGCHKUB(b,f,s) rngchkub(b,f,s,__LINE__,__FILE__)
static INT inline rngchkub(INT b, INT f, INT s, int line, char *file)
{
  if ((f<0)||((b+f)>s)) { fprintf(stderr,MSG_EUBCF,f,(b)+(f),s); seterr(MSG_NULL,line,file); }
  return b;
}
#endif
#else
#define RNGCHK(v,s) (v)
#define RNGCHKLB(v) (v)
#define RNGCHKUB(b,f,s) (b)
#endif

#ifdef SHOWTASKING
#define IDENTIFYTASK()   fprintf(stderr,"Task %c%d (%s) -",(CURTASK->TaskPriority)?'L':'H',CURTASK->TaskId,CURTASK->name);
#define IDENTIFYCALL(n)  fprintf(stderr,"CALL: %s()\n",n);
#else
#define IDENTIFYCALL(n)  
#define IDENTIFYTASK()
#endif
/*}}}*/


