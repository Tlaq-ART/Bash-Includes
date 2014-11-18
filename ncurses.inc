##########################################################
# ncurses.inc   : Cursor control include for bash scripts
# 
#                 A curses script, to give bash scripts
#                 limited ncurses capabilites. It is partly
#                 witten handle some basic bash and Windows 
#                 Cygwin/Mingw bash quirks, such as bash 
#                 lack of returning strings from functions
#                 and not able to assign a "calling" var 
#                 directly. A global var _retVal is set
#                 to return data. 
#
#                 Cygwin/Mingw bash uses /c/dir to handle
#                 windows disks, while Win32 zsh uses c:/dir   
# 
#  Note 1       : ncurses.inc is a shell include, added 
#                 to the calling shellscript by the line
#                 "source _path_/ncurses.inc"   or
#                 ".  _path_/ncurses.inc"   
#
#  Note 2       : Some ideas in this include are from Dana 
#                 Frensh's IBM/Mt. Xia ksh93 ncurses package.
#                 That script is not used since it creates 
#                 some issues with Cygwin/Mingw bash and 
#                 do not support the here used individual 
#                 windows.
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
# Author        : Kurt Magnusson, Tlaq A.R.T. 
# Contact       : { tlaq {.} se {/} mailform {.} html }
#
# Date          : 14.09.05
# Version       : 1.0
# Revisions     
# xx.xx.xx      :
##########################################################
#
# set boolean if needed
[[ "$_inclinc" = ""   ]] && . /c/usr/local/include/incl.inc

# load includes if not yet loaded
[[ "$_envinc" != $TRUE     ]] && $INCLUDE $ENV
[[ "$_stdioinc" != $TRUE   ]] && $INCLUDE $STDIO 
[[ "$_stringsinc" != $TRUE ]] && $INCLUDE $STRINGS

# define that ncurses loaded
_ncursesinc=$TRUE

#***********************
# some environmental
_NCwinColor="b1"                    # mode con: color code
_NCcodePage="850"                   # mode con: NCcodePage 
_resetNCcodePage="850"              # mode con: NCcodePage reset

#***********************
# 
[[ "$ECHO" = "" ]] && ECHO="echo -en "
CLS="echo -e \"\e[2J\""            # clear macro

# using tput is to slow, so ANSI esc is used to define
# some functions needed [adapted from Mt Xia macros]
CLEAR="\e[2J"                       # Clear display
CMD_LINES=                          # Number of lines on display
CMD_COLS=                           # Number of cols on display
CLRTOEOL="\e[0K"                    # Clear to end of line
CLRTOBOL="\e[1K"                    # Clear to beginning of line
CLRTOEOD="\e[J"                     # Clear to end of display
DELCH="\e[P"                        # Delete current character
DELETELN="\e[M"                     # Delete current line
INSCH="\e[1@"                       # Insert 1 character
INSERTLN="\e[L"                     # Insert 1 Line
ATTROFF="\e[0m"                     # All Attributes OFF
ATTRSET="\e[0m"                     # All Attributes OFF
BEEP="\a"                           # ring bell
ROWS_NBR=24                         # standard term rows
COLS_NBR=80                         # standard term col
ERROR_PAUSE=`sleep 2`               # sleep if error, allowing read of msg
DEV_NULL="/dev/null"                # the holy dump

#***********************
# ncurses.inc data defs and bash needed shortcuts
# 
_NCmaxbuf=5                                 # max no of screen buffs
_NCcurscr=0                                 # currently active screen main screen = 0
declare -a _NCwinptr=('0' '0' '0' '0' '0')  # store 4 screen buffers data; "0" = unused
declare -a _NCwinBuf=('' '' '' '' ' ')      # declare $_NCmaxbuf screen buffers
declare -a _NCsaveBuf=('' '' '' '' '')      # declare saved screen under screen buffers
_NCchgBuf=                                  # non permanent changes to _NCtotBuf

_ncFullRefrFlag=$FALSE                      # Force only last screen to refresh
_ncErrFlag=$FALSE                           # hinder err msg loops
NC_Div="¤"                                  # divider used in separating posts in arrays

