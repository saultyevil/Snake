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

#include "snake.h"

int write_grid_to_file (char *filename)
{
  int i;
  FILE *outgrid;

  if ((outgrid = fopen (filename, "w")) == NULL)
    Exit (5, "Can't write to file %s\n", filename);

  for (i = 0; i < geo.nx_cells; i++)
  {
    fprintf (outgrid, "%f\n", grid[i].opac);
  }

  if (fclose (outgrid))
    Exit (5, "Can't close file %s\n", filename);

  return EXIT;
}

int output_results (void)
{
  return EXIT;
}

int calculate_intensity (void)
{
  return EXIT;
}
