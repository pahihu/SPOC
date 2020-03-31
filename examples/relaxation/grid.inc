--------------------------------------------------------------------------------
--     S.M.Wykes - Transputers and Scientific Programming Course              --
--                                                                            --
--  Assignment 2 - Equation solving using Simultaneous Overrelaxation         --
--                                                                            --
--       Phase 2 - Multi Transputer System                                    --
--                                                                            --
--        Module - Header File (Constants and Protocol Definitions)           --
--------------------------------------------------------------------------------

VAL GridDim IS 4 :                  -- Processor Array Side Length
VAL NumProcs IS GridDim * GridDim : -- Number of Processors
VAL MaxArray IS 100 :                -- Maximum array size (per worker)
VAL BigArray IS GridDim * MaxArray:
VAL ZERO IS 0.0(REAL32) :
VAL ONE IS 1.0(REAL32) :
VAL TWO IS 2.0(REAL32) :
VAL HALF IS 0.5(REAL32) :
VAL QTR IS 0.25(REAL32) :
VAL FOUR IS 4.0(REAL32) :
VAL MaxIterations IS 5000 :    -- Maximum iterations

VAL TOP IS 0:
VAL RIGHT IS 1:
VAL BOTTOM IS 2:
VAL LEFT IS 3:

PROTOCOL sor
  CASE
    mapping.packet; INT16; INT
                    -- Source Node; LinkIn
    initial.packet; INT; INT; REAL32; REAL32; REAL32
                    -- ArraySize, NumProcs; kappa; rjac; MinNorm
    residue.packet; INT16; REAL32
                    -- Source Node; Residue
    array.packet; INT16; INT16; INT; INT::[]REAL32
                    -- Dest Node; Source; Pos; []Elements
    swap.packet; INT::[]REAL32
                    -- Swap between nodes
    req.packet; INT16
                    -- Dest Node
    go.packet
    stop.packet
    kill.packet
    dead.packet
    debug.packet; INT16; INT; INT; INT

:
