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
typedef struct SF_F_LONGADD_Intrinsics tSF_F_LONGADD_Intrinsics;
typedef struct SF_F_LONGSUB_Intrinsics tSF_F_LONGSUB_Intrinsics;
typedef struct SF_F_ASHIFTRIGHT_Intrinsics tSF_F_ASHIFTRIGHT_Intrinsics;
typedef struct SF_F_ASHIFTLEFT_Intrinsics tSF_F_ASHIFTLEFT_Intrinsics;
typedef struct SF_F_ROTATERIGHT_Intrinsics tSF_F_ROTATERIGHT_Intrinsics;
typedef struct SF_F_ROTATELEFT_Intrinsics tSF_F_ROTATELEFT_Intrinsics;
typedef struct SF_F_LONGSUM_Intrinsics tSF_F_LONGSUM_Intrinsics;
typedef struct SF_F_LONGDIFF_Intrinsics tSF_F_LONGDIFF_Intrinsics;
typedef struct SF_F_LONGPROD_Intrinsics tSF_F_LONGPROD_Intrinsics;
typedef struct SF_F_LONGDIV_Intrinsics tSF_F_LONGDIV_Intrinsics;
typedef struct SF_F_SHIFTRIGHT_Intrinsics tSF_F_SHIFTRIGHT_Intrinsics;
typedef struct SF_F_SHIFTLEFT_Intrinsics tSF_F_SHIFTLEFT_Intrinsics;
typedef struct SF_F_NORMALISE_Intrinsics tSF_F_NORMALISE_Intrinsics;
typedef struct SF_F_ABS_Intrinsics tSF_F_ABS_Intrinsics;
typedef struct SF_F_DABS_Intrinsics tSF_F_DABS_Intrinsics;
typedef struct SF_F_SCALEB_Intrinsics tSF_F_SCALEB_Intrinsics;
typedef struct SF_F_DSCALEB_Intrinsics tSF_F_DSCALEB_Intrinsics;
typedef struct SF_F_COPYSIGN_Intrinsics tSF_F_COPYSIGN_Intrinsics;
typedef struct SF_F_DCOPYSIGN_Intrinsics tSF_F_DCOPYSIGN_Intrinsics;
typedef struct SF_F_SQRT_Intrinsics tSF_F_SQRT_Intrinsics;
typedef struct SF_F_DSQRT_Intrinsics tSF_F_DSQRT_Intrinsics;
typedef struct SF_F_DNEXTAFTER_Intrinsics tSF_F_DNEXTAFTER_Intrinsics;
typedef struct SF_F_REAL32OP_Intrinsics tSF_F_REAL32OP_Intrinsics;
typedef struct SF_F_REAL64OP_Intrinsics tSF_F_REAL64OP_Intrinsics;
typedef struct SF_F_IEEE32OP_Intrinsics tSF_F_IEEE32OP_Intrinsics;
typedef struct SF_F_IEEE64OP_Intrinsics tSF_F_IEEE64OP_Intrinsics;
typedef struct SF_F_REAL32REM_Intrinsics tSF_F_REAL32REM_Intrinsics;
typedef struct SF_F_REAL64REM_Intrinsics tSF_F_REAL64REM_Intrinsics;
typedef struct SF_F_IEEE32REM_Intrinsics tSF_F_IEEE32REM_Intrinsics;
typedef struct SF_F_IEEE64REM_Intrinsics tSF_F_IEEE64REM_Intrinsics;
typedef struct SF_F_REAL32EQ_Intrinsics tSF_F_REAL32EQ_Intrinsics;
typedef struct SF_F_REAL64EQ_Intrinsics tSF_F_REAL64EQ_Intrinsics;
typedef struct SF_F_REAL32GT_Intrinsics tSF_F_REAL32GT_Intrinsics;
typedef struct SF_F_REAL64GT_Intrinsics tSF_F_REAL64GT_Intrinsics;
typedef struct SF_F_IEEECOMPARE_Intrinsics tSF_F_IEEECOMPARE_Intrinsics;
typedef struct SF_F_DIEEECOMPARE_Intrinsics tSF_F_DIEEECOMPARE_Intrinsics;
typedef struct SF_F_LOGB_Intrinsics tSF_F_LOGB_Intrinsics;
typedef struct SF_F_DLOGB_Intrinsics tSF_F_DLOGB_Intrinsics;
typedef struct SF_F_ISNAN_Intrinsics tSF_F_ISNAN_Intrinsics;
typedef struct SF_F_DISNAN_Intrinsics tSF_F_DISNAN_Intrinsics;
typedef struct SF_F_NOTFINITE_Intrinsics tSF_F_NOTFINITE_Intrinsics;
typedef struct SF_F_DNOTFINITE_Intrinsics tSF_F_DNOTFINITE_Intrinsics;
typedef struct SF_F_ORDERED_Intrinsics tSF_F_ORDERED_Intrinsics;
typedef struct SF_F_DORDERED_Intrinsics tSF_F_DORDERED_Intrinsics;
typedef struct SF_F_BITCOUNT_Intrinsics tSF_F_BITCOUNT_Intrinsics;
typedef struct SF_F_BITREVNBITS_Intrinsics tSF_F_BITREVNBITS_Intrinsics;
typedef struct SF_F_BITREVWORD_Intrinsics tSF_F_BITREVWORD_Intrinsics;
typedef struct SF_P_ASSERT_Intrinsics tSF_P_ASSERT_Intrinsics;
typedef struct SF_P_CAUSEERROR_Intrinsics tSF_P_CAUSEERROR_Intrinsics;
typedef struct SF_P_264 tSF_P_264;
typedef struct SF_P_RESCHEDULE_Intrinsics tSF_P_RESCHEDULE_Intrinsics;
void P_RESCHEDULE_Intrinsics (tSF_P_RESCHEDULE_Intrinsics *FP);
struct SF_F_LONGADD_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LONGSUB_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ASHIFTRIGHT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ASHIFTLEFT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ROTATERIGHT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ROTATELEFT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LONGSUM_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LONGDIFF_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LONGPROD_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LONGDIV_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_SHIFTRIGHT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_SHIFTLEFT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_NORMALISE_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ABS_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DABS_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_SCALEB_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DSCALEB_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_COPYSIGN_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DCOPYSIGN_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_SQRT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DSQRT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DNEXTAFTER_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL32OP_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL64OP_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_IEEE32OP_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_IEEE64OP_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL32REM_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL64REM_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_IEEE32REM_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_IEEE64REM_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL32EQ_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL64EQ_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL32GT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_REAL64GT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_IEEECOMPARE_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DIEEECOMPARE_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_LOGB_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DLOGB_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ISNAN_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DISNAN_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_NOTFINITE_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DNOTFINITE_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_ORDERED_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_DORDERED_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_BITCOUNT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_BITREVNBITS_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_F_BITREVWORD_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_ASSERT_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_CAUSEERROR_Intrinsics
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_264
{
  tHeader _Header;
  tSF_P_RESCHEDULE_Intrinsics *Chain;
};
struct SF_P_RESCHEDULE_Intrinsics
{
  tHeader _Header;
  void *Chain;
  tTask _T0;
  tSF_P_264 _C0;
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
  {"INT F_LONGADD_Intrinsics(INT left_1,INT right_2,INT carry_in_3);",NULL,0},
  {"INT F_LONGSUB_Intrinsics(INT left_5,INT right_6,INT borrow_in_7);",NULL,0},
  {"INT F_ASHIFTRIGHT_Intrinsics(INT w_9,INT places_10);",NULL,0},
  {"INT F_ASHIFTLEFT_Intrinsics(INT w_14,INT places_15);",NULL,0},
  {"INT F_ROTATERIGHT_Intrinsics(INT w_19,INT places_20);",NULL,0},
  {"INT F_ROTATELEFT_Intrinsics(INT w_27,INT places_28);",NULL,0},
  {"void F_LONGSUM_Intrinsics(INT left_35,INT right_36,INT carry_in_37,INT *Param0,INT *Param1);",NULL,0},
  {"void F_LONGDIFF_Intrinsics(INT left_42,INT right_43,INT borrow_in_44,INT *Param0,INT *Param1);",NULL,0},
  {"void F_LONGPROD_Intrinsics(INT left_56,INT right_57,INT carry_in_58,INT *Param0,INT *Param1);",NULL,0},
  {"void F_LONGDIV_Intrinsics(INT divhi_72,INT divlo_73,INT divisor_74,INT *Param0,INT *Param1);",NULL,0},
  {"void F_SHIFTRIGHT_Intrinsics(INT hi_79,INT lo_80,INT places_81,INT *Param0,INT *Param1);",NULL,0},
  {"void F_SHIFTLEFT_Intrinsics(INT hi_87,INT lo_88,INT places_89,INT *Param0,INT *Param1);",NULL,0},
  {"void F_NORMALISE_Intrinsics(INT hi_in_95,INT lo_in_96,INT *Param0,INT *Param1,INT *Param2);",NULL,0},
  {"REAL32 F_ABS_Intrinsics(REAL32 x_105);",NULL,0},
  {"REAL64 F_DABS_Intrinsics(REAL64 x_109);",NULL,0},
  {"REAL32 F_SCALEB_Intrinsics(REAL32 x_113,INT n_114);",NULL,0},
  {"REAL64 F_DSCALEB_Intrinsics(REAL64 x_118,INT n_119);",NULL,0},
  {"REAL32 F_COPYSIGN_Intrinsics(REAL32 x_123,REAL32 y_124);",NULL,0},
  {"REAL64 F_DCOPYSIGN_Intrinsics(REAL64 x_128,REAL64 y_129);",NULL,0},
  {"REAL32 F_SQRT_Intrinsics(REAL32 x_133);",NULL,0},
  {"REAL64 F_DSQRT_Intrinsics(REAL64 x_137);",NULL,0},
  {"REAL64 F_DNEXTAFTER_Intrinsics(REAL64 x_141,REAL64 y_142);",NULL,0},
  {"REAL32 F_REAL32OP_Intrinsics(REAL32 x_146,INT op_147,REAL32 y_148);",NULL,0},
  {"REAL64 F_REAL64OP_Intrinsics(REAL64 x_152,INT op_153,REAL64 y_154);",NULL,0},
  {"void F_IEEE32OP_Intrinsics(REAL32 x_158,INT Rm_159,INT op_160,REAL32 y_161,BOOL *Param0,REAL32 *Param1);",NULL,0},
  {"void F_IEEE64OP_Intrinsics(REAL64 x_163,INT Rm_164,INT op_165,REAL64 y_166,BOOL *Param0,REAL64 *Param1);",NULL,0},
  {"REAL32 F_REAL32REM_Intrinsics(REAL32 x_168,REAL32 y_169);",NULL,0},
  {"REAL64 F_REAL64REM_Intrinsics(REAL64 x_173,REAL64 y_174);",NULL,0},
  {"void F_IEEE32REM_Intrinsics(REAL32 x_178,REAL32 y_179,BOOL *Param0,REAL32 *Param1);",NULL,0},
  {"void F_IEEE64REM_Intrinsics(REAL64 x_183,REAL64 y_184,BOOL *Param0,REAL64 *Param1);",NULL,0},
  {"BOOL F_REAL32EQ_Intrinsics(REAL32 x_188,REAL32 y_189);",NULL,0},
  {"BOOL F_REAL64EQ_Intrinsics(REAL64 x_191,REAL64 y_192);",NULL,0},
  {"BOOL F_REAL32GT_Intrinsics(REAL32 x_194,REAL32 y_195);",NULL,0},
  {"BOOL F_REAL64GT_Intrinsics(REAL64 x_197,REAL64 y_198);",NULL,0},
  {"INT F_IEEECOMPARE_Intrinsics(REAL32 x_200,REAL32 y_201);",NULL,0},
  {"INT F_DIEEECOMPARE_Intrinsics(REAL64 x_205,REAL64 y_206);",NULL,0},
  {"REAL32 F_LOGB_Intrinsics(REAL32 X_210);",NULL,0},
  {"REAL32 F_DLOGB_Intrinsics(REAL64 X_214);",NULL,0},
  {"BOOL F_ISNAN_Intrinsics(REAL32 X_218);",NULL,0},
  {"BOOL F_DISNAN_Intrinsics(REAL64 X_222);",NULL,0},
  {"BOOL F_NOTFINITE_Intrinsics(REAL32 X_226);",NULL,0},
  {"BOOL F_DNOTFINITE_Intrinsics(REAL64 X_230);",NULL,0},
  {"BOOL F_ORDERED_Intrinsics(REAL32 X_234,REAL32 Y_235);",NULL,0},
  {"BOOL F_DORDERED_Intrinsics(REAL64 X_237,REAL64 Y_238);",NULL,0},
  {"INT F_BITCOUNT_Intrinsics(INT Word_240,INT CountIn_241);",NULL,0},
  {"INT F_BITREVNBITS_Intrinsics(INT x_249,INT n_250);",NULL,0},
  {"INT F_BITREVWORD_Intrinsics(INT x_256);",NULL,0},
  {"void P_ASSERT_Intrinsics (BOOL test_261);",NULL,0},
  {"void P_CAUSEERROR_Intrinsics (void);",NULL,0},
  {"void P_RESCHEDULE_Intrinsics (tSF_P_RESCHEDULE_Intrinsics *FP);","P_RESCHEDULE_Intrinsics",sizeof(tSF_P_RESCHEDULE_Intrinsics)},

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
    name="Intrinsics.hdr";
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
    fprintf(f,"#ifndef __Intrinsics_INCLUDES__\n");
    fprintf(f,"#define __Intrinsics_INCLUDES__\n");
    
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
#include <math.h>
INT F_LONGADD_Intrinsics(INT left_1,INT right_2,INT carry_in_3)
{
  INT _Result;
  tSF_F_LONGADD_Intrinsics SF;
  tSF_F_LONGADD_Intrinsics *FP = &SF;
  _Result = ((left_1 + right_2) + (carry_in_3 & 1));
  return _Result;
}

INT F_LONGSUB_Intrinsics(INT left_5,INT right_6,INT borrow_in_7)
{
  INT _Result;
  tSF_F_LONGSUB_Intrinsics SF;
  tSF_F_LONGSUB_Intrinsics *FP = &SF;
  _Result = ((left_5 - right_6) - (borrow_in_7 & 1));
  return _Result;
}

INT F_ASHIFTRIGHT_Intrinsics(INT w_9,INT places_10)
{
  INT _Result;
  tSF_F_ASHIFTRIGHT_Intrinsics SF;
  tSF_F_ASHIFTRIGHT_Intrinsics *FP = &SF;
  {
    INT r_11;
    r_11=w_9>>places_10;
    _Result = r_11;
  }
  return _Result;
}

INT F_ASHIFTLEFT_Intrinsics(INT w_14,INT places_15)
{
  INT _Result;
  tSF_F_ASHIFTLEFT_Intrinsics SF;
  tSF_F_ASHIFTLEFT_Intrinsics *FP = &SF;
  {
    INT r_16;
    r_16=w_14<<places_15;
    _Result = r_16;
  }
  return _Result;
}

INT F_ROTATERIGHT_Intrinsics(INT w_19,INT places_20)
{
  INT _Result;
  tSF_F_ROTATERIGHT_Intrinsics SF;
  tSF_F_ROTATERIGHT_Intrinsics *FP = &SF;
  {
    INT _V0_266;
    BYTE *a_21;
    INT places_23;
    INT rest_24;
    _V0_266 = (INT)0;
    {
      static INT TMP; TMP = 0;
      a_21 = (BYTE*)&TMP;
    }
    places_23 = (INT)(places_20 % 32);
    rest_24 = (INT)(32 - places_23);
    _Result = (LOGSHIFTRIGHT(INT,w_19,places_23) + LOGSHIFTLEFT(INT,w_19,rest_24));
  }
  return _Result;
}

INT F_ROTATELEFT_Intrinsics(INT w_27,INT places_28)
{
  INT _Result;
  tSF_F_ROTATELEFT_Intrinsics SF;
  tSF_F_ROTATELEFT_Intrinsics *FP = &SF;
  {
    INT _V0_267;
    BYTE *a_29;
    INT places_31;
    INT rest_32;
    _V0_267 = (INT)0;
    {
      static INT TMP; TMP = 0;
      a_29 = (BYTE*)&TMP;
    }
    places_31 = (INT)(places_28 % 32);
    rest_32 = (INT)(32 - places_31);
    _Result = (LOGSHIFTLEFT(INT,w_27,places_31) + LOGSHIFTRIGHT(INT,w_27,rest_32));
  }
  return _Result;
}

void F_LONGSUM_Intrinsics(INT left_35,INT right_36,INT carry_in_37,INT *Param0,INT *Param1)
{
  tSF_F_LONGSUM_Intrinsics SF;
  tSF_F_LONGSUM_Intrinsics *FP = &SF;
  {
    INT carry_out_38;
    INT sum_39;
    {
      sum_39 = ((left_35 + right_36) + (carry_in_37 & 1)); 
      carry_out_38 = INTBOOL(((((left_35 < 0) || (right_36 < 0)) && (sum_39 >= 0)) || ((left_35 < 0) && (right_36 < 0)))); 
    }
    *Param0=carry_out_38;
    *Param1=sum_39;
  }
}

void F_LONGDIFF_Intrinsics(INT left_42,INT right_43,INT borrow_in_44,INT *Param0,INT *Param1)
{
  tSF_F_LONGDIFF_Intrinsics SF;
  tSF_F_LONGDIFF_Intrinsics *FP = &SF;
  {
    INT borrow_out_45;
    INT diff_46;
    {
      INT64 left_i_47;
      INT64 right_i_48;
      INT64 borrow_i_49;
      INT64 diff_i_50;
      INT _V0_268;
      BYTE *word_51;
      _V0_268 = (INT)0;
      {
        static INT TMP; TMP = 0;
        word_51 = (BYTE*)&TMP;
      }
      {
        left_i_47 = (INT64INT(left_42) & 0xFFFFFFFF); 
        right_i_48 = (INT64INT(right_43) & 0xFFFFFFFF); 
        borrow_i_49 = INT64INT((borrow_in_44 & 1)); 
        diff_i_50 = ((left_i_47 - right_i_48) - borrow_i_49); 
        borrow_out_45 = INTBOOL((diff_i_50 < 0)); 
        diff_46 = (INT) diff_i_50;      
      }
    }
    *Param0=borrow_out_45;
    *Param1=diff_46;
  }
}

void F_LONGPROD_Intrinsics(INT left_56,INT right_57,INT carry_in_58,INT *Param0,INT *Param1)
{
  tSF_F_LONGPROD_Intrinsics SF;
  tSF_F_LONGPROD_Intrinsics *FP = &SF;
  {
    INT prod_lo_59;
    INT prod_hi_60;
    {
      INT64 left_i_61;
      INT64 right_i_62;
      INT64 carry_i_63;
      INT64 prod_i_64;
      INT _V0_269;
      BYTE *word_65;
      _V0_269 = (INT)0;
      {
        static INT TMP; TMP = 0;
        word_65 = (BYTE*)&TMP;
      }
      {
        left_i_61 = (INT64INT(left_56) & 0xFFFFFFFF); 
        right_i_62 = (INT64INT(right_57) & 0xFFFFFFFF); 
        carry_i_63 = (INT64INT(carry_in_58) & 0xFFFFFFFF); 
        prod_i_64 = ((left_i_61 * right_i_62) + carry_i_63); 
        {
          INT *prod_i_32_68;
          INT64 _V1_270;
          INT *end_69;
          prod_i_32_68 = (INT*)&prod_i_64;
          CHK((2*sizeof(INT)) == sizeof(INT64),MSG_ARM);
          _V1_270 = (INT64)1;
          {
            static INT64 TMP; TMP = 1;
            end_69 = (INT*)&TMP;
          }
          {
            prod_lo_59 = *((INT*)(((BYTE*)prod_i_32_68)+(0*sizeof(INT)))); 
            prod_hi_60 = *((INT*)(((BYTE*)prod_i_32_68)+(1*sizeof(INT)))); 
          }
        }
      }
    }
    *Param0=prod_hi_60;
    *Param1=prod_lo_59;
  }
}

void F_LONGDIV_Intrinsics(INT divhi_72,INT divlo_73,INT divisor_74,INT *Param0,INT *Param1)
{
  tSF_F_LONGDIV_Intrinsics SF;
  tSF_F_LONGDIV_Intrinsics *FP = &SF;
  {
    INT quot_75;
    INT rem_76;
    {
      {
        unsigned long long val,rem,quot;
        val = (((unsigned long long)divhi_72)<<32)|((unsigned long long)divlo_73);
        quot = val / ((unsigned long)divisor_74);
        rem = val % ((unsigned long)divisor_74);
        quot_75 = quot;
        rem_76 = rem;
      }
    }
    *Param0=quot_75;
    *Param1=rem_76;
  }
}

void F_SHIFTRIGHT_Intrinsics(INT hi_79,INT lo_80,INT places_81,INT *Param0,INT *Param1)
{
  tSF_F_SHIFTRIGHT_Intrinsics SF;
  tSF_F_SHIFTRIGHT_Intrinsics *FP = &SF;
  {
    INT _V0_271;
    BYTE *a_82;
    INT rest_84;
    _V0_271 = (INT)0;
    {
      static INT TMP; TMP = 0;
      a_82 = (BYTE*)&TMP;
    }
    rest_84 = (INT)(32 - places_81);
    *Param0=LOGSHIFTRIGHT(INT,hi_79,places_81);
    *Param1=(LOGSHIFTLEFT(INT,hi_79,rest_84) + LOGSHIFTRIGHT(INT,lo_80,places_81));
  }
}

void F_SHIFTLEFT_Intrinsics(INT hi_87,INT lo_88,INT places_89,INT *Param0,INT *Param1)
{
  tSF_F_SHIFTLEFT_Intrinsics SF;
  tSF_F_SHIFTLEFT_Intrinsics *FP = &SF;
  {
    INT _V0_272;
    BYTE *a_90;
    INT rest_92;
    _V0_272 = (INT)0;
    {
      static INT TMP; TMP = 0;
      a_90 = (BYTE*)&TMP;
    }
    rest_92 = (INT)(32 - places_89);
    *Param0=(LOGSHIFTLEFT(INT,hi_87,places_89) + LOGSHIFTRIGHT(INT,lo_88,rest_92));
    *Param1=LOGSHIFTLEFT(INT,lo_88,places_89);
  }
}

void F_NORMALISE_Intrinsics(INT hi_in_95,INT lo_in_96,INT *Param0,INT *Param1,INT *Param2)
{
  tSF_F_NORMALISE_Intrinsics SF;
  tSF_F_NORMALISE_Intrinsics *FP = &SF;
  {
    INT places_97;
    INT hi_out_98;
    INT lo_out_99;
    {
      INT _V0_273;
      BYTE *word_100;
      _V0_273 = (INT)0;
      {
        static INT TMP; TMP = 0;
        word_100 = (BYTE*)&TMP;
      }
      {
        lo_out_99 = lo_in_96; 
        hi_out_98 = hi_in_95; 
        if (((hi_in_95 == 0) && (lo_in_96 == 0)))
        {
          places_97 = 64; 
        }
        else
        if (true)
        {
          {
            if ((hi_in_95 == 0))
            {
              {
                places_97 = 32; 
                hi_out_98 = lo_out_99; 
              }
            }
            else
            if (true)
            {
              places_97 = 0; 
            }
            else
            {
              SETERR(MSG_IF);
            }
            {
              while (((MOSTNEGINT32 & hi_out_98) == 0))
              {
                {
                  places_97 = (places_97 + 1); 
                  hi_out_98 = LOGSHIFTLEFT(INT,hi_out_98,1); 
                }
              }
            }
            {
              F_SHIFTLEFT_Intrinsics(hi_in_95,lo_in_96,places_97,&hi_out_98,&lo_out_99);
            }
          }
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
    }
    *Param0=places_97;
    *Param1=hi_out_98;
    *Param2=lo_out_99;
  }
}

REAL32 F_ABS_Intrinsics(REAL32 x_105)
{
  REAL32 _Result;
  tSF_F_ABS_Intrinsics SF;
  tSF_F_ABS_Intrinsics *FP = &SF;
  {
    REAL32 y_106;
    if ((x_105 < 0.0e+00))
    {
      y_106 = ( - x_105); 
    }
    else
    if (true)
    {
      y_106 = x_105; 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = y_106;
  }
  return _Result;
}

REAL64 F_DABS_Intrinsics(REAL64 x_109)
{
  REAL64 _Result;
  tSF_F_DABS_Intrinsics SF;
  tSF_F_DABS_Intrinsics *FP = &SF;
  {
    REAL64 y_110;
    if ((x_109 < 0.0e+00))
    {
      y_110 = ( - x_109); 
    }
    else
    if (true)
    {
      y_110 = x_109; 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = y_110;
  }
  return _Result;
}

REAL32 F_SCALEB_Intrinsics(REAL32 x_113,INT n_114)
{
  REAL32 _Result;
  tSF_F_SCALEB_Intrinsics SF;
  tSF_F_SCALEB_Intrinsics *FP = &SF;
  {
    REAL32 y_115;
    y_115 = (REAL32) ldexp(x_113,n_114); 
    _Result = y_115;
  }
  return _Result;
}

REAL64 F_DSCALEB_Intrinsics(REAL64 x_118,INT n_119)
{
  REAL64 _Result;
  tSF_F_DSCALEB_Intrinsics SF;
  tSF_F_DSCALEB_Intrinsics *FP = &SF;
  {
    REAL64 y_120;
    y_120 = (REAL64) ldexp(x_118,n_119); 
    _Result = y_120;
  }
  return _Result;
}

REAL32 F_COPYSIGN_Intrinsics(REAL32 x_123,REAL32 y_124)
{
  REAL32 _Result;
  tSF_F_COPYSIGN_Intrinsics SF;
  tSF_F_COPYSIGN_Intrinsics *FP = &SF;
  {
    REAL32 r_125;
    if ((x_123 > 0.0e+00))
    {
      if ((y_124 > 0.0e+00))
      {
        r_125 = x_123; 
      }
      else
      if (true)
      {
        r_125 = ( - x_123); 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
    else
    if (true)
    {
      if ((y_124 > 0.0e+00))
      {
        r_125 = ( - x_123); 
      }
      else
      if (true)
      {
        r_125 = x_123; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = r_125;
  }
  return _Result;
}

REAL64 F_DCOPYSIGN_Intrinsics(REAL64 x_128,REAL64 y_129)
{
  REAL64 _Result;
  tSF_F_DCOPYSIGN_Intrinsics SF;
  tSF_F_DCOPYSIGN_Intrinsics *FP = &SF;
  {
    REAL64 r_130;
    if ((x_128 > 0.0e+00))
    {
      if ((y_129 > 0.0e+00))
      {
        r_130 = x_128; 
      }
      else
      if (true)
      {
        r_130 = ( - x_128); 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
    else
    if (true)
    {
      if ((y_129 > 0.0e+00))
      {
        r_130 = ( - x_128); 
      }
      else
      if (true)
      {
        r_130 = x_128; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = r_130;
  }
  return _Result;
}

REAL32 F_SQRT_Intrinsics(REAL32 x_133)
{
  REAL32 _Result;
  tSF_F_SQRT_Intrinsics SF;
  tSF_F_SQRT_Intrinsics *FP = &SF;
  {
    REAL32 r_134;
    if ((x_133 >= 0.0e+00))
    {
      r_134 = (REAL32) sqrt(x_133);
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = r_134;
  }
  return _Result;
}

REAL64 F_DSQRT_Intrinsics(REAL64 x_137)
{
  REAL64 _Result;
  tSF_F_DSQRT_Intrinsics SF;
  tSF_F_DSQRT_Intrinsics *FP = &SF;
  {
    REAL64 r_138;
    if ((x_137 >= 0.0e+00))
    {
      r_138 = (REAL64) sqrt(x_137);
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = r_138;
  }
  return _Result;
}

REAL64 F_DNEXTAFTER_Intrinsics(REAL64 x_141,REAL64 y_142)
{
  REAL64 _Result;
  tSF_F_DNEXTAFTER_Intrinsics SF;
  tSF_F_DNEXTAFTER_Intrinsics *FP = &SF;
  {
    REAL64 next_143;
    {
      #if (defined sun4)||(defined solaris)
        next_143 = nextafter(x_141,y_142);
      #else
        SETERR("No nextafter function");
      #endif
    }
    _Result = next_143;
  }
  return _Result;
}

REAL32 F_REAL32OP_Intrinsics(REAL32 x_146,INT op_147,REAL32 y_148)
{
  REAL32 _Result;
  tSF_F_REAL32OP_Intrinsics SF;
  tSF_F_REAL32OP_Intrinsics *FP = &SF;
  {
    REAL32 res_149;
    if ((op_147 == 0))
    {
      res_149 = (x_146 + y_148); 
    }
    else
    if ((op_147 == 1))
    {
      res_149 = (x_146 - y_148); 
    }
    else
    if ((op_147 == 2))
    {
      res_149 = (x_146 * y_148); 
    }
    else
    if ((op_147 == 3))
    {
      res_149 = (x_146 / y_148); 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = res_149;
  }
  return _Result;
}

REAL64 F_REAL64OP_Intrinsics(REAL64 x_152,INT op_153,REAL64 y_154)
{
  REAL64 _Result;
  tSF_F_REAL64OP_Intrinsics SF;
  tSF_F_REAL64OP_Intrinsics *FP = &SF;
  {
    REAL64 res_155;
    if ((op_153 == 0))
    {
      res_155 = (x_152 + y_154); 
    }
    else
    if ((op_153 == 1))
    {
      res_155 = (x_152 - y_154); 
    }
    else
    if ((op_153 == 2))
    {
      res_155 = (x_152 * y_154); 
    }
    else
    if ((op_153 == 3))
    {
      res_155 = (x_152 / y_154); 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = res_155;
  }
  return _Result;
}

void F_IEEE32OP_Intrinsics(REAL32 x_158,INT Rm_159,INT op_160,REAL32 y_161,BOOL *Param0,REAL32 *Param1)
{
  tSF_F_IEEE32OP_Intrinsics SF;
  tSF_F_IEEE32OP_Intrinsics *FP = &SF;
  *Param0=false;
  *Param1=F_REAL32OP_Intrinsics(x_158,op_160,y_161);
}

void F_IEEE64OP_Intrinsics(REAL64 x_163,INT Rm_164,INT op_165,REAL64 y_166,BOOL *Param0,REAL64 *Param1)
{
  tSF_F_IEEE64OP_Intrinsics SF;
  tSF_F_IEEE64OP_Intrinsics *FP = &SF;
  *Param0=false;
  *Param1=F_REAL64OP_Intrinsics(x_163,op_165,y_166);
}

REAL32 F_REAL32REM_Intrinsics(REAL32 x_168,REAL32 y_169)
{
  REAL32 _Result;
  tSF_F_REAL32REM_Intrinsics SF;
  tSF_F_REAL32REM_Intrinsics *FP = &SF;
  {
    REAL32 res_170;
    {
      #if (defined sun4)||(defined solaris)
        res_170 = remainder(x_168,y_169);
      #else
        SETERR("remainder function");
      #endif
    }
    _Result = res_170;
  }
  return _Result;
}

REAL64 F_REAL64REM_Intrinsics(REAL64 x_173,REAL64 y_174)
{
  REAL64 _Result;
  tSF_F_REAL64REM_Intrinsics SF;
  tSF_F_REAL64REM_Intrinsics *FP = &SF;
  {
    REAL64 res_175;
    {
      #if (defined sun4)||(defined solaris)
        res_175 = remainder(x_173,y_174);
      #else
        SETERR("remainder function");
      #endif
    }
    _Result = res_175;
  }
  return _Result;
}

void F_IEEE32REM_Intrinsics(REAL32 x_178,REAL32 y_179,BOOL *Param0,REAL32 *Param1)
{
  tSF_F_IEEE32REM_Intrinsics SF;
  tSF_F_IEEE32REM_Intrinsics *FP = &SF;
  {
    REAL32 res_180;
    {
      #if (defined sun4)||(defined solaris)
        res_180 = remainder(x_178,y_179);
      #else
        SETERR("remainder function");
      #endif
    }
    *Param0=false;
    *Param1=res_180;
  }
}

void F_IEEE64REM_Intrinsics(REAL64 x_183,REAL64 y_184,BOOL *Param0,REAL64 *Param1)
{
  tSF_F_IEEE64REM_Intrinsics SF;
  tSF_F_IEEE64REM_Intrinsics *FP = &SF;
  {
    REAL64 res_185;
    {
      #if (defined sun4)||(defined solaris)
        res_185 = remainder(x_183,y_184);
      #else
        SETERR("remainder function");
      #endif
    }
    *Param0=false;
    *Param1=res_185;
  }
}

BOOL F_REAL32EQ_Intrinsics(REAL32 x_188,REAL32 y_189)
{
  BOOL _Result;
  tSF_F_REAL32EQ_Intrinsics SF;
  tSF_F_REAL32EQ_Intrinsics *FP = &SF;
  _Result = (x_188 == y_189);
  return _Result;
}

BOOL F_REAL64EQ_Intrinsics(REAL64 x_191,REAL64 y_192)
{
  BOOL _Result;
  tSF_F_REAL64EQ_Intrinsics SF;
  tSF_F_REAL64EQ_Intrinsics *FP = &SF;
  _Result = (x_191 == y_192);
  return _Result;
}

BOOL F_REAL32GT_Intrinsics(REAL32 x_194,REAL32 y_195)
{
  BOOL _Result;
  tSF_F_REAL32GT_Intrinsics SF;
  tSF_F_REAL32GT_Intrinsics *FP = &SF;
  _Result = (x_194 > y_195);
  return _Result;
}

BOOL F_REAL64GT_Intrinsics(REAL64 x_197,REAL64 y_198)
{
  BOOL _Result;
  tSF_F_REAL64GT_Intrinsics SF;
  tSF_F_REAL64GT_Intrinsics *FP = &SF;
  _Result = (x_197 > y_198);
  return _Result;
}

INT F_IEEECOMPARE_Intrinsics(REAL32 x_200,REAL32 y_201)
{
  INT _Result;
  tSF_F_IEEECOMPARE_Intrinsics SF;
  tSF_F_IEEECOMPARE_Intrinsics *FP = &SF;
  {
    INT result_202;
    if ((x_200 == y_201))
    {
      result_202 = 0; 
    }
    else
    if ((x_200 > y_201))
    {
      result_202 = 1; 
    }
    else
    if (true)
    {
      result_202 = -1; 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = result_202;
  }
  return _Result;
}

INT F_DIEEECOMPARE_Intrinsics(REAL64 x_205,REAL64 y_206)
{
  INT _Result;
  tSF_F_DIEEECOMPARE_Intrinsics SF;
  tSF_F_DIEEECOMPARE_Intrinsics *FP = &SF;
  {
    INT result_207;
    if ((x_205 == y_206))
    {
      result_207 = 0; 
    }
    else
    if ((x_205 > y_206))
    {
      result_207 = 1; 
    }
    else
    if (true)
    {
      result_207 = -1; 
    }
    else
    {
      SETERR(MSG_IF);
    }
    _Result = result_207;
  }
  return _Result;
}

REAL32 F_LOGB_Intrinsics(REAL32 X_210)
{
  REAL32 _Result;
  tSF_F_LOGB_Intrinsics SF;
  tSF_F_LOGB_Intrinsics *FP = &SF;
  {
    REAL32 result_211;
    {
      #if (defined sun4)||(defined solaris)
      result_211 = (REAL32) ilogb ((double) X_210);
      #else
      SETERR("LOGB not implemented");
      #endif
    }
    _Result = result_211;
  }
  return _Result;
}

REAL32 F_DLOGB_Intrinsics(REAL64 X_214)
{
  REAL32 _Result;
  tSF_F_DLOGB_Intrinsics SF;
  tSF_F_DLOGB_Intrinsics *FP = &SF;
  {
    REAL32 result_215;
    {
      #if (defined sun4)||(defined solaris)
      result_215 = (REAL64) ilogb (X_214);
      #else
      SETERR("LOGB not implemented");
      #endif
    }
    _Result = result_215;
  }
  return _Result;
}

BOOL F_ISNAN_Intrinsics(REAL32 X_218)
{
  BOOL _Result;
  tSF_F_ISNAN_Intrinsics SF;
  tSF_F_ISNAN_Intrinsics *FP = &SF;
  {
    BOOL result_219;
    {
      #if (defined sun4)||(defined solaris)
      result_219 = isnan ((double) X_218);
      #else
      SETERR("ISNAN not implemented");
      #endif
    }
    _Result = result_219;
  }
  return _Result;
}

BOOL F_DISNAN_Intrinsics(REAL64 X_222)
{
  BOOL _Result;
  tSF_F_DISNAN_Intrinsics SF;
  tSF_F_DISNAN_Intrinsics *FP = &SF;
  {
    BOOL result_223;
    {
      #if (defined sun4)||(defined solaris)
      result_223 = isnan (X_222);
      #else
      SETERR("DISNAN not implemented");
      #endif
    }
    _Result = result_223;
  }
  return _Result;
}

BOOL F_NOTFINITE_Intrinsics(REAL32 X_226)
{
  BOOL _Result;
  tSF_F_NOTFINITE_Intrinsics SF;
  tSF_F_NOTFINITE_Intrinsics *FP = &SF;
  {
    BOOL result_227;
    {
      #if (defined sun4)
      result_227 = isinf ((double) X_226);
      #else
      SETERR("NOTFINITE not implemented");
      #endif
    }
    _Result = result_227;
  }
  return _Result;
}

BOOL F_DNOTFINITE_Intrinsics(REAL64 X_230)
{
  BOOL _Result;
  tSF_F_DNOTFINITE_Intrinsics SF;
  tSF_F_DNOTFINITE_Intrinsics *FP = &SF;
  {
    BOOL result_231;
    {
      #if (defined sun4)
      result_231 = isinf (X_230);
      #else
      SETERR("DNOTFINITE not implemented");
      #endif
    }
    _Result = result_231;
  }
  return _Result;
}

BOOL F_ORDERED_Intrinsics(REAL32 X_234,REAL32 Y_235)
{
  BOOL _Result;
  tSF_F_ORDERED_Intrinsics SF;
  tSF_F_ORDERED_Intrinsics *FP = &SF;
  _Result = ( ! (F_ISNAN_Intrinsics(X_234) || F_ISNAN_Intrinsics(Y_235)));
  return _Result;
}

BOOL F_DORDERED_Intrinsics(REAL64 X_237,REAL64 Y_238)
{
  BOOL _Result;
  tSF_F_DORDERED_Intrinsics SF;
  tSF_F_DORDERED_Intrinsics *FP = &SF;
  _Result = ( ! (F_DISNAN_Intrinsics(X_237) || F_DISNAN_Intrinsics(Y_238)));
  return _Result;
}

INT F_BITCOUNT_Intrinsics(INT Word_240,INT CountIn_241)
{
  INT _Result;
  tSF_F_BITCOUNT_Intrinsics SF;
  tSF_F_BITCOUNT_Intrinsics *FP = &SF;
  {
    INT n_242;
    INT W_243;
    {
      INT _V0_274;
      BYTE *w_244;
      _V0_274 = (INT)0;
      {
        static INT TMP; TMP = 0;
        w_244 = (BYTE*)&TMP;
      }
      {
        W_243 = Word_240; 
        n_242 = 0; 
        {
          INT i_246;
          for (i_246 = 0; i_246 != 0 + 32; i_246++)
          {
            {
              n_242 = (n_242 + (W_243 & 1)); 
              W_243 = LOGSHIFTRIGHT(INT,W_243,1); 
            }
          }
        }
      }
    }
    _Result = (CountIn_241 + n_242);
  }
  return _Result;
}

INT F_BITREVNBITS_Intrinsics(INT x_249,INT n_250)
{
  INT _Result;
  tSF_F_BITREVNBITS_Intrinsics SF;
  tSF_F_BITREVNBITS_Intrinsics *FP = &SF;
  {
    INT r_251;
    INT X_252;
    {
      X_252 = x_249; 
      r_251 = 0; 
      {
        INT i_253;
        INT i_253_End;
        i_253 = 0;i_253_End = i_253 + n_250;
        for (; i_253 != i_253_End; i_253++)
        {
          {
            r_251 = LOGSHIFTLEFT(INT,r_251,1); 
            r_251 = (r_251 + (X_252 & 1)); 
            X_252 = LOGSHIFTRIGHT(INT,X_252,1); 
          }
        }
      }
    }
    _Result = r_251;
  }
  return _Result;
}

INT F_BITREVWORD_Intrinsics(INT x_256)
{
  INT _Result;
  tSF_F_BITREVWORD_Intrinsics SF;
  tSF_F_BITREVWORD_Intrinsics *FP = &SF;
  {
    INT _V0_275;
    BYTE *w_257;
    _V0_275 = (INT)0;
    {
      static INT TMP; TMP = 0;
      w_257 = (BYTE*)&TMP;
    }
    _Result = F_BITREVNBITS_Intrinsics(x_256,32);
  }
  return _Result;
}

void P_ASSERT_Intrinsics (BOOL test_261)
{
  tSF_P_ASSERT_Intrinsics SF;
  tSF_P_ASSERT_Intrinsics *FP = &SF;
  if (test_261)
  {
  }
  else
  if (true)
  {
    SETERR("Assertion Failed\n");
  }
  else
  {
    SETERR(MSG_IF);
  }
}

void P_CAUSEERROR_Intrinsics (void)
{
  tSF_P_CAUSEERROR_Intrinsics SF;
  tSF_P_CAUSEERROR_Intrinsics *FP = &SF;
  SETERR("CAUSEERROR() Called");
}

static void P_264 (tSF_P_264 *FP)
{
  ENDP();
}

void P_RESCHEDULE_Intrinsics (tSF_P_RESCHEDULE_Intrinsics *FP)
{
  while(true)
  {
    switch(FP->_Header.IP)
    {
      CASE(0):  
        STARTP(&FP->_T0,P_264,&FP->_C0,"P_264",1);
        WAITP(1);
      CASE(1):  
        RETURN();
      default: SETERR(MSG_IP);
    }
  }
}


/*}}}*/
#endif
