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

int init_geo (void)
{
  double out_freq = 1e5;

  /*
   * Get any variables which don't fit elsewhere
   */

  get_double ("output_freq", &out_freq);
  if ((PROGRESS_OUT_FREQ = (int) out_freq) < 0)
    Exit (2, "Invalid value for output_freq: output_freq > 0\n");

  /*
   * Get the grid and model types sand call the appropriate grid initialisation
   * routine
   */

  get_string ("geo_type", geo.geo_type);

  if (!(strcmp (geo.geo_type, PLANAR)) || !(strcmp (geo.geo_type, SPHERICAL)))
  {
    Log ("\t- Initialising grid for 1d %s atmosphere\n", geo.geo_type);
    init_grid ();
  }
  else
    Exit (2, "Invalid choice '%s' for geo_type\n", geo.geo_type);

  return SUCCESS;
}
