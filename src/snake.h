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
#define LINE_LEN 128
#define MAX_ITER 500

/*
 * Global variables
 */

int INIT_LOGFILE;
int VERBOSITY;

/*
 * The available grid types -- note that this code will exploit symmetry
 */

#define PLANAR "planar"
#define SPHERICAL "spherical"
#define OPAL_FILENAME "GN93hz"

/*
 * An enumerator storing the possible error return codes in Snake
 */

enum ERRORS
{
  FILE_OPEN_ERR = 1,
  FILE_CLOSE_ERR,
  MEM_ALLOC_ERR,
  TABLE_BOUNDS,
  UNKNOWN_PARAMETER,
  NO_INPUT,
  PAR_FILE_SYNTAX_ERR,
  FILE_IN_ERR,
  UNKNOWN_MODE,
  INVALID_VALUE,
  INVALID_TABLE,
  NO_LOG_RMO_RETURNED,
  NEGATIVE_OPACITY
};

/*
 * The structure to hold various settings and modes
 */

typedef struct Modes
{
  int opal;
  int low_temp;
} Modes;

Modes modes;

/*
 * The structure to hold various geometry parameters
 */

typedef struct Geometry
{
  char geo_type[LINE_LEN];
  char density_filepath[LINE_LEN];
  char opacity_table_filepath[LINE_LEN];
  int icycle;
  size_t nz_cells;
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

#include "snake_functions.h"
