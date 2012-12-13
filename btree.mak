maintest.exe: maintest.o btree.o treescan.o knuran.o
	gcc -o maintest.exe maintest.o btree.o treescan.o knuran.o
maintest.o: maintest.c bstuff.h
	gcc -c -o maintest.o maintest.c
btree.o: btree.c bstuff.h
	gcc -c -o btree.o btree.c
treescan.o: treescan.c bstuff.h
	gcc -c -o treescan.o treescan.c
knuran.o: knuran.c
	gcc -c -o knuran.o knuran.c
