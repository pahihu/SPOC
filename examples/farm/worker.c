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
#include "dblmath.hdr"
typedef struct SF_P_95 tSF_P_95;
typedef struct SF_P_96 tSF_P_96;
typedef struct SF_P_97 tSF_P_97;
typedef struct SF_P_98 tSF_P_98;
typedef struct SF_P_forward_99 tSF_P_forward_99;
typedef struct SF_P_calculator_113 tSF_P_calculator_113;
typedef struct SF_P_return_121 tSF_P_return_121;
typedef struct SF_P_128 tSF_P_128;
typedef struct SF_P_129 tSF_P_129;
typedef struct SF_P_130 tSF_P_130;
typedef struct SF_P_131 tSF_P_131;
typedef struct SF_P_worker_worker tSF_P_worker_worker;
void P_worker_worker (tSF_P_worker_worker *FP);
struct SF_P_95
{
  tHeader _Header;
  tSF_P_forward_99 *Chain;
};
struct SF_P_96
{
  tHeader _Header;
  tSF_P_forward_99 *Chain;
};
struct SF_P_97
{
  tHeader _Header;
  tSF_P_forward_99 *Chain;
};
struct SF_P_98
{
  tHeader _Header;
  tSF_P_forward_99 *Chain;
};
struct SF_P_forward_99
{
  tHeader _Header;
  tSF_P_worker_worker *Chain;
  CHAN *toThis_78;
  CHAN *toNext_79;
  CHAN *toWorker_80;
  CHAN *GiveMeMore_81;
  INT EqnType_82;
  INT Samples_83;
  REAL64 Power_84;
  REAL64 deltax_85;
  REAL64 x_86;
  REAL64 bufferx_87;
  REAL64 buf2x_88;
  BOOL More_89;
  BOOL Buffered_90;
  BOOL Busy_91;
  BOOL Armed_92;
  BOOL Buffered2_93;
  INT Node_94;
  union
  {
    struct
    {
      tTask _T0;
      tSF_P_95 _C0;
      tTask _T1;
      tSF_P_96 _C1;
    } _S0;
    struct
    {
      tTask _T2;
      tSF_P_97 _C2;
      tTask _T3;
      tSF_P_98 _C3;
    } _S1;
  } _U0;
};
struct SF_P_calculator_113
{
  tHeader _Header;
  tSF_P_worker_worker *Chain;
  CHAN *toWorker_100;
  CHAN *fromWorker_101;
  CHAN *GiveMeMore_102;
  INT EqnType_103;
  INT Samples_104;
  INT Node_105;
  REAL64 Power_106;
  REAL64 deltax_107;
  REAL64 x_108;
  REAL64 sum_109;
  REAL64 value_110;
  BOOL More_111;
};
struct SF_P_return_121
{
  tHeader _Header;
  tSF_P_worker_worker *Chain;
  CHAN *fromThis_114;
  CHAN *fromNext_115;
  CHAN *fromWorker_116;
  REAL64 sum_117;
  BOOL WorkerActive_118;
  BOOL NextActive_119;
  INT Node_120;
};
struct SF_P_128
{
  tHeader _Header;
  tSF_P_130 *Chain;
  tSF_P_forward_99 _C4;
};
struct SF_P_129
{
  tHeader _Header;
  tSF_P_130 *Chain;
  tSF_P_return_121 _C6;
};
struct SF_P_130
{
  tHeader _Header;
  tSF_P_worker_worker *Chain;
  tTask _T4;
  tSF_P_128 _C5;
  tTask _T5;
  tSF_P_129 _C7;
};
struct SF_P_131
{
  tHeader _Header;
  tSF_P_worker_worker *Chain;
  tSF_P_calculator_113 _C9;
};
struct SF_P_worker_worker
{
  tHeader _Header;
  void *Chain;
  CHAN *toThis_74;
  CHAN *fromThis_75;
  CHAN *toNext_76;
  CHAN *fromNext_77;
  CHAN toWorker_123_CHAN;
  CHAN *toWorker_123;
  CHAN fromWorker_125_CHAN;
  CHAN *fromWorker_125;
  CHAN GiveMeMore_127_CHAN;
  CHAN *GiveMeMore_127;
  tTask _T6;
  tSF_P_130 _C8;
  tTask _T7;
  tSF_P_131 _C10;
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
  {"void P_worker_worker (tSF_P_worker_worker *FP);","P_worker_worker",sizeof(tSF_P_worker_worker)},

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
    name="worker.hdr";
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
    fprintf(f,"#ifndef __worker_INCLUDES__\n");
    fprintf(f,"#define __worker_INCLUDES__\n");
    
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
static void P_95 (tSF_P_95 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_Header.Temp.VBYTE = 0;
        OUTPUT1(FP->Chain->toWorker_80,&FP->_Header.Temp.VBYTE,1);
      CASE(1):  
        OUTPUT4(FP->Chain->toWorker_80,&FP->Chain->EqnType_82,2);
      CASE(2):  
        OUTPUT4(FP->Chain->toWorker_80,&FP->Chain->Samples_83,3);
      CASE(3):  
        OUTPUT8(FP->Chain->toWorker_80,&FP->Chain->deltax_85,4);
      CASE(4):  
        OUTPUT8(FP->Chain->toWorker_80,&FP->Chain->Power_84,5);
      CASE(5):  
        OUTPUT4(FP->Chain->toWorker_80,&FP->Chain->Node_94,6);
      CASE(6):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_96 (tSF_P_96 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_Header.Temp.VBYTE = 0;
        OUTPUT1(FP->Chain->toNext_79,&FP->_Header.Temp.VBYTE,1);
      CASE(1):  
        OUTPUT4(FP->Chain->toNext_79,&FP->Chain->EqnType_82,2);
      CASE(2):  
        OUTPUT4(FP->Chain->toNext_79,&FP->Chain->Samples_83,3);
      CASE(3):  
        OUTPUT8(FP->Chain->toNext_79,&FP->Chain->deltax_85,4);
      CASE(4):  
        OUTPUT8(FP->Chain->toNext_79,&FP->Chain->Power_84,5);
      CASE(5):  
        FP->_Header.Temp.VINT = (FP->Chain->Node_94 + 1);
        OUTPUT4(FP->Chain->toNext_79,&FP->_Header.Temp.VINT,6);
      CASE(6):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_97 (tSF_P_97 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->Chain->toNext_79,&FP->_Header.Temp.VBYTE,1);
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_98 (tSF_P_98 *FP)
{
  FP->Chain->More_89 = false; 
  ENDP();
}

static void P_forward_99 (tSF_P_forward_99 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->More_89 = true; 
        FP->Buffered_90 = false; 
        FP->Buffered2_93 = false; 
        FP->Busy_91 = false; 
        FP->Armed_92 = false; 
        GOTO(1);
      CASE(2):  
        ALT();
        ENBC(true,FP->toThis_78);
        ENBC(true,FP->GiveMeMore_81);
        ALTWT(3);
      CASE(3):  
        {
          BOOL TMP = false;
          TMP |= DISC(4,true,FP->toThis_78);
          TMP |= DISC(5,true,FP->GiveMeMore_81);
        }
        ALTEND();
      CASE(4):  
        INPUT1(FP->toThis_78,&FP->_Header.Temp.VBYTE,7);
      CASE(7):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 0: GOTO (9);
          case 1: GOTO (10);
          case 4: GOTO (11);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(9):  
        INPUT4(FP->toThis_78,&FP->EqnType_82,12);
      CASE(12):  
        INPUT4(FP->toThis_78,&FP->Samples_83,13);
      CASE(13):  
        INPUT8(FP->toThis_78,&FP->deltax_85,14);
      CASE(14):  
        INPUT8(FP->toThis_78,&FP->Power_84,15);
      CASE(15):  
        INPUT4(FP->toThis_78,&FP->Node_94,16);
      CASE(16):  
        STARTP(&FP->_U0._S0._T0,P_95,&FP->_U0._S0._C0,"P_95",1);
        STARTP(&FP->_U0._S0._T1,P_96,&FP->_U0._S0._C1,"P_96",1);
        WAITP(17);
      CASE(17):  
        GOTO(8);
      CASE(10):  
        INPUT8(FP->toThis_78,&FP->x_86,18);
      CASE(18):  
        if (!( ! FP->Busy_91)) GOTO(20);
        FP->_Header.Temp.VBYTE = 1;
        OUTPUT1(FP->toWorker_80,&FP->_Header.Temp.VBYTE,21);
      CASE(21):  
        OUTPUT8(FP->toWorker_80,&FP->x_86,22);
      CASE(22):  
        FP->Busy_91 = true; 
        FP->Armed_92 = false; 
        GOTO(19);
      CASE(20):  
        if (!( ! FP->Buffered_90)) GOTO(23);
        {
          FP->bufferx_87 = FP->x_86; 
          FP->Buffered_90 = true; 
        }
        GOTO(19);
      CASE(23):  
        if (!(FP->Armed_92 && ( ! FP->Buffered2_93))) GOTO(24);
        {
          FP->Buffered2_93 = true; 
          FP->buf2x_88 = FP->x_86; 
        }
        GOTO(19);
      CASE(24):  
        if (!true) GOTO(25);
        FP->_Header.Temp.VBYTE = 1;
        OUTPUT1(FP->toNext_79,&FP->_Header.Temp.VBYTE,26);
      CASE(26):  
        OUTPUT8(FP->toNext_79,&FP->x_86,27);
      CASE(27):  
        GOTO(19);
      CASE(25):  
        SETERR(MSG_IF);
      CASE(19):  
        GOTO(8);
      CASE(11):  
        STARTP(&FP->_U0._S1._T2,P_97,&FP->_U0._S1._C2,"P_97",1);
        STARTP(&FP->_U0._S1._T3,P_98,&FP->_U0._S1._C3,"P_98",1);
        WAITP(28);
      CASE(28):  
        GOTO(8);
      CASE(8):  
        GOTO(6);
      CASE(5):  
        INPUT1(FP->GiveMeMore_81,&FP->_Header.Temp.VBYTE,29);
      CASE(29):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 3: GOTO (31);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(31):  
        if (!( ! FP->Armed_92)) GOTO(33);
        FP->Armed_92 = true; 
        GOTO(32);
      CASE(33):  
        if (!FP->Buffered_90) GOTO(34);
        FP->_Header.Temp.VBYTE = 1;
        OUTPUT1(FP->toWorker_80,&FP->_Header.Temp.VBYTE,35);
      CASE(35):  
        OUTPUT8(FP->toWorker_80,&FP->bufferx_87,36);
      CASE(36):  
        FP->Busy_91 = true; 
        FP->Armed_92 = false; 
        FP->Buffered_90 = FP->Buffered2_93; 
        FP->bufferx_87 = FP->buf2x_88; 
        FP->Buffered2_93 = false; 
        GOTO(32);
      CASE(34):  
        if (!true) GOTO(37);
        FP->Busy_91 = false; 
        GOTO(32);
      CASE(37):  
        SETERR(MSG_IF);
      CASE(32):  
        GOTO(30);
      CASE(30):  
        GOTO(6);
      CASE(6):  
      CASE(1):  
        if (((FP->More_89 || FP->Buffered_90) || FP->Busy_91))
        {
          GOTO(2);
        }
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->toWorker_80,&FP->_Header.Temp.VBYTE,38);
      CASE(38):  
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_calculator_113 (tSF_P_calculator_113 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->More_111 = true; 
        GOTO(1);
      CASE(2):  
        INPUT1(FP->toWorker_100,&FP->_Header.Temp.VBYTE,3);
      CASE(3):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 0: GOTO (5);
          case 1: GOTO (6);
          case 4: GOTO (7);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(5):  
        INPUT4(FP->toWorker_100,&FP->EqnType_103,8);
      CASE(8):  
        INPUT4(FP->toWorker_100,&FP->Samples_104,9);
      CASE(9):  
        INPUT8(FP->toWorker_100,&FP->deltax_107,10);
      CASE(10):  
        INPUT8(FP->toWorker_100,&FP->Power_106,11);
      CASE(11):  
        INPUT4(FP->toWorker_100,&FP->Node_105,12);
      CASE(12):  
        GOTO(4);
      CASE(6):  
        INPUT8(FP->toWorker_100,&FP->x_108,13);
      CASE(13):  
        FP->sum_109 = 0.0e+00; 
        {
          INT i_112;
          INT i_112_End;
          i_112 = 0;i_112_End = i_112 + (FP->Samples_104 + 1);
          for (; i_112 != i_112_End; i_112++)
          {
            {
              if ((FP->EqnType_103 == 1))
              {
                FP->value_110 = F_DPOWER_dblmath(FP->x_108,FP->Power_106); 
              }
              else
              if ((FP->EqnType_103 == 2))
              {
                FP->value_110 = F_DEXP_dblmath(( - F_DPOWER_dblmath(FP->x_108,FP->Power_106))); 
              }
              else
              {
                SETERR(MSG_IF);
              }
              if ((i_112 == 0))
              {
                FP->sum_109 = (FP->value_110 / 2.0e+00); 
              }
              else
              if ((i_112 == FP->Samples_104))
              {
                FP->sum_109 = (FP->sum_109 + (FP->value_110 / 2.0e+00)); 
              }
              else
              if (true)
              {
                FP->sum_109 = (FP->sum_109 + FP->value_110); 
              }
              else
              {
                SETERR(MSG_IF);
              }
              FP->x_108 = (FP->x_108 + FP->deltax_107); 
            }
          }
        }
        FP->_Header.Temp.VBYTE = 3;
        OUTPUT1(FP->GiveMeMore_102,&FP->_Header.Temp.VBYTE,14);
      CASE(14):  
        FP->_Header.Temp.VBYTE = 2;
        OUTPUT1(FP->fromWorker_101,&FP->_Header.Temp.VBYTE,15);
      CASE(15):  
        OUTPUT8(FP->fromWorker_101,&FP->sum_109,16);
      CASE(16):  
        OUTPUT4(FP->fromWorker_101,&FP->Node_105,17);
      CASE(17):  
        FP->_Header.Temp.VBYTE = 3;
        OUTPUT1(FP->GiveMeMore_102,&FP->_Header.Temp.VBYTE,18);
      CASE(18):  
        GOTO(4);
      CASE(7):  
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->fromWorker_101,&FP->_Header.Temp.VBYTE,19);
      CASE(19):  
        FP->More_111 = false; 
        GOTO(4);
      CASE(4):  
      CASE(1):  
        if (FP->More_111)
        {
          GOTO(2);
        }
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_return_121 (tSF_P_return_121 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->NextActive_119 = true; 
        FP->WorkerActive_118 = true; 
        GOTO(1);
      CASE(2):  
        ALT();
        ENBC(true,FP->fromNext_115);
        ENBC(true,FP->fromWorker_116);
        ALTWT(3);
      CASE(3):  
        {
          BOOL TMP = false;
          TMP |= DISC(4,true,FP->fromNext_115);
          TMP |= DISC(5,true,FP->fromWorker_116);
        }
        ALTEND();
      CASE(4):  
        INPUT1(FP->fromNext_115,&FP->_Header.Temp.VBYTE,7);
      CASE(7):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 2: GOTO (9);
          case 4: GOTO (10);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(9):  
        INPUT8(FP->fromNext_115,&FP->sum_117,11);
      CASE(11):  
        INPUT4(FP->fromNext_115,&FP->Node_120,12);
      CASE(12):  
        FP->_Header.Temp.VBYTE = 2;
        OUTPUT1(FP->fromThis_114,&FP->_Header.Temp.VBYTE,13);
      CASE(13):  
        OUTPUT8(FP->fromThis_114,&FP->sum_117,14);
      CASE(14):  
        OUTPUT4(FP->fromThis_114,&FP->Node_120,15);
      CASE(15):  
        GOTO(8);
      CASE(10):  
        FP->NextActive_119 = false; 
        GOTO(8);
      CASE(8):  
        GOTO(6);
      CASE(5):  
        INPUT1(FP->fromWorker_116,&FP->_Header.Temp.VBYTE,16);
      CASE(16):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 2: GOTO (18);
          case 4: GOTO (19);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(18):  
        INPUT8(FP->fromWorker_116,&FP->sum_117,20);
      CASE(20):  
        INPUT4(FP->fromWorker_116,&FP->Node_120,21);
      CASE(21):  
        FP->_Header.Temp.VBYTE = 2;
        OUTPUT1(FP->fromThis_114,&FP->_Header.Temp.VBYTE,22);
      CASE(22):  
        OUTPUT8(FP->fromThis_114,&FP->sum_117,23);
      CASE(23):  
        OUTPUT4(FP->fromThis_114,&FP->Node_120,24);
      CASE(24):  
        GOTO(17);
      CASE(19):  
        FP->WorkerActive_118 = false; 
        GOTO(17);
      CASE(17):  
        GOTO(6);
      CASE(6):  
      CASE(1):  
        if ((FP->NextActive_119 || FP->WorkerActive_118))
        {
          GOTO(2);
        }
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->fromThis_114,&FP->_Header.Temp.VBYTE,25);
      CASE(25):  
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_128 (tSF_P_128 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_C4.toThis_78=FP->Chain->Chain->toThis_74;
        FP->_C4.toNext_79=FP->Chain->Chain->toNext_76;
        FP->_C4.toWorker_80=FP->Chain->Chain->toWorker_123;
        FP->_C4.GiveMeMore_81=FP->Chain->Chain->GiveMeMore_127;
        FP->_C4.Chain = FP->Chain->Chain;
        CALL(P_forward_99,&FP->_C4,1,"P_forward_99");
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_129 (tSF_P_129 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_C6.fromThis_114=FP->Chain->Chain->fromThis_75;
        FP->_C6.fromNext_115=FP->Chain->Chain->fromNext_77;
        FP->_C6.fromWorker_116=FP->Chain->Chain->fromWorker_125;
        FP->_C6.Chain = FP->Chain->Chain;
        CALL(P_return_121,&FP->_C6,1,"P_return_121");
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_130 (tSF_P_130 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        STARTP(&FP->_T4,P_128,&FP->_C5,"P_128",1);
        STARTP(&FP->_T5,P_129,&FP->_C7,"P_129",1);
        WAITP(1);
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_131 (tSF_P_131 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->_C9.toWorker_100=FP->Chain->toWorker_123;
        FP->_C9.fromWorker_101=FP->Chain->fromWorker_125;
        FP->_C9.GiveMeMore_102=FP->Chain->GiveMeMore_127;
        FP->_C9.Chain = FP->Chain;
        CALL(P_calculator_113,&FP->_C9,1,"P_calculator_113");
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

void P_worker_worker (tSF_P_worker_worker *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->toWorker_123 = &FP->toWorker_123_CHAN;
        INITCH(FP->toWorker_123);
        FP->fromWorker_125 = &FP->fromWorker_125_CHAN;
        INITCH(FP->fromWorker_125);
        FP->GiveMeMore_127 = &FP->GiveMeMore_127_CHAN;
        INITCH(FP->GiveMeMore_127);
        STARTP(&FP->_T6,P_130,&FP->_C8,"P_130",0);
        STARTP(&FP->_T7,P_131,&FP->_C10,"P_131",1);
        WAITP(1);
      CASE(1):  
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}


/*}}}*/
#endif