#************************************************
# Functions
#****************************
# window management
#
initscr(){
    local _rows=$1
    local _cols=$2
    local _height=
    local _width=
    local binDir=
    local tputs=tput

    # clean up screen
    $CLS

    if [ $_NCcurscr == 0 ]
    then
        # init the terminal win
        case "$_OS" in
          "windows_NT"|"WindowsNT") 
            # we're running under some window bash
            _SYS="Win"
            if test -d "/c/windows"
            then
                # if Cygwin or MINGW bash set some win configs
                # cols need to be _in_ active window so + 1
                _width=$(($_cols + 1))
                /c/Windows/system32/mode.com con: COLS=$_width LINES=$_rows   > /dev/null 1>&2      
                /c/Windows/system32/cmd.exe //c color $_NCwinColor    > /dev/null 2>&1      
                /c/Windows/system32/cmd.exe //c chcp $_NCcodePage  > /dev/null 2>&1     
                export LINES=$_rows
                export COLUMNS=$_cols
                binDir="c/usr/bin"
                tputs="$binDir/tput"
            else
                # if another win shell like zsh.exe do it this way
                # cols need to be _in_ active window so + 1
                _width=$(($_cols + 1))
                /Windows/system32/mode.com con: COLS=$_width LINES=$_rows   > /dev/null 1>&2      
                /Windows/system32/cmd.exe //c color _$_NCwinColor    > /dev/null 2>&1      
                /Windows/system32/cmd.exe //c chcp $_NCcodePage  > /dev/null 2>&1     
                export LINES=$_rows
                export COLUMNS=$_cols
                binDir="/usr/bin"
                tputs="$binDir/tput"
           fi    
            ;;
          "Linux") 
                export LINES=$_rows
                export COLUMNS=$_cols
                binDir="/usr/bin"
                tputs="$binDir/tput"
            ;;
        esac
        IFS_CR="$'\n'"
        IFS_CR="
"
        IFS_NORM="$' \t\n'"
        IFS_NORM=" 	
"

        MAC_TIME="TIMESTAMP=\`date +\"%y:%m:%d:%H:%M:%S\"\`"
        MAX_LINES="$LINES"
        MAX_COLS="$COLUMNS"

        # macro for upper left corner
        _NCver="1"
        _NChor="1"
        # handle main window 0:0 -> 1:1 coordinate issue
        _NCrows=$(($LINES - 1))
        _NCcols=$(($COLUMNS - 2))
        
        _NCcurscr=0

        init_buf $_NCcurscr
        _NCwinptr[$_NCcurscr]="$_NCver$NC_Div$_NChor$NC_Div$_NCrows$NC_Div$_NCcols"
        _ret=$?

    else
        errTxt "ncurses:  initscr-screen already initiated, can't create new init screen"
        _ret=$FALSE
    fi

    return $_ret
}

endwin() {
    unset _NCwinptr
    unset _NCwinBuf
    unset _NCsaveBuf

    [[ "$_SYS" == "Win" ]] && /c/window/system32/cmd.exe //c chcp $_resetNCcodePage > /dev/null 2>&1      
    _ret=$?

    $CLS
    return $_ret
}

#***********************************************
# Standard screens ops
#
# newwin, allocate memory and config a subwindow 
# to the main one.
newwin(){
    local _vpos=$1      # top left corner, start
    local _hpos=$2      # top left corner, start
    local _rows=$3      # hight of box
    local _cols=$4      # width of box
    local _win          # win check 

    if [ "$_OS" == "Win" ]
    then
        # Cygwin/Mingw bash can't use full terminal screen without
        # wrapping it, so rows always -1 but cols +1 
        # (yes, a really stupid kludge, but due to MS cmd.exe)
        _rows=$(($_rows-1))
        _cols=$(($_cols+1))
    fi

    # Newwin is a sub-win so add 1 to current _NCcurscr
    _NCcurscr=$(($_NCcurscr+1))
    
    # check if win yet unallocated
    _win="${_NCwinptr[$_NCcurscr]}"

    # check if OK to create a new window
    if [ "$_win" == $TRUE ] && [ "$_NCcurscr" -lt "$_NCmaxbuf" ]
    then
        # if a new win, save the the current screen buffer
        [[ "$_NCcurscr" -gt 0 ]] && save_buf $_NCcurscr-1 && _ret=$?

        # save new win values
        _NCwinptr[$_NCcurscr]="$_vpos$NC_Div$_hpos$NC_Div$_rows$NC_Div$_cols"
        _ret=$?
      
        # Set active coordinate macros
        _NCver="$_vpos"
        _NChor="$_hpos"
        _NCrows="$_rows"
        _NCcols="$_cols"

        # init the actual buffer 
        init_buf $_NCcurscr
        _ret=$?

        # prepare for full screen update
        _FullRefrFlag=$TRUE

    elif [ "$_NCcurscr" -ge "$_NCmaxbuf" ]
    then
        errTxt "newwin() to many windows declared = $_NCcurscr; maxwin = $_NCmaxbuf !"
        _ret=$FALSE
        _retVal=255
    else
        errTxt "newwin() newwin $_NCcurscr do already exist"
        _ret=$FALSE
        _retVal=255
    fi

   _retVal=$_NCcurscr
    return $_ret
}

