# Makefile for showansi
C=	.c
X=	.cpp
O=	.obj
E=	.exe
CX=	wpp386
OO=	-Fo=
OE=	-Fe=
CC=	wcc386
LD=	wcl386
CDB=	-od -d2
LDB=	-d2
OS=     os2
OSSYS=  os2v2
CFLAGS=	-bt=$(OS) -Ox -Zq -w4 $(CDB)
CXFLAGS=$(CFLAGS)
LDFLAGS=-bt=$(OS) -l=$(OSSYS) $(LDB)

.SUFFIXES: $(E) $(O) $(X) $(C)

$(X)$(O):
	$(CX) $(CXFLAGS) $(OO)$*$(O) $<

$(C)$(O):
	$(CC) $(CFLAGS) $(OO)$*$(O) $<

OBJS = showansi$(O) ansisys$(O) avatar$(O) scrintrp$(O) video$(O) dirport$(O)
TARG = showansi$(E)

$(TARG): $(OBJS)
	$(LD) $(LDFLAGS) $(OE)$@ $(OBJS)

