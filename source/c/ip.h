/* $Id: ip.h,v 1.1.1.3 1995/10/30 13:58:22 spoc Exp $ */

/*{{{  File banner*/
/*@(#)=====================================================*\
||@(#)  Project : GPMIMD ESPRIT P5404
||@(#)  Authors : Mark Debbage, X Fu, Mark Hill and Sean Wykes
||@(#)            University of Southampton
||  
||@(#)    Title : Omega Test Header Module
||@(#)   System : occam 2 c
||@(#) Filename : ip.h
||@(#)  Version : 1.3
||@(#)     Date : 30/10/95
\*@(#)====================================================*/
/*}}}*/

/* This is based on Omega version 3.2.3 */

#ifndef Already_Included_IP
#define Already_Included_IP 1

#include <stdio.h>

#define maxVars 38
#define maxGEQs 150
#define maxEQs 27


typedef int EqnKey;

typedef struct _eqn {
    EqnKey  key;
    int     touched;
    int     color;
    int     coef[maxVars + 1];
} *Eqn;

#define headerWords 3

typedef struct _problem {
    int     _nVars, _numEQs, _numGEQs,_safeVars,_numSUBs;
    int  hashVersion;
    int variablesInitialized;
    int variablesFreed;
    int     _var[maxVars+2];
    int     forwardingAddress[maxVars+2];
    char *(*_getVarName)(unsigned int var, void *args);
    void *_getVarNameArgs;
    struct _eqn _GEQs[maxGEQs];
    struct _eqn _EQs[maxEQs];
    struct _eqn _SUBs[maxVars+1];
} Problem;



#define UNKNOWN 2
#define SIMPLIFY 3
#define posInfinity (0x7ffffff)
#define negInfinity (-0x7ffffff)
#define red 1
#define black 0

#define eqnncpy(e1,e2,s) {int *p00,*q00,*r00; p00 = (int *)(e1); q00 = (int *)(e2); r00 = &p00[headerWords+1+s]; while(p00 < r00) *p00++ = *q00++; }
#define eqncpy(e1,e2) eqnncpy(e1,e2,nVars)
#define eqnnzero(e,s) {int *p00,*r00; p00 = (int *)(e); r00 = &p00[headerWords+1+(s)]; while(p00 < r00) *p00++ = 0;}
#define eqnzero(e) eqnnzero(e,nVars)

#define intDiv(a,b) ((1024 * b + a)/b - 1024)
#define intMod(a,b) ((a)-(b)*intDiv(a,b))


#define singleVarGEQ(e,nV) ((e).key != 0 && -maxVars <= (e).key && (e).key <= maxVars)


extern void  initializeOmega();

extern void initializeProblem(Problem *);
extern void problemcpy(Problem *, Problem *);
extern void  printProblem(Problem *);
extern void  printRedEquations(Problem *);
extern void  prettyPrintProblem(Problem *);
extern int  simplifyProblem(Problem *);
extern int  simplifyApproximate(Problem *);
extern void unprotectVariable(Problem *, int var);
extern void negateGEQ(Problem *, int);


/* set extra to 0 for normal use */
extern void printEqn (Problem *p, Eqn e, int is_geq, int extra);
extern void sprintEqn (char *str, Problem *p, Eqn e, int is_geq, int extra);

/*
   Return 1 if red equations constrain the set of possible solutions.
   We assume that there are solutions to the black equations by themselves,
   so if there is no solution to the combined problem, we return 1.
 */
extern int hasRedEquations(Problem * problemPtr, bool expensive);

extern int eliminateRedundant (Problem *problemPtr, bool expensive);
extern void eliminateRed (Problem *problemPtr, bool eliminateAll);

/* constrainVariableSign also unprotects var & simplifies the problem */
extern int
constrainVariableSign(Problem *, int color, int var, int sign); 

/* constrainVariableValue adds an EQ that makes variable var have
   value "value", even if variable i has been substituted out */
extern void 
constrainVariableValue(Problem *problemPtr, int color, int var, int value);

extern int 
queryVariable(Problem *, int var, int *lowerBound, int *upperBound);

extern int
queryVariableSigns(Problem *, int, int, int, int, int, 
int, 
bool *, 
int*);

extern int
queryVariableBounds(Problem * problemPtr, int i, int *l, int *u);

extern int solve(Problem *problemPtr, int desiredResult);

extern void setOutputFile(FILE *file);
     /* set "file" to the file to which the output of printProblem should go */

extern int reduceWithSubs;

extern int omegaPrintResult;
 /* set to non-zero to have constrainVariableSign and simplifyProblem
    print the resulting simplified problems */

extern int firstCheckForRedundantEquations;

extern void (*whenReduced)();

extern void noProcedure();

#endif

