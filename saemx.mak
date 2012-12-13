# Makefile for showansi
C=	.c
X=	.cpp
O=	.o
E=	.exe
CC=	gcc
CX=	$(CC)
LD=	$(CC)
#CDB=	-g
#LDB=	-g
OO=	-o
OE=	-o
CFLAGS=	-O2 -fno-strength-reduce -W -Wall $(CDB)
CXFLAGS=$(CFLAGS) -xc++
LDFLAGS=$(LDB)
LDLIBS= -lstdcpp

.SUFFIXES: $(E) $(O) $(X) $(C)

$(X)$(O):
	$(CX) $(CXFLAGS) $(OO) $*$(O) -c $<

$(C)$(O):
	$(CC) $(CFLAGS) $(OO) $*$(O) -c $<

OBJS = showansi$(O) ansisys$(O) avatar$(O) scrintrp$(O) video$(O) dirport$(O)
TARG = showansi$(E)

$(TARG): $(OBJS)
	$(LD) $(LDFLAGS) $(OE) $@ $(OBJS) $(LDLIBS)

