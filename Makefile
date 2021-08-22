# makefile for tictactoe
#
# make tictactoe

CC=gcc
CFLAGS=-lncurses

clean:
	rm -rf *.o
	rm -rf tictactoe

tictactoe: tictactoe.c tictactoe.h
	$(CC) $(CFLAGS) tictactoe.c -o tictactoe