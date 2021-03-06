/* ***************************************************************************
 *
 * @file time.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions for time keeping.
 *
 * @details
 *
 * ************************************************************************** */

#include <time.h>

#include "snake.h"

// Print the current time and date to screen
void
print_time_date (void)
{
  char *c_time_string;
  time_t current_time;
  
  current_time = time (NULL);
  c_time_string = ctime (&current_time);
  
  Log (" Current time: %s", c_time_string);
}

// Create a timespec structure for the current time
struct timespec
get_time (void)
{
  struct timespec time;

  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &time);

  return time;
}

// Print the time difference between two timespec structs
void
print_duration (struct timespec start_time, char *message)
{
  double td;
  struct timespec end_time;

  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end_time);
  td = (end_time.tv_sec - start_time.tv_sec) +
                                (end_time.tv_nsec - start_time.tv_nsec) * 1e-9;
  Log ("%s %f seconds\n", message, td);
}
