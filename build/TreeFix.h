# ifndef yyTreeFix
# define yyTreeFix

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"


extern void (* TreeFix_Exit) ();

extern tTree mArrayFromExprList ARGS((tTree yyP2, tTree yyP1));
extern tTree mTableElementFromExprList ARGS((tTree yyP5, tPosition yyP4, int yyP3));
extern tTree CatFieldList ARGS((tTree yyP7, tTree yyP6));

extern void BeginTreeFix ();
extern void CloseTreeFix ();

# endif
