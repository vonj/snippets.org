# Makefile for WATCOM C/C++ for OS/2 10.0

all: qmenup.exe

COPTS=-oneatx

.c.obj:
  wcl386 $(COPTS) -zq -wx -c $<

objs=qmenu.obj    \
     vidmgr.obj   \
     vmgros2.obj  \
     vioimage.obj

qmenup.exe: $(objs)
  wcl386 $(COPTS) -fe=qmenup.exe $(objs)
