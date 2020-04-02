/*
 --   ---------------------------------------------------------------------------
 --
 --      ISERVER  -  INMOS standard file server
 --
 --      iserver.c
 --
 --      The main body
 --
 --      Copyright (c) INMOS Ltd., 1988.
 --      All Rights Reserved.
 --
 --   ---------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include <errno.h>

#ifdef MSC
#include <stdlib.h>
#include <malloc.h>
#endif

#ifdef VMS
#include <unixio.h>
#include <file.h>
#endif

#ifdef HELIOS
#include <stdlib.h>
#include <posix.h>
#endif


#include "inmos.h"
#include "iserver.h"




EXTERN VOID SpOpen();
EXTERN VOID SpClose();
EXTERN VOID SpRead();
EXTERN VOID SpWrite();
EXTERN VOID SpGetBlock();
EXTERN VOID SpPutBlock();
EXTERN VOID SpGets();
EXTERN VOID SpPuts();
EXTERN VOID SpFlush();
EXTERN VOID SpSeek();
EXTERN VOID SpTell();
EXTERN VOID SpEof();
EXTERN VOID SpError();
EXTERN VOID SpRemove();
EXTERN VOID SpRename();

EXTERN VOID SpGetkey();
EXTERN VOID SpPollkey();
EXTERN VOID SpGetenv();
EXTERN VOID SpTime();
EXTERN VOID SpSystem();
EXTERN VOID SpCommand();

EXTERN VOID SpCore();
EXTERN VOID SpId();
EXTERN int  SpExit();
EXTERN VOID SpUnknown();

#ifdef MSC
EXTERN VOID SpMsdos();
#endif

EXTERN LINK OpenLink();
EXTERN int  CloseLink();
EXTERN int  ReadLink();
EXTERN int  WriteLink();
EXTERN int  ResetLink();
EXTERN int  AnalyseLink();
EXTERN int  TestError();
EXTERN int  TestRead();
EXTERN int  TestWrite();

EXTERN VOID HostBegin();
EXTERN VOID HostEnd();
EXTERN VOID ResetTerminal();

PRIVATE VOID SendReply();

PUBLIC BYTE *Tbuf;   /*  buffer for all server operations  */

PUBLIC BYTE RealCommandLine     [ MAX_COMMAND_LINE_LENGTH+1 ];
PUBLIC BYTE DoctoredCommandLine [ MAX_COMMAND_LINE_LENGTH+1 ];

PUBLIC BOOL AnalyseSwitch;                                                 /*  command line switches  */
PUBLIC BOOL TestErrorSwitch;
PUBLIC BOOL VerboseSwitch;
PUBLIC BOOL LinkSwitch;
PUBLIC BOOL ResetSwitch;
PUBLIC BOOL ServeSwitch;
PUBLIC BOOL LoadSwitch;
PUBLIC BOOL CocoPops = FALSE;

PRIVATE BYTE BootFileName[MAX_BOOT_FILE_LENGTH+1];

PRIVATE BYTE LinkName[MAX_LINK_NAME_LENGTH+1];  

PUBLIC LINK TheLink = -1 ;                                                         /*  the server's idea of the active link  */

PUBLIC BYTE *CoreDump;
PUBLIC INT32 CoreSize;

PRIVATE char *LinkMessages[] = {
   "Strange result from link module",
   "Transputer no longer available",         /* ER_LINK_BAD       */
   "No response from transputer",            /* ER_LINK_CANT      */
   "Comms software failure",                 /* ER_LINK_SOFT      */
   "Empty data packet received",             /* ER_LINK_NODATA    */
   "Link synchronisation lost",              /* ER_LINK_NOSYNC    */
   "Link hardware already in use",           /* ER_LINK_BUSY      */
   "No link interface hardware",             /* ER_NO_LINK        */
   "Bad link specification"                  /* ER_LINK_SYNTAX    */
};

PRIVATE char *HardwareNames[] = {
   "???", 
   "B004/B017",
   "B008", 
   "B010",    
   "B011", 
   "B014", 
   "DRX11", 
   "QT0", 
   "IBM_CAT",
   "B015", 
   "B016",
   "UDP-Link"
};


/*
 *   Serve  -  the main loop (read buffer from link, call a function, write buffer to link)
 */

