/* ***************************************************************************
 *
 * @file eddington.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions for controlling the Eddington iteration algorithm and the
 *        functions required to update cell opacities, optical depth and
 *        temperatures.
 *
 * @details
 *
 * ************************************************************************** */

#include <time.h>
#include <math.h>

#include "snake.h"
#include "flib.h"

int eddington_iterations (void)
{
  int n_iters = 0;
  int converged = FALSE;
  double converge_fraction = 0.9;
  struct timespec edd_start;

  get_double ("converge_fraction", &converge_fraction);
  if (converge_fraction <= 0)
    Exit (2, "Invalid value for converge_fraction: converge_fraction > 0");

  check_opacity_table ();

  edd_start = get_time ();

  while (!converged && n_iters < MAX_ITER)
  {
    Log ("\t- Beginning iteration %i\n", geo.icycle = ++n_iters);

    eddington_update ();

    if (report_convergence () >= converge_fraction)
      converged = TRUE;

    write_grid ();
  }

  if (n_iters == MAX_ITER)
    Log (" - Ruh roh, max number of iterations reached!\n");

  Log ("\n - Cells converged in %i iterations in", n_iters);
  print_duration (edd_start, "");

  return SUCCESS;
}

double eddington_approximation (double T_eff, double tau)
{
  return 0.75 * pow (T_eff, 4.0) * (tau + (2.0 / 3.0));
}

int eddington_update (void)
{
  update_cell_opacities ();
  find_vertical_tau ();
  update_cell_temperatures ();

  return SUCCESS;
}

int update_cell_opacities (void)
{
  int i;
  float Z, X, T6, R;
  float logT, logR;

  Verbose_log ("\t\t- Updating cell opacities\n");

  X = (float) geo.X;
  Z = (float) geo.Z;

  for (i = 0; i < geo.nz_cells; i++)
  {
    T6 = (float) (grid[i].T * 1e-6);
    R = (float) (grid[i].rho / pow (T6, 3.0));

    /*
     * Check that the T6 and R values are within the Opal table range
     */

    logR = (float) log10 (R);
    logT = (float) log10 (grid[i].T);
    if ((logR < MIN_LOG_R) || (logR > MAX_LOG_R))
    {
      Log_error ("\t- logR out of bounds: %f\n", logR);
      Log_error ("\t- %f < logR < %f\n", MIN_LOG_R, MAX_LOG_R);
      Exit (72, "logR out of Opal table range for cell %i\n", grid[i].n);
    }
    if ((logT < MIN_LOG_T) || (logT > MAX_LOG_T))
    {
      Log_error ("\t- logT out of bounds: %f\n", logT);
      Log_error ("\t- %f < logT < %f\n", MIN_LOG_T, MAX_LOG_T);
      Exit (72, "logT out of Opal table range for cell %i\n", grid[i].n);
    }

    /*
     * Call the Opal Opacity interpolation function -- see opal.f and flib.h
     * for more detailed description of how this works
     */

    opacgn93_ (&Z, &X, &T6, &R);

    grid[i].kappa = pow (10.0, e_.opact);
  }

  return SUCCESS;
}

int find_vertical_tau (void)
{
  int i;
  double dx;

  Verbose_log ("\t\t- Calculating total vertical optical depth for cells\n");

  geo.tot_tau = 0.0;

  for (i = geo.nz_cells - 1; i > -1; i--)
  {
    dx = geo.hz;
    grid[i].cell_tau = dx * grid[i].rho * grid[i].kappa;
    grid[i].tau_depth = geo.tot_tau += grid[i].cell_tau;
  }

  Log ("\t\t- Total vertical optical depth %e\n", geo.tot_tau);

  return SUCCESS;
}

int update_cell_temperatures (void)
{
  int i;
  double T_inter, Teff;
  double rtau = 0.0;

  Verbose_log ("\t\t- Updating cell temperatures\n");

  Teff = update_Teff ();
  Log ("\t\t- Effective temperature %e\n", Teff);

  for (i = geo.nz_cells - 1; i > -1; i--)
  {
    grid[i].T_old = grid[i].T;
    rtau += grid[i].cell_tau;
    T_inter = eddington_approximation (Teff, grid[i].tau_depth);
    grid[i].T = pow (T_inter, 0.25);
  }

  Verbose_log ("\t\t- Total rtau %e\n", rtau);
  if (float_compare (rtau, geo.tot_tau))
  {
    if (rtau > geo.tot_tau)
      Log_error ("\t- rtau > tot_tau\n");
    else
      Log_error ("\t- rtau < tot_tau\n");
    Log_error ("\t- rtau = %e tot_tau = %e\n", rtau, geo.tot_tau);
  }

  return SUCCESS;
}

double update_Teff (void)
{
  double Teff4;

  // T_{eff}^4 = \frac{4T_{disk}^{4}}{3\tau_{tot} + 2}
  Teff4 = (4 * pow (geo.T_disk, 4.0)) / (3 * geo.tot_tau + 2);

  return pow (Teff4, 0.25);
}
