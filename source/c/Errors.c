/* $Id: Errors.c,v 1.2 1995/03/21 10:21:02 mbh Exp $ */

#include "Errors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "System.h"
#include "Sets.h"
#include "Idents.h"

#include "Scanner.h"

bool ErrorDetail = false;

# define MaxError        100
int ErrorCount = 0;

static void yyExit () { Exit (1); }
void (* Errors_Exit) () = yyExit;
typedef struct
{
   tPosition        Position;
   bool             IsErrorCode;
   short            ErrorNumber;
   short            ErrorCode;
   short            ErrorClass;
   short            InfoClass;
   union
   {
      int           vInteger;
      short         vShort;
      long          vLong;
      double        vReal;
      bool          vBoolean;
      char          vCharacter;
      tStringRef    vString;
      tSet *        vSet;
      tIdent        vIdent;
   }                Info;
} tError;

static void WriteHead        ARGS((tPosition Position, int ErrorClass));
static void WriteCode        ARGS((int ErrorCode));
static void WriteInfo        ARGS((int InfoClass, char * Info));
static void WriteMessage ARGS((bool IsErrorCode, int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info));
static void StoreMessage ARGS((bool IsErrorCode, int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info));
static int IsLess        ARGS((tError * i, tError * j));

static tError        ErrorTable [MaxError + 1];
static int        MessageCount;
static bool        IsStore                = false;
static void (*        HandleMessage) ARGS((bool IsErrorCode, int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info)) = WriteMessage;
static FILE *        Out = NULL;


void ErrorMessage (int ErrorCode, int ErrorClass, tPosition Position)
{
   (* HandleMessage) (true, ErrorCode, ErrorClass, Position, xxNone, NULL);
}

void ErrorMessageI (int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info)
{
   (* HandleMessage) (true, ErrorCode, ErrorClass, Position, InfoClass, Info);
}

void Message (char * ErrorText, int ErrorClass, tPosition Position)
{
   (* HandleMessage) (false, MakeIdent (ErrorText, strlen (ErrorText)), ErrorClass, Position, xxNone, NULL);
}

void MessageI (char * ErrorText, int ErrorClass, tPosition Position, int InfoClass, char * Info)
{
   (* HandleMessage) (false, MakeIdent (ErrorText, strlen (ErrorText)), ErrorClass, Position, InfoClass, Info);
}

static void WriteHead (tPosition Position, int ErrorClass)
{
   WritePosition (Out, Position);
   (void)fputs(": ",Out);
   switch (ErrorClass)
   {
     case xxFatal                 : (void) fputs ("Fatal - ",       Out); break;
     case xxRestriction           : (void) fputs ("Restriction - ", Out); break;
     case xxError                 : (void) fputs ("Error - ",       Out); break;
     case xxWarning               : (void) fputs ("Warning - ",     Out); break;
     case xxRepair                : (void) fputs ("Repair - ",      Out); break;
     case xxNote                  : (void) fputs ("Note - ",        Out); break;
     case xxInformation           : (void) fputs ("Information -", Out); break;
     default                      : (void) fprintf (Out, "Error class: %d - ", ErrorClass);
   }
}

static void WriteTail (int ErrorClass)
{
   (void) fputc ('\n', Out);
   if (ErrorClass == xxFatal) Errors_Exit ();
}

static void WriteCode (int ErrorCode)
{
   switch (ErrorCode)
   {
     case xxNoText              : break;
     case xxSyntaxError         : (void) fputs ("syntax error"                , Out); break;
     case xxExpectedTokens      : (void) fputs ("expected tokens "        , Out); break;
     case xxRestartPoint        : (void) fputs ("restart point"                , Out); break;
     case xxTokenInserted       : (void) fputs ("token inserted "        , Out); break;
     case xxParserError         : (void) fputs ("Parser error/restriction "        , Out); break;
     case xxOpError             : (void) fputs ("Invalid operator here "        , Out); break;
     case xxTypeError           : (void) fputs ("Type check error "            , Out); break;
     case xxUndeclared          : (void) fputs ("Identifier not declared "     , Out); break;
     case xxUsage               : (void) fputs ("Incorrect identifier usage "  , Out); break;
     case xxParallelUsage       : (void) fputs ("Parallel usage of variable"  , Out); break;
     case xxNotImplemented      : (void) fputs ("Implementation Restriction",Out); break;
     case xxNonConstant         : (void) fputs ("Expression with constant value required",Out); break;
     case xxTooManyErrors       : (void) fputs ("too many errors " , Out); break;
     default                    : (void) fprintf (Out, " error code: %d", ErrorCode);
   }
}

static void WriteInfo (int InfoClass, char * Info)
{
   if (InfoClass == xxNone) return;
   switch (InfoClass)
   {
     case xxInteger               : (void) fprintf (Out, "%d", * (int *) Info); break;
     case xxShort                 : (void) fprintf (Out, "%d", * (short *) Info); break;
     case xxCharacter             : if (isprint(*Info)) fprintf (Out, "%c", * Info);
				    else fprintf(Out,"0x%02x",* Info);
				    break;
     case xxString                : (void) fputs (Info, Out); break;
     case xxSet                   : WriteSet (Out, (tSet *) Info); break;
     case xxIdent                 : WriteIdent (Out, * (tIdent *) Info); break;
     default                      : (void) fprintf (Out, "info class: %d", InfoClass);
   }
}

