/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Prototype Source File for generated C
||@(#)   System : Occam 2 C
||@(#)  Version : 1.3
||@(#) Filename : Occam2C.c
||@(#)     Date : 3/4/97
\*@(#)====================================================*/
/*}}}*/

/*{{{  Insert SWITCHES*/
#define LIBRARY_UNIT
#define BYTES_PER_WORD 4
#define TERMINATE_ON_ERROR
#define CHECK_CONVERSIONS
#define CHECK_ARRAYS
#define CHECK_RANGES
#define NO_DEBUG
#define FASTGOTOS
#define SOURCE LITTLE

/*}}}*/
/*{{{  defines*/
#define DEFAULT_MEMSIZE_IN_INTS 1000
#define OCCAM_MAIN

/*}}}*/
/*{{{  includes*/
#include <stdio.h>
#include <errno.h>

#if  (defined USESIGTIMER) || (defined USE_NONBLOCKING_SOCKETS)
#include <signal.h>
#include <sys/time.h>
#endif

#ifdef USE_NONBLOCKING_SOCKETS
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef solaris
#include <stropts.h>
#include <sys/file.h>
#include <sys/conf.h>
#endif
#endif

#ifdef CALL_NAG_F90
#include <f90.h>
#endif

#include "Occam2C.h"

#ifdef USE_NONBLOCKING_SOCKETS
  #ifdef solaris
  #define SOCKET_SIGNAL SIGPOLL
  #else
  #define SOCKET_SIGNAL SIGIO
  #endif
#endif
/*}}}*/
/*{{{  Insert HEADER*/
#include "Intrinsics.hdr"
typedef struct SF_P_INTTOSTRING_convert tSF_P_INTTOSTRING_convert;
typedef struct SF_P_INT16TOSTRING_convert tSF_P_INT16TOSTRING_convert;
typedef struct SF_P_INT32TOSTRING_convert tSF_P_INT32TOSTRING_convert;
typedef struct SF_P_INT64TOSTRING_convert tSF_P_INT64TOSTRING_convert;
typedef struct SF_P_STRINGTOINT_convert tSF_P_STRINGTOINT_convert;
typedef struct SF_P_INTTOINT16_89 tSF_P_INTTOINT16_89;
typedef struct SF_P_STRINGTOINT16_convert tSF_P_STRINGTOINT16_convert;
typedef struct SF_P_STRINGTOINT32_convert tSF_P_STRINGTOINT32_convert;
typedef struct SF_P_STRINGTOINT64_convert tSF_P_STRINGTOINT64_convert;
typedef struct SF_P_hex_digit_110 tSF_P_hex_digit_110;
typedef struct SF_P_HTOS_convert tSF_P_HTOS_convert;
typedef struct SF_P_HEXTOSTRING_convert tSF_P_HEXTOSTRING_convert;
typedef struct SF_P_HEX16TOSTRING_convert tSF_P_HEX16TOSTRING_convert;
typedef struct SF_P_HEX32TOSTRING_convert tSF_P_HEX32TOSTRING_convert;
typedef struct SF_P_hex_digit_132 tSF_P_hex_digit_132;
typedef struct SF_P_HEX64TOSTRING_convert tSF_P_HEX64TOSTRING_convert;
typedef struct SF_F_hex_digit_143 tSF_F_hex_digit_143;
typedef struct SF_P_STRINGTOHEX_convert tSF_P_STRINGTOHEX_convert;
typedef struct SF_P_INTTOINT16_156 tSF_P_INTTOINT16_156;
typedef struct SF_P_STRINGTOHEX16_convert tSF_P_STRINGTOHEX16_convert;
typedef struct SF_P_STRINGTOHEX32_convert tSF_P_STRINGTOHEX32_convert;
typedef struct SF_F_hex_digit_171 tSF_F_hex_digit_171;
typedef struct SF_P_STRINGTOHEX64_convert tSF_P_STRINGTOHEX64_convert;
typedef struct SF_P_REAL32TOSTRING_convert tSF_P_REAL32TOSTRING_convert;
typedef struct SF_P_REAL64TOSTRING_convert tSF_P_REAL64TOSTRING_convert;
typedef struct SF_P_STRINGTOREAL32_convert tSF_P_STRINGTOREAL32_convert;
typedef struct SF_P_STRINGTOREAL64_convert tSF_P_STRINGTOREAL64_convert;
typedef struct SF_P_STRINGTOBOOL_convert tSF_P_STRINGTOBOOL_convert;
typedef struct SF_P_BOOLTOSTRING_convert tSF_P_BOOLTOSTRING_convert;
void P_BOOLTOSTRING_convert (INT *len_218,INT string_219_Dim0,BYTE *string_219,BOOL b_220);
struct SF_P_INTTOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_INT16TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_INT32TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_INT64TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRINGTOINT_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_INTTOINT16_89
{
  tHeader _Header;
  tSF_P_STRINGTOINT16_convert *Chain;
};
struct SF_P_STRINGTOINT16_convert
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U0;
};
struct SF_P_STRINGTOINT32_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRINGTOINT64_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_hex_digit_110
{
  tHeader _Header;
  tSF_P_HTOS_convert *Chain;
};
struct SF_P_HTOS_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_HEXTOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_HEX16TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_HEX32TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_hex_digit_132
{
  tHeader _Header;
  tSF_P_HEX64TOSTRING_convert *Chain;
};
struct SF_P_HEX64TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_hex_digit_143
{
  tHeader _Header;
  tSF_P_STRINGTOHEX_convert *Chain;
};
struct SF_P_STRINGTOHEX_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_INTTOINT16_156
{
  tHeader _Header;
  tSF_P_STRINGTOHEX16_convert *Chain;
};
struct SF_P_STRINGTOHEX16_convert
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U1;
};
struct SF_P_STRINGTOHEX32_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_hex_digit_171
{
  tHeader _Header;
  tSF_P_STRINGTOHEX64_convert *Chain;
};
struct SF_P_STRINGTOHEX64_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_REAL32TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_REAL64TOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRINGTOREAL32_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRINGTOREAL64_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRINGTOBOOL_convert
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_BOOLTOSTRING_convert
{
  tHeader _Header;
  void *Chain;
};

/*}}}*/

#if (defined GENPROTO) || !(defined LIBRARY_UNIT)
/*{{{  statics*/
jmp_buf STOPP_env;
int MAIN_ARGC ;
char **MAIN_ARGV;
tTask *QFP[NUM_PRIORITY_LEVELS],*QBP[NUM_PRIORITY_LEVELS],*CURTASK,*QTM; 
static volatile int TimerEvent = false;
static volatile int SocketEvent = false;
tTimer TIMER;
static int process_count;


/*}}}*/
#endif

#ifdef GENPROTO
/*{{{  seterr*/
void seterr(char *M,int L,char *F) {}
/*}}}*/
/*{{{  struct Prototypes*/
struct Prototypes
{
  char *Proto;
  char *Define;
  int Space;
};
/*}}}*/
/*{{{  static struct Prototypes Table[] =*/
static struct Prototypes Table[] =
{
  {"void P_INTTOSTRING_convert (INT *len_51,INT string_52_Dim0,BYTE *string_52,INT n_53);",NULL,0},
  {"void P_INT16TOSTRING_convert (INT *len_59,INT string_60_Dim0,BYTE *string_60,INT16 n_61);",NULL,0},
  {"void P_INT32TOSTRING_convert (INT *len_63,INT string_64_Dim0,BYTE *string_64,INT32 n_65);",NULL,0},
  {"void P_INT64TOSTRING_convert (INT *len_67,INT string_68_Dim0,BYTE *string_68,INT64 n_69);",NULL,0},
  {"void P_STRINGTOINT_convert (BOOL *error_75,INT *n_76,INT string_77_Dim0,BYTE *string_77);",NULL,0},
  {"void P_STRINGTOINT16_convert (BOOL *error_82,INT16 *n_83,INT string_84_Dim0,BYTE *string_84);",NULL,0},
  {"void P_STRINGTOINT32_convert (BOOL *error_92,INT32 *n_93,INT string_94_Dim0,BYTE *string_94);",NULL,0},
  {"void P_STRINGTOINT64_convert (BOOL *error_97,INT64 *n_98,INT string_99_Dim0,BYTE *string_99);",NULL,0},
  {"void P_HTOS_convert (INT *len_104,INT string_105_Dim0,BYTE *string_105,INT n_106,INT npw_107);",NULL,0},
  {"void P_HEXTOSTRING_convert (INT *len_114,INT string_115_Dim0,BYTE *string_115,INT n_116);",NULL,0},
  {"void P_HEX16TOSTRING_convert (INT *len_119,INT string_120_Dim0,BYTE *string_120,INT16 n_121);",NULL,0},
  {"void P_HEX32TOSTRING_convert (INT *len_123,INT string_124_Dim0,BYTE *string_124,INT32 n_125);",NULL,0},
  {"void P_HEX64TOSTRING_convert (INT *len_127,INT string_128_Dim0,BYTE *string_128,INT64 n_129);",NULL,0},
  {"void P_STRINGTOHEX_convert (BOOL *error_136,INT *n_137,INT string_138_Dim0,BYTE *string_138);",NULL,0},
  {"void P_STRINGTOHEX16_convert (BOOL *error_149,INT16 *n_150,INT string_151_Dim0,BYTE *string_151);",NULL,0},
  {"void P_STRINGTOHEX32_convert (BOOL *error_159,INT32 *n_160,INT string_161_Dim0,BYTE *string_161);",NULL,0},
  {"void P_STRINGTOHEX64_convert (BOOL *error_164,INT64 *n_165,INT string_166_Dim0,BYTE *string_166);",NULL,0},
  {"void P_REAL32TOSTRING_convert (INT *l_176,INT string_177_Dim0,BYTE *string_177,REAL32 r_178,INT m_179,INT n_180);",NULL,0},
  {"void P_REAL64TOSTRING_convert (INT *l_184,INT string_185_Dim0,BYTE *string_185,REAL64 r_186,INT m_187,INT n_188);",NULL,0},
  {"void P_STRINGTOREAL32_convert (BOOL *error_192,REAL32 *n_193,INT string_194_Dim0,BYTE *string_194);",NULL,0},
  {"void P_STRINGTOREAL64_convert (BOOL *error_200,REAL64 *n_201,INT string_202_Dim0,BYTE *string_202);",NULL,0},
  {"void P_STRINGTOBOOL_convert (BOOL *error_208,BOOL *b_209,INT string_210_Dim0,BYTE *string_210);",NULL,0},
  {"void P_BOOLTOSTRING_convert (INT *len_218,INT string_219_Dim0,BYTE *string_219,BOOL b_220);",NULL,0},

 {NULL,NULL,0}
} ;
/*}}}*/
/*{{{  int main(int argc, char**argv)*/
int main(int argc, char**argv)
{
  char *name;
  FILE *f;
  int fcnt;
  
  CheckPrimitiveSizes();

  if (argc<2)
  {
    name="convert.hdr";
    f=fopen(name,"w");
  }
  else
  {
    name = argv[argc-1];
    if (name[0]=='-')
    {
      name = "stdout";
      f=stdout;
    }
    else
    {
      f=fopen(name,"w");
    }
  }
  fcnt=argc-1;
  
  if (f==NULL)
    { fprintf(stderr,"Error - could not open file %s\n",name);exit(-1); }
  else
  {
    int arg;
    int i;
    fprintf(f,"#ifndef __convert_INCLUDES__\n");
    fprintf(f,"#define __convert_INCLUDES__\n");
    
    for(arg = 1; arg < fcnt; ++arg)
    {
      FILE *fh = fopen(argv[arg],"r");
      if (fh == NULL) { fprintf(stderr,"Error - could not read file %s\n",name); exit(-1); }
      while(!feof(fh))
      {
	char line[256];

	if (fgets(line,256,fh) == NULL) break;
	if (line[0] == '#')
	  if (strncmp(line,"#define WS",10)!=0) continue;
	fputs(line,f);
      }
      fclose(fh);
    }      
      
    i = 0;
    while (Table[i].Proto!=NULL)
    {
      if (Table[i].Define != NULL)
      {
	fprintf(f,"typedef struct SF_%s tSF_%s;\n",Table[i].Define,Table[i].Define);
	fprintf(f,"#define WS_%s (%d)\n",Table[i].Define,Table[i].Space);
      }
      fprintf(f,"%s\n",Table[i].Proto);
      i++;
    }
    fprintf(f,"#endif\n");

    if (f!=stdout) fclose(f);
  }
  return(0);
}     
/*}}}*/
#endif

#ifndef LIBRARY_UNIT
/*{{{  seterr*/
static void DONE_SETERR() {}

void seterr(char *M,int L,char *F)
{
  if (M!=NULL) fputs(M,stderr);
  fprintf(stderr,MSG_EFROL,L,F);
  DONE_SETERR();
  SETERR_PROCEED;
}
/*}}}*/
/*{{{  DEBUGDONE,MARK_SCHEDULE*/
#ifndef NO_DEBUG
void DEBUGDONE(void) {}
void MARK_SCHEDULE(void) {}
#endif

/*}}}*/
/*{{{  timer*/
#ifdef USESIGTIMER
static struct timeval tv_start;
/*{{{  static void TimerSignalHandler(int sig)*/
static void TimerSignalHandler(int sig)
{
  switch(sig)
  {
    case SIGALRM:
      {
	struct timeval tv;
	tTask *task;

	gettimeofday(&tv,NULL);
	TIMER = ((tv.tv_usec - tv_start.tv_usec)>>6) + (1000000>>6) * (tv.tv_sec - tv_start.tv_sec);
	if (QTM != NoTask && AFTER(TIMER,QTM->comms.timer.time))
	  TimerEvent = true;
      }
      break;

    default:
      fprintf(stderr,"Error - Caught signal %d - Exiting\n",sig);
      fflush(stderr);
      exit(20);
  }
}
/*}}}*/
/*{{{  static void Init_Timer()*/
static void Init_Timer()
{
  struct itimerval itv;
#ifndef solaris
  struct sigaction sa;
#endif

  TIMER = 0;
  gettimeofday(&tv_start,NULL);
  TimerSignalHandler(SIGALRM);
  
#ifndef solaris
  memset((char*)&sa,0,sizeof(sa));
  sa.sa_handler = TimerSignalHandler;

#ifdef SA_RESTART
  sa.sa_flags = SA_RESTART;
#endif

  sigaction(SIGALRM,&sa,NULL);
#else
  sigset(SIGALRM,TimerSignalHandler);
#endif

  itv.it_interval.tv_usec = 1000;
  itv.it_interval.tv_sec = 0;
     
  itv.it_value.tv_usec = 1;
  itv.it_value.tv_sec = 0;

  if (setitimer(ITIMER_REAL,&itv,NULL)) 
    fprintf(stderr,"Error - setitimer failed\n");

  QTM = NoTask;
  TimerEvent = false;
}

/*}}}*/

#else
static void Init_Timer() { (void)clock(); TimerEvent = false; }
#endif
/*}}}*/
/*{{{  sockets*/
#ifdef USE_NONBLOCKING_SOCKETS
static int io_limit;
static struct timeval io_zerotime;

static int    io_socket;
static tTask *io_task = NoTask;

/*{{{  void SocketHandler (int sig)*/
void SocketHandler (int sig)
{
  switch (sig)
  {
    case SOCKET_SIGNAL:
    {
      #ifdef MD_DEBUG
      fprintf(stderr,"Caught signal %d\n",sig);
      #endif
      
      SocketEvent = true;
      
      #ifdef solaris
      signal (SOCKET_SIGNAL, SocketHandler);
      #endif
      
      break;
    }
    default:
      fprintf(stderr,"Error - Caught unexpected signal %d - exiting\n",sig);
      fflush(stderr);
      exit(20);
  }  
}
/*}}}*/
 
/*{{{  void WaitSocket (int socket)*/
void WaitSocket (int socket)
{
  fd_set io_set;

  io_socket = socket;

  FD_ZERO (&io_set);
  FD_SET  (socket,&io_set);
  
  errno = 0;
  while (select (io_limit, &io_set, NULL, NULL, &io_zerotime) < 0)
    if (errno != EINTR)
    {
      fprintf(stderr, "Error - Socket select failed\n");
      exit(-1);
    }
    else
      errno = 0;
  
  if (!FD_ISSET(socket,&io_set))
  {
    if (io_task == NoTask)
      io_task = CURTASK;
    else
    {
      fprintf(stderr, "Error - Implementation restriction, only one signaling socket\n");
      exit(-1);
    }
    DeSchedule(TSOCK_p);
  }
}
/*}}}*/

/*{{{  void EnableSocket (int socket)*/
void EnableSocket (int socket)
{
  errno = 0;
  
  #ifdef solaris
  if (ioctl(socket,I_SETSIG,S_RDNORM) < 0)
  {
    fprintf(stderr,"Error - could not set signal handler for socket (errno %d)\n",errno);
    exit(-1);
  }
  #else
  if (fcntl(socket, F_SETOWN, getpid()) < 0)
  {
    fprintf(stderr,"Error - could not set owner on socket (errno %d)\n",errno);
    exit(-1);
  }
  if (fcntl(socket, F_SETFL, FASYNC)    < 0)
  {
    fprintf(stderr,"Error - could not set asynchronous socket (errno %d)\n",errno);
    exit(-1);
  }
  #endif
}
/*}}}*/

/*{{{  void DisableSocket (int socket)*/
void DisableSocket (int socket)
{
  #ifdef sun4
  if (fcntl(socket, F_SETFL, FSYNC) < 0)
  {
    fprintf(stderr,"Error - could not set synchronous socket\n");
    exit(-1);
  }
  #endif
  #ifdef linux
  if (fcntl(socket, F_SETFL, FASYNC) < 0)
  {
    fprintf(stderr,"Error - could not set synchronous socket\n");
    exit(-1);
  }
  #endif
}
/*}}}*/

/*{{{  void ServeSocket (void)*/
void ServeSocket (void)
{
  fd_set io_set;
  int res;

  /* In the general case there's a timing hazard here since a new signal
     could arrive and we can lose it. Really need test-and-set or a
     semaphore. For hostio, however, there's only ever one outstanding 
     communication and this thread is serving it. Hence, no problem here. */

  SocketEvent = false; 

  FD_ZERO (&io_set);
  FD_SET  (io_socket,&io_set);
  
  errno = 0;
  while ((res = select (io_limit, &io_set, NULL, NULL, &io_zerotime)) < 0)
    if (errno != EINTR)
    {
      fprintf(stderr, "Error - Socket select failed\n");
      exit(-1);
    }
    else
      errno = 0;
      
  if (res == 0)
  {
    /*fprintf(stderr,"Could not locate socket signal source, ignoring select\n");*/
    return;
  }
  
  if (!FD_ISSET(io_socket,&io_set))
  {
    fprintf(stderr,"Error - socket is not ready\n");
    exit(-1);
  }

  if (io_task != NoTask)
  {
    ReSchedule(io_task);
    io_task = NoTask;
  }
}
/*}}}*/

/*{{{  void Init_Sockets()*/
void Init_Sockets()
{
  io_limit = ulimit (4,0);

  timerclear (&io_zerotime);

  signal (SOCKET_SIGNAL, SocketHandler);
  #ifdef linux
  {
    struct sigaction action;
    sigaction(SIGIO,NULL,&action);
    action.sa_flags = (action.sa_flags & (~SA_ONESHOT));
    sigaction(SIGIO,&action,NULL);
  }
  #endif
}
/*}}}*/
#else
void WaitSocket (int socket)
{ 
  fprintf(stderr, "Error - No SIGSOCKETS\n"); 
}
#endif
/*}}}*/

/*{{{  static void Init_Scheduler()*/
static void Init_Scheduler()
{
  int i;

  for(i = 0; i < NUM_PRIORITY_LEVELS; ++i)
    QFP[i] = QBP[i] = NoTask;

  CURTASK = NoTask;

  Init_Timer();

  #ifdef USE_NONBLOCKING_SOCKETS
  Init_Sockets();  
  #endif
}
/*}}}*/
/*{{{  static void Scheduler()*/
static void Scheduler()
{
  if (setjmp(STOPP_env)) DeSchedule(0);
  
  while(CURTASK != NoTask || TimerEvent || SocketEvent)
  {
    int pri;

#ifdef USESIGTIMER
    /*{{{  if (TimerEvent)*/
    if (TimerEvent)
    {
    #ifdef DEBUG
	fprintf(stderr,"Timer = %d\n",TIMER);
    #endif
      TimerEvent = false;
      while(QTM != NoTask && AFTER(TIMER,QTM->comms.timer.time))
      {
    #ifdef DEBUG
	{  
	  tTask *task = QTM;
    
	  fprintf(stderr,"Timer Queue contains:");
	  if (task == NoTask) fprintf(stderr,"No Tasks");
	  else while(task != NoTask) { fprintf(stderr,"%d(%d) ",task->TaskId,task->comms.timer.time); task = task->Next; }
	  fprintf(stderr,"\n");
	}  
    #endif
	if (QTM->AltMode == Waiting_p) QTM->AltMode = TReady_p;
	ReSchedule(QTM);
      }
    }
    /*}}}*/
#endif

#ifdef USE_NONBLOCKING_SOCKETS
    if (SocketEvent)
      ServeSocket();
#endif

    for(pri = 0; pri < NUM_PRIORITY_LEVELS; ++pri)
    {
#ifdef DEBUG
  {
    tTask *task = QFP[pri];
    fprintf(stderr,"Task Queue %d contains:",pri);
    if (task == NoTask) fprintf(stderr,"No Tasks");
    else while(task != NoTask) { fprintf(stderr,"%d ",task->TaskId); task = task->Next; }
    fprintf(stderr,"\n");
  }
#endif

      if ((CURTASK=QFP[pri]) != NoTask)
      {
	/*{{{  #ifdef SHOWTASKING*/
	#ifdef SHOWTASKING
		fprintf(stderr,"Scheduler - Current task is %c%d (%s) IP=%d\n",(CURTASK->TaskPriority)?'L':'H',CURTASK->TaskId,((tHeader *)CURTASK->FP)->ProcName,((tHeader *)CURTASK->FP)->IP); fflush(stderr); 
	#endif
	/*}}}*/
	/*{{{  #ifndef NO_DEBUG*/
	#ifndef NO_DEBUG
	MARK_SCHEDULE() ;
	#endif
	/*}}}*/
	CURTASK->Func (CURTASK->FP);
	break;
      }
    }

    /*{{{  catch any signals*/
    #if (defined USESIGTIMER && defined USE_NONBLOCKING_SOCKETS)
	if (CURTASK == NoTask && (QTM != NoTask || io_task != NoTask))
	  while(!TimerEvent && !SocketEvent)
	    pause();
    #else
      #if (defined USESIGTIMER)
	  if (CURTASK == NoTask && QTM != NoTask)
	    while(!TimerEvent)
	      pause();
      #endif
      
      #if (defined USE_NONBLOCKING_SOCKETS)
	  if (CURTASK == NoTask && io_task != NoTask)
	    while(!SocketEvent)
	      pause();
      #endif
    #endif
    /*}}}*/

  }

#ifdef DEBUG
  fprintf(stderr,"Scheduling Queue empty\n");
#endif
}

/*}}}*/
/*{{{  void Start_Process(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Priority)*/
void Start_Process(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Priority)
{
  static int NextTaskId = 0;
  process_count++;
  
  task->FP = FP;
  task->Func = Func;
  task->state = EXEC_p;
  task->name = name;
  task->Parent = CURTASK;
  task->Children = 0;
  task->Next = NoTask;
  task->AltMode = NotAlting_p;
  task->TaskId = NextTaskId++;
  task->TaskPriority = (CURTASK != NoTask && CURTASK->TaskPriority == HighPriority) ? HighPriority : Priority;
  ((tHeader *)FP)->ProcName = name;
  
  if (Priority < 0 || Priority >= NUM_PRIORITY_LEVELS) SETERR(MSG_PRI);
  
  if (QFP[task->TaskPriority])
    QBP[task->TaskPriority]->Next = task;
  else
    QFP[task->TaskPriority] = task;

  QBP[task->TaskPriority] = task;

  if (CURTASK != NoTask) CURTASK->Children++;
  else CURTASK = task;
  
#ifdef SHOWTASKING
  fprintf(stderr,"  New Task %c%d (%s) FP=%x\n",(task->TaskPriority)?'L':'H',task->TaskId,name,FP);
#endif
}
/*}}}*/
/*{{{  void Start_Placed(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Priority)*/
void Start_Placed(register tTask *task, register tFuncPtr Func, register void *FP, register char *name,int Processor)
{
  static int NextTaskId = 0;
  process_count++;

  task->FP = FP;
  task->Func = Func;
  task->state = EXEC_p;
  task->name = name;
  task->Parent = CURTASK;
  task->Children = 0;
  task->Next = NoTask;
  task->AltMode = NotAlting_p;
  task->TaskId = NextTaskId++;
  task->TaskPriority = LowPriority;

  ((tHeader *)FP)->ProcName = name;

  SETERR("PLACED PAR is not supported in this implementation");

  if (QFP[task->TaskPriority])
    QBP[task->TaskPriority]->Next = task;
  else
    QFP[task->TaskPriority] = task;

  QBP[task->TaskPriority] = task;

  if (CURTASK != NoTask) CURTASK->Children++;
  else CURTASK = task;
  
#ifdef SHOWTASKING
  fprintf(stderr,"  New Task %c%d (%s) FP=%x\n",(task->TaskPriority)?'L':'H',task->TaskId,name,FP);
#endif
}
/*}}}*/
/*{{{  void End_Process()*/
void End_Process()
{
  register tTask *task = CURTASK;
  process_count--;
    
  QFP[task->TaskPriority] = task->Next;
  if (QFP[task->TaskPriority] == NoTask) QBP[task->TaskPriority] = NoTask;
  task->Next = NoTask;
  if (task->Parent)
  {
    if (--(task->Parent->Children) == 0) ReSchedule(task->Parent);
  }
}
/*}}}*/

/*{{{  SPOC iserver definitions*/
#ifdef USE_SPOC_ISERVER
extern int CocoPops;
#include "spserver.hdr"

struct SF_P_spoc_iserver_spserver
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  char WS[WS_P_spoc_iserver_spserver];
};

typedef struct
{
  tHeader _Header;
  void *Chain;
  CHAN *fs;
  CHAN *ts;
  tSF_P_spoc_iserver_spserver spserver;
} tSF_P_spoc_iserver_caller;

static tSF_P_spoc_iserver_caller spoc_iserver_data;
static tTask spoc_iserver_task;

static tFuncPtr P_spoc_iserver_caller (tSF_P_spoc_iserver_caller *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      case  0:
		FP->spserver.Param0=FP->fs;
		FP->spserver.Param1=FP->ts;
		FP->spserver.Chain = NULL;
		CALL(P_spoc_iserver_spserver,&FP->spserver,1,"P_spoc_iserver_spserver");
      case  1:
		ENDP();
    }
  }
}
#endif
/*}}}*/
/*{{{  SPOC aserver definitions*/
#ifdef USE_SPOC_ASERVER
#include "spserver.hdr"

