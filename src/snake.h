/* ***************************************************************************
 *
 * @file snake.h
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Header file containing global macros, constants, variables and
 *        structs.
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
  int icycle;
  int nz_cells;
  double tot_tau;
  double T_init;
  double T_disk;
  double irho;
  double hz;
  double z_max;
  double X, Y, Z;
} Geometry;

Geometry geo;

/*
 * The structure for each cell on the 1D grid
 */

typedef struct Grid
{
  int n;
  double z;
  double T;
  double T_old;
  double kappa;
  double rho;
  double cell_tau;
  double tau_depth;
} Grid;

Grid *grid;

#include "functions.h"
