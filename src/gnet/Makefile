
OFILES=thing.o\
	conn.o\
	wire.o\
	text.o\
	box.o\
	master.o\
	main.o\

HFILES=thing.h\
	text.h\
	conn.h\
	wire.h\
	box.h\
	master.h\

gnet: $(OFILES)
	g++ $(OFILES) -o gnet

.c.o: $(HFILES)
	g++ -c $(CFLAGS) $(INCLUDES) -o $@ $< 