struct SF_P_spoc_aserver_spserver
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  char WS[WS_P_spoc_aserver_spserver];
};

typedef struct
{
  tHeader _Header;
  void *Chain;
  CHAN *fs;
  CHAN *ts;
  tSF_P_spoc_aserver_spserver spserver;
} tSF_P_spoc_aserver_caller;

static tSF_P_spoc_aserver_caller spoc_aserver_data;
static tTask spoc_aserver_task;

static tFuncPtr P_spoc_aserver_caller (tSF_P_spoc_aserver_caller *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      case  0:
		FP->spserver.Param0=FP->fs;
		FP->spserver.Param1=FP->ts;
		FP->spserver.Chain = NULL;
		CALL(P_spoc_aserver_spserver,&FP->spserver,1,"P_spoc_aserver_spserver");
      case  1:
		ENDP();
    }
  }
}
#endif
/*}}}*/

/*{{{  int main(int argc, char **argv)*/
int main(int argc, char **argv)
{
  void *FP=NULL;
  tSF_ *MAINFP=(tSF_ *)malloc(sizeof(tSF_));
  tTask main_t;
  INT mem_size = (argc>1 && atoi(argv[1])>0)?atoi(argv[1]):DEFAULT_MEMSIZE_IN_INTS;
  INT *mem=(INT*)malloc(mem_size*sizeof(INT));
  CHAN fs,ts;
  
  if (MAINFP==NULL)
  {
    fprintf(stderr,"Error - Malloc of %s bytes for occam workspace failed\n",sizeof(tSF_));
    exit(-1);
  }
  if (mem_size>0 && mem==NULL)
  {
    fprintf(stderr,"Error - Malloc of %s INTs for occam memory array failed\n",mem_size);
    exit(-1);
  }
  (void) clock();
  MAIN_ARGC = argc ;
  MAIN_ARGV = argv ;
  process_count = 0;
 
  CheckPrimitiveSizes();
  INITCH(&fs);
  INITCH(&ts);
  Init_Scheduler();
  
  STARTP(&main_t,,MAINFP,"",LowPriority);

  /*{{{  #ifdef USE_SPOC_ISERVER*/
  #ifdef USE_SPOC_ISERVER
  #ifdef SHOWTASKING
  CocoPops=true;
  #endif
  spoc_iserver_data.fs = &fs;
  spoc_iserver_data.ts = &ts;
  CURTASK = NoTask;  /* needed because don't want this a child of main_t */
  STARTP(&spoc_iserver_task, P_spoc_iserver_caller, &spoc_iserver_data, "P_spoc_iserver_spserver",LowPriority);
  #endif
  /*}}}*/
  /*{{{  #ifdef USE_SPOC_ASERVER*/
  #ifdef USE_SPOC_ASERVER
  spoc_aserver_data.fs = &fs;
  spoc_aserver_data.ts = &ts;
  CURTASK = NoTask;  /* needed because don't want this a child of main_t */
  STARTP(&spoc_aserver_task, P_spoc_aserver_caller, &spoc_aserver_data, "P_spoc_aserver_spserver",LowPriority);
  #endif
  /*}}}*/
/*{{{  #ifdef CALL_NAG_F90*/
#ifdef CALL_NAG_F90
  f90_init(argc,argv);
#endif
/*}}}*/
  
  Scheduler();

/*{{{  #ifdef CALL_NAG_F90*/
#ifdef CALL_NAG_F90
  f90_finish(0);
#endif
/*}}}*/
  if (process_count == 0)
    return 0;
  else
  {
    fprintf(stderr,"Deadlock: %d stopped process%s.\n", process_count,(process_count>1)?"es":"");
    return 1;
  }
}     
/*}}}*/
#endif

