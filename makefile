#  SNIPPETS master makefile
#
#  Due to the lack of standard syntax between various make utilities, this
#  generic makefile template is provided as a starting point. It may be used
#  as-is with most reasonably standard Unix-style make utilties. For further
#  ideas on customizing it for your system, see MAKE.INI.
#
#  Public domain by Bob Stout

ver=9707

#  Uncomment one set of the following for DOS real mode or add your own...

#cmm=s
#amm=SMALL

cmm=c
amm=COMPACT

#cmm=m
#amm=MEDIUM

#cmm=l
#amm=LARGE

#cmm=h
#amm=HUGE

#  Uncomment one of the following or add your own...

cc=sc -m$(cmm)		# Symantec
lc=lib /C /B
lb=sls$(cmm)$(ver)
lr=lib.rsp

#cc=wcl -m$(cmm)	# Watcom
#lc=wlib
#lb=slw$(cmm)$(ver)
#lr=wlib.rsp

#cc=cl /A$(cmm)		# Microsoft
#lc=lib
#lb=slm$(cmm)$(ver)
#lr=lib.rsp

#cc=bcc -m$(cmm)	# Borland
#lc=tlib
#lb=slb$(cmm)$(ver)
#lr=tlib.rsp

#cc=ztc -m$(cmm)	# Zortech
#lc=zorlib
#lb=slz$(cmm)$(ver)
#lr=zorlib.rsp

#cc=pc /m$(cmm)		# Mix

#cc=gcc			# Gnu


ct=$(cc) -DTEST         # For files which include an "#ifdef TEST" section
                        # when compiling to an executable

#  Uncomment one of each of the following sets, or add your own...

as=MASM /Mx /z /Dmemodel=$(amm)
#as=TASM /jMASM /mx /z /Dmemodel=$(amm)

rm=del
#rm=rm

mv=move
#mv=mv

o=obj
#o=o

x=exe
#x=com


#.SUFFIXES: .c .cpp .asm        # For make utilities which require
                                # .SUFFIXES be set for implicit rules

l=lib

$(lb).$(l)	: allobj
	$(lc) $(lb).$(l) @$(lr)

#  The following macros provides for post processing after compilation.
#
#  The first macro aids debugging when using a compiler (e.g. Watcom) which
#  outputs errors to individual .err files. Files which include a test main()
#  function, and which include functions which are themselves linked into
#  other executables, may have errors in the test main() function which go
#  undetected when the file is recompiled without the test main() being
#  enabled. In such a case, the .err file written when first compiled as an
#  executable may be erased when recompiled as a linkable object. Invoking
#  the $(extra1) macro renames the .err file resulting from compilation to
#  an executable to a .erx file.
#
#  The second and third macroa are for use with the DJGPP port of gcc, to
#  convert COFF output files to DOS executables.

extra1=if exist $*.err $(mv) $*.err $*.erx

#extra2=$(mv) a.out $*
#extra3=coff2exe $*


#  Default rules

.c.$(o):
	$(cc) -c $*.c

.cpp.$(o):
	$(cc) -c $*.cpp

.c.$(x):
	$(cc) $*.c

.cpp.$(x):
	$(cc) $*.cpp

.asm.$(o):
	$(as) $*.asm;


#  Clean the directory, start from scratch

clean :
	$(rm) *.$(o)
	$(rm) *.$(x)
	$(rm) *.er*


#  Executable dependencies begin here

bastrngs.$(x)	: bastrngs.c sniptype.h bastrngs.h
	$(ct) bastrngs.c
	$(rm) bastrngs.$(o)
	$(extra1)

cards.$(x)	: cards.cpp cards.hpp
	$(ct) cards.cpp
	$(rm) cards.$(o)
	$(extra1)

ccard.$(x)	: ccard.c ccard.h ccardplv.h
	$(ct) ccard.c
	$(rm) ccard.$(o)
	$(extra1)

ctrlprnt.$(x)	: ctrlprnt.c ctrlprnt.h
	$(ct) ctrlprnt.c
	$(rm) ctrlprnt.$(o)
	$(extra1)

err_exit.$(x)	: err_exit.c errors.h
	$(ct) err_exit.c
	$(rm) err_exit.$(o)
	$(extra1)

ferrorf.$(x) : ferrorf.c errors.h
	$(ct) ferrorf.c
	$(rm) ferrorf.$(o)
	$(extra1)

initvars.$(x)	: initvars.c initvars.h errors.h ferrorf.$(o)
	$(ct) initvars.c ferrorf.$(o)
	$(rm) initvars.$(o)
	$(extra1)

isisbn.$(x)	: isisbn.c isisbn.h
	$(ct) isisbn.c
	$(rm) isisbn.$(o)
	$(extra1)

whicharc.$(x)	: whicharc.cpp whicharc.h archive.$(o) archive.hpp
	$(ct) whicharc.cpp
	$(rm) whicharc.$(o)
	$(extra1)

ansiflen.$(x)	: ansiflen.c snipfile.h
	$(ct) ansiflen.c
	$(rm) ansiflen.$(o)
	$(extra1)

existsx.$(x)	: existsx.c snipfile.h snip_str.h stptok.$(o) unix2dos.$(o)
	$(ct) existsx.c stptok.$(o) unix2dos.$(o)
	$(rm) existsx.$(o)
	$(extra1)

fcompare.$(x)	: fcompare.c snipfile.h sniptype.h
	$(ct) fcompare.c
	$(rm) fcompare.$(o)
	$(extra1)

srchfile.$(x)	: srchfile.c sniptype.h snipfile.h ferrorf.$(o)
	$(ct) srchfile.c ferrorf.$(o)
	$(rm) srchfile.$(o)
	$(extra1)

textmod.$(x)	: textmod.c snipfile.h ferrorf.$(o)
	$(cc) textmod.c ferrorf.$(o)
	$(rm) textmod.$(o)

update.$(x)	: update.c update.h
	$(ct) update.c
	$(rm) update.$(o)
	$(extra1)

memmem.$(x)	: memmem.c snip_str.h
	$(ct) memmem.c
	$(rm) memmem.$(o)
	$(extra1)

fmemmem.$(x)	: fmemmem.c fptr_add.$(o) snip_str.h snpdosys.h
	$(ct) fmemmem.c fptr_add.$(o)
	$(rm) fmemmem.$(o)
	$(extra1)

sstrdel.$(x) : sstrdel.c snip_str.h
	$(ct) sstrdel.c
	$(rm) sstrdel.$(o)
	$(extra1)

stptok.$(x)	: stptok.c snip_str.h
	$(ct) stptok.c
	$(rm) stptok.$(o)
	$(extra1)

strchcat.$(x)	: strchcat.c snip_str.h
	$(ct) strchcat.c
	$(rm) strchcat.$(o)
	$(extra1)

strdel.$(x)	: strdel.c snip_str.h
	$(ct) strdel.c
	$(rm) strdel.$(o)
	$(extra1)

strdelch.$(x)	: strdelch.c snip_str.h
	$(ct) strdelch.c
	$(rm) strdelch.$(o)
	$(extra1)

stristr.$(x) : stristr.c snip_str.h
	$(ct) stristr.c
	$(rm) stristr.$(o)
	$(extra1)

strrepl.$(x) : strrepl.c snip_str.h
	$(ct) strrepl.c
	$(rm) strrepl.$(o)
	$(extra1)

strrev.$(x)	: strrev.c snip_str.h
	$(ct) strrev.c
	$(rm) strrev.$(o)
	$(extra1)

strrpbrk.$(x)	: strrpbrk.c snip_str.h
	$(ct) strrpbrk.c
	$(rm) strrpbrk.$(o)
	$(extra1)

strupr.$(x)	: strupr.c snip_str.h
	$(ct) strupr.c
	$(rm) strupr.$(o)
	$(extra1)

ruleline.$(x)	: ruleline.c snip_str.h
	$(ct) ruleline.c
	$(rm) ruleline.$(o)
	$(extra1)

trim.$(x)	: trim.c snip_str.h
	$(ct) trim.c
	$(rm) trim.$(o)
	$(extra1)

hugesort.$(x)	: hugesort.c snipsort.h extkword.h err_exit.$(o)
	$(ct) hugesort.c err_exit.$(o)
	$(rm) hugesort.$(o)
	$(extra1)

ll_msort.$(x)	: ll_msort.c snipsort.h
	$(cc) ll_msort.c
	$(rm) ll_msort.$(o)

