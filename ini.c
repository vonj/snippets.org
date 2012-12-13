/*
** This file is placed into the public domain on February 22, 1996,  by
** its author: Carey Bloodworth
**
** Modifications:
**
** 07-Dec-1999 by
** Bob Stout & Jon Guthrie
**  General cleanup, use NUL (in SNIPTYPE.H) instead of NULL where appropriate.
**  Allow spaces in tag names.
**  Allow strings in quotes.
**  Allow trailing comments.
**  Allow embedded comment separator(s) in quoted strings.
**  Add comments.
**  ReadCfg() now returns the number of variables found if no error occurred.
**  Changed integer type to short.
**  Use cant() calls in lieu of fopen() calls,
**    include ERRORS.H for prototype.
**  Fix parsing error with null string data.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "ini.h"
#include "errors.h"
#include "filnames.h"

#define BUFFERSIZE (INI_LINESIZE + 2)

enum LineTypes {
      EmptyLine = 0, CommentLine = 1, InSection = 2, NotInSection = 3,
      NewSection = 4, FoundSection = 5, LeavingSection = 6
};

/*
**  StripLeadingSpaces() - Strips leading spaces from a string.
**
**  Paramters: 1 - String.
**
**  Returns: Pointer to first non-whitespace character in the string.
**
**  Note: This does not modify the original string.
*/

static char *StripLeadingSpaces(char *string)
{
      if (!string || (0 == strlen(string)))
            return NULL;

      while ((*string) && (isspace(*string)))
            string++;
      return string;
}

/*
**  StripTrailingSpaces() - Strips traling spaces from a string.
**
**  Paramters: 1 - String.
**
**  Returns: Pointer to the string.
**
**  Note: This does modify the original string.
*/

static char *StripTrailingSpaces(char *string)
{
      if (!string || (0 == strlen(string)))
            return NULL;

      while (isspace(LAST_CHAR(string)))
            LAST_CHAR(string) = NUL;

      return string;
}

/*
**  ReadLine() - Read a line from the active file
**
**  Paramters: 1 - File pointer of the active file.
**             2 - Pointer to the line's storage.
**
**  Returns: Length of line or EOF.
**
**  Side effects: Strips newline characters (DOS, Unix, or Mac).
*/

static int ReadLine(FILE *fp, char *line)
{
      char *cp;

      cp = fgets(line, INI_LINESIZE, fp);

      if (NULL == cp)
      {
            *line = 0;
            return EOF;
      }
      if (feof(fp))
      {
            *line = 0;
            return EOF;
      }
      if (0 != ferror(fp))
      {
            *line = 0;
            return EOF;
      }

      /*
      **  Allow both DOS and Unix style newlines.
      */
      
      while (strlen(line) && strchr("\n\r", LAST_CHAR(line)))
            LAST_CHAR(line) = NUL;

      return strlen(line);
}

/*
**  StrEq() - Case-insensitive string compare.
**
**  Paramters: 1 - First string.
**             2 - second string.
**
**  Returns: True_ if strings match, else False_.
*/

static Boolean_T StrEq(char *s1, char *s2)
{
      while (tolower(*s1) == tolower(*s2))
      {
            if (NUL == *s1)
                  return True_;
            s1++;
            s2++;
      }
      return False_;
}

/*
**  ParseLine() - This routine divides the line into two parts. The
**                variable, and the 'string'.
**
**  Paramters: 1 - The line to parse:
**             2 - Pointer to variable name storage.
**             3 = Pointer to textual data storage.
**
**  Returns: Nothing.
*/

static void ParseLine(char *line, char *var, char *data)
{
      int len = 0;

      line = StripLeadingSpaces(line);
      strcpy(var, line);
      strcpy(data, "");

      while (*line)
      {
            char *ptr;

            if (/*isspace(*line) || */ ('=' == *line))
            {
                  var[len] = 0;
                  var  = StripTrailingSpaces(var);
                  line = StripLeadingSpaces(line+1);

                  /* Remove a possible '=' */
                  /* This could allow var==string in some cases. No big deal*/

                  while (line && '=' == *line)
                        line = StripLeadingSpaces(line+1);
                  if (line)
                  {
                        strcpy(data, line);
                        if (NULL != (ptr = strrchr(data, ';')) &&
                            NULL == strchr(ptr, '\"'))
                        {
                              *ptr = NUL;
                        }
                        StripTrailingSpaces(data);
                  }
                  return;
            }
            else
            {
                  line++;
                  len++;
            }
      }
}

