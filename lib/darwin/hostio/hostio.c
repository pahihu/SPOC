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
#include "convert.hdr"
typedef struct SF_P_so_mark_channels_hostio tSF_P_so_mark_channels_hostio;
typedef struct SF_P_so_write_char_hostio tSF_P_so_write_char_hostio;
typedef struct SF_P_so_write_nl_hostio tSF_P_so_write_nl_hostio;
typedef struct SF_P_so_write_string_hostio tSF_P_so_write_string_hostio;
typedef struct SF_P_so_write_string_nl_hostio tSF_P_so_write_string_nl_hostio;
typedef struct SF_P_so_write_int_hostio tSF_P_so_write_int_hostio;
typedef struct SF_P_so_write_int32_hostio tSF_P_so_write_int32_hostio;
typedef struct SF_P_so_write_hex_int_hostio tSF_P_so_write_hex_int_hostio;
typedef struct SF_P_so_write_hex_int32_hostio tSF_P_so_write_hex_int32_hostio;
typedef struct SF_P_so_write_real32_hostio tSF_P_so_write_real32_hostio;
typedef struct SF_P_so_write_real64_hostio tSF_P_so_write_real64_hostio;
typedef struct SF_P_so_write_int64_hostio tSF_P_so_write_int64_hostio;
typedef struct SF_P_so_write_hex_int64_hostio tSF_P_so_write_hex_int64_hostio;
typedef struct SF_P_so_fwrite_char_hostio tSF_P_so_fwrite_char_hostio;
typedef struct SF_P_so_fwrite_nl_hostio tSF_P_so_fwrite_nl_hostio;
typedef struct SF_P_so_fwrite_string_hostio tSF_P_so_fwrite_string_hostio;
typedef struct SF_P_so_fwrite_string_nl_hostio tSF_P_so_fwrite_string_nl_hostio;
typedef struct SF_P_so_fwrite_int_hostio tSF_P_so_fwrite_int_hostio;
typedef struct SF_P_so_fwrite_int32_hostio tSF_P_so_fwrite_int32_hostio;
typedef struct SF_P_so_fwrite_hex_int_hostio tSF_P_so_fwrite_hex_int_hostio;
typedef struct SF_P_so_fwrite_hex_int32_hostio tSF_P_so_fwrite_hex_int32_hostio;
typedef struct SF_P_so_fwrite_real32_hostio tSF_P_so_fwrite_real32_hostio;
typedef struct SF_P_so_fwrite_real64_hostio tSF_P_so_fwrite_real64_hostio;
typedef struct SF_P_so_fwrite_int64_hostio tSF_P_so_fwrite_int64_hostio;
typedef struct SF_P_so_fwrite_hex_int64_hostio tSF_P_so_fwrite_hex_int64_hostio;
typedef struct SF_P_MALLOC_hostio tSF_P_MALLOC_hostio;
typedef struct SF_P_FREE_hostio tSF_P_FREE_hostio;
typedef struct SF_P_STRING_OCCAM_TO_C_hostio tSF_P_STRING_OCCAM_TO_C_hostio;
typedef struct SF_P_so_open_hostio tSF_P_so_open_hostio;
typedef struct SF_P_so_close_hostio tSF_P_so_close_hostio;
typedef struct SF_P_so_test_exists_hostio tSF_P_so_test_exists_hostio;
typedef struct SF_P_so_open_temp_hostio tSF_P_so_open_temp_hostio;
typedef struct SF_P_so_popen_read_hostio tSF_P_so_popen_read_hostio;
typedef struct SF_P_so_write_hostio tSF_P_so_write_hostio;
typedef struct SF_P_so_read_hostio tSF_P_so_read_hostio;
typedef struct SF_P_so_gets_hostio tSF_P_so_gets_hostio;
typedef struct SF_P_so_puts_hostio tSF_P_so_puts_hostio;
typedef struct SF_P_so_flush_hostio tSF_P_so_flush_hostio;
typedef struct SF_P_so_seek_hostio tSF_P_so_seek_hostio;
typedef struct SF_P_so_tell_hostio tSF_P_so_tell_hostio;
typedef struct SF_P_so_eof_hostio tSF_P_so_eof_hostio;
typedef struct SF_P_so_ferror_hostio tSF_P_so_ferror_hostio;
typedef struct SF_P_so_remove_hostio tSF_P_so_remove_hostio;
typedef struct SF_P_so_rename_hostio tSF_P_so_rename_hostio;
typedef struct SF_P_so_commandline_hostio tSF_P_so_commandline_hostio;
typedef struct SF_P_so_parse_command_line_hostio tSF_P_so_parse_command_line_hostio;
typedef struct SF_P_so_getenv_hostio tSF_P_so_getenv_hostio;
typedef struct SF_P_so_time_hostio tSF_P_so_time_hostio;
typedef struct SF_P_so_system_hostio tSF_P_so_system_hostio;
typedef struct SF_P_so_exit_hostio tSF_P_so_exit_hostio;
typedef struct SF_P_so_core_hostio tSF_P_so_core_hostio;
typedef struct SF_P_so_version_hostio tSF_P_so_version_hostio;
typedef struct SF_P_so_getkey_hostio tSF_P_so_getkey_hostio;
typedef struct SF_P_so_read_echo_line_hostio tSF_P_so_read_echo_line_hostio;
typedef struct SF_P_so_read_echo_int_hostio tSF_P_so_read_echo_int_hostio;
typedef struct SF_P_sp_getkey_hostio tSF_P_sp_getkey_hostio;
void P_sp_getkey_hostio (CHAN *fs_578,CHAN *ts_579,BYTE *key_580,BYTE *result_581);
struct SF_P_so_mark_channels_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_char_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_nl_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_string_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_string_nl_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U0;
};
struct SF_P_so_write_int_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_int32_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_hex_int_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_hex_int32_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_real32_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U1;
};
struct SF_P_so_write_real64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U2;
};
struct SF_P_so_write_int64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U3;
};
struct SF_P_so_write_hex_int64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U4;
};
struct SF_P_so_fwrite_char_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_nl_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_string_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_string_nl_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U5;
};
struct SF_P_so_fwrite_int_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_int32_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_hex_int_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_hex_int32_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_fwrite_real32_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U6;
};
struct SF_P_so_fwrite_real64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U7;
};
struct SF_P_so_fwrite_int64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U8;
};
struct SF_P_so_fwrite_hex_int64_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U9;
};
struct SF_P_MALLOC_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_FREE_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_STRING_OCCAM_TO_C_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_open_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U10;
};
struct SF_P_so_close_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_test_exists_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U11;
};
struct SF_P_so_open_temp_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U12;
};
struct SF_P_so_popen_read_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_write_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_read_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_gets_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_puts_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_flush_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_seek_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_tell_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_eof_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_ferror_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_remove_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U13;
};
struct SF_P_so_rename_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U14;
};
struct SF_P_so_commandline_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_parse_command_line_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_getenv_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U15;
};
struct SF_P_so_time_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_system_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_exit_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_core_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U16;
};
struct SF_P_so_version_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_getkey_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_read_echo_line_hostio
{
  tHeader _Header;
  void *Chain;
};
struct SF_P_so_read_echo_int_hostio
{
  tHeader _Header;
  void *Chain;
  union
  {
    INT _P_dummy;
  } _U17;
};
struct SF_P_sp_getkey_hostio
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
  {"void P_so_mark_channels_hostio (CHAN *fs_183,CHAN *ts_184);",NULL,0},
  {"void P_so_write_char_hostio (CHAN *fs_189,CHAN *ts_190,BYTE ch_191);",NULL,0},
  {"void P_so_write_nl_hostio (CHAN *fs_193,CHAN *ts_194);",NULL,0},
  {"void P_so_write_string_hostio (CHAN *fs_196,CHAN *ts_197,INT string_198_Dim0,BYTE *string_198);",NULL,0},
  {"void P_so_write_string_nl_hostio (CHAN *fs_201,CHAN *ts_202,INT string_203_Dim0,BYTE *string_203);",NULL,0},
  {"void P_so_write_int_hostio (CHAN *fs_205,CHAN *ts_206,INT n_207,INT field_208);",NULL,0},
  {"void P_so_write_int32_hostio (CHAN *fs_210,CHAN *ts_211,INT32 n_212,INT field_213);",NULL,0},
  {"void P_so_write_hex_int_hostio (CHAN *fs_215,CHAN *ts_216,INT n_217,INT field_218);",NULL,0},
  {"void P_so_write_hex_int32_hostio (CHAN *fs_220,CHAN *ts_221,INT32 n_222,INT field_223);",NULL,0},
  {"void P_so_write_real32_hostio (CHAN *fs_225,CHAN *ts_226,REAL32 r_227,INT Ip_228,INT Dp_229);",NULL,0},
  {"void P_so_write_real64_hostio (CHAN *fs_233,CHAN *ts_234,REAL64 r_235,INT Ip_236,INT Dp_237);",NULL,0},
  {"void P_so_write_int64_hostio (CHAN *fs_241,CHAN *ts_242,INT64 r_243,INT field_244);",NULL,0},
  {"void P_so_write_hex_int64_hostio (CHAN *fs_248,CHAN *ts_249,INT64 r_250,INT field_251);",NULL,0},
  {"void P_so_fwrite_char_hostio (CHAN *fs_255,CHAN *ts_256,INT32 streamid_257,BYTE ch_258,BYTE *result_259);",NULL,0},
  {"void P_so_fwrite_nl_hostio (CHAN *fs_262,CHAN *ts_263,INT32 streamid_264,BYTE *result_265);",NULL,0},
  {"void P_so_fwrite_string_hostio (CHAN *fs_267,CHAN *ts_268,INT32 streamid_269,INT string_270_Dim0,BYTE *string_270,BYTE *result_271);",NULL,0},
  {"void P_so_fwrite_string_nl_hostio (CHAN *fs_274,CHAN *ts_275,INT32 streamid_276,INT string_277_Dim0,BYTE *string_277,BYTE *result_278);",NULL,0},
  {"void P_so_fwrite_int_hostio (CHAN *fs_280,CHAN *ts_281,INT32 streamid_282,INT n_283,INT field_284,BYTE *result_285);",NULL,0},
  {"void P_so_fwrite_int32_hostio (CHAN *fs_288,CHAN *ts_289,INT32 streamid_290,INT32 n_291,INT field_292,BYTE *result_293);",NULL,0},
  {"void P_so_fwrite_hex_int_hostio (CHAN *fs_295,CHAN *ts_296,INT32 streamid_297,INT n_298,INT field_299,BYTE *result_300);",NULL,0},
  {"void P_so_fwrite_hex_int32_hostio (CHAN *fs_303,CHAN *ts_304,INT32 streamid_305,INT32 n_306,INT field_307,BYTE *result_308);",NULL,0},
  {"void P_so_fwrite_real32_hostio (CHAN *fs_310,CHAN *ts_311,INT32 streamid_312,REAL32 r_313,INT Ip_314,INT Dp_315,BYTE *result_316);",NULL,0},
  {"void P_so_fwrite_real64_hostio (CHAN *fs_320,CHAN *ts_321,INT32 streamid_322,REAL64 r_323,INT Ip_324,INT Dp_325,BYTE *result_326);",NULL,0},
  {"void P_so_fwrite_int64_hostio (CHAN *fs_330,CHAN *ts_331,INT32 streamid_332,INT64 r_333,INT field_334,BYTE *result_335);",NULL,0},
  {"void P_so_fwrite_hex_int64_hostio (CHAN *fs_339,CHAN *ts_340,INT32 streamid_341,INT64 r_342,INT field_343,BYTE *result_344);",NULL,0},
  {"void P_MALLOC_hostio (INT32 *pointer_348,INT size_349);",NULL,0},
  {"void P_FREE_hostio (INT32 *pointer_351);",NULL,0},
  {"void P_STRING_OCCAM_TO_C_hostio (INT occam_353_Dim0,BYTE *occam_353,INT32 c_354);",NULL,0},
  {"void P_so_open_hostio (CHAN *fs_358,CHAN *ts_359,INT name_360_Dim0,BYTE *name_360,BYTE type_361,BYTE mode_362,INT32 *streamid_363,BYTE *result_364);",NULL,0},
  {"void P_so_close_hostio (CHAN *fs_373,CHAN *ts_374,INT32 *streamid_375,BYTE *result_376);",NULL,0},
  {"void P_so_test_exists_hostio (CHAN *fs_379,CHAN *ts_380,INT name_381_Dim0,BYTE *name_381,BOOL *exists_382);",NULL,0},
  {"void P_so_open_temp_hostio (CHAN *fs_386,CHAN *ts_387,BYTE type_388,BYTE *filename_389,INT32 *streamid_390,BYTE *result_391);",NULL,0},
  {"void P_so_popen_read_hostio (CHAN *fs_398,CHAN *ts_399,INT filename_400_Dim0,BYTE *filename_400,INT path_variable_name_401_Dim0,BYTE *path_variable_name_401,BYTE open_type_402,INT *full_len_403,INT full_name_404_Dim0,BYTE *full_name_404,INT32 *streamid_405,BYTE *result_406);",NULL,0},
  {"void P_so_write_hostio (CHAN *fs_409,CHAN *ts_410,INT32 streamid_411,INT data_412_Dim0,BYTE *data_412,INT *length_413);",NULL,0},
  {"void P_so_read_hostio (CHAN *fs_416,CHAN *ts_417,INT32 streamid_418,INT *length_419,INT data_420_Dim0,BYTE *data_420);",NULL,0},
  {"void P_so_gets_hostio (CHAN *fs_423,CHAN *ts_424,INT32 streamid_425,INT *length_426,INT data_427_Dim0,BYTE *data_427,BYTE *result_428);",NULL,0},
  {"void P_so_puts_hostio (CHAN *fs_432,CHAN *ts_433,INT32 streamid_434,INT data_435_Dim0,BYTE *data_435,BYTE *result_436);",NULL,0},
  {"void P_so_flush_hostio (CHAN *fs_442,CHAN *ts_443,INT32 streamid_444,BYTE *result_445);",NULL,0},
  {"void P_so_seek_hostio (CHAN *fs_448,CHAN *ts_449,INT32 streamid_450,INT32 offset_451,INT32 origin_452,BYTE *result_453);",NULL,0},
  {"void P_so_tell_hostio (CHAN *fs_457,CHAN *ts_458,INT32 streamid_459,INT32 *position_460,BYTE *result_461);",NULL,0},
  {"void P_so_eof_hostio (CHAN *fs_464,CHAN *ts_465,INT32 streamid_466,BYTE *result_467);",NULL,0},
  {"void P_so_ferror_hostio (CHAN *fs_470,CHAN *ts_471,INT32 streamid_472,INT32 *error_no_473,INT *length_474,INT message_475_Dim0,BYTE *message_475,BYTE *result_476);",NULL,0},
  {"void P_so_remove_hostio (CHAN *fs_479,CHAN *ts_480,INT name_481_Dim0,BYTE *name_481,BYTE *result_482);",NULL,0},
  {"void P_so_rename_hostio (CHAN *fs_486,CHAN *ts_487,INT oldname_488_Dim0,BYTE *oldname_488,INT newname_489_Dim0,BYTE *newname_489,BYTE *result_490);",NULL,0},
  {"void P_so_commandline_hostio (CHAN *fs_495,CHAN *ts_496,BYTE all_497,INT *length_498,INT string_499_Dim0,BYTE *string_499,BYTE *result_500);",NULL,0},
  {"void P_so_parse_command_line_hostio (CHAN *fs_505,CHAN *ts_506,INT option_strings_507_Dim0,INT option_strings_507_Dim1,BYTE *option_strings_507,INT option_parameters_required_508_Dim0,INT *option_parameters_required_508,INT option_exists_509_Dim0,BOOL *option_exists_509,INT option_parameters_510_Dim0,INT *option_parameters_510,INT *error_len_511,INT line_512_Dim0,BYTE *line_512);",NULL,0},
  {"void P_so_getenv_hostio (CHAN *fs_515,CHAN *ts_516,INT name_517_Dim0,BYTE *name_517,INT *length_518,INT value_519_Dim0,BYTE *value_519,BYTE *result_520);",NULL,0},
  {"void P_so_time_hostio (CHAN *fs_526,CHAN *ts_527,INT32 *localtime_528,INT32 *UTCtime_529);",NULL,0},
  {"void P_so_system_hostio (CHAN *fs_531,CHAN *ts_532,INT command_533_Dim0,BYTE *command_533,INT32 *status_534,BYTE *result_535);",NULL,0},
  {"void P_so_exit_hostio (CHAN *fs_537,CHAN *ts_538,INT32 status_539);",NULL,0},
  {"void P_so_core_hostio (CHAN *fs_541,CHAN *ts_542,INT32 offset_543,INT *bytes_read_544,INT data_545_Dim0,BYTE *data_545,BYTE *result_546);",NULL,0},
  {"void P_so_version_hostio (CHAN *fs_549,CHAN *ts_550,BYTE *version_551,BYTE *host_552,BYTE *os_553,BYTE *board_554);",NULL,0},
  {"void P_so_getkey_hostio (CHAN *fs_556,CHAN *ts_557,BYTE *key_558,BYTE *result_559);",NULL,0},
  {"void P_so_read_echo_line_hostio (CHAN *fs_562,CHAN *ts_563,INT *len_564,INT line_565_Dim0,BYTE *line_565,BYTE *result_566);",NULL,0},
  {"void P_so_read_echo_int_hostio (CHAN *fs_569,CHAN *ts_570,INT *n_571,BOOL *error_572);",NULL,0},
  {"void P_sp_getkey_hostio (CHAN *fs_578,CHAN *ts_579,BYTE *key_580,BYTE *result_581);",NULL,0},

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
    name="hostio.hdr";
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
    fprintf(f,"#ifndef __hostio_INCLUDES__\n");
    fprintf(f,"#define __hostio_INCLUDES__\n");
    
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if (OS == SUNOS)
#include <unistd.h>
#endif
void P_so_mark_channels_hostio (CHAN *fs_183,CHAN *ts_184)
{
  tSF_P_so_mark_channels_hostio SF;
  tSF_P_so_mark_channels_hostio *FP = &SF;
  {
    INT16 i_185;
    BYTE array_186[1];
    BOOL never_187;
    {
      never_187 = false; 
      if (never_187)
      {
        {
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
    }
  }
}

void P_so_write_char_hostio (CHAN *fs_189,CHAN *ts_190,BYTE ch_191)
{
  tSF_P_so_write_char_hostio SF;
  tSF_P_so_write_char_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_189,ts_190);
    putchar (ch_191) ;
  }
}