index.$(x)	: index.c ferrorf.$(o)
	$(cc) index.c ferrorf.$(o)
	$(rm) index.$(o)

lookup.$(x)	: lookup.c ferrorf.$(o)
	$(cc) lookup.c ferrorf.$(o)
	$(rm) lookup.$(o)

dtotp6.$(x)	: dtotp6.c dtotp6.h
	$(ct) dtotp6.c
	$(rm) dtotp6.$(o)
	$(extra1)

bcdl.$(x)	: bcdl.c sniptype.h snipmath.h
	$(ct) bcdl.c
	$(rm) bcdl.$(o)
	$(extra1)

bcdd.$(x)	: bcdd.c sniptype.h snipmath.h
	$(ct) bcdd.c
	$(rm) bcdd.$(o)
	$(extra1)

combin.$(x)	: combin.c snipmath.h
	$(ct) combin.c
	$(rm) combin.$(o)
	$(extra1)

cubic.$(x)	: cubic.c snipmath.h
	$(ct) cubic.c
	$(rm) cubic.$(o)
	$(extra1)

dbl2long.$(x)	: dbl2long.c snipmath.h
	$(ct) dbl2long.c
	$(rm) dbl2long.$(o)
	$(extra1)

dblround.$(x)	: dblround.c snipmath.h
	$(ct) dblround.c
	$(rm) dblround.$(o)
	$(extra1)

etphi.$(x)	: etphi.c isqrt.$(o)
	$(ct) etphi.c isqrt.$(o)
	$(rm) etphi.$(o)
	$(extra1)

factoryl.$(x)	: factoryl.c snipmath.h dblround.$(o)
	$(ct) factoryl.c dblround.$(o)
	$(rm) factoryl.$(o)
	$(extra1)

fibo.$(x)	: fibo.c snipmath.h dblround.$(o)
	$(ct) fibo.c dblround.$(o)
	$(rm) fibo.$(o)
	$(extra1)

ipow.$(x)	: ipow.c snipmath.h
	$(ct) ipow.c
	$(rm) ipow.$(o)
	$(extra1)

ispow2.$(x)	: ispow2.c snipmath.h
	$(ct) ispow2.c
	$(rm) ispow2.$(o)
	$(extra1)

isqrt.$(x)	: isqrt.c snipmath.h
	$(ct) isqrt.c
	$(rm) isqrt.$(o)
	$(extra1)

rad2deg.$(x) : rad2deg.c snipmath.h
	$(ct) rad2deg.c
	$(rm) rad2deg.$(o)
	$(extra1)

spigot.$(x)	: spigot.c
	$(cc) spigot.c
	$(rm) spigot.$(o)

pi.$(x)	: pi.c extkword.h big_mall.h
	$(cc) pi.c
	$(rm) pi.$(o)

pi8.$(x)	: pi8.c
	$(cc) pi6.c
	$(rm) pi6.$(o)

pi_agm.$(x)	: pi_agm.c ispow2.$(o) snipmath.h
	$(cc) pi_agm.c ispow2.$(o)
	$(rm) pi_agm.$(o)

rand1.$(x)	: rand1.c
	$(cc) rand1.c
	$(rm) rand1.$(o)

rg_rand.$(x) : rg_rand.c
	$(ct) rg_rand.c
	$(rm) rg_rand.$(o)
	$(extra1)

bigtest.exe	: bigtest.c bignum.h bignum1.$(o) bignum2.$(o)
	$(cc) bigtest.c bignum1.$(o) bignum2.$(o)
	$(rm) bigtest.$(o)

bascnvrt.$(x)	: bascnvrt.c numcnvrt.h extkword.h ltostr.$(o)
	$(ct) bascnvrt.c ltostr.$(o)
	$(rm) bascnvrt.$(o)
	$(extra1)

commaflt.$(x)	: commaflt.c numcnvrt.h snip_str.h
	$(ct) commaflt.c
	$(rm) commaflt.$(o)
	$(extra1)

commafmt.$(x)	: commafmt.c numcnvrt.h
	$(ct) commafmt.c
	$(rm) commafmt.$(o)
	$(extra1)

eng.$(x)	: eng.c snipmath.h
	$(ct) eng.c
	$(rm) eng.$(o)
	$(extra1)

eval.$(x)	: eval.c sniptype.h snip_str.h snipmath.h rmallws.$(o)
	$(ct) eval.c rmallws.$(o)
	$(rm) eval.$(o)
	$(extra1)

fmtmoney.$(x)	: fmtmoney.c snipmath.h
	$(ct) fmtmoney.c
	$(rm) fmtmoney.$(o)
	$(extra1)

hexorint.$(x)	: hexorint.c sniptype.h numcnvrt.h
	$(ct) hexorint.c
	$(rm) hexorint.$(o)
	$(extra1)

hstr_i.$(x)	: hstr_i.c numcnvrt.h
	$(ct) hstr_i.c
	$(rm) hstr_i.$(o)
	$(extra1)

ltoa.$(x)	: ltoa.c numcnvrt.h
	$(ct) ltoa.c
	$(rm) ltoa.$(o)
	$(extra1)

ltostr.$(x)	: ltostr.c numcnvrt.h
	$(ct) ltostr.c
	$(rm) ltostr.$(o)
	$(extra1)

ord_text.$(x)	: ord_text.c numcnvrt.h
	$(ct) ord_text.c
	$(rm) ord_text.$(o)
	$(extra1)

scanfrac.$(x)	: scanfrac.c snipmath.h
	$(ct) scanfrac.c
	$(rm) scanfrac.$(o)
	$(extra1)

l2roman.$(x)	: l2roman.c sniptype.h
	$(ct) l2roman.c
	$(rm) l2roman.$(o)
	$(extra1)

roman2l.$(x)	: roman2l.c sniptype.h
	$(ct) roman2l.c
	$(rm) roman2l.$(o)
	$(extra1)

str27seg.$(x)	: str27seg.c numcnvrt.h
	$(ct) str27seg.c
	$(rm) str27seg.$(o)
	$(extra1)

amalloc.$(x)	: amalloc.c snparray.h
	$(ct) amalloc.c
	$(rm) amalloc.$(o)
	$(extra1)

mdasort.$(x)	: mdasort.c amalloc.$(o) mdalloc.$(o) snparray.h
	$(cc) mdasort.c amalloc.$(o) mdalloc.$(o)
	$(rm) mdasort.$(o)

bitcnt_1.$(x)	: bitcnt_1.c bitops.h snip_str.h
	$(ct) bitcnt_1.c
	$(rm) bitcnt_1.$(o)
	$(extra1)

bitcnt_2.$(x)	: bitcnt_2.c bitops.h snip_str.h
	$(ct) bitcnt_2.c
	$(rm) bitcnt_2.$(o)
	$(extra1)

bitcnt_3.$(x)	: bitcnt_3.c bitops.h snip_str.h
	$(ct) bitcnt_3.c
	$(rm) bitcnt_3.$(o)
	$(extra1)

bitcnt_4.$(x)	: bitcnt_4.c bitops.h snip_str.h
	$(ct) bitcnt_4.c
	$(rm) bitcnt_4.$(o)
	$(extra1)

bitcnts.$(x)	: bitcnts.c bitops.h bitcnt_1.$(o) bitcnt_2.$(o) \
		  bitcnt_3.$(o) bitcnt_4.$(o)
	$(cc) bitcnts.c bitcnt_1.$(o) bitcnt_2.$(o) bitcnt_3.$(o) \
		bitcnt_4.$(o)
	$(rm) bitcnts.$(o)

bitfiles.$(x)	: bitfiles.c bitops.h
	$(ct) bitfiles.c
	$(rm) bitfiles.$(o)
	$(extra1)

bitstrng.$(x)	: bitstrng.c bitops.h
	$(ct) bitstrng.c
	$(rm) bitstrng.$(o)
	$(extra1)

bstr_i.$(x)	: bstr_i.c bitops.h
	$(ct) bstr_i.c
	$(rm) bstr_i.$(o)
	$(extra1)

hash.$(x)	: hash.c hash.h
	$(ct) hash.c
	$(rm) hash.$(o)
	$(extra1)

match.$(x)	: match.c match.h
	$(ct) match.c
	$(rm) match.$(o)
	$(extra1)

soundex.$(x) : soundex.c phonetic.h
	$(ct) soundex.c
	$(rm) soundex.$(o)
	$(extra1)

soundex4.$(x)	: soundex4.c phonetic.h snip_str.h
	$(ct) soundex4.c
	$(rm) soundex4.$(o)
	$(extra1)

