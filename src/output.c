/* ***************************************************************************
 *
 * @file output.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Functions for initialising and closing the output file and functions
 *        for writing out the grid to file.
 *
 * @details
 *
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>

#include "snake.h"

FILE *outfile;
char output_name[LINE_LEN];

int init_outfile (void)
{
  strcpy (output_name, "sgrid.out");
  Log ("\t- Initialising output file %s\n", output_name);
  if (!(outfile = fopen (output_name, "w")))
    Exit (5, "Can't open file %s to write\n", output_name);
  Verbose_log ("\t\t- Opened %s with write access\n", output_name);

  return SUCCESS;
}

int close_outfile (void)
{
  if (fclose (outfile))
    Exit (5, "Can't close the output file\n");
  Verbose_log (" - Closed %s successfully\n", output_name);

  return SUCCESS;
}

int write_grid (void)
{
  int i;

  if (geo.icycle == 0)
    fprintf (outfile, "# Grid init\n");
  else
    fprintf (outfile, "# Cycle %i\n", geo.icycle);

  fprintf (outfile,  // Write header
           "# n_cell zcoord rho rosseland_opacity transverse_optical_depth temperature\n");

  for (i = 0; i < geo.nz_cells; i++)  // Write grid
    fprintf (outfile, "%i %e %e %e %e %e\n", grid[i].n, grid[i].z, grid[i].rho,
             grid[i].kappa, grid[i].trans_tau, grid[i].T);

  return SUCCESS;
}