/*
**  SectionLine() - This routine checks each line of the file and identifies
**                  only those lines that are within the right section.
**
**  Paramters: 1 - The line to scan.
**             2 - The specific section wanted.
**             3 - The current section name.
**
**  Returns: Line type.
*/

static enum LineTypes SectionLine(char *line,
                                  char *SectionWanted,
                                  char *CurrentSection)
{
      enum LineTypes linetype;

      line = StripLeadingSpaces(line);
      if (!line || NUL == *line)
            return EmptyLine;

      /*
      ** Comments are started with a "%", ";" or "#"
      */

      if (';' == *line)
            return CommentLine;
      if ('%' == *line)
            return CommentLine;
      if ('#' == *line)
            return CommentLine;

      if ('[' == line[0])  /* Section Header */
      {
            linetype = NewSection;
            if (StrEq(CurrentSection, SectionWanted))
                  linetype = LeavingSection;

            strcpy(CurrentSection, line);
            if (StrEq(line, SectionWanted))
                  linetype = FoundSection;
      }
      else
      {            /* Just a regular line */
            linetype = NotInSection;
            if (StrEq(CurrentSection, SectionWanted))
                  linetype = InSection;
      }

      return linetype;
}

/*
**  ReadCfg() - Reads a .ini / .cfg file. May read multiple lines within the
**              specified section.
**
**  Paramters: 1 - File name
**             2 - Section name
**             3 - Array of CfgStruct pointers
**
**  Returns: Number of variables located
**           -1 if any type spec failed
**           -2 for any type of file error
*/

int ReadCfg(const char *FileName, char *SectionName, struct CfgStruct *MyVars)
{
      FILE *CfgFile;
      char line[BUFFERSIZE];
      char SectionWanted[BUFFERSIZE];
      char CurrentSection[BUFFERSIZE];
      enum LineTypes linetype;
      char var[BUFFERSIZE];
      char data[BUFFERSIZE], *dp;
      int retval = 0;
      struct CfgStruct *mv;

      CfgFile = cant((char *)FileName, "r");

      strcpy(CurrentSection, "[]");
      sprintf(SectionWanted, "[%s]", SectionName);

      while (EOF != ReadLine(CfgFile, line))
      {
            linetype = SectionLine(line, SectionWanted, CurrentSection);
            switch (linetype)
            {
            case EmptyLine:
                  break;  /* Nothing to parse */

            case CommentLine:
                  break;  /* Nothing to parse */

            case InSection:              /* In our section, parse it. */
            {
                  ParseLine(line, var, data);

                  for (mv = MyVars; mv->Name; ++mv)
                  {
                        if (StrEq(mv->Name, var))
                        {
                              switch (mv->VarType)
                              {
                              case Cfg_String:
                                    if ('\"' == *data)
                                    {
                                          dp = data + 1;
                                          data[strlen(data)-1] = NUL;
                                    }
                                    else  dp = data;
                                    /*
                                    ** Use sprintf to assure embedded
                                    ** escape sequences are handled.
                                    */
                                    sprintf(mv->DataPtr, dp);
                                    ++retval;
                                    break;

                              case Cfg_Byte:
                                    *((unsigned char*)mv->DataPtr) =
                                          (unsigned char)atoi(data);
                                    ++retval;
                                    break;

                              case Cfg_Ushort:
                                    *((unsigned int*)mv->DataPtr) =
                                          (unsigned int)atoi(data);
                                    ++retval;
                                    break;

                              case Cfg_Short:
                                    *((int*)mv->DataPtr) = atoi(data);
                                    ++retval;
                                    break;

                              case Cfg_Ulong:
                                    *((unsigned long*)mv->DataPtr) =
                                          (unsigned long)atol(data);
                                    ++retval;
                                    break;

                              case Cfg_Long:
                                    *((long*)mv->DataPtr) = atol(data);
                                    ++retval;
                                    break;

                              case Cfg_Double:
                                    *((double*)mv->DataPtr) = atof(data);
                                    ++retval;
                                    break;

                              case Cfg_Boolean:
                                    *((int*)mv->DataPtr) = 0;
                                    data[0] = tolower(data[0]);
                                    if (('y' == data[0]) || ('t' == data[0]))
                                          *((int*)mv->DataPtr) = 1;
                                    ++retval;
                                    break;

                              case Cfg_I_Array:
                              {
                                    int *ip;
                                    char *str;

                                    ip = ((int*)mv->DataPtr);
                                    str = strtok(data, " ,\t");
                                    while (NULL != str)
                                    {
                                          *ip = atoi(str);
                                          ip++;
                                          str = strtok(NULL, " ,\t");
                                    }
                                    ++retval;
                                    break;
                              }

                              default:
#ifdef TEST
                                    printf("Unknown conversion type\n");
#endif
                                    retval = -1;
                                    break;
                              }
                        }
                        if (-1 == retval)
                              break;
                  };

                  /*
                  ** Variable wasn't found.  If we don't want it,
                  ** then ignore it
                  */
            }
            case NotInSection:
                  break;  /* Not interested in this line */

            case NewSection:
                  break;  /* Who cares? It's not our section */

            case FoundSection:
                  break;  /* We found our section! */

            case LeavingSection:
                  break;  /* We finished our section! */
            }

            if (-1 == retval)
                  break;
      }

      if (ferror(CfgFile))
      {
            fclose(CfgFile);
            return -2;
      }
      else
      {
            fclose(CfgFile);
            return retval;
      }
}

