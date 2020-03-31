# ifndef yyOccamOut
# define yyOccamOut

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"

/* line 21 "/home/hpcc/SPOC/spoc.1.3/source/gmd/occam.trafo" */

  extern void WriteStrRef(tStringRef r) ;
  extern void OutputOccam(char *, tTree);


extern void (* OccamOut_Exit) ();

extern void CodeType ARGS((tTree yyP33));
extern void Code ARGS((tTree yyP1));

extern void BeginOccamOut ();
extern void CloseOccamOut ();

# endif
