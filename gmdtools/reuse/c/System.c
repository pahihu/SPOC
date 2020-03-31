/* $Id: System.c,v 1.6 1992/05/05 13:19:05 grosch rel $ */

/* $Log:
 */

/* Ich, Doktor Josef Grosch, Informatiker, Jan. 1992 */

/* interface for machine dependencies */

/* See the header file System.h for comments concerning the external routines!	*/

/* compilation with the option -DUNIX uses UNIX system calls for IO (efficient),
   otherwise the C library routines are used for IO (portable).			*/

static char rcsid [] = "$Id: System.c,v 1.6 1992/05/05 13:19:05 grosch rel $";

# include "System.h"

# ifdef m68000
# define hz 50
# else
# define hz 60
# endif

# ifndef UNIX

# include <stdio.h>
# define NOFILES 32

static char IsLineBuffered [NOFILES] = { 1, 1, 1, };

static FILE *	FileStore [NOFILES] = {
   stdin, stdout, stderr, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static tFile FileToInt (File)
   FILE *	File;
{
   register int	f = fileno (File);
   FileStore [f] = File;
   return f;
}

static FILE * IntToFile (File)
   tFile	File;
{
   return FileStore [File];
}

# endif

/* binary IO */

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>

bool IsCharacterSpecial (File) tFile File;
{
   struct stat	buf;
   (void) fstat (File, & buf);
   return (0020000 & buf.st_mode) == 0020000;
}

tFile OpenInput (FileName)
   char *	FileName;
{
# ifndef UNIX
   tFile File = FileToInt (fopen (FileName, "r"));
   IsLineBuffered [File] = IsCharacterSpecial (File);
   return File;
# else
   return open (FileName, O_RDONLY);
# endif
}

tFile OpenOutput (FileName)
   char *	FileName;
{
# ifndef UNIX
   return FileToInt (fopen (FileName, "w"));
# else
   return creat (FileName, 0666);
# endif
}

int Read (File, Buffer, Size)
   tFile	File;
   char *	Buffer;
   int		Size;
{
# ifndef UNIX
   if (IsLineBuffered [File]) {
      Buffer [0] = '\0';
      (void) fgets (Buffer, Size, IntToFile (File));
      return strlen (Buffer);
   } else
      return fread (Buffer, 1, Size, IntToFile (File));
# else
   return read (File, Buffer, Size);
# endif
}

int Write (File, Buffer, Size)
   tFile	File;
   char *	Buffer;
   int		Size;
{
# ifndef UNIX
   return fwrite (Buffer, 1, Size, IntToFile (File));
# else
   return write (File, Buffer, Size);
# endif
}

void Close (File)
   tFile	File;
{
# ifndef UNIX
   (void) fclose (IntToFile (File));
# else
   (void) close (File);
# endif
}

/* calls other than IO */

/* # include <malloc.h> */

char * SysAlloc (ByteCount) int32_t ByteCount; { return (char *) malloc ((unsigned) ByteCount); }

# include <sys/times.h>

int32_t Time ()
{
   struct tms	buffer;
   (void) times (& buffer);
   return (buffer.tms_utime + buffer.tms_stime) * 1000 / hz;
}

static int	argc;
static char * *	argv;

int GetArgCount ()
{
   return argc;
}

void GetArgument (ArgNum, Argument)
   int		ArgNum;
   char *	Argument;
{
   register int	i = 0;
   for (;; i ++)
      if ((Argument [i] = argv [ArgNum][i]) == '\0') return;
}

void PutArgs (Argc, Argv)
   int		Argc;
   char * *	Argv;
{
   argc = Argc;
   argv = Argv;
}

# include <errno.h>

int ErrNum () { return errno; }

int System (String) char * String; { return system (String); }

extern void exit ();

void Exit (Status) int Status; { exit (Status); }

void BEGIN_System () {}