# Delete a subwindow
delwin(){
    local _win=$1           # current screen to delete
    local _curwin=          # current windows parameters 
    local _vpos=            # top left corner, start
    local _hpos=            # top left corner, start
    local _rows=            # height of box
    local _cols=            # width of box
    local _vabs=            # absolut pos of vertical lines to keep screen sane
    local _line=            # intermediate lines
    local _topl=            # box top line
    local _mline=           # box side line
    local _botl=            # box bottom line
    local i                 # row counter
    local j                 # cols counter
    
    if [ "$_win" -gt 0 ]
    then
        _NCcurscr=$(($_NCcurscr-1))
        _NCwinptr[$_win]="$TRUE"
        _NCwinBuf[$_win]=""
        _NCsaveBuf[$_win]=""
        _win=$(($_win-1))
        restore_buf $_win
        _ret=$?
    elif [ "$_win" == 0 ]
    then
        _NCcurscr=0
        _NCwinptr[$_win]="$TRUE"
        restore_buf 0
        _ret=$?
    else
        errTxt "delwin() delwin  of window $_NCcurscr failed! "
        _ret=$FALSE
    fi
    
    # prepare previous screen reset    
    _FullRefrFlag=$TRUE

    # Reset coordinates vars
    # get old windows data ( filtering from _NCwinptr in stable)
    _curwin="${_NCwinptr[$_win]}"

    # access _curwin array
    split "$_curwin" coord "$NC_Div"

    debug="${coord[@]}"
    # get topmost pos
    _NCver=${coord[0]}

   # get topmost pos
    _NChor=${coord[1]}

   # get topmost pos
    _NCrows=${coord[2]}

    # get topmost pos
    _NCcols=${coord[3]}

    _retVal=$_win
    return $_ret    
}

# make a box around a window
box() {
    local _win=$1           # pointer to this window in _NCwinptr array
    local _vchar=$2         # vertical char 
    local _hchar=$3         # horisontal char 
    local _curwin=          # current windows parameters 
    local _vpos=$_NCver     # top left corner, start
    local _hpos=$_NChor     # top left corner, start
    local _rows=$_NCrows    # height of box
    local _cols=$_NCcols    # width of box
    local _vabs=            # absolut pos of vertical lines to keep screen sane
    local _line=            # intermediate lines
    local _topl=            # box top line
    local _mline=           # box side line
    local _botl=            # box bottom line
    local i                 # row counter
    local j                 # cols counter
    local v                 # \e[vpos;hposH at times mess 
    local h                 # up if v/h-vars with long names    
    local _scrbuf           # current window buffer

    # Set set default color (only grey on/off) and chars
    local grON="\e[47m"     # grey on
    local grOF="\e[0m"      # grey off
    local _ulChar="*"       # upper left frame corner character
    local _urChar="*"       # upper right frame corner character
    local _llChar="*"       # lower left frame corner character
    local _lrChar="*"       # lower right frame corner character
    local _hChar="="        # horisontal character
    local _vChar="|"        # vertical character


    if [ "$_hchar" = "" ] 
    then
        errTxt "box(): box [windows pointer] [vertical char] [horizontal char] !"
    else

        # initiate frame
        case $_vchar in
             0)
                _ulchar="$grON*$grOF"               # Make upper left frame corner character
                _urchar="$grON*$grOF"               # Make upper right frame corner character
                _llchar="$grON*$grOF"               # Make lower left frame corner character
                _lrchar="$grON*$grOF"               # Make lower right frame corner character
                _hchar="$grON$_hChar$grOF"          # Make frame horisontal character
                _vchar="$grON$_vChar$grOF";;        # Make frame vertical character
             *)
                _ulchar="$grON*$grOF"               # Make upper left frame corner character
                _urchar="$grON*$grOF"               # Make upper right frame corner character
                _llchar="$grON*$grOF"               # Make lower left frame corner character
                _lrchar="$grON*$grOF"               # Make lower right frame corner character
                _hchar="$grON$_hchar$grOF"          # Make frame horisontal character
                _vchar="$grON$_vchar$grOF";;        # Make frame vertical character
        esac
    
        # make top line        
        for ((j=1 ; j < $_cols-1 ; j++ ))
        do
            _line="$_line$_hchar"                   # add chars to make line
        done    
        _topl="$_ulchar$_line$_urchar"
        _line=

        # make middle line        
        for ((j=1 ; j < $_cols-1 ; j++ ))
        do
            _line="$_line "                         # add chars to make line
        done    
        _mline="$_vchar$_line$_vchar"
        _line=

        # make bottom line        
        for ((j=1 ; j < $_cols-1 ; j++ ))
        do
            _line="$_line$_hchar"                   # add chars to make line
        done    
        _botl="$_llchar$_line$_lrchar"
        _line=

        # write the box
        # top line
        v=$(($_vpos))                               # tput is using a 1;1 array start not 0;0
        h=$(($_hpos))                               # tput is using a 1;1 array start not 0;0
        _scrbuf="\e[${v};${h}H$_topl"               # start position + line

        # again, compensate for tput using 1:1 coord
        for (( i=1 ; i < $_rows ; i++ ))
        do
            # add middle lines
            v=$(($_vpos+$i))                        # get abs _vpos for line
            _scrbuf="$_scrbuf\e[${v};${h}H$_mline"  # line position + line
        done
        
        v=$(($_vpos+$i))                            # get abs _vpos for line
                                                    # note that $i is +1 already
        _scrbuf="$_scrbuf\e[${v};${h}H$_botl"       # start position

        if [[ "$_win" -ge 0 ]] && [[ "$_win" -le "$_NCmaxbuf" ]]
        then
            _NCwinBuf[$_win]="$_scrbuf"       
            _NCsaveBuf[$_win]="$_scrbuf"
        else
            errTxt "init_buf()  cannot init a buffer since current win $_NCcurscr > max win $_NCmaxbuf" 
        fi
        _ret=$?
    fi    
    return $_ret
}


