-- msdos.inc
-- Copyright 1989 INMOS Limited
--{{{  dos command tags
VAL dos.send.block.tag     IS  0(BYTE) :
VAL dos.receive.block.tag  IS  1(BYTE) :
VAL dos.call.interrupt.tag IS  2(BYTE) :
VAL dos.read.regs.tag      IS  3(BYTE) :
VAL dos.port.write.tag     IS  4(BYTE) :
VAL dos.port.read.tag      IS  5(BYTE) :
--}}}
--{{{  call.interrupt register layout
VAL dos.interrupt.regs.size IS 40 :

VAL dos.interrupt.regs.ax   IS  0 :
VAL dos.interrupt.regs.bx   IS  4 :
VAL dos.interrupt.regs.cx   IS  8 :
VAL dos.interrupt.regs.dx   IS 12 :
VAL dos.interrupt.regs.di   IS 16 :
VAL dos.interrupt.regs.si   IS 20 :
VAL dos.interrupt.regs.cs   IS 24 :
VAL dos.interrupt.regs.ds   IS 28 :
VAL dos.interrupt.regs.es   IS 32 :
VAL dos.interrupt.regs.ss   IS 36 :
--}}}
--{{{  read.regs register layout
VAL dos.read.regs.size IS 16 :

VAL dos.read.regs.cs   IS  0 :
VAL dos.read.regs.ds   IS  4 :
VAL dos.read.regs.es   IS  8 :
VAL dos.read.regs.ss   IS 12 :
--}}}
--{{{  buffer sizes (These depend on sp.max.packet.data.size)
VAL dos.max.send.block.buffer.size    IS sp.max.packet.data.size - 8 :
VAL dos.max.receive.block.buffer.size IS sp.max.packet.data.size - 3 :

-- this is the smaller of send & receive
VAL dos.max.block.buffer.size IS dos.max.send.block.buffer.size :
--}}}
