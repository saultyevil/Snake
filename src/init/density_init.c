/* ***************************************************************************
 *
 * @file init_density.c
 *
 * @author E. J. Parkinson
 *
 * @date 21 Nov 2018
 *
 * @brief Functions designed to set the density for the 1D grid.
 *
 * @details
 *
 * ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../snake.h"

FILE *density;

int get_num_cells (void)
{
  int n_cells = 0;
  char line[LINE_LEN];

  while (fgets (line, LINE_LEN, density) != NULL)
  {
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    n_cells++;
  }

  rewind (density);

  return n_cells;
}

// TODO: some form of interpolation might be nice?
int density_from_file (char *filepath)
{
  int i;
  int cell = 0, line_num = 0;
  char line[LINE_LEN], z_coord[LINE_LEN], rho[LINE_LEN];

  if (!(density = fopen (filepath, "r")))
    Exit (19, "Unable to open density file %s\n", filepath);

  /*
   * Get the number of density and z coordinates, initialise the grid and read
   * in z and rho from file
   */

  geo.nz_cells = get_num_cells ();
  allocate_1d_grid ();

  for (i = 0; i < geo.nz_cells; i++)
  {
    grid[i].n = i;
    grid[i].T = grid[i].T_old = geo.T_init;
  }

  while (fgets (line, LINE_LEN, density) != NULL)
  {
    line_num++;
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;

    if (sscanf (line, "%s %s", z_coord, rho) != 2)
      Exit (7, "Syntax error on line %i in density file\n", line);

    grid[cell].z = atof (z_coord);
    grid[cell].rho = atof (rho);
    cell++;
  }

  if (fclose (density))
    Exit (19, "Unable to close density file %s\n", filepath);

  return SUCCESS;
}

int standard_density_profile (void)
{
  int i;

  get_int ("nz_cells", &geo.nz_cells);
  if (geo.nz_cells <= 0)
    Exit (2, "Invalid value for nx_cells: nx_cells > 0\n");
  get_double ("irho", &geo.irho);
  if (geo.irho < 0)
    Exit (2, "Invalid value for irho: irho >= 0\n");

  allocate_1d_grid ();

  for (i = 0; i < geo.nz_cells; i++)
  {
    grid[i].n = i;
    grid[i].T = grid[i].T_old = geo.T_init;
    grid[i].z = i * geo.hz;
    grid[i].rho = density_profile_disk_height (grid[i].z);
  }

  return SUCCESS;
}

double density_profile_disk_height (double z)
{
  return geo.irho * exp (-1.0 * pow(z, 2.0) / (2.0 * pow (geo.z_max, 2.0)));
}
