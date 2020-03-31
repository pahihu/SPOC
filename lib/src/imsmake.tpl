#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : Additional Omakef rules for spserver support
# @(#)   System : Occam 2 C
# @(#) Filename : imsmake.tpl
# @(#)  Version : 1.2
# @(#)     Date : 2/21/94
# @(#)====================================================
#}}}
#Omakef template extension to link with spserver and INMOS libraries
HEADER1
{
XOCCOPT=-s
XCCOPT=-DUSE_NONBLOCKING_SOCKETS
XCL-DEP=$(SPOC)/lib/$(ARCH)/libspserver.a
}

