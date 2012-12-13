call set-dmclf
sc -c -DPC_TEST binsrch
sc -DPC_TEST -DTEST mathstat binsrch.obj
mathstat 107 108 109 7 7 8 9 1007 1008 1009 1009 108 > mathstat.out
