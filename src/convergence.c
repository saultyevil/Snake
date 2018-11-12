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

#include <math.h>

#include "snake.h"

int check_cell_convergence (void)
{
  int i;

  int n_converged = 0;
  double eps = 0.05;

  for (i = 0; i  < geo.nx_cells; i++)
    if (fabs ((grid[i].T_old - grid[i].T) / (grid[i].T_old + grid[i].T)) < eps)
      n_converged += 1;

  return n_converged;
}

double report_convergence (void)
{
  int n_converged;
  double c_fraction;

  n_converged = check_cell_convergence ();
  c_fraction = (double) n_converged / geo.nx_cells;
  Log_verbose ("\t\t- %i cells out of %i converged (%1.3f)\n", n_converged,
               geo.nx_cells, c_fraction);

  return c_fraction;
}
