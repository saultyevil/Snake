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

#include "../snake.h"
#include "../interp/2d_interp.h"

double FLOAT_EPS = 1e-6;

// Index an element for a 2D array which has been flattened into 1D
int i2d (int row, int col)
{
  return row * N_COLS + col;
}

// Compare if two floats are similar values
int float_compare (double a, double b)
{
  if (fabs (a - b) / (a + b) < FLOAT_EPS)
    return SUCCESS;
  else
    return FAILURE;
}

// Free up memory allocation and close files -- should be used at the end
int clean_up (void)
{
  Log_verbose ("\n - Cleaning up memory and files before exit\n");
  free (grid);
  close_outfile ();
  close_parameter_file ();

  #ifndef OPAL
    clean_up_opac_tables ();
    clean_up_gsl ();
  #endif

  return SUCCESS;
}

// Exit from the program with an error code and description
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

// Print to screen -- previously used when MPI existed and would only print to
// the master process
int Log (char *fmt, ...)
{
  va_list arg_list;

  va_start (arg_list, fmt);
  vprintf (fmt, arg_list);
  va_end (arg_list);
  
  return SUCCESS;
}

// Print to screen if verbose output is enabled
int Log_verbose (char *fmt, ...)
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

// Print a message to screen prefixed by Error:
int Log_error (char *fmt, ...)
{
  va_list arg_list;
  
  va_start (arg_list, fmt);
  printf (" Error: ");
  vprintf (fmt, arg_list);
  va_end (arg_list);
  
  return SUCCESS;
}
