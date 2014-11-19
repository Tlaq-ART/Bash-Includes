Shell Includes Library v1.0 
---------------------------
Date    :   Nov 11, 2014 
Author  :   Kurt Magnusson
Contact :   tlaq {.} se {/a} mailform {.} html  
Licence :   Creative Common with identification of origin version
---------------------------

Shell Include Library started in 2005, when writing a secure sftp data
switch for an employer needing to fulfill some Sarbanes-Oxley
requirements. Initially it was just an path, command and environment
variable include file, standardizing a couple of sftp controlling shell
scripts. 

Over the years it evolved to, during autum 2014, needing to support a
command based document manager, handling some 7 Gb of texts, mostly
pdfs'.  The manager is a 600 line shell script with some screen
graphical properties, using ANSI screen codes. 

However, writing it, I noted I was not consequent, but used different
scripting solution in different functions. After deciding to be
consistent, I rewrote the old include files to give me a C-/awk-like
function set, easy to remember and use.  

Note that since bash do not allow function data returns, like c-functions
this library had to resort to a global return variable "_retVal". 
Hopefully this name will not collied with user name standards. It is 
also important that it is copied to a user variable directly after the 
call ended. Otherwise use of another function will write over the data.

Example:
#*******************
# Function
someFunc(){
	_retVal="Some text produced by this function"
}

nextFunc() {
	_retVal=$(($1-$2))
}

#*******************
# Main
someFunc
result=$_retVal
nextFunc  22 8
sum=$_retVal
echo "Functions responded with: $result and $sum"

This problem can be solved by the design: 
#*******************
# Functions
someFunc(){
	echo "Some text produced by this function"
}

nextFunc() {
	echo $(($1-$2))
}

#*******************
# Main
result=$(someFunc)
sum=$(nextFunc 22 8)
echo "Functions responded with: $result and $sum"

But, while this looks neater, it has a performance penalty of ca 20-35%,
since the script is forking a copy, running the calls and returns. In
some cases, using read or doing screen operations, this also can wreak
havok, since some data gets lost in the process switch. 

As is, the include libraries is developed under Mingw win32 bash, using
Mingw Gnu tools as well as the old UnxUtils toolset. Is currently using
a mix of these two, since a couple of UnxUtils  commands have some
issues, primarly gawk and expr.
 
However, writing it for Mingw/UnixUtils do mean that some of the library
includes some Windows idiosyncraties in them, that most Unix/Linux users
probably like to clean out.

Note that some of my code is extremely crude, this to retain some
readability. A shellscript guru is likely to reduce the code with some
30%, but that is not my intent. I want a working code, that is  easy to
maintain.

The ncurses library has been influenced by Mr Dana French (Mt.Xia) 1993
Ksh93 shell function library. However, not getting it to work properly,
needing the use of a tput command, all made it painfully slow. Me also
needing to create sub windows, lead to implementing my own curses
framework. Not as elegant as Mr French's curses, but working, I do owe
him for the re-use of a number of additative commands, on top of my
basic command set. 

Initially I was reluctant loaning these commands, but since Mr Frensh
code do not come with restrictions and my underlying framework is
completely different, I in the end chosed the better version of these
commands. Any user of my implementation, please note Mr French's
contribution and if any bugs, well, my code is likely the culprit. 

To finish of these ramblings, some legal comments; any use of the Shell
Include Libraries is "as is" and on your own responsibility. If in
question about legal responsibility, you not wanting it, do not use
these libraries. I just posted them as inspiration with what one can do
with shellscripts.  

In the end, I got six libraries together:

incl.inc    : "include" is the primary include file setting some 
	            basic parameters, to simplify the other includes.

env.inc     : "environment" is the file defining environment 
	            variables and absolut paths to a number of common unix
	            commands as well as some windows processes used when
	            running Mingw bash.

std.inc     : a bad name choice, it is not an equivalent to C std.h
	            but an include that defines a couple of freequent used
	            functions, I like to have available. 

proc.inc    : "Process" control, how to, primarily under windows, be 
	            able to spawn a process as a new window. There is an
	            untested gnome-terminal linux version in the file.

stdio.inc   : mimicing C "stdio", with its get and put etc. However,
	            note that the file versions, fgetc/fputc etc, is 
	            defined, but yet not tested. 

strings.inc : shell version of awk "string" functions, where only a
	            couple of "expr index" and "expr match" is external
	            commands used. "expr" used is the Mingw's version since
	            UnxUtil "expr" match  and index commands fails. 

tput.inc    : there is no win Mingw "tput", neither a UnxUtil so 
	            therefore a shell script based "tput" was written.
	            This include is the include-version, covering most ANSI
	            mapped tput sub commands. However, Mingw Bash do not 
	            fully support bash all ANSI codes. 

ncurses.inc : Dana French, Mt. Xia, wrote in the 1990:ies a very 
	            intriguing shell script include file, that with use 
 	            of tput created a partial shellscript ncurses 
	            capability. However, needing tput and written for 
	            Ksh93, it does not work very well with Mingw Bash and 
	            a shellscript tput. Is simply to slow. 

	            It also didn't support independent sub windows, like 
	            real ncurses do. This ncurses was therefor written to 
	            take care of these limitations in my environment. It 
	            has initwin, newwin, refresh, delwin, endwin. As done 
	            here, 4 concurrent windows is max, but that is a 
	            definition issue in ncurses.inc. 

	            Since Dana French made very elegant implementations 
	            of the actual commands, most is his re-used, stupid 
	            not to. He has, as fared I can find, not conditionaled
	            his code. No boilerplate limitations. However, the key 
	            commands, used by Mr French's; move, wmove, mvcur, 
	            addstr, mvaddstr is completely different, based on my 
	            windows model. 

	            This ncurses implementation do have some limitations:
	            - It is possible to write outside active window. To 
	              check range proved to time consuming. 
	            - Part of the code is very crude, but since I just 
	              needed parts of it, it has to wait for further 
	              refinement. 
	            - A single window is stored in a shell variable, in
	              Mingw Bash these has a limit of 4096 bytes, with some 
	              odd results if the window gets to big (data added but
	              not shown). Using much graphics, it will easy be 4K. 
	            - The box graphics is extremely rudimentary and crude.
	              Its "*.=" and "|" only. Cmd.com, Mingw + Bash + non 
	              US ASCII is a very non-productive semigraphic mix.


Installation:

Recommended that you extract the files in /usr/local/include. The files,
particular incl.inc and env.inc, is currently reflecting such
installation. If needing to move, change in incl.inc and env.inc to
adapt to new directory. The library should not need root access, but is
open for any user.  

To use the libraries, if loading incl.inc first and rest after, a simple
case:

#!/bin/bash
#-------------------
# my_script
#
#-------------------
source /usr/local/include/incl.inc
$INCLUDE $ENV
$INCLUDE $STD
$INCLUDE $STDIO
$INCLUDE $STRINGS
$INCLUDE $PROC
$INCLUDE $NCURSES

# Rest of the script below.

However, it is recommended to rather do conditional loads, as per below:


[[ "$_envinc" != $TRUE ]] && $INCLUDE $ENV

Some of the the include files do use some of the others, like ncurses
using env.inc and strings.inc. Using a conditional load, you do not load
the same file twice. This is done by each setting a profile variable in
each include to true, then tested for. If not true, we load the include.


incl.inc sets some basic variable macros, as $INCLUDE, for the include
files, as well as the boolean $TRUE and $FALSE, both same as for C
true/false. This to avoid some of the original bash true/false command
quirks. 

Otherwise, there is no other instruction, than to read the file for
command context. 

Happy coding
Kurt Magnusson
