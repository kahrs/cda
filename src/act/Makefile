
OFILES=tree.o\
	mach.o\
	par.o\
	util.o\
	act.o\
	unix.o\

HFILES=dat.h\
	symbols.h\
	y.tab.h\

YFLAGS=-d -v
YFILES=par.y

act: $(OFILES) $(HFILES) $(YFILES)
	cc $(OFILES) -o act

par.tab.c:	$(YFILES)
	bison $(YFLAGS) $(YFILES)

act.c:	act.mt
	../twig/twig -d act.mt
	mv walker.c act.c

act.mt:	pro.mt def.awk gates comb
	awk -f def.awk gates comb | cat pro.mt > act.mt

nuke:
	rm -f act.mt act.c par.c symbols.h

clean:
	rm -f $(OFILES) act par.tab.c

wc:
	wc dat.h par.y pro.mt util.c tree.c mach.c def.awk gates

install: act
	install -m 0644 act /usr/local/cda

