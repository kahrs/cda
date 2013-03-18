OFILES=lde.tab.o\
	comp.o\
	field.o\
	lex.o\
	main.o\
	out.o\
	tree.o\

HFILES=dat.h\
	fns.h\
	gram.h\

YFILES=lde.y\

YFLAGS=-d -v
CFLAGS=

#gram.h: lde.tab.h
#	if (! test -f gram.h || ! cmp -s lde.tab.h gram.h) then
#		cp lde.tab.h gram.h
#	fi

lde: $(OFILES)
	cc $(OFILES) -o lde

lde.tab.o: lde.tab.c dat.h fns.h
	cc -c $(CFLAGS) -DYYMAXDEPTH=2000 lde.tab.c

lde.tab.h: $(YFILES)
	bison $(YFLAGS) $(YFILES)

lde.tab.c: $(YFILES)
	bison $(YFLAGS) $(YFILES)

clean:
	rm -f *.[oS] [oS].out lde.tab.? y.debug gram.h y.output lde

