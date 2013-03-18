FILES= wrap\
	artwork\
	fizzcvt\
	check\
	list\
	draw\
	pkgplot\
	clip\
	ring\
	getparts\
	prance

LIBS = lib/libfizz.a  ../libbio/libbio.a  ../libfmt/libfmt.a ../libutf/libutf.a -lm
CFLAGS=-g  -I../include

all: $(FILES)

wrap: wrap.o
	gcc $< $(LIBS) -o $@

artwork: artwork.o
	gcc $< $(LIBS) -o $@

fizzcvt: fizzcvt.o
	gcc $< $(LIBS) -o $@

check: check.o
	gcc $< $(LIBS) -o $@

list: list.o
	gcc $< $(LIBS) -o $@

draw: draw.o
	gcc $< $(LIBS) -o $@

pkgplot: pkgplot.o
	gcc $< $(LIBS) -o $@

clip: clip.o
	gcc $< $(LIBS) -o $@

prance: prance.o
	gcc $< $(LIBS) -o $@

ring: ring.o
	gcc $< $(LIBS) -o $@

getparts: getparts.o
	gcc $< $(LIBS) -o $@

clean:
	rm -f *.o $(FILES)
