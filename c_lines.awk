# count lines in a C program, not counting comments, blank lines or 
# form feeds
# does separate count of preprocessor directives
# if a preprocessor directive is commented out, it does not count
#
# By: Dan Kozak
 
{
 if (file == "") {
  file = FILENAME
 }
 if (file != FILENAME) {
  printf("Number of lines in %s is: %d\n",file,nl+ppd)
  printf("Number of preprocessor directives is: %d\n",ppd)
  printf("Number of lines excluding preprocessor directives is: %d\n\n",nl)
  file = FILENAME
  tnl += nl
  tppd += ppd
  nl = 0
  ppd = 0
 }
 
 if ($0 == "") { ; }
 else if ($1 ~ /^\/\*/ && $NF ~ /\*\/$/) { ; }
 else if ($0 ~ /\/\*/ && $0 !~ /\*\//) { in_comment = 1 }
 else if ($0 !~ /\/\*/ && $0 ~ /\*\//) { in_comment = 0 }
 else if (in_comment) { ; }
 else if ($1 ~ /^#/) { ppd++ }
 else { nl++ }
}
 
END { printf("Number of lines in %s is: %d\n",file,nl+ppd)
      printf("Number of preprocessor directives is: %d\n",ppd)
      printf("Number of lines excluding preprocessor directives is: 
%d\n\n",nl)
      file = FILENAME
      tnl += nl
      tppd += ppd
      printf("Total number of lines is: %d\n",tnl+tppd)
      printf("Number of preprocessor directives is: %d\n",tppd)
      printf("Number of lines excluding preprocessor directives is: 
%d\n",tnl)
    }
