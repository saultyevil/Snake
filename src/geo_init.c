/* ***************************************************************************
 *
 * @file setup.c
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Contains the function to initialise the geometry of the problem.
 *
 * @details
 *
 * ************************************************************************** */

#include <string.h>

#include "snake.h"

// Initialise the geometry of the problem
int init_geo (void)
{
  /*
   * Get the grid and model types and call the appropriate grid initialisation
   * routine -- assuming at some point we will try a cylindrical and spherical
   * grids
   */

  get_string ("geo_type", geo.geo_type);
  if (!(strcmp (geo.geo_type, PLANAR)) || !(strcmp (geo.geo_type, SPHERICAL)))
  {
    Log ("\t- Initialising grid for 1d %s atmosphere\n", geo.geo_type);
    init_grid ();
  }
  else
    Exit (UNKNOWN_PARAMETER, "Invalid choice %s for geo_type\n", geo.geo_type);

  return SUCCESS;
}
