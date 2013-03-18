OFILES=parser.tab.o\
	part.o\
	alloc.o\
	utility.o\
	scanner.o\
	unix.o

HFILES=part.h

LFILES=scanner.l

YFILES=paddle.y

CFLAGS=
YFLAGS=-v -d

INCLUDES=-I../include
LEX=flex
LIBS=../libfmt/libfmt.a ../libutf/libutf.a

.c.o:  $(HEADERS)
	gcc -c $(CFLAGS) $(INCLUDES) -o $@ $<

all: xpart

xpart: $(OFILES)
	$(CC) $(OFILES) $(LIBS) -o $@

scanner.c:	$(LFILES) parser.tab.h
	$(LEX) -t $(LFILES) > $@

parser.tab.h parser.tab.c parser.debug: $(YFILES)
	bison $(YFLAGS) -b parser $(YFILES)

parser.tab.o: parser.tab.c 
	$(CC) -c $(CFLAGS) -DYYDEBUG $(INCLUDES) parser.tab.c

clean:
	rm -f *.[oS] parser.tab.? parser.debug parser.output xpart parser.* scanner.c

