/* ***************************************************************************
 *
 * @file grid_set.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions for initialising the 1D grid.
 *
 * @details
 *
 * ************************************************************************** */

#include <math.h>
#include <stdlib.h>

#include "snake.h"

int init_grid (void)
{
  int i;

  get_grid_params ();
  allocate_1d_grid ();

  Verbose_log ("\t\t- Initialising grid cells\n");

  for (i = 0; i < geo.nz_cells; i++)
  {
    grid[i].n = i;
    grid[i].z = i * geo.hz;
    grid[i].T = grid[i].T_old = geo.t_star;
    grid[i].rho = density_profile_disk_height (grid[i].z);
  }

  /*
   * Update the opacity and optical depth for each cell using the Rosseland
   * opacity from the Opal tables
   */

  update_cell_opacities ();
  find_vertical_tau ();

  Verbose_log ("\t\t- Writing initial grid to file\n");
  write_grid ();

  return SUCCESS;
}

// TODO: improve density profile
double density_profile_disk_height (double z)
{
  return geo.irho * exp (-1.0 * pow(z, 2.0) / (2.0 * pow (geo.z_max, 2.0)));
}

int get_grid_params (void)
{
  get_int ("nz_cells", &geo.nz_cells);
  if (geo.nz_cells <= 0)
    Exit (2, "Invalid value for nx_cells: nx_cells > 0\n");
  get_double ("z_max", &geo.z_max);
  if (geo.z_max < 0)
    Exit (2, "Invalid value for z_max: z_max >= 0\n");
  get_double ("t_star", &geo.t_star);
  if (geo.t_star < 0)
    Exit (2, "Invalid value for t_star: t_star >= 0\n");
  get_double ("irho", &geo.irho);
  if (geo.irho < 0)
    Exit (2, "Invalid value for irho: irho >= 0\n");

  geo.hz = geo.z_max / geo.nz_cells;

  return SUCCESS;
}

int allocate_1d_grid (void)
{
  long mem_req;

  mem_req = geo.nz_cells * sizeof (*grid);

  if (!(grid = calloc (geo.nz_cells, sizeof (*grid))))
    Exit (3, "Could not allocate memory for grid of size %li\n", mem_req);
  Verbose_log ("\t\t- Allocated %1.2e bytes for %1.2e grid cells\n",
               (double) mem_req, (double) geo.nz_cells);

  return SUCCESS;
}