soundex5.$(x)	: soundex5.c phonetic.h
	$(ct) soundex5.c
	$(rm) soundex5.$(o)
	$(extra1)

metaphon.$(x)	: metaphon.c phonetic.h dirent.h vfname.$(o) strchcat.$(o) \
			  posixdir.$(o) unix2dos.$(o)
	$(ct) metaphon.c vfname.$(o) strchcat.$(o) posixdir.$(o) unix2dos.$(o)
	$(rm) metaphon.$(o)
	$(extra1)

approx.$(x)	: approx.c phonetic.h
	$(ct) approx.c
	$(rm) approx.$(o)
	$(extra1)

getcmt.$(x)	: getcmt.c
	$(cc) getcmt.c
	$(rm) getcmt.$(o)

testcmt.$(x) : testcmt.c
	$(cc) testcmt.c
	$(rm) testcmt.$(o)

c_cmnt.$(x)	: c_cmnt.c
	$(cc) c_cmnt.c
	$(rm) c_cmnt.$(o)

cmtconvr.$(x)	: cmtconvr.cpp cmtparsr.cpp
	$(cc) cmtconvr.cpp cmtparsr.cpp
	$(rm) cmtconvr.$(o)
	$(rm) cmtparsr.$(o)

cmtcount.$(x)	: cmtcount.cpp cmtparsr.cpp
	$(cc) cmtcount.cpp cmtparsr.cpp
	$(rm) cmtcount.$(o)
	$(rm) cmtparsr.$(o)

cmtremov.$(x)	: cmtremov.cpp cmtparsr.cpp
	$(cc) cmtremov.cpp cmtparsr.cpp
	$(rm) cmtremov.$(o)
	$(rm) cmtparsr.$(o)

cmtxtrac.$(x)	: cmtxtrac.cpp cmtparsr.cpp
	$(cc) cmtxtrac.cpp cmtparsr.cpp
	$(rm) cmtxtrac.$(o)
	$(rm) cmtparsr.$(o)

commconv.$(x)	: commconv.c
	$(cc) commconv.c
	$(rm) commconv.$(o)

remcmmnt.$(x)	: remcmmnt.c
	$(cc) remcmmnt.c
	$(rm) remcmmnt.$(o)

csplit.$(x)	: csplit.c
	$(cc) csplit.c
	$(rm) csplit.$(o)

daynum.$(x)	: daynum.c scaldate.h scaldate.$(o)
	$(ct) daynum.c scaldate.$(o)
	$(rm) daynum.$(o)
	$(extra1)

isdst.$(x)	: isdst.c sniptype.h scaldate.h datetime.h scaldate.$(o) \
		  parsdate.$(o) daynum.$(o)
	$(ct) isdst.c scaldate.$(o) parsdate.$(o) daynum.$(o)
	$(rm) isdst.$(o)
	$(extra1)

today.$(x)	: today.c scaldate.h scaldate.$(o)
	$(ct) today.c scaldate.$(o)
	$(rm) today.$(o)
	$(extra1)

dow.$(x)	: dow.c scaldate.h
	$(ct) dow.c
	$(rm) dow.$(o)
	$(extra1)

fdate.$(x)	: fdate.c ftime.h dirport.h scaldate.h scaldate.$(o) \
		  ftime.$(o)
	$(ct) fdate.c scaldate.$(o) ftime.$(o)
	$(rm) fdate.$(o)
	$(extra1)

cal.$(x)	: cal.c scaldate.h scaldate.$(o) today.$(o)
	$(cc) cal.c scaldate.$(o) today.$(o)
	$(rm) cal.$(o)

jdn_l.$(x)	: jdn_l.c datetime.h
	$(ct) jdn_l.c
	$(rm) jdn_l.$(o)
	$(extra1)

easter.$(x)	: easter.c datetime.h
	$(ct) easter.c
	$(rm) easter.$(o)
	$(extra1)

addtime.$(x) : addtime.c datetime.h
	$(ct) addtime.c
	$(rm) addtime.$(o)
	$(extra1)

moon_age.$(x)	: moon_age.c datetime.h
	$(ct) moon_age.c
	$(rm) moon_age.$(o)
	$(extra1)

parsdate.$(x)	: parsdate.c datetime.h
	$(ct) parsdate.c
	$(rm) parsdate.$(o)
	$(extra1)

parstime.$(x)	: parstime.c datetime.h
	$(ct) parstime.c
	$(rm) parstime.$(o)
	$(extra1)

strftime.$(x)	: strftime.c
	$(ct) strftime.c
	$(rm) strftime.$(o)
	$(extra1)

datedemo.$(x)	: datedemo.cpp date.hpp date.$(o)
	$(cc) datedemo.cpp date.$(o)
	$(rm) datedemo.$(o)

w_wrap.$(x)	: w_wrap.c w_wrap.h
	$(ct) w_wrap.c
	$(rm) w_wrap.$(o)
	$(extra1)

center.$(x)	: center.c w_wrap.h w_wrap.$(o)
	$(ct) center.c w_wrap.$(o)
	$(rm) center.$(o)
	$(extra1)

arccrc16.$(x)	: arccrc16.c crc.h
	$(ct) arccrc16.c
	$(rm) arccrc16.$(o)
	$(extra1)

crc_16f.$(x) : crc_16f.c crc.h unistd.h
	$(ct) crc_16f.c
	$(rm) crc_16f.$(o)
	$(extra1)

crc_32.$(x)	: crc_32.c crc.h
	$(ct) crc_32.c
	$(rm) crc_32.$(o)
	$(extra1)

checksum.$(x)	: checksum.c crc.h
	$(ct) checksum.c
	$(rm) checksum.$(o)
	$(extra1)

checkexe.$(x)	: checkexe.c crc.h
	$(ct) checkexe.c
	$(rm) checkexe.$(o)
	$(extra1)

getoptst.$(x)	: getoptst.c getopts.h eval.$(o) fnsplit.$(o) dblround.$(o) \
			  ferrorf.$(o) err_exit.$(o) getopts.$(o)
	$(ct) getoptst.c eval.$(o) fnsplit.$(o) dblround.$(o) ferrorf.$(o) \
		err_exit.$(o) getopts.$(o)
	$(rm) getoptst.$(o)
	$(extra1)

palnfilt.$(x)	: palnfilt.c getopts.h getopts.$(o) fnsplit.$(o) \
			  err_exit.$(o) ferrorf.$(o) unix2dos.$(o)
	$(cc) palnfilt.c getopts.$(o) ferrorf.$(o) fnsplit.$(o) unix2dos.$(o) \
		err_exit.$(o)
	$(rm) palnfilt.$(o)

getoptsl.$(x)	: getoptsl.c getoptsl.h
	$(ct) getoptsl.c
	$(rm) getoptsl.$(o)
	$(extra1)

pbmsrch.$(x) : pbmsrch.c
	$(ct) pbmsrch.c
	$(rm) pbmsrch.$(o)
	$(extra1)

jmalloc.$(x) : jmalloc.c jmalloc.h
	$(ct) jmalloc.c
	$(rm) jmalloc.$(o)
	$(extra1)

jnew.$(x)	: jnew.cpp jnew.h jmalloc.$(o)
	$(ct) jnew.cpp jmalloc.$(o)
	$(rm) jnew.$(o)
	$(extra1)

ansitest.$(x)	: ansitest.c ansicode.h dosgetch.$(o) dosgetch.h
	$(cc) ansitest.c dosgetch.$(o)
	$(rm) ansitest.$(o)

posix_ls.$(x)	: posix_ls.c dirent.h posixdir.$(o) dirmask.$(o) \
			  xstrcmp.$(o) unix2dos.$(o) extkword.h
	$(cc) posix_ls.c posixdir.$(o) dirmask.$(o) xstrcmp.$(o) unix2dos.$(o)
	$(rm) posix_ls.$(o)

xstrcmp.$(x) : xstrcmp.c sniptype.h dirent.h
	$(ct) xstrcmp.c
	$(rm) xstrcmp.$(o)
	$(extra1)

chgext.$(x)	: chgext.c filnames.h
	$(ct) chgext.c
	$(rm) chgext.$(o)
	$(extra1)

fnsplit.$(x)	:fnsplit.c sniptype.h filnames.h unix2dos.$(o)
	$(ct) fnsplit.c unix2dos.$(o)
	$(rm) fnsplit.$(o)
	$(extra1)

