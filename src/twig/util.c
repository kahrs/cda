#ifdef PLAN9
#include <u.h>
#include <libc.h>
#endif
#ifndef PLAN9
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif

void dprint(char *z, ...);

void
exits(char *message)
{

        if (message == 0)
                exit(0);
        else
                exit(1);
}

void
dprint(char *z, ...)
{
        va_list args;
        char buf[2048];

        va_start(args, z);
        vsprintf(buf, z, args);
        fprintf(stderr, "%s", buf);
        va_end(args);
}
