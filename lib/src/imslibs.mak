#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Top level makefile portion for INMOS libraries
# @(#)   System : Occam 2 C
# @(#) Filename : imslibs.mak
# @(#)  Version : 1.2
# @(#)     Date : 1/14/94
# @(#)====================================================
#}}}
imslibs: hostio.lib streamio.lib string.lib

#{{{  hostio
hostio.lib: hostio/hostio.lib
	cp hostio/hostio.lib hostio.lib
	cp hostio/hostio.hdr hostio.hdr
	cp hostio/hostio.liu hostio.liu
	cp hostio/hostio.a libhostio.a

hostio/hostio.lib: hostio/hostio.mkf convert.lib Intrinsics.lib
	cd hostio ; make -f hostio.mkf

hostio/hostio.mkf: hostio/hostio.occ hostio/solib.lib
	cd hostio ; $(OMAKEF) hostio.lib $(OCCOPTIONS)

hostio/solib.lib: hostio/solib.mkf
	cd hostio ; make -f solib.mkf

hostio/solib.mkf: hostio/hostio.occ hostio/splib.lib
	cd hostio ; $(OMAKEF) solib.lib $(OCCOPTIONS) 

hostio/splib.lib: hostio/splib.mkf
	cd hostio ; make -f splib.mkf

hostio/splib.mkf: hostio/hostio.occ hostio/readwrit.lib
	cd hostio ; $(OMAKEF) splib.lib $(OCCOPTIONS) 

hostio/readwrit.lib: hostio/readwrit.mkf
	cd hostio ; make -f readwrit.mkf

hostio/readwrit.mkf: hostio/hostio.occ
	cd hostio ; $(OMAKEF) readwrit.lib $(OCCOPTIONS) 

#}}}

#{{{  streamio
streamio.lib: streamio/streamio.lib
	cp streamio/streamio.lib streamio.lib
	cp streamio/streamio.hdr streamio.hdr
	cp streamio/streamio.liu streamio.liu
	cp streamio/streamio.a libstreamio.a

streamio/streamio.lib: streamio/streamio.mkf hostio.lib convert.lib Intrinsics.lib
	cd streamio ; make -f streamio.mkf

streamio/streamio.mkf: streamio/streamio.occ streamio.inc
	cd streamio ; $(OMAKEF) streamio.lib -s $(OCCOPTIONS)

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
	cd string ; $(OMAKEF) string.lib -d $(OCCOPTIONS) 

#}}}