vfname.$(x)	: vfname.c dirport.h filnames.h vfname.h
	$(ct) vfname.c
	$(rm) vfname.$(o)
	$(extra1)

addhndls.$(x)	: addhndls.c dosfiles.h
	$(ct) addhndls.c
	$(rm) addhndls.$(o)
	$(extra1)

addpath.$(x) : addpath.c dosfiles.h trim.$(o)
	$(ct) addpath.c trim.$(o)
	$(rm) addpath.$(o)
	$(extra1)

drvalid.$(x) : drvalid.c dosfiles.h absdisk.$(o) absdiskc.$(o)
	$(ct) drvalid.c absdisk.$(o) absdiskc.$(o)
	$(rm) drvalid.$(o)
	$(extra1)

favail.$(x)	: favail.c dosfiles.h
	$(ct) favail.c
	$(rm) favail.$(o)
	$(extra1)

fchsize.$(x)	: fchsize.c ferrorf.$(o) sniptype.h
	$(ct) fchsize.c ferrorf.$(o)
	$(rm) fchsize.$(o)
	$(extra1)

files.$(x)	: files.c dosfiles.h
	$(ct) files.c
	$(rm) files.$(o)
	$(extra1)

getdcwd.$(x) : getdcwd.c dosfiles.h extkword.h
	$(ct) getdcwd.c
	$(rm) getdcwd.$(o)
	$(extra1)

iscons.$(x)	: iscons.c dosfiles.h
	$(ct) iscons.c
	$(rm) iscons.$(o)
	$(extra1)

isfopen.$(x) : isfopen.c dosfiles.h
	$(ct) isfopen.c
	$(rm) isfopen.$(o)
	$(extra1)

isnetdr.$(x) : isnetdr.c dosfiles.h
	$(ct) isnetdr.c
	$(rm) isnetdr.$(o)
	$(extra1)

isramdsk.$(x)	: isramdsk.c dos5boot.h dosfiles.h snpdskio.h \
			  absdisk.$(o) absdiskc.$(o)
	$(ct) isramdsk.c absdisk.$(o) absdiskc.$(o)
	$(rm) isramdsk.$(o)
	$(extra1)

iswprot.$(x) : iswprot.c dosfiles.h
	$(ct) iswprot.c
	$(rm) iswprot.$(o)
	$(extra1)

mkdirs.$(x)	: mkdirs.c dosfiles.h unistd.h pushdir.$(o) drvalid.$(o) \
		  absdisk.$(o) absdiskc.$(o)
	$(ct) mkdirs.c pushdir.$(o) drvalid.$(o) absdisk.$(o) absdiskc.$(o)
	$(rm) mkdirs.$(o)
	$(extra1)

truname.$(x)	: truname.c dosfiles.h trim.$(o)
	$(ct) truname.c trim.$(o)
	$(rm) truname.$(o)
	$(extra1)

filelist.$(x)	: filelist.c filelist.h
	$(ct) filelist.c
	$(rm) filelist.$(o)
	$(extra1)

ftime.$(x)	: ftime.c ferrorf.$(o) err_exit.$(o)
	$(ct) ftime.c ferrorf.$(o) err_exit.$(o)
	$(rm) ftime.$(o)
	$(extra1)

delay.$(x)	: delay.c delay.h
	$(ct) delay.c
	$(rm) delay.$(o)
	$(extra1)

dosfrmat.$(x)	: dosfrmat.c snpdosys.h
	$(ct) dosfrmat.c
	$(rm) dosfrmat.$(o)
	$(extra1)

format.$(x)	: format.c format.h pc-port.h
	$(cc) format.c
	$(rm) format.obj

isshare.$(x) : isshare.c snpdosys.h ferrorf.$(o)
	$(ct) isshare.c ferrorf.$(o)
	$(rm) isshare.$(o)
	$(extra1)

memavail.$(x)	: memavail.c snpdosys.h
	$(ct) memavail.c
	$(rm) memavail.$(o)
	$(extra1)

nwlinnam.$(x)	: nwlinnam.c snpdosys.h
	$(ct) nwlinnam.c
	$(rm) nwlinnam.$(o)
	$(extra1)

os_id.$(x)	: os_id.c snpdosys.h
	$(ct) os_id.c
	$(rm) os_id.$(o)
	$(extra1)

pfopen.$(x)	: pfopen.c snpdosys.h
	$(ct) pfopen.c
	$(rm) pfopen.$(o)
	$(extra1)

redirect.$(x)	: redirect.c unistd.h dirport.h snpdosys.h
	$(ct) redirect.c
	$(rm) redirect.$(o)
	$(extra1)

shel2dos.$(x)	: shel2dos.c snpdosys.h
	$(ct) shel2dos.c
	$(rm) shel2dos.$(o)
	$(extra1)

xtest.$(x)	: xtest.c xfile.h xfile.$(o)
	$(cc) xtest.c xfile.$(o)
	$(rm) xtest.$(o)

vidport.$(x) : vidport.c scrnmacs.h scroll.$(o)
	$(ct) vidport.c scroll.$(o)
	$(rm) vidport.$(o)
	$(extra1)

scrnsave.$(x)	: scrnsave.c scrnmacs.h vidport.$(o) scroll.$(o) fmemops.$(o)
	$(ct) scrnsave.c vidport.$(o) scroll.$(o) fmemops.$(o)
	$(rm) scrnsave.$(o)
	$(extra1)

fscrnsav.$(x)	: fscrnsav.c sniptype.h scrnmacs.h vidport.$(o) scroll.$(o) \
		  hugeread.$(o) fmemops.$(o) fptr_add.$(o) extkword.h
	$(ct) fscrnsav.c vidport.$(o) scroll.$(o) fmemops.$(o) hugeread.$(o) \
		  fptr_add.$(o)
	$(rm) fscrnsav.$(o)
	$(extra1)

ansiload.$(x)	: ansiload.c sniptype.h ansiload.h
	$(ct) ansiload.c
	$(rm) ansiload.$(o)
	$(extra1)

vgablank.$(x)	: vgablank.c delay.$(o) pchwio.h
	$(ct) vgablank.c delay.$(o)
	$(rm) vgablank.$(o)
	$(extra1)

bresnham.$(x)	: bresnham.c bresnham.h
	$(ct) bresnham.c
	$(rm) bresnham.$(o)
	$(extra1)

bhamdemo.$(x)	: bhamdemo.c bresnham.$(o) bresnham.h
	$(cc) bhamdemo.c bresnham.$(o)
	$(rm) bhamdemo.$(o)

moreproc.$(x)	: moreproc.c scrnmacs.h ext_keys.$(o) isshift.$(o) \
		  ferrorf.$(o)
	$(ct) moreproc.c ext_keys.$(o) isshift.$(o) ferrorf.$(o)
	$(rm) moreproc.$(o)
	$(extra1)

adjscrol.$(x)	: adjscrol.c scrnmacs.h ext_keys.$(o) isshift.$(o) \
		  ferrorf.$(o) delay.$(o) faskbhit.$(o)
	$(ct) adjscrol.c ext_keys.$(o) isshift.$(o) ferrorf.$(o) delay.$(o) \
		faskbhit.$(o)
	$(rm) adjscrol.$(o)
	$(extra1)

more.$(x)	: more.c scrnmacs.h moreproc.$(o) ext_keys.$(o) isshift.$(o) \
		  ferrorf.$(o)
	$(cc) more.c moreproc.$(o) ext_keys.$(o) isshift.$(o) ferrorf.$(o)
	$(rm) more.$(o)

showansi.$(x)	: showansi.cpp scrintrp.$(o) video.$(o) ansisys.$(o) \
		  avatat.$(o)
	$(cc) showansi.cpp scrintrp.$(o) video.$(o) ansisys.$(o) avatat.$(o)
	$(rm) showansi.$(o)

uclock.$(x)	: uclock.h uclock.c uclock_.$(o) os_id.$(o)
	$(ct) uclock.c uclock_.$(o) os_id.$(o)
	$(rm) uclock.$(o)
	$(extra1)

mktone.$(x)	: mktone.c sound.h uclock.h uclock.$(o) uclock_.$(o) \
		  os_id.$(o)
	$(ct) mktone.c uclock.$(o) uclock_.$(o) os_id.$(o)
	$(rm) mktone.$(o)
	$(extra1)

