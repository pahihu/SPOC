-- hostio.inc
-- Copyright 1989 INMOS Limited
-- updated for iserver v1.42, apart from buffer size 5-June-90 SRH
--{{{  SP protocol
PROTOCOL SP IS INT16::[]BYTE :
--}}}
--{{{  Command tags
-- values up to 127 are reserved for use by INMOS
--{{{  File command tags
VAL sp.open.tag     IS 10(BYTE) :
VAL sp.close.tag    IS 11(BYTE) :
VAL sp.read.tag     IS 12(BYTE) :
VAL sp.write.tag    IS 13(BYTE) :
VAL sp.gets.tag     IS 14(BYTE) :
VAL sp.puts.tag     IS 15(BYTE) :
VAL sp.flush.tag    IS 16(BYTE) :
VAL sp.seek.tag     IS 17(BYTE) :
VAL sp.tell.tag     IS 18(BYTE) :
VAL sp.eof.tag      IS 19(BYTE) :
VAL sp.ferror.tag   IS 20(BYTE) :
VAL sp.remove.tag   IS 21(BYTE) :
VAL sp.rename.tag   IS 22(BYTE) :
VAL sp.getblock.tag IS 23(BYTE) :
VAL sp.putblock.tag IS 24(BYTE) :
VAL sp.isatty.tag   IS 25(BYTE) :
--}}}  File command tags
--{{{  Host command tags
VAL sp.getkey.tag  IS 30(BYTE) :
VAL sp.pollkey.tag IS 31(BYTE) :
VAL sp.getenv.tag  IS 32(BYTE) :
VAL sp.time.tag    IS 33(BYTE) :
VAL sp.system.tag  IS 34(BYTE) :
VAL sp.exit.tag    IS 35(BYTE) :
--}}}  Host command tags
--{{{  Server command tags
VAL sp.commandline.tag IS 40(BYTE) :
VAL sp.core.tag        IS 41(BYTE) :
VAL sp.version.tag     IS 42(BYTE) :
--}}}  Server command tags
--{{{  OS specific command tags
-- These OS specific tags will be followed by another tag indicating
-- which OS specific function is required

VAL sp.DOS.tag    IS 50(BYTE) :
VAL sp.HELIOS.tag IS 51(BYTE) :
VAL sp.VMS.tag    IS 52(BYTE) :
VAL sp.SUNOS.tag  IS 53(BYTE) :
--}}}  OS specific command tags
--}}}  Command tags
--{{{  Packet and buffer Sizes
VAL sp.max.packet.size IS 512 : -- bytes transferred, includes length & data
VAL sp.min.packet.size IS   8 : -- bytes transferred, includes length & data

VAL sp.max.packet.data.size IS sp.max.packet.size - 2 :  -- INT16 length
VAL sp.min.packet.data.size IS sp.min.packet.size - 2 :  -- INT16 length

--{{{  individual command maxima
VAL sp.max.openname.size      IS sp.max.packet.data.size - 5 : -- 5 bytes extra
VAL sp.max.readbuffer.size    IS sp.max.packet.data.size - 3 : -- 3 bytes extra
-- ditto for gets
VAL sp.max.writebuffer.size   IS sp.max.packet.data.size - 7 : -- 7 bytes extra
-- ditto for puts
VAL sp.max.removename.size    IS sp.max.packet.data.size - 3 : -- 3 bytes extra
VAL sp.max.renamename.size    IS sp.max.packet.data.size - 5 : -- 5 bytes extra
VAL sp.max.getenvname.size    IS sp.max.packet.data.size - 3 : -- 3 bytes extra
VAL sp.max.systemcommand.size IS sp.max.packet.data.size - 3 : -- 3 bytes extra
VAL sp.max.corerequest.size   IS sp.max.packet.data.size - 3 : -- 3 bytes extra

VAL sp.max.buffer.size IS sp.max.writebuffer.size :  -- smaller of read & write
--}}}  individual command maxima
--}}}  Packet and buffer Sizes
--{{{  Result values         (spr.)

VAL spr.ok                  IS   0(BYTE) : -- success

VAL spr.not.implemented     IS   1(BYTE) :
VAL spr.bad.name            IS   2(BYTE) : -- filename is null
VAL spr.bad.type            IS   3(BYTE) : -- open file type is incorrect
VAL spr.bad.mode            IS   4(BYTE) : -- open file mode is incorrect
VAL spr.invalid.streamid    IS   5(BYTE) : -- never opened that streamid
VAL spr.bad.stream.use      IS   6(BYTE) : -- reading an output file, or vice versa
VAL spr.buffer.overflow     IS   7(BYTE) : -- buffer too small for required data
VAL spr.bad.packet.size     IS   8(BYTE) : -- data too big or small for packet
VAL spr.bad.origin          IS   9(BYTE) : -- seek origin is incorrect
VAL spr.full.name.too.short IS  10(BYTE) : -- a truncation of a filename would be
                                           -- required
VAL spr.notok               IS 127(BYTE) : -- a general fail result

-- anything 128 or above is a server dependent 'failure' result
VAL spr.operation.failed    IS 128(BYTE) : -- general failure
VAL spr.failed.operation    IS 129(BYTE) : -- identical in meaning to
                                           -- spr.operation.failed due
                                           -- to historical accident

--}}}  Result values         (spr.)
--{{{  Predefined streams    (spid.)
VAL spid.stdin  IS 0(INT32) :
VAL spid.stdout IS 1(INT32) :
VAL spid.stderr IS 2(INT32) :
--}}}  Predefined streams    (spid.)
--{{{  Open types            (spt.)
VAL spt.binary IS 1(BYTE) :
VAL spt.text   IS 2(BYTE) :
--}}}
--{{{  Open modes            (spm.)
VAL spm.input           IS 1(BYTE) :
VAL spm.output          IS 2(BYTE) :
VAL spm.append          IS 3(BYTE) :
VAL spm.existing.update IS 4(BYTE) :
VAL spm.new.update      IS 5(BYTE) :
VAL spm.append.update   IS 6(BYTE) :
--}}}  Open modes            (spm.)
--{{{  Status values         (sps.)
VAL sps.success IS  999999999(INT32) :
VAL sps.failure IS -999999999(INT32) :
--}}}
--{{{  Seek origins          (spo.)
VAL spo.start   IS 1(INT) :
VAL spo.current IS 2(INT) :
VAL spo.end     IS 3(INT) :
--}}}
--{{{  Version information   (sph., spo., spb.)
--{{{  Host types            (sph.)
-- values up to 127 are reserved for use by INMOS
VAL sph.PC         IS 1(BYTE) :
VAL sph.NECPC      IS 2(BYTE) :
VAL sph.VAX        IS 3(BYTE) :
VAL sph.SUN3       IS 4(BYTE) :
VAL sph.S370       IS 5(BYTE) :
VAL sph.BOX.SUN4   IS 6(BYTE) :
VAL sph.BOX.SUN386 IS 7(BYTE) :
VAL sph.BOX.APOLLO IS 8(BYTE) :
--}}}  Host types            (sph.)
--{{{  OS types              (spo.)
VAL spo.DOS    IS 1(BYTE) :
VAL spo.HELIOS IS 2(BYTE) :
VAL spo.VMS    IS 3(BYTE) :
VAL spo.SUNOS  IS 4(BYTE) :
VAL spo.CMS    IS 5(BYTE) :
-- values up to 127 are reserved for use by INMOS
--}}}  OS types              (spo.)
--{{{  Interface Board types (spb.)
-- This determines the interface between the link and the host
VAL spb.B004    IS 1(BYTE) :
VAL spb.B008    IS 2(BYTE) :
VAL spb.B010    IS 3(BYTE) :
VAL spb.B011    IS 4(BYTE) :
VAL spb.B014    IS 5(BYTE) :
VAL spb.DRX11   IS 6(BYTE) :
VAL spb.QT0     IS 7(BYTE) :
VAL spb.B015    IS 8(BYTE) :
VAL spb.IBMCAT  IS 9(BYTE) :
VAL spb.B016    IS 10(BYTE) :
VAL spb.UDPLINK IS 11(BYTE) :
-- values up to 127 are reserved for use by INMOS
--}}}  Interface Board types (spb.)
--}}}  Version information   (sph., spo., spb.)
--{{{  Command line
VAL sp.short.commandline IS BYTE 0 :  -- remove  server's own arguments
VAL sp.whole.commandline IS BYTE 1 :  -- include server's own arguments

-- values for so.parse.commandline indicate whether
-- an option requires a following parameter
VAL spopt.never  IS 0 :
VAL spopt.maybe  IS 1 :
VAL spopt.always IS 2 :
--}}}  Command line
--{{{  Time string and date lengths
VAL so.time.string.len IS 19 :  -- enough for "HH:MM:SS DD/MM/YYYY"
VAL so.date.len        IS  6 :  -- enough for DDMMYY (as integers)
--}}}
--{{{  Temp filename length
VAL so.temp.filename.length IS 6 :  -- six chars will work on anything!
--}}}
