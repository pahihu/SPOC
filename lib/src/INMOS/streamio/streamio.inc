-- streamio.inc
-- Copyright 1989 INMOS Limited
-- Updated to match TDS3 strmhdr list; 4-Feb-91 SRH
VAL st.max.string.size IS 256 :
VAL ft.terminated   IS  -8 : -- used to terminate a keystream
VAL ft.number.error IS -11 :
PROTOCOL KS IS INT:
PROTOCOL SS
  CASE
    st.reset
    st.up
    st.down
    st.left
    st.right
    st.goto; INT32; INT32
    st.ins.char; BYTE
    st.del.char
    st.out.string; INT32::[]BYTE
    st.clear.eol
    st.clear.eos
    st.ins.line
    st.del.line
    st.beep
    st.spare
    st.terminate
    st.help
    st.initialise
    st.out.byte; BYTE
    st.out.int; INT32
    st.key.raw
    st.key.cooked
    st.release
    st.claim
    st.endstream
    st.set.poll; INT32
:
