/*
**  WORDWRAP.C - Simple CRT word wrap demonstration routine
**
**  public domain by Robert Morgan
*/ 

#include <stdio.h>
#include <conio.h>
#include <string.h>

int get_ln(int rmargin); 
void clr_eol(const int curpos, const int pos); 
 
main() 
{ 
      printf("Enter text.  Press CTRL-A to quit.\n"); 
      while((get_ln(75)) != 0)      /* Change 75 to whatever number you */
            ;                       /* wish to be the right margin      */ 
      return 0;
} 
 
void clr_eol(const int curpos, const int pos) 
{ 
      int distance; 
      int count; 
 
      distance = curpos - pos; 
 
      for (count = 1; count <= distance; count++) 
            putch('\b'); 
      for (count = 1; count <= distance; count++) 
            putch(' '); 
} 
 
int get_ln(int rmargin) 
{ 
      char word[80]; 
      static int wordpos = 0; 
      static int curpos = 1; 
      static int ch = 0; 
      static int pos = 0; 

      word[wordpos] = '\0'; 
 
      while (ch != 1) 
      { 
            ch = getch(); 
 
            switch(ch) 
            { 
            case 1:
                  return(0); 
            case ' ':
                  pos = curpos; 
                  putch(' '); 
                  curpos++; 
                  wordpos = 0; 
                  word[0] = '\0'; 
                  break; 
            case '\b':
                  putch('\b'); 
                  curpos--; 
                  if (wordpos > 0) 
                        wordpos--; 
                  break; 
            case '\r':
                  puts("\r"); 
                  wordpos = 0; 
                  word[wordpos] = '\0'; 
                  curpos = 1; 
                  pos = 0; 
                  break; 
            default:
                  putch(ch); 
                  word[wordpos] = (char)ch; 
                  curpos++; 
                  wordpos++; 
                  break; 
            } 

            if(curpos == rmargin) 
            { 
                  word[wordpos] = '\0'; 
                  clr_eol(curpos,pos); 
                  wordpos = 0; 
                  curpos = strlen(word); 
                  pos = 0; 
                  puts("\r"); 
                  printf("%s",word); 
            } 
      } 
      return -1;
} 
