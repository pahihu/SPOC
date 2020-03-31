#{{{ File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill, Denis Nicole and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Makefile for Occam 2 C binaries
# @(#)   System : Occam 2 C
# @(#) Filename : Makefile
# @(#)  Version : 1.64
# @(#)     Date : 4/4/97
# @(#)====================================================
#}}}

#{{{  environment
VERSION=1.3
TARGET=all
#}}}

#{{{  architecure selection stuff
help:   
	@echo "Welcome to the SPOC build system" ;\
	echo "================================" ;\
	echo "" ;\
	if [ -r ./ARCH ]; then echo "  Current target architecture is `cat ARCH`" ; else echo "No target architecture selected" ; fi ;\
	echo "" ;\
	echo "A full build requires 3 invocations of make:" ;\
	echo "" ;\
	echo "  1. Select the target architecture for subsequent makes:" ;\
	echo "     > make select ARCH=<name> ";\
	echo "          predefined names are {linux solaris sun4 win95}" ;\
	echo " " ;\
	echo "  2. Build the compiler. Choose one of:" ;\
	echo "     > make all               : compile using GMD tools" ;\
	echo "     > make all-wotools       : compile from generated C files" ;\
	echo "     > make develop           : compile for debug" ;\
	echo "     > make develop-wotools   : compile for debug from generated C files" ;\
	echo "" ;\
	echo "  3. Install the libraries. Choose one of:" ;\
	echo "     > make lib-install       : install public domain libraries" ;\
	echo "     > make lib-install INMOS_SRC=<dir> : install proprietary libraries" ;\
	echo "         <dir> should locate the src of the Inmos occam libraries" ;\
	echo " " ;\
	echo "Other options: " ;\
	echo "     > make generate-c        : just regenerate C files" ;\
	echo "         useful when current architecture has no GMD toolkit installed" ;\
	echo "     > clean                  : tidy up" ;\
	echo "" ;\
	echo "  Once installed you will need to set SPOC=`dirname $(PWD)`" ;\
	echo "  and add `dirname $(PWD)`/bin/`cat ARCH` to your PATH" 

all:    ARCH generate-c
	@echo Compiling SPOC for `cat ARCH` ...
	make -f Occam2C.mak $(TARGET) VERSION=$(VERSION) ARCH=`cat ARCH` CCOPTIONS=-O

all-wotools: ARCH 
	@echo Compiling SPOC for `cat ARCH` without using GMD Compiler Toolkit....
	make -f Occam2C.mak $(TARGET) VERSION=$(VERSION) ARCH=`cat ARCH` CCOPTIONS=-O

develop: ARCH generate-c
	@echo Compiling SPOC for `cat ARCH` development ...
	make -f Occam2C.mak $(TARGET) VERSION=$(VERSION) ARCH=`cat ARCH` CCOPTIONS=-g DRVOPTS=-DCORE_DUMP

develop-wotools: ARCH 
	@echo Compiling SPOC for `cat ARCH` development ...
	make -f Occam2C.mak $(TARGET) VERSION=$(VERSION) ARCH=`cat ARCH` CCOPTIONS=-g DRVOPTS=-DCORE_DUMP

select:	ARCH 
	@Arch=`cat ARCH` ; \
	if [ `echo $(ARCH) | wc -w` != 0 ]; then \
	  if [ $(ARCH) != $$Arch ]; then echo $(ARCH) > ARCH; fi ; fi ; \
        if [ ! -d ../bin ]; then mkdir ../bin; fi; \
	if [ ! -d ../bin/$$Arch ]; then mkdir ../bin/$$Arch; fi; \
	if [ ! -d ../lib/$$Arch ]; then mkdir ../lib/$$Arch; fi; \
	echo "Selected architecture is `cat ARCH`" ; \
	echo ""

ARCH:
	if [ `echo $(ARCH) | wc -w` = 0 ]; then echo Use \"make select ARCH="<name>"\"  to target new architecture; exit 5; fi
	echo $(ARCH) > ARCH

clean:  
	make -f Occam2C.mak clean

gmdclean:
	make -f Occam2C.mak gmdclean

generate-c:
	@echo "Regenerating C files using GMD tools ..."
	make -f Gmd.mak

lib-install:
	@echo "Library installation procedure for architecture `cat ARCH`"
	@SPOC=`dirname $(PWD)` INMOS_SRC=$(INMOS_SRC) ARCH=`cat ARCH` \
	../lib/src/libinstall
	
#}}}
