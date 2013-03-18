OFILES=main.o\
	pintab.o\
	wire.o\
	net.o\
	eqelim.o\
	diff.o\
	dumbdiff.o\
	order.o\
	quickdiff.o\
	getfields.o\

HFILES=rework.h\

CC=c++
LD=c++

CFLAGS=-D_RESEARCH_SOURCE

rework: $(OFILES) $(LIB)
	$(LD) -o rework $(OFILES) $(LIB) -lv

test:	rework
	rework -d -q k?
	echo UN; cat UN.wr; echo RE; cat RE.wr

