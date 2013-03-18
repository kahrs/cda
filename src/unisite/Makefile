LIB=/sys/lib/cda
CFLAGS=-I../include -g
LIBS=../libbio/libbio.a ../libfmt/libfmt.a ../libutf/libutf.a

all:	urom.codes urom.notes

urom.codes: dump alg.sys
	./dump -8 -P -s alg.sys > urom.codes

urom.notes: notes devfnote.sys
	./notes <devfnote.sys > urom.notes

dump: dump.c
	$(CC) -c $(CFLAGS) $@.c
	$(CC) $@.o -o $@  $(LIBS)

notes:  notes.c
	$(CC) -c $(CFLAGS) $@.c
	$(CC) $@.o -o $@  $(LIBS)

clean:
	rm -f *.o urom.codes urom.notes dump notes

install: urom.codes urom.notes

nuke:	clean