#*******************************
# buffer manipulations
init_buf(){
    local _win=$1           # current screen to delete
    local _curwin=          # current windows parameters 
    local _vpos=$_NCver     # top left corner, start
    local _hpos=$_NChor     # top left corner, start
    local _rows=$_NCrows    # height of box
    local _cols=$_NCcols    # width of box
    local _vabs=            # absolut pos of vertical lines to keep screen sane
    local _line=            # intermediate lines
    local _topl=            # box top line
    local _mline=           # box side line
    local _botl=            # box bottom line
    local i                 # row counter
    local j                 # cols counter
    local v                 # \e[vpos;hposH at times mess 
    local h                 # up if v/h-vars with long names    
    local _scrbuf           # current window buffer

    if [ "$_NCcurscr" -ge 0 ]
    then
        h=$_hpos

        # make an empty line 
        for ((j=1 ; j <= $_cols ; j++ ))
        do
           _line="$_line "                          # make line
        done

        # make empty screen
        for (( i=1 ; i <= $_rows ; i++ ))
        do
            v=$(($_vpos+$i))                        # here we need to compensate for 0:0 pos
            _scrbuf="$_scrbuf\e[${v};${h}H$_line"   # line start position
            _scrbuf="$_scrbuf$_line"                # add next row to buffer
        done

        if [ "$_win" -ge 0 ] && [ "$_win" -le "$_NCmaxbuf" ]
        then
            _NCwinBuf[$_win]="$_scrbuf"       
        else
            errTxt "init_buf()  cannot init a buffer since current win $_NCcurscr > max win $_NCmaxbuf" 
        fi
    fi
    return $_ret
}    

# refresh output to screen
refresh(){
    ECHO="echo -e"
    if [ "$_FullRefrFlag" == $TRUE ]
    then
        for (( i=0 ; i <= $_NCcurscr ; i++ )) 
        do
            $ECHO "${_NCwinBuf[$i]}"
        done
        _FullRefrFlag=$FALSE
    fi
    $ECHO "$_NCchgBuf"
    ECHO="echo -en"
}

# force change buffer _NCchgBuf's output to screen
touchwin(){
    local _win=$1           # window to touch
    
    _FullRefrFlag=$TRUE     # tell refresh to do a full refresh

}

# save current screen before doing a new window
# or before other major changes
save_buf(){
    local _win=$1           # what screen to use 
    
    # read whats currently on screen
    _NCsaveBuf[$_win]="${_NCwinBuf[$_win]}$_NCchgBuf"  # save current screen before a new win
    _NCwinBuf[$_win]="${_NCsaveBuf[$_win]}"            # also save as current 
    _NCchgBuf=""                                       # reset changes for next win
   return $?
}

