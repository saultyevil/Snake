/* ***************************************************************************
 *
 * @file opac.c
 *
 * @author E. J. Parkinson
 *
 * @date 10 Dec 2018
 *
 * @brief Functions for determining the Rosseland Mean Opacity of a cell.
 *
 * @details
 *
 * ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../snake.h"
#include "../flib/flib.h"
#include "opac.h"

double *buffer;

int init_opacity_table (void)
{
#ifdef OPAL
  Log ("\t- Checking for Opal Opacity Table GN93hz\n\n");
  if (access ("GN93hz", F_OK) == -1)
    Exit (15, "GN93hz not found in current directory.\n");

  get_double ("X", &geo.X);
  get_double ("Z", &geo.Z);
  if (geo.X + geo.Z > 1)
    Exit (2, "Invalid choice for X =%f or Z = %f. X + Z <= 1.0", geo.X, geo.Z);
  geo.Y = 1.0 - geo.X - geo.Z;
#else
  get_string ("table_location", geo.opacity_table_filepath);
  read_2d_opact_table (geo.opacity_table_filepath, buffer);
  init_gsl_interp ();
#endif

  return SUCCESS;
}

int allocate_opacity (void)
{
  /*
   * Allocate a temporary buffer to use when scanf is used to read in the
   * opacity table from file
   */

  if (!(buffer = calloc (N_ROWS * N_COLS, sizeof (*buffer))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for opacity table\n");
  int mem_req = N_ROWS * N_COLS * sizeof (*buffer);
  Log ("\t\t- Allocated %1.2e bytes for opacity table\n", (double) mem_req);

  /*
   * Allocate memory for logT, logR and logRMO arrays
   */

  if (!(logT_table = calloc (N_LOG_T, sizeof (*logT_table))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for gsl_logT\n");
  if (!(logR_table = calloc (N_LOG_R, sizeof (*logR_table))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for gsl_logR\n");
  if (!(logRMO_table = calloc (N_LOG_R * N_LOG_T, sizeof (*logRMO_table))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for gsl_logRMO\n");

  return SUCCESS;
}

int read_2d_opact_table (char *file_path, double *opact_table)
{
  int i, j;
  int n, col, row, skip_lines;
  FILE *opact_file;

  if (!(opact_file = fopen (file_path, "r")))
    Exit (15, "Can't open 2d opacity %s\n", file_path);
  Log ("\t- Opacity table %s opened\n", file_path);

  allocate_opacity ();

  /*
   * Skip the first two lines in the opacity data which are assumed to be the
   * header lines logR and logT
   */

  skip_lines = 2;
  for (i = 0; i < skip_lines; i++)
  {
    n = fscanf (opact_file, "%*s");
    if (n)
      Exit (11, "(%s:%s): fscanf didn't discard header\n", __FILE__, __LINE__);
  }

  /*
   * Read the data into opacity data array - we are assuming here that memory
   * has already been allocated for the array
   */

  col = 0;
  row = 0;
  while ((n = fscanf (opact_file, "%lf", &buffer[i2d (row, col)])) != EOF)
  {
    if (n != 1)
      Exit (11, "(%s:%s): fscanf failed to return a single number\n", __FILE__,
            __LINE__);
    if (++col == N_COLS)
    {
      col = 0;
      row++;
    }
  }

  /*
   * Write the temporary buffer into more appropriate arrays which are better
   * suited for GSL interpolation routines
   */

  for (i = 0; i < N_LOG_T; i++)
    logT_table[i] = buffer[i2d (1+i, 0)];

  for (i = 0; i < N_LOG_R; i++)
    logR_table[i] = buffer[i2d (0, 1+i)];

  for (i = 0; i < N_LOG_T; i++)
    for (j = 0; j < N_LOG_R; j++)
      logRMO_table[i2d (i, j)] = buffer[i2d (1+i, 1+j)];

  free (buffer);

  #ifdef DEBUG
    Log ("logT\n");
    for (i = 0; i < N_LOG_T; i++)
      Log ("%f ", logT_table[i]);
    Log ("\nlogR\n");
    for (i = 0; i < N_LOG_R; i++)
      Log ("%f ", logR_table[i]);
    Log("\nlogRMO\n");
    for (i = 0; i < N_LOG_T; i++)
    {
      for (j = 0; j < N_LOG_R; j++)
        Log ("%+f ", logRMO_table[i2d (i, j)]);
      Log ("\n");
    }
  #endif

  return SUCCESS;
}

int update_cell_opacities (void)
{
  int i;
  double logT, logR, logRMO;

  Verbose_log ("\t\t- Updating cell opacities\n");

  for (i = 0; i < geo.nz_cells; i++)
  {
    logT = log10 (grid[i].T);
    logR = log10 (grid[i].rho / pow (grid[i].T * 1e-6, 3.0));

    #ifdef DEBUG
      Log ("logT = %f logR = %f\n", logT, logR);
    #endif

    // #ifdef OPAL

      /*
       * Due to how my lazy Fortran interoparability works, we have to first
       * convert these variables to floats, otherwise we will segfault.
       */

      float X = (float) geo.X;
      float Z = (float) geo.Z;
      float T6f = (float) (grid[i].T * 1e-6);
      float Rf = (float) (grid[i].rho / pow (T6f, 3.0));

      /*
       * Ensure that T and R are in the table range
       */

      if ((logR < OP_MIN_LOG_R) || (logR > OP_MAX_LOG_R))
      {
        Log_error ("Cell %i: logR out of bounds: %f\n", grid[i].n, logR);
        Log_error ("\t%f < logR < %f\n", OP_MIN_LOG_R, OP_MAX_LOG_R);
        Exit (TABLE_BOUNDS, "logR out of Opal table range for cell %i\n", grid[i].n);
      }
      if ((logT < OP_MIN_LOG_T) || (logT > OP_MAX_LOG_T))
      {
        Log_error ("Cell %i: logT out of bounds: %f\n", grid[i].n, logT);
        Log_error ("\t%f < logT < %f\n", OP_MIN_LOG_T, OP_MAX_LOG_T);
        Exit (TABLE_BOUNDS, "logT out of Opal table range for cell %i\n", grid[i].n);
      }

      /*
       * Call the Opal Opacity interpolation function -- see opal.f and flib.h
       * for more detailed description of how this works
       */

      opacgn93_ (&Z, &X, &T6f, &Rf);
      logRMO = e_.opact;

      #ifdef DEBUG
        Log ("opal interpolated logRMO = %f\n", logRMO);
      #endif
    // #else

      /*
       * Ensure that T and R are in the table range
       */

      if ((logR < MIN_LOG_R) || (logR > MAX_LOG_R))
      {
        Log_error ("Cell %i: logR out of bounds: %f\n", grid[i].n, logR);
        Log_error ("\t%f < logR < %f\n", MIN_LOG_R, MAX_LOG_R);
        Exit (TABLE_BOUNDS, "logR out of table range for cell %i\n", grid[i].n);
      }
      if ((logT < MIN_LOG_T) || (logT > MAX_LOG_T))
      {
        Log_error ("Cell %i: logT out of bounds: %f\n", grid[i].n, logT);
        Log_error ("\t%f < logT < %f\n", MIN_LOG_T, MAX_LOG_T);
        Exit (TABLE_BOUNDS, "logT out of table range for cell %i\n", grid[i].n);
      }

      /*
       * Call the 2D interpolation function designed to work with the tables which
       * is created by the included Python script create_opacity_table.py.
       */

      opac_2d (logT, logR, &logRMO);

    // #endif

    /*
     * Finally update the opacity of the grid cell
     */

    grid[i].kappa = pow (10.0, logRMO);
  }

  return SUCCESS;
}
