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

FILE *outfile;

int init_outfile (char *filename)
{
  Log ("\t- Initialising output file %s\n", OUTPUT_NAME);
  if (!(outfile = fopen (filename, "w")))
    Exit (5, "Can't open file %s to write\n", filename);
  Log_verbose ("\t\t- Opened %s with write access\n", filename);
  OUTPUT_FILE_STAT = 1;

  return SUCCESS;
}

int close_outfile (void)
{
  if (fclose (outfile))
    Exit (5, "Can't close the output file\n");
  Log_verbose (" - Closed %s successfully\n", OUTPUT_NAME);

  return SUCCESS;
}

int write_grid_to_file ()
{
  int i;

  if (!OUTPUT_FILE_STAT)
    init_outfile (OUTPUT_NAME);

  /*
   * Write the grid parameters to file
   */

  if (geo.icycle == 0)
    fprintf (outfile, "# Grid init\n");
  else
    fprintf (outfile, "# Cycle %i\n", geo.icycle);

  fprintf (outfile, "# n_cell xcoord rho ross_kappa trans_tau temp\n");
  for (i = 0; i < geo.nx_cells; i++)
    fprintf (outfile, "%i %e %e %e %e %e\n", grid[i].n, grid[i].x, grid[i].rho,
             grid[i].trans_tau, grid[i].kappa, grid[i].T);

  return SUCCESS;
}
