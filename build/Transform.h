# ifndef yyTransform
# define yyTransform

# if defined __STDC__ | defined __cplusplus
# define ARGS(parameters)	parameters
# else
# define ARGS(parameters)	()
# endif

# ifndef bool
# define bool char
# endif

# include "Tree.h"


extern void (* Transform_Exit) ();

extern void Transform ARGS((tTree yyP1));

extern void BeginTransform ();
extern void CloseTransform ();

# endif
