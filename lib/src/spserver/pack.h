
/*
 --   ---------------------------------------------------------------------------
 --
 --      ISERVER  -  INMOS standard file server
 --
 --      pack.h
 --
 --      Macros to cut and paste the Tbuf
 --
 --      Copyright (c) INMOS Ltd., 1988.
 --      All Rights Reserved.
 --
 --   ---------------------------------------------------------------------------
*/



#ifndef _INMOS_H
#include "inmos.h"
#endif



/*
 *  some stuff used in processing all commands
 */

#define BUFFER_DECLARATIONS register BYTE *InBuf, *OutBuf; int OutCount
#define INIT_BUFFERS InBuf = &Tbuf[3]; OutBuf = &Tbuf[2]; OutCount = 0
#define REPLY PUT_COUNT( OutCount ); return



/*
 *   PACKING ROUTINES
 */

/*
 *   pack one byte with int x 
 */

#define PUT_BYTE(x) {*OutBuf++ = x; OutCount++;}

/*
 *  pack lower 16 bits of int x into 2 bytes
 */

#if (defined BIG_AND_LITTLE)
#define PUT_INT16(x) {BYTE *c; c=(BYTE *)&x+3; *OutBuf++ = *c--; *OutBuf++ = *c--; OutCount += 2;}
#elif (defined LITTLE_AND_LITTLE)
#define PUT_INT16(x) {BYTE *c; c=(BYTE *)&x; *OutBuf++ = *c++; *OutBuf++ = *c++; OutCount += 2;}
#elif (defined BIG_AND_BIG)
#define PUT_INT16(x) {BYTE *c; c=(BYTE *)&x+2; *OutBuf++ = *c++; *OutBuf++ = *c++; OutCount += 2;}
#elif (defined LITTLE_AND_BIG)
#define PUT_INT16(x) {BYTE *c; c=(BYTE *)&x+1; *OutBuf++ = *c--; *OutBuf++ = *c--; OutCount += 2;}
#endif

/*
 *  pack int32 into 4 bytes
 */

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_BIG)
#define PUT_INT32(x) {BYTE *c; c=(BYTE *)&x+3; *OutBuf++ = *c--; *OutBuf++ = *c--; *OutBuf++ = *c--; *OutBuf++ = *c; OutCount += 4;}
#elif (defined LITTLE_AND_LITTLE || defined BIG_AND_BIG)
#define PUT_INT32(x) {BYTE *c; c=(BYTE *)&x; *OutBuf++ = *c++; *OutBuf++ = *c++; *OutBuf++ = *c++; *OutBuf++ = *c; OutCount += 4;}
#endif

/*
 *  pack FILE *x as 4 bytes
 */

#ifdef MSC
#define PUT_FD(x) {BYTE *c; c=(BYTE *)&x; *OutBuf++ = *c++; *OutBuf++ = *c;*OutBuf++ = 0; *OutBuf++ = 0; OutCount += 4;}
#else
#define PUT_FD(x) PUT_INT32(x)
#endif

/*
 *  pack lower 16 bits of int x as 2 bytes and then pack x bytes from y
 */

#if (defined BIG_AND_LITTLE)
#define PUT_SLICE(x,y) {int i; BYTE *c; c=(BYTE *)&x+3; *OutBuf++ = *c--; *OutBuf++ = *c--; c=(BYTE *)y; for( i=0; i<x; i++ ) *OutBuf++ = *c++; OutCount += x+2;}
#elif (defined LITTLE_AND_LITTLE)
#define PUT_SLICE(x,y) {int i; BYTE *c; c=(BYTE *)&x; *OutBuf++ = *c++; *OutBuf++ = *c; c=(BYTE *)y; for( i=0; i<x; i++ ) *OutBuf++ = *c++; OutCount += x+2; }
#elif (defined BIG_AND_BIG)
#define PUT_SLICE(x,y) {int i; BYTE *c; c=(BYTE *)&x+2; *OutBuf++ = *c++; *OutBuf++ = *c; c=(BYTE *)y; for( i=0; i<x; i++ ) *OutBuf++ = *c++; OutCount += x+2; }
#elif (defined LITTLE_AND_BIG)
#define PUT_SLICE(x,y) {int i; BYTE *c; c=(BYTE *)&x+1; *OutBuf++ = *c--; *OutBuf++ = *c--; c=(BYTE *)y; for( i=0; i<x; i++ ) *OutBuf++ = *c++; OutCount += x+2;}
#endif

