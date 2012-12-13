@echo off
if exist $qmtemp.cmd del $qmtemp.cmd > nul
qmenup
if exist $qmtemp.cmd $qmtemp
