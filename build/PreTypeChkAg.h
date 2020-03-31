# ifndef yyPreTypeChkAg
# define yyPreTypeChkAg

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# include "Tree.h"

/* line 574 "" */

  #include "TypeChk.h"


extern void PreTypeChkAg ARGS((tTree yyt));
extern void BeginPreTypeChkAg ();
extern void ClosePreTypeChkAg ();

# endif
