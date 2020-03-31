# Manual Pages for standard libraries (c) Dave Beckett, University of Kent 
# extensions for SPOC support (C) University of Southampton

INSTALL=install -c -m 644

MANDIR=..
SUFFIX=3l

TBMATHLIBLINKS  = snglmath.lib.$(SUFFIX) snglmath.$(SUFFIX)\
                  dblmath.lib.$(SUFFIX) dblmath.$(SUFFIX)

BASENAMES= hostio string convert tbmaths streamio
LIBRARIES= $(BASENAMES:%=$(MANDIR)/man3/%.lib.$(SUFFIX))
LINKS= $(BASENAMES:%=$(MANDIR)/man3/%.$(SUFFIX))
BINARIES= $(MANDIR)/man1/odebug.1 $(MANDIR)/man1/omakef.1 $(MANDIR)/man1/occ2c.1

all: install

install: $(LIBRARIES) $(BINARIES)

$(MANDIR)/man3/hostio.lib.$(SUFFIX): hostio-lib.man $(MANDIR)/man3
	rm -f $(MANDIR)/man3/hostio.lib.$(SUFFIX) $(MANDIR)/man3/hostio.$(SUFFIX)
	$(INSTALL) hostio-lib.man $(MANDIR)/man3/hostio.lib.$(SUFFIX)
	ln -s $(MANDIR)/man3/hostio.lib.$(SUFFIX) $(MANDIR)/man3/hostio.$(SUFFIX)

$(MANDIR)/man3/streamio.lib.$(SUFFIX): streamio-lib.man $(MANDIR)/man3
	rm -f $(MANDIR)/man3/streamio.lib.$(SUFFIX) $(MANDIR)/man3/streamio.$(SUFFIX)
	$(INSTALL) streamio-lib.man $(MANDIR)/man3/streamio.lib.$(SUFFIX)
	ln -s $(MANDIR)/man3/streamio.lib.$(SUFFIX) $(MANDIR)/man3/streamio.$(SUFFIX)

$(MANDIR)/man3/string.lib.$(SUFFIX): string-lib.man $(MANDIR)/man3
	rm -f $(MANDIR)/man3/string.lib.$(SUFFIX) $(MANDIR)/man3/string.$(SUFFIX)
	$(INSTALL) string-lib.man $(MANDIR)/man3/string.lib.$(SUFFIX)
	ln -s $(MANDIR)/man3/string.lib.$(SUFFIX) $(MANDIR)/man3/string.$(SUFFIX)

$(MANDIR)/man3/convert.lib.$(SUFFIX): convert-lib.man $(MANDIR)/man3
	rm -f $(MANDIR)/man3/convert.lib.$(SUFFIX) $(MANDIR)/man3/convert.$(SUFFIX)
	$(INSTALL) convert-lib.man $(MANDIR)/man3/convert.lib.$(SUFFIX)
	ln -s $(MANDIR)/man3/convert.lib.$(SUFFIX) $(MANDIR)/man3/convert.$(SUFFIX)

$(MANDIR)/man3/tbmaths.lib.$(SUFFIX): tbmaths-lib.man $(MANDIR)/man3
	rm -f $(MANDIR)/man3/tbmaths.lib.$(SUFFIX) $(MANDIR)/man3/tbmaths.$(SUFFIX)
	$(INSTALL) tbmaths-lib.man $(MANDIR)/man3/tbmaths.lib.$(SUFFIX)
	ln -s $(MANDIR)/man3/tbmaths.lib.$(SUFFIX) $(MANDIR)/man3/tbmaths.$(SUFFIX)
	for link in $(TBMATHLIBLINKS); do\
	  rm -f $(MANDIR)/man3/$$link;\
	  ln -s $(MANDIR)/man3/tbmaths.lib.$(SUFFIX) $(MANDIR)/man3/$$link; \
	done

clean:
	rm -f $(LIBRARIES)
	rm -f $(BINARIES)
	rm -f $(LINKS)
	for link in $(TBMATHLIBLINKS); do\
	  rm -f $(MANDIR)/man3/$$link;\
	done

$(MANDIR)/man1/odebug.1: odebug.man $(MANDIR)/man1
	rm -f $(MANDIR)/man1/odebug.1 
	$(INSTALL) odebug.man $(MANDIR)/man1/odebug.1

$(MANDIR)/man1/omakef.1: omakef.man $(MANDIR)/man1
	rm -f $(MANDIR)/man1/omakef.1 
	$(INSTALL) omakef.man $(MANDIR)/man1/omakef.1

$(MANDIR)/man1/occ2c.1: occ2c.man $(MANDIR)/man1
	rm -f $(MANDIR)/man1/occ2c.1 
	$(INSTALL) occ2c.man $(MANDIR)/man1/occ2c.1

$(MANDIR)/man3:
	$(INSTALL) -d -m 755 $(MANDIR)/man3

$(MANDIR)/man1:
	install -d -m 755 $(MANDIR)/man1