void P_so_write_nl_hostio (CHAN *fs_193,CHAN *ts_194)
{
  tSF_P_so_write_nl_hostio SF;
  tSF_P_so_write_nl_hostio *FP = &SF;
  P_so_write_char_hostio(fs_193,ts_194,10);
}

void P_so_write_string_hostio (CHAN *fs_196,CHAN *ts_197,INT string_198_Dim0,BYTE *string_198)
{
  tSF_P_so_write_string_hostio SF;
  tSF_P_so_write_string_hostio *FP = &SF;
  {
    INT i_199;
    INT i_199_End;
    i_199 = 0;i_199_End = i_199 + (string_198_Dim0);
    for (; i_199 != i_199_End; i_199++)
    {
      P_so_write_char_hostio(fs_196,ts_197,*((BYTE*)(((BYTE*)string_198)+(RNGCHK(i_199,string_198_Dim0)*sizeof(BYTE)))));
    }
  }
}

void P_so_write_string_nl_hostio (CHAN *fs_201,CHAN *ts_202,INT string_203_Dim0,BYTE *string_203)
{
  tSF_P_so_write_string_nl_hostio SF;
  tSF_P_so_write_string_nl_hostio *FP = &SF;
  {
    P_so_write_string_hostio(fs_201,ts_202,string_203_Dim0,(BYTE*)string_203);
    P_so_write_nl_hostio(fs_201,ts_202);
  }
}

