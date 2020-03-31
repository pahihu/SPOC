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
typedef struct SF_P_wait_socket_168 tSF_P_wait_socket_168;
typedef struct SF_P_open_session_169 tSF_P_open_session_169;
typedef struct SF_P_close_session_170 tSF_P_close_session_170;
typedef struct SF_P_iserver_175 tSF_P_iserver_175;
typedef struct SF_P_spoc_iserver_spserver tSF_P_spoc_iserver_spserver;
typedef struct SF_P_wait_socket_182 tSF_P_wait_socket_182;
typedef struct SF_P_aserver_out_186 tSF_P_aserver_out_186;
typedef struct SF_P_aserver_in_190 tSF_P_aserver_in_190;
typedef struct SF_P_open_session_191 tSF_P_open_session_191;
typedef struct SF_P_close_session_192 tSF_P_close_session_192;
typedef struct SF_P_199 tSF_P_199;
typedef struct SF_P_202 tSF_P_202;
typedef struct SF_P_spoc_aserver_spserver tSF_P_spoc_aserver_spserver;
void P_spoc_aserver_spserver (tSF_P_spoc_aserver_spserver *FP);
struct SF_P_wait_socket_168
{
  tHeader _Header;
  tSF_P_spoc_iserver_spserver *Chain;
  BOOL true_165;
  CHAN c_167_CHAN;
  CHAN *c_167;
};
struct SF_P_open_session_169
{
  tHeader _Header;
  tSF_P_spoc_iserver_spserver *Chain;
};
struct SF_P_close_session_170
{
  tHeader _Header;
  tSF_P_spoc_iserver_spserver *Chain;
};
struct SF_P_iserver_175
{
  tHeader _Header;
  tSF_P_spoc_iserver_spserver *Chain;
  BYTE *buffer_171;
  INT buffer_171_Dim0;
  INT16 *length_172;
  INT16 len_173;
  INT max_size_174;
  tSF_P_wait_socket_168 _C0;
};
struct SF_P_spoc_iserver_spserver
{
  tHeader _Header;
  void *Chain;
  CHAN *fs_156;
  CHAN *ts_157;
  CHAN fs_158_CHAN;
  CHAN *fs_158;
  CHAN ts_159_CHAN;
  CHAN *ts_159;
  BOOL serving_161;
  BOOL flag_162;
  INT16 length_163;
  BYTE buffer_164[512];
  union
  {
    tSF_P_iserver_175 _C2;
  } _U0;
};
struct SF_P_wait_socket_182
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
  BOOL true_179;
  CHAN c_181_CHAN;
  CHAN *c_181;
};
struct SF_P_aserver_out_186
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
};
struct SF_P_aserver_in_190
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
  BYTE *buffer_187;
  INT buffer_187_Dim0;
  INT *length_188;
  tSF_P_wait_socket_182 _C0;
};
struct SF_P_open_session_191
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
};
struct SF_P_close_session_192
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
};
struct SF_P_199
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
  INT length_197;
  BYTE buffer_198[1024];
};
struct SF_P_202
{
  tHeader _Header;
  tSF_P_spoc_aserver_spserver *Chain;
  INT length_200;
  BYTE buffer_201[1024];
  tSF_P_aserver_in_190 _C4;
};
struct SF_P_spoc_aserver_spserver
{
  tHeader _Header;
  void *Chain;
  CHAN *fs_177;
  CHAN *ts_178;
  CHAN fs_195_CHAN;
  CHAN *fs_195;
  CHAN ts_196_CHAN;
  CHAN *ts_196;
  union
  {
    struct
    {
      tTask _T0;
      tSF_P_199 _C3;
      tTask _T1;
      tSF_P_202 _C5;
    } _S0;
  } _U1;
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
  {"void P_spoc_iserver_spserver (tSF_P_spoc_iserver_spserver *FP);","P_spoc_iserver_spserver",sizeof(tSF_P_spoc_iserver_spserver)},
  {"void P_spoc_aserver_spserver (tSF_P_spoc_aserver_spserver *FP);","P_spoc_aserver_spserver",sizeof(tSF_P_spoc_aserver_spserver)},

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
    name="spserver.hdr";
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
    fprintf(f,"#ifndef __spserver_INCLUDES__\n");
    fprintf(f,"#define __spserver_INCLUDES__\n");
    
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
#if (defined USE_SOCKETS || defined USE_NONBLOCKING_SOCKETS)
  extern int  ServeBuffer  (char *, short*);
  extern void ServeSockets (int, int);
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <stdio.h>
  #include <errno.h>
  static int occam_to_iserver[2];
  static int iserver_to_occam[2];
  #define reader_end 0
  #define writer_end 1
#else
  extern void HostBegin();
  extern void HostEnd();
#endif
#line   47 "spserver.occ"
static void P_wait_socket_168 (tSF_P_wait_socket_168 *FP)
#line   47 "spserver.occ"
{
#line   47 "spserver.occ"
  while(true)
#line   47 "spserver.occ"
  {
#line   47 "spserver.occ"
    switch(FP->_Header.IP)
#line   47 "spserver.occ"
    {
#line   47 "spserver.occ"
      CASE(0):  
#line   50 "spserver.occ"
        #ifdef USE_NONBLOCKING_SOCKETS
#line   51 "spserver.occ"
        WAITSOCKET (iserver_to_occam[reader_end]);
#line   52 "spserver.occ"
        #endif
#line   59 "spserver.occ"
        FP->true_165 = true; 
#line   61 "spserver.occ"
        if (!FP->true_165) GOTO(2);
#line   62 "spserver.occ"
        GOTO(1);
#line   62 "spserver.occ"
      CASE(2):  
#line   63 "spserver.occ"
        if (!true) GOTO(3);
#line   64 "spserver.occ"
        FP->c_167 = &FP->c_167_CHAN;
#line   64 "spserver.occ"
        INITCH(FP->c_167);
#line   65 "spserver.occ"
        FP->_Header.Temp.VINT = 0;
#line   65 "spserver.occ"
        OUTPUT4(FP->c_167,&FP->_Header.Temp.VINT,4);
#line   65 "spserver.occ"
      CASE(4):  
#line   65 "spserver.occ"
        GOTO(1);
#line   65 "spserver.occ"
      CASE(3):  
#line   65 "spserver.occ"
        SETERR(MSG_IF);
#line   65 "spserver.occ"
      CASE(1):  
#line   65 "spserver.occ"
        RETURN();
#line   65 "spserver.occ"
      default: SETERR(MSG_IP);
#line   65 "spserver.occ"
    }
#line   65 "spserver.occ"
  }
#line   65 "spserver.occ"
}

#line   69 "spserver.occ"
static void P_open_session_169 (void)
#line   69 "spserver.occ"
{
#line   69 "spserver.occ"
  tSF_P_open_session_169 SF;
#line   69 "spserver.occ"
  tSF_P_open_session_169 *FP = &SF;
#line   70 "spserver.occ"
  {
#line   71 "spserver.occ"
    #if (defined USE_SOCKETS || defined USE_NONBLOCKING_SOCKETS)
#line   72 "spserver.occ"
    {
#line   73 "spserver.occ"
      static int identity = -1;
#line   74 "spserver.occ"
      if (identity < 0)
#line   75 "spserver.occ"
      {
#line   76 "spserver.occ"
        #ifdef linux
#line   77 "spserver.occ"
        if (socketpair(AF_UNIX,SOCK_STREAM,0,iserver_to_occam) < 0 || 
#line   78 "spserver.occ"
            socketpair(AF_UNIX,SOCK_STREAM,0,occam_to_iserver) < 0)
#line   79 "spserver.occ"
        #else
#line   80 "spserver.occ"
        if (pipe (iserver_to_occam) < 0 ||
#line   81 "spserver.occ"
            pipe (occam_to_iserver) < 0)
#line   82 "spserver.occ"
        #endif
#line   83 "spserver.occ"
        {
#line   84 "spserver.occ"
          fprintf(stderr,"Could not open stream socket pairs\n");
#line   85 "spserver.occ"
          exit(1);
#line   86 "spserver.occ"
        }
#line   87 "spserver.occ"
        if ((identity = fork()) == -1)
#line   88 "spserver.occ"
        {
#line   89 "spserver.occ"
          fprintf(stderr,"Could not create iserver\n");
#line   90 "spserver.occ"
          exit(1);
#line   91 "spserver.occ"
        }
#line   92 "spserver.occ"
        else if (identity == 0)
#line   93 "spserver.occ"
        {
#line   94 "spserver.occ"
          ServeSockets (iserver_to_occam[writer_end], occam_to_iserver[reader_end]);
#line   95 "spserver.occ"
          close(iserver_to_occam[reader_end]);
#line   96 "spserver.occ"
          close(iserver_to_occam[writer_end]);
#line   97 "spserver.occ"
          close(occam_to_iserver[reader_end]);
#line   98 "spserver.occ"
          close(occam_to_iserver[writer_end]);
#line   99 "spserver.occ"
          exit(0);
#line  100 "spserver.occ"
        }
#line  101 "spserver.occ"
        #ifdef USE_NONBLOCKING_SOCKETS
#line  102 "spserver.occ"
        else
#line  103 "spserver.occ"
          EnableSocket (iserver_to_occam[reader_end]);
#line  104 "spserver.occ"
        #endif
#line  105 "spserver.occ"
      }
#line  106 "spserver.occ"
    }
#line  107 "spserver.occ"
    #else
#line  108 "spserver.occ"
    HostBegin();
#line  109 "spserver.occ"
    #endif
#line  109 "spserver.occ"
  }
#line  109 "spserver.occ"
}

#line  113 "spserver.occ"
static void P_close_session_170 (void)
#line  113 "spserver.occ"
{
#line  113 "spserver.occ"
  tSF_P_close_session_170 SF;
#line  113 "spserver.occ"
  tSF_P_close_session_170 *FP = &SF;
#line  114 "spserver.occ"
  {
#line  115 "spserver.occ"
    #if (defined USE_SOCKETS || defined USE_NONBLOCKING_SOCKETS)
#line  116 "spserver.occ"
    #ifdef USE_NONBLOCKING_SOCKETS
#line  117 "spserver.occ"
    DisableSocket(iserver_to_occam[reader_end]);
#line  118 "spserver.occ"
    #endif
#line  119 "spserver.occ"
    close(iserver_to_occam[reader_end]);
#line  120 "spserver.occ"
    close(iserver_to_occam[writer_end]);
#line  121 "spserver.occ"
    close(occam_to_iserver[reader_end]);
#line  122 "spserver.occ"
    close(occam_to_iserver[writer_end]);
#line  123 "spserver.occ"
    #else
#line  124 "spserver.occ"
    HostEnd();
#line  125 "spserver.occ"
    #endif
#line  125 "spserver.occ"
  }
#line  125 "spserver.occ"
}

#line  129 "spserver.occ"
static void P_iserver_175 (tSF_P_iserver_175 *FP)
#line  129 "spserver.occ"
{
#line  129 "spserver.occ"
  while(true)
#line  129 "spserver.occ"
  {
#line  129 "spserver.occ"
    switch(FP->_Header.IP)
#line  129 "spserver.occ"
    {
#line  129 "spserver.occ"
      CASE(0):  
#line  131 "spserver.occ"
        FP->max_size_174 = (INT)(FP->buffer_171_Dim0);
#line  133 "spserver.occ"
        #if (defined USE_SOCKETS || defined USE_NONBLOCKING_SOCKETS)
#line  134 "spserver.occ"
        errno = 0;
#line  135 "spserver.occ"
        while ((FP->len_173 = write(occam_to_iserver[writer_end], (char *) &*FP->length_172, 2)) <= 0)
#line  136 "spserver.occ"
          if (errno != EAGAIN && errno != EINTR)
#line  137 "spserver.occ"
          {
#line  138 "spserver.occ"
            fprintf(stderr,"Error writing length to client socket (errno %d)\n", errno);
#line  139 "spserver.occ"
            exit(1);
#line  140 "spserver.occ"
          }
#line  141 "spserver.occ"
          else
#line  142 "spserver.occ"
            errno = 0;
#line  143 "spserver.occ"
        if (FP->len_173 != 2)
#line  144 "spserver.occ"
          {
#line  145 "spserver.occ"
            fprintf(stderr,"Error - Only wrote %d out of %d bytes\n", FP->len_173, 2);
#line  146 "spserver.occ"
            exit(1);
#line  147 "spserver.occ"
          }
#line  148 "spserver.occ"
        while ((FP->len_173 = write(occam_to_iserver[writer_end], (char *) &*FP->buffer_171+2, (int) *FP->length_172)) <= 0)
#line  149 "spserver.occ"
          if (errno != EAGAIN && errno != EINTR)
#line  150 "spserver.occ"
          {
#line  151 "spserver.occ"
            fprintf(stderr,"Error writing packet to client socket (errno %d)\n", errno);
#line  152 "spserver.occ"
            exit(1);
#line  153 "spserver.occ"
          }
#line  154 "spserver.occ"
          else
#line  155 "spserver.occ"
            errno = 0;
#line  156 "spserver.occ"
        if (FP->len_173 != *FP->length_172)
#line  157 "spserver.occ"
          {
#line  158 "spserver.occ"
            fprintf(stderr,"Error - Only wrote %d out of %d bytes\n", FP->len_173, *FP->length_172);
#line  159 "spserver.occ"
            exit(1);
#line  160 "spserver.occ"
          }
#line  161 "spserver.occ"
        #ifdef USE_NONBLOCKING_SOCKETS
#line  162 "spserver.occ"
        FP->_C0.Chain = FP->Chain;
#line  162 "spserver.occ"
        CALL(P_wait_socket_168,&FP->_C0,1,"P_wait_socket_168");
#line  162 "spserver.occ"
      CASE(1):  
#line  163 "spserver.occ"
        #endif
#line  164 "spserver.occ"
        while ((FP->len_173 = read(iserver_to_occam[reader_end], (char *) &*FP->length_172, 2)) <= 0)
#line  165 "spserver.occ"
          if (errno != EAGAIN && errno != EINTR)
#line  166 "spserver.occ"
          {
#line  167 "spserver.occ"
            fprintf(stderr,"Error reading length from client socket (errno %d)\n", errno);
#line  168 "spserver.occ"
            exit(1);
#line  169 "spserver.occ"
          }
#line  170 "spserver.occ"
          else
#line  171 "spserver.occ"
            errno = 0;
#line  172 "spserver.occ"
        if (FP->len_173 != 2)
#line  173 "spserver.occ"
          {
#line  174 "spserver.occ"
            fprintf(stderr,"Error - Only read %d out of %d bytes\n", FP->len_173, 2);
#line  175 "spserver.occ"
            exit(1);
#line  176 "spserver.occ"
          }
#line  177 "spserver.occ"
        if (*FP->length_172 == 0 || *FP->length_172 > FP->max_size_174-2)
#line  178 "spserver.occ"
        {
#line  179 "spserver.occ"
          fprintf(stderr,"Error reading from client socket - bad length (%d bytes)\n", *FP->length_172);
#line  180 "spserver.occ"
          exit(1);
#line  181 "spserver.occ"
        }
#line  182 "spserver.occ"
        while ((FP->len_173 = read(iserver_to_occam[reader_end], (char *) &*FP->buffer_171+2, (int) *FP->length_172)) <= 0)
#line  183 "spserver.occ"
          if (errno != EAGAIN && errno != EINTR)
#line  184 "spserver.occ"
          {
#line  185 "spserver.occ"
            fprintf(stderr,"Error reading packet from client socket (errno %d)\n",errno);
#line  186 "spserver.occ"
            exit(1);
#line  187 "spserver.occ"
          }
#line  188 "spserver.occ"
          else
#line  189 "spserver.occ"
            errno = 0;
#line  190 "spserver.occ"
        if (FP->len_173 != *FP->length_172)
#line  191 "spserver.occ"
          {
#line  192 "spserver.occ"
            fprintf(stderr,"Error - Only read %d out of %d bytes\n", FP->len_173, *FP->length_172);
#line  193 "spserver.occ"
            exit(1);
#line  194 "spserver.occ"
          }
#line  195 "spserver.occ"
        #else
#line  196 "spserver.occ"
        ServeBuffer((char*) &*FP->buffer_171, (short*) &*FP->length_172);
#line  197 "spserver.occ"
        #endif
#line  197 "spserver.occ"
        RETURN();
#line  197 "spserver.occ"
      default: SETERR(MSG_IP);
#line  197 "spserver.occ"
    }
#line  197 "spserver.occ"
  }
#line  197 "spserver.occ"
}

#line   37 "spserver.occ"
void P_spoc_iserver_spserver (tSF_P_spoc_iserver_spserver *FP)
#line   37 "spserver.occ"
{
#line   37 "spserver.occ"
  while(true)
#line   37 "spserver.occ"
  {
#line   37 "spserver.occ"
    switch(FP->_Header.IP)
#line   37 "spserver.occ"
    {
#line   37 "spserver.occ"
      CASE(0):  
#line   39 "spserver.occ"
        FP->fs_158 = (CHAN*)FP->fs_156;
#line   40 "spserver.occ"
        FP->ts_159 = (CHAN*)FP->ts_157;
#line  202 "spserver.occ"
        FP->serving_161 = true; 
#line  204 "spserver.occ"
        P_open_session_169();
#line  206 "spserver.occ"
        GOTO(1);
#line  206 "spserver.occ"
      CASE(2):  
#line  208 "spserver.occ"
        INPUT2(FP->ts_159,&FP->length_163,3);
#line  208 "spserver.occ"
      CASE(3):  
#line  208 "spserver.occ"
        CHK(FP->length_163<=510,MSG_CITL);
#line  208 "spserver.occ"
        INPUT(FP->ts_159,((BYTE*)(((BYTE*)FP->buffer_164)+((2)*sizeof(BYTE)))),(FP->length_163)*(sizeof(BYTE)),4);
#line  208 "spserver.occ"
      CASE(4):  
#line  211 "spserver.occ"
        if ((*((BYTE*)(((BYTE*)FP->buffer_164)+(2*sizeof(BYTE)))) == 35))
#line  211 "spserver.occ"
        {
#line  212 "spserver.occ"
          FP->serving_161 = false; 
#line  212 "spserver.occ"
        }
#line  212 "spserver.occ"
        else
#line  213 "spserver.occ"
        if (true)
#line  213 "spserver.occ"
        {
#line  214 "spserver.occ"
        }
#line  214 "spserver.occ"
        else
#line  214 "spserver.occ"
        {
#line  214 "spserver.occ"
          SETERR(MSG_IF);
#line  214 "spserver.occ"
        }
#line  216 "spserver.occ"
        FP->_U0._C2.buffer_171_Dim0=512;
#line  216 "spserver.occ"
        FP->_U0._C2.buffer_171= (BYTE*)FP->buffer_164;
#line  216 "spserver.occ"
        FP->_U0._C2.length_172=&FP->length_163;
#line  216 "spserver.occ"
        FP->_U0._C2.Chain = FP;
#line  216 "spserver.occ"
        CALL(P_iserver_175,&FP->_U0._C2,5,"P_iserver_175");
#line  216 "spserver.occ"
      CASE(5):  
#line  218 "spserver.occ"
        OUTPUT2(FP->fs_158,&FP->length_163,6);
#line  218 "spserver.occ"
      CASE(6):  
#line  218 "spserver.occ"
        OUTPUT(FP->fs_158,((BYTE*)(((BYTE*)FP->buffer_164)+((RNGCHKUB(2,INTINT16(FP->length_163),512),2)*sizeof(BYTE)))),(FP->length_163)*(sizeof(BYTE)),7);
#line  218 "spserver.occ"
      CASE(7):  
#line  206 "spserver.occ"
      CASE(1):  
#line  206 "spserver.occ"
        if (FP->serving_161)
#line  206 "spserver.occ"
        {
#line  206 "spserver.occ"
          GOTO(2);
#line  206 "spserver.occ"
        }
#line  220 "spserver.occ"
        P_close_session_170();
#line  220 "spserver.occ"
        RETURN();
#line  220 "spserver.occ"
      default: SETERR(MSG_IP);
#line  220 "spserver.occ"
    }
#line  220 "spserver.occ"
  }
#line  220 "spserver.occ"
}

#line  226 "spserver.occ"
static void P_wait_socket_182 (tSF_P_wait_socket_182 *FP)
#line  226 "spserver.occ"
{
#line  226 "spserver.occ"
  while(true)
#line  226 "spserver.occ"
  {
#line  226 "spserver.occ"
    switch(FP->_Header.IP)
#line  226 "spserver.occ"
    {
#line  226 "spserver.occ"
      CASE(0):  
#line  229 "spserver.occ"
        #ifdef USE_NONBLOCKING_SOCKETS
#line  230 "spserver.occ"
        WAITSOCKET (fileno(stdin));
#line  231 "spserver.occ"
        #endif
#line  238 "spserver.occ"
        FP->true_179 = true; 
#line  240 "spserver.occ"
        if (!FP->true_179) GOTO(2);
#line  241 "spserver.occ"
        GOTO(1);
#line  241 "spserver.occ"
      CASE(2):  
#line  242 "spserver.occ"
        if (!true) GOTO(3);
#line  243 "spserver.occ"
        FP->c_181 = &FP->c_181_CHAN;
#line  243 "spserver.occ"
        INITCH(FP->c_181);
#line  244 "spserver.occ"
        FP->_Header.Temp.VINT = 0;
#line  244 "spserver.occ"
        OUTPUT4(FP->c_181,&FP->_Header.Temp.VINT,4);
#line  244 "spserver.occ"
      CASE(4):  
#line  244 "spserver.occ"
        GOTO(1);
#line  244 "spserver.occ"
      CASE(3):  
#line  244 "spserver.occ"
        SETERR(MSG_IF);
#line  244 "spserver.occ"
      CASE(1):  
#line  244 "spserver.occ"
        RETURN();
#line  244 "spserver.occ"
      default: SETERR(MSG_IP);
#line  244 "spserver.occ"
    }
#line  244 "spserver.occ"
  }
#line  244 "spserver.occ"
}

#line  248 "spserver.occ"
static void P_aserver_out_186 (INT buffer_183_Dim0,BYTE *buffer_183,INT length_184)
#line  248 "spserver.occ"
{
#line  248 "spserver.occ"
  tSF_P_aserver_out_186 SF;
#line  248 "spserver.occ"
  tSF_P_aserver_out_186 *FP = &SF;
#line  249 "spserver.occ"
  {
#line  250 "spserver.occ"
    {
#line  250 "spserver.occ"
      BYTE *lenbytes_185;
#line  250 "spserver.occ"
      lenbytes_185 = (BYTE*)&length_184;
#line  250 "spserver.occ"
      CHK((4*sizeof(BYTE)) == sizeof(INT),MSG_ARM);
#line  251 "spserver.occ"
      {
#line  252 "spserver.occ"
        { 
#line  253 "spserver.occ"
          int res, count=0;
#line  254 "spserver.occ"
          while (count<sizeof(INT)) 
#line  255 "spserver.occ"
          {
#line  256 "spserver.occ"
            res = write(fileno(stdout), &*lenbytes_185 + count, sizeof(INT)-count);
#line  257 "spserver.occ"
            if (res>0) count += res;
#line  258 "spserver.occ"
          }
#line  259 "spserver.occ"
        }
#line  259 "spserver.occ"
      }
#line  259 "spserver.occ"
    }
#line  260 "spserver.occ"
    {
#line  261 "spserver.occ"
      int res, count=0;
#line  262 "spserver.occ"
      while (count<length_184)
#line  263 "spserver.occ"
      {
#line  264 "spserver.occ"
        res = write(fileno(stdout),buffer_183 + count, length_184-count);
#line  265 "spserver.occ"
        if (res>0) count += res;
#line  266 "spserver.occ"
      }
#line  267 "spserver.occ"
    }
#line  268 "spserver.occ"
    #ifdef COMMENT_OUT      
#line  269 "spserver.occ"
    fprintf(stderr,"SEND: %d packet ",length_184);
#line  270 "spserver.occ"
    { int i; for (i=0;i<length_184;i++) fprintf(stderr,"%02x",buffer_183[i]); }
#line  271 "spserver.occ"
    fprintf(stderr,"\n");
#line  272 "spserver.occ"
    #endif
#line  272 "spserver.occ"
  }
#line  272 "spserver.occ"
}

#line  276 "spserver.occ"
static void P_aserver_in_190 (tSF_P_aserver_in_190 *FP)
#line  276 "spserver.occ"
{
#line  276 "spserver.occ"
  while(true)
#line  276 "spserver.occ"
  {
#line  276 "spserver.occ"
    switch(FP->_Header.IP)
#line  276 "spserver.occ"
    {
#line  276 "spserver.occ"
      CASE(0):  
#line  278 "spserver.occ"
        #ifdef USE_NONBLOCKING_SOCKETS
#line  279 "spserver.occ"
        FP->_C0.Chain = FP->Chain;
#line  279 "spserver.occ"
        CALL(P_wait_socket_182,&FP->_C0,1,"P_wait_socket_182");
#line  279 "spserver.occ"
      CASE(1):  
#line  280 "spserver.occ"
        #endif
#line  281 "spserver.occ"
        {
#line  281 "spserver.occ"
          BYTE *lenbytes_189;
#line  281 "spserver.occ"
          lenbytes_189 = (BYTE*)FP->length_188;
#line  281 "spserver.occ"
          CHK((4*sizeof(BYTE)) == sizeof(INT),MSG_ARM);
#line  282 "spserver.occ"
          {
#line  283 "spserver.occ"
            { 
#line  284 "spserver.occ"
              int res, count=0;
#line  285 "spserver.occ"
              while (count<sizeof(INT)) 
#line  286 "spserver.occ"
              {
#line  287 "spserver.occ"
                res = read(fileno(stdin), &*lenbytes_189 + count, sizeof(INT)-count);
#line  288 "spserver.occ"
                if (res>0) count += res;
#line  289 "spserver.occ"
              }
#line  290 "spserver.occ"
            }
#line  290 "spserver.occ"
          }
#line  290 "spserver.occ"
        }
#line  291 "spserver.occ"
        {
#line  292 "spserver.occ"
          int res, count=0;
#line  293 "spserver.occ"
          while (count<*FP->length_188)
#line  294 "spserver.occ"
          {
#line  295 "spserver.occ"
            res = read(fileno(stdin), (& *FP->buffer_187) + count, *FP->length_188-count);
#line  296 "spserver.occ"
            if (res>0) count += res;
#line  297 "spserver.occ"
          }
#line  298 "spserver.occ"
        }
#line  299 "spserver.occ"
        #ifdef COMMENT_OUT
#line  300 "spserver.occ"
        fprintf(stderr,"RECV: %d packet\n",*FP->length_188);      
#line  301 "spserver.occ"
        #endif
#line  301 "spserver.occ"
        RETURN();
#line  301 "spserver.occ"
      default: SETERR(MSG_IP);
#line  301 "spserver.occ"
    }
#line  301 "spserver.occ"
  }
#line  301 "spserver.occ"
}

#line  305 "spserver.occ"
static void P_open_session_191 (void)
#line  305 "spserver.occ"
{
#line  305 "spserver.occ"
  tSF_P_open_session_191 SF;
#line  305 "spserver.occ"
  tSF_P_open_session_191 *FP = &SF;
#line  306 "spserver.occ"
  {
#line  307 "spserver.occ"
    #ifdef USE_NONBLOCKING_SOCKETS
#line  308 "spserver.occ"
      EnableSocket (fileno(stdin));
#line  309 "spserver.occ"
    #endif
#line  309 "spserver.occ"
  }
#line  309 "spserver.occ"
}

#line  313 "spserver.occ"
static void P_close_session_192 (void)
#line  313 "spserver.occ"
{
#line  313 "spserver.occ"
  tSF_P_close_session_192 SF;
#line  313 "spserver.occ"
  tSF_P_close_session_192 *FP = &SF;
#line  314 "spserver.occ"
  {
#line  315 "spserver.occ"
    #ifdef USE_NONBLOCKING_SOCKETS
#line  316 "spserver.occ"
      DisableSocket(fileno(stdin));
#line  317 "spserver.occ"
    #endif
#line  317 "spserver.occ"
  }
#line  317 "spserver.occ"
}

#line  327 "spserver.occ"
static void P_199 (tSF_P_199 *FP)
#line  327 "spserver.occ"
{
#line  327 "spserver.occ"
  while(true)
#line  327 "spserver.occ"
  {
#line  327 "spserver.occ"
    switch(FP->_Header.IP)
#line  327 "spserver.occ"
    {
#line  327 "spserver.occ"
      CASE(0):  
#line  331 "spserver.occ"
        FP->length_197 = 1; 
#line  332 "spserver.occ"
        GOTO(1);
#line  332 "spserver.occ"
      CASE(2):  
#line  334 "spserver.occ"
        INPUT4(FP->Chain->ts_196,&FP->length_197,3);
#line  334 "spserver.occ"
      CASE(3):  
#line  334 "spserver.occ"
        CHK(FP->length_197<=1024,MSG_CITL);
#line  334 "spserver.occ"
        INPUT(FP->Chain->ts_196,FP->buffer_198,(FP->length_197)*(sizeof(BYTE)),4);
#line  334 "spserver.occ"
      CASE(4):  
#line  335 "spserver.occ"
        P_aserver_out_186(1024,(BYTE*)FP->buffer_198,FP->length_197);
#line  332 "spserver.occ"
      CASE(1):  
#line  332 "spserver.occ"
        if ((FP->length_197 != 0))
#line  332 "spserver.occ"
        {
#line  332 "spserver.occ"
          GOTO(2);
#line  332 "spserver.occ"
        }
#line  332 "spserver.occ"
        ENDP();
#line  332 "spserver.occ"
      default: SETERR(MSG_IP);
#line  332 "spserver.occ"
    }
#line  332 "spserver.occ"
  }
#line  332 "spserver.occ"
}

#line  327 "spserver.occ"
static void P_202 (tSF_P_202 *FP)
#line  327 "spserver.occ"
{
#line  327 "spserver.occ"
  while(true)
#line  327 "spserver.occ"
  {
#line  327 "spserver.occ"
    switch(FP->_Header.IP)
#line  327 "spserver.occ"
    {
#line  327 "spserver.occ"
      CASE(0):  
#line  339 "spserver.occ"
        FP->length_200 = 1; 
#line  340 "spserver.occ"
        GOTO(1);
#line  340 "spserver.occ"
      CASE(2):  
#line  342 "spserver.occ"
        FP->_C4.buffer_187_Dim0=1024;
#line  342 "spserver.occ"
        FP->_C4.buffer_187= (BYTE*)FP->buffer_201;
#line  342 "spserver.occ"
        FP->_C4.length_188=&FP->length_200;
#line  342 "spserver.occ"
        FP->_C4.Chain = FP->Chain;
#line  342 "spserver.occ"
        CALL(P_aserver_in_190,&FP->_C4,3,"P_aserver_in_190");
#line  342 "spserver.occ"
      CASE(3):  
#line  343 "spserver.occ"
        OUTPUT4(FP->Chain->fs_195,&FP->length_200,4);
#line  343 "spserver.occ"
      CASE(4):  
#line  343 "spserver.occ"
        OUTPUT(FP->Chain->fs_195,FP->buffer_201,(FP->length_200)*(sizeof(BYTE)),5);
#line  343 "spserver.occ"
      CASE(5):  
#line  340 "spserver.occ"
      CASE(1):  
#line  340 "spserver.occ"
        if ((FP->length_200 != 0))
#line  340 "spserver.occ"
        {
#line  340 "spserver.occ"
          GOTO(2);
#line  340 "spserver.occ"
        }
#line  340 "spserver.occ"
        ENDP();
#line  340 "spserver.occ"
      default: SETERR(MSG_IP);
#line  340 "spserver.occ"
    }
#line  340 "spserver.occ"
  }
#line  340 "spserver.occ"
}

#line  224 "spserver.occ"
void P_spoc_aserver_spserver (tSF_P_spoc_aserver_spserver *FP)
#line  224 "spserver.occ"
{
#line  224 "spserver.occ"
  while(true)
#line  224 "spserver.occ"
  {
#line  224 "spserver.occ"
    switch(FP->_Header.IP)
#line  224 "spserver.occ"
    {
#line  224 "spserver.occ"
      CASE(0):  
#line  323 "spserver.occ"
        FP->fs_195 = (CHAN*)FP->fs_177;
#line  324 "spserver.occ"
        FP->ts_196 = (CHAN*)FP->ts_178;
#line  326 "spserver.occ"
        P_open_session_191();
#line  327 "spserver.occ"
        STARTP(&FP->_U1._S0._T0,P_199,&FP->_U1._S0._C3,"P_199",1);
#line  327 "spserver.occ"
        STARTP(&FP->_U1._S0._T1,P_202,&FP->_U1._S0._C5,"P_202",1);
#line  327 "spserver.occ"
        WAITP(1);
#line  327 "spserver.occ"
      CASE(1):  
#line  344 "spserver.occ"
        P_close_session_192();
#line  344 "spserver.occ"
        RETURN();
#line  344 "spserver.occ"
      default: SETERR(MSG_IP);
#line  344 "spserver.occ"
    }
#line  344 "spserver.occ"
  }
#line  344 "spserver.occ"
}


/*}}}*/
#endif
