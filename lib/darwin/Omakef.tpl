#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Omakef rule template file
# @(#)   System : Occam 2 C
# @(#) Filename : Omakef.tpl
# @(#)  Version : 1.4
# @(#)     Date : 2/15/94
# @(#)====================================================
#}}}
#Template file for Omakef Makefile generator

HEADER
{
OCCOPT.linux=
OCCOPT.sun4=-mi -me
OCCOPT.solaris=-mi -me

A=a
C=c
O=o
T=t
HDR=hdr
TMP=tmp
X=x
S=/
OCC=occ
LIB=lib
LBB=lbb
LDD=ldd

OCC2C=SPOC=$(SPOC) $(SPOC)/bin/$(ARCH)/occ2c
SPOCLIB=$(SPOC)/lib/$(ARCH)

OCC2C-DEP=$(SPOCLIB)/Occam2C.c

OCCOPT=$(OCCOPTIONS) $(OCCOPT.$(ARCH))
OCC2PGM=$(OCC2C) $(OCCOPT) $(XOCCOPT)
OCC2MOD=$(OCC2C) $(OCCOPT) -l

C2OBJ-DEP=$(SPOCLIB)/Occam2C.h
CCOPT=-I$(SPOCLIB) -DHOST=SUN4 -DOS=SUNOS -D$(ARCH) $(CCOPTIONS)
C2OBJ=gcc -m32 -c $(CCOPT) $(XCCOPT) -DUSESIGTIMER

C2PRO-DEP=$(SPOCLIB)/Occam2C.h
C2PRO=gcc -m32 $(CCOPT) -DGENPROTO

CLINK-DEP=$(XCL-DEP)
CLOPT=
CLLIB=-lm $(CLOPTIONS) $(XCLLIB)
CLINK=gcc -m32 $(CLOPT) 

INVOKE=exec
AR=ar r
RL=ranlib
COPY=cp
CAT=cat
RM=rm -f

GFLAGS=-s -G$@
.SUFFIXES:
.KEEP_STATE:
}

EXE
{
$(<FILE>): $(<FILE>).$(O) $(CLINK-DEP) $(<FILE>-LDD)
	$(CLINK) $(<FILE>).$(O) -o $(<FILE>) $(<FILE>-LDD) $(CLINK-DEP) $(CLLIB)
}

O
{
$(<FILE>).$(O): $(<FILE>).$(C) $(C2OBJ-DEP) $(<FILE>-HDR)
	$(C2OBJ) $(<FILE>).$(C) -o $(<FILE>).$(O)
}

MOD
{
$(<FILE>).$(C): $(<FILE>).$(OCC) $(<FILE>-DEP) $(OCC2C-DEP)
	$(OCC2MOD) $(<FILE>).$(OCC) -o $(<FILE>).$(C)
}

PGM
{
$(<FILE>).$(C): $(<FILE>).$(OCC) $(<FILE>-DEP) $(OCC2C-DEP)
	$(OCC2PGM) $(<FILE>).$(OCC) -o $(<FILE>).$(C)
}

HDR
{
$(<FILE>).$(HDR): $(<FILE>).$(C) $(<FILE>-HDR) $(<FILE>-LHD) $(C2PRO-DEP) 
	$(C2PRO) $(<FILE>).$(C) -o $(<FILE>).$(X)
	$(INVOKE) ./$(<FILE>).$(X) $(<FILE>-LHD) $(<FILE>).$(HDR)
	$(RM) $(<FILE>).$(X)
}

A
{
$(<FILE>).$(A): $(<FILE>).$(O) $(<FILE>-LDD)
	$(AR) $(<FILE>).$(A) $(<FILE>).$(O) $(<FILE>-LDD)
	$(RL) $(<FILE>).$(A)
}

T
{
$(<FILE>).$(T): $(<FILE>).$(C)
}

LIB
{
$(<FILE>).$(LIB): $(<FILE>).$(T) $(<FILE>).$(A) $(<FILE>-LBB) $(<FILE>).$(HDR)
	$(CAT) $(<FILE>).$(T) $(<FILE>-LBB) > $(<FILE>).$(LIB)
}
