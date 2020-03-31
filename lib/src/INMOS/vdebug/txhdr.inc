--{{{  LIB txhdr
--{{{  Links / priorities
VAL no.of.links   IS 4 :
VAL bits.per.link IS 2 :  -- how many bits you can fit a link number into

VAL no.priorities IS 2 :
VAL hi.priority   IS 0 :
VAL lo.priority   IS 1 :

VAL priority.bit  IS #1 :
VAL priority.mask IS BITNOT priority.bit :
--}}} 
--{{{  my.bpw
VAL my.bpw IS ((INT ((MOSTNEG INT) =  #8000)) * 2) +
              ((INT ((MOSTNEG INT) <> #8000)) * 4) :
--}}} 
--{{{  Breakpoint slots
-- The slots just above MemStart are used to control breakpoints
-- On T425, T801, T805 etc, MemStart is #70 (28 words)

-- The next words are used as follows:
-- MemStart + 0 : Hi pri Wptr
-- MemStart + 1 : Hi pri Iptr
-- MemStart + 2 : Lo pri Wptr
-- MemStart + 3 : Lo pri Iptr

VAL  breakslot.Wptr.offset IS 0 :
VAL  breakslot.Iptr.offset IS 1 :
--}}} 
--{{{  Below Memstart locations
VAL TPtrOffset0 IS  9 :
VAL TPtrOffset1 IS 10 :
--}}} 
--{{{  Below Workspace locations
VAL Iptr.s    IS -1 :
VAL Link.s    IS -2 :
VAL Pointer.s IS -3 :
--}}} 
--{{{  ALT states (values of Wptr + Pointer.s)
VAL NotProcess.p IS  MOSTNEG INT :
VAL Enabling.p   IS (MOSTNEG INT) + 1 :
VAL Waiting.p    IS (MOSTNEG INT) + 2 :
VAL Ready.p      IS (MOSTNEG INT) + 3 :
--}}} 
--}}} 
