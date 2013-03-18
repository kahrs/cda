PROGNAME = graw
LIBS = ../libXg/libXg.a
INCLUDES = -I../include -I/usr/include/posix
LDFLAGS = 
OBJECTS=file.o line.o graw.o util.o mhit.o 
HEADERS=thing.h event.h menu.h 
CFLAGS=-D_LIBXG_EXTENSION -g

OS=-Ddebian

#	set this if your X libraries are in different locations
#	or if you need extra libraries to load with X11 applications
XLIBS=-lXt -lX11

all: $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	gcc -o $(PROGNAME) $(OBJECTS) $(LIBS) $(INCLUDES) $(LDFLAGS) $(XLIBS)

$(OBJECTS): Makefile

.c.o:  $(HEADERS)
	gcc -c $(CFLAGS) $(INCLUDES) -o $@ $< 

clean:
	rm -f *.o core

nuke:	clean
	rm -f $(PROGNAME)

install:	$(PROGNAME)
	install -D $(PROGNAME) 

