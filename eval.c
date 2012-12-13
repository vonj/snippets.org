/************************************************************************/
/*                                                                      */
/*  EVAL.C - A simple mathematical expression evaluator in C            */
/*                                                                      */
/*  operators supported: Operator               Precedence              */
/*                                                                      */
/*                         (                     Lowest                 */
/*                         )                     Highest                */
/*                         +   (addition)        Low                    */
/*                         -   (subtraction)     Low                    */
/*                         *   (multiplication)  Medium                 */
/*                         /   (division)        Medium                 */
/*                         \   (modulus)         High                   */
/*                         ^   (exponentiation)  High                   */
/*                         sin(                  Lowest                 */
/*                         cos(                  Lowest                 */
/*                         atan(                 Lowest                 */
/*                         abs(                  Lowest                 */
/*                         sqrt(                 Lowest                 */
/*                         ln(                   Lowest                 */
/*                         exp(                  Lowest                 */
/*                                                                      */
/*  constants supported: pi                                             */
/*                                                                      */
/*  Original Copyright 1991-93 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/*  Requires RMALLWS.C, also in SNIPPETS.                               */
/*                                                                      */
/************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "sniptype.h"
#include "snip_str.h"                     /* For rmallws(), strupr()    */
#include "snipmath.h"
#include "numcnvrt.h"

/*
**  Other SNIPPETS functions
*/

char *rmallws(char *);
char *strupr(char *);


struct operator {
      char        token;
      char       *tag;
      size_t      taglen;
      int         precedence;
};

static struct operator verbs[] = {
      {'+',  "+",       1, 2 },
      {'-',  "-",       1, 3 },
      {'*',  "*",       1, 4 },
      {'/',  "/",       1, 5 },
      {'\\', "\\",      1, 5 },
      {'^',  "^",       1, 6 },
      {'(',  "(",       1, 0 },
      {')',  ")",       1, 99},
      {'S',  "SIN(",    4, 0 },
      {'C',  "COS(",    4, 0 },
      {'A',  "ABS(",    4, 0 },
      {'L',  "LN(",     3, 0 },
      {'E',  "EXP(",    4, 0 },
      {'t',  "ATAN(",   5, 0 },
      {'s',  "SQRT(",   5, 0 },
      {NUL,  NULL,      0, 0 }
};

static char   op_stack[256];                    /* Operator stack       */
static double arg_stack[256];                   /* Argument stack       */
static char   token[256];                       /* Token buffer         */
static int    op_sptr,                          /* op_stack pointer     */
              arg_sptr,                         /* arg_stack pointer    */
              parens,                           /* Nesting level        */
              state;                            /* 0 = Awaiting expression
                                                   1 = Awaiting operator
                                                */
const double Pi = 3.14159265358979323846;

static int              do_op(void);
static int              do_paren(void);
static void             push_op(char);
static void             push_arg(double);
static int              pop_arg(double *);
static int              pop_op(int *);
static char            *get_exp(char *);
static struct operator *get_op(char *);
static int              getprec(char);
static int              getTOSprec(void);

/************************************************************************/
/*                                                                      */
/*  evaluate()                                                          */
/*                                                                      */
/*  Evaluates an ASCII mathematical expression.                         */
/*                                                                      */
/*  Arguments: 1 - String to evaluate                                   */
/*             2 - Storage to receive double result                     */
/*                                                                      */
/*  Returns: Success_ if successful                                     */
/*           Error_ if syntax error                                     */
/*           R_ERROR if runtime error                                   */
/*                                                                      */
/*  Side effects: Removes all whitespace from the string and converts   */
/*                it to U.C.                                            */
/*                                                                      */
/************************************************************************/

int evaluate(char *line, double *val)
{
      double arg;
      char *ptr = line, *str, *endptr;
      int ercode;
      struct operator *op;

      strupr(line);
      rmallws(line);
      state = op_sptr = arg_sptr = parens = 0;

      while (*ptr)
      {
            switch (state)
            {
            case 0:
                  if (NULL != (str = get_exp(ptr)))
                  {
                        if (NULL != (op = get_op(str)) &&
                              strlen(str) == op->taglen)
                        {
                              push_op(op->token);
                              ptr += op->taglen;
                              break;
                        }

                        if (Success_ == strcmp(str, "-"))
                        {
                              push_op(*str);
                              ++ptr;
                              break;
                        }

                        if (Success_ == strcmp(str, "PI"))
                              push_arg(Pi);

                        else
                        {
                              if (0.0 == (arg = strtod(str, &endptr)) &&
                                    NULL == strchr(str, '0'))
                              {
                                    return Error_;
                              }
                              push_arg(arg);
                        }
                        ptr += strlen(str);
                  }
                  else  return Error_;

                  state = 1;
                  break;

            case 1:
                  if (NULL != (op = get_op(ptr)))
                  {
                        if (')' == *ptr)
                        {
                              if (Success_ > (ercode = do_paren()))
                                    return ercode;
                        }
                        else
                        {
                              while (op_sptr &&
                                    op->precedence <= getTOSprec())
                              {
                                    do_op();
                              }
                              push_op(op->token);
                              state = 0;
                        }

                        ptr += op->taglen;
                  }
                  else  return Error_;

                  break;
            }
      }

      while (1 < arg_sptr)
      {
            if (Success_ > (ercode = do_op()))
                  return ercode;
      }
      if (!op_sptr)
            return pop_arg(val);
      else  return Error_;
}

