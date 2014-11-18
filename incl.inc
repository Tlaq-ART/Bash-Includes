###########################################
# incl.inc    : Basic include controls for 
#               bash scripts. Together with
#               env.inc, it sets right paths
#               for used tools in scripts  
#           
#               Loaded with the line
#               . [c:|/c]/usr/local/include/incl.inc
#                
#               Since bash do not support return of
#               strings a global general return var 
#               "_retVal" is used to return sub 
#               function data
#           
# Legal       : This include file is part of Shell 
# boilerplate   Includes Library. 
#               They are relesed under Creative Common
#               with notice of origin version. 
#               Any use of the Shell Include Libraries
#               is "as is" and on your own responsibility. 
#               If in question, you not wanting any 
#               any responsibility, do not use these 
#               libraries.
#
# Author      : Kurt Magnusson  TLAQ A.R.T.
# Contact     : { tlaq {.} se {/} mailform {.} html }
# Date        : 14.10.13
# Version     : 1.0
# Revisions
# xx.xx.xx    :
###########################################
#
# set boolean values
TRUE=0 
FALSE=1

# set include check for env.inc to true (=0)
_inclinc=$TRUE

# test if Unix/Linux or Windows
_OS=`uname -s`

# test what shell and env, need to adapted paths to used environment
[[ -f /c/bin/bash.exe ]] && [[ $_OS == "WindowsNT" ]]  && _pathDir="/c"
[[ -f /bin/zsh.exe    ]] && [[ $_OS == "WindowsNT" ]]  && _pathDir="c:/"
[[ -f /bin/sh         ]] && [[ $_OS != "WindowsNT" ]]  && _pathDir=""

# set path for scripts
export PATH=$PATH:$_pathDir/bin:$_pathDIR/usr/bin:$_pathDIR/usr/local/bin

# needed directories
LBIN=$_pathDir/usr/bin
INCL=$_pathDir/usr/local/include
HOME=$_pathDir/home   
TMP=$_pathDir/tmp
LTMP=$_pathDir/tmp/tmp

# set macro for other includes
INCLUDE="source"

# include macros
#
ENV="$INCL/env.inc"
PROC="$INCL/proc.inc"
STD="$INCL/std.inc"
STDIO="$INCL/stdio.inc"
STRINGS="$INCL/strings.inc"
TPUT="$INCL/tput.inc"
CURSES="$INCL/curses.inc"
NCURSES="$INCL/ncurses.inc"
CGI="$INCL/cgi.inc"

# a common clear, lacking in Cygwin/Mingw bash
cls(){
    echo -en "\e[2J"
}

_inc_profiler="incl: `date +\"%T\"`" 

