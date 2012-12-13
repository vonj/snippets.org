##
# Makefile for Myio project
# Created on 06/13/93 at 09:47 after a long night
##

##### MSDOS & OS/2 extensions #####

X = .cpp
O = .obj
E = .EXE

##### UNIX ####

#X = .cc
#O = .o
#E =

###### Borland C for OS/2 ######
#M = 2
#CC = bcc
#LD = tlink
#LIBPATH = F:\Bcc\lib
#INCLUDEPATH = F:\Bcc\include
#CFLAGS = -Oiabec -I$(INCLUDEPATH) -vi- -d -k- -w -a
#LDFLAGS = /x /Toe /ap /L$(LIBPATH)
#STARTUP = $(LIBPATH)\c0$(M)$(O)
#LDLIBS = C$(M).LIB OS2.LIB

###### Borland C for MSDOS ######
#M = S
#CC = bcc
#LD = tlink
#LIBPATH = F:\Bcc\lib.dos
#INCLUDEPATH = F:\Bcc\include.dos
#CFLAGS = -I$(INCLUDEPATH) -ms -Oiabec -k- -vi- -d -k- -w
#LDFLAGS = /L$(LIBPATH)
#STARTUP = $(LIBPATH)\c0$(M)$(O)
#LDLIBS = C$(M).LIB

###### MicroSoft C/C++ for MSDOS ######
#M = S
#CC = cl
#LD = link
#LIBPATH = F:\msc\lib
#INCLUDEPATH = F:\msc\include
#CFLAGS = -I$(INCLUDEPATH) -A$(M) -Oceglnota -W4
#LDFLAGS =
#STARTUP =
#LDLIBS = ;

###### Zortech C/C++ for MSDOS/OS/2 ######
#M = s
#CC = ztc
#LD = blink
#LIBPATH = F:\zortech\lib
#INCLUDEPATH = F:\zortech\include
#CFLAGS = -I$(INCLUDEPATH) -m$(M) -o
#LDFLAGS =
#STARTUP =
#LDLIBS = ;

###### Symantec C/C++ for MSDOS/Mac/OS/2 ######
#M = s
#CC = sc
#LD = link
#LIBPATH = F:\symantec\lib
#INCLUDEPATH = F:\symantec\include
#CFLAGS = -I$(INCLUDEPATH) -m$(M) -o
#LDFLAGS =
#STARTUP =
#LDLIBS = ;

###### IBM C++Set/2 for OS/2 ######
M =
CC = icc
LD = link386
LIBPATH = F:\IbmCpp\lib
INCLUDEPATH = F:\IbmCpp\include
CFLAGS = /Q /I$(INCLUDEPATH) /W2 /Wall /O /Oip
LDFLAGS = /NOLOGO/BAT/NOI/NOE/PM:VIO
LDLIBS = ;

##### Implicit Rules ####

.SUFFIXES:
.SUFFIXES: $(X) $(O)

$(X)$(O):
     $(CC) $(CFLAGS) -c $<

TARGET = Myiodemo$(E)
OBJECTS = Myiodemo$(O) Myio$(O) Mystream$(O) myLine$(O)
AOBJECTS = $(STARTUP) $(OBJECTS)

$(TARGET): $(OBJECTS)
    $(LD) $(LDFLAGS) $(AOBJECTS),$@,,$(LDLIBS)
#   $(LD) $(LDFLAGS) $(AOBJECTS) -o $@ $(LDLIBS)

###### Dependancies #######

myLine$(O): myLine$(X) myLine.h

Mystream$(O): Mystream$(X) Mystream.h Myio.h

Myio$(O): Myio$(X) Mystream.h Myio.h

Myiodemo$(O): Myiodemo$(X) myLine.h Myio.h Mystream.h
