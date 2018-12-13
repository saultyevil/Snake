/* ***************************************************************************
 *
 * @file convergence.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions for checking the convergence of the Eddington algorithm.
 *
 * @details
 *
 * ************************************************************************** */

#include <math.h>

#include "../snake.h"

int check_cell_convergence (void)
{
  int i;

  int n_converged = 0;
  double eps = 0.05;  // TODO: remove hardcoded convergence limit

  for (i = 0; i  < geo.nz_cells; i++)
    if (fabs ((grid[i].T_old - grid[i].T) / (grid[i].T_old + grid[i].T)) < eps)
      n_converged += 1;

  return n_converged;
}

double report_convergence (void)
{
  int n_converged;
  double c_fraction;

  n_converged = check_cell_convergence ();
  c_fraction = (double) n_converged / geo.nz_cells;
  Verbose_log ("\t\t- %i cells out of %i converged (%1.3f)\n", n_converged,
               geo.nz_cells, c_fraction);

  return c_fraction;
}
