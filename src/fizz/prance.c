
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fizz.h>
#include <bio.h>
#include <libc.h>

/*
 * stolen from plan 9
 */

#ifndef PLAN9
#define ARGBEGIN        for((argv0? 0: (argv0=*argv)),argv++,argc--;\
                            argv[0] && argv[0][0]=='-' && argv[0][1];\
                            argc--, argv++) {\
                                char *_args, *_argt, _argc;\
                                _args = &argv[0][1];\
                                if(_args[0]=='-' && _args[1]==0){\
                                        argc--; argv++; break;\
                                }\
                                while(*_args) switch(_argc=*_args++)
#define ARGEND          }
#define ARGF()          (_argt=_args, _args="",\
                                (*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define ARGC()          _argc
char *argv0;
#endif

typedef struct Netpoint {
	Point p;
	int num;
	Chip *c;
	Signal *s;
} Netpoint;

extern int printcol;

int	debug;
Board	b;
Biobuf	out;
#define HSIZE	100000
Netpoint	*netpt[HSIZE];

Netpoint *
ptfind(Point p)
{
	int i;
	Netpoint *np=0;
	for (i = ((p.x*p.y)/10000)%HSIZE; np = netpt[i];) {
		if (p.x == np->p.x && p.y == np->p.y)
			return np;
		if (++i >= HSIZE)
			i = 0;
	}
	np = NEW(Netpoint);
	np->p = p;
	netpt[i] = np;
	return np;
}

void
walksig(Signal *s)
{
	int i;
	Netpoint *np;
	for (i = 0; i < s->n; i++) {
		np = ptfind(s->pins[i].p);
		np->s = s;
	}
}

void
walkchip(Chip *c)
{
	int i;
	Netpoint *np;
	for (i = 0; i < c->npins; i++) {
		np = ptfind(c->pins[i].p);
		np->c = c;
		np->num = i+1;
	}
}

void
outpkg(Package *pk)
{
	int i;
	Pin *p;
	for (i = 0; i < pk->npins; i++) {
		p = &pk->pins[i];
		Bprint(&out, "PIN\t%s\t%d\t%d\t%d\t%c\n", pk->name, i+1, p->p.x, p->p.y, p->drill);
	}
}

void
outconn(void)
{
	int i;
	Netpoint *np;
	for (i = 0; i < HSIZE; i++)
		if (np = netpt[i])
			if (np->s) {
				if (np->c)
				Bprint(&out, "NET\t%s\t%s\t%d\t%s\t%s\t%d\t%d\n",
				np->s->name, np->c->name, np->num,
				np->c->typename, np->c->type->pkgname,
				np->p.x, np->p.y);
			}
			else
				Bprint(&out, "NC\t%s\t%d\t%s\t%s\t%d\t%d\n",
				np->c->name, np->num,
				np->c->typename, np->c->type->pkgname,
				np->p.x, np->p.y);
}

void
main(int argc, char **argv)
{
	char *fname;
	int n;

	Binit(&out, 1, OWRITE);
	ARGBEGIN{
	case 'D':
		++debug; break;
	default:
		fprint(2, "usage: %s [files...]\n", argv0);
		exits("usage");
	}ARGEND
	fizzinit();
	f_init(&b);
	if(*argv)
		fname = *argv++;
	else
		fname = "/fd/0";
	do{
		if(n = f_crack(fname, &b)){
			fprint(2, "%s: %d errors\n", fname, n);
			exits("crack");
		}
	}while(fname = *argv++);	/* assign = */
	fizzplane(&b);
	if(n = fizzplace()){
		fprint(2, "warning: %d chip%s unplaced\n",
			n, n==1?"":"s");
	}
	if(fizzprewrap())
		exits("prewrap");
	symtraverse(S_PACKAGE, outpkg);
	symtraverse(S_SIGNAL, walksig);
	symtraverse(S_CHIP, walkchip);
	outconn();
	exits(0);
}
