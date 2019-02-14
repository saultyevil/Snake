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

// Allocate memory for the grid structure
int allocate_1d_grid (void)
{
  long mem_req;

  mem_req = geo.nz_cells * sizeof (*grid);

  if (!(grid = calloc ((size_t) geo.nz_cells, sizeof (*grid))))
    Exit (MEM_ALLOC_ERR, "Could not allocate memory for grid of size %li\n",
          mem_req);
  Log ("\t\t- Allocated %1.2e bytes for %1.2e grid cells\n",
       (double) mem_req, (double) geo.nz_cells);

  return SUCCESS;
}

// Figure out the number of grid cell points in the density file
size_t get_num_cells (void)
{
  size_t n_cells = 0;
  char line[LINE_LEN];

  while (fgets (line, LINE_LEN, density) != NULL)
  {
    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;
    n_cells++;
  }

  /*
   * Send the file pointer back to the beginning of the file
   */

  rewind (density);

  return n_cells;
}

// Reverse the order of an array -- assuming that it is ordered in some way
int reverse_sort (void)
{
  size_t i, j;
  double *orig_z, *orig_rho;

  orig_z = calloc (geo.nz_cells, sizeof (*orig_z));
  orig_rho = calloc (geo.nz_cells, sizeof (*orig_rho));

  for (i = 0; i < geo.nz_cells; i++)
  {
    orig_z[i] = grid[i].z;
    orig_rho[i] = grid[i].rho;
  }

  for (i = geo.nz_cells - 1, j = 0; i >=  0; i--, j++)
  {
    grid[j].z = orig_z[i];
    grid[j].rho = orig_rho[i];
  }

  free (orig_z);
  free (orig_rho);

  return SUCCESS;
}

// Read in the density from file and assign to the grid cells
// TODO: GSL interpolation for an arbitrary number of grid cells
int density_from_file (char *filepath)
{
  int i;
  int cell = 0, line_num = 0;
  char line[LINE_LEN];
  double z_coord, rho;

  if (!(density = fopen (filepath, "r")))
    Exit (FILE_OPEN_ERR, "Unable to open density file %s\n", filepath);

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

    if (sscanf (line, "%lf %lf", &z_coord, &rho) != 2)
      Exit (FILE_IN_ERR, "Syntax error on line %i in density file\n", line_num);

    grid[cell].z = z_coord;
    grid[cell].rho = rho;
    cell++;
  }

  /*
   * If the density grid is in descending order rather than ascending, reverse
   * the order
   */

  if (grid[0].z > grid[1].z)
    reverse_sort ();

  if (fclose (density))
    Exit (FILE_CLOSE_ERR, "Unable to close density file %s\n", filepath);

  return SUCCESS;
}

// A density equation I found in some lecture notes
double density_profile_disk_height (double z)
{
  return geo.irho * exp (-1.0 * pow(z, 2.0) / (2.0 * pow (geo.z_max, 2.0)));
}

// Generate a density profile based on a density equation I found in some
// lecture notes
int standard_density_profile (void)
{
  int i;
  int ncells;

  get_double ("z_max", &geo.z_max);
  if (geo.z_max < 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for z_max: z_max >= 0\n");
  get_int ("nz_cells", &ncells);
  geo.nz_cells = (size_t) ncells;
  if (geo.nz_cells <= 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for nx_cells: nx_cells > 0\n");
  get_double ("irho", &geo.irho);
  if (geo.irho < 0)
    Exit (UNKNOWN_PARAMETER, "Invalid value for irho: irho >= 0\n");

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
