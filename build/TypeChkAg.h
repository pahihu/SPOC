# ifndef yyTypeChkAg
# define yyTypeChkAg

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# include "Tree.h"

/* line 650 "" */
 
  #include "TypeChk.h" 
  #include "Cout.h"


extern void TypeChkAg ARGS((tTree yyt));
extern void BeginTypeChkAg ();
extern void CloseTypeChkAg ();

# endif
