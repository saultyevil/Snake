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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "snake.h"

int init_filenames (void)
{
  strcpy (file_names.grid_density, "grid_density.out");
  strcpy (file_names.grid_opacity, "grid_opacity.out");
  strcpy (file_names.grid_temperature, "grid_temperature.out");

  return SUCCESS;
}

int write_grid_to_file (char *dir_name)
{
  int i;
  int n_files = 3;
  char path[LINE_LEN];
  char *names[] = {file_names.grid_opacity, file_names.grid_density,
                   file_names.grid_temperature};
  struct stat st;

  /*
   * Check to see if a directory with name dir_name already exists. If it
   * doesn't, then create the directory.
   */

  if (stat (dir_name, &st) == -1)
  {
    if (mkdir (dir_name, 0700) == -1)
      Exit (11, "Cannot create directory %s\n", dir_name);
    Log_verbose (" - Created directory %s\n", dir_name);
  }

  /*
   * Create an array of file pointers and point them at a file
   */

  FILE **files = malloc (sizeof (*files) * n_files);
  if (!files)
    Exit (15, "Couldn't allocate memory to store output files\n");

  for (i = 0; i < n_files; i++)
  {
    strcpy (path, dir_name);
    strcat (path, "/");
    strcat (path, names[i]);
    if (!(files[i] = fopen (path, "w")))
      Exit (5, "Can't open file %s to write\n", names[i]);
  }

  /*
   * Write the grid parameters to file
   */

  for (i = 0; i < geo.nx_cells; i++)
  {
    fprintf (files[0], "%e\n", grid[i].kappa);
    fprintf (files[1], "%e\n", grid[i].rho);
    fprintf (files[2], "%e\n", grid[i].T);
  }

  for (i = 0; i < n_files; i++)
    if (fclose (files[i]))
      Exit (5, "Can't close file %s\n", names[i]);

  return SUCCESS;
}
