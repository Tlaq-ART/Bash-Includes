##########################################################
# Bash Includes : Defining the included functions usage
# function list
#                   
# Legal       : This text file is part of Bash 
# boilerplate   Includes Library. 
#               They are relesed under Creative Common
#               with notice of origin version. 
#               Any use of the Bash Include Libraries
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

Incl - set up basic parameters for the libraries to work
incl.inc:	cls  	        - 

Env - no functions, just a lot of enviroment definitions and processes as variables

Std - a file for frequent used functions, including MS Windows stuff
std.inc:	signal  	    - a function standardizing format of bash trap command
std.inc:	getDir   	    - get the current dir and truncate the path
std.inc:	fullName   	  - get the fully expanded name of a file given with truncated name
std.inc:	wincp_on  	  - macros to handle windows AutoHotKey calls
std.inc:	wincp_off  	  - macros to handle shutdown of windows AutoHotKey

Stdio - follows C stdio 
stdio.inc:	open  	    - open console and return file descriptor. Bash only allows 1-9
stdio.inc:	fopen  	    - open a file and return file descriptor. Bash only allows 1-9
stdio.inc:	close  	    - close console
stdio.inc:	fclose  	  - close file descriptor
stdio.inc:	getc   	    - get a character in raw mode (allows for meta chars)
stdio.inc:	fgetc   	  - get a character from file in raw mode (allows for meta chars)
stdio.inc:	getchar   	- get a character
stdio.inc:	getchars   	- get a string 64 byte long
stdio.inc:	gets   	    - get a string 255 byte long
stdio.inc:	scanf   	  - read a console text stream and format it
stdio.inc:	fscanf   	  - read a file text stream and format it
stdio.inc:	getline   	- get a full line
stdio.inc:	putc  	    - put a character to console in raw mode
stdio.inc:	fputc  	    - put a character to file in raw mode
stdio.inc:	putchar  	  - put a character to console 
stdio.inc:	puts  	    - put a string to console 
stdio.inc:	printf  	  - print a string to console     
stdio.inc:	fprintf  	  - print a string to a file     
stdio.inc:	putline  	  - put a full line
stdio.inc:	fseek  	    - seek a file position
stdio.inc:	flushbuf   	- flush the bash read buffer

Strings - called as per nawk equivalents
strings.inc:	split  	  - split a string into an user given array
strings.inc:	index  	  - find position of a sub string in a string
strings.inc:	length  	- find length of a string
strings.inc:	strlen  	- c alternativ to length (macro of length)
strings.inc:	substr  	- find a sub string in a string at given position
strings.inc:	esubstr  	- a non-nawk function, returning the x last chars
strings.inc:	match  	  - returning position of substring
strings.inc:	gsub  	  - exchange all instances of a string in the text with another 
strings.inc:	sub  	    - exchange first instance of a string in the text with another 
strings.inc:	sprintf  	- format variables to a string and save in a variable
strings.inc:	toupper  	- convert to upper case
strings.inc:	tolower  	- convert to lower case
strings.inc:	join  	  - join two strings
strings.inc:	prefix  	- get a prefix
strings.inc:	suffix  	- get a suffix

Proc - process control to open new "real" windows for called programs - incl. MS Windows
proc.inc:	fork  	      - forking a new program into it own window, Linux call not tested

Tput - tput as bash include file
tput.inc:	tput  	      - a "full" implementation of tput as a function call.
                          Conformance is dependent on platform ANSI implementation
                          
Ncurses - format based on C ncurses calls, except that vars comes after func: "newwin y x h w"
NOTE: this library is in an alpha state, several functions not yet tested. 
ncurses.inc:	initscr  	- init curses
ncurses.inc:	endwin   	- close curses
ncurses.inc:	newwin  	- make a new window
ncurses.inc:	delwin  	- delete a window
ncurses.inc:	box   	  - draw a box (just a basic text char box)
ncurses.inc:	refresh  	- refresh a window after changes
ncurses.inc:	touchwin  - touch a canged window to force a refresh
ncurses.inc:	addch  	  - add a char
ncurses.inc:	attroff  	- turn of attributes  (just one basic, gray background)
ncurses.inc:	attron  	- turn on attributes
ncurses.inc:	attrset  	- set attribute (not tested)
ncurses.inc:	beep  	  - terminal to beep
ncurses.inc:	clear  	  - clear screen
ncurses.inc:	clrtobol  - clear to beginning of line
ncurses.inc:	clrtobot  - clear to beginning of line
ncurses.inc:	clrtoeol  - clear to end of line
ncurses.inc:	delch  	  - delete char  (not tested)
ncurses.inc:	deleteln  - delete to end of line  (not tested)
ncurses.inc:	getch  	  - get character in raw mode
ncurses.inc:	getstr  	- "macro" of getch to get a string 
ncurses.inc:	getwd  	  - "macro" of getch to get a word
ncurses.inc:	insch  	  - insert character  (not tested)
ncurses.inc:	insertln  - insert line  (not tested)
ncurses.inc:	mvaddch  	- move to position and add character  (not tested)
ncurses.inc:	mvaddstr  - move to position and add character  (not tested)
ncurses.inc:	mvclrtoeol - move and clear to end of line  (not tested)
ncurses.inc:	mvclrtobol - move and clear to beginning of line  (not tested)
ncurses.inc:	mvclrtobot - move and clear to end of screen  (not tested)
ncurses.inc:	mvdelch  	- move and delete char  (not tested)
ncurses.inc:	mvinsch  	- move and insert char  (not tested)
ncurses.inc:	addstr  	- add a string
ncurses.inc:	move  	  - move
ncurses.inc:	mvcur  	  - move cursor  (not tested)
ncurses.inc:	wmove  	  - move in defined window  (not tested)
ncurses.inc:	mvwaddstr - move and add a string
ncurses.inc:	tput  	  - mini implementation of tput
ncurses.inc:	errTxt  	- a standardised error window
