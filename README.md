Changes
=======

* able to build on macOS Mojave 64bit using clang
* ported GMDTOOLS runtime library to 64bit (the runtime is 32bit though)


SPOC : Southampton's Portable Occam Compiler, Version 1.3, 4/4/97
M. Debbage, X Fu, M. Hill, D. Nicole, S. Wykes  University Of Southampton, ESPRIT GPMIMD P5404

README for SPOC : Southampton's Portable Occam Compiler
=======================================================

Denis Nicole	dan@uk.ac.soton.ecs
Mark Debbage	md@uk.ac.srf.pact
Mark Hill	mbh@uk.ac.srf.pact

Copyright of the software and documentation produced 
by the authors is retained by the University of Southampton.

The installation procedure for this system is documented
in the file "./docs/install.doc". A postscript version
of this document is also distributed (./docs/install.ps).

If a file ./docs/latest.doc exists, it will give details of any
last-minute changes to the system or bugs which have occurred since the
installation and user-guides were was last updated.

Use of the system is documented in the User Guide, this is distributed
solely as postscript (./docs/guide.ps).

ACKNOWLEDGEMENTS
================

The authors would like to acknowledge contributions from
the following individuals and organisations:

WILLIAM PUGH (pugh@cs.umd.edu) et al. at the Department of
Computer Science, UNIVERSITY OF MARYLAND, College Park for
the Omega test.  They have produced both papers and public 
domain software to implement the test which determines
whether there is an integer solution to an arbitrary
set of linear equalities and inequalities.  This is applied
to perform range, alias and parallel usage checking to
array accesses performed with arbitrary linear replicator 
expressions. More information about the test can be found
in the article "A practical algorithm for exact array
dependence analysis. William Pugh. Comms of the ACM. Aug 1992.
Vol 35/8.". Their software is available via anonymous ftp
from ftp.cs.umd.edu in directory pub/omega and is released 
with the following disclaimer:

 "The implementation of the Omega test and extensions to tiny have
  been done by a number of people at the University of Maryland:
        William Pugh
        Dave Wonnacott
        Udayan Borkar
        Wayne Kelly
        Jerry Sobieski
        Vadim Maslov

  This software is public domain, and no legal restrictions of any
  kind whatsoever are placed on uses of it). You may do whatever you want 
  with it, and no guarantees of any kind are made about its performance or 
  lack of errors. You can copy it, use it, incorporate it or even sell it.
  We request that any research or products that make use of this software 
  acknowledge that use and that you keep us informed of your use.

  Please send mail to omega@cs.umd.edu if you wish to be added to a mailing
  list for people interesting in using this software. We will notify
  people on the mail list of bug fixes and new releases.

  Also send mail to omega@cs.umd.edu if you have any trouble installing
  the software, bug reports or questions. 

  Our work on this software has been supported by NSF grants CCR-8908900 and 
  CCR-9157384 and by a Packard Fellowship, as well as being based on
  Michael Wolfe's original implementation of tiny."

---------------------

JOSEF GROSCH and associates at GMD,KARLSRUHE for his compiler construction
tool box. The following tools have been used to help construct the
compiler:
  bnf  : Parser generator
  cg   : Command grammar tool for generating abstract syntax trees and
          attribute grammars.
  puma : Tree Pattern matching tool for transformation and code generation.
We used Version 9208 of the tools whose Copyright is retained by GMD (1989, 
1990, 1991, 1992 ). Direct requests, comments, questions, and 
error reports should be addressed to:
   Josef Grosch
   GMD Forschungsstelle
   Vincenz-Priessnitz-Str. 1
   D-7500 Karlsruhe 1
   Phone: +721-662226
   Email: grosch@karlsruhe.gmd.de
The compiler construction tool box is distributed via anonymous ftp in
compressed tar format from ftp.gmd.de in the directory gmd/cocktail.

---------------------

DAVID BECKETT and PETER WELCH at the UNIVERSITY OF KENT AT CANTERBURY,
the former for his manual pages to the standard Occam libraries, the latter
for suggesting the scheduling benchmark distributed as part of the
examples (./examples/comstime). The manual pages are 
Copyright (C) 1993 David  Beckett,  University of Kent at Canterbury 
and are also available via anonymous
ftp from unix.hensa.ac.uk. in pub/parallel/documents/ukc.

---------------------

The LEXER is loosely based on the Occam 2 lexer developed by
PETER POLKINGHORNE. His disclaimer is reproduced here:

 "This work is in the public domain.
  It was written by Peter Polkinghorne in 1986 & 1989 at
  GEC Hirst Research Centre, Wembley, England.
  No liability is accepted or warranty given by the Author,
  still less my employers."
 
---------------------

TONY DEBLING at INMOS LIMITED has given kind permission for us
to distribute the standard OCCAM TOOLSET include files hostio.inc and mathvals.inc.
Copyright is retained by INMOS LTD.

---------------------

The INMOS library sources are now included in lib/src/INMOS from the NMOS D7202a-oc 
oc compiler product sources published on 
	http://www.hensa.ac.uk/parallel/occam/compilers/inmos/oc/index.html
as part of the UK HENSA archive.   The conditions of use are:
 
                         INMOS Source Releases
  
  CONDITIONS OF USE:
  
     *************************************************************************
     *                                                                       *
     * These copyright sources are made available for research purposes.     *
     * No support will be provided from INMOS for their use.  They may not   *
     * be incorporated into any commercial products without explicit written *
     * permission, and agreed licensing terms, from INMOS in Bristol.        *
     *                                                                       *
     *************************************************************************
  
  SUPPORT/WARRANTY/BUG FIXES
  
    There is no warranty for this set of files and INMOS especially
  are not interested in bug fixes.  If you have something to contribute to
  add to these files, please contact me.
  
  
  THANKS
  
    Thanks go to Tony Debling and Michael Poole for arranging the release
  of these files to the Transputer and occam community.

---------------------

Oyvind Teig of Autronica, Trondheim, Norway funded the development of version 1.3
of SPOC and of the Windows95 port.
