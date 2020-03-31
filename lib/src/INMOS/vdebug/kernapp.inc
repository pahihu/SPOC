--{{{  LIB kernapp
--{{{  APPLICATION.PACKET protocol
PROTOCOL APPLICATION.PACKET IS [3]INT; INT::[]BYTE :
-- [command; Iptr; Wdesc]; len::data
--}}} 
--{{{  Application to kernel commands
VAL ak.message IS 0 :
VAL ak.error   IS 1 :
--}}} 
--{{{  Addresses of shared variables / channels    [Nigel]
--{{{  comment
-- breakpoint.slot.addresses are from MemStart to MemStart + 3 (inclusive)
-- On a T425 etc, MemStart is 28 words (it is also assumed there on T414)
-- Therefore, word offsets 28, 29, 30, 31  are used for breakpoints
-- The next word holds a word index to a pointer table.
-- On a 16 bit machine, MemStart is at 18 words, so the pointer is stored
-- at 22
-- 'my.bpw' is defined in library txhdr
--}}} 

--{{{  pointers to pointer table
--  [Nigel]    watch out coz pathologist needs the following, only
--  it needs them dynamicall

VAL kern.pt.ptr.offset.16bit IS 22 :    --  [Nigel]
VAL kern.pt.ptr.offset.32bit IS 32 :    --  [Nigel]
VAL kern.pt.ptr.offset IS ((INT (my.bpw = 2)) * kern.pt.ptr.offset.16bit) +
                          ((INT (my.bpw = 4)) * kern.pt.ptr.offset.32bit) :


--  'pt' is read as 'pointer.table'
-- The next word is maintained to be the bitwise negation of this word.
-- This provides a cheap test for running within the debugger.

VAL kern.pt.check.offset IS kern.pt.ptr.offset + 1 :
--}}} 

--{{{  indexes into ponter table (kern.pt stuff)
-- Here 'index' implies an index into a word array placed on memory
VAL kern.pt.app.sem.word.index   IS  0 :  -- Application's semaphores
VAL kern.pt.app.chan.word.index  IS  1 :  -- Application's shared channels
VAL kern.pt.neg.event.word.index IS  2 :  -- The 'hard' event information
VAL kern.pt.pos.event.word.index IS  3 :  -- The 'soft' event information
VAL kern.pt.pos.event.size       IS  4 :  -- The size of the soft array
VAL kern.pt.vle.sem.word.index   IS  5 :  -- Virtual link engine's semaphores
VAL kern.pt.vle.chan.word.index  IS  6 :  -- Virtual link engine's shared channels
VAL kern.pt.max.message.size     IS  7 :  -- Max size of DEBUG.MESSAGE
VAL kern.pt.vle.input.base       IS  8 :  -- Base address of input links
VAL kern.pt.vle.output.base      IS  9 :  -- Base address of output links

VAL kern.pt.size                 IS 10 :
--}}} 
--}}} 
--}}} 
