/* ***************************************************************************
 *
 * @file default_pars.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Contains the function for initialising default parameters to the
 *        parameters which are read in from a parameter file.
 *
 * @details
 *
 * ************************************************************************** */

#include <string.h>

#include "snake.h"

// Initialise various default various for global parameters which are read in
int init_snake (void)
{
  /*
   * System parameters
   */

  modes.opal = FALSE;
  modes.low_temp = FALSE;

  /*
   * Geometry parameters for planar atmosphere
   */

  strcpy (geo.geo_type, PLANAR);
  geo.nz_cells = 100;
  geo.z_max = 1e10;
  geo.T_init = 1e5;
  geo.irho = 1e-5;

  /*
   * Mass fractions for hydrogen X, helium Y and metals Z - default values are
   * the standard solar composition
   */

  geo.X = 0.74;
  geo.Z = 0.02;
  geo.Y = 1.0 - geo.X - geo.Y;

  return SUCCESS;
}
