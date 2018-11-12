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
#define SUCCESS 0
#define FAILURE 1
#define LINE_LEN 256
#define MAX_ITER 500

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
  double irho;
  double hx;
  double x_max;
  double X, Y, Z;
} Geometry;

Geometry geo;

typedef struct Grid
{
  double n;
  double x;
  double T;
  double T_old;
  double kappa;
  double rho;
} Grid;

Grid *grid;

/*
 * Note that filenames are set in the file output.c in the function
 * int init_filenames (void).
 */

typedef struct File_names
{
  char grid_density[LINE_LEN];
  char grid_opacity[LINE_LEN];
  char grid_temperature[LINE_LEN];
} File_names;

File_names file_names;

#include "functions.h"
