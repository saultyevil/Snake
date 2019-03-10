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

// Initialise the grid output file
void
init_outfile (void)
{
  strcpy (output_name, "sgrid.out");
  Log ("\t- Initialising output file %s\n", output_name);
  if (!(outfile = fopen (output_name, "w")))
    Exit (FILE_IN_ERR, "Can't open file %s to write\n", output_name);
  Log_verbose ("\t\t- Opened %s with write access\n", output_name);
}

// Close the grid output file
void
close_outfile (void)
{
/* ************************************************************************** */

  /*
   * Something odd is going on with memory allocations when I'm using GSL, i.e.
   * when OPAL is not defined. When this happens, fclose fails and sends a
   * SIGABT.
   *
   * free(): invalid next size (normal)
   * Aborted (core dumped)
   *
   * I think this could be somewhat related to the weird memory issues I'm
   * having when I'm allocating memory for logRMO_table. For now, the code will
   * run fine if I only close the file when using Opal instead of GSL
   * interpolation, hence I've added a very hack fix :^).
   *
   * 14/12/18: something I changed about how much memory logRMO_table is
   * allocated fixed this problem.
   */


  if (fclose (outfile))
    Exit (FILE_CLOSE_ERR, "Can't close the output file\n");
  Log_verbose (" - Closed %s successfully\n", output_name);
}

// Write to the grid output file. This should only need to be called and not
// looped over and called for each cell
void
write_grid (void)
{
  int i;

  if (geo.icycle == 0)
    fprintf (outfile, "# Grid init tot_tau %e\n", geo.tot_tau);
  else
    fprintf (outfile, "# Cycle %i tot_tau %e\n", geo.icycle, geo.tot_tau);

  fprintf (outfile,  // Write header
           "# n_cell zcoord rho rosseland_opacity cell_optical_depth cumulative_tau temperature\n");

  for (i = 0; i < geo.nz_cells; i++)  // Write grid
    fprintf (outfile, "%+i %+e %+e %+e %+e %+e %+e\n", grid[i].n, grid[i].z, grid[i].rho, grid[i].kappa,
             grid[i].cell_tau, grid[i].tau_depth, grid[i].T);
}
