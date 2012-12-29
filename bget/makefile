
CC = cc
COPTS = -O

bget:	bget.c bget.h
	$(CC) $(COPTS) bget.c -o bget -lm
	rm -f core

lint:
	lint bget.c -lm

clean:
	rm -f bget core *.o *.bak *.exe *.obj *.swp