#ifndef GENPROTO
/*{{{  Insert DEBUG*/

/*}}}*/
/*{{{  Insert CODE*/
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
void P_INTTOSTRING_convert (INT *len_51,INT string_52_Dim0,BYTE *string_52,INT n_53)
{
  tSF_P_INTTOSTRING_convert SF;
  tSF_P_INTTOSTRING_convert *FP = &SF;
  {
    INT val_54;
    BOOL neg_55;
    {
      *len_51 = 0; 
      val_54 = n_53; 
      neg_55 = (n_53 < 0); 
      while (((val_54 > 9) || (val_54 < -9)))
      {
        {
          if (neg_55)
          {
            *((BYTE*)(((BYTE*)string_52)+(RNGCHK(*len_51,string_52_Dim0)*sizeof(BYTE)))) = BYTEINT((48 - (val_54 % 10))); 
          }
          else
          if (true)
          {
            *((BYTE*)(((BYTE*)string_52)+(RNGCHK(*len_51,string_52_Dim0)*sizeof(BYTE)))) = BYTEINT((48 + (val_54 % 10))); 
          }
          else
          {
            SETERR(MSG_IF);
          }
          *len_51 = (*len_51 + 1); 
          val_54 = (val_54 / 10); 
        }
      }
      if (neg_55)
      {
        {
          *((BYTE*)(((BYTE*)string_52)+(RNGCHK(*len_51,string_52_Dim0)*sizeof(BYTE)))) = BYTEINT((48 - (val_54 % 10))); 
          *len_51 = (*len_51 + 1); 
          *((BYTE*)(((BYTE*)string_52)+(RNGCHK(*len_51,string_52_Dim0)*sizeof(BYTE)))) = 45; 
        }
      }
      else
      if (true)
      {
        *((BYTE*)(((BYTE*)string_52)+(RNGCHK(*len_51,string_52_Dim0)*sizeof(BYTE)))) = BYTEINT((48 + (val_54 % 10))); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      *len_51 = (*len_51 + 1); 
      {
        INT i_56;
        INT i_56_End;
        i_56 = 0;i_56_End = i_56 + (*len_51 / 2);
        for (; i_56 != i_56_End; i_56++)
        {
          {
            BYTE b_57;
            {
              b_57 = *((BYTE*)(((BYTE*)string_52)+(RNGCHK(i_56,string_52_Dim0)*sizeof(BYTE)))); 
              *((BYTE*)(((BYTE*)string_52)+(RNGCHK(i_56,string_52_Dim0)*sizeof(BYTE)))) = *((BYTE*)(((BYTE*)string_52)+(RNGCHK(((*len_51 - 1) - i_56),string_52_Dim0)*sizeof(BYTE)))); 
              *((BYTE*)(((BYTE*)string_52)+(RNGCHK(((*len_51 - 1) - i_56),string_52_Dim0)*sizeof(BYTE)))) = b_57; 
            }
          }
        }
      }
    }
  }
}

void P_INT16TOSTRING_convert (INT *len_59,INT string_60_Dim0,BYTE *string_60,INT16 n_61)
{
  tSF_P_INT16TOSTRING_convert SF;
  tSF_P_INT16TOSTRING_convert *FP = &SF;
  P_INTTOSTRING_convert(len_59,string_60_Dim0,(BYTE*)string_60,INTINT16(n_61));
}

void P_INT32TOSTRING_convert (INT *len_63,INT string_64_Dim0,BYTE *string_64,INT32 n_65)
{
  tSF_P_INT32TOSTRING_convert SF;
  tSF_P_INT32TOSTRING_convert *FP = &SF;
  P_INTTOSTRING_convert(len_63,string_64_Dim0,(BYTE*)string_64,INTINT32(n_65));
}

void P_INT64TOSTRING_convert (INT *len_67,INT string_68_Dim0,BYTE *string_68,INT64 n_69)
{
  tSF_P_INT64TOSTRING_convert SF;
  tSF_P_INT64TOSTRING_convert *FP = &SF;
  {
    INT64 val_70;
    BOOL neg_71;
    {
      *len_67 = 0; 
      val_70 = n_69; 
      neg_71 = (n_69 < 0); 
      while (((val_70 > 9) || (val_70 < -9)))
      {
        {
          if (neg_71)
          {
            *((BYTE*)(((BYTE*)string_68)+(RNGCHK(*len_67,string_68_Dim0)*sizeof(BYTE)))) = BYTEINT64((48 - (val_70 % 10))); 
          }
          else
          if (true)
          {
            *((BYTE*)(((BYTE*)string_68)+(RNGCHK(*len_67,string_68_Dim0)*sizeof(BYTE)))) = BYTEINT64((48 + (val_70 % 10))); 
          }
          else
          {
            SETERR(MSG_IF);
          }
          *len_67 = (*len_67 + 1); 
          val_70 = (val_70 / 10); 
        }
      }
      if (neg_71)
      {
        {
          *((BYTE*)(((BYTE*)string_68)+(RNGCHK(*len_67,string_68_Dim0)*sizeof(BYTE)))) = BYTEINT64((48 - (val_70 % 10))); 
          *len_67 = (*len_67 + 1); 
          *((BYTE*)(((BYTE*)string_68)+(RNGCHK(*len_67,string_68_Dim0)*sizeof(BYTE)))) = 45; 
        }
      }
      else
      if (true)
      {
        *((BYTE*)(((BYTE*)string_68)+(RNGCHK(*len_67,string_68_Dim0)*sizeof(BYTE)))) = BYTEINT64((48 + (val_70 % 10))); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      *len_67 = (*len_67 + 1); 
      {
        INT i_72;
        INT i_72_End;
        i_72 = 0;i_72_End = i_72 + (*len_67 / 2);
        for (; i_72 != i_72_End; i_72++)
        {
          {
            BYTE b_73;
            {
              b_73 = *((BYTE*)(((BYTE*)string_68)+(RNGCHK(i_72,string_68_Dim0)*sizeof(BYTE)))); 
              *((BYTE*)(((BYTE*)string_68)+(RNGCHK(i_72,string_68_Dim0)*sizeof(BYTE)))) = *((BYTE*)(((BYTE*)string_68)+(RNGCHK(((*len_67 - 1) - i_72),string_68_Dim0)*sizeof(BYTE)))); 
              *((BYTE*)(((BYTE*)string_68)+(RNGCHK(((*len_67 - 1) - i_72),string_68_Dim0)*sizeof(BYTE)))) = b_73; 
            }
          }
        }
      }
    }
  }
}

void P_STRINGTOINT_convert (BOOL *error_75,INT *n_76,INT string_77_Dim0,BYTE *string_77)
{
  tSF_P_STRINGTOINT_convert SF;
  tSF_P_STRINGTOINT_convert *FP = &SF;
  {
    BOOL neg_78;
    INT i_79;
    BOOL over_80;
    {
      neg_78 = false; 
      *error_75 = false; 
      over_80 = false; 
      *n_76 = 0; 
      if (((string_77_Dim0) == 0))
      {
        *error_75 = true; 
      }
      else
      if (((*((BYTE*)(((BYTE*)string_77)+(0*sizeof(BYTE)))) == 45) && ((string_77_Dim0) > 1)))
      {
        {
          i_79 = 1; 
          neg_78 = true; 
        }
      }
      else
      if (((*((BYTE*)(((BYTE*)string_77)+(0*sizeof(BYTE)))) == 43) && ((string_77_Dim0) > 1)))
      {
        i_79 = 1; 
      }
      else
      if (true)
      {
        i_79 = 0; 
      }
      else
      {
        SETERR(MSG_IF);
      }
      while ((( ! *error_75) && (i_79 < (string_77_Dim0))))
      {
        {
          if (((*((BYTE*)(((BYTE*)string_77)+(RNGCHK(i_79,string_77_Dim0)*sizeof(BYTE)))) >= 48) && (*((BYTE*)(((BYTE*)string_77)+(RNGCHK(i_79,string_77_Dim0)*sizeof(BYTE)))) <= 57)))
          {
            if (neg_78)
            {
              *n_76 = ((*n_76 * 10) - (INTBYTE(*((BYTE*)(((BYTE*)string_77)+(RNGCHK(i_79,string_77_Dim0)*sizeof(BYTE))))) - 48)); 
            }
            else
            if (true)
            {
              *n_76 = ((*n_76 * 10) + (INTBYTE(*((BYTE*)(((BYTE*)string_77)+(RNGCHK(i_79,string_77_Dim0)*sizeof(BYTE))))) - 48)); 
            }
            else
            {
              SETERR(MSG_IF);
            }
          }
          else
          if (true)
          {
            *error_75 = true; 
          }
          else
          {
            SETERR(MSG_IF);
          }
          i_79 = (i_79 + 1); 
          if (((neg_78 && (*n_76 > 0)) || (( ! neg_78) && (*n_76 < 0))))
          {
            over_80 = true; 
          }
          else
          if (true)
          {
          }
          else
          {
            SETERR(MSG_IF);
          }
        }
      }
      if (over_80)
      {
        *error_75 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

static void P_INTTOINT16_89 (INT a_85,INT16 *b_86,BOOL *error_87)
{
  tSF_P_INTTOINT16_89 SF;
  tSF_P_INTTOINT16_89 *FP = &SF;
  {
    INT n_88;
    {
      if (((a_85 > 32767) || (a_85 < -32768)))
      {
        {
          *error_87 = true; 
          n_88 = (a_85 & 65535); 
          if ((n_88 > 32767))
          {
            n_88 = ( - (n_88 & 32767)); 
          }
          else
          if (true)
          {
          }
          else
          {
            SETERR(MSG_IF);
          }
        }
      }
      else
      if (true)
      {
        n_88 = a_85; 
      }
      else
      {
        SETERR(MSG_IF);
      }
      *b_86 = INT16INT(n_88); 
    }
  }
}

void P_STRINGTOINT16_convert (BOOL *error_82,INT16 *n_83,INT string_84_Dim0,BYTE *string_84)
{
  tSF_P_STRINGTOINT16_convert SF;
  tSF_P_STRINGTOINT16_convert *FP = &SF;
  {
    INT n_int_90;
    {
      P_STRINGTOINT_convert(error_82,&n_int_90,string_84_Dim0,(BYTE*)string_84);
      P_INTTOINT16_89(n_int_90,n_83,error_82);
    }
  }
}

void P_STRINGTOINT32_convert (BOOL *error_92,INT32 *n_93,INT string_94_Dim0,BYTE *string_94)
{
  tSF_P_STRINGTOINT32_convert SF;
  tSF_P_STRINGTOINT32_convert *FP = &SF;
  {
    INT n_int_95;
    {
      P_STRINGTOINT_convert(error_92,&n_int_95,string_94_Dim0,(BYTE*)string_94);
      *n_93 = INT32INT(n_int_95); 
    }
  }
}

void P_STRINGTOINT64_convert (BOOL *error_97,INT64 *n_98,INT string_99_Dim0,BYTE *string_99)
{
  tSF_P_STRINGTOINT64_convert SF;
  tSF_P_STRINGTOINT64_convert *FP = &SF;
  {
    BOOL neg_100;
    INT i_101;
    BOOL over_102;
    {
      neg_100 = false; 
      *error_97 = false; 
      over_102 = false; 
      *n_98 = 0; 
      if (((string_99_Dim0) == 0))
      {
        *error_97 = true; 
      }
      else
      if (((*((BYTE*)(((BYTE*)string_99)+(0*sizeof(BYTE)))) == 45) && ((string_99_Dim0) > 1)))
      {
        {
          i_101 = 1; 
          neg_100 = true; 
        }
      }
      else
      if (((*((BYTE*)(((BYTE*)string_99)+(0*sizeof(BYTE)))) == 43) && ((string_99_Dim0) > 1)))
      {
        i_101 = 1; 
      }
      else
      if (true)
      {
        i_101 = 0; 
      }
      else
      {
        SETERR(MSG_IF);
      }
      while ((( ! *error_97) && (i_101 < (string_99_Dim0))))
      {
        {
          if (((*((BYTE*)(((BYTE*)string_99)+(RNGCHK(i_101,string_99_Dim0)*sizeof(BYTE)))) >= 48) && (*((BYTE*)(((BYTE*)string_99)+(RNGCHK(i_101,string_99_Dim0)*sizeof(BYTE)))) <= 57)))
          {
            if (neg_100)
            {
              *n_98 = ((*n_98 * 10) - (INT64BYTE(*((BYTE*)(((BYTE*)string_99)+(RNGCHK(i_101,string_99_Dim0)*sizeof(BYTE))))) - 48)); 
            }
            else
            if (true)
            {
              *n_98 = ((*n_98 * 10) + (INT64BYTE(*((BYTE*)(((BYTE*)string_99)+(RNGCHK(i_101,string_99_Dim0)*sizeof(BYTE))))) - 48)); 
            }
            else
            {
              SETERR(MSG_IF);
            }
          }
          else
          if (true)
          {
            *error_97 = true; 
          }
          else
          {
            SETERR(MSG_IF);
          }
          i_101 = (i_101 + 1); 
          if (((neg_100 && (*n_98 > 0)) || (( ! neg_100) && (*n_98 < 0))))
          {
            over_102 = true; 
          }
          else
          if (true)
          {
          }
          else
          {
            SETERR(MSG_IF);
          }
        }
      }
      if (over_102)
      {
        *error_97 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

static void P_hex_digit_110 (BYTE *byte_108,INT digit_109)
{
  tSF_P_hex_digit_110 SF;
  tSF_P_hex_digit_110 *FP = &SF;
  {
    if ((digit_109 < 10))
    {
      *byte_108 = BYTEINT((digit_109 + 48)); 
    }
    else
    if ((digit_109 < 16))
    {
      *byte_108 = BYTEINT((digit_109 + 55)); 
    }
    else
    {
      SETERR(MSG_IF);
    }
  }
}

void P_HTOS_convert (INT *len_104,INT string_105_Dim0,BYTE *string_105,INT n_106,INT npw_107)
{
  tSF_P_HTOS_convert SF;
  tSF_P_HTOS_convert *FP = &SF;
  {
    INT rest_111;
    {
      rest_111 = n_106; 
      {
        INT i_112;
        INT i_112_End;
        i_112 = 0;i_112_End = i_112 + npw_107;
        for (; i_112 != i_112_End; i_112++)
        {
          {
            P_hex_digit_110(((BYTE*)(((BYTE*)string_105)+(RNGCHK(((npw_107 - 1) - i_112),string_105_Dim0)*sizeof(BYTE)))),(15 & rest_111));
            rest_111 = LOGSHIFTRIGHT(INT,rest_111,4); 
          }
        }
      }
      *len_104 = npw_107; 
    }
  }
}

void P_HEXTOSTRING_convert (INT *len_114,INT string_115_Dim0,BYTE *string_115,INT n_116)
{
  tSF_P_HEXTOSTRING_convert SF;
  tSF_P_HEXTOSTRING_convert *FP = &SF;
  {
    INT _V0_224;
    BYTE *word_117;
    _V0_224 = (INT)1;
    {
      static INT TMP; TMP = 1;
      word_117 = (BYTE*)&TMP;
    }
    P_HTOS_convert(len_114,string_115_Dim0,(BYTE*)string_115,INTINT(n_116),8);
  }
}

void P_HEX16TOSTRING_convert (INT *len_119,INT string_120_Dim0,BYTE *string_120,INT16 n_121)
{
  tSF_P_HEX16TOSTRING_convert SF;
  tSF_P_HEX16TOSTRING_convert *FP = &SF;
  P_HTOS_convert(len_119,string_120_Dim0,(BYTE*)string_120,INTINT16(n_121),4);
}

void P_HEX32TOSTRING_convert (INT *len_123,INT string_124_Dim0,BYTE *string_124,INT32 n_125)
{
  tSF_P_HEX32TOSTRING_convert SF;
  tSF_P_HEX32TOSTRING_convert *FP = &SF;
  P_HTOS_convert(len_123,string_124_Dim0,(BYTE*)string_124,INTINT32(n_125),8);
}

static void P_hex_digit_132 (BYTE *byte_130,INT digit_131)
{
  tSF_P_hex_digit_132 SF;
  tSF_P_hex_digit_132 *FP = &SF;
  {
    if ((digit_131 < 10))
    {
      *byte_130 = BYTEINT((digit_131 + 48)); 
    }
    else
    if ((digit_131 < 16))
    {
      *byte_130 = BYTEINT((digit_131 + 55)); 
    }
    else
    {
      SETERR(MSG_IF);
    }
  }
}

void P_HEX64TOSTRING_convert (INT *len_127,INT string_128_Dim0,BYTE *string_128,INT64 n_129)
{
  tSF_P_HEX64TOSTRING_convert SF;
  tSF_P_HEX64TOSTRING_convert *FP = &SF;
  {
    INT64 rest_133;
    {
      rest_133 = n_129; 
      {
        INT i_134;
        for (i_134 = 0; i_134 != 0 + 16; i_134++)
        {
          {
            P_hex_digit_132(((BYTE*)(((BYTE*)string_128)+((15 - i_134)*sizeof(BYTE)))),INTINT64((15 & rest_133)));
            rest_133 = LOGSHIFTRIGHT(INT64,rest_133,4); 
          }
        }
      }
      *len_127 = 16; 
    }
  }
}

static void F_hex_digit_143(tSF_P_STRINGTOHEX_convert *PP,BYTE b_139,BOOL *Param0,INT *Param1)
{
  tSF_F_hex_digit_143 SF;
  tSF_F_hex_digit_143 *FP = &SF;
  FP->Chain = PP;
  {
    BOOL error_140;
    INT n_141;
    if (((b_139 >= 48) && (b_139 <= 57)))
    {
      {
        n_141 = (INTBYTE(b_139) - 48); 
        error_140 = false; 
      }
    }
    else
    if (((b_139 >= 65) && (b_139 <= 70)))
    {
      {
        n_141 = (INTBYTE(b_139) - 55); 
        error_140 = false; 
      }
    }
    else
    if (true)
    {
      {
        error_140 = true; 
        n_141 = 0; 
      }
    }
    else
    {
      SETERR(MSG_IF);
    }
    *Param0=error_140;
    *Param1=n_141;
  }
}

void P_STRINGTOHEX_convert (BOOL *error_136,INT *n_137,INT string_138_Dim0,BYTE *string_138)
{
  tSF_P_STRINGTOHEX_convert SF;
  tSF_P_STRINGTOHEX_convert *FP = &SF;
  {
    INT i_144;
    BOOL over_145;
    {
      i_144 = 0; 
      *n_137 = 0; 
      over_145 = false; 
      *error_136 = false; 
      if (((string_138_Dim0) > 0))
      {
        while ((( ! *error_136) && (i_144 < (string_138_Dim0))))
        {
          {
            INT nibble_146;
            {
              {
                F_hex_digit_143(FP,*((BYTE*)(((BYTE*)string_138)+(RNGCHK(i_144,string_138_Dim0)*sizeof(BYTE)))),error_136,&nibble_146);
              }
              if (*error_136)
              {
              }
              else
              if (true)
              {
                {
                  INT o_147;
                  {
                    i_144 = (i_144 + 1); 
                    {
                      F_SHIFTLEFT_Intrinsics(0,*n_137,4,&o_147,n_137);
                    }
                    *n_137 = (*n_137 + nibble_146); 
                    over_145 = (over_145 || (o_147 != 0)); 
                  }
                }
              }
              else
              {
                SETERR(MSG_IF);
              }
            }
          }
        }
      }
      else
      if (true)
      {
        *error_136 = true; 
      }
      else
      {
        SETERR(MSG_IF);
      }
      if (over_145)
      {
        *error_136 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

static void P_INTTOINT16_156 (INT a_152,INT16 *b_153,BOOL *error_154)
{
  tSF_P_INTTOINT16_156 SF;
  tSF_P_INTTOINT16_156 *FP = &SF;
  {
    INT n_155;
    {
      n_155 = a_152; 
      if ((n_155 > 65535))
      {
        {
          *error_154 = true; 
          n_155 = (n_155 & 65535); 
        }
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
      if ((n_155 > 32767))
      {
        n_155 = ( - (n_155 & 32767)); 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
      *b_153 = INT16INT(n_155); 
    }
  }
}

void P_STRINGTOHEX16_convert (BOOL *error_149,INT16 *n_150,INT string_151_Dim0,BYTE *string_151)
{
  tSF_P_STRINGTOHEX16_convert SF;
  tSF_P_STRINGTOHEX16_convert *FP = &SF;
  {
    INT n_int_157;
    {
      P_STRINGTOHEX_convert(error_149,&n_int_157,string_151_Dim0,(BYTE*)string_151);
      P_INTTOINT16_156(n_int_157,n_150,error_149);
    }
  }
}

void P_STRINGTOHEX32_convert (BOOL *error_159,INT32 *n_160,INT string_161_Dim0,BYTE *string_161)
{
  tSF_P_STRINGTOHEX32_convert SF;
  tSF_P_STRINGTOHEX32_convert *FP = &SF;
  {
    INT n_int_162;
    {
      P_STRINGTOHEX_convert(error_159,&n_int_162,string_161_Dim0,(BYTE*)string_161);
      *n_160 = INT32INT(n_int_162); 
    }
  }
}

static void F_hex_digit_171(tSF_P_STRINGTOHEX64_convert *PP,BYTE b_167,BOOL *Param0,INT *Param1)
{
  tSF_F_hex_digit_171 SF;
  tSF_F_hex_digit_171 *FP = &SF;
  FP->Chain = PP;
  {
    BOOL error_168;
    INT n_169;
    if (((b_167 >= 48) && (b_167 <= 57)))
    {
      {
        n_169 = (INTBYTE(b_167) - 48); 
        error_168 = false; 
      }
    }
    else
    if (((b_167 >= 65) && (b_167 <= 70)))
    {
      {
        n_169 = (INTBYTE(b_167) - 55); 
        error_168 = false; 
      }
    }
    else
    if (true)
    {
      {
        error_168 = true; 
        n_169 = 0; 
      }
    }
    else
    {
      SETERR(MSG_IF);
    }
    *Param0=error_168;
    *Param1=n_169;
  }
}

void P_STRINGTOHEX64_convert (BOOL *error_164,INT64 *n_165,INT string_166_Dim0,BYTE *string_166)
{
  tSF_P_STRINGTOHEX64_convert SF;
  tSF_P_STRINGTOHEX64_convert *FP = &SF;
  {
    INT i_172;
    {
      i_172 = 0; 
      *n_165 = 0; 
      *error_164 = false; 
      if (((string_166_Dim0) > 0))
      {
        while ((( ! *error_164) && (i_172 < (string_166_Dim0))))
        {
          {
            INT nibble_173;
            {
              {
                F_hex_digit_171(FP,*((BYTE*)(((BYTE*)string_166)+(RNGCHK(i_172,string_166_Dim0)*sizeof(BYTE)))),error_164,&nibble_173);
              }
              if (*error_164)
              {
              }
              else
              if (true)
              {
                {
                  INT o_174;
                  {
                    i_172 = (i_172 + 1); 
                    *n_165 = LOGSHIFTLEFT(INT64,*n_165,4); 
                    *n_165 = (*n_165 + INT64INT(nibble_173)); 
                  }
                }
              }
              else
              {
                SETERR(MSG_IF);
              }
            }
          }
        }
      }
      else
      if (true)
      {
        *error_164 = true; 
      }
      else
      {
        SETERR(MSG_IF);
      }
      if ((i_172 > 16))
      {
        *error_164 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_REAL32TOSTRING_convert (INT *l_176,INT string_177_Dim0,BYTE *string_177,REAL32 r_178,INT m_179,INT n_180)
{
  tSF_P_REAL32TOSTRING_convert SF;
  tSF_P_REAL32TOSTRING_convert *FP = &SF;
  {
    INT len_181;
    BYTE tmp_182[100];
    {
      if (((m_179 == 0) && (n_180 == 0)))
      {
        {
          sprintf((char*)tmp_182,"%G",(REAL64)r_178);
          len_181 = strlen((char*)tmp_182);
        }
      }
      else
      if (((m_179 > 0) && (n_180 >= 0)))
      {
        {
          sprintf((char*)tmp_182,"%*.*f",m_179+n_180,n_180,(REAL64)r_178);
          len_181 = strlen((char*)tmp_182);
        }
      }
      else
      if (((m_179 == 0) && (n_180 >= 0)))
      {
        {
          sprintf((char*)tmp_182,"%*.*E",n_180+6,n_180,(REAL64)r_178);
          len_181 = strlen((char*)tmp_182);
        }
      }
      else
      {
        SETERR(MSG_IF);
      }
      if (((len_181 < 100) && ((string_177_Dim0) >= len_181)))
      {
        CHK(len_181==len_181,MSG_ASM);
        ASSIGN(((BYTE*)(((BYTE*)string_177)+((RNGCHKUB(0,len_181,string_177_Dim0),0)*sizeof(BYTE)))),((BYTE*)(((BYTE*)tmp_182)+((RNGCHKUB(0,len_181,100),0)*sizeof(BYTE)))),(len_181*sizeof(BYTE))); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      *l_176 = len_181; 
    }
  }
}

void P_REAL64TOSTRING_convert (INT *l_184,INT string_185_Dim0,BYTE *string_185,REAL64 r_186,INT m_187,INT n_188)
{
  tSF_P_REAL64TOSTRING_convert SF;
  tSF_P_REAL64TOSTRING_convert *FP = &SF;
  {
    INT len_189;
    BYTE tmp_190[100];
    {
      if (((m_187 == 0) && (n_188 == 0)))
      {
        {
          sprintf((char*)tmp_190,"%G",r_186);
          len_189 = strlen((char*)tmp_190);
        }
      }
      else
      if (((m_187 > 0) && (n_188 >= 0)))
      {
        {
          sprintf((char*)tmp_190,"%*.*f",m_187+n_188,n_188,r_186);
          len_189 = strlen((char*)tmp_190);
        }
      }
      else
      if (((m_187 == 0) && (n_188 >= 0)))
      {
        {
          sprintf((char*)tmp_190,"%*.*E",n_188+6,n_188,r_186);
          len_189 = strlen((char*)tmp_190);
        }
      }
      else
      {
        SETERR(MSG_IF);
      }
      if (((len_189 < 100) && ((string_185_Dim0) >= len_189)))
      {
        CHK(len_189==len_189,MSG_ASM);
        ASSIGN(((BYTE*)(((BYTE*)string_185)+((RNGCHKUB(0,len_189,string_185_Dim0),0)*sizeof(BYTE)))),((BYTE*)(((BYTE*)tmp_190)+((RNGCHKUB(0,len_189,100),0)*sizeof(BYTE)))),(len_189*sizeof(BYTE))); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      *l_184 = len_189; 
    }
  }
}

void P_STRINGTOREAL32_convert (BOOL *error_192,REAL32 *n_193,INT string_194_Dim0,BYTE *string_194)
{
  tSF_P_STRINGTOREAL32_convert SF;
  tSF_P_STRINGTOREAL32_convert *FP = &SF;
  {
    BYTE tmp_195[30];
    INT len_196;
    INT valid_len_197;
    {
      if (((string_194_Dim0) >= 30))
      {
        len_196 = 29; 
      }
      else
      if (true)
      {
        len_196 = (string_194_Dim0); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      CHK(len_196==len_196,MSG_ASM);
      ASSIGN(((BYTE*)(((BYTE*)tmp_195)+((RNGCHKUB(0,len_196,30),0)*sizeof(BYTE)))),((BYTE*)(((BYTE*)string_194)+((RNGCHKUB(0,len_196,string_194_Dim0),0)*sizeof(BYTE)))),(len_196*sizeof(BYTE))); 
      *((BYTE*)(((BYTE*)tmp_195)+(RNGCHK(len_196,30)*sizeof(BYTE)))) = 48; 
      {
        BYTE s_198;
        s_198 = (BYTE)*((BYTE*)(((BYTE*)string_194)+(0*sizeof(BYTE))));
        if (((((s_198 >= 48) && (s_198 <= 57)) || (s_198 == 43)) || (s_198 == 45)))
        {
          *error_192 = false; 
        }
        else
        if (true)
        {
          *error_192 = true; 
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
      { BYTE *v; *n_193 = (REAL32) strtod((char*)tmp_195,(char**)&v); valid_len_197= v-((BYTE*) tmp_195); }
      { if (errno==ERANGE) *error_192=true; }
      if ((len_196 > valid_len_197))
      {
        *error_192 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_STRINGTOREAL64_convert (BOOL *error_200,REAL64 *n_201,INT string_202_Dim0,BYTE *string_202)
{
  tSF_P_STRINGTOREAL64_convert SF;
  tSF_P_STRINGTOREAL64_convert *FP = &SF;
  {
    BYTE tmp_203[30];
    INT len_204;
    INT valid_len_205;
    {
      if (((string_202_Dim0) >= 30))
      {
        len_204 = 29; 
      }
      else
      if (true)
      {
        len_204 = (string_202_Dim0); 
      }
      else
      {
        SETERR(MSG_IF);
      }
      CHK(len_204==len_204,MSG_ASM);
      ASSIGN(((BYTE*)(((BYTE*)tmp_203)+((RNGCHKUB(0,len_204,30),0)*sizeof(BYTE)))),((BYTE*)(((BYTE*)string_202)+((RNGCHKUB(0,len_204,string_202_Dim0),0)*sizeof(BYTE)))),(len_204*sizeof(BYTE))); 
      *((BYTE*)(((BYTE*)tmp_203)+(RNGCHK(len_204,30)*sizeof(BYTE)))) = 48; 
      {
        BYTE s_206;
        s_206 = (BYTE)*((BYTE*)(((BYTE*)string_202)+(0*sizeof(BYTE))));
        if (((((s_206 >= 48) && (s_206 <= 57)) || (s_206 == 43)) || (s_206 == 45)))
        {
          *error_200 = false; 
        }
        else
        if (true)
        {
          *error_200 = true; 
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
      { BYTE *v; *n_201 = (REAL64) strtod((char*)tmp_203,(char**)&v); valid_len_205= v-((BYTE*) tmp_203); }
      { if (errno==ERANGE) *error_200=true; }
      if ((len_204 > valid_len_205))
      {
        *error_200 = true; 
      }
      else
      if (true)
      {
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_STRINGTOBOOL_convert (BOOL *error_208,BOOL *b_209,INT string_210_Dim0,BYTE *string_210)
{
  tSF_P_STRINGTOBOOL_convert SF;
  tSF_P_STRINGTOBOOL_convert *FP = &SF;
  {
    BYTE *true_212;
    BYTE *false_214;
    {
      static BYTE TMP[4] = {84,82,85,69};
      true_212 = TMP;
    }
    {
      static BYTE TMP[5] = {70,65,76,83,69};
      false_214 = TMP;
    }
    {
      *error_208 = false; 
      if (((string_210_Dim0) < 4))
      {
        *error_208 = true; 
      }
      else
      {
        INT i_215;
        for (i_215 = 0; i_215 != 0 + 4; i_215++)
        {
          if ((*((BYTE*)(((BYTE*)string_210)+(i_215*sizeof(BYTE)))) != *((BYTE*)(((BYTE*)true_212)+(i_215*sizeof(BYTE))))))
          {
            if (((string_210_Dim0) < 5))
            {
              *error_208 = true; 
            }
            else
            {
              INT i_216;
              for (i_216 = 0; i_216 != 0 + 5; i_216++)
              {
                if ((*((BYTE*)(((BYTE*)string_210)+(i_216*sizeof(BYTE)))) != *((BYTE*)(((BYTE*)false_214)+(i_216*sizeof(BYTE))))))
                {
                  *error_208 = true; 
                }
                else
                {
                  continue;
                }
                break;
              }
              if (i_216 == (0 + 5))
              if (true)
              {
                *b_209 = false; 
              }
              else
              {
                SETERR(MSG_IF);
              }
            }
          }
          else
          {
            continue;
          }
          break;
        }
        if (i_215 == (0 + 4))
        if (true)
        {
          *b_209 = true; 
        }
        else
        if (true)
        {
          *error_208 = true; 
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
    }
  }
}

void P_BOOLTOSTRING_convert (INT *len_218,INT string_219_Dim0,BYTE *string_219,BOOL b_220)
{
  tSF_P_BOOLTOSTRING_convert SF;
  tSF_P_BOOLTOSTRING_convert *FP = &SF;
  if (b_220)
  {
    {
      {
        BYTE *_V1_221;
        {
          static BYTE TMP[4] = {84,82,85,69};
          _V1_221 = TMP;
        }
        ASSIGN(((BYTE*)(((BYTE*)string_219)+((0)*sizeof(BYTE)))),_V1_221,(4*sizeof(BYTE))); 
      }
      *len_218 = 4; 
    }
  }
  else
  if (true)
  {
    {
      {
        BYTE *_V0_222;
        {
          static BYTE TMP[5] = {70,65,76,83,69};
          _V0_222 = TMP;
        }
        ASSIGN(((BYTE*)(((BYTE*)string_219)+((0)*sizeof(BYTE)))),_V0_222,(5*sizeof(BYTE))); 
      }
      *len_218 = 5; 
    }
  }
  else
  {
    SETERR(MSG_IF);
  }
}


/*}}}*/
#endif
