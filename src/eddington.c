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

#include <time.h>
#include <stdio.h>

#include "snake.h"

int eddington_iterations (void)
{
  int n_iters = 0;
  int converged = 0;
  struct timespec edd_start;

  Log (" - Beginning Eddington iterations\n");

  edd_start = get_time ();

  /*
   * Put the iterations here -- will need some way to check the convergence?
   */

  while (!converged)
  {
    n_iters++;
    if (n_iters == 10)
      converged = 1;
    if (!(n_iters % PROGRESS_OUT_FREQ))
      Log ("\t\t* %4i iterations complete\n", n_iters);
  }

  Log (" - %i iterations ", n_iters);
  print_duration (edd_start, "completed in");

  return EXIT;
}
