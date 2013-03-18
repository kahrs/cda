PROGNAME=place
OFILES=attr.o\
	but1.o\
	clip.o\
	draw.o\
	gnotfix.o\
	grid.o\
	insert.o\
	keepout.o\
	map.o\
	menu.o\
	move.o\
	overlay.o\
	plane.o\
	rcv.o\
	rot.o\
	signal.o\
	timprove.o\
	tmain.o\
	transform.o\
	mhit.o\
	icons.o\
	unix.o\

HFILES=term.h\
	proto.h\
	menu.h\

INCLUDES = -I../include -I/usr/include/posix
LIBS = ../libXg/libXg.a ../libfmt/libfmt.a ../libutf/libutf.a
CFLAGS=-D_LIBXG_EXTENSION -g

#	set this if your X libraries are in different locations
#	or if you need extra libraries to load with X11 applications
XLIBS=-lXt -lX11

OS=-Ddebian

all: $(PROGNAME)

$(PROGNAME): $(OFILES)
	gcc -o $(PROGNAME) $(OFILES) $(LIBS) $(INCLUDES) $(LDFLAGS) $(XLIBS)

$(OBJECTS): Makefile

.c.o:  $(HEADERS)
	gcc -c $(CFLAGS) $(INCLUDES) -o $@ $< 

clean:
	rm -f *.o core

nuke:	clean
	rm -f $(PROGNAME)

install:	$(PROGNAME)
	install -D $(PROGNAME) 

