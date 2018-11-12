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
  struct timespec edd_start;

  int n_iters = 0;
  int converged = FALSE;

  Log (" - Beginning Eddington iterations\n");

  edd_start = get_time ();

  while (!converged && n_iters < MAX_ITER)
  {
    n_iters++;
    get_rosseland_opac ();

    if (report_convergence () >= 0.9)
      converged = TRUE;

    if (!(n_iters % PROGRESS_OUT_FREQ))
      Log ("\t\t* %4i iterations complete\n", n_iters);
  }

  if (n_iters == MAX_ITER)
    Log (" - Max iterations reached!\n");

  Log (" - %i iterations ", n_iters);
  print_duration (edd_start, "completed in");

  return SUCCESS;
}

// Pretend this is clean and nice to look at
int get_rosseland_opac (void)
{
  int i;
  float z, xh, t6, r;
  z = 0.02;
  xh = 0.74;

  for (i = 0; i < geo.nx_cells; i++)
  {
    t6 = grid[i].T/1e6;
    r = grid[i].rho;
    opacgn93_ (&z, &xh, &t6, &r);
    grid[i].kappa = e_.opact;
  }

  write_grid_to_file ();

  return SUCCESS;
}