/*
**  SearchCfg() - Search an .ini / .cfg file for a specific single datum.
**
**  Parameters: 1 - File name.
**              2 - Section name.
**              3 - Variable to find.
**              4 - Pointer to variable's storage.
**              5 - Type of vatiable.
**
**  Returns:  1 if succesful
**            0 if section/variable not found
**           -1 if invalid type spec
**           -2 for file error
*/

int SearchCfg(const char *FileName,
              char *SectionName,
              char *VarName,
              void *DataPtr,
              enum CfgTypes VarType
             )
{
      struct CfgStruct MyVars[2];

      MyVars[0].Name    = VarName;
      MyVars[0].DataPtr = DataPtr;
      MyVars[0].VarType = VarType;

      MyVars[1].Name = MyVars[1].DataPtr = NULL;

      return ReadCfg(FileName, SectionName, MyVars);
}

/*
**  UpdateCfg() - This will update a variable in a specific section in your
**                .ini file. It will do so safely by copying it to a new file,
**                and when finished, will delete the old one and rename the
**                new one to the correct name. If any fatal error occurs, it
**                will return a -1 to indiate failure. I generally don't care
**                why it failed, just knowing that it failed is usually enough.
**
**  Paramters: 1 - File name.
**             2 - Section name.
**             3 - Variable tag.
**             4 - Pointer to textual representation of the variable's value.
**
**  Returns: -1 if a file error occurred, else 0.
**
**  Notes: 1. If the section doesn't yet exist in the file, it will be added.
**         2. If the variable doesn't yet exist in the file, it will be added.
**         3. New variables are created at the end of existing sections, or
**            on the last line before the first section name in the case where
**            the section name is "".
**         4. New sections are created at the end of the file.
*/

