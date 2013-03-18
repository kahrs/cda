OFILES=edit.o\
	file.o\
	hmain.o\
	himprove.o\

HFILES=host.h\
	proto.h\

INCLUDES=-I../../include
LIBS=../lib/libfizz.a ../../libfmt/libfmt.a ../../libutf/libutf.a

.c.o:  $(HEADERS)
	gcc -c $(CFLAGS) $(INCLUDES) -o $@ $<

hplace: $(OFILES)
	cc $(OFILES) $(LIBS) -o hplace

