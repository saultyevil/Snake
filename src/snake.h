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
char OUTPUT_NAME[LINE_LEN];
int OUTPUT_FILE_STAT;

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
  int n;
  double x;
  double T;
  double T_old;
  double kappa;
  double rho;
  double trans_tau;
} Grid;

Grid *grid;

#include "functions.h"