void P_so_write_int_hostio (CHAN *fs_205,CHAN *ts_206,INT n_207,INT field_208)
{
  tSF_P_so_write_int_hostio SF;
  tSF_P_so_write_int_hostio *FP = &SF;
  if ((field_208 >= 0))
  {
    {
      P_so_mark_channels_hostio(fs_205,ts_206);
      printf("%*d",field_208,n_207) ;
    }
  }
  else
  {
    SETERR(MSG_IF);
  }
}

void P_so_write_int32_hostio (CHAN *fs_210,CHAN *ts_211,INT32 n_212,INT field_213)
{
  tSF_P_so_write_int32_hostio SF;
  tSF_P_so_write_int32_hostio *FP = &SF;
  P_so_write_int_hostio(fs_210,ts_211,INTINT32(n_212),field_213);
}

void P_so_write_hex_int_hostio (CHAN *fs_215,CHAN *ts_216,INT n_217,INT field_218)
{
  tSF_P_so_write_hex_int_hostio SF;
  tSF_P_so_write_hex_int_hostio *FP = &SF;
  if ((field_218 >= 0))
  {
    {
      P_so_mark_channels_hostio(fs_215,ts_216);
      printf("#%0*X",field_218,n_217) ;
    }
  }
  else
  {
    SETERR(MSG_IF);
  }
}

void P_so_write_hex_int32_hostio (CHAN *fs_220,CHAN *ts_221,INT32 n_222,INT field_223)
{
  tSF_P_so_write_hex_int32_hostio SF;
  tSF_P_so_write_hex_int32_hostio *FP = &SF;
  P_so_write_hex_int_hostio(fs_220,ts_221,INTINT32(n_222),field_223);
}

void P_so_write_real32_hostio (CHAN *fs_225,CHAN *ts_226,REAL32 r_227,INT Ip_228,INT Dp_229)
{
  tSF_P_so_write_real32_hostio SF;
  tSF_P_so_write_real32_hostio *FP = &SF;
  {
    BYTE buffer_230[24];
    INT len_231;
    {
      P_REAL32TOSTRING_convert(&len_231,24,(BYTE*)buffer_230,r_227,Ip_228,Dp_229);
      P_so_write_string_hostio(fs_225,ts_226,len_231,(BYTE*)((BYTE*)(((BYTE*)buffer_230)+((RNGCHKUB(0,len_231,24),0)*sizeof(BYTE)))));
    }
  }
}