int UpdateCfg(const char *FileName,
              char *SectionName,
              char *VarWanted,
              char *NewData)
{
      FILE *CfgFile;
      char line[BUFFERSIZE];
      char SectionWanted[BUFFERSIZE];
      char CurrentSection[BUFFERSIZE];
      enum LineTypes linetype;
      char var[BUFFERSIZE];
      char data[BUFFERSIZE];
      char TempFileName[FILENAME_MAX];
      FILE *NewCfgFile;
      int Error = 0;
      int updated = 0;

      CfgFile = cant((char *)FileName, "r");

      strcpy(TempFileName, FileName);
      chgext(TempFileName, NULL, "tmp");
      NewCfgFile = cant(TempFileName, "w");

      strcpy(CurrentSection, "[]");
      sprintf(SectionWanted, "[%s]", SectionName);

      while (EOF != ReadLine(CfgFile, line))
      {
            linetype = SectionLine(line, SectionWanted, CurrentSection);
            switch (linetype)
            {
            case InSection:              /* In our section, parse it. */
                  ParseLine(line, var, data);
                  if ((StrEq(var, VarWanted)) && (!updated))
                  {
                        strncpy(data, NewData, BUFFERSIZE);
                        data[BUFFERSIZE-1] = NUL;
                        updated = 1;
                  }
                  fprintf(NewCfgFile, "%s = %s\n", var, data);
                  break;

            case EmptyLine:         /* Fall Through.  Just copy it. */
            case CommentLine:       /* Fall Through.  Just copy it. */
            case NotInSection:      /* Fall Through.  Just copy it. */
            case NewSection:        /* Fall Through.  Just copy it. */
            case FoundSection:      /* Fall Through.  Just copy it. */
                  fprintf(NewCfgFile, "%s\n", line);
                  break;

            case LeavingSection:    /* Leaving section, may have to add it */
                  if (!updated)     /* Variable wasn't found, we have      */
                  {                 /* to add it.                          */
                        fprintf(NewCfgFile, "%s = %s\n", VarWanted, NewData);
                        updated = 1;
                  }
                  /*
                  ** Now print current line
                  */

                  fprintf(NewCfgFile, "%s\n", line);
                  break;
            }
      }

      /*
      ** Our section may not have even been there, in which case we have
      ** to add both the variable and the section itself.
      */

      if (!updated)
      {     /* We may have hit EOF while still in our section. */
            /* If so, we don't need to add the section header. */
            if (!StrEq(CurrentSection, SectionWanted))
                  fprintf(NewCfgFile, "%s\n", SectionWanted);
            fprintf(NewCfgFile, "%s = %s\n", VarWanted, NewData);
      }

      if (ferror(CfgFile))
            Error = -1;
      if (ferror(NewCfgFile))
            Error = -1;
      fclose(CfgFile);
      fclose(NewCfgFile);

      if (!Error)
      {
            if (remove(FileName))
                  return -1;
            if (rename(TempFileName, FileName))
                  return -1;
      }
      return Error;
}

#ifdef TEST

#include <stdlib.h>

#if defined(MSDOS) || defined (_MSDOS_)
 #define PINI_fname "prices.ini"
#else
 #define PINI_fname "/home/mars_nwe/engr/disp/prices.ini"
#endif

FILE *log_ = stderr;