# if a sub window closed, reset screen below
restore_buf(){
    local _win=$1           # active window

    _NCchgBuf="${_NCsaveBuf[$_win]}"
    _FullRefrFlag=$TRUE

    return $?
}


#********************************
# basic ncurses commands
#
#*********************************
# reused Curses func from Mt Xia's script
#
#********************************* 
addch(){
    addstr "${1:0:1}"
    return ${?}
}

#********************************* 
attroff(){
    addstr "${ATTROFF}"
    return ${?}
}

#********************************* 
attron(){
    return 0
}

#********************************* 
attrset(){
    addstr "$( ${ATTRSET} ${1} )"
    return ${?}
}

#********************************* 
beep(){
    addstr "${BEEP}"
    return ${?}
}

#********************************* 
clear(){
    addstr "${CLEAR}"
    return ${?}
}

#********************************* 
clrtobol(){
    addstr "${CLRTOBOL}"
    return ${?}
}

#********************************* 
clrtobot(){
    addstr "${CLRTOEOD}"
    return ${?}
}

#********************************* 
clrtoeol(){
    addstr "${CMD_CLRTOEOL}"
    return ${?}
}

#********************************* 
delch(){
    addstr "${DELCH}"
    return ${?}
}

#********************************* 
deleteln(){
    addstr "${DELETELN}"
    return ${?}
}

#********************************* 
getch(){
    IFS='' read -r -- TMP_GETCH
    STATUS="${?}"
    eval \${ECHO} \"\${TMP_GETCH}\"
    return ${STATUS}

#    if [[ "$1" = 0 ]]
#    then
#        read -r  TMP_GETCH
#        STATUS="${?}"
#    else
#        read -rn $1  TMP_GETCH
#        STATUS="${?}"
#    fi
#    $_retVal="$TMP_GETCH"
#    eval \${ECHO} \"\${TMP_GETCH}\"
#    return ${STATUS}
}

#********************************* 
getstr(){
    read -r TMP_GETCH
    eval \${ECHO} \"\${TMP_GETCH}\"
    STATUS="${?}"
    return ${STATUS}
#    IFS="${IFS_CR}"
#    [[ "$2" != "" ]] && \
#        getch $2 || \
#        getch 0 
#    STATUS="${?}"
#   IFS="${IFS_NORM}"
#    return ${STATUS}
}

#********************************* 
getwd(){
    getch
    return ${?}
}

#********************************* 
insch(){
    addstr "${INSCH}"
    return ${?}
}

#********************************* 
insertln(){
    addstr "${INSERTLN}"
    return ${?}
}

#********************************* 
mvaddch(){
    move "${1}" "${2}" &&
      addch "${3}"
    return ${?}
}

#********************************* 
mvaddstr(){
    move "${1}" "${2}" &&
      addstr "${3}"
    return ${?}
}

#********************************* 
mvclrtobol(){
    move "${1}" "${}" &&
      clrtobol
    return ${?}
}

#********************************* 
mvclrtobot(){
    move "${1}" "${2}" &&
      clrtobot
    return ${?}
}

#********************************* 
mvclrtoeol(){
    move "${1}" "${2}" &&
      clrtoeol
    return ${?}
}

#********************************* 
mvdelch(){
    move "${1}" "${2}" &&
      addstr "${DELCH}"
    return ${?}
}

#********************************* 
mvinsch(){
    move "${1}" "${2}" &&
      addstr "${INSCH}"
    return ${?}
}

#************************************* 
# ncurses functions not in/from Mt Xia 
# or altered 

#********************************* 
addstr(){
    [[ "_${1}" != "_" ]] &&
      _NCchgBuf="$_NCchgBuf${1}"
    return ${?}
}

#********************************* 
move(){
    wmove $_NCcurscr "${1}" "${2}"    
    return ${?}
}