void P_so_write_real64_hostio (CHAN *fs_233,CHAN *ts_234,REAL64 r_235,INT Ip_236,INT Dp_237)
{
  tSF_P_so_write_real64_hostio SF;
  tSF_P_so_write_real64_hostio *FP = &SF;
  {
    BYTE buffer_238[30];
    INT len_239;
    {
      P_REAL64TOSTRING_convert(&len_239,30,(BYTE*)buffer_238,r_235,Ip_236,Dp_237);
      P_so_write_string_hostio(fs_233,ts_234,len_239,(BYTE*)((BYTE*)(((BYTE*)buffer_238)+((RNGCHKUB(0,len_239,30),0)*sizeof(BYTE)))));
    }
  }
}

void P_so_write_int64_hostio (CHAN *fs_241,CHAN *ts_242,INT64 r_243,INT field_244)
{
  tSF_P_so_write_int64_hostio SF;
  tSF_P_so_write_int64_hostio *FP = &SF;
  {
    BYTE buffer_245[30];
    INT len_246;
    {
      P_INT64TOSTRING_convert(&len_246,30,(BYTE*)buffer_245,r_243);
      P_so_write_string_hostio(fs_241,ts_242,len_246,(BYTE*)((BYTE*)(((BYTE*)buffer_245)+((RNGCHKUB(0,len_246,30),0)*sizeof(BYTE)))));
    }
  }
}

void P_so_write_hex_int64_hostio (CHAN *fs_248,CHAN *ts_249,INT64 r_250,INT field_251)
{
  tSF_P_so_write_hex_int64_hostio SF;
  tSF_P_so_write_hex_int64_hostio *FP = &SF;
  {
    BYTE buffer_252[30];
    INT len_253;
    {
      P_HEX64TOSTRING_convert(&len_253,30,(BYTE*)buffer_252,r_250);
      P_so_write_string_hostio(fs_248,ts_249,len_253,(BYTE*)((BYTE*)(((BYTE*)buffer_252)+((RNGCHKUB(0,len_253,30),0)*sizeof(BYTE)))));
    }
  }
}

