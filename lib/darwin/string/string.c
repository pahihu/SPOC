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
typedef struct SF_F_is_in_range_string tSF_F_is_in_range_string;
typedef struct SF_F_is_upper_string tSF_F_is_upper_string;
typedef struct SF_F_is_lower_string tSF_F_is_lower_string;
typedef struct SF_F_is_digit_string tSF_F_is_digit_string;
typedef struct SF_F_is_hex_digit_string tSF_F_is_hex_digit_string;
typedef struct SF_P_to_upper_case_string tSF_P_to_upper_case_string;
typedef struct SF_P_to_lower_case_string tSF_P_to_lower_case_string;
typedef struct SF_F_compare_strings_string tSF_F_compare_strings_string;
typedef struct SF_F_eqstr_string tSF_F_eqstr_string;
typedef struct SF_P_append_text_string tSF_P_append_text_string;
typedef struct SF_F_char_pos_string tSF_F_char_pos_string;
INT F_char_pos_string(BYTE c_120,INT s_121_Dim0,BYTE *s_121);
struct SF_F_is_in_range_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_is_upper_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_is_lower_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_is_digit_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_is_hex_digit_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_to_upper_case_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_to_lower_case_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_compare_strings_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_eqstr_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_append_text_string
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_char_pos_string
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
  {"BOOL F_is_in_range_string(BYTE c_74,BYTE lower_75,BYTE upper_76);",NULL,0},
  {"BOOL F_is_upper_string(BYTE c_79);",NULL,0},
  {"BOOL F_is_lower_string(BYTE c_83);",NULL,0},
  {"BOOL F_is_digit_string(BYTE c_87);",NULL,0},
  {"BOOL F_is_hex_digit_string(BYTE c_91);",NULL,0},
  {"void P_to_upper_case_string (INT str_95_Dim0,BYTE *str_95);",NULL,0},
  {"void P_to_lower_case_string (INT str_98_Dim0,BYTE *str_98);",NULL,0},
  {"INT F_compare_strings_string(INT s1_101_Dim0,BYTE *s1_101,INT s2_102_Dim0,BYTE *s2_102);",NULL,0},
  {"BOOL F_eqstr_string(INT s1_109_Dim0,BYTE *s1_109,INT s2_110_Dim0,BYTE *s2_110);",NULL,0},
  {"void P_append_text_string (INT *len_116,INT s_117_Dim0,BYTE *s_117,INT text_118_Dim0,BYTE *text_118);",NULL,0},
  {"INT F_char_pos_string(BYTE c_120,INT s_121_Dim0,BYTE *s_121);",NULL,0},

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
    name="string.hdr";
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
    fprintf(f,"#ifndef __string_INCLUDES__\n");
    fprintf(f,"#define __string_INCLUDES__\n");
    
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
#include <ctype.h>
#include <string.h>
BOOL F_is_in_range_string(BYTE c_74,BYTE lower_75,BYTE upper_76)
{
  BOOL _Result;
  tSF_F_is_in_range_string SF;
  tSF_F_is_in_range_string *FP = &SF;
  _Result = ((c_74 >= lower_75) && (c_74 <= upper_76));
  return _Result;
}

BOOL F_is_upper_string(BYTE c_79)
{
  BOOL _Result;
  tSF_F_is_upper_string SF;
  tSF_F_is_upper_string *FP = &SF;
  {
    BOOL res_80;
    res_80 = isupper(c_79);
    _Result = res_80;
  }
  return _Result;
}

BOOL F_is_lower_string(BYTE c_83)
{
  BOOL _Result;
  tSF_F_is_lower_string SF;
  tSF_F_is_lower_string *FP = &SF;
  {
    BOOL res_84;
    res_84 = islower(c_83);
    _Result = res_84;
  }
  return _Result;
}

BOOL F_is_digit_string(BYTE c_87)
{
  BOOL _Result;
  tSF_F_is_digit_string SF;
  tSF_F_is_digit_string *FP = &SF;
  {
    BOOL res_88;
    res_88 = isdigit(c_87);
    _Result = res_88;
  }
  return _Result;
}

BOOL F_is_hex_digit_string(BYTE c_91)
{
  BOOL _Result;
  tSF_F_is_hex_digit_string SF;
  tSF_F_is_hex_digit_string *FP = &SF;
  {
    BOOL res_92;
    res_92 = isdigit(c_91) || (toupper(c_91) >= 'A' && toupper(c_91) <= 'F'); 
    _Result = res_92;
  }
  return _Result;
}

