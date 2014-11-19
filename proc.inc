####################################
# proc.inc    : implement proc functions for scripts
#           
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/proc.inc
#               or as
#               $INCLUDE $PROC
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
# Author      : Kurt Magnusson  TLAQ A.R.T.
# Contact     : { tlaq {.} se {/} mailform {.} html }
# Date        : 14.09.22
# Version     : 1.0
# Revisions
# xx.xx.xx    :
####################################
#
# set boolean if needed
[[ "$_inclinc" = ""   ]] && . /c/usr/local/include/incl.inc

# load includes if not yet loaded
[[ "$_stringsinc" != $TRUE ]] && $INCLUDE $STRINGS

# set that proc is loaded
_procinc=$TRUE

# define Linux Terminal process
TERMINAL="gnu-xterm -x"


# fork a process 
fork(){
    local _proc="$1"        # process to start
    local _args="$2"        # any arguments passed
    local _pid              # process id
    local _type             # type if process (needing bash?)
    local _SYS              # linux/unix, Cygwin/Mingw or zsh
    
    # id type of bash/zsh
    _SYS=`pwd`
    substr $_OS 0 3
    _SYS=$_retVal
    [[ "$_SYS" = "c:/" ]] && _SHELL="zsh"
    [[ "$_SYS" = "/c/" ]] && _SHELL="cbash"
    [[ "$_SYS" != "zsh" ]] && [[ "$_SHELL" != "cbash" ]] && _OS="linux"

    [[ "$_OS" = "WindowsNT" ]] && [[ "$_SHELL" = "cbash" ]] && local _winDir="/c/windows/system32" 
    [[ "$_OS" = "WindowsNT" ]] && [[ "$_SHELL" = "zsh" ]] && local _winDir="c:/windows/system32" 
    
    if [ "$_OS" = "WindowsNT" ] && [ "$_winDir" != "" ] 
    then 
        # split "$_proc" _parts "."  
        _type=$(suffix "$_proc" ".") 
        _type=$(toupper "$_type" )

        case $_type in
            "BAT")
                $_winDir/cmd.exe //c "start $_proc \"$_args\"" 
	        ;;
            "SH"|"")
                $_winDir/cmd.exe //c "start c:\bin\bash.exe $_proc  \"$_args\"" 
	        ;;
            "EXE"|"COM")
                $_winDir/cmd.exe //c "start $_proc  \"$_args\""
	        ;;
            *) $ECHO "fork: \"$_proc\" not a valid process!"
	        ;;
        esac
        _pid="`$TSLIST | $GREP \"cmd\.exe\" | $TAIL -1 | $NAWK '{print $2}'`"
    else
        # ???? split "$_proc" ":" 2
        $TERMINAL $_proc
        _pid="`ps | grep $$ | tail -1 | nawk '{print $1}'`"
    fi

   return $_pid
}

_inc_profiler="proc: `date +\"%T\"`" 

