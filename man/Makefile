PREFIX=/usr

install: 
	test -d $(PREFIX)/share/man/man1 || mkdir -p $(PREFIX)/share/man/man1
	test -d $(PREFIX)/share/man/man6 || mkdir -p $(PREFIX)/share/man/man6
	install -m 0644 man1/* $(PREFIX)/share/man/man3
	install -m 0644 man6/* $(PREFIX)/share/man/man6

