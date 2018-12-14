/* ***************************************************************************
 *
 * @file 2d_interp.c
 *
 * @author E. J. Parkinson
 *
 * @date 12 Dec 2018
 *
 * @brief Functions for initialising and interpolating over a 2D opacity table
 *
 * @details
 *
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <gsl/gsl_interp2d.h>

#include "2d_interp.h"
#include "../snake.h"

gsl_interp2d *interp;
gsl_interp_accel *logR_accel, *logT_accel;

// Initialise the opacity table which is going to be used
int init_opacity_table (void)
{
  #ifdef OPAL
    Log ("\t- Checking for Opal Opacity Table GN93hz\n");
      if (access ("GN93hz", F_OK) == -1)
        Exit (15, "GN93hz not found in current directory.\n");

    get_double ("X", &geo.X);
    get_double ("Z", &geo.Z);
    if (geo.X + geo.Z > 1)
      Exit (2, "Invalid choice for X =%f or Z = %f. X + Z <= 1.0", geo.X, geo.Z);
    geo.Y = 1.0 - geo.X - geo.Z;
  #else
    get_string ("table_location", geo.opacity_table_filepath);
    read_2d_opact_table (geo.opacity_table_filepath);
    init_gsl_interp ();
  #endif

  return SUCCESS;
}

// Allocate memory for the opacity tables
int allocate_opacity_table (void)
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
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for logT_table\n");
  if (!(logR_table = calloc (N_LOG_R, sizeof (*logR_table))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for logR_table\n");

/* ************************************************************************** */

  /*
   * Valgrind is very upset with this memory allocation and I'm not sure why...
   * Address 0x64b0e68 is 0 bytes after a block of size 11,016 alloc'd
   *
   * This seems to be causing trouble when I try to close the grid output file
   * as well :-(
   *
   * TODO: figure out why the above error occurs
   */

  if (!(logRMO_table = calloc (N_LOG_R * N_LOG_T, sizeof (*logRMO_table))))
    Exit (MEM_ALLOC_ERR, "Unable to allocate memory for logRMO_table\n");

/* ************************************************************************** */

  return SUCCESS;
}

// Read the 2D opacity table into memory
int read_2d_opact_table (char *file_path)
{
  int i, j;
  int n, col, row, skip_lines;
  FILE *opact_file;

  if (!(opact_file = fopen (file_path, "r")))
    Exit (FILE_OPEN_ERR, "Can't open opacity table %s\n", file_path);
  Log ("\t- Opacity table %s opened\n", file_path);

  allocate_opacity_table ();

  /*
   * Skip the first two lines in the opacity data which are assumed to be the
   * header lines logR and logT
   */

  skip_lines = 2;
  for (i = 0; i < skip_lines; i++)
  {
    n = fscanf (opact_file, "%*s");
    if (n)
      Exit (FILE_IN_ERR, "fscanf didn't discard header\n");
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
      Exit (FILE_IN_ERR, "fscanf failed to return a single number\n");
    if (++col == N_COLS)
    {
      col = 0;
      row++;
    }
  }

  if (fclose (opact_file))
    Exit (FILE_CLOSE_ERR, "Cannot close opacity table %s\n", file_path);

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

// Initialise the GSL interpolation routines
int init_gsl_interp (void)
{
  const gsl_interp2d_type *T = gsl_interp2d_bilinear;

  Log ("\t- Initialising GSL interpolation routines\n");

  /*
   * Choose the type of simple 2D interpolation -- 2D splines are also possible
   */

  strcpy (gsl_interp_choice, "bilinear");
  get_string ("gsl_interpolation", gsl_interp_choice);
  if (!(strcmp (gsl_interp_choice, "bilinear")))
    T = gsl_interp2d_bilinear;
  else if (!(strcmp (gsl_interp_choice, "bicubic")))
    T = gsl_interp2d_bicubic;
  else
    Exit (UNKNOWN_PARAMETER, "Unknown interpolation choice for GSL\n");

  /*
   * Allocate memory for the GSL interpolation routines
   */

  interp = gsl_interp2d_alloc (T, N_LOG_R, N_LOG_T);
  logR_accel = gsl_interp_accel_alloc ();
  logT_accel = gsl_interp_accel_alloc ();

  #ifdef DEBUG
    Log("logRMO before being read into gsl\n");
    for (int i = 0; i < N_LOG_T / 5; i++)
    {
      int ncols = 0;
      for (int j = 0; j < N_LOG_R; j++)
      {
        if (logRMO_table[i2d (i, j)] != 0)
          ncols++;
        Log ("%+f ", logRMO_table[i2d (i, j)]);
      }
      Log ("\n");
      if (ncols != N_LOG_R)
        Log ("panic, ncols != N_LOG_R");
    }
  #endif

  /*
   * Initialise the interpolation routine
   */

  gsl_interp2d_init (interp, logR_table, logT_table, logRMO_table, N_LOG_R,
                     N_LOG_T);

  #ifdef DEBUG
    Log ("logRMO within gsl\n");
    for (int y = 0; y < N_LOG_T / 5; y++)
    {
      for (int x = 0; x < N_LOG_R; x++)
      {
        double logRMO = gsl_interp2d_get (interp, logRMO_table, x, y);
        Log ("%+f ", logRMO);
      }
      Log ("\n");
    }
  #endif

/* ************************************************************************** */
  /*
   * Temporary hacky fix for GSL until I work out why the diagonal elements in
   * the GSL routine are read in as being 0. Most likely a weird memory issues
   * with incorrect dimensions or some other awful thing
   * TODO: figure out why GSL isn't working how I want it to
   */

  for (int y = 0; y < N_LOG_T; y++)
    for (int x = 0; x < N_LOG_R; x++)
      gsl_interp2d_set (interp, logRMO_table, x, y, logRMO_table[i2d (y, x)]);


/* ************************************************************************** */

  return SUCCESS;
}

// Clean up the opacity tables
int clean_up_opac_tables (void)
{
  free (logR_table);
  free (logT_table);
  free (logRMO_table);
  Log_verbose (" - Opacity table cleaned up successfully\n");

  return SUCCESS;
}

// Clean up the GSL stuff
int clean_up_gsl (void)
{
  gsl_interp2d_free (interp);
  gsl_interp_accel_free (logR_accel);
  gsl_interp_accel_free (logT_accel);
  Log_verbose (" - GSL routines cleaned up successfully\n");

  return SUCCESS;
}

// Interpolate using the 2D GSL interpolation routines
int opac_2d (double logT, double logR, double *logRMO)
{
  *logRMO = gsl_interp2d_eval (interp, logR_table, logT_table, logRMO_table,
                               logR, logT, logR_accel, logT_accel);

  return SUCCESS;
}