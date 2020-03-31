# define beginint(a)		
# define closeint(a)		
# define readint(a)		(void) fscanf (yyf, "%d", & a);
# define writeint(a)		(void) fprintf (yyf, "%d", a);
# define getint(a)		yyGet ((char *) & a, sizeof (a));
# define putint(a)		yyPut ((char *) & a, sizeof (a));
# define copyint(a, b)		
# define equalint(a, b)		a == b
# define beginbool(a)		
# define closebool(a)		
# define readbool(a)		a = fgetc (yyf) == 'T';
# define writebool(a)		(void) fputc (a ? 'T' : 'F', yyf);
# define getbool(a)		yyGet ((char *) & a, sizeof (a));
# define putbool(a)		yyPut ((char *) & a, sizeof (a));
# define copybool(a, b)		
# define equalbool(a, b)	a == b
# define begintStringRef(a)	
# define closetStringRef(a)	
# define readtStringRef(a)	
# define writetStringRef(a)	WriteString (yyf, a);
# define gettStringRef(a)	
# define puttStringRef(a)	
# define copytStringRef(a, b)		
# define equaltStringRef(a, b)	a == b
# define begintIdent(a)		
# define closetIdent(a)		
# define readtIdent(a)		a = yyReadIdent ();
# define writetIdent(a)		WriteIdent (yyf, a);
# define gettIdent(a)		yyGetIdent (& a);
# define puttIdent(a)		yyPutIdent (a);
# define copytIdent(a, b)		
# define equaltIdent(a, b)	a == b
# define begintPosition(a)		
# define closetPosition(a)		
# define readtPosition(a)
# define writetPosition(a)	WritePosition (yyf, a);
# define gettPosition(a)		
# define puttPosition(a)		
# define copytPosition(a, b)		
# define equaltPosition(a, b)	Compare (a, b) == 0
# define begintSymbol(a)
# define closetSymbol(a)
# define readtSymbol(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetSymbol(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettSymbol(a)	yyGet ((char *) & a, sizeof (a));
# define puttSymbol(a)	yyPut ((char *) & a, sizeof (a));
# define copytSymbol(a, b)
# define equaltSymbol(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintValue(a)
# define closetValue(a)
# define readtValue(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetValue(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettValue(a)	yyGet ((char *) & a, sizeof (a));
# define puttValue(a)	yyPut ((char *) & a, sizeof (a));
# define copytValue(a, b)
# define equaltValue(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintParType(a)
# define closetParType(a)
# define readtParType(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetParType(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettParType(a)	yyGet ((char *) & a, sizeof (a));
# define puttParType(a)	yyPut ((char *) & a, sizeof (a));
# define copytParType(a, b)
# define equaltParType(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintLang(a)
# define closetLang(a)
# define readtLang(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetLang(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettLang(a)	yyGet ((char *) & a, sizeof (a));
# define puttLang(a)	yyPut ((char *) & a, sizeof (a));
# define copytLang(a, b)
# define equaltLang(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintType(a)
# define closetType(a)
# define readtType(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetType(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettType(a)	yyGet ((char *) & a, sizeof (a));
# define puttType(a)	yyPut ((char *) & a, sizeof (a));
# define copytType(a, b)
# define equaltType(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintConstArray(a)
# define closetConstArray(a)
# define readtConstArray(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetConstArray(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettConstArray(a)	yyGet ((char *) & a, sizeof (a));
# define puttConstArray(a)	yyPut ((char *) & a, sizeof (a));
# define copytConstArray(a, b)
# define equaltConstArray(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintUsage(a)
# define closetUsage(a)
# define readtUsage(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetUsage(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettUsage(a)	yyGet ((char *) & a, sizeof (a));
# define puttUsage(a)	yyPut ((char *) & a, sizeof (a));
# define copytUsage(a, b)
# define equaltUsage(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintChain(a)
# define closetChain(a)
# define readtChain(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetChain(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettChain(a)	yyGet ((char *) & a, sizeof (a));
# define puttChain(a)	yyPut ((char *) & a, sizeof (a));
# define copytChain(a, b)
# define equaltChain(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintFrame(a)
# define closetFrame(a)
# define readtFrame(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetFrame(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettFrame(a)	yyGet ((char *) & a, sizeof (a));
# define puttFrame(a)	yyPut ((char *) & a, sizeof (a));
# define copytFrame(a, b)
# define equaltFrame(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintChoiceList(a)
# define closetChoiceList(a)
# define readtChoiceList(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetChoiceList(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettChoiceList(a)	yyGet ((char *) & a, sizeof (a));
# define puttChoiceList(a)	yyPut ((char *) & a, sizeof (a));
# define copytChoiceList(a, b)
# define equaltChoiceList(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintIfType(a)
# define closetIfType(a)
# define readtIfType(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetIfType(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettIfType(a)	yyGet ((char *) & a, sizeof (a));
# define puttIfType(a)	yyPut ((char *) & a, sizeof (a));
# define copytIfType(a, b)
# define equaltIfType(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintParUsage(a)
# define closetParUsage(a)
# define readtParUsage(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writetParUsage(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define gettParUsage(a)	yyGet ((char *) & a, sizeof (a));
# define puttParUsage(a)	yyPut ((char *) & a, sizeof (a));
# define copytParUsage(a, b)
# define equaltParUsage(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define beginpLinExpr(a)
# define closepLinExpr(a)
# define readpLinExpr(a)	yyReadHex ((unsigned char *) & a, sizeof (a));
# define writepLinExpr(a)	yyWriteHex ((unsigned char *) & a, sizeof (a));
# define getpLinExpr(a)	yyGet ((char *) & a, sizeof (a));
# define putpLinExpr(a)	yyPut ((char *) & a, sizeof (a));
# define copypLinExpr(a, b)
# define equalpLinExpr(a, b)	memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# define begintTree(a)	a = NoTree;
# define closetTree(a)	yyChild = a; a = NoTree; yyReleaseTree (yyChild);
# define readtTree(a)	yyReadTree (a);
# define writetTree(a)	yyWriteTree (a);
# define gettTree(a)	yyGetTree (a);
# define puttTree(a)	yyPutTree (a);
# define copytTree(a, b)	yyCopyTree (b, & a);
# define equaltTree(a, b)	IsEqualTree (a, b)
