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
    geo.icycle = ++n_iters;
    Log ("\t- Beginning iteration %i\n", n_iters);

    Log_verbose ("\t\t- Updating cell opacities\n");
    update_cell_opacities ();
    Log_verbose ("\t\t- Calculating total vertical optical depth for cells\n");
    find_vertical_tau ();
    Log_verbose ("\t\t- Updating cell temperatures\n");
    update_cell_temperatures ();

    if (report_convergence () >= 0.9)
      converged = TRUE;

    write_grid_to_file ();
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
    T = (float) (grid[i].T * 1e-6);
    R = (float) (grid[i].rho / pow (T, 3.0));

    // Log ("cell %i T6/Log(R) = %e\n", grid[i].n, T / log (R));

    opacgn93_ (&Z, &X, &T, &R);

    grid[i].kappa = pow (10.0, e_.opact);
  }

  return SUCCESS;
}

double tot_tau;

int find_vertical_tau (void)
{
  int i;
  double dx;

  tot_tau = 0.0;

  for (i = 0; i < geo.nx_cells - 1; i++)
  {
    dx = grid[i + 1].x - grid[i].x;
    grid[i].trans_tau = dx * grid[i].rho * grid[i].kappa;
    tot_tau += grid[i].trans_tau;
  }

  Log ("\t\t- Total vertical optical depth %e\n", tot_tau);

  return SUCCESS;
}

int update_cell_temperatures (void)
{
  int i;
  double T_inter;

  double rtau = 0.0;

  for (i = geo.nx_cells; i > -1; i--)
  {
    grid[i].T_old = grid[i].T;
    rtau += grid[i].trans_tau;
    T_inter = eddington_approximation (grid[i].T, rtau);
    grid[i].T = pow (T_inter, 1.0 / 4.0);
  }

  Log_verbose ("\t\t- Total rtau %e\n", rtau);
  if (rtau > tot_tau)
    Log_error ("rtau > tot_tau!\n");

  return SUCCESS;
}

double eddington_approximation (double T_eff, double tau)
{
  return 0.75 * pow (T_eff, 4.0) * (tau + (2.0 / 3.0));
}