playdemo.$(x)	: playdemo.c uclock.h sound.h sound.$(o) mktone.$(o) \
		  playlib.$(o) uclock.$(o) uclock_.$(o) os_id.$(o)
	$(cc) playdemo.c sound.$(o) mktone.$(o) playlib.$(o) \
		uclock.$(o) uclock_.$(o) os_id.$(o)
	$(rm) playdemo.$(o)

disk_sn.$(x) : disk_sn.c dirport.h disk_sn.h extkword.h
	$(ct) disk_sn.c
	$(rm) disk_sn.$(o)
	$(extra1)

regit.$(x)	: regit.c
	$(cc) regit.c
	$(rm) regit.$(o)

chkreg.$(x)	: chkreg.c
	$(cc) chkreg.c
	$(rm) chkreg.$(o)

printq.$(x)	: printq.c prnspool.h prnspool.$(o)
	$(cc) printq.c prnspool.$(o)
	$(rm) printq.$(o)

assignpr.$(x)	: assignpr.c sniprint.h
	$(ct) assignpr.c
	$(rm) assignpr.$(o)
	$(extra1)

prtscrn.$(x) : prtscrn.c dirport.h sniprint.h
	$(ct) prtscrn.c
	$(rm) prtscrn.$(o)
	$(extra1)

prtstat.$(x) : prtstat.c sniprint.h
	$(ct) prtstat.c
	$(rm) prtstat.$(o)
	$(extra1)

trapdemo.$(x)	: trapdemo.c trapflag.$(o)
	$(cc) trapdemo.c trapflag.$(o)
	$(rm) trapdemo.$(o)

os2_boot.$(x)	: os2_boot.c
	$(cc) os2_boot.c
	$(rm) os2_boot.$(o)

scrnpick.$(x)	: scrnpick.c mouse.h mouse.$(o)
	$(cc) scrnpick.c mouse.$(o)
	$(rm) scrnpick.$(o)

ext_keys.$(x)	: ext_keys.c hilobyte.h snipkbio.h ext_keys.h isshift.$(o)
	$(ct) ext_keys.c isshift.$(o)
	$(rm) ext_keys.$(o)
	$(extra1)

faskbhit.$(x)	: faskbhit.c dirport.h ext_keys.h ext_keys.$(o) isshift.$(o)
	$(ct) faskbhit.c ext_keys.$(o) isshift.$(o)
	$(rm) faskbhit.$(o)
	$(extra1)

isxkbrd.$(x) : isxkbrd.c snipkbio.h
	$(ct) isxkbrd.c
	$(rm) isxkbrd.$(o)
	$(extra1)

timegetc.$(x)	: timegetc.c snipkbio.h
	$(ct) timegetc.c
	$(rm) timegetc.$(o)
	$(extra1)

editgets.$(x)	: editgets.c sniptype.h ext_keys.h cursor.h editgets.h \
		  minmax.h ext_keys.$(o) isshift.$(o) cursor.$(o)
	$(ct) editgets.c ext_keys.$(o) isshift.$(o) cursor.$(o)
	$(rm) editgets.$(o)
	$(extra1)

getstrng.$(x)	: getstrng.c editgets.h
	$(ct) getstrng.c
	$(rm) getstrng.$(o)
	$(extra1)

xmstest.$(x)	: xmstest.c xms.$(o) xms.h
	$cc xmstest.c xms.$(o)
	$rm xmstest.$(o)

emstest.$(x)	: emstest.c ems.$(o) ems.h
	$cc emstest.c ems.$(o)
	$rm emstest.$(o)

strat.$(x)	: strat.c strat.h
	$(ct) strat.c
	$(rm) strat.$(o)
	$(extra1)

ccomcall.$(x)	: ccomcall.c int2e.h int2e.$(o)
	$(ct) ccomcall.c int2e.$(o)
	$(rm) ccomcall.$(o)
	$(extra1)

glbl_env.$(x)	: glbl_env.c
	$(cc) glbl_env.c
	$(rm) glbl_env.$(o)

mcb_env.$(x) : mcb_env.c
	$(cc) mcb_env.c
	$(rm) mcb_env.$(o)

setenvar.$(x)	: setenvar.c ferrorf.$(o) kb_stuff.$(o)
	$(cc) setenvar.c ferrorf.$(o) kb_stuff.$(o)
	$(rm) setenvar.$(o)

2dlife.$(x)	: 2dlife.c vidport.$(o) scroll.$(o)
	$(cc) 2dlife.c vidport.$(o) scroll.$(o)
	$(rm) 2dlife.$(o)

ansiself.$(x)	: ansiself.c
	$(cc) ansiself.c
	$(rm) ansiself.$(o)

bigfac.$(x)	: bigfac.c
	$(cc) bigfac.c
	$(rm) bigfac.$(o)

bincomp.$(x) : bincomp.c minmax.h
	$(cc) bincomp.c
	$(rm) bincomp.$(o)

bordcolr.$(x)	: bordcolr.c
	$(cc) bordcolr.c
	$(rm) bordcolr.$(o)

cdir.$(x)	: cdir.c
	$(cc) cdir.c
	$(rm) cdir.$(o)

chbytes.$(x) : chbytes.c sniptype.h
	$(cc) chbytes.c
	$(rm) chbytes.$(o)

chmod.$(x)	: chmod.c
	$(cc) chmod.c
	$(rm) chmod.$(o)

cmdline.$(x) : cmdline.c snip_str.h
	$(cc) cmdline.c
	$(rm) cmdline.$(o)

cursize.$(x) : cursize.c
	$(cc) cursize.c
	$(rm) cursize.$(o)

do.$(x)	: do.c
	$(cc) do.c
	$(rm) do.$(o)

nlcnvrt.$(x)	: nlcnvrt.c sniptype.h errors.h ferrorf.obj err_exit.obj
	$(cc) nlcnvrt.c ferrorf.obj err_exit.obj
	$(rm) nlcnvrt.$(o)

dossort.$(x) : dossort.c
	$(cc) dossort.c
	$(rm) dossort.$(o)

drivsrch.$(x)	: drivsrch.c
	$(cc) drivsrch.c
	$(rm) drivsrch.$(o)

drvs.$(x)	: drvs.c
	$(cc) drvs.c
	$(rm) drvs.$(o)

# Borland only!
dspclock.$(x)	: dspclock.c
	$(cc) dspclock.c
	$(rm) dspclock.$(o)

dspdtst.$(x) : dspdtst.c ferrorf.$(o) vidport.$(o) scroll.$(o) dvidport.$(o)
	$(cc) dspdtst.c ferrorf.$(o) vidport.$(o) scroll.$(o) dvidport.$(o)
	$(rm) dspdtst.$(o)

errfix.$(x)	: errfix.c extkword.h
	$(cc) errfix.c
	$(rm) errfix.$(o)

factor.$(x)	: factor.c
	$(cc) factor.c
	$(rm) factor.$(o)

filcount.$(x)	: filcount.c sniptype.h dirport.h
	$(cc) filcount.c
	$(rm) filcount.$(o)

filecat.$(x)	: filecat.c maxline.$(o)
	$(cc) filecat.c maxline.$(o)
	$(rm) filecat.$(o)

maxline.$(x)	: maxline.c
	$(ct) maxline.c
	$(rm) maxline.$(o)
	$(extra1)

flopcopy.$(x)	: flopcopy.c snipfile.h wb_fcopy.$(o)
	$(cc) flopcopy.c wb_fcopy.$(o)
	$(rm) flopcopy.$(o)

fraction.$(x)	: fraction.c
	$(ct) fraction.c
	$(rm) fraction.$(o)
	$(extra1)

getvol.$(x)	: getvol.c
	$(ct) getvol.c
	$(rm) getvol.$(o)
	$(extra1)

grafline.$(x)	: grafline.c
	$(cc) grafline.c
	$(rm) grafline.$(o)

head.$(x)	: head.c
	$(cc) head.c
	$(rm) head.$(o)

hexdump.$(x) : hexdump.c ferrorf.$(o) err_exit.$(o)
	$(cc) hexdump.c ferrorf.$(o) err_exit.$(o)
	$(rm) hexdump.$(o)

howdy.$(x)	: howdy.c
	$(cc) howdy.c
	$(rm) howdy.$(o)

ifactor.$(x) : ifactor.c
	$(cc) ifactor.c
	$(rm) ifactor.$(o)

inchcvrt.$(x)	: inchcvrt.c round.h
	$(cc) inchcvrt.c
	$(rm) inchcvrt.$(o)

kbflip.$(x)	: kbflip.c
	$(cc) kbflip.c
	$(rm) kbflip.$(o)

