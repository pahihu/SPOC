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
#include "hostio.hdr"
struct SF_P_so_read_echo_int_reint
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  INT *Param2;
  BOOL *Param3;
  char WS[WS_P_so_read_echo_int_reint];
};
struct SF_P_so_write_string_wstring
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  BYTE *Param2;
  INT Param2_Dim0;
  char WS[WS_P_so_write_string_wstring];
};
struct SF_P_so_write_string_nl_wstring
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  BYTE *Param2;
  INT Param2_Dim0;
  char WS[WS_P_so_write_string_nl_wstring];
};
struct SF_P_so_write_nl_wstring
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  char WS[WS_P_so_write_nl_wstring];
};
struct SF_P_so_write_int_wint
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  INT Param2;
  INT Param3;
  char WS[WS_P_so_write_int_wint];
};
struct SF_P_so_write_real64_wreal64
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  REAL64 Param2;
  INT Param3;
  INT Param4;
  char WS[WS_P_so_write_real64_wreal64];
};
struct SF_P_so_exit_exit
{
  tHeader _Header;
  void *Chain;
  CHAN *Param0;
  CHAN *Param1;
  INT32 Param2;
  char WS[WS_P_so_exit_exit];
};
typedef struct SF_P_611 tSF_P_611;
typedef struct SF_P_613 tSF_P_613;
typedef struct SF_P_617 tSF_P_617;
typedef struct SF_P_618 tSF_P_618;
typedef struct SF_P_619 tSF_P_619;
typedef struct SF_P_control_controller tSF_P_control_controller;
void P_control_controller (tSF_P_control_controller *FP);
struct SF_P_611
{
  tHeader _Header;
  tSF_P_618 *Chain;
  INT i_610;
  INT i_610_End;
};
struct SF_P_613
{
  tHeader _Header;
  tSF_P_618 *Chain;
  INT i_612;
  INT i_612_End;
};
struct SF_P_617
{
  tHeader _Header;
  tSF_P_618 *Chain;
  INT PacketsOutstanding_614;
  REAL64 x_615;
  BOOL More_616;
};
struct SF_P_618
{
  tHeader _Header;
  tSF_P_control_controller *Chain;
  tTask _T0;
  tSF_P_611 _C30;
  tTask _T1;
  tSF_P_613 _C31;
  tTask _T2;
  tSF_P_617 _C32;
};
struct SF_P_619
{
  tHeader _Header;
  tSF_P_control_controller *Chain;
};
struct SF_P_control_controller
{
  tHeader _Header;
  void *Chain;
  CHAN *fs_566;
  CHAN *ts_567;
  CHAN *toFarm_568;
  CHAN *fromFarm_569;
  REAL64 Lower_570;
  REAL64 Upper_571;
  REAL64 PacketDeltaX_572;
  REAL64 SampleDeltaX_573;
  REAL64 Index_574;
  REAL64 Power_575;
  REAL64 Sum_576;
  REAL64 Value_577;
  INT NoOfSamples_578;
  INT NoOfPackets_579;
  INT TotalSamples_580;
  INT EqnType_581;
  BOOL Error_582;
  BOOL More_583;
  BOOL AskFunc_584;
  REAL64 Elapsed_585;
  INT Time1_586;
  INT Time2_587;
  INT PktCount_589[16];
  INT NoOfWorkers_590;
  INT Node_591;
  CHAN FlowCtrl_593_CHAN;
  CHAN *FlowCtrl_593;
  CHAN toValve_595_CHAN;
  CHAN *toValve_595;
  union
  {
    tSF_P_so_write_nl_wstring _C0;
    tSF_P_so_write_nl_wstring _C1;
    struct
    {
      BYTE *_V0_596;
      tSF_P_so_write_string_nl_wstring _C2;
    } _S0;
    struct
    {
      BYTE *_V1_597;
      tSF_P_so_write_string_nl_wstring _C3;
    } _S1;
    struct
    {
      BYTE *_V2_598;
      tSF_P_so_write_string_nl_wstring _C4;
    } _S2;
    struct
    {
      BYTE *_V3_599;
      tSF_P_so_write_string_wstring _C5;
    } _S3;
    tSF_P_so_read_echo_int_reint _C6;
    tSF_P_so_write_nl_wstring _C7;
    struct
    {
      BYTE *_V4_600;
      tSF_P_so_write_string_wstring _C8;
    } _S4;
    tSF_P_so_read_echo_int_reint _C9;
    tSF_P_so_write_nl_wstring _C10;
    struct
    {
      BYTE *_V5_601;
      tSF_P_so_write_string_wstring _C11;
    } _S5;
    tSF_P_so_read_echo_int_reint _C12;
    tSF_P_so_write_nl_wstring _C13;
    tSF_P_so_write_nl_wstring _C14;
    struct
    {
      BYTE *_V8_603;
      tSF_P_so_write_string_wstring _C18;
    } _S6;
    tSF_P_so_write_real64_wreal64 _C19;
    struct
    {
      BYTE *_V6_604;
      tSF_P_so_write_string_wstring _C15;
    } _S7;
    tSF_P_so_write_real64_wreal64 _C16;
    struct
    {
      BYTE *_V7_605;
      tSF_P_so_write_string_wstring _C17;
    } _S8;
    tSF_P_so_write_nl_wstring _C20;
    struct
    {
      BYTE *_V9_606;
      tSF_P_so_write_string_wstring _C21;
    } _S9;
    tSF_P_so_write_real64_wreal64 _C22;
    struct
    {
      BYTE *_V10_607;
      tSF_P_so_write_string_wstring _C23;
    } _S10;
    tSF_P_so_write_real64_wreal64 _C24;
    struct
    {
      BYTE *_V11_608;
      tSF_P_so_write_string_wstring _C25;
    } _S11;
    tSF_P_so_write_real64_wreal64 _C26;
    struct
    {
      BYTE *_V12_609;
      tSF_P_so_write_string_wstring _C27;
    } _S12;
    tSF_P_so_write_real64_wreal64 _C28;
    tSF_P_so_write_nl_wstring _C29;
    struct
    {
      tTask _T3;
      tSF_P_618 _C33;
      tTask _T4;
      tSF_P_619 _C34;
    } _S13;
    struct
    {
      BYTE *_V13_620;
      tSF_P_so_write_string_wstring _C35;
    } _S14;
    tSF_P_so_write_real64_wreal64 _C36;
    tSF_P_so_write_nl_wstring _C37;
    struct
    {
      BYTE *_V14_621;
      tSF_P_so_write_string_wstring _C38;
    } _S15;
    tSF_P_so_write_real64_wreal64 _C39;
    struct
    {
      BYTE *_V15_622;
      tSF_P_so_write_string_wstring _C40;
    } _S16;
    tSF_P_so_write_nl_wstring _C41;
    struct
    {
      BYTE *_V16_623;
      tSF_P_so_write_string_wstring _C42;
    } _S17;
    tSF_P_so_write_nl_wstring _C43;
    struct
    {
      INT i_624;
      INT i_624_End;
      union
      {
        tSF_P_so_write_int_wint _C44;
        struct
        {
          BYTE *_V17_625;
          tSF_P_so_write_string_wstring _C45;
        } _S19;
        tSF_P_so_write_int_wint _C46;
        struct
        {
          BYTE *_V18_626;
          tSF_P_so_write_string_wstring _C47;
        } _S20;
        tSF_P_so_write_real64_wreal64 _C48;
        tSF_P_so_write_nl_wstring _C49;
      } _U1;
    } _S18;
    tSF_P_so_exit_exit _C50;
  } _U0;
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
  {"void P_control_controller (tSF_P_control_controller *FP);","P_control_controller",sizeof(tSF_P_control_controller)},

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
    name="controller.hdr";
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
    fprintf(f,"#ifndef __controller_INCLUDES__\n");
    fprintf(f,"#define __controller_INCLUDES__\n");
    
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
static void P_611 (tSF_P_611 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->Chain->Chain->Index_574 = FP->Chain->Chain->Lower_570; 
        FP->i_610 = 0;
        FP->i_610_End = FP->i_610 + FP->Chain->Chain->NoOfPackets_579;
        GOTO(1);
      CASE(2):  
        FP->_Header.Temp.VBYTE = 1;
        OUTPUT1(FP->Chain->Chain->toValve_595,&FP->_Header.Temp.VBYTE,3);
      CASE(3):  
        OUTPUT8(FP->Chain->Chain->toValve_595,&FP->Chain->Chain->Index_574,4);
      CASE(4):  
        FP->Chain->Chain->Index_574 = (FP->Chain->Chain->Index_574 + FP->Chain->Chain->PacketDeltaX_572); 
        FP->i_610++;
      CASE(1):  
        if (FP->i_610 != FP->i_610_End) GOTO(2);
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_613 (tSF_P_613 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->Chain->Chain->Sum_576 = 0.0e+00; 
        FP->i_612 = 0;
        FP->i_612_End = FP->i_612 + FP->Chain->Chain->NoOfPackets_579;
        GOTO(1);
      CASE(2):  
        INPUT1(FP->Chain->Chain->fromFarm_569,&FP->_Header.Temp.VBYTE,3);
      CASE(3):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 2: GOTO (5);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(5):  
        INPUT8(FP->Chain->Chain->fromFarm_569,&FP->Chain->Chain->Value_577,6);
      CASE(6):  
        INPUT4(FP->Chain->Chain->fromFarm_569,&FP->Chain->Chain->Node_591,7);
      CASE(7):  
        FP->Chain->Chain->Sum_576 = (FP->Chain->Chain->Sum_576 + FP->Chain->Chain->Value_577); 
        *((INT*)(((BYTE*)FP->Chain->Chain->PktCount_589)+(RNGCHK(FP->Chain->Chain->Node_591,16)*sizeof(INT)))) = (*((INT*)(((BYTE*)FP->Chain->Chain->PktCount_589)+(RNGCHK(FP->Chain->Chain->Node_591,16)*sizeof(INT)))) + 1); 
        FP->_Header.Temp.VBYTE = 3;
        OUTPUT1(FP->Chain->Chain->FlowCtrl_593,&FP->_Header.Temp.VBYTE,8);
      CASE(8):  
        GOTO(4);
      CASE(4):  
        FP->i_612++;
      CASE(1):  
        if (FP->i_612 != FP->i_612_End) GOTO(2);
        FP->Chain->Chain->Sum_576 = (FP->Chain->Chain->Sum_576 * FP->Chain->Chain->SampleDeltaX_573); 
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->Chain->Chain->FlowCtrl_593,&FP->_Header.Temp.VBYTE,9);
      CASE(9):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_617 (tSF_P_617 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->PacketsOutstanding_614 = 0; 
        FP->More_616 = true; 
        GOTO(1);
      CASE(2):  
        ALT();
        ENBC(true,FP->Chain->Chain->FlowCtrl_593);
        ENBC((FP->PacketsOutstanding_614 < (2 * FP->Chain->Chain->NoOfWorkers_590)),FP->Chain->Chain->toValve_595);
        ALTWT(3);
      CASE(3):  
        {
          BOOL TMP = false;
          TMP |= DISC(4,true,FP->Chain->Chain->FlowCtrl_593);
          TMP |= DISC(5,(FP->PacketsOutstanding_614 < (2 * FP->Chain->Chain->NoOfWorkers_590)),FP->Chain->Chain->toValve_595);
        }
        ALTEND();
      CASE(4):  
        INPUT1(FP->Chain->Chain->FlowCtrl_593,&FP->_Header.Temp.VBYTE,7);
      CASE(7):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 3: GOTO (9);
          case 4: GOTO (10);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(9):  
        FP->PacketsOutstanding_614 = (FP->PacketsOutstanding_614 - 1); 
        GOTO(8);
      CASE(10):  
        FP->More_616 = false; 
        GOTO(8);
      CASE(8):  
        GOTO(6);
      CASE(5):  
        INPUT1(FP->Chain->Chain->toValve_595,&FP->_Header.Temp.VBYTE,11);
      CASE(11):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 1: GOTO (13);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(13):  
        INPUT8(FP->Chain->Chain->toValve_595,&FP->x_615,14);
      CASE(14):  
        FP->_Header.Temp.VBYTE = 1;
        OUTPUT1(FP->Chain->Chain->toFarm_568,&FP->_Header.Temp.VBYTE,15);
      CASE(15):  
        OUTPUT8(FP->Chain->Chain->toFarm_568,&FP->x_615,16);
      CASE(16):  
        FP->PacketsOutstanding_614 = (FP->PacketsOutstanding_614 + 1); 
        GOTO(12);
      CASE(12):  
        GOTO(6);
      CASE(6):  
      CASE(1):  
        if (FP->More_616)
        {
          GOTO(2);
        }
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_618 (tSF_P_618 *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        STARTP(&FP->_T0,P_611,&FP->_C30,"P_611",1);
        STARTP(&FP->_T1,P_613,&FP->_C31,"P_613",1);
        STARTP(&FP->_T2,P_617,&FP->_C32,"P_617",1);
        WAITP(1);
      CASE(1):  
        ENDP();
      default: SETERR(MSG_IP);
    }
  }
}

static void P_619 (tSF_P_619 *FP)
{
  ENDP();
}

void P_control_controller (tSF_P_control_controller *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        FP->FlowCtrl_593 = &FP->FlowCtrl_593_CHAN;
        INITCH(FP->FlowCtrl_593);
        FP->toValve_595 = &FP->toValve_595_CHAN;
        INITCH(FP->toValve_595);
        FP->More_583 = true; 
        FP->AskFunc_584 = true; 
        GOTO(1);
      CASE(2):  
        FP->_U0._C0.Param0=FP->fs_566;
        FP->_U0._C0.Param1=FP->ts_567;
        FP->_U0._C0.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C0,3,"P_so_write_nl_wstring");
      CASE(3):  
        FP->_U0._C1.Param0=FP->fs_566;
        FP->_U0._C1.Param1=FP->ts_567;
        FP->_U0._C1.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C1,4,"P_so_write_nl_wstring");
      CASE(4):  
        {
          static BYTE TMP[21] = {78,117,109,101,114,105,99,97,108,32,73,110,116,101,103,114,97,116,105,111,
             110};
          FP->_U0._S0._V0_596 = TMP;
        }
        FP->_U0._S0._C2.Param0=FP->fs_566;
        FP->_U0._S0._C2.Param1=FP->ts_567;
        FP->_U0._S0._C2.Param2_Dim0=21;
        FP->_U0._S0._C2.Param2= (BYTE*)FP->_U0._S0._V0_596;
        FP->_U0._S0._C2.Chain = NULL;
        CALL(P_so_write_string_nl_wstring,&FP->_U0._S0._C2,5,"P_so_write_string_nl_wstring");
      CASE(5):  
        {
          static BYTE TMP[66] = {84,104,105,115,32,112,114,111,103,114,97,109,32,117,115,101,115,32,97,32,
             112,114,111,99,101,115,115,32,102,97,114,109,32,111,102,32,117,112,32,116,
             111,32,49,54,32,99,97,108,99,117,108,97,116,105,111,110,32,112,114,111,
             99,101,115,115,101,115};
          FP->_U0._S1._V1_597 = TMP;
        }
        FP->_U0._S1._C3.Param0=FP->fs_566;
        FP->_U0._S1._C3.Param1=FP->ts_567;
        FP->_U0._S1._C3.Param2_Dim0=66;
        FP->_U0._S1._C3.Param2= (BYTE*)FP->_U0._S1._V1_597;
        FP->_U0._S1._C3.Chain = NULL;
        CALL(P_so_write_string_nl_wstring,&FP->_U0._S1._C3,6,"P_so_write_string_nl_wstring");
      CASE(6):  
        {
          static BYTE TMP[59] = {116,111,32,112,101,114,102,111,114,109,32,110,117,109,101,114,105,99,97,108,
             32,105,110,116,101,103,114,97,116,105,111,110,32,111,102,32,97,32,115,105,
             109,112,108,101,32,101,113,117,97,116,105,111,110,44,121,61,120,94,50};
          FP->_U0._S2._V2_598 = TMP;
        }
        FP->_U0._S2._C4.Param0=FP->fs_566;
        FP->_U0._S2._C4.Param1=FP->ts_567;
        FP->_U0._S2._C4.Param2_Dim0=59;
        FP->_U0._S2._C4.Param2= (BYTE*)FP->_U0._S2._V2_598;
        FP->_U0._S2._C4.Chain = NULL;
        CALL(P_so_write_string_nl_wstring,&FP->_U0._S2._C4,7,"P_so_write_string_nl_wstring");
      CASE(7):  
        FP->EqnType_581 = 1; 
        FP->Power_575 = 2.0e+00; 
        FP->Lower_570 = 0.0e+00; 
        FP->Upper_571 = 1.000e+02; 
        FP->Error_582 = true; 
        GOTO(8);
      CASE(9):  
        {
          static BYTE TMP[33] = {69,110,116,101,114,32,78,117,109,98,101,114,32,111,102,32,87,111,114,107,
             101,114,32,80,114,111,99,101,115,115,101,115,58};
          FP->_U0._S3._V3_599 = TMP;
        }
        FP->_U0._S3._C5.Param0=FP->fs_566;
        FP->_U0._S3._C5.Param1=FP->ts_567;
        FP->_U0._S3._C5.Param2_Dim0=33;
        FP->_U0._S3._C5.Param2= (BYTE*)FP->_U0._S3._V3_599;
        FP->_U0._S3._C5.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S3._C5,10,"P_so_write_string_wstring");
      CASE(10):  
        FP->_U0._C6.Param0=FP->fs_566;
        FP->_U0._C6.Param1=FP->ts_567;
        FP->_U0._C6.Param2=&FP->NoOfWorkers_590;
        FP->_U0._C6.Param3=&FP->Error_582;
        FP->_U0._C6.Chain = NULL;
        CALL(P_so_read_echo_int_reint,&FP->_U0._C6,11,"P_so_read_echo_int_reint");
      CASE(11):  
        FP->_U0._C7.Param0=FP->fs_566;
        FP->_U0._C7.Param1=FP->ts_567;
        FP->_U0._C7.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C7,12,"P_so_write_nl_wstring");
      CASE(12):  
        if ((FP->Error_582 == true))
        {
        }
        else
        if (((FP->NoOfWorkers_590 < 1) || (FP->NoOfWorkers_590 > 16)))
        {
          FP->Error_582 = true; 
        }
        else
        if (true)
        {
        }
        else
        {
          SETERR(MSG_IF);
        }
      CASE(8):  
        if (FP->Error_582)
        {
          GOTO(9);
        }
        FP->Error_582 = true; 
        GOTO(13);
      CASE(14):  
        {
          static BYTE TMP[25] = {69,110,116,101,114,32,78,117,109,98,101,114,32,111,102,32,80,97,99,107,
             101,116,115,58,32};
          FP->_U0._S4._V4_600 = TMP;
        }
        FP->_U0._S4._C8.Param0=FP->fs_566;
        FP->_U0._S4._C8.Param1=FP->ts_567;
        FP->_U0._S4._C8.Param2_Dim0=25;
        FP->_U0._S4._C8.Param2= (BYTE*)FP->_U0._S4._V4_600;
        FP->_U0._S4._C8.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S4._C8,15,"P_so_write_string_wstring");
      CASE(15):  
        FP->_U0._C9.Param0=FP->fs_566;
        FP->_U0._C9.Param1=FP->ts_567;
        FP->_U0._C9.Param2=&FP->NoOfPackets_579;
        FP->_U0._C9.Param3=&FP->Error_582;
        FP->_U0._C9.Chain = NULL;
        CALL(P_so_read_echo_int_reint,&FP->_U0._C9,16,"P_so_read_echo_int_reint");
      CASE(16):  
        FP->_U0._C10.Param0=FP->fs_566;
        FP->_U0._C10.Param1=FP->ts_567;
        FP->_U0._C10.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C10,17,"P_so_write_nl_wstring");
      CASE(17):  
        if ((FP->Error_582 == true))
        {
        }
        else
        if ((FP->NoOfPackets_579 < 1))
        {
          FP->Error_582 = true; 
        }
        else
        if (true)
        {
        }
        else
        {
          SETERR(MSG_IF);
        }
      CASE(13):  
        if (FP->Error_582)
        {
          GOTO(14);
        }
        FP->Error_582 = true; 
        GOTO(18);
      CASE(19):  
        {
          static BYTE TMP[32] = {69,110,116,101,114,32,78,117,109,98,101,114,32,111,102,32,83,97,109,112,
             108,101,115,47,80,97,99,107,101,116,58,32};
          FP->_U0._S5._V5_601 = TMP;
        }
        FP->_U0._S5._C11.Param0=FP->fs_566;
        FP->_U0._S5._C11.Param1=FP->ts_567;
        FP->_U0._S5._C11.Param2_Dim0=32;
        FP->_U0._S5._C11.Param2= (BYTE*)FP->_U0._S5._V5_601;
        FP->_U0._S5._C11.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S5._C11,20,"P_so_write_string_wstring");
      CASE(20):  
        FP->_U0._C12.Param0=FP->fs_566;
        FP->_U0._C12.Param1=FP->ts_567;
        FP->_U0._C12.Param2=&FP->NoOfSamples_578;
        FP->_U0._C12.Param3=&FP->Error_582;
        FP->_U0._C12.Chain = NULL;
        CALL(P_so_read_echo_int_reint,&FP->_U0._C12,21,"P_so_read_echo_int_reint");
      CASE(21):  
        FP->_U0._C13.Param0=FP->fs_566;
        FP->_U0._C13.Param1=FP->ts_567;
        FP->_U0._C13.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C13,22,"P_so_write_nl_wstring");
      CASE(22):  
        if ((FP->Error_582 == true))
        {
        }
        else
        if ((FP->NoOfSamples_578 < 1))
        {
          FP->Error_582 = true; 
        }
        else
        if (true)
        {
        }
        else
        {
          SETERR(MSG_IF);
        }
      CASE(18):  
        if (FP->Error_582)
        {
          GOTO(19);
        }
        FP->TotalSamples_580 = (FP->NoOfPackets_579 * FP->NoOfSamples_578); 
        FP->PacketDeltaX_572 = ((FP->Upper_571 - FP->Lower_570) / REAL64INTROUND(FP->NoOfPackets_579)); 
        FP->SampleDeltaX_573 = (FP->PacketDeltaX_572 / REAL64INTROUND(FP->NoOfSamples_578)); 
        {
          INT i_602;
          INT i_602_End;
          i_602 = 0;i_602_End = i_602 + FP->NoOfWorkers_590;
          for (; i_602 != i_602_End; i_602++)
          {
            *((INT*)(((BYTE*)FP->PktCount_589)+(RNGCHK(i_602,16)*sizeof(INT)))) = 0; 
          }
        }
        FP->_U0._C14.Param0=FP->fs_566;
        FP->_U0._C14.Param1=FP->ts_567;
        FP->_U0._C14.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C14,23,"P_so_write_nl_wstring");
      CASE(23):  
        if (!(FP->EqnType_581 == 1)) GOTO(25);
        {
          static BYTE TMP[26] = {67,97,108,99,117,108,97,116,105,110,103,32,73,110,116,101,103,114,97,108,
             32,111,102,32,120,94};
          FP->_U0._S6._V8_603 = TMP;
        }
        FP->_U0._S6._C18.Param0=FP->fs_566;
        FP->_U0._S6._C18.Param1=FP->ts_567;
        FP->_U0._S6._C18.Param2_Dim0=26;
        FP->_U0._S6._C18.Param2= (BYTE*)FP->_U0._S6._V8_603;
        FP->_U0._S6._C18.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S6._C18,26,"P_so_write_string_wstring");
      CASE(26):  
        FP->_U0._C19.Param0=FP->fs_566;
        FP->_U0._C19.Param1=FP->ts_567;
        FP->_U0._C19.Param2=FP->Power_575;
        FP->_U0._C19.Param3=0;
        FP->_U0._C19.Param4=0;
        FP->_U0._C19.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C19,27,"P_so_write_real64_wreal64");
      CASE(27):  
        GOTO(24);
      CASE(25):  
        if (!(FP->EqnType_581 == 2)) GOTO(28);
        {
          static BYTE TMP[31] = {67,97,108,99,117,108,97,116,105,110,103,32,73,110,116,101,103,114,97,108,
             32,111,102,32,101,120,112,40,45,120,94};
          FP->_U0._S7._V6_604 = TMP;
        }
        FP->_U0._S7._C15.Param0=FP->fs_566;
        FP->_U0._S7._C15.Param1=FP->ts_567;
        FP->_U0._S7._C15.Param2_Dim0=31;
        FP->_U0._S7._C15.Param2= (BYTE*)FP->_U0._S7._V6_604;
        FP->_U0._S7._C15.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S7._C15,29,"P_so_write_string_wstring");
      CASE(29):  
        FP->_U0._C16.Param0=FP->fs_566;
        FP->_U0._C16.Param1=FP->ts_567;
        FP->_U0._C16.Param2=FP->Power_575;
        FP->_U0._C16.Param3=0;
        FP->_U0._C16.Param4=0;
        FP->_U0._C16.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C16,30,"P_so_write_real64_wreal64");
      CASE(30):  
        {
          static BYTE TMP[1] = {41};
          FP->_U0._S8._V7_605 = TMP;
        }
        FP->_U0._S8._C17.Param0=FP->fs_566;
        FP->_U0._S8._C17.Param1=FP->ts_567;
        FP->_U0._S8._C17.Param2_Dim0=1;
        FP->_U0._S8._C17.Param2= (BYTE*)FP->_U0._S8._V7_605;
        FP->_U0._S8._C17.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S8._C17,31,"P_so_write_string_wstring");
      CASE(31):  
        GOTO(24);
      CASE(28):  
        SETERR(MSG_IF);
      CASE(24):  
        FP->_U0._C20.Param0=FP->fs_566;
        FP->_U0._C20.Param1=FP->ts_567;
        FP->_U0._C20.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C20,32,"P_so_write_nl_wstring");
      CASE(32):  
        {
          static BYTE TMP[6] = {82,97,110,103,101,32};
          FP->_U0._S9._V9_606 = TMP;
        }
        FP->_U0._S9._C21.Param0=FP->fs_566;
        FP->_U0._S9._C21.Param1=FP->ts_567;
        FP->_U0._S9._C21.Param2_Dim0=6;
        FP->_U0._S9._C21.Param2= (BYTE*)FP->_U0._S9._V9_606;
        FP->_U0._S9._C21.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S9._C21,33,"P_so_write_string_wstring");
      CASE(33):  
        FP->_U0._C22.Param0=FP->fs_566;
        FP->_U0._C22.Param1=FP->ts_567;
        FP->_U0._C22.Param2=FP->Lower_570;
        FP->_U0._C22.Param3=0;
        FP->_U0._C22.Param4=0;
        FP->_U0._C22.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C22,34,"P_so_write_real64_wreal64");
      CASE(34):  
        {
          static BYTE TMP[4] = {32,116,111,32};
          FP->_U0._S10._V10_607 = TMP;
        }
        FP->_U0._S10._C23.Param0=FP->fs_566;
        FP->_U0._S10._C23.Param1=FP->ts_567;
        FP->_U0._S10._C23.Param2_Dim0=4;
        FP->_U0._S10._C23.Param2= (BYTE*)FP->_U0._S10._V10_607;
        FP->_U0._S10._C23.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S10._C23,35,"P_so_write_string_wstring");
      CASE(35):  
        FP->_U0._C24.Param0=FP->fs_566;
        FP->_U0._C24.Param1=FP->ts_567;
        FP->_U0._C24.Param2=FP->Upper_571;
        FP->_U0._C24.Param3=0;
        FP->_U0._C24.Param4=0;
        FP->_U0._C24.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C24,36,"P_so_write_real64_wreal64");
      CASE(36):  
        {
          static BYTE TMP[15] = {32,32,80,97,99,107,101,116,32,68,101,108,116,97,32};
          FP->_U0._S11._V11_608 = TMP;
        }
        FP->_U0._S11._C25.Param0=FP->fs_566;
        FP->_U0._S11._C25.Param1=FP->ts_567;
        FP->_U0._S11._C25.Param2_Dim0=15;
        FP->_U0._S11._C25.Param2= (BYTE*)FP->_U0._S11._V11_608;
        FP->_U0._S11._C25.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S11._C25,37,"P_so_write_string_wstring");
      CASE(37):  
        FP->_U0._C26.Param0=FP->fs_566;
        FP->_U0._C26.Param1=FP->ts_567;
        FP->_U0._C26.Param2=FP->PacketDeltaX_572;
        FP->_U0._C26.Param3=0;
        FP->_U0._C26.Param4=0;
        FP->_U0._C26.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C26,38,"P_so_write_real64_wreal64");
      CASE(38):  
        {
          static BYTE TMP[15] = {32,32,83,97,109,112,108,101,32,68,101,108,116,97,32};
          FP->_U0._S12._V12_609 = TMP;
        }
        FP->_U0._S12._C27.Param0=FP->fs_566;
        FP->_U0._S12._C27.Param1=FP->ts_567;
        FP->_U0._S12._C27.Param2_Dim0=15;
        FP->_U0._S12._C27.Param2= (BYTE*)FP->_U0._S12._V12_609;
        FP->_U0._S12._C27.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S12._C27,39,"P_so_write_string_wstring");
      CASE(39):  
        FP->_U0._C28.Param0=FP->fs_566;
        FP->_U0._C28.Param1=FP->ts_567;
        FP->_U0._C28.Param2=FP->SampleDeltaX_573;
        FP->_U0._C28.Param3=0;
        FP->_U0._C28.Param4=0;
        FP->_U0._C28.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C28,40,"P_so_write_real64_wreal64");
      CASE(40):  
        FP->_U0._C29.Param0=FP->fs_566;
        FP->_U0._C29.Param1=FP->ts_567;
        FP->_U0._C29.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C29,41,"P_so_write_nl_wstring");
      CASE(41):  
        FP->_Header.Temp.VBYTE = 0;
        OUTPUT1(FP->toFarm_568,&FP->_Header.Temp.VBYTE,42);
      CASE(42):  
        OUTPUT4(FP->toFarm_568,&FP->EqnType_581,43);
      CASE(43):  
        OUTPUT4(FP->toFarm_568,&FP->NoOfSamples_578,44);
      CASE(44):  
        OUTPUT8(FP->toFarm_568,&FP->SampleDeltaX_573,45);
      CASE(45):  
        OUTPUT8(FP->toFarm_568,&FP->Power_575,46);
      CASE(46):  
        FP->_Header.Temp.VINT = 0;
        OUTPUT4(FP->toFarm_568,&FP->_Header.Temp.VINT,47);
      CASE(47):  
        FP->Time1_586 = ReadTimer();
        STARTP(&FP->_U0._S13._T3,P_618,&FP->_U0._S13._C33,"P_618",0);
        STARTP(&FP->_U0._S13._T4,P_619,&FP->_U0._S13._C34,"P_619",1);
        WAITP(48);
      CASE(48):  
        FP->Time2_587 = ReadTimer();
        {
          static BYTE TMP[20] = {86,97,108,117,101,32,111,102,32,73,110,116,101,103,114,97,108,32,61,32};
          FP->_U0._S14._V13_620 = TMP;
        }
        FP->_U0._S14._C35.Param0=FP->fs_566;
        FP->_U0._S14._C35.Param1=FP->ts_567;
        FP->_U0._S14._C35.Param2_Dim0=20;
        FP->_U0._S14._C35.Param2= (BYTE*)FP->_U0._S14._V13_620;
        FP->_U0._S14._C35.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S14._C35,49,"P_so_write_string_wstring");
      CASE(49):  
        FP->_U0._C36.Param0=FP->fs_566;
        FP->_U0._C36.Param1=FP->ts_567;
        FP->_U0._C36.Param2=FP->Sum_576;
        FP->_U0._C36.Param3=0;
        FP->_U0._C36.Param4=0;
        FP->_U0._C36.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C36,50,"P_so_write_real64_wreal64");
      CASE(50):  
        FP->_U0._C37.Param0=FP->fs_566;
        FP->_U0._C37.Param1=FP->ts_567;
        FP->_U0._C37.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C37,51,"P_so_write_nl_wstring");
      CASE(51):  
        {
          static BYTE TMP[17] = {67,97,108,99,117,108,97,116,105,111,110,32,116,111,111,107,32};
          FP->_U0._S15._V14_621 = TMP;
        }
        FP->_U0._S15._C38.Param0=FP->fs_566;
        FP->_U0._S15._C38.Param1=FP->ts_567;
        FP->_U0._S15._C38.Param2_Dim0=17;
        FP->_U0._S15._C38.Param2= (BYTE*)FP->_U0._S15._V14_621;
        FP->_U0._S15._C38.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S15._C38,52,"P_so_write_string_wstring");
      CASE(52):  
        FP->Elapsed_585 = (REAL64INTROUND((FP->Time2_587 - FP->Time1_586)) / 1.56250e+04); 
        FP->_U0._C39.Param0=FP->fs_566;
        FP->_U0._C39.Param1=FP->ts_567;
        FP->_U0._C39.Param2=FP->Elapsed_585;
        FP->_U0._C39.Param3=3;
        FP->_U0._C39.Param4=3;
        FP->_U0._C39.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._C39,53,"P_so_write_real64_wreal64");
      CASE(53):  
        {
          static BYTE TMP[8] = {32,115,101,99,111,110,100,115};
          FP->_U0._S16._V15_622 = TMP;
        }
        FP->_U0._S16._C40.Param0=FP->fs_566;
        FP->_U0._S16._C40.Param1=FP->ts_567;
        FP->_U0._S16._C40.Param2_Dim0=8;
        FP->_U0._S16._C40.Param2= (BYTE*)FP->_U0._S16._V15_622;
        FP->_U0._S16._C40.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S16._C40,54,"P_so_write_string_wstring");
      CASE(54):  
        FP->_U0._C41.Param0=FP->fs_566;
        FP->_U0._C41.Param1=FP->ts_567;
        FP->_U0._C41.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C41,55,"P_so_write_nl_wstring");
      CASE(55):  
        {
          static BYTE TMP[39] = {87,111,114,107,101,114,32,32,32,32,32,80,97,99,107,101,116,115,32,80,
             114,111,99,101,115,115,101,100,32,32,32,32,80,101,114,99,101,110,116};
          FP->_U0._S17._V16_623 = TMP;
        }
        FP->_U0._S17._C42.Param0=FP->fs_566;
        FP->_U0._S17._C42.Param1=FP->ts_567;
        FP->_U0._S17._C42.Param2_Dim0=39;
        FP->_U0._S17._C42.Param2= (BYTE*)FP->_U0._S17._V16_623;
        FP->_U0._S17._C42.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S17._C42,56,"P_so_write_string_wstring");
      CASE(56):  
        FP->_U0._C43.Param0=FP->fs_566;
        FP->_U0._C43.Param1=FP->ts_567;
        FP->_U0._C43.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._C43,57,"P_so_write_nl_wstring");
      CASE(57):  
        FP->_U0._S18.i_624 = 0;
        FP->_U0._S18.i_624_End = FP->_U0._S18.i_624 + FP->NoOfWorkers_590;
        GOTO(58);
      CASE(59):  
        FP->_U0._S18._U1._C44.Param0=FP->fs_566;
        FP->_U0._S18._U1._C44.Param1=FP->ts_567;
        FP->_U0._S18._U1._C44.Param2=FP->_U0._S18.i_624;
        FP->_U0._S18._U1._C44.Param3=4;
        FP->_U0._S18._U1._C44.Chain = NULL;
        CALL(P_so_write_int_wint,&FP->_U0._S18._U1._C44,60,"P_so_write_int_wint");
      CASE(60):  
        {
          static BYTE TMP[6] = {32,32,32,32,32,32};
          FP->_U0._S18._U1._S19._V17_625 = TMP;
        }
        FP->_U0._S18._U1._S19._C45.Param0=FP->fs_566;
        FP->_U0._S18._U1._S19._C45.Param1=FP->ts_567;
        FP->_U0._S18._U1._S19._C45.Param2_Dim0=6;
        FP->_U0._S18._U1._S19._C45.Param2= (BYTE*)FP->_U0._S18._U1._S19._V17_625;
        FP->_U0._S18._U1._S19._C45.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S18._U1._S19._C45,61,"P_so_write_string_wstring");
      CASE(61):  
        FP->_U0._S18._U1._C46.Param0=FP->fs_566;
        FP->_U0._S18._U1._C46.Param1=FP->ts_567;
        FP->_U0._S18._U1._C46.Param2=*((INT*)(((BYTE*)FP->PktCount_589)+(RNGCHK(FP->_U0._S18.i_624,16)*sizeof(INT))));
        FP->_U0._S18._U1._C46.Param3=10;
        FP->_U0._S18._U1._C46.Chain = NULL;
        CALL(P_so_write_int_wint,&FP->_U0._S18._U1._C46,62,"P_so_write_int_wint");
      CASE(62):  
        {
          static BYTE TMP[14] = {32,32,32,32,32,32,32,32,32,32,32,32,32,32};
          FP->_U0._S18._U1._S20._V18_626 = TMP;
        }
        FP->_U0._S18._U1._S20._C47.Param0=FP->fs_566;
        FP->_U0._S18._U1._S20._C47.Param1=FP->ts_567;
        FP->_U0._S18._U1._S20._C47.Param2_Dim0=14;
        FP->_U0._S18._U1._S20._C47.Param2= (BYTE*)FP->_U0._S18._U1._S20._V18_626;
        FP->_U0._S18._U1._S20._C47.Chain = NULL;
        CALL(P_so_write_string_wstring,&FP->_U0._S18._U1._S20._C47,63,"P_so_write_string_wstring");
      CASE(63):  
        FP->_U0._S18._U1._C48.Param0=FP->fs_566;
        FP->_U0._S18._U1._C48.Param1=FP->ts_567;
        FP->_U0._S18._U1._C48.Param2=(REAL64INTROUND((*((INT*)(((BYTE*)FP->PktCount_589)+(RNGCHK(FP->_U0._S18.i_624,16)*sizeof(INT)))) * 100)) / REAL64INTROUND(FP->NoOfPackets_579));
        FP->_U0._S18._U1._C48.Param3=3;
        FP->_U0._S18._U1._C48.Param4=2;
        FP->_U0._S18._U1._C48.Chain = NULL;
        CALL(P_so_write_real64_wreal64,&FP->_U0._S18._U1._C48,64,"P_so_write_real64_wreal64");
      CASE(64):  
        FP->_U0._S18._U1._C49.Param0=FP->fs_566;
        FP->_U0._S18._U1._C49.Param1=FP->ts_567;
        FP->_U0._S18._U1._C49.Chain = NULL;
        CALL(P_so_write_nl_wstring,&FP->_U0._S18._U1._C49,65,"P_so_write_nl_wstring");
      CASE(65):  
        FP->_U0._S18.i_624++;
      CASE(58):  
        if (FP->_U0._S18.i_624 != FP->_U0._S18.i_624_End) GOTO(59);
      CASE(1):  
        if (FP->More_583)
        {
          GOTO(2);
        }
        FP->_Header.Temp.VBYTE = 4;
        OUTPUT1(FP->toFarm_568,&FP->_Header.Temp.VBYTE,66);
      CASE(66):  
        INPUT1(FP->fromFarm_569,&FP->_Header.Temp.VBYTE,67);
      CASE(67):  
        switch(FP->_Header.Temp.VBYTE)
        {
          case 4: GOTO (69);
          default: SETERR(MSG_CASEIN);
        }
        break;
      CASE(69):  
        GOTO(68);
      CASE(68):  
        FP->_U0._C50.Param0=FP->fs_566;
        FP->_U0._C50.Param1=FP->ts_567;
        FP->_U0._C50.Param2=0x3B9AC9FF;
        FP->_U0._C50.Chain = NULL;
        CALL(P_so_exit_exit,&FP->_U0._C50,70,"P_so_exit_exit");
      CASE(70):  
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}


/*}}}*/
#endif
