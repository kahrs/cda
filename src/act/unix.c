#include <stdarg.h>
#include <stdlib.h>

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
	printf(buf);
	va_end(args);
}

