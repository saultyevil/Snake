/* ***************************************************************************
 *
 * @file 2d_interp.h
 *
 * @author E. J. Parkinson
 *
 * @date 11 Dec 2018
 *
 * @brief Various constants and variables for the 2D opacity table created by
 *        create_opacity_table.py.
 *
 * @details
 *
 * ************************************************************************** */



#define LINE_LEN 128

/*
 * Constants for the maximum and minimum values of logR and logT in the opacity
 * table
 */

#define MIN_LOG_R -7.0
#define MAX_LOG_R 1.0
#define MIN_LOG_T 3.2
#define MAX_LOG_T 8.5

/*
 * Constants relating to the dimensions for the 2D opacity table
 */

#define N_LOG_T 81
#define N_LOG_R 17
#define N_ROWS (N_LOG_T + 1)
#define N_COLS (N_LOG_R + 1)

/*
 * Global variables for interpolation and opacity
 */

char gsl_interp_choice[LINE_LEN];

// Pointers for tables
double *buffer;
double *logR_table, *logT_table, *logRMO_table;

/*
 * This function is defined here because so it isn't used implicitly
 */

int init_gsl_interp (void);

#include "snake_functions.h"
