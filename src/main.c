/* ***************************************************************************
*
* @file main.c
*
* @author E. J. Parkinson
*
* @date 14 Nov 2018
*
* @brief Contains the main function.
*
* @details
*
 * ************************************************************************** */

#include <time.h>
#include <string.h>

#include "snake.h"

int
main (int argc, char **argv)
{
  struct timespec start_time;
  char par_file_path[LINE_LEN];
  int verbosity = FALSE;

  /*
   * Set flag that the logfile is needed to be initialised
   */

  INIT_LOGFILE = TRUE;

  start_time = get_time ();

  Log ("\n--------------------------------------------------------------\n\n");
  print_time_date ();
  Log ("\n--------------------------------------------------------------\n\n");

  /*
   * Begin the process of reading in the parameters from file:
   *  - If no arguments to the program are provided, the user will be prompted
   *    to input a file path to find the parameter file and then parameters
   *    will be read in from the file.
   *  - If 1 argument is provided, the program will attempt to load parameters
   *    from the provided file path from the argument list.
   *  - If more than 1 argument is provided, the program will exit.
   */

  if (argc == 1)
    find_par_file (par_file_path);
  else if (argc == 2)
    strcpy (par_file_path, argv[1]);
  else
    Exit (FILE_IN_ERR, "Too many arguments provided\n");
  init_parameter_file (par_file_path);

  get_optional_int ("verbosity", &verbosity);
  VERBOSITY = verbosity;
  if ((VERBOSITY != FALSE) && (VERBOSITY != TRUE))
    Exit (UNKNOWN_PARAMETER, "Invalid value for verbosity: verbosity should be 0 or 1\n");

  Log (" - Beginning initialisation routines\n");
  init_snake ();
  init_outfile ();
  init_geo ();
  Log (" - End of initialisation routines\n");

  eddington_iterations ();

  Log ("\n--------------------------------------------------------------\n\n");
  print_duration (start_time, " Simulation completed in");
  Log ("\n--------------------------------------------------------------\n\n");

  clean_up ();

  return SUCCESS;
}