void P_to_upper_case_string (INT str_95_Dim0,BYTE *str_95)
{
  tSF_P_to_upper_case_string SF;
  tSF_P_to_upper_case_string *FP = &SF;
  {
    INT i_96;
    INT i_96_End;
    i_96 = 0;i_96_End = i_96 + (str_95_Dim0);
    for (; i_96 != i_96_End; i_96++)
    {
      str_95[i_96] = toupper(str_95[i_96]);
    }
  }
}

void P_to_lower_case_string (INT str_98_Dim0,BYTE *str_98)
{
  tSF_P_to_lower_case_string SF;
  tSF_P_to_lower_case_string *FP = &SF;
  {
    INT i_99;
    INT i_99_End;
    i_99 = 0;i_99_End = i_99 + (str_98_Dim0);
    for (; i_99 != i_99_End; i_99++)
    {
      str_98[i_99] = tolower(str_98[i_99]);
    }
  }
}

INT F_compare_strings_string(INT s1_101_Dim0,BYTE *s1_101,INT s2_102_Dim0,BYTE *s2_102)
{
  INT _Result;
  tSF_F_compare_strings_string SF;
  tSF_F_compare_strings_string *FP = &SF;
  {
    INT order_103;
    INT l1_104;
    INT l2_105;
    l1_104 = (INT)(s1_101_Dim0);
    l2_105 = (INT)(s2_102_Dim0);
    {
      INT len_106;
      {
        if ((l1_104 > l2_105))
        {
          len_106 = l2_105; 
        }
        else
        if (true)
        {
          len_106 = l1_104; 
        }
        else
        {
          SETERR(MSG_IF);
        }
        order_103 = 2*strncmp((char*)s1_101,(char*)s2_102,len_106); 
        if (((order_103 == 0) && (l1_104 > l2_105)))
        {
          order_103 = 1; 
        }
        else
        if (((order_103 == 0) && (l2_105 > l1_104)))
        {
          order_103 = -1; 
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
    _Result = order_103;
  }
  return _Result;
}

BOOL F_eqstr_string(INT s1_109_Dim0,BYTE *s1_109,INT s2_110_Dim0,BYTE *s2_110)
{
  BOOL _Result;
  tSF_F_eqstr_string SF;
  tSF_F_eqstr_string *FP = &SF;
  {
    BOOL eq_111;
    {
      INT l1_112;
      INT l2_113;
      l1_112 = (INT)(s1_109_Dim0);
      l2_113 = (INT)(s2_110_Dim0);
      eq_111 = (l1_112 != l2_113) ? false : (strcmp((char*)s1_109,(char*)s2_110) != 0);
    }
    _Result = eq_111;
  }
  return _Result;
}

void P_append_text_string (INT *len_116,INT s_117_Dim0,BYTE *s_117,INT text_118_Dim0,BYTE *text_118)
{
  tSF_P_append_text_string SF;
  tSF_P_append_text_string *FP = &SF;
  {
    CHK((text_118_Dim0)==text_118_Dim0,MSG_ASM);
    ASSIGN(((BYTE*)(((BYTE*)s_117)+((RNGCHKUB(*len_116,(text_118_Dim0),s_117_Dim0),RNGCHKLB(*len_116))*sizeof(BYTE)))),text_118,((text_118_Dim0)*sizeof(BYTE))); 
    *len_116 = (*len_116 + (text_118_Dim0)); 
  }
}

INT F_char_pos_string(BYTE c_120,INT s_121_Dim0,BYTE *s_121)
{
  INT _Result;
  tSF_F_char_pos_string SF;
  tSF_F_char_pos_string *FP = &SF;
  {
    INT res_122;
    {
      INT i_123;
      INT i_123_End;
      i_123 = 0;i_123_End = i_123 + (s_121_Dim0);
      for (; i_123 != i_123_End; i_123++)
      {
        if ((*((BYTE*)(((BYTE*)s_121)+(RNGCHK(i_123,s_121_Dim0)*sizeof(BYTE)))) == c_120))
        {
          res_122 = i_123; 
        }
        else
        {
          continue;
        }
        break;
      }
      if (i_123 == i_123_End) 
      if (true)
      {
        res_122 = -1; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
    _Result = res_122;
  }
  return _Result;
}


/*}}}*/
#endif
