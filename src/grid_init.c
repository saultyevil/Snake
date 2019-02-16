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

// Get the grid parameters from file
int get_temp_params (void)
{
  get_double ("T_init", &geo.T_init);
  if (geo.T_init < 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for T_init: T_init >= 0\n");
  get_double ("T_disk", &geo.T_disk);
  if (geo.T_disk < 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for T_disk: T_disk >= 0\n");

  geo.hz = geo.z_max / geo.nz_cells;

  return SUCCESS;
}

// Main control function for initialising the grid cells
int init_grid (void)
{
  Log_verbose ("\t\t- Initialising grid cells\n");

  get_temp_params ();

  if (check_for_parameter ("opacity_table"))
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

  Log ("\t\t- Atmosphere height %e cm\n", grid[geo.nz_cells - 1].z);

  /*
   * Update the opacity and optical depth for each cell. We also initialise the
   * opacity table which is being used here
   */

  init_opacity_table ();
  update_cell_opacities ();
  find_vertical_tau ();

  Log_verbose ("\t\t- Writing initial grid to file\n");
  write_grid ();

  return SUCCESS;
}
