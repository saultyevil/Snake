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

#include <string.h>

#include "snake.h"

int init_default_pars (void)
{
  /*
   * Initialise the general parameters for iterations
   */

  PROGRESS_OUT_FREQ = (int) 1e5;

  /*
   * Initialise the geo pars to be a small planar atmosphere
   */

  strcpy (geo.geo_type, PLANAR);
  geo.nx_cells = 100;
  geo.x_max = 1e12;
  geo.t_star = 1e5;
  geo.irho = 1e-5;
  geo.X = 0.74;  // mass fractions for hydrogen X, helium Y and metals Z
  geo.Z = 0.02;
  geo.Y = 1.0 - geo.X - geo.Y;

  return SUCCESS;
}
