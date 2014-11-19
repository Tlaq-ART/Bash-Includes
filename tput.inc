#*************************************************
# tput.inc    : Emulate Unix tput basic screeen adressing
#               in bash - include file version   
#
#               There is an an Win32 bash "command" version
#
#               Some definitions in the begining is 
#               OS dependent and need change
#
#
#               Include either loaded with the line
#               . [c:|/c]/usr/local/include/tput.inc
#               or as
#               $INCLUDE $TPUT
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
# Author    : Kurt Magnusson, Tlaq A.R.T.
# Contact   : { tlaq {.} se {/} mailform {.} html }
# Version   : 1.1   
# Date      : 2014.09.09 (include version)
# ---------
# Revisions 
# yy.mm.dd  : Comment
# 14.09.23  : Added a "full" cmd list
#*************************************************
#
# defined ansi codes for tput  
# ansi|ansi/pc-term compatible with color,
#    am, mc5i, mir, msgr,
#    colors#8, cols#80, it#8, lines#24, ncv#3, pairs#64,
#    acsc=+\020\,\021\030.^Y0\333`\004a\261f\370g\361h\260j\331k\277l\332m\300n\305o~p\304q\304r
#	  \304s_t\303u\264v\301w\302x\263y\363z\362{\343|\330}\234~\376,
#    bel=^G, blink=\E[5m, bold=\E[1m, cbt=\E[Z, clear=\E[H\E[J,
#    cr=^M, cub=\E[%p1%dD, cub1=\E[D, cud=\E[%p1%dB, cud1=\E[B,
#    cuf=\E[%p1%dC, cuf1=\E[C, cup=\E[%i%p1%d;%p2%dH,
#    cuu=\E[%p1%dA, cuu1=\E[A, dch=\E[%p1%dP, dch1=\E[P,
#    dl=\E[%p1%dM, dl1=\E[M, ech=\E[%p1%dX, ed=\E[J, el=\E[K,
#    el1=\E[1K, home=\E[H, hpa=\E[%i%p1%dG, ht=\E[I, hts=\EH,
#    ich=\E[%p1%d@, il=\E[%p1%dL, il1=\E[L, ind=^J,
#    indn=\E[%p1%dS, invis=\E[8m, kbs=^H, kcbt=\E[Z, kcub1=\E[D,
#    kcud1=\E[B, kcuf1=\E[C, kcuu1=\E[A, khome=\E[H, kich1=\E[L,
#    mc4=\E[4i, mc5=\E[5i, nel=\r\E[S, op=\E[39;49m,
#    rep=%p1%c\E[%p2%{1}%-%db, rev=\E[7m, rin=\E[%p1%dT,
#    rmacs=\E[10m, rmpch=\E[10m, rmso=\E[m, rmul=\E[m,
#    s0ds=\E(B, s1ds=\E)B, s2ds=\E*B, s3ds=\E+B,
#    setab=\E[4%p1%dm, setaf=\E[3%p1%dm,
#    sgr=\E[0;10%?%p1%t;7%;%?%p2%t;4%;%?%p3%t;7%;%?%p4%t;5%;%?%p6%t;1%;%?%p7%t;8%;%?%p9%t;11%;m,
#    sgr0=\E[0;10m, smacs=\E[11m, smpch=\E[11m, smso=\E[7m,
#    smul=\E[4m, tbc=\E[3g, u6=\E[%i%d;%dR, u7=\E[6n,
#    u8=\E[?%[;0123456789]c, u9=\E[c, vpa=\E[%i%p1%dd,
#*************************************************
#
# set boolean values
[[ "$_inclinc" = "" ]] && . /c/usr/local/include/incl.inc

# strings needed
[[ "$_stringsinc" != $TRUE ]] && $INCLUDE $STRINGS

# identify include
_tputinc=$TRUE

# Mingw bash ANSI = smso and rmso has a bug 
# so rmso need to be a new smso controlled
# by a global var to cover different runs
_smso_stat=$FALSE


