CFLAGS = -DUNIX -O
BINDIR = .

HEADERS = set.h espresso.h

CFILES = main.c espresso.c compl.c essen.c expand.c gasp.c irred.c reduce.c\
cofactor.c contain.c cvrm.c cvrmisc.c cvrin.c cvrout.c explode.c opo.c\
pair.c pop.c mincov.c setc.c set.c memory.c ptime.c

OFILES = main.o espresso.o compl.o essen.o expand.o gasp.o irred.o reduce.o\
cofactor.o contain.o cvrm.o cvrmisc.o cvrin.o cvrout.o explode.o opo.o\
pair.o pop.o mincov.o setc.o set.o memory.o ptime.o

espresso: $(OFILES)
	csh -f -c "cc $(CFLAGS) -o $(BINDIR)/espresso $(OFILES)"
$(OFILES): espresso.h set.h
esptype.h: $(CFILES)
	csh -f -c "global-tags $(CFILES) >esptype.h"

list:
	print esphead.c $(HEADERS) esptype.h $(CFILES)
ESP: ESP.c
	csh -f -c "cc -DUNIX -O -pg -o $(BINDIR)/espresso-pg ESP.c"
ESP.c: $(HEADERS) $(CFILES) esphead.c esptype.h
	csh -f -c "catf $(HEADERS) esptype.h $(CFILES) ucbqsort.c \
	    | fgrep -v '#include' | catf esphead.c - >ESP.c"
lint:
	lint -DUNIX $(CFILES)

ibm: ESP.c
	ibmcc -s -prm IBM 1 ESP.c
	sleep 30
	ibmld esp second

install:
	csh -f -c "cp $(BINDIR)/espresso ~cad/bin/espresso"
	csh -f -c "cp ./espresso.1 ~cad/man/man1/espresso.1"
	csh -f -c "cp ./espresso.5 ~cad/man/man5/espresso.5"


distclean:
	-rsh ${DESTHOST} -n "rm -rf ${DESTDIR}/bin/espresso \
		${DESTDIR}/man/man1/espresso.1 ${DESTDIR}/man/man5/espresso.5 \
		${DESTDIR}/src/espresso

disttape:
	-rsh ${DESTHOST} -n "mkdir ${DESTDIR}/src ${DESTDIR}/src/espresso \
                ${DESTDIR}/bin ${DESTDIR}/man ${DESTDIR}/man/man1 \
		${DESTDIR}/man/man5"
	-rsh ${DESTHOST} -n "rm -f ${DESTDIR}/bin/espresso \
		${DESTDIR}/man/man1/espresso.1 ${DESTDIR}/man/man5/espresso.5"
	rcp * ${DESTHOST}:${DESTDIR}/src/espresso
	-rsh ${DESTHOST} -n "rm -f ${DESTDIR}/src/espresso/*.o"
	csh -f -c "rcp ~cad/bin/espresso ${DESTHOST}:${DESTDIR}/bin"
	csh -f -c "rcp ~cad/man/man1/espresso.1 ${DESTHOST}:${DESTDIR}/man/man1"
	csh -f -c "rcp ~cad/man/man5/espresso.5 ${DESTHOST}:${DESTDIR}/man/man5"


# here is the routine global-tags
#! /bin/csh -f
#ctags $*
#sed -e '/define/d' -e '/static/d' -e '/main/d'	\
#	-e 's#	/^# */Qx9w#' -e 's#^.*	#/* #' -e 's#Qx9w#	#'	\
#	-e 's/..$/' -e 's/(.*$/();/' tags		\
#  | sort
#rm -f tags
