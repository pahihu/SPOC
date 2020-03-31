#{{{  File banner
# @(#)=====================================================
# @(#)  Project : GPMIMD ESPRIT P5404
# @(#)  Authors : Mark Debbage, Mark Hill and Sean Wykes
# @(#)            University of Southampton
#  
# @(#)    Title : GDB command file for occam debugging
# @(#)   System : Occam 2 C
# @(#) Filename : occam.gdb
# @(#)  Version : 1.1
# @(#)     Date : 12/10/93
# @(#)====================================================
#}}}

break DONE_SETERR
commands
silent
up-silently
up
end

define oshow
set $savepc=$pc
break Debug.h:1
jump DEBUGOCCAM
tbreak *$savepc
jump *$savepc
end

document oshow
Repeats most recent occam stack dump
end

define oids
set CFP=FP
set MODE=0
oshow
end

document oids
Displays values of a list of occam identifiers

   A space separated identifier list should be placed in ID 
   using the set command before this command is called.
   For example:

   set ID="a b fred"
   oids

end

define oall
set CFP=FP
set MODE=1
oshow
end

document oall
Displays values of all occam variables in scope
end

define oframe
set CFP=FP
set DEPTH=0
set MODE=2
oshow
end

document oframe
Displays values of all occam variables declared in current stack frame.
end

define oup
set CFP=FP
set DEPTH=DEPTH+1
set MODE=2
oshow
end

document oup
Displays values of all occam variables declared in caller's stack frame.
end

define odown
set CFP=FP
set DEPTH=DEPTH-1
set MODE=2
oshow
end

document odown
Displays values of all occam variables declared in callee's stack frame.
end

define ostep
step
set CFP=FP
oshow
end

document ostep
Steps through the occam program displaying selected variables
end

define olistsubs
set MODE=3
oshow
end

document olistsubs
Lists occam subroutine names and their C equivalents
end

set prompt spod>
echo ******************************************************\n
echo ** SPOD : Southampton's Portable Occam Debugger     **\n
echo ** M. Debbage, M. Hill, S. Wykes                    **\n
echo ** University Of Southampton, ESPRIT GPMIMD P5404   **\n
echo ** Shell for standard GNU gdb utility               **\n
echo ** Usual GNU licence applies                        **\n
echo ******************************************************\n\n
echo Some useful gdb commands:\n
echo    break    \t Sets a break point e.g.\n
echo \t \t "break P_jim_file" Sets bp at top level "PROC jim" in module file.occ\n
echo \t \t "break file.occ:12" Sets bp at line 12 in module file.occ\n
echo    run      \t Starts execution of program\n
echo    step     \t Executes occam line by line\n
echo    cont     \t Continues execution after breakpoint\n
echo    list     \t Lists code around current line of execution\n
echo \n
echo Occam debugging command set:\n
echo    oids     \t Displays values of a list of occam identifiers\n
echo    oall     \t Displays values of all occam variables in scope\n
echo    ostep    \t Steps through the occam program displaying selected variables\n
echo    odown    \t Displays values of all occam variables declared in callee's stack frame\n
echo    oup      \t Displays values of all occam variables declared in caller's stack frame\n
echo    oframe   \t Displays values of all occam variables declared in current stack frame\n
echo    oshow    \t Repeats most recent occam stack dump\n
echo    olistsubs\t Lists occam subroutine names and their C equivalents\n
echo \n
