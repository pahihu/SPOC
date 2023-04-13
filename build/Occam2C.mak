#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Makefile for Occam 2 C binaries
# @(#)   System : Occam 2 C
# @(#) Filename : Occam2C.mak
# @(#)  Version : 1.3
# @(#)     Date : 4/4/97
# @(#)====================================================
#}}}

#{{{  architecture and compiler
S=/
ARCH=darwin
CC=gcc $(CCARCH)
CFLAGS=-Wno-return-type $(CCOPTIONS) -DFULL -I. -I$(CSRC) -I$(GMDTOOLS)$(S)c -DYYDEBUG=1 -DSUPPORT_INT64 -DARCH=\"$(ARCH)\"
MKDIR=mkdir -p
#}}}
#{{{  environment
GMDTOOLS=$(SPOC)$(S)gmdtools$(S)reuse

BIN=$(SPOC)$(S)bin$(S)$(ARCH)
SRC=$(SPOC)$(S)source
LIB=$(SPOC)$(S)lib$(S)$(ARCH)
LIBSRC=$(SPOC)$(S)lib$(S)src

CSRC=$(SRC)$(S)c

OMEGAOPTS=-fno-builtin  -DExit=exit -DeliminateRedundantConstraints=1\
          -DreduceWithSubstitutions=0 -DverifySimplifications=0

GFLAGS=-s -G$@

OBJECTS=Lexer.o ParserDrv.o Symbol.o Positions.o Errors.o Parser.o Cout.o Tree.o TreeFix.o \
        OccamOut.o TypeChk.o TypeChkAg.o PreTypeChkAg.o Transform.o ParUsage.o ParUsageAg.o CoutAg.o Omega.o ip.o Fopenenv.o
TREE.H=Tree.h $(CSRC)$(S)Symbol.h $(CSRC)$(S)Scanner.h $(CSRC)$(S)Positions.h $(CSRC)$(S)Errors.h Tree.c $(CSRC)$(S)Omega.h ParUsage.h ARCH

#}}}

#{{{  Standard rules
.KEEP_STATE:

.SUFFIXES:

#}}}

all: occ2c omakef libfiles


#{{{  top level rules
occ2c:		$(BIN)$(S)occ2c

omakef:		$(BIN)$(S)omakef

libfiles:	$(LIB)$(S)Occam2C.c $(BIN)$(S)occam.gdb $(BIN)$(S)odebug \
                $(LIB)$(S)Debug.h $(LIB)$(S)Occam2C.h

#}}}
#{{{  binary rules
$(BIN)$(S)occ2c: $(OBJECTS) $(GMDTOOLS)$(S)$(ARCH)$(S)libreuse.a
	@if [ ! -d $(BIN) ]; then $(MKDIR) $(BIN); fi;
	@echo "char *VersionDate=\"`date`\";char *VersionName=\"$(VERSION)\";" > Version.c
	$(CC) $(CFLAGS) Version.c -c -o Version.o
	$(CC) $(CFLAGS) $(OBJECTS) Version.o $(GMDTOOLS)$(S)$(ARCH)$(S)libreuse.a -o $(BIN)$(S)occ2c -lm

$(BIN)$(S)omakef: Omakef.o Fopenenv.o
	@if [ ! -d $(BIN) ]; then $(MKDIR) $(BIN); fi;
	$(CC) $(CFLAGS) Omakef.o Fopenenv.o -o $(BIN)$(S)omakef

$(BIN)$(S)occam.gdb: $(LIBSRC)$(S)occam.gdb
	@if [ ! -d $(BIN) ]; then $(MKDIR) $(BIN); fi;
	cp  $(LIBSRC)$(S)occam.gdb $(BIN)

$(BIN)$(S)odebug: $(LIBSRC)$(S)odebug
	@if [ ! -d $(BIN) ]; then $(MKDIR) $(BIN); fi;
	cp $(LIBSRC)$(S)odebug $(BIN); chmod ugo+x $(BIN)$(S)odebug

#}}}
#{{{  library/template rules
$(LIB)$(S)Debug.h: $(LIBSRC)$(S)Debug.h
	@if [ ! -d $(LIB) ]; then $(MKDIR) $(LIB); fi;
	cp $(LIBSRC)$(S)Debug.h $(LIB)

$(LIB)$(S)Occam2C.h: $(LIBSRC)$(S)Occam2C.h
	@if [ ! -d $(LIB) ]; then $(MKDIR) $(LIB); fi;
	@rm -rf $(LIB)$(S)Occam2C.h
	cp $(LIBSRC)$(S)Occam2C.h $(LIB)

$(LIB)$(S)Occam2C.c: $(LIBSRC)$(S)Occam2C.c
	@if [ ! -d $(LIB) ]; then $(MKDIR) $(LIB); fi;
	@rm -rf $(LIB)$(S)Occam2C.c
	cp $(LIBSRC)$(S)Occam2C.c $(LIB)

#}}}
#{{{  reusable module rules
$(GMDTOOLS)$(S)$(ARCH)$(S)libreuse.a:
	@if [ ! -d $(GMDTOOLS)$(S)$(ARCH) ]; then $(MKDIR) $(GMDTOOLS)$(S)$(ARCH); fi;
	( cd $(GMDTOOLS)$(S)c; make clean; make CC="$(CC)"; cp libreuse.a ..$(S)$(ARCH) )
