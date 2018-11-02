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

#define TRUE 1
#define FALSE 0
#define EXIT 0
#define ERROR 1
#define LINE_LEN 256


/*
 * Global variables
 */

int VERBOSITY;
int PROGRESS_OUT_FREQ;

/*
 * The available grid types -- note that this code will exploit symmetry
 */

#define PLANAR "planar"
#define SPHERICAL "spherical"

/*
 * The structure to hold various geometry parameters
 */

typedef struct Geometry
{
  char geo_type[LINE_LEN];
  int nx_cells;
  double t_star;
  double hx;
  double x_max;
} Geometry;

Geometry geo;

typedef struct Grid
{
  double n;
  double x;
  double T;
  double opac;
  double dens;
} Grid;

Grid *grid;

#include "functions.h"