main()
{
      char        Line[INI_LINESIZE];
      int         Int;
      long        Long;
      double      Double;
      Boolean_T   Bool;
      struct      CfgStruct this_var;
      FILE*       tst;
      long        prices[2];


      /*
      **  First work with a test file
      */
      tst = cant("test.ini", "w");
      fputs("[Section 1]\n", tst);
      fputs("[Section 2]\n", tst);
      fputs("[Section 3]\n", tst);
      fputs("[Section 4]\n", tst);
      fputs("[Section 5]\n", tst);
      fputs("[Section 6]\n", tst);
      fclose(tst);
      puts("Updating the test configuration file");

      puts("Updating section 1");
      UpdateCfg("test.ini", "Section 1", "string #1", "section 1 test");
      puts("Updating section 2");
      UpdateCfg("test.ini", "Section 2", "short #2", "2");
      puts("Updating section 3");
      UpdateCfg("test.ini", "Section 3", "long #3", "3");
      UpdateCfg("test.ini", "Section 4", "double #4", "4.4");
      UpdateCfg("test.ini", "Section 5", "boolean #5", "Y");
      UpdateCfg("test.ini", "Section 6", "boolean #6", "N");
      UpdateCfg("test.ini", "", "global string", "\"Hello, world!\" ;Comment");

      puts("I've finished the updates, now to try to get the data back");

      this_var.Name    = "global string";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(0) returned %d; Line=\n",
             ReadCfg("test.ini", "", &this_var));
      puts(Line);

      this_var.Name    = "string #1";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(1) returned %d; Line=\n",
             ReadCfg("test.ini", "Section 1", &this_var));
      puts(Line);

      this_var.Name    = "short #2";
      this_var.DataPtr = &Int;
      this_var.VarType = Cfg_Short;
      printf("ReadCfg(2) returned %d; Value= ",
             ReadCfg("test.ini", "Section 2", &this_var));
      printf("%d\n", Int);

      this_var.Name    = "long #3";
      this_var.DataPtr = &Long;
      this_var.VarType = Cfg_Long;
      printf("ReadCfg(3) returned %d; Value = ",
             ReadCfg("test.ini", "Section 3", &this_var));
      printf("%ld\n", Long);

      this_var.Name    = "double #4";
      this_var.DataPtr = &Double;
      this_var.VarType = Cfg_Double;
      printf("ReadCfg(4) returned %d; Value = ",
             ReadCfg("test.ini", "Section 4", &this_var));
      printf("%f\n", Double);

      this_var.Name    = "boolean #5";
      this_var.DataPtr = &Bool;
      this_var.VarType = Cfg_Boolean;
      printf("ReadCfg(5) returned %d; Value = ",
             ReadCfg("test.ini", "Section 5", &this_var));
      printf("%c\n", Bool ? 'T' : 'F');

      this_var.Name    = "boolean #6";
      this_var.DataPtr = &Bool;
      this_var.VarType = Cfg_Boolean;
      printf("ReadCfg(6) returned %d; Value = ",
             ReadCfg("test.ini", "Section 6", &this_var));
      printf("%c\n", Bool ? 'T' : 'F');

      /*
      **  Look for non-existant sections and/or variables
      */

      Line[0] = NUL;
      this_var.Name    = "string #99";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(99) returned %d; Line=\n",
             ReadCfg("test.ini", "Section 99", &this_var));
      puts(Line);

      Line[0] = NUL;
      this_var.Name    = "string #99";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(0/99) returned %d; Line=\n",
             ReadCfg("test.ini", "", &this_var));
      puts(Line);

      Line[0] = NUL;
      this_var.Name    = "string #99";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(1/99) returned %d; Line=\n",
             ReadCfg("test.ini", "Section 1", &this_var));
      puts(Line);

      Line[0] = NUL;
      this_var.Name    = "string #1";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(1/100) returned %d; Line=\n",
             ReadCfg("test.ini", "Section 100", &this_var));
      puts(Line);

      /*
      **  Next, add a section and new variables
      */

      UpdateCfg("test.ini", "", "new global variable", "abc");
      UpdateCfg("test.ini", "Section -1", "new variable", "xyz");
     
      /*
      **  Next work with a sample real PRICES.INI file
      */

      this_var.Name    = "Price of #1";
      this_var.DataPtr = &prices[0];
      this_var.VarType = Cfg_Long;
      printf("ReadCfg(1) returned %d; Value = ",
             ReadCfg(PINI_fname, "", &this_var));
      printf("%ld\n", prices[0]);

      this_var.Name    = "Price of #2";
      this_var.DataPtr = &prices[1];
      this_var.VarType = Cfg_Long;
      printf("ReadCfg(2) returned %d; Value = ",
             ReadCfg(PINI_fname, "", &this_var));
      printf("%ld\n", prices[1]);

      UpdateCfg("prices.ini", "", "Price of #2", "999");

      this_var.Name    = "Price of #2";
      this_var.DataPtr = &prices[1];
      this_var.VarType = Cfg_Long;
      printf("ReadCfg(2) returned %d; Value = ",
             ReadCfg(PINI_fname, "", &this_var));
      printf("%ld\n", prices[1]);

      UpdateCfg(PINI_fname, "", "Price of #2", "389");

      this_var.Name    = "Price of #2";
      this_var.DataPtr = &prices[1];
      this_var.VarType = Cfg_Long;
      printf("ReadCfg(2) returned %d; Value = ",
             ReadCfg(PINI_fname, "", &this_var));
      printf("%ld\n", prices[1]);

      /*
      **  Finally, try an invalid file name
      */

      this_var.Name    = "global string";
      this_var.DataPtr = Line;
      this_var.VarType = Cfg_String;
      printf("ReadCfg(0) returned %d; Line=\n",
             ReadCfg("none.ini", "", &this_var));
      puts(Line);

      return EXIT_SUCCESS;
}

#endif