tput(){
    # make echo the bash echo 
    [[ "$ECHO" != "echo" ]] && local ECHO="echo"

    # local vars
    local _cmd="$1"
    local _lval="$2"
    local _cval="$3"
    local _loop=0
    local _ret=0
    local _scrset=
    local _init=
    local _win=$FALSE

    # needed procs
    case "${_OS}" in
       "windows_NT"|"WindowsNT") 
          _win=$TRUE
          # we're running under some window bash
          if test -d "/c/windows"
          then
             _scrset="/c/Windows/system32/mode.com con: "
          else
             _scrset="/Windows/system32/mode.com con: "
          fi    
          ;;
       "Linux") 
             _win=$FALSE
             _scrset="export "
          ;;
    esac

    # version dependent procs
    local TR="$LBIN/tr"
    local SANE="$LBIN/stty sane"
    
    tolower "$_cmd"
    _cmd="$_retVal"

    case $_cmd in
        # win functions
        init|reset) # init or reset window (not exact Unix tput)
             if [ "$_win" == $TRUE ]
             then
                  $_scrset COLS=$_cval LINES=$_lval
             else
                  $_scrset COLUMNS=$_cval LINES=$_lval
             fi 
             ;;
        li|lines) # Get number of lines
             if [ "$_win" == $TRUE ]
             then
                 _ret="" #`$_scrset | $NAWK 'BEGIN {}; /Lines/ { print $2 }' `"
             else
                 _ret="" #`$ECHO $LINES`"
             fi
             ;;
        co|cols) # Get number of cols 
             if [ "$_win" == $TRUE ]
             then
                 _ret="`$_scrset | $NAWK 'BEGIN {}; /Columns/ { print $2 }' `"
             else
                 _ret="`$ECHO $COLUMNS`"
             fi
             ;;

        # position functions
        home)  # goto home
             $ECHO -e '\e[H'
             return 0
             ;;
        cup)
             if [ "$3" = "" ]
             then
                  $ECHO -e "tput: tput [cup row col] - cup h-pos missing !"
             elif [ "$2" = "" ]
             then
                  $ECHO -e "tput: tput [cup row col] - cup v-pos missing !"
             else
                  $ECHO -e  "\e[$2;$3H"
             fi
             return 0
             ;;
        cub)  # move cursor _lval left
             $ECHO -e "\e[$_lvalD"
             return 0
             ;;
        cub1) # move cursor one step left
             $ECHO -e "\e[D"
             return 0
             ;;
        cud)  # move cursor _lval down
             $ECHO -e "\e[$_lvalB"
             return 0
             ;;
        cud1) # move cursor one line down
             $ECHO -e '\e[B'
             return 0
             ;;
        cuf)  # move cursor _lval right
             $ECHO -e "\e[$_lvalC"
             return 0
             ;;
        cuf1) # move cursor one line right
             $ECHO -e '\e[C'
             return 0
             ;;
        cuu)  # move cursor _lval up
             $ECHO -e "\e[$_lvalA"
             return 0
             ;;
        cuu1) # move cursor one line up
             $ECHO -e '\e[A'
             return 0
             ;;
        hd)   # move down a half line - dec100, here full line
             $ECHO -e '\e[B'
             return 0
             ;;
        hpa)  # move cursor to col _lval.
             $ECHO -e "\e[$_lvalG"
             return 0
             ;;
        ht)   # tab to next tab stop (8 chars)
             $ECHO -e '\e[I'
             return 0
             ;;
        hts)  # set tab in all rows  current column
             $ECHO -e '\e[I'
             return 0
             ;;
        hu)   # move up a half line - dec100, here full line
             $ECHO -e '\e[A'
             return 0
             ;;
        ll)   # last line pos 1          
             _ret="`$_scrset | $SED \"s/ //g\" | $NAWK 'BEGIN {FS=\":\"};/Lines/ { print $2 }'`"
             $ECHO -e "\e[$_ret;1H"
             return 0
             ;;
        vpa)  # move to line _lval
             $ECHO -e "\e[$lvald"
             return 0
             ;;

        # erase functions
        clear|cl)  # clear screen
             $ECHO -e  '\e[2J'
             return 0
             ;;
        bt|cbt)  # backtab
             $ECHO -e "\e[?25h"
             return 0
             ;;
        dch)    # erase x chars
             $ECHO -e "\e[$_lvalP"
             return 0
             ;;
        dch1)   # erase  char
             $ECHO -e '\e[P'
             return 0
             ;;
        dl)     # delete _lval lines
             if [ "$_lval" = "" ] 
             then
                $ECHO -e "tput: tput [clear, el, dl, cup row col] - dl lines missing !"
             else
                $ECHO -e  "\e[$_lvalM"
             fi
             return 0
             ;;     
        dl1)    # erase one line
             $ECHO -e '\e[M'
             return 0
             ;;
        ech)    # erase _lval chars
             $ECHO -e "\e[$_lvalX"
             return 0
             ;;
        ed)     # erase to end of display
             $ECHO -e "\e[J"
             return 0
             ;;
        el)     # clear to end of line
             $ECHO -e  "\e[0K"
             return 0
             ;;    
        el1)    # clear line
             $ECHO -e  "\e[1K"
             return 0
             ;;    
        el2)    # clear line
             $ECHO -e  "\e[2K"
             return 0
             ;;    

        # manipulative functions
        bel)    # ring the bell
             $ECHO -e '\a'
             return 0
             ;;
        dim)    # set dim attrib
             $ECHO -e '\e[2M'
             return 0
             ;;
        blink)  # set blink attrib
             $ECHO -e '\e[5M'
             return 0
             ;;
        bold|cbt)  # set bright attrib
             $ECHO -e '\e[1M'
             return 0
             ;;
        cr)     # set carrage return
             $ECHO -e '^M'
             return 0
             ;;
        cbreak) # set carrage return
             $ECHO -e "\e[18h\e[7l"
             return 0
             ;;
        fsl)    # retrun from status line - dec100, not impl.
             $ECHO -e ""
             return 0
             ;;
        ich)    # insert _lval blank chars 
             $ECHO -e "\e[$_lval@"
             return 0
             ;;
        ich1)   # insert one blank chars 
             $ECHO -e "\e[1@"
             return 0
             ;;
        il)     # insert _lval blank lines
             $ECHO -e "\e[$_lvalL"
             return 0
             ;;
        il1)    # insert one blank line
             $ECHO -e '\e[L'
             return 0
             ;;
        ind)    # scroll forward (up) one line
             $ECHO -e '\e[1S'
             return 0
             ;;
        indn)   # scroll forward (up) _lval lines
             $ECHO -e "\e[$_lvalS"
             return 0
             ;;
        invis)  # set invisible attrib
             $ECHO -e '\e[8m'
             return 0
             ;;
        iv|civis)  # cursor invisible
             $ECHO -e "\e[?25L"
             return 0
             ;;
        nocbreak) # set carrage return
             $ECHO -e "\e[18l\e[7l"
             return 0
             ;;
        ve|cnorm|vs|cvvis)  # cursor visible
             $ECHO -e "\e[?25H"
             return 0
             ;;
        rc)     # restor cursor pos
             $ECHO -e '\e[8'
             return 0
             ;;
        rev)    # set reverse attrib
             $ECHO -e '\e[7m'
             return 0
             ;;
        rep)    # repeat char _lval _cval times - something missing so empty for now
             # $ECHO -e '$_lval\e[$_cvalb'
             $ECHO -e ""
             return 0
             ;;
        rmso)   # end stand out mode - 
             # due to some MINGW bash quirk, 
             # it is a reversed stand out
             # not correct [m 
             #. $ECHO -e '\e[7m'
             # bug correction for Mingw bash
             if [[ "$_win" == $TRUE ]] && [[ "$_smso_stat" == "$TRUE" ]]
             then
                 $ECHO -e '\e[7m'
                 _smso_stat=$FALSE
             else
                 $ECHO -e '\e[0m'
             fi
             return 0
             ;;
        rmul)   # unset underline attrib
             # note due to some MINGW bash quirk, 
             # it needs to be done in current process
             # not as a sub process
             . $ECHO -e '\e[m'
             return 0
             ;;
        rs1|rs2|rs3)  # reset to sane
             $SANE 2> /dev/null
             return 0
             ;;
        sc)     # save cursor pos
             $ECHO -e '\e[7'
             return 0
             ;;
        sgr)    # Sets attributes with _lval, but not used here,
             # use setf/setaf
             $ECHO -e ""
             return 0
             ;;
        sgr0)   # reset all attrib
             $ECHO -e '\e[0m'
             return 0
             ;;
        smul)   # set underline attrib
             $ECHO -e '\e[4m'
             return 0
             ;;
        smso)   # begin stand out mode
             # bug correction for Mingw bash
             if [[ "$_win" == $TRUE ]] && [[ "$_smso_stat" == "$FALSE" ]]
             then
                 $ECHO -e '\e[7m'
                 _smso_stat=$TRUE
             else
                 $ECHO -e '\e[7m'
             fi
             return 0
             ;;
        placeholder)  # set  attrib
             $ECHO -e " "
             return 0
             ;;
        setf|setaf)
         if [ "$_lval" = "" ] 
         then
              $ECHO -e "tput: tput [setaf color code] - code missing !"
         else
             case $_lval in  
             0) # set foreground black
              $ECHO -e '\e[30m'
              return 0
              ;;
             1) # set foreground red
              $ECHO -e '\e[31m'
              return 0
              ;;
             2) # set foreground green
              $ECHO -e '\e[32m'
              return 0
              ;;
             3) # set foreground yellow
              $ECHO -e '\e[33m'
              return 0
              ;;
             4) # set foreground blue
              $ECHO -e '\e[34m'
              return 0
              ;;
             5) # set foreground magenta
              $ECHO -e '\e[35m'
              return 0
              ;;
             6) # set foreground cyan
              $ECHO -e '\e[35m'
              return 0
              ;;
             7) # set foreground white
              $ECHO -e '\e[37m'
              return 0
              ;;
             9) # set foreground to default
              $ECHO -e '\e[39m'
              return 0
              ;;
             *)
              $ECHO -e "tput: tput [setaf color] !"
              return 0
              ;;
             esac
          fi
          ;;
        setb|setab)
         if [ "$_lval" = "" ] 
         then
              $ECHO -e "tput: tput [setab color code] - code missing !"
         else
             case $_lval in  
             0) # set background black
              $ECHO -e '\e[40m'
              return 0
              ;;
             1) # set background red
              $ECHO -e '\e[41m'
              return 0
              ;;
             2) # set background green
              $ECHO -e '\e[42m'
              return 0
              ;;
             3) # set background yellow
              $ECHO -e '\e[43m'
              return 0
              ;;
             4) # set background blue
              $ECHO -e '\e[44m'
              return 0
              ;;
             5) # set background magenta
              $ECHO -e '\e[45m'
              return 0
              ;;
             6) # set background cyan
              $ECHO -e '\e[45m'
              return 0
              ;;
             7) # set background white
              $ECHO -e '\e[47m'
              return 0
              ;;
             9) # set background to default
              $ECHO -e '\e[49m'
              return 0
              ;;
             *)
              $ECHO -e "tput: tput [setab color] !"
              return 0
              ;;
             esac
          fi
          ;;

        # not implemented
        acsc|am|bw|chts|da|db|dsl|ff|flash|enacs|eo|eslok|gn|hc|hs|hz|if|ih|in|it)
          #  not impl.
          $ECHO -e ""
          return 0
          ;;
        km|lm|lw|mc|mc0|mc1|mc2|mc3|mc4|mc5|mc5i|mir|mc5p|mgc|mrcup|msgr|npc|nrrmc)
          #  not impl.
          $ECHO -e ""
          return 0
          ;;
        os|pb|prot|nxon|nlab|rf|rfi|rmacs|rmam|rmcup|rmdc|rmir|rmkx|rmln|rmm|rmp)
          #  not impl.
          $ECHO -e ""
          return 0
          ;;
        rmpch|rep|rmir|rmkx|rmln|rmm|rmo|rmxon|smacs|smgl|smgr|smir|smkx|smln|smm)
          #  not impl.
          $ECHO -e ""
          return 0
          ;;
        tbc|tsl|uc|ul|vt|wind|wsl|xenl|xhp|xoffc|xon|xonc|xms|xsb|xt|u6|u7|u8|u9)
          #  not impl.
          $ECHO -e ""
          return 0
          ;;
        *)
          $ECHO -e "tput: tput [clear, el, dl lines, setaf/setab color, cup row col] !"
          ;;
    esac
    return $_ret
}   

_inc_profiler="tput: `date +\"%T\"`" 


