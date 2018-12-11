/* ***************************************************************************
 *
 * @file opacities.c
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

#include "snake.h"
#include "flib.h"
#include "opacities.h"

int i2d (int row, int col)
{
  return row * N_COLS + col;
}

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
  rmo_table_2d = calloc (N_ROWS * N_COLS, sizeof (*rmo_table_2d));
  read_2d_opact_table (geo.opacity_table_filepath, rmo_table_2d);
#endif

  return SUCCESS;
}

int read_2d_opact_table (char *file_path, double *opact_table)
{
  int i, n, col, row;
  int skip_lines;
  FILE *opact_file;

  if (!(opact_file = fopen (file_path, "r")))
    Exit (15, "Can't open 2d opacity %s\n", file_path);
  Log ("\t- Opacity table %s opened\n\n", file_path);

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
  while ((n = fscanf (opact_file, "%lf", &opact_table[i2d (row, col)])) != EOF)
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

  return SUCCESS;
}

int opac_2d (double T6, double R, double *log_kappa)
{
  *log_kappa = 10.5;
  return SUCCESS;
}

int update_cell_opacities (void)
{
  int i;
  double T6, R;
  double logT, logR;
  double log_kappa;

  Verbose_log ("\t\t- Updating cell opacities\n");

  for (i = 0; i < geo.nz_cells; i++)
  {
    T6 = grid[i].T * 1e-6;
    R = grid[i].rho / pow (T6, 3.0);
    logR = (float) log10 (R);
    logT = (float) log10 (grid[i].T);

    #ifdef OPAL
      /*
       * Due to how my lazy Fortran interoparability works, we have to first
       * convert these variables to floats, otherwise we will segfault.
       */

      float X = (float) geo.X;
      float Z = (float) geo.Z;
      float T6f = (float) T6;
      float Rf = (float) R;

      /*
       * Ensure that T and R are in the table range
       */

      if ((logR < OP_MIN_LOG_R) || (logR > OP_MAX_LOG_R))
      {
        Log_error ("Cell %i:\n", grid[i].n);
        Log_error ("\tlogR out of bounds: %f\n", logR);
        Log_error ("\t%f < logR < %f\n", OP_MIN_LOG_R, OP_MAX_LOG_R);
        Exit (72, "logR out of Opal table range for cell %i\n", grid[i].n);
      }
      if ((logT < OP_MIN_LOG_T) || (logT > OP_MAX_LOG_T))
      {
        Log_error ("Cell %i:\n", grid[i].n);
        Log_error ("\tlogT out of bounds: %f\n", logT);
        Log_error ("\t%f < logT < %f\n", OP_MIN_LOG_T, OP_MAX_LOG_T);
        Exit (72, "logT out of Opal table range for cell %i\n", grid[i].n);
      }

      /*
       * Call the Opal Opacity interpolation function -- see opal.f and flib.h
       * for more detailed description of how this works
       */

      opacgn93_ (&Z, &X, &T6f, &Rf);
      log_kappa = e_.opact;
      grid[i].kappa = pow (10.0, log_kappa);
    #else
      /*
       * Ensure that T and R are in the table range
       */

      if ((logR < MIN_LOG_R) || (logR > MAX_LOG_R))
      {
        Log_error ("Cell %i:\n", grid[i].n);
        Log_error ("\tlogR out of bounds: %f\n", logR);
        Log_error ("\t%f < logR < %f\n", MIN_LOG_R, MAX_LOG_R);
        Exit (72, "logR out of table range for cell %i\n", grid[i].n);
      }
      if ((logT < MIN_LOG_T) || (logT > MAX_LOG_T))
      {
        Log_error ("Cell %i:\n", grid[i].n);
        Log_error ("\tlogT out of bounds: %f\n", logT);
        Log_error ("\t%f < logT < %f\n", MIN_LOG_T, MAX_LOG_T);
        Exit (72, "logT out of table range for cell %i\n", grid[i].n);
      }

      /*
       * Call the 2D interpolation function designed to work with the tables which
       * is created by the included Python script create_opacity_table.py.
       */

      opac_2d (T6, R, &log_kappa);
      grid[i].kappa = pow (10.0, log_kappa);
    #endif
  }

  return SUCCESS;
}