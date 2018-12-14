/* ***************************************************************************
 *
 * @file update_opac.c
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

#include "../snake.h"
#include "../flib/flib.h"
#include "../interp/2d_interp.h"

// Update the opacity in each grid cell using the Rosseland Mean Opacity
int update_cell_opacities (void)
{
  int i;
  double logT, logR, logRMO;

  Log_verbose ("\t\t- Updating cell opacities\n");

  for (i = 0; i < geo.nz_cells; i++)
  {
    logT = log10 (grid[i].T);
    logR = log10 (grid[i].rho / pow (grid[i].T * 1e-6, 3.0));

    #ifdef DEBUG
      Log ("logT = %f logR = %f\n", logT, logR);
    #endif

    #ifdef OPAL
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
    #else
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

      #ifdef DEBUG
        Log ("GSL interpolated logRMO = %f\n\n", *logRMO);
      #endif
    #endif

    /*
     * Finally update the opacity of the grid cell
     */

    grid[i].kappa = pow (10.0, logRMO);
  }

  return SUCCESS;
}
