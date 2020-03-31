/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Opening a file on environment variable search path
||@(#)   System : OCC2C
||@(#) Filename : Fopenenv.c
||@(#)  Version : 1.6
||@(#)     Date : 25/3/97
\*@(#)====================================================*/
/*}}}*/

/*{{{  includes*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Fopenenv.h"
/*}}}*/

/*#define DEBUG*/

/*{{{  void *malloc_check(size_t size)*/
void *malloc_check(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    fputs("\nOut of memory\n", stderr);
    exit(EXIT_ERR);
  }
  return(ptr);
}
/*}}}*/
    
/*{{{  FILE *fopenenv(char *file, char *opts, char *env, char *def, char **name)*/
FILE *fopenenv(char *file, char *opts, char *env, char *def, char **name)
{
  char *tryname;
  char *gotenv;
  FILE *fp;
  char *start, *end;

  if (name != NULL)
    *name = NULL;
  
  if ((fp=fopen(file,opts))!=NULL)
  {
    if (name != NULL)
    {
      *name = (char *) malloc_check(strlen(file)+1) ;
      strcpy(*name,file) ;
    }
    return fp;
  }

  if ((gotenv=getenv(env))!=NULL)
    /*{{{  check through fullnames*/
    {
      start = gotenv;
      do
      {
        while ((*start==' ')||(*start==';')
    #ifndef MSARCH 
               ||(*start==':')
    #endif
                              ) start++;
        end=start;
        while ((*end!=' ')&&(*end!=';')
    #ifndef MSARCH    
               &&(*end!=':')
    #endif
               &&(*end!=0)) end++;
        if ((end-start)>0)
        {
          /*{{{  malloc and build full filename*/
          tryname = malloc_check((end-start)+strlen(file)+1);
          strncpy(tryname,start,(end-start));
          tryname[end-start]=0;
          strcat(tryname,file);
          /*}}}*/
    
#ifdef DEBUG
          fprintf(stderr,"Opening %s with options %s\n",tryname,opts);
#endif
          
          fp=fopen(tryname,opts);
    
          if (fp!=NULL && name!=NULL)
            *name = tryname ;
          else
            free(tryname);
          
          if (fp!=NULL)
            return fp;
          else if (*(end-1) != '/' && *(end-1) != '\\')
            fprintf(stderr,"Warning-%s(%d)-Expected %s item to end in path separator (found %c)\n",
                    __FILE__,__LINE__,env,*(end-1));
        }
        start = end + 1;
      } while (*end != 0);
    }
    /*}}}*/
  if (def != NULL)
    /*{{{  try default*/
    {
      tryname = (char *) malloc_check(strlen(def)+strlen(file)+1) ;
      strcpy(tryname,def) ;
      strcat(tryname,file);
    
#ifdef DEBUG
      fprintf(stderr,"Opening %s with options %s\n",tryname,opts);
#endif
      fp=fopen(tryname,opts);
    
      if (fp!=NULL && name!=NULL)
        *name = tryname ;
      else
        free(tryname);
      
      return(fp);
    }
    /*}}}*/
  else
    /*{{{  no default exists*/
    {
      return NULL;
    }
    /*}}}*/
}
/*}}}*/

/*{{{  int  ftestenv (char *file, char *opts, char *env, char *def, name)*/
int  ftestenv (char *file, char *opts, char *env, char *def, char **name)
{
  int result ;
  FILE *fp;

  fp = fopenenv (file, opts, env, def, name);

  if (fp == NULL)
    result = 0;
  else
  {
    fclose(fp);
    result = 1;
  }

  return (result);
}
/*}}}*/

#ifdef TEST
int main (int argc, char **argv)
{
  int i ;

  for (i=2;i<argc;i++)
  {
    FILE *fp = fopenenv (argv[1],"r",argv[i],NULL);
    if (fp == NULL)
      printf("Could not find %s on %s\n",argv[1],argv[i]);
    else
    {
      printf("Found %s on %s\n",argv[1],argv[i]);
      fclose(fp);
    }
  }  
}
#endif
