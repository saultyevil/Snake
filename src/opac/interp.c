/* ***************************************************************************
 *
 * @file interp.c
 *
 * @author E. J. Parkinson
 *
 * @date 12 Dec 2018
 *
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

#include <gsl/gsl_interp2d.h>

#include "opac.h"
#include "../snake.h"

const gsl_interp2d_type *T;
gsl_interp2d *interp;
gsl_interp_accel *logR_accel, *logT_accel;

int init_gsl_interp (void)
{
  Log ("\t- Initialising GSL interpolation routines\n");

  T = gsl_interp2d_bilinear;
  // T = gsl_interp2d_bicubic;
  interp = gsl_interp2d_alloc (T, N_LOG_R, N_LOG_T);

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

  gsl_interp2d_init (interp, logR_table, logT_table, logRMO_table,
                     N_LOG_R, N_LOG_T);

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

  Log ("time to be awful\n");
  for (int y = 0; y < N_LOG_T; y++)
  {
    for (int x = 0; x < N_LOG_R; x++)
    {
      gsl_interp2d_set (interp, logRMO_table, x, y, logRMO_table[i2d (y, x)]);
    }
  }

  Log ("logRMO within gsl after being awful\n");
  for (int y = 0; y < N_LOG_T / 5; y++)
  {
    for (int x = 0; x < N_LOG_R; x++)
    {
      double logRMO = gsl_interp2d_get (interp, logRMO_table, x, y);
      Log ("%+f ", logRMO);
    }
    Log ("\n");
  }

  logR_accel = gsl_interp_accel_alloc ();
  logT_accel = gsl_interp_accel_alloc ();

  return SUCCESS;
}

int opac_2d (double logT, double logR, double *logRMO)
{
  double temp_logRMO;

  temp_logRMO = gsl_interp2d_eval (interp, logR_table, logT_table,
    logRMO_table, logR, logT, logR_accel, logT_accel);

  #ifdef DEBUG
   Log ("my interpolated logRMO = %f\n\n", temp_logRMO);
  #endif

  *logRMO = temp_logRMO;

  return SUCCESS;
}