/*
 *  pack lower 16 bits of int x into bytes 0 and 1 of Tbuf
 */

#if (defined BIG_AND_LITTLE)
#define PUT_COUNT(x) {BYTE *c; c=(BYTE *)&x+3; Tbuf[0]= *c--; Tbuf[1]= *c; }
#elif (defined LITTLE_AND_LITTLE)
#define PUT_COUNT(x) {BYTE *c; c=(BYTE *)&x; Tbuf[0]= *c++; Tbuf[1]= *c; }
#elif (defined BIG_AND_BIG)
#define PUT_COUNT(x) {BYTE *c; c=(BYTE *)&x+2; Tbuf[0] = *c++; Tbuf[1] = *c; }
#elif (defined LITTLE_AND_BIG)
#define PUT_COUNT(x) {BYTE *c; c=(BYTE *)&x+1; Tbuf[0] = *c--; Tbuf[1] = *c; }
#endif




/*
 *   UNPACKING ROUTINES
 */



/*
 *  unpack one byte into int x
 */

#define GET_BYTE(x) x = *InBuf++
/*
 *  unpack 2 bytes into int x
 */

#if (defined BIG_AND_LITTLE)
#define GET_INT16(x) {BYTE *c; x=0; c=(BYTE *)&x+3; *c-- = *InBuf++; *c = *InBuf++; }
#elif (defined LITTLE_AND_LITTLE)
#define GET_INT16(x) {BYTE *c; x=0; c=(BYTE *)&x; *c++ = *InBuf++; *c = *InBuf++; }
#elif (defined BIG_AND_BIG)
#define GET_INT16(x) {BYTE *c; x=0; c=(BYTE *)&x+2; *c++ = *InBuf++; *c = *InBuf++; }
#elif (defined LITTLE_AND_BIG)
#define GET_INT16(x) {BYTE *c; x=0; c=(BYTE *)&x+1; *c-- = *InBuf++; *c = *InBuf++; }
#define 
#endif

/*
 *  unpack 4 bytes into int32 x
 */

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_BIG)
#define GET_INT32(x) {BYTE *c; c=((BYTE *)&x)+3; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c = *InBuf++; }
#elif (defined LITTLE_AND_LITTLE || defined BIG_AND_BIG)
#define GET_INT32(x) {BYTE *c; c=(BYTE *)&x; *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c = *InBuf++; }
#endif

/*
 *  unpack 4 bytes into FILE *x
 */

#ifdef MSC
#define GET_FD(x) {BYTE *c; c=(BYTE *)&x; *c++ = *InBuf++; *c = *InBuf++; InBuf += 2; }
#else
#define GET_FD(x) GET_INT32(x)
#endif

/*
 *  unpack 2 bytes into int x and then x bytes into array y
 */

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_LITTLE)
#define GET_SLICE(x,y) {int i; BYTE *j; j=y; x = *InBuf++; x += (256*(*InBuf++)); for( i=0; i<x; i++ ) *j++ = *InBuf++; }
#elif (defined BIG_AND_BIG || defined BIG_AND_BIG)
#define GET_SLICE(x,y) {int i; BYTE *j; j=y; x = (256*(*InBuf++)); x += *InBuf++; for( i=0; i<x; i++ ) *j++ = *InBuf++; }
#endif

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_BIG)
#define GET_double(x) {BYTE *c; c=(&x)+7; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c = *InBuf++; }
#elif (defined LITTLE_AND_LITTLE || defined BIG_AND_BIG)
#define GET_double(x) {BYTE *c; c=(BYTE *)x;      *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c = *InBuf++;}
#endif

#if (defined BIG_AND_LITTLE || defined LITTLE_AND_BIG)
#define GET_REAL32(x) {BYTE *c; c=((BYTE *)x)+3; *c-- = *InBuf++; *c-- = *InBuf++; *c-- = *InBuf++; *c = *InBuf++; }
#elif (defined LITTLE_AND_LITTLE || defined BIG_AND_BIG)
#define GET_REAL32(x) {BYTE *c; c=(BYTE *)x;     *c++ = *InBuf++; *c++ = *InBuf++; *c++ = *InBuf++; *c = *InBuf++; }
#endif

