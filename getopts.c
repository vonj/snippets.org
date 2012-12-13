/************************************************************************/
/*                                                                      */
/*  GETOPTS.C - Universal command line options parser                   */
/*                                                                      */
/*  Original Copyright 1990-96 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

/*
**  Modified 02-Aug-2001 for Win/Solaris by rbs
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "snipmath.h"
#include "sniptype.h"
#include "snip_str.h"
#include "filnames.h"
#include "errors.h"
#include "minmax.h"
#include "getopts.h"
#include "numcnvrt.h"
#include "dirport.h"

int         xargc = 0;
char       *xargv[MAX_XARGS]        = {NULL};
Boolean_T   getopts_range_err       = False_;

#if defined(__unix__)
 Boolean_T  xargs_on = False_;
#else
 Boolean_T  xargs_on = True_;
#endif

static FILE       *rspfile = NULL;
static int         count = 0, argidx = 0;
static Boolean_T   string_pending = False_;
static Boolean_T   additive_pending = False_;
static char       *pending_buf = NULL;

enum proc_stat { PSerror = -1, PSok, PSliteral};

static Boolean_T      bounds(struct Option_Tag *);
static enum proc_stat swProc(char *swStr);
static enum proc_stat VoProc(char *swStr);
static void           argProc(char *argStr);

/*
**  getopts()
**
**  Parameters: 1 - argc from main()
**              2 - argv from main()
**              3 - your program's options[] array
**
**  Returns: Number of options specified or Error_
**
**  Notes: 1. Your program should declare the global options[] array which
**            specifies all options recognized by getopts().
**
**         2. Out of range data are coerced into range and getopts_range_err
**            is set True_.
*/

int getopts(int argc, char *argv[])
{
      int i;
      Boolean_T scanning = True_;
      struct Option_Tag *ptr;
      char rspfname[FILENAME_MAX];
      char newarg[256];
      enum proc_stat ps;

      xargc = argc;
      xargv[argidx++] = argv[0];
      for (i = 1, count = 0; i < argc; )
      {
            /*
            ** Get the next argument - first see if we're looking for a string
            */

            if (string_pending)
            {
                  if (additive_pending)
                        strcat(pending_buf, argv[i]);
                  else  strcpy(pending_buf, argv[i]);
                  string_pending = additive_pending = False_;
                  pending_buf = NULL;
                  --xargc;
                  ++i;
                  continue;
            }

            /*
            ** Next try a response file
            */

            if (rspfile && !feof(rspfile))
            {
                  if (NULL == fgets(newarg, 256, rspfile))
                  {
                        fclose(rspfile);
                        rspfile = NULL;
                        ++i;
                        continue;
                  }
                  else  strdelch(newarg, "\r\n");
            }
            /*
            **  Next, see if we're supposed to open a response file
            */
            else  if (scanning && !rspfile && '@' == argv[i][0])
            {
                  /*
                  ** If necessary, open a response file
                  */
                  rspfile = cant(&argv[i][1], "r");
                  --xargc;
                  continue;
            }
            /*
            **  None of the above, copy the argument verbatim
            */
            else
            {
                  strcpy(newarg, argv[i++]);
            }

            /*
            ** Per Unix tradition, back-to-back switch characters signify
            ** the end of the switches
            */

            if ('-' ==  newarg[0] && '-' == newarg[1])
            {
                  printf("Found %s\n", newarg);
                  if (2 == strlen(newarg))
                  {
                        scanning = False_;
                        if (!rspfile)
                              --xargc;
                        continue;
                  }
                  else                    /* Process a verbose option   */
                  {
                        VoProc(&newarg[2]);
                        continue;
                  }
            }

            /*
            **  If we got here, we're still processing switches
            */

            if (scanning && (strchr("-", newarg[0])))
            {
                  ps = swProc(newarg);

                  if (PSerror == ps)
                        return Error_;

                  if (PSok == ps)
                        continue;
            }

            /*
            ** If we got here, newarg must be an argument or filename
            */

            else  argProc(newarg);
      }
      return count;
}

