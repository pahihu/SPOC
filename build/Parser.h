# ifndef yyParser
# define yyParser

/* $Id: Parser.h,v 2.1 1992/08/07 15:28:42 grosch rel $ */



# ifdef yacc_interface
# define Parser			yyparse
# define yyInitStackSize	YYMAXDEPTH
# endif

extern	char *	Parser_TokenName [];

extern	int	Parser	();
extern	void	CloseParser	();

# endif
