static char rcsid [] = "$Id: IdentsDrv.c,v 1.3 1992/05/05 13:19:05 grosch rel $";

# include <stdio.h>
# include "Idents.h"

extern int strlen ();

static void loop ()
{
   char string [256];
   tIdent ident;

   (void) printf ("enter strings, one per line, - terminates\n");
   do {
      (void) scanf ("%s", string);
      ident = MakeIdent (string, strlen (string));
      WriteIdent (stdout, ident);
      (void) printf ("\n");
   } while (string [0] != '-' || string [1] != '\0');
   (void) printf ("\n");
   WriteIdents ();
}

main ()
{
   loop ();
   InitIdents ();
   loop ();
   return 0;
}