keywatch.$(x)	: keywatch.c
	$(cc) keywatch.c
	$(rm) keywatch.$(o)

krnldemo.$(x)	: krnldemo.c
	$(cc) krnldemo.c
	$(rm) krnldemo.$(o)

killff.$(x)	: killff.c
	$(cc) killff.c
	$(rm) killff.$(o)

line.$(x)	: line.c
	$(cc) line.c
	$(rm) line.$(o)

log.$(x)	: log.c
	$(cc) log.c
	$(rm) log.$(o)

lsd.$(x)	: lsd.c dirport.h sniptype.h
	$(cc) lsd.c
	$(rm) lsd.$(o)

lzhuf.$(x)	: lzhuf.c
	$(cc) lzhuf.c
	$(rm) lzhuf.$(o)

mainmain.$(x)	: mainmain.c
	$(cc) mainmain.c
	$(rm) mainmain.$(o)

maze_1.$(x)	: maze_1.c
	$(cc) maze_1.c
	$(rm) maze_1.$(o)

maze_2.$(x)	: maze_2.c
	$(cc) maze_2.c
	$(rm) maze_2.$(o)

maze_3.$(x)	: maze_3.c
	$(cc) maze_3.c
	$(rm) maze_3.$(o)

morse.$(x)	: morse.c
	$(cc) morse.c
	$(rm) morse.$(o)

mterm.$(x)	: mterm.c
	$(cc) mterm.c
	$(rm) mterm.$(o)

mv.$(x)	: mv.c snipfile.h dosfiles.h wb_fcopy.$(o) drvalid.$(o) \
	  absdisk.$(o) absdiskc.$(o)
	$(cc) mv.c wb_fcopy.$(o) drvalid.$(o) absdisk.$(o) absdiskc.$(o)
	$(rm) mv.$(o)

palndrom.$(x)	: palndrom.c
	$(cc) palndrom.c
	$(rm) palndrom.$(o)

permute1.$(x)	: permute1.c
	$(cc) permute1.c
	$(rm) permute1.$(o)

permute2.$(x)	: permute2.c
	$(cc) permute2.c
	$(rm) permute2.$(o)

pr.$(x)	: pr.c getopts.h getopts.$(o) fnsplit.$(o) ferrorf.$(o) \
		  unix2dos.$(o) err_exit.$(o)
	$(cc) pr.c getopts.$(o) fnsplit.$(o) ferrorf.$(o) unix2dos.$(o) \
		err_exit.$(o)
	$(rm) pr.$(o)

prntself.$(x)	: prntself.c
	$(cc) prntself.c
	$(rm) prntself.$(o)

query.$(x)	: query.c
	$(cc) query.c
	$(rm) query.$(o)

rdxcnvrt.$(x)	: rdxcnvrt.c
	$(ct) rdxcnvrt.c
	$(rm) rdxcnvrt.$(o)
	$(extra1)

remtab.$(x)	: remtab.c
	$(cc) remtab.c
	$(rm) remtab.$(o)

reversi.$(x) : reversi.c
	$(cc) reversi.c
	$(rm) reversi.$(o)

rm_all.$(x)	: rm_all.c sniptype.h dirport.h unistd.h
	$(cc) rm_all.c
	$(rm) rm_all.$(o)

seqtouch.$(x)	: seqtouch.c getopts.$(o) pushdir.$(o) err_exit.$(o) \
		  existsx.$(o) stptok.$(o) unix2dos.$(o) ftime.$(o) \
		  parsdate.$(o) parstime.$(o) ferrorf.$(o) drvalid.$(o) \
		  absdisk.$(o) absdiskc.$(o) getyn.$(o) fnsplit.$(o)
	$(cc) seqtouch.c getopts.$(o) pushdir.$(o) err_exit.$(o) \
	      existsx.$(o) stptok.$(o) unix2dos.$(o) ftime.$(o) \
	      parsdate.$(o) parstime.$(o) ferrorf.$(o) drvalid.$(o) \
	      absdisk.$(o) absdiskc.$(o) getyn.$(o) fnsplit.$(o)
	$(rm) seqtouch.$(o)

setimeto.$(x)	: setimeto.c ftime.h ftime.$(o)
	$(cc) setimeto.c ftime.$(o)
	$(rm) setimeto.$(o)

setvol.$(x)	: setvol.c dirport.h dos5boot.h pushdir.$(o) drvalid.$(o) \
		  absdisk.$(o) absdiskc.$(o) unistd.h
	$(ct) setvol.c pushdir.$(o) drvalid.$(o) absdisk.$(o) absdiskc.$(o)
	$(rm) setvol.$(o)
	$(extra1)

skiplist.$(x)	: skiplist.c
	$(cc) skiplist.c
	$(rm) skiplist.$(o)

speed.$(x)	: speed.c ferrorf.$(o)
	$(cc) speed.c ferrorf.$(o)
	$(rm) speed.$(o)

spin.$(x)	: spin.c
	$(cc) spin.c
	$(rm) spin.$(o)

split.$(x)	: split.c fnsplit.$(o)
	$(cc) split.c fnsplit.$(o)
	$(rm) split.$(o)

stats.$(x)	: stats.c ferrorf.$(o)
	$(cc) stats.c ferrorf.$(o)
	$(rm) stats.$(o)

stripeof.$(x)	: stripeof.c
	$(cc) stripeof.c
	$(rm) stripeof.$(o)

stub.$(x)	: stub.c
	$(cc) stub.c
	$(rm) stub.$(o)

sunriset.$(x)	: sunriset.c
	$(cc) sunriset.c
	$(rm) sunriset.$(o)

tabtrick.$(x)	: tabtrick.c
	$(cc) tabtrick.c
	$(rm) tabtrick.$(o)

tail.$(x)	: tail.c
	$(cc) tail.c
	$(rm) tail.$(o)

timer.$(x)	: timer.c
	$(cc) timer.c
	$(rm) timer.$(o)

tiresize.$(x)	: tiresize.c
	$(cc) tiresize.c
	$(rm) tiresize.$(o)

#  SNIPPETS.NDX to 4DOS conversion utility
to4dos.$(x)	: to4dos.c snipfile.h ferrorf.$(o)
	$(cc) to4dos.c ferrorf.$(o)
	$(rm) to4dos.$(o)

todaybak.$(x)	: todaybak.c
	$(cc) todaybak.c
	$(rm) todaybak.$(o)

touch.$(x)	: touch.c ftime.h ftime.$(o)
	$(cc) touch.c ftime.$(o)
	$(rm) touch.$(o)

treedir.$(x) : treedir.c sniptype.h dirport.h
	$(cc) treedir.c
	$(rm) treedir.$(o)

uuencode.$(x)	: uuencode.c
	$(cc) uuencode.c
	$(rm) uuencode.$(o)

uudecode.$(x)	: uudecode.c
	$(cc) uudecode.c
	$(rm) uudecode.$(o)

wc.$(x)	: wc.c
	$(cc) wc.c
	$(rm) wc.$(o)

weird.$(x)	: weird.c
	$(cc) weird.c
	$(rm) weird.$(o)

where.$(x)	: where.c dirport.h
	$(cc) where.c
	$(rm) where.$(o)

wordwrap.$(x)	: wordwrap.c
	$(cc) wordwrap.c
	$(rm) wordwrap.$(o)

# Microsoft or Borland only!
wputch.$(x) : wputch.c
	$(cc) wputch.c
	$(rm) wputch.$(o)

t_clean.$(x) : t_clean.c sniptype.h dirport.h ftime.h scaldate.h today.$(o) \
		  scaldate.$(o) fdate.$(o) ftime.$(o) unistd.h
	$(cc) t_clean.c today.$(o) scaldate.$(o) fdate.$(o) ftime.$(o)
	$(rm) t_clean.$(o)

jgrep.$(x)	: jgrep.c ferrorf.$(o)
	$(cc) jgrep.c ferrorf.$(o)
	$(rm) jgrep.$(o)

grep.$(x)	: grep.c
	$(cc) grep.c
	$(rm) grep.$(o)

compiler.$(x)	: compiler.c
	$(cc) compiler.c
	$(rm) compiler.$(o)

myiodemo.$(x)	: myio.h mystream.h myline.h myiodemo.cpp myio.$(o) \
		  mystream.$(o) myline.$(o)
	$(cc) myiodemo.cpp myio.$(o) mystream.$(o) myline.$(o)
	$(rm) myiodemo.$(o)

