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
int check_for_parameter (char *par_name);
void clean_up (void);
void clean_up_gsl (void);
void clean_up_opac_tables (void);
void close_outfile (void);
void close_parameter_file (void);
// D
void density_from_file (char *filepath);
// E
void eddington_iterations (void);
void Exit (int error_code, char *fmt, ...);
// F
void find_par_file (char *file_path);
void find_vertical_tau (void);
int float_compare (double a, double b);
// G
void get_double (char *par_name, double *value);
void get_int (char *par_name, int *value);
void get_optional_int (char *par_name, int *value);
void get_string (char *par_name, char *value);
struct timespec get_time (void);
// I
int i2d (int row, int col);
void init_snake (void);
void init_geo (void);
void init_grid (void);
void init_opacity_table (void);
void init_outfile (void);
void init_parameter_file (char *par_filepath);
void input_double (char *par_name, double *value);
void input_int (char *par_name, int *value);
void input_string (char *par_name, char *value);
// O
void opac_2d (double logT, double logR, double *logRMO);
// L
void Log (char *fmt, ...);
void Log_error (char *fmt, ...);
void Log_verbose (char *fmt, ...);
// P
void print_duration (struct timespec start_time, char *message);
void print_time_date (void);
// R
double report_convergence (void);
// S
void standard_density_profile (void);
// U
void update_cell_opacities (void);
// W
void write_grid (void);
