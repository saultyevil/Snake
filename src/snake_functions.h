/* ***************************************************************************
 *
 * @file functions.h
 *
 * @author E. J. Parkinson
 *
 * @date 14 Nov 2018
 *
 * @brief Header file containing function definitions.
 *
 * @details
 *
 * ************************************************************************** */

/*
 * convergence.c
 */

int check_cell_convergence (void);
double report_convergence (void);

/*
 * default_pars.c
 */

int init_default_pars (void);

/*
 * density_init.c
 */

int density_from_file (char *filepath);
double density_profile_disk_height (double z);
int standard_density_profile (void);

/*
 * eddington.c
 */

int eddington_iterations (void);
int find_vertical_tau (void);
int update_cell_temperatures (void);
double eddington_approximation (double T_eff, double tau);
double update_Teff (void);

/*
 * geo_init.c
 */

int init_geo (void);

/*
 * grid_init.c
 */

int allocate_1d_grid (void);
int get_initial_grid_params (void);
int init_grid (void);

/*
 * opacities.c
 */

int i2d (int row, int col);
int init_opacity_table (void);
int read_2d_opact_table (char *file_path, double *opact_table);
int opac_2d (double logT, double logR, double *logRMO);
int update_cell_opacities (void);

/*
 * output.c
 */

int init_outfile (void);
int close_outfile (void);
int write_grid (void);

/*
 * read_pars.c
 */

int get_double (char *par_name, double *value);
int get_int (char *par_name, int *value);
int get_string (char *par_name, char *value);
int get_optional_int (char *par_name, int *value);
int input_double (char *par_name, double *value);
int input_int (char *par_name, int *value);
int input_string (char *par_name, char *value);
int find_par_file (char *file_path);
int init_parameter_file (char *par_filepath);
int close_parameter_file (void);

/*
 * time.c
 */

int print_time_date (void);
struct timespec get_time (void);
int print_duration (struct timespec start_time, char *message);

/*
 * utility.c
 */

int float_compare (double a, double b);
void Exit (int error_code, char *fmt, ...);
int Log (char *fmt, ...);
int Verbose_log (char *fmt, ...);
int Log_error (char *fmt, ...);
int clean_up (void);
