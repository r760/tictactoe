CC=gcc
CFLAGS=-Wall -g

SRCDIR=src
BINDIR=bin

help:	Makefile
	@sed -n 's/^##[ ]*/\n/p' $< | sed 's/: /:\n\t/g; 1d'

## bin/tictactoe: compile tictactoe
$(BINDIR)/tictactoe:	$(SRCDIR)/tictactoe.c
	make clean
	mkdir $(BINDIR)
	${CC} ${CFLAGS} $^ -lncurses -o $(BINDIR)/tictactoe

## install: install tictactoe binary
install:	$(BINDIR)/tictactoe
	[ -d /usr/local/bin ] || sudo mkdir -p /usr/local/bin
	cd $(BINDIR); find . -type f -exec sudo cp -v {} /usr/local/bin/ \;

## uninstall: uninstall tictactoe binary
uninstall:	$(BINDIR)/tictactoe
	cd $(BINDIR); find . -type f -exec sudo rm -v /usr/local/bin/{} \;

## clean: remove bin dir (and everything inside)
clean:
	rm -rf $(BINDIR) &> /dev/null || true
