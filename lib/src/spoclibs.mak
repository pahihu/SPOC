#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Top level makefile portion for SPOC hostio/string libs
# @(#)   System : Occam 2 C
# @(#) Filename : spoclibs.mak
# @(#)  Version : 1.2
# @(#)     Date : 1/14/94
# @(#)====================================================
#}}}
spoclibs: hostio.lib string.lib

#{{{  hostio
hostio.lib: hostio/hostio.lib
	cp hostio/hostio.lib hostio.lib
	cp hostio/hostio.hdr hostio.hdr
	cp hostio/hostio.liu hostio.liu
	cp hostio/hostio.a libhostio.a

hostio/hostio.lib: hostio/hostio.mkf convert.lib Intrinsics.lib
	cd hostio ; make -f hostio.mkf

hostio/hostio.mkf: hostio/hostio.occ
	cd hostio ; $(OMAKEF) hostio.lib  -d -mn $(OCCOPTIONS)

#}}}

#{{{  string
string.lib: string/string.lib
	cp string/string.lib string.lib
	cp string/string.hdr string.hdr
	cp string/string.liu string.liu
	cp string/string.a libstring.a

string/string.lib: string/string.mkf convert.lib Intrinsics.lib
	cd string ; make -f string.mkf

string/string.mkf: string/string.occ
	cd string ; $(OMAKEF) string.lib -d -mn $(OCCOPTIONS)

#}}}

