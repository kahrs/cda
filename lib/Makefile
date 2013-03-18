GFILES=toy.g
WFILES=toy.w
NAME=toy
ALLPINS = $(WFILES) $(PALP) my.pins io.pins lib.pins discrete.pins

FAIL=(rm $@; false)
WHAT='again|annotate|check|clip|commit|draw|fix|list|fx|net|pins|place|schematics|smoke|wr|wrap'

all:
	@echo $(WHAT)

$(NAME): $(NAME).wx
rework: $(NAME).rework
smoke: $(NAME).smoke
parts: $(NAME).parts
pack: $(NAME).pack
check: $(NAME).check
place: $(NAME).place

# Schematic conversion to netlists, etc.

$(NAME).wx: all.pins $(WFILES) 
	cdmglob -f -v all.pins $(WFILES) > $(NAME).wx.ng 2> errors
	sed -f slash.sed $(NAME).wx.ng >$@ || $(FAIL)

all.pins: $(ALLPINS)
	mkpins $(ALLPINS) /usr/lib/cda/pins >all.pins.new 2>undefined
	mv all.pins.new all.pins

%.w: %.g
	gnet $< | sed 's/\$$N00/$$$%/' > $@ || $(FAIL)

%.p: %.e
	lde -W $< > $@

%.min: %.e %.p
	lde -a $< | quine | cover | hazard > $@.ng || $(FAIL)
	mv $@.ng $@

%.smoke: %.wx all.pins
	smoke -pbx $< all.pins > eyes

# Physical layout stuff

%.fx:	%.wx
	cvt $< > $@

%.pkg:	%.fx library.pkg
	mkpkgs $^ > $*.pkg.ng 2> errors
	mv $*.pkg.ng $*.pkg

%.place: %.fx %.brd %.pkg %.pos
	place $^

%.wr:	%.fx %.brd %.pkg %.pos
	wrap -ov $^ > $*.wr.mils || $(FAIL)
	wrap.awk $*.wr.mils > $@

%.check: %.fx %.brd %.pkg %.pos
	check -w $^

%.clip: %.brd %.pkg %.fx %.pos
	clip $^ > $@

%.plot: %.fx %.brd %.pkg %.pos
	draw -ptv $^ | plot -Ttroff | troff -ms | lp -d ps84

%.parts: $(NAME).wx
	getparts $(NAME).wx | findparts > $@

%.pack: $(NAME).wx all.pins
	awk -f ../scripts/pack.awk $(NAME).wx all.pins > $@

%.A: %.g %.w $(NAME).wx
	annotate $(NAME).wx $*.w
	cat $*.g >> $*.a	

%.annotate: $(WFILES) $(NAME).wx
	annotate $(NAME).wx $(WFILES)
	for i in $(GFILES)
	do
		cat $i.g >>$i.a
	done

%.schematics: %.annotate
	grawp $(AFILES) | lp -d ps84

wlist: $(NAME).wx wlist.awk
	awk -f wlist.awk $(NAME).wx | sort > $@

list: wlist
	egrep -v '^(\$$|GND|VCC)' wlist | prtiny -3

# wire wrap

%.wrap:
	if [ -f N$%.wr ]; then
		wrap -s -r$(ORIENT) -d30 UN.wr
		wrap -s -r$(ORIENT) -d30 RE.wr
	else
		wrap -s -r$(ORIENT) -d30 $%.wr
	fi

%.fix:
	wrap -s -r$(ORIENT) -d30 fix.wr

%.commit:
	mv N$%.wr $%.wr && rm -f UN.wr RE.wr .lastwire

%.rework: %.wx %.brd %.pkg %.pos
	cp $%.wr O$%.wr
	shrink N$%.wr
	rework $%.wr N$%.wr && mv NEW.wr N$%.wr

# PCB

%.pcb: preamble.pcb %.elements
	cat preamble.pcb $*.elements > $@

%.elements: %.wx
	cvt -P $< | ../scripts/topcb > $*.elements

%.net: %.wx
	cvt -N -P $< > $@

# The end

clean:
	rm -f *.w *.wx *.p *.min

