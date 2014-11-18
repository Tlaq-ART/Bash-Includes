##################################################
# stdio.inc   : C-like Input/output functions for
#               bash script, based on C's stdio.h
#           
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/stdio.inc
#               or as
#               $INCLUDE $STDIO
#               using "incl.inc" base include
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
# Author      : Kurt Magnusson, Tlaq A.R.T. 
# Contact     : { tlaq {.} se {/} mailform {.} html }
#
# Date        : 14.10.05
# Version     : 2.0
# Revisions     
# xx.xx.xx    :
##################################################
#
# set boolean if needed
[[ "$_inclinc" = ""   ]] && . /c/usr/local/include/incl.inc

# define that ncurses loaded
_stdioinc=$TRUE

# cat -v used to make control chars readable
[[ "$CAT" != "echo -e" ]] && CAT="cat"
 
# set define of stdio
stdin=0
stdout=1
stderr=2

EOF="-1"
SEEK_SET=0
SEEK_CUR=
SEEK_END=

#************************************
# stdio functions
#
open(){
    local _ioNode=$1    # fileIO to open
    local _fName=$2     # filename to open in Bash

    [[ "$_ioNode" -le 9 ]] && echo -e  "file descriptors to be between 0 - 9!" && return 255

    exec $_ioNode<> "$_fName"   
    return $_ret
}

fopen(){
    local _ioNode=$1    # fileIO to open
    local _fName=$2     # filename to open in Bash

    [[ "$_ioNode" -le 9 ]] && echo -e  "file descriptors to be between 0 - 9!"  && return 255

    exec $_ioNode<> "$_fName"   
    return $?
}

close(){
    local _ioNode=$1    # fileIO to close

    exec $_ioNode>&- 
    return $?
}

fclose(){
    local _ioNode=$1    # fileIO to close

    exec $_ioNode>&- 
    return $?
}

#*****************************
# get input
#
getc() {
    # get one char and leave
    local _char

    # read stream of chars one and one from std in
    read -t 1 -sn 1 _retVal
    _retVal=`echo -e  $_retVal | $CAT -v`
    return $?
}

fgetc() {
    local _ioNode=$1    # fileIO to read from
    local _char

    read -rsn 1 _retVal <$_ioNode
    _retVal=`echo -e  $_retVal | $CAT -v`
    return $?
}

getchar() {
    read -r -sn 1 _retVal
    _retVal="`echo -en $_retVal | $CAT -v | $SED \"s/!/\!/g\"`"
}

getchars() {
    # no ioNode, since getchar uses stdio
    local _noChars=$1    

    [[ -x "$_noChars" ]] && _noChars=64  

    read -r -sn $_noChars _retVal
    _retVal="`echo -en $_retVal | $CAT -v | $SED \"s/!/\!/g\"`"
}

gets() {
    # no ioNode, since getchar uses stdio
    # always max 256 chars

    read -rsn 265 _retVal 
    _retVal=`echo -en $_retVal | $CAT -v | $SED "s/!/\!/g"`
    return $?
}

scanf() {
    # no ioNode, since getchar uses stdio
    local _format=$1    # scanf's read format
    local _fillVar      # the read var

    read -s _fillVar
    _retVal=`printf "$_format", "_fillVar"  | $CAT -v`
    return $?
}

fscanf() {
    local _format="$1"  # scanf's read format
    local _ioNode=$2    # no ioNode, since getchar uses stdio
    local _fillVar      # the read var

    read -s _fillVar <$_ioNode
    _retVal=`printf "$_format", "$_fillVar"`
    return $?
}

# "macro" - not in stdio.h
getline() {
    # no ioNode, since getchar uses stdio
    local _line

    read -s _line
    _retVal=`echo -e  \"$_line\" | $CAT -v`
    return $?
}

#*****************************
# put to output
#
putc(){
    local _char=$1      # char to put
    local _ioNode=$2    # file descriptor

    echo -e  "$_char" >&$_ioNode
    return $?
}

fputc(){
    local _char=$1      # char to put
    local _ioNode=$2    # file descriptor

    echo -e  "$_char" >&$_ioNode
    return $?
}

putchar(){
    # no ioNode, since putchar uses stdout
    local _char=$1      # char to put

    echo -e  "$_char"  | $CAT -v
    return $?
}

puts(){
    # no ioNode, since putchar uses stdout
    local _str=$1      # char to puts

    echo -e  "$_str" | $CAT -v
    return $?
}

# uses the built-in printf command, so not declared
# printf(){} 

fprintf(){
    local _format=$1    # scanf's read format
    local _ioNode=$2    # no ioNode, since getchar uses stdio
    local _str          # the read var

    printf "$_format", "$_str" >&$_ioNode
    return $?
}

# "macro" - not in stdio.h
putline(){
    # no ioNode, since putchar uses stdout
    local _str=$1      # str to put

    echo -e  "$_str" | $CAT -v
    return $?
}

#*****************************
# file functions
#
fseek(){
    # placeholder
    echo ""
}

#*****************************
# other functions
#
flushbuf() {
    local _DUMMY

    read _DUMMY < /dev/null
    return $?
}
 
_inc_profiler="stdio: `date +\"%T\"`" 
