/* ***************************************************************************
 *
 * @file functions.h
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Header file containing function definitions which required global
 *        scope.
 *
 * @details
 *
 * ************************************************************************** */

// C
int clean_up (void);
int clean_up_gsl (void);
int clean_up_opac_tables (void);
int close_outfile (void);
int close_parameter_file (void);
// D
int density_from_file (char *filepath);
// E
int eddington_iterations (void);
void Exit (int error_code, char *fmt, ...);
// F
int find_par_file (char *file_path);
int find_vertical_tau (void);
int float_compare (double a, double b);
// G
int get_double (char *par_name, double *value);
int get_int (char *par_name, int *value);
int get_optional_int (char *par_name, int *value);
int get_string (char *par_name, char *value);
struct timespec get_time (void);
// I
int i2d (int row, int col);
int init_snake (void);
int init_geo (void);
int init_grid (void);
int init_opacity_table (void);
int init_outfile (void);
int init_parameter_file (char *par_filepath);
int input_double (char *par_name, double *value);
int input_int (char *par_name, int *value);
int input_string (char *par_name, char *value);
// O
int opac_2d (double logT, double logR, double *logRMO);
// L
int Log (char *fmt, ...);
int Log_error (char *fmt, ...);
int Log_verbose (char *fmt, ...);
// P
int print_duration (struct timespec start_time, char *message);
int print_time_date (void);
// R
double report_convergence (void);
// S
int standard_density_profile (void);
// U
int update_cell_opacities (void);
// W
int write_grid (void);
