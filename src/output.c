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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "snake.h"

int init_filenames (void)
{
  strcpy (file_names.gDensity, "gridDensity.txt");
  strcpy (file_names.gOpacity, "gridOpacity.txt");
  strcpy (file_names.gTemperature, "gridTemperature.txt");

  return SUCCESS;
}

int write_grid_to_file (void)
{
  int i;
  int n_files = 3;
  char *names[] = {file_names.gOpacity,
                   file_names.gDensity,
                   file_names.gTemperature};

  /*
   * Create an array of file pointers and point them at a file
   */

  FILE **files = malloc (sizeof (*files) * n_files);
  if (!files)
    Exit (15, "Couldn't allocate memory to store output files\n");

  for (i = 0; i < n_files; i++)
    if (!(files[i] = fopen (names[i], "w")))
      Exit (5, "Can't open file %s to write\n", names[i]);

  /*
   * Write the grid parameters to file
   */

  for (i = 0; i < geo.nx_cells; i++)
  {
    fprintf (files[0], "%f\n", grid[i].kappa);
    fprintf (files[1], "%f\n", grid[i].rho);
    fprintf (files[2], "%f\n", grid[i].T);
  }

  for (i = 0; i < n_files; i++)
    if (fclose (files[i]))
      Exit (5, "Can't close file %s\n", names[i]);

  return SUCCESS;
}
