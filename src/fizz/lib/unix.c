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
/*
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

void
print(char *fmt, char *z, ...)
{
	va_list args;
	char buf[2048];

	va_start(args, z);
	vsprintf(buf, fmt, args);
	write(1, buf, strlen(buf));
	va_end(args);
}

void
fprint(int fd, char *fmt, char *z, ...)
{
	va_list args;
	char buf[2048];

	va_start(args, z);
	vsprintf(buf, fmt, args);
	write(fd, buf, strlen(buf));
	va_end(args);
}

void
sprint(char *buf, char *fmt, char *z, ...)
{
	va_list args;

	va_start(args, z);
	vsprintf(buf, fmt, args);
	va_end(args);
}
*/

void
strconv(char *s, void *p)
{
	printf("strconv %s\n", s);
	abort();
}

