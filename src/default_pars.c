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
  geo.x_max = 1.0;
  geo.t_star = 4e4;

  return EXIT;
}
