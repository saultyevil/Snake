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
#include <stdlib.h>

#include "snake.h"

int init_grid (void)
{
  int i;
  int write_grid = FALSE;

  get_grid_params ();
  allocate_1d_grid ();

  Log_verbose ("\t\t- Initialising grid cells\n");

  for (i = 0; i < geo.nx_cells; i++)
  {
    grid[i].n = i;
    grid[i].x = i * geo.hx;
    grid[i].T = grid[i].T_old = geo.t_star;
    // grid[i].rho = geo.irho;
    grid[i].rho = geo.irho * exp (- pow(grid[i].x, 2.0) /
                                                 (2.0 * pow (geo.x_max, 2.0)));
    // Log ("grid[%i].rho = %e\n", grid[i].n, grid[i].rho);
  }

  get_optional_int ("init_grid_out", &write_grid);
  if (write_grid != FALSE && write_grid != TRUE)
    Log_error ("Invalid value for write_grid: write_grid is either 0 or 1\n");
  if (write_grid)
  {
    Log_verbose ("\t\t- Writing initial grid to file\n");
    write_grid_to_file ();
  }

  return SUCCESS;
}

int get_grid_params (void)
{
  get_int ("nx_cells", &geo.nx_cells);
  if (geo.nx_cells <= 0)
    Exit (2, "Invalid value for nx_cells: nx_cells > 0\n");
  get_double ("x_max", &geo.x_max);
  if (geo.x_max < 0)
    Exit (2, "Invalid value for x_max: x_max >= 0\n");
  get_double ("t_star", &geo.t_star);
  if (geo.t_star < 0)
    Exit (2, "Invalid value for t_star: t_star >= 0\n");
  get_double ("irho", &geo.irho);
  if (geo.irho < 0)
    Exit (2, "Invalid value for irho: irho >= 0\n");

  geo.hx = geo.x_max / geo.nx_cells;

  return SUCCESS;
}

int allocate_1d_grid (void)
{
  long mem_req;

  mem_req = geo.nx_cells * sizeof (*grid);

  if (!(grid = calloc (geo.nx_cells, sizeof (*grid))))
    Exit (3, "Could not allocate memory for grid of size %li\n", mem_req);
  Log_verbose ("\t\t- Allocated %1.2e bytes for %1.2e grid cells\n",
               (double) mem_req, (double) geo.nx_cells);

  return SUCCESS;
}