void P_so_fwrite_char_hostio (CHAN *fs_255,CHAN *ts_256,INT32 streamid_257,BYTE ch_258,BYTE *result_259)
{
  tSF_P_so_fwrite_char_hostio SF;
  tSF_P_so_fwrite_char_hostio *FP = &SF;
  {
    BOOL failed_260;
    {
      P_so_mark_channels_hostio(fs_255,ts_256);
      failed_260 = (fputc(ch_258,(FILE*)streamid_257) == EOF) ;
      if (failed_260)
      {
        *result_259 = 127; 
      }
      else
      if (true)
      {
        *result_259 = 0; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_fwrite_nl_hostio (CHAN *fs_262,CHAN *ts_263,INT32 streamid_264,BYTE *result_265)
{
  tSF_P_so_fwrite_nl_hostio SF;
  tSF_P_so_fwrite_nl_hostio *FP = &SF;
  P_so_fwrite_char_hostio(fs_262,ts_263,streamid_264,10,result_265);
}

void P_so_fwrite_string_hostio (CHAN *fs_267,CHAN *ts_268,INT32 streamid_269,INT string_270_Dim0,BYTE *string_270,BYTE *result_271)
{
  tSF_P_so_fwrite_string_hostio SF;
  tSF_P_so_fwrite_string_hostio *FP = &SF;
  {
    {
      INT i_272;
      INT i_272_End;
      i_272 = 0;i_272_End = i_272 + (string_270_Dim0);
      for (; i_272 != i_272_End; i_272++)
      {
        P_so_fwrite_char_hostio(fs_267,ts_268,streamid_269,*((BYTE*)(((BYTE*)string_270)+(RNGCHK(i_272,string_270_Dim0)*sizeof(BYTE)))),result_271);
      }
    }
  }
}

void P_so_fwrite_string_nl_hostio (CHAN *fs_274,CHAN *ts_275,INT32 streamid_276,INT string_277_Dim0,BYTE *string_277,BYTE *result_278)
{
  tSF_P_so_fwrite_string_nl_hostio SF;
  tSF_P_so_fwrite_string_nl_hostio *FP = &SF;
  {
    P_so_fwrite_string_hostio(fs_274,ts_275,streamid_276,string_277_Dim0,(BYTE*)string_277,result_278);
    if ((*result_278 == 0))
    {
      P_so_fwrite_nl_hostio(fs_274,ts_275,streamid_276,result_278);
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

void P_so_fwrite_int_hostio (CHAN *fs_280,CHAN *ts_281,INT32 streamid_282,INT n_283,INT field_284,BYTE *result_285)
{
  tSF_P_so_fwrite_int_hostio SF;
  tSF_P_so_fwrite_int_hostio *FP = &SF;
  if ((field_284 >= 0))
  {
    {
      BOOL failed_286;
      {
        P_so_mark_channels_hostio(fs_280,ts_281);
        failed_286 = (fprintf((FILE*)streamid_282,"%*d",field_284,n_283) == EOF) ;
        if (failed_286)
        {
          *result_285 = 127; 
        }
        else
        if (true)
        {
          *result_285 = 0; 
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
    }
  }
  else
  {
    SETERR(MSG_IF);
  }
}

void P_so_fwrite_int32_hostio (CHAN *fs_288,CHAN *ts_289,INT32 streamid_290,INT32 n_291,INT field_292,BYTE *result_293)
{
  tSF_P_so_fwrite_int32_hostio SF;
  tSF_P_so_fwrite_int32_hostio *FP = &SF;
  P_so_fwrite_int_hostio(fs_288,ts_289,streamid_290,INTINT32(n_291),field_292,result_293);
}

void P_so_fwrite_hex_int_hostio (CHAN *fs_295,CHAN *ts_296,INT32 streamid_297,INT n_298,INT field_299,BYTE *result_300)
{
  tSF_P_so_fwrite_hex_int_hostio SF;
  tSF_P_so_fwrite_hex_int_hostio *FP = &SF;
  if ((field_299 >= 0))
  {
    {
      BOOL failed_301;
      {
        P_so_mark_channels_hostio(fs_295,ts_296);
        failed_301 = (fprintf((FILE*)streamid_297,"#%0*X",field_299,n_298) == EOF) ;
        if (failed_301)
        {
          *result_300 = 127; 
        }
        else
        if (true)
        {
          *result_300 = 0; 
        }
        else
        {
          SETERR(MSG_IF);
        }
      }
    }
  }
  else
  {
    SETERR(MSG_IF);
  }
}

void P_so_fwrite_hex_int32_hostio (CHAN *fs_303,CHAN *ts_304,INT32 streamid_305,INT32 n_306,INT field_307,BYTE *result_308)
{
  tSF_P_so_fwrite_hex_int32_hostio SF;
  tSF_P_so_fwrite_hex_int32_hostio *FP = &SF;
  P_so_fwrite_hex_int_hostio(fs_303,ts_304,streamid_305,INTINT32(n_306),field_307,result_308);
}

void P_so_fwrite_real32_hostio (CHAN *fs_310,CHAN *ts_311,INT32 streamid_312,REAL32 r_313,INT Ip_314,INT Dp_315,BYTE *result_316)
{
  tSF_P_so_fwrite_real32_hostio SF;
  tSF_P_so_fwrite_real32_hostio *FP = &SF;
  {
    BYTE buffer_317[24];
    INT len_318;
    {
      P_REAL32TOSTRING_convert(&len_318,24,(BYTE*)buffer_317,r_313,Ip_314,Dp_315);
      P_so_fwrite_string_hostio(fs_310,ts_311,streamid_312,len_318,(BYTE*)((BYTE*)(((BYTE*)buffer_317)+((RNGCHKUB(0,len_318,24),0)*sizeof(BYTE)))),result_316);
    }
  }
}

void P_so_fwrite_real64_hostio (CHAN *fs_320,CHAN *ts_321,INT32 streamid_322,REAL64 r_323,INT Ip_324,INT Dp_325,BYTE *result_326)
{
  tSF_P_so_fwrite_real64_hostio SF;
  tSF_P_so_fwrite_real64_hostio *FP = &SF;
  {
    BYTE buffer_327[30];
    INT len_328;
    {
      P_REAL64TOSTRING_convert(&len_328,30,(BYTE*)buffer_327,r_323,Ip_324,Dp_325);
      P_so_fwrite_string_hostio(fs_320,ts_321,streamid_322,len_328,(BYTE*)((BYTE*)(((BYTE*)buffer_327)+((RNGCHKUB(0,len_328,30),0)*sizeof(BYTE)))),result_326);
    }
  }
}

void P_so_fwrite_int64_hostio (CHAN *fs_330,CHAN *ts_331,INT32 streamid_332,INT64 r_333,INT field_334,BYTE *result_335)
{
  tSF_P_so_fwrite_int64_hostio SF;
  tSF_P_so_fwrite_int64_hostio *FP = &SF;
  {
    BYTE buffer_336[30];
    INT len_337;
    {
      P_INT64TOSTRING_convert(&len_337,30,(BYTE*)buffer_336,r_333);
      P_so_fwrite_string_hostio(fs_330,ts_331,streamid_332,len_337,(BYTE*)((BYTE*)(((BYTE*)buffer_336)+((RNGCHKUB(0,len_337,30),0)*sizeof(BYTE)))),result_335);
    }
  }
}

void P_so_fwrite_hex_int64_hostio (CHAN *fs_339,CHAN *ts_340,INT32 streamid_341,INT64 r_342,INT field_343,BYTE *result_344)
{
  tSF_P_so_fwrite_hex_int64_hostio SF;
  tSF_P_so_fwrite_hex_int64_hostio *FP = &SF;
  {
    BYTE buffer_345[30];
    INT len_346;
    {
      P_HEX64TOSTRING_convert(&len_346,30,(BYTE*)buffer_345,r_342);
      P_so_fwrite_string_hostio(fs_339,ts_340,streamid_341,len_346,(BYTE*)((BYTE*)(((BYTE*)buffer_345)+((RNGCHKUB(0,len_346,30),0)*sizeof(BYTE)))),result_344);
    }
  }
}

void P_MALLOC_hostio (INT32 *pointer_348,INT size_349)
{
  tSF_P_MALLOC_hostio SF;
  tSF_P_MALLOC_hostio *FP = &SF;
  *pointer_348 = (INT32) malloc (size_349);
}

void P_FREE_hostio (INT32 *pointer_351)
{
  tSF_P_FREE_hostio SF;
  tSF_P_FREE_hostio *FP = &SF;
  free((void*)*pointer_351);
}

void P_STRING_OCCAM_TO_C_hostio (INT occam_353_Dim0,BYTE *occam_353,INT32 c_354)
{
  tSF_P_STRING_OCCAM_TO_C_hostio SF;
  tSF_P_STRING_OCCAM_TO_C_hostio *FP = &SF;
  {
    INT size_355;
    size_355 = (INT)(occam_353_Dim0);
    {
      {
        INT i_356;
        INT i_356_End;
        i_356 = 0;i_356_End = i_356 + size_355;
        for (; i_356 != i_356_End; i_356++)
        {
          ((char *) c_354)[i_356] = occam_353[i_356];
        }
      }
      ((char *) c_354)[size_355] = '\0';
    }
  }
}

void P_so_open_hostio (CHAN *fs_358,CHAN *ts_359,INT name_360_Dim0,BYTE *name_360,BYTE type_361,BYTE mode_362,INT32 *streamid_363,BYTE *result_364)
{
  tSF_P_so_open_hostio SF;
  tSF_P_so_open_hostio *FP = &SF;
  {
    INT options_cnt_366;
    INT32 string_367;
    BYTE options_str_368[4];
    {
      P_so_mark_channels_hostio(fs_358,ts_359);
      *result_364 = 0; 
      options_cnt_366 = 0; 
      switch(mode_362)
      {
        case 1: 
        {
          *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 114; 
          options_cnt_366 = (options_cnt_366 + 1); 
        }
        break;
        case 2: 
        {
          *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 119; 
          options_cnt_366 = (options_cnt_366 + 1); 
        }
        break;
        case 3: 
        {
          *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 97; 
          options_cnt_366 = (options_cnt_366 + 1); 
        }
        break;
        case 4: 
        {
          {
            BYTE *_V2_369;
            {
              static BYTE TMP[2] = {114,43};
              _V2_369 = TMP;
            }
            ASSIGN(((BYTE*)(((BYTE*)options_str_368)+((RNGCHKUB(options_cnt_366,2,4),RNGCHKLB(options_cnt_366))*sizeof(BYTE)))),_V2_369,(2*sizeof(BYTE))); 
          }
          options_cnt_366 = (options_cnt_366 + 2); 
        }
        break;
        case 5: 
        {
          {
            BYTE *_V1_370;
            {
              static BYTE TMP[2] = {119,43};
              _V1_370 = TMP;
            }
            ASSIGN(((BYTE*)(((BYTE*)options_str_368)+((RNGCHKUB(options_cnt_366,2,4),RNGCHKLB(options_cnt_366))*sizeof(BYTE)))),_V1_370,(2*sizeof(BYTE))); 
          }
          options_cnt_366 = (options_cnt_366 + 2); 
        }
        break;
        case 6: 
        {
          {
            BYTE *_V0_371;
            {
              static BYTE TMP[2] = {97,43};
              _V0_371 = TMP;
            }
            ASSIGN(((BYTE*)(((BYTE*)options_str_368)+((RNGCHKUB(options_cnt_366,2,4),RNGCHKLB(options_cnt_366))*sizeof(BYTE)))),_V0_371,(2*sizeof(BYTE))); 
          }
          options_cnt_366 = (options_cnt_366 + 2); 
        }
        break;
        default: 
        *result_364 = 4; 
        break;
      }
      if ((*result_364 == 0))
      {
        {
          switch(type_361)
          {
            case 1: 
            if (false)
            {
              {
                *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 98; 
                options_cnt_366 = (options_cnt_366 + 1); 
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
            break;
            case 2: 
            if (false)
            {
              {
                *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 116; 
                options_cnt_366 = (options_cnt_366 + 1); 
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
            break;
            default: 
            *result_364 = 3; 
            break;
          }
          if ((*result_364 == 0))
          {
            {
              *((BYTE*)(((BYTE*)options_str_368)+(RNGCHK(options_cnt_366,4)*sizeof(BYTE)))) = 0; 
              if (((name_360_Dim0) == 0))
              {
                *result_364 = 2; 
              }
              else
              if (true)
              {
                {
                  P_MALLOC_hostio(&string_367,((name_360_Dim0) + 1));
                  if ((string_367 == 0))
                  {
                    *result_364 = 128; 
                  }
                  else
                  if (true)
                  {
                    {
                      P_STRING_OCCAM_TO_C_hostio(name_360_Dim0,(BYTE*)name_360,string_367);
                      *streamid_363 = (INT32) fopen((char*)string_367,(char*)options_str_368);
                      if ((*streamid_363 == 0))
                      {
                        *result_364 = 128; 
                      }
                      else
                      if (true)
                      {
                      }
                      else
                      {
                        SETERR(MSG_IF);
                      }
                      P_FREE_hostio(&string_367);
                    }
                  }
                  else
                  {
                    SETERR(MSG_IF);
                  }
                }
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
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_close_hostio (CHAN *fs_373,CHAN *ts_374,INT32 *streamid_375,BYTE *result_376)
{
  tSF_P_so_close_hostio SF;
  tSF_P_so_close_hostio *FP = &SF;
  {
    INT r_377;
    {
      P_so_mark_channels_hostio(fs_373,ts_374);
      r_377 = fclose ((FILE*)*streamid_375);
      if ((r_377 == 0))
      {
        *result_376 = 0; 
      }
      else
      if (true)
      {
        *result_376 = 128; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_test_exists_hostio (CHAN *fs_379,CHAN *ts_380,INT name_381_Dim0,BYTE *name_381,BOOL *exists_382)
{
  tSF_P_so_test_exists_hostio SF;
  tSF_P_so_test_exists_hostio *FP = &SF;
  {
    BYTE result_383;
    INT32 streamid_384;
    {
      P_so_open_hostio(fs_379,ts_380,name_381_Dim0,(BYTE*)name_381,1,1,&streamid_384,&result_383);
      if ((result_383 == 0))
      {
        {
          P_so_close_hostio(fs_379,ts_380,&streamid_384,&result_383);
          *exists_382 = true; 
        }
      }
      else
      if (true)
      {
        *exists_382 = false; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_open_temp_hostio (CHAN *fs_386,CHAN *ts_387,BYTE type_388,BYTE *filename_389,INT32 *streamid_390,BYTE *result_391)
{
  tSF_P_so_open_temp_hostio SF;
  tSF_P_so_open_temp_hostio *FP = &SF;
  {
    INT num_392;
    BOOL searching_393;
    {
      {
        BYTE *_V0_394;
        {
          static BYTE TMP[6] = {116,101,109,112,48,48};
          _V0_394 = TMP;
        }
        ASSIGN(filename_389,_V0_394,(6*sizeof(BYTE))); 
      }
      num_392 = 0; 
      searching_393 = true; 
      *result_391 = 0; 
      while (searching_393)
      {
        {
          INT n_395;
          INT i_396;
          {
            n_395 = num_392; 
            i_396 = 5; 
            while ((n_395 > 0))
            {
              {
                *((BYTE*)(((BYTE*)filename_389)+(RNGCHK(i_396,6)*sizeof(BYTE)))) = BYTEINT(((n_395 % 10) + 48)); 
                n_395 = (n_395 / 10); 
                i_396 = (i_396 - 1); 
              }
            }
            P_so_test_exists_hostio(fs_386,ts_387,6,(BYTE*)filename_389,&searching_393);
            if (searching_393)
            {
              {
                num_392 = (num_392 + 1); 
                if ((num_392 == 10000))
                {
                  {
                    searching_393 = false; 
                    *result_391 = 127; 
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
          }
        }
      }
      if ((*result_391 == 0))
      {
        P_so_open_hostio(fs_386,ts_387,6,(BYTE*)filename_389,type_388,5,streamid_390,result_391);
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

void P_so_popen_read_hostio (CHAN *fs_398,CHAN *ts_399,INT filename_400_Dim0,BYTE *filename_400,INT path_variable_name_401_Dim0,BYTE *path_variable_name_401,BYTE open_type_402,INT *full_len_403,INT full_name_404_Dim0,BYTE *full_name_404,INT32 *streamid_405,BYTE *result_406)
{
  tSF_P_so_popen_read_hostio SF;
  tSF_P_so_popen_read_hostio *FP = &SF;
  {
    {
      BYTE *_V0_407;
      {
        static BYTE TMP[29] = {115,111,46,112,111,112,101,110,46,114,101,97,100,32,110,111,116,32,105,109,
           112,108,101,109,101,110,116,101,100};
        _V0_407 = TMP;
      }
      P_so_write_string_nl_hostio(fs_398,ts_399,29,(BYTE*)_V0_407);
    }
    *result_406 = 127; 
  }
}

void P_so_write_hostio (CHAN *fs_409,CHAN *ts_410,INT32 streamid_411,INT data_412_Dim0,BYTE *data_412,INT *length_413)
{
  tSF_P_so_write_hostio SF;
  tSF_P_so_write_hostio *FP = &SF;
  {
    INT size_414;
    size_414 = (INT)(data_412_Dim0);
    {
      P_so_mark_channels_hostio(fs_409,ts_410);
      *length_413 = fwrite(data_412,1,size_414,(FILE*)streamid_411);
    }
  }
}

void P_so_read_hostio (CHAN *fs_416,CHAN *ts_417,INT32 streamid_418,INT *length_419,INT data_420_Dim0,BYTE *data_420)
{
  tSF_P_so_read_hostio SF;
  tSF_P_so_read_hostio *FP = &SF;
  {
    INT size_421;
    size_421 = (INT)(data_420_Dim0);
    {
      P_so_mark_channels_hostio(fs_416,ts_417);
      *length_419 = fread(&*data_420,1,size_421,(FILE*)streamid_418);
    }
  }
}

void P_so_gets_hostio (CHAN *fs_423,CHAN *ts_424,INT32 streamid_425,INT *length_426,INT data_427_Dim0,BYTE *data_427,BYTE *result_428)
{
  tSF_P_so_gets_hostio SF;
  tSF_P_so_gets_hostio *FP = &SF;
  {
    BOOL reading_429;
    {
      P_so_mark_channels_hostio(fs_423,ts_424);
      *result_428 = 0; 
      reading_429 = true; 
      *length_426 = 0; 
      while (reading_429)
      {
        {
          INT r_430;
          {
            r_430 = fgetc((FILE*)streamid_425);
            if ((r_430 < 0))
            {
              {
                *result_428 = 128; 
                reading_429 = false; 
              }
            }
            else
            if ((BYTEINT(r_430) == 10))
            {
              reading_429 = false; 
            }
            else
            if (true)
            {
              *((BYTE*)(((BYTE*)data_427)+(RNGCHK(*length_426,data_427_Dim0)*sizeof(BYTE)))) = BYTEINT(r_430); 
            }
            else
            {
              SETERR(MSG_IF);
            }
            if (((*length_426 + 1) > (data_427_Dim0)))
            {
              {
                *result_428 = 7; 
                reading_429 = false; 
              }
            }
            else
            if (true)
            {
              *length_426 = (*length_426 + 1); 
            }
            else
            {
              SETERR(MSG_IF);
            }
          }
        }
      }
    }
  }
}

void P_so_puts_hostio (CHAN *fs_432,CHAN *ts_433,INT32 streamid_434,INT data_435_Dim0,BYTE *data_435,BYTE *result_436)
{
  tSF_P_so_puts_hostio SF;
  tSF_P_so_puts_hostio *FP = &SF;
  {
    BOOL writing_437;
    INT length_438;
    {
      P_so_mark_channels_hostio(fs_432,ts_433);
      *result_436 = 0; 
      writing_437 = true; 
      length_438 = 0; 
      while ((writing_437 && (length_438 < (data_435_Dim0))))
      {
        {
          {
            BYTE b_439;
            INT r_440;
            b_439 = (BYTE)*((BYTE*)(((BYTE*)data_435)+(RNGCHK(length_438,data_435_Dim0)*sizeof(BYTE))));
            {
              r_440 = fputc(b_439,(FILE*)streamid_434);
              if ((r_440 < 0))
              {
                {
                  *result_436 = 128; 
                  writing_437 = false; 
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
            }
          }
          length_438 = (length_438 + 1); 
        }
      }
    }
  }
}

void P_so_flush_hostio (CHAN *fs_442,CHAN *ts_443,INT32 streamid_444,BYTE *result_445)
{
  tSF_P_so_flush_hostio SF;
  tSF_P_so_flush_hostio *FP = &SF;
  {
    INT r_446;
    {
      P_so_mark_channels_hostio(fs_442,ts_443);
      r_446 = fflush ((FILE*)streamid_444);
      if ((r_446 < 0))
      {
        *result_445 = 128; 
      }
      else
      if (true)
      {
        *result_445 = 0; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_seek_hostio (CHAN *fs_448,CHAN *ts_449,INT32 streamid_450,INT32 offset_451,INT32 origin_452,BYTE *result_453)
{
  tSF_P_so_seek_hostio SF;
  tSF_P_so_seek_hostio *FP = &SF;
  {
    INT org_454;
    {
      P_so_mark_channels_hostio(fs_448,ts_449);
      *result_453 = 0; 
      switch(origin_452)
      {
        case 1: 
        org_454 = SEEK_SET;
        break;
        case 2: 
        org_454 = SEEK_CUR;
        break;
        case 3: 
        org_454 = SEEK_END;
        break;
        default: 
        *result_453 = 9; 
        break;
      }
      if ((*result_453 == 0))
      {
        {
          INT r_455;
          {
            r_455 = fseek ((FILE*)streamid_450, offset_451, org_454);
            if ((r_455 < 0))
            {
              *result_453 = 128; 
            }
            else
            if (true)
            {
              *result_453 = 0; 
            }
            else
            {
              SETERR(MSG_IF);
            }
          }
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
    }
  }
}

void P_so_tell_hostio (CHAN *fs_457,CHAN *ts_458,INT32 streamid_459,INT32 *position_460,BYTE *result_461)
{
  tSF_P_so_tell_hostio SF;
  tSF_P_so_tell_hostio *FP = &SF;
  {
    INT32 r_462;
    {
      P_so_mark_channels_hostio(fs_457,ts_458);
      r_462 = (INT32) ftell ((FILE*)streamid_459);
      if ((r_462 < 0))
      {
        *result_461 = 128; 
      }
      else
      if (true)
      {
        {
          *result_461 = 0; 
          *position_460 = r_462; 
        }
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_eof_hostio (CHAN *fs_464,CHAN *ts_465,INT32 streamid_466,BYTE *result_467)
{
  tSF_P_so_eof_hostio SF;
  tSF_P_so_eof_hostio *FP = &SF;
  {
    INT r_468;
    {
      P_so_mark_channels_hostio(fs_464,ts_465);
      r_468 = feof ((FILE *)streamid_466);
      if ((r_468 != 0))
      {
        *result_467 = 0; 
      }
      else
      if (true)
      {
        *result_467 = 128; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_ferror_hostio (CHAN *fs_470,CHAN *ts_471,INT32 streamid_472,INT32 *error_no_473,INT *length_474,INT message_475_Dim0,BYTE *message_475,BYTE *result_476)
{
  tSF_P_so_ferror_hostio SF;
  tSF_P_so_ferror_hostio *FP = &SF;
  {
    INT32 r_477;
    {
      P_so_mark_channels_hostio(fs_470,ts_471);
      r_477 = (INT32) ferror ((FILE*)streamid_472);
      if ((r_477 != 0))
      {
        {
          *result_476 = 0; 
          *length_474 = 0; 
          *error_no_473 = r_477; 
        }
      }
      else
      if (true)
      {
        *result_476 = 128; 
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_remove_hostio (CHAN *fs_479,CHAN *ts_480,INT name_481_Dim0,BYTE *name_481,BYTE *result_482)
{
  tSF_P_so_remove_hostio SF;
  tSF_P_so_remove_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_479,ts_480);
    if (((name_481_Dim0) == 0))
    {
      *result_482 = 2; 
    }
    else
    if (true)
    {
      {
        INT32 string_483;
        {
          P_MALLOC_hostio(&string_483,((name_481_Dim0) + 1));
          if ((string_483 == 0))
          {
            *result_482 = 128; 
          }
          else
          if (true)
          {
            {
              INT r_484;
              {
                P_STRING_OCCAM_TO_C_hostio(name_481_Dim0,(BYTE*)name_481,string_483);
                r_484 = remove((char*)string_483);
                if ((r_484 == 0))
                {
                  *result_482 = 0; 
                }
                else
                if (true)
                {
                  *result_482 = 128; 
                }
                else
                {
                  SETERR(MSG_IF);
                }
                P_FREE_hostio(&string_483);
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
    else
    {
      SETERR(MSG_IF);
    }
  }
}

void P_so_rename_hostio (CHAN *fs_486,CHAN *ts_487,INT oldname_488_Dim0,BYTE *oldname_488,INT newname_489_Dim0,BYTE *newname_489,BYTE *result_490)
{
  tSF_P_so_rename_hostio SF;
  tSF_P_so_rename_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_486,ts_487);
    if ((((oldname_488_Dim0) == 0) || ((newname_489_Dim0) == 0)))
    {
      *result_490 = 2; 
    }
    else
    if (true)
    {
      {
        INT32 old_491;
        INT32 new_492;
        {
          P_MALLOC_hostio(&old_491,((oldname_488_Dim0) + 1));
          P_MALLOC_hostio(&new_492,((newname_489_Dim0) + 1));
          if (((old_491 == 0) || (new_492 == 0)))
          {
            *result_490 = 128; 
          }
          else
          if (true)
          {
            {
              INT r_493;
              {
                P_STRING_OCCAM_TO_C_hostio(oldname_488_Dim0,(BYTE*)oldname_488,old_491);
                P_STRING_OCCAM_TO_C_hostio(newname_489_Dim0,(BYTE*)newname_489,new_492);
                r_493 = rename((char*)old_491,(char*)new_492);
                if ((r_493 == 0))
                {
                  *result_490 = 0; 
                }
                else
                if (true)
                {
                  *result_490 = 128; 
                }
                else
                {
                  SETERR(MSG_IF);
                }
                P_FREE_hostio(&old_491);
                P_FREE_hostio(&new_492);
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
    else
    {
      SETERR(MSG_IF);
    }
  }
}

void P_so_commandline_hostio (CHAN *fs_495,CHAN *ts_496,BYTE all_497,INT *length_498,INT string_499_Dim0,BYTE *string_499,BYTE *result_500)
{
  tSF_P_so_commandline_hostio SF;
  tSF_P_so_commandline_hostio *FP = &SF;
  {
    INT start_501;
    INT i_502;
    INT strlen_503;
    strlen_503 = (INT)(string_499_Dim0);
    {
      P_so_mark_channels_hostio(fs_495,ts_496);
      *result_500 = 0; 
      switch(all_497)
      {
        case 0: 
        start_501 = 1; 
        break;
        case 1: 
        start_501 = 0; 
        break;
        default: 
        *result_500 = 128; 
        break;
      }
      if ((*result_500 == 0))
      {
        {
          *length_498 = 0; 
          {
            extern int MAIN_ARGC;
            extern char **MAIN_ARGV;
            char *str = (char*) & *string_499;
            for (i_502=start_501;i_502<MAIN_ARGC;i_502++)
            {
              char *argv = MAIN_ARGV[i_502];
              while(*length_498 < strlen_503 && *argv != '\0')
                str[(*length_498)++] = *(argv++);
              if (*length_498 < strlen_503) str[(*length_498)++] = ' ';
              else break ;
            }
          }
          if ((*length_498 >= strlen_503))
          {
            *result_500 = 7; 
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
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_parse_command_line_hostio (CHAN *fs_505,CHAN *ts_506,INT option_strings_507_Dim0,INT option_strings_507_Dim1,BYTE *option_strings_507,INT option_parameters_required_508_Dim0,INT *option_parameters_required_508,INT option_exists_509_Dim0,BOOL *option_exists_509,INT option_parameters_510_Dim0,INT *option_parameters_510,INT *error_len_511,INT line_512_Dim0,BYTE *line_512)
{
  tSF_P_so_parse_command_line_hostio SF;
  tSF_P_so_parse_command_line_hostio *FP = &SF;
  {
    {
      BYTE *_V0_513;
      {
        static BYTE TMP[37] = {115,111,46,112,97,114,115,101,46,99,111,109,109,97,110,100,46,108,105,110,
           101,32,110,111,116,32,105,109,112,108,101,109,101,110,116,101,100};
        _V0_513 = TMP;
      }
      P_so_write_string_nl_hostio(fs_505,ts_506,37,(BYTE*)_V0_513);
    }
  }
}

void P_so_getenv_hostio (CHAN *fs_515,CHAN *ts_516,INT name_517_Dim0,BYTE *name_517,INT *length_518,INT value_519_Dim0,BYTE *value_519,BYTE *result_520)
{
  tSF_P_so_getenv_hostio SF;
  tSF_P_so_getenv_hostio *FP = &SF;
  {
    INT strlen_521;
    INT32 string_522;
    INT res_523;
    strlen_521 = (INT)(value_519_Dim0);
    {
      P_so_mark_channels_hostio(fs_515,ts_516);
      P_MALLOC_hostio(&string_522,((name_517_Dim0) + 1));
      if ((string_522 == 0))
      {
        *result_520 = 128; 
      }
      else
      if (true)
      {
        {
          P_STRING_OCCAM_TO_C_hostio(name_517_Dim0,(BYTE*)name_517,string_522);
          *length_518 = 0; 
          res_523 = 0; 
          {
            char *val = (char *) & *value_519;
            char *env = getenv ((char *) string_522) ;
            if (env == NULL)
              res_523 = 1;
            else
            {
              while (*length_518 < strlen_521 && *env != '\0')
                val[(*length_518)++] = *(env++);
              if (*env != '\0') res_523 = 2;
            }
          }
          {
            BYTE *_V0_524;
            {
              static BYTE TMP[3] = {0,128,7};
              _V0_524 = (BYTE*)TMP;
            }
            *result_520 = *((BYTE*)(((BYTE*)_V0_524)+(res_523*sizeof(BYTE)))); 
          }
          P_FREE_hostio(&string_522);
        }
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_time_hostio (CHAN *fs_526,CHAN *ts_527,INT32 *localtime_528,INT32 *UTCtime_529)
{
  tSF_P_so_time_hostio SF;
  tSF_P_so_time_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_526,ts_527);
    {
      time_t now = time(NULL); 
      *localtime_528 = (INT32) now;
      *UTCtime_529 = (INT32) 0;
    }
  }
}

void P_so_system_hostio (CHAN *fs_531,CHAN *ts_532,INT command_533_Dim0,BYTE *command_533,INT32 *status_534,BYTE *result_535)
{
  tSF_P_so_system_hostio SF;
  tSF_P_so_system_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_531,ts_532);
    *result_535 = 0; 
    if (((command_533_Dim0) != 0))
    {
      {
        *status_534 = system ((char*)command_533);
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
  }
}

void P_so_exit_hostio (CHAN *fs_537,CHAN *ts_538,INT32 status_539)
{
  tSF_P_so_exit_hostio SF;
  tSF_P_so_exit_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_537,ts_538);
    switch(status_539)
    {
      case 0x3B9AC9FF: 
      exit(0);
      break;
      case -999999999: 
      exit(-1);
      break;
      default: 
      exit(status_539);
      break;
    }
  }
}

void P_so_core_hostio (CHAN *fs_541,CHAN *ts_542,INT32 offset_543,INT *bytes_read_544,INT data_545_Dim0,BYTE *data_545,BYTE *result_546)
{
  tSF_P_so_core_hostio SF;
  tSF_P_so_core_hostio *FP = &SF;
  {
    P_so_mark_channels_hostio(fs_541,ts_542);
    {
      BYTE *_V0_547;
      {
        static BYTE TMP[23] = {115,111,46,99,111,114,101,32,110,111,116,32,105,109,112,108,101,109,101,110,
           116,101,100};
        _V0_547 = TMP;
      }
      P_so_write_string_nl_hostio(fs_541,ts_542,23,(BYTE*)_V0_547);
    }
    *result_546 = 127; 
  }
}

void P_so_version_hostio (CHAN *fs_549,CHAN *ts_550,BYTE *version_551,BYTE *host_552,BYTE *os_553,BYTE *board_554)
{
  tSF_P_so_version_hostio SF;
  tSF_P_so_version_hostio *FP = &SF;
  {
    *version_551 = 0; 
    #if (HOST == PC)
    *host_552 = 1; 
    #elif (HOST == VAX)
    *host_552 = 3; 
    #elif (HOST == SUN3)
    *host_552 = 4; 
    #elif (HOST == SUN4)
    *host_552 = 6; 
    #else
    *host_552 = 0; 
    #endif
    #if (OS == DOS)
    *os_553 = 1; 
    #elif (OS == VMS)
    *os_553 = 3; 
    #elif (OS == SUNOS)
    *os_553 = 4; 
    #else
    *os_553 = 0; 
    #endif
    *board_554 = 0; 
  }
}

void P_so_getkey_hostio (CHAN *fs_556,CHAN *ts_557,BYTE *key_558,BYTE *result_559)
{
  tSF_P_so_getkey_hostio SF;
  tSF_P_so_getkey_hostio *FP = &SF;
  {
    INT c_560;
    {
      P_so_mark_channels_hostio(fs_556,ts_557);
       c_560 = getchar();
      if ((c_560 == -1))
      {
        *result_559 = 128; 
      }
      else
      if (true)
      {
        {
          *key_558 = BYTEINT(c_560); 
          *result_559 = 0; 
        }
      }
      else
      {
        SETERR(MSG_IF);
      }
    }
  }
}

void P_so_read_echo_line_hostio (CHAN *fs_562,CHAN *ts_563,INT *len_564,INT line_565_Dim0,BYTE *line_565,BYTE *result_566)
{
  tSF_P_so_read_echo_line_hostio SF;
  tSF_P_so_read_echo_line_hostio *FP = &SF;
  {
    INT llen_567;
    llen_567 = (INT)(line_565_Dim0);
    {
      P_so_mark_channels_hostio(fs_562,ts_563);
      while((fgets((char*)(&*line_565),llen_567,stdin))==NULL);
      *len_564 = strlen((char*)(&*line_565));
      if (*len_564 > 0 && (&*line_565)[*len_564-1]=='\n')
        (&*line_565)[--*len_564] = 0;
      *result_566 = 0; 
    }
  }
}

void P_so_read_echo_int_hostio (CHAN *fs_569,CHAN *ts_570,INT *n_571,BOOL *error_572)
{
  tSF_P_so_read_echo_int_hostio SF;
  tSF_P_so_read_echo_int_hostio *FP = &SF;
  {
    INT r_573;
    BYTE b_574[256];
    INT l_575;
    BYTE res_576;
    {
      P_so_mark_channels_hostio(fs_569,ts_570);
      P_so_read_echo_line_hostio(fs_569,ts_570,&l_575,256,(BYTE*)b_574,&res_576);
      r_573 = sscanf((char*)b_574,"%d",& *n_571);
      *error_572 = ( ! ((r_573 == 1) && (res_576 == 0))); 
    }
  }
}

void P_sp_getkey_hostio (CHAN *fs_578,CHAN *ts_579,BYTE *key_580,BYTE *result_581)
{
  tSF_P_sp_getkey_hostio SF;
  tSF_P_sp_getkey_hostio *FP = &SF;
  {
    P_so_getkey_hostio(fs_578,ts_579,key_580,result_581);
  }
}


/*}}}*/
#endif
