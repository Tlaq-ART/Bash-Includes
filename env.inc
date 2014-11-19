###########################################
# env.inc     : Basic macro includes for bash scripts
#           
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/env.inc
#               or as
#               $INCLUDE $ENV
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
# Date        : 10.05.29
# Version     : 1.2
# Revisions
# xx.xx.xx    :
# 14.09.30    : Reworked to check if Cygwin/Mingw 
#               bash, Win-zsh or Linux/Unix and adapt
###########################################
#
# include basics
[[ "${_inclinc}" = "" ]] && . /c/usr/local/include/incl.inc
_envinc=$TRUE

# gnu cmds  
# 
AT="$LBIN/at.exe"
CAT="$LBIN/cat.exe"
CD="cd"
CLS="cmd.exe /c \"cls\""
CP="$LBIN/cp.exe"                    
CRON="$LBIN/crontab"
CUT="$LBIN/cut.exe"
FIND="$LBIN/find.exe"
ECHO="echo "
EXPR="$LBIN/expr.exe"
GREP="$LBIN/egrep.exe"
HEAD="$LBIN/head.exe"
LS="$LBIN/ls.exe"
MV="$LBIN/mv.exe"
NAWK="$LBIN/nawk.exe"
PWD="$LBIN/pwd.exe"
PRINTF="printf" 
PS="$LBIN/ps.exe"
RM="$LBIN/rm.exe"
RMD="$LBIN/rmdir.exe"
SED="$LBIN/sed.exe"
SORT="$LBIN/sort.exe"
TAIL="$LBIN/tail.exe"
TPUT="$LBIN/tput "
TR="$LBIN/tr.exe"
UNIQ="$LBIN/uniq.exe"
WC="$LBIN/wc.exe"

if [ "$_OS" == "WindowsNT" ]
then 
    # Win Dirs
    _winExt="C:\Program Files (x86)\Windows NT\Accessories"
    _winOff="C:\Program Files (x86)\Microsoft Office\Office14"
    _winDir="Windows"

    # basic Win cmd
    CMD="$_pathDir/$_winDir/system32/cmd.exe "
    TSLIST="$_pathDir/$_winDir/system32/tasklist.exe "
    TSKILL="$_pathDir/$_winDir/system32/taskkill.exe //PID "
    WOFF="$_pathDir/$_winDir/explorer.exe c:\ProgramData\Microsoft\$_winDir\Start Menu\Programs\Microsoft Office"
    WWORD="$_winOff\winword.exe"
    PWPPT="$_winOff\powerpnt.exe"
    EXLS="$_winOff\excel.exe"
    WDPD="$_pathDir/$_winDir/write.exe"
    IE="$_pathDir/Program?Files/Internet?Explorer/iexplore.exe"
    NTPD=notepad
    CALC="$_pathDir/$_winDir/system32/calc.exe"
fi

# User defined cmds  - your own extra processes
ACRO="$_pathDir/usr/local/bin/mupdf.exe"
AHK="/c/usr/local/bin/AutoHotKey.exe "
ACDSee="/c/usr/local/share/ACDSee/ACDSee "
BASH="/c/bin/bash.exe "
EMACS="$_pathDir/usr/local/bin/mewin.exe"
EPUBV="$_pathDir/usr/local/share/Calibre/Calibre Portable/Calibre/ebook-viewer.exe"
GIMP="/c/usr/local/share/gimp/bin/gimp-2.8.exe"
PUTTY="/c/bin/putty.exe "
SQL="/C/usr/local/share/SQLite/SQLite Database Browser 2.0 b1.exe "
PACRO="/c/bat/acroprt.sh"
CaPaste="/c/bat/PaCP.ahk"

fReturn="echo -e"

_inc_profiler="env: `date +\"%T\"`" 

