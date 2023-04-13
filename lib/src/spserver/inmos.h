
/*
 --   ---------------------------------------------------------------------------
 --
 --      ISERVER  -  INMOS standard file server
 --
 --      inmos.h
 --
 --      Some global definitions
 --
 --      Copyright (c) INMOS Ltd., 1988.
 --      All Rights Reserved.
 --
 --   ---------------------------------------------------------------------------
*/



#ifndef _INMOS_H
#define _INMOS_H
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif
#define BOOL int

#ifdef MSC
#define INT32 long
#else
#define INT32 int
#endif

/*
#ifdef sun3
#define BIG_AND_LITTLE
#else
#ifdef sun4
#define BIG_AND_LITTLE
#else
#define LITTLE_AND_LITTLE
#endif
#endif
*/

#if defined(linux)||defined(darwin)
  /* Little-endian iserver and little-endian protocol */
  #define LITTLE_AND_LITTLE
#else
  /* Big-endian iserver and little-endian protocol */
  #define BIG_AND_LITTLE
#endif

#ifndef TRUE 
#define TRUE 1
#endif
#ifndef FALSE 
#define FALSE 0
#endif

#ifndef PUBLIC
#define PUBLIC
#endif
#ifndef PRIVATE
#define PRIVATE static
#endif
#define VOID void
#define EXTERN extern

#define LINK int

static char Copyright[] = "Copyright (c) INMOS Ltd, 1988.  All Rights Reserved.\n";


/*
 *   Eof
 */

