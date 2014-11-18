#############################################
# strings.inc : internal string functions,
#               not to need to call external
#               processes as awk, sed, wc
#               for simpler string handling
#
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/strings.inc
#               or as
#               $INCLUDE $STRINGS
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
# Author      : Kurt Magnusson  TLAQ A.R.T.
# Contact     : { tlaq {.} se {/} mailform {.} html }
# Date        : 14.09.28
# Version     : 2.0
# Revisions
# xx.xx.xx    :
# 14.10.15    : re-written to support bash 
#               var=$(func var1 var2) calls
#############################################
#
# set boolean if needed
[[ "$_inclinc" = "" ]] && . /c/usr/local/include/incl.inc

# load includes if not yet loaded
[[ "$TR" = "" ]] && TR="$LBIN/tr"

# define that strings loaded
_stringsinc=$TRUE

# a shell split returning a substring at position pos 
# using _char divider. Is not working as awk split
split(){
    # split a line and return a given segment 
    local _str="$1"         # string to split
    local _array="$2"       # arrayname to store result in
    local _char="$3"        # char to split with

    IFS="$_char" 
    read -a "${_array}" <<< "$_str"
    _ret=$?

    IFS=' '
    return $_ret
}

# get position of a substring in the string
index(){
    local _str="$1"         # string to split
    local _sub="$2"         # position of needed substring
    
    _retVal=`expr index "$_str" "$_sub"`
    return $?
}

# a shell string lenght function
length(){
    local _str="$1"         # string to count

    _retVal="${#_str}"
    return $?
}

# macro for length (c "format")
strlen(){

    length "${*}"
}

# a shell substring returning a substring at pos _pos and length _len
substr(){
    # split a line and return a segment - similar to nawk substr
    local _str="$1"         # string to split
    local _pos="$2"         # position of needed substring
    local _len="$3"         # length of needed substring
    
    [[ "$_pos" != 0 ]] && _pos="$(($_pos-1))"

    _retVal="${_str:$_pos:$_len}"
    return $?
}

# a shell substring returning the last lenght chars
esubstr(){
    # split a line and return a segment - similar to nawk substr
    local _str="$1"         # string to split
    local _len="$3"         # length of needed substring
    
    _retVal="${_str:($_len)}"
    return $?
}

# a shell substring returning position of a substring
match(){
    # split a line and return a segment - similar to nawk substr
    local _str="$1"         # string to split
    local _regex="$2"       # string to match
    local _index            # index returned
    local _index2
    
    _index=`expr match "$_str" ".*$_regex"`
    length "$_regex"
    # note that bash starts indexes at 0
    # so +1 added
    _retVal="$(($_index-$_retVal+1))"
    return $?
}

# Similar to awk gsub, except that first arg
# is the string to be changed, not the regex
gsub(){
    local _str="$1"     # string to change
    local _seek="$2"    # string to replace
    local _repl="$3"    # replacement strings

    _retVal=${_str//$_seek/$_repl}
    return $?
}

# Similar to awk sub, except that first arg
# is the string to be changed, not the regex
sub(){
    local _str="$1"     # string to count
    local _seek="$2"    # string to replace
    local _repl="$3"    # replacement strings

    _retVal=${_str/$_seek/$_repl}
    return $?
}

# printf to variable
sprintf(){
    local _fmt="$1"   # output format
    shift
    local _args="$@"   # all the rest
    
    _retVal=`printf "$_fmt" "$_args"`
    return $?
}

# make string to upper case
toupper(){
    local _str="$1"    # string to convert 

    _retVal=`$TR a-z A-Z  <<< "$_str"`
    return $?
}

# make string to lower case
tolower(){
    local _str="$1"    # string to convert 

    _retVal=`$TR A-Z a-z <<< "$_str"` 
    return $?
}

# join strings
join(){
    local IFS

    _retVal=`IFS="$1"; shift; echo "$*"` 
    return $?
}

# non std strings, but often used in scripts
# get dir/file prefix
prefix(){
    local _str="$1"     # file to test for prefix
    local _div="$2"     # used divider

    split "$_str" "_out"  "$_div"
    _retVal="${_out[0]}"
    return $?
}

# get dir/file suffix
suffix(){
    local _str="$1"     # file to test for suffix
    local _div="$2"     # used divider
    local _noDiv        # no of divs

    # there might exist more than one divider
    # identify numers of dividers in this string 
    _noDiv="${_str//[^$_div]}"
    _noDiv="${#_noDiv}"

    split "$_str" "_out" "$_div"

    _retVal="${_out[$_noDiv]}"
    return $?
}

_inc_profiler="strings: `date +\"%T\"`" 
