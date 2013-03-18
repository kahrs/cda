LIB=libfizz.a
OFILES=alloc.o \
	board.o \
	chip.o \
	chkvsig.o \
	chtype.o \
	coords.o \
	csmap.o \
	cutout.o \
	crack.o \
	drill.o \
	fizz.o \
	fizzprewrap.o \
	format.o \
	getopt.o \
	graphic.o \
	hand.o \
	keymap.o \
	layout.o \
	mst.o \
	mst3.o \
	net.o \
	netlen.o \
	nn.lin.o \
	package.o \
	pinholes.o \
	pintab.o \
	pins.o \
	pkgclass.o \
	place.o \
	plane.o \
	positions.o \
	prmst.o \
	prseq.o \
	rect.o \
	route.o \
	symtab.o \
	tsp.o \
	tspe.o \
	tt.o \
	type.o \
	vsig.o \
	wires.o \
	wrap.o\
	unix.o\

HFILES=/sys/include/cda/fizz.h\
	symtab.h\

CFLAGS=

all: $(OFILES)
	ar rv $(LIB) $(OFILES) 
	ranlib $(LIB)

install:	$(LIB)
	#cp $prereq /$objtype/lib/cda/libfizz.a

clean:
	rm -f $(OFILES)

nuke:
	make clean
	rm -f libfizz.a[oS]