lttest.$(x) : loctm.h lttest.cpp loctm.$(o) locdfmt.$(o) loctfmt.$(o) locfmt.$(o)
	$(cc) lttest.cpp loctm.$(o) locdfmt.$(o) loctfmt.$(o) locfmt.$(o)
	$(rm) lttest.$(o)

strecpy.$(o)	: snip_str.h strecpy.c
	$(cc) -c strecpy.c


#  Make target "allc" to compile all C executables

allc	:\
	bastrngs.$(x)\
	ccard.$(x)\
	ctrlprnt.$(x)\
	err_exit.$(x)\
	ferrorf.$(x)\
	initvars.$(x)\
	isisbn.$(x)\
	ansiflen.$(x)\
	existsx.$(x)\
	fcompare.$(x)\
	srchfile.$(x)\
	textmod.$(x)\
	update.$(x)\
	memmem.$(x)\
	sstrdel.$(x)\
	stptok.$(x)\
	strchcat.$(x)\
	strdel.$(x)\
	strdelch.$(x)\
	stristr.$(x)\
	strrepl.$(x)\
	strrev.$(x)\
	strrpbrk.$(x)\
	strupr.$(x)\
	ruleline.$(x)\
	trim.$(x)\
	hugesort.$(x)\
	ll_msort.$(x)\
	index.$(x)\
	lookup.$(x)\
	dtotp6.$(x)\
	bcdl.$(x)\
	bcdd.$(x)\
	combin.$(x)\
	cubic.$(x)\
	dbl2long.$(x)\
	dblround.$(x)\
	etphi.$(x)\
	factoryl.$(x)\
	fibo.$(x)\
	ipow.$(x)\
	ispow2.$(x)\
	isqrt.$(x)\
	rad2deg.$(x)\
	rand1.$(x)\
	rg_rand.$(x)\
	bigtest.$(x)\
	bascnvrt.$(x)\
	commaflt.$(x)\
	commafmt.$(x)\
	eng.$(x)\
	eval.$(x)\
	fmtmoney.$(x)\
	hexorint.$(x)\
	hstr_i.$(x)\
	ltoa.$(x)\
	ltostr.$(x)\
	ord_text.$(x)\
	scanfrac.$(x)\
	str27seg.$(x)\
	amalloc.$(x)\
	mdasort.$(x)\
	bitcnt_1.$(x)\
	bitcnt_2.$(x)\
	bitcnt_3.$(x)\
	bitcnt_4.$(x)\
	bitcnts.$(x)\
	bitfiles.$(x)\
	bitstrng.$(x)\
	bstr_i.$(x)\
	hash.$(x)\
	match.$(x)\
	soundex.$(x)\
	soundex4.$(x)\
	soundex5.$(x)\
	metaphon.$(x)\
	approx.$(x)\
	getcmt.$(x)\
	testcmt.$(x)\
	c_cmnt.$(x)\
	commconv.$(x)\
	remcmmnt.$(x)\
	csplit.$(x)\
	daynum.$(x)\
	isdst.$(x)\
	today.$(x)\
	dow.$(x)\
	fdate.$(x)\
	cal.$(x)\
	jdn_l.$(x)\
	easter.$(x)\
	addtime.$(x)\
	moon_age.$(x)\
	parsdate.$(x)\
	parstime.$(x)\
	strftime.$(x)\
	w_wrap.$(x)\
	center.$(x)\
	arccrc16.$(x)\
	crc_16f.$(x)\
	crc_32.$(x)\
	checksum.$(x)\
	checkexe.$(x)\
	getoptst.$(x)\
	getoptsl.$(x)\
	palnfilt.$(x)\
	pbmsrch.$(x)\
	jmalloc.$(x)\
	posix_ls.$(x)\
	xstrcmp.$(x)\
	chgext.$(x)\
	fnsplit.$(x)\
	vfname.$(x)\
	addhndls.$(x)\
	addpath.$(x)\
	drvalid.$(x)\
	favail.$(x)\
	fchsize.$(x)\
	files.$(x)\
	getdcwd.$(x)\
	iscons.$(x)\
	isfopen.$(x)\
	isnetdr.$(x)\
	isramdsk.$(x)\
	iswprot.$(x)\
	mkdirs.$(x)\
	truname.$(x)\
	filelist.$(x)\
	delay.$(x)\
	dosfrmat.$(x)\
	format.$(x)\
	isshare.$(x)\
	memavail.$(x)\
	nwlinnam.$(x)\
	os_id.$(x)\
	pfopen.$(x)\
	redirect.$(x)\
	shel2dos.$(x)\
	xtest.$(x)\
	vidport.$(x)\
	scrnsave.$(x)\
	fscrnsav.$(x)\
	ansiload.$(x)\
	bresnham.$(x)\
	bhamdemo.$(x)\
	moreproc.$(x)\
	adjscrol.$(x)\
	more.$(x)\
	uclock.$(x)\
	playdemo.$(x)\
	disk_sn.$(x)\
	regit.$(x)\
	chkreg.$(x)\
	printq.$(x)\
	assignpr.$(x)\
	prtscrn.$(x)\
	prtstat.$(x)\
	trapdemo.$(x)\
	scrnpick.$(x)\
	ext_keys.$(x)\
	faskbhit.$(x)\
	isxkbrd.$(x)\
	timegetc.$(x)\
	editgets.$(x)\
	getstrng.$(x)\
	strat.$(x)\
	ccomcall.$(x)\
	glbl_env.$(x)\
	mcb_env.$(x)\
	2dlife.$(x)\
	ansiself.$(x)\
	bigfac.$(x)\
	bincomp.$(x)\
	bordcolr.$(x)\
	cdir.$(x)\
	chbytes.$(x)\
	chmod.$(x)\
	cmdline.$(x)\
	cursize.$(x)\
	do.$(x)\
	nlcnvrt.$(x)\
	dossort.$(x)\
	drivsrch.$(x)\
	drvs.$(x)\
	dspdtst.$(x)\
	errfix.$(x)\
	factor.$(x)\
	filcount.$(x)\
	maxline.$(x)\
	filecat.$(x)\
	flopcopy.$(x)\
	fraction.$(x)\
	getvol.$(x)\
	grafline.$(x)\
	hamlet.$(x)\
	head.$(x)\
	hexdump.$(x)\
	howdy.$(x)\
	ifactor.$(x)\
	inchcvrt.$(x)\
	kbflip.$(x)\
	keywatch.$(x)\
	krnldemo.$(x)\
	killff.$(x)\
	line.$(x)\
	log.$(x)\
	lsd.$(x)\
	lzhuf.$(x)\
	mainmain.$(x)\
	maze_1.$(x)\
	maze_2.$(x)\
	maze_3.$(x)\
	morse.$(x)\
	mterm.$(x)\
	palndrom.$(x)\
	permute1.$(x)\
	permute2.$(x)\
	spigot.$(x)\
	pi.$(x)\
	pi8.$(x)\
	pi_agm.$(x)\
	pr.$(x)\
	prntself.$(x)\
	query.$(x)\
	rdxcnvrt.$(x)\
	remtab.$(x)\
	reversi.$(x)\
	rm_all.$(x)\
	l2roman.$(x)\
	roman2l.$(x)\
	seqtouch.$(x)\
	setimeto.$(x)\
	setvol.$(x)\
	speed.$(x)\
	spin.$(x)\
	split.$(x)\
	stats.$(x)\
	stripeof.$(x)\
	stub.$(x)\
	sunriset.$(x)\
	tabtrick.$(x)\
	tail.$(x)\
	timer.$(x)\
	tiresize.$(x)\
	todaybak.$(x)\
	touch.$(x)\
	treedir.$(x)\
	uuencode.$(x)\
	uudecode.$(x)\
	wc.$(x)\
	weird.$(x)\
	where.$(x)\
	wordwrap.$(x)\
	t_clean.$(x)\
	jgrep.$(x)\
	grep.$(x)\
	compiler.$(x)\
	mv.$(x)


#  Make target "allcpp" to compile all C++ executables

allcpp	:\
	cards.$(x)\
	datedemo.$(x)\
	myiodemo.$(x)\
	lttest.$(x)\
	jnew.$(x)\
	showansi.$(x)\
	cmtconvr.$(x)\
	cmtcount.$(x)\
	cmtremov.$(x)\
	cmtxtrac.$(x)\
	whicharc.$(x)


#  Make target "bachmlib" to compile Erik Bachmann's library functions