PUBLIC int ServeBuffer (BYTE *buffer, short *length)
{
   register int Count, Size;
   BOOL TerminateFlag = FALSE;
   int Result;
   
   DEBUG(("-=-"));
   Size = *length;
   Tbuf = buffer;

   #if (defined BIG_AND_LITTLE || defined LITTLE_AND_LITTLE)
   Tbuf[0] = (*length) & 0xff; Tbuf[1] = (*length) >> 8 ;
   #elif (defined BIG_AND_BIG || defined LITTLE_AND_BIG)
   Tbuf[1] = (*length) & 0xff; Tbuf[0] = (*length) >> 8 ;
   #endif
   
   switch( Tbuf[2] ) {
      case SP_OPEN     : SpOpen();     break; 
      case SP_CLOSE    : SpClose();    break; 
      case SP_READ     : SpRead();     break; 
      case SP_WRITE    : SpWrite();    break; 
      case SP_GETBLOCK : SpGetBlock(); break;
      case SP_PUTBLOCK : SpPutBlock(); break;
      case SP_GETS     : SpGets();     break; 
      case SP_PUTS     : SpPuts();     break; 
      case SP_FLUSH    : SpFlush();    break; 
      case SP_SEEK     : SpSeek();     break; 
      case SP_TELL     : SpTell();     break; 
      case SP_EOF      : SpEof();      break; 
      case SP_FERROR   : SpError();    break; 
      case SP_REMOVE   : SpRemove();   break; 
      case SP_RENAME   : SpRename();   break; 
      case SP_GETKEY   : SpGetkey();   break; 
      case SP_POLLKEY  : SpPollkey();  break; 
      case SP_GETENV   : SpGetenv();   break; 
      case SP_TIME     : SpTime();     break; 
      case SP_SYSTEM   : SpSystem();   break; 
      case SP_EXIT     : Result = SpExit(); TerminateFlag = TRUE; break;
      case SP_COMMAND  : SpCommand();  break; 
      case SP_CORE     : SpCore();     break; 
      case SP_ID       : SpId();       break; 
#ifdef MSC
      case SP_MSDOS    : SpMsdos();    break;
#endif
      default          :
	 SpUnknown(); break;
   }

   #if (defined BIG_AND_LITTLE || defined LITTLE_AND_LITTLE)
   *length = Tbuf[0] + (Tbuf[1]<<8);
   #elif (defined BIG_AND_BIG || defined LITTLE_AND_BIG)
   *length = Tbuf[1] + (Tbuf[0]<<8);
   #endif

   return ( !TerminateFlag );
}

PUBLIC void ServeSockets (int to_occam, int from_occam)
{
  short length;
  int len;
  BYTE buffer[TRANSACTION_BUFFER_SIZE];
  int serving ;

  HostBegin();

  do
  {
    /*{{{  read length and packet*/
    errno = 0;
    
    DEBUG(( "iserver reading length\n"));
    
    while ((len = read(from_occam, &length, 2)) <= 0)
      if (errno != EAGAIN && errno != EINTR)
      {
	fprintf(stderr,"Error reading length from server socket (errno %d)\n", errno);
	exit(1);
      }
      else
	errno = 0;
	
    if (len != 2)
    {
      fprintf(stderr,"Error - Only read %d out of %d bytes\n", len, 2);
      exit(1);
    }
    
    if (length > TRANSACTION_BUFFER_SIZE-2)
    {
      fprintf(stderr,"Error reading from server socket - bad length\n");
      exit(1);
    }
    
    DEBUG(( "iserver reading body\n"));
	    
    while ((len = read(from_occam, buffer+2, length)) <= 0)
      if (errno != EAGAIN && errno != EINTR)
      {
	fprintf(stderr,"Error reading packet from server socket (errno %d)\n", errno);
	exit(1);
      }
      else
	errno = 0;
    
    if (len != length)
    {
      fprintf(stderr,"Error - Only read %d out of %d bytes\n", len, length);
      exit(1);
    }
    /*}}}*/
    
    DEBUG(( "iserver serving packet\n"));
    
    serving = ServeBuffer (buffer, &length);

    /*{{{  write length and packet*/
    errno = 0;
    
    DEBUG(( "iserver writing length\n"));
	    
    while ((len = write(to_occam, &length, 2)) <= 0)
      if (errno != EAGAIN && errno != EINTR)
      {
	fprintf(stderr,"Error writing length to server socket (errno %d)\n",errno);
	exit(1);
      }
      else
	errno = 0;
    
    if (len != 2)
    {
      fprintf(stderr,"Error - Only wrote %d out of %d bytes\n", len, 2);
      exit(1);
    }
    
    DEBUG(( "iserver writing body\n"));
    
    while ((len = write(to_occam, buffer+2, length)) <= 0)
      if (errno != EAGAIN && errno != EINTR)
      {
	fprintf(stderr,"Error writing packet to server socket (errno %d)\n",errno);
	exit(1);
      }
      else
	errno = 0;
    
    if (len != length)
    {
      fprintf(stderr,"Error - Only wrote %d out of %d bytes\n", len, length);
      exit(1);
    }
    /*}}}*/

    DEBUG(( "iserver finished packet\n"));
  }
  while (serving && getppid()!=1);

  HostEnd();

  DEBUG(( "iserver process is terminating\n"));
  
}
