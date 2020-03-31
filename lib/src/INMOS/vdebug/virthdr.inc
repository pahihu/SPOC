--{{{  LIB virthdr
--{{{  virtual channel control block
VAL virt.input.id.i       IS 0 :  -- must be 0 for ALT to work
VAL virt.input.count.i    IS 1 :
VAL virt.input.pointer.i  IS 2 :
VAL virt.input.restart.i  IS 3 :

VAL virt.output.id.i      IS 4 :
VAL virt.output.count.i   IS 5 :
VAL virt.output.pointer.i IS 6 :
VAL virt.output.restart.i IS 7 :

VAL virt.queue.link.i     IS 8 :
VAL virt.queue.flags.i    IS 9 :

VAL virt.buffer.index.i   IS 10 :

VAL virt.output.header.len.i   IS 11 :
VAL virt.output.header.start.i IS 12 :

--VAL virt.block.size IS 16 : -- need to change current user prog with this
VAL virt.block.size IS 14 : -- need to change current user prog with this

VAL virt.max.no.headers IS virt.block.size - virt.output.header.start.i :
                        -- including primary header

VAL virt.output.chan.offset.words IS virt.output.id.i     - virt.input.id.i :
VAL virt.restart.offset.words     IS virt.input.restart.i - virt.input.id.i :
--}}} 
--{{{  Array sizes
VAL virt.max.packet.size IS 64 :  -- 5, 16, 64
--}}} 
--{{{  PROTOCOL VIRTUAL.LINK.PACKET
PROTOCOL VIRTUAL.LINK.PACKET IS
  [6]INT :-- input.id.i; v.chan; len; adr; Wdesc; Restart
--}}} 
--{{{  Commands from Application to VLE
VAL av.input.message  IS 0 :
VAL av.output.message IS 1 :
VAL av.stop           IS 2 :
--}}} 
--}}} 
