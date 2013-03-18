/*
    Program:	espresso.c

    Author: 	Richard Rudell
    Address:	Graduate Student
		231 Cory Hall
		University of California
		Berkeley, CA 94720

    Revision:	Version #2.0, December 28, 1984
		Multiple-valued variable support plus a general
		re-write of most routines for efficiency.

		Version #2.1, March 2, 1985
		expand rewritten to be a little more clear; essen
		didn't listen to the advice of expand for non-
		essentials; minor changes here and there to support
		a 16-bit integer machine (i.e., IBM PC)

    Description:

    This program implements a multiple-valued extension of the
    Espresso-II algorithms for two-level Boolean Minimization described
    in the book "Logic Minimization Algorithms for VLSI Synthesis" by
    R. Brayton, G. Hachtel, C. McMullen, and A. Sangiovanni-
    Vincentelli, Kluwer Academic Publishers, 1984.

    Two supporting documents (ESPRESSO(CAD1) and ESPRESSO(CAD5))
    describe the command line options, and input/output file formats.

    This is a re-write of the original ESPRESSO-IIC program first
    distributed by Berkeley ILP in the fall of 1984.  The major
    change is that multiple-valued variables are directly supported.


Portability Considerations:
    It is intended that this file be compiled as a single C program.

    Standard C library routines needed:

	I/O: printf(), sprintf(), scanf(), fscanf(), fflush(), 
		 fclose(), fopen(), getc(), putc(), ungetc()
	Dynamic memory allocation: malloc()
	String manipulation: strcpy(), strlen(), strcmp(), atol()
	Character identification: isspace()
	Efficient sorting: qsort()
	Other: exit()

    The routine ptime() can be connected to the appropriate magic to
    get at the number of seconds of processor time used by the process.
    Compile with "-DUNIX" for Berkeley 4.1/4.2 bsd, "-DVMS" for
    VAX/VMS, or "-DIBM" for IBM VM/CMS to get the correct processor
    time routine.  ptime() will return 0.0 for all other machines.
*/

#include <stdio.h>
#include <ctype.h>
#define EXTERN
#define INIT
