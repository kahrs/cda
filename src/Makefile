DIRS=$(wildcard [a-z]*)
LIBS=fizz
PROGS=act\
	cdmglob\
	drawp\
	espresso\
	gnet\
	graw\
	grawp\
	jcc\
	lde\
	mkpins\
	part\
	pll\
	quine\
	rework\
	rom\
	smoke\
	unisite\
	xil\
	xnf\
	xpal\
	xpart

all:
	-for d in $(DIRS); do (cd $$d; $(MAKE) all ); done

install:
	-for d in $(DIRS); do (cd $$d; $(MAKE) install ); done

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

nuke:
	-for d in $(DIRS); do (cd $$d; $(MAKE) nuke ); done
