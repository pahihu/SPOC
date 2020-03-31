/* $Id: DynArray.c,v 1.6 1992/08/17 11:42:24 grosch rel $ */

/* $Log: DynArray.c,v $
 * Revision 1.6  1992/08/17  11:42:24  grosch
 * added error message if out of memory
 *
 * Revision 1.5  1992/05/05  13:19:05  grosch
 * added rcsid
 *
 * Revision 1.4  1991/11/21  14:28:16  grosch
 * new version of RCS on SPARC
 *
 * Revision 1.3  90/09/04  17:32:07  grosch
 * automatic determination of alignment
 * 
 * Revision 1.2  90/07/04  14:33:51  grosch
 * introduced conditional include
 * 
 * Revision 1.1  88/10/18  17:32:05  grosch
 * fixed bug: invariant must hold: ElmtCount * AlignedSize (ElmtSize) % 4 == 0
 * 
 * Revision 1.0  88/10/04  11:44:35  grosch
 * Initial revision
 * 
 */

/* Ich, Doktor Josef Grosch, Informatiker, Sept. 1987 */

static char rcsid [] = "$Id: DynArray.c,v 1.6 1992/08/17 11:42:24 grosch rel $";

# include "ratc.h"
# include "DynArray.h"
# include <stdio.h>
# include "General.h"
# include "Memory.h"

/* INVARIANT ElmtCount * AlignedSize (ElmtSize) % sizeof (int32_t) == 0 */

static uint32_t AlignedSize (ElmtSize)
   uint32_t	ElmtSize;
   {
      register uint32_t Align;

      if (ElmtSize >= yyMaxAlign) {
	 Align = yyMaxAlign;
      } else {
	 Align = Exp2 (Log2 (ElmtSize + ElmtSize - 2));
      }
      return ElmtSize + Align - 1 - (ElmtSize - 1) % Align;
   }

void MakeArray (ArrayPtr, ElmtCount, ElmtSize)
   char * *	ArrayPtr	;
   uint32_t * ElmtCount	;
   uint32_t ElmtSize	;
   {
      ElmtSize = AlignedSize (ElmtSize);
      switch (ElmtSize % 4) {
      case 0: break;
      case 2: if (* ElmtCount & 1) (* ElmtCount) ++; break;
      case 1:
      case 3: * ElmtCount += sizeof (int32_t) - 1 - (* ElmtCount - 1) % sizeof (int32_t); break;
      }
      * ArrayPtr = Alloc (* ElmtCount * ElmtSize);
      if (* ArrayPtr == NULL) (void) fprintf (stderr, "MakeArray: out of memory\n");
   }

void ExtendArray (ArrayPtr, ElmtCount, ElmtSize)
   char * *	ArrayPtr	;
   uint32_t * ElmtCount	;
   uint32_t ElmtSize	;
   {
      		char *	NewPtr	;
      register	int32_t *	Source	;
      register	int32_t *	Target	;
      register	int32_t	i     	;

      ElmtSize = AlignedSize (ElmtSize);
      NewPtr = Alloc (* ElmtCount * ElmtSize * 2);
      Source = (int32_t *) * ArrayPtr;
      Target = (int32_t *) NewPtr;
      i      = * ElmtCount * ElmtSize / sizeof (int32_t);

      if (NewPtr == NULL)
	 (void) fprintf (stderr, "ExtendArray: out of memory\n");
      else {
	 do {
	    * Target ++ = * Source ++;
	 } while (i -- > 0);

	 Free (* ElmtCount * ElmtSize, * ArrayPtr);
	 * ElmtCount += * ElmtCount;
      }
      * ArrayPtr = NewPtr;
   }

void ReleaseArray (ArrayPtr, ElmtCount, ElmtSize)
   char * *	ArrayPtr	;
   uint32_t * ElmtCount	;
   uint32_t ElmtSize	;
   {
      ElmtSize = AlignedSize (ElmtSize);
      Free (* ElmtCount * ElmtSize, * ArrayPtr);
   }