bachmlib	:\
	Date__.$(o)\
	Date__.$(o)\
	Time__.$(o)\
	Cmpstr.$(o)\
	Modulus.$(o)\
	Repstr.$(o)\
	Strcase.$(o)\
	Strnsub.$(o)\
	Strrepc.$(o)\
	Strtrim.$(o)\
	Strtrimc.$(o)\
	Fsif.$(o)



#  Make target "allobj" to compile all C and C++ linkable objects

allobj	:\
	absdisk.$(o)\
	dblround.$(o)\
	etphi.$(o)\
	rmallws.$(o)\
	strchcat.$(o)\
	posixdir.$(o)\
	unix2dos.$(o)\
	scaldate.$(o)\
	daynum.$(o)\
	ftime.$(o)\
	today.$(o)\
	w_wrap.$(o)\
	getopts.$(o)\
	getoptsl.$(o)\
	dirmask.$(o)\
	trim.$(o)\
	uclock_.$(o)\
	absdiskc.$(o)\
	pushdir.$(o)\
	drvalid.$(o)\
	xfile.$(o)\
	scroll.$(o)\
	vidport.$(o)\
	fmemops.$(o)\
	vfname.$(o)\
	hugeread.$(o)\
	trapflag.$(o)\
	os_id.$(o)\
	uclock.$(o)\
	sound.$(o)\
	mktone.$(o)\
	playlib.$(o)\
	prnspool.$(o)\
	int2e.$(o)\
	mouse.$(o)\
	getyn.$(o)\
	kb_stuff.$(o)\
	isshift.$(o)\
	ext_keys.$(o)\
	pchwio.$(o)\
	cursor.$(o)\
	parsdate.$(o)\
	xstrcmp.$(o)\
	wb_fcopy.$(o)\
	fdate.$(o)\
	a2e.$(o)\
	toascii.$(o)\
	bastrngs.$(o)\
	cards.$(o)\
	ccard.$(o)\
	ctrlprnt.$(o)\
	err_exit.$(o)\
	ferrorf.$(o)\
	fscanbin.$(o)\
	initvars.$(o)\
	isisbn.$(o)\
	memrev.$(o)\
	strucfil.$(o)\
	whicharc.$(o)\
	archive.$(o)\
	windchil.$(o)\
	ansiflen.$(o)\
	existsx.$(o)\
	fcompare.$(o)\
	fopenx.$(o)\
	srchfile.$(o)\
	wb_fapnd.$(o)\
	update.$(o)\
	memmem.$(o)\
	sstrcpy.$(o)\
	sstrdel.$(o)\
	stptok.$(o)\
	strdel.$(o)\
	strdelch.$(o)\
	strdup.$(o)\
	strecpy.$(o)\
	stristr.$(o)\
	strrepl.$(o)\
	strrev.$(o)\
	strrpbrk.$(o)\
	strupr.$(o)\
	translat.$(o)\
	xstrcat.$(o)\
	ruleline.$(o)\
	rmlead.$(o)\
	rmtrail.$(o)\
	trim.$(o)\
	lv1ws.$(o)\
	hugesort.$(o)\
	ll_msort.$(o)\
	ll_qsort.$(o)\
	rg_isort.$(o)\
	rg_qsort.$(o)\
	rgiqsort.$(o)\
	rg_ssort.$(o)\
	strsort.$(o)\
	dtotp6.$(o)\
	bcdl.$(o)\
	bcdd.$(o)\
	combin.$(o)\
	cubic.$(o)\
	dbl2long.$(o)\
	factoryl.$(o)\
	fibo.$(o)\
	frand.$(o)\
	ipow.$(o)\
	ispow2.$(o)\
	isqrt.$(o)\
	ldfloor.$(o)\
	msb2ieee.$(o)\
	perm_idx.$(o)\
	rad2deg.$(o)\
	triglib.$(o)\
	rand2.$(o)\
	rg_rand.$(o)\
	bignum1.$(o)\
	bignum2.$(o)\
	bascnvrt.$(o)\
	commaflt.$(o)\
	commafmt.$(o)\
	eng.$(o)\
	eval.$(o)\
	fmtmoney.$(o)\
	hexorint.$(o)\
	hstr_i.$(o)\
	ltoa.$(o)\
	ltostr.$(o)\
	ord_text.$(o)\
	scanfrac.$(o)\
	str27seg.$(o)\
	l2roman.$(o)\
	roman2l.$(o)\
	amalloc.$(o)\
	mdalloc.$(o)\
	bitarray.$(o)\
	bitcnt_1.$(o)\
	bitcnt_2.$(o)\
	bitcnt_3.$(o)\
	bitcnt_4.$(o)\
	bitfiles.$(o)\
	bitstrng.$(o)\
	bstr_i.$(o)\
	lls.$(o)\
	lls_blob.$(o)\
	lld.$(o)\
	lld_blob.$(o)\
	stack.$(o)\
	stk_blob.$(o)\
	deque.$(o)\
	stk.$(o)\
	hash.$(o)\
	match.$(o)\
	soundex.$(o)\
	soundex4.$(o)\
	soundex5.$(o)\
	metaphon.$(o)\
	approx.$(o)\
	isdst.$(o)\
	today.$(o)\
	dow.$(o)\
	jdn_l.$(o)\
	easter.$(o)\
	addtime.$(o)\
	moon_age.$(o)\
	parsdate.$(o)\
	parstime.$(o)\
	strftime.$(o)\
	date.$(o)\
	w_wrap.$(o)\
	center.$(o)\
	arccrc16.$(o)\
	crc_16.$(o)\
	crc_16f.$(o)\
	crc_32.$(o)\
	checksum.$(o)\
	checkexe.$(o)\
	pbmsrch.$(o)\
	bmhsrch.$(o)\
	bmhisrch.$(o)\
	bmhasrch.$(o)\
	mem.$(o)\
	jmalloc.$(o)\
	jnew.$(o)\
	dosgetch.$(o)\
	fln_fix.$(o)\
	flnorm.$(o)\
	chgext.$(o)\
	fnsplit.$(o)\
	addhndls.$(o)\
	addpath.$(o)\
	doscopy.$(o)\
	favail.$(o)\
	fchsize.$(o)\
	files.$(o)\
	getdcwd.$(o)\
	iscons.$(o)\
	isfopen.$(o)\
	isnetdr.$(o)\
	isramdsk.$(o)\
	iswprot.$(o)\
	mkdirs.$(o)\
	truname.$(o)\
	filelist.$(o)\
	biport.$(o)\
	delay.$(o)\
	break.$(o)\
	fndislot.$(o)\
	dosfrmat.$(o)\
	fptr_add.$(o)\
	isshare.$(o)\
	joystick.$(o)\
	memavail.$(o)\
	nwlinnam.$(o)\
	pfopen.$(o)\
	redirect.$(o)\
	shel2dos.$(o)\
	vidport.$(o)\
	scrnsave.$(o)\
	fscrnsav.$(o)\
	atr2ansi.$(o)\
	dvideo.$(o)\
	dvidport.$(o)\
	ansiload.$(o)\
	bresnham.$(o)\
	moreproc.$(o)\
	adjscrol.$(o)\
	doansi_1.$(o)\
	doansi_2.$(o)\
	scrintrp.$(o)\
	video.$(o)\
	ansisys.$(o)\
	avatar.$(o)\
	disk_sn.$(o)\
	x00api.$(o)\
	queue.$(o)\
	serial.$(o)\
	modemio.$(o)\
	assignpr.$(o)\
	changprn.$(o)\
	prtoggle.$(o)\
	prtscrn.$(o)\
	prtstat.$(o)\
	faskbhit.$(o)\
	isxkbrd.$(o)\
	keylocks.$(o)\
	timegetc.$(o)\
	editgets.$(o)\
	getstrng.$(o)\
	strat.$(o)\
	xms.$(o)\
	ems.$(o)\
	ccomcall.$(o)\
	fraction.$(o)\
	getvol.$(o)\
	8087_sav.$(o)\
	cctrap.$(o)\
	cbtrap.$(o)\
	cerrinst.$(o)\
	cerrtrap.$(o)\
	reboot.$(o)\
	maxline.$(o)\
	ntstream.$(o)\
	list.$(o)\
	str.$(o)\
	date.$(o)\
	myio.$(o)\
	mystream.$(o)\
	myline.$(o)\
	loctm.$(o)\
	locdfmt.$(o)\
	loctfmt.$(o)\
	locfmt.$(o)