/*
**  Static function to process switch statements
**
**  Parameters: 1 - argv[i] containing the switch
**
**  Returns: PSok      if switch successful
**           PSerror   if invalid
**           PSliteral if literal (non-switch) argument
*/

static enum proc_stat swProc(char *swStr)
{
      struct Option_Tag *ptr;
      Boolean_T searching;
      char *arg_ptr;
      unsigned char     byte_var;
      int               int_var;
      short             short_var;
      unsigned short    word_var;
      long              long_var;
      unsigned long     dword_var, bitfield_var;
      double            double_var;

      /*
      ** Found a switch - If the 2nd character is also a switch
      ** character. If so, then it's a literal and is skipped
      */

      if (strchr("-@", swStr[1]))
            return PSliteral;

      for (ptr = options, searching = True_; searching; ++ptr)
      {
            if (!ptr->case_sense)
            {
                  ptr->letter = toupper(ptr->letter);
                  swStr[1]    = toupper(swStr[1]);
            }
            if ((int)swStr[1] == ptr->letter) switch (ptr->type)
            {
            case Boolean_Tag:
                  if ('-' == swStr[2])
                        *((Boolean_T *)(ptr->buf)) = False_;
                  else  *((Boolean_T *)(ptr->buf)) = True_;
                  searching = False_;
                  break;

            case Bitfield_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%lx", (unsigned long *)(bitfield_var));
                  *((unsigned long *)(ptr->buf)) |= (unsigned long)bitfield_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case Byte_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%lx", &long_var);
                  byte_var = (unsigned char)(long_var & 0xffL);
                  if (ptr->additive)
                        *((char *)(ptr->buf)) += byte_var;
                  else  *((char *)(ptr->buf))  = byte_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case Int_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  int_var = (int)(dround(getopts_eval(arg_ptr)));
                  if (ptr->additive)
                        *((int *)(ptr->buf)) += (int)(dround(getopts_eval(arg_ptr)));
                  else  *((int *)(ptr->buf))  = (int)(dround(getopts_eval(arg_ptr)));
                  bounds(ptr);
                  searching = False_;
                  break;

            case Short_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  short_var = (short)dround(getopts_eval(arg_ptr));
                  if (ptr->additive)
                        *((short *)(ptr->buf)) += short_var;
                  else  *((short *)(ptr->buf))  = short_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case Word_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%hx", (unsigned short *)(&word_var));
                  if (ptr->additive)
                        *((unsigned short *)(ptr->buf)) += word_var;
                  else  *((unsigned short *)(ptr->buf))  = word_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case Long_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  long_var = dround(getopts_eval(arg_ptr));
                  if (ptr->additive)
                        *((long *)(ptr->buf)) += long_var;
                  else  *((long *)(ptr->buf))  = long_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case DWord_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%lx", (unsigned long *)(dword_var));
                  if (ptr->additive)
                        *((unsigned long *)(ptr->buf)) += dword_var;
                  else  *((unsigned long *)(ptr->buf))  = dword_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case Float_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  double_var = (double)getopts_eval(arg_ptr);
                  if (ptr->additive)
                        *((double *)(ptr->buf)) += double_var;
                  else  *((double *)(ptr->buf))  = double_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case DFloat_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  double_var = (double)getopts_eval(arg_ptr);
                  if (ptr->additive)
                        *((double *)(ptr->buf)) += double_var;
                  else  *((double *)(ptr->buf))  = double_var;
                  bounds(ptr);
                  searching = False_;
                  break;

            case String_Tag:
                  if (!swStr[2] && ptr->Default && ptr->Default[0])
                  {
                        if (ptr->additive)
                              strcat(ptr->buf, (char *)(ptr->Default));
                        else  strcpy(ptr->buf, (char *)(ptr->Default));
                  }
                  else if (!swStr[2])
                  {
                        string_pending    = True_;
                        additive_pending  = ptr->additive;
                        pending_buf       = ptr->buf;
                  }
                  else
                  {
                        if (ptr->additive)
                              strcat(ptr->buf, &swStr[2]);
                        else  strcpy(ptr->buf, &swStr[2]);
                  }
                  bounds(ptr);
                  searching = False_;
                  break;

            default:
                  ErrExit("Unrecognized switch \"%s\"", swStr);
                  return PSerror;
            }
      }
      ++count;
      if (!rspfile)
            --xargc;

