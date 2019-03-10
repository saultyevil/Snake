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
#include <stdlib.h>

#include "snake.h"

// The Eddington approximation for T^4
double
eddington_approximation (double T_eff, double tau)
{
  return 0.75 * pow (T_eff, 4.0) * (tau + (2.0 / 3.0));
}

// A slight rearrangement of the Eddington approximation to act as our boundary
// condition to calculate T_eff at the bottom of the Eddington geometry
// T_{eff}^4 = \frac{4T_{disk}^{4}}{3\tau_{tot} + 2}
double
update_Teff (void)
{
  return pow ((4.0 * pow (geo.T_disk, 4.0)) / (3 * (geo.tot_tau + 2.0 / 3.0)),
              0.25);
}

// Find the total amount of optical depth from bottom to top of the Eddington
// geometry
void
find_vertical_tau (void)
{
  int i;
  double dz;

  Log_verbose ("\t\t- Calculating total vertical optical depth for cells\n");

  geo.tot_tau = 0.0;

  for (i = geo.nz_cells - 1; i > -1; i--)
  {
    if (i == 0)
      dz = grid[i].z;
    else
      dz = grid[i].z - grid[i - 1].z;

    grid[i].cell_tau = dz * grid[i].rho * grid[i].kappa;
    grid[i].tau_depth = geo.tot_tau += grid[i].cell_tau;
  }

  Log ("\t\t- Total vertical optical depth %e\n", geo.tot_tau);
}

// Update the temperature of the cell using the Eddington approximation
void
update_cell_temperatures (void)
{
  int i;
  double T_inter, Teff;
  double rtau = 0.0;

  Log_verbose ("\t\t- Updating cell temperatures\n");

  Teff = update_Teff ();
  Log ("\t\t- Effective temperature %e K\n", Teff);

  for (i = geo.nz_cells - 1; i > -1; i--)
  {
    grid[i].T_old = grid[i].T;
    rtau += grid[i].cell_tau;
    T_inter = eddington_approximation (Teff, grid[i].tau_depth);
    grid[i].T = pow (T_inter, 0.25);
  }

  #ifdef DEBUG
    Log ("\t\t- Total rtau %e\n", rtau);
  #endif

  if (float_compare (rtau, geo.tot_tau))
  {
    if (rtau > geo.tot_tau)
      Log_error ("\t- rtau > tot_tau\n");
    else
      Log_error ("\t- rtau < tot_tau\n");
    Log_error ("\t- rtau = %e tot_tau = %e\n", rtau, geo.tot_tau);
  }
}

// Calculate the hydrogen column density
void
calculate_column_density (void)
{
  int i;
  double *nh, *ne;
  double dz, nh_col, ne_col;
  double m_proton = 1.6726219e-24;  // grams
  double m_electon = 9.10938e-28;   // grams

  nh = calloc ((size_t) geo.nz_cells, sizeof (*nh));
  ne = calloc ((size_t) geo.nz_cells, sizeof (*ne));

  for (i = 0; i < geo.nz_cells; i++)
  {
    nh[i] = grid[i].rho / m_proton;
    ne[i] = grid[i].rho / m_electon;
  }

  #ifdef DEBUG
    for (i = 0; i < geo.nz_cells; i++)
      Log ("Grid[%i].rho = %e\n          nh = %e\n          ne = %e\n", i,
           grid[i].rho, nh[i], ne[i]);
  #endif

  nh_col = ne_col = 0;
  for (i = 0; i < geo.nz_cells; i++)
  {
    if (i == 0)
      dz = grid[i].z;
    else
      dz = grid[i].z - grid[i - 1].z;

    nh_col += dz * nh[i];
    ne_col += dz * ne[i];
  }

  Log ("\t\t- H column density %e cm^-2\n", nh_col);
  Log ("\t\t- e- column density %e cm^-2\n", ne_col);

  free (nh);
  free (ne);
}

// Main controlling function for the Eddington algorithm
void
eddington_iterations (void)
{
  int n_iters = 0;
  int converged = FALSE;
  double converge_fraction = 0.9;
  struct timespec edd_start;

  Log ("\n - Beginning Eddington iterations\n");

  get_double ("converge_fraction", &converge_fraction);
  if (converge_fraction <= 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for converge_fraction: converge_fraction > 0");

  edd_start = get_time ();

  while (!converged && n_iters < MAX_ITER)
  {
    Log ("\t- Beginning iteration %i\n", geo.icycle = ++n_iters);

    update_cell_opacities ();
    find_vertical_tau ();
    update_cell_temperatures ();
    calculate_column_density ();

    if (report_convergence () >= converge_fraction)
      converged = TRUE;

    write_grid ();
  }

  if (n_iters == MAX_ITER)
    Log (" - Ruh roh, max number of iterations reached!\n");

  Log ("\n - Cells converged in %i iterations in", n_iters);
  print_duration (edd_start, "");
}
