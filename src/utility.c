/* ***************************************************************************
 *
 * @file
 *
 * @author
 *
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "snake.h"

int check_opacity_table ()
{
  Log (" - Checking for Opal Opacity Table: 'GN93hz'\n");
  if (access ("GN93hz", F_OK) == -1)
    Exit (15, "GN93hz not found in current directory.\n");
  Log ("\t- GN93hz found!\n");

  return SUCCESS;
}

void Exit (int error_code, char *fmt, ...)
{
  va_list arg_list;

  va_start (arg_list, fmt);

  printf ("\n--------------------------------------------------------------\n\n");
  printf ("\tFAILURE: ");
  vprintf (fmt, arg_list);
  printf ("\t         Exiting with error code %i\n", error_code);
  printf ("\n--------------------------------------------------------------\n");

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

int Log_verbose (char *fmt, ...)
{
  if (VERBOSITY == TRUE)
  {
    va_list arg_list;
    
    va_start (arg_list, fmt);
    vprintf (fmt, arg_list);
    va_end (arg_list);
  
    return SUCCESS;
  }
  else
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
