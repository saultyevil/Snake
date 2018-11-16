/* ***************************************************************************
 *
 * @file utility.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief General utility functions, i.e. logging and error functions.
 *
 * @details
 *
 * ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "snake.h"

double FLOAT_EPS = 1e-6;

int float_compare (double a, double b)
{
  if (fabs (a - b) / (a + b) < FLOAT_EPS)
    return SUCCESS;
  else
    return FAILURE;
}

int check_opacity_table (void)
{
  Log ("\t- Checking for Opal Opacity Table GN93hz\n\n");
  if (access ("GN93hz", F_OK) == -1)
    Exit (15, "GN93hz not found in current directory.\n");

  return SUCCESS;
}

int clean_up (void)
{
  Verbose_log ("\n - Cleaning up memory and files before exit\n");
  free (grid);
  close_outfile ();
  close_parameter_file ();

  return SUCCESS;
}

void Exit (int error_code, char *fmt, ...)
{
  va_list arg_list;

  va_start (arg_list, fmt);

  printf ("\n--------------------------------------------------------------\n");
  printf ("\n\tALART: ");
  vprintf (fmt, arg_list);
  printf ("\t       Exiting with error code %i\n", error_code);
  printf ("\n--------------------------------------------------------------\n\n");

  va_end (arg_list);

  exit (error_code);
}

int Log (char *fmt, ...)
{
  va_list arg_list;

  va_start (arg_list, fmt);
  vprintf (fmt, arg_list);
  va_end (arg_list);
  
  return SUCCESS;
}

int Verbose_log (char *fmt, ...)
{
  if (VERBOSITY == TRUE)
  {
    va_list arg_list;
    
    va_start (arg_list, fmt);
    vprintf (fmt, arg_list);
    va_end (arg_list);
  }

  return SUCCESS;
}

int Log_error (char *fmt, ...)
{
  va_list arg_list;
  
  va_start (arg_list, fmt);
  printf (" Error: ");
  vprintf (fmt, arg_list);
  va_end (arg_list);
  
  return SUCCESS;
}
