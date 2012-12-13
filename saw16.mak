# Makefile for showansi
C=	.c
X=	.cpp
O=	.obj
E=	.exe
CX=	wpp
OO=	-Fo=
OE=	-Fe=
CC=	wcc
LD=	wcl
#CDB=	-od -d2
#LDB=	-d2
OS=     os2
OSSYS=  os2
CFLAGS=	-bt=$(OS) -Ox -Zq -w4 -ml $(CDB)
CXFLAGS=$(CFLAGS)
LDFLAGS=-bt=$(OS) -l=$(OSSYS) -k0x4000 -ml $(LDB)

.SUFFIXES: $(E) $(O) $(X) $(C)

$(X)$(O):
	$(CX) $(CXFLAGS) $(OO)$*$(O) $<

$(C)$(O):
	$(CC) $(CFLAGS) $(OO)$*$(O) $<


OBJS = showansi$(O) ansisys$(O) avatar$(O) scrintrp$(O) video$(O) dirport$(O)
TARG = showansi$(E)

default:
	@echo You must specify an operating system 'dos' or 'os2'

dos:
	$(MAKE) -f makefile.w16 $(TARG) OS=dos OSSYS=dos

os2:
	$(MAKE) -f makefile.w16 $(TARG) OS=os2 OSSYS=dos

$(TARG): $(OBJS)
	$(LD) $(LDFLAGS) $(OE)$@ $(OBJS)

