OFILES=xnf.o

CFLAGS=-I../include -g

LIBS=../libbio/libbio.a ../libfmt/libfmt.a ../libutf/libutf.a

all: xnf

xnf: $(OFILES)
	cc $(OFILES) $(LIBS) -o xnf 

clean:
	rm -f *.o xnf

install:

