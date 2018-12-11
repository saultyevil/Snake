/* ***************************************************************************
 *
 * @file opacities.h
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
 * The array for holding the 2D table - note that this is a flattened 2D array
 */

double *rmo_table_2d;
