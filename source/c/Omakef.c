/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Automated makefile generation
||@(#)   System : OCC2C
||@(#) Filename : Omakef.c
||@(#)  Version : 1.50
||@(#)     Date : 2/18/97
\*@(#)====================================================*/
/*}}}*/

#define ADD_SPSERVER

/*{{{  includes*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>

#include "Fopenenv.h"
/*}}}*/
/*{{{  string and character constants*/
#define QUOTE '"'
#define DOT   '.'
#define SEP   '/'
#define NUL   '\0'
#define NEWL  '\n'
#define EQU   '='
#define DEF_ENV "/spoc"

#define TAB_SIZE 8

static char arch_string[] = ARCH;
static char arch_macro[] = "ARCH=";

static char occam_comment[] = "--";
static char  hash_comment[] = "##";
static char  empty_string[] = "";
static char    dot_string[] = ".";
static char    sep_string[] = "/";
static char  space_string[] = " ";

static char intrinsics_lib_string[] = "Intrinsics.lib";
#ifdef ADD_SPSERVER
static char serve_lib_string[] = "spserver.lib";
#endif
static char use_string[] = "#USE";
static int  use_size     = sizeof(use_string)-1;
static char include_string[] = "#INCLUDE";
static int  include_size     = sizeof(include_string)-1;

static char env_osearch[]  = "OSEARCH";
static char env_spoc[]     = "SPOC";
static char dir_lib[]     = "lib";
static char *default_dir  = NULL;
static char *template_name = "Omakef.tpl";

static char occ_options[] = "OCCOPTIONS";
static char  cc_options[] =  "CCOPTIONS";

static char occ_dep_string[]   = "-DEP";
static char hdr_dep_string[]   = "-HDR";
static char ldd_dep_string[]   = "-LDD";
static char lbb_dep_string[]   = "-LBB";
static char lhd_dep_string[]   = "-LHD";

static char  clean_string[]  = "-CLEAN";
static char  clean_rule[]    = "\nclean:\n\t";
static char  clean_command[] = "$(RM) ";

static char VERSION[] = "1.50";

#define output_width 78
/*}}}*/
/*{{{  booleans*/
typedef int bool;
#define false (0)
#define true  (!false)
/*}}}*/
/*{{{  compilation flags*/
/*#define DEBUG*/
/*}}}*/

/*{{{  string parsing routines*/
/*{{{  char *skip_non_blanks (char *s)*/
char *skip_non_blanks (char *s)
{
  while ((*s != NUL) && (!isspace(*s)))
    s++ ;
  return(s) ;
}

/*}}}*/
/*{{{  char *skip_blanks (char *s)*/
char *skip_blanks (char *s)
{
  while ((*s != NUL) && (isspace(*s)))
    s++ ;
  return(s) ;
}

/*}}}*/
/*{{{  char *skip_digits (char *s)*/
char *skip_digits (char *s)
{
  while ((*s != NUL) && (isdigit(*s)))
    s++ ;
  return(s) ;
}

/*}}}*/
/*{{{  char *skip_until (char *s, char c)*/
char *skip_until (char *s, char c)
{
  while ((*s != NUL) && (*s != c))
    s++ ;
  return(s) ;
}
/*}}}*/

/*{{{  bool  match_char (char *s, char c)*/
bool  match_char (char *s, char c)
{
  return (*s == c) ;
}
/*}}}*/
/*{{{  char *match_string (char *s)*/
char *match_string (char *s)
{
  char *f;

  if (match_char (s,QUOTE))
  {
    s = s+1;
    f = skip_until(s,QUOTE);

    if (match_char(f,QUOTE))
      {
        char *m; int i;
        m = malloc_check (f-s+1);
        for (i=0;i<f-s;i++)
          m[i] = s[i];
        m[i]=NUL;
        return(m);
      }
  }

  return(NULL);    
}
/*}}}*/
/*}}}*/
/*{{{  file reading primitives*/
/*{{{  char *fgetstring (..)*/
/*
   This routine reads a line, strips out comments and strips
   out lines of just white space. Usage much like fgets.
   Comments are limited to `ignore-rest-of-line' type.
*/

char *fgetstring (char *line, int n, FILE *file, char *comment, int *linenum)
{
  char *s ;

  do
  {
    if (feof(file))
      return(NULL);
    if ((s=fgets (line, n, file)) == NULL)
    {
      if (ferror(file))
      {
        fputs("\nFile read error.\n",stderr);
        exit(EXIT_ERR);
      }
      return(NULL) ;
    }
    if ((s[0] != NUL))
    {
      char *comm = strstr (s, comment) ;
      int i ;
       
      if ((strlen(s) == n-1) && (s[n-2] != NEWL))
        fprintf(stderr, "Warning-%s(%d)- Line %d too long so wrapped round\n",__FILE__,__LINE__,*linenum) ;
      else
        (*linenum)++ ;

      if (comm != NULL)
        *comm = NUL ;
       
      for (i=0;i<strlen(s);i++)
        if (!isspace(s[i]))
          break ;
       
      if (i==strlen(s))
        s[0] = NUL ;    
    }
  } while (*s == NUL) ;

  return(s) ;
}
/*}}}*/
  
#define buflen 1024

FILE *fp_out;
char *fn_out;
/*}}}*/
/*{{{  parse command*/
static bool debug=false;
#ifdef ADD_SPSERVER
static bool serve=false;
#endif
static bool noisy=false;
static bool intrinsics=true;
static bool spserver=false;
typedef enum { gen_none, gen_trans, gen_comp, gen_both } out_selector;

static out_selector make_type = gen_none;

static int farg = 0;

/*{{{  char *parse_command (int argc, char **argv)*/
char *parse_command (int argc, char **argv)
{
  int i;
  char *target = NULL;
  
  for(i=1;i<argc;i++)
  {
    if (argv[i][0]=='-')
    {
      if (target == NULL)
      {
        if (argv[i][1] == 'c')
        {
          if (make_type == gen_none) make_type = gen_comp;
          else if (make_type == gen_trans) make_type = gen_both;
        }
        else if (argv[i][1] == 't')
        {
          if (make_type == gen_none) make_type = gen_trans;
          else if (make_type == gen_comp) make_type = gen_both;
        }
        else if (argv[i][1] == 'z')
          noisy = true;
        else
          fprintf(stderr,"Invalid Omakef option: %s\n",argv[i]);
      }
      else
      {
        switch(argv[i][1])
        {
          case 'm': case 'z': case 'p': case 'e': case 'x': break;
          default:
          {
            int j=1;
          
            while (argv[i][j]!='\0')
            {
              if (argv[i][j] == 'g')
                debug = true;
              else if (argv[i][j] == 'i')
                intrinsics = false;
#ifdef ADD_SPSERVER
              else if (argv[i][j] == 's')
                serve = true;
#endif
              j++;
            }
          }
          break;
        }
      }
    }
    else if (target == NULL)
    {  target = argv[i]; farg = i + 1; }
  }

  if (target == NULL)
  {
    fprintf(stderr,"%s (version %s) : compiled on %s\n", argv[0], VERSION, __DATE__);
    exit(EXIT_ERR);
  }

  if (make_type == gen_none) make_type = gen_both;
  
  return (target);
}

/*}}}*/

/*{{{  void generate_options (int argc, char **argv)*/
void generate_options (int argc, char **argv)
{
  int i;

  fprintf(fp_out,"%s=",occ_options);
  
  for(i=farg;i<argc;i++)
  {
    if (argv[i][0]=='-')
    {
      int j=1;
      fprintf(fp_out,"-");
      while (argv[i][j]!='\0')
      {
        /*if (argv[i][j] != 'z') Why skip -z options ?*/
          fprintf(fp_out,"%c",argv[i][j]);
        j++;
      }
      fprintf(fp_out," ");
    }
  }

  fprintf(fp_out,"\n%s=", cc_options);

  if (debug) fprintf(fp_out, "-g");
  fprintf(fp_out, "\n");
}
/*}}}*/

/*{{{  void generate_defines (int argc, char **argv)*/
void generate_defines (int argc, char **argv)
{
  int i;

  fprintf(fp_out, "\n# Any command-line definitions:\n");
  
  for(i=farg;i<argc;i++)
  {
    if (argv[i][0]=='-')
      continue;
    else
    {
      char *equals = strchr (argv[i],EQU);

      if (equals != NULL);
        fprintf(fp_out,"%s\n",argv[i]);
    }
  }
  fprintf(fp_out, "\n");
}
/*}}}*/
/*}}}*/

/*{{{  file extensions*/
/*{{{  file expected*/
typedef enum { from_command, from_use, from_include } file_expected;
/*}}}*/
/*{{{  extensions*/
typedef enum { file_cfg, file_occ, file_inc, file_t, file_lib, file_mod, file_pgm, file_x, file_hdr,
               file_o,   file_a,   file_exe, file_ini, file_num_types, file_unknown=-1 } file_hype;
static char *file_ext[]  = { "cfg", "occ", "inc", "t", "lib", "c", "c", "x", "hdr", "o", "a", "" };
static char *tmpl_ext[]  = { "cfg", "occ", "inc", "t", "lib", "mod", "pgm", "x", "hdr", "o", "a", "exe", "header" };
/*}}}*/
/*{{{  other extensions*/
typedef enum { file_mkf, file_mkt, file_mkc, file_liu } other_type;
static char *other_ext[] = { "mkf", "mkt", "mkc", "liu" };
/*}}}*/
/*{{{  source files*/
static file_hype file_src[] = { file_cfg, file_occ };
static int  file_num_srcs = sizeof(file_src)/sizeof(file_hype);
/*}}}*/

/*{{{  bool is_source_file (file_hype t)*/
bool is_source_file (file_hype t)
{
  return (t==file_cfg || t==file_occ || t==file_inc);
}
/*}}}*/
/*{{{  bool is_module_file (file_hype t)*/
bool is_module_file (file_hype t)
{
  return (t==file_t || t==file_lib);
}
/*}}}*/
/*{{{  bool is_include_file (file_hype t)*/
bool is_include_file (file_hype t)
{
  return (t==file_inc);
}
/*}}}*/

/*{{{  bool equivalent_extensions (file_hype t1, file_hype t2)*/
bool equivalent_extensions (file_hype t1, file_hype t2)
{
  return (t1==t2 || (is_module_file(t1) && is_module_file(t2)));
}
/*}}}*/
/*{{{  file_hype match_extension (char *name)*/
file_hype match_extension (char *name)
{
  char *s = strrchr (name,DOT);

  if (s == NULL)
    return(file_unknown);
  else
    {
      file_hype t ;

      for (t=0; t<file_num_types; t++)
        if (strcmp (tmpl_ext[t],s+1) == 0)
          return(t);
      return (file_unknown);
    }
}
/*}}}*/
/*{{{  file_hype map_extension (char *name, file_expected exp)*/
file_hype map_extension (char *name, file_expected exp)
{
  file_hype type = match_extension (name);
  
  switch (exp)
  {
    case from_command :
      if (type == file_unknown)
      {
        type = file_exe;
      }
      else if (is_source_file(type))
      {
        fprintf(stderr, "Cannot build makefile for source file, %s\n", name);
        exit(EXIT_ERR);
      }
      break;
  
    case from_use :
      if (type == file_lib)
      {
        /*printf("Encountered library file %s\n", name);*/
      }
      else if (type != file_t)
      {
        /*fprintf(stderr, "Extension did not match, assuming that %s is module\n", name);*/
        type = file_t;
      }
      break;
  
    case from_include :
      if (type != file_inc && type != file_occ)
      {
        /*fprintf(stderr, "Extension did not match, assuming that %s is include\n", name);*/
        type = file_inc;
      }
      break;
  }

  return (type);
}
/*}}}*/
  
/*{{{  int sizeof_root_name (char *name)*/
int sizeof_root_name (char *name)
{
  char *s = strrchr (name,DOT);

  if (s == NULL)
    return (strlen(name));
  else
    return (s-name);
}    
/*}}}*/
/*{{{  char *extract_root (char *name)*/
char *extract_root (char *name)
{
  int root = sizeof_root_name (name);
  char *n;

  n = malloc_check (root + 1);
  
  memcpy(n,name,root);
  n[root] = NUL;

  return(n);
}
/*}}}*/
/*{{{  char *form_extended_name (char *root, char *ext)*/
char *form_extended_name (char *root, char *ext)
{
  char *n;
  int r = strlen(root);

  n = malloc_check (r + strlen(ext) + 2);

  memcpy(n,root,r);
  n[r] = DOT;
  strcpy(&n[r+1],ext);

  return(n);
}
/*}}}*/
/*}}}*/

/*{{{  FILE *fopen_file (char *name, char **full)*/
FILE *fopen_file (char *name, char **full)
{
  FILE *fh;


#ifdef DEBUG
  printf("Opening file %s with default directory %s : ", name,default_dir);
#endif
  
  fh = fopenenv(name,"r",env_osearch,default_dir,full);
#ifdef DEBUG
  printf("%s\n",(fh==NULL)?"FAIL":"OK");  
#endif
  return fh;
}
/*}}}*/
/*{{{  bool  ftest_file (char *name, char **full)*/
bool  ftest_file (char *name, char **full)
{
  FILE *fh = fopen_file(name,full);

  if (fh != NULL) fclose(fh);
  return (fh != NULL);
}
/*}}}*/

/*{{{  module/dependency handling*/
typedef struct module module;
typedef struct dependence depend;

/*{{{  typedef struct dqueue*/
typedef struct dqueue dqueue;
struct dqueue
{
  depend *head;
  depend *tail;
  int length;
};
/*}}}*/
/*{{{  typedef struct mqueue*/
typedef struct mqueue mqueue;
struct mqueue
{
  module *head;
  module *tail;
  int length;
};

/*}}}*/
/*{{{  struct dependence*/
struct dependence
{
  depend *next;
  module *module;
};
/*}}}*/
/*{{{  struct module*/
struct module
{
  module *next;
  char *name;
  char *root;
  char *path;
  char *source;
  bool done;
  file_hype type;
  file_expected exp;
  int index;
  dqueue inc_queue;
  dqueue use_queue;
  dqueue link_queue;
};
/*}}}*/

static mqueue modules, pending, includes, complete;
static int modnum = 0;

/*{{{  defines*/
#define TOP_QUEUE(q)    ((q)->head)
#define NEXT_QUEUE(q)   ((q)->head->next)
#define EMPTY_QUEUE(q)  ((q)->head == NULL)
#define INIT_QUEUE(q)   {(q)->head = NULL; (q)->tail = NULL; (q)->length = 0;}
#define LENGTH_QUEUE(q) ((q)->length)
#define ENQUEUE(q,t)    {(t)->next=NULL;\
                         if ((q)->head==NULL) (q)->head=t; else (q)->tail->next=t;\
                         (q)->tail=t;(q)->length++;}
#define PREPENDQUEUE(q,t) { (t)->next=(q)->head; (q)->head=t; if ((q)->tail==NULL) (q)->tail=t; (q)->length++; }

#define DEQUEUE_TOP(q)  {(q)->head=(q)->head->next; if ((q)->head==NULL) (q)->tail=NULL; }

/*}}}*/

/*{{{  void display_modules(mqueue *mq)*/
void display_modules(mqueue *mq)
{
  module *mod = TOP_QUEUE(mq);
  
  printf("Modules\n");
  while(mod != NULL)
  {
    printf("  %-10s.%-3s:%2d ",mod->root,file_ext[mod->type],mod->index);
    if (!EMPTY_QUEUE(&mod->use_queue))
    {
      depend *dep = TOP_QUEUE(&mod->use_queue);
      
      printf("[");
      while(dep)
      {
        printf("%s ",dep->module->root);
        dep = dep->next;
      }
      printf("]");
    }
    printf("\n");
    mod = mod->next;
  }
}

/*}}}*/

/*{{{  module *make_module (char *name, file_expected exp)*/
module *make_module (char *name, file_expected exp)
{
  module *m;
  
  m = (module *) malloc_check(sizeof(module));
  m->name = (char *) malloc_check (strlen(name)+1);
  
  strcpy(m->name, name);
  
  m->type = map_extension (name,exp);
  m->exp = exp;

  /*{{{  generate root*/
  if (m->type == file_exe)
    m->root = m->name ;
  else
    m->root = extract_root(m->name);
  /*}}}*/

  /*{{{  generate and validate source filename*/
  if (exp == from_include)
  {
    m->source = m->name;
  
    if (ftest_file (m->source, &m->path) == 0)
    {
      fprintf(stderr,"Error-%s(%d): Could not find source file, ignoring %s in Makefile\n", __FILE__,__LINE__,m->source);
      m->source = NULL;
      m->path = empty_string;
    }
    else
    {
      m->path[strlen(m->path) - strlen(m->source)] = NUL ;
      if (strcmp(m->path,default_dir) == 0)
      {
        sprintf(m->path,"$(%s)%s%s%s%s%s",env_spoc,sep_string,dir_lib,sep_string,arch_string,sep_string);
      }
    }
  }
  else
  {
    int i;
  
    for (i=0;i<file_num_srcs;i++)
      /*{{{  check if source exists*/
      {
        if (exp!=from_command && file_src[i]==file_cfg) continue;
             
        m->source = form_extended_name (m->root,file_ext[file_src[i]]);
      
        if (ftest_file (m->source, &m->path))
          break;
      
        free(m->source);
        free(m->path);
      }
      /*}}}*/
  
    if (i==file_num_srcs)
      /*{{{  source does not exist*/
      {
        m->source = NULL;
      
        if (ftest_file (m->name, &m->path) == 0)
        {    
          fprintf(stderr,"Error-%s(%d): Could not find source or library for file %s\n",__FILE__,__LINE__,m->name);
          m->path = empty_string;
          if (exp==from_command) exit(EXIT_ERR);
        }    
        else 
        {    
          m->path[strlen(m->path) - strlen(m->name)] = NUL ;
      if (strcmp(m->path,default_dir) == 0)
          {  
            sprintf(m->path,"$(%s)%s%s%s%s%s",env_spoc,sep_string,dir_lib,sep_string,arch_string,sep_string);
          }  
        }    
      }
      /*}}}*/
    else
    {
      m->path[strlen(m->path) - strlen(m->source)] = NUL ;
  if (strcmp(m->path,default_dir) == 0)
      {
        sprintf(m->path,"$(%s)%s%s%s%s%s",env_spoc,sep_string,dir_lib,sep_string,arch_string,sep_string);
      }
    }
  }
  /*}}}*/

  INIT_QUEUE(&m->inc_queue);
  INIT_QUEUE(&m->use_queue);
  INIT_QUEUE(&m->link_queue);
  m->done = false;
  m->index = (exp != from_include) ? modnum++ : 0;
  
  return(m);
}
/*}}}*/
/*{{{  void free_module (module *m)*/
void free_module (module *m)
{
  if (m->name != NULL) free(m->name);

  if (m->path != NULL && m->path != empty_string) free(m->path);

  if (m->source != m->name) free(m->source);
  if (m->root   != m->name) free(m->root);
  
  free(m);
}
/*}}}*/
/*{{{  module *search_module (mqueue *q, char *name)*/
module *search_module (mqueue *q, char *name)
{
  module *this = q->head ;

  while (this != NULL)
  {
    if ( (sizeof_root_name(name) == strlen(this->root)) &&
         (strncmp(this->root,name,strlen(this->root))==0))
      return (this);
    this = this->next;
  }
  return (NULL);
}
/*}}}*/

/*{{{  depend *make_depend (module *module,file_hype type)*/
depend *make_depend (module *module)
{
  depend *this;

  this = (depend *)malloc_check(sizeof(depend));

  this->module = module;
  this->next = NULL;
  
  return this;
}
/*}}}*/
/*{{{  void free_depend (depend *depend)*/
void free_depend (depend *depend)
{
  free(depend);
}
/*}}}*/
/*{{{  dep *search_dep(dqueue *q, module *m)*/
depend *search_depend(dqueue *q, module *m)
{
  depend *this = q->head ;

  while (this != NULL)
  {
    if (this->module == m) return this;
    this = this->next;
  }
  return (NULL);
}
/*}}}*/
/*{{{  bool find_circle(dqueue *q, module *m)*/
bool find_circle(dqueue *q, module *m, bool print)
{
  depend *this = TOP_QUEUE(q);

  while (this != NULL)
  {
    if (this->module == m)
    {
      if (print) fprintf(stderr,"%s",this->module->root);
      return true;
    }
    if (find_circle(&this->module->use_queue,m,false))
    {
      if (print)
      {
        fprintf(stderr,"%s->",this->module->root);
        (void)find_circle(&this->module->use_queue,m,true);
      }
      return true;
    }
    this = this->next;
  }
  return false;
}
/*}}}*/

/*{{{  bool sort_modules(module *mod)*/
bool sort_modules(module *mod)
{
  depend *this = TOP_QUEUE(&mod->use_queue);
  bool change = false;

  while(this)
  {
    module *m = this->module;
    
    change |= sort_modules(m);

    if (m->index < mod->index)
    {
      int index = m->index;
      
      m->index = mod->index;
      mod->index = index;
      change = true;
    }

    this = this->next;
  }
  return change;
}
/*}}}*/
/*{{{  void form_link_queue(module *top, int mcount)*/
void form_link_queue(module *top, int mcount)
{
  int changed;
  int index;
  
  do
  {
    changed = sort_modules(top);
  } while(changed);
  
  for(index = 1; index < mcount; ++index)
  {
    module *mod = top;

    while(mod)
    {
      if (mod->index == index)
      {
        depend *new = make_depend(mod);
        ENQUEUE(&top->link_queue,new);
        break;
      }
      mod = mod->next;
    }
    if (mod==NULL) fprintf(stderr,"Internal error, lost module when sorting\n");
  }  
}
/*}}}*/

/*{{{  void process_use (module *m, char *s,int line)*/
void process_use (module *m, char *s,int line)
{
  module *son;
  depend *depend;
  
  if (noisy) printf("Processing reference to %s\n",s);

  if ((son = search_module (&pending, s)) != NULL)
  {
#ifdef DEBUG
    printf("Already encountered %s (pending)\n",s);
#endif
  }
  else if ((son = search_module (&modules, s)) != NULL)
  {
#ifdef DEBUG
    printf("Already encountered %s (parsed)\n",s);
#endif
  }
  else
  {
    son = make_module(s,from_use);
    ENQUEUE(&pending,son);

    if (son->source == NULL && son->type == file_lib)
      /*{{{  add any dependent library files*/
      {
        char *file;
        FILE *fp;
             
        file = form_extended_name (son->root, other_ext[file_liu]);
             
        if ((fp = fopen_file (file, NULL)) != NULL)
        {    
          char linebuf[buflen];
          int line = 0; 
      
          if (noisy) printf("Parsing indirect file %s%s\n",son->path,file);
      
          while (!feof(fp))
          {  
            if (fgetstring (linebuf,buflen,fp,occam_comment,&line)==NULL)
              break ;
      
            s = skip_blanks (linebuf);
            *skip_non_blanks (s) = NUL;
      
      #ifdef DEBUG
            printf("Line %d of %s yields %s\n",line,file,s);
      #endif
             
            process_use (son, s, line);
          }  
      
          fclose(fp);
        }    
      
        free(file);
      }
      /*}}}*/
  }

  if (search_depend(&m->use_queue,son) == NULL)
  {
    depend = make_depend(son);
    PREPENDQUEUE(&m->use_queue,depend);
  }

  if (find_circle(&son->use_queue,m,false))
  {
    fprintf(stderr,"Error - Circular dependance - %s->%s->",m->root,son->root);
    (void)find_circle(&son->use_queue,m,true);
    fprintf(stderr,"\n");
    exit(EXIT_ERR);
  }

}
/*}}}*/
/*{{{  void parse_include(module *m, char *name)*/
void parse_include(module *m, char *name)
{
  void process_inc (module *m, char *s);
  char linebuf[buflen];
  int line = 0; FILE *fp; 
  if ((fp = fopen_file(name,NULL))==NULL)
  {
    fprintf(stderr,"Error-%s(%d): Could not find file, %s will be a leaf module\n", __FILE__,__LINE__,name);
    return;
  }  
  /*{{{  parse lines of file */
  while (!feof(fp))
  {
    char *p, *s = NULL;
  
    if (fgetstring (linebuf,buflen,fp,occam_comment,&line)==NULL)
      break ;
  
    p = skip_blanks (linebuf);
  
    if (strstr(p,use_string) == p)
      /*{{{  match USE*/
      {
        p = skip_blanks(p + use_size);
      
        if ((s = match_string (p)) == NULL)
          fprintf(stderr,"Warning: Could not extract filename from USE on line %d of %s\n",line,name);
        else
          process_use (m, s, line);
      }
      /*}}}*/
    else if (strstr(p,include_string) == p)
      /*{{{  match INCLUDE*/
      {
        p = skip_blanks(p + include_size);
      
        if ((s = match_string (p)) == NULL)
          fprintf(stderr,"Warning: Could not extract filename from INCLUDE on line %d of %s%s\n",line,m->path,m->source);
        else
          process_inc (m, s);
      }
      /*}}}*/
  
  }
  /*}}}*/
  
  fclose(fp);
}
/*}}}*/
/*{{{  void process_inc (module *m, char *s)*/
void process_inc (module *m, char *s)
{
  module *son;
  depend *depend;
  
  if (noisy) printf("Parsing reference to %s\n",s);
  
  parse_include(m,s);
    
  if (noisy) printf("Processing reference to %s\n",s);

  if ((son = search_module (&includes, s)) != NULL)
  {
#ifdef DEBUG
    printf("Already encountered %s (included)\n",s);
#endif
  }
  else
  {
    son = make_module(s,from_include);
    ENQUEUE(&includes,son);
  }

  if (search_depend(&m->inc_queue,son)==NULL)
  {
    depend = make_depend(son);
    ENQUEUE(&m->inc_queue,depend);
  }
}
/*}}}*/
/*{{{  void parse_module (module *m)*/
void parse_module (module *m)
{
  char linebuf[buflen];
  int line = 0; FILE *fp; 

  /*{{{  open file*/
  if (m->source==NULL)
  {
    if (m->type == file_occ || m->type == file_cfg)
      fprintf(stderr,"Warning: Cannot build makefile for source file, ignoring %s\n",m->name);
    return;
  }
  
  if ((fp = fopen_file (m->source, NULL)) == NULL)
  {
    fprintf(stderr,"Error-%s(%d): Could not find file, %s will be a leaf module\n", __FILE__,__LINE__,m->name);
  
    if (m->source != m->name) free (m->source);
    m->source = NULL;
  }
  /*}}}*/

  if (noisy) printf("Parsing module %s%s\n",m->path,m->source);
  
  /*{{{  implicit libraries*/
  if (intrinsics)
    process_use (m, intrinsics_lib_string, line);
  #ifdef ADD_SPSERVER
  if (serve && m->type == file_exe)
    process_use (m, serve_lib_string, line);
  #endif
  /*}}}*/
  
  /*{{{  parse lines of file */
  while (!feof(fp))
  {
    char *p, *s = NULL;
  
    if (fgetstring (linebuf,buflen,fp,occam_comment,&line)==NULL)
      break ;
  
    p = skip_blanks (linebuf);
  
    if (strstr(p,use_string) == p)
      /*{{{  match USE*/
      {
        p = skip_blanks(p + use_size);
      
        if ((s = match_string (p)) == NULL)
          fprintf(stderr,"Warning: Could not extract filename from USE on line %d of %s%s\n",line,m->path,m->source);
        else 
          process_use (m, s, line);
      }
      /*}}}*/
    else if (strstr(p,include_string) == p)
      /*{{{  match INCLUDE*/
      {
        p = skip_blanks(p + include_size);
      
        if ((s = match_string (p)) == NULL)
          fprintf(stderr,"Warning: Could not extract filename from INCLUDE on line %d of %s%s\n",line,m->path,m->source);
        else 
          process_inc (m, s);
      }
      /*}}}*/
  
  }
  /*}}}*/

  fclose(fp);
}  
/*}}}*/
/*}}}*/
/*{{{  template handling*/
static FILE *fp_tmpl;

static int template_indent = 0;
static int template_position = 0;

/*{{{  void generate_space (void)*/
void generate_space (void)
{
  fputc(' ',fp_out);
  template_position++;
}
/*}}}*/
/*{{{  void generate_slash (void)*/
void generate_slash (void)
{
  while (template_position <= output_width)
    generate_space();
    
  fputc('\\',fp_out);
  template_position++;
}
/*}}}*/
/*{{{  void generate_newline (void)*/
void generate_newline (void)
{
  int i;
  
  fputc('\n',fp_out);

  for (i=0;i<template_indent;i++)
    generate_space();

  template_position=template_indent;
}
/*}}}*/
/*{{{  void generate_tab (void)*/
void generate_tab (void)
{
  int i;
  
  fputc('\t',fp_out);

  template_position = template_indent = 8;
}
/*}}}*/
/*{{{  void generate_empty_rule (char *p1, ...)*/
void generate_empty_rule (char *p1, ...)
{
  va_list ap;
  char *s;

  fprintf(fp_out,"%s",p1);
  va_start (ap,p1);
  while ((s = (char*) va_arg(ap,char*)) != NULL)
    fprintf(fp_out,"%s",s);
  va_end(ap);

  fprintf(fp_out, ":\n\n");
  template_position=0;
  template_indent=0;
}
/*}}}*/
/*{{{  void generate_open_variable (char *p1, ...)*/
void generate_open_variable (char *p1, ...)
{
  va_list ap;
  char *s;

  template_position += fprintf(fp_out,"%s",p1);
  va_start (ap,p1);
  while ((s = (char*) va_arg(ap,char*)) != NULL)
    template_position += fprintf(fp_out,"%s",s);
  va_end(ap);

  template_position += fprintf(fp_out,"=");
  template_indent   = template_position;
}
/*}}}*/
/*{{{  void generate_string (char *name)*/
void generate_string (char *name)
{
  if (strlen(name)+template_position > output_width)
  {
    generate_slash();
    generate_newline();
  }
    
  template_position += fprintf(fp_out,"%s",name);
}
/*}}}*/
/*{{{  void generate_strings (char *p1, ...)*/
void generate_strings (char *p1, ...)
{
  va_list ap;
  int length=0;
  char *s;

  length += strlen(p1);
  va_start (ap,p1);
  while ((s = (char*) va_arg(ap,char*)) != NULL)
    length += strlen(s);
  va_end(ap);

  if (length+template_position > output_width)
  {
    generate_slash();
    generate_newline();
  }

  template_position += fprintf(fp_out,"%s",p1);

  va_start (ap,p1);
  while ((s = (char*) va_arg(ap,char*)) != NULL)
    template_position += fprintf(fp_out,"%s",s);
  va_end(ap);
}
/*}}}*/
/*{{{  void generate_close_variable (void)*/
void generate_close_variable (void)
{
  template_indent=0;
  generate_newline();
}
/*}}}*/

/*}}}*/

/*{{{  void generate_liu_file(module *module)*/
void generate_liu_file(module *mod)
{
  char *fn_liu;
  FILE *fp_liu;
  depend *depend = TOP_QUEUE(&mod->link_queue);
  
  fn_liu = form_extended_name (mod->root, other_ext[file_liu]);
  unlink(fn_liu);

  if (depend == NULL) return;
  
  if ((fp_liu = fopen (fn_liu,"w")) == NULL)
  {
    fprintf(stderr,"Cannot open output file %s, writing to stdout\n",fn_liu);
    fp_liu = stdout;
  }

  while(depend != NULL)
  {
    module *mod=depend->module;

    if (mod->type == file_lib && mod->source == NULL)
      fprintf(fp_liu,"%s%s%s\n",mod->root,dot_string,file_ext[file_lib]);

    depend = depend->next;
  }
  fclose (fp_liu);
}
  
/*}}}*/
/*{{{  void generate_depend_strings(dqueue *dq, file_hype type,bool inc_libs)*/
void generate_depend_strings(dqueue *dq, file_hype type,bool inc_libs)
{
  depend *depend = TOP_QUEUE(dq);

  while(depend != NULL)
  {
    module *mod = depend->module;
    file_hype ext=mod->type;

    if (mod->type == file_lib)
    {
      if (inc_libs)
      {
        if (type==file_exe)
        {
          ext = file_a;
          generate_strings(mod->path,"lib",mod->root,dot_string,file_ext[ext],space_string,NULL);
        }
        else
        {
          if (type==file_o) ext = file_hdr;
          generate_strings(mod->path,mod->root,dot_string,file_ext[ext],space_string,NULL);
        }
      }
    }
    else if (mod->type == file_t)
    {
      if (type==file_exe) ext = file_o;
      else if (type==file_o) ext = file_hdr;
    
      generate_strings(mod->path,mod->root,dot_string,file_ext[ext],space_string,NULL);
    }
    else if (mod->exp == from_include && type==file_pgm)
    {
      generate_strings(mod->path,mod->root,dot_string,file_ext[ext],space_string,NULL);
    }
    else
      fprintf(stderr,"Oops, internal error - gen-dep-str(%s,%s)\n",file_ext[type],file_ext[mod->type]);
  
    depend = depend->next;
  }
}
/*}}}*/
/*{{{  int template_find (char *rule)*/
int template_find (char *rule)
{
  char line_in[buflen];
  int line=0;
  bool found = false;
  bool braket = false;
  
  fseek(fp_tmpl,0L,0);
#ifdef DEBUG
  fprintf(stderr,"Looking for template matching '%s' .. ",rule);
#endif
  
  while(true)
  {
    char *cp = line_in;
    if (fgetstring(line_in,buflen,fp_tmpl,hash_comment,&line) == NULL) break;
    
    switch(line_in[0])
    {
      case '{':
        braket=true;
#ifdef DEBUG
        if (found) fprintf(stderr,"Found\n");
#endif
        if (found) return true;
        break;
      case '}':
        braket=false;
        break;
      default:
        if (!braket)
        {
          int o = strlen(cp);
        
          while(o > 1 && line_in[o-1] <= ' ') line_in[--o] = 0;
          
          if (strcasecmp(line_in,rule)==0) found=true;
        }
        break;
    }
  }
#ifdef DEBUG
  fprintf(stderr,"NOT found\n");
#endif
  return false;
}
/*}}}*/
/*{{{  void template_copy (module *mod)*/
void template_copy (module *mod)
{
  char line_in[buflen];
  int line=0;
  
  while(!feof(fp_tmpl))
  {
    char *cp = line_in;
    if (fgetstring(line_in,buflen,fp_tmpl,hash_comment,&line) == NULL) break;
  
    if (line_in[0] == '}') break;

    while (*cp != NUL && *cp != '\n')
    {
      if (mod != NULL && strlen(cp) > 5 && strncmp(cp,"<FILE>",6)==0)
      {
        cp += 6;
        fprintf(fp_out,"%s",mod->root);
      }
      else
        fputc(*cp++,fp_out);
    }
    fprintf(fp_out,"\n");
  }
 
  fputc('\n',fp_out);
}
/*}}}*/
/*{{{  void generate_rule (file_hype type, module *mod)*/
void generate_rule (file_hype type, module *mod)
{
  char *rule = tmpl_ext[type];
  
  if (template_find(rule)) template_copy(mod);
  else fprintf(stderr,"Error-Unable to find makefile rule for %s\n",rule);
}
/*}}}*/
/*{{{  void create_makefile(char *name,char *hdr, int argc, char **argv)*/
void create_makefile(char *name,char *hdr, int argc, char **argv)
{
  int i;
#ifdef DEBUG
  printf("Opening file %s\n",name);
#endif

  if ((fp_out = fopen (name,"w")) == NULL)
  {
    fprintf(stderr,"Cannot open output file %s, writing to stdout\n",name);
    fp_out = stdout;
  }

  fprintf(fp_out, "# Makefile for %s produced by omakef version %s\n\n",
          hdr, VERSION);
  fprintf(fp_out,"%s%s\n",arch_macro,arch_string);

  generate_options (argc,argv);
  for(i = 0; i < 10; ++i)
  {
    char buff[10];

    strcpy(buff,"HEADER");
    if (i) { buff[6] = '0'+i; buff[7] = 0; }
    if (template_find(buff)) template_copy(NULL);
  }
  generate_defines (argc,argv);
}
/*}}}*/
/*{{{  void generate_module(module *m,file_hype type, bool clean)*/
void generate_module(module *m,file_hype type, bool clean)
{
  if (m->source == NULL) return;
  
  switch(type)
  {
    case file_exe:
      if (make_type != gen_trans)
      {
        if (clean) generate_strings(m->root,space_string,NULL);
        else
        {
          generate_open_variable(m->root,ldd_dep_string,NULL);
          generate_depend_strings(&m->link_queue,file_exe,true);
          generate_close_variable();
          generate_rule(type,m);
        }
      }
      generate_module(m,file_o,clean);
      break;

    case file_o:
      if (make_type != gen_trans)
      {
        if (clean) generate_strings(m->root,dot_string,file_ext[file_o],space_string,NULL);
        else
        {
          generate_open_variable(m->root,hdr_dep_string,NULL);
          generate_depend_strings(&m->use_queue,file_o,true);
          generate_close_variable();
          generate_rule(type,m);
        }
      }
      generate_module(m,m->type==file_exe?file_pgm:file_mod,clean);
      break;

    case file_pgm:
      if (make_type != gen_comp)
      {
        if (clean) generate_strings(m->root,dot_string,file_ext[file_pgm],space_string,NULL);
        else
        {
          generate_open_variable(m->root,occ_dep_string,NULL);
          generate_depend_strings(&m->use_queue,file_pgm,true);
          generate_depend_strings(&m->inc_queue,file_pgm,true);
          generate_close_variable();
          generate_rule(type,m);
        }
      }
      break;

    case file_mod:
      if (make_type != gen_comp)
      {
        if (clean) generate_strings(m->root,dot_string,file_ext[file_mod],space_string,NULL);
        else
        {
          generate_open_variable(m->root,occ_dep_string,NULL);
          generate_depend_strings(&m->use_queue,file_pgm,true);
          generate_depend_strings(&m->inc_queue,file_pgm,true);
          generate_close_variable();
          generate_rule(type,m);
        }
      }
      break;
    
    case file_t:
      generate_module(m,file_o,clean);
      generate_module(m,file_hdr,clean);
      if (make_type != gen_comp)
      {
        if (clean) generate_strings(m->root,dot_string,file_ext[file_t],space_string,NULL);
        else
        {
          generate_rule(type,m);
        }
      }
      break;

    case file_hdr:
      if (make_type != gen_trans)
      {
        if (clean) generate_strings(m->root,dot_string,file_ext[file_hdr],space_string,NULL);
        else
        {
          generate_open_variable(m->root,lhd_dep_string,NULL);
          generate_depend_strings(&m->link_queue,file_o,false);
          generate_close_variable();
          generate_rule(type,m);
        }
      }
      break;    

    case file_lib:
      if (m->exp == from_command)
      {
        if (make_type != gen_trans)
        {
          if (clean) generate_strings(m->root,dot_string,file_ext[file_lib],space_string,NULL);
          else
          {
            generate_open_variable(m->root,lbb_dep_string,NULL);
            generate_depend_strings(&m->link_queue,file_lib,false);
            generate_close_variable();
            generate_rule(type,m);
          }
        }
        if (!clean) generate_liu_file(m);
        generate_module(m,file_a,clean);
        generate_module(m,file_t,clean);
      }
      break;    

    case file_a:
      if (m->exp == from_command)
      {
        if (make_type != gen_trans)
        {
          if (clean) generate_strings(m->root,dot_string,file_ext[file_a],space_string,NULL);
          else
          {
            generate_open_variable(m->root,ldd_dep_string,NULL);
            generate_depend_strings(&m->use_queue,file_exe,false);
            generate_close_variable();
            generate_rule(type,m);
          }
        }
      }
      break;    

    default:
      break;
  }
  if (m->type == type && !clean) fprintf(fp_out,"\n\n");
}

    
/*}}}*/
/*{{{  void generate_clean_rule(module *m)*/
void generate_clean_rule(module *m)
{
  generate_newline();

  generate_strings("clean:",NULL); generate_newline();
  generate_tab(); generate_string("rm -f ");
  while(m)
  {
    generate_module(m,m->type,true);
    m = m->next;
  }
  generate_close_variable();
}
  
  
/*}}}*/
/*{{{  int main (int argc, char **argv)*/
int main (int argc, char **argv)
{
  char *target;
  module *top;
  if (default_dir == NULL)
    /*{{{  get default libs directory*/
    {
      char *env = getenv (env_spoc);
      if (env == NULL)
        env=DEF_ENV;
      default_dir = malloc_check (strlen(env)+strlen(dir_lib)+strlen(arch_string)+4);
      strcpy(default_dir,env);
      strcat(default_dir,sep_string);
      strcat(default_dir,dir_lib);
      strcat(default_dir,sep_string);
      strcat(default_dir,arch_string);
      strcat(default_dir,sep_string);
    }
    /*}}}*/
  target = parse_command (argc, argv);
  top = make_module(target,from_command) ;

  INIT_QUEUE(&modules);
  INIT_QUEUE(&pending);
  INIT_QUEUE(&includes);
  INIT_QUEUE(&complete);
  
  ENQUEUE(&pending,top) ;
  while(!EMPTY_QUEUE(&pending))
  {
    module *module = TOP_QUEUE(&pending);

#ifdef DEBUG
    printf("\nPROCESSING: %s %s\n",module->path,module->name);
#endif
    
    parse_module(module);
    DEQUEUE_TOP(&pending);
    ENQUEUE(&modules,module)
  }

  form_link_queue(top,LENGTH_QUEUE(&modules));
  
  if ((fp_tmpl = fopenenv(template_name,"r",env_osearch,default_dir,NULL)) == NULL)
  {
    fprintf(stderr,"Error: Could not open template file %s\n",template_name);
    exit(EXIT_ERR);
  }

  if (noisy) display_modules(&modules);
  
  {
    char *tmp;
    if (top->type==file_exe)
      tmp = top->name;
    else
      tmp = extract_root(top->name);
    switch(make_type)
    {
      default:        create_makefile(fn_out = form_extended_name (tmp, "mkf"),top->name,argc,argv); break;
      case gen_comp:  create_makefile(fn_out = form_extended_name (tmp, "mkc"),top->name,argc,argv); break;
      case gen_trans: create_makefile(fn_out = form_extended_name (tmp, "mkt"),top->name,argc,argv); break;
    }
  }
  
  while(!EMPTY_QUEUE(&modules))
  {
    module *module = TOP_QUEUE(&modules);

    if (module->source != NULL)
    {
      generate_open_variable(module->root,NULL);
      generate_strings(module->path,module->root,NULL);
      generate_close_variable();
    }
    
    generate_module(module,module->type,false);
    DEQUEUE_TOP(&modules);
    ENQUEUE(&complete,module)
  }

  generate_clean_rule(TOP_QUEUE(&complete));
    
  fclose(fp_tmpl);
  fclose(fp_out);

  return(0);
}
/*}}}*/
