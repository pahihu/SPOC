-- ticks.inc
-- V1.0, 09/May/90
-- Copyright 1990 INMOS Limited
-- These values are not for the A revision of the T414 ( which is no longer
-- supported ).

-- these values are the rates at which the two priority clocks
-- increment on the transputer
VAL lo.ticks.per.second IS   15625 ( INT32 ) :
VAL hi.ticks.per.second IS 1000000 ( INT32 ) :


VAL lo.tick.in.micro.seconds IS 64 ( INT ) : --  1000000 / lo.ticks.per.second
VAL hi.tick.in.micro.seconds IS  1 ( INT ) : --  1000000 / hi.ticks.per.second
