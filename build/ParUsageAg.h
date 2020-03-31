# ifndef yyParUsageAg
# define yyParUsageAg

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# include "Tree.h"


extern void ParUsageAg ARGS((tTree yyt));
extern void BeginParUsageAg ();
extern void CloseParUsageAg ();

# endif