static void WriteMessage (bool IsErrorCode, int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info)
{
  if (Out == NULL) Out=stderr;
  
  if (ErrorClass==xxError || ErrorClass==xxRestriction || ErrorClass==xxFatal)
    ErrorCount++;
  else
    if (nowarnings) return;
  
  if (ErrorClass!=xxInformation && ErrorClass!=xxRepair && Position.File!=NULL)
    /*{{{  display errant code line*/
    {
      FILE *fi;
      char buff[1024];
      
      StGetString(Position.File,buff);
      fi = fopen(buff,"r");
      if (fi != NULL)
      {
	int i;
	char c;
	
	(void) fputc ('\n', Out);
      
	for(i = 1; i <= Position.Line && !feof(fi); ++i)
	{
	  bool full;
	  (void)fgets_padding_tabs(buff,1024,fi,&full);
	}
	for(i=0; buff[i]!=0; ++i)
	  if (buff[i]<' ' || buff[i]>126) buff[i]=' ';
	
	fprintf(Out,":%s\n",buff);
	for(i = 0; i < Position.Column; ++i)
	  buff[i] = '-';
      
	buff[i] = '^'; buff[i+1] = 0;
	fprintf(Out,"-%s\n",buff);
	fclose(fi);
      }
    }
    /*}}}*/

  WriteHead (Position, ErrorClass);
  if (IsErrorCode) WriteCode (ErrorCode); else WriteIdent (Out, ErrorCode);
  WriteInfo (InfoClass, Info);
  (void) fputc ('\n', Out);
  if (ErrorClass == xxFatal && ! IsStore) (* Errors_Exit) ();
}

void WriteMessages (FILE * File)
{
   int i;
   char * Info;
   char s [256];

   ErrorCount=0;
   qsort ((char *) & ErrorTable [1], MessageCount, sizeof (tError), IsLess);
   Out = File;
   for (i = 1; i <= MessageCount; i ++) {
      register tError * With = & ErrorTable [i];

      switch (With->InfoClass) {
      case xxInteger	: Info = (char *) & With->Info.vInteger	; break;
      case xxShort	: Info = (char *) & With->Info.vShort	; break;
      case xxLong	: Info = (char *) & With->Info.vLong	; break;
      case xxReal	: Info = (char *) & With->Info.vReal	; break;
      case xxBoolean	: Info = (char *) & With->Info.vBoolean	; break;
      case xxCharacter	: Info = (char *) & With->Info.vCharacter; break;
      case xxString	: StGetString (With->Info.vString, s); Info = s; break;
      case xxSet	: Info = (char *) With->Info.vSet; break;
      case xxIdent	: Info = (char *) & With->Info.vIdent	; break;
      }
      WriteMessage (With->IsErrorCode, With->ErrorCode, With->ErrorClass, With->Position, With->InfoClass, Info);
   }
   Out = stderr;
}

static void StoreMessage (bool IsErrorCode, int ErrorCode, int ErrorClass, tPosition Position, int InfoClass, char * Info)
{
  if (ErrorClass==xxError || ErrorClass==xxRestriction || ErrorClass==xxFatal)
    ErrorCount++;
  else
    if (nowarnings) return;
    
  if (MessageCount < MaxError)
  {
    MessageCount ++;
    {
      register tError * With = & ErrorTable [MessageCount];

      With->Position	= Position;
      With->IsErrorCode	= IsErrorCode;
      With->ErrorNumber	= MessageCount;
      With->ErrorCode	= ErrorCode;
      With->ErrorClass	= ErrorClass;
      With->InfoClass	= InfoClass;
      switch (With->InfoClass) {
      case xxInteger	: With->Info.vInteger	= * (int	*) Info; break;
      case xxShort	: With->Info.vShort	= * (short	*) Info; break;
      case xxLong	: With->Info.vLong	= * (long	*) Info; break;
      case xxReal	: With->Info.vReal	= * (double	*) Info; break;
      case xxBoolean	: With->Info.vBoolean	= * (bool	*) Info; break;
      case xxCharacter	: With->Info.vCharacter	= * (char	*) Info; break;
      case xxString	: With->Info.vString	= PutString (Info, strlen (Info)); break;
      case xxSet	: With->Info.vSet = (tSet *) Alloc ((unsigned long) sizeof (tSet));
			  MakeSet (With->Info.vSet, Size ((tSet *) Info));
			  Assign (With->Info.vSet, (tSet *) Info); break;
      case xxIdent	: With->Info.vIdent	= * (tIdent	*) Info; break;
      }
    }
  }
  else
  {
    {
      register tError * With = & ErrorTable [MessageCount];

      With->IsErrorCode	= true;
      With->ErrorCode	= xxTooManyErrors;
      With->ErrorClass	= xxRestriction;
      With->InfoClass	= xxNone;
    }
  }
  if (ErrorClass == xxFatal) { WriteMessages (stderr); (* Errors_Exit) (); }
}

static int IsLess (tError * i, tError * j)
{
  register int r = Compare (i->Position, j->Position);
  return r != 0 ? r : i->ErrorNumber - j->ErrorNumber;
}

void StoreMessages (bool Store)
{
  if (Store)
  {
    HandleMessage = StoreMessage;
    MessageCount  = 0;
  }
  else
  {
    HandleMessage = WriteMessage;
  }
  IsStore = Store;
}

