CC = gcc
CFLAGS = -m32 -o

all: machine 

machine: machine.c
	$(CC) $(CFLAGS) $@  $?
clean:
	rm machine
sandmark:
	cat um.um sandmark.umz > cmu.um
codex:
	cat um.um codex.umz > cmu.um