#}}}
#{{{  object dependencies of generated C
CoutAg.o:       CoutAg.h CoutAg.c $(TREE.H)
	$(COMPILE.c) CoutAg.c -o CoutAg.o
Cout.o:         Cout.h Cout.c $(TREE.H) Transform.h TypeChk.h
	$(COMPILE.c) Cout.c -o Cout.o
Transform.o:	Transform.h Transform.c $(TREE.H)
	$(COMPILE.c) Transform.c -o Transform.o
TypeChkAg.o:	TypeChkAg.h TypeChkAg.c $(TREE.H) TypeChk.h TypeChk.c ParUsage.h Cout.h
	$(COMPILE.c) TypeChkAg.c -o TypeChkAg.o
PreTypeChkAg.o:	PreTypeChkAg.h PreTypeChkAg.c $(TREE.H) TypeChk.h TypeChk.c
	$(COMPILE.c) PreTypeChkAg.c -o PreTypeChkAg.o
TypeChk.o:      TypeChk.h TypeChk.c $(TREE.H)
	$(COMPILE.c) TypeChk.c -o TypeChk.o
OccamOut.o:     OccamOut.h OccamOut.c $(TREE.H)
	$(COMPILE.c) OccamOut.c -o OccamOut.o
ParUsageAg.o:   ParUsageAg.h ParUsageAg.c $(TREE.H) ParUsage.c ParUsage.h
	$(COMPILE.c) ParUsageAg.c -o ParUsageAg.o
ParUsage.o:     ParUsage.c ParUsage.h $(TREE.H)
	$(COMPILE.c) ParUsage.c -o ParUsage.o
Parser.o:       Parser.h Parser.c $(CSRC)$(S)Scanner.h $(TREE.H) TreeFix.h
	$(COMPILE.c) Parser.c -o Parser.o
Tree.o:         Tree.c $(TREE.H)
	$(COMPILE.c) Tree.c -o Tree.o
TreeFix.o:	TreeFix.h TreeFix.c $(TREE.H)
	$(COMPILE.c) TreeFix.c -o TreeFix.o
#}}}
#{{{  Omakef
Omakef.o: $(CSRC)$(S)Omakef.c $(CSRC)$(S)Fopenenv.h ARCH
	$(COMPILE.c) $(CSRC)$(S)Omakef.c -o Omakef.o

Fopenenv.o: $(CSRC)$(S)Fopenenv.c $(CSRC)$(S)Fopenenv.h ARCH
	$(COMPILE.c) $(CSRC)$(S)Fopenenv.c -o Fopenenv.o
#}}}
#{{{  Lexer MODULE
Lexer.o:        KeyTable.h TokenDefs.h $(TREE.H)  $(CSRC)$(S)Lexer.c $(CSRC)$(S)Fopenenv.h
	$(COMPILE.c) $(CSRC)$(S)Lexer.c -o Lexer.o

#}}}
#{{{  Positions MODULE
Positions.o:	$(CSRC)$(S)Positions.h  $(CSRC)$(S)Positions.c ARCH
	$(COMPILE.c) $(CSRC)$(S)Positions.c -o Positions.o
#}}}
#{{{  Errors MODULE
Errors.o:	$(CSRC)$(S)Errors.h $(CSRC)$(S)Positions.h  $(CSRC)$(S)Errors.c $(CSRC)$(S)Scanner.h ARCH
	$(COMPILE.c) $(CSRC)$(S)Errors.c -o Errors.o
#}}}
#{{{  ParserDrv MODULE
ParserDrv.o:    Parser.h $(TREE.H) OccamOut.h TypeChkAg.h Cout.h ParUsage.h ParUsageAg.h CoutAg.h $(CSRC)$(S)ParserDrv.c
	$(COMPILE.c) $(DRVOPTS) $(CSRC)$(S)ParserDrv.c -o ParserDrv.o
#}}}
#{{{  Symbol MODULE
Symbol.o:	$(TREE.H) OccamOut.h $(CSRC)$(S)Symbol.c
	$(COMPILE.c) $(CSRC)$(S)Symbol.c -o Symbol.o

Symbol:         $(CSRC)$(S)Symbol.c $(CSRC)$(S)Symbol.h ARCH
		$(CC) $(CFLAGS) $(CSRC)$(S)Symbol.c -DTEST $(GMDTOOLS)$(S)libreuse.a -o Symbol

#}}}
#{{{  omega test MODULES
Omega.o: $(TREE.H)  $(CSRC)$(S)Omega.c $(CSRC)$(S)ip.h
	$(COMPILE.c) $(CSRC)$(S)Omega.c -o Omega.o
ip.o:	$(CSRC)$(S)ip.h $(CSRC)$(S)flags.h $(CSRC)$(S)portable.h $(CSRC)$(S)ip.c ARCH
	$(COMPILE.c) $(OMEGAOPTS) -o ip.o $(CSRC)$(S)ip.c
#}}}
#{{{  clean RULE
gmdclean: clean
	rm -f *.c *.h core *.w extoccam.P extoccam.TS occam.P occam.TS occam.lalr

clean:  cleano
	rm -Rf $(BIN) $(LIB)
	rm -f  _Debug ARCH
	rm -Rf $(GMDTOOLS)$(S)$(ARCH)
	cd $(GMDTOOLS)$(S)c; make clean

cleano: 
	rm -f *.o
#}}}
