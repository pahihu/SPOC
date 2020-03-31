--{{{  constants (single)
--{{{  REALOP codes
VAL Add IS 0 :
VAL Sub IS 1 :
VAL Mul IS 2 :
VAL Div IS 3 :
--}}}
--{{{  UNPACK type constants
VAL nan    IS 3 :
VAL inf    IS 2 :
VAL normal IS 1 :
VAL nought IS 0 :
--}}}
--{{{  REAL32 implementation constants
VAL BitsPerWord IS  32(INT) :
VAL RealShift   IS   8(INT) :
VAL RealXcess   IS 127(INT) :
VAL RealExpSh   IS  23(INT) :
--}}}
--{{{  NaNs and Inf
VAL REAL32 undefined.NaN RETYPES #7F800010(INT) :
VAL REAL32 unstable.NaN  RETYPES #7F800008(INT) :
VAL REAL32 inexact.NaN   RETYPES #7F800004(INT) :
VAL RealInf                   IS #7F800000(INT) :
VAL REAL32 RInf RETYPES RealInf :
--}}}
--{{{  small constants
VAL Zero IS 0.0(REAL32) :
VAL One  IS 1.0(REAL32) :
VAL Two  IS 2.0(REAL32) :
VAL INT Half  RETYPES 0.5(REAL32) :
VAL INT unity RETYPES 1.0(REAL32) :
--}}}
--{{{  trigonometric constants
VAL REAL32 InvPi RETYPES #3EA2F983(INT)  :
-- Pi.part.1 + Pi.part.2 = Pi to extra precision
VAL REAL32 Pi.part.1 RETYPES #40490000(INT) :
VAL REAL32 Pi.part.2 RETYPES #3A7DAA22(INT) :
VAL REAL32 Pi RETYPES #40490FDB(INT) :
VAL REAL32 PiBy2 RETYPES #3FC90FDB(INT) :
VAL Nearly.PiBy2 IS #3FC907DB(INT) :  -- Pi/2 - Eps
--}}}
--}}}
--{{{  constants (double)
--{{{  REAL64 implementation constants
VAL BitsPerI64  IS   64(INT) :
VAL DRealShift  IS   11(INT) :
VAL DRealXcess  IS 1023(INT) :
VAL DRealExpSh  IS   52(INT) :
VAL DBitsInFrac IS   53(INT) :
--}}}
--{{{  NaNs and Inf
VAL REAL64 Dundefined.NaN RETYPES #7FF0000200000000(INT64) :
VAL REAL64 Dunstable.NaN  RETYPES #7FF0000100000000(INT64) :
VAL REAL64 Dinexact.NaN   RETYPES #7FF0000080000000(INT64) :
VAL DRealInf                   IS #7FF0000000000000(INT64) :
VAL REAL64 DRInf RETYPES DRealInf :
--}}}
--{{{  trigonometric constants
VAL REAL64 DPiBy6 RETYPES #3FE0C152382D7366(INT64) :
VAL Nearly.DPiBy2 IS #3FF921FB50442D18(INT64) : -- DPiBy2 - Eps
VAL REAL64 DPiBy2 RETYPES #3FF921FB54442D18(INT64) :
VAL REAL64 DPi    RETYPES #400921FB54442D18(INT64) :
VAL REAL64 DInvPi RETYPES #3FD45F306DC9C883(INT64) :
-- Pi.part.1 + Pi.part.2 = Pi to extra precision
VAL REAL64 DPi.part.1 RETYPES #4009220000000000(INT64) :
VAL REAL64 DPi.part.2 RETYPES #BEE2AEEF4B9EE59E(INT64) :
--}}}
--{{{  small constants
VAL DZero IS 0.0(REAL64) :
VAL DOne  IS 1.0(REAL64) :
VAL DTwo  IS 2.0(REAL64) :
VAL Dzero IS 0(INT64) :
VAL Done  IS 1(INT64) :
VAL INT64 Dunity RETYPES 1.0(REAL64) :
VAL INT64 DHalf  RETYPES 0.5(REAL64) :
--}}}
--}}}
