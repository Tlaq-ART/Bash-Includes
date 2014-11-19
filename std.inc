####################################
# std.inc     : standard functions for scripts
#           
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/std.inc
#               or as
#               $INCLUDE $STD
#               using "incl.inc" base include
#                
#               Since bash do not support return of
#               strings a global general return var 
#               "_retVal" is used to return sub 
#               function data
#           
# Legal       : This include file is part of Bash 
# boilerplate   Includes Library. 
#               They are relesed under Creative Common
#               with notice of origin version. 
#               Any use of the Shell Include Libraries
#               is "as is" and on your own responsibility. 
#               If in question, you not wanting any 
#               any responsibility, do not use these 
#               libraries.
#
# Author      : Kurt Magnusson, Tlaq A.R.T. 
# Contact     : { tlaq {.} se {/} mailform {.} html }
#
# Date        : 14.08.27
# Version     : 1.0
# Revisions     
# xx.xx.xx    :
# 14.09.30    : Replaced internal echo with printf
##################################################
#
# set boolean if needed
[[ "$_inclinc" = ""   ]] && . /c/usr/local/include/incl.inc

# load includes if not yet loaded
[[ "$_envinc" != $TRUE     ]] && $INCLUDE $ENV

# define that ncurses loaded
_stdinc=$TRUE

#*************************************************
# Functions

# Trap signals to break
signal(){
    local SIGINT=$1     # what signals to react on
    local _action=$2    # what action to perform

    trap "$_action" $SIGINT
}

# often used pwd modifyer
getDir() {
    local delim=$1

    _retVal=`pwd | $CUT -d'//' -f$delim-`
}

# expand a dir/file name to full name
fullName() {
    local _object="$1"   # dir-/filename to expand
    local _chkNo         # check no of matches

    # get full name if wildcarded
    if test -e "$_object"
    then
        _retVal="$_object"
       _ret=$TRUE
    else
        _object="`$FIND . -name \"$_object*\" -maxdepth 1 | $SED \"s/\..//\" | $TR \"\\†\Ž\„\™\”\" \"ÅåÄäÖö\"`" 

        # get number of hits
        _chkNo=`echo "$_object" | $WC -l | sed "s/ //g" `

        if [ $_chkNo = 1 ]
        then 
            # one hit, return full name
            _retVal="$_object"
            _ret=$TRUE
        else
            # user need to refine name given, for better match
            _retVal="Name \"$1\" is matching $_chkNo objects!" 
            _ret=$FALSE
        fi
    fi
    return $_ret
}

# Windows (mingw32 bash) copy and paste
wincp_on(){
    # Win cmd has copy but no paste, using a 
    # AutoHotkey script for this. Script runned
    # separately for this program

    ($AHK $CaPaste)&

    _Xnoter_pasteon=`$TSLIST | $GREP "AutoHotkey" | $NAWK '{print $2 }' | tail -1`
}

# Windows (mingw32 bash) copy and paste shutdown
wincp_off(){
    # Win cmd has copy but no paste, using a 
    # AutoHotkey script for this. Allow for 
    # delete

    [[ "$_Xnoter_pasteon" != "" ]] && $TSKILL $_Xnoter_pasteon > /dev/null 2>&1
}

_inc_profiler="std: `date +\"%T\"`" 

