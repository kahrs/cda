/*
    module: ptime.c
    purpose: routines for dealing with processor time usage
*/

#include <stdio.h>

#ifdef IBM
extern int second(), inter();
#endif

/*
    ptime -- return a floating point number which represents the
    elasped processor time in seconds since some constant reference
*/
double ptime()
{
    double time;

#ifdef IBM				/* IBM 3081 VM/CMS */
    inter(second, &time);
#else
#ifdef UNIX				/* Berkeley Unix 4.1/4.2 bsd */
    struct tms {int user, sys, cuser, csys;} buffer;
    times(&buffer);
    time = buffer.user / 60.0;
#else
#ifdef VMS				/* VAX/VMS */
    struct tms {int user, sys, cuser, csys;} buffer;
    times(&buffer);
    time = buffer.user / 100.0;
#else
    time = 0.0;				/* anybody else */
#endif
#endif
#endif
    return time;
}

/*
    print_time -- massage a floating point number which represents a
    time interval in seconds, into a string suitable for output
*/
char *print_time(time)
double time;
{
    static char s[40];
#ifdef IBM
    (void) sprintf(s, "%4.2f sec", time);
#else
    (void) sprintf(s, "%3.1f sec", time);
#endif
    return s;
}