#********************************* 
mvcur(){
    ECHO="echo -e"
    local _win=$_NCcurscr   # pointer to this window in _NCwinptr array
    local _wvpos=$1         # vertical char in active win
    local _whpos=$2         # horisontal char in active win
    local _vpos=$_NCver     # window top left corner, start
    local _hpos=$_NChor     # window top left corner, start
    local _rows=$_NCrows    # height of box
    local _cols=$_NCcols    # width of box
    local _wvmax=           # max lowest row in win
    local _whmax=           # max right pos in win
    local _scrbuf           #

    # set inside the box so +1 to compensate for frame
    _wvpos=$(($_vpos+$_wvpos))
    _whpos=$(($_hpos+$_whpos))

    strlen "$_str"
    _slen=$?

    _smax=$(($_whpos+$_slen))

    _wvmax=$(($_vpos+$_rows))
    _whmax=$(($_hpos+$_cols))

    if [ $_wvpos -gt $_vpos ] && [ $_whpos -gt $_hpos ] && [ $_wvpos -lt $_wvmax ] && [ $_smax -lt $_whmax ] 
    then
        $ECHO "\e[${_wvpos};${_whpos}H\c"
    else
        errTxt "mvcur has incorrect values: $_wvpos $_whpos $_smax and window $_vpos $_hpos $_wvmax $_whmax"
        exit $FALSE
    fi
    ECHO="echo -en"
    return $?
}

#********************************* 
wmove(){
    local _win=$1           # pointer to this window in _NCwinptr array
    local _wvpos=$2         # vertical char in active win
    local _whpos=$3         # horisontal char in active win
    local _vpos=$_NCver     # window top left corner, start
    local _hpos=$_NChor     # window top left corner, start
    local _rows=$_NCrows    # height of box
    local _cols=$_NCcols    # width of box
    local _wvmax=           # max lowest row in win
    local _whmax=           # max right pos in win
    local _scrbuf           #

    # set inside the box so +1 to compensate for frame
    _wvpos=$(($_vpos+$_wvpos))
    _whpos=$(($_hpos+$_whpos))

    strlen "$_str"
    _slen=$?

    _smax=$(($_whpos+$_slen))

    _wvmax=$(($_vpos+$_rows))
    _whmax=$(($_hpos+$_cols))

    if [ $_wvpos -gt $_vpos ] && [ $_whpos -gt $_hpos ] && [ $_wvpos -lt $_wvmax ] && [ $_smax -lt $_whmax ] 
    then
        _scrbuf="$_scrbuf\e[${_wvpos};${_whpos}H"
        _NCchgBuf="$_NCchgBuf$_scrbuf"
    else
        errTxt "wmove has incorrect values: $_wvpos $_whpos $_smax and window $_vpos $_hpos $_wvmax $_whmax"
        exit $FALSE
    fi
    return $?
}

#********************************* 
mvwaddstr(){
    local _win=$1           # pointer to this window in _NCwinptr array
    local _wvpos=$2         # vertical char in active win
    local _whpos=$3         # horisontal char in active win
    local _str=$4           # string to print
    local _scrbuf           #

    wmove $_win $_wvpos $_whpos
    
    [[ "$(($_whpos+${#_str}+1))" -lt $_NCcols ]] && addstr "$_str" ; _ret=$TRUE \
       || errTxt "mvaddstr() $_str to long: $_whpos + ${#_str} \< $_cols" ; _ret=255 
       
    return $_ret
}

#******************************************************
# Specials not in ncurses
#*******************************
# a local tput cup replacement macro
# to handle speed issues.
tput(){
    if [ "$3" != "" ] 
    then 
        echo -e "\e[$2;$3H\c"   # tput cup replacement
    else
       addstr "${1}: Invalid line number $2 $3" >&2 
       refresh 
       ${ERROR_PAUSE}
    fi
}

#*******************************
# 
errTxt(){
    local _str="$1"         # get error text string
    local _slen=            # tmp buf for read of text
    local _vpos=            # window top left corner, start
    local _hpos=            # window top left corner, start
    local _rows=            # height of box
    local _cols=            # width of box
    local _slen=            # length of string
    local _smax=            # max length of string
    local _wvmax=           # max lowest row in win
    local _whmax=           # max right pos in win
    local _scrbuf           #

    if [ "$_ncErrFlag" == $TRUE ] ; then return $FALSE ; else _ncErrFlag=$TRUE ;fi

    _vpos=$(($LINES-10))
    _hpos=$((($MAX_COLS*10)*27/1000))

    newwin $_vpos $_hpos 4 47   # create a new err window
    _win=$_retVal

    box $_win 0 0

    strlen "$_str"          # get length of string
    _slen=$?
    if [ "$_slen" -gt 36 ] 
    then 
        substr "$_str" 1 36
        _str="$_retVal"
    fi
    mvaddstr 2 2 "$_str"    # add text to win at win vpos/hpos
    refresh                 # put on screen
    getchars 2              # get a user return
    
    delwin $_win            # restore to previous
    _win=$retVal
    return $?
}

_inc_profiler="ncurses: `date +\"%T\"`" 

