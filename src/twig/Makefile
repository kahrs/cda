
TARG=twig
OFILES=twig.tab.o\
	sym.o\
	path.o\
	machine.o\
	trees.o\
	lex.o\
	code.o\
	io.o\
	mem.o\
	util.o\

YFILES=twig.y\

HFILES=common.h\
	code.h\
	sym.h\
	machine.h\
	mem.h\
	machcomm.h\
	twig.tab.h\

WFILES=walker.stdio\
	walker.libc\
	walker.ansi\

YFLAGS=-d
CFLAGS=-g

all:	twig walker.ansi

twig: $(OFILES)
	cc -o twig $(OFILES)

twig.tab.h: $(YFILES)
	bison $(YFLAGS) $(YFILES)

sim.o: twig.tab.h sim.c
	cc $(CFLAGS) -c sim.c

twig.tab.o: twig.tab.h
	cc $(CFLAGS) -c twig.tab.c

walker.ansi: machcomm.h walker.h rawwalker.ansi
	cat machcomm.h walker.h rawwalker.ansi > walker.ansi

install:
	cp $(WFILES) /sys/lib/twig

nuke:
	make clean
	rm -f $(WFILES)

clean:
	rm -f *.o twig

