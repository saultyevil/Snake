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
#include <math.h>
#include <stdio.h>

#include "snake.h"
#include "flib.h"

int eddington_iterations (void)
{
  struct timespec edd_start;

  int n_iters = 0;
  int converged = FALSE;

  Log (" - Beginning Eddington iterations\n");

  edd_start = get_time ();

  while (!converged && n_iters < MAX_ITER)
  {
    update_cell_opacities ();
    find_vertical_tau ();
    update_cell_temperatures ();

    if (report_convergence () >= 0.9)
      converged = TRUE;

    if (!(n_iters % PROGRESS_OUT_FREQ))
      Log ("\t\t- %3i iterations complete\n", n_iters + 1);

    n_iters++;
  }

  if (n_iters == MAX_ITER)
    Log (" - Max iterations reached!\n");

  Log (" - %i iterations ", n_iters);
  print_duration (edd_start, "completed in");

  return SUCCESS;
}

int update_cell_opacities (void)
{
  int i;
  float Z, X, T, R;

  X = (float) geo.X;
  Z = (float) geo.Z;

  for (i = 0; i < geo.nx_cells; i++)
  {
    T = (float) (grid[i].T / 1e6);
    R = (float) (grid[i].rho / pow (T, 3.0));

    opacgn93_ (&Z, &X, &T, &R);

    grid[i].kappa = pow (10.0, (double) e_.opact);
  }

  return SUCCESS;
}

int find_vertical_tau (void)
{
  return SUCCESS;
}

int update_cell_temperatures (void)
{
  return SUCCESS;
}
