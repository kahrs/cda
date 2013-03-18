L=toy.w.l toy.pins.l toy.wx.l

cda.pdf: $S $L cda.ms
	groff -p -t -ms cda.ms | ps2pdf - cda.pdf

%.pic: %.g
	cda/grawp -t $< > $@

%.l: %
	sed 's/^/\\\&/' $< > $@

clean:
	rm -f cda.i *.pic
