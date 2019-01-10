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

FILE *LOGFILE;
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

// Initialise the log file
int init_logfile (void)
{
  char *logname = "logfile";

  if (!(LOGFILE = fopen (logname, "w")))
    Exit (FILE_IN_ERR, "Can't open file %s to write log\n", logname);
  INIT_LOGFILE = FALSE;
  Log_verbose ("\t\t- Open %s with write access\n", logname);

  return SUCCESS;
}

// Close the log file
int close_logfile (void)
{

  fprintf (LOGFILE, "\n");
  if (fclose (LOGFILE))
    Exit (FILE_CLOSE_ERR, "Cannot close logfile\n");
  printf ("\n");

  return SUCCESS;
}

// Free up memory allocation and close files -- should be used at the end
int clean_up (void)
{
  Log_verbose (" - Cleaning up memory and files before exit\n");
  free (grid);
  close_outfile ();
  close_parameter_file ();

  if (modes.low_temp)
  {
    clean_up_opac_tables ();
    clean_up_gsl ();
  }

  close_logfile ();

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

// Print to screen and log file
int Log (char *fmt, ...)
{
  va_list arg_list, arg_list_log;

  if (INIT_LOGFILE == TRUE)
    init_logfile ();

  va_start (arg_list, fmt);
  va_copy (arg_list_log, arg_list);
  vprintf (fmt, arg_list);
  vfprintf (LOGFILE, fmt, arg_list_log);
  va_end (arg_list);
  
  return SUCCESS;
}

// Print to screen and the log file if verbose output is enabled
int Log_verbose (char *fmt, ...)
{
  if (VERBOSITY == TRUE)
  {
    va_list arg_list, arg_list_log;
    
    va_start (arg_list, fmt);
    va_copy (arg_list_log, arg_list);
    vprintf (fmt, arg_list);
    vfprintf (LOGFILE, fmt, arg_list_log);
    va_end (arg_list);
  }

  return SUCCESS;
}

// Print a message to screen and the log file prefixed by Error:
int Log_error (char *fmt, ...)
{
  va_list arg_list, arg_list_log;
  
  va_start (arg_list, fmt);
  va_copy (arg_list_log, arg_list);
  printf (" Error: ");
  vprintf (fmt, arg_list);
  fprintf (LOGFILE, " Error: ");
  vfprintf (LOGFILE, fmt, arg_list);
  va_end (arg_list);
  
  return SUCCESS;
}