      return PSok;
}

/*
**  Static function to process verbose options
**
**  Parameters: 1 - argv[i] containing the verbose option
**
**  Returns: PSok      if switch successful
**           PSerror   if invalid
**           PSliteral if literal (non-switch) argument
*/

#define MAXOPTSTR 192

static enum proc_stat VoProc(char *swStr)
{
      struct Voption_tag *ptr;
      Boolean_T searching;
      char *arg_ptr;
      unsigned char     byte_var;
      int               int_var;
      short             short_var;
      unsigned short    word_var;
      long              long_var;
      unsigned long     dword_var, bitfield_var;
      double            double_var;
      char              string_var[MAXOPTSTR];

      /*
      ** Found a verbose option
      */

      printf("VoProc(%s): Entry\n", swStr);
      for (ptr = voptions, searching = True_; searching; ++ptr)
      {
            char *p, optbuf[MAXOPTSTR];

            p = stptok(swStr, optbuf, MAXOPTSTR, "=");
            if ((p == swStr) || (p == swStr + MAXOPTSTR))
                  ErrExit("Illegal option \"%s\"", swStr);

            printf("VoProc(): optbuf=%s, ptr->optstr=%s\n", optbuf, ptr->optstr);
            if (!ptr->case_sense)
            {
                  strupr(ptr->optstr);
                  strupr(optbuf);
            }

            printf("VoProc(): optbuf=%s, ptr->optstr=%s\n", optbuf, ptr->optstr);
            if (STREQ(optbuf, ptr->optstr)) switch (ptr->type)
            {
                  case Boolean_Tag:
                        strupr(p);
                        if (STREQ(p, "Y") || STREQ(p, "YES") || STREQ(p, "TRUE"))
                              *((Boolean_T *)(ptr->buf)) = True_;
                        else if(STREQ(p, "N") || STREQ(p, "NO") || STREQ(p, "FALSE"))
                                *((Boolean_T *)(ptr->buf)) = False_;
                        else  ErrExit("Illegal boolean value %s");
                        searching = False_;
                        break;

                  case Bitfield_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        sscanf(arg_ptr, "%lx", (unsigned long *)(bitfield_var));
                        *((unsigned long *)(ptr->buf)) |= (unsigned long)bitfield_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Byte_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        sscanf(arg_ptr, "%lx", &long_var);
                        byte_var = (unsigned char)(long_var & 0xffL);
                        if (ptr->additive)
                              *((char *)(ptr->buf)) += byte_var;
                        else  *((char *)(ptr->buf))  = byte_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Int_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        int_var = (int)(dround(getopts_eval(arg_ptr)));
                        if (ptr->additive)
                              *((int *)(ptr->buf)) += (int)(dround(getopts_eval(arg_ptr)));
                        else  *((int *)(ptr->buf))  = (int)(dround(getopts_eval(arg_ptr)));
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Short_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        short_var = (short)dround(getopts_eval(arg_ptr));
                        if (ptr->additive)
                              *((short *)(ptr->buf)) += short_var;
                        else  *((short *)(ptr->buf))  = short_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Word_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        sscanf(arg_ptr, "%hx", (unsigned short *)(&word_var));
                        if (ptr->additive)
                              *((unsigned short *)(ptr->buf)) += word_var;
                        else  *((unsigned short *)(ptr->buf))  = word_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Long_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        long_var = dround(getopts_eval(arg_ptr));
                        if (ptr->additive)
                              *((long *)(ptr->buf)) += long_var;
                        else  *((long *)(ptr->buf))  = long_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case DWord_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        sscanf(arg_ptr, "%lx", (unsigned long *)(dword_var));
                        if (ptr->additive)
                              *((unsigned long *)(ptr->buf)) += dword_var;
                        else  *((unsigned long *)(ptr->buf))  = dword_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case Float_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        double_var = (double)getopts_eval(arg_ptr);
                        if (ptr->additive)
                              *((double *)(ptr->buf)) += double_var;
                        else  *((double *)(ptr->buf))  = double_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case DFloat_Tag:
                        if (NUL == *p)
                        {
                              if (ptr->Default)
                                    arg_ptr = ptr->Default;
                              else  return PSerror;
                        }
                        else  arg_ptr = p;

                        double_var = (double)getopts_eval(arg_ptr);
                        if (ptr->additive)
                              *((double *)(ptr->buf)) += double_var;
                        else  *((double *)(ptr->buf))  = double_var;
                        bounds(ptr);
                        searching = False_;
                        break;

                  case String_Tag:
                        if (NUL == *p && ptr->Default && ptr->Default[0])
                        {
                              if (ptr->additive)
                                    strcat(ptr->buf, (char *)(ptr->Default));
                              else  strcpy(ptr->buf, (char *)(ptr->Default));
                        }
                        else if (NUL == *p)
                        {
                              string_pending    = True_;
                              additive_pending  = ptr->additive;
                              pending_buf       = ptr->buf;
                        }
                        else
                        {
                              if (ptr->additive)
                                    strcat(ptr->buf, p);
                              else  strcpy(ptr->buf, p);
                        }
                        bounds(ptr);
                        searching = False_;
                        break;

                        default:
                              ErrExit("Unrecognized switch \"%s\"", swStr);
                              return PSerror;
            }
      }
      ++count;
      if (!rspfile)
            --xargc;

