# ifndef yyCoutAg
# define yyCoutAg

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# include "Tree.h"

/* line 776 "" */

  #include "Cout.h"


extern void CoutAg ARGS((tTree yyt));
extern void BeginCoutAg ();
extern void CloseCoutAg ();

# endif
