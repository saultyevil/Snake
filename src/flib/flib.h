/* ***************************************************************************
 *
 * @file flib.h
 *
 * @author E. J. Parkinson
 *
 * @data 12 Nov 2018
 *
 * @brief Header file for the Fortran files used to interpolate the Opal
 *        opacity tables.
 *
 * @details
 *
 * As the Opal Opacity Tables (https://opalopacity.llnl.gov/) are being used
 * to calculate the Rosseland mean opacity, and because I do not want to
 * re-write a routine to interpolate the table, we will instead use C and
 * Fortran77 together in a horrid interoperable series of wrapper functions and
 * structures.
 *
 * For this to work, we require the opacity tables, GN93hz, to be provided in
 * the current working directory the program is being executed from.
 *
 * ************************************************************************** */

#define OP_MIN_LOG_R -8.0
#define OP_MAX_LOG_R 1.0
#define OP_MIN_LOG_T 3.75
#define OP_MAX_LOG_T 8.7

/*
 * As the provided interpolation routines use common blocks to return values of
 * the Rosseland opacity, we first need to declare memory to store the common
 * block. This is done via a struct...
 */

struct
{

/*
 * OPACT       Is the Log of the Rosseland mean opacity: Log(kappa)
 * DOPACT      Is Dlog(kappa)/Dlog(T6)   at constant R
 * DOPACR      Is Dlog(kappa)/Dlog(R)    at constant T
 * DOPACTD     Is Dlog(kappa)/Dlog(T6)   at constant Rho
 */

  float opact;
  float dopact;
  float dopacr;
  float dopactd;
} e_;

/*
 * z: the metallicity fraction, Z
 * xh: the hydrogen mass fraction, X
 * t6: the temperature in millions of Kelvin
 * r = rho(g/c^3)/t6^3
 */

void opacgn93_ (float *z, float *xh, float *t6, float *r);

/*
 * izi: Keeps or recalculates table indices. A value of 0 causes the table
 *      indices to be recalculated. A value other than 0 cuases the previous
 *      indices to be used.
 * mzin: The integer value of i of the Z value to use. The choices are:
 *       1=0.0  2=0.0001 3=0.0003 4=0.001 5=0.002 6=0.004 7=0.01
 *       8=0.02 9=0.03  10=0.04  11=0.06 12=0.08 13=0.1
 */

void opac_ (int *izi, int *mzin, float *xh, float *t6, float *r);