      return PSok;
}

/*
**  Static function to process arguments
*/

static void argProc(char *argStr)
{
      DOSFileData ff;

      /* If no wildcards or ignoring wildcards, just copy it */

      if (!xargs_on || !has_wild(argStr))
      {
            xargv[argidx] = malloc(strlen(argStr) + 1);
            if (NULL == xargv[argidx])
                  ErrExit("Out of memory");
            strcpy(xargv[argidx], argStr);
            ++xargc;
            ++argidx;
            return;
      }
      else  /* Expand wildcards, if possible                      */
      {
            if (Success_ == FIND_FIRST(argStr, _A_ANY, &ff))
            {
                  char path[FILENAME_MAX];
                  char *p;

                  /* Save the path for re-attachment              */

                  fnSplit(argStr, NULL, path, NULL, NULL, NULL, NULL);

                  --xargc;    /* We add stuff in the loop, so back up   */
                  do
                  {                             
                        xargv[argidx] = malloc(strlen(ff_name(&ff))
                                               + strlen(path) + 2);
                        if (NULL == xargv[argidx])
                              ErrExit("Out of memory");
                        fnMerge(xargv[argidx], NULL, path, NULL, ff_name(&ff),
                                NULL, NULL);
                        ++argidx;
                        ++xargc;

                  } while (Success_ == FIND_NEXT(&ff));
                  FIND_END(&ff);
            }
      }
}

/*
**  Assure new data are within specified ranges, return non-zero if coerced
*/

