/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Flags header file for Pugh's Omega Test
||@(#)   System : occam 2 c
||@(#) Filename : flags.h
||@(#)  Version : 1.1
||@(#)     Date : 9/23/93
\*@(#)====================================================*/
/*}}}*/

/*
 * Global flag variables
 */

#if !defined(GLOB)
#define GLOB extern
#endif

GLOB int quiet;
GLOB int AllowComments;

GLOB int ivr_on;
GLOB int ivr_ForwSub;
GLOB int ivr_SubstReals;
GLOB int ivr_debug;

GLOB int ivr_ElimUnused;
GLOB int ivr_DepAnalysis;
GLOB int ivr_Assert;
GLOB int ivr_RepAffine;
GLOB int ivr_DefEntryClass;
GLOB int ivr_ShadowShadow;
GLOB int ivr_RemoveNoOp;

GLOB int makeReductionOps;
GLOB int doArrayExpn;
GLOB int repeatArrayExpn;
GLOB int doPrivatization;
GLOB int arrDefInOut;
GLOB int doEEdeps;
#if defined OMIT_DDS_FOR_TOPLEVEL
#define omitScalars 1
#else
GLOB int omitScalars;
#endif

GLOB int debugLevel;
GLOB int omegaPrintResult;
GLOB int printing_zap_gists;

GLOB int printAffine;
GLOB int printShadow;
GLOB int printPhis;

#if defined OMIT_DDS_FOR_TOPLEVEL
#define omitTopLevel 1
#else
GLOB int omitTopLevel;
#endif

GLOB char **Argv;
GLOB int   Argc;
