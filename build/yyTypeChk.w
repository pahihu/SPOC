# define beginint(a)		
# define equalint(a, b)		a == b
# define begindouble(a)		
# define equaldouble(a, b)	a == b
# define beginbool(a)		
# define equalbool(a, b)	a == b
# define begintStringRef(a)	
# define equaltStringRef(a, b)	a == b
# define begintIdent(a)		
# define equaltIdent(a, b)	a == b
# define begintPosition(a)		
# define equaltPosition(a, b)	Compare (a, b) == 0
# define begintSymbol(a)
# define equaltSymbol(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintUsage(a)
# define equaltUsage(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintValue(a)
# define equaltValue(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintInt(a)
# define equaltInt(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintConstArray(a)
# define equaltConstArray(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintLang(a)
# define equaltLang(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintParType(a)
# define equaltParType(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintTree(a)	a = NULL;
# define equaltTree(a, b)	IsEqualTree (a, b)
