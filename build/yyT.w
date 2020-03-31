# define begintStringRef(a)	
# define equaltStringRef(a, b)	a = b
# define begintIdent(a)		
# define equaltIdent(a, b)	a = b
# define begintPosition(a)		
# define equaltPosition(a, b)	Positions.Compare (a, b) = 0
# define begintSymbol(a)
# define equaltSymbol(a, b)	yyIsEqual (a, b)
# define begintLang(a)
# define equaltLang(a, b)	yyIsEqual (a, b)
# define beginint(a)
# define equalint(a, b)	yyIsEqual (a, b)
# define begintValue(a)
# define equaltValue(a, b)	yyIsEqual (a, b)
# define beginbool(a)
# define equalbool(a, b)	yyIsEqual (a, b)
# define begintParType(a)
# define equaltParType(a, b)	yyIsEqual (a, b)
# define begintTree(a)	a := NIL;
# define equaltTree(a, b)	Tree.IsEqualTree (a, b)