/*
**  Evaluate stacked arguments and operands
*/

static int do_op(void)
{
      double arg1, arg2;
      int op;

      if (Error_ == pop_op(&op))
            return Error_;

      pop_arg(&arg1);
      pop_arg(&arg2);

      switch (op)
      {
      case '+':
            push_arg(arg2 + arg1);
            break;

      case '-':
            push_arg(arg2 - arg1);
            break;

      case '*':
            push_arg(arg2 * arg1);
            break;

      case '/':
            if (0.0 == arg1)
                  return R_ERROR;
            push_arg(arg2 / arg1);
            break;

      case '\\':
            if (0.0 == arg1)
                  return R_ERROR;
            push_arg(fmod(arg2, arg1));
            break;

      case '^':
            push_arg(pow(arg2, arg1));
            break;

      case 't':
            ++arg_sptr;
            push_arg(atan(arg1));
            break;

      case 'S':
            ++arg_sptr;
            push_arg(sin(arg1));
            break;

      case 's':
            if (0.0 > arg2)
                  return R_ERROR;
            ++arg_sptr;
            push_arg(sqrt(arg1));
            break;

      case 'C':
            ++arg_sptr;
            push_arg(cos(arg1));
            break;

      case 'A':
            ++arg_sptr;
            push_arg(fabs(arg1));
            break;

      case 'L':
            if (0.0 < arg1)
            {
                  ++arg_sptr;
                  push_arg(log(arg1));
                  break;
            }
            else  return R_ERROR;

      case 'E':
            ++arg_sptr;
            push_arg(exp(arg1));
            break;

      case '(':
            arg_sptr += 2;
            break;

      default:
            return Error_;
      }
      if (1 > arg_sptr)
            return Error_;
      else  return op;
}

/*
**  Evaluate one level
*/

static int do_paren(void)
{
      int op;

      if (1 > parens--)
            return Error_;
      do
      {
            if (Success_ > (op = do_op()))
                  break;
      } while (getprec((char)op));
      return op;
}

/*
**  Stack operations
*/

static void push_op(char op)
{
      if (!getprec(op))
            ++parens;
      op_stack[op_sptr++] = op;
}

static void push_arg(double arg)
{
      arg_stack[arg_sptr++] = arg;
}

static int pop_arg(double *arg)
{
      *arg = arg_stack[--arg_sptr];
      if (0 > arg_sptr)
            return Error_;
      else  return Success_;
}

static int pop_op(int *op)
{
      if (!op_sptr)
            return Error_;
      *op = op_stack[--op_sptr];
      return Success_;
}

/*
**  Get an expression
*/

static char * get_exp(char *str)
{
      char *ptr = str, *tptr = token;
      struct operator *op;

      if (Success_ == strncmp(str, "PI", 2))
            return strcpy(token, "PI");


      while (*ptr)
      {
            if (NULL != (op = get_op(ptr)))
            {
                  if ('-' == *ptr)
                  {
                        if (str != ptr && 'E' != ptr[-1])
                              break;
                        if (str == ptr && !isdigit(ptr[1]) && '.' != ptr[1])
                        {
                              push_arg(0.0);
                              strcpy(token, op->tag);
                              return token;
                        }
                  }

                  else if (str == ptr)
                  {
                        strcpy(token, op->tag);
                        return token;
                  }

                  else break;
            }

            *tptr++ = *ptr++;
      }
      *tptr = NUL;

      return token;
}

/*
**  Get an operator
*/

static struct operator * get_op(char *str)
{
      struct operator *op;

      for (op = verbs; op->token; ++op)
      {
            if (Success_ == strncmp(str, op->tag, op->taglen))
                  return op;
      }
      return NULL;
}

/*
**  Get precedence of a token
*/

static int getprec(char token)
{
      struct operator *op;

      for (op = verbs; op->token; ++op)
      {
            if (token == op->token)
                  break;
      }
      if (op->token)
            return op->precedence;
      else  return 0;
}

/*
**  Get precedence of TOS token
*/

static int getTOSprec(void)
{
      if (!op_sptr)
            return 0;
      return getprec(op_stack[op_sptr - 1]);
}

#ifdef TEST

#include <stdio.h>

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      int retval;
      double val;

      printf("evaluate(%s) ", argv[1]);
      printf("returned %d\n", retval = evaluate(argv[1], &val));
      if (0 == retval)
            printf("val = %f\n", val);
      return 0;
}

#endif /* TEST */