static Boolean_T bounds(struct Option_Tag *option)
{
      Boolean_T coerced = False_;
      union {
            unsigned char      B;
            int                I;
            short              S;
            unsigned short     W;
            long               L;
            unsigned long      DW;
            float              F;
            double             D;
            char              *P;
      } tmp, val;
      size_t len;

      switch(option->type)
      {
      case Bitfield_Tag:
            tmp.DW = *((unsigned long *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%lx", &val.DW);
                  tmp.DW |= val.DW ;
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.DW);
                  tmp.DW &= val.DW;
            }
            if (*((unsigned long *)(option->buf)) != tmp.DW)
            {
                  getopts_range_err = True_;
                  *((unsigned long *)(option->buf)) = tmp.DW;
                  coerced = True_;
            }
            break;

      case Byte_Tag:
            tmp.B = *((unsigned char *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%lx", &val.L);
                  val.L &= 0xff;
                  tmp.B = min(tmp.B, (unsigned char)val.L);
            }
            if (option->min)
            {
                  sscanf(option->min, "%lx", &val.L);
                  val.L &= 0xff;
                  tmp.B = max(tmp.B, (unsigned char)val.L);
            }
            if (*((unsigned char *)(option->buf)) != tmp.B)
            {
                  getopts_range_err = True_;
                  *((unsigned char *)(option->buf)) = tmp.B;
                  coerced = True_;
            }
            break;

      case Int_Tag:
            tmp.I = *((int *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)INT_MAX)
                        val.I = INT_MAX;
                  else  val.I = (int)val.D;
                  tmp.I = min(tmp.I, val.I);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)INT_MIN)
                        val.I = INT_MIN;
                  else  val.I = (int)val.D;
                  tmp.I = max(tmp.I, val.I);
            }
            if (*((int *)(option->buf)) != tmp.I)
            {
                  getopts_range_err = True_;
                  *((int *)(option->buf)) = tmp.I;
                  coerced = True_;
            }
            break;

      case Short_Tag:
            tmp.S = *((short *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)SHRT_MAX)
                        val.S = SHRT_MAX;
                  else  val.S = (short)val.D;
                  tmp.S = min(tmp.I, val.I);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)SHRT_MIN)
                        val.S = SHRT_MIN;
                  else  val.S = (short)val.D;
                  tmp.S = max(tmp.I, val.I);
            }
            if (*((short *)(option->buf)) != tmp.S)
            {
                  getopts_range_err = True_;
                  *((short *)(option->buf)) = tmp.I;
                  coerced = True_;
            }
            break;

      case Word_Tag:
            tmp.W = *((unsigned short *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%hx", &val.W);
                  tmp.W = min(tmp.W, val.W);
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.W);
                  tmp.W = max(tmp.W, val.W);
            }
            if (*((unsigned short *)(option->buf)) != tmp.W)
            {
                  getopts_range_err = True_;
                  *((unsigned short *)(option->buf)) = tmp.W;
                  coerced = True_;
            }
            break;

      case Long_Tag:
            tmp.L = *((long *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)LONG_MAX)
                        val.L = LONG_MAX;
                  else  val.L = (long)val.D;
                  tmp.L = min(tmp.L, val.L);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)LONG_MIN)
                        val.L = LONG_MIN;
                  else  val.L = (int)val.D;
                  tmp.L = max(tmp.L, val.L);
            }
            if (*((long *)(option->buf)) != tmp.L)
            {
                  getopts_range_err = True_;
                  *((long *)(option->buf)) = tmp.L;
                  coerced = True_;
            }
            break;

      case DWord_Tag:
            tmp.DW = *((unsigned long *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%lx", &val.DW);
                  tmp.DW = min(tmp.DW, val.DW);
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.DW);
                  tmp.DW = max(tmp.DW, val.DW);
            }
            if (*((unsigned long *)(option->buf)) != tmp.DW)
            {
                  getopts_range_err = True_;
                  *((unsigned long *)(option->buf)) = tmp.DW;
                  coerced = True_;
            }
            break;

      case Float_Tag:
            tmp.F = *((float *)(option->buf));
            if (option->max)
            {
                  val.F = (float)getopts_eval(option->max);
                  tmp.F = min(tmp.F, val.F);
            }
            if (option->min)
            {
                  val.F = (float)getopts_eval(option->min);
                  tmp.F = max(tmp.F, val.F);
            }
            if (*((float *)(option->buf)) != tmp.F)
            {
                  getopts_range_err = True_;
                  *((float *)(option->buf)) = tmp.F;
                  coerced = True_;
            }
            break;

      case DFloat_Tag:
            tmp.D = *((double *)(option->buf));
            if (option->max)
            {
                  val.D = getopts_eval(option->max);
                  tmp.D = min(tmp.D, val.D);
            }
            if (option->min)
            {
                  val.D = getopts_eval(option->min);
                  tmp.D = max(tmp.D, val.D);
            }
            if (*((double *)(option->buf)) != tmp.D)
            {
                  getopts_range_err = True_;
                  *((double *)(option->buf)) = tmp.D;
                  coerced = True_;
            }
            break;
      }

      return coerced;
}

/*
**  Simplified evaluate() call - returns double or aborts
*/

double getopts_eval(char *str)
{
      return strtod(str, NULL);
}
