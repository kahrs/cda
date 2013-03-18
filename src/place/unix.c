/* Copyright (c) 1992 AT&T - All rights reserved. */
#include	<u.h>
#include	<libc.h>
#include	<sys/stat.h>
#include	<sys/wait.h>
#include	<signal.h>

#ifdef	NEEDVARARG
#include	<varargs.h>
#else
#include	<stdarg.h>
#endif

#define TMPDIR "/var/tmp"

static Rune l1[] = { '{', '[', '(', '<', 0253, 0};
static Rune l2[] = { '\n', 0};
static Rune l3[] = { '\'', '"', '`', 0};
Rune *left[]= { l1, l2, l3, 0};

static Rune r1[] = {'}', ']', ')', '>', 0273, 0};
static Rune r2[] = {'\n', 0};
static Rune r3[] = {'\'', '"', '`', 0};
Rune *right[]= { r1, r2, r3, 0};

int
statfile(char *name, ulong *dev, ulong *id, long *time, long *length, long *appendonly)
{
	struct stat dirb;

	if (stat(name, &dirb) == -1)
		return -1;
	if (dev)
		*dev = dirb.st_dev;
	if (id)
		*id = dirb.st_ino;
	if (time)
		*time = dirb.st_mtime;
	if (length)
		*length = dirb.st_size;
	if(appendonly)
		*appendonly = 0;
	return 1;
}

int
statfd(int fd, ulong *dev, ulong *id, long *time, long *length, long *appendonly)
{
	struct stat dirb;

	if (fstat(fd, &dirb) == -1)
		return -1;
	if (dev)
		*dev = dirb.st_dev;
	if (id)
		*id = dirb.st_ino;
	if (time)
		*time = dirb.st_mtime;
	if (length)
		*length = dirb.st_size;
	if(appendonly)
		*appendonly = 0;
	return 1;
}

void
hup(int sig)
{
	exit(1);
}

int
notify (void(*f)(void *, char *))
{
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, hup);
	signal(SIGPIPE, SIG_IGN);
#ifdef	v10
	close(3);		/* redirect v10 /dev/tty */
	open("/dev/null", 2);
#endif
	return 1;
}

void
notifyf(void *a, char *b)	/* never called */
{
}

/*
 *	if your system doesn't have tempnam(), substitute the following
 *	code for this function:
 *	FILE *f;
 *	f = tmpfile();
 *	if (f == 0)
 *		return -1;
 *	return fileno(f);
 *
 *	we use tempnam to allow temp files to be allocated in the
 *	most efficient place; nodes with disks may mount /usr/tmp
 *	remotely, causing excessive network traffic.  place
 *	the temp files locally, if possible.
 */
int
newtmp(int i)
{
	char *s;
	FILE *f;
	extern char *tempnam(const char*, const char*);

	s = tempnam(TMPDIR, "place.");
	if (!s)
		return -1;
	f = fopen(s, "w+");
	if (!f)
		return -1;
	unlink(s);
	return fileno(f);
}

int
waitfor(int pid)
{
	int wm;
	int rpid;

	do; while((rpid = wait(&wm)) != pid && rpid != -1);
	return (WEXITSTATUS(wm));
}

void*
emalloc(ulong n)
{
	void *p;

	if (n < sizeof(int))
		n = sizeof(int);
	p = malloc(n);
	if(p == 0)
		panic("malloc fails");
	memset(p, 0, n);
	return p;
}

void*
erealloc(void *p, ulong n)
{
	p = realloc(p, n);
	if(p == 0)
		panic("realloc fails");
	return p;
}

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
/*	termwrite(buf);*/
	write(2, buf, strlen(buf));
	va_end(args);
}

