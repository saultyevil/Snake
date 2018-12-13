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

#include "../snake.h"

int init_grid (void)
{
  int read_from_file = FALSE;

  Verbose_log ("\t\t- Initialising grid cells\n");

  get_initial_grid_params ();

  get_optional_int ("rho_from_file", &read_from_file);
  if (read_from_file)
  {
    Log ("\t\t- Initialising density profile from file\n");
    get_string ("density_file", geo.density_filepath);
    density_from_file (geo.density_filepath);
  }
  else
  {
    Log ("\t\t- Initialising standard density profile\n");
    standard_density_profile ();
  }

  /*
   * Update the opacity and optical depth for each cell. We also initialise the
   * opacity table which is being used here
   */

  init_opacity_table ();
  update_cell_opacities ();
  find_vertical_tau ();

  Verbose_log ("\t\t- Writing initial grid to file\n");
  write_grid ();

  return SUCCESS;
}

int get_initial_grid_params (void)
{
  get_double ("z_max", &geo.z_max);
  if (geo.z_max < 0)
    Exit (BAD_INPUT, "Invalid value for z_max: z_max >= 0\n");
  get_double ("T_init", &geo.T_init);
  if (geo.T_init < 0)
    Exit (BAD_INPUT, "Invalid value for T_init: T_init >= 0\n");
  get_double ("T_disk", &geo.T_disk);
  if (geo.T_disk < 0)
    Exit (BAD_INPUT, "Invalid value for T_disk: T_disk >= 0\n");

  geo.hz = geo.z_max / geo.nz_cells;

  return SUCCESS;
}

int allocate_1d_grid (void)
{
  long mem_req;

  mem_req = geo.nz_cells * sizeof (*grid);

  if (!(grid = calloc (geo.nz_cells, sizeof (*grid))))
    Exit (MEM_ALLOC_ERR, "Could not allocate memory for grid of size %li\n",
          mem_req);
  Verbose_log ("\t\t- Allocated %1.2e bytes for %1.2e grid cells\n",
               (double) mem_req, (double) geo.nz_cells);

  return SUCCESS;
}
