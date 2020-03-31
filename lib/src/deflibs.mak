#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Top level makefile portion for all libraries
# @(#)   System : Occam 2 C
# @(#) Filename : deflibs.mak
# @(#)  Version : 1.2
# @(#)     Date : 1/14/94
# @(#)====================================================
#}}}
deflibs: Intrinsics.lib convert.lib snglmath.lib dblmath.lib mathvals.inc spserver.lib

#{{{  Intrinsics
Intrinsics.lib: Intrinsics/Intrinsics.lib
	cp Intrinsics/Intrinsics.lib Intrinsics.lib
	cp Intrinsics/Intrinsics.hdr Intrinsics.hdr
	cp Intrinsics/Intrinsics.a libIntrinsics.a

Intrinsics/Intrinsics.lib: Intrinsics/Intrinsics.mkf
	cd Intrinsics ; make -f Intrinsics.mkf

Intrinsics/Intrinsics.mkf: Intrinsics/Intrinsics.occ
	cd Intrinsics ; $(OMAKEF) Intrinsics.lib -i -d -mn $(OCCOPTIONS) 

#}}}

#{{{  convert
convert.lib: convert/convert.lib
	cp convert/convert.lib convert.lib
	cp convert/convert.hdr convert.hdr
	cp convert/convert.liu convert.liu
	cp convert/convert.a libconvert.a

convert/convert.lib: convert/convert.mkf Intrinsics.lib
	cd convert ; make -f convert.mkf

convert/convert.mkf: convert/convert.occ
	cd convert ; $(OMAKEF) convert.lib -d -mn $(OCCOPTIONS) 

#}}}

#{{{  snglmath
snglmath.lib: snglmath/snglmath.lib
	cp snglmath/snglmath.lib snglmath.lib
	cp snglmath/snglmath.hdr snglmath.hdr
	cp snglmath/snglmath.liu snglmath.liu
	cp snglmath/snglmath.a libsnglmath.a

snglmath/snglmath.lib: snglmath/snglmath.mkf Intrinsics.lib
	cd snglmath ; make -f snglmath.mkf

snglmath/snglmath.mkf: snglmath/snglmath.occ
	cd snglmath ; $(OMAKEF) snglmath.lib -d -mn $(OCCOPTIONS) 

#}}}

#{{{  dblmath
dblmath.lib: dblmath/dblmath.lib
	cp dblmath/dblmath.lib dblmath.lib
	cp dblmath/dblmath.hdr dblmath.hdr
	cp dblmath/dblmath.liu dblmath.liu
	cp dblmath/dblmath.a libdblmath.a

dblmath/dblmath.lib: dblmath/dblmath.mkf Intrinsics.lib
	cd dblmath ; make -f dblmath.mkf

dblmath/dblmath.mkf: dblmath/dblmath.occ
	cd dblmath ; $(OMAKEF) dblmath.lib -d -mn $(OCCOPTIONS) 

#}}}

#{{{  spserver
ISERVER=spserver/iserver.o spserver/serverc.o spserver/hostc.o spserver/filec.o

spserver.lib: spserver/spserver.lib $(ISERVER)
	cp spserver/spserver.lib spserver.lib
	cp spserver/spserver.hdr spserver.hdr
	cp spserver/spserver.liu spserver.liu
	ar r libspserver.a spserver/spserver.o $(ISERVER)
	ranlib libspserver.a

spserver/spserver.lib: spserver/spserver.mkf
	cd spserver ; make -f spserver.mkf

spserver/spserver.mkf: spserver/spserver.occ hostio.inc
	cd spserver ; $(OMAKEF) spserver.lib -d $(OCCOPTIONS)

CC=gcc
CFLAGS=-DSUN -DBOARD_ID=B011 -D$(ARCH) -c

spserver/iserver.o: $(LIBSRC)/spserver/iserver.c $(LIBSRC)/spserver/inmos.h $(LIBSRC)/spserver/iserver.h
	( cd spserver; $(CC) $(LIBSRC)/spserver/iserver.c -o iserver.o $(CFLAGS); )
spserver/serverc.o: $(LIBSRC)/spserver/serverc.c $(LIBSRC)/spserver/inmos.h $(LIBSRC)/spserver/iserver.h $(LIBSRC)/spserver/pack.h
	( cd spserver; $(CC) $(LIBSRC)/spserver/serverc.c -o serverc.o $(CFLAGS); )
spserver/hostc.o: $(LIBSRC)/spserver/hostc.c $(LIBSRC)/spserver/udplink.h $(LIBSRC)/spserver/inmos.h $(LIBSRC)/spserver/iserver.h $(LIBSRC)/spserver/pack.h
	( cd spserver; $(CC) $(LIBSRC)/spserver/hostc.c -o hostc.o $(CFLAGS); )
spserver/filec.o: $(LIBSRC)/spserver/filec.c $(LIBSRC)/spserver/inmos.h $(LIBSRC)/spserver/iserver.h $(LIBSRC)/spserver/pack.h
	( cd spserver; $(CC) $(LIBSRC)/spserver/filec.c -o filec.o $(CFLAGS); )
#}}